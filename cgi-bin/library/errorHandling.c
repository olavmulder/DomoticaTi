#include "domoticaTi.h"

void finish_with_error(MYSQL *con) {
  char error[150];
  strncpy(error, mysql_error(con), 150);
  errorResponse(500, error);
  mysql_close(con);
  exit(1);        
}

void errorResponse(int statusCode, char* message) {
  printf("STATUS: %d\n", statusCode);
  printf("Content-Type: application/json\n\n");
  printf("{\"errorMessage\": \"%s\"}", message);
  exit(0);
}