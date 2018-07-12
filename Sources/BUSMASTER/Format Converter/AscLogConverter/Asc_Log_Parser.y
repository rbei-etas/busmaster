%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define TIME_MODE_ABSOLUTE			 0
#define TIME_MODE_RELATIVE			 1
#define	TIME_MODE_UNDEFINED			-1

#define NUMERIC_MODE_DEC			0
#define NUMERIC_MODE_HEX			1
#define NUMERIC_MODE_NONE			-1

#define DEF_LOG_END_TEXT			"\n***END DATE AND TIME ***"
// PTV[1.6.4]
// Added new line
#define	DEF_LOG_STOP_TEXT			"\n***[STOP LOGGING SESSION]***\r\n"

#define	MAX_LOG_HEADER				400

int nLen = 0;
int nTimeMode = TIME_MODE_ABSOLUTE;
int nNumLines = 0;
int nNumericMode = NUMERIC_MODE_HEX;
char data[256];

int nDay = 1;
int nMonth = 1;
int nYear = 2000;
int nHour = 1;
int nMins = 1;
int nSec = 1;
int nHeaderFound = 0;
int nTimeStampFound = 0;
char gchErrorString[255] = {'\0'};



extern FILE *yyin, *yyout;

void yyerror(const char *str)
{
	//fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	// PTV[1.6.4]
	//fprintf(yyout, DEF_LOG_END_TEXT);
	//fprintf(yyout, DEF_LOG_STOP_TEXT);
	// PTV[1.6.4]
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
	int nLen = 0;
	int i = 0;
	int nMax = 0;
	char chMilliSeconds[5] = {'0'};
	
	int nHours, nMin, nSec;
	unsigned int unSeconds;
	double fMilli = 0;
	chMilliSeconds[4] = '\0';
	memset(chMilliSeconds, 0x30, 4);
	pchTemp = strtok(pchAscTime, chSeparators);
	unSeconds = strtoul(pchTemp, NULL, 10);
	nSecondToTime(unSeconds, &nHours, &nMin, &nSec);
	
	pchTemp = strtok(NULL, chSeparators);
	nLen = strlen(pchTemp);
	nMax = min(nLen, 4);
	for ( i=0; i < nMax ; i++ )
	{
		chMilliSeconds[i] = pchTemp[i];
	}
	
	sprintf(pchLogTime, "%u:%u:%u:%s", nHours, nMin, nSec, chMilliSeconds);
}
int nInitialiseDefaultValues()
{
	nLen = 0;
	nTimeMode = TIME_MODE_ABSOLUTE;
	nNumLines = 0;
	nNumericMode = NUMERIC_MODE_HEX;
	nDay = 1;
	nMonth = 1;
	nYear = 2000;
	nHour = 1;
	nMins = 1;
	nSec = 1;
	nHeaderFound = 0;
	nTimeStampFound = 0;
	strcpy(gchErrorString, "Conversion Completed Successfully");
	
	nInitialiseDataBytes();
}
int nConvertFile(FILE* fpInputFile, FILE* fpOutputFile)
{
int i =0;
	if( (NULL != fpInputFile) && (NULL != fpOutputFile) )
	{ 
		yyin = fpInputFile;
		yyout = fpOutputFile;

		nInitialiseDefaultValues();
		
		//Add Dummy Header with initial values for log file.This will be replaced with the proper values
		//afrter conversion.
		for( i = 0 ; i < MAX_LOG_HEADER; i++ )
		{
			fprintf(yyout, " ");
		}

		yyparse();
		
		//Actual Header
		nAddFunctionHeader();

		if ( nTimeStampFound == 0 )
		{
			//TODO::
			strcpy(gchErrorString, "Proper Header was not found in ASC File.Logging Mode is Assumed as Absolute & Numeric Format Assumed as Hex");
		}


	}
	return 0;
}
int nAddFunctionHeader()
{
	fseek(yyout, 0L, SEEK_SET );

	fprintf(yyout, "***BUSMASTER Ver %s***\n", BUSMASTER_VER);
	fprintf(yyout, "***PROTOCOL CAN***\n");
	fprintf(yyout, "***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***\n");
	fprintf(yyout, "***[START LOGGING SESSION]***\n");
	fprintf(yyout,"***START DATE AND TIME %d:%d:%d %d:%d:%d:%s%s", nDay, nMonth, nYear, nHour, nMins, nSec, "000", "***\n");
		
	
	
	if ( NUMERIC_MODE_DEC == nNumericMode )
	{
		fprintf(yyout, "***DEC***\n");
	}
	else
	{	
		fprintf(yyout, "***HEX***\n");
	}

	if ( nTimeMode == TIME_MODE_RELATIVE )
	{
		fprintf(yyout, "***RELATIVE MODE***\n");
	}
	else
	{
		fprintf(yyout, "***ABSOLUTE MODE***\n");
	}
	fprintf(yyout,"***START DATABASE FILES***\n");

	fprintf(yyout,"***END DATABASE FILES***\n");

	fprintf(yyout,"***<Time><Tx/Rx><Channel><CAN ID><Type><DLC><DataBytes>***");

    return 0;
}
int GetMonth(char* pchValue)
{
	int nMonth = 1;
	if( strcmp("Jan", pchValue) == 0 )
			nMonth = 1;
	else if( strcmp("Feb", pchValue) == 0 )
		nMonth = 2; 
	else if( strcmp("Mar", pchValue) == 0 )
		nMonth = 3; 
	else if( strcmp("Apr", pchValue) == 0 )
		nMonth = 4; 
	else if( strcmp("May", pchValue) == 0 )
		nMonth = 5; 
	else if( strcmp("Jun", pchValue) == 0 )
		nMonth = 6; 
	else if( strcmp("Jul", pchValue) == 0 )
		nMonth = 7; 
	else if( strcmp("Aug", pchValue) == 0 )
		nMonth = 8; 
	else if( strcmp("Sep", pchValue) == 0 )
		nMonth = 9; 
	else if( strcmp("Oct", pchValue) == 0 )
		nMonth = 10; 
	else if( strcmp("Nov", pchValue) == 0 )
		nMonth = 11; 
	else if( strcmp("Dec", pchValue) == 0 )
		nMonth = 12; 
	return nMonth;
}
%}

%token HEXNUMBER NUMBER TOKHEAT STATE TOKTARGET TOKTEMPERATURE DOUBLEVAL MSGDIR DATAMSG LENGTHTOKEN BITCOUNTTOKEN EQUAL EXTID MONTH_BASE DAY FULLTIME
%token DATETOKEN REMOTE IGNORE TEXT COLON AM_PM BASETOKEN BASE NO_INTERNAL_EVENTS_LOGGED_TOKEN TIMESTAMPSTOKEN TIMEMODE BEGINTRIGGERTOKEN LINEEND ENDTRIGGERTOKEN
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
	ExtendedRTR_Msg
	|
	RemoteFrame
	|
	Base_TimeStamps
	|
	Base_No_Internal_Events
	|// PTV[1.6.4]
	End_Statement
	// PTV[1.6.4]
	|
	Msg_Len
	|
	Log_Creation_Time
	|
	Log_Creation
	|
	Bit_Count
	|
	Line_End
	;
	
Can_Data_Bytes:
	| Can_Data_Bytes NUMBER
	{
		int nStrlen = strlen((char*)$2);
		int i=0;
		char o = '0';
		//prefix zeros to get 3-digits per byte.
		if(nNumericMode == NUMERIC_MODE_DEC)
		{
			if(nStrlen < 3)
			{
				for(i=0 ; i<(3-nStrlen); i++)
				{
					strcpy((data+nLen) ,&o);
					nLen += 1;
				}
			}
		}
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
		char chId[12] = {'\0'};

		nGetLogTimeStamp((char*)$1, chLogTime);
		
		if (NUMERIC_MODE_DEC == nNumericMode )
		{
			strcpy(chId,$3);	
		}
		else
		{
			strcpy(chId,"0x");
			strcat(chId, $3);
		}
		fprintf(yyout, "\n%s %s %s %s %s %s %s", chLogTime, $4, $2, chId, "s", $6, data);
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
		nGetLogTimeStamp((char*)$1, chLogTime);
		if (NUMERIC_MODE_DEC == nNumericMode )
		{
			strcpy(chId,$3);	
		}
		else
		{
			strcpy(chId,"0x");
			strcat(chId, $3);
		}

		
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
ExtendedRTR_Msg:
	DOUBLEVAL NUMBER EXTID MSGDIR REMOTE
	{
		
		char chLogTime[256] = {'\0'};
		char chId[12] = {'\0'};		
		nGetLogTimeStamp((char*)$1, chLogTime);



		if (NUMERIC_MODE_DEC == nNumericMode )
		{
			strcpy(chId,$3);	
		}
		else
		{
			strcpy(chId,"0x");
			strcat(chId, $3);
		}
		
		fprintf(yyout,"\n%s %s %s %s %s %s %s", chLogTime, $4, $2, chId, "xr", "0",  "");
		nLen = 0;
		nInitialiseDataBytes();
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);*/
	}
RemoteFrame:
	DOUBLEVAL NUMBER NUMBER MSGDIR REMOTE
	{
		
		char chLogTime[256] = {'\0'};
		char chId[12] = {'\0'};		
		nGetLogTimeStamp((char*)$1, chLogTime);



		if (NUMERIC_MODE_DEC == nNumericMode )
		{
			strcpy(chId,$3);	
		}
		else
		{
			strcpy(chId,"0x");
			strcat(chId, $3);
		}
		
		fprintf(yyout,"\n%s %s %s %s %s %s %s", chLogTime, $4, $2, chId, "sr", "0",  "");
		nLen = 0;
		nInitialiseDataBytes();
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);*/
	}
	
Base_TimeStamps:
	BASETOKEN MONTH_BASE TIMESTAMPSTOKEN TIMEMODE
	{
		//Default
		nTimeStampFound = 1;
		if(_stricmp ("dec", (char*)$2) == 0)
		{
			nNumericMode = NUMERIC_MODE_DEC;
			
		}
		else
		{
			nNumericMode = NUMERIC_MODE_HEX;
		}
		if(strcmp("relative", (char*)$4) == 0)
		{
			nTimeMode = TIME_MODE_RELATIVE;
		}
		else
		{
			nTimeMode = TIME_MODE_ABSOLUTE;
		}
		
		/*free($1);
		free($2);
		free($3);
		free($4);*/
	}
Base_No_Internal_Events:
	BASETOKEN MONTH_BASE NO_INTERNAL_EVENTS_LOGGED_TOKEN 
	{
		//Default
		nTimeStampFound = 1;
		if(_stricmp ("dec", (char*)$2) == 0)
		{
			nNumericMode = NUMERIC_MODE_DEC;
		}
		else
		{
			nNumericMode = NUMERIC_MODE_HEX;
		}
		nTimeMode = TIME_MODE_ABSOLUTE;
		free($1);
		free($2);
		free($3);
	}
Log_Creation_Time:
//date Wed Dec 7 12:23:39 pm 2011
//The Functionality has to move to single function
	DATETOKEN DAY MONTH_BASE NUMBER FULLTIME AM_PM NUMBER
	{	
		/*date Wed Dec 7 12:23:39 pm 2011*/
		/*8:12:2011 20:15:28:553****/

		
		char chSeparators[]   = " :,\t\n";
		char* chTemp;
		
		
		nMonth = GetMonth((char*)$3);
		nHeaderFound = 1;
		
		
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
		
		nDay = atoi( (char*)$4 );
		nYear = atoi( (char*)$7 );

		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);
		free($7);*/
	}
Log_Creation:
	//date Wed Dec 7 12:23:39 2011.
	DATETOKEN DAY MONTH_BASE NUMBER FULLTIME NUMBER
	{	
		/*date Wed Dec 7 12:23:39 pm 2011*/
		
		char chSeparators[]   = " :,\t\n";
		char* chTemp;
		nMonth = GetMonth((char*)$3);
		/*8:12:2011 20:15:28:553****/
		
		chTemp = strtok((char*)$5, chSeparators);
		nHour = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nMins = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nSec = atoi(chTemp);
		
		nDay = atoi( (char*)$4 );
		nYear = atoi( (char*)$6 );

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
	// PTV[1.6.4]
	// Added new entry for End Trigger Token
End_Statement:
	ENDTRIGGERTOKEN
	{
	fprintf(yyout, DEF_LOG_END_TEXT);
	fprintf(yyout, DEF_LOG_STOP_TEXT);
	}
	// PTV[1.6.4]
stmnt: 
	error ';'