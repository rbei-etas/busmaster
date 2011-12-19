
/*  A Bison parser, made from d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	HEXNUMBER	258
#define	NUMBER	259
#define	TOKHEAT	260
#define	STATE	261
#define	TOKTARGET	262
#define	TOKTEMPERATURE	263
#define	DOUBLEVAL	264
#define	MSGDIR	265
#define	DATAMSG	266
#define	LENGTHTOKEN	267
#define	BITCOUNTTOKEN	268
#define	EQUAL	269
#define	EXTID	270
#define	MONTH	271
#define	DAY	272
#define	FULLTIME	273
#define	DATETOKEN	274
#define	REMOTE	275
#define	IGNORE	276
#define	TEXT	277
#define	COLON	278
#define	AM_PM	279
#define	BASETOKEN	280
#define	BASE	281
#define	TIMESTAMPSTOKEN	282
#define	TIMEMODE	283
#define	BEGINTRIGGERTOKEN	284
#define	LINEEND	285
#define	ENDTRIGGERTOKEN	286

#line 1 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define TIME_MODE_ABSOLUTE			 0
#define TIME_MODE_RELATIVE			 1
#define	TIME_MODE_UNDEFINED			-1
#define DEF_LOG_END_TEXT			"***END DATE AND TIME ***\n"
#define	DEF_LOG_STOP_TEXT			"***[STOP LOGGING SESSION]***"


int nSize = 0;
int nLen = 0;
int nTimeMode = TIME_MODE_UNDEFINED;
int nNumLines = 0;
char data[256];
extern FILE *yyin, *yyout;

void yyerror(const char *str)
{
	//fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	fprintf(yyout, DEF_LOG_END_TEXT);
	fprintf(yyout, DEF_LOG_STOP_TEXT);
	return 1;	//1 Sepcifies conversion is over.
				//0 specifies the parser start the conversion from different
				//Input
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
	char chMilliSeconds[5] = {'\0'};
	int nHours, nMin, nSec;
	unsigned int unSeconds;
	unsigned int unMilliSeconds;
	
	pchTemp = strtok(pchAscTime, chSeparators);
	unSeconds = strtoul(pchTemp, NULL, 10);
	nSecondToTime(unSeconds, &nHours, &nMin, &nSec);
	
	pchTemp = strtok(NULL, chSeparators);
	
	strncpy( chMilliSeconds, pchTemp, 4);
	
	sprintf(pchLogTime, "%u:%u:%u:%s", nHours, nMin, nSec, chMilliSeconds);
}
int nConvertFile(FILE* fpInputFile, FILE* fpOutputFile)
{
	if( (NULL != fpOutputFile) && (NULL != fpOutputFile) )
	{ 
		yyin = fpInputFile;
		yyout = fpOutputFile;
		yyparse();
	}
	return 0;
}

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		48
#define	YYFLAG		-32768
#define	YYNTBASE	33

#define YYTRANSLATE(x) ((unsigned)(x) <= 286 ? yytranslate[x] : 45)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    32,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,     9,    11,    13,    15,    17,    19,
    21,    23,    25,    26,    29,    33,    37,    45,    53,    59,
    64,    72,    74
};

static const short yyrhs[] = {    34,
     0,    33,    34,     0,     1,     0,    38,     0,    44,     0,
    39,     0,    40,     0,    41,     0,    36,     0,    42,     0,
    37,     0,    43,     0,     0,    35,     4,     0,    12,    14,
     4,     0,    13,    14,     4,     0,     9,     4,     4,    10,
     4,     4,    35,     0,     9,     4,    15,    10,     4,     4,
    35,     0,     9,     4,     4,    10,    20,     0,    25,    26,
    27,    28,     0,    19,    17,    16,     4,    18,    24,     4,
     0,    30,     0,     1,    32,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    77,    78,    81,    83,    85,    87,    89,    91,    93,    95,
    97,    99,   103,   104,   112,   117,   123,   141,   161,   177,
   204,   260,   265
};

static const char * const yytname[] = {   "$","error","$undefined.","HEXNUMBER",
"NUMBER","TOKHEAT","STATE","TOKTARGET","TOKTEMPERATURE","DOUBLEVAL","MSGDIR",
"DATAMSG","LENGTHTOKEN","BITCOUNTTOKEN","EQUAL","EXTID","MONTH","DAY","FULLTIME",
"DATETOKEN","REMOTE","IGNORE","TEXT","COLON","AM_PM","BASETOKEN","BASE","TIMESTAMPSTOKEN",
"TIMEMODE","BEGINTRIGGERTOKEN","LINEEND","ENDTRIGGERTOKEN","';'","commands",
"command","Can_Data_Bytes","Msg_Len","Bit_Count","Standard_Msg","Extended_Msg",
"RemoteFrame","Base_TimeStamps","Log_Creation_Time","Line_End","stmnt",""
};
#endif

static const short yyr1[] = {     0,
    33,    33,    34,    34,    34,    34,    34,    34,    34,    34,
    34,    34,    35,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    44
};

static const short yyr2[] = {     0,
     1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     0,     2,     3,     3,     7,     7,     5,     4,
     7,     1,     2
};

static const short yydefact[] = {     0,
     3,     0,     0,     0,     0,     0,    22,     0,     1,     9,
    11,     4,     6,     7,     8,    10,    12,     5,    23,     0,
     0,     0,     0,     0,     2,     0,     0,    15,    16,     0,
     0,     0,     0,     0,    20,     0,    19,     0,     0,    13,
    13,     0,    17,    18,    21,    14,     0,     0
};

static const short yydefgoto[] = {     8,
     9,    43,    10,    11,    12,    13,    14,    15,    16,    17,
    18
};

static const short yypact[] = {     2,
   -28,     3,    -8,    -6,    -7,    -9,-32768,     0,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,     1,
    16,    18,     7,    -3,-32768,    19,    21,-32768,-32768,    22,
     5,    -2,    24,    17,-32768,    30,-32768,    32,    13,-32768,
-32768,    34,    35,    35,-32768,-32768,    40,-32768
};

static const short yypgoto[] = {-32768,
    33,     4,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768
};


#define	YYLAST		45


static const short yytable[] = {    47,
     1,    36,     1,    19,    26,    21,    20,    22,     2,    23,
     2,     3,     4,     3,     4,    27,    24,    37,     5,    28,
     5,    29,    30,    31,     6,    34,     6,    38,    32,     7,
    33,     7,    35,    40,    39,    41,    42,    45,    46,    48,
    25,     0,     0,     0,    44
};

static const short yycheck[] = {     0,
     1,     4,     1,    32,     4,    14,     4,    14,     9,    17,
     9,    12,    13,    12,    13,    15,    26,    20,    19,     4,
    19,     4,    16,    27,    25,     4,    25,     4,    10,    30,
    10,    30,    28,     4,    18,     4,    24,     4,     4,     0,
     8,    -1,    -1,    -1,    41
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
#line 82 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{yyerrok;yyclearin; ;
    break;}
case 14:
#line 105 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{
		strcpy((data+nLen) , (char*)yyvsp[0]);
		nLen += strlen((char*)yyvsp[0]);
		data[nLen] = ' ';
		nLen++;
		data[nLen] = '\0';
	;
    break;}
case 15:
#line 114 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{
		yyval = yyvsp[0];
	;
    break;}
case 16:
#line 119 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{
		yyval = yyvsp[0];
	;
    break;}
case 17:
#line 125 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{
		char chLogTime[256] = {'\0'};
		
		nGetLogTimeStamp((char*)yyvsp[-6], chLogTime);
		fprintf(yyout, "%s %s %s %s %s %s %s\n", chLogTime, yyvsp[-3], yyvsp[-5], yyvsp[-4], "s", yyvsp[-1], data);
		nLen = 0;
		
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);
		free($7);*/
	;
    break;}
case 18:
#line 143 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{
		char chLogTime[256] = {'\0'};
		char chId[12] = {'\0'};
		int nStrLen;
		nGetLogTimeStamp((char*)yyvsp[-6], chLogTime);
		strcpy(chId, yyvsp[-4]);
		nStrLen = strlen(chId);
		chId[nStrLen-1] = '\0';
		fprintf(yyout, "%s %s %s %s %s %s %s\n", chLogTime, yyvsp[-3], yyvsp[-5], chId, "x", yyvsp[-1], data);
		nLen = 0;
		
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);*/
	;
    break;}
case 19:
#line 163 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{
		
		char chLogTime[256] = {'\0'};
		
		nGetLogTimeStamp((char*)yyvsp[-4], chLogTime);
		fprintf(yyout,"%s %s %s %s %s %s %s\n", chLogTime, yyvsp[-1], yyvsp[-3], yyvsp[-2], "sr", "8",  "00 00 00 00 00 00 00 00");
		
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);*/
	;
    break;}
case 20:
#line 179 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{
		if(strcmp("dec", (char*)yyvsp[-2]) == 0)
		{
			fprintf(yyout,"%s\n", "***DEC***");
		}
		else
		{
			fprintf(yyout,"%s\n", "***HEX***");
		}
		if(strcmp("relative", (char*)yyvsp[0]) == 0)
		{
			fprintf(yyout,"%s\n", "***RELATIVE MODE***");
			nTimeMode = TIME_MODE_RELATIVE;
		}
		else
		{
			fprintf(yyout,"%s\n", "***ABSOLUTE MODE***");
			nTimeMode = TIME_MODE_ABSOLUTE;
		}
		fprintf(yyout,"%s\n", "***<Time><Tx/Rx><Channel><CAN ID><Type><DLC><DataBytes>***");
		/*free($1);
		free($2);
		free($3);
		free($4);*/
	;
    break;}
case 21:
#line 207 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{	
		/*date Wed Dec 7 12:23:39 pm 2011*/
		char chSeparators[]   = " :,\t\n";
		char* chTemp;
		int nMonth;
		int nHour, nMins, nSec;
		if( strcmp("Jan", (char*)yyvsp[-4]) == 0 )
			nMonth = 1;
		else if( strcmp("Feb", (char*)yyvsp[-4]) == 0 )
			nMonth = 2; 
		else if( strcmp("Mar", (char*)yyvsp[-4]) == 0 )
			nMonth = 3; 
		else if( strcmp("Apr", (char*)yyvsp[-4]) == 0 )
			nMonth = 4; 
		else if( strcmp("May", (char*)yyvsp[-4]) == 0 )
			nMonth = 5; 
		else if( strcmp("Jun", (char*)yyvsp[-4]) == 0 )
			nMonth = 6; 
		else if( strcmp("Jul", (char*)yyvsp[-4]) == 0 )
			nMonth = 7; 
		else if( strcmp("Aug", (char*)yyvsp[-4]) == 0 )
			nMonth = 8; 
		else if( strcmp("Sep", (char*)yyvsp[-4]) == 0 )
			nMonth = 9; 
		else if( strcmp("Oct", (char*)yyvsp[-4]) == 0 )
			nMonth = 10; 
		else if( strcmp("Nov", (char*)yyvsp[-4]) == 0 )
			nMonth = 11; 
		else if( strcmp("Dec", (char*)yyvsp[-4]) == 0 )
			nMonth = 12; 
		/*8:12:2011 20:15:28:553****/
		
		chTemp = strtok((char*)yyvsp[-2], chSeparators);
		nHour = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nMins = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nSec = atoi(chTemp);
		
		nHour = nHour % 12;
		if( strcmp("pm", (char*)yyvsp[-1]) == 0 )
		{
			nHour = nHour + 12;
		}
		fprintf(yyout,"%s:%d:%s %d:%d:%d:%s%s\n", yyvsp[-3], nMonth, yyvsp[0], nHour, nMins, nSec, "000", "***");
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);
		free($7);*/
	;
    break;}
case 22:
#line 262 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
{
		nNumLines++;
	;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 487 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 267 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\asclogconverter\asc_log_parser.y"
