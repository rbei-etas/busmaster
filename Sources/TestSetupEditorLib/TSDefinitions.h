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
 * \file      TSDefinitions.h
 * \brief     Definition of the Class SInfo
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the Class SInfo
 */

#pragma once

#include "include/error.h"
#define WM_TS_SELCHANGED           0X402
#define WM_TS_SELCHANGING          0X401

#define def_EMPTYFILENAME       "<Untitled>"        //File Name can't contain with '<'.
#define defDELETE_MSG_SYMBOL    _T("[Delete Message]")
#define defFIRSTCOLUMN_NAME     "Category"
#define defSECONDCOLUMN_NAME    "Value"
#define defCOLCATEGORY_INDEX    0
#define defCOLVALUE_INDEX       1
#define defPROPERTYCOLUMN_NUM   2


//xml File Elements Definitions
#define def_DOM_INSTRUCTION     "xml", "version=\"1.0\" encoding=\"UTF-8\""
#define def_STR_TSNODE_NAME     "testsetup"
#define def_STR_TSATTRIB_TITLE  "title"
#define def_STR_DESC_NODE       "description"
#define def_STR_TSDESCRIPTION   "testsetup/description"
#define def_STR_HEADER_NAME     "header"
#define def_STR_TSHEADER        "testsetup/header"
#define def_STR_INFO_NODE       "info"
#define def_STR_CATEGORY_NODE   "category"
#define def_STR_VALUE_NODE      "value"
#define def_STR_BUS_TYPE        "bustype"
#define def_STR_DBPATH_NODE     "list_of_database"
#define def_STR_LISTOFDB        "list_of_database/database"
#define def_STR_DATABASE        "database"
#define def_STR_REPORT_NODE     "reportfile"
#define def_STR_PATH_NODE       "path"
#define def_STR_REPORT_PATH     "reportfile/path"
#define def_STR_REPORT_FORMAT   "reportfile/format"
#define def_STR_REPORT_TIMEMODE "reportfile/timemode"
#define def_STR_FORMAT_NODE     "format"
#define def_STR_TIMEMODE_NODE   "timemode"
#define def_STR_TCLIST_NODE     "list_of_test_cases"
#define def_STR_TSTCLIST        "testsetup/list_of_test_cases"
#define def_STR_TESTCASE_NODE   "testcase"
#define def_STR_SEND_NODE       "send"
#define def_STR_VERIFY_NODE     "verify"
#define def_STR_VERIFYRES_NODE  "verifyResponse"
#define def_STR_ATTRIB_FAIL     "failure"
#define def_STR_ATTRIB_WAITFOR  "waitfor"
#define def_STR_TCATTRIB_ID     "identifier"
#define def_STR_TCATTRIB_TITLE  "title"
#define def_STR_TCATTRIB_H_EXP  "exp_handler"
#define def_STR_SENDMSG_NODE    "send_message"
#define def_STR_TCATTRIB_UNIT   "unit"
#define def_STR_ATTRIIB_DEFAULT "default"
#define def_STR_SIGNAL_NODE     "signal"
#define def_NAME_NODE           "name"
#define def_STR_VERIFYMSG_NODE  "verify_message"
#define def_STR_WAIT_NODE       "wait"
#define def_STR_REPLAY_NODE     "replay"
#define def_STR_STTRIB_PURPOSE  "purpose"
#define def_STR_MODULENAME      "Module Name"
#define def_STR_ENGINEERNAME    "Engineer Name"
#define def_STR_ENGINERROLE     "Engineer Role"
#define def_STR_VERSION         "version"

enum eTYPE_REPORT
{
    TXT = 0,
    HTM
};
enum eTIME_MODE
{
    ABS,
    REL
};
enum eTYPE_ENTITY
{
    BASE_ENTITY = -1,
    TEST_SETUP,
    TEST_CASE,
    WAIT,
    REPLAY,
    SEND,
    SEND_MESSAGE,
    VERIFY,
    VERIFY_MESSAGE,
    VERIFYRESPONSE
};

enum eTYPE_UNIT_SIGNAL
{
    RAW = 0,
    ENG
};
enum eERROR_ATTRIBUTE
{
    SUCCESS = 0,
    WARNING,
    ERRORS,
    FATAL
};
enum eACTION_EXCP
{
    CONTINUE = 0,
    EXIT
};
union tagUSIGNALVALUE
{

public:
    tagUSIGNALVALUE();
    float m_fValue;
    UINT64 m_u64Value;

};
class SInfo
{

public:
    CString m_omCategory;
    CString m_omValue;

    SInfo();
    virtual ~SInfo();
    VOID vInitialise(CString omCategory = CString(""), CString omValue=CString(""));
};


class SReportFile
{
public:
    eTYPE_REPORT m_eType;
    CString m_omPath;
    eTIME_MODE m_eTimeMode;
    SReportFile();
    virtual ~SReportFile();
    VOID vInitialise(eTYPE_REPORT eType = TXT, CString omPath = CString(""), eTIME_MODE eTimeMode = ABS);
};
