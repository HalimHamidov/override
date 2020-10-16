1. Basically the program asks for a login, a password, and makes a complex calculation based on each character of the login, to get a number which should equals the password. This auth function then would return 0, and the main would call a shell with level07 permissions

2. The problem is that there is a `ptrace` call which checks whehter the program `./level06` is launched by another process. Hence, when we launch the program through gdb, it returns -1 and return with 1.

3. The xploit consists in setting a breakpoint right after the call to `ptrace()`, at `0x080487ba`, and change the value of `$eax` to 0 with the command : `set $eax = 0`. Then, we can break at the last if, line 289 (`0x08048869`) and check the integer stored as the result of all the calculations : `x/d $ebp-0x10`.

4. This gives us the password corresponding to our login.


```
level06@OverRide:~$ ./level06
***********************************
*		level06		  *
***********************************
-> Enter Login: aaaaaa
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6231562
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```