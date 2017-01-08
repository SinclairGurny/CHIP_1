//Program to control Programmable Alarm Clock Lamp
#include "chip.h"
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv) {
  //Read command line args
    //-s set alarms
    //-v view alarms
  
  //Set up GPIO pins
  //add checks -- fail if not sudo
  open_pin(LAMP); write_dir(LAMP, "out");
  open_pin(SNOOZE); write_dir(SNOOZE, "in");
  open_pin(SILENT); write_dir(SILENT, "in");
  
  //Read in alarms
  FILE * alarm_file_ptr = fopen(ALARM_FILE, "r");
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
