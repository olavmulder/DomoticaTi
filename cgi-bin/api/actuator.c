#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "tables.h"

void finish_with_error(MYSQL*);
void errorResponse(int, char*);
void getMethod(char[], char**);
int getContentSize(char**);
void executeQuery(char*);
int validateActuator(struct actuator);
void createInsertQueryActuator(char*, struct actuator);
struct actuator postActuator();
int selectQueryJSON(char*);

int CONTENT_SIZE = 0;

int main(int argc, const char* argv[], char* env[]) {


  char METHOD[10];
  CONTENT_SIZE = getContentSize(env);

  /*Get Method from request*/
  getMethod(METHOD, env);

  if(argc == 1) {

    if(strcmp(METHOD, "GET") == 0) {
        selectQueryJSON("SELECT * FROM actuator");

    } else if(strcmp(METHOD, "POST") == 0) {
      struct actuator actuator;
      actuator = postActuator();

      if(validateActuator(actuator) > 0) {
        char* query = malloc(200);
        createInsertQueryActuator(query, actuator);
        executeQuery(query);
      } else {
        errorResponse(400, "validation failed.");
      }

    } else {
      errorResponse(400, "check request url");
    }
  } else if(argc == 2) { // one data
    if(strcmp(METHOD, "DELETE") == 0) {

      const char* actuatorid = argv[1];

      if(atoi(actuatorid) != 0 && strlen(actuatorid) > 0) {
        char* query = malloc(100);
        sprintf(query, "DELETE FROM actuator WHERE actuatorid=%d", atoi(actuatorid));
        executeQuery(query);

      } else {
        errorResponse(400, "validation vailed");
      }
    } else {
      errorResponse(400, "check request url");
    }
  } else if(argc > 2) { // one redirect + 1 data
    
  } else {
    errorResponse(404, "URL Not found. Please check all parameters");
  }

  return 0;
}

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

void actuator(char* method, const char** argv, int argc) {
  
}

void executeQuery(char* query) {
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
      errorResponse(500, "mysql_init() failed");
  }  
  
  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }    
  
  if (mysql_query(con, query)) {  
      finish_with_error(con);
  }
  
  mysql_close(con);
  printf("Content-Type: application/json\n\n");
  printf("{\"done\": true}");
}

int validateActuator(struct actuator actuator) {

  if(actuator.actuatorid != -1) { //user is not allowed to create own id.
    return -1;
  }

  if(actuator.arduinoid > 0) { //uncomment following piece when arduino table is active.
    /*char* query = malloc(100);
    char* response = malloc(2000);
    sprintf(query, "SELECT id FROM arduino WHERE id = %d", actuator.actuatorid);
    if(selectQueryJSON(response, query) <= 0) {
      return -1;
    }*/
  } else {
    return -1;
  }

  actuator.value = 0; //No matter what the user sends, the first value is always 0

  if(strlen(actuator.type) == 0) {
    return -1;
  }

  if(strlen(actuator.arduinovalueid) != 3) {
    return -1;
  }

  if(strlen(actuator.actuatorname) == 0) {
    return -1;
  }

  return 1;
}

void createInsertQueryActuator(char* query, struct actuator actuator) {
  strcpy(query, "INSERT INTO actuator (arduinoid, value, type, arduinovalueid, actuatorname) VALUES(");

  sprintf(query, "%s%d", query, actuator.arduinoid);
  strcat(query, ",");
  sprintf(query, "%s%d", query, actuator.value);
  strcat(query, ",'");
  strcat(query, actuator.type);
  strcat(query, "','");
  strcat(query, actuator.arduinovalueid);
  strcat(query, "','");
  strcat(query, actuator.actuatorname);
  strcat(query, "')");
}

struct actuator postActuator() {
  struct actuator newActuator = {-1,-1,-1,"","",""}; //define all elements so we can validate the struct
  
  char* data = malloc(CONTENT_SIZE+10);
  int index;

  fgets(data, CONTENT_SIZE, stdin);
  for(index = 0; index < ACTUATOR_FIELDS; index++) {
    char* dataPointer = strstr(data, ACTUATOR_FIELD_NAMES[index]);
    if(dataPointer != NULL) {

      dataPointer = dataPointer + 4 + strlen(ACTUATOR_FIELD_NAMES[index]); //+4 for: ": "
      char* dataPointerEnd = strchr(dataPointer, '"');

      int dataSize = dataPointerEnd - dataPointer;

      char* retrievedData = malloc(dataSize+1);
      strncpy(retrievedData, dataPointer, dataSize);
      retrievedData[dataSize] = '\0';

      switch (index) {
      case 0:        
        newActuator.actuatorid = atoi(retrievedData);
        break;
      
      case 1:
        newActuator.arduinoid = atoi(retrievedData);
        break;
      
      case 2:
        newActuator.value = atoi(retrievedData);
        break;
      
      case 3:
        strncpy(newActuator.type, retrievedData, ACTUATOR_FIELD_TYPE_SIZE);
        break;
      
      case 4:
        strncpy(newActuator.arduinovalueid, retrievedData, ACTUATOR_FIELD_ARDUINOVALUEID_SIZE);
        break;
      
      case 5:
        memcpy(newActuator.actuatorname, retrievedData, ACTUATOR_FIELD_ACTUATORNAME_SIZE);
        break;
      
      default:
        break;
      }
    }
  }

  return newActuator;
}

int selectQueryJSON(char* query) {

  int fieldCounter = 0, i = 0;
  printf("Content-Type: application/json\n\n");
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
      errorResponse(500, "mysql_init() failed");
  }  

  if (mysql_real_connect(con, "localhost", "domoticati", "domoticati", "domoticati", 0, NULL, 0) == NULL) {
      finish_with_error(con);
  }    

  if (mysql_query(con, query)) {  
      finish_with_error(con);
  }

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL) {
      finish_with_error(con);
  }  

  int num_fields = mysql_num_fields(result);
  char fields[num_fields][51];

  MYSQL_ROW row;
  MYSQL_FIELD *field;

  while(field = mysql_fetch_field(result)) {
    strncpy(fields[fieldCounter], field->name, 50);
    fieldCounter++;
  }

  printf("{\"data\": [ ");
  while (row = mysql_fetch_row(result)) { 
    if(i > 0) {
      printf(",");
    }
    printf("{");

      for(int i = 0; i < num_fields; i++) { 
          printf("\"");
          printf("%s", fields[i]);
          printf("\": \"");
          printf("%s", row[i] ? row[i] : "NULL");
          printf("\"");
          printf("%s", ((i+1) != num_fields) ? "," : "");
      }

    printf("}");
    i++;
  }

  printf("]}");
  
  mysql_free_result(result);
  mysql_close(con);

  return i;
}