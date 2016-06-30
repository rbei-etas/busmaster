%{
   #define YYSTYPE double
   #include <math.h>
   #include <stdio.h>
   #include <malloc.h>
   extern FILE *yyin, *yyout;
   int yylex (void);
   int nPareseError = 0;
 %}
 
 %token NUMBER EQ ADD SUB DIV MUL RPA LPA GT LT LE GE NE OR AND NL POW ERROR WSPACE
 
%left OR
%left AND
%left EQ NE
%left GT LT GE LE
%left ADD SUB
%left MUL DIV
%left NEGATIVE 
%right POW

  %% 
 START:    /* empty */
		 | START INFIX
 ;
 
 INFIX:     NL
		 | Expression NL  {return $1; }
 ;
 
 Expression:      NUMBER					{ $$ = $1;         }
		 | Expression ADD Expression		{ $$ = $1 + $3;    }
		 | Expression SUB Expression		{ $$ = $1 - $3;    }
		 | Expression MUL Expression		{ $$ = $1 * $3;    }
		 | Expression DIV Expression		{ if($3 == 0){return 0;} $$ = $1 / $3;    }
		 | SUB Expression  %prec NEGATIVE	{ $$ = -$2;        }
		 | Expression POW Expression		{ $$ = pow ($1, $3); }
		 | Expression GT Expression			{ $$ = ($1 > $3)?1:0;  }
		 | Expression LT Expression			{ $$ = ($1 < $3)?1:0;  }
		 | RPA Expression LPA				{ $$ = $2;}
		 | Expression LE Expression			{ $$ = ($1 <= $3)?1:0; }
		 | Expression GE Expression			{ $$ = ($1 >= $3)?1:0; }
		 | Expression EQ Expression			{ $$ = ($1 == $3)?1:0; }
		 | Expression NE Expression			{ $$ = ($1 != $3)?1:0; }
		 | Expression OR Expression			{ $$ = (int)$1 | (int)$3; }
		 | Expression AND Expression		{ $$ = (int)$1 && (int)$3; }
		 | ERROR							{ return 0;}
		 | WSPACE							{}
 ;
 %%
int yyerror(char *s)        /* called by yyparse on error */
{
    printf("%s\n",s);
	nPareseError = 1;
    //return(-999);
}

int bGetExpressionResult(char *pchExpression)
{
	float nVal = 0;
	yyin = NULL;
	yyout = NULL;
	yy_scan_string(pchExpression);
	nPareseError = 0;
	nVal = yyparse();
	if( nPareseError == 1 )
	{
		nVal = 0;
	}
	return (nVal);
}
