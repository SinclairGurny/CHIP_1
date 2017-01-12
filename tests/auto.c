#include <stdio.h>
#include <time.h>

int main() {
  FILE * f = fopen("/home/chip/Desktop/foo.txt", "w");
  if (f == NULL) { printf("Unsuccessful 1\n"); return 1; }
  time_t now; struct tm* current;
  time(&now); current = localtime(&now);
  fprintf(f, "Hello World :: %02d:%02d:%02d\n", current->tm_hour, current->tm_min, current->tm_sec);
  fclose(f);
  printf("Successful 1\n");
  return 0;
}
