//Program to control Programmable Alarm Clock Lamp
#include "chip.h"
#include <time.h>
#include <unistd.h>

#define ALARM_FILE "alarms.sg" //filetype?
#define LAMP "1013"   //XI0
#define SNOOZE "1015" //XI2
#define SILENT "1017" //XI4

int main(int argc, char** argv) {
  //Read command line args
  
  //Set up GPIO pins
  
  //Read in alarms
  
  //RUN PROGRAM
  
  //Look for soonest alarm
  //If silent switch is off do not run alarms
  
  //Sleep until alarm
  
  //ALARM
    //Wait for snooze to be pressed to turn off
  
  //Close pins
  
  return 0;
}
