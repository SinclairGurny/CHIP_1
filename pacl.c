//Program to control Programmable Alarm Clock Lamp
#include "alarm_clock.h"
#include "chip.h"
#include <stdio.h>
#include <string.h>

void pin_setup() {
  open_pin(LAMP); write_dir(LAMP, "out");
  open_pin(SNOOZE); write_dir(SNOOZE, "in");
  open_pin(SILENT); write_dir(SILENT, "in");
}

void pin_cleanup() {
  write_val(LAMP, "0"); close_pin(LAMP);
  close_pin(SNOOZE);
  close_pin(SILENT);
}

int main(int argc, char** argv) {
  //Read command line args
  if (argc > 2) {
	  if (strcmp( argv[1], "-s" ) == 0) {
		  //Set alarms
      printf("set alarms...\n");
      return 0;
	  } else if (strcmp( argv[1], "-v") == 0) {
		  //Print out alarms
      printf("all alarms...\n");
      return 0;
	  }
  }

  //Set up GPIO pins
  if (has_gpio_access()) {
    //pin_setup();
  } else {
    printf("Cannot access GPIO pins\n");
    //return 1;
  }

  //Read in alarms
  FILE * alarm_file_ptr = fopen(ALARM_FILE, "r");
  if (alarm_file_ptr == NULL) {
    printf("Could not open alarm file\n");
    //pin_cleanup();
    return 1;
  }

  size_t num_alarms=0;
  chip_alarm* all_alarms[MAX_ALARMS];
  char buffer[50];
  //create and store all alarms
  while (fgets(buffer, sizeof(buffer), alarm_file_ptr) || num_alarms > MAX_ALARMS) {
    all_alarms[num_alarms] = parse_to_alarm(buffer);
    ++num_alarms;
  }
  fclose(alarm_file_ptr);

  //No Alarms
  if (num_alarms == 0) {
    printf("No alarms have been set");
    return 0;
  }

  //Check for silent mode
  //TODO

  //Look for soonest alarm
  time_t now; struct tm* current;
  time(&now); current = localtime(&now);
  size_t i; chip_alarm * next_alarm = all_alarms[0];
  size_t min_dt = time_to_alarm(current, all_alarms[0]);
  for (i = 1; i < num_alarms; ++i) {
    size_t tmp_dt = time_to_alarm(current, all_alarms[i]);
    if (tmp_dt < min_dt) {
      next_alarm = all_alarms[i];
      min_dt = tmp_dt;
    }
  }

  print_alarm(next_alarm);
  time(&now); current = localtime(&now);
  long time_to_sleep = time_to_alarm(current, next_alarm);
  if (time_to_sleep < 0) {
    printf("ERROR -- alarm not valid");
    return -1;
  }
  printf("Sleeping until alarm\n");
  sleep(time_to_sleep);
  ring_alarm(next_alarm);

  //do stuff
	//make alarm structs from all alarms

  //RUN PROGRAM
  //If silent switch is off do not run alarms

	//Look for soonest alarm

	//Sleep until alarm

	//ALARM
	  //Wait for snooze to be pressed to turn off

	//Close pins

  return 0;
}
