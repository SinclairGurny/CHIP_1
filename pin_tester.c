#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
	FILE *f;
	char s[256], tmp[256];
	//Loop
	while (1) {
		printf("Command: ");
		scanf("%s", s);
		if (strcmp(s, "quit") == 0) {
			break;
		} else if (strcmp(s, "open") == 0) {
			//open port
			f = fopen("/sys/class/gpio/export", "w");
			if (f != NULL) {
				scanf("%s", s); //read in port number
				fputs(s, f); //exports pin
				fclose(f);
				printf("success\n");
			}
		} else if (strcmp(s, "close") == 0) {
			//close port
			f = fopen("/sys/class/gpio/unexport", "w");
			if (f != NULL) {
				scanf("%s", s); //read in port number
				fputs(s, f); //unexports pin
				fclose(f);
				printf("success\n");
			}
		} else if (strcmp(s, "dir") == 0) {
			//set direction of port
			scanf("%s", s); //get port
			sprintf(tmp, "/sys/class/gpio/gpio%s/direction", s);
			f = fopen(tmp, "w");
			if (f != NULL) {
				scanf("%s", s);
				fputs(s, f); //set direction of pin
				fclose(f);
				printf("success\n");
			}
		} else if (strcmp(s, "val") == 0) {
			//set value of port
			scanf("%s", s);
			sprintf(tmp, "/sys/class/gpio/gpio%s/value", s);
			f = fopen(tmp, "w");
			if (f != NULL) {
				scanf("%s", s);
				fputs(s, f); //set value of pin
				fclose(f);
				printf("success\n");
			}
		}
	}
	printf("exiting\n");
	return 0;
}

