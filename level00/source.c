#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int n;
	puts("***********************************"); //35 times *
	puts("* \t     -Level00 -\t\t  *");
	puts("***********************************"); //35 times *
	printf("Password:");
	scanf("%d", &n);
	if (n == 0x149c){ //5276 in decimal
		puts("\nAuthenticated!");
		system("/bin/sh");
	}
	else
		puts("\nInvalid Password!");
	return 0;
}
