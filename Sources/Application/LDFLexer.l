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

{DOUBLE_VAL}				yylval = _strdup(yytext);return FLOAT;
{DEC}						return DECNUMBER;
{HEX}						yylval = _strdup(yytext);return HEXNUMBER;

{STRINGLIT}					yylval = _strdup(yytext); return STRING;
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

"{"							return OPENBRACE;
"}"							return CLOSEBRACE;



{ID}						yylval = _strdup(yytext);return IDENTIFIER;
","							/**/
.							/**/
[\r]						/**/
[\n]						/**/
[ \t]+						/**/
{LINECOMMENT}	{;}
{BLOCKCOMMENT} {;}


%%
