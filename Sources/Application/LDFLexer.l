%{

#include <stdio.h>
#include <string.h>
#include "LDFParser.cpp.h"
%}
%option yylineno

DIGIT		[0-9]
ALPHA		[A-Fa-f]
VARIABLE	[_a-zA-Z][_a-zA-Z0-9]*
HEX		    0x[0-9a-fA-F]+
DEC			[-+]?[0-9]+
STRINGLIT   \"([^"\\]|(\\.))*\"
ID			[a-zA-Z_]([_a-zA-Z0-9]*)?
DOUBLE_VAL  [-+]?[0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)?
LINECOMMENT	"//".*
BLOCKCOMMENT "/*"([^\*]|\*[^*/])*"*/"

%%
"LIN_description_file"		return LIN_DESCRIPTION_FILE;
"LIN_protocol_version"		return LIN_PROTOCOL_VERSION;
"LIN_language_version"		return LIN_LANGUAGE_VERSION;
"LIN_speed"					return LIN_SPEED;
"="							return EQUAL;
"kbps"						return KBPS;
"Nodes"						return NODE;
"Master"					return MASTER;
"Slaves"					return SLAVES;
"Signals"					return SIGNALS;
"Diagnostic_signals"		return DIAGNOSTIC_SIGNALS;
"Sporadic_frames"			return SPORADIC_FRAMES;
"Event_triggered_frames"	return EVENT_TRIGGERED_FRAMES;
"Frames"					return FRAMES;
"Diagnostic_frames"			return DIAGNOSTIC_FRAMES;
"Signal_encoding_types"		return SIGNAL_ENCODING_TYPES;
"physical_value"			return PHYSICAL_VALUE;
"logical_value"				return LOGICAL_VALUE;
"Signal_representation"		return SIGNAL_REPRESENTATION;
":"							yylval = _strdup(yytext);return COLONE;
"ms"						return MS;
";"							return ENDOFSTATEMENT;

"Schedule_tables"			return SCHEDULE_TABLES;
"delay"						return DELAY;
"AssignFrameId"				return ASSIGNFRAMEID;
"AssignFrameIdRange"        return ASSIGNFRAMEIDRANGE; // 2.1
"UnassignFrameId"			return UNASSIGNFRAMEID;
"AssignNAD"					return ASSIGNNAD;
"FreeFormat"				return FREEFORMAT;
"ConditionalChangeNAD"      return CONDITIONALCHANGENAD; // 2.1
"DataDump"                  return DATADUMP; // 2.1
"SaveConfiguration"			return SAVECONFIGURATION; // 2.1
"Node_attributes"			return NODE_ATTRIBUTES;
"LIN_protocol"				return LIN_PROTOCOL;
"configured_NAD"			return CONFIGURED_NAD;
"initial_NAD"				return INITIAL_NAD; // new with LIN 2.1
"product_id"				return PRODUCT_ID;
"response_error"			return RESPONSE_ERROR;
"fault_state_signals"		return FAULT_STATE_SIGNALS;
"P2_min"					return P2_MIN;
"ST_min"					return ST_MIN;
"N_As_timeout"				return N_AS_TIMEOUT;
"N_Cr_timeout"				return N_CR_TIMEOUT;
"configurable_frames"		return CONFIGURABLE_FRAMES;



"{"							return OPENBRACE;
"}"							return CLOSEBRACE;
[\+\-]?[0-9]+										yylval = strtoul((char*)yytext, NULL, 10);return DECNUMBER; 
[\+\-]?(0x|0X)[0-9a-fA-F]+							yylval = strtoul((char*)yytext, NULL, 16);return HEXNUMBER; 
[\+\-]?([0-9]*\.{1}[0-9]+)([eE]{1}[\-\+]{1}[0-9]+)? yylval = _strdup(yytext);return FLOAT; 

{STRINGLIT}					yylval = _strdup(yytext); return STRING;
{ID}						yylval = _strdup(yytext);return IDENTIFIER;

","							/**/
.							/**/
[\r]						/**/
[\n]						/**/
[ \t]+						/**/
{LINECOMMENT}	{;}
{BLOCKCOMMENT} {;}


%%
