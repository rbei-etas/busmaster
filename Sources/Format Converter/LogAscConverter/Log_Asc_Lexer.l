%{

#include <stdio.h>
#include <string.h>
#include "log_asc_parser.h"
char x[5] = "Hi";
%}
DIGIT		[0-9]
ALPHA		[A-Fa-f]
HEXTAIL		({DIGIT}|{ALPHA})+
HEX			{HEXTAIL}
%%
"***[START LOGGING SESSION]***"		yylval = _strdup(yytext); return BEGINLOGTOKEN;
"***[STOP LOGGING SESSION]***"			yylval = _strdup(yytext);return ENDLOGTOKEN;
"***START DATE AND TIME"			yylval = _strdup(yytext); return DATETOKEN;
[0-9]+\:[0-9]+\:[0-9]+\:[0-9]+	yylval = _strdup(yytext);return TIME;
[0-9]+\:[0-9]+\:[0-9]+		yylval = _strdup(yytext);return DATE;
("***HEX***"|"***DEC***")	yylval = _strdup(yytext);return BASE;
("***RELATIVE MODE***") 	yylval = _strdup(yytext);return TIMEMODE;
("***ABSOLUTE MODE***") 	yylval = _strdup(yytext);return TIMEMODE;
("***SYSTEM MODE***") 		yylval = _strdup(yytext);return TIMEMODE;
{HEX}						yylval =_strdup(yytext);return NUMBER;
[0-9]+\.?[0-9]*				yylval =_strdup(yytext);return DOUBLEVAL;
\:							yylval =_strdup(yytext);return COLON;
"s"							yylval =_strdup(yytext);return STDMSG;
"x"							yylval =_strdup(yytext);return EXTMSG;
"sr"						yylval =_strdup(yytext);return STDRMSG;
"xr"						yylval =_strdup(yytext);return EXTRMSG;
("Tx"|"Rx")					yylval =_strdup(yytext);return MSGDIR;
"="							return EQUAL;
"Length"					return LENGTHTOKEN;
"BitCount"					return BITCOUNTTOKEN;
\n                      	return LINEEND;
[ \t]+                  	/* ignore whitespace */;
.							/* ignore Anything else */;
%%
