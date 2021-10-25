#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mysql.h>
#include <Windows.h>
#include "models/user/usermodal.h"
#include "models/salary/salary.h"
#include "models/loan/loan.h"
#include "myutils/stringutils.h"

#pragma warning (once: 4996 6031 )

COORD coord = { 0,0 };

void gotoxy(int x, int y);
void showSalaryMenu(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, MYSQL_STMT* stmt, long userId, struct tm* tm);
void showLoanMenu(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, MYSQL_STMT* stmt, long userId);

int main() {
	char* username = "root";
	char* password = "";
	char* server = "localhost";
	char* database = "personalfinance";
	unsigned int port = 0;
	char* unix_socket = NULL;
	unsigned long clientflag = 0;

	MYSQL* conn;
	MYSQL_RES* res=NULL;
	MYSQL_STMT* stmt=NULL;
	MYSQL_ROW row=NULL;

	char* user = (char *) malloc(50);
	char* pass = (char *) malloc(50);
	long int auth_result; // This variable holds the User id if there is a user authenticated

	time_t t=time(0);
	struct tm tm;
	localtime_s(&tm,&t);
	int mainMenuChoice=0;

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	/* MoveWindow(console, r.left, r.top, width, height, TRUE) Format*/
	MoveWindow(console, r.left, r.top, 840, 400, TRUE);


	conn = mysql_init(NULL);
	// Connect to database 
	if (!mysql_real_connect(conn, server, username, password, database, port, unix_socket, clientflag)) {
		fprintf(stderr, "%s \n", mysql_error(conn));
		exit(1);
	}

	//initalise statement stmt ;
	stmt = mysql_stmt_init(conn);
	if (stmt == NULL) {
		fprintf(stderr, "\nError in initialising statement : %s \n", mysql_error(conn));
		exit(1);
	}

	// Send Sql query 
	/*if (mysql_query(conn,"show tables")) {
		fprintf(stderr, "%s \n", mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);*/
	// output table name 
	/*printf("MY SQL Table in mysql database: \n");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s \n", row[0]);
	}*/


	/*
	* Below code starts the authentication loop.. the user can try entering username/passwords and 
	* loop only stops when the user is authenticated !
	*/
	do {
		system("cls");
		printf("\n\n\n\n\n\t\tEnter username\n\t\t");
		fgets(user, sizeof(user), stdin);
		printf("\n\t\tEnter password\n\t\t");
		fgets(pass, sizeof(pass), stdin);

		//printf("\n Authenticating the user \n");
		auth_result = authenticate_user(conn, res, row, user, pass);

		if (auth_result == 0) {
			printf("\n\n\t\tNo user records found !");
			system("pause");
		}
		else {
			printf("\n\t\tUser Authenticated success ");
			break;
		}
	} while (1);
	/*
	* Authentication loop ends here ! 
	*/

	
	/*
	* Main Menu loop starts here
	*/
	while (mainMenuChoice != -1) {
		system("cls");
		printf("\n\n\n\n\n");
		printf("\n\t\tWelcome, %s UID : %li \t\t\t\t %02d-%02d-%d %02d:%02d:%02d \n\n", user, auth_result, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
		printf("\n\t\t1. Salary");
		printf("\n\t\t2. Loans");
		printf("\n\t\t3. Export Data");
		printf("\n\t\t4. Exit");
		printf("\n\n\t\tEnter Choice");
		printf("\n\n\t\t");
		scanf_s("%d", &mainMenuChoice);
		switch (mainMenuChoice)
		{
			case 1:
				system("cls");
				showSalaryMenu(conn,res,row,stmt,auth_result,&tm);
				break;
			case 2:
				system("cls");
				showLoanMenu(conn, res, row, stmt, auth_result);
				break;
			case 3:
				system("cls");
				printf("\nExporting your data ! \n");
				system("pause");
				break;
			case 4: 
				system("cls");
				printf("\n\n\t\tGood Bye\n\t\t");
				mainMenuChoice = -1;
				break;
			default:
				system("cls");
				printf("\n\t\tInvalid Choice Main Menu\n\t\t");
				system("pause");
				break;
		}
	}
	/*
	* Main Menu loop ends here
	*/

	
	
	/*
	* Free Dynamically allocated variables and pointers 
	*/
	free(user);
	free(pass);

	// close connection 
	mysql_free_result(res);
	mysql_stmt_close(stmt);
	mysql_close(conn);

	return 0;
}



/*
* gotoxy is used to place the cursor into the given coordinates like in old c gotoxy function
*/
void gotoxy(int x, int y) {
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/// <summary>
/// Shows the salary menu loop, takes MYSQL *conn connection, MYSQL_RES *res result set, MYSQL_STMT *stmt ,userId and time structure *tm;
/// All the parameters are required.
/// </summary>
/// <param name="conn">MYSQL *conn MYSQL connection structure</param>
/// <param name="res">MYSQL_RES *res to store result</param>
/// <param name="row">MYSQL_ROW row to iterate rows</param>
/// <param name="stmt"></param>
/// <param name="userId"></param>
/// <param name="tm"></param>
void showSalaryMenu(MYSQL *conn, MYSQL_RES *res, MYSQL_ROW row, MYSQL_STMT *stmt, long userId, struct tm *tm) {
	int salaryMenuChoice = 0;
	int currentMonthSalary;
	while (salaryMenuChoice != -1) {
		system("cls");
		printf("\n\n\n\n\n");
		printf("\n\t\tShowing Salary \n\n");
		printf("\n\t\t1. Current Month Salary Status");
		printf("\n\t\t2. Current Month Salary Entry");
		printf("\n\t\t3. View Salary History");
		printf("\n\t\t4. Go Back");
		printf("\n\n\t\tEnter Choice");
		printf("\n\n\t\t");
		scanf_s("%d", &salaryMenuChoice);
		switch (salaryMenuChoice)
		{
			case 1:
				system("cls");
				currentMonthSalary = show_current_month_salary(tm, userId, conn, res, row);
				if (currentMonthSalary == -1) {
					printf("\n\n\n\n\t\tNo Salary This Month \n\n\n\n\t\t");
				}
				else {
					printf("\n\n\n\n\t\tCurrent Month Salary : %d \n\n\n\n\t\t", currentMonthSalary);
				}
				system("pause");
				break;
			case 2:
				system("cls");
				printf("\nEnter Current Month Salary\n");
				scanf_s("%d", &currentMonthSalary);
				if ((add_current_month_salary(currentMonthSalary, userId, conn, stmt, tm)) == 1) {
					system("cls");
					printf("\n\n\n\n\t\tSomething went wrong while addding salary. Try again later. \n\n\n\n\t\t");
				}
				else {
					system("cls");
					printf("\n\n\n\n\t\tSalary Added Successfully\n\n\n\n\t\t");
				}
				system("pause");
				break;
			case 3:
				system("cls");
				printf("\nViewing Your Salary History\n");
				show_salary_history(conn, res, row, tm, userId);
				printf("\n\n\n\t\t");
				system("pause");
				break;
			case 4:
				system("cls");
				printf("\nGoing Back\n");
				salaryMenuChoice = -1;
				break;
			default:
				system("cls");
				printf("\nInvalid Choice Salary\n");
				system("pause");
				break;
		}
	}
}

void showLoanMenu(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, MYSQL_STMT* stmt, long user) {
	int choice = 0;
	while (choice != -1) {
		system("cls");
		fflush(stdin);
		printf("\n\n\n\n\n");
		printf("\n\t\tShowing Loans \n\n");
		printf("\n\t\t1. Add Loans");
		printf("\n\t\t2. List Loans");
		printf("\n\t\t3. Loan status entry");
		printf("\n\t\t4. View Loan status");
		printf("\n\t\t5. Go Back");
		printf("\n\n\t\tEnter Choice");
		printf("\n\n\t\t");
		scanf_s("%d", &choice);
		switch (choice) {
			case 1:
				system("cls");
				add_loan(conn, res, row, user);
				break;
			case 2:
				system("cls");
				list_loans(conn,res,row,user);
				break;
			case 3:
				system("cls");
				add_loan_status(conn,res,row,user);
				break;
			case 4:
				system("cls");
				view_loan_status(conn, res, row, user);
				break;
			case 5:
				system("cls");
				choice = -1;
				break;
			default :
				system("cls");
				printf("\n\t\tInvalid Choice");
				system("pause");
				break;
		}
	}
}