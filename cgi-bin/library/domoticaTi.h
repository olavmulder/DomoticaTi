#include <mariadb/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Error Response
*/

void finish_with_error(MYSQL*);
void errorResponse(int, char*);



/**
 * db_JSON
*/

int selectQueryJSON(char*);



/**
 * database
*/

void executeQuery(char*);

/**
 * httpFunctions
*/

void getMethod(char[], char**);
int getContentSize(char**);

/**
 * validation
*/

void removeBadCharacters(char*);