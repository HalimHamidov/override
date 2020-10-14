
void test(arg1, arg2){
    unsigned int i;

    i = arg2 - arg1;
    if (i <= 21) {
        // i += 134515184;
        if (!(i >= 10 && i <= 15))
            decrypt(i);
        else 
            decrypt(rand());
    else 
        decrypt(rand());
    return ;
}


void decrypt(value) {
    int len;
    int i;
    char *passphrase = "Q}|u`sfg~sf{}|a3"; //16 chars
    len = strlen(passphrase); // Lines <+60> to <+94>
    i = 0; //Line<+94>
    while (i < len) {
		passphrase[i] = passphrase[i] ^ value; //Lines<+103> to <+139> with counter increment Line<+129>, xor Line<+117> and replacement Line<124/127>
        i+=1;
    }
    if (strcmp(passphrase, "Congratulations!") == 0){
		system("/bin/sh");
    }
	else {
		puts("\nInvalid Password");
	return
}

int main(void){
    int n;
    srand(time()); //Line<+33> and abve
    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    printf("%s\n", "Password:");
    scanf("%d", &n);
    test(n, 322424845); //0x1337d00d Line<+112>
    return 0;
}

