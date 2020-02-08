#include "domoticaTi.h"

void getMethod(char method[], char **env) {
  int index = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "REQUEST_METHOD=", 15) == 0) {
      memcpy(method, env[index]+15, 9);
    }
    index++;
  }
}

int getContentSize(char **env) {
  int index = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "CONTENT_LENGTH=", 15) == 0){
      if(strlen(env[index]) > 15) {
        return atoi(env[index]+15);
      } else {
        return 0;
      }
    }
    index++;
  }
  return 0;
}