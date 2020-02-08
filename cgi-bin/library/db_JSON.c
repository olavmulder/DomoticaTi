#include <mariadb/mysql.h>
#include "domoticaTi.h"


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

  printf("],\"time\":%ld}", time(NULL));
  
  mysql_free_result(result);
  mysql_close(con);

  return i;
}