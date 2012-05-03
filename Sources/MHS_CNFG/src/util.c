/***************************************************************************
                           util.c  -  description
                             -------------------
    begin             : 01.11.2010
    copyright         : (C) 2010 by MHS-Elektronik GmbH & Co. KG, Germany
    autho             : Klaus Demlehner, klaus@mhs-elektronik.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software, you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation.             *
 *                                                                         *
 ***************************************************************************/

/**
    Library to talk to Tiny-CAN devices. You find the latest versions at
       http://www.tiny-can.com/
**/
//#include "global.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "util.h"


#ifdef __WIN32__
  #define DIR_SEPARATOR '\\'
  #define DIR_SEPARATOR_STR "\\"
#else
  #define DIR_SEPARATOR '/'
  #define DIR_SEPARATOR_STR "/"
#endif


/**************************************************************************/
/*                        F U N K T I O N E N                             */
/**************************************************************************/

/*
******************** strlwc ********************
Funktion  : Einen String in Kleinbuchstaben umwandeln

Eingaben  : str => Zu wandelnder String

Ausgaben  : keine

Call's    : tolower
*/
void strlwc(char *str)
{
while (*str) 
  {
  *str = (char)tolower((int)*str);
  str++;
  }    
}


/*
******************** strupc ********************
Funktion  : Einen String in Grossbuchstaben umwandeln

Eingaben  : str => Zu wandelnder String

Ausgaben  : keine

Call's    : toupper
*/
void strupc(char *str)
{
while (*str) 
  {
  *str = (char)toupper((int)*str);
  str++;
  }
}


/*
******************** strskp ********************
Funktion  : Fuehrende Leerzeichen des Strings ueberspringen

Eingaben  : str => Input String

Ausgaben  : keine

Call's    : isspace
*/
void strskp(char **str)
{
char *s;

if (!str) 
  return;
s = *str;
while (isspace((int)*s))
  s++;
*str = s;
} 


/*
******************** strcrop ********************
Funktion  : Abschliesende Leerzeichen des Strings loeschen

Eingaben  : str => Zu bearbeitender String

Ausgaben  : keine

Call's    : strlen, isspace
*/
void strcrop(char *str)
{
char *last;
int len;

if (!str)
  return;
len = strlen(str);
if (len == 0) return;
last = str + (len-1);
while ((last != str) && (isspace((int)*last)))
  last--;
last++;
*last=0;
}


/*
******************** strstrip ********************
Funktion  : Fuehrende Leerzeichen des Strings ueberspringen
            und abschliesende Leerzeichen loeschen

Eingaben  : str => Input String

Ausgaben  : keine

Call's    : isspace
*/
void strstrip(char **str)
{
char *last;
int len;

last = *str;
// Leerzeichen werden ignoriert
while (isspace((int)*last))
  last++;
*str = last;
len = strlen(last);
if (len) 
  {
  last += (len-1);
  while ((len--) && (isspace((int)*last)))
    last--;
  last++;
  *last = 0;
  }
}


int str_has_char(char *s)
{
char c;

if (!s)
  return(-1);
while ((c = *s++))
  {
  if (c != ' ')
    break;
  }
if (c)
  return(0);
else
  return(-1);
}

void *mhs_malloc(int size)
{
return(malloc(size));
}


void *mhs_calloc(int num, int size)
{
return(calloc(num, size));
}


void mhs_free(void *mem)
{
free(mem);
}


void *mhs_memdup(void const *mem, int size)
{
void *new_mem;

if (mem)
  {
  new_mem = mhs_malloc(size);
  memcpy(new_mem, mem, size);
  }
else
  new_mem = NULL;

return(new_mem);
}


int save_strcmp(const char *s1, const char *s2)
{
if ((!s1) && (!s2))
  return(0);
if ((!s1) || (!s2))
  return(-1);
return(strcmp(s1, s2));
}


int save_strcasecmp(const char *s1, const char *s2)
{
char c1, c2;

if ((!s1) && (!s2))
  return(0);
if ((!s1) || (!s2))
  return(-1);
while ((c1 = *s1++))
  {
  c1 = (char)toupper((int)c1);
  c2 = *s2++;
  c2 = (char)toupper((int)c2);
  if (c1 != c2)
    return(-1);
  }
if (*s2)
  return(-1);
return(0);
//return(strcasecmp(s1, s2));
}


int find_item(char *s, char *list, char trenner)
{
int hit, idx;
char c, l;
char *in;

if ((!s) || (!list))
  return(-1);
idx = 0;
while (*list)
  {
  in = s;
  hit = 1;
  while ((c = *in++))
    {
    c = (char)toupper((int)c);
    l = *list++;
    if (l == trenner)
      {
      if (c)
        hit = 0;
      break;
      }
    l = (char)toupper((int)l);

    if (c != l)
      {
      hit = 0;
      break;
      }
    }
  if (hit)
    return(idx);
  idx++;
  }
return(-1);
}


/*
******************** find_upc ********************
Funktion  :

Eingaben  :

Ausgaben  : keine

Call's    :
*/
char *find_upc(char *str, char *search)
{
char *s;
char c;

s = search;
while ((c = *str++))
  {
  c = (char)toupper((int)c);
  if (c == *s++)
    {
    if (!*s)
      return(str);
    }
  else
    s = search;
  }
return(NULL);
}


/*
******************** get_tick ********************
Funktion  :

Eingaben  :

Ausgaben  : keine

Call's    :
*/
#ifndef __WIN32__
unsigned long get_tick(void)
{
//struct timespec now;
struct timeval now;

//clock_gettime(CLOCK_REALTIME, &now);
//return((now.tv_sec * 1000) + (now.tv_nsec / 1000000));
gettimeofday(&now, NULL);
return((now.tv_sec * 1000) + (now.tv_usec / 1000));
}
#endif


/*
******************** get_unix_time ********************
Funktion  : Unix Zeit unter Windows ermitteln

Eingaben  : p => Buffer fuer Ergebnis

Ausgaben  : *p => Unix Zeit

Call's    : GetSystemTimeAsFileTime
*/
#ifdef __WIN32__

const ULONGLONG VALUE_10      = {10};
const ULONGLONG VALUE_1000000 =  {1000000};
const ULONGLONG VALUE_10000000 = {10000000};
const ULONGLONG VALUE_116444736000000000 = {116444736000000000};
/*
const uint64_t VALUE_10      = {10};
const uint64_t VALUE_1000000 =  {1000000};
const uint64_t VALUE_10000000 = {10000000};
const uint64_t VALUE_116444736000000000 = {116444736000000000};*/

void get_unix_time(struct timeval *p)
{
union
  {
  ULONGLONG ns100; // time since 1 Jan 1601 in 100ns units
  FILETIME ft;
  }
now;

#ifdef WINCE
SYSTEMTIME time;
GetSystemTime(&time);
// if SystemTimeToFileTime() fails, it returns zero.
if (!SystemTimeToFileTime(&time, &(now.ft)))
  return;
#else
GetSystemTimeAsFileTime(&(now.ft));
#endif
p->tv_usec=(uint32_t)((now.ns100 / VALUE_10) % VALUE_1000000);
p->tv_sec= (uint32_t)((now.ns100-(VALUE_116444736000000000))/ VALUE_10000000);
}
#endif


/*void get_timestamp(struct TTime* time_stamp)
{
struct timeval t;

get_unix_time(&t);
time_stamp->USec = (uint32_t)t.tv_usec;
time_stamp->Sec = (uint32_t)t.tv_sec;
}*/


/*
******************** mhs_diff_time ********************
Funktion  : Differenzeit zwischen now und stamp
            berechnen unter beruecksichtigung eines
            Ueberlaufs

Eingaben  : now => Aktuelle Zeit
            stamp => Zeitstempel in der Vergangenheit

Ausgaben  : result => Differenz-Zeit x = now - stamp

Call's    : keine
*/
unsigned long mhs_diff_time(unsigned long now, unsigned long stamp)
{
if (stamp > now)
  return((0xFFFFFFFF - stamp) + now + 1);
else
  return(now - stamp);
}


#define SET_RESULT(result, value) if ((result)) *(result) = (value)

char *get_item_as_string(char **str, char *trenner, int *result)
{
int hit, cnt, l;
char *s, *t, *start, *end, *item;

if ((!str) || (!trenner))
  {
  SET_RESULT(result, -1);
  return(NULL);
  }
s = *str;
if (!s)
  {
  SET_RESULT(result, -1);
  return(NULL);
  }
// Fuehrende Leerzeichen ueberspringen
while (*s == ' ')
  s++;
if (*s == '\0')
  {
  SET_RESULT(result, -1);
  return(NULL);
  }
SET_RESULT(result, 0);
end = s;
item = s;
start = s;
// Eintrag in "xx" Zeichen
if ((s = strchr(s, '"')))
  {
  item = s + 1;
  if ((s = strchr(item, '"')))
    {
    *s++ = '\0';
    end = s;
    }
  }

hit = 0;
for (s = end; *s; s++)
  {
  cnt = 0;
  for (t = trenner; *t; t++)
    {
    cnt++;
    if (*s == *t)
      {
      *s++ = '\0';
      end = s;
      SET_RESULT(result, cnt);
      hit = 1;
      break;
      }
    }
  if (hit)
    break;
  }
// Abschliesende Leerzeichen loeschen
if ((l = strlen(item)))
  {
  s = item + (l-1);
  while ((l) && ((isspace((int)*s)) || (*s == '\n') || (*s == '\r')))
  {
    l--;
    s--;
  }
  if (l)
    s++;
  *s = 0;
  }
if (end == start)
  *str = start + strlen(end);
else
  *str = end;
return(item);
}


unsigned long get_item_as_ulong(char **str, char *trenner, int *result)
{
unsigned long v;
char *s;
int e;
char *endptr;

e = 0;
v = 0L;
s = get_item_as_string(str, trenner, NULL);
if (!s)
  e = -1;
else
  {
  v = strtoul(s, (char**)&endptr, 0);
  if (s == endptr)
    e = -1;
  }
SET_RESULT(result, e);
return(v);
}


long get_item_as_long(char **str, char *trenner, int *result)
{
long v;
char *s;
int e;
char *endptr;

e = 0;
v = 0L;
s = get_item_as_string(str, trenner, NULL);
if (!s)
  e = -1;
else
  {
  v = strtol(s, (char**)&endptr, 0);
  if (s == endptr)
    e = -1;
  }
SET_RESULT(result, e);
return(v);
}


double get_item_as_double(char **str, char *trenner, int *result)
{
double v;
char *s;
int e;

e = 0;
v = 0;
s = get_item_as_string(str, trenner, NULL);
if (!s)
  e = -1;
else  
  v = atof(s);
SET_RESULT(result, e);
return(v);
}


char *mhs_stpcpy(char *dest, const char *src)
{
register char *d = dest;
register const char *s = src;

do
  *d++ = *s;
while (*s++);

return(d - 1);
}


char *mhs_strdup(const char *str)
{
char *new_str;

if (str)
  {
  int len = strlen(str) + 1;
  new_str = (char *)mhs_malloc(len);
  if (!new_str)
    return(NULL);
  memcpy(new_str, str, len);
  return(new_str);
  }
else
  return(NULL);
}


char *mhs_strconcat(const char *string1, ...)
{
int l;
va_list args;
char *s, *concat, *ptr;

if (!string1)
  return(NULL);

l = 1 + strlen(string1);
va_start(args, string1);
s = va_arg(args, char*);
while (s)
  {
  l += strlen(s);
  s = va_arg(args, char*);
  }
va_end(args);

concat = (char *)mhs_malloc(l);
if (!concat)
  return(NULL);
ptr = concat;

ptr = mhs_stpcpy(ptr, string1);
va_start (args, string1);
s = va_arg (args, char*);
while (s)
  {
  ptr = mhs_stpcpy(ptr, s);
  s = va_arg(args, char*);
  }
va_end (args);

return(concat);
}


char *get_file_name(const char *file_name)
{
char *p;

if (!file_name)
  return(NULL);
if ((p = strrchr(file_name, DIR_SEPARATOR)))
  return(p+1);
else
  return(NULL);
}


char *create_file_name(const char *dir, const char *file_name)
{
if ((!dir) || (!file_name))
  return(NULL);
if (strchr(file_name, DIR_SEPARATOR))
  return(mhs_strdup(file_name));
else
  {
  if (dir[strlen(dir)-1] == DIR_SEPARATOR)
    return(mhs_strconcat(dir, file_name, NULL));
  else
    return(mhs_strconcat(dir, DIR_SEPARATOR_STR, file_name, NULL));
  }
}


char *path_get_dirname(const char *file_name)
{
char *base;

if (!file_name)
  return(NULL);
base = strrchr(file_name, DIR_SEPARATOR);
if (base)
  {
  while ((base > file_name) && (*base == DIR_SEPARATOR))
    base--;
  int len = base - file_name + 1;

  base = (char *)mhs_malloc(len + 1);
  if (!base)
    return(NULL);
  memcpy(base, file_name, len);
  base[len] = 0;
  return(base);
  }
else
  return(mhs_strdup("."));
}


char *change_file_ext(const char *file_name, const char *ext)
{
char *last_period, *tmp, *new_filename;

tmp = mhs_strdup(file_name);
last_period = strrchr(tmp, '.');
if (last_period)
  *last_period = '\0';
new_filename = mhs_strconcat(tmp, ".", ext, NULL);
save_free(tmp);
return(new_filename);
}
