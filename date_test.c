#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	time_t now;
	time(&now);
	printf("Current Timer: %s\n", ctime(&now));
	return 0;
}
