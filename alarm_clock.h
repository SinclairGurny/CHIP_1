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
  size_t on_time;   //how long to blink on in milliseconds
  size_t off_time; //how long to stay off
  size_t week[7]; //starts on Sunday
};
typedef struct alarm_type chip_alarm;
//===================================================================
//alarm functions
long _next_alarm_day(struct tm* curr, chip_alarm* alrm, int can_today);

//calculates time to next alarm in milliseconds
long time_to_alarm(struct tm* curr, chip_alarm* alrm) {
  long dh = alrm->hour - curr->tm_hour;
  long dm = alrm->minute - curr->tm_min - 1;
  long ds = 60 - curr->tm_sec;
  long d_tod = 3600 * dh + 60 * dm + ds;
  long days = _next_alarm_day(curr, alrm, d_tod > 0);
  long tta = SEC_PER_DAY * days + d_tod;
  return tta;
}

//calculates number of days until the alarm can go off
long _next_alarm_day(struct tm* curr, chip_alarm* alrm, int can_today) {
  int curr_day = curr->tm_wday;
  int i;
  if (can_today == 1) i=0;
  else i=1;
  for (; i < 7; ++i) {
    if (alrm->week[(curr_day + i)%7] == 1) {
      return (long)i;
    }
  }
  return 8; // should not reach
}

int _snooze_hit(char* prev_val) {
  char curr_val[10];
  read_value(SNOOZE, curr_val);
  if (strcmp( curr_val, prev_val ) == 0) {
    return 0;
  } else {
    return 1;
  }
}

//sets off alarm
void ring_alarm(chip_alarm* alrm) {
  printf("RINGING...\n");
  //read current value of snooze button
  char prev_val[10]; read_value(SNOOZE, prev_val);
  while (_snooze_hit(prev_val) == 0) {
    printf("BZZZZZ\n");
    write_val(LAMP, "1");
    usleep(alrm->on_time*1000);
   // if (_snooze_hit(prev_val) == 1) break;
    if (alrm->off_time > 0) {
      write_val(LAMP, "0");
      usleep(alrm->off_time*1000);
    }
  }
  printf("Alarm off\n");
  write_val(LAMP, "0");
}

//============================================================
//Silent mode

//check if silent mode is on/off
int is_silent_mode() {
  char val[10];
  read_value(SILENT, val);
  if (strcmp( val, "1") == 0) {
    return 1;
  } else {
    return 0;
  }
}

//run silent mode
void silent_mode() {
  printf("Entering Silent Mode\n");
  char val[10], new_val[10];
  while (1) {
    read_value(SILENT, new_val);
    if (strcmp(val, new_val) == 0) {
      usleep(500);
    } else break;
  }
  printf("Exiting Silent Mode\n");
}

//============================================================
//Parsing functions
void _parse_week(chip_alarm* alrm, char* str);
void _parse_light_pattern(chip_alarm* alrm, char* str);
void print_alarm(chip_alarm* alrm);

//parse string from .alrm file into a struct
chip_alarm* parse_to_alarm(char* str_alarm) {
  chip_alarm * alrm = (chip_alarm*)malloc(sizeof(chip_alarm));
  char* ptr = strtok(str_alarm, ":[]()"); alrm->hour = strtoul(ptr, NULL, 0);
  ptr = strtok(NULL, ALRM_DELIMS); alrm->minute = strtoul(ptr, NULL, 0);
  ptr = strtok(NULL, ALRM_DELIMS); _parse_week(alrm, ptr);
  ptr = strtok(NULL, ALRM_DELIMS); _parse_light_pattern(alrm, ptr);
  return alrm;
}

//parse weekdays into struct
void _parse_week(chip_alarm* alrm, char* str) {
  int i;
  for (i=0; i<7; ++i) alrm->week[i]=0;
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

//parse light pattern into struct
void _parse_light_pattern(chip_alarm* alrm, char* str) {
  if (strchr(str, 'C') != NULL) {
    alrm->on_time = 500;
    alrm->off_time = 0;
  } if (strchr(str, 'B') != NULL) {
    char * ptr = strtok(str, "B|");
    alrm->on_time = strtoul(ptr, NULL, 0);
    alrm->off_time = strtoul(ptr, NULL, 0);
  }
}

//======================================================================
//prints some details of an alarm
void print_alarm(chip_alarm* alrm) {
  size_t j=0; size_t num_days=0;
  for (j=0; j < 7; ++j) {
    if (alrm->week[j]) ++num_days;
  }
  printf("Alarm: %02zu:%02zu on %zu day(s) of the week with a %zu/%zums delay pattern\n",
          alrm->hour, alrm->minute, num_days, alrm->on_time, alrm->off_time);
}

#endif
