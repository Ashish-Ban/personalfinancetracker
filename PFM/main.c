#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <Windows.h>
#include "modals/usermodal.h"
#include "myutils/stringutils.h"


COORD coord = { 0,0 };

void gotoxy(int x, int y);

int main() {
	char* username = "root";
	char* password = "";
	char* server = "localhost";
	char* database = "personalfinance";
	unsigned int port = 0;
	char* unix_socket = NULL;
	unsigned long clientflag = 0;

	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row=NULL;

	char* user = (char *) malloc(50);
	char* pass = (char *) malloc(50);
	int auth_result;
	


	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	/* MoveWindow(console, r.left, r.top, width, height, TRUE) Format*/
	MoveWindow(console, r.left, r.top, 800, 800, TRUE);


	conn = mysql_init(NULL);
	// Connect to database 
	if (!mysql_real_connect(conn, server, username, password, database, port, unix_socket, clientflag)) {
		fprintf(stderr, "%s \n", mysql_error(conn));
		exit(1);
	}

	// Send Sql query 
	if (mysql_query(conn,"show tables")) {
		fprintf(stderr, "%s \n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	// output table name 
	/*printf("MY SQL Table in mysql database: \n");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s \n", row[0]);
	}*/
	printf("\n Enter the username\n");
	fgets(user,sizeof(user),stdin);
	system("cls");
	printf("\n Enter the password for new user\n");
	fgets(pass, sizeof(pass),stdin );


	printf("\n Authenticating the user \n");
	auth_result = authenticate_user(conn, res, row, user, pass);

	if (auth_result == 0) {
		printf("\n No user records found !");
	}
	else {
		printf("\n User Authenticated success ");
	}

	free(user);
	free(pass);

	// close connection 
	mysql_free_result(res);
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