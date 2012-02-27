%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define TIME_MODE_ABSOLUTE			 0
#define TIME_MODE_RELATIVE			 1
#define	TIME_MODE_UNDEFINED			-1
#define DEF_LOG_END_TEXT			"***END DATE AND TIME ***\n"
#define	DEF_LOG_STOP_TEXT			"***[STOP LOGGING SESSION]***"


int nSize = 0;
int nLen = 0;
int nTimeMode = TIME_MODE_UNDEFINED;
int nNumLines = 0;
char data[256];
extern FILE *yyin, *yyout;

void yyerror(const char *str)
{
	//fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	fprintf(yyout, DEF_LOG_END_TEXT);
	fprintf(yyout, DEF_LOG_STOP_TEXT);
	return 1;	//1 Sepcifies conversion is over.
				//0 specifies the parser start the conversion from different
				//Input
}
int nInitialiseDataBytes()
{
	memset(data, 0, 256);
}
int nSecondToTime(unsigned int nSeconds, int *nHours, int *nMin, int *nSec)
{
	*nHours = nSeconds / 3600; 
	nSeconds = nSeconds -(*nHours * 3600);
	*nMin = nSeconds / 60; 
	*nSec = nSeconds - (*nMin) * 60;
}
int nGetLogTimeStamp(char* pchAscTime, char* pchLogTime)
{
	char chSeparators[] = ".";
	char* pchTemp;
	char chMilliSeconds[5] = {'\0'};
	int nHours, nMin, nSec;
	unsigned int unSeconds;
	unsigned int unMilliSeconds;
	
	pchTemp = strtok(pchAscTime, chSeparators);
	unSeconds = strtoul(pchTemp, NULL, 10);
	nSecondToTime(unSeconds, &nHours, &nMin, &nSec);
	
	pchTemp = strtok(NULL, chSeparators);
	
	strncpy( chMilliSeconds, pchTemp, 4);
	
	sprintf(pchLogTime, "%u:%u:%u:%s", nHours, nMin, nSec, chMilliSeconds);
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

%token HEXNUMBER NUMBER TOKHEAT STATE TOKTARGET TOKTEMPERATURE DOUBLEVAL MSGDIR DATAMSG LENGTHTOKEN BITCOUNTTOKEN EQUAL EXTID MONTH DAY FULLTIME
%token DATETOKEN REMOTE IGNORE TEXT COLON AM_PM BASETOKEN BASE TIMESTAMPSTOKEN TIMEMODE BEGINTRIGGERTOKEN LINEEND ENDTRIGGERTOKEN
%%

commands: /* empty */
	command | commands command
	;

command:
	error {yyerrok;yyclearin; } 
	|
	Standard_Msg
	|
	stmnt
	|
	Extended_Msg
	|
	RemoteFrame
	|
	Base_TimeStamps
	|
	Msg_Len
	|
	Log_Creation_Time
	|
	Bit_Count
	|
	Line_End
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

Standard_Msg:
	DOUBLEVAL NUMBER NUMBER MSGDIR NUMBER NUMBER Can_Data_Bytes
	{
		char chLogTime[256] = {'\0'};
		
		nGetLogTimeStamp((char*)$1, chLogTime);
		fprintf(yyout, "\n%s %s %s %s %s %s %s", chLogTime, $4, $2, $3, "s", $6, data);
		nLen = 0;
		nInitialiseDataBytes();
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);
		free($7);*/
	}

Extended_Msg:
	DOUBLEVAL NUMBER EXTID MSGDIR NUMBER NUMBER Can_Data_Bytes
	{
		char chLogTime[256] = {'\0'};
		char chId[12] = {'\0'};
		int nStrLen;
		nGetLogTimeStamp((char*)$1, chLogTime);
		strcpy(chId, $3);
		nStrLen = strlen(chId);
		chId[nStrLen-1] = '\0';
		fprintf(yyout, "\n%s %s %s %s %s %s %s", chLogTime, $4, $2, chId, "x", $6, data);
		nLen = 0;
		nInitialiseDataBytes();
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);*/
	}
RemoteFrame:
	DOUBLEVAL NUMBER NUMBER MSGDIR REMOTE
	{
		
		char chLogTime[256] = {'\0'};
		
		nGetLogTimeStamp((char*)$1, chLogTime);
		fprintf(yyout,"\n%s %s %s %s %s %s %s", chLogTime, $4, $2, $3, "sr", "0",  "00 00 00 00 00 00 00 00");
		nLen = 0;
		nInitialiseDataBytes();
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);*/
	}
	
Base_TimeStamps:
	BASETOKEN BASE  TIMESTAMPSTOKEN TIMEMODE
	{
		if(strcmp("dec", (char*)$2) == 0)
		{
			fprintf(yyout,"\n%s", "***DEC***");
		}
		else
		{
			fprintf(yyout,"\n%s", "***HEX***");
		}
		if(strcmp("relative", (char*)$4) == 0)
		{
			fprintf(yyout,"\n%s", "***RELATIVE MODE***");
			nTimeMode = TIME_MODE_RELATIVE;
		}
		else
		{
			fprintf(yyout,"\n%s", "***ABSOLUTE MODE***");
			nTimeMode = TIME_MODE_ABSOLUTE;
		}
		fprintf(yyout,"\n%s", "***<Time><Tx/Rx><Channel><CAN ID><Type><DLC><DataBytes>***");
		/*free($1);
		free($2);
		free($3);
		free($4);*/
	}
Log_Creation_Time:
//date Wed Dec 7 12:23:39 pm 2011
	DATETOKEN DAY MONTH NUMBER FULLTIME AM_PM NUMBER
	{	
		/*date Wed Dec 7 12:23:39 pm 2011*/
		
		
		char chSeparators[]   = " :,\t\n";
		char* chTemp;
		int nMonth;
		int nHour, nMins, nSec;
		
		if( strcmp("Jan", (char*)$3) == 0 )
			nMonth = 1;
		else if( strcmp("Feb", (char*)$3) == 0 )
			nMonth = 2; 
		else if( strcmp("Mar", (char*)$3) == 0 )
			nMonth = 3; 
		else if( strcmp("Apr", (char*)$3) == 0 )
			nMonth = 4; 
		else if( strcmp("May", (char*)$3) == 0 )
			nMonth = 5; 
		else if( strcmp("Jun", (char*)$3) == 0 )
			nMonth = 6; 
		else if( strcmp("Jul", (char*)$3) == 0 )
			nMonth = 7; 
		else if( strcmp("Aug", (char*)$3) == 0 )
			nMonth = 8; 
		else if( strcmp("Sep", (char*)$3) == 0 )
			nMonth = 9; 
		else if( strcmp("Oct", (char*)$3) == 0 )
			nMonth = 10; 
		else if( strcmp("Nov", (char*)$3) == 0 )
			nMonth = 11; 
		else if( strcmp("Dec", (char*)$3) == 0 )
			nMonth = 12; 
		/*8:12:2011 20:15:28:553****/
		
		chTemp = strtok((char*)$5, chSeparators);
		nHour = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nMins = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nSec = atoi(chTemp);
		
		nHour = nHour % 12;
		if( strcmp("pm", (char*)$6) == 0 )
		{
			nHour = nHour + 12;
		}
		fprintf(yyout,"%s:%d:%s %d:%d:%d:%s%s", $4, nMonth, $7, nHour, nMins, nSec, "000", "***");
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);
		free($7);*/
	}
Line_End:
	LINEEND
	{
		nNumLines++;
	}
stmnt: 
	error ';'