#ifndef __CHIP_H__
#define __CHIP_H__
#include "chip.h"

#define ALARM_FILE "alarms.alrm"
#define LAMP "1014"   //XI0
#define SNOOZE "1016" //XI2
#define SILENT "1018" //XI4

//alarm struct

struct alarm_t {
  size_t hour;
  size_t minute;
  size_t light_pattern;
  size_t week[7];
}
typedef struct alarm_t alarm;

//alarm functions

void ring_alarm(alarm a) {
  //check type of ring pattern
}

void blink(size_t pause) {
  
}

void steady() {

}

void turn_off() {

}

void is_snoozed() {

}



//parsing and other functions

alarm _parse_to_alarm(char* str) {
  return NULL;
}

#endif
