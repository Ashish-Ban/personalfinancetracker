#ifndef SALARY_H
#define SALARY_H
#include <time.h>
#include <Windows.h>
#include <stdio.h>
#include <mysql.h>

int show_current_month_salary(const struct tm* tm, long int userId, MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row);

/// <summary>
/// Add Salary to database for given user. All parameters are required.
/// </summary>
/// <param name="salary">int salary is the amount of salary</param>
/// <param name="userId">long int userId is the user id whose salary is to be entered</param>
/// <param name="conn">MYSQL *conn is the MYSQL structure pointer conn (connection object) required to query the database </param>
/// <param name="stmt">MYSQL_STMT *stmt is the statement object required to execute the statement</param>
/// <param name="tm">struct tm *tm is pointer to a time struct , used to extract date/month etc </param>
/// <returns>1 if any errors while inserting data.Returns 0 if everything is successfull.</returns>
int add_current_month_salary(int salary, long int userId, MYSQL* conn, MYSQL_STMT* stmt, struct tm* tm);

#endif // SALARY_H
