//Sets alarm at time given
//Cannot set on next day
#include "chip.h"
#include <time.h>
#include <unistd.h>

int main (int argc, char** argv) {
  if (argc < 2) {
    printf("Usage Alarm: HH:MM");
    return 0;
  }
  //Read in time value
  size_t h=0, m=0;
  char *ptr = strtok(argv[1], ":");
  h = strtoul(ptr, NULL, 10);
  ptr = strtok(NULL, ":");
  m = strtoul(ptr, NULL, 10);
  //Calculate alarm time
  if (h==0 && m==0) { return 0;}
  time_t now; struct tm* current;
  time(&now); current = localtime(&now);
  size_t dh = h - current->tm_hour;
  size_t dm = m - current->tm_min - 1;
  size_t ds = 60 - current->tm_sec;
  if (dh < 0 || dm < 0 || ds < 0 || (dh==0 && dm==0 && ds==0)) { return 0; } // bad time (next day), current time
  size_t alarm_time = 60*60*dh + 60*dm + ds;
  //Setup Output pin
  if (open_pin("1013") == -1) { return -1; };
  write_dir("1013", "out");
  write_val("1013", "1"); //Use pin as source -- off
  //Set Alarm
  sleep(alarm_time);
  //Set off Alarm
  write_val("1013", "0");//Turn ON
  sleep(5);           //Keep on for 5s
  close_pin("1013");
  return 0;
}
