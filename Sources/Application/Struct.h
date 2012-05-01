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
 * \file      Struct.h
 * \brief     This file contain the definition of all structures used in
 * \authors   Amitesh Bharti, Amarnath Shastry, Ravikumar Patil, Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of all structures used in
 */

#pragma once

#include "include/Struct_CAN.h"
#include "HashDefines.h"
//#include "canapi2.h"
#include "include/Basedefs.h"
#include "DataTypes/Filter_DataTypes.h"

// All function prototype is used in function editor.
typedef VOID (*PFMSG_HANDLER)(STCAN_MSG Rx_Msg);
typedef VOID (*PFTIMER_HANDLER)();
typedef VOID (*PFKEY_HANDLER)(UCHAR ucKeyVal);
typedef VOID (*PFERROR_HANDLER)(SCAN_ERR ErrorMsg);
typedef VOID (*PFDLL_HANDLER)();
typedef BOOL (*PFGET_PRG_VER)(int*,int*,HMODULE);
// Used is application call back function
typedef VOID (CALLBACK* APPTIMERPOINTER)(UINT,UINT,DWORD,DWORD,DWORD);

// This structure definition is to store a list of simulated systems whose paths have
// to be stored in the main configuration file.
struct sSIMSYSARRAY
{
    //  UINT m_unNoOfSimsys;                // No. of simulated systems.
    CString m_omStrSimSysPath;          // simulated system path.
    CString m_omStrSimSysName;          // simulated system name.
    sSIMSYSARRAY* psNextSimsys;         // Pointer to the next simulated system.
    sSIMSYSARRAY();
    sSIMSYSARRAY& operator=(const sSIMSYSARRAY& RefObj);
};
typedef sSIMSYSARRAY SSIMSYSARRAY;
typedef SSIMSYSARRAY* PSSIMSYSARRAY;

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

struct sMESSAGEATTR
{
    CString     omStrMsgname;
    UINT        unMsgID;
    COLORREF    sColor ;
};
typedef sMESSAGEATTR  SMESSAGEATTR;
typedef SMESSAGEATTR* PSMESSAGEATTR;

// This structure will have message and signal information
// for signal watch window
struct sSIGNALINFO
{
    char* m_omStrSignalName;    // Signal Name
    BYTE    m_byStartByte;      // Signal definition start byte in Msg
    UINT    m_unSignalLength;   // Signal Length, One indexed
    BYTE    m_byStartBit;       // Signal start bit, Zero indexed
    float   m_fSignalFactor;    // Signal Multiplication Factor
    float   m_fSignalOffset;    // Signal Offset value.
    char*   m_omStrSignalUnit;  // Signal Measurement Unit
    BYTE    m_bySignalType;     // Singal data type ( BOOL,UINT,INT) *****31.05.2002

};
typedef sSIGNALINFO  SSIGNALINFO;
typedef SSIGNALINFO* PSSIGNALINFO;

struct sMESSAGEINFO
{
    UINT unMessageID;               // Message ID
    UINT m_unMessageLength;         // Message length
    BOOL bFormat;                   // Message format
    UINT unNumberOfSignalsAdded;    // Number of signals added
    PSSIGNALINFO psSignalsInfo;     // Array of signal info
};
typedef sMESSAGEINFO  SMESSAGEINFO;
typedef SMESSAGEINFO* PSMESSAGEINFO;

// These structures will be used by
// "Send Multiple Messages on CAN - bus" module

// For Database messages only
struct sSENDSIGINFO
{
    INT nValue;                     // Signal value
};
typedef sSENDSIGINFO  SSENDSIGINFO;
typedef SSENDSIGINFO* PSSENDSIGINFO;

struct sSENDMSGINFO
{
    INT nMsgId;                     // Message ID
    char* omStrMsgName;             // Message Name
    char* omStrMsgDetails;          // Message details-code,DLC,databyes,
    // triggering mode, time delay
    UINT acBinaryData[64];          // Holds binary data for matrix
    __int64  an64SignalValues[64];  // Holds values of every signal
    INT nDLC;                       // Data length count
    PSSENDSIGINFO pSendSgInfo;      // Structure pointer to signal details
};
typedef sSENDMSGINFO  SSENDMSGINFO;
typedef SSENDMSGINFO* PSSENDMSGINFO;

struct sSENDMULTIMSGINFO
{
    UINT unNoOfMsgsToBeSent;        // Number of messages to be sent on CAN-bus
    CStringArray omMsgFormatList;         // Array of message names in DB list box
    BOOL bChecked;                  // 1 - MonoShot, 0 - Cyclic
    INT nTimeDelay;                 // Time delay
    PSSENDMSGINFO pSendMsgInfo;     // Structure pointer to message details
};
typedef sSENDMULTIMSGINFO  SSENDMULTIMSGINFO;
typedef SSENDMULTIMSGINFO* PSENDMULTIMSGINFO;

struct sTHREAD_INFO
{
    HANDLE m_hThread;
    VOID*  m_pvThread;
};

typedef sTHREAD_INFO  STHREADINFO;
typedef sTHREAD_INFO* PTHREADINFO;

typedef enum eUSERSELCTION
{
    eHEXDECCMD = 0,
    eTXMSGCMD,
    eCONNECTCMD,
    eDATABASEIMPORTCMD,
    eCONFIGCHANGECMD,
    eCHANNELCOUNTUPDATED
};
typedef enum eTXMSGTRIGGERTYPE
{
    eONTIME = 0,
    eONKEY
};
struct sTXCANMSGDETAILS
{
    BOOL m_bIsMsgDirty;          // for a database message;to Indicate user enter
    // bytes value instead of signal value.
    BOOL m_bEnabled;            // To indicate eligiblity of the message for Tx
    STCAN_MSG m_sTxMsg;
};
typedef sTXCANMSGDETAILS STXCANMSGDETAILS;
typedef sTXCANMSGDETAILS* PSTXCANMSGDETAILS;

struct sTXCANMSGLIST
{
    STXCANMSGDETAILS m_sTxMsgDetails;
    struct sTXCANMSGLIST* m_psNextMsgDetails;
};
typedef sTXCANMSGLIST STXCANMSGLIST;
typedef sTXCANMSGLIST* PSTXCANMSGLIST;

struct sMSGBLOCKLIST
{
    CString m_omStrBlockName;
    unsigned char m_ucTrigger;
    BOOL m_bType;
    BOOL m_bActive;
    BOOL m_bTxAllFrame;
    UCHAR m_ucKeyValue;
    UINT m_unTimeInterval;
    UINT m_unMsgCount;
    PSTXCANMSGLIST m_psTxCANMsgList;
    sMSGBLOCKLIST* m_psNextMsgBlocksList;

    sMSGBLOCKLIST()
    {
        m_psNextMsgBlocksList = NULL;
        m_unMsgCount          = 0;
        m_bActive             = TRUE;
        m_bType               = TRUE;
        m_bTxAllFrame         = FALSE;
        m_ucTrigger           = defTIME_TRIGGER;
        m_omStrBlockName      = defDEFAULT_MSG_BLOCK_NAME;
        m_psTxCANMsgList      = NULL;
        m_ucKeyValue          = 0;
        m_unTimeInterval      = defDEFAULT_TIME_VAL;
    }
};
typedef sMSGBLOCKLIST SMSGBLOCKLIST;
typedef sMSGBLOCKLIST* PSMSGBLOCKLIST;

struct sTXMSGINFO
{
    UINT m_unTimeInterval;
    STHREADINFO m_sTimerThreadInfo;
    STHREADINFO m_sKeyThreadInfo;
    CEvent m_omTxBlockTimerEvent;
    CEvent m_omTxBlockKeyEvent;
    CEvent m_omKeyEvent;
    BOOL m_bType;
    BOOL m_bSendAllMessages;
    //CRITICAL_SECTION m_sMsgBlocksCriticalSection;
    HANDLE m_hSemaphore;
    PSTXCANMSGLIST m_psTxCANMsgList;
    struct sTXMSGINFO* m_psNextTxMsgInfo;
};

typedef  sTXMSGINFO   STXMSG;
typedef  sTXMSGINFO*  PSTXMSG;

struct sMSGFILTERINFO
{
    BOOL        m_bStopFilter;          // Mode of filtering
    UINT        m_unMsgIDFrom;          // Start of message ID range
    UINT        m_unMsgIDTo;            // End of message ID range
    UINT*       m_puMsgIDList;          // Message IDs in the filter list who don't
    // appear under the range
};
typedef sMSGFILTERINFO SMSGFILTERINFO;
typedef sMSGFILTERINFO* PSMSGFILTERINFO;

// Global struncture definition
struct sREPLAY_INFO
{
    WORD m_wReplayMode;              // Mode of replay
    CStringArray m_omReplayStrArray; // Array of message string for replay
    UINT m_unTimeDelay;              // Fixed Time Delay between messages
    UINT m_unCycleTimeDelay;         // Time delay between two cycle of replay
    BOOL m_bTimeDelayReplayFile;     // Flag to indicate user option for time
    // delay between messages ( Fixed or Recorded)
    BOOL m_bReplayHexON;                 // flag to store the status of the reply mode
    BOOL m_bStepCmdReq;              // Step command request
    WORD m_wLogReplyTimeMode;        // time mode
};

typedef sREPLAY_INFO SREPLAYINFO;
typedef sREPLAY_INFO* PSREPLAYINFO;

struct sTXMSG_DATA
{
    UINT  m_unDelay;                 // Computed Time delay
    STCAN_MSG m_sTxMsg;              // structure for Transmitting the data.
};

typedef sTXMSG_DATA STXMSGDATA;
typedef sTXMSG_DATA* PSTXMSGDATA;

struct sTXMSGDATA
{
    UINT  m_unCount;              // Total array element in the point m_psTxMsg
    PSTCAN_MSG m_psTxMsg;         // pointer to array of structure for
    // Transmitting the data.
};

typedef sTXMSGDATA STXSELMSGDATA;
typedef sTXMSGDATA* PSTXSELMSGDATA;
// information of all the window coordinates
struct sWINDOWCOORDINATES
{
    WINDOWPLACEMENT    m_sReplayWndCoOrd;      //  replay window coords
    WINDOWPLACEMENT    m_sOutputWndCoOrd;      //  output window coords
    WINDOWPLACEMENT    m_sMsgWndCoOrd;         //  message window coords
    WINDOWPLACEMENT    m_sNotificWndCoOrd;     //  Trace window coords
    WINDOWPLACEMENT    m_sSigWatchWindow;      //  Signal Watch Window
    WINDOWPLACEMENT    m_sMsgInterpWindow;     //  Message Interpretation Window
};
typedef sWINDOWCOORDINATES  SWND_CO_ORDINATES;
typedef sWINDOWCOORDINATES* PSWND_CO_ORDINATES;


// array of the message attirbute details
struct sMESSAGEATTRIBUTES
{
    USHORT          m_usMsgCount;           // count of number of entries in
    // the messages attribute details
    PSMESSAGEATTR   m_psMsgAttribDetails;   // array of MESSAGEATTR structures
};
typedef sMESSAGEATTRIBUTES  SMESSAGE_ATTRIB;
typedef sMESSAGEATTRIBUTES* PSMESSAGE_ATTRIB;

// details of the message filters
struct sMESSAGEFILTERDETAILS
{
    UINT*    m_punUndefinedMsgID;       // ID's of undefined messages. The
    // first element will have the count of
    // message ID's.
    CStringArray    m_omMsgNameArray;   // the database message names in the
    // filter list
    INT     m_nRangeFrom;               // range start information
    INT     m_nRangeTo;                 // range end information
    UINT    m_unCount;                  // count information of message names
    BYTE    m_byFilterType;             // type of filter
    BOOL    m_bChekTxMsg;               // filter transmitted messages
};
typedef sMESSAGEFILTERDETAILS   SMESSAGE_FILTER_DETAILS;
typedef sMESSAGEFILTERDETAILS*   PSMESSAGE_FILTER_DETAILS;

// toolbar info needs 64 bits
struct sTOOLBAR_BUTTON_STATUS
{
    BYTE    m_byMsgFilter;// : 1;
    BYTE    m_byLogFilter;// : 1;
    BYTE    m_byLogging ;//:   1;
    BYTE    m_byMsgInterpret;// : 2;
    BYTE    m_byOverwrite ;//: 1;
    BYTE    m_byDisplayTimeMode ;//:2;
    BYTE    m_byDisplayHexON; // 1
    BYTE    m_byLogTimeMode; // 2
    BYTE    m_byLogHexON;    // 1
    BYTE    m_byLogOverWriteON; // 1
    BYTE    m_byReplayMsgType;//2
    BYTE    m_byEnsureVisible;

};
typedef sTOOLBAR_BUTTON_STATUS STOOLBARINFO;
typedef sTOOLBAR_BUTTON_STATUS* PSTOOLBARINFO;


struct sMSGINFORMATION
{
    sTCANDATA   m_sCanData;
    CString     m_omStrText;
    long        m_lColor;
};

typedef sMSGINFORMATION SMSGINFORMATION;
typedef SMSGINFORMATION* PSMSGINFORMATION;





static char s_cStatus[][2] = {"|","/","-","\\"};

static char sg_acNumCh[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E',
    'F'
};

// Function pointer to overload situation handler
typedef void (*PF_OVERLOAD_HANDLER)();

typedef struct tagSBYTE
{
    unsigned char Bit0 : 1;
    unsigned char Bit1 : 1;
    unsigned char Bit2 : 1;
    unsigned char Bit3 : 1;
    unsigned char Bit4 : 1;
    unsigned char Bit5 : 1;
    unsigned char Bit6 : 1;
    unsigned char Bit7 : 1;
} STRUCT_BYTE;

typedef union tagUBYTE
{
    BYTE        byByte;
    STRUCT_BYTE sByte;
} UNION_BYTE;

// Types of Network Statistics parameters
#define defSTAT_PARAMETERS_COUNT        24
const char acStatParams[ defSTAT_PARAMETERS_COUNT ][25] =
{
    "Total Messages",
    "Message Rate",
    "Total Errors",
    "Error Rate",
    "Network Load",
    "Peak Network Load",
    "Tx Message Total",
    "Standard Tx Message",
    "Standard Tx Rate",
    "Extended Tx Message",
    "Extended Tx Rate",
    "Standard.Tx RTR",
    "Extended Tx RTR",
    "Tx Errors",
    "Tx Error Rate",
    "Rx Message Total",
    "Standard Rx Message",
    "Standard Rx Rate",
    "Extended Rx Message",
    "Extended Rx Rate",
    "Standard.Rx RTR",
    "Extended Rx RTR",
    "Rx Errors",
    "Rx Error Rate",
};

struct sPropertiesList
{
    CStringArray m_omBaudRate;
    CStringArray m_omLogFile;
    CStringArray m_omReplayFile;
    CStringArray m_omDatabaseFile;
    CStringArray m_omDLLFile;
};

typedef sPropertiesList SPROPERTIESLIST;
typedef SPROPERTIESLIST* PSPROPERTIESLIST;



// Tx window spilitter details
struct tagTxMsgSplitterData
{
    int m_nRootSplitterData[2][2];
    int m_nLeftViewSplitterData[2][2];
    int m_nRightViewSplitterData[2][2];
};

typedef tagTxMsgSplitterData sTxMsgSplitterData;
typedef sTxMsgSplitterData STXMSGSPLITTERDATA;
typedef STXMSGSPLITTERDATA* PSTXMSGSPLITTERDATA;

// Graph window splitter details
struct tagGraphSplitterData
{
    int m_nRootSplitterData[2][2];
    int m_nRightViewSplitterData[2][2];
};

typedef tagGraphSplitterData sGraphSplitterData;
typedef sGraphSplitterData SGRAPHSPLITTERDATA;
typedef SGRAPHSPLITTERDATA* PSGRAPHSPLITTERDATA;

// For Tx message views enumeration
enum tagTxViewTypes
{
    eTxMsgBlocksView,
    eTxMsgMessageListView,
    eTxMsgMessageDetailsView,
    eTxMsgFunctionsView
};

typedef tagTxViewTypes eTxMsgViewType;
typedef eTxMsgViewType ETXMSGVIEWTYPE;
typedef ETXMSGVIEWTYPE* PETXMSGVIEWTYPE;


// Named Filter List
struct tagNamedFilter
{
    CArray<SFILTER_CAN,SFILTER_CAN> m_omFilterList; // List of Filters
    BOOL                    m_bFilterType;  // Filter Type 0 - Stop, 1 for Pass
};

typedef tagNamedFilter  sNamedFilter;
typedef sNamedFilter    SNAMEDFILTER;
typedef SNAMEDFILTER*   PSNAMEDFILTER;

struct tagFilterList
{
    CMapStringToPtr&    m_romFilterMap;
    CStringArray&       m_romFilterNames;
    // Constructors to init references
    tagFilterList( CMapStringToPtr& romFilterMap,
                   CStringArray& romFilterNames) :
        m_romFilterMap( romFilterMap), m_romFilterNames( romFilterNames)
    {
    }
};

typedef tagFilterList sFilterList;
typedef sFilterList SFILTERLIST;
typedef SFILTERLIST* PSFILTERLIST;

struct tagModuleFilter
{
    char    m_omFilterName[LENGTH_FILTERNAME];
    BOOL    m_bEnabled;

    tagModuleFilter& operator=(tagModuleFilter& TempObj)
    {
        strcpy_s(m_omFilterName, TempObj.m_omFilterName);
        m_bEnabled = TempObj.m_bEnabled;
        return *this;
    }
    bool operator==(tagModuleFilter& TempObj)
    {
        bool bReturn = TRUE;

        if (_tcscmp(m_omFilterName, TempObj.m_omFilterName))
        {
            bReturn = FALSE;
        }

        if (bReturn)
        {
            if (m_bEnabled != TempObj.m_bEnabled)
            {
                bReturn = FALSE;
            }
        }

        return bReturn;
    }
    bool operator!=(tagModuleFilter& TempObj)
    {
        return !(*this == TempObj);
    }
};

typedef tagModuleFilter sModuleFilter;
typedef sModuleFilter SMODULEFILTER;
typedef SMODULEFILTER* PSMODULEFILTER;
typedef CArray<SMODULEFILTER,SMODULEFILTER> CModuleFilterArray;

struct tagFilterDisplayInfo
{
    CString m_omStrMsgIDFrom;
    CString m_omStrMsgIDTo;
    CString m_omStrMsgIDType;
    CString m_omStrMsgType;
    CString m_omStrMsgDirection;
    CString m_omStrMsgChannel;
    int     m_nImageIndex;
};

typedef tagFilterDisplayInfo sFilterDisplayInfo;
typedef sFilterDisplayInfo sFilterDisplayInfo;
typedef sFilterDisplayInfo SFILTERDISPLAYINFO;
typedef SFILTERDISPLAYINFO* PSFILTERDISPLAYINFO;

//
//class CExecuteFunc;
////New data structure to store  node name and the "CExecuteFunc" pointer associated to that node:
//struct sNODEOBJECT
//{
// CString om_NodeName;
// CExecuteFunc* m_psExecuteFunc;
// sNODEOBJECT* m_psNextNode;
//};
//typedef  sNODEOBJECT SNODEOBJECT;
//typedef  SNODEOBJECT* PSNODEOBJECT;
//
////structure to store information of the thread
////calculating the timer's execution time
//struct sCALCEXECTIMETHREAD
//{
// bool  m_bThreadStop;
// CWinThread* m_pomThreadPtr;
// CEvent m_omExitThreadEvent;
//};
//typedef  sCALCEXECTIMETHREAD SCALCEXECTIMETHREAD;
//typedef  SCALCEXECTIMETHREAD* PSCALCEXECTIMETHREAD;
//
//
////The structure pass to thread function for load Fn execution
//struct sEXECUTE_LOAD_HANDLER
//{
//    PFDLL_HANDLER pFDllHandler;
//    CExecuteFunc* m_pCExecuteFunc;
//};
//typedef sEXECUTE_LOAD_HANDLER SEXECUTE_LOAD_HANDLER;
//typedef sEXECUTE_LOAD_HANDLER* PSEXECUTE_LOAD_HANDLER;
//
////The structure to store message and hmodule for the messages
////transmitted from a DLL
//struct sDLL_MSG
//{
//    STCAN_MSG sRxMsg;
//    HMODULE h_DllHandle;
//};
//typedef sDLL_MSG SDLL_MSG;
//typedef SDLL_MSG* PSDLL_MSG;


// Log File States enumaration
enum eLogFileStates { LOGGING_YET_TO_START, LOGGING_STARTED, LOGGING_STOPPED } ;

// Replay States
enum eReplayStates { REPLAY_TO_START, REPLAY_RUNNING, REPLAY_INVALID };
