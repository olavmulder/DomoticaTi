#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "tables.h"

void errorResponse(int, char*);
void getMethod(char[], char**);
void actuator(char*, const char**, MYSQL*);
void printActuatorJSON(struct actuator* actuators, int size);

void finish_with_error(MYSQL *con)
{
  errorResponse(500, mysql_error(con));
  mysql_close(con);
  exit(1);        
}

int main(int argc, const char* argv[], char* env[]) {

  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      printf("mysql_init() failed\n");
      return 0;
  }  
  
  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    

  int i = 0;

  char METHOD[10];
  char REQUEST_REDIRECT[100];

  /*Get Method from request*/
  getMethod(METHOD, env);

  if(argc > 1) {
    memcpy(REQUEST_REDIRECT, argv[1], 99);

    if(strncmp(REQUEST_REDIRECT, "actuator", 8) == 0){
      actuator(METHOD, argv, con);
    } else {
      errorResponse(404, "Didn't found requested url");
    }


  } else {
    errorResponse(400, "Bad Request didn't recieve any parameters.");
  }

  mysql_close(con);
  
  return 0;
}

void errorResponse(int statusCode, char* message) {
  printf("STATUS: %d\n", statusCode);
  printf("Content-Type: application/json\n\n");
  printf("{\"errorMessage\": \"%s\"}", message);
}

void getMethod(char method[], char **env) {
  int index = 0;
  while(env[index] != NULL) {
    if(strncmp(env[index], "REQUEST_METHOD=", 15) == 0)
      memcpy(method, env[index]+15, 9);
    index++;
  }
}

void actuator(char* method, const char** argv, MYSQL* con) {
  struct actuator* actuators = malloc(1*sizeof(struct actuator));
  int i = 0, jsonCount = 0;

  if(memcmp(argv[1]+8, "/", 1) == 0) {
    //Further redirect
  } else {
    if(strcmp(method, "GET") == 0) {
       if (mysql_query(con, "SELECT * FROM actuator")) 
      {
          finish_with_error(con);
      }
      
      MYSQL_RES *result = mysql_store_result(con);
      
      if (result == NULL) 
      {
          finish_with_error(con);
      }

      int num_fields = mysql_num_fields(result);

      MYSQL_ROW row;

      if(num_fields == ACTUATOR_FIELDS) {
        i = 0;
        while ((row = mysql_fetch_row(result))) {
          actuators[i].actuatorid = atoi(row[0]);
          actuators[i].arduinoid = atoi(row[1]);
          actuators[i].value = atoi(row[2]);
          memcpy(actuators[i].type, row[3], ACTUATOR_SIZE_UNIT);
          memcpy(actuators[i].arduinovalueid, row[4], ACTUATOR_SIZE_ARDUINOVALUEID);
          memcpy(actuators[i].actuatorname, row[5], ACTUATOR_SIZE_ACTUATORNAME);
          i++;           
          actuators = realloc(actuators, (i+1) * sizeof(struct actuator));
        } 

        printf("Content-Type: application/json\n\n");
        printActuatorJSON(actuators, i);

      } else {
        errorResponse(500, "Struct doesn't match query output");
      }
    } else if(strcmp(method, "POST") == 0) {
      errorResponse(200, "METHODPOST");
    }
  }
}

void printActuatorJSON(struct actuator* actuators, int size) {
  int count;
  printf("{\"data\": [");

  for (count = 0; count < size; count++) {
    printf("{");
    printf("\"%s\": %d,", ACTUATOR_FIELD_NAMES[0], actuators[count].actuatorid);
    printf("\"%s\": %d,", ACTUATOR_FIELD_NAMES[1], actuators[count].arduinoid);
    printf("\"%s\": %d,", ACTUATOR_FIELD_NAMES[2], actuators[count].value);
    printf("\"%s\": \"%s\",", ACTUATOR_FIELD_NAMES[3], actuators[count].type);
    printf("\"%s\": \"%s\",", ACTUATOR_FIELD_NAMES[4], actuators[count].arduinovalueid);
    printf("\"%s\": \"%s\"", ACTUATOR_FIELD_NAMES[5], actuators[count].actuatorname);          
    printf("}");
    if(count+1 != size) {
      printf(",");
    }
  }
  printf("]}");
}
