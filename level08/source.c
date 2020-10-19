int log_wrapper(FILE *fd, char *step, char *argv_1) {
    // $rsp = $rbp-0x130 = $rbp-304 bytes
    // Arguments are stored from rbp-0x118 to rbp-0x128 => lines 11 to 25
    char buf[272]; // assumed buf size is 0x110 as there is no stored value below it in the rest of the function
    strcpy(buf, step); //buf is at rbp-0x110 cf Line 54. so after the 3 arguments. ste is 2nd argument, in rsi
    int len = strlen(buf); // +79 to +123. Il le refait lines +138 to +182 ???
    snprintf(&buf[len], 254 - len, argv[1]); // Line +193 : we add len to buf pointer // 0xfe (=254) - len comes from lines 127 to 135 in r8
    // WATCH OUT : argv[1] is passed to sprintf instead of format string!! + size <0 => very big as size_t
    buf[strcspn(buf, '\n')] = '\0'; // +230 / +235
    fprintf(fd, "LOG: %s\n", buf); //+273
    return 0;
}


int main(int argc, char **argv) {

    long n = 0x28; // Line<+24 / 33>
    char c = 0xff; // Line<+39>
    int b = 0xffffffff; // Line <+43> 
    if (argc != 2)  // Line <+11> => in edi we have the 1st arg = argc
        printf("Usage: %s filename\n", argv[0]); // 17 => 2nd arg in rsi, recovered and dereferenced Lines <+59 / 66>
    FILE *fd = fopen("./backups/.log", 'w'); //+106
    if ( fd == 0)  {// +118
        printf("ERROR: Failed to open %s\n", "./backups/.log"); //+146
        exit(1);
    }
    log_wrapper(fd, "Starting back up: ", argv[1]); // Note : fd is stored at $rbp-0x88
    //+161 & 168 => getting argv stored at $rbp-0xa0. +0x8 = moving to argv[1] because we are in a 64-bits memory layout
    //log_wrapper call Line <190>
    FILE *fd_arg1 = fopen(argv[1], "r"); // + 220
    if (fd_arg1 == 0) { //+229
        //+236
        printf("ERROR: Failed to open %s\n", argv[1]); //+266
        exit(1)
    }
    //+ 281
    // Je ne comprends pas bein ce qu'il fait entre les lignes 286 et 311.. il store "./backups/" dans [rbp-0x70] via eax, mais apres je ne vois pas trop lines +296 a +308
    char *backup = "./backups/"; // at rbp-0x70. Put in there line +293
    int len = strlen(backup); //+311 to +352
    strncat(backup, argv[1], 0x63 - len); //370 to 381 = argv[1] // 356 to 367 : 0x63 - len in $rdx. WATCH OUT: eax = 0x63 (only 4 bytes erased) and not rax... any issues with that?
    //<+402>
    int file = open(backup, 0xc1, 0x1b0); // These are open flags, including O_CREAT : the file is created if does not exist)
    if (file < 0){ // 432 & 436
        printf("ERROR: Failed to open %s%s\n", backup, argv[1]); //+470 // Lines 438 to 449 => retrieve argv[1] and set in rdx
        exit(1) // 480
    }
    int c; 
    while ((c = fgetc(fd_arg1)) != 0xff){ //+510 to 529 note : fd_arg1 is at rbp-0x80. c is casted to char (1 byte) and stored at rbp-0x71
        write(file, &c, 0x1); // 502
    }
    log_wrapper(fd, "Finished back up ", argv[1]); // 562 note : fd is sotred at rbp-0x88
    fclose(fd_arg1);//574
    close(file); // 584
    return(0);
}