//tests the loop until button press functions
#include "../chip.h"
#include <string.h>
#include <unistd.h>

int main(int argc, char ** argv) {
  if (argc < 2) printf("Usage: pin to use");
  open_pin(argv[1]);
  write_dir(argv[1], "in");
  char val[10], new_val[10]; read_value(argv[1], val);
  printf("Initial value: %s\n", val);
  while (1) {
    printf("Looping\n");
    read_value(argv[1], new_val);
    printf("Current value: %s\n", new_val);
    if (strcmp( new_val, val ) != 0) {
      printf("Breaking\n");
      break;
    }
    usleep(500);
  }
  return 0;
}
