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
 * \file      definitions.h
 * \brief     Defintitons File
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defintitons File
 */

//Comments.cpp
#define defCON_MAX_LINE_LEN 1026  //Maximum no of char in one line

//Message.cpp
#define defMSG_MAX_NAME_LEN	33 // IN canoe a name can't be more than 32bytes.

//ParameterVal.h
#define	defNODE_MAX_LEN 33		// IN canoe a name can't be more than 32bytes.
#define defVAL_MAX_LINE_LEN 1026

//Signal.cpp
#define defVTAB_MAX_LINE_LEN 1026
//#define defVTAB_MAX_TEMP_LEN 1526

//Signal.h
#define defSIG_MAX_NAME_LEN	33 // normally name > 100 chars not expected
#define	defSIG_MAX_UNIT_LEN	33	//Maximum length of a given unit 
#define	defSIG_MAX_MULTIPLEX_LEN	33	//Maximum length of a given Mulyiplexing 
#define	defSIG_MAX_RXNODE_LEN	385	//Maximum length of a given RX node

//ValueDescriptor.h
#define	defVDES_MAX_DESC_LEN 129 // hopefully nobody puts value descriptor strings
// more than this
#define defVDES_MAX_OUT_STR  (17+defVDES_MAX_DESC_LEN+20)  // 17(tag)+(Value descriptor)+ 20(Value)

//ValueTable.cpp
#define defVTAB_MAX_LINE_LEN 1026
#define defVTAB_MAX_TEMP_LEN 1126
#define defVTAB_MAX_DESC_LEN 33  //Maximum length of a value descriptor
#define defVTAB_MAX_TNAM_LEN 33  //Maximum length of the table name
#define defVTAB_MAX_VALU_LEN 33  //Maximum length of value
#define defPARAM_RANGE 10
//tags.h
#define defCON_TEMP_LEN 250
typedef union _SIG_VALUE
{
    int iValue;
    unsigned int uiValue;
    float fValue;
    double	dValue;
    char cValue[defCON_TEMP_LEN];
    LONGLONG i64Value;
    ULONGLONG ui64Value;
} SIG_VALUE;
