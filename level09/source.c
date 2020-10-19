void secret_backdoor(void){
    char buf[128];
    fgets(buf, 128, stdin);
    system(buf);
    return;
}

void set_username(char *buf){
    // $rsp = $rbp-0xa0 = $rbp-160 bytes => Line <+4>
    // pointer to buf is stored at address $rbp-0x98 => Line<+11>
    char username[128]; // Line +18 => set to zero below. gives the size
    bzero(username, 128); //+18 to +44. 0x10 * 8 bytes = 0x80 = 128 bytes
    puts(">: Enter your username");
    printf(">>: ");
    fgets(username, 128, stdin); //+107
    int counter = 0; //+112
    While (counter <= 40 && username[counter] != '\0'){ // 0x28 line 157 // comparison to 0x00 lines 134 to 161 
        // +121
        buf[140 + counter] = username[counter]; //Line 146 : buf at $rdx (stored line 134), counter in rax...? 0x8c = 140
        counter++; //+153
    }
    // Basically : the username is stored in the 40 last characters of buf...?
    //!!!! NO '\0' at the end of buffer if username is > 40 chars long
    printf(">: Welcome, %s", buf + 140); // .. and is printed out => Line<+212>
    return;
}

void set_msg(char *buf){
    // $rsp = $rbp-0x410 = $rbp-1040 bytes => Line <+4>
    // pointer to buf is stored at address $rbp-0x408 => Line<+11>
    char msg[1024]; // Line +18 and set to zero over 0x80 * 8 = 0x400 = 1024 bytes
    bzero(msg, 1024); //+25 to +44. 0x80 * 8 bytes = 0x400 = 1024 bytes
    puts(">: Msg @Unix-Dude"); // +54
    printf(">>: "); // +74
    fgets(msg, 1024, stdin); // +107
    strncpy(buf, msg, 140); //Note length is stored 180 bytes from beginning of buffer : cf handle_msg line +63
    return;
}

void handle_msg(void){
    char buf[180]; // $rbp-0xc0 line 11 +0xc line 63
    set_username(buf); //+80
    set_msg(buf); // +95
    puts(">: Msg sent!");
    return;
}

int main() {
    puts("--------------------------------------------\n
        |   ~Welcome to l33t-m$n ~    v1337        |\n
        --------------------------------------------\n");

    handle_msg();
    return 0;
}