#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
	FILE * open, * pin;
	pin = fopen("/sys/class/gpio1016/direction", "w");
	fputs("low", pin);
	fclose(pin);
	return 0;
}




                                                                                                                                                                        