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
 * \file      HashDefines.h
 * \author    Amarnath Shastry, Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#ifndef _HASH_DEFINES_H
#define _HASH_DEFINES_H

#define DATA_FMT_INTEL          1

#include "Include/BaseDefs.h"

typedef enum eDLLHANDLER
{
    DLL_LOAD = 0,
    DLL_UNLOAD
};

//// This enumeration defines current error state
//enum eERROR_STATE
//{
//    ERROR_ACTIVE  =   0,
//    ERROR_WARNING_LIMIT,
//    ERROR_PASSIVE,
//    ERROR_BUS_OFF,
//    ERROR_FRAME
//};


//The structure to store message and hmodule for the messages
//transmitted from a DLL
struct sDLL_MSG
{
    void* sRxMsg;
    HMODULE h_DllHandle;
};
typedef sDLL_MSG SDLL_MSG;
typedef SDLL_MSG* PSDLL_MSG;

struct sTHREAD_INFO
{
    HANDLE m_hThread;
    VOID*  m_pvThread;
};

typedef sTHREAD_INFO  STHREADINFO;
typedef sTHREAD_INFO* PTHREADINFO;


#define SPLT_ONE_ROWS   1
#define SPLT_ONE_COLS   2

#define SPLT_TWO_ROWS   2
#define SPLT_TWO_COLS   1

#define BUS_FN_HDR           _T("/* Start PLACE_HODLER_FOR_BUSNAME generated function - \
PLACE_HODLER_FOR_FUNCTIONNAME */")

#define BUS_FN_FOOTER        _T("}/* End PLACE_HODLER_FOR_BUSNAME generated function - \
PLACE_HODLER_FOR_FUNCTIONNAME */")

#define FRAME_FN_PARTIAL_HDR   _T("/* Start PLACE_HODLER_FOR_BUSNAME generated function - ")

#define BUS_VAR_HDR          _T("/* Start PLACE_HODLER_FOR_BUSNAME global variable */")
#define BUS_VAR_FOOTER       _T("/* End PLACE_HODLER_FOR_BUSNAME global variable */")

#define BUS_INCLUDE_HDR      _T("/* Start PLACE_HODLER_FOR_BUSNAME include header */")
#define BUS_INCLUDE_FOOTER   _T("/* End PLACE_HODLER_FOR_BUSNAME include header */")

#define BUS_FN_PROTOTYPE_HDR _T("/* Start PLACE_HODLER_FOR_BUSNAME Function Prototype  */")
#define BUS_FN_PROTOTYPE_FOOTER   _T("/* End PLACE_HODLER_FOR_BUSNAME Function Prototype  */")

#define UTILS_PREFIX            _T("Utils_")
#define OPEN_PARENTHESIS        _T("(")
#define NO_OF_CHAR_IN_UTILS     6

#define GLOBAL_VAR_PREFIX         _T("g_")
#define MIN_CHAR_IN_GLOBAL_VAR    2
#define FILENAME_NOT_FOUND        _T("File not found!")

#define defDOT_C                    _T(".C")
#define defDOT_DEF                  _T(".def")
#define defDOT_EXP                  _T(".exp")
#define defDOT_DLL                  _T(".dll")

#define MESSAGE_HANDLERS  _T("Message Handlers")
#define TIMER_HANDLERS    _T("Timer Handlers")
#define KEY_HANDLERS      _T("Key Handlers")
#define ERROR_HANDLERS    _T("Error Handlers")
#define EVENTIND_HANDLERS _T("Event Handlers")
#define DLL_HANDLERS      _T("DLL Handlers")
#define UTILITY_FUNCS     _T("Utility Functions")
#define GLOBAL_VARIABLES  _T("Global Variables")
#define INCLUDE_HEADERS   _T("Include Headers")

#define SPACE                   _T(' ')
#define SEMI_COLON              _T(';')
#define defDEFAULT_KEY_HANDLER_CODE _T("void OnKey_KEYNAME(unsigned char KeyValue)")


/*#define defDEFAULT_TIMER_HANDLER_CODE   _T("void __stdcall OnTimer_TIMERNAME_TIMERVALUE(unsigned int uTimerID,\
unsigned int uMsg, unsigned long dwUser, unsigned long dw1, unsigned long dw2)")*/
#define defDEFAULT_TIMER_HANDLER_CODE _T("void OnTimer_TIMERNAME_TIMERVALUE( )")

#define defTODO_FUNCTION_INSERT     _T("/* TODO: Define and Initialise the following variables*/")
#define defNEW_LINE                 _T("\n")
#define defRETURN_TYPE_NAME         _T("ReturnValue")
#define defPARAMETER_NAME           _T("Para%d")
#define defTODO                     _T("/* TODO */")
#define defMSG_DUPL_MSG_HANDLER     _T("Message handler for TYPE MESSAGENAME already exists!")
#define defDUPL_CORR_MSG_HANDLER     _T("Corresponding Message handler for TYPE \"MESSAGENAME\" already exists!")
#define defMSG_DUPL_TMR_HANDLER     _T("Timer handler already exists!")
#define defMSG_DUPL_KEY_HANDLER     _T("Key handler for the key \"KEYNAME\" already exists!")
#define defMSG_DUPL_UTIL_FUNC       _T("Utility function \"FUNCTIONNAME\" already exists!")
#define defMSG_DUPL_GLO_VAR         _T("Global variable is already defined!")
#define defINIT_HANDLER_STRUCT_MSG  _T("Unable to initialise handler functions for execution.")
#define defDLL_UNLOADED_MSG         _T("The DLL \"FILENAME\" is unloaded")
#define defOBJECT_BUILDPROGRAM_MSG  _T("Unable to allocate memory for BuildProgram module")
#define defSTRMSG_RANGE_INVALID     _T("Message ID range:\"FROM\" field has value greater then or equal to \"TO\" field")
#define defMESSAGE_HANDLER         _T("OnMsg")
#define defPGN_HANDLER             _T("OnPGN")
#define defKEY_HANDLER             _T("OnKey_")
#define defCON_IND_HANDLER         _T("OnConnectionInd_")
#define defTIMER_HANDLER           _T("OnTimer_")
#define defERROR_HANDLER_FN        _T("OnError_")
#define defDLL_HANDLER_FN          _T("OnDLL_")
#define defEVENT_IND_HANDLER       _T("OnEvent_")
#define defUTILS_FUNCTION_FN       _T("Utils_")
#define defMSG_NAME_HANDLER        _T("Name_")
#define defMSG_ID_HANDLER          _T("ID_")
#define defMSG_ALL                 _T("_All")
#define defMSG_IDRANGE_HANDLER     _T("IDRange_")
#define defMSGID_NAME_START_CHAR   '['

#define WM_LOAD_UNLOAD              (WM_USER + 39) //If you are changing this change in McNet/CommonDefs.h accordingly
// Prefix of a handler
#define HANDLER_PRFX_DLL           _T("OnDLL_")

// Suffix of a handler
#define HANDLER_SFX_LOADDLL        _T("Load")
#define HANDLER_SFX_UNLOADDLL      _T("Unload")

// Function Editor
#define defCONFIRM_DELETE_HANDLER   _T("You are about to delete the function FUNCTION_NAME(...).\n\
Are you sure you want to delete?")

#define defVOID                     _T("void")
#define defCONFIRM_DELETE_GLOVAR    _T("You are about to delete the variable.\n\
Do you wish to proceed?")

#define defCONFIRM_DELETE_INCHDR    _T("You are about to delete the #include.\n\
Are you sure you want to delete?")
#define defDEFAULT_MSG_HANDLER_CODE     _T("void %s%s(%s* RxMsg)")
#define defDEFAULT_MSG_HANDLER_CODE_CAN _T("void %s%s(%s RxMsg)")
#define defDEFAULT_KEY_HANDLER_CODE _T("void OnKey_KEYNAME(unsigned char KeyValue)")
#define defDEFAULT_CON_IND_CODE     _T("void %s( )")
#define defDEFAULT_TIMER_HANDLER_CODE _T("void OnTimer_TIMERNAME_TIMERVALUE( )")

#define defERROR_HANDLER_NUMBER  5
#define defDLL_HANDLER_NUMBER    2
#define defERROR_HANDLER_TEXT_ADD _T("Add Error Handler(s)")
#define defERROR_HANDLER_TEXT_DEL _T("Delete Error Handler(s)")
#define defDLL_HANDLER_TEXT_ADD   _T("Add DLL Handler(s)")
#define defDLL_HANDLER_TEXT_DEL   _T("Delete DLL Handler(s)")
#define defGENERIC_KEY            _T('*')
#define defGENERIC_KEY_HANDLER_TEXT  _T("All")
// Signal Type
#define defBOOLEAN          _T("bool")
#define defUNSIGNED_INT     _T("unsigned int")
#define defUNSIGNED_CHAR    _T("unsigned char")
#define defUNSIGNED_LONG    _T("unsigned long")
#define defUNSIGNED_SHORT   _T("unsigned short")
#define defSIGNED_SHORT     _T("short")
#define defSIGNED_CHAR      _T("char")
#define defSIGNED_INT       _T("int")
#define defSIGNED_LONG      _T("long")
#define defFLOAT            _T("float")

#define defMSG_NAME_TEXT        _T("message name")
#define defMSG_ID_TEXT          _T("message id")
#define defMSG_IDRANGE_TEXT     _T("message id range")
#define defMSG_ALL_TEXT         _T("all")
#define defMSG_HANDLER_ARG_TYPE1  _T("void*")
#define defCANMSG_HANDLER_ARG_TYPE  _T("STCAN_MSG")
#define defSTR_MSG_SPECIFIC_HANDLER _T("Name_")
#define defMESSAGE_NAME_INDEX       5
#define defCHAR_EQUALS           '='
#define defFNS_COMMA             _T(",")
#define defFNS_INIT_FORMAT       _T("%s %s %s")
#define defFNS_INIT_SIG_FORMAT   _T("%s\n\n%s.%s")
#define defFNS_INIT_VAL          _T(" 0")
#define defSPACE_STR            _T(" ")
#define defMSG_INIT_FORMAT_CAN       _T("= { 0x%X, %d, %d, %d, %c")
#define defMSG_INIT_FORMAT_MCNET       _T("= { %d, 0x%X, %d, %d, %d, %d")
#define defMSG_INIT_FORMAT_J1939       _T("= { {%d, %c, %s, %s, 0x%X}, %d")

#define defMSG_TYPECAST_CODE    _T("%s *pMsg = (%s*) RxMsg;")

#define MSG_RET_TYPE_EMPTY      _T("Please select the return type for the function.")

enum eSELTYPE
{
    SEL_MESSAGE = 0,
    SEL_SIGNAL,
    SEL_GLOBAL_MESSAGE
};
#define defNUMBER_OF_TIMER           100
#define defFNS_INIT_SIG_FORMAT              _T("%s\n\n%s.%s")
#define defHASHINCLUDE                      _T("#include ")
#define defSIGNALMEMBER                     _T("m_sWhichBit")
#define defSTR_INSERT_GLOBAL_DLG_CAPTION    _T("Message Variable List")
#define defUNDERSCORE                       _T("_")
#define MSG_STRUCT_VAR                      _T("sMsgStruct")
#define PERIOD                              _T('.')
#define NEW_LINE                            _T("\n")
#define TVOID                               _T("void")
#define defMSG_DUP_TIMER_HANDLER            _T("Duplicate timer handler name!")
#define defTIMER_FN_NAME                    _T("OnTimer_TIMERNAME_TIMERVALUE")
/*#define defDEFAULT_TIMER_HANDLER_CODE   _T("void __stdcall OnTimer_TIMERNAME_TIMERVALUE(unsigned int uTimerID,\
unsigned int uMsg, unsigned long dwUser, unsigned long dw1, unsigned long dw2)")*/
#define defDEFAULT_TIMER_HANDLER_CODE       _T("void OnTimer_TIMERNAME_TIMERVALUE( )")
#define STR_EMPTY                           _T("")
#define STR_INCLUDE_FILE                    _T("\n#include <%s>")


//no. of entries in CExecute func message handler buffer
#define defMAX_FUNC_MSG                     1000
//no. of entries in global message handler structure buffer
#define defMAX_HMODULE_MSG                  1000
#define defCONFIG_FILE_LOADING              0
#define defCONFIG_FILE_SAVING               1
#define defCONFIG_FILE_NEW                  2


#define defSIMVALIDEXTN        _T(".sim")
#define defDOT_SMALL_C         _T(".c")
#define defDOT                 _T('.')

//Parameters for the CEvent objects array used in CExecuteFunc
#define defEVENT_EXFUNC_TOTAL        6
//define for CExecute utility thread
#define defKEY_HANDLER_THREAD        0
#define defERROR_HANDLER_THREAD      1
#define defDLL_LOAD_HANDLER_THREAD   2
#define defDLL_UNLOAD_HANDLER_THREAD 3
#define defMSG_HANDLER_THREAD        4
#define defEVENT_HANDLER_THREAD      5


#define NAME_BUS                     _T("CAN Bus")
#define defERROR_IN_REGISTRY         _T("Unable to obtain path of GCC from registry key")
#define defERROR_OPEN_REGISTRY       _T("The registry value is corrupted. Please re-install.")
#define defERROR_OPEN_REGISTRY_FN    _T("The registry value is corrupted. The interface function is not available.\nPlease re-install.")
#define defIMPORT_DLLLOAD_WARNNING   _T("Please Unload all the Dlls and try again")

// All function prototype is used in function editor.
typedef VOID (__cdecl* PFMSG_HANDLER_CAN)(STCAN_MSG Rx_Msg);
typedef VOID (__cdecl* PFMSG_HANDLER)(void* pRxMsg);
typedef VOID (__cdecl* PFTIMER_HANDLER)();
typedef VOID (__cdecl* PFKEY_HANDLER)(UCHAR ucKeyVal);
typedef VOID (__cdecl* PFEVENT_HANDLER)(...);
typedef VOID (__cdecl* PFERROR_HANDLER)(SCAN_ERR ErrorMsg);
typedef VOID (__cdecl* PFDLL_HANDLER)();
typedef BOOL (__cdecl* PFGET_PRG_VER)(int*,int*,HMODULE);
// Used is application call back function
typedef VOID (CALLBACK* APPTIMERPOINTER)(UINT,UINT,DWORD,DWORD,DWORD);


// This structure definition is for storing all information about a timer handler
// defined by user.
struct sTIMERHANDLER
{
    CString         omStrTimerHandlerName; // Name of Timer Handler Function
    BOOL            bTimerType;            // Type : Cyclic or Monshot
    BOOL            bTimerSelected;        // Selected or not for execution
    UINT            unTimerVal;            // the time-out value, in milliseconds
    PFTIMER_HANDLER pFTimerHandler;        // Long pointer to the function to be
    // notified when the time-out value
    // elapses.
    UINT            unTimerID;             // Specifies a nonzero timer identifier
    //    BOOL            bIsExecuting;          // Timer call back is under execution
    UINT            unCurrTime; //(ani1)
    CEvent          omWaitEvent;
    //  sTIMERHANDLER*  psNextTimer;
    HANDLE          hDllHandle;
    CWinThread*     pomThreadHandle;
};
typedef sTIMERHANDLER  STIMERHANDLER;
typedef STIMERHANDLER* PSTIMERHANDLER;


//this structure store STIMERHANDLER and pointer to make a list
struct sTIMERHANDLERLIST
{
    STIMERHANDLER sTimerHandler;
    sTIMERHANDLERLIST* psNextTimer;
};
typedef sTIMERHANDLERLIST  STIMERHANDLERLIST;
typedef STIMERHANDLERLIST*  PSTIMERHANDLERLIST;

//This structure stores node name and pointer to the list of timer of
//the node
struct sNODETIMERLIST
{
    PSTIMERHANDLERLIST psTimerListPtr;
    CString omNodeName;
    sNODETIMERLIST* psNextTimerListPtr;
};
typedef sNODETIMERLIST  SNODETIMERLIST;
typedef SNODETIMERLIST*  PSNODETIMERLIST;

class CExecuteFunc;
//New data structure to store  node name and the "CExecuteFunc" pointer associated to that node:
struct sNODEOBJECT
{
    CString om_NodeName;
    CExecuteFunc* m_psExecuteFunc;
    sNODEOBJECT* m_psNextNode;
};
typedef  sNODEOBJECT SNODEOBJECT;
typedef  SNODEOBJECT* PSNODEOBJECT;

// This structure stores the DLL Handler and pointer to its corresponding function
// to be executed on press of the key.
struct sDLLHANDLER
{
    eDLLHANDLER  m_eDLLHandler;           // DLL Handler
    PFDLL_HANDLER  m_pFDLLHandlers;       // Long pointer to the function to be
    // executed on DLL Load\Unload
};
typedef sDLLHANDLER  SDLLHANDLER;
typedef sDLLHANDLER* PSDLLHANDLER;

// This structure stores the error code and pointer to its corresponding function
// to be executed on press of the key.
struct sERRORHANDLER
{
    eERROR_STATE    m_eErrorCode;         // Error value
    PFERROR_HANDLER m_pFErrorHandlers;    // Long pointer to the function to be  executed on error condition
    SCAN_ERR sErrorVal;                   // parameter to be passed to the error handler
    CExecuteFunc* m_pCExecuteFunc;        //to pass this pointer to access its variable
};
typedef sERRORHANDLER  SERRORHANDLER;
typedef sERRORHANDLER* PSERRORHANDLER;

// This structure stores the event handler info and pointer to its corresponding function
// to be executed.
struct sEVENTHANDLER
{
    BYTE   m_byEventType;         // Error value
    // Long pointer to the function to be  executed on error condition
    UINT32 m_unPGN;
    BYTE   m_bySrc;
    BYTE   m_byDest;
    BOOL   m_bSuccess;
    PFEVENT_HANDLER m_pFEventHandlers;
    CExecuteFunc* m_pCExecuteFunc;        //to pass this pointer to access its variable
};
typedef sEVENTHANDLER  SEVENTHANDLER;
typedef sEVENTHANDLER* PSEVENTHANDLER;

// This structure stores the key value and pointer to its corresponding function
// to be executed on press of the key.
struct sKEYHANDLER
{
    UCHAR          ucKey;                 // Key value
    PFKEY_HANDLER  pFKeyHandlers;         // Long pointer to the function to be
    // executed on press of corresponding
    // key
};
typedef sKEYHANDLER  SKEYHANDLER;
typedef SKEYHANDLER* PSKEYHANDLER;



struct sMSG_RANGE
{
    UINT    m_unFrom;
    UINT    m_unTo;
};

typedef sMSG_RANGE SMSG_RANGE;
typedef sMSG_RANGE* PSMSG_RANGE;


struct sMSGINFO_FOR_HANDLER
{
    // For message handler defined on range of messages
    UINT        m_unRangeCount;
    PSMSG_RANGE m_psRange;

    // For a generic message handler
    BOOL    m_bGenericHandlerON;
};
typedef sMSGINFO_FOR_HANDLER SMSGINFO_FOR_HANDLER;
typedef sMSGINFO_FOR_HANDLER* PSMSGINFO_FOR_HANDLER;

struct sMSGID_HANDLER_CAN
{
    UINT    m_unMsgID;
    PFMSG_HANDLER_CAN m_pFMsgHandler;
};

typedef sMSGID_HANDLER_CAN SMSGID_HANDLER_CAN;
typedef sMSGID_HANDLER_CAN* PSMSGID_HANDLER_CAN;

//This is used for all the buses except CAN
struct sMSGID_HANDLER
{
    UINT    m_unMsgID;
    PFMSG_HANDLER m_pFMsgHandler;
};

typedef sMSGID_HANDLER SMSGID_HANDLER;
typedef sMSGID_HANDLER* PSMSGID_HANDLER;

struct sMSGID_RANGE_HANDLER_CAN
{
    SMSG_RANGE m_sMsgIDRange;
    PFMSG_HANDLER_CAN m_pFMsgHandler;
};

typedef sMSGID_RANGE_HANDLER_CAN SMSGID_RANGE_HANDLER_CAN;
typedef sMSGID_RANGE_HANDLER_CAN* PSMSGID_RANGE_HANDLER_CAN;

//This is used for all the buses except CAN
struct sMSGID_RANGE_HANDLER
{
    SMSG_RANGE m_sMsgIDRange;
    PFMSG_HANDLER m_pFMsgHandler;
};

typedef sMSGID_RANGE_HANDLER SMSGID_RANGE_HANDLER;
typedef sMSGID_RANGE_HANDLER* PSMSGID_RANGE_HANDLER;

struct sEXECUTE_MSG_HANDLER_CAN
{
    STCAN_MSG m_sRxMsg;
    PFMSG_HANDLER_CAN m_pFMsgHandler;
    CExecuteFunc* m_pCExecuteFunc;
};
typedef sEXECUTE_MSG_HANDLER_CAN SEXECUTE_MSG_HANDLER_CAN;
typedef sEXECUTE_MSG_HANDLER_CAN* PSEXECUTE_MSG_HANDLER_CAN;

//This is used for all the buses except CAN
struct sEXECUTE_MSG_HANDLER
{
    void* m_pRxMsg;
    PFMSG_HANDLER m_pFMsgHandler;
    CExecuteFunc* m_pCExecuteFunc;
};
typedef sEXECUTE_MSG_HANDLER SEXECUTE_MSG_HANDLER;
typedef sEXECUTE_MSG_HANDLER* PSEXECUTE_MSG_HANDLER;

struct sEXECUTE_KEY_HANDLER
{
    UCHAR m_ucKeyValue;
    PFKEY_HANDLER m_pFKeyHandler;
    CExecuteFunc* m_pCExecuteFunc;
};
typedef sEXECUTE_KEY_HANDLER SEXECUTE_KEY_HANDLER;
typedef sEXECUTE_KEY_HANDLER* PSEXECUTE_KEY_HANDLER;

struct sMsgHandlerDataCan
{
    WORD wMsgCode;
    PFMSG_HANDLER_CAN m_pFMsgHandler; // Pointer to Message Handler function
    UINT unDLC;                   // Data Length Count
    int nMsgFormat;               // Big/Little Endian
    sMsgHandlerDataCan()             // To init pointers and data members
    {
        wMsgCode = 0;
        m_pFMsgHandler = NULL;
        unDLC = 0;
        nMsgFormat = -1;
    }
    BOOL operator==(const sMsgHandlerDataCan& RefObj) const
    {
        BOOL bReturn = FALSE;

        if (RefObj.wMsgCode == wMsgCode)
        {
            bReturn = TRUE;
        }

        return bReturn;
    }

};
// typedef for sMsgHandlerData and pointer to sMsgHandlerData
typedef sMsgHandlerDataCan SMSGHANDLERDATA_CAN;
typedef SMSGHANDLERDATA_CAN* PSMSGHANDLERDATA_CAN;

struct sMsgHandlerData
{
    UINT unMsgId;
    PFMSG_HANDLER m_pFMsgHandler; // Pointer to Message Handler function
    UINT unDLC;                   // Data Length Count
    int nMsgFormat;               // Big/Little Endian
    sMsgHandlerData()             // To init pointers and data members
    {
        unMsgId = 0;
        m_pFMsgHandler = NULL;
        unDLC = 0;
        nMsgFormat = -1;
    }
    BOOL operator==(const sMsgHandlerData& RefObj) const
    {
        BOOL bReturn = FALSE;

        if (RefObj.unMsgId == unMsgId)
        {
            bReturn = TRUE;
        }

        return bReturn;
    }

};
// typedef for sMsgHandlerData and pointer to sMsgHandlerData
typedef sMsgHandlerData SMSGHANDLERDATA;
typedef SMSGHANDLERDATA* PSMSGHANDLERDATA;

// error defines for CConfigDetails class..
#define defCONFIG_FILE_ERROR                1000
#define defCONFIG_FILE_OPEN_FAILED          defCONFIG_FILE_ERROR+2
#define defCONFIG_FILE_CLOSE_FAILED         defCONFIG_FILE_ERROR+5
#define defCONFIG_FILE_INVALID_FILE_EXTN    defCONFIG_FILE_ERROR+6
#define defCONFIG_FILE_CORRUPT              defCONFIG_FILE_ERROR+7
#define defCONFIG_FILE_HDR_CORRUPT          defCONFIG_FILE_ERROR+8
#define defCONFIG_FILE_NOT_FOUND            defCONFIG_FILE_ERROR+9
#define defCONFIG_FILE_VERSION_ERR          defCONFIG_FILE_ERROR+10
#define defCONFIG_FILE_SUCCESS              10
#define defFILE_LOADED                      11

#define defCONFIG_FILE_LOADING              0
#define defCONFIG_FILE_SAVING               1
#define defCONFIG_FILE_NEW                  2


#define defMAJOR_VERSION                    1
#define defMINOR_VERSION                    0
//To get the Version Number from the application DLL
#define defUNSUPPORTED_VER                  _T("Unsupported DLL Version %d.%d")
#define defOLD_VER                          _T("%s is created with old version of McNet. Please recompile the source code to get latest version of DLL")
#define defREGISTRY_LOCATION      _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\FRAME.exe")
#define def_MSG_MAP_HASH_SIZE     257
#define defSTR_ERROR_IN_MSG_PROG   "Exception occured in Message Handler ID : 0x%X"
#define defSTR_ERROR_IN_PGN_PROG   "Exception occured in PGN Handler ID : 0x%X"
#define defSTR_ERROR_IN_KEY_PROG   "Exception occured in Key Handler of '%c'"
#define defSTR_ERROR_IN_EVENT_PROG   "Exception occured in Event Handler"
#define defSTR_ERROR_IN_ERR_PROG   "Exception occured in Error Handler"
#define defSTR_ERROR_IN_DLL_LOAD   "Exception occured in DLL Load Handler"
#define defSTR_ERROR_IN_DLL_UNLOAD "Exception occured in DLL UnLoad Handler"
#define defSTR_ERROR_IN_TIMER_PROG "Exception occured in Timer Handler \"%s\""
const BYTE BIT_MSG_HANDLER_THREAD       = 0x01; // First bit
const BYTE BIT_KEY_HANDLER_THREAD       = 0x02; // Second bit
//const BYTE BIT_TX_SEL_MSG_THREAD        = 0x04; // Third bit
//const BYTE BIT_REPLAY_THREAD            = 0x08; // fourth bit
const BYTE BIT_DLL_LOAD_HANDLER_THREAD   = 0x10; // fifth bit
const BYTE BIT_DLL_UNLOAD_HANDLER_THREAD = 0x20; // sixth bit
const BYTE BIT_ERROR_HANDLER_THREAD      = 0x40; // Seventh bit
//const BYTE BIT_MULTI_MSG_THREAD          = 0x80; // Eighth bit
#define defSTR_ERROR_LOADING_HANDLER    _T("Failed to get address of %s Handlers: %s")
#define defSTR_DLL_HANDLER  _T("DLL")
#define defSTR_KEY_HANDLER  _T("Key")
#define defSTR_EVENT_HANDLER  _T("Event")
#define defSTR_MESSAGE_HANDLER  _T("Message")
#define defSTR_ERROR_HANDLER  _T("Error")
#define defSTR_TIMER_HANDLER  _T("Timer")


// This structure definition is to store a list of simulated systems whose paths have
// to be stored in the main configuration file.
struct sSIMSYSARRAY
{
    CString m_omStrSimSysPath;          // simulated system path.
    CString m_omStrSimSysName;          // simulated system name.
    sSIMSYSARRAY* psNextSimsys;         // Pointer to the next simulated system.
    sSIMSYSARRAY()
    {
        //      m_omStrSimSysPath = NULL;
        //      m_omStrSimSysName = NULL;
        psNextSimsys = NULL;
    }

};
typedef sSIMSYSARRAY SSIMSYSARRAY;
typedef SSIMSYSARRAY* PSSIMSYSARRAY;

#define defMASK_CHECK_UNCHECK    0x00003000
#define defMASK_LIST_CTRL        0x00003003

#define ALL_NODE_EDIT_CONFMN     _T("You are about to change all the Node details!\n\nAre you sure you want to Continue?")
#define ALL_SIMSYS_DELETE_CONFMN _T("You are about to Delete all the Simulated Systems.\n\nAre you sure you want to Delete?")
#define ASK_SIMSYS_SAVE_PROMPT   _T("Do you want to save changes to the simulated system file(s)?")
#define defSIZE_OF_ERROR_BUFFER  1024
#define defSIZE_OF_IMAGE         25
#define defSTR_BUILD_TRACE_LINE_MARK       _T(" ")
#define defTIMER_HEADER_NAME               _T("Timer Handler Name")
#define defTMPFILEEXTN                     _T("tmp")
#define MSG_MEMORY_CONSTRAINT              _T("Insufficient Memory available")
#define SIMSYS_DELETE_CONFMN     _T("You are about to Delete a Simulated System.\n\nAre you sure you want to Delete?")
#define ALL_SIMSYS_DELETE_CONFMN _T("You are about to Delete all the Simulated Systems.\n\nAre you sure you want to Delete?")
#define ALL_NODE_EDIT_CONFMN     _T("You are about to change all the Node details!\n\nAre you sure you want to Continue?")
#define NODE_DELETE_CONFMN       _T("You are about to Delete a Node.\n\nAre you sure you want to Delete?")
#define ALL_NODE_DELETE_CONFMN   _T("You are about to Delete all the Nodes.\n\nAre you sure you want to Delete?")
#define COPYFILE(SRC,DESTN)         ::CopyFile(SRC,DESTN,FALSE);
#define defCHANGEDSIMSYS           0x0000000000000001

#define defTMPFILEATTRIB    FILE_ATTRIBUTE_HIDDEN
#define defEMPTYSTR         _T("")
#define defSAVECONFIGERRSTR _T("Error while saving file %s. Restoring old data")// for sim sys 
#define defCONFIGSAVEERR    _T("File %s not saved properly. Please try again")// for sim sys 
#define defSIMSYSCOPYRIGHT  _T("RBEI_FRAME")
#define defSIMSYSVERSION_CAN 1.1
#define defSIMSYSVERSION_ALL_BUS 1.2
#define defCHAR_OPEN_PARENTHESIS    '{'
#define defCHAR_CLOSE_PARENTHESIS   '}'

#define defOPEN_PARENTHESIS      _T(", {")
#define defCLOSE_PARENTHESIS     _T(" }")
#define SEMI_COLON_STR           ";"

enum eMESSAGEFROM
{
    FNTREEVIEW = 0,
    OUTWND,
    FNVIEW,
    SENDMESGBUTTON
};
// enumerators for obtaining information from the configuration file
enum eACONFIGDETAILS
{
    OUT_WND_PLACEMENT = 0,
    SIMSYS_WND_PLACEMENT
};
//The structure pass to thread function for load Fn execution
struct sEXECUTE_LOAD_HANDLER
{
    PFDLL_HANDLER pFDllHandler;
    CExecuteFunc* m_pCExecuteFunc;
};
typedef sEXECUTE_LOAD_HANDLER SEXECUTE_LOAD_HANDLER;
typedef sEXECUTE_LOAD_HANDLER* PSEXECUTE_LOAD_HANDLER;

#define BLUE_COLOR              RGB(0,0,255)
#define DEFAULT_FONT            _T("Courier")
#define DEFAULT_FONT_ESCAPEMENT  0
#define DEFAULT_FONT_ORIENTATION 0
#define NOT_ITALIC               0
#define NO_UNDERLINE             0
#define NO_STRIKEOUT             0

#define defCHAR_PATH_SEPRATER    _T('\\')
#define defHEADER_FILE_NAME                 _T("_Unions.h")
#define defWAIT_DELAY_FOR_DLL_UNLOAD        500

//Color constants
#define WHITE_COLOR             RGB(255,255,255)
#define RED_COLOR               RGB(255,0,0)
#define BLUE_COLOR              RGB(0,0,255)
#define CONTEX_WND_COLOR        RGB(255,255,200) // color for context window
#define GREEN_COLOR             RGB(0,255,0)
#define TREE_BCK_COLOR          RGB(215,215,215) // Tree background color
#define BLACK_COLOR             RGB(0,0,0)
#define DIALOG_COLOR            RGB(150,150,150)
#define DEFAULT_MSG_COLOUR      RGB(0, 0, 0)

#define DEFAULT_X_POS           0
#define DEFAULT_Y_POS           0
#define DEFAULT_HEIGHT          0
#define DEFAULT_WIDTH           0
#define INCR_LEN                1

#define defNO_OF_CHARS_IN_TAB   4
#define MARGIN_FOR_FILE_VIEW    5
#define TAB_POSITION            1
#define TAB_ORIGIN              0

#define INT_INIT                (INT)0
#define LONG_INIT               (LONG)0
#define DWORD_INIT              (DWORD)0
#define CHAR_INIT               (CHAR)0
#define defCOUNT_INIT              0//initial value of a counter

#define NEXT_POSITION           1
#define START_POSITION          0

#define SPACE_BET_CHAR_IN_FILE_VIEW 20
#define SPACE_BET_LINE_IN_FILE_VIEW 5
#define IDC_LSTB_OUTPUT             300


#define GET_CONTEXT_WND_CHAR_HEIGHT() \
    ((::GetDeviceCaps(hDeviceContext,LOGPIXELSY) * 10) / 72)
#define GET_CONTEXT_WND_CHAR_WIDTH() \
    ((::GetDeviceCaps(hDeviceContext,LOGPIXELSX) * 6) / 72)
#define GET_FILE_VIEW_CHAR_HEIGHT() \
    ((omDeviceContext.GetDeviceCaps (LOGPIXELSY) * 10) / 72)
#define GET_FILE_VIEW_CHAR_WIDTH() \
    ((omDeviceContext.GetDeviceCaps (LOGPIXELSX) * 6) / 72)


#endif                  //_HASH_DEFINES_H