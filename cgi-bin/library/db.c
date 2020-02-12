#include <mariadb/mysql.h>
#include "domoticaTi.h"

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