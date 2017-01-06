#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
	FILE * f;
	f = fopen("/sys/class/gpio/export", "w");
	printf("export\n");
	fclose(f);
	return 0;
}



