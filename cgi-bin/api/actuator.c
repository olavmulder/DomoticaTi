#include "tables.h"
#include "../library/domoticaTi.h"
#include <mariadb/mysql.h>


int validateActuator(struct actuator);
void createInsertQueryActuator(char*, struct actuator);
struct actuator postActuator();

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