#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
	FILE * open, * pin;
	pin = fopen("/sys/class/gpio/gpio1016/direction", "w");
	fputs("high", pin);
	fclose(pin);
	return 0;
}





                                                                                                                                                                        