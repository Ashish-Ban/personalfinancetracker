#ifndef USERMODAL_H
#define USERMODAL_H

#include<mysql.h>

int create_user(MYSQL* conn, MYSQL_STMT* stmt, char user[], char pass[]);
long int authenticate_user(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, char user[], char pass[]);

#endif
