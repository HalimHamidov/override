
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

int main(void){
	char username[96]; //0xc * 8o = size. located at $rbp - 0x70 cf lines <+24> to <+41>
	char password_file[40]; //0x5 * 8o = size. located at $rbp - 0xa0 cf lines <+53> to <+73>
	char password[96]; //0xc * 8o = size. located at  $rbp-0x110 cf lines <+85> to <+105>
	bzero(username, 96);
	bzero(password_file, 40);
	bzero(password, 96);
	FILE *fd = fopen("/home/users/level03/.pass", "r");
	if (!fd){
		fwrite("ERROR: failed to open password file\n", 0x1, 0x24, stderr);
		exit(1); // Line <+205>
	}
	int res = fread(password_file, 0x1, 41, fd); //STRANGE : read 42 but buffer is of size 40 / cf Line<+237>
	int n = strcspn(password_file, "\n"); // cf line <+260>
	password_file[n] = '\0';
	if (res != 41){
		fwrite("ERROR: failed to read password file\n", 0x1, 0x24, stderr);
		fwrite("ERROR: failed to read password file\n", 0x1, 0x24, stderr);
		exit(1);
	}
	//Line <+361>
	fclose(fd);
	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\\**************************************/");
	//Line <+413>
	printf("--[ Username: ");
	fgets(username, 100, stdin); // Line <+453>
	n = strcsnpn(username, "\n");
	username[n] = '\0';
	printf("--[ Password: ");
	fgets(password, 100, stdin); //Line <+523>
	n = strcsnpn(password, "\n");
	password[n] = '\0';
	puts("\\**************************************/");
	if(strncmp(password_file, password, 41) != 0){ //Line<+591>
		printf(username); //Line<+620>
		puts(" does not have access!");
		exit(1);
	}
	printf("Greetings, %s!\n", username);
	system("/bin/sh");
	return 0;
}
