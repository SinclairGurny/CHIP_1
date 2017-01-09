#ifndef __ALRM_CLK_H__
#define __ALRM_CLK_H__
#include "chip.h"
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ALARM_FILE "alarms.alrm"
#define LAMP "1014"   //XIO-P1
#define SNOOZE "1016" //XIO-P3
#define SILENT "1018" //XIO-P5

#define MAX_ALARMS 50
#define SEC_PER_DAY 86400

#define ALRM_DELIMS ":[]()"

//alarm struct
struct alarm_type {
  size_t hour;
  size_t minute;
  size_t light_pattern; //delay in milliseconds
  size_t week[7]; //starts on Sunday
};
typedef struct alarm_type chip_alarm;

//alarm functions
long _next_alarm_day(struct tm* curr, chip_alarm* alrm, int can_today);

long time_to_alarm(struct tm* curr, chip_alarm* alrm) {
  long dh = alrm->hour - curr->tm_hour;
  long dm = alrm->minute - curr->tm_min - 1;
  long ds = 60 - curr->tm_sec;
  long d_tod = 3600 * dh + 60 * dm + ds;
  long days = _next_alarm_day(curr, alrm, d_tod > 0);
  long tta = SEC_PER_DAY * days + d_tod;
  printf("Time to alarm: %ld\n", tta);
  return tta;
}

long _next_alarm_day(struct tm* curr, chip_alarm* alrm, int can_today) {
  int curr_day = curr->tm_wday;
  int i;
  if (can_today == 1) i=0;
  else i=1;
  for (i; i < 7; ++i) {
    if (alrm->week[(curr_day + i)%7] == 1) {
      return (long)i;
    }
  }
  return 8; // should not reach
}

void ring_alarm(chip_alarm* alrm) {
  printf("RINGING...");
  //TODO patterns, snooze, ...
  //read current value of snooze button
  while (true) {
    //on
    //delay
  }
}

/*
void blink(size_t pause);
void steady();
void turn_off();
void is_snoozed();
//parsing and other functions
*/

//Parsing functions
void _parse_week(chip_alarm* alrm, char* str);
void _parse_light_pattern(chip_alarm* alrm, char* str);
void print_alarm(chip_alarm* alrm);

chip_alarm* parse_to_alarm(char* str_alarm) {
  chip_alarm * alrm = (chip_alarm*)malloc(sizeof(chip_alarm));
  char* ptr = strtok(str_alarm, ":[]()"); alrm->hour = strtoul(ptr, NULL, 0);
  ptr = strtok(NULL, ALRM_DELIMS); alrm->minute = strtoul(ptr, NULL, 0);
  ptr = strtok(NULL, ALRM_DELIMS); _parse_week(alrm, ptr);
  ptr = strtok(NULL, ALRM_DELIMS); _parse_light_pattern(alrm, ptr);
  return alrm;
}

void _parse_week(chip_alarm* alrm, char* str) {
  memset(alrm->week, 0, 7);
  if (strchr(str, 'M') != NULL || strchr(str, 'A') != NULL || strchr(str, 'D') != NULL) {
    alrm->week[1] = 1;
  } if (strchr(str, 'T') != NULL || strchr(str, 'A') != NULL || strchr(str, 'D') != NULL) {
    alrm->week[2] = 1;
  } if (strchr(str, 'W') != NULL || strchr(str, 'A') != NULL || strchr(str, 'D') != NULL) {
    alrm->week[3] = 1;
  } if (strchr(str, 'R') != NULL || strchr(str, 'A') != NULL || strchr(str, 'D') != NULL) {
    alrm->week[4] = 1;
  } if (strchr(str, 'F') != NULL || strchr(str, 'A') != NULL || strchr(str, 'D') != NULL) {
    alrm->week[5] = 1;
  } if (strchr(str, 'S') != NULL || strchr(str, 'A') != NULL || strchr(str, 'E') != NULL) {
    alrm->week[6] = 1;
  } if (strchr(str, 'U') != NULL || strchr(str, 'A') != NULL || strchr(str, 'E') != NULL) {
    alrm->week[0] = 1;
  }
}

void _parse_light_pattern(chip_alarm* alrm, char* str) {
  if (strchr(str, 'C') != NULL) {
    alrm->light_pattern = 0;
  } if (strchr(str, 'B') != NULL) {
    alrm->light_pattern = strtoul(str+1, NULL, 0);
  }
}

//Printing functions
void print_alarm(chip_alarm* alrm) {
  size_t j=0; size_t num_days=0;
  for (j=0; j < 7; ++j) {
    if (alrm->week[j]) ++num_days;
  }
  printf("Alarm: %02zu:%02zu on %zu day(s) of the week with a %zums delay pattern\n",
          alrm->hour, alrm->minute, num_days, alrm->light_pattern);
}

#endif
