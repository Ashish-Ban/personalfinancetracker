#include "salary.h"
#include "../../myutils/stringutils.h"

#pragma warning(disable : 4996)

int show_current_month_salary(const struct tm* tm,const long int userId, MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row) {
	char CURRENT_MONTH_SALARY_QUERY[100] = "select idsalary,amount,month,year,user from salary where user='";
	char *AND_QUERY="' and month='";
	char* AND_QUERY_YEAR = "' and year='";
	char *CLOSING_QUERY = "'";
	char USER[6];
	char month[5];
	char year[5];

	int result_amount = -1;


	sprintf(month,"%d", (tm->tm_mon) + 1);
	sprintf(USER, "%li", userId);
	sprintf(year, "%d", (int)tm->tm_year + 1900);

	replace_with_values(CURRENT_MONTH_SALARY_QUERY, 6, USER, AND_QUERY, month, AND_QUERY_YEAR, year, CLOSING_QUERY);

	system("cls");
	/*printf("\n The MONTH = %s", month);
	printf("\n The YEAR = %s", year);
	printf("\n The USER = %s", USER);
	printf("\n The QUERY = %s \n\n", CURRENT_MONTH_SALARY_QUERY);*/
	if (mysql_query(conn, CURRENT_MONTH_SALARY_QUERY)) {
		fprintf(stderr, "Error occured while executing CURRENT_MONTH_SALARY_QUERY : %s", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	while((row = mysql_fetch_row(res)) != NULL) {
		//printf("\nThe row[1] in amount is : %s\n", row[1]);
		result_amount = (int) strtol(row[1], NULL,10);
		mysql_free_result(res);
		return result_amount;
	}
	mysql_free_result(res);
	return -1;
}

int add_current_month_salary(int salary, long int userId, MYSQL* conn, MYSQL_STMT* stmt, struct tm *tm){
	char INSERT_SALARY_QUERY[150] = "insert into salary(`amount`,`month`,`year`,`user`) values(?,?,?,?);";
	int user;
	int month;
	int year;
	int monthSalaryLength;
	char monthSalary[10];

	MYSQL_BIND bind[4];

	month = (tm->tm_mon) + 1;
	year = (int)tm->tm_year + 1900;
	user = (int)userId;
	sprintf(monthSalary, "%d", salary);
	
	printf("\n\n\n Salary inside add_current_month_salary is %d %li %d %d", salary,user,month,year);
	if (mysql_stmt_prepare(stmt, INSERT_SALARY_QUERY,120) != 0) {
		fprintf(stderr, "\nError while preparing query : %s\n %d", mysql_error(conn),mysql_errno(conn));
		exit(1);
	}

	monthSalaryLength =(int) strlen(monthSalary);
	memset(bind, 0, sizeof(bind));
	bind[0].buffer_type = MYSQL_TYPE_NEWDECIMAL;
	bind[0].buffer = monthSalary;
	bind[0].buffer_length = monthSalary;
	bind[0].is_null = 0;
	bind[0].length = &monthSalaryLength;


	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &month;
	bind[1].is_null = 0;
	bind[1].length = 0;

	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer = &year;
	bind[2].is_null = 0;
	bind[2].length = 0;

	bind[3].buffer_type = MYSQL_TYPE_LONG;
	bind[3].buffer = &user;
	bind[3].is_null = 0;
	bind[3].length = 0;

	if (mysql_stmt_bind_param(stmt,bind) != 0) {
		fprintf(stderr, "\nError occured while binding params in add_current_month_salary \nError: %s", mysql_stmt_error(stmt));
		return 1;
	}

	if ((mysql_stmt_execute(stmt)) != 0) {
		fprintf(stderr, "\nError occured while executing statement in add_current_month_salary \nError: %s", mysql_stmt_error(stmt));
		return 1;
	}

	printf("\n\n\n\t\tRs. %d added as salary of month %d %d", salary, month, year);
	system("pause");

	return 0;
}


void show_salary_history(MYSQL *conn, MYSQL_RES *res ,MYSQL_ROW row, struct tm *tm, long int userId) {
	char SELECT_QUERY[100] = "SELECT idsalary,amount,month,year,user from salary where year=";
	char year[5],user[6];

	sprintf(year, "%d", (int)tm->tm_year + 1900);
	sprintf(user,"%li",userId);

	replace_with_values(SELECT_QUERY, 4, year, " and user=", user, ";");
	if (mysql_query(conn, SELECT_QUERY)) {
		fprintf(stderr, "show_salary_history => Error in executing SELECT QUERY : %s", mysql_error(conn));
		return;
	}

	res = mysql_use_result(conn);
	system("cls");
	printf("\n\n\n\x1b[36m \t\t Showing Salary History for year %s  \x1b[0m \n",year);
	printf("\x1b[31m \t\t Amount\t | Month\t | Year\t  \x1b[0m");
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("\n\t\t\x1b[32m %s\t | \x1b[34m%s\t | \x1b[34m%s\t \x1b[0m  \n", row[1], row[2], row[3]);
	}

	mysql_free_result(res);
	return;
}