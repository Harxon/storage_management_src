#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sqlite3.h"
#include "cgic.h"

#define N	64
#define SQLITE_LOGIN "/dat/user.db"

int cgiMain()
{
	char name[N] = {0};
	char pw[N] = {0};
	char sql[N] = {0};
	char **result;
	int row, column;
	sqlite3 *db;

	cgiFormStringNoNewlines("username", name, N);
	cgiFormStringNoNewlines("password", pw, N);

	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<TITLE>登录提示</TITLE></HEAD>\n");

	if(sqlite3_open(SQLITE_LOGIN, &db) != SQLITE_OK)
	{
		fprintf(cgiOut, "<BODY>");
		fprintf(cgiOut, "<H1>%s</H1>", "Server is busy...");		
		fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../index.html\">");
		return -1;
	}
//	printf("open user.db success!\n");
	sprintf(sql, "select * from usr where name='%s' and password='%s'", name, pw);
//	printf("sql = %s\n",sql);
	if(sqlite3_get_table(db, sql, &result, &row, &column, NULL) != SQLITE_OK)
	{
		fprintf(cgiOut, "<BODY>");
		fprintf(cgiOut, "<H1>%s</H1>", "Server is busy...");		
		fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../index.html\">");
		sqlite3_close(db);
		return -1;
	}
	if(row == 0)
	{
		fprintf(cgiOut, "<BODY>");
		fprintf(cgiOut, "<H1>%s</H1>", "Name or password error");		
		fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../index.html\">");
		sqlite3_close(db);
		return 0;
	}
	
	fprintf(cgiOut, "<BODY>");
	fprintf(cgiOut, "<H1>%s<Hi>","Login OK!");		
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../main.html\">");
	sqlite3_close(db);
	return 0;
}
