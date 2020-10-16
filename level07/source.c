


unsigned int get_unum() {
    unsigned int num = 0;
    fflush(stdout);
    scanf("%u", num);
    clear_stdin();
    return num;
}

unsigned int store_number(int *tab) {
    unsigned int number;
    unsigned int index;

    printf(" Number: ");
    number = get_unum();
    printf( " Index: ");
    index = get_unum();

    if (index % 3 != 0 || (number >> 0x18) == 0xb7) { //3070230528 <= number < 3087007744
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return 1;
    }

    tab[index] = number;
    return 0;
}

int read(int *tab){
    printf(" Index: ");
    unsigned int n = get_unum();
    printf(" Number at data[%u] is %u\n", n, tab[n]); //Line 37 multiplies index by 4 because we have a table of int // Line 43 retrieves value at pointer in tab
}

int main(int argc, char **argv, char **envp) {
    // $esp = $ebp - 0x8 => alignment Line<+6>
    // $esp = $esp - 0x1d0 = $ebp - 0x1d8 = $ebp - 472 bytes => Line<+9>
    int tab[25];
    int n = 0; //Line +44 at address $esp+0x1b4
    char buf[20]; //cf line <+341> : buffer starts at $esp+0x1b8 
    bzero(buf, 30); //20 bytes are set to 0 at lines <55 to 99> (5 * 4bytes)
    bzero(tab, 100);
    char **arg = argv; //Lines 15 - 18
    char **environ = envp; //Lines 22 & 25
    while (*arg){ //Line <+199> to <205>
        int len = strlen(arg[i]); // 132 to 165
        memset(*arg, 0x0, len); // Lines <168> to <189>
        arg++; // The pointer is incremented by four Line <+194>. If not null, goes back into the loop
    }
    while (*environ){ // Line 278 => environ stored in $esp+0x18
        len = strlen(*environ); // Lines <211> to <244>
        memset(*environ, 0x0, len);
        environ++; //Line 273
    }
    //Line 288
    puts("----------------------------------------------------
        Welcome to wil's crappy number storage service!
        ----------------------------------------------------
        Commands:
            store - store a number into the data storage
            read  - read a number from the data storage
            quit  - exit the program
        ----------------------------------------------------
        wil has reserved some storage :>
        ----------------------------------------------------");

    printf("Input command: ");

    //Line 313
    while (1){
        fgets(buf, 20, stdin); //Line 351
        len = strlen(buf);
        buf[len-1] = '\0'; //Line 396
        if (strncmp(buf, "store", 0x5) == 0)
            n = store_number(tab); // Line 448
        else if (strncmp(buf, "read", 0x4) == 0)
            n = read_number(tab); // Line 513
        else if (strncmp(buf, "quit", 0x4) == 0)
            break;
        if (n == 0){ //Line 578
            printf("Completed %s command successfully\n", buf);
        }
        bzero(buf, 30); //Lines <+645> to <+672>
    }
    return 0;
}
