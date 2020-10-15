#include <stdio.h>
#include <string.h>

char username[256]; //at address location 0x804a040 cf main line <+81> => arg of first fgets

int verify_user_name(void){
	return (strncmp(username, "dat_wil", 7));
}

int verify_user_pass(char *password){
	return (strncmp(password, "admin", 5));
}

int main(void) {
	char password[64]; //at $esp + 0x1c => cf main line <+157> => arg of second fgets. buffer size is 64 bytes cf line below
	bzero(password, 64); // Line <+15> to <+29> : 0x10 * 4 = 0x40 bytes set to 0 = 64 bytes
	puts("********* ADMIN LOGIN PROMPT ********");
	printf("Enter Username: ");
	fgets(username, 0x100, stdin);// WATCH OUT : can go beyond buffer size and erase memory locations of previous ebp and eip!!
	if (verify_user_name() == 0){
		puts("Enter Password: ");
		fgets(password, 0x64, stdin);
		if (verify_user_pass(password) == 0)
			puts("nope, incorrect password...\n");
		else
			return 0;
	}
	return 1;
}
