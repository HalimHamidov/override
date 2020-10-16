// $esp = $ebp - 0x8 => alignment Line<+3>
// $esp = $esp - 0x50 = $ebp - 0x58 = $ebp - 88 bytes => Line<+8>

int main () {
    char login[32];
    unsigned int password; //at $ep + 0x28 cf Line<169> as argument to scanf
    put("***********************************");
    put("*		level06		  *             ");
    put("***********************************");
    printf("-> Enter Login: ");
    fgets(login, 32, stdin); //Line <110>
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    printf("-> Enter Serial: "); //Line<159>
    scanf("%u", password);
    if (auth(login, password) == 0){ //Line <+200>
        puts("Authenticated!");
        system("/bin/sh");
        return 0;
    }
    return 1;
}

int auth(char *login, unsigned int password) {
    int npos = strcspn(login, '\n');
    login[npos] = '\0';
    len = strnlen(login, 32);

    if (len <= 5) {
        return 1;
    } 
    if (ptrace(0, 0, 1, 0) == -1) //+109 +114
    {
        puts("\033[32m.", '---------------------------', ".");
        puts("\033[31m| !! TAMPERING DETECTED !!  |");
        puts("\033[32m'", '---------------------------', "'");
        return 1; // +160
    }
    //<+165>
    unsigned int i = (int)login[3] ^ 0x1337; // +177
    i += 0x5eeded; // +182
    int count = 0;

    while(count < len) {
        if (login[count] > 0x1f) { //printable chars
            int res = (int)login[count] ^ i; // +233
            int mul = res * 0x88233b2b; // +236
            int calc = res - mul; // +247
            calc = (calc / 2) + mul; // calc >> 1  +249 +251
            calc = calc >> 0xa; //  +253
            calc *= 0x539; // +256
            calc = res - calc; // +264
            i += calc; // +268
        }
        else 
            return 1;
        count++;
    }              
    if (password == i) {
        return 0;
    }
    else
        return 1;
}