//Program to control Programmable Alarm Clock Lamp
#include "alarm_clock.h"
#include "chip.h"
#include <stdio.h>
#include <string.h>

//opens pins that are used
void pin_setup() {
  open_pin(LAMP); write_dir(LAMP, "out");
  open_pin(SNOOZE); write_dir(SNOOZE, "in");
  open_pin(SILENT); write_dir(SILENT, "in");
}

//closes pins that are used
void pin_cleanup() {
  write_val(LAMP, "0"); close_pin(LAMP);
  close_pin(SNOOZE);
  close_pin(SILENT);
}

//deallocate memory
void delete_alarms(chip_alarm ** all_alarms, int num_alarms) {
  int i;
  for (i=0; i < num_alarms; ++i) {
    free(all_alarms[i]);
    all_alarms[i] = NULL;
  }
}

//read alarms in from alarm file
int read_alarms(chip_alarm ** all_alarms, size_t * num_alarms) {
  //Read in alarms
  FILE * alarm_file_ptr = fopen(ALARM_FILE, "r");
  if (alarm_file_ptr == NULL) {
    printf("Could not open alarm file\n");
    //pin_cleanup();
    return 1;
  }

  *num_alarms=0;
  char buffer[50];
  //create and store all alarms
  while (fgets(buffer, sizeof(buffer), alarm_file_ptr) || *num_alarms > MAX_ALARMS) {
    all_alarms[*num_alarms] = parse_to_alarm(buffer);
    ++(*num_alarms);
  }
  fclose(alarm_file_ptr);

  //No Alarms
  if (num_alarms == 0) {
    printf("No alarms have been set");
    return -1;
  }
  return 0;
}

//find next alarm
chip_alarm* find_soonest_alarm(chip_alarm** all_alarms, int num_alarms, chip_alarm* next_alarm) {
  //Calculate current time
  time_t now; struct tm* current;
  time(&now); current = localtime(&now);
  //find soonest alarm
  size_t i; next_alarm = all_alarms[0];
  size_t min_dt = time_to_alarm(current, all_alarms[0]);
  for (i = 1; i < num_alarms; ++i) {
    size_t tmp_dt = time_to_alarm(current, all_alarms[i]);
    if (tmp_dt < min_dt) {
      next_alarm = all_alarms[i];
      min_dt = tmp_dt;
    }
  }
  return next_alarm;
}

int main(int argc, char** argv) {
  //Read command line args
  if (argc > 1) {
	  if (strcmp( argv[1], "-s" ) == 0) {
		  //Set alarms
      printf("set alarms...\n");
      return 0;
	  } else if (strcmp( argv[1], "-v") == 0) {
		  //Print out alarms
      chip_alarm* all_alarms[MAX_ALARMS];
      size_t num_alarms;
      if (read_alarms(all_alarms, &num_alarms) != 0) {
        printf("Alarms not valid");
        return 1;
      }
      int i;
      puts("All Alarms:");
      for (i=0; i < num_alarms; ++i) {
        print_alarm(all_alarms[i]);
      }
      return 0;
	  }
  }

  //Set up GPIO pins
  if (has_gpio_access()) {
    pin_setup();
  } else {
    printf("Cannot access GPIO pins\n");
    return 1;
  }

  //Read in alarms
  chip_alarm* all_alarms[MAX_ALARMS];
  size_t num_alarms;
  if (read_alarms(all_alarms, &num_alarms) != 0) {
    printf("Alarms not valid");
    return 0;
  }

  while (1) {
    //Check for silent mode
    //If silent switch is off do not run alarms
    if (is_silent_mode()) {
      silent_mode();
    }

    //Look for soonest alarm
    chip_alarm* next_alarm = NULL;
    next_alarm = find_soonest_alarm(all_alarms, num_alarms, next_alarm);

    //Set alarm
    print_alarm(next_alarm); //DEBUG
    time_t now; struct tm* current;
    time(&now); current = localtime(&now);
    long time_to_sleep = time_to_alarm(current, next_alarm);
    if (time_to_sleep < 0) { // just to be careful
      printf("ERROR -- alarm not valid");
      return -1;
    }
    printf("Sleeping until alarm (%ld)s\n", time_to_sleep); //DEBUG
    //sleep until alarm
    if (is_silent_mode()) {
      continue;
    }
    sleep(time_to_sleep);
    printf("RING\n"); //DEBUG
    ring_alarm(next_alarm); //Wait for snooze to be pressed to turn off
    break; //DEBUG
  }
  printf("exiting\n");
  delete_alarms(all_alarms, num_alarms);
  pin_cleanup();
  return 0;
}
