%{
   #define YYSTYPE double
   #include <math.h>
   #include <stdio.h>
   #include <malloc.h>
   extern FILE *yyin, *yyout;
   int yylex (void);
   
 %}
 
 %token NUMBER ADD SUB DIV MUL RPA LPA GT LT LE GE EQ NE OR AND NL POW
 %left SUB ADD
 %left LE EQ GE NE OR AND MUL DIV LT GT
 %left NEGATIVE     
 %right POW    

  %% 
 START:    /* empty */
		 | START INFIX
 ;
 
 INFIX:     NL
		 | Expression NL  {fprintf(yyout, "in Result %lf", $1); return $1; /*printf ("\t%.10g\n", $1);*/ }
 ;
 
 Expression:      NUMBER					{ fprintf(yyout, "\nin Number %lf", $1);$$ = $1;         }
		 | Expression ADD Expression		{ $$ = $1 + $3;    }
		 | Expression SUB Expression		{ $$ = $1 - $3;    }
		 | Expression MUL Expression		{ $$ = $1 * $3;    }
		 | Expression DIV Expression		{ $$ = $1 / $3;    }
		 | SUB Expression  %prec NEGATIVE	{ $$ = -$2;        }
		 | Expression POW Expression		{ $$ = pow ($1, $3); }
		 | Expression GT Expression			{ $$ = ($1 > $3)?1:0;  }
		 | Expression LT Expression			{ $$ = ($1 < $3)?1:0;  }
		 | RPA Expression LPA				{ $$ = $2;}
		 | Expression LE Expression			{ $$ = ($1 <= $3)?1:0; }
		 | Expression GE Expression			{ $$ = ($1 >= $3)?1:0; }
		 | Expression EQ Expression			{ fprintf(yyout, "\nin EE %lf %lf", $1, $3);$$ = ($1 == $3)?1:0; }
		 | Expression NE Expression			{ $$ = ($1 != $3)?1:0; }
		 | Expression OR Expression			{ $$ = (int)$1 || (int)$3; }
		 | Expression AND Expression		{ $$ = (int)$1 && (int)$3; }
 ;
 %%
int yyerror(char *s)        /* called by yyparse on error */
{
    printf("%s\n",s);
    return(-1);
}

int bGetExpressionResult(char *pchExpression)
{
	float nVal = 0;
	char tstr[] = "-12.4-3+4+5\n\0\0";
	yyin = NULL;
	yyout = fopen("hi1result.txt", "w+");
	/*yyin = fopen("hi1.txt", "r");
	yyout = fopen("hi1result.txt", "w+");
	if(yyin == NULL)
		nVal = -1;
	else*/
	yy_scan_string(pchExpression);
	nVal = yyparse();
	//fclose(yyin);
	fclose(yyout);
	return (nVal);
}
/*
int main(void)
{
	char tstr[] = "-12.4-3+4+5\n\0\0";
	// note yy_scan_buffer is is looking for a double null string
	//yy_scan_buffer(tstr, sizeof(tstr));
    
    printf("Result =%.10g\n", nval);
    exit(0);
}
*/