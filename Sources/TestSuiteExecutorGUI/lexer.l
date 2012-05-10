%{
// lexer.l -- From tcalc: a simple calculator program

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
extern YYSTYPE yylval;
%}
%option noyywrap
SPACE { \t}+
%%
[0-9]*\.?[0-9]+|[0-9]+\.[0-9]* { sscanf(yytext, "%lf", &yylval); return NUMBER;}
"+" { return ADD; }
"-" { return SUB; }
"/" { return DIV; }
"*" { return MUL; }
"(" { return RPA; }
")" { return LPA; }
">" { return GT; }
"<" { return LT; }
"<=" { return LE; }
">=" { return GE; }
"==" { return EQ; }
"!=" { return NE; }
"||" { return OR; }
"&&" { return AND; }
"^" { return POW;}
"\n" { return NL; }
SPACE { }
