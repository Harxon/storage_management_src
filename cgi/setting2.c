#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include "cgic.h"
#include "sqlite3.h"
#define STORAGE_NO	'2'

#define N 64
#define SQLITE_WAREHOUSE "/dat/warehouse.db"

char receive_phone[12] = {0};
char center_phone[12] = {0};
struct msg
{
	long type;
	long msgtype;
	char text[N];
};

struct setEnv
{
	int temMAX; //10-50
	int temMIN; //10-50
	int humMAX; //10-50
	int humMIN; //10-50
	int illMAX; //10-500
	int illMIN; //10-500
};

int cgiMain()
{
	struct setEnv new; //环境变量临时结构体
	sqlite3 *db;
	char **result;
	char sql[N] = {0};
	int column, row;

	if (sqlite3_open(SQLITE_WAREHOUSE,&db) != SQLITE_OK)
	{
		fprintf(cgiOut, "<BODY>");
		fprintf(cgiOut, "<H1>%s</H1>", "Server is busy...");		
		return -1;
	}
	sprintf(sql, "select * from env where dev_no='%d'",STORAGE_NO-'0');

	if(sqlite3_get_table(db, sql, &result, &row, &column, NULL) != SQLITE_OK)
	{
		fprintf(cgiOut, "<BODY>");
		fprintf(cgiOut, "<H1>%s</H1>", "Server is busy...");		
		fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../setting1.html\">");
		sqlite3_close(db);
		return -1;
	}
	if (row == 1)
	{
		new.temMAX = atoi(result[column+1]);
//		printf("new.temMAX = %d\n",new.temMAX);
		new.temMIN = atoi(result[column+2]);
		new.humMAX = atoi(result[column+3]);
		new.humMIN = atoi(result[column+4]);
		new.illMAX = atoi(result[column+5]);
//		printf("new.illMAX = %d\n",new.illMAX);
		new.illMIN = atoi(result[column+6]);
	}

	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>高级设置</TITLE></HEAD>"); 
	fprintf(cgiOut, "<style type=\"text/css\"> body,td,th {font-size: 12px;margin-left: 0px;margin-top: 0px;}</style></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#F8F9FA\">"); 
	

	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" colspan=\"5\" align=\"left\">");
	fprintf(cgiOut, "<table width=\"100%%\" height=\"31\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" style=\"background-color:#e1e5ee;border-top-width:1px;border-right-width:1px;border-bottom-width:1px;border-left-width:1px;border-top-style:solid;border-top-color:#bfc4ca;border-right-color:#bfc4ca;border-bottom-color:#bfc4ca;border-left-color:#bfc4ca;\">");
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\" style=\"font-family:Arial,Helvetica,sans-serif;font-size:12px;line-height:25px;font-weight:bold;color:#333333\">&nbsp;&nbsp;&nbsp;&nbsp;环境参数设置</td>");
	fprintf(cgiOut, "</tr>");
	fprintf(cgiOut, "</table></td>");
	fprintf(cgiOut, "</tr>");//78 
#if 1
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"0\"><table width=\"100%%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">");
	fprintf(cgiOut, "<form name=\"set_2_m0_env\" id=\"set_2_m0_env\" method=\"post\" action=\"./set_2_m0_env.cgi\">");
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td width=\"20%%\" height=\"30\" align=\"left\" bgcolor=\"#f2f2f2\" class=\"left_txt2\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;温度上限：%d℃</td>", new.temMAX);
	fprintf(cgiOut, "<td width=\"3%%\" height=\"30\" bgcolor=\"#f2f2f2\">&nbsp;</td>");
	fprintf(cgiOut, "<td width=\"27%%\" height=\"30\" bgcolor=\"#f2f2f2\">");
	fprintf(cgiOut, "<input name=\"temMAX\" type=\"text\" id=\"temMAX\" value=\"%s\" size=\"30\" maxlength=\"4\" />℃</td>",result[column+1]);
	fprintf(cgiOut, "<td width=\"6%%\" height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td width=\"44%%\" height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");//89 TemperatureMax
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\" class=\"left_txt2\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;温度下限：%d℃</td>", new.temMIN);
	fprintf(cgiOut, "<td height=\"30\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\">");
	fprintf(cgiOut, "<input name=\"temMIN\" type=\"text\" value=\"%d\" size=\"30\" maxlength=\"4\" id=\"temMIN\" />℃</td>",new.temMIN);
	fprintf(cgiOut, "<td height=\"30\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");//97 TemMin
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\" bgcolor=\"#f2f2f2\" class=\"left_txt2\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;湿度上限：%d ％</td>", new.humMAX);
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#f2f2f2\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#f2f2f2\">");
	fprintf(cgiOut, "<input name=\"humMAX\" type=\"text\" value=\"%d\" size=\"30\" maxlength=\"4\" id=\"humMAX\" />％</td>",new.humMAX);
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");//105 humMAX
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\" class=\"left_txt2\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;湿度下限：%d ％</td>", new.humMIN);
	fprintf(cgiOut, "<td height=\"30\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\">");
	fprintf(cgiOut, "<input name=\"humMIN\" type=\"text\" value=\"%d\" size=\"30\" maxlength=\"4\" id=\"humMIN\" />％</td>",new.humMIN);
	fprintf(cgiOut, "<td height=\"30\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");// 113 humMIN
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\" bgcolor=\"#f2f2f2\" class=\"left_txt2\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;光度上限：%d lux</td>",new.illMAX);
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#f2f2f2\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#f2f2f2\">");
	fprintf(cgiOut, "<input name=\"illMAX\" type=\"text\" value=\"%d\" size=\"30\" maxlength=\"4\" id=\"illMAX\" />lux</td>",new.illMAX);
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");//121 illMAX
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\" bgcolor=\"#F7F8F9\" class=\"left_txt2\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;光度下限：%d lux</td>",new.illMIN);
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#F7F8F9\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#F7F8F9\">");
	fprintf(cgiOut, "<input name=\"illMIN\" type=\"text\" value=\"%d\" size=\"30\" maxlength=\"4\" id=\"illMIN\" />lux</td>", new.illMIN);
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#F7F8F9\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" bgcolor=\"#F7F8F9\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");//129 illMIN
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt2\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\">");                
	fprintf(cgiOut, "<input type=\"submit\" value=\"确认提交\" name=\"submit_set_env\" id=\"submit_set_env\"/></td>");
	fprintf(cgiOut, "<td height=\"30\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\">");
	fprintf(cgiOut, "<input type=\"reset\" value=\"取消设置\" name=\"reset_set_env\" id=\"reset_set_env\"/></td>");
	fprintf(cgiOut, "</tr>");//137 submit
	fprintf(cgiOut, "</form>");//  88 set_1_m0_env.cgi;

	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" colspan=\"5\" align=\"left\">");
	fprintf(cgiOut, "<table width=\"100%%\" height=\"31\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" style=\"background-color:#e1e5ee;border-top-width:1px;border-right-width:1px;border-bottom-width:1px;border-left-width:1px;border-top-style:solid;border-top-color:#bfc4ca;border-right-color:#bfc4ca;border-bottom-color:#bfc4ca;border-left-color:#bfc4ca;\">");
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\" style=\"font-family:Arial,Helvetica,sans-serif;font-size:12px;line-height:25px;font-weight:bold;color:#333333\">&nbsp;&nbsp;&nbsp;&nbsp;信息中心设置</td>");
	fprintf(cgiOut, "</tr>");
	fprintf(cgiOut, "</table></td>");
	fprintf(cgiOut, "</tr>");//78 

	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" colspan=\"5\" class=\"left_txt2\"><table width=\"100%%\" height=\"115\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");
	fprintf(cgiOut, "<form name=\"set_msg\" id=\"set_msg\" method=\"post\" action=\"./set_msg.cgi\">");

	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td width=\"20%%\" height=\"30\" align=\"left\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;报警电话：%s</td>",result[column+7]);
	fprintf(cgiOut, "<td width=\"3%%\" height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td width=\"27%%\" height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">");
	fprintf(cgiOut, "<input name=\"phonenum\" type=\"text\" value=\"%s\" size=\"30\" maxlength=\"11\" id=\"phonenum\" /></td>",result[column+7]);
	fprintf(cgiOut, "<td width=\"6%%\" height=\"30\" align=\"right\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td width=\"44%%\" height=\"30\" align=\"right\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");//161

	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\" class=\"left_txt\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;中心号码：%s</td>",result[column+12]);
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" class=\"left_txt\">");
	fprintf(cgiOut, "<input name=\"centernum\" type=\"text\" id=\"centernum\" value=\"%s\" size=\"30\" maxlength=\"11\" /></td>",result[column+12]);
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");//170 中心号码

	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\">");
	fprintf(cgiOut, "<input type=\"submit\" value=\"确认提交\" name=\"submit_set_msg\" id=\"submit_set_msg\"/></td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\">");
	fprintf(cgiOut, "<input type=\"reset\" value=\"取消设置\" name=\"reset_set_msg\" id=\"reset_set_msg\"/></td>");
	fprintf(cgiOut, "</tr>"); //179

	fprintf(cgiOut, "</form>");//159 form set_msg.cgi
#endif
#if 0 
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"31\" colspan=\"5\" align=\"right\" class=\"left_txt2\"><table width=\"100%%\" height=\"31\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"nowtable\">");
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\"  align=\"left\" class=\"left_bt2\" aclass=\"left_bt2\">&nbsp;&nbsp;&nbsp;&nbsp;无线网络设置</td>");
	fprintf(cgiOut, "</tr>");
	fprintf(cgiOut, "</table></td>");//192 table
	fprintf(cgiOut, "</tr>");// 191

	fprintf(cgiOut, "</table></td>");//158
	fprintf(cgiOut, "</tr>");//157
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" colspan=\"5\" class=\"left_txt2\"><table width=\"100%%\" height=\"85\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");
	fprintf(cgiOut, "<form name=\"set_wifi\" id=\"set_wifi\" method=\"post\" action=\"./set_wifi.cgi\">");
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td width=\"20%%\" height=\"30\" align=\"right\" bgcolor=\"#f2f2f2\" class=\"left_txt\">IP 设置：</td>");
	fprintf(cgiOut, "<td width=\"3%%\" height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td width=\"27%%\" height=\"30\" bgcolor=\"#f2f2f2\" class=\"left_txt\">");
	fprintf(cgiOut, "<input name=\"ipaddr\" type=\"text\" value=\"192.168.0.20\" size=\"30\" maxlength=\"15\" id=\"ipaddr\" /></td>");
	fprintf(cgiOut, "<td width=\"6%%\" height=\"30\" align=\"right\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td width=\"44%%\" height=\"30\" align=\"right\" bgcolor=\"#f2f2f2\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">网关设置：</td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" class=\"left_txt\">");
	fprintf(cgiOut, "<input name=\"gateway\" type=\"text\" value=\"192.168.0.1\" size=\"30\" maxlength=\"15\" id=\"gateway\" /></td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "</tr>");
	fprintf(cgiOut, "<tr>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, " <td height=\"30\" align=\"right\">");
	fprintf(cgiOut, "<input type=\"submit\" value=\"开启wifi\" name=\"submit_set_wifi\" id=\"submit_on_wifi\"/></td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"right\" class=\"left_txt\">&nbsp;</td>");
	fprintf(cgiOut, "<td height=\"30\" align=\"left\">");
	fprintf(cgiOut, "<input type=\"submit\" value=\"关闭wifi\" name=\"submit_set_wifi\" id=\"submit_off_wifi\"/></td>");
	fprintf(cgiOut, "</tr>");
	fprintf(cgiOut, "</form>"); //203 set_wifi.cgi
	fprintf(cgiOut, "</table></td>");
	fprintf(cgiOut, "</tr>");
#endif
	fprintf(cgiOut, "</table></td>");
	fprintf(cgiOut, "</tr>");
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);
	sqlite3_close(db);
	return 0; 
}
