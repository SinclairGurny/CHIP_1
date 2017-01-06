#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
	if (argc < 2) { return 1; }
	char* value = argv[1];
	FILE * pin;
	pin = fopen("sys/class/gpio1016", "w");
	fputs(value, pin);
	fclose(pin);
	return 0;
}

