#pragma once

#include "../../myutils/stringutils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql.h>

void list_loans(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, long user);
void add_loan(MYSQL* conn, MYSQL_RES* res, MYSQL_ROW row, long user);