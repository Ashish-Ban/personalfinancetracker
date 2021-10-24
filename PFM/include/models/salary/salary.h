#ifndef SALARY_H
#define SALARY_H
#include <time.h>
#include <Windows.h>
#include <stdio.h>
#include <mysql.h>

/// <summary>
/// Shows the current Months salary for the logged in user.
/// </summary>
/// <param name="tm">struct tm *tm used to get current month / year.</param>
/// <param name="userId">user id of the logged in user.</param>
/// <param name="conn">MYSQL_CONN *conn connection object.</param>
/// <param name="res">MYSQL_RES *res connection </param>
/// <param name="row"></param>
/// <returns></returns>
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

/// <summary>
/// Prints out the all the salary records of this year.
/// Also can show a graph ;)
/// </summary>
/// <param name="conn">MYSQL_CONN *conn, Mysql connection to use.</param>
/// <param name="res">MYSQL_RES *res, Mysql result to store the result.</param>
/// <param name="row">MYSQL_ROW row, to store the result row</param>
/// <param name="tm">struct tm *tm, to use to get current year and month</param>
/// <param name="userId">Id of user to get and show the salary history </param>
void show_salary_history(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, struct tm* tm, long int userId);

#endif // SALARY_H
