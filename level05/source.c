// $esp = $ebp - 0x8 => alignment Line<+5>
// $esp = $esp - 0x90 = $ebp - 0x98 = $ebp - 152 bytes => Line<+8>

int main(void){

    char buf[100] // at $esp + 0x28 = $esp + 40 bytes = $ebp - 112 bytes. <+67>
    int i = 0; // Line <+14> at $esp + 0x8c
    fgets(buf, 100, stdin);
    //loop
    i = 0; // Line<+54>
    while (i < strlen(buf)){
        if (buf[i] > 0x40 && buf[i] <= 0x5a) //Line<78 - 81> = if char is digit or punctuation / and capital letter
            buf[i] = buf[i] ^ 0x20; //+103 to +133
        i++;
    }
    printf(buf);
    exit(0);
}