
/*  A Bison parser, made from d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	BEGINLOGTOKEN	258
#define	ENDLOGTOKEN	259
#define	DATETOKEN	260
#define	FULLTIME	261
#define	BASE	262
#define	TIMEMODE	263
#define	NUMBER	264
#define	DOUBLEVAL	265
#define	DATE	266
#define	TIME	267
#define	STDMSG	268
#define	EXTMSG	269
#define	STDRMSG	270
#define	EXTRMSG	271
#define	MSGDIR	272
#define	EQUAL	273
#define	LENGTHTOKEN	274
#define	BITCOUNTTOKEN	275
#define	LINEEND	276
#define	COLON	277
#define	TIMESTAMPTOKEN	278

#line 1 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define TIME_MODE_ABSOLUTE			 0
#define TIME_MODE_RELATIVE			 1
#define TIME_MODE_SYSTEM			 2
#define	TIME_MODE_UNDEFINED			-1
#define DEF_LOG_END_TEXT			"***END DATE AND TIME ***\n"
#define	DEF_LOG_STOP_TEXT			"***[STOP LOGGING SESSION]***"
extern int nGetDayFromDate(char* pchDate, char *chMonth, char* chWeekDay);

int nSize = 0;
int nLen = 0;
int nTimeMode = TIME_MODE_UNDEFINED;
int nNumLines = 0;
char data[256];
int nNumModeProcd = 0;
int nTimeModeProcd = 0;
extern FILE *yyin, *yyout;

void yyerror(const char *str)
{
	//fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	fprintf(yyout, "End TriggerBlock\n");
	return 1;	//1 Sepcifies conversion is over.
				//0 specifies the parser start the conversion from different
				//Input
}
int nGetDate(char* pchDateString,  int* nDate, char* pchMonth, char* pchDay, int* nYear)
{
	char chTempDate[18] = {'\0'};
	char chTemp;
	int nMonth;
	strcpy(chTempDate, pchDateString);
	nGetDayFromDate(chTempDate, pchMonth, pchDay);
	sscanf(pchDateString, "%d%c%d%c%d", nDate, &chTemp, &nMonth, &chTemp, nYear);
	/*printf("%d, %d, %d\n", nDay, nMonth, *nYear);*/
	return 0;
}
int nGetTime( char* pchTimeString, int* nHour, int* nMin, int* nSec, char* pchAmPm)
{
	char chTemp;
	sscanf(pchTimeString, "%d%c%d%c%d", nHour, &chTemp, nMin, &chTemp, nSec);
	strcpy(pchAmPm, "am");
	if(*nHour > 12)
	{
		*nHour = *nHour - 12;
		strcpy(pchAmPm, "pm");
	}
	return 0;
}
int nSecondToTime(unsigned int nSeconds, int *nHours, int *nMin, int *nSec)
{
	*nHours = nSeconds / 3600; 
	nSeconds = nSeconds -(*nHours * 3600);
	*nMin = nSeconds / 60; 
	*nSec = nSeconds - (*nMin) * 60;
}
int nGetAscTimeStamp(char* pchLogTime, char* pchAscTime)
{
	char chSeparators[] = ".";
	char pchTemp;
	char chMilliSeconds[5] = {'\0'};
	int nHours, nMin, nSec;
	int nMicroSeconds;
	unsigned int unTotalSec;
	float fAscTime;
	if(pchAscTime == NULL || pchLogTime == NULL)
	{
		return -1;
	}
	printf("%s\n", pchLogTime);
	sscanf(pchLogTime, "%d%c%d%c%d%c%d", &nHours, &pchTemp, &nMin, &pchTemp, &nSec, &pchTemp, &nMicroSeconds);
	unTotalSec = nHours*3660 + nMin * 60 + nSec;
	printf("%d\n", nMicroSeconds);
	sprintf(pchAscTime, "%u.%06d", unTotalSec, nMicroSeconds*100);
	return 0;
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



#define	YYFINAL		41
#define	YYFLAG		-32768
#define	YYNTBASE	25

#define YYTRANSLATE(x) ((unsigned)(x) <= 278 ? yytranslate[x] : 38)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    24,     2,
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
    16,    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,     9,    11,    13,    15,    17,    19,
    21,    23,    25,    27,    28,    31,    35,    39,    43,    45,
    47,    55,    63,    70,    77
};

static const short yyrhs[] = {    26,
     0,    25,    26,     0,     1,     0,    33,     0,    34,     0,
    35,     0,    36,     0,    37,     0,    28,     0,    29,     0,
    30,     0,    31,     0,    32,     0,     0,    27,     9,     0,
    19,    18,     9,     0,    20,    18,     9,     0,     5,    11,
    12,     0,     7,     0,     8,     0,    12,    17,     9,     9,
    13,     9,    27,     0,    12,    17,     9,     9,    14,     9,
    27,     0,    12,    17,     9,     9,    15,     9,     0,    12,
    17,     9,     9,    15,     9,     0,     1,    24,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   103,   104,   107,   109,   111,   113,   115,   117,   119,   121,
   123,   125,   127,   131,   132,   140,   145,   150,   165,   181,
   199,   218,   227,   236,   245
};

static const char * const yytname[] = {   "$","error","$undefined.","BEGINLOGTOKEN",
"ENDLOGTOKEN","DATETOKEN","FULLTIME","BASE","TIMEMODE","NUMBER","DOUBLEVAL",
"DATE","TIME","STDMSG","EXTMSG","STDRMSG","EXTRMSG","MSGDIR","EQUAL","LENGTHTOKEN",
"BITCOUNTTOKEN","LINEEND","COLON","TIMESTAMPTOKEN","';'","commands","command",
"Can_Data_Bytes","Msg_Len","Bit_Count","Creation_Time","Number_Mode","TimeStamp_Mode",
"Standard_Msg","Extended_Msg","Standard_R_Msg","Extended_R_Msg","stmnt",""
};
#endif

static const short yyr1[] = {     0,
    25,    25,    26,    26,    26,    26,    26,    26,    26,    26,
    26,    26,    26,    27,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    36,    37
};

static const short yyr2[] = {     0,
     1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     0,     2,     3,     3,     3,     1,     1,
     7,     7,     6,     6,     2
};

static const short yydefact[] = {     0,
     3,     0,    19,    20,     0,     0,     0,     0,     1,     9,
    10,    11,    12,    13,     4,     5,     6,     7,     8,    25,
     0,     0,     0,     0,     2,    18,     0,    16,    17,     0,
     0,     0,     0,    14,    14,    23,    21,    22,    15,     0,
     0
};

static const short yydefgoto[] = {     8,
     9,    37,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19
};

static const short yypact[] = {     9,
   -18,    -2,-32768,-32768,    -6,    -5,    -3,     0,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     6,    13,    14,    15,-32768,-32768,    16,-32768,-32768,   -11,
    17,    18,    21,-32768,-32768,-32768,    22,    22,-32768,    32,
-32768
};

static const short yypgoto[] = {-32768,
    25,    -1,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768
};


#define	YYLAST		34


static const short yytable[] = {    40,
     1,    31,    32,    33,     2,    20,     3,     4,    21,     1,
    22,     5,    23,     2,    24,     3,     4,    26,     6,     7,
     5,    27,    28,    29,    30,    34,    35,     6,     7,    36,
    39,    41,    25,    38
};

static const short yycheck[] = {     0,
     1,    13,    14,    15,     5,    24,     7,     8,    11,     1,
    17,    12,    18,     5,    18,     7,     8,    12,    19,    20,
    12,     9,     9,     9,     9,     9,     9,    19,    20,     9,
     9,     0,     8,    35
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
#line 108 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{yyerrok;yyclearin; ;
    break;}
case 15:
#line 133 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		strcpy((data+nLen) , (char*)yyvsp[0]);
		nLen += strlen((char*)yyvsp[0]);
		data[nLen] = ' ';
		nLen++;
		data[nLen] = '\0';
	;
    break;}
case 16:
#line 142 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		yyval = yyvsp[0];
	;
    break;}
case 17:
#line 147 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		yyval = yyvsp[0];
	;
    break;}
case 18:
#line 152 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		//Fri Dec 9 11:58:31 am 2011
		//9:12:2011 11:58:31:000
		int nDate, nYear;
		int nHour, nMin, nSec;
		char pchAmPm[3] = {'\0'};
		char pchMonth[4] = {'\0'};
		char pchDay[4] = {'\0'};
		nGetDate((char*)yyvsp[-1], &nDate, pchMonth, pchDay, &nYear);
		nGetTime((char*)yyvsp[0], &nHour, &nMin, &nSec, pchAmPm);
		//date Fri Dec 9 11:58:31 am 2011
		fprintf(yyout, "date %s %s %d %d:%d:%d %s %d\n", pchDay, pchMonth, nDate, nHour, nMin, nSec, pchAmPm, nYear);
	;
    break;}
case 19:
#line 167 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		if(nNumModeProcd == 0)
		{
			if(strcmp((char*)yyvsp[0], "***HEX***") == 0)
			{
				fprintf(yyout, "base hex  ");
			}
			else
			{
				fprintf(yyout, "base dec  ");
			}
			nNumModeProcd = 1;
		}
	;
    break;}
case 20:
#line 183 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		if(nTimeModeProcd == 0)
		{
			if(strcmp((char*)yyvsp[0] ,"***RELATIVE MODE***") == 0)
			{
				fprintf(yyout, "timestamps relative\n");
			}
			else if(strcmp((char*)yyvsp[0] ,"***ABSOLUTE MODE***") == 0)
			{
				fprintf(yyout, "timestamps absolute\n");
			}
			fprintf(yyout, "no internal events logged\n");
			fprintf(yyout, "// version 7.1.0\n");
			nTimeModeProcd = 1;
		}
	;
    break;}
case 21:
#line 202 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		/*0:0:0:0012 Tx 1 9 s 8 00 00 00 00 00 00 00 00 */
		char chAscTime[256] = {'\0'};
		nGetAscTimeStamp((char*)yyvsp[-6], chAscTime);
		/*0.001250 1  9               Tx   d 8 00 00 00 00 00 00 00 00*/
		fprintf(yyout, "%s %s %s %s d %s %s\n", chAscTime, yyvsp[-4], yyvsp[-3], yyvsp[-5], yyvsp[-1], data);
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
case 22:
#line 220 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		char chAscTime[256] = {'\0'};
		nGetAscTimeStamp((char*)yyvsp[-6], chAscTime);
		/*0.001250 1  9               Tx   d 8 00 00 00 00 00 00 00 00*/
		fprintf(yyout, "%s %s %sx %s d %s %s\n", chAscTime, yyvsp[-4], yyvsp[-3], yyvsp[-5], yyvsp[-1], data);
		nLen = 0;
	;
    break;}
case 23:
#line 229 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		/*<Time> <Channel> <ID> <Dir> r*/
		char chAscTime[256] = {'\0'};
		nGetAscTimeStamp((char*)yyvsp[-5], chAscTime);
		/*0.001250 1  9               Tx   d 8 00 00 00 00 00 00 00 00*/
		fprintf(yyout, "%s %s %s %s r\n", chAscTime, yyvsp[-3], yyvsp[-2], yyvsp[-4], yyvsp[0]);
	;
    break;}
case 24:
#line 238 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
{
		/*<Time> <Channel> <ID> <Dir> r*/
		char chAscTime[256] = {'\0'};
		nGetAscTimeStamp((char*)yyvsp[-5], chAscTime);
		/*0.001250 1  9               Tx   d 8 00 00 00 00 00 00 00 00*/
		fprintf(yyout, "%s %s %s %s r\n", chAscTime, yyvsp[-3], yyvsp[-2], yyvsp[-4], yyvsp[0]);
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
#line 247 "d:\venkatanarayana\myprograms\bbautosattool_testautomation\oss\rbei-etas-busmaster-v1.3.0-191211\rbei-etas-busmaster-934748d\sources\format converter\logascconverter\log_asc_parser.y"
