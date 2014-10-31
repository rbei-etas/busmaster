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

CScheduleCommands g_obScheduleCommand;
INT g_nLastInteger;
CParserHelper *g_obParserHelper = nullptr;
Ecu_Lin_Params g_ouLinEcuParams;
void yyerror(const char * /* str */)
{
	
}
extern "C" int yywrap()
{
	g_obParserHelper->CreateNetwork();
	return 1;
}
extern "C" int  nParseLDFFile(std::string strInputFile, CHANNEL_CONFIG& ouCluster)
{
	FILE *fpInputFile = fopen(strInputFile.c_str(), "r");
	if( (nullptr != fpInputFile) )
	{ 
		if ( g_obParserHelper == nullptr )
		{
			g_obParserHelper = new CParserHelper(ouCluster);
		}
		yyin = fpInputFile;
		yyout = nullptr;
		yyparse();

		if ( g_obParserHelper != nullptr )
		{
			delete g_obParserHelper;
			g_obParserHelper = nullptr;
		}

	}
	return 0;
}
%}



%token BEGINLOGTOKEN ENDLOGTOKEN DATETOKEN FULLTIME BASE TIMEMODE TIMEMODE TIMEMODE NUMBER DOUBLEVAL DATE TIME STDMSG EXTMSG STDRMSG EXTRMSG MSGDIR EQUAL 
%token LENGTHTOKEN BITCOUNTTOKEN LINEEND TIMESTAMPTOKEN ENDOFSTATEMENT KBPS
%token LIN_DESCRIPTION_FILE LIN_PROTOCOL_VERSION LIN_LANGUAGE_VERSION LIN_SPEED EQUAL SIGNAL_REPRESENTATION LOGICAL_VALUE
%token FLOATNUMBER CONSTANT SIGNALS DIAGNOSTIC_SIGNALS FRAMES SPORADIC_FRAMES EVENT_TRIGGERED_FRAMES DIAGNOSTIC_FRAMES SIGNAL_ENCODING_TYPES PHYSICAL_VALUE
%token NODE OPENBRACE CLOSEBRACE MASTER SLAVES MS SCHEDULE_TABLES DELAY ASSIGNFRAMEID ASSIGNFRAMEIDRANGE UNASSIGNFRAMEID ASSIGNNAD FREEFORMAT
%token CONDITIONALCHANGENAD DATADUMP SAVECONFIGURATION
%token NODE_ATTRIBUTES LIN_PROTOCOL CONFIGURED_NAD INITIAL_NAD PRODUCT_ID RESPONSE_ERROR FAULT_STATE_SIGNALS P2_MIN ST_MIN N_AS_TIMEOUT N_CR_TIMEOUT CONFIGURABLE_FRAMES
%token COLONE DECNUMBER HEXNUMBER STRING IDENTIFIER
%%

commands: /* empty */
	command | commands command
	;

real_or_integer:
	  DECNUMBER
	| HEXNUMBER
	| FLOATNUMBER
	;
INTEGER:
	  DECNUMBER
	| HEXNUMBER
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
	sporadic_frame_def
	|
	Event_Triggered_Frame_Def
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
	|
	Schedule_Table_Def
	|
	Node_Attribute
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
		g_obParserHelper->nOnProtocolVersion((char*)($3));
	}
LIN_language_version:
	LIN_LANGUAGE_VERSION EQUAL STRING ENDOFSTATEMENT
	{
	//fprintf(yyout, "Launguage version = %s\n", (char*)$3 );
	}	
LIN_Speed:
	LIN_SPEED EQUAL FLOATNUMBER KBPS ENDOFSTATEMENT
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
	MASTER COLONE IDENTIFIER  real_or_integer MS  real_or_integer MS ENDOFSTATEMENT
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
	IDENTIFIER COLONE INTEGER  INTEGER  IdentifierList ENDOFSTATEMENT
	{
		LdfSignal ouSignal;
		ouSignal.m_strName = (char*)$1;
		ouSignal.m_nLength = (int)$3;
		ouSignal.m_nDefVal = (int)$4;
		ouSignal.m_strTxEcuList = g_obParserHelper->m_strIdList;
		g_obParserHelper->m_strIdList.clear();
		g_obParserHelper->nOnSignal(ouSignal);
		//fprintf(yyout, "Signal Definitions %s\n", $1);
	}
Signal_Multiple:
	IDENTIFIER COLONE INTEGER OPENBRACE ArrayVal CLOSEBRACE IdentifierList 
	{
		LdfSignal ouSignal;
		ouSignal.m_strName = (char*)$1;
		ouSignal.m_nLength = (int)$3;
		ouSignal.m_nDefVal = g_obParserHelper->m_unSignalVal;
		ouSignal.m_strTxEcuList = g_obParserHelper->m_strIdList;
		g_obParserHelper->m_strIdList.clear();
		g_obParserHelper->m_unSignalVal = 0;
		g_obParserHelper->nOnSignal(ouSignal);
		//fprintf(yyout, "Signal Definitions %s\n", $1);
	}
ArrayVal:
	| ArrayVal FLOATNUMBER
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
	IDENTIFIER COLONE DECNUMBER IDENTIFIER DECNUMBER OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_FRAME_DEF);
		int nId = (int)$3;
		g_obParserHelper->OnFrameStarted((char*)$1, nId, (char*)$4, $5 );
		//fprintf(yyout, "Frame are %s\n", $1);
	}
Unconditional_Frame_Def_Hex:
	IDENTIFIER COLONE HEXNUMBER IDENTIFIER DECNUMBER OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_FRAME_DEF);
		int nId = $3;
		g_obParserHelper->OnFrameStarted((char*)$1, nId, (char*)$4, $5 );
		//fprintf(yyout, "Frame are %s\n", $1);
	}
Unconditional_Frame_Signal_Map:
	IDENTIFIER DECNUMBER ENDOFSTATEMENT
	{
		//Both For Diag and Unconditional Frame Signals
		g_obParserHelper->nAddSignaltoFrame((char*)$1,(int)$2);
		//fprintf(yyout, "Frame Signal are %s\n", $1);
	}

// Sporadic frames
sporadic_frame_def
	: /* empty */
    SPORADIC_FRAMES OPENBRACE SPORADIC_FRAME_LIST CLOSEBRACE
	{
		
	}
    

SPORADIC_FRAME_LIST	
    : SPORADIC_FRAME
    | SPORADIC_FRAME_LIST SPORADIC_FRAME
	;

SPORADIC_FRAME
	: IDENTIFIER COLONE IdentifierList ENDOFSTATEMENT
	{
		g_obParserHelper->OnSporadicFrame((char*)$1);
	}


Event_Triggered_Frame_Def
	: /* empty */
    | EVENT_TRIGGERED_FRAMES OPENBRACE EVENT_TRIGGERED_FRAME_LIST CLOSEBRACE
	{
	}
    ;

EVENT_TRIGGERED_FRAME_LIST	
    : EVENT_TRIGGERED_FRAME
    | EVENT_TRIGGERED_FRAME_LIST EVENT_TRIGGERED_FRAME
	{

	}

	;

// LIN 2.1: new parameter collision_resolving_schedule_table
EVENT_TRIGGERED_FRAME
	: IDENTIFIER COLONE IDENTIFIER INTEGER IdentifierList ENDOFSTATEMENT
	{
		g_obParserHelper->OnEventTriggeredFrame((char*)$1, (int)$4);
	}

	;


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
	IDENTIFIER COLONE DECNUMBER OPENBRACE
	{
		int nId = (int)$3;
		g_obParserHelper->OnFrameStarted((char*)$1, nId, "", 0 );
		//fprintf(yyout, "Diag Frame is %s\n", $1);
		g_obParserHelper->OnSectionStarted(SEC_DIAG_FRAME_DEF);
	}
Diagnostic_Frame_Start_HEX:
	IDENTIFIER COLONE HEXNUMBER OPENBRACE
	{
		int nId = (int)$3;
		g_obParserHelper->OnFrameStarted((char*)$1, nId, "", 0 );
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
	PHYSICAL_VALUE FLOATNUMBER FLOATNUMBER FLOATNUMBER FLOATNUMBER STRING
	{
		g_obParserHelper->nOnSignalEncoding(atoi((char*)$2),atoi((char*)$3),atof((char*)$4),atof((char*)$5),(char*)$6) ;
		//fprintf(yyout, "Signal Phyical Val%s\n", $1);
	}	
Signal_Logical_Val_dec:
	LOGICAL_VALUE FLOATNUMBER STRING
	{
		//fprintf(yyout, "Signal Logical Val%s\n", $1);
	}
Signal_Representation_Start:
	SIGNAL_REPRESENTATION OPENBRACE
	{
		g_obParserHelper->OnSectionStarted(SEC_SIGNAL_REP_DEC);
	}
Schedule_Table_Def: 
	SCHEDULE_TABLES OPENBRACE schedule_Table_List CLOSEBRACE
	{
		
	}

schedule_Table_List
	: schedule_table
	| schedule_Table_List schedule_table
	;

schedule_table: 
	IDENTIFIER OPENBRACE command_list CLOSEBRACE 
	{
		g_obParserHelper->nAddScheduleTable((char*)$1);
	}

	;
command_list: 
	  command_item
	| command_list command_item
	;

command_item:
	command DELAY INTEGER MS ENDOFSTATEMENT
	{
		g_obScheduleCommand.m_fDelay = $3;
		g_obParserHelper->nAddCommand(g_obScheduleCommand);
	}
	|
	command DELAY FLOATNUMBER MS ENDOFSTATEMENT
	{
		
	}
	;

command: 
	IDENTIFIER {                

		// LIN 2.1 parameter
	
		std::string strFrameName = (char*)$1;
		if ( S_OK == g_obParserHelper->nGetMessageTypeId(strFrameName, g_obScheduleCommand.m_nId, g_obScheduleCommand.m_eCommandType  ))
		{
			g_obScheduleCommand.m_strCommandName = (char*)$1;
			g_obScheduleCommand.m_strNodeName = "";
		}
	  }
	| AssignFrameId // obsolete in LIN 2.1
	| UnassignFrameId // obsolete in LIN 2.1
	| AssignNAD // changed parameter with LIN 2.1
	| FreeFormat
	| ConditionalChangeNAD // new with LIN 2.1
	| DataDump // new with LIN 2.1
	| SaveConfiguration // new with LIN 2.1
	| AssignFrameIdRange // new with LIN 2.1
	;

AssignFrameId: 
	ASSIGNFRAMEID OPENBRACE IDENTIFIER IDENTIFIER CLOSEBRACE
	{
		g_obScheduleCommand.m_eCommandType= COMMAND_ASSIGN_FRAME_ID;
		g_obScheduleCommand.m_strCommandName = m_omCommandType[COMMAND_ASSIGN_FRAME_ID];
		g_obScheduleCommand.m_strNodeName = (char*)$3;
		g_obScheduleCommand.m_nId = 0x3C;
	}
	;

UnassignFrameId: 
	UNASSIGNFRAMEID OPENBRACE IDENTIFIER IDENTIFIER CLOSEBRACE    
	{
		g_obScheduleCommand.m_eCommandType= COMMAND_UNASSIGN_FRAME_ID;
		g_obScheduleCommand.m_strCommandName = m_omCommandType[COMMAND_UNASSIGN_FRAME_ID];
		g_obScheduleCommand.m_strNodeName = (char*)$3;
		g_obScheduleCommand.m_nId = 0x3C;
	}
	;

// LIN 2.1: changed parameters for AssignNAD command
AssignNAD: 
	ASSIGNNAD OPENBRACE AssignNAD_parameter CLOSEBRACE
	{
		//ASSIGNNAD OPENBRACE AssignNAD_parameter CLOSEBRACE
		g_obScheduleCommand.m_eCommandType= COMMAND_ASSIGN_NAD;
		g_obScheduleCommand.m_strCommandName = m_omCommandType[COMMAND_ASSIGN_NAD];
	}
	;

AssignNAD_parameter:
	// LIN 2.0 parameter
	INTEGER INTEGER INTEGER INTEGER 
	{
		//INTEGER INTEGER INTEGER INTEGER 
		g_obScheduleCommand.m_listIDs[0] = (unsigned char)$1;
		g_obScheduleCommand.m_listIDs[1] = (unsigned char)$2;
		g_obScheduleCommand.m_listIDs[2] = (unsigned char)$3;
		g_obScheduleCommand.m_listIDs[3] = (unsigned char)$4;
		g_obScheduleCommand.m_nId = 0x3C;
		g_obScheduleCommand.m_strNodeName = "";
	}
	|
	IDENTIFIER
	{
		g_obScheduleCommand.m_strNodeName = (char*)$1;
		g_obScheduleCommand.m_nId = 0x3C;
	}
	;

FreeFormat: 
	FREEFORMAT OPENBRACE INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER CLOSEBRACE	
	{
		g_obScheduleCommand.m_eCommandType= COMMAND_FREE_FORMAT;
		g_obScheduleCommand.m_strCommandName = m_omCommandType[COMMAND_FREE_FORMAT];
		g_obScheduleCommand.m_strNodeName = "";
		

		g_obScheduleCommand.m_listIDs[0] = (unsigned char)$3;
		g_obScheduleCommand.m_listIDs[1] = (unsigned char)$4;
		g_obScheduleCommand.m_listIDs[2] = (unsigned char)$5;
		g_obScheduleCommand.m_listIDs[3] = (unsigned char)$6;
		g_obScheduleCommand.m_listIDs[4] = (unsigned char)$7;
		g_obScheduleCommand.m_listIDs[5] = (unsigned char)$8;
		g_obScheduleCommand.m_listIDs[6] = (unsigned char)$9;
		g_obScheduleCommand.m_listIDs[7] = (unsigned char)$10;
		g_obScheduleCommand.m_nId = 0x3C;
	}
	;
// new with LIN 2.1
ConditionalChangeNAD: 
	CONDITIONALCHANGENAD OPENBRACE INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER CLOSEBRACE
	{
		g_obScheduleCommand.m_eCommandType= COMMAND_CONDITIONAL_CHANGE_NAD;
		g_obScheduleCommand.m_strCommandName = m_omCommandType[COMMAND_CONDITIONAL_CHANGE_NAD];
		g_obScheduleCommand.m_strNodeName = "";
		
		g_obScheduleCommand.m_listIDs[0] = (unsigned char)$3;
		g_obScheduleCommand.m_listIDs[1] = (unsigned char)$4;
		g_obScheduleCommand.m_listIDs[2] = (unsigned char)$5;
		g_obScheduleCommand.m_listIDs[3] = (unsigned char)$6;
		g_obScheduleCommand.m_listIDs[4] = (unsigned char)$7;
		g_obScheduleCommand.m_listIDs[5] = (unsigned char)$8;
		g_obScheduleCommand.m_nId = 0x3C;
	}
	;

// new with LIN 2.1
DataDump: 
	DATADUMP OPENBRACE IDENTIFIER INTEGER INTEGER INTEGER INTEGER INTEGER CLOSEBRACE
	{
		g_obScheduleCommand.m_eCommandType= COMMAND_DATA_DUMP;
		g_obScheduleCommand.m_strCommandName = m_omCommandType[COMMAND_DATA_DUMP];
		g_obScheduleCommand.m_strNodeName = (char*)$3;
		g_obScheduleCommand.m_listIDs[0] = (unsigned char)$4;
		g_obScheduleCommand.m_listIDs[1] = (unsigned char)$5;
		g_obScheduleCommand.m_listIDs[2] = (unsigned char)$6;
		g_obScheduleCommand.m_listIDs[3] = (unsigned char)$7;
		g_obScheduleCommand.m_listIDs[4] = (unsigned char)$8;
		g_obScheduleCommand.m_nId = 0x3C;
	}

	;

// new with LIN 2.1
SaveConfiguration: 
	SAVECONFIGURATION OPENBRACE IDENTIFIER CLOSEBRACE
	{
		g_obScheduleCommand.m_eCommandType= COMMAND_SAVE_CONFIGURATION;
		g_obScheduleCommand.m_strCommandName = m_omCommandType[COMMAND_SAVE_CONFIGURATION];
		g_obScheduleCommand.m_strNodeName = (char*)$3;
		g_obScheduleCommand.m_nId = 0x3C;
	}
	;

// new with LIN 2.1
AssignFrameIdRange: 
	ASSIGNFRAMEIDRANGE OPENBRACE IDENTIFIER INTEGER AssignFrameIdRange_frame_PID_optional CLOSEBRACE
	{
		g_obScheduleCommand.m_eCommandType= COMMAND_ASSIGN_FRAME_ID_RANGE;
		g_obScheduleCommand.m_strCommandName = m_omCommandType[COMMAND_ASSIGN_FRAME_ID_RANGE];
		g_obScheduleCommand.m_strNodeName = (char*)$3;

		g_obScheduleCommand.m_listIDs[0] = (unsigned char)$4;
		g_obScheduleCommand.m_nId = 0x3C;
	}

	;

AssignFrameIdRange_frame_PID_optional: 
	/* empty */
	INTEGER INTEGER INTEGER INTEGER
	{
		g_obScheduleCommand.m_listIDs[0] = (unsigned char)$1;
		g_obScheduleCommand.m_listIDs[1] = (unsigned char)$2;
		g_obScheduleCommand.m_listIDs[2] = (unsigned char)$3;
		g_obScheduleCommand.m_listIDs[3] = (unsigned char)$4;
	}
	|
	{
	}

	//Optional
	;

/*
////////////// Node Attributess Definition /////////////
Node_attributes {
  Motor1{
	LIN_protocol = "2.1" ;
	configured_NAD = 0x4 ;
	initial_NAD = 0xF ;
	product_id = 0x1F, 0x3, 3 ;
	response_error = Motor1LinError ;
	fault_state_signals = Motor1ErrorValue, Motor1ErrorCode ;
	P2_min = 10 ms ;
	ST_min = 20 ms ;
	N_As_timeout = 10 ms ;
	N_Cr_timeout = 10 ms ;
	configurable_frames {
	  Frame1 ;
	  Frame2 ;
	  Frame3 ;
	  Frame4 ;
	  Frame5 ;
	}
  }
*/

Node_Attribute: 
	NODE_ATTRIBUTES OPENBRACE Node_Details_List CLOSEBRACE	
	{
		int nMine = 0;
	}
	;

Node_Details_List: 
	  Node_Detail
	| Node_Details_List Node_Detail
	{
		int n15 = 0;
	}
	;

Node_Detail: 
	IDENTIFIER OPENBRACE LIN_Protocol Configured_NAD Initial_NAD Product_Id Response_Error Fault_State_Signals P2_min ST_min N_As_timeout N_Cr_timeout configurable_frames CLOSEBRACE
	{
		g_obParserHelper->nUpdateEcuDetails((char*)$1, g_ouLinEcuParams);
	}
		;

LIN_Protocol: 
	LIN_PROTOCOL EQUAL STRING ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_strProtocolVer = (char*)$3;
	}
	;

Configured_NAD: 
	CONFIGURED_NAD EQUAL INTEGER ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_nConfiguredNAD = (int)$3;
	}
	;

// new optional parameter for LIN 2.1
Initial_NAD
	:
	| INITIAL_NAD EQUAL INTEGER ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_nInitialNAD = (int)$3;
	}
	;

Product_Id
	: 
	| PRODUCT_ID EQUAL INTEGER INTEGER Optional_Variant ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_unProductId[0] = (unsigned char)$3;	
		g_ouLinEcuParams.m_unProductId[1] = (unsigned char)$4;	
	}
	;

// parameter variant is optional with LIN 2.1
Optional_Variant
	:
	| INTEGER
	{
		g_ouLinEcuParams.m_unProductId[2] = (unsigned char)$1;
	}
	;

// parameter response_error is mandatory with LIN 2.1
Response_Error
	: 
	| RESPONSE_ERROR EQUAL IDENTIFIER ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_strResponseError = (char*)$3;
	}
	;

// new parameter with LIN 2.1
Fault_State_Signals
	: 
	| FAULT_STATE_SIGNALS EQUAL fault_state_signal_list ENDOFSTATEMENT
	{
		//This information Not required at this moment
	}
	;

fault_state_signal_list
	: IDENTIFIER 
	| fault_state_signal_list IDENTIFIER
	{
		//This information Not required at this moment
	}
	;

P2_min
	:
	| P2_MIN EQUAL FLOATNUMBER MS ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_p2Min = atof((char*)$3);
	}
	| P2_MIN EQUAL INTEGER MS ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_p2Min = (float)$3;
	}
	;

ST_min
	: 
	| ST_MIN EQUAL FLOATNUMBER MS ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_stMin = atof((char*)$3);
	}
	| ST_MIN EQUAL INTEGER MS ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_stMin = (float)$3;
	}
	;

// new parameter with LIN 2.1
N_As_timeout
	:
	| N_AS_TIMEOUT EQUAL FLOATNUMBER MS ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_nAsTimeout = atof((char*)$3);
	}
	| N_AS_TIMEOUT EQUAL INTEGER MS ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_nAsTimeout = (float)$3;
	}
	;

// new parameter with LIN 2.1
N_Cr_timeout
   : /* empty */
   | N_CR_TIMEOUT EQUAL FLOATNUMBER MS ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_nCrTimeout = atof((char*)$3);
	}
   | N_CR_TIMEOUT EQUAL INTEGER MS ENDOFSTATEMENT
	{
		g_ouLinEcuParams.m_nCrTimeout = (float)$3;
	}
   ;

// configurable_frames_def is mandatory in LIN 2.1
configurable_frames
	:
	| CONFIGURABLE_FRAMES OPENBRACE configurable_frame_list CLOSEBRACE
	{
		int n12 = 0;
	}
	;

configurable_frame_list
	: configurable_frame
	| configurable_frame_list configurable_frame
	;

configurable_frame
	: IDENTIFIER Message_Id ENDOFSTATEMENT
	{
		int n13 = 0;
	}
	;
IdentifierList:
	| IdentifierList IDENTIFIER
	{
		g_obParserHelper->m_strIdList.push_back((char*)$2);
		//fprintf(yyout, "Identifier is %s\n", $2);
	}
// message_id not used anymore in LIN 2.1, use order of frames in configurable_frame_list instead
Message_Id
	:
	| EQUAL INTEGER 

////////////// Node Attributess Definition Ends////////////
	;
