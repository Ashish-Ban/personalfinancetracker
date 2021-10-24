#include "loan.h"

#pragma warning(disable : 4996 6031)

int continue_add_loan(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, long user, int start_year, int duration_years, int amount, float interest_rate, char* loan_name, char* bank_name, int* main_loop_status) {
	char QUERY[250] = "insert into loan (`loanName`,`startYear`,`interestRate`,`durationYears`,`bankName`,`amount`,`addedBy`) values (?,?,?,?,?,?,?);";
	char INTEREST_RATE[6];
	char AMOUNT[11];
	unsigned long loan_length, interest_length, bank_length, amount_length;
	int userId;
	
	MYSQL_BIND bind[7];
	MYSQL_STMT* stmt;

	sprintf(INTEREST_RATE, "%4.2f", interest_rate);
	sprintf(AMOUNT, "%d", amount);

	stmt = mysql_stmt_init(conn);
	if (mysql_stmt_prepare(stmt, QUERY, 250) != 0) {
		fprintf(stderr, "\nError while preparing query continue_add_loan : %s\n %d", mysql_error(conn), mysql_errno(conn));
		mysql_stmt_close(stmt);
		return 1;
	}

	memset(bind, 0, sizeof(bind));
	loan_length = (unsigned long)strlen(loan_name);
	interest_length = (unsigned long)strlen(INTEREST_RATE);
	amount_length = (unsigned long)strlen(AMOUNT);
	bank_length = (unsigned long)strlen(bank_name);
	userId = (int)user;

	printf("\n Loan Length : %d \n Interest Length : %d \n Amount Length : %d \n User Id : %d ", loan_length, interest_length, amount_length, userId);

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = loan_name;
	bind[0].buffer_length = loan_length;
	bind[0].is_null = 0;
	bind[0].length = &loan_length;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &start_year;
	bind[1].is_null = 0;
	bind[1].length = 0;

	bind[2].buffer_type = MYSQL_TYPE_NEWDECIMAL;
	bind[2].buffer = INTEREST_RATE;
	bind[2].buffer_length = interest_length;
	bind[2].is_null = 0;
	bind[2].length = &interest_length;

	bind[3].buffer_type = MYSQL_TYPE_LONG;
	bind[3].buffer = &duration_years;
	bind[3].is_null = 0;
	bind[3].length = 0;

	bind[4].buffer_type = MYSQL_TYPE_STRING;
	bind[4].buffer = bank_name;
	bind[4].buffer_length = bank_length;
	bind[4].is_null = 0;
	bind[4].length = &bank_length;

	bind[5].buffer_type = MYSQL_TYPE_NEWDECIMAL;
	bind[5].buffer = AMOUNT;
	bind[5].buffer_length = amount_length;
	bind[5].is_null = 0;
	bind[5].length = &amount_length;

	bind[6].buffer_type = MYSQL_TYPE_LONG;
	bind[6].buffer = &user;
	bind[6].is_null = 0;
	bind[6].length = 0;

	if (mysql_stmt_bind_param(stmt, bind) != 0) {
		fprintf(stderr, "\nError occured while binding params in continue_add_loan \nError: %s", mysql_stmt_error(stmt));
		return 1;
	}

	if ((mysql_stmt_execute(stmt)) != 0) {
		fprintf(stderr, "\nError occured while executing statement in continue_add_loan \nError: %s", mysql_stmt_error(stmt));
		return 1;
	}

	printf("\n\n\n\t\tLoan %s Added Successfully \n\t\t", loan_name);
	system("pause");

	mysql_stmt_close(stmt);

	// Stop the add_loan Menu
	*main_loop_status = -1;
	return 0;
}

void add_loan(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, long user) {
	int start_year=0, duration_years=0, amount,choice=0;
	float interest_rate = 0;
	char loan_name[45], bank_name[45];

	while (choice != -1) {
		system("cls");
		printf("\n\n\n\t\t Loan Name      \t\t");
		getchar(); // getchar to eat up the \n left over by scanf So that fgets reads properly.
		fgets(loan_name, sizeof(loan_name), stdin);
		loan_name[strcspn(loan_name, "\n")] = '\0';
		printf("\n\t\t Bank Name        \t\t");
		fgets(bank_name, sizeof(bank_name), stdin);
		bank_name[strcspn(bank_name, "\n")] = '\0';
		printf("\n\t\t Start Year       \t\t");
		scanf_s("%d", &start_year);
		printf("\n\t\t Duration (Years) \t\t");
		scanf_s("%d", &duration_years);
		printf("\n\t\t Interest Rate    \t\t");
		scanf_s("%f", &interest_rate);
		printf("\n\t\t Amount           \t\t");
		scanf_s("%d", &amount);


		system("cls");
		printf("\n\n\t\tLoan Details you Entered\n\n\n");
		printf("\n\t\tLoan Name           : %s ",loan_name);
		printf("\n\t\tBank Name           : %s ", bank_name);
		printf("\n\t\tStart Year          : %d ", start_year);
		printf("\n\t\tInterest Rate       : %4.2f %% ", interest_rate);
		printf("\n\t\tDuration Years      : %d Yrs ", duration_years);
		printf("\n\t\tAmount              : Rs. %d \n", amount);
		printf("\n\t\tVerify above details. Continue?");
		printf("\n\t\t1. Yes, Continue");
		printf("\n\t\t2. No, Change values");
		printf("\n\t\t3. Go Back");
		printf("\n\n\t\t");
		scanf_s("%d", &choice);
		switch (choice)
		{
			case 1:
				if (loan_name != " " && loan_name != "\n" && loan_name != "\r" && bank_name != " " && bank_name != "\n" && bank_name != "\r" && start_year > 0 && duration_years > 0 && interest_rate > 0 && amount > 0) {
					if (continue_add_loan(conn, res, row, user, start_year, duration_years, amount, interest_rate, loan_name, bank_name, &choice)) {
						fprintf(stderr, "\n\t\tError occured while adding loan. Try again later");
						system("pause");
						start_year = 0, interest_rate = 0, duration_years = 0, amount = 0;
						strcpy(loan_name, "\0");
						strcpy(bank_name, "\0");
					}
				}
				else {
					printf("\n\t\t\x1b[31mLoan Name, Bank, start year, duration , interest rate and amount are required.\x1b[0m");
					system("pause");
					continue;
				}
				break;
			case 2:
				start_year = 0, interest_rate = 0, duration_years = 0, amount = 0;
				strcpy(loan_name, "\0");
				strcpy(bank_name, "\0");
				break;
			case 3:
				choice = -1;
				break;
			default:
				printf("\n\x1b[31mInvalid Choice\n\x1b[0m");
				system("pause");
				break;
		}

	}
}

void list_loans(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, long user) {
	char QUERY[120] = "SELECT idloan,loanName,startYear,interestRate,durationYears,bankName,amount,addedBy FROM loan where addedBy=";
	char USER[6];

	sprintf(USER, "%li", user);
	replace_with_values(QUERY, 2, USER, ";");

	if (mysql_query(conn, QUERY)) {
		fprintf(stderr, "\nError in getting loans : %s", mysql_error(conn));
		return;
	}

	res = mysql_use_result(conn);
	system("cls");
	printf("\n\n\n\x1b[36m \t\t Loans Added By You  \x1b[0m \n");
	printf("\x1b[31m \t\t Name\t | Start Year\t | Interest Rate\t | Duration \t | Bank\t | Amount\t \x1b[0m");
	
	while ((row= mysql_fetch_row(res)) != NULL) {
		printf("\n\t\t\x1b[32m %s\t | %s\t | %s %% \t | %s Yrs\t | %s\t | Rs. %s\t \x1b[0m  \n", row[1], row[2], row[3], row[4], row[5], row[6]);
	}

	mysql_free_result(res);
	printf("\n\t\t ");
	system("pause");
	return;
}