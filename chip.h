#ifndef __CHIP_H__
#define __CHIP_H__
#include <stdio.h>
#include <stdlib.h>

#define PIN0 "1013"

//Opens new GPIO pin
//Does not check if GPIO pin exists
int open_pin(char* pin) {
  FILE * f = fopen("/sys/class/gpio/export", "w");
  if (f == NULL) return -1;
  fputs(pin, f); //exports pin
  fclose(f);
  return 0;
}

//Closes GPIO pin
int close_pin(char* pin) {
  FILE * f = fopen("/sys/class/gpio/unexport", "w");
  if (f == NULL) return -1;
  fputs(pin, f); //exports pin
  fclose(f);
  return 0;
}

//Writes direction of pin (IN/OUT)
int write_dir(char* pin, char* d) {
  char tmp[50];
  sprintf(tmp, "/sys/class/gpio/gpio%s/direction", pin);
  FILE * f = fopen(tmp, "w");
  if (f == NULL) return -1;
  fputs(d, f); //set direction of pin
  fclose(f);
  return 0;
}

//Writes value of pin -- (pin = out, 1/0)
int write_val(char* pin, char* v) {
  char tmp[50];
  sprintf(tmp, "/sys/class/gpio/gpio%s/value", pin);
  FILE * f = fopen(tmp, "w");
  if (f == NULL) return -1;
  fputs(v, f); //set value of pin
  fclose(f);
  return 0;
}

//Read value from pin -- (1/0)
int read_value(char* pin, char* ret_val) {
  char tmp[50];
  sprintf(tmp, "/sys/class/gpio/gpio%s/value", pin);
  FILE * f = fopen(tmp, "r");
  if (f == NULL){
    ret_val = "error";
    return -1;
  }
  fscanf(f, "%s", ret_val); //read value of pin
  fclose(f);
  return 0;
}

//Check if program is run on CHIP w/ sudo access
int has_gpio_access() {
  if (open_pin(PIN0) == 0) {
    close_pin(PIN0);
    return 1;
  } else {
    return 0;
  }
}

#endif
