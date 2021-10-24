#ifndef USERMODAL_H
#define USERMODAL_H

#include<mysql.h>

/// <summary>
/// Creates a user record in users table in the database with given username and password 
/// </summary>
/// <param name="conn">MYSQL_CONN *conn msyql connection to use</param>
/// <param name="stmt">MYSQL_STMT *stmt mysql statement to use</param>
/// <param name="user">String user which is the username to create a user record</param>
/// <param name="pass">String password for user created in users record </param>
/// <returns>error?1:0</returns>
int create_user(MYSQL* conn, MYSQL_STMT* stmt, char user[], char pass[]);

/// <summary>
/// Checks whether the user is present in users table. If user is present the returns user id else 0.
/// </summary>
/// <param name="conn">MYSQL_CONN *conn , Mysql connection to use.</param>
/// <param name="res">MYSQL_RES *res to store the result.</param>
/// <param name="row">MYSQl_ROW to store the result rows.</param>
/// <param name="user">User to check.</param>
/// <param name="pass">Password to check.</param>
/// <returns>user? userid:0</returns>
long int authenticate_user(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, char user[], char pass[]);

#endif
