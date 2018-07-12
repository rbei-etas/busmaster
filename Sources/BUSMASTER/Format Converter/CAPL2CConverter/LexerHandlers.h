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
#include <map>
//#include "Converter.h"

#include "../DBC2DBFConverterLibrary/DBCConverterBase.h"


#define INVALID_FUNCTION        -1
#define CONDITIONAL_STATEMENT   1
#define VALID_FUNCTION          2

std::string g_MsgInitialize;
bool g_bBusConnectGenerated;
bool g_bGlobalVariable;

char acSptdKey[defINT_SizeofacAltKeyArray];//to store supported keys
CSymbolTable ouUnSptdKey;
//to store unsupported keys
CSymbolTable ouFuncName ;
//to store the name of utility function
CSymbolTable ouTimerName;
//to store the name of timer variable

typedef struct _FlxMsgVariables
{
    CString m_omStrMsgObj;  //FlexRay - FrPdu,FrFrame.
    CString m_omStrChannelMask; //FlexRay - A,B.
    CString m_omStrECU;
    CString m_omStrCluster;
    int m_nSlotId;
    int m_nBaseCycle;
    _FlxMsgVariables()
    {
        m_omStrMsgObj.Empty();
        m_omStrChannelMask.Empty();
        m_omStrECU.Empty();
        m_omStrCluster.Empty();
        m_nSlotId = 0;
        m_nBaseCycle = 0;
    }
    _FlxMsgVariables& operator=(const _FlxMsgVariables& RefObj)
    {
        m_omStrMsgObj = RefObj.m_omStrMsgObj;
        m_omStrChannelMask = RefObj.m_omStrChannelMask;
        m_omStrECU = RefObj.m_omStrECU;
        m_omStrCluster = RefObj.m_omStrCluster;
        m_nSlotId = RefObj.m_nSlotId;
        m_nBaseCycle = RefObj.m_nBaseCycle;
        return *this;
    }
    _FlxMsgVariables(const _FlxMsgVariables& RefObj)
    {
        *this = RefObj;
    }
} FlxMsgVariables;



typedef struct _MsgVariables
{
    FlxMsgVariables m_ouFlxMsgVar;
    CString m_omStrMsgName;
    CString m_omStrMsgType;
    _MsgVariables()
    {
        m_omStrMsgName.Empty();
        m_omStrMsgType.Empty();
        //memset(&m_ouFlxMsgVar,0,sizeof(m_ouFlxMsgVar));
    }
    _MsgVariables& operator=(const _MsgVariables& RefObj)
    {
        m_omStrMsgName = RefObj.m_omStrMsgName;
        m_omStrMsgType = RefObj.m_omStrMsgType;
        m_ouFlxMsgVar = RefObj.m_ouFlxMsgVar;
        return *this;
    }
    _MsgVariables(const _MsgVariables& RefObj)
    {
        m_omStrMsgName = RefObj.m_omStrMsgName;
        m_omStrMsgType = RefObj.m_omStrMsgType;
        m_ouFlxMsgVar = RefObj.m_ouFlxMsgVar;
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
bool bTimeStampNs = false;      //FlexRay for MessageTimeNs function.
int linenumber = 0;
FILE* pFileLog;                 //for log file
//CString dest1;                //to store the path of log file (needed)
CString canoedb , BUSMASTERdb;  //to store the name of canoe and BUSMASTER database

const std::string strBUS_TYPE[BUS_TOTAL]=
{
    "BUSMASTER", //Can Protocol must be replaced by BUSMASTER.
    "MCNET",
    "J1939",
    "FlexRay",
    "LIN",
    "MOST"
};


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
    CAPL_QWORD,
    CAPL_PQWORD,
    CAPL_INT64,
    CAPL_PINT64,
    CAPL_VOID,
    CAPL_PVOID,
    CAPL_MESSAGE,
    CAPL_PMESSAGE,
    /*FlexRay-Begin*/
    CAPL_PFRPDU,
    /*FlexRay-End*/
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
    CAPL_EVENT_ERRORFRAME,
    /*until this*/
    /*FlexRay Events Begins*/
    CAPL_EVENT_FRPDU,
    CAPL_EVENT_FRFRAME,
    CAPL_EVENT_FRNULLFRAME,
    CAPL_EVENT_FRSTARTCYCLE,
    CAPL_EVENT_FRPOCSTATE,
    /*FlexRay Events Ends*/
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
    ETYPE_BUS m_eBus;
    //ParseSate
    int g_nLastParseState;
    //Converter Class
    CDBCConverterBase* g_ouDBC2DBFConverter;
    //FlexRay Cluster list -- begin
    std::list<ClusterResult> g_ouClusterResult;
    ICluster* g_oupCluster;
    //FlexRay -- End


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
    //Function Wrapper Array
    CStringArray g_omStrFunWrappers;
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
        m_eBus = ETYPE_BUS::CAN;
        g_ouDBC2DBFConverter = NULL;
        g_ouClusterResult.clear();
        g_oupCluster = NULL;
        g_ouFunctionPrototypeList.RemoveAll();
        g_ouLastFunProtoTypeList.RemoveAll();
        g_ouGlobalMsgVariables.RemoveAll();
        g_ouLocalMsgVariables.RemoveAll();
        g_oucaplEventHandleState = WRONG_HANDLER;
        nFunCalls = 0;
        g_nGlobalMsgCount = 0;
        g_omStrMsTimers.RemoveAll();
        g_omStrFunCalls.RemoveAll();
        g_omStrFunWrappers.RemoveAll();
        g_omStrMsTimers.RemoveAll();
        omStrLastEvent.Empty();
        g_ouWarningStrings.clear();
        g_ouErrorStrings.clear();
        g_omStrRepeatedMsg.RemoveAll();
        m_bChannelSpecMsg = FALSE;
        g_nLastParseState = 1;
    }
};



class BusmasterKeyword
{
    std::map <std::string, std::string> m_KeywordMap;
public:
    BusmasterKeyword()
    {
        //FLEXRAY
        m_KeywordMap["time"] = "timeStamp";
        m_KeywordMap["time_ns"] = "timeStamp*1E5";
        m_KeywordMap["msgchannel"] = "cluster";
        m_KeywordMap["fr_slotid"] = "slot";
        m_KeywordMap["fr_cycle"] = "currentCycle";
        m_KeywordMap["fr_cycleoffset"] = "basecycle";
        m_KeywordMap["fr_cyclerepetition"] = "repetition";
        m_KeywordMap["fr_headercrc"] = "headerCrc";
        m_KeywordMap["fr_pdulength"] = "dlc";
        m_KeywordMap["fr_payloadlength"] = "dlc";
        m_KeywordMap["name"] = "name";
        m_KeywordMap["updatebit"] = "updateBit";
        m_KeywordMap["fr_pocstate"] = "pocStatus";
        m_KeywordMap["fr_channelmask"] = "flexrayChannel";
        m_KeywordMap["fr_payload"] = "data";
        m_KeywordMap["fr_flags"] = "headerInfo";

        //Ignore variables
        m_KeywordMap["pocstatus"] = "pocStatus";
        m_KeywordMap["cluster"] = "cluster";
        m_KeywordMap["data"] = "data";
        m_KeywordMap["dlc"] = "dlc";

        //Unsupported variables.
        m_KeywordMap["fr_segment"] = "unsupported";
        m_KeywordMap["fr_status"]  = "unsupported";
        m_KeywordMap["simulated"]  = "unsupported";
        m_KeywordMap["dir"]        = "unsupported";
        m_KeywordMap["fr_headercrc"] = "unsupported";
        m_KeywordMap["fr_info1"] = "unsupported";
        m_KeywordMap["fr_info2"] = "unsupported";
        m_KeywordMap["fr_info3"] = "unsupported";
        m_KeywordMap["fr_info4"] = "unsupported";
        m_KeywordMap["fr_nmlength"] = "unsupported";
        m_KeywordMap["fr_nmvector"] = "unsupported";
        m_KeywordMap["fr_correctionoffset"] = "unsupported";
        m_KeywordMap["fr_correctionrate"] = "unsupported";
        m_KeywordMap["fr_correctionfailedcounter"] = "unsupported";
        m_KeywordMap["fr_passivetoactivecounter"] = "unsupported";

        //CAN
        m_KeywordMap["id"] = "id";
        m_KeywordMap["dir"] = "RX";
        m_KeywordMap["can"] = "cluster";
        m_KeywordMap["rtr"] = "isExtended";

        //Ignore variables
        m_KeywordMap["m_ucchannel"] = "cluster";
        m_KeywordMap["m_ucdlc"] = "dlc";
    }

    std::string GetBusmasterEquivalent(std::string& strCaplKeyword)
    {
        std::string strRetVal = "";
        std::map<std::string, std::string>::iterator  itr = m_KeywordMap.find(strCaplKeyword);
        if (m_KeywordMap.end() != itr)
        {
            strRetVal = itr->second;
        }
        return strRetVal;
    }
};


BusmasterKeyword g_obBusmasterKeyword;
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
void vMsgDecl(CString& omStrValue, MsgVariables& ouMsgVariable, bool bGlobal);
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

int onVaribleDecl(bool bGlobal = true);
int onConstVaribleDecl(bool bGlobal = true);
int onCaplVaribleDecl(bool bGlobal = true);
int onGlobalVariables();
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

void vHandleCanCaplEvents();


void getCaplEvent(CString& caplEvent, CString& caplEventFunction);
//BOOL bGetMessageObjType(CString omStrMsgObj, CString& omStrMsgType);
bool bGetMessageObj(const CString& omStrMsgObj,MsgVariables& ouMsgVariable);

bool bGetBMFrameName(std::string& strFrame);


void vTokenize(CString strInput, CString strToken, CString& strOutput, int& nStart);

//~venkatanarayana
void vKeyStoreHeader();
void vHandleTimerDeclaration(CString&);

void vCnvrtThis();
void vCnvrtThisData();

void vHandleUnsupportedFunctionEnd();
void vHandleUnsupportedLine();

void vResetCan();
void vDisconnect();

std::string getBusmasterKeyword();
void vHandleDot();
void vRemovePhys();

int  nConvert(ETYPE_BUS eBus, CString omStrCAPLFile, CString omStrCFile, CString omStrLogFile, CStringArray& omStrDbcFileArray, CDBCConverterBase*, CString& omStrResult);

