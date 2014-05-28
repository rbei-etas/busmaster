%{
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "ParserHelper.h"
extern "C" FILE *yyin, *yyout;
extern "C" int yylval;
extern "C" int yylex();
extern "C" int yyparse(void);
extern "C" char* yytext;

CParserHelper *g_obParserHelper = NULL;
void yyerror(const char *str)
{
	
	//fprintf(stderr,"error: %s\n", yytext);
}
extern "C" int yywrap()
{
	g_obParserHelper->CreateNetwork();
	return 1;
}
extern "C" int  nParseLDFFile(std::string strInputFile, CHANNEL_CONFIG& ouCluster)
{
	FILE *fpInputFile = fopen(strInputFile.c_str(), "r");
	if( (NULL != fpInputFile) )
	{ 
		if ( g_obParserHelper == NULL )
		{
			g_obParserHelper = new CParserHelper(ouCluster);
		}
		yyin = fpInputFile;
		yyout = NULL;
		yyparse();

		if ( g_obParserHelper != NULL )
		{
			delete g_obParserHelper;
			g_obParserHelper = NULL;
		}

	}
	return 0;
}
%}


%token BEGINLOGTOKEN ENDLOGTOKEN DATETOKEN FULLTIME BASE TIMEMODE TIMEMODE TIMEMODE NUMBER DOUBLEVAL DATE TIME STDMSG EXTMSG STDRMSG EXTRMSG MSGDIR EQUAL 
%token LENGTHTOKEN BITCOUNTTOKEN LINEEND TIMESTAMPTOKEN STRING ENDOFSTATEMENT KBPS IDENTIFIER DECNUMBER HEXNUMBER
%token LIN_DESCRIPTION_FILE LIN_PROTOCOL_VERSION LIN_LANGUAGE_VERSION LIN_SPEED EQUAL SIGNAL_REPRESENTATION LOGICAL_VALUE
%token FLOAT CONSTANT SIGNALS DIAGNOSTIC_SIGNALS FRAMES SPORADIC_FRAMES EVENT_TRIGGERED_FRAMES DIAGNOSTIC_FRAMES SIGNAL_ENCODING_TYPES PHYSICAL_VALUE
%token NODE OPENBRACE CLOSEBRACE MASTER SLAVES COLONE  MS
%%

commands: /* empty */
	command | commands command
	;

command:
	error {yyerrok;yyclearin; } 
	|
	Section_End
	|
	LinDescriptionFile
	|
	LIN_protocol_version
	|
	LIN_language_version
	|
	LIN_Speed
	|
	NodeDecleration
	|
	Master_Decleration
	|
	Slaves_Deleration
	|
	Signal_Start
	|
	Signal_Def
	|
	DigSignal_Start
	|
	Unconditional_Frame_Start
	|
	Unconditional_Frame_Def
	|
	Unconditional_Frame_Signal_Map
	|
	Sporadic_Frame_Start
	|
	Sporadic_Frames_Dec
	|
	Event_Triggered_Frames
	|
	Event_Trigger_Frame_Start
	|
	Diagnostic_Frames_Dec
	|
	Diagnostic_Frame_Start
	|
	Section_Start
	|
	Signal_Encodeing_Start
	|
	Signal_Physical_Val_dec
	|
	Signal_Logical_Val_dec
	|
	Signal_Representation_Start
	;

Section_End:
	CLOSEBRACE
	{
		g_obParserHelper->OnSectionClosed();
	}
LinDescriptionFile:
	LinDescriptionEmpty
	|
	LinDescription
	;
LinDescriptionEmpty:
	LIN_DESCRIPTION_FILE ENDOFSTATEMENT
	{
	//fprintf(yyout, "Description = null\n");
	}
LinDescription:
	LIN_DESCRIPTION_FILE EQUAL STRING ENDOFSTATEMENT
	{
	//fprintf(yyout, "Description = %s\n", (char*)$3);
	}
LIN_protocol_version:
	LIN_PROTOCOL_VERSION EQUAL STRING ENDOFSTATEMENT
	{
		g_obParserHelper->nOnProtocolVersion((char*)$3);
	}
LIN_language_version:
	LIN_LANGUAGE_VERSION EQUAL STRING ENDOFSTATEMENT
	{
	//fprintf(yyout, "Launguage version = %s\n", (char*)$3 );
	}	
LIN_Speed:
	LIN_SPEED EQUAL FLOAT KBPS ENDOFSTATEMENT
	{
		g_obParserHelper->nOnLinBaudRate((char*)$3);
	}	
NodeDecleration:
	NODE OPENBRACE
	{
		//fprintf(yyout, "Node Dec\n", (char*)$1 );
		g_obParserHelper->OnSectionStarted(SEC_NODE_DEC); 
	}
Master_Decleration:
	MASTER COLONE IDENTIFIER  FLOAT MS  FLOAT MS ENDOFSTATEMENT
	{
		g_obParserHelper->m_strMasterEcu = (char*)$3;
		//fprintf(yyout, "Master is %s\n", $3);
	}
Slaves_Deleration:
	SLAVES COLONE IdentifierList ENDOFSTATEMENT
	{
		g_obParserHelper->m_strSlaveEcuList = g_obParserHelper->m_strIdList;
		g_obParserHelper->m_strIdList.clear();
		//fprintf(yyout, "Slaves is %s\n", $1);
	}
IdentifierList:
	| IdentifierList IDENTIFIER
	{
		g_obParserHelper->m_strIdList.push_back((char*)$2);
		//fprintf(yyout, "Identifier is %s\n", $2);
	}
Signal_Start:
	SIGNALS OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_SIGNAL_DEC);
		//fprintf(yyout, "Signal is %s\n", $1);
	}
Signal_Def:	//Works for Normal signal and Diag signals
	Signal_Single
	|
	Signal_Multiple
	;
Signal_Single:
	IDENTIFIER COLONE FLOAT  FLOAT  IdentifierList ENDOFSTATEMENT
	{
		LdfSignal ouSignal;
		ouSignal.m_strName = (char*)$1;
		ouSignal.m_nLength = atoi((char*)$3);
		ouSignal.m_nDefVal = atoi((char*)$4);
		ouSignal.m_strTxEcuList = g_obParserHelper->m_strIdList;
		g_obParserHelper->m_strIdList.clear();
		g_obParserHelper->nOnSignal(ouSignal);
		//fprintf(yyout, "Signal Definitions %s\n", $1);
	}
Signal_Multiple:
	IDENTIFIER COLONE FLOAT OPENBRACE ArrayVal CLOSEBRACE IdentifierList 
	{
		LdfSignal ouSignal;
		ouSignal.m_strName = (char*)$1;
		ouSignal.m_nLength = atoi((char*)$3);
		ouSignal.m_nDefVal = g_obParserHelper->m_unSignalVal;
		ouSignal.m_strTxEcuList = g_obParserHelper->m_strIdList;
		g_obParserHelper->m_strIdList.clear();
		g_obParserHelper->m_unSignalVal = 0;
		g_obParserHelper->nOnSignal(ouSignal);
		//fprintf(yyout, "Signal Definitions %s\n", $1);
	}
ArrayVal:
	| ArrayVal FLOAT
	{
		g_obParserHelper->m_unSignalVal = g_obParserHelper->m_unSignalVal * 100;
		int nVal =  atoi((char*)$2);
		g_obParserHelper->m_unSignalVal = g_obParserHelper->m_unSignalVal + nVal ;
		int  i = 0;
	} 

DigSignal_Start:
	DIAGNOSTIC_SIGNALS OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_DIAG_SIGNAL_DEC);
		//fprintf(yyout, "Dig signals %s\n", $1);
	}
Unconditional_Frame_Start:
	FRAMES OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_FRAME_DEC);
		//fprintf(yyout, "Frame Start %s\n", $1);
	}
Unconditional_Frame_Def:
	Unconditional_Frame_Def_Int
	|
	Unconditional_Frame_Def_Hex
	;
Unconditional_Frame_Def_Int:
	IDENTIFIER COLONE FLOAT IDENTIFIER FLOAT OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_FRAME_DEF);
		int nId = strtoul((char*)$3, NULL, 10);
		g_obParserHelper->OnFrameStarted((char*)$1, nId, (char*)$4, (char*)$5 );
		//fprintf(yyout, "Frame are %s\n", $1);
	}
Unconditional_Frame_Def_Hex:
	IDENTIFIER COLONE HEXNUMBER IDENTIFIER FLOAT OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_FRAME_DEF);
		int nId = strtoul((char*)$3, NULL, 16);
		g_obParserHelper->OnFrameStarted((char*)$1, nId, (char*)$4, (char*)$5 );
		//fprintf(yyout, "Frame are %s\n", $1);
	}
Unconditional_Frame_Signal_Map:
	IDENTIFIER FLOAT ENDOFSTATEMENT
	{
		//Both For Diag and Unconditional Frame Signals
		g_obParserHelper->nAddSignaltoFrame((char*)$1,atoi((char*)$2));
		//fprintf(yyout, "Frame Signal are %s\n", $1);
	}

Sporadic_Frame_Start:
	SPORADIC_FRAMES OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_SPORADIC_FRAME_DEC);
	}
Sporadic_Frames_Dec:
	Sporadic_Frames_Dec_List
	|
	Sporadic_Frames_Dec_Sinlge
	;
Sporadic_Frames_Dec_List:
	IDENTIFIER COLONE IdentifierList ENDOFSTATEMENT
	{
		g_obParserHelper->OnSporadicOrCompuType((CHAR*)$1);
		g_obParserHelper->m_strIdList.clear();
		//Works for Signal Representation also
		//fprintf(yyout, "Sporadic_Frames Signal are %s\n", $1);
	}
Sporadic_Frames_Dec_Sinlge:
	IDENTIFIER COLONE IDENTIFIER ENDOFSTATEMENT
	{
		g_obParserHelper->m_strIdList.push_back((char*)$3);
		g_obParserHelper->OnSporadicOrCompuType((CHAR*)$1);
		g_obParserHelper->m_strIdList.clear();
		//Works for Signal Representation also
		//fprintf(yyout, "Sporadic_Frames Signal are %s\n", $1);
	}
Event_Triggered_Frames:
	EVENT_TRIGGERED_FRAMES OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_EVENT_TRIGGER_FRAME_DEC);
	}
Event_Trigger_Frame_Start:
	IDENTIFIER COLONE IDENTIFIER FLOAT IdentifierList
	{
		//fprintf(yyout, "Event_Trigger_Frames are %s\n", $1);
	}

Diagnostic_Frames_Dec:
	DIAGNOSTIC_FRAMES OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_DIAG_FRAME_DEC);
	}
Diagnostic_Frame_Start:
	Diagnostic_Frame_Start_DEC
	|
	Diagnostic_Frame_Start_HEX
	;
Diagnostic_Frame_Start_DEC:
	IDENTIFIER COLONE FLOAT OPENBRACE
	{
		int nId = strtoul((char*)$3, NULL, 10);
		g_obParserHelper->OnFrameStarted((char*)$1, nId, "", "" );
		//fprintf(yyout, "Diag Frame is %s\n", $1);
		g_obParserHelper->OnSectionStarted(SEC_DIAG_FRAME_DEF);
	}
Diagnostic_Frame_Start_HEX:
	IDENTIFIER COLONE HEXNUMBER OPENBRACE
	{
		int nId = strtoul((char*)$3, NULL, 16);
		g_obParserHelper->OnFrameStarted((char*)$1, nId, "", "" );
		//fprintf(yyout, "Diag Frame is %s\n", $1);
		g_obParserHelper->OnSectionStarted(SEC_DIAG_FRAME_DEF);
	}
Section_Start:
	IDENTIFIER OPENBRACE
	{
		//Node Attributes, Signal Encoding
		g_obParserHelper->m_strLastId = (char*)$1;
		//fprintf(yyout, "Section Starts %s\n", $1);
	}
Signal_Encodeing_Start:
	SIGNAL_ENCODING_TYPES OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_SIGNAL_ENCODING_DEC);
	}
Signal_Physical_Val_dec:
	PHYSICAL_VALUE FLOAT FLOAT FLOAT FLOAT STRING
	{
		g_obParserHelper->nOnSignalEncoding(atoi((char*)$2),atoi((char*)$3),atof((char*)$4),atof((char*)$5),(char*)$6) ;
		//fprintf(yyout, "Signal Phyical Val%s\n", $1);
	}	
Signal_Logical_Val_dec:
	LOGICAL_VALUE FLOAT STRING
	{
		//fprintf(yyout, "Signal Logical Val%s\n", $1);
	}
Signal_Representation_Start:
	SIGNAL_REPRESENTATION OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_SIGNAL_REP_DEC);
	}
;
