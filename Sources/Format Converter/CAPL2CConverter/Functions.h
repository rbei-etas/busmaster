/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      Functions.h
 * \brief     Interface file for parsing functions
 * \authors   Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for parsing functions
 */

#pragma once

/* C includes */
#include <stdlib.h>

/* C++ includes */
#include <string>

/* Project includes */
#include "ConstString.h"
#include "Converter.h"
#include "SymbolTable.h"

char acSptdKey[defINT_SizeofacAltKeyArray];//to store supported keys
CSymbolTable ouUnSptdKey;
//to store unsupported keys
CSymbolTable ouFuncName ;
//to store the name of utility function
CSymbolTable ouTimerName;
//to store the name of timer variable

char acAltKey[254];  //to store the alternate keys
CString Footer;   //for header and footer

CString ExceptionFormat = "In %s Exception occuered.\nFile : %s\nLine : %d";
char cIndex2 = 0;       //to store the acSptdKey (needed)
int  nSourceLine = 0;           //to count no. of lines in a source file
float  nUnCnvrtdLine = 0;         //to count no. of lines not converted
char cFlag = 0;   //used in 2 functions
char cFlagH = 0 ; //to count the no. of white space (needed)
char cFlagLog = 1;//to differentiate "write" and "writetolog" function
int m = 0 ; //neccessary
char fprestart = 0;//flag for prestart
char fstart = 0;//flag for start (on load handler)


FILE *logfile ;                    //for log file
CString dest1;  //to store the path of log file (needed)


char cFlagHeader = 0; //used as flag to go in vardecl state from header state
int counter = 1; // to count "{" and "}" (needed)

CString canoedb , busmasterdb; //to store the name of canoe and busmaster database

CConverter convert;

void vFuncCall();
void vCancelTimer();
void vOutput();
void vDelete();
void vCnvrtByte();
void vHeader();
void vPutKeyCase();
void vFooter();
void vMsgDecl();
void vKeyStoreCase();
void vCaplFunc();
void vEnvVar();
void vKeyStoreHeader();
void vSetTimer();
void vTimerFunc();
void vCnvrtThis();
void vWriteTerminator();
void vMsgID();

void vUnSptdFunc();
void vOnLine();
void vOffLine();
void vTrigger();
void vResetCan();
void vDisconnect();

void vWrite();
void vWriteDot();
void vRemovePhys();

void vAddLine();
void vEndBody();
void vMsgCpy();
void velCount();
void vUtilFunc();

int nConvert(CString Source, CString Destination, CString dest1);
