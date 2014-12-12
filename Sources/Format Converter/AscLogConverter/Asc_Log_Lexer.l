%{

#include <stdio.h>
#include <string.h>
#include "Asc_Log_Parser.h"
%}
DIGIT		[0-9]
ALPHA		[A-Fa-f]
HEXTAIL		({DIGIT}|{ALPHA})+
HEX			{HEXTAIL}
%%
"Begin Triggerblock"		yylval = (int)_strdup(yytext);return BEGINTRIGGERTOKEN;
"End Triggerblock"			yylval = (int)_strdup(yytext);return ENDTRIGGERTOKEN;
"date"						yylval = (int)_strdup(yytext);return DATETOKEN;
("Jan"|"Feb"|"Mar"|"Apr") 	yylval = (int)_strdup(yytext); return MONTH_BASE;
("May"|"Jun"|"Jul"|"Aug")  	yylval = (int)_strdup(yytext); return MONTH_BASE; 
("Sep"|"Oct"|"Nov"|"Dec"|"hex") 	yylval = (int)_strdup(yytext); return MONTH_BASE;
("Sat"|"Sun"|"Mon"|"Tue") 	yylval = (int)_strdup(yytext); return DAY;
("Wed"|"Thu"|"Fri"|"Fre")	yylval = (int)_strdup(yytext); return DAY;
[0-9]+\:[0-9]+\:[0-9]+		yylval = (int)_strdup(yytext);return FULLTIME;
"base" 						yylval = (int)_strdup(yytext);return BASETOKEN;
"no internal events logged" yylval = (int)_strdup(yytext);return NO_INTERNAL_EVENTS_LOGGED_TOKEN;
"timestamps"				yylval = (int)_strdup(yytext);return TIMESTAMPSTOKEN;
("absolute"|"relative") 	yylval = (int)_strdup(yytext);return TIMEMODE;
("am"|"pm")					yylval = (int)_strdup(yytext); return AM_PM;
{HEX}						yylval =(int)_strdup(yytext);return NUMBER;
[0-9]+\.?[0-9]*				yylval =(int)_strdup(yytext);return DOUBLEVAL;
{HEXTAIL}x							yylval =(int)_strdup(yytext);return EXTID;
"r"							yylval =(int)_strdup(yytext);return REMOTE;
("Tx"|"Rx")					yylval =(int)_strdup(yytext);return MSGDIR;
"TxRq"						yylval =(int)_strdup("Tx");return MSGDIR;
"="							return EQUAL;
"Length"					return LENGTHTOKEN;
"BitCount"					return BITCOUNTTOKEN;
\n                      	return LINEEND;
[ \t]+                  	/* ignore whitespace */;
.							/* ignore Anything else */;
%%
