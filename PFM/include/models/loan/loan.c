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

	//printf("\n Loan Length : %d \n Interest Length : %d \n Amount Length : %d \n User Id : %d ", loan_length, interest_length, amount_length, userId);

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
		printf("\n\t\t ");
		system("pause");
		return;
	}

	res = mysql_use_result(conn);
	system("cls");
	printf("\n\n\n\x1b[36m \t\t Loans Added By You  \x1b[0m \n");
	printf("\x1b[31m \t\t Loan Id\t Name\t | Start Year\t | Interest Rate\t | Duration \t | Bank\t | Amount\t \x1b[0m\n\n");
	
	while ((row= mysql_fetch_row(res)) != NULL) {
		printf("\n\t\t\x1b[32m %s\t %s\t  %s\t  %s %% \t  %s Yrs\t  %s\t  Rs. %s\t \x1b[0m", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
	}

	mysql_free_result(res);
	printf("\n\t\t ");
	system("pause");
	return;
}

void add_loan_status(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, long user) {
	char SELECT_QUERY[120] = "SELECT `startYear`,`interestRate`,`durationYears`,`amount` FROM loan WHERE idloan=\"";
	char INSERT_QUERY[120] = "INSERT INTO loanstatus(`loan_idloan`,`loan_addedBy`,`month`,`year`,`amount`) VALUES (";
	char lid[6], uid[6], monthly[7 * 8], curr_month[5], curr_year[5]; // float is converted to string, so during convertion it requires char buffer to contain the float so 7 char * 8 bytes of a float.(Else corruption occurs in the stack)
	float amount, interest_rate,installment_amount, interest;
	int loanId=-1, year, duration;
	
	time_t t = time(0);
	struct tm tm;
	localtime_s(&tm, &t);

	this:
	system("cls");
	printf("\n\n\t\tRecord Loan Installment");
	printf("\n\n\t\tEnter Loan Id \t\t");
	scanf_s("%d", &loanId);
	if (loanId == -1) {
		printf("\n\n\t\tChech your loan id\n\n\t\t");
		system("pause");
		goto this;
	}

	sprintf(lid,"%d",loanId);
	sprintf(uid, "%li", user);

	replace_with_values(SELECT_QUERY,4,lid,"\" and addedBy=\"",uid,"\";");
	//printf("\n\n\t\t QUERY: %s \n\t\t", SELECT_QUERY);

	if (mysql_query(conn, SELECT_QUERY)) {
		fprintf(stderr, "\nError in getting loan based on entered loan id : %s", mysql_error(conn));
		return;
	}

	res = mysql_use_result(conn);
	//while ((row = mysql_fetch_row(res)) != NULL) {
	//	//printf("\n\t\t %s\t %s %%\t  %s\t  %s\n\t\t", row[0], row[1], row[2], row[3]);
	//	year = atoi(row[0]);
	//	interest_rate = atof(row[1]);
	//	duration = atoi(row[2]);
	//	amount = atof(row[3]);
	//	//printf("\n\t\t Converted:\n\t\t %d\t %4.2f %%\t  %d\t  %4.2f\n\t\t", year, interest_rate, duration,amount);
	//}
	//mysql_free_result(res);

	do {
		if ((row = mysql_fetch_row(res)) != NULL) {
			//printf("\n\t\t %s\t %s %%\t  %s\t  %s\n\t\t", row[0], row[1], row[2], row[3]);
			year = atoi(row[0]);
			interest_rate = atof(row[1]);
			duration = atoi(row[2]);
			amount = atof(row[3]);
			//printf("\n\t\t Converted:\n\t\t %d\t %4.2f %%\t  %d\t  %4.2f\n\t\t", year, interest_rate, duration,amount);
		}
		else {
			system("cls");
			printf("\n\n\t\tNo records found for the entered loan id\n\n\t\t");
			system("pause");
			mysql_free_result(res);
			return;
		}
	} while ((row = mysql_fetch_row(res)) != NULL);
	mysql_free_result(res);

	interest = amount * (interest_rate / 100) * duration;// interest amount  = p * r * y // simple interest
	installment_amount = (amount + interest) / (duration * 12); // monthly amount = p + i / r * 12;
	//printf("\n\t\t The Installment amount is %4.2f ", installment_amount);
	sprintf(monthly, "%10.2f", installment_amount);
	sprintf(curr_month, "%d", (int)tm.tm_mon + 1);
	sprintf(curr_year, "%d", (int)tm.tm_year + 1900);
	//printf("\n\t\t The String Monthly is %s Mon: %s Year: %s", monthly,curr_month,curr_year);

	replace_with_values(INSERT_QUERY, 10, lid, ", ", uid, ", ", curr_month, ", ", curr_year, ", ", monthly, ");");
	//printf("\n\t\t INSERT QUERY : %s", INSERT_QUERY);

	if (mysql_query(conn, INSERT_QUERY)) {
		fprintf(stderr, "\nError in inserting loan based on entered loan id : %s", mysql_error(conn));
		return;
	}

	printf("\n\t\t Montly Installment recorded. ");
	system("pause");
	return;
}

void view_loan_status(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, long user) {
	char SELECT_QUERY[150] = "SELECT `startYear`,`interestRate`,`durationYears`,`amount`,`loanName`,`bankName`,`idloan`  FROM loan WHERE idloan=\"";
	char STATUS_QUERY[150] = "SELECT `idloanStatus`,`loan_idloan`,`loan_addedBy`,`month`,`year`,`amount` from loanstatus where loan_idloan=";
	char lid[6], uid[6],loanName[46];
	float amount, interest_rate, installment_amount, interest, ls_amount;
	int loanId = -1, year, duration,count=0;

	this:
	system("cls");
	printf("\n\n\t\tView Loan Status");
	printf("\n\n\t\tEnter Loan Id \t\t");
	scanf_s("%d", &loanId);
	if (loanId == -1) {
		printf("\n\n\t\tChech your loan id\n\n\t\t");
		system("pause");
		goto this;
	}

	sprintf(lid, "%d", loanId);
	sprintf(uid, "%li", user);

	replace_with_values(SELECT_QUERY, 4, lid, "\" and addedBy=\"", uid, "\";");
	//printf("\n\n\t\t QUERY: %s \n\t\t", SELECT_QUERY);

	replace_with_values(STATUS_QUERY, 4, lid, " and loan_addedBy=", uid, ";");
	//printf("\n\n\t\t QUERY: %s \n\t\t", STATUS_QUERY);

	if (mysql_query(conn, SELECT_QUERY)) {
		fprintf(stderr, "\nError in getting loan based on entered loan id : %s", mysql_error(conn));
		printf("\n\n\t\t");
		system("pause");
		return;
	}

	res = mysql_use_result(conn);
	do {
		if ((row = mysql_fetch_row(res)) != NULL) {
			//printf("\n\t\t %s\t %s %%\t  %s\t  %s\n\t\t", row[0], row[1], row[2], row[3]);
			system("cls");
			printf("\n\n");
			printf("\n\t\tShowing status of %s\n", row[4]);
			printf("\n\t\tLoan ID       : %s", row[6]);
			printf("\n\t\tName          : %s", row[4]);
			printf("\n\t\tBank          : %s", row[5]);
			printf("\n\t\tStarted Year  : %s", row[0]);
			printf("\n\t\tDuration      : %s", row[2]);
			printf("\n\t\tAmount        : %s", row[3]);
			printf("\n\t\tInterest Rate : %s", row[1]);
			strcpy(loanName, row[4]);
			year = atoi(row[0]);
			interest_rate = atof(row[1]);
			duration = atoi(row[2]);
			amount = atof(row[3]);
		}
		else {
			system("cls");
			printf("\n\n\t\tNo records found for the entered loan id\n\n\t\t");
			mysql_free_result(res);
			printf("\n\n\t\t");
			system("pause");
			return;
		}
	} while ((row = mysql_fetch_row(res)) != NULL);
	mysql_free_result(res);

	interest = amount * (interest_rate / 100) * duration;// interest amount  = p * r * y // simple interest
	installment_amount = (amount + interest) / (duration * 12); // monthly amount = p + i / r * 12;
	//printf("\n\t\t The Installment amount is %4.2f ", installment_amount);

	if (mysql_query(conn, STATUS_QUERY)) {
		fprintf(stderr, "\nError in getting loanstatus based on entered loan id : %s", mysql_error(conn));
		printf("\n\n\t\t");
		system("pause");
		return;
	}

	res = mysql_use_result(conn);
	printf("\n\n\t\tInstallment Status of loan %s\n\n\t\tAmount \t\t Month \t\t Year \t\n",loanName);
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("\n\t\t%s\t\t %s\t\t %s\t\t", row[5], row[3], row[4]);
		ls_amount = atof(row[5]);
		count++;
	}
	mysql_free_result(res);

	//printf("\n\n\t\t COUNT : %d \n\t\t", count);
	if (count > 0) {
		printf("\n");
		printf("\n\t\tTotal Amount    : Rs %10.2lf ", ((double)amount + (double)interest));
		printf("\n\t\tAmount Pending  : Rs %10.2lf ", ((double)amount + (double)interest) - ((double)ls_amount * count));
		printf("\n\t\tAmount Paid     : Rs %10.2lf ", ((double)ls_amount * count));
	}

	printf("\n\n\t\t");
	system("pause");
	return;
}