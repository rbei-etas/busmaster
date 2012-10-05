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
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for parsing functions
 */
#pragma once
#include <afxtempl.h>
#include<string.h>
#include<stdlib.h>
#include <set>
#include "ConstString.h"
#include "SymbolTable.h"

//#include "Converter.h"

#include "../DBC2DBFConverterLibrary/DBCConverterBase.h"

#define INVALID_FUNCTION        -1
#define CONDITIONAL_STATEMENT   1
#define VALID_FUNCTION          2

char acSptdKey[defINT_SizeofacAltKeyArray];//to store supported keys
CSymbolTable ouUnSptdKey;
//to store unsupported keys
CSymbolTable ouFuncName ;
//to store the name of utility function
CSymbolTable ouTimerName;
//to store the name of timer variable

typedef struct _MsgVariables
{
    CString m_omStrMsgName;
    CString m_omStrMsgType;
    _MsgVariables()
    {
        m_omStrMsgName.Empty();
        m_omStrMsgType.Empty();
    }
    _MsgVariables& operator=(const _MsgVariables& RefObj)
    {
        m_omStrMsgName = RefObj.m_omStrMsgName;
        m_omStrMsgType = RefObj.m_omStrMsgType;
        return *this;
    }
    _MsgVariables(const _MsgVariables& RefObj)
    {
        m_omStrMsgName = RefObj.m_omStrMsgName;
        m_omStrMsgType = RefObj.m_omStrMsgType;
    }
} MsgVariables;





char acAltKey[254];             //to store the alternate keys

CString ExceptionFormat = "In %s Exception occuered.\nFile : %s\nLine : %d";
char cIndex2 = 0;               //to store the acSptdKey (needed)
int  nSourceLine = 0;           //to count no. of lines in a source file
float  nUnCnvrtdLine = 0;       //to count no. of lines not converted
char cFlagLog = 1;              //to differentiate "write" and "writetolog" function
int m = 0 ;                     //neccessary
BOOL bPreStart = FALSE;         //flag for prestart
int linenumber = 0;
FILE* pFileLog;                 //for log file
//CString dest1;                //to store the path of log file (needed)
CString canoedb , BUSMASTERdb;  //to store the name of canoe and BUSMASTER database

enum eCALLTYPE
{
    TYPE_FUNCALL = 0,
    TYPE_FUNDEF
};
enum caplArgType
{
    CAPL_CHAR,
    CAPL_PCHAR,
    CAPL_INT,
    CAPL_PINT,
    CAPL_WORD,
    CAPL_PWORD,
    CAPL_LONG,
    CAPL_PLONG,
    CAPL_DWORD,
    CAPL_PDWORD,
    CAPL_FLOAT,
    CAPL_PFLOAT,
    CAPL_DOUBLE,
    CAPL_PDOUBLE,
    CAPL_BYTE,
    CAPL_PBYTE,
    CAPL_VOID,
    CAPL_PVOID,
    CAPL_MESSAGE,
    CAPL_PMESSAGE,
};

enum caplEventHandleState
{
    WRONG_HANDLER,
    CAPL_VARIABLE_HANDLER,
    CAPL_EVENT_KEY,
    CAPL_EVENT_MESSAGE,
    CAPL_EVENT_START,
    CAPL_EVENT_PRESTART,
    CAPL_EVENT_STOP,
    CAPL_EVENT_TIMER,
    CAPL_UTILITY_HANDLER,
    /*Don't change the following sequence*/
    CAPL_EVENT_BUSOFF,
    CAPL_EVENT_ERRORACTIVE,
    CAPL_EVENT_ERRORPASSIVE,
    CAPL_EVENT_WARNINGLIMIT,
    CAPL_EVENT_ERRORFRAME
    /*until this*/
};

typedef struct _SupportedFunctions
{
    CString m_omStrCaplFunName;
    CString m_omStrBMFunName;
    _SupportedFunctions& operator= (const _SupportedFunctions& RefObj)
    {
        m_omStrCaplFunName = RefObj.m_omStrCaplFunName;
        m_omStrBMFunName = RefObj.m_omStrBMFunName;
        return *this;
    }
} sSupportedFunctions;
typedef CArray<caplArgType, caplArgType&> listArgType;


typedef struct _FunctionPrototype
{
    CString m_omStrFunName;
    caplArgType m_ouRetType;
    listArgType m_ouArgList;
    CStringArray m_omStrFunNameArray;
    BOOL m_bMsgArgPresent;
    _FunctionPrototype& operator= (const _FunctionPrototype& ouObj)
    {
        m_omStrFunName = ouObj.m_omStrFunName;
        m_ouRetType = ouObj.m_ouRetType;
        m_ouArgList.RemoveAll();
        m_ouArgList.Append(ouObj.m_ouArgList);
        m_omStrFunNameArray.RemoveAll();
        m_omStrFunNameArray.Append(ouObj.m_omStrFunNameArray);
        m_bMsgArgPresent = ouObj.m_bMsgArgPresent;
        return *this;
    }
    _FunctionPrototype::_FunctionPrototype()
    {
        m_omStrFunName = " ";
        m_ouRetType = CAPL_VOID;
        m_ouArgList.RemoveAll();
        m_omStrFunNameArray.RemoveAll();
        m_bMsgArgPresent = FALSE;
    }
} FunctionPrototype;



char g_chErrorString[1024];
class GlobalVaribles
{
public:
    //Converter Class
    CDBCConverterBase* g_ouDBC2DBFConverter;
    //FunCtion PrototypeList
    CList<FunctionPrototype, FunctionPrototype&> g_ouFunctionPrototypeList;
    CArray<FunctionPrototype, FunctionPrototype&> g_ouLastFunProtoTypeList;
    //Current Capl state
    caplEventHandleState g_oucaplEventHandleState;
    //FunctionCalls
    INT nFunCalls;
    //Global Msg Count
    INT g_nGlobalMsgCount;
    //Ms Timers Array
    CStringArray g_omStrMsTimers;
    //Sec Timers Array
    CStringArray g_omStrSecTimers;
    //Function Calls Array
    CStringArray g_omStrFunCalls;
    //Last Function Handled used for comments
    CString omStrLastEvent;

    CArray<MsgVariables, MsgVariables&> g_ouGlobalMsgVariables;

    CArray<MsgVariables, MsgVariables&> g_ouLocalMsgVariables;

    set<string> g_ouWarningStrings;
    set<string> g_ouErrorStrings;

    CStringList g_omStrRepeatedMsg;

    BOOL m_bChannelSpecMsg;

private:

public:
    void vInitialise()
    {
        g_ouDBC2DBFConverter = NULL;
        g_ouFunctionPrototypeList.RemoveAll();
        g_ouLastFunProtoTypeList.RemoveAll();
        g_ouGlobalMsgVariables.RemoveAll();
        g_ouLocalMsgVariables.RemoveAll();
        g_oucaplEventHandleState = WRONG_HANDLER;
        nFunCalls = 0;
        g_nGlobalMsgCount = 0;
        g_omStrMsTimers.RemoveAll();
        g_omStrFunCalls.RemoveAll();
        g_omStrMsTimers.RemoveAll();
        omStrLastEvent.Empty();
        g_ouWarningStrings.clear();
        g_ouErrorStrings.clear();
        g_omStrRepeatedMsg.RemoveAll();
        m_bChannelSpecMsg = FALSE;
    }
};

GlobalVaribles g_ouGlobalVariables;
int nLineNum;
void vWriteLogFile();
INT nFormBMFunctionCall(CString& omStrFunction, CString& omStrBMFunction);
BOOL HandleArgument(FunctionPrototype& ouFunDetails, INT& nArgIndex, CString& omStrFunction );
BOOL isFunction(CString omStrArg);
BOOL bAddExport(CString& omFun);
void OnArgument();
void vReadFunctionCall(CString& omStrFunction);
VOID nRecFuNCall();
void vHandleByte(CString );
void vMsgDecl(CString& omStrValue, MsgVariables& ouMsgVariable);
void vKeyStoreCase();
void vCaplFunc();


//venkatanarayana
void bReplaceBMFunctionName(CString omStrName);
void vUpdateArraySize();
void SearchForRepeatedMessage();
void vHandleCaplSupportedFunction();
void vHandleSetTimerCall();
void vStoreTimers();
BOOL IsValidAlpaNum(char ch);

int onGlobalVariables();
int onConstGlobalVariables(bool bGlobal = true);
int onLocalVariables();
int ConvertToBMVariableDec(CString& omStrcaplVariable, bool bGlobal = true);
int onGlobalVariablesHeader(BOOL bStart);
void vFuNdef();
int GetUtilityFunctionPrototype(CString& omFunName, FunctionPrototype& ouFunType);
int isCaplSupportedFunction(CString& omFunName);
int GetBusmasterStorageType(CString& omStrCAPLType, CString&);
BOOL RemoveComments(CString&);
INT RetriveFunctionDetails(CString, FunctionPrototype& ouFunType, eCALLTYPE eType);
caplArgType GetArgTypeFromName(const CString& omStrName);
void GetNameFromArgType(caplArgType, CString&);
VOID vHandleFunctionCall();
void vHandleUtilityFun();
int nReplaceThisInFunction(CString& omStrText, CString& omStrThisText);
INT FormBMFunction(FunctionPrototype&, CString&);
void vHandleTypeCaste();
void vHandleMsgTypeCaste();
void vHandleOnPreStart();
void vHandleOnStart();
void vHandleOnstopMeasurement();
void vHandleOnmessage(CString& );
void vHandleOnkey(CString& omStrKey);
void vHandleCaplEvents();
BOOL bGetMessageObjType(CString omStrMsgObj, CString& omStrMsgType);

void vTokenize(CString strInput, CString strToken, CString& strOutput, int& nStart);

//~venkatanarayana
void vKeyStoreHeader();
void vHandleTimerDeclaration(CString&);
void vCnvrtThis();
void vCnvrtThisData();
void vHandleMessageMembers();

void vResetCan();
void vDisconnect();

void vHandleDot();
void vRemovePhys();

int  nConvert(CString omStrCAPLFile,CString omStrCFile ,CString omStrLogFile, CStringArray& omStrDbcFileArray, CDBCConverterBase*, CString& omStrResult);

