#include "chip.h"
#include <string.h>
#include <unistd.h>

//finds substr after last occurrence of a character in k
char* substring(char *str, char *k) {
  //make return string
  char * ret = (char*) malloc(6*sizeof(char));
  memset(ret, '\0', 6);
  //create iterators
  char * ret_itr = ret;
  char *itr_s, *itr_c;
  //boolean
  int matched = 0;

  //search through str
  for (itr_s = str; *itr_s != '\0'; ++itr_s) {
    *ret_itr = *itr_s;
    //check each character in k
    for (itr_c = k; *itr_c != '\0'; ++itr_c) {
      if (*itr_c == *itr_s) {
        matched = 1;
        break;
      }
    }
    if (matched == 1) {
      //clear and reset return string
      memset(ret, '\0', 6);
      ret_itr = ret;
      matched = 0;
    } else {
      ++ret_itr;
    }
  }

  return ret;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage Timer: XhYmZs\n");
    return 0;
  }
  // Append times into one string
  char * time_str = argv[1];
  int i;
  for (i=2; i < argc; ++i) {
    time_str = strcat(time_str, argv[i]);
  }
  //Create variables and look for h,m,s
  unsigned int h=0, m=0, s=0;
  char time_cpy[20];
  char *h_loc = strchr(time_str, 'h');
  char *m_loc = strchr(time_str, 'm');
  char *s_loc = strchr(time_str, 's');
  //convert to time
  if (h_loc != NULL) {
    memcpy(time_cpy, time_str, strlen(time_str)+1);
    strtok(time_cpy, "h");
    h = strtoul(substring(time_cpy, "ms"), NULL, 0);
  }
  //Minute
  if (m_loc != NULL) {
    memcpy(time_cpy, time_str, strlen(time_str)+1);
    strtok(time_cpy, "m");
    m = strtoul(substring(time_cpy, "hs\0"), NULL, 0);
  }
  //Second
  if (s_loc != NULL) {
    memcpy(time_cpy, time_str, strlen(time_str)+1);
    strtok(time_cpy, "s");
    s = strtoul(substring(time_cpy, "hm\0"), NULL, 0);
  }
  printf("%d Hours %d Minutes %d Seconds\n", h, m, s);
  //set timer for that long
  if (open_pin("1013") == -1) { return -1; };
  write_dir("1013", "out");
  write_val("1013", "1"); //Use pin as source -- off
  unsigned long sleep_sec = 60*60*h + 60*m + s;
  sleep(sleep_sec);
  write_val("1013", "0");//Turn ON
  sleep(5);           //Keep on for 5s
  close_pin("1013");
  return 0;
}
