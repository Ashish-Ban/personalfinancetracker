#include<stdio.h>
#include<string.h>
#include <mysql.h>
#include "usermodal.h"
#include "../../myutils/stringutils.h"

int create_user(MYSQL* conn, MYSQL_STMT* stmt, char user[], char pass[]) {
	MYSQL_BIND bind[2];
	const char INSERT_QUERY[] = "insert into users(username,password) values(?,?)";
	unsigned long userLen = strlen(user);
	unsigned long passLen = strlen(pass);

	// prepare the query in c 
	if (mysql_stmt_prepare(stmt, INSERT_QUERY, sizeof(INSERT_QUERY)) != 0) {
		fprintf(stderr, "Error while preparing query : %s\n", mysql_error(conn));
		exit(1);
	}

	// Bind the paramerters to the bind variable 
	memset(bind, 0, sizeof(bind));
	bind[0].buffer_type = MYSQL_TYPE_VARCHAR;
	bind[0].buffer = user;
	bind[0].buffer_length = user;
	bind[0].length = &userLen;

	bind[1].buffer_type = MYSQL_TYPE_VARCHAR;
	bind[1].buffer = pass;
	bind[1].buffer_length = pass;
	bind[1].length = &passLen;

	if (mysql_stmt_bind_param(stmt, bind) != 0) {
		fprintf(stderr, "Error occured while binding params to query : %s \n", mysql_stmt_error(stmt));
		return 1;
	}

	if ((mysql_stmt_execute(stmt)) != 0) {
		fprintf(stderr, "Error occured while executing insert operation : %s \n", mysql_stmt_error(stmt));
		return 1;
	}

	fputs("User created Successfully ", stdout);
	return 0;
}


long int authenticate_user(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row , char *user, char *pass) {
	char CHECK_USER_QUERY[200]="SELECT idusers,username, password from users where username=\"";
	char *and_clause = "\" and password=\"";
	char *quote_string = "\"";
	long int userId;
	replace_with_values(CHECK_USER_QUERY, 4, user, and_clause, pass, quote_string);
	
	if (mysql_query(conn,CHECK_USER_QUERY)) {
		fprintf(stderr, "%s \n", mysql_error(conn));
		exit(1);
	} 
	res = mysql_use_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("\n\n ROW : %s %s %s  \n", row[0], row[1],row[2] );
		userId = strtol(row[0], NULL, 10);
		mysql_free_result(res);
		return userId;
	}
	mysql_free_result(res);
	return 0;
}