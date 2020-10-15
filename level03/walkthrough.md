1. the program requires a password

2. `i functions` => there is a main, a test and a decrypt functions

3. `disas main` => it saves the user input as an int, and calls the `test()` function with the user input as argument 1 and a number as 2nd argument (0x1337d00d)

4. The test function is trickier :
    - First it subtracts first to 2nd argument, so `user input - 0x1337d00d`. We will call this value `difference` => Line<+14>
    - Then it compares `difference` with `0x15 = 21` => Line<+21>
    - If above, it jumps to Line<+259> and calls decrypt on a random number
    - Else, it multiplies `difference` by 4 => through the left shift 2 (`shl eax,0x2`) operand on Line<+34> 
    - This will give the index in a sort of "array of pointers" to jump to, starting at address `0x80489f0` (cf Line<+37>)
    - Here is the table:
        ```
        (gdb) x/30wx 0x80489f0
        0x80489f0:	0x0804884a	0x08048775	0x08048785	0x08048795
        0x8048a00:	0x080487a5	0x080487b5	0x080487c5	0x080487d5
        0x8048a10:	0x080487e2	0x080487ef	0x0804884a	0x0804884a
        0x8048a20:	0x0804884a	0x0804884a	0x0804884a	0x0804884a
        0x8048a30:	0x080487fc	0x08048809	0x08048816	0x08048823
        0x8048a40:	0x08048830	0x0804883d	0x2a2a2a2a	0x2a2a2a2a
        0x8048a50:	0x2a2a2a2a	0x2a2a2a2a	0x2a2a2a2a	0x2a2a2a2a
        0x8048a60:	0x2a2a2a2a	0x2a2a2a2a
        ```

    - at `0x80489f0 + 0x0` we have `0x0804884a`, then at `0x80489f0 + 0x4` we have `0x08048775`...
    - `0x0804884a` is the address where the `decrypt` function is called on a random integer. We can see that it is at position 0 and between 10 and 15 included (for the value of `difference`; before multiplying by 4!)
    - Otherwise, up to 21 included, these are addresses to the same instrucitons : `decrypt(difference)`


5. The decrypt function:
    - It takes a passphrase stored at `ebp-0x1d`:
    ```
    0x08048673 <+19>:	mov    DWORD PTR [ebp-0x1d],0x757c7d51
    0x0804867a <+26>:	mov    DWORD PTR [ebp-0x19],0x67667360
    0x08048681 <+33>:	mov    DWORD PTR [ebp-0x15],0x7b66737e
    0x08048688 <+40>:	mov    DWORD PTR [ebp-0x11],0x33617c7d
    0x0804868f <+47>:	mov    BYTE PTR [ebp-0xd],0x0
    ```

    - Then, for each byte of this passphrase, it `xor` it with the value given as argument (so our `difference`!) and replaces it with the result
    - If the result is the string "Congratulations!", then it enters `system("/bin/sh")` and we have our shell!
    - xoring the string with the value 18 gives us congratulations!

    ````
    level03@OverRide:~$ ./level03
    ***********************************
    *		level03		**
    ***********************************
    Password:322424827
    $ whoami
    level04
    $ cat /home/users/level04/.pass
    kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
    ```