%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define TIME_MODE_ABSOLUTE			 0
#define TIME_MODE_RELATIVE			 1
#define TIME_MODE_SYSTEM			 2
#define	TIME_MODE_UNDEFINED			-1
#define DEF_LOG_END_TEXT			"***END DATE AND TIME ***\n"
#define	DEF_LOG_STOP_TEXT			"***[STOP LOGGING SESSION]***"
extern int nGetDayFromDate(char* pchDate, char *chMonth, char* chWeekDay);

int nSize = 0;
int nLen = 0;
int nTimeMode = TIME_MODE_UNDEFINED;
int nNumLines = 0;
char data[256];
int nNumModeProcd = 0;
int nTimeModeProcd = 0;
extern FILE *yyin, *yyout;

void yyerror(const char *str)
{
	//fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	fprintf(yyout, "End TriggerBlock\n");
	return 1;	//1 Sepcifies conversion is over.
				//0 specifies the parser start the conversion from different
				//Input
}
int nGetDate(char* pchDateString,  int* nDate, char* pchMonth, char* pchDay, int* nYear)
{
	char chTempDate[18] = {'\0'};
	char chTemp;
	int nMonth;
	strcpy(chTempDate, pchDateString);
	nGetDayFromDate(chTempDate, pchMonth, pchDay);
	sscanf(pchDateString, "%d%c%d%c%d", nDate, &chTemp, &nMonth, &chTemp, nYear);
	/*printf("%d, %d, %d\n", nDay, nMonth, *nYear);*/
	return 0;
}
int nGetTime( char* pchTimeString, int* nHour, int* nMin, int* nSec, char* pchAmPm)
{
	char chTemp;
	sscanf(pchTimeString, "%d%c%d%c%d", nHour, &chTemp, nMin, &chTemp, nSec);
	strcpy(pchAmPm, "am");
	if(*nHour > 12)
	{
		*nHour = *nHour - 12;
		strcpy(pchAmPm, "pm");
	}
	return 0;
}
int nSecondToTime(unsigned int nSeconds, int *nHours, int *nMin, int *nSec)
{
	*nHours = nSeconds / 3600; 
	nSeconds = nSeconds -(*nHours * 3600);
	*nMin = nSeconds / 60; 
	*nSec = nSeconds - (*nMin) * 60;
}
int nGetAscTimeStamp(char* pchLogTime, char* pchAscTime)
{
	char chSeparators[] = ".";
	char pchTemp;
	char chMilliSeconds[5] = {'\0'};
	int nHours, nMin, nSec;
	int nMicroSeconds;
	unsigned int unTotalSec;
	float fAscTime;
	if(pchAscTime == NULL || pchLogTime == NULL)
	{
		return -1;
	}
	printf("%s\n", pchLogTime);
	sscanf(pchLogTime, "%d%c%d%c%d%c%d", &nHours, &pchTemp, &nMin, &pchTemp, &nSec, &pchTemp, &nMicroSeconds);
	unTotalSec = nHours*3660 + nMin * 60 + nSec;
	printf("%d\n", nMicroSeconds);
	sprintf(pchAscTime, "%u.%06d", unTotalSec, nMicroSeconds*100);
	return 0;
}
int nConvertFile(FILE* fpInputFile, FILE* fpOutputFile)
{
	if( (NULL != fpInputFile) && (NULL != fpOutputFile) )
	{ 
		yyin = fpInputFile;
		yyout = fpOutputFile;
		yyparse();
	}
	return 0;
}
%}

%token BEGINLOGTOKEN ENDLOGTOKEN DATETOKEN FULLTIME BASE TIMEMODE TIMEMODE TIMEMODE NUMBER DOUBLEVAL DATE TIME STDMSG EXTMSG STDRMSG EXTRMSG MSGDIR EQUAL 
%token LENGTHTOKEN BITCOUNTTOKEN LINEEND COLON TIMESTAMPTOKEN
%%

commands: /* empty */
	command | commands command
	;

command:
	error {yyerrok;yyclearin; } 
	|
	Standard_Msg
	|
	Extended_Msg
	|
	Standard_R_Msg
	|
	Extended_R_Msg
	|
	stmnt
	|
	Msg_Len
	|
	Bit_Count
	|
	Creation_Time
	|
	Number_Mode
	|
	TimeStamp_Mode
	;

Can_Data_Bytes:
	| Can_Data_Bytes NUMBER
	{
		strcpy((data+nLen) , (char*)$2);
		nLen += strlen((char*)$2);
		data[nLen] = ' ';
		nLen++;
		data[nLen] = '\0';
	}
Msg_Len:
	LENGTHTOKEN EQUAL NUMBER
	{
		$$ = $3;
	}
Bit_Count:
	BITCOUNTTOKEN EQUAL NUMBER
	{
		$$ = $3;
	}
Creation_Time:
	DATETOKEN DATE TIME
	{
		//Fri Dec 9 11:58:31 am 2011
		//9:12:2011 11:58:31:000
		int nDate, nYear;
		int nHour, nMin, nSec;
		char pchAmPm[3] = {'\0'};
		char pchMonth[4] = {'\0'};
		char pchDay[4] = {'\0'};
		nGetDate((char*)$2, &nDate, pchMonth, pchDay, &nYear);
		nGetTime((char*)$3, &nHour, &nMin, &nSec, pchAmPm);
		//date Fri Dec 9 11:58:31 am 2011
		fprintf(yyout, "date %s %s %d %d:%d:%d %s %d\n", pchDay, pchMonth, nDate, nHour, nMin, nSec, pchAmPm, nYear);
	}
Number_Mode:
	BASE
	{
		if(nNumModeProcd == 0)
		{
			if(strcmp((char*)$1, "***HEX***") == 0)
			{
				fprintf(yyout, "base hex  ");
			}
			else
			{
				fprintf(yyout, "base dec  ");
			}
			nNumModeProcd = 1;
		}
	}
TimeStamp_Mode:
	TIMEMODE
	{
		if(nTimeModeProcd == 0)
		{
			if(strcmp((char*)$1 ,"***RELATIVE MODE***") == 0)
			{
				fprintf(yyout, "timestamps relative\n");
			}
			else if(strcmp((char*)$1 ,"***ABSOLUTE MODE***") == 0)
			{
				fprintf(yyout, "timestamps absolute\n");
			}
			fprintf(yyout, "no internal events logged\n");
			fprintf(yyout, "// version 7.1.0\n");
			nTimeModeProcd = 1;
		}
	}
Standard_Msg:
	TIME MSGDIR NUMBER NUMBER STDMSG NUMBER Can_Data_Bytes
	
	{
		/*0:0:0:0012 Tx 1 9 s 8 00 00 00 00 00 00 00 00 */
		char chAscTime[256] = {'\0'};
		nGetAscTimeStamp((char*)$1, chAscTime);
		/*0.001250 1  9               Tx   d 8 00 00 00 00 00 00 00 00*/
		fprintf(yyout, "%s %s %s %s d %s %s\n", chAscTime, $3, $4, $2, $6, data);
		nLen = 0;
		
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);
		free($7);*/
	}
Extended_Msg:
	TIME MSGDIR NUMBER NUMBER EXTMSG NUMBER Can_Data_Bytes
	{
		char chAscTime[256] = {'\0'};
		nGetAscTimeStamp((char*)$1, chAscTime);
		/*0.001250 1  9               Tx   d 8 00 00 00 00 00 00 00 00*/
		fprintf(yyout, "%s %s %sx %s d %s %s\n", chAscTime, $3, $4, $2, $6, data);
		nLen = 0;
	}
Standard_R_Msg:
	TIME MSGDIR NUMBER NUMBER STDRMSG NUMBER
	{
		/*<Time> <Channel> <ID> <Dir> r*/
		char chAscTime[256] = {'\0'};
		nGetAscTimeStamp((char*)$1, chAscTime);
		/*0.001250 1  9               Tx   d 8 00 00 00 00 00 00 00 00*/
		fprintf(yyout, "%s %s %s %s r\n", chAscTime, $3, $4, $2, $6);
	}
Extended_R_Msg:
	TIME MSGDIR NUMBER NUMBER STDRMSG NUMBER
	{
		/*<Time> <Channel> <ID> <Dir> r*/
		char chAscTime[256] = {'\0'};
		nGetAscTimeStamp((char*)$1, chAscTime);
		/*0.001250 1  9               Tx   d 8 00 00 00 00 00 00 00 00*/
		fprintf(yyout, "%s %s %s %s r\n", chAscTime, $3, $4, $2, $6);
	}
stmnt: 
	error ';'
