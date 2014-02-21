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
 * \file      MainFrm.cpp
 * \brief     Implementation of the CMainFrame class
 * \authors   RBIN/EMC2 - Amarnath Shastry, Amitesh Bharti, Anish Kumar, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CMainFrame class
 */
#include "stdafx.h"             // Standard include header
#include "include/Struct_CAN.h"
#include "include/Struct_LIN.h"
#include "Include/CAN_Error_Defs.h"
#include "Include/LIN_Error_Defs.h"
#include "BUSMASTER.h"        // App class definition file
#include "Utility\UtilFunctions.h"

// PTV XML
#include "include/XMLDefines.h"
// PTV XML
//#include "MsgMDIChildWnd.h"     // Message window class defintion file
#include "MainFrm.h"            // Main frame class defintion file
//#include "FunctionEditorDoc.h"  // Document class defintion file
//#include "ExploreMsgSg.h"       // CExploreMsgSg dialog class defintion file
#include "Properties.h"         // Properties dialog class definition file
// Special file dialog class to
#include "SplFileDlg.h"         // customise CSplFileDlg brower
#include "MessageAttrib.h"      // Message attribute dlg class defintion file
#include "DataType.h"           // Structure definition file for msg attribute
#include "DriverInfoDlg.h"      // Driver info class definition file
#include "Splash.h"             // splash screen implementation file
#include "Flags.h"              // CFlags class
//#include "ErrHandlerDlg.h"      // Dialog class for error handlers
// For getting name of a permanent exported function from the user-defined DLL.
//#include "Export_userdll.h"
#include "MessageList.h"
#include "PPageMessage.h"
#include "common.h"
#include "TimeManager.h"
#include "MsgBufferConfigPage.h"    // For Message Buffer Configuration PPage
#include "MsgFilterConfigPage.h"    // For Filter configuration page
#include "DatabaseDissociateDlg.h"
#include "AppServices_Impl.h"
#include "include/utils_macro.h"
#include "Include/BaseDefs.h"
#include "InterfaceGetter.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "Utility/Utility.h"
#include "Utility/Utility_Thread.h"
#include "Utility/WaitIndicator.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/BaseDIL_LIN.h"
#include "DIL_Interface/BaseDIL_J1939.h"
#include "DIL_Interface/BaseDIL_FLEXRAY.h"
#include "include/ModuleID.h"
#include "FrameProcessor/FrameProcessor_extern.h"
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "FrameProcessor/BaseFrameProcessor_LIN.h"
#include "FrameProcessor/BaseFrameProcessor_J1939.h"
#include "ConfigMsgLogDlg.h"
#include "Replay/Replay_Extern.h"

#include "SignalWatch/SignalWatch_extern.h"
#include "SignalWatch/BaseSignalWatch_CAN.h"

#include "Filter/Filter_extern.h"
#include "ConfigAdapter.h"
#include ".\mainfrm.h"
#include "WaveformSelectionDlg.h"

#include "BusStatistics.h"
#include "SigGrphConfigDlg.h"
#include <DataTypes/SigGrphWnd_Datatypes.h>
#include "J1939TimeOutCfg.h"
#include "include/XMLDefines.h"
#include "Utility\MultiLanguageSupport.h"
#include "FlexRayDatabaseDissociateDlg.h"
#include "ConfigMsgDispPropPg.h"
#include "FibexConfigurationDlg.h"
#define MSG_GET_CONFIGPATH  10000
#ifndef _FLEXRAY_
#define _FLEXRAY_
#endif

// For bus statistics information
extern SBUSSTATISTICS g_sBusStatistics[ defNO_OF_CHANNELS ];

extern CCANMonitorApp theApp;       // Application object
//extern DWORD GUI_dwThread_MsgDisp;  // GUI display thread
//extern HWND GUI_hDisplayWindow;     // Message window handle
CCANBufFSE g_ouCANBufFSE;    //Global CAN buffer
CLINBufFSE g_ouLINBufFSE;    //Global LIN buffer

//const BYTE MSGBUF_ADD = 0x1;
//const BYTE MSGBUF_CLEAR = 0x0;

HWND g_hMainGUI;                     // Main GUI

DWORD g_dwUIThread = 0;

extern BOOL g_bStopSendMultMsg;     // Flag - carried from Utility.h
extern BOOL g_bStopTimerHandlers;   // Flag - carried from Utility.h
extern BOOL g_bStopKeyHandlers;     // Flag - carried from Utility.h
extern BOOL g_bStopErrorHandlers;   // Flag - carried from Utility.h
extern BOOL g_bStopDLLHandlers;
extern BOOL g_bStopMsgHandlers;

extern int nWriteToCAN(STCAN_MSG sTxMsg);
extern int nWriteToLIN(STLIN_MSG sTxMsg);
extern int gnSendCANMsg(STCAN_MSG sTxMsg,HMODULE hModule);
extern int gnSendLINMsg(STLIN_MSG sTxMsg,HMODULE hModule);
extern BOOL gbMsgTransmissionOnOff(BOOL bOnOff,HMODULE hModule);
//BOOL g_bStopMsgBlockTx    ;
BOOL g_bStopSelectedMsgTx ;
PSTXMSG g_psTxMsgBlockList;
// Critical Section for Signal Watch
//extern CRITICAL_SECTION g_CritSectionForSW;
// Flag for Msg Handler status
extern BOOL g_bMsgHandlerON;
// Message Handler Related
// To stop all message handler threads
extern void gvStopMessageHandlerThreads();
extern UINT g_unDllMsgCnt;
CRITICAL_SECTION g_omDllMsgCritiSec;
extern UINT g_unWriteDllMsg;

static CBaseFrameProcessor_CAN* sg_pouFrameProcCAN = NULL; // CAN logger interface
CBaseDIL_CAN* g_pouDIL_CAN_Interface = NULL; // CAN DIL interface
CBaseDIL_FLEXRAY* g_pouDIL_FLEXRAY_Interface = NULL; // FLEXRAY DIL interface
static CBaseSignalWatch_CAN* sg_pouSWInterface[BUS_TOTAL] = {NULL}; // SIGNAL WATCH INTERFACE

static CBaseFrameProcessor_LIN* sg_pouFrameProcLIN = NULL; // CAN logger interface
CBaseDIL_LIN* g_pouDIL_LIN_Interface = NULL; // LIN DIL interface

//static bool m_bLinDILChanging;
//static bool m_shLINDriverId;

DWORD g_dwClientID = 0;
//extern CRITICAL_SECTION g_CritSectDllBufferRead;
extern BOOL g_bReadDllMsg;
HANDLE g_hSemaphore = NULL;

// Related to J1939
static CBaseFrameProcessor_J1939* sg_pouIJ1939Logger = NULL; // Logger interface
static CBaseDILI_J1939* sg_pouIJ1939DIL = NULL; // DIL interface

// Related to LIN
static CBaseFrameProcessor_LIN* sg_pouLinLogger = NULL; // Logger interface
static CBaseDIL_LIN* sg_pouLinDIL = NULL; // DIL interface

//J1939 Database editor window
BOOL CMsgSignalDBWnd::sm_bValidJ1939Wnd = FALSE;
SMSGENTRY* CTxMsgWndJ1939::m_psMsgRoot = NULL;

//LIN Database editor window
//BOOL CMsgSignalDBWnd::sm_bValidLinWnd = FALSE;


#define CREATE_TOOLBAR(pParentWnd, ToolBarObj, ID, Title) {if (nCreateToolbar(pParentWnd, ToolBarObj, ID, Title) != 0){return -1;}}
#define defCONFIGFILTER         _T("BUSMASTER Configuration files(*.cfx)|*.cfx||")
#define defFILEEXT              _T("cfx")
#define defDLGFLAGS             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST
#define defDEFAULTDLLFILENAME   ("*.dll")
#define TIMER_REFRESH_MAINGUI   0x100
// PTV [1.6.4]
#define TIMER_REFRESH_LOG_STATUS 0x101
#define TIMER_REFRESH_J1939_LOG_STATUS 0x102
#define TIMER_REFRESH_FLEXRAY_STATUS   0X103
#define TIMER_REFRESH_LIN_LOG_STATUS 0x103
// PTV [1.6.4] END
#define STSBAR_REFRESH_TIME_PERIOD      1000  // in milliseconds
#define STSBAR_REFRESH_TIME_PERIOD_LOG  1000  // in milliseconds
#define PROFILE_CAN_MONITOR                   _T("RBEI_ECF2_CAN_Monitor")

const BYTE CAPL_2_C_MASK  = 0x1;
const BYTE DBF_2_DBC_MASK = 0x2;
const BYTE DBC_2_DBF_MASK = 0x4;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_COMMAND(IDM_CONFIGURE_DATABASE_OPENACTIVE, OnOpenDatabase)
    ON_COMMAND(IDM_CONFIGURE_DATABASE_CLOSE, OnCloseDatabase)
    ON_COMMAND(IDM_FILE_IMPORT_DATABASE, OnImportDatabase)
    ON_COMMAND(IDM_CONFIGURE_BAUDRATE, OnConfigBaudrate)
    ON_COMMAND(IDM_CONFIGURE_LIN_CHANNEL, OnConfigLinChannel)
    ON_COMMAND(IDM_CONFIGURE_DATABASE_NEW, OnNewDatabase)
    ON_COMMAND(IDM_CONFIGURE_DATABASE_SAVEAS, OnConfigDatabaseSaveAs)
    ON_COMMAND(IDM_CONFIGURE_DATABASE_SAVE, OnConfigDatabaseSave)
    ON_COMMAND(IDM_CONFIGURE_MESSAGE, OnConfigMessageDisplay)
    ON_COMMAND(IDM_REPLAY_SKIP, OnReplaySingleSkip)
    ON_COMMAND(IDM_REPLAY_STEP, OnReplaySingleStep)
    ON_COMMAND(IDM_REPLAY_STOP, OnReplayStop)
    ON_COMMAND(IDM_REPLAY_GO, OnReplayGo)
    ON_COMMAND(IDM_EXECUTE_MESSAGEHANDLERS, OnActivateMessageHandler)
    ON_COMMAND(IDM_EXECUTE_MESSAGEHANDLERS_LIN, OnActivateMessageHandlerLIN)
    ON_COMMAND(IDM_EXECUTE_TIMERHANDLER, OnActivateTimerHandler)
    ON_COMMAND(IDM_EXECUTE_TIMERHANDLER_LIN, OnActivateTimerHandlerLIN)
    ON_COMMAND(IDM_DLL_BUILDLOAD_ALL, OnDLLBuildLoad)
    ON_COMMAND(IDM_DLL_BUILDLOAD_ALL_LIN, OnDLLBuildLoadLIN)
    ON_COMMAND(IDM_DLL_BUILD_ALL, OnDLLBuild)
    ON_COMMAND(IDM_DLL_BUILD_ALL_LIN, OnDLLBuildLIN)
    ON_COMMAND(IDM_DLL_LOADALLDLL, OnDllLoad)
    ON_COMMAND(IDM_DLL_LOADALLDLL_LIN, OnDllLoadLIN)
    ON_COMMAND(IDM_DLL_UNLOAD_ALL, OnDllUnload)
    ON_COMMAND(IDM_DLL_UNLOAD_ALL_LIN, OnDllUnloadLIN)
    ON_COMMAND(IDM_DISPLAY_MESSAGEWATCHWINDOW_INTERPRET, OnMessageInterpretation)
    ON_COMMAND(IDM_SIGNALWATCH_ADDSIGNAL, OnAddSignalToSignalWindow)
    ON_COMMAND(IDM_FILTER_MESSAGEFILTEROFF, OnMessageFilterButton)
    ON_COMMAND(IDM_FILTER_REPLAYFILTEROFF, OnReplayFilter)
    ON_COMMAND(IDM_FILTER_LOGFILTEROFF, OnLogFilter)
    ON_COMMAND(IDM_FILTER_MESSAGE_SELECTMESSAGES, OnSelectMessage)
    ON_COMMAND(IDM_APP_ABOUT, OnAboutApplication)
    ON_COMMAND(IDM_LOG_ON_OFF, OnLogEnable)


    ON_COMMAND(IDM_LOG_LIN_ON_OFF, OnLog_LIN_Enable)
    ON_UPDATE_COMMAND_UI(IDM_LOG_LIN_ON_OFF, OnUpdateLogOnOff_LIN)

    //ON_COMMAND(IDM_FILE_RESTART_CONTROLLER, OnRestartController)
    ON_COMMAND(IDR_TOOL_HEXDEC, OnHex_DecButon)
    ON_COMMAND(IDR_TOOL_BUTTON_MSG_DISP, OnButtonMsgDispButton)
    ON_COMMAND(IDR_TOOL_BUTTON_SIGNAL_WATCH, OnButtonSignalWatchButton)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_DATABASE_CLOSE, OnUpdateConfigureDatabaseClose)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_DATABASE_NEW, OnUpdateConfigureDatabaseNew)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_DATABASE_SAVE, OnUpdateConfigureDatabaseSave)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_DATABASE_SAVEAS, OnUpdateConfigureDatabaseSaveas)
    ON_WM_CLOSE()
    ON_UPDATE_COMMAND_UI(IDM_DISPLAY_MESSAGEWATCHWINDOW_INTERPRET, OnUpdateMessageInterpret)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_MESSAGEHANDLERS, OnUpdateExecuteMessagehandlers)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_MESSAGEHANDLERS_LIN, OnUpdateExecuteMessagehandlersLIN)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_LOGFILTEROFF, OnUpdateLogFilter)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_REPLAYFILTEROFF, OnUpdateReplayFilter)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_MESSAGEFILTEROFF, OnUpdateMessageFilterButton)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_MESSAGEHANDLERS_BUTTON, OnUpdateExecuteMessagehandlersButton)
    ON_COMMAND(IDM_EXECUTE_MESSAGEHANDLERS_BUTTON, OnExecuteMessagehandlersButton)
    ON_COMMAND(IDR_TOOL_SENDMSG, OnSendMessage)
    ON_COMMAND(IDR_TOOL_FLEX_SENDMSG, OnSendFlexRayMessage)
    ON_COMMAND(ID_TRANSMIT_ACTIVATE_LIN, OnSendMessageLIN)
    ON_COMMAND(IDM_FILTER_LOGFILTEROFF_BUTTON, OnLogFilter)
    //    ON_COMMAND(IDM_FILTER_LOGFILTEROFF_BUTTON, OnLogFilterButton)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_LOGFILTEROFF_BUTTON, OnUpdateLogFilter)
    //    ON_UPDATE_COMMAND_UI(IDM_FILTER_LOGFILTEROFF_BUTTON, OnUpdateLogFilterButton)
    ON_COMMAND(IDM_FILTER_MESSAGEFILTEROFF_BUTTON, OnMessageFilterButton)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_MESSAGEFILTEROFF_BUTTON, OnUpdateMessageFilterButton)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_TIMERHANDLER, OnUpdateExecuteTimerhandler)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_TIMERHANDLER_LIN, OnUpdateExecuteTimerhandlerLIN)
    ON_COMMAND(IDM_DISPLAY_MESSAGEWINDOW_OVERWRITE, OnDisplayMessagewindowOverwrite)
    ON_COMMAND(IDM_CLEAR_MSG_WINDOW, OnClearMsgWindow)
    ON_UPDATE_COMMAND_UI(IDM_DLL_BUILD_ALL, OnUpdateDllBuildAll)
    ON_UPDATE_COMMAND_UI(IDM_DLL_BUILD_ALL_LIN, OnUpdateDllBuildAllLIN)
    ON_UPDATE_COMMAND_UI(IDM_DLL_BUILDLOAD_ALL, OnUpdateDllBuildloadAll)
    ON_UPDATE_COMMAND_UI(IDM_DLL_BUILDLOAD_ALL_LIN, OnUpdateDllBuildloadAllLIN)
    ON_UPDATE_COMMAND_UI(IDM_DLL_UNLOAD_ALL, OnUpdateDllUnloadAll)
    ON_UPDATE_COMMAND_UI(IDM_DLL_UNLOAD_ALL_LIN, OnUpdateDllUnloadAllLIN)
    ON_UPDATE_COMMAND_UI(IDM_DISPLAY_MESSAGEWINDOW_OVERWRITE, OnUpdateDisplayMessagewindowOverwrite)
    ON_UPDATE_COMMAND_UI(IDR_TOOL_HEXDEC, OnUpdateToolHexdec)
    ON_UPDATE_COMMAND_UI(IDM_LOG_ON_OFF, OnUpdateLogOnOff)
    ON_UPDATE_COMMAND_UI(IDM_REPLAY_GO, OnUpdateReplayGo)
    ON_UPDATE_COMMAND_UI(IDM_REPLAY_SKIP, OnUpdateReplaySkip)
    ON_UPDATE_COMMAND_UI(IDM_REPLAY_STEP, OnUpdateReplayStep)
    ON_UPDATE_COMMAND_UI(IDM_REPLAY_STOP, OnUpdateReplayStop)
    ON_WM_SHOWWINDOW()
    ON_WM_DESTROY()
    ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
    ON_COMMAND(IDM_CFGN_LOG, OnCfgLogFile)

    ON_COMMAND(IDM_CFGN_LIN_LOG, OnCfgLogFile_LIN)
    ON_UPDATE_COMMAND_UI(IDR_TOOL_BUTTON_MSG_DISP, OnUpdateToolButtonMsgDisp)
    ON_WM_ENDSESSION()
    ON_COMMAND(IDM_CFGN_SEND_MSGS, OnCfgSendMsgs)
    ON_UPDATE_COMMAND_UI(IDR_TOOL_SENDMSG, OnUpdateToolSendmsg)
    ON_UPDATE_COMMAND_UI(IDR_TOOL_FLEX_SENDMSG, OnUpdateToolFlexRaySendmsg)
    ON_COMMAND(IDM_FILE_CONNECT, OnFileConnect)
    ON_UPDATE_COMMAND_UI(IDM_FILE_CONNECT, OnUpdateFileConnect)
    ON_COMMAND(ID_FLEXRAY_CONNECT, OnFlexRayConnect)
    ON_UPDATE_COMMAND_UI(ID_FLEXRAY_CONNECT, OnUpdateFlexRayConnect)
    ON_COMMAND(IDM_LIN_CONNECT, OnLINConnect)
    ON_UPDATE_COMMAND_UI(IDM_LIN_CONNECT, OnUpdateLINConnect)
    ON_COMMAND(IDM_EXECUTE_KEYHANDLERS, OnExecuteKeyhandlers)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_KEYHANDLERS, OnUpdateExecuteKeyhandlers)
    ON_COMMAND(IDM_EXECUTE_KEYHANDLERS_LIN, OnExecuteKeyhandlersLIN)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_KEYHANDLERS_LIN, OnUpdateExecuteKeyhandlersLIN)
    ON_COMMAND(IDM_CONFIG_LOAD, OnLoadConfigFile)
    ON_COMMAND(IDM_CONFIG_NEW, OnNewConfigFile)
    ON_COMMAND(IDM_SAVE_CONFIG, OnSaveConfigFile)
    ON_COMMAND(IDM_SAVEAS_CONFIG, OnSaveAsConfigFile)
    ON_UPDATE_COMMAND_UI(IDM_CONFIG_LOAD, OnUpdateConfigLoad)
    ON_UPDATE_COMMAND_UI(IDM_CONFIG_NEW, OnUpdateConfigNew)
    ON_UPDATE_COMMAND_UI(IDM_SAVE_CONFIG, OnUpdateConfigSave)
    ON_UPDATE_COMMAND_UI(IDM_SAVEAS_CONFIG, OnUpdateConfigSaveAs)
    ON_COMMAND(IDM_FILE_PROPETIES, OnFilePropeties)
    ON_WM_DROPFILES()
    ON_COMMAND(IDM_EXECUTE_ALL_HANDLERS, OnExecuteAllHandlers)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_ALL_HANDLERS, OnUpdateExecuteAllHandlers)
    ON_COMMAND(IDM_EXECUTE_ALL_HANDLERS_LIN, OnExecuteAllHandlersLIN)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_ALL_HANDLERS_LIN, OnUpdateExecuteAllHandlersLIN)
    ON_COMMAND(IDM_EXECUTE_ERROR_HANDLERS, OnExecuteErrorHandlers)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_ERROR_HANDLERS, OnUpdateExecuteErrorHandlers)
    ON_COMMAND(IDM_EXECUTE_ERROR_HANDLERS_LIN, OnExecuteErrorHandlersLIN)
    ON_UPDATE_COMMAND_UI(IDM_EXECUTE_ERROR_HANDLERS_LIN, OnUpdateExecuteErrorHandlersLIN)
    ON_COMMAND(IDM_CONFIGURE_ACTIVE, OnConfigureModeActive)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_ACTIVE, OnUpdateConfigureModeActive)
    ON_WM_TIMER()

    ON_COMMAND(IDM_NETWORK_STATISTICS_WND, OnNetworkStatisticsWnd)
    ON_UPDATE_COMMAND_UI(IDM_NETWORK_STATISTICS_WND, OnUpdateNetworkStatisticsWnd)

    ON_COMMAND(IDM_CONFIGURE_PASSIVE, OnConfigurePassive)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_PASSIVE, OnUpdateConfigurePassive)
    ON_COMMAND(IDM_TRACE_WND, OnTraceWnd)
    ON_MESSAGE(IDM_TRACE_WND, OnMessageTraceWnd)
    ON_UPDATE_COMMAND_UI(IDM_TRACE_WND, OnUpdateTraceWnd)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_BAUDRATE, OnUpdateConfigureBaudrate)
    ON_COMMAND(IDM_DISPLAY_MESSAGE_DISPLAY_ABSOLUTETIME, OnDisplayAbsoluteTime)
    ON_UPDATE_COMMAND_UI(IDM_DISPLAY_MESSAGE_DISPLAY_ABSOLUTETIME, OnUpdateDisplayAbsolutetime)
    ON_COMMAND(IDM_DISPLAY_MESSAGE_DISPLAY_RELATIVETIME, OnDisplayRelativetime)
    ON_UPDATE_COMMAND_UI(IDM_DISPLAY_MESSAGE_DISPLAY_RELATIVETIME, OnUpdateDisplayRelativetime)
    ON_COMMAND(IDM_DISPLAY_MESSAGE_DISPLAY_SYSTEM_TIME, OnDisplaySystemTime)
    ON_UPDATE_COMMAND_UI(IDM_DISPLAY_MESSAGE_DISPLAY_SYSTEM_TIME, OnUpdateDisplaySystemTime)
    ON_COMMAND(IDM_DISPLAY_MESSAGE_DISPLAYRELATIVETIME,OnEnableTimeStampButton)
    ON_WM_SIZE()
    ON_UPDATE_COMMAND_UI(IDR_TOOL_BUTTON_SIGNAL_WATCH, OnUpdateSignalWatchWnd)
    ON_COMMAND(IDM_GRAPH_WND, OnGraphWindow)
    ON_UPDATE_COMMAND_UI(IDM_GRAPH_WND, OnUpdateGraphWnd)
    ON_COMMAND(IDM_CFGN_REPLAY, OnCfgnReplay)
    ON_UPDATE_COMMAND_UI(IDM_CFGN_REPLAY, OnUpdateCfgnReplay)
    ON_COMMAND(IDM_CONFIGURE_SIMULATEDSYSTEMS, OnConfigureSimulatedsystems)
    ON_COMMAND(IDM_CONFIGURE_SIMULATEDSYSTEMS_LIN, OnConfigureSimulatedsystemsLin)
    ON_UPDATE_COMMAND_UI(IDM_DLL_LOADALLDLL, OnUpdateDllLoadAll)
    ON_UPDATE_COMMAND_UI(IDM_DLL_LOADALLDLL_LIN, OnUpdateLINDllLoadAll)
    ON_COMMAND_RANGE(IDC_SELECT_DRIVER,IDC_SELECT_DRIVER + DIL_TOTAL, OnSelectDriver)
    ON_UPDATE_COMMAND_UI_RANGE(IDC_SELECT_DRIVER,IDC_SELECT_DRIVER + DIL_TOTAL, OnUpdateSelectDriver)
    ON_COMMAND_RANGE(IDC_SELECT_FLEX_DRIVER,IDC_SELECT_FLEX_DRIVER + FLEXRAY_DIL_TOTAL, OnSelectFLEXRAYDriver)
    ON_UPDATE_COMMAND_UI_RANGE(IDC_SELECT_FLEX_DRIVER,IDC_SELECT_FLEX_DRIVER + FLEXRAY_DIL_TOTAL, OnUpdateSelectFLEXRAYDriver)
    ON_COMMAND_RANGE(IDC_SELECT_LIN_DRIVER,IDC_SELECT_LIN_DRIVER + DIL_LIN_TOTAL, OnSelectLINDriver)
    ON_UPDATE_COMMAND_UI_RANGE(IDC_SELECT_LIN_DRIVER,IDC_SELECT_LIN_DRIVER + DIL_LIN_TOTAL, OnUpdateSelectLINDriver)
    //}}AFX_MSG_MAP
    // Global help commands
    ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
    ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
    ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
    ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
    ON_MESSAGE(WM_PROCESS_ERROR_MESSAGE, OnErrorMessageProc)
    ON_UPDATE_COMMAND_UI(ID_ACTIVE_DATABASE_NAME, OnUpdateConfigurationFileName)
    ON_COMMAND_RANGE(IDM_REC_CFG_FILE1, IDM_REC_CFG_FILE5, OnClickMruList)
    ON_UPDATE_COMMAND_UI_RANGE(IDM_REC_CFG_FILE1, IDM_REC_CFG_FILE5, OnUpdateMruList)
    ON_MESSAGE(WM_FILE_DISCONNECT,vDisconnect)
    ON_MESSAGE(WM_SET_WARNING_LIMIT_VAR,vSetWarningLimitVar)
    ON_MESSAGE(WM_KEY_PRESSED_MSG_WND,vKeyPressedInMsgWnd)
    ON_MESSAGE(WM_NOTIFICATION_FROM_OTHER, vNotificationFromOtherWin)
    ON_MESSAGE(WM_CREATE_TRACEWIN, vCreateTraceWindow)
    ON_MESSAGE(WM_ENABLE_DISABLE_HANDLERS, vEnableDisableHandlers)
    ON_WM_HELPINFO()
    ON_COMMAND(IDM_DATABASE_DISSOCIATE_DB, OnDissociateDatabase)
    ON_COMMAND(IDM_SAVE_IMPORT, OnSaveImportDatabase)
    ON_UPDATE_COMMAND_UI(IDM_SAVE_IMPORT, OnUpdateSaveImportDatabase)
    //    ON_COMMAND(IDM_SAVE_IMPORT_J1939, OnSaveImportJ1939Database)
    //    ON_UPDATE_COMMAND_UI(IDM_SAVE_IMPORT_J1939, OnUpdateSaveImportJ1939Database)
    ON_MESSAGE(WM_GET_DB_PTR, OnProvideMsgDBPtr)
    ON_MESSAGE(WM_GET_MSG_NAME_FROM_CODE, OnProvideMsgNameFromCode)
    ON_MESSAGE(WM_GET_PGN_NAME_FROM_CODE, OnProvidePGNNameFromCode)
    ON_UPDATE_COMMAND_UI(IDM_CFGN_LOG, OnUpdateCfgnLog)
    ON_UPDATE_COMMAND_UI(IDM_CFGN_LIN_LOG, OnUpdateCfgnLog_LIN)
    ON_COMMAND(ID_DISPLAY_MAIN, OnDisplayMain)
    ON_UPDATE_COMMAND_UI(ID_DISPLAY_MAIN, OnUpdateDisplayMain)
    ON_COMMAND(ID_DISPLAY_MSG_WND, OnDisplayMsgWnd)
    ON_UPDATE_COMMAND_UI(ID_DISPLAY_MSG_WND, OnUpdateDisplayMsgWnd)
    ON_COMMAND(ID_DISPLAY_NODE_SIMULATION, OnDisplayNodeSim)
    ON_UPDATE_COMMAND_UI(ID_DISPLAY_NODE_SIMULATION, OnUpdateDisplayNodeSim)
    ON_WM_ACTIVATE()
    ON_COMMAND(ID_CONFIGURE_WAVEFORM_MESSAGES, OnConfigureWaveformMessages)
    ON_COMMAND(IDR_START_SIGNAL_TRANSMISSION, OnStartSignalTransmission)
    ON_UPDATE_COMMAND_UI(IDR_START_SIGNAL_TRANSMISSION, OnUpdateStartSignalTransmission)
    ON_MESSAGE(WM_FROM_USER_DLL, OnMessageFromUserDll)
    ON_COMMAND(ID_CONFIGURE_SIGNALGRAPHWINDOW, OnConfigureSignalgraphwindow)
    ON_UPDATE_COMMAND_UI(ID_CONFIGURE_SIGNALGRAPHWINDOW, OnUpdateConfigureSignalgraphwindow)
    ON_COMMAND(ID_SIGNALGRAPHWINDOW_CAN, OnSignalgraphwindowCAN)
    ON_UPDATE_COMMAND_UI(ID_SIGNALGRAPHWINDOW_CAN, OnUpdateSignalgraphwindowCAN)
    ON_COMMAND(ID_SIGNALGRAPHWINDOW_MCNET, OnSignalgraphwindowMcnet)
    ON_UPDATE_COMMAND_UI(ID_SIGNALGRAPHWINDOW_MCNET, OnUpdateSignalgraphwindowMcnet)
    ON_COMMAND(33056, OnActivateJ1939)
    ON_UPDATE_COMMAND_UI(33056, OnUpdateActivateJ1939)
    ON_COMMAND(ID_J1939_CONFIG_LOG, OnJ1939ConfigLog)
    ON_UPDATE_COMMAND_UI(ID_J1939_CONFIG_LOG, OnUpdateJ1939ConfigLog)
    ON_COMMAND(ID_ACTION_J1939_ONLINE, OnActionJ1939Online)
    ON_UPDATE_COMMAND_UI(ID_ACTION_J1939_ONLINE, OnUpdateActionJ1939Online)
    ON_COMMAND(ID_ACTION_J1939_TX_MESSAGE, OnActionJ1939TxMessage)
    ON_UPDATE_COMMAND_UI(ID_ACTION_J1939_TX_MESSAGE, OnUpdateActionJ1939TxMessage)
    ON_COMMAND(ID_ACTION_J1939_LOG, OnActionJ1939Log)
    ON_UPDATE_COMMAND_UI(ID_ACTION_J1939_LOG, OnUpdateActionJ1939Log)
    ON_COMMAND(ID_TOOLBAR_J1939, OnToolbarJ1939)
    ON_UPDATE_COMMAND_UI(ID_TOOLBAR_J1939, OnUpdateToolbarJ1939)
    ON_COMMAND(ID_FLEXRAY_TOOLBAR, OnToolbarFlexRay)
    ON_UPDATE_COMMAND_UI(ID_FLEXRAY_TOOLBAR, OnUpdateToolbarFlexRay)
    ON_COMMAND(ID_CONFIGURATION_TOOLBAR, OnToolbarConfiguration)
    ON_UPDATE_COMMAND_UI(ID_CONFIGURATION_TOOLBAR, OnUpdateToolbarConfiguration)
    ON_COMMAND(ID_TOOLBAR_LIN, OnToolbarLIN)
    ON_UPDATE_COMMAND_UI(ID_TOOLBAR_LIN, OnUpdateToolbarLIN)
    ON_COMMAND(33077, OnJ1939ConfigureTimeouts)
    ON_UPDATE_COMMAND_UI(33077, OnUpdateJ1939Timeouts)
    ON_COMMAND(33079, OnJ1939DBNew)
    ON_UPDATE_COMMAND_UI(33079, OnUpdateJ1939DBNew)
    ON_COMMAND(33080, OnJ1939DBOpen)
    ON_COMMAND(ID_DATABASE_CLOSE, OnJ1939DBClose)
    ON_UPDATE_COMMAND_UI(ID_DATABASE_CLOSE, OnUpdateJ1939DBClose)
    ON_COMMAND(ID_DATABASE_SAVE, OnJ1939DBSave)
    //    ON_COMMAND(IDM_CONFIGURE_J1939_DB_SAVEAS, OnJ1939DBSaveAs)
    ON_UPDATE_COMMAND_UI(ID_DATABASE_SAVE, OnUpdateJ1939DBSave)
    //    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_J1939_DB_SAVEAS, OnUpdateJ1939DBSaveAs)
    ON_COMMAND(33082, OnJ1939DBAssociate)
    ON_COMMAND(33083, OnJ1939DBDissociate)
    ON_COMMAND(ID_CONFIGURE_SIMULATEDSYSTEMS, OnJ1939CfgSimSys)
    ON_COMMAND(33087, OnJ1939LoadAll)
    ON_UPDATE_COMMAND_UI(33087, OnUpdateJ1939LoadAll)
    ON_COMMAND(33088, OnJ1939UnloadAll)
    ON_UPDATE_COMMAND_UI(33088, OnUpdateJ1939UnloadAll)
    ON_COMMAND(33089, OnJ1939BuildAndLoadAll)
    ON_UPDATE_COMMAND_UI(33089, OnUpdateJ1939BuildAndLoadAll)
    ON_COMMAND(33090, OnJ1939BuildAll)
    ON_UPDATE_COMMAND_UI(33090, OnUpdateJ1939BuildAll)
    ON_COMMAND(33092, OnJ1939AllMessageHandlers)
    ON_UPDATE_COMMAND_UI(33092, OnUpdateJ1939AllMessageHandlers)
    ON_COMMAND(33093, OnJ1939AllKeyHandlers)
    ON_UPDATE_COMMAND_UI(33093, OnUpdateJ1939AllKeyHandlers)
    ON_COMMAND(33094, OnJ1939AllTimerHandlers)
    ON_UPDATE_COMMAND_UI(33094, OnUpdateJ1939AllTimerHandlers)
    ON_COMMAND(ID_EXECUTE_ALLEVENTHANDLERS, OnJ1939AllEventHandlers)
    ON_UPDATE_COMMAND_UI(ID_EXECUTE_ALLEVENTHANDLERS, OnUpdateJ1939AllEventHandlers)
    ON_COMMAND(33094, OnJ1939AllTimerHandlers)
    ON_UPDATE_COMMAND_UI(33094, OnUpdateJ1939AllTimerHandlers)
    ON_COMMAND(33096, OnJ1939AllHandlers)
    ON_UPDATE_COMMAND_UI(33096, OnUpdateJ1939AllHandlers)
    ON_COMMAND(ID_DISPLAY_CONFIGURE, OnDisplayConfig)
    ON_UPDATE_COMMAND_UI(ID_DISPLAY_CONFIGURE, OnUpdateDisplayConfig)
    ON_UPDATE_COMMAND_UI(33085, OnUpdateJ1939CfgSimSys)
    ON_COMMAND(ID_SIGNALWATCH_ADD, OnJ1939SignalwatchAdd)
    ON_COMMAND(ID_SIGNALWATCH_SHOWWINDOW, OnJ1939SignalwatchShow)
    ON_UPDATE_COMMAND_UI(ID_SIGNALWATCH_SHOWWINDOW, OnUpdateJ1939SignalwatchShow)
    ON_COMMAND(ID_CONFIGURE_MESSAGEDISPLAY_J1939, OnConfigureMessagedisplayJ1939)
    // ON_COMMAND(ID_CONFIGURE_MESSAGEDISPLAY_FLEXRAY, OnConfigureMessageDisplayFlexRay)
    ON_COMMAND_RANGE(ID_SHOWMESSAGEWINDOW_CAN,ID_SHOWMESSAGEWINDOW_J1939, OnShowHideMessageWindow)
    ON_UPDATE_COMMAND_UI_RANGE(ID_SHOWMESSAGEWINDOW_CAN,ID_SHOWMESSAGEWINDOW_J1939, OnUpdateShowHideMessageWindow)
    ON_COMMAND_RANGE(ID_SHOWMESSAGEWINDOW_CAN,ID_SHOWMESSAGEWINDOW_FLEXRAY, OnShowHideMessageWindow)
    ON_UPDATE_COMMAND_UI_RANGE(ID_SHOWMESSAGEWINDOW_CAN,ID_SHOWMESSAGEWINDOW_FLEXRAY, OnUpdateShowHideMessageWindow)
    ON_COMMAND_RANGE(ID_SHOWMESSAGEWINDOW_CAN,ID_SHOWMESSAGEWINDOW_LIN, OnShowHideMessageWindow)
    ON_UPDATE_COMMAND_UI_RANGE(ID_SHOWMESSAGEWINDOW_CAN,ID_SHOWMESSAGEWINDOW_LIN, OnUpdateShowHideMessageWindow)


    //ON_UPDATE_COMMAND_UI_RANGE(ID_SHOWMESSAGEWINDOW_CAN,ID_SHOWMESSAGEWINDOW_J1939, OnUpdateShowHideMessageWindow)
    ON_COMMAND(ID_TB_CANDATABASE, OnToolbarCandatabase)
    ON_UPDATE_COMMAND_UI(ID_TB_CANDATABASE, OnUpdateToolbarCanDatabase)

    //venkat
    ON_COMMAND(ID_TESTAUTOMATION_EDITOR, OnAutomationTSEditor)
    ON_COMMAND(ID_TESTAUTOMATION_EXECUTOR, OnAutomationTSExecutor)
    ON_COMMAND(ID_UTILITY_FILE_CONVERTER, OnFileConverter)
    ON_COMMAND(ID_CONFIGURE_CHANNELSELECTION, OnConfigChannelSelection)
    ON_UPDATE_COMMAND_UI(ID_CONFIGURE_CHANNELSELECTION, OnUpdateConfigChannelSelection)
    ON_MESSAGE(WM_KEYBOARD_CHAR, OnReceiveKeyBoardData)
    ON_MESSAGE(WM_KEYBOARD_KEYDOWN, OnReceiveKeyDown)
    ON_MESSAGE(MSG_GET_CONFIGPATH, onGetConfigPath)
    ON_MESSAGE(WM_J1939_TX_CLOSE_MSG, onJ1939TxWndClose)

    ON_COMMAND(IDM_FLEX_CFGN_SEND_MSGS, OnFlexRayTxWindow)             //open flexray window
    ON_COMMAND(ID_FLEXRAY_CLUSTER_CONFIG, OnFlexRayDBAssociate)               //open flexray DB association
    ON_UPDATE_COMMAND_UI(ID_FLEXRAY_CLUSTER_CONFIG, OnUpdateFlexrayAssociate)
    ON_COMMAND(ID_FLEXRAY_DB_DISOCIATE, OnFlexRayDBDisociate)               //open flexray DB association

    //LIN MESSAGE MAPPING
    ON_UPDATE_COMMAND_UI(ID_HARDWAREINTERFACE_CUBASLIN, OnConfigBaudrateLIN)
    //ON_COMMAND(ID_TRANSMIT_ACTIVATE_LIN, OnActivateLIN)
    ON_COMMAND(ID_TRANSMIT_CONFIGURE_LIN, OnCfgSendMsgsLIN)

    ON_COMMAND(ID_LIN_CLUSTER_CONFIG, OnLinClusterConfig)
    ON_UPDATE_COMMAND_UI(ID_LIN_CLUSTER_CONFIG, OnUpdateLinClusterConfig)

END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_ACTIVE_DATABASE_NAME,// Active configuration name
    ID_LOG_RECORD_CAN_ICON,
    ID_LOG_RECORD_J1939_ICON,
    IDS_CHANNELS            // Number of channels supported by the application
};

const int INDEX_DB_NAME = 0x1;
const int INDEX_CHANNELS = 0x4;
const int INDEX_J1939_LOG_ICON = 0x3;
const int INDEX_CAN_LOG_ICON = 0x2;
const int INDEX_LIN_LOG_ICON = 0x5;

CAppServices_Impl sg_ouAppServiceObj;




/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
/*******************************************************************************
 Function Name    : CMainFrame
 Input(s)         :
 Output           :
 Functionality    : Constructor
 Member of        : CMainFrame
 Friend of        :     -
 Author(s)        : Amarnath Shastry,Amitesh Bharti,Ratnadip Choudhury
 Date Created     : 05.03.2002
 Modifications    : Amitesh Bhati
                    07.08.2002,Initialisation of m_omStrPrevLoadedDll added
 Modifications    : Amitesh Bhati
                    19.01.2004,Initialisation of m_unTimerSB and  added
                               m_omStrSavedConfigFile added.
 Modifications    : Raja N
                    05.04.2004,Modified to refer the latest signal watch list
                    structure
 Modifications    : Raja N
                    28.04.2004,Replaced hardcoded string "Empty" with
                    defSTR_DEFAULT_MRU_ITEM
 Modifications    : Raja N
                    12.12.2004,Added code to init graph related member variables
 Modifications    : Raja N
                    14.03.2005,Added code to init controller state variable
 Modifications    : Raja N
                    13.04.2005,Added code to init Tx Msg Window pointer and Tx
                    message manager.
 Modifications    : Raja N
                    19.07.2005,Removed code to init replay window pointer
 Modifications    : Pradeep Kadoor on 12.06.2009.
                    A Variable to store application directory is added.
*******************************************************************************/
CMainFrame::CMainFrame()
{
    GetCurrentDirectory(MAX_PATH, theApp.m_acApplicationDirectory);// Get application directory
    m_nMaxFlexChannels = 1;
    m_nMaxLinChannels = 1;
    m_podMsgSgWnd                   = NULL;
    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        m_pomMsgSgDetViews[i]       = NULL;
        m_pomMsgSgTreeViews[i]      = NULL;
    }

    m_podMsgWndThread               = NULL;

    m_bIsNewDatabase                = FALSE;
    m_hModuleHandle                 = NULL;
    // PTV [1.6.4]
    m_bIconSetFlag = 0;
    m_bJ1939IconSetFlag = 0;
    m_nSendMsgLogCnt = 0;
    m_nSendMsgJ1939LogCnt = 0;
    // PTV [1.6.4]

    INITIALISE_ARRAY(m_psSignalWatchList);
    m_unReplayTimeDelay             = 50;
    m_unCycleTimeDelay              = 0;
    m_unTimerSB                     = 0;
    m_unFlexRayNSTimer              = 0;
    m_omStrPrevLoadedDll            = "";
    m_bIsSendingMsg                 = FALSE;
    m_bMessageName                  = TRUE;
    //m_bDateActive                   = FALSE;
    m_bAbortMsgHandler              = FALSE;
    m_bMsgHandlerRxDataByte         = FALSE;
    m_bNoHardwareFound              = true;
    // Clear all MRU strings
    m_omStrMRU_ConfigurationFiles[0]    = _(defSTR_DEFAULT_MRU_ITEM);
    m_omStrMRU_ConfigurationFiles[1]    = STR_EMPTY;
    m_omStrMRU_ConfigurationFiles[2]    = STR_EMPTY;
    m_omStrMRU_ConfigurationFiles[3]    = STR_EMPTY;
    m_omStrMRU_ConfigurationFiles[4]    = STR_EMPTY;

    // Config menu option - default
    m_bCfgNewMenuOption     = TRUE;
    m_bCfgLoadMenuOption    = TRUE;
    m_bCfgSaveMenuOption    = TRUE;
    m_bCfgSaveAsMenuOption  = TRUE;

    // Send toolbar button is not enabled.
    //m_bEnableSendToolbarButton = FALSE;
    m_bIsStatWndCreated = FALSE;
    m_bIsFlexRayStatWndCreated == FALSE;
    m_bNotificWndVisible         = FALSE;
    // Initially the controller will be in error active mode.
    for( UINT unIndex = 0; unIndex < defNO_OF_CHANNELS; unIndex++)
    {
        m_eCurrErrorState[ unIndex ] = ERROR_ACTIVE;
    }

    m_unWarningLimit  = 96;
    m_podUIThread = NULL;
    m_omStrSavedConfigFile = STR_EMPTY;
    // PTV [1.6.4]
    m_unTimerSBLog = NULL;

    m_unJ1939TimerSBLog = NULL;

    // Graph related pointers
    // Graph window status
    for(int nBUSID = CAN; nBUSID<AVAILABLE_PROTOCOLS; nBUSID++)
    {
        m_sGraphWndPlacement[nBUSID].length = 0;
        m_sGraphWndPlacement[nBUSID].rcNormalPosition.top = -1;
        m_sGraphSplitterPos[nBUSID].m_nRootSplitterData[0][0] = -1;
    }
    //SGW Code commented by Arun 21-10-2010
    //m_bGraphWindowVisible = FALSE;
    // Graph Child
    //m_pomGraphChild = NULL;
    // Graph UI Thread
    //m_pomGraphThread = NULL;
    // Init Graph Control
    //m_podGraphControl = NULL;
    //SGW Code commented by Arun 21-10-2010

    // Init Bus Statistics pointer
    m_podBusStatistics = NULL;
    m_podFlexRayBusStatistics = NULL;
    // Initialise Tx Msg Window Pointer
    //m_pomTxMsgChildWindow = NULL;
    //Initialize notific window position
    m_sNotificWndPlacement.length = 0;
    m_sNotificWndPlacement.rcNormalPosition.top = -1;

    // Create message manager
    //venkat
    //Initialize HW Interface
    //INITIALISE_ARRAY(m_asINTERFACE_HW);
    //Initialize CONTROLLER DETAILS
    //INITIALISE_ARRAY(m_asControllerDetails);
    //Default Hw mode
    m_byControllerMode = defMODE_ACTIVE;
    char acTmp[MAX_PATH] = {'\0'};
    GetCurrentDirectory(MAX_PATH, acTmp);
    m_omAppDirectory = acTmp;
    m_bInterPretMsg = FALSE;
    m_pouMsgInterpretBuffer = NULL;
    m_objTxHandler.vLoadTx_DLL();
    m_objFlexTxHandler.vLoadTx_DLL();
    //m_objTxHandlerLin.vLoadTx_DLL();
    m_objSigGrphHandler.vLoadSigGrph_DLL();
    m_pouMsgInterpretBuffer = m_objSigGrphHandler.vGetGraphBuffer();

    INITIALISE_ARRAY(m_anMsgBuffSize);
    INITIALISE_DATA(m_sMsgFrmtWndDets);
    m_sMsgWndPlacement.length = 0;//MSG_WND_PLACEMENT
    m_sMsgInterpretPlacement.length = 0;
    //Driver selection popup menu updation
    m_pDILSubMenu = NULL;
    m_pFlxDILSubMenu = NULL;
    m_dwDriverId = DRIVER_CAN_STUB;
    m_pouTxMsgWndJ1939 = NULL;
    m_sJ1939ClientParam.m_byAddress = ADDRESS_NULL;
    m_ouGraphReadThread.m_hActionEvent = NULL;
    m_ouGraphReadThread.m_unActionCode = IDLE;
    m_nTimeStamp = 0;
    m_podMsgSgWndJ1939 = NULL;
    m_pouMsgSigJ1939   = NULL;
    m_pouActiveDbJ1939 = NULL;
    m_pouMsgSigFLEXRAY = NULL;
    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        m_abLogOnConnect[i] = FALSE;
    }
    //venkat
    m_objTSEditorHandler.vLoadTSEditor_DLL();
    m_objTSExecutorHandler.vLoadTSExecutor_DLL();


    //MVN
    m_xmlConfigFiledoc = NULL;
    m_bIsXmlConfig = TRUE;
    //~MVN

    // language support
    vGettextBusmaster();

    m_bUseAdvancedUILib = false;
    m_hModAdvancedUILib = NULL;
    m_bytIconSize       = 20;

    /* Try to load resource DLL for icons*/
    m_hModAdvancedUILib = ::LoadLibrary("AdvancedUIPlugIn.dll");
    /* If the AdvancedUIPlugIn DLL exists, use it */
    if ( m_hModAdvancedUILib )
    {
        m_bytIconSize       = 32;
        m_bUseAdvancedUILib = true;
    }
    /* Set the associated FIBEX Database/configuration files list */
    m_acFlexDBConfigInfo.m_nFilesCount = 0;
    m_acFlexDBConfigInfo.m_nSelectedIndex = 0;

    /* Set the FLEXRAY Monitor client ID to NULL */
    m_dwFLEXClientID = NULL;
    m_bFRConfigModified = false;
    /* Set default FlexRay DIL selection to NONE */
    m_shFLEXRAYDriverId = DAL_NONE;

    /* Initialize FlexRay DB object */
    m_pouMsgSigFLEXRAY = new CMsgSignal(sg_asDbParams[FLEXRAY], FALSE);
    m_pouMsgSigLIN = new CMsgSignal(sg_asDbParams[LIN], FALSE);

    m_bFlxDILChanging = FALSE;

    m_shLINDriverId = DAL_NONE;


}
/*******************************************************************************
 Function Name    : ~CMainFrame
 Input(s)         :
 Output           :
 Functionality    : Destructor
 Member of        : CMainFrame
 Friend of        :     -
 Author(s)        : Amarnath Shastry,Amitesh Bharthi,Ratnadip Choudhury
 Date Created     : 05.03.2002
 Modifications    : Raja N on 10.05.2004
                    Removed the deletion of memory m_punTrigMsg as it will be
                    deleted in the Destructor of CMsgWnd.
 Modifications    : Raja N on 26.04.2005
                    Added code to delete global tx list and tx message manager
*******************************************************************************/
CMainFrame::~CMainFrame()
{

    m_objTxHandler.vDeleteTxBlockMemory();
    m_objFlexTxHandler.vDeleteTxBlockMemory();
    m_objTxHandlerLin.vDeleteTxBlockMemory();

    vReleaseSignalWatchListMemory(m_psSignalWatchList[CAN]);
    vReleaseSignalWatchListMemory(m_psSignalWatchList[J1939]);

    m_pDILSubMenu->DestroyMenu();
    DELETE_PTR(m_pDILSubMenu);
    m_pFlxDILSubMenu->DestroyMenu();
    DELETE_PTR(m_pFlxDILSubMenu);
    DELETE_PTR(m_podMsgWndThread);
    if (m_pouMsgSigJ1939 != NULL)
    {
        m_pouMsgSigJ1939->bDeAllocateMemory(STR_EMPTY);
    }
    DELETE_PTR(m_pouMsgSigJ1939);

    if (m_pouActiveDbJ1939 != NULL)
    {
        m_pouActiveDbJ1939->bDeAllocateMemoryInactive();
    }
    DELETE_PTR(m_pouActiveDbJ1939);

    if (m_pouTxMsgWndJ1939 != NULL)
    {
        m_pouTxMsgWndJ1939->DestroyWindow();
    }
    DELETE_PTR(m_pouTxMsgWndJ1939);
    CTxMsgWndJ1939::vClearDataStore();

    /* Free the resource DLL */
    if ( m_bUseAdvancedUILib && m_hModAdvancedUILib )
    {
        ::FreeLibrary(m_hModAdvancedUILib);
        m_hModAdvancedUILib = NULL;
    }
}

/******************************************************************************
    Function Name    :  vGettextBusmaster
    Input(s)         :  -
    Output           :  -
    Functionality    :  National Language Support
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :
    Date Created     :  01.05.2012
******************************************************************************/

void CMainFrame::vGettextBusmaster()
{
    setlocale(LC_ALL, "");
    bindtextdomain("BUSMASTER", getenv("LOCALDIR"));
    textdomain("BUSMASTER");
}

/******************************************************************************
    Function Name    :  nFindMenuItem
    Input(s)         :  CMenu* Menu, LPCTSTR MenuString
    Output           :  int
    Functionality    :  FindMenuItem() will find a menu item string from the
                        specified popup menu and returns its position (0-based)
                        in the specified popup menu.
                        It returns -1 if no such menu item string is found.
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath Shastry
    Date Created     :  15.11.2002
******************************************************************************/
int CMainFrame::nFindMenuItem(CMenu* Menu, LPCTSTR MenuString)
{
    ASSERT(Menu);
    ASSERT(::IsMenu(Menu->GetSafeHmenu()));
    int count = Menu->GetMenuItemCount();

    for (int i = 0; i < count; i++)
    {
        CString str;

        if (Menu->GetMenuString(i, str, MF_BYPOSITION) &&
                (strcmp(str, MenuString) == 0))
        {
            return i;
        }
    }

    return -1;
}

static BOOL bIsMsgExist(UINT MsgId, const SMSGENTRY* psFromList, sMESSAGE*& psMsg)
{
    BOOL bResult = FALSE;
    if (psFromList != NULL)
    {
        const SMSGENTRY* pTempMsgEntry = psFromList;
        while ((pTempMsgEntry != NULL))
        {
            if (pTempMsgEntry->m_psMsg->m_unMessageCode == MsgId)
            {
                psMsg = pTempMsgEntry->m_psMsg;
                bResult = TRUE;
                break;
            }
            pTempMsgEntry = pTempMsgEntry->m_psNext;
        }
    }
    return bResult;
}
static BOOL bIsSigExist(const CString& omSigName, sSIGNALS* pSigList, sSIGNALS*& pSig)
{
    BOOL bResult = FALSE;
    while (pSigList != NULL)
    {
        if (omSigName == pSigList->m_omStrSignalName)
        {
            pSig = pSigList;
            bResult = TRUE;
            break;
        }
        pSigList = pSigList->m_psNextSignalList;
    }
    return bResult;
}

static void vPopulateMainEntryList(CMainEntryList* podResultingList, const SMSGENTRY* psExistingList,
                                   CMsgSignal* pouDatabase)
{
    if ((podResultingList != NULL) && (pouDatabase != NULL))
    {
        UINT nNoMsgs = pouDatabase->unGetNumerOfMessages();
        UINT* pMsgIds = new UINT[nNoMsgs];
        pouDatabase->unListGetMessageIDs(pMsgIds);
        for (UINT i = 0; i < nNoMsgs; i++)
        {
            SMAINENTRY sMainEntry;
            sMESSAGE* pMsg = pouDatabase->psGetMessagePointer(pMsgIds[i]);
            if (pMsg != NULL)
            {
                sMainEntry.m_unMainEntryID = pMsg->m_unMessageCode;
                sMainEntry.m_omMainEntryName = pMsg->m_omStrMessageName;
                sMESSAGE* pListMsg = NULL;
                if (bIsMsgExist(pMsg->m_unMessageCode, psExistingList, pListMsg))
                {
                    sSIGNALS* pSig = pMsg->m_psSignals;
                    while (pSig != NULL)
                    {
                        SSUBENTRY sSubEntry;
                        sSubEntry.m_omSubEntryName = pSig->m_omStrSignalName;
                        sSIGNALS* pJunk = NULL;
                        if (bIsSigExist(pSig->m_omStrSignalName, pListMsg->m_psSignals, pJunk))
                        {
                            sMainEntry.m_odSelEntryList.AddTail(sSubEntry);
                        }
                        else
                        {
                            sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);
                        }
                        pSig = pSig->m_psNextSignalList;
                    }
                }
                else //If Msg does not exist copy the full list directly
                {
                    sSIGNALS* pSig = pMsg->m_psSignals;
                    while (pSig != NULL)
                    {
                        SSUBENTRY sSubEntry;
                        sSubEntry.m_omSubEntryName = pSig->m_omStrSignalName;
                        sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);
                        pSig = pSig->m_psNextSignalList;
                    }
                }
                podResultingList->AddTail(sMainEntry);
            }
        }//End of msg for loop
        //delete the ID array
        delete[] pMsgIds;
        pMsgIds = NULL;
    }
}


/******************************************************************************
 Function Name    : OnCreate
 Input(s)         : LPCREATESTRUCT
 Output           : int
 Functionality    : #Called by the framework while a window is being created
                    #Creates toolbar, status bar.
                    #Creates combo box in the menu.
                    #Creates Dropdwon menu for signal watch window.
                    #Creates Dropdwon menu for time mode.
 Member of        : CMainFrame
 Friend of        :     -
 Author(s)        : Amarnath Shastry
 Date Created     : 05.03.2002
 Modifications    : Ratnadip Choudhury
 Date Modified    : 05.03.2002
 Modifications    : Amitesh Bharti
                    26.11.2002, The double line toolbar is reverted back to
                                single line
 Modifications    : Krishnaswamy B.N
                    03.06.2003, The style of the time mode toolbar button
                    changed to dropdown type.
 Modifications    : Raja N
                    28.04.2004,Replaced hardcoded string "Empty" with
                    defSTR_DEFAULT_MRU_ITEM
 Modifications    : Raja N
                    12.05.2004, Added tool tip for Bus Status
 Modifications    : Raja N
                    07.09.2004, Added code to avoid setting tool tip in case
                    of USB and to set empty text to "Error Counter" tab.
 Modifications    : Raja N
                    15.09.2004, Added code to support tool tip as the controller
                    state support is implemented
 Modifications    : Raja N
                    14.03.2005, Removed error counter and controller status pans
                    Added a pane to show number of channels supported by the
                    application
Modifications    : ArunKumar K
                    20.05.2010, Added code to load toolbar positions
                    previosly saved.
                    Added new toolbar groups.
Modifications    : Ashwin R Uchil
                    7.8.2012, Added toolbar configuration loading from global xml file
/******************************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }
    vGetWinStatus(m_WinCurrStatus);
    // Update the window placement
    SetWindowPlacement(&m_WinCurrStatus);

    // The node simulation toolbar
    CREATE_TOOLBAR(this, m_wndToolbarNodeSimul, IDR_NODE_SIMULATION, _T(_("Node Simulation")));
    CREATE_TOOLBAR(this, m_wndToolbarMsgWnd, IDR_MSG_WND, _T(_("Message Window")));
    CREATE_TOOLBAR(this, m_wndToolBar, IDR_MAINFRAME, _T(_("Main")));
    CREATE_TOOLBAR(this, m_wndToolbarConfig, IDR_TLB_CONFIGURE, _T(_("Configure")));
    CREATE_TOOLBAR(this, m_wndToolbarJ1939, IDR_FUNCTIONS_J1939, _T(_("J1939")));
    CREATE_TOOLBAR(this, m_wndToolbarCANDB, IDR_CAN_DATABASE, _T(_("CAN Database")));
    CREATE_TOOLBAR(this, m_wndToolbarFlexRay, IDB_TLB_FLEXRAY, _T(_("FlexRay")));
    CREATE_TOOLBAR(this, m_wndToolbarConfiguration, IDR_CONFIG_TOOLBAR, _T(_("Configuration")));
    CREATE_TOOLBAR(this, m_wndToolbarLIN, IDB_LIN_IMG, _T(_("LIN")));

    /* Set toolbar button size to small to fit default icons*/
    if ( !m_bUseAdvancedUILib )
    {
        CSize objSize;
        /* 27 pixels is the default size of toolbar icons */
        objSize.cx = 27;
        objSize.cy = 27;
        vSetToolbarButtonSize( m_wndToolBar,            objSize);
        vSetToolbarButtonSize( m_wndToolbarNodeSimul,   objSize);
        vSetToolbarButtonSize( m_wndToolbarMsgWnd,      objSize);
        vSetToolbarButtonSize( m_wndToolbarConfig,      objSize);
        vSetToolbarButtonSize( m_wndToolbarCANDB,       objSize);
        vSetToolbarButtonSize( m_wndToolbarJ1939,       objSize);
        vSetToolbarButtonSize( m_wndToolbarFlexRay,       objSize);
        vSetToolbarButtonSize( m_wndToolbarConfiguration,       objSize);
        vSetToolbarButtonSize( m_wndToolbarLIN,       objSize);
    }

    m_wndToolbarNodeSimul.bLoadCNVTCToolBar(m_bytIconSize, IDB_NODE_SIMULATION,IDB_NODE_SIMULATION_HOT, IDB_NODE_SIMULATION_DISABLED);
    m_wndToolBar.bLoadCNVTCToolBar(m_bytIconSize, IDB_MAINFRAME,IDB_MAINFRAME_HOT, IDB_MAINFRAME_DISABLED);
    m_wndToolbarMsgWnd.bLoadCNVTCToolBar(m_bytIconSize, IDB_MSG_WND,IDB_MSG_WND_HOT, IDB_MSG_WND_DISABLED);
    m_wndToolbarConfig.bLoadCNVTCToolBar(m_bytIconSize, IDB_CONFIGURE,IDB_CONFIGURE_HOT, IDB_CONFIGURE_DISABLED);
    m_wndToolbarCANDB.bLoadCNVTCToolBar(m_bytIconSize, IDB_CAN_DATABASE,IDB_CAN_DATABASE_HOT, IDB_CAN_DATABASE_DISABLED);
    m_wndToolbarJ1939.bLoadCNVTCToolBar(m_bytIconSize, IDB_TLB_J1939,IDB_TLB_J1939_HOT, IDB_TLB_J1939_DISABLED);
    m_wndToolbarFlexRay.bLoadCNVTCToolBar(m_bytIconSize, IDB_TLB_FLEXRAY,IDB_TLB_FLEXRAY_HOT, IDB_TLB_FLEXRAY_DISABLED);
    m_wndToolbarConfiguration.bLoadCNVTCToolBar(m_bytIconSize, IDR_CONFIG_TOOLBAR,IDR_CONFIG_TOOLBAR_HOT, IDR_CONFIG_TOOLBAR_DISABLED);
    m_wndToolbarLIN.bLoadCNVTCToolBar(m_bytIconSize, IDB_LIN_IMG,IDB_LIN_IMGHOT, IDB_LIN_IMGDISABLED);

    EnableDocking(CBRS_ALIGN_ANY);

    bSetDefaultToolbarPosition();
    if (!m_wndStatusBar.CreateEx(this,SBT_TOOLTIPS) ||
            !m_wndStatusBar.SetIndicators(indicators,
                                          sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0(_("Failed to create status bar\n"));
        return -1;      // fail to create
    }
    LoadBarState(PROFILE_CAN_MONITOR);




    //CheckDlgButton(IDR_TOOL_HEXDEC,BST_CHECKED);
    if (!m_wndStatusBar.CreateEx(this,SBT_TOOLTIPS) ||
            !m_wndStatusBar.SetIndicators(indicators,
                                          sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // Set pane info for displaying active database name
    m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 340);

    m_wndStatusBar.SetPaneInfo( INDEX_CHANNELS, IDS_CHANNELS,
                                SBPS_NOBORDERS, 320);
    m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 340);

    // PTV [1.6.4]
    // For Can Logging status
    m_wndStatusBar.SetPaneInfo( INDEX_CAN_LOG_ICON, ID_LOG_RECORD_CAN_ICON,
                                SBPS_NOBORDERS, 100);

    // PTV [1.6.4]
    // For J1939 Logging status
    m_wndStatusBar.SetPaneInfo( INDEX_J1939_LOG_ICON, ID_LOG_RECORD_J1939_ICON,
                                SBPS_NOBORDERS, 110);

    m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 340);
    // PTV [1.6.4]

    m_wndStatusBar.SetPaneInfo( INDEX_DB_NAME, ID_ACTIVE_DATABASE_NAME,
                                SBPS_STRETCH, 50);

    // PTV [1.6.4]
    m_hLogIcon1 = (HICON)::LoadImage(AfxGetInstanceHandle(),
                                     MAKEINTRESOURCE(IDI_ICON_LOG_ON),
                                     IMAGE_ICON, 16, 16, LR_SHARED);

    m_hLogIcon2 = (HICON)::LoadImage(AfxGetInstanceHandle(),
                                     MAKEINTRESOURCE(IDI_ICON_LOG_ONLIGHT),
                                     IMAGE_ICON, 16, 16, LR_SHARED);

    m_hLogOffIcon = (HICON)::LoadImage(AfxGetInstanceHandle(),
                                       MAKEINTRESOURCE(IDI_ICON_LOG_OFF),
                                       IMAGE_ICON, 16, 16, LR_SHARED);

    m_bIconSetFlag = 1;
    m_bJ1939IconSetFlag = 1;
    m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
    m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_J1939_LOG_ICON, m_hLogOffIcon);
    // PTV [1.6.4] END
    // Set number of channels supported
    CString omStrChannels;
    omStrChannels.Format( _(defSTR_CHANNELS_SUPPORTED),
                          CHANNEL_ALLOWED,
                          _(defSTR_CHANNEL_NAME) );
    // Set Channel string
    m_wndStatusBar.SetPaneText(INDEX_CHANNELS, omStrChannels );

    // Create the timer which will be fired to update the status bar.
    m_unTimerSB = SetTimer(TIMER_REFRESH_MAINGUI, STSBAR_REFRESH_TIME_PERIOD,
                           NULL);

    // Set Drop down style for Toolbars containing TBSTYLE_DROPDOWN styles.
    m_wndToolBar.GetToolBarCtrl().SetExtendedStyle( TBSTYLE_EX_DRAWDDARROWS );
    m_wndToolbarMsgWnd.GetToolBarCtrl().SetExtendedStyle( TBSTYLE_EX_DRAWDDARROWS );
    //m_wndToolbarMsgWnd.GetToolBarCtrl().SetButtonWidth(20,25);

    TBBUTTONINFO tbi;

    tbi.dwMask= TBIF_STYLE;
    tbi.cbSize= sizeof(TBBUTTONINFO);

    m_wndToolbarMsgWnd.GetToolBarCtrl().GetButtonInfo(
        IDM_DISPLAY_MESSAGE_DISPLAYRELATIVETIME, &tbi);


    tbi.fsStyle |= BTNS_WHOLEDROPDOWN | TBSTYLE_BUTTON | BTNS_SHOWTEXT;

    m_wndToolbarMsgWnd.GetToolBarCtrl().SetButtonInfo(
        IDM_DISPLAY_MESSAGE_DISPLAYRELATIVETIME, &tbi);

    tbi.cbSize = sizeof tbi;
    tbi.dwMask = TBIF_SIZE;

    if ( m_bUseAdvancedUILib )
    {
        /* As the width of toolbar item is more, the tick mark position for timer button should be moved */
        tbi.cx = (WORD)42;
    }
    else
    {
        tbi.cx = (WORD)30;
    }
    m_wndToolbarMsgWnd.GetToolBarCtrl().SetButtonInfo(IDM_DISPLAY_MESSAGE_DISPLAYRELATIVETIME, &tbi);

    //vGetWinStatus(m_WinCurrStatus);

    // Set the text for the first sub menu item under
    // "File -> Recent Configurations -> Empty" menu option
    // The first sub menu is statically created and is disabled.
    // So enable it and change the text top the first MRU file name
    // Then create menu for the rest of the MRU files
    DWORD dwVal;
    theApp.bReadFromRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE1), REG_SZ, m_omStrMRU_ConfigurationFiles[0], dwVal);
    theApp.bReadFromRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE2), REG_SZ, m_omStrMRU_ConfigurationFiles[1], dwVal);
    theApp.bReadFromRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE3), REG_SZ, m_omStrMRU_ConfigurationFiles[2], dwVal);
    theApp.bReadFromRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE4), REG_SZ, m_omStrMRU_ConfigurationFiles[3], dwVal);
    theApp.bReadFromRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE5), REG_SZ, m_omStrMRU_ConfigurationFiles[4], dwVal);

    // Create MRU under "Recent Configurations" menu item
    vCreateMRU_Menus();

    //Update DIL List
    if (g_pouDIL_CAN_Interface == NULL)
    {
        DIL_GetInterface(CAN, (void**)&g_pouDIL_CAN_Interface);
    }
    m_nDILCount = g_pouDIL_CAN_Interface->DILC_GetDILList(false, &m_ouList);

    //Update FlexRay DIL List
    if (g_pouDIL_FLEXRAY_Interface == NULL)
    {
        DIL_GetInterface(FLEXRAY, (void**)&g_pouDIL_FLEXRAY_Interface);
    }
    m_nFlexRayDILCount = g_pouDIL_FLEXRAY_Interface->DILF_GetDILList(false, &m_ouFlexRayList);

    if (g_pouDIL_LIN_Interface == NULL)
    {
        DIL_GetInterface(LIN, (void**)&g_pouDIL_LIN_Interface);
    }
    m_nDILCountLin = g_pouDIL_LIN_Interface->DILL_GetDILList(false, &m_ouListLin);

    // Do initialisation for the waveform transmission object
    m_ouWaveTransmitter.vDoInitialisation(&m_objWaveformDataHandler,
                                          g_pouDIL_CAN_Interface, &(theApp.m_pouMsgSignal));

    // CG: The following line was added by the Splash Screen component.
    CSplashScreen::DisplaySplashScreen(this, SW_SHOW);
    Sleep(1500);

    // Setting Hex Mode by default
    bSetHexDecFlags(TRUE);
    //theApp.pouGetFlagsPtr()->vSetFlagStatus(HEX,TRUE);
    //m_objFlexTxHandler.SetFibexConfig(FLEXRAY, m_ouClusterConfig[FLEXRAY]);


    vInitialiaseLINConfig(1);
    m_objFlexTxHandler.SetFibexConfig(LIN, m_ouClusterConfig[LIN]);

    //TODO::Venkat - Appropriate Place has to be decided.
    bInitFrameProcLIN(); // Initialize logger module
    //vInitializeBusStatLIN();



    return 0;
}

/******************************************************************************
 Function Name    : bSetDefaultToolbarPosition
 Input(s)         : -
 Output           : bool - failure/ success
 Functionality    : sets the default position for the toolbar
 Member of        : CMainFrame
 Friend of        :     -
 Author(s)        : Ashwin. R. Uchil
 Date Created     : 06.09.2012
 ******************************************************************************/
bool CMainFrame::bSetDefaultToolbarPosition()
{
    DockControlBar(&m_wndToolbarConfiguration, AFX_IDW_DOCKBAR_TOP);             //set the first toolbar at the top
    DockControlBarLeftOf(&m_wndToolBar,&m_wndToolbarConfiguration);        //align others next to it
    DockControlBarLeftOf(&m_wndToolbarMsgWnd,&m_wndToolBar);        //align others next to it
    DockControlBarLeftOf( &m_wndToolbarCANDB, &m_wndToolbarMsgWnd);
    DockControlBarLeftOf( &m_wndToolbarNodeSimul, &m_wndToolbarCANDB);
    DockControlBarLeftOf( &m_wndToolbarFlexRay,&m_wndToolbarNodeSimul);
    DockControlBar(&m_wndToolbarJ1939,AFX_IDW_DOCKBAR_LEFT);        //align the toolabr to the left
    DockControlBarLeftOf( &m_wndToolbarConfig, &m_wndToolbarJ1939); //aign the next toolbar below it
    DockControlBar(&m_wndToolbarLIN,AFX_IDW_DOCKBAR_LEFT);        //align the toolabr to the left
    DockControlBarLeftOf( &m_wndToolbarConfig, &m_wndToolbarLIN); //aign the next toolbar below it
    return true;
}

/******************************************************************************
 Function Name    : nGetToolBarNodeFrmDocSection
 Input(s)         : xmlXPathObjectPtr - objectpath to user-defined node
 Output           : bool - failure/ success
 Functionality    : Gets the node position from the object path
 Member of        : CMainFrame
 Friend of        :     -
 Author(s)        : Ashwin. R. Uchil
 Date Created     : 06.09.2012
 ******************************************************************************/
//int CMainFrame::nGetToolBarNodeFrmDocSection(xmlXPathObjectPtr pObjectPath)
//{
//  int         nPosSet = 0;
//  xmlNodePtr pNode, pChildNode;
//  if( NULL != pObjectPath )
//  {
//      xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;   //get the nodeset from user defined section
//      if( NULL != pNodeSet )
//      {
//          pNode = pNodeSet->nodeTab[0];       //Take First One only from node set
//      }
//      if( NULL != pNode )
//      {
//          pNode = pNode->xmlChildrenNode;     //ToolBar_main or ToolBar_MsgWnd etc
//      }
//      while (pNode != NULL)
//      {
//          //the order in which you write the code to get the node doesn't matter since since its in while loop
//          //and it will match the respective if-condition each time
//          //node simulation
//          if ((!xmlStrcmp(pNode->name, (const xmlChar *)DEF_TOOLBAR_NODE_SIM)))
//          {
//              pChildNode = pNode->xmlChildrenNode;
//              if(nGetToolBarNodes(pChildNode,m_wndToolbarNodeSimul,  IDR_NODE_SIMULATION, _T("Node Simulation"))== 1)
//              {
//                  nPosSet++;
//              }
//          }
//          //Message Window
//          if ((!xmlStrcmp(pNode->name, (const xmlChar *)DEF_TOOLBAR_MSG_WND)))
//          {
//              pChildNode = pNode->xmlChildrenNode;
//              if(nGetToolBarNodes(pChildNode,m_wndToolbarMsgWnd, IDR_MSG_WND, _T("Message Window"))==1)
//              {
//                  nPosSet++;
//              }
//          }
//          //Main
//          if ((!xmlStrcmp(pNode->name, (const xmlChar *)DEF_TOOLBAR_MAIN)))
//          {
//              pChildNode = pNode->xmlChildrenNode;
//              if(nGetToolBarNodes(pChildNode,m_wndToolBar, IDR_MAINFRAME, _T("Main"))==1)
//              {
//                  nPosSet++;
//              }
//          }
//          //Configure
//          if ((!xmlStrcmp(pNode->name, (const xmlChar *)DEF_TOOLBAR_CONFIG)))
//          {
//              pChildNode = pNode->xmlChildrenNode;
//              if(nGetToolBarNodes(pChildNode,m_wndToolbarConfig, IDR_TLB_CONFIGURE, _T("Configure"))==1)
//              {
//                  nPosSet++;
//              }
//          }
//          //CAN Database
//          if ((!xmlStrcmp(pNode->name, (const xmlChar *)DEF_TOOLBAR_CAN_DB)))
//          {
//              pChildNode = pNode->xmlChildrenNode;
//              if(nGetToolBarNodes(pChildNode,m_wndToolbarCANDB, IDR_CAN_DATABASE, _T("CAN Database"))==1)
//              {
//                  nPosSet++;
//              }
//          }
//          //J1939
//          if ((!xmlStrcmp(pNode->name, (const xmlChar *)DEF_TOOLBAR_J1939)))
//          {
//              pChildNode = pNode->xmlChildrenNode;
//              if(nGetToolBarNodes(pChildNode, m_wndToolbarJ1939, IDR_FUNCTIONS_J1939, _T("J1939"))==1)
//              {
//                  nPosSet++;
//              }
//          }
//          pNode = pNode->next;
//      }
//      if(nPosSet == 6)        //if all the 6 positions are retreived only then send as true
//      {
//          return true;
//      }
//      else
//      {
//          return false;
//      }
//  }
//  else
//  {
//      return false;
//  }
//}
/******************************************************************************
 Function Name    : nGetToolBarNodes
 Input(s)         : xmlNodePtr  - node of each toolbar containing the position
                    CToolBar&   - reference to toolbar object
                    UINT        - ID of the toolbar
                    CString     - Name of the toolbar
 Output           : int - failure/ success
 Functionality    : Gets the node position from the toolbar node
 Member of        : CMainFrame
 Friend of        :     -
 Author(s)        : Ashwin. R. Uchil
 Date Created     : 06.09.2012
 ******************************************************************************/
//int CMainFrame::nGetToolBarNodes(xmlNodePtr pNode, CToolBar& omToolbar, UINT unID, CString omTitle)
//{
//  int         nPosRetreived = 0;
//  int         nVisible;
//  int         nDock;
//  CRect       rRec;
//  string      strVar, strName;
//  UINT        unStlye;
//
//  //get the postion from the indvidual nodes
//  while(pNode != NULL)
//  {
//      if (xmlUtils::GetDataFrmNode(pNode,DEF_NAME,strVar))
//      {
//          strName = strVar;
//          nPosRetreived++;
//      }
//      if (xmlUtils::GetDataFrmNode(pNode,DEF_TOP,strVar))
//      {
//          rRec.top = atoi(strVar.c_str());
//          nPosRetreived++;
//      }
//      if (xmlUtils::GetDataFrmNode(pNode,DEF_Left,strVar))
//      {
//          rRec.left = atoi(strVar.c_str());
//          nPosRetreived++;
//      }
//      if (xmlUtils::GetDataFrmNode(pNode,DEF_Bottom,strVar))
//      {
//          rRec.bottom = atoi(strVar.c_str());
//          nPosRetreived++;
//      }
//      if (xmlUtils::GetDataFrmNode(pNode,DEF_Right,strVar))
//      {
//          rRec.right = atoi(strVar.c_str());
//          nPosRetreived++;
//      }
//      if (xmlUtils::GetDataFrmNode(pNode,DEF_Bottom,strVar))
//      {
//          rRec.bottom = atoi(strVar.c_str());
//          nPosRetreived++;
//      }
//      if (xmlUtils::GetDataFrmNode(pNode,DEF_IS_VISIBLE,strVar))
//      {
//          nVisible = atoi(strVar.c_str());
//          if(nVisible == 0)
//              nVisible = SW_HIDE;
//          else if(nVisible == 1)
//              nVisible = SW_SHOW;
//          nPosRetreived++;
//      }
//      if (xmlUtils::GetDataFrmNode(pNode,DEF_TOOLBAR_ALIGNMENT,strVar))
//      {
//
//          if(strVar == "TOP")
//          {
//              unStlye = AFX_IDW_DOCKBAR_TOP;
//          }
//          else if(strVar == "BOTTOM")
//          {
//              unStlye = AFX_IDW_DOCKBAR_BOTTOM;
//          }
//          else if(strVar == "LEFT")
//          {
//              unStlye = AFX_IDW_DOCKBAR_LEFT;
//          }
//          else if(strVar == "RIGHT")
//          {
//              unStlye = AFX_IDW_DOCKBAR_RIGHT;
//          }
//          else
//          {
//              unStlye = AFX_IDW_DOCKBAR_TOP;
//          }
//          nPosRetreived++;
//      }
//      if (xmlUtils::GetDataFrmNode(pNode,DEF_TOOLBAR_DOCKING,strVar))
//      {
//          nDock = atoi(strVar.c_str());
//          nPosRetreived++;
//      }
//      pNode = pNode->next;
//  }
//  if(nPosRetreived != 9)      //if all the nodes are not recieved then return false
//      return false;
//
//  if(nDock)           //if toolbar was docked then set toolbar to docking position
//  {
//      CRect       rFrameSize(-4,-4,1604,1170);
//
//      //GetWindowRect(&rFrameSize);
//      ClientToScreen(&rRec);
//      //rRec.top -= rFrameSize.top;
//      //rRec.left -= rFrameSize.left;
//      //rRec.bottom -= rFrameSize.top;
//      //rRec.right -= rFrameSize.left;
//      if(rRec.right > rFrameSize.right)
//      {
//          rRec.left = rFrameSize.right - (rRec.right - rRec.left) -5; //subtract the width from the right of frame
//          rRec.right = rFrameSize.right - 5;
//      }
//      DockControlBar(&omToolbar, unStlye,&rRec);
//      //omToolbar.MoveWindow(&rRec);
//      omToolbar.ShowWindow( nVisible );
//  }
//  else
//  {
//      CPoint      ptTopLeft;
//      ptTopLeft.x = rRec.left - 2;            //2 is subtracted to nullify the shift during setting the toolbar
//      ptTopLeft.y = rRec.top - 20;        //20 is subtracted to nullify the shift during setting the toolbar
//      FloatControlBar(&omToolbar,ptTopLeft, unStlye); //if the toolbar was floating then create the same
//      ShowControlBar(&omToolbar, nVisible, FALSE);
//  }
//  return true;
//}
//int CMainFrame::nCreateToolbarFrmXML(CToolBar& omToolbar, UINT unID,CRect rRect,
//                               CString omTitle)
//{
//  int Result = 0;
//    if (!omToolbar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT,
//                            WS_BORDER | WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
//                            CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), unID) ||
//            !omToolbar.LoadToolBar(unID))
//    {
//        TRACE0("Failed to create toolbar\n");
//        Result = -1;      // fail to create
//    }
//    else
//    {
//        omToolbar.EnableDocking(CBRS_ALIGN_ANY);
//        omToolbar.SetWindowText((LPCTSTR) omTitle);
//      omToolbar.SetWindowPos(&CWnd::wndBottom,rRect.left,rRect.top,(rRect.right-rRect.left),(rRect.bottom-rRect.top),SWP_NOZORDER|SWP_SHOWWINDOW);
//    }
//   return Result;
//}
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

// HELPER FUNCTIONS -- START
int CMainFrame::nCreateToolbar(CWnd* pomParent, CToolBar& omToolbar, UINT unID,
                               CString omTitle)
{
    int Result = 0;

    if (!omToolbar.CreateEx(pomParent, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT,
                            WS_BORDER | WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
                            CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), unID) ||
            !omToolbar.LoadToolBar(unID))
    {
        TRACE0(_("Failed to create toolbar\n"));
        Result = -1;      // fail to create
    }
    else
    {
        omToolbar.EnableDocking(CBRS_ALIGN_ANY);
        omToolbar.SetWindowText((LPCTSTR) omTitle);
    }

    return Result;
}
void CMainFrame::DockControlBarLeftOf(CToolBar* pomCurrBar,CToolBar* pomLeftOf)
{
    ASSERT(pomCurrBar != NULL);
    ASSERT(pomLeftOf != NULL);

    CRect omRect;

    /* Get MFC to adjust the dimensions of all docked ToolBars so that
    GetWindowRect will be accurate */
    RecalcLayout();
    pomLeftOf->GetWindowRect(&omRect);
    omRect.OffsetRect(1, 0);
    DWORD dwBarStyle = pomLeftOf->GetBarStyle();

    UINT unBarID = 0;
    unBarID = (dwBarStyle & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : unBarID;
    unBarID = (dwBarStyle & CBRS_ALIGN_BOTTOM && unBarID == 0) ? AFX_IDW_DOCKBAR_BOTTOM : unBarID;
    unBarID = (dwBarStyle & CBRS_ALIGN_LEFT && unBarID == 0) ? AFX_IDW_DOCKBAR_LEFT : unBarID;
    unBarID = (dwBarStyle & CBRS_ALIGN_RIGHT && unBarID == 0) ? AFX_IDW_DOCKBAR_RIGHT : unBarID;

    // When we take the default parameters on rect, DockControlBar will dock
    // each Toolbar on a seperate line.  By calculating a rectangle, we in effect
    // are simulating a Toolbar being dragged to that location and docked.
    DockControlBar(pomCurrBar, unBarID, &omRect);
}
// HELPER FUNCTIONS -- END

/******************************************************************************
    Function Name    :  OnNotify
    Input(s)         :  WPARAM wParam, LPARAM lParam, LRESULT* pResult
    Output           :  BOOL
    Functionality    :  #Called by the framework when the user clicks on
                         drop down of signal watch button
                        #Called by the framework when the user clicks on
                         drop down of time mode button
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath Shastry
    Date Created     :  05.03.2002
    Modified By      :  Krishnaswamy B.N
                        03.06.2003, Added code for dropdown functionality for
                        Time Mode toolbar button
******************************************************************************/
BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    // TODO: Add your specialized code here and/or call the base class
#define lpnm   ((LPNMHDR)lParam)
#define lpnmTB ((LPNMTOOLBAR)lParam)

    switch(lpnm->code)
    {
        case TBN_DROPDOWN:
            if(lpnmTB ->iItem == IDM_DISPLAY_MESSAGE_DISPLAYRELATIVETIME)
            {
                vToolBarDropDownMenu(IDM_MENU_TIMEMODE,
                                     IDM_DISPLAY_MESSAGE_DISPLAYRELATIVETIME);
            }
            return FALSE; //indicates the TBN_DROPDOWN
            //notification was handled.
    }
    return CMDIFrameWnd::OnNotify(wParam, lParam, pResult);
}
/******************************************************************************
FUNCTION:           OnOpenDatabase
DESCRIPTION:        Opens a database file for editing
PARAMETERS:
RETURN VALUE:
AUTHOR:             RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002          Initial Version     Amarnath Shastry
Modification Date:  01.04.2002
Modified By:        Amarnath Shastry
Modification        Amarnath Shastry, 17-12-2002
                    Message box displayed if database file is not found
Modification        Raja N
                    10/03/2004 Modified Active/Passive database concept and the
                    database editor will use inactive data structure always for
                    editing purpose
Modifications:      Pradeep Kadoor on 12.06.2009.
                    Code added to enable menu item 'Save and Save & Import'
*******************************************************************************/
void CMainFrame::OnOpenDatabase()
{
    INT nReturn = IDYES;

    // Check if any database is already open
    if ( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ))
    {
        // Some database is open
        // Flash a message as to whether the user
        // wants to open another database
        nReturn =
            AfxMessageBox( _("Are yor sure you want to close the \n\
database that is already open?"), MB_YESNO, MB_ICONINFORMATION);
        if ( nReturn == IDYES)
        {
            // Close the database that was open
            OnCloseDatabase();
        }
    }

    // if the user wants to close prev db
    // and wants new to be open
    // or no database might have opened
    // yet
    if ( nReturn == IDYES)
    {
        BOOL bDisplayEditor = FALSE;

        // Display a open file dialog
        CFileDialog fileDlg( TRUE,      // Open File dialog
                             "dbf",     // Default Extension,
                             NULL,
                             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                             _("BUSMASTER Database File(s)(*.dbf)|*.dbf||"),
                             NULL );

        // Set Title
        fileDlg.m_ofn.lpstrTitle  = _T(_("Select BUSMASTER Database Filename..."));

        if ( IDOK == fileDlg.DoModal() )
        {
            CString strExtName  = fileDlg.GetFileExt();

            CString strDbName   = fileDlg.GetPathName();

            if ( strDbName.ReverseFind('.') )
            {
                strDbName = strDbName.Left( strDbName.ReverseFind('.') + 1);

                strDbName.TrimRight();

                strDbName += strExtName;

                m_omStrDatabaseName = strDbName;

            }

            // file-attribute information
            struct _finddata_t fileinfo;

            // Auto Select DB File
            if (_findfirst( strDbName, &fileinfo)!= -1L)
            {
                // Load the File & fill the Structure
                if (theApp.m_pouMsgSgInactive->
                        bFillDataStructureFromDatabaseFile(strDbName, PROTOCOL_CAN))
                {
                    // No corruption in database, display the editor
                    bDisplayEditor = TRUE;
                }

            }
            else
            {
                AfxMessageBox(_("Specified database file is not found.\n\
Operation unsuccessful."), MB_OK|MB_ICONINFORMATION);
            }

            if ( bDisplayEditor == TRUE )
            {
                if ( m_podMsgSgWnd != NULL )
                {
                    m_podMsgSgWnd = NULL;
                }
                sg_asDbParams[CAN].m_ppvActiveDB = (void**)&theApp.m_pouMsgSgInactive;
                sg_asDbParams[CAN].m_ppvImportedDBs = (void**)&theApp.m_pouMsgSignal;
                m_podMsgSgWnd = new CMsgSignalDBWnd(sg_asDbParams[CAN]);

                if ( m_podMsgSgWnd != NULL )
                {
                    sg_asDbParams[CAN].m_omDBPath = m_omStrDatabaseName;
                    m_podMsgSgWnd->vSetDBName(m_omStrDatabaseName);
                    // Create child window
                    if ( !m_podMsgSgWnd->Create(NULL,
                                                _("Database Editor"),
                                                WS_CHILD | WS_VISIBLE |
                                                WS_OVERLAPPED | WS_CAPTION |
                                                WS_THICKFRAME, rectDefault,
                                                this ) )
                    {
                        MessageBox( _("Create BUSMASTER Database Window Failed!"),
                                    NULL, MB_OK|MB_ICONERROR );
                        return;
                    }

                    m_podMsgSgWnd->ShowWindow( SW_SHOWMAXIMIZED );
                    m_podMsgSgWnd->UpdateWindow();

                    // Set the flag to indicate the opening of database window
                    CFlags* pFlags = theApp.pouGetFlagsPtr();
                    pFlags->vSetFlagStatus( DBOPEN, TRUE );
                    // Enable 'Save' & 'Save Import' menu item
                    CMenu* pMenu = GetMenu();
                    if (pMenu != NULL)
                    {
                        HMENU hMenu = pMenu->GetSafeHmenu();
                        if (hMenu != NULL)
                        {
                            EnableMenuItem(hMenu, IDM_SAVE_IMPORT, MF_ENABLED);
                            EnableMenuItem(hMenu, IDM_CONFIGURE_DATABASE_SAVE, MF_ENABLED);
                        }
                    }
                }
                else
                {
                    AfxMessageBox(_(MSG_MEMORY_CONSTRAINT),
                                  MB_OK | MB_ICONINFORMATION);
                }
            }
        }
    }
}
/******************************************************************************
FUNCTION:       pomGetMsgSgWnd
DESCRIPTION:    Opens a database file for editing
PARAMETERS:
RETURN VALUE:   Pointer to "CMsgSignalDBWnd" class
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
*******************************************************************************/

CMsgSignalDBWnd* CMainFrame::pomGetMsgSgWnd()
{
    return ( m_podMsgSgWnd );
}


/******************************************************************************
FUNCTION:       pomGetSimSysWnd
DESCRIPTION:    Opens the simulated system window for editing
PARAMETERS:
RETURN VALUE:   Pointer to "CSimSysWnd" class
AUTHOR:         Harika M
HISTORY:

*******************************************************************************/

//CSimSysWnd* CMainFrame::pomGetSimSysWnd()
//{
//    return ( m_pomSimSysWnd );
//}

/******************************************************************************
FUNCTION:       m_pomSimSysConfig
DESCRIPTION:    Returns pointer to CSimSysConfigDetails class
PARAMETERS:
RETURN VALUE:   Pointer to "CSimSysWnd" class
AUTHOR:         Harika M
HISTORY:

*******************************************************************************/
//CSimSysConfigDetails* CMainFrame::pomGetSimSysConfig()
//{
//  return (m_pomSimSysConfig );
//}


/******************************************************************************
FUNCTION:       OnCloseDatabase
DESCRIPTION:    Closes a database file
PARAMETERS:
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
25.03.2002      Added flag closing  Amarnath Shastry
                status
Modification    Raja N
                10/03/2004 Modified Active/Passive database concept and the
                database editor will use inactive data structure always for
                editing purpose
*******************************************************************************/
void CMainFrame::OnCloseDatabase()
{
    if ( m_podMsgSgWnd != NULL )
    {
        // Get appropriate data structure
        CMsgSignal* pTempMsgSg = NULL;
        // Get the pointer to the editor database structure
        pTempMsgSg = theApp.m_pouMsgSgInactive;

        if ( pTempMsgSg != NULL && !pTempMsgSg->bGetModifiedFlag())
        {
            UINT bRetVal =
                AfxMessageBox(_("Do you want to save changes to the database file?"),
                              MB_YESNO | MB_ICONQUESTION);
            if ( bRetVal == IDYES )
            {
                BeginWaitCursor();
                pTempMsgSg->
                bWriteIntoDatabaseFileFromDataStructure(
                    m_omStrDatabaseName, PROTOCOL_CAN);
                if (pTempMsgSg->bGetDBAcitveFlagStatus() == TRUE)
                {
                    vPostMsgToSendMsgDlg(CAN);
                }
                EndWaitCursor();
            }
        }
        // User doesn't want to save changes nothing to process

        // inactive data structure is no more required
        // delete previously allocated memory if any
        pTempMsgSg->bDeAllocateMemoryInactive();
        m_podMsgSgWnd->MDIDestroy();
        m_podMsgSgWnd = NULL;
        m_bIsNewDatabase = FALSE;

        // Reset the flag to indicate the closing of database window
        CFlags* pFlags = theApp.pouGetFlagsPtr();
        pFlags->vSetFlagStatus( DBOPEN, FALSE );

    }
}
/******************************************************************************
FUNCTION:       podGetMsgSgDetView
DESCRIPTION:    Returns pointer to CMsgSgDetView class
PARAMETERS:
RETURN VALUE:   Pointer to "CMsgSgDetView" class
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
*******************************************************************************/
CMsgSgDetView* CMainFrame::podGetMsgSgDetView(ETYPE_BUS eBus)
{
    return m_pomMsgSgDetViews[eBus];
}
/******************************************************************************
FUNCTION:       podGetMsgSgTreeView
DESCRIPTION:    Returns pointer to CMsgSgTreeView class
PARAMETERS:
RETURN VALUE:   Pointer to "CMsgSgTreeView" class
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
*******************************************************************************/
CMsgSgTreeView* CMainFrame::podGetMsgSgTreeView(ETYPE_BUS eBus)
{
    return(m_pomMsgSgTreeViews[eBus]);
}
/******************************************************************************
FUNCTION:       podSetMsgSgTreeView
DESCRIPTION:    Sets pointer to CMsgSgTreeView class
PARAMETERS:     Pointer to "CMsgSgTreeView" class
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
*******************************************************************************/
void CMainFrame::podSetMsgSgTreeView(CMsgSgTreeView* pTreeView, ETYPE_BUS eBus)
{
    m_pomMsgSgTreeViews[eBus] = pTreeView;
}
/******************************************************************************
FUNCTION:       podSetMsgSgDetView
DESCRIPTION:    Sets pointer to CMsgSgDetView class
PARAMETERS:     Pointer to "CMsgSgDetView" class
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
*******************************************************************************/
void CMainFrame::podSetMsgSgDetView(CMsgSgDetView* pDetView, ETYPE_BUS eBus)
{
    m_pomMsgSgDetViews[eBus] = pDetView;
}


/******************************************************************************
FUNCTION:       podGetSimSysDetView
DESCRIPTION:    Returns pointer to CMsgSgDetView class
PARAMETERS:
RETURN VALUE:   Pointer to "CMsgSgDetView" class
AUTHOR:         Harika M
HISTORY:

*******************************************************************************/
//CSimSysDetView* CMainFrame::podGetSimSysDetView()
//{
//    return(m_pomSimSysDetView);
//}
/******************************************************************************
FUNCTION:       podGetSimSysTreeView
DESCRIPTION:    Returns pointer to CMsgSgTreeView class
PARAMETERS:
RETURN VALUE:   Pointer to "CMsgSgTreeView" class
AUTHOR:         Harika M
HISTORY:

*******************************************************************************/
//CSimSysTreeView* CMainFrame::podGetSimSysTreeView()
//{
//    return(m_pomSimSysTreeView);
//}
/******************************************************************************
FUNCTION:       podSetSimSysTreeView
DESCRIPTION:    Sets pointer to CMsgSgTreeView class
PARAMETERS:     Pointer to "CSimSysTreeView" class
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
*******************************************************************************/
//void CMainFrame::podSetSimSysTreeView(CSimSysTreeView *pTreeView)
//{
//    m_pomSimSysTreeView = pTreeView;
//}
/******************************************************************************
FUNCTION:       podSetSimSysDetView
DESCRIPTION:    Sets pointer to CMsgSgDetView class
PARAMETERS:     Pointer to "CSimSysDetView" class
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
*******************************************************************************/
//void CMainFrame::podSetSimSysDetView(CSimSysDetView *pDetView)
//{
//    m_pomSimSysDetView = pDetView;
//}

//void CMainFrame::pomSetSimSysConfig(CSimSysConfigDetails *pSimsysConfig)
//{
//  m_pomSimSysConfig = pSimsysConfig;
//}

/******************************************************************************
FUNCTION:       OnImportDatabase
DESCRIPTION:    Loads a database file selected by the user
PARAMETERS:
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
12-11-2002      Modifications       Gopi, Changed usage to configuration from
                                    registry
Modifications:  Raja N
                10/03/2004 Included checks for clearing signal watch list and
                to post warnning message if DLL is loaded
Modifications:  Raja N
                12/03/2004 Included checks to prevent import if signal watch
                list has some elements. If the user still wants to import then
                go ahead import and clear the list
Modifications:  Raja N
                05/04/2004 Modified to refer latest signal watch list. Add added
                check to compare the importing database with active database. If
                both are same Signal Watch list delete option will be given to
                the user. If both are different signal watch list will be
                cleared with out any option.
Modifications:  Raja N
                18.05.2004 Added check before popping the import warning for
                Signal Watch
Modifications:  Raja N
                10.08.2004 Added code to delete Signal Watch list incase of
                different database got imported
Modifications:  Raja N
                10.08.2004 Added code to delete graph list incase of different
                database got imported
Modifications:  Raja N
                26.04.2005 Modified code to refer Tx Msg Window to send database
                import notification.
Modifications:  Anish
                07.06.2006 Modified code to implement COM interface
Modifications:  Anish
                07.12.2006 Modified code to for Multiple database
*******************************************************************************/
void CMainFrame::OnImportDatabase()
{
    CStringArray strFilePathArray;
    // Display a open file dialog
    CFileDialog fileDlg( TRUE,      // Open File dialog
                         "dbf",     // Default Extension,
                         NULL,// Initial database file name.
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,
                         _("BUSMASTER Datatbase File(*.dbf)|*.dbf||"),
                         NULL );

    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _T(_("Select Active Database Filename..."));

    if ( IDOK == fileDlg.DoModal() )
    {
        POSITION pos = NULL;
        pos = fileDlg.GetStartPosition();
        while(NULL != pos)
        {
            CString strTempFile = fileDlg.GetNextPathName(pos);
            strFilePathArray.Add(strTempFile);
        }
        CString omStrMsg = _("Database File: \n ");
        BOOL bAllFilesImported = TRUE;
        int nFileCount = strFilePathArray.GetSize();
        for(int nCount = 0; nCount < nFileCount; nCount++)
        {
            CString strTempFileName = strFilePathArray.GetAt(nCount);
            //FALSE because it is not called using COM
            DWORD dError=dLoadDataBaseFile(strTempFileName,FALSE);
            if(E_INVALIDARG==dError)
            {
                bAllFilesImported = FALSE;
                //Add the file name for warning display
                omStrMsg += strTempFileName;
                omStrMsg += defNEW_LINE;
                strFilePathArray.RemoveAt(nCount);
            }
        }
        if(bAllFilesImported == FALSE)
        {
            omStrMsg += _(" not found!");
            MessageBox(omStrMsg,_("BUSMASTER"),MB_OK|MB_ICONERROR);
        }

        // Check for Warnning condition
        CFlags* pFlags = NULL;
        pFlags = theApp.pouGetFlagsPtr();
        if( pFlags != NULL)
        {
            if(pFlags->nGetFlagStatus(DLLLOADED) == TRUE)
            {
                AfxMessageBox(_(defIMPORT_DLLLOAD_WARNNING),
                              MB_OK|MB_ICONINFORMATION);//dll loaded unload it
            }
        }

        HWND hWnd;
        hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);

        if(hWnd)
        {
            ::SendMessage(hWnd, WM_DATABASE_CHANGE, (WPARAM)TRUE, NULL);
        }

        //Added by Arun to update Data Handler Main entry list.
        vUpdateMainEntryListInWaveDataHandler();
    }
}

DWORD CMainFrame::dLoadJ1939DBFile(CString omStrActiveDataBase,BOOL bFrmCom)
{
    DWORD dReturn= (DWORD)E_FAIL;
    if (m_pouMsgSigJ1939 == NULL)
    {
        sg_asDbParams[J1939].m_ppvActiveDB = (void**)&m_pouActiveDbJ1939;
        sg_asDbParams[J1939].m_ppvImportedDBs = (void**)&m_pouMsgSigJ1939;
        m_pouMsgSigJ1939 = new CMsgSignal(sg_asDbParams[J1939], bFrmCom);
    }
    if (m_pouMsgSigJ1939 != NULL)
    {

        //MVN::Construct File Path
        if (TRUE == PathIsRelative(omStrActiveDataBase))
        {
            string omStrBasePath;
            CString omConfigFileName;
            vGetLoadedCfgFileName(omConfigFileName);
            CUtilFunctions::nGetBaseFolder(omConfigFileName.GetBuffer(MAX_PATH), omStrBasePath);
            char chAbsPath[MAX_PATH];
            PathCombine(chAbsPath, omStrBasePath.c_str(), omStrActiveDataBase.GetBuffer(MAX_PATH));
            omStrActiveDataBase = chAbsPath;
        }
        //~MVN::Construct File Path

        //Check for same DB path......
        CStringArray aomOldDatabases;
        m_pouMsgSigJ1939->vGetDataBaseNames(&aomOldDatabases);
        int nFileCount = aomOldDatabases.GetSize();
        BOOL bFilePresent = FALSE;
        for(int nCount = 0; (nCount < nFileCount)&&(!bFilePresent); nCount++)
        {
            CString omstrFileName = aomOldDatabases.GetAt(nCount);
            if( !omstrFileName.Compare(omStrActiveDataBase) )
            {
                bFilePresent = TRUE;
            }
        }
        if(bFilePresent == TRUE)
        {
            //if file is already present remove it and then load the file again
            m_pouMsgSigJ1939->bDeAllocateMemory(omStrActiveDataBase);
        }
        // file-attribute information
        struct _finddata_t fileinfo;

        // Auto Select DB File
        BOOL bRetVal;
        if (_findfirst( omStrActiveDataBase, &fileinfo)!= -1L)
        {
            // Fill data struct with new data base info
            bRetVal = m_pouMsgSigJ1939->bFillDataStructureFromDatabaseFile(
                          omStrActiveDataBase, PROTOCOL_J1939);
            if((bRetVal != FALSE)&&(bFilePresent != TRUE))
            {
                m_pouMsgSigJ1939->bAddDbNameEntry(omStrActiveDataBase);
            }
            //Update in NodeSimEx
            GetIJ1939NodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[J1939].m_omDefinedMsgHeaders), UPDATE_UNIONS_HEADER_FILES);
            //Update Msg Name<-->Msg Code List
            vUpdateMsgNameCodeList(m_pouMsgSigJ1939, m_sExFuncPtr[J1939].m_odMsgNameMsgCodeListDB);
            //Update in NodeSimEx
            GetIJ1939NodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[J1939].m_odMsgNameMsgCodeListDB), UPDATE_DATABASE_MSGS);

            dReturn = S_OK;
            // Create Unions.h in local directory
            // and fill the file with the latest data structure

            m_pouMsgSigJ1939->bWriteDBHeader(omStrActiveDataBase);
        }

        vPopulateJ1939PGNList();
        /* Update the Message window with latest J1939 DB details */
        HWND hWnd;
        hWnd = m_podMsgWndThread->hGetHandleMsgWnd(J1939);

        /* Set the J1939 DB pointer in MsgFrmtWnd class */
        if(hWnd)
        {
            ::SendMessage(hWnd, WM_NOTIFICATION_FROM_OTHER,
                          eLOAD_DATABASE,
                          (LPARAM)&(m_pouMsgSigJ1939));
            //::SendMessage(hWnd, WM_DATABASE_CHANGE, (WPARAM)TRUE, NULL);
        }

        /* Update J1939 Tx window */
        vPostMsgToSendMsgDlg(J1939);
    }
    return dReturn;
}
/******************************************************************************
FUNCTION:       bLoadDataBaseFile
DESCRIPTION:    Loads a database file selected by the user
PARAMETERS:     file name ,bFrmCom
RETURN VALUE:   bool
AUTHOR:         Anish kumar
Modifications:  Anish
                07.12.2006 Modified code to for Multiple database
*******************************************************************************/
DWORD CMainFrame::dLoadDataBaseFile(CString omStrActiveDataBase,BOOL /*bFrmCom*/)
{
    DWORD dReturn= (DWORD)E_FAIL;
    //Check for same DB path......
    CStringArray aomOldDatabases;
    theApp.m_pouMsgSignal->vGetDataBaseNames(&aomOldDatabases);
    int nFileCount = aomOldDatabases.GetSize();
    BOOL bFilePresent = FALSE;
    //MVN::Construct File Path
    if (TRUE == PathIsRelative(omStrActiveDataBase))
    {
        string omStrBasePath;
        CString omConfigFileName;
        vGetLoadedCfgFileName(omConfigFileName);
        CUtilFunctions::nGetBaseFolder(omConfigFileName.GetBuffer(MAX_PATH), omStrBasePath);
        char chAbsPath[MAX_PATH];
        PathCombine(chAbsPath, omStrBasePath.c_str(), omStrActiveDataBase.GetBuffer(MAX_PATH));
        omStrActiveDataBase = chAbsPath;
    }
    //~MVN::Construct File Path
    for(int nCount = 0; (nCount < nFileCount)&&(!bFilePresent); nCount++)
    {
        CString omstrFileName = aomOldDatabases.GetAt(nCount);
        if( !omstrFileName.Compare(omStrActiveDataBase) )
        {
            bFilePresent = TRUE;
        }
    }
    if(bFilePresent == TRUE)
    {
        //if file is already present remove it and then load the file again
        theApp.m_pouMsgSignal->bDeAllocateMemory(omStrActiveDataBase);
    }
    // file-attribute information
    struct _finddata_t fileinfo;

    // Auto Select DB File
    if (_findfirst( omStrActiveDataBase, &fileinfo)!= -1L)
    {
        BOOL bRetVal = FALSE;
        if ( theApp.m_pouMsgSignal !=  NULL )
        {
            // Fill data struct with new data base info
            bRetVal = theApp.m_pouMsgSignal->bFillDataStructureFromDatabaseFile(
                          omStrActiveDataBase, PROTOCOL_CAN);
            // If dbf file loaded successfully
            if(TRUE == bRetVal)
            {
                // Create Unions.h in local directory
                // and fill the file with the latest data structure
                theApp.m_pouMsgSignal->bWriteDBHeader(omStrActiveDataBase);
                //Update Defined Msg Headers
                m_sExFuncPtr[CAN].m_omDefinedMsgHeaders.RemoveAll();
                CStringArray omDatabaseNames;
                theApp.m_pouMsgSignal->vGetDataBaseNames(&omDatabaseNames);
                for (int i = 0; i < omDatabaseNames.GetSize(); i++)
                {
                    CString omHeaderPath = omStrGetUnionFilePath(omDatabaseNames.GetAt(i));
                    m_sExFuncPtr[CAN].m_omDefinedMsgHeaders.Add(omHeaderPath);
                }
                //Update in NodeSimEx
                GetICANNodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[CAN].m_omDefinedMsgHeaders), UPDATE_UNIONS_HEADER_FILES);
                //Update Msg Name<-->Msg Code List
                vUpdateMsgNameCodeList(theApp.m_pouMsgSignal, m_sExFuncPtr[CAN].m_odMsgNameMsgCodeListDB);
                //Update in NodeSimEx
                GetICANNodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[CAN].m_odMsgNameMsgCodeListDB), UPDATE_DATABASE_MSGS);

                // User can open the active DB
                theApp.pouGetFlagsPtr()->vSetFlagStatus(SELECTDATABASEFILE, TRUE );
                //venkat update signal watch
                vUpdateSWList();
            }

        }
        theApp.vPopulateCANIDList();
        // Send a message to Tx Window about database change
        if(m_objTxHandler.hConfigWindowShown() == S_OK)
        {
            eUSERSELCTION eUserSel = eDATABASEIMPORTCMD;
            m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel,0);
        }
        // Delete all the memory associated with the data structure.
        CMessageAttrib::ouGetHandle(CAN).vSaveMessageAttribData();
        ////Clear the message Window Content
        //      if ( !bFrmCom )
        //      {
        //    //Clear the message Window Content
        //    OnClearMsgWindow();
        //    // Clear Message Interpretation Window Content
        //      }
        //if the file is not present then add its name to file list
        if((bRetVal != FALSE)&&(bFilePresent != TRUE))
        {
            //If the DATABASE is added add the file name to theApp class
            theApp.m_pouMsgSignal->bAddDbNameEntry(omStrActiveDataBase);
        }


        dReturn= S_OK;
    }
    else
    {
        dReturn = (DWORD)E_INVALIDARG; //file not found
    }
    return dReturn;
}
/******************************************************************************
FUNCTION:       nConvertStringToInt
DESCRIPTION:    Closes a database file
PARAMETERS:     CString omStrHexNo -- Hex number
RETURN VALUE:   Equivalent decimal number
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
02.06.2003      Changed the int to __int64 Amitesh Bharti
12.06.2003      review comment for CRH0005 incorporated. Amitesh Bharti
*******************************************************************************/
__int64 CMainFrame::nConvertStringToInt(CString omStrHexNo)
{
    __int64 n64RetVal = 0;

    BOOL b_IsNegative = FALSE;

    for (int nCount = 0; nCount < omStrHexNo.GetLength(); nCount++)
    {
        char cChar = omStrHexNo.GetAt( nCount);

        if ( cChar == '0' )
        {
            n64RetVal = n64RetVal * 16 + 0;
        }
        else if ( cChar == '1' )
        {
            n64RetVal = n64RetVal * 16 + 1;
        }
        else if ( cChar == '2' )
        {
            n64RetVal = n64RetVal * 16 + 2;
        }
        else if ( cChar == '3' )
        {
            n64RetVal = n64RetVal * 16 + 3;
        }
        else if ( cChar == '4' )
        {
            n64RetVal = n64RetVal * 16 + 4;
        }
        else if ( cChar == '5' )
        {
            n64RetVal = n64RetVal * 16 + 5;
        }
        else if ( cChar == '6' )
        {
            n64RetVal = n64RetVal * 16 + 6;
        }
        else if ( cChar == '7' )
        {
            n64RetVal = n64RetVal * 16 + 7;
        }
        else if ( cChar == '8' )
        {
            n64RetVal = n64RetVal * 16 + 8;
        }
        else if ( cChar == '9' )
        {
            n64RetVal = n64RetVal * 16 + 9;
        }
        else if ( cChar == 'A' || cChar == 'a')
        {
            n64RetVal = n64RetVal * 16 + 10;
        }
        else if ( cChar == 'B' || cChar == 'b')
        {
            n64RetVal = n64RetVal * 16 + 11;
        }
        else if ( cChar == 'C' || cChar == 'c')
        {
            n64RetVal = n64RetVal * 16 + 12;
        }
        else if ( cChar == 'D' || cChar == 'd')
        {
            n64RetVal = n64RetVal * 16 + 13;
        }
        else if ( cChar == 'E' || cChar == 'e')
        {
            n64RetVal = n64RetVal * 16 + 14;
        }
        else if ( cChar == 'F' || cChar == 'f')
        {
            n64RetVal = n64RetVal * 16 + 15;
        }
        else if ( cChar == '-' )
        {
            b_IsNegative = TRUE;
        }
    }
    if ( b_IsNegative )
    {
        n64RetVal = -n64RetVal;
    }
    return n64RetVal;
}
/******************************************************************************/
/*  Function Name    :  OnConfigBaudrate                                      */
/*  Input(s)         :  BTR0 and BTR1                                         */
/*  Output           :  baudrate                                              */
/*  Functionality    :  This function is called to invoke the CChangeRegisters*/
/*                   dialog box. On selection of menu, this function is called*/
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  22.02.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CMainFrame::OnConfigBaudrate()
{
    PCHAR pInitData = (PCHAR)m_asControllerDetails;
    int nSize = sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS;
    if (g_pouDIL_CAN_Interface->DILC_DisplayConfigDlg(m_asControllerDetails, nSize) == S_OK)
    {
        //Set Controller to ConfigDetails
        //memcpy(m_asControllerDetails, pInitData, nSize);
    }
    //Update hardware info in status bar
    vUpdateHWStatusInfo();

    //update baudrate details in global statistics buffer
    for (int i = 0; i < defNO_OF_CHANNELS; i++)
    {
#ifdef BOA_FD_VERSION
        GetICANBusStat()->BSC_SetBaudRate(i, m_asControllerDetails[i].m_unDataBitRate);
#else
        GetICANBusStat()->BSC_SetBaudRate(i, _tstof(m_asControllerDetails[i].m_omStrBaudrate.c_str()));
#endif
    }

}

void CMainFrame::OnConfigLinChannel()
{
    PCHAR pInitData = (PCHAR)m_asControllerDetailsLIN;
    int nSize = sizeof(SCONTROLLER_DETAILS_LIN) * defNO_OF_LIN_CHANNELS;
    //   if (g_pouDIL_LIN_Interface->DILL_DisplayConfigDlg(m_asControllerDetailsLIN, nSize) == S_OK)
    {
        //    //Set Controller to ConfigDetails
        //    //memcpy(m_asControllerDetails, pInitData, nSize);
    }
    //Update hardware info in status bar
    vUpdateHWStatusInfo();
}

/******************************************************************************/
/*  Function Name    :  OnConfigBaudrate                                      */
/*  Input(s)         :  BTR0 and BTR1                                         */
/*  Output           :  baudrate                                              */
/*  Functionality    :  This function is called to invoke the CChangeRegisters*/
/*                   dialog box. On selection of menu, this function is called*/
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  22.02.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/

/**
* \brief         This function pops out the hardware selection dialog
* \param         void
* \return        void
* \authors       Arunkumar Karri
* \date          13.12.2011 Created
*/
void CMainFrame::OnConfigChannelSelection()
{
    INT nCount = CHANNEL_ALLOWED;
    HRESULT hResult = S_FALSE;

    /* Deselect hardware interfaces if selected */
    hResult = g_pouDIL_CAN_Interface->DILC_DeselectHwInterfaces();

    if (g_pouDIL_CAN_Interface->DILC_ListHwInterfaces(m_asINTERFACE_HW, nCount) == S_OK)
    {
        hResult = g_pouDIL_CAN_Interface->DILC_SelectHwInterfaces(m_asINTERFACE_HW, nCount);
        if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
        {
            /* Updates the number of channels selected */
            m_nNumChannels = nCount;

            //Update hardware info in status bar
            vUpdateHWStatusInfo();

            //Update NW statistics window channel information
            vUpdateChannelInfo();

            // Update controller information
            g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails, nCount);
        }
    }
    else
    {
        /* Select previously available channels */
        g_pouDIL_CAN_Interface->DILC_SelectHwInterfaces(m_asINTERFACE_HW, nCount);
    }
}

void CMainFrame::OnUpdateConfigChannelSelection(CCmdUI* pCmdUI)
{
    if(pCmdUI != NULL)
    {
        CFlags* podFlag = theApp.pouGetFlagsPtr();
        if( podFlag != NULL )
        {
            // Disable if it is connected
            BOOL bDisable = podFlag->nGetFlagStatus(CONNECTED);
            // In Simulation mode is selected then disable Controller option
            LONG lParam= 0;
            if( g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, HW_MODE) == S_OK)
            {
                if (lParam == defMODE_SIMULATE)
                {
                    bDisable = TRUE;
                }
            }
            // If bDisable = TRUE then set the Enable to FALSE
            pCmdUI->Enable(!bDisable);
        }
    }
}


/******************************************************************************
FUNCTION:       OnNewDatabase
DESCRIPTION:    #Called by the framework when user selects new database
                option from the menu
                #Creates a new database file name and displays
                database editior
PARAMETERS:
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
29.05.2002      Get the current working directory and add it
                with file name      Amitesh Bharti
10.03.2004      Removed Active database flag set call - Raja N
*******************************************************************************/
void CMainFrame::OnNewDatabase()
{
    UINT unCount = 1;
    BOOL bResult = FALSE;
    while (TRUE)
    {
        CString omStrDbName = NEW_DATABASE_NAME;

        CString omStr = STR_EMPTY;

        omStr.Format( "%d", unCount);

        omStr += ".";

        omStr += DATABASE_EXTN;

        omStrDbName += omStr;

        // file-attribute information
        struct _finddata_t fileinfo;

        // Auto Select DB File
        if (_findfirst( omStrDbName, &fileinfo)== -1L)
        {
            CStdioFile om_File;

            TRY
            {
                CFileDialog fileDlg(FALSE, DATABASE_EXTN, omStrDbName.GetBuffer(MAX_PATH),
                OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, DATABASE_FILTER);
                fileDlg.m_ofn.lpstrTitle  = _T(_("New Database file name..."));
                if (fileDlg.DoModal() == IDOK)
                {
                    m_omStrDatabaseName = fileDlg.GetPathName();
                    bResult = TRUE;

                    // create the selected file
                    if (om_File.Open(m_omStrDatabaseName,
                    CFile::modeCreate | CFile::modeRead |
                    CFile::typeText))
                    {
                        om_File.Close();
                    }
                }
            }
            CATCH_ALL(om_Fe)
            {
                if(om_Fe != NULL )
                {
                    LPTSTR lpszError = NULL;
                    // Get error
                    om_Fe->GetErrorMessage( lpszError, 255);

                    MessageBox( lpszError, NULL, MB_OK );

                    om_Fe->Delete();
                }
            }
            END_CATCH_ALL
            //// Get the current working directory and add it with file name
            //char acBeffer[512] = {NULL};
            //_getcwd(acBeffer,sizeof(acBeffer));
            //m_omStrDatabaseName  = acBeffer;
            //m_omStrDatabaseName +="\\";
            //m_omStrDatabaseName += omStrDbName;



            break;
        }

        unCount++;
    }
    if (bResult == TRUE)
    {

        if ( m_podMsgSgWnd != NULL )
        {
            m_podMsgSgWnd = NULL;
        }
        sg_asDbParams[CAN].m_ppvActiveDB = (void**)&theApp.m_pouMsgSgInactive;
        sg_asDbParams[CAN].m_ppvImportedDBs = (void**)&theApp.m_pouMsgSignal;
        m_podMsgSgWnd = new CMsgSignalDBWnd(sg_asDbParams[CAN]);

        if ( m_podMsgSgWnd != NULL )
        {
            sg_asDbParams[CAN].m_omDBPath = m_omStrDatabaseName;
            m_podMsgSgWnd->vSetDBName(m_omStrDatabaseName);
            m_bIsNewDatabase = TRUE;

            // Create child window
            if ( !m_podMsgSgWnd->Create( NULL,
                                         _("Database Editor"),
                                         WS_CHILD|WS_VISIBLE |WS_OVERLAPPED|
                                         WS_CAPTION|WS_THICKFRAME,
                                         rectDefault, this ) )
            {
                MessageBox( _("Create BUSMASTER Database Window Failed!"),
                            NULL, MB_OK|MB_ICONERROR );
                return;
            }

            m_podMsgSgWnd->ShowWindow( SW_SHOWMAXIMIZED );

            m_podMsgSgWnd->UpdateWindow();
            // Set the flag to indicate the opening of database window
            CFlags* pFlags = theApp.pouGetFlagsPtr();
            pFlags->vSetFlagStatus( DBOPEN, TRUE );
        }
        else
        {
            AfxMessageBox( _(MSG_MEMORY_CONSTRAINT), MB_OK|MB_ICONINFORMATION);
        }
    }
}
/******************************************************************************
FUNCTION:       OnConfigDatabaseSaveAs
DESCRIPTION:    #Called by the framework when user selects SaveAs...
                option from the menu for the database editor
                #Dispalys Save file dialog
                #Saves the datastructure contents into the selected file
PARAMETERS:
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
29.05.2002      Added code to change the font after saving the file -- Amitesh
Modification    Raja N
                10/03/2004 Modified Active/Passive database concept and the
                database editor will use inactive data structure always for
                editing purpose
*******************************************************************************/
void CMainFrame::OnConfigDatabaseSaveAs()
{
    // Display a save file dialog
    CFileDialog fileDlg( FALSE,     // Save File dialog
                         "dbf",     // Default Extension,
                         "NewDB.dbf",
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         _("CAN Datatbase File(*.dbf)|*.dbf||"),
                         NULL );

    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _T(_("Save As..."));

    if ( IDOK == fileDlg.DoModal() )
    {
        CString strExtName  = fileDlg.GetFileExt();
        CString strDbName   = fileDlg.GetPathName();

        if ( strDbName.Find('.') )
        {
            strDbName = strDbName.Left( strDbName.Find('.') + 1);
            strDbName.TrimRight();
            strDbName += strExtName;
        }

        CMsgSignal* pTempMsgSg = NULL;
        // Get the pointer to the editor database data structure
        pTempMsgSg = theApp.m_pouMsgSgInactive;

        if ( pTempMsgSg != NULL )
        {
            BeginWaitCursor( );
            // save into the file
            pTempMsgSg->bWriteIntoDatabaseFileFromDataStructure( strDbName, PROTOCOL_CAN );
            if (pTempMsgSg->bGetDBAcitveFlagStatus() == TRUE)
            {
                vPostMsgToSendMsgDlg(CAN);
            }
            EndWaitCursor( );
            // If this is new database, remove the
            // default file name already saved
            if ( m_bIsNewDatabase == TRUE )
            {
                CFile::Remove( m_omStrDatabaseName );
            }
            // Update the selected file name with full path
            m_omStrDatabaseName = strDbName;
            // Set the New Flag to False
            vSetNewDatabaseFlag(FALSE);
            // Now set the root item in the tree view to
            // selected name
            if ( m_pomMsgSgTreeViews[CAN] != NULL)
            {
                m_pomMsgSgTreeViews[CAN]->vSetRootItemText(m_omStrDatabaseName);
            }

            // Set all the items in the tree view to normal font
            if ( m_pomMsgSgTreeViews[CAN] != NULL)
            {
                m_pomMsgSgTreeViews[CAN]->vSetAllItemsNormal();
            }
        }
    }
}

/**
* \brief         Called by the framework when user selects SaveAs...
                 option from the menu for the database editor
* \param[in]     NULL
* \return        void
* \authors       Arunkumar Karri
* \date          12.06.2013 Created
*/
void CMainFrame::OnJ1939DBSaveAs()
{
    // Display a save file dialog
    CFileDialog fileDlg( FALSE,     // Save File dialog
                         "dbf",     // Default Extension,
                         "NewDB.dbf",
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         _("CAN Datatbase File(*.dbf)|*.dbf||"),
                         NULL );

    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _T(_("Save As..."));

    if ( IDOK == fileDlg.DoModal() )
    {
        CString strExtName  = fileDlg.GetFileExt();
        CString strDbName   = fileDlg.GetPathName();

        if ( strDbName.Find('.') )
        {
            strDbName = strDbName.Left( strDbName.Find('.') + 1);
            strDbName.TrimRight();
            strDbName += strExtName;
        }

        CMsgSignal* pTempMsgSg = NULL;
        // Get the pointer to the editor database data structure
        pTempMsgSg = (CMsgSignal*)*m_podMsgSgWndJ1939->m_sDbParams.m_ppvActiveDB;

        if ( pTempMsgSg != NULL )
        {
            BeginWaitCursor( );
            // save into the file
            pTempMsgSg->bWriteIntoDatabaseFileFromDataStructure( strDbName, PROTOCOL_J1939 );
            if (pTempMsgSg->bGetDBAcitveFlagStatus() == TRUE)
            {
                vPostMsgToSendMsgDlg(J1939);
            }
            EndWaitCursor( );
            // Now set the root item in the tree view to
            // selected name
            if ( m_pomMsgSgTreeViews[J1939] != NULL)
            {
                m_pomMsgSgTreeViews[J1939]->vSetRootItemText(strDbName);
            }

            // Set all the items in the tree view to normal font
            if ( m_pomMsgSgTreeViews[J1939] != NULL)
            {
                m_pomMsgSgTreeViews[J1939]->vSetAllItemsNormal();
            }
        }
    }
}

/******************************************************************************
FUNCTION:       OnConfigDatabaseSave
DESCRIPTION:    #Called by the framework when user selects Save...
                option from the menu for the database editor
                #Saves the datastructure contents into the selected file
PARAMETERS:
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
HISTORY:
20-02-2002      Initial Version     Amarnath Shastry
Modification    Raja N
                10/03/2004 Modified Active/Passive database concept and the
                database editor will use inactive data structure always for
                editing purpose
*******************************************************************************/
void CMainFrame::OnConfigDatabaseSave()
{

    // Get appropriate message data structure
    CMsgSignal* pTempMsgSg = NULL;
    // Get the data structure pointer
    pTempMsgSg = theApp.m_pouMsgSgInactive;

    // Save into the database
    if ( pTempMsgSg != NULL )
    {
        CMsgSgDetView* podMsgSg = podGetMsgSgDetView(CAN);

        // Special case:
        // If the user edits message details and
        // directly clicks the "save" tool bar button
        // or menu options, save the edited values to
        // the data structure first
        if (podMsgSg != NULL)
        {
            if (podMsgSg->bUpdateEditedMesageDetails())
            {
                if ( !pTempMsgSg->bGetModifiedFlag())
                {
                    BeginWaitCursor( );
                    // save into the file
                    pTempMsgSg->
                    bWriteIntoDatabaseFileFromDataStructure(
                        m_omStrDatabaseName, PROTOCOL_CAN);
                    if (pTempMsgSg->bGetDBAcitveFlagStatus() == TRUE)
                    {
                        vPostMsgToSendMsgDlg(CAN);
                    }
                    // Check if the modified file is being loaded or not.
                    //If yes then prompt the user whether he wants to
                    //import it or not.
                    CStringArray omImportedDBNames;
                    if (theApp.m_pouMsgSignal != NULL)
                    {
                        theApp.m_pouMsgSignal->vGetDataBaseNames(&omImportedDBNames);
                        for (INT nDBCount = 0; nDBCount < omImportedDBNames.GetSize();
                                nDBCount++)
                        {
                            if (m_omStrDatabaseName ==
                                    omImportedDBNames.GetAt(nDBCount))
                            {
                                CString omText;
                                omText.Format( _T(_("File  \"%s\"  has been modified which is currently being loaded.\nDo you want to re-import it to reflect the changes?")),
                                               m_omStrDatabaseName);

                                if (MessageBox(omText, "BUSMASTER", MB_ICONQUESTION | MB_YESNO) == IDYES)
                                {
                                    dLoadDataBaseFile(m_omStrDatabaseName, FALSE);
                                }
                            }
                        }
                    }
                    //Checking ends
                    m_bIsNewDatabase = FALSE;

                    EndWaitCursor( );
                    // Set all the items in the tree view to normal font
                    if ( m_pomMsgSgTreeViews[CAN] != NULL)
                    {
                        m_pomMsgSgTreeViews[CAN]->vSetAllItemsNormal();
                    }

                    pTempMsgSg->vSetModifiedFlag(TRUE);
                }
            }
        }
    }
}
/******************************************************************************
FUNCTION:       OnConfigMessageDisplay
DESCRIPTION:    #Called by the framework when user selects Configure Message...
                option from the menu.
                #Displays message attribute dialog
PARAMETERS:
RETURN VALUE:
AUTHOR:         RBIN/EMC2 - Ratnadip Choudhury
HISTORY:
20-02-2002      Initial Version     Ratnadip Choudhury
04-11-2003      Change to have property page for database and non database
                messages.
22.07.2004      Raja N, Added a new page for message buffer configuration
31.07.2004      Raja N, Modified the name as per code review comments
20.06.2005      Raja N, Modified to include Filter Page
*******************************************************************************/
void CMainFrame::OnConfigMessageDisplay()
{
    CPPageMessage odDBMsg(TRUE, CAN, theApp.m_pouMsgSignal), odNDBMsg(FALSE, CAN,  theApp.m_pouMsgSignal);
    CPropertySheet omAllMessages(_("Configure Message Display- CAN"));
    CMsgFilterConfigPage omFilter(&m_sFilterAppliedCAN, m_podMsgWndThread->hGetHandleMsgWnd(CAN));

    BOOL bConnected = FALSE;
    CFlags* pouFlag      = theApp.pouGetFlagsPtr();
    // Get the connection status
    if(pouFlag != NULL)
    {
        bConnected   = pouFlag->nGetFlagStatus(CONNECTED);
    }

    omAllMessages.m_psh.dwFlags |= PSH_NOAPPLYNOW;
    omAllMessages.m_psh.dwFlags &= ~PSH_HASHELP;
    // Add Filter Page
    omAllMessages.AddPage(&omFilter);
    // Add DB & NDB pages
    omAllMessages.AddPage(&odDBMsg);
    omAllMessages.AddPage(&odNDBMsg);
    // Show display configuration only if it is not connected
    if( bConnected == FALSE )
    {
        CMsgBufferConfigPage obMsgBuffConf;
        obMsgBuffConf.vSetBufferSize(m_anMsgBuffSize);
        omAllMessages.AddPage(&obMsgBuffConf);
        BOOL bReturnVal = omAllMessages.DoModal();

        if (m_podMsgWndThread != NULL)//Msg window
        {
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_GET_BUFFER_DETAILS, (LPARAM)m_anMsgBuffSize);
        }

        if(bReturnVal == IDOK)
        {
            // Check if filter for message display is enabled
            ApplyMessageFilterButton();
        }
    }
    else
    {
        BOOL bReturnVal = omAllMessages.DoModal();

        if(bReturnVal == IDOK)
        {
            // Check if filter for message display is enabled
            ApplyMessageFilterButton();
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnReplaySingleSkip                                    */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user select command skip    */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.04.2002                                            */
/*  Modifications    :  Raja N on 19.07.2005, Changed the implimentation to   */
/*                      call replay manager                                   */
/******************************************************************************/
void CMainFrame::OnReplaySingleSkip()
{
    // Dispatch it to Replay Manager
    vREP_CmdSkip();
}
/******************************************************************************/
/*  Function Name    :  OnReplaySingleStep                                    */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user select command step    */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.04.2002                                            */
/*  Modifications    :  Raja N on 19.07.2005, Changed the implimentation to   */
/*                      call replay manager                                   */
/******************************************************************************/
void CMainFrame::OnReplaySingleStep()
{
    // Dispatch it to Replay Manager
    vREP_CmdStep();
}
/******************************************************************************/
/*  Function Name    :  OnReplayStop                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user select command stop    */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.04.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  22.10.2002, Flag g_bStopReplay name changed           */
/*  Modifications    :  Raja N on 19.07.2005, Changed the implimentation to   */
/*                      call replay manager                                   */
/******************************************************************************/
void CMainFrame::OnReplayStop()
{
    // Dispatch it to Replay Manager
    vREP_CmdStop();
}
/******************************************************************************/
/*  Function Name    :  OnReplayGo                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user select go command      */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.04.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  29.10.2002, keep the go button always pressed if      */
/*                                  button is enable and pressed              */
/*  Modifications    :  Raja N on 19.07.2005, Changed the implimentation to   */
/*                      call replay manager                                   */
/******************************************************************************/
void CMainFrame::OnReplayGo()
{
    // Dispatch it to Replay Manager
    vREP_CmdGo();
}
/******************************************************************************/
/*  Function Name    :  OnActivateMessageHandler                              */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user select to activate     */
/*                      message handler                                       */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  14.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.04.2002,                                           */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  29.04.2004,                                           */
/*                   :  Replaced PostMessage with direct assignment of global */
/*                      variable g_bMsgHandlerON Msg Handler Enable flag      */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :  27.12.2005,                                           */
/*                   :   Inserted CExecute manger for activating flags        */
/******************************************************************************/
extern BOOL gbStopActiveThreads(int);
void CMainFrame::OnActivateMessageHandler()
{
    //Get present status
    BOOL bEnable = GetICANNodeSim()->NS_GetHandlerStatus(H_MSGHANDLERBUTTON);
    //Change the status of handler
    GetICANNodeSim()->NS_EnableAllMessageHandler( !bEnable );
}

void CMainFrame::OnActivateMessageHandlerLIN()
{
    //Get present status
    BOOL bEnable = GetILINNodeSim()->NS_GetHandlerStatus(H_MSGHANDLERBUTTON);
    //Change the status of handler
    GetILINNodeSim()->NS_EnableAllMessageHandler( !bEnable );
}

/******************************************************************************/
/*  Function Name    :  OnActivateTimerHandler                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user select to activate     */
/*                      timer handler                                         */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  14.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  07.10.2003, The flag is 'm_bTimerInvokedOnce'         */
/*                      introduced due to change from model dialog to         */
/*                      modelless dialog.                                     */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :  27.12.2005,                                           */
/*                   :   Inserted CExecute manger for activating flags        */
/******************************************************************************/
void CMainFrame::OnActivateTimerHandler()
{
    //Get present status
    BOOL bEnable = GetICANNodeSim()->NS_GetHandlerStatus(H_TIMERBUTTON);
    //Change the status of handler
    GetICANNodeSim()->NS_EnableAllTimers( !bEnable );
}

/******************************************************************************/
/*  Function Name    :  OnActivateTimerHandler                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user select to activate     */
/*                      timer handler                                         */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  14.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  07.10.2003, The flag is 'm_bTimerInvokedOnce'         */
/*                      introduced due to change from model dialog to         */
/*                      modelless dialog.                                     */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :  27.12.2005,                                           */
/*                   :   Inserted CExecute manger for activating flags        */
/******************************************************************************/
void CMainFrame::OnActivateTimerHandlerLIN()
{
    //Get present status
    BOOL bEnable = GetILINNodeSim()->NS_GetHandlerStatus(H_TIMERBUTTON);
    //Change the status of handler
    GetILINNodeSim()->NS_EnableAllTimers( !bEnable );
}


/******************************************************************************/
/*  Function Name    :  OnDLLBuildLoad                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user want to build and load */
/*                       all the DLL under the configuration.                 */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :                                                        */
/******************************************************************************/
void CMainFrame::OnDLLBuildLoad()
{
    CStringArray omStrBuildFiles;

    BOOL bSucces = GetICANNodeSim()->NS_DLLBuildLoadAll(&omStrBuildFiles);
    if(!bSucces)  // if the build is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are either not properly build or loaded:"));
        CString omStrErrorMsgDummy="";

        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

/******************************************************************************/
/*  Function Name    :  OnDLLBuildLoad                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user want to build and load */
/*                       all the DLL under the configuration.                 */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :                                                        */
/******************************************************************************/
void CMainFrame::OnDLLBuildLoadLIN()
{
    CStringArray omStrBuildFiles;

    BOOL bSucces = GetILINNodeSim()->NS_DLLBuildLoadAll(&omStrBuildFiles);
    if(!bSucces)  // if the build is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are either not properly build or loaded:"));
        CString omStrErrorMsgDummy="";

        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}


/******************************************************************************/
/*  Function Name    :  bDLLBuildLoad                                         */
/*  Input(s)         :  Stringarray                                           */
/*  Output           :  array of files not built properly                     */
/*  Functionality    :  It builds and load                                    */
/*                       all the DLL under the configuration.                 */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish                                                 */
/*  Date Created     :  15.06.2006                                            */
/*  Modification By  :  Anish                                                 */
/*                   :  18.10.06,Bug if any file is opened and BL is called   */
/******************************************************************************/

BOOL CMainFrame::bDLLBuildLoad(CStringArray* /*omStrBuildFiles*/)
{
    BOOL bReturn = FALSE;
    //  BOOL bSuccess = FALSE;
    //  int nFailure=0;
    //  /*CSimSysNodeInfo::PSSIMSYSINFO pTempSimsys =
    //      theApp.psReturnSimsysInfoPtr();*/
    //  CExecuteManager::ouGetExecuteManager().vStopTimer();
    //  CExecuteManager::ouGetExecuteManager().vClearOutputWnd();
    //  int nNodeCount = 0;
    //
    //  omStrBuildFiles->RemoveAll();
    //
    //  CWaitCursor omWait;
    //  //while(pTempSimsys != NULL)
    //  //{
    //  //  nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
    //  //  CSimSysNodeInfo::PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
    //
    //  //  while(pTempNode != NULL)
    //  //  {
    //  //      if(pTempNode->m_sNodeInfo.m_omStrFileName != "")
    //  //      {
    //  //          //TO find which all files are opened through Framewnd
    //  //          CMDIFrameWnd *pMainWnd = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //  //          if(pMainWnd != NULL)
    //  //          {
    //  //              //Get the active child wnd
    //  //              CWnd *pWndTemp = pMainWnd->GetActiveFrame();
    //  //              if(pWndTemp != NULL)
    //  //              {
    //  //                  CWnd *pWnd = pWndTemp;
    //  //                  do
    //  //                  {
    //  //                      CString omStrWndName;
    //  //                      CString omStrFileName =pTempNode->m_sNodeInfo.m_omStrFileName;
    //  //                      int nIndex = omStrFileName.ReverseFind(defCHAR_PATH_SEPRATER);
    //  //                      int nLength = omStrFileName.GetLength();
    //  //                      omStrFileName = omStrFileName.Right(nLength - nIndex -1);
    //  //                      pWnd->GetWindowText(omStrWndName);
    //  //                      //if the file is opened save it
    //  //                      if(!(omStrFileName.Compare(omStrWndName)))
    //  //                      {
    //  //                          CDocument*(pDoc) = theApp.OpenDocumentFile(pTempNode->
    //  //                              m_sNodeInfo.m_omStrFileName);
    //  //                          if(pDoc!=NULL)
    //  //                          {
    //  //                              // If file name is not empty generate new def file
    //  //                              pDoc->OnSaveDocument(pTempNode->
    //  //                                  m_sNodeInfo.m_omStrFileName);
    //  //                          }
    //  //                          break;
    //  //                      }
    //  //                      pWnd = pWnd->GetNextWindow();
    //  //                  }
    //  //                  while((pWndTemp != pWnd) && pWnd != NULL);
    //
    //  //              }
    //  //              bSuccess = CExecuteManager::ouGetExecuteManager().
    //  //                  bExecuteDllBuildLoad(&pTempNode->m_sNodeInfo);
    //  //              if(!bSuccess)
    //  //              {
    //  //                  nFailure++;
    //  //                  omStrBuildFiles->Add(pTempNode->m_sNodeInfo.m_omStrFileName);
    //  //              }
    //  //          }
    //  //      }
    //  //      pTempNode = pTempNode->m_psNextNode;
    //  //  }
    //  //  pTempSimsys = pTempSimsys->m_psSimsysNext;
    //  //}
    //
    //
    //
    //    if((nFailure == 0) || (nFailure != nNodeCount))
    //    {   // if the build n load is successfull atleast for one.
    //
    //        // to indicate to the tree view about the dlls loaded.
    //        /*CSimSysTreeView* psSimSysTree = podGetSimSysTreeView();
    //        if(psSimSysTree != NULL)
    //        psSimSysTree->bPopulateTree();*/
    //
    //    }
    //  if(nFailure == 0)
    //  {
    //      bReturn= TRUE;
    //  }
    //
    //  CExecuteManager::ouGetExecuteManager().vStartTimer();
    return bReturn;
}


/******************************************************************************/
/*  Function Name    :  OnDLLBuild                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame-work when user wants to build         */
/*                      all the C files associated with all the simulated
/*                      systems.                                              */
/*
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :
/******************************************************************************/
void CMainFrame::OnDLLBuild()
{
    CStringArray omStrBuildFiles;

    BOOL bSucces = GetICANNodeSim()->NS_DLLBuildAll(&omStrBuildFiles);
    if(!bSucces)  // if the build is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are not properly build:"));
        CString omStrErrorMsgDummy="";

        for(int i = 0 ; i < nFailure; i++)
        {
            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}


/******************************************************************************/
/*  Function Name    :  OnDLLBuild                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame-work when user wants to build         */
/*                      all the C files associated with all the simulated
/*                      systems.                                              */
/*
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :
/******************************************************************************/
void CMainFrame::OnDLLBuildLIN()
{
    CStringArray omStrBuildFiles;

    BOOL bSucces = GetILINNodeSim()->NS_DLLBuildAll(&omStrBuildFiles);
    if(!bSucces)  // if the build is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are not properly build:"));
        CString omStrErrorMsgDummy="";

        for(int i = 0 ; i < nFailure; i++)
        {
            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

/******************************************************************************/
/*  Function Name    :  bDLLBuild                                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :   to build                                             */
/*                      all the C files associated with all the simulated     */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish Kr                                              */
/*  Date Created     :  16.06.06                                              */
/*  Modification By  :  Anish                                                 */
/*                   :  18.10.06,Bug if any file is opened and BA is called   */
/*  Modification By  :  Anish,02.02.07
                        Bug fixed,removed infinite loop if file name is empty
/******************************************************************************/
BOOL CMainFrame::bDLLBuild(CStringArray* /*omStrBuildFiles*/)
{
    BOOL bReturn = FALSE;
    //BOOL bSuccess = FALSE;
    //int nFailure = 0;
    /* CSimSysNodeInfo::PSSIMSYSINFO pTempSimsys =
             theApp.psReturnSimsysInfoPtr();*/
    //CString omStrFileName = STR_EMPTY;
    //int nNodeCount = 0;
    //omStrBuildFiles->RemoveAll();

    //CExecuteManager::ouGetExecuteManager().vClearOutputWnd();
    //CWaitCursor omWait;
    //while(pTempSimsys != NULL)
    //{
    //  nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
    //  CSimSysNodeInfo::PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
    //  while(pTempNode != NULL)
    //  {
    //      if(pTempNode->m_sNodeInfo.m_omStrFileName != "")
    //      {
    //          //TO find which all files are opened through Framewnd
    //          CMDIFrameWnd *pMainWnd = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //          if(pMainWnd != NULL)
    //          {
    //              //Get the active child wnd
    //              CWnd *pWndTemp = pMainWnd->GetActiveFrame();
    //              if(pWndTemp != NULL)
    //              {
    //                  CWnd *pWnd = pWndTemp;
    //                  do
    //                  {
    //                      CString omStrWndName;
    //                      CString omStrFileName =pTempNode->m_sNodeInfo.m_omStrFileName;
    //                      int nIndex = omStrFileName.ReverseFind(defCHAR_PATH_SEPRATER);
    //                      int nLength = omStrFileName.GetLength();
    //                      omStrFileName = omStrFileName.Right(nLength - nIndex -1);
    //                      pWnd->GetWindowText(omStrWndName);
    //                      //if the file is opened save it
    //                      if(!(omStrFileName.Compare(omStrWndName)))
    //                      {
    //                          CDocument*(pDoc) = theApp.OpenDocumentFile(pTempNode->
    //                                                      m_sNodeInfo.m_omStrFileName);
    //                          if(pDoc!=NULL)
    //                          {
    //                              // If file name is not empty generate new def file
    //                              pDoc->OnSaveDocument(pTempNode->
    //                                       m_sNodeInfo.m_omStrFileName);
    //                          }
    //                          break;
    //                      }
    //                      pWnd = pWnd->GetNextWindow();
    //                  }
    //                  while((pWndTemp != pWnd) && pWnd != NULL);


    //                  bSuccess = CExecuteManager::ouGetExecuteManager().
    //                      bExecuteDllBuild(&pTempNode->m_sNodeInfo);
    //                  if(!bSuccess)
    //                  {
    //                      nFailure++;
    //                      omStrBuildFiles->Add(pTempNode->m_sNodeInfo.m_omStrFileName);
    //                  }

    //              }
    //          }
    //      }
    //      pTempNode = pTempNode->m_psNextNode;
    //  }
    //  pTempSimsys = pTempSimsys->m_psSimsysNext;
    //}
    //if((nFailure == 0) || (nFailure != nNodeCount))
    //{ // if the build is successfull atleast for one.

    // to indicate to the tree view about the new dlls built.
    /*CSimSysTreeView* psSimSysTree = podGetSimSysTreeView();
    if(psSimSysTree != NULL)
        psSimSysTree->bPopulateTree();*/
    //}
    //if(nFailure == 0)
    //{
    //  bReturn= TRUE;
    //}
    return bReturn;


}

/******************************************************************************/
/*  Function Name    :  OnDllLoad                                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user want to  load          */
/*                       all the DLLs under the configuration.                */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :                                            */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :                                                        */
/******************************************************************************/
void CMainFrame::OnDllLoad()
{
    CStringArray omStrBuildFiles;

    BOOL bSucces = GetICANNodeSim()->NS_DllLoadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the load is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are not properly loaded:"));
        CString omStrErrorMsgDummy="";

        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }

}

/******************************************************************************/
/*  Function Name    :  OnDllLoad                                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user want to  load          */
/*                       all the DLLs under the configuration.                */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :                                            */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :                                                        */
/******************************************************************************/
void CMainFrame::OnDllLoadLIN()
{
    CStringArray omStrBuildFiles;

    BOOL bSucces = GetILINNodeSim()->NS_DllLoadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the load is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are not properly loaded:"));
        CString omStrErrorMsgDummy="";

        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }

}



/******************************************************************************/
/*  Function Name    :  bDllLoad                                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user want to  load          */
/*                       all the DLLs under the configuration.                */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :                                                        */
/******************************************************************************/
BOOL CMainFrame::bDllLoad(CStringArray* /*omStrBuildFiles*/)
{
    BOOL bReturn = FALSE;
    //BOOL bSuccess = FALSE;
    //int nFailure = 0;
    /* CSimSysNodeInfo::PSSIMSYSINFO pTempSimsys =
             theApp.psReturnSimsysInfoPtr();*/
    //int nNodeCount = 0;

    //omStrBuildFiles->RemoveAll();

    //CWaitCursor omWait;
    //CExecuteManager::ouGetExecuteManager().vStopTimer();
    //CExecuteManager::ouGetExecuteManager().vClearOutputWnd();
    //while(pTempSimsys != NULL)
    //{
    //       nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
    //       CSimSysNodeInfo::PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
    //       while(pTempNode != NULL)
    //       {
    //           if(pTempNode->m_sNodeInfo.m_omStrDllName != "")
    //           {
    //               bSuccess = CExecuteManager::ouGetExecuteManager().bExecuteDllLoad(&
    //                   pTempNode->m_sNodeInfo);
    //               if(!bSuccess)
    //               {
    //                   nFailure++;
    //                   omStrBuildFiles->Add(pTempNode->m_sNodeInfo.m_omStrDllName);
    //               }
    //               else
    //               {    // to make the status of the handlers disabled
    //                   pTempNode->m_sNodeInfo.m_bDllHandlersEnabled = FALSE;
    //                   pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = FALSE;
    //                   pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled = FALSE;
    //                   pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled = FALSE;
    //                   pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled = FALSE;
    //                   pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = FALSE;
    //
    //
    //                   theApp.pouGetFlagsPtr()->vSetFlagStatus(ALL_HANDLER, FALSE);
    //
    //                   theApp.pouGetFlagsPtr()->vSetFlagStatus(ERROR_HANDLER, FALSE);
    //                   theApp.pouGetFlagsPtr()->vSetFlagStatus(KEY_HANDLER_ON, FALSE);
    //                   theApp.pouGetFlagsPtr()->vSetFlagStatus(MSGHANDLERBUTTON, FALSE);
    //               }
    //
    //           }
    //           pTempNode = pTempNode->m_psNextNode;
    //       }
    //       pTempSimsys = pTempSimsys->m_psSimsysNext;
    //
    //   }
    //if((nFailure == 0) || (nFailure != nNodeCount))
    //{   // if the load is successfull atleast for one dll.

    // to indicate to the tree view about the dlls loaded.
    /*CSimSysTreeView* psSimSysTree = podGetSimSysTreeView();
    if(psSimSysTree != NULL)
        psSimSysTree->bPopulateTree();*/
    //}

    //CExecuteManager::ouGetExecuteManager().vStartTimer();
    //if(nFailure == 0)
    //{
    //  bReturn= TRUE;
    //}
    return bReturn;
}


/******************************************************************************/
/*  Function Name    :  bDllUnload                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :   to  unload                                           */
/*                       all the DLLs under the configuration.                */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish Kr                                              */
/*  Date Created     :  16.06.2006                                            */
/*  Modification By  :                                                        */
/******************************************************************************/
BOOL CMainFrame::bDllUnload(CStringArray* /*omStrBuildFiles*/)
{
    BOOL bReturn = FALSE;
    //BOOL bSuccess = FALSE;
    //int nFailure = 0;
    /*CSimSysNodeInfo::PSSIMSYSINFO pTempSimsys =
                theApp.psReturnSimsysInfoPtr();*/
    /* int nNodeCount = 0;

     CStringArray omStrUnLoadFiles;
     omStrBuildFiles->RemoveAll();

    CWaitCursor omWait;
    CExecuteManager::ouGetExecuteManager().vStopTimer();
    CExecuteManager::ouGetExecuteManager().vClearOutputWnd();*/
    //while(pTempSimsys != NULL)
    //{
    //    nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
    //    CSimSysNodeInfo::PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
    //    while(pTempNode != NULL)
    //    {
    //        if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)  // if any dll is loaded
    //        {
    //            bSuccess = CExecuteManager::ouGetExecuteManager().bExecuteDllUnload(&
    //                pTempNode->m_sNodeInfo);
    //            if(!bSuccess)
    //            {
    //                nFailure++;
    //                omStrBuildFiles->Add(pTempNode->m_sNodeInfo.m_omStrDllName);
    //            }
    //            else
    //            {    // to make the status of the handlers disabled
    //                pTempNode->m_sNodeInfo.m_bDllHandlersEnabled = FALSE;
    //                pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = FALSE;
    //                pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled = FALSE;
    //                pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled = FALSE;
    //                pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled = FALSE;
    //                pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = FALSE;
    //
    //
    //                theApp.pouGetFlagsPtr()->vSetFlagStatus(ALL_HANDLER, FALSE);
    //
    //                theApp.pouGetFlagsPtr()->vSetFlagStatus(ERROR_HANDLER, FALSE);
    //                theApp.pouGetFlagsPtr()->vSetFlagStatus(KEY_HANDLER_ON, FALSE);
    //                theApp.pouGetFlagsPtr()->vSetFlagStatus(MSGHANDLERBUTTON, FALSE);
    //            }
    //
    //        }
    //        pTempNode = pTempNode->m_psNextNode;
    //    }
    //    pTempSimsys = pTempSimsys->m_psSimsysNext;
    //
    //}
    //   if((nFailure == 0) || (nFailure != nNodeCount))
    //   {   // if the unload is successfull atleast for one dll.
    //
    //       // to indicate to the tree view about the dlls unloaded.
    //       /*CSimSysTreeView* psSimSysTree = podGetSimSysTreeView();
    //       if(psSimSysTree != NULL)
    //           psSimSysTree->bPopulateTree();*/
    //   }

    //CExecuteManager::ouGetExecuteManager().vStartTimer();

    //if(nFailure == 0)
    //{
    //  bReturn= TRUE;
    //}
    return bReturn;

}

/******************************************************************************/
/*  Function Name    :  OnDllUnload                                           */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user want to  unload        */
/*                       all the DLLs under the configuration.                */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :                                                        */
/******************************************************************************/
void CMainFrame::OnDllUnload()
{
    CStringArray omStrBuildFiles;
    BOOL bSucces = GetICANNodeSim()->NS_DllUnloadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the unload is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are not properly unloaded:"));
        CString omStrErrorMsgDummy="";

        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

/******************************************************************************/
/*  Function Name    :  OnDllUnload                                           */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user want to  unload        */
/*                       all the DLLs under the configuration.                */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :                                                        */
/******************************************************************************/
void CMainFrame::OnDllUnloadLIN()
{
    CStringArray omStrBuildFiles;
    BOOL bSucces = GetILINNodeSim()->NS_DllUnloadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the unload is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are not properly unloaded:"));
        CString omStrErrorMsgDummy="";

        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}
/******************************************************************************/
/*  Function Name    :  OnDllUnload                                           */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by frame work when user want to  unload        */
/*                       all the DLLs under the configuration.                */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.01.2006                                            */
/*  Modification By  :                                                        */
/******************************************************************************/
void CMainFrame::OnDllUnloadJ1939()
{
    CStringArray omStrBuildFiles;
    BOOL bSucces = GetIJ1939NodeSim()->NS_DllUnloadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the unload is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are not properly unloaded:"));
        CString omStrErrorMsgDummy="";

        for(int i = 0 ; i < nFailure; i++)
        {
            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

/******************************************************************************/
/*  Function Name    :  vPopulateJ1939PGNList                                 */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Fills the J1939PGNList struct with dtabase message ID,
                        name and color for future use.
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Arunkumar K                                           */
/*  Date Created     :  08.03.2011                                            */
/*  Modifications    :                                                        */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CMainFrame::vPopulateJ1939PGNList()
{
    if ( m_pouMsgSigJ1939 != NULL )
    {
        CMessageAttrib& ouMsgAttr = CMessageAttrib::ouGetHandle(J1939);
        CStringList omStrMsgNameList;
        UINT unNoOfMsgs =
            m_pouMsgSigJ1939->unGetNumerOfMessages();

        UINT* pIDArray = new UINT[unNoOfMsgs];

        m_pouMsgSigJ1939->omStrListGetMessageNames(omStrMsgNameList);

        if (pIDArray != NULL )
        {
            m_pouMsgSigJ1939->unListGetMessageIDs( pIDArray );

            SCanIDList sList;

            POSITION pos = omStrMsgNameList.GetHeadPosition();

            UINT unCount = 0;
            POSITION pos1 = pos;

            for ( pos1 = pos, unCount = (unNoOfMsgs - 1);
                    ((pos1 != NULL) && (unCount >= 0));
                    unCount--)
            {
                sList.nCANID        = pIDArray[unCount];
                sList.omCANIDName   = omStrMsgNameList.GetNext( pos1 );

                if (ouMsgAttr.nValidateNewID(sList.nCANID) == MSGID_DUPLICATE)
                {
                    sList.Colour = ouMsgAttr.GetCanIDColour(sList.nCANID);
                    ouMsgAttr.nModifyAttrib(sList);
                }
                else
                {
                    sList.Colour = DEFAULT_MSG_COLOUR;
                    ouMsgAttr.nAddNewAttrib( sList );
                }
            }

            ouMsgAttr.vDoCommit();
            delete [] pIDArray;
            pIDArray = NULL;
        }
    }
}

/******************************************************************************
    Function Name    :  OnMessageInterpretation

    Input(s)         :      -
    Output           :      -
    Functionality    :  Changes the press state of Interpretation
                        tool bar button
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  20.03.2002
    Modifications    :  Ratnadip Choudhury
    Date modified    :  26.03.2002
******************************************************************************/
void CMainFrame::OnMessageInterpretation()
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        if (IS_MODE_OVER(byGetDispFlag))
        {
            CLEAR_EXPR_DISP_BITS(byGetDispFlag);
            SET_MODE_INTRP(byGetDispFlag);
            m_bInterPretMsg = TRUE;
            theApp.pouGetFlagsPtr()->vSetFlagStatus(TRANSLATIONMODE, TRUE);
        }
        else if (IS_MODE_INTRP(byGetDispFlag))
        {
            CLEAR_EXPR_DISP_BITS(byGetDispFlag);
            SET_MODE_OVER(byGetDispFlag);
            m_bInterPretMsg = FALSE;
            theApp.pouGetFlagsPtr()->vSetFlagStatus(TRANSLATIONMODE, FALSE);
        }
        else
        {
            ASSERT(FALSE); // Must not arise
        }
        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
            //Update Message Window
            if(hWnd)
            {
                BYTE bModes = DISPLAY_MODE;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }
}

void CMainFrame::vPopulateSigWatchList(CMainEntryList& odFromList, SMSGENTRY*& psToList, CMsgSignal* pouDatabase)
{
    vReleaseSignalWatchListMemory(psToList);
    SMSGENTRY* pTemp;
    POSITION MsgPos = odFromList.GetHeadPosition();

    while (MsgPos != NULL)
    {
        SMAINENTRY& sMainEntry = odFromList.GetNext(MsgPos);
        POSITION SigPos = sMainEntry.m_odSelEntryList.GetHeadPosition();
        sMESSAGE* pDBMsg =  pouDatabase->psGetMessagePointer(sMainEntry.m_unMainEntryID);
        if ((SigPos != NULL) && (pDBMsg != NULL))//Atleast one signal is there
        {
            pTemp = new SMSGENTRY;
            pTemp->m_psMsg = SMSGENTRY::psCopyMsgVal (pDBMsg);
            sSIGNALS* psTempSig = NULL;
            while (SigPos != NULL)
            {
                SSUBENTRY& sSubEntry = sMainEntry.m_odSelEntryList.GetNext(SigPos);
                sSIGNALS* pSig;
                if (bIsSigExist(sSubEntry.m_omSubEntryName, pDBMsg->m_psSignals, pSig))
                {
                    psTempSig = SMSGENTRY::psCopySignalVal(pSig);

                    if (pTemp->m_psMsg->m_psSignals == NULL)
                    {
                        pTemp->m_psMsg->m_psSignals = psTempSig;
                    }
                    else
                    {
                        //copy signal at the end and swap the pointers
                        psTempSig->m_psNextSignalList = pTemp->m_psMsg->m_psSignals;
                        pTemp->m_psMsg->m_psSignals = psTempSig;
                    }
                }
            }

            if (psToList == NULL)//For the very first time
            {
                psToList = pTemp;
            }
            else
            {
                //Copy the message at the end and swap the pointers
                pTemp->m_psNext = psToList;
                psToList = pTemp;
            }

        }
    }
}

void CMainFrame::vUpdateSWList()
{
    CMainEntryList odResultingList;
    vPopulateMainEntryList(&odResultingList, m_psSignalWatchList[CAN], theApp.m_pouMsgSignal);
    vPopulateSigWatchList(odResultingList, m_psSignalWatchList[CAN], theApp.m_pouMsgSignal);// This populates m_psSignalWatchList
    if (sg_pouSWInterface[CAN] != NULL)
    {
        m_ouMsgInterpretSW_C.vSetMessageList(m_psSignalWatchList[CAN]);
        sg_pouSWInterface[CAN]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_C);
    }

}
/*******************************************************************************
 Function Name    : OnAddSignalToSignalWindow
 Input(s)         : -
 Output           : -
 Functionality    : This function is called by framework when user wants to
                    watch a signal value. This function will display the
                    database messages to select signals. The selected signal
                    names will be copied into Signal watch data structure.user
                    can also remove added signal from the Signal Watch List
 Member of        : CMainFrame
 Friend of        :  -
 Author(s)        : Amarnath Shastry
 Date Created     : 25.03.2002
 Modification By  : Amarnath Shastry
 Modifications on : 27.03.2002
 Modifications    : Raja N on 30.03.2004
                    Modified to refer latest dialog for add/del operations
 Modifications    : Raja N on 10.08.2004
                    Added check to find the database import. If database is not
                    imported don't show the dialog and show error msg box.
 Modifications    : Anish on 13.12.2006
                    Changed code for checking whether a database is selected
                    from CFlag class.
*******************************************************************************/
void CMainFrame::OnAddSignalToSignalWindow()
{
    CMsgSignal* pomDatabase = NULL;
    pomDatabase = theApp.m_pouMsgSignal;
    if( pomDatabase != NULL )
    {
        if( pomDatabase->unGetNumerOfMessages() > 0)
        {
            /* Test code starts*/
            CMainEntryList odResultingList;
            vPopulateMainEntryList(&odResultingList, m_psSignalWatchList[CAN], theApp.m_pouMsgSignal);
            if (sg_pouSWInterface[CAN] == NULL)
            {
                if (SW_GetInterface(CAN, (void**)&sg_pouSWInterface[CAN]) == S_OK)
                {
                    sg_pouSWInterface[CAN]->SW_DoInitialization();
                }
            }
            if (sg_pouSWInterface[CAN] != NULL)
            {
                if (sg_pouSWInterface[CAN]->SW_ShowAddDelSignalsDlg(this, &odResultingList) == IDOK)
                {
                    vPopulateSigWatchList(odResultingList, m_psSignalWatchList[CAN], theApp.m_pouMsgSignal);// This populates m_psSignalWatchList
                    m_ouMsgInterpretSW_C.vSetMessageList(m_psSignalWatchList[CAN]);
                    sg_pouSWInterface[CAN]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_C);
                }
            }
        }
        else
        {
            // Database is not imported!!
            AfxMessageBox( _(defSTR_EMPTY_ACTIVE_DATABASE) );
        }
    }

}
/******************************************************************************
    Function Name    :  OnMessageFilter

    Input(s)         :  void
    Output           :  void
    Functionality    :  Called by the framework when user selects message filter
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  05.03.2002
    Modifications    :  Amitesh Bharti, 26.12.2002, Function name is changed.
******************************************************************************/
//void CMainFrame::OnMessageFilter()
//{
//    OnMessageFilterButton();
//}
/******************************************************************************
    Function Name    :  OnLogFilter
    Input(s)         :  void
    Output           :  void
    Functionality    :  Called by the framework when user select Log Filter
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  05.03.2002
    Modifications    :  Amitesh Bharti, 26.12.2002, Function name is changed
                                               and CFlag pointer is validated
    Modifications    :  Raja N on 19.07.2005, Changed the code to call Log file
                        manager to enable filter
******************************************************************************/
void CMainFrame::OnLogFilter()
{
    CFlags* pouFlags = NULL;
    BOOL bLogFilterStatus = FALSE;

    pouFlags = theApp.pouGetFlagsPtr();
    if(pouFlags != NULL )
    {
        bLogFilterStatus = pouFlags->nGetFlagStatus(LOGFILTER);
        bLogFilterStatus = bLogFilterStatus ? FALSE : TRUE;
        pouFlags->vSetFlagStatus(LOGFILTER, bLogFilterStatus);
        if (sg_pouFrameProcCAN != NULL)
        {
            INT Count = sg_pouFrameProcCAN->FPC_GetLoggingBlockCount();
            for (INT i = 0; i < Count; i++)
            {
                sg_pouFrameProcCAN->FPC_EnableFilter((USHORT)i, bLogFilterStatus);
            }
        }
        /* Modify filter icon accordingly in Main toolbar*/
        BYTE bytTbrItemIndex = 5;
        vModifyToolbarIcon( m_wndToolBar, bytTbrItemIndex, bLogFilterStatus, IDI_ICON_LOG_FILTER_ON, IDI_ICON_LOG_FILTER );
    }
}

void CMainFrame::ApplyReplayFilter()
{
    CFlags* pouFlags = NULL;
    BOOL bReplayFilterStatus = FALSE;

    pouFlags = theApp.pouGetFlagsPtr();
    if(pouFlags != NULL )
    {
        bReplayFilterStatus = pouFlags->nGetFlagStatus(REPLAYFILTER);
        pouFlags->vSetFlagStatus(REPLAYFILTER, bReplayFilterStatus);

        vREP_EnableFilters(bReplayFilterStatus);

        /* Modify filter icon accordingly in Main toolbar*/
        BYTE bytTbrItemIndex = 7;
        vModifyToolbarIcon( m_wndToolBar, bytTbrItemIndex, bReplayFilterStatus, IDI_ICON_REPLAY_FILTER_ON, IDI_ICON_REPLAY_FILTER );
    }
}

void CMainFrame::ApplyLogFilter()
{
    CFlags* pouFlags = NULL;
    BOOL bLogFilterStatus = FALSE;

    pouFlags = theApp.pouGetFlagsPtr();
    if(pouFlags != NULL )
    {
        bLogFilterStatus = pouFlags->nGetFlagStatus(LOGFILTER);
        //bLogFilterStatus = bLogFilterStatus ? FALSE : TRUE;
        pouFlags->vSetFlagStatus(LOGFILTER, bLogFilterStatus);
        if (sg_pouFrameProcCAN != NULL)
        {
            INT Count = sg_pouFrameProcCAN->FPC_GetLoggingBlockCount();
            for (INT i = 0; i < Count; i++)
            {
                sg_pouFrameProcCAN->FPC_EnableFilter((USHORT)i, bLogFilterStatus);
            }
        }
        /* Modify filter icon accordi5ngly in Main toolbar*/
        BYTE bytTbrItemIndex = 5;
        vModifyToolbarIcon( m_wndToolBar, bytTbrItemIndex, bLogFilterStatus, IDI_ICON_LOG_FILTER_ON, IDI_ICON_LOG_FILTER );
        // logKadoor CLogManager::ouGetLogManager().vUpdateLogFilterEnable(bLogFilterStatus);
    }
}
/******************************************************************************
    Function Name    :  OnAboutApplication
    Input(s)         :  void
    Output           :  void
    Functionality    :  Called by the framework when user selects About
                        Application... menu option
                        Displays a dialog with program information.
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath Shastry
    Date Created     :  05.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnAboutApplication()
{
    theApp.OnAppAbout();

}
static void vPopulateMsgEntryFromDB(SMSGENTRY*& psMsgEntry,
                                    CMsgSignal* pouMsgSig)
{
    if (pouMsgSig != NULL)
    {
        UINT nCount = pouMsgSig->unGetNumerOfMessages();
        UINT* punMsgIds = new UINT[nCount];
        pouMsgSig->unListGetMessageIDs(punMsgIds);
        for (UINT i = 0; i < nCount; i++)
        {
            sMESSAGE* pMsg = pouMsgSig->
                             psGetMessagePointer(punMsgIds[i]);
            if (pMsg != NULL)
            {
                SMSGENTRY::bUpdateMsgList(psMsgEntry, pMsg);
            }
        }
        delete[] punMsgIds;
    }
}
/******************************************************************************/
/*  Function Name    :  OnSelectMessage                                       */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework when user wants  */
/*                      to select message filter list.A dialog box will be    */
/*                      for user to select the message list the choose options*/
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.02.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  23.03.2002, Reset the filter flag                     */
/*  Modification By  :  Raja N                                                */
/*  Modifications on :  02.08.2004, Removed disabling filtering call to avoid */
/*                      disabling of filtering                                */
/*  Modifications    :  Raja N on 19.07.2005, Changed the implementation to   */
/*                      call Filter Manager to show config dlg                */
/******************************************************************************/
void CMainFrame::OnSelectMessage()
{
    // Ask Filter Manager to show filter configuration dialog
    // This will update configuration module if user makes some
    // Modification
    SMSGENTRY* psMsgEntry = NULL;
    vPopulateMsgEntryFromDB(psMsgEntry, theApp.m_pouMsgSignal);
    LPARAM lParam = NULL;
    if (NULL != GetICANDIL())
    {
        GetICANDIL()->DILC_GetControllerParams(lParam, NULL, NUMBER_HW);
    }

    HRESULT hResult = Filter_ShowConfigDlg((void*)&m_sFilterAppliedCAN, psMsgEntry, CAN, (UINT)lParam, this);

    SMSGENTRY::vClearMsgList(psMsgEntry);
    psMsgEntry = NULL;

    if(hResult == S_OK)//if some changes occurs in filters, update it in all related modules
    {
        //Indicate to the modules
        //1. Log
        if (sg_pouFrameProcCAN != NULL)
        {
            UINT unLogCount = sg_pouFrameProcCAN->FPC_GetLoggingBlockCount();

            for (USHORT i = 0; i < unLogCount; i++)
            {
                SFILTERAPPLIED_CAN sFilterCan;
                sg_pouFrameProcCAN->FPC_GetFilteringScheme(i, sFilterCan);
                Filter_ReUpdateAppliedFilter(&sFilterCan, &m_sFilterAppliedCAN, CAN);
                //            sg_pouFrameProcCAN->FPC_StartEditingSession(); //editing is not required here
                sg_pouFrameProcCAN->FPC_ApplyFilteringScheme(i, sFilterCan);
                //            sg_pouFrameProcCAN->FPC_StopEditingSession(TRUE);
            }
        }
        //2. Replay
        vREP_SetConfiguredFilter(&m_sFilterAppliedCAN);
        //3. Message window
        if (m_podMsgWndThread != NULL)
        {
            SFILTERAPPLIED_CAN sMsgWndFilter;
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_GET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);

            //store the previous filter
            SFILTERAPPLIED_CAN      sTempAppliedFilter;
            sTempAppliedFilter.bClone(sMsgWndFilter);
            Filter_ReUpdateAppliedFilter(&sMsgWndFilter, &m_sFilterAppliedCAN, CAN);

            //restore the enable flag for all filters
            for(int nFilterCnt = 0; nFilterCnt < sTempAppliedFilter.m_ushTotal; nFilterCnt++)
            {
                if((sTempAppliedFilter.m_psFilters) != NULL)
                {
                    ((sMsgWndFilter.m_psFilters)+ nFilterCnt) ->m_bEnabled
                        =  ((sTempAppliedFilter.m_psFilters)+ nFilterCnt) ->m_bEnabled;
                }
            }
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);

            //if msg filter is enable, disable it and then re-enable it to affect the changes
            if(theApp.pouGetFlagsPtr()->nGetFlagStatus(DISPLAYFILTERON))
            {
                //Disable the msg filter, so it will update the modified changes
                theApp.pouGetFlagsPtr()->vSetFlagStatus(DISPLAYFILTERON, FALSE);
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_ENABLE_FILTER_APPLIED, (WPARAM)FALSE, NULL);

                //re-enable the msg filter, so it will show the modified changes
                theApp.pouGetFlagsPtr()->vSetFlagStatus(DISPLAYFILTERON, TRUE);
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_ENABLE_FILTER_APPLIED, (WPARAM)TRUE, NULL);

                /* Modify filter icon accordingly in Main toolbar*/
                BYTE bytTbrItemIndex = 6;
                vModifyToolbarIcon( m_wndToolBar, bytTbrItemIndex, TRUE, IDI_ICON_MSG_FILTER_ON, IDI_ICON_MSG_FILTER );
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  OnLogEnable                                           */
/*  Input(s)         :  -                                                     */
/*  Output           :  -                                                     */
/*  Functionality    :  This function is called by framework when user wants  */
/*                      a log file. This will set the status of corresponding
                        toolbar button.
                        Gets the log file from the registry.
                        Post a thread message to perform logging to the specifi
                        -ed log file name.
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ratnadip Choudhury, Amarnath Shastry                  */
/*  Date Created     :  29.03.2002                                            */
/*  Modifications    :  11.12.2002, Amarnath S                                */
/*                      Integration with config class included                */
/*  Modifications    :  19.08.2003, Krishnaswamy B.N                          */
/*                      Changes for CRH0008,Increment of log file name        */
/*                      depending on the eLOGOVERWRITEMODEON flag status      */
/*  Modifications    :  20.01.2003, Amitesh Bharti                            */
/*                      Checking for increment of file is corrected. It was   */
/*                      bOverWriteModeON == FALSE.                            */
/*  Modifications    :  Raja N on 19.07.2005, Modified the code to call Log   */
/*                      manager to start/stop logging                         */
/******************************************************************************/
void CMainFrame::OnLogEnable()
{
    // TODO: Add your command handler code here
    BOOL bLogON = FALSE;
    if (NULL != sg_pouFrameProcCAN)
    {
        bLogON = sg_pouFrameProcCAN->FPC_IsLoggingON();
    }
    bLogON = bLogON ? FALSE : TRUE;

    // PTV [1.6.4]
    if(bLogON == FALSE)
    {
        if (NULL != sg_pouFrameProcCAN)
        {
            sg_pouFrameProcCAN->FPC_DisableDataLogFlag();
        }
    }
    // PTV [1.6.4]

    // Set the status of logging
    // PTV[1.6.4]
    BOOL bIsConnected = FALSE;
    // PTV[1.6.4]
    CFlags* pouFlags = NULL;
    pouFlags = theApp.pouGetFlagsPtr();
    if (NULL != pouFlags)
    {
        pouFlags->vSetFlagStatus(LOGTOFILE, bLogON);
        bIsConnected = pouFlags->nGetFlagStatus(CONNECTED);
    }

    // PTV[1.6.5]
    // If Connected and Log is enabled
    if(bLogON == TRUE && bIsConnected == TRUE)
    {
        //if(theApp.m_pouMsgSignal != NULL)
        {
            //if(theApp.m_pouMsgSignal->unGetNumerOfMessages() > 0)
            //if (NULL != sg_pouFrameProcCAN)
            {
                //if(sg_pouFrameProcCAN->FPC_IsDataLogged() == TRUE)
                {
                    m_unTimerSBLog = SetTimer(TIMER_REFRESH_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, NULL);
                }
            }
        }
    }
    // PTV[1.6.5]
    // If not connected or log is disabled
    else
    {
        if(m_unTimerSBLog != NULL)
        {
            ::KillTimer(NULL, m_unTimerSBLog);
            m_unTimerSBLog = NULL;
            // Update Status bar
            //m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
            m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
        }
    }

    vStartStopLogging( bLogON );
}

/******************************************************************************/
/*  Function Name    :  OnLog_LIN_Enable                                      */
/*  Input(s)         :  -                                                     */
/*  Output           :  -                                                     */
/*  Functionality    :  This function is called by framework when user wants  */
/*                      a log file. This will set the status of corresponding
                        toolbar button.
                        Gets the log file from the registry.
                        Post a thread message to perform logging to the specifi
                        -ed log file name.
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Shashank Vernekar                                     */
/*  Date Created     :  29.01.2014                                            */
/******************************************************************************/

void CMainFrame::OnLog_LIN_Enable()
{
    //shashank
    // TODO: Add your command handler code here
    BOOL bLogON = FALSE;
    if (NULL != sg_pouFrameProcLIN )
        //if (NULL != sg_pouFrameProcLIN)
    {
        bLogON = sg_pouFrameProcLIN->FPL_IsLoggingON();
    }
    bLogON = bLogON ? FALSE : TRUE;


    // PTV [1.6.4]
    if(bLogON == FALSE)
    {
        if (NULL != sg_pouFrameProcLIN)
            //if (NULL != sg_pouFrameProcLIN)
        {
            sg_pouFrameProcLIN->FPL_DisableLINDataLogFlag();
        }
    }
    // PTV [1.6.4]

    // Set the status of logging
    // PTV[1.6.4]
    BOOL bIsConnected = FALSE;
    // PTV[1.6.4]
    CFlags* pouFlags = NULL;
    pouFlags = theApp.pouGetFlagsPtr();
    if (NULL != pouFlags)
    {
        pouFlags->vSetFlagStatus(LOGTOFILE_LIN, bLogON);
        bIsConnected = pouFlags->nGetFlagStatus(LIN_CONNECTED);
    }

    // PTV[1.6.5]
    // If Connected and Log is enabled
    if(bLogON == TRUE && bIsConnected == TRUE)
    {
        //if(theApp.m_pouMsgSignal != NULL)
        {
            //if(theApp.m_pouMsgSignal->unGetNumerOfMessages() > 0)
            //if (NULL != sg_pouFrameProcCAN)
            {
                //if(sg_pouFrameProcCAN->FPC_IsDataLogged() == TRUE)
                {
                    m_unTimerSBLog = SetTimer(TIMER_REFRESH_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, NULL);
                }
            }
        }
    }
    // PTV[1.6.5]
    // If not connected or log is disabled
    else
    {
        if(m_unTimerSBLog != NULL)
        {
            ::KillTimer(NULL, m_unTimerSBLog);
            m_unTimerSBLog = NULL;
            // Update Status bar
            //m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
            //shashank
            //m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
            m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_LIN_LOG_ICON, m_hLogOffIcon);
        }
    }

    vStartStopLogging_LIN( bLogON );
}
void CMainFrame::OnButtonMsgDispButton()
{

}

void CMainFrame::OnButtonSignalWatchButton()
{
    if (sg_pouSWInterface[CAN] == NULL)
    {
        if (SW_GetInterface(CAN, (void**)&sg_pouSWInterface[CAN]) == S_OK)
        {
            sg_pouSWInterface[CAN]->SW_DoInitialization();
        }
    }
    if (sg_pouSWInterface[CAN] != NULL)
    {
        INT nCmd = sg_pouSWInterface[CAN]->SW_IsWindowVisible() ? SW_HIDE : SW_SHOW;
        sg_pouSWInterface[CAN]->SW_ShowSigWatchWnd(this, nCmd);
        BOOL bHexON = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);
        sg_pouSWInterface[CAN]->SW_SetDisplayMode(bHexON);
    }
}
/******************************************************************************/
/*  Function Name    :  OnSendMessage                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework when user wants  */
/*                      to send message selected from combo box.              */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  23.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  25.03.2002, Interface for send message                */
/*  Modified By      :  Ratnadip Choudhury; 02-12-2002                        */
/*  Modification     :  Data buffer from the configuration file was released  */
/*                      even after passing its address to multiple message    */
/*                      sending thread: code to release the same is moved     */
/*                      away from here.                                       */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  08.01.2004, Interface for send message is changed     */
/*                                  The message block is transmitted by       */
/*                                  calling member function of CSendMumtiMsg  */
/*  Modifications    :  Raja N on 26.04.2005, Modified code to refer new      */
/*                      Tx Msg configuration window for posting message about */
/*                      Tx start and used Tx Manager object for msg transmission
/******************************************************************************/
void CMainFrame::OnSendMessage()
{
    // Check for SENDMESG flag, If true,
    // make it false and stop sending messages
    // If false, make it true and start sending messages
    CToolBarCtrl& omRefToolBarCtrl = vGetReferenceToToolBarCtrl();
    // Use of this flag can be avoided by using "g_bStopSendMultMsg"
    if ( !theApp.pouGetFlagsPtr()->nGetFlagStatus( SENDMESG ) )
    {

        // Now start sending messages, update flag
        theApp.pouGetFlagsPtr()->vSetFlagStatus( SENDMESG, TRUE );

        // Set the tool bar button to pressed state
        omRefToolBarCtrl.PressButton(IDR_TOOL_SENDMSG, TRUE);

        m_objTxHandler.vSetTxStopFlag(FALSE);
        //g_bStopMsgBlockTx       = FALSE;
        // Call Message Manager function to start message transmission
        //m_podTxMsgManager->vStartTransmission(0);
        m_objTxHandler.vStartTransmission(0);
    }
    else // Started, Stop the same
    {
        //m_objTxHandler.vSetTxStopFlag(TRUE);
        m_objTxHandler.vStopTransmission(0);
        //g_bStopMsgBlockTx       = TRUE;
        theApp.vDestroyUtilThreads(500, BIT_MULTI_MSG_THREAD );
        // Now start sending messages, update flag
        theApp.pouGetFlagsPtr()->vSetFlagStatus( SENDMESG, FALSE );
        // ReSet the tool bar button to pressed state
        omRefToolBarCtrl.PressButton(IDR_TOOL_SENDMSG, FALSE);
        // PTV [1.6.4]
        // Disabling Data logging Flag
        if (NULL != sg_pouFrameProcCAN)
        {
            sg_pouFrameProcCAN->FPC_DisableDataLogFlag();
        }
    }
    // Inform Tx Window about message transmission start if it exists
    if(m_objTxHandler.hConfigWindowShown() == S_OK)
    {
        CFlags* pouFlag = theApp.pouGetFlagsPtr();
        BOOL bTxON = FALSE;
        if(pouFlag != NULL )
        {
            bTxON = static_cast<BOOL> (pouFlag->nGetFlagStatus(SENDMESG));
        }
        eUSERSELCTION eUserSel     = eTXMSGCMD;
        m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bTxON);
    }
}

void CMainFrame::OnSendFlexRayMessage()
{
    bool        bConnected = false;
    if ( !theApp.pouGetFlagsPtr()->nGetFlagStatus( FLEXRAYSENDMSG ) )
    {
        m_objFlexTxHandler.vStartTransmission(0);
        theApp.pouGetFlagsPtr()->vSetFlagStatus( FLEXRAYSENDMSG, TRUE );
        bConnected = true;
    }
    else
    {
        m_objFlexTxHandler.vStopTransmission(0);
        theApp.pouGetFlagsPtr()->vSetFlagStatus( FLEXRAYSENDMSG, FALSE );
        bConnected = false;
    }
    CToolBar* pToolBar = &m_wndToolbarFlexRay;
    CToolBarCtrl& omRefToolBarCtrl = ((pToolBar)->GetToolBarCtrl());
    omRefToolBarCtrl.PressButton(IDR_TOOL_FLEX_SENDMSG, bConnected);
}
/******************************************************************************
    Function Name    :  podGetFunctionViewPtr

    Input(s)         :
    Output           :  CFunctionView*
    Functionality    :  Return pointer to CFunctionView class
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :
******************************************************************************/
//CFunctionView* CMainFrame::podGetFunctionViewPtr()
//{
//    return (m_pomFunctionView);
//}
/******************************************************************************
    Function Name    :  podGetFileViewPtr

    Input(s)         :
    Output           :  CFileView*
    Functionality    :  Return pointer to CFileView class
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :
******************************************************************************/
//CFileView* CMainFrame::podGetFileViewPtr()
//{
//    return (m_pomFileView);
//}
/******************************************************************************
    Function Name    :  podGetFuncsTreeViewPtr

    Input(s)         :
    Output           :  CFnsTreeView*
    Functionality    :  Return pointer to CFnsTreeView class
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :
******************************************************************************/
//CFnsTreeView* CMainFrame::podGetFuncsTreeViewPtr()
//{
//    return (m_pomFnsTreeView);
//}
/******************************************************************************
    Function Name    :  hGetModuleHandle

    Input(s)         :
    Output           :  HMODULE
    Functionality    :  Returns HMODULE which will have the handle to the DLL
                        loaded
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :
******************************************************************************/
HMODULE CMainFrame::hGetModuleHandle()
{
    return m_hModuleHandle;
}
/******************************************************************************
    Function Name    :  vSetModuleHandle

    Input(s)         :  HMODULE hModuleHandle, CString& omStrDLLName
    Output           :      -
    Functionality    :  Sets HMODULE which will have the handle to the DLL
                        loaded to the member variable and the corresponding DLL
                        name.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :  Amitesh Bharti
                        07.08.2002, changed the prototype of function to set
                        the DLL name which will be use
******************************************************************************/
VOID CMainFrame::vSetModuleHandleAndDLLName(HMODULE hModuleHandle, CString& omStrDLLName)
{
    m_hModuleHandle      = hModuleHandle;
    //   m_omStrPrevLoadedDll = omStrDLLName;
    m_omStrSourceFilePathName = omStrDLLName;

    //    theApp.bSetData(MRU_DLL_FILE_NAME, (void**)&m_omStrPrevLoadedDll);
}
/******************************************************************************
    Function Name    :  omStrConvByteArrToStr

    Input(s)         :  CByteArray *byteArr
    Output           :  CString
    Functionality    :  Function will convert a byte array to CString
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  19.03.2002
    Modifications    :
******************************************************************************/
CString CMainFrame::omStrConvByteArrToStr(CByteArray* byteArr)
{
    CString strDisplay="";
    BYTE oneByte;
    unsigned char firstCh, secondCh;
    for (int arrSize=0; arrSize< byteArr->GetSize(); arrSize++)
    {
        //Get each byte from the array
        oneByte = byteArr->GetAt(arrSize);
        //First char is arrived at by logical "AND"ing the byte with
        //0xf0 and then right shifting it by 4 bits
        firstCh = oneByte & 0xf0;
        firstCh>>=4;
        //Second char is arrived at by
        //"AND"ing with 0x0f
        secondCh = oneByte & 0x0f;

        //Following piece of code converts
        //the chars to the respective toASCII
        //values before adding to the string
        if ((firstCh >= 0) && (firstCh <=9))
        {
            firstCh+='0';
        }
        else
        {
            firstCh+=87;
        }
        if ((secondCh >= 0) && (secondCh <=9))
        {
            secondCh+='0';
        }
        else
        {
            secondCh+=87;
        }
        //Add the chars to the CStirng object
        strDisplay+=firstCh;
        strDisplay+=secondCh;
    }
    //The extra white space is removed here
    strDisplay= strDisplay.Left(strDisplay.GetLength() - 1);
    return strDisplay;
}
/******************************************************************************
    Function Name    :  vConvStrtoByteArray

    Input(s)         :   char* tempBuf
    Output           :  CByteArray *byteArr
    Functionality    :  Function will convert a string to equivalent
                        byte array.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  19.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::vConvStrtoByteArray(CByteArray* bufferTx, char* tempBuf)
{
    //Copy the modified string to a local Cstring
    //variable and remove all white spaces
    CString strTemp(tempBuf);
    char ch = ' ';
    strTemp.Remove(ch);
    unsigned int ncount=0;
    int nStrLength = strTemp.GetLength();
    unsigned char firstCh;
    unsigned char secondCh;

    //Take two characters at a time and then
    //start converting them into a byte.
    //   do
    while (nStrLength)
    {
        //get the first char
        firstCh = strTemp.GetAt(ncount++);
        ASSERT(AfxCheckMemory());

        //get the second char
        //if the value is greater than or equal to zero or
        //less than or equal to nine, get the value by
        //subtracting the ASCII value of '0'
        if (('0' <= firstCh) && (firstCh <= '9'))
        {
            firstCh -='0';
        }
        else
        {
            //The character is between 'a' to 'f'. First
            //convert to lowercase and deduct 87 which
            //will give the correct value for the char
            //as the ASCII value of 'a' is 97.
            firstCh = (UCHAR)tolower (firstCh);
            firstCh-= (UCHAR)87;
        }

        //if the value is greater than or equal to zero or
        //less than or equal to nine, get the value by
        //subtracting the ASCII value of '0'
        nStrLength --;

        if(nStrLength)
        {
            secondCh = strTemp.GetAt(ncount++);
            ASSERT(AfxCheckMemory());

            if (('0' <= secondCh) && (secondCh <= '9'))
            {
                secondCh -='0';
            }
            else
            {
                //The character is between 'a' to 'f'. First
                //convert to lowercase and deduct 87 which
                //will give the correct value for the char
                //as the ASCII value of 'a' is 97.
                secondCh = (UCHAR)tolower (secondCh);
                secondCh-= 87;
            }

            //Left shift first char by 4 bits and do
            //a logical "OR" with the second char
            firstCh <<= 4;
            firstCh |= secondCh;
            //firstCh = McnetCharSet[secondCh][firstCh];
            //end of insertion
            //Now add the byte to the byte array
            bufferTx->Add(firstCh);
            //Deduct the value of string length by 2
            nStrLength --;
        }
    }

    //while (nStrLength);
    return;
}
/******************************************************************************/
/*  Function Name    :  vGetReferenceToToolBarCtrl                            */
/*  Input(s)         :                                                        */
/*  Output           :  CToolBarCtrl&                                         */
/*  Functionality    :  Returns reference to CToolBarCtrl
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  14.03.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/

CToolBarCtrl& CMainFrame::vGetReferenceToToolBarCtrl()
{
    CToolBar* pToolBar = &m_wndToolBar;
    return ((pToolBar)->GetToolBarCtrl());
}
/******************************************************************************/
/*  Function Name    :  vSetFuncsTreeViewPtr
/*  Input(s)         :  CFnsTreeView* pFnsTreeView
/*  Output           :  void
/*  Functionality    :  Sets CFnsTreeView class pointer to member variable
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  14.03.2002
/*  Modifications    :
/******************************************************************************/
//void CMainFrame::vSetFuncsTreeViewPtr(CFnsTreeView* pFnsTreeView)
//{
//    m_pomFnsTreeView = pFnsTreeView;
//}
/******************************************************************************/
/*  Function Name    :  vSetFileViewPtr
/*  Input(s)         :  CFileView* pFnsTreeView
/*  Output           :  void
/*  Functionality    :  Sets CFileView class pointer to member variable
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  14.03.2002
/*  Modifications    :
/******************************************************************************/
//void CMainFrame::vSetFileViewPtr(CFileView* pFileView)
//{
//    m_pomFileView = pFileView;
//}
/******************************************************************************/
/*  Function Name    :  vSetFunctionViewPtr
/*  Input(s)         :  CFunctionView* pFnsTreeView
/*  Output           :  void
/*  Functionality    :  Sets CFunctionView class pointer to member variable
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  14.03.2002
/*  Modifications    :
/******************************************************************************/
//void CMainFrame::vSetFunctionViewPtr(CFunctionView* pFunctionView)
//{
//    m_pomFunctionView = pFunctionView;
//}
/******************************************************************************/
/*  Function Name    :  vGetNewDatabaseFlag
/*  Input(s)         :
/*  Output           :  BOOL
/*  Functionality    :  Returns the value of m_bIsNewDatabase
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  14.03.2002
/*  Modifications    :
/******************************************************************************/
BOOL CMainFrame::vGetNewDatabaseFlag()
{
    return (m_bIsNewDatabase);
}
/******************************************************************************/
/*  Function Name    :  vSetNewDatabaseFlag
/*  Input(s)         :  BOOL
/*  Output           :
/*  Functionality    :  Sets the value of m_bIsNewDatabase to "bValue"
                        passed as parameter
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  14.03.2002
/*  Modifications    :
/******************************************************************************/
void CMainFrame::vSetNewDatabaseFlag(BOOL bValue)
{
    m_bIsNewDatabase = bValue;
}
/******************************************************************************/
/*  Function Name    :  OnUpdateStatusBar                                     */
/*  Input(s)         :  "wpParam" contains the string to be displayed         */
/*  Output           :  baudrate                                              */
/*  Functionality    :  This function is by frame work when a WM_STATUSBAR    */
/*                      message is posted. It displayed on status bar text    */
/*                      passed a parameter "wpParam"                          */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.02.2002                                            */
/*  Modifications    :  Raja N on 07.09.2004                                  */
/*                      Added code to prevent error counter access in case of */
/*                      USB build                                             */
/*  Modifications    :  Raja N on 14.03.2005                                  */
/*                      Added code to extract channel information from wpParam*/
/*  Modifications    :  Anish kumar on 20.01.06,changed the function to refer */
/*                       Execute manager for executing handlers               */
/******************************************************************************/
LRESULT CMainFrame::OnErrorMessageProc(WPARAM wpParam, LPARAM lParam)
{
    TRACE(_("POSTED ERROR MSG"));
    SCAN_ERR ErrorMsg ;
    BOOL bToCallHandler = FALSE;
    eERROR_STATE eCurrError;
    // Get Channel Index
    UCHAR ucChannel = HIBYTE(wpParam);
    // Get Error code
    UCHAR ucError = LOBYTE( wpParam );
    // Check for channel ID validity to prevenmt array boundary condition
    // violation
    if( ucChannel >= defNO_OF_CHANNELS )
    {
        // Invalid channel index
        ASSERT( FALSE );
        // Set to default
        ucChannel = 0;
    }
    // If it is warning limit message then only invoke
    // warning limit handler
    if (ucError == ERROR_WARNING_LIMIT_REACHED)
    {
        bToCallHandler = TRUE;
        eCurrError = ERROR_WARNING_LIMIT;
    }
    // For all other error messages
    else
    {
        // Extract error counter value from the parameter
        BYTE byRxError = LOBYTE(lParam);
        BYTE byTxError = HIBYTE(lParam);

        // Find the state transition. This function will return true if
        // There is any state transition.
        bToCallHandler = bIsTransitionInState( ucChannel,
                                               byRxError, byTxError );

        // If it is Rx Error message then execure only error handler.
        // This will override the Passive error handler with error frame gandler
        if (ucError == STUFF_ERROR_RX)
        {
            eCurrError = ERROR_FRAME;
            bToCallHandler = TRUE;
        }
        else
        {
            // Set the latest state to select error hadler proc
            eCurrError = m_eCurrErrorState[ ucChannel ];
        }
    }

    // If user-defined error handling is enabled, call the suitable function
    // from the associated object here.
    // Execute user function only if bToCallHandler is set and user prog is
    // loaded
    if( bToCallHandler == TRUE )
    {
        // Check the status of error handler
        CFlags* pouFlag = theApp.pouGetFlagsPtr();
        if( pouFlag != NULL )
        {
            ErrorMsg.m_ucTxError = HIBYTE(lParam);
            ErrorMsg.m_ucRxError = LOBYTE(lParam);
            //channel number is one greater than the index number, index number starts from 0 and channel number starts from 1
            ErrorMsg.m_ucChannel = ucChannel + 1;
            //// Execute the error handler
            //CExecuteManager::ouGetExecuteManager().
            GetICANNodeSim()->NS_ManageOnErrorHandler(eCurrError,(void*)&ErrorMsg);
        }
    }
    return 0;
}

/******************************************************************************
    Function Name    :  bIsTransitionInState

    Input(s)         :  unChannel - Channel ID
                        byRxError - Rx Error counter value
                        byTxError - Tx Error counter value
    Output           :  TRUE if the function finds out analysing byRxError and
                        byTxError that
    Functionality    :  This analyses byRxError and byTxError to find out the
                        current error state. If it is different than what is
                        indicated currently with m_eCurrErrorState, this
                        returns TRUE, else FALSE.
    Member of        :  CMainFrame

    Author(s)        :  Ratnadip Choudhury
    Date Created     :
    Modifications    :  Raja N on 14.03.2005, Modified logic to support multi
                        channel controller state.
******************************************************************************/
BOOL CMainFrame::bIsTransitionInState( UINT unChannel,
                                       BYTE byRxError,
                                       BYTE byTxError )
{
    BOOL bIsTransition = FALSE;
    // Based on the value of transmit and receive error counter decide
    // the current error state

    if ((byTxError <= 127) && (byRxError <= 127))
    {
        // Error Active Mode
        if (m_eCurrErrorState[ unChannel ] != ERROR_ACTIVE)
        {
            bIsTransition = TRUE;
            m_eCurrErrorState[ unChannel ] = ERROR_ACTIVE;
        }
    }
    else if (byTxError == 255)
        // The sudden shift to the third state is to avoid
        // "else if ((byTxError > 127) || (byRxError > 127))"
    {
        // Bus off
        if (m_eCurrErrorState[ unChannel ] != ERROR_BUS_OFF)
        {
            bIsTransition = TRUE;
            m_eCurrErrorState[ unChannel ] = ERROR_BUS_OFF;
        }
    }
    else
    {
        // Error passive
        if (m_eCurrErrorState[ unChannel ] != ERROR_PASSIVE)
        {
            bIsTransition = TRUE;
            m_eCurrErrorState[ unChannel ] = ERROR_PASSIVE;
        }
    }

    return bIsTransition;
}
/******************************************************************************/
/*  Function Name    :  omStrConvertIntegerToHex                              */
/*  Input(s)         :  CString omStrInt                                      */
/*  Output           :  CString                                               */
/*  Functionality    :  This function will convert a integer passed as CString
                        to hexadecimal no.
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.03.2002                                            */
/*  Modifications    :  rajesh: 05.03.2003. Formating rewritten               */
/*  Modifications    :  Amitesh Bharti, 05.06.2003,replace datatype for       */
/*                      supporting 64bits to __int64                          */
/******************************************************************************/
CString CMainFrame::omStrConvertIntegerToHex(CString omStrInt)
{
    CString omStrHexNo = STR_EMPTY;
    int nCount = omStrInt.GetLength();

    int nIndex = omStrInt.Find('-');

    if ( nIndex != -1 )
    {
        // Delete "-" sign from the string
        omStrInt.Delete( nIndex, 1 );
        nCount--;
    }
    __int64 n64Val = _atoi64( omStrInt );
    __int64 n64Temp = 0;
    while ( nCount )
    {
        n64Temp = n64Val;
        // Get value
        n64Val = n64Val/16;
        // Get remainder
        n64Temp = n64Temp % 16;

        // Insert matching character
        switch(n64Temp)
        {
            case 0:
                omStrHexNo.Insert( 0, '0');
                break;
            case 1:
                omStrHexNo.Insert( 0, '1');
                break;
            case 2:
                omStrHexNo.Insert( 0, '2');
                break;
            case 3:
                omStrHexNo.Insert( 0, '3');
                break;
            case 4:
                omStrHexNo.Insert( 0, '4');
                break;
            case 5:
                omStrHexNo.Insert( 0, '5');
                break;
            case 6:
                omStrHexNo.Insert( 0, '6');
                break;
            case 7:
                omStrHexNo.Insert( 0, '7');
                break;
            case 8:
                omStrHexNo.Insert( 0, '8');
                break;
            case 9:
                omStrHexNo.Insert( 0, '9');
                break;
            case 10:
                omStrHexNo.Insert( 0, 'A');
                break;
            case 11:
                omStrHexNo.Insert( 0, 'B');
                break;
            case 12:
                omStrHexNo.Insert( 0, 'C');
                break;
            case 13:
                omStrHexNo.Insert( 0, 'D');
                break;
            case 14:
                omStrHexNo.Insert( 0, 'E');
                break;
            case 15:
                omStrHexNo.Insert( 0, 'F');
                break;
            default:
                break;
        }

        nCount--;
    }
    // If number is "-ve", insert '-' sign
    if ( nIndex != -1 )
    {
        omStrHexNo.Insert( 0, '-');
    }
    // u r done!
    return omStrHexNo;
}
/******************************************************************************
    Function Name    :  bIsHexNumber

    Input(s)         :  CString omStrHexNumber
    Output           :  BOOL
    Functionality    :  Checks whether the value passed as parameter is valid
                        Hexadecimal number. If yes, the function returns TRUE,
                        otherwise FALSE
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  10.03.2002
    Modifications    :
******************************************************************************/
BOOL CMainFrame::bIsHexNumber(CString omStrHexNumber)
{
    BOOL b_RetVal = FALSE;

    omStrHexNumber.MakeUpper();

    for ( int nCount = 0; nCount < omStrHexNumber.GetLength(); nCount++ )
    {
        char t_cChar = omStrHexNumber.GetAt(nCount);

        if ( ( t_cChar >= 'A' && t_cChar <= 'F' ) ||
                ( t_cChar >= '0' && t_cChar <= '9' ) ||
                ( t_cChar == '-') )
        {
            b_RetVal = TRUE;
        }
        else
        {
            b_RetVal = FALSE;
            break;
        }
    }

    return b_RetVal;
}
/******************************************************************************
    Function Name    :  OnUpdateConfigureDatabaseClose

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  Called by the framework to enable or disable
                        Database Close menu option.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  10.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateConfigureDatabaseClose(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ));
}
/******************************************************************************
    Function Name    :  OnUpdateConfigureDatabaseNew

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  Called by the framework to enable or disable
                        Database New menu option.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  10.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateConfigureDatabaseNew(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( !theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ));
}
/******************************************************************************
    Function Name    :  OnUpdateConfigureDatabaseSave

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  Called by the framework to enable or disable
                        Database Save menu option.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  10.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateConfigureDatabaseSave(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ));
}

/******************************************************************************
    Function Name    :  OnUpdateConfigureDatabaseSaveas

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  Called by the framework to enable or disable
                        Database SaveAs menu option.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  10.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateConfigureDatabaseSaveas(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ));
}
/******************************************************************************
    Function Name    :  OnClose

    Input(s)         :  void
    Output           :  void
    Functionality    :  Called by the framework when mainframe receives WM_CLOSE
                        message.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Ratnadip Choudhury
    Date Created     :  05.03.2002
    Modifications    :  27.11.2002, Gopi, Fixed bug. The window coords of Msg
                        window, Replay window and Output window were not
                        saved into the config file. Only the file name where
                        the current info can be saved is now stored in this
                        method. The file is actually saved in onDestroy(..)
                        of this class.
                        Store the coords of the output window and the replay
                        window here
    Modifications    :  29.11.2002, Amarnath S
                        For the first time after selecting the config filename
                        SaveConfiguration(..) is called.
                        Use of m_omstrConfigurationFile removed.
                        Registry is updated only if SaveConfig.. is successful
                        SaveMesageAttributes(...) function shifted to
                        OnImportDatabase().
    Modifications    :  08.05.2003, Amitesh Bharti
                        The file explorer to select a configuration file for
                        the default configuration will be thrown only if the
                        configuration setting is modified from default setting.
    Modifications    :  08.05.2003, Amitesh Bharti
                        The timer is killed it is has a valid handle. Also
                        the flag m_bIsStatWndCreated is reset to ensure if
                        application is closed it does not try to post a message.
                        to update the statistics since window may be closed.
    Modifications    :  10.03.2004, Raja N
                        OnCloseDatabase function is called to take actions if
                        editor is open and related code is removed from here.
    Modifications    :  05.04.2004, Raja N
                        Added window position update for Signal Watch Window and
                        Interpretation window.
    Modifications    :  02.05.2005, Raja N
                        Moved window save functionality to the function
                        vSaveWindowPostion and made a call to this function
    Modifications    :  02.05.2005, Raja N
                        Added code to stop logging if it is on
    Modifications    :  30.01.2007, Anish
                        Added code to terminate CUIThread & CGraphUIThread properly
    Modifications    :  20.05.2010, ArunKumar K
                        Added code to Save Toolbar Positions
    Modifications    :  Saravanan K S, 16-11-2011,
            Added Cancel button option in the close warning message box
******************************************************************************/
void CMainFrame::OnClose()
{
    CString oCfgFilename;
    vGetLoadedCfgFileName(oCfgFilename);
    bool bSaveConfig = true; // Used for node simuation closure operations
    // get the name of the loaded config file only if no filename has
    // been specified until now..
    if(oCfgFilename.IsEmpty() == TRUE && bIsConfigurationModified())
    {
        if(theApp.m_bFromAutomation == FALSE)
        {
            UINT unMsgRetVal = AfxMessageBox(_T(_("You have made changes to the configuration.\nDo you want to save it?")),
                                             MB_YESNOCANCEL);

            if ( unMsgRetVal == IDYES )
            {
                CFileDialog oCfgFileDlg(FALSE,   // Open dialog as Save as File dlg
                                        defFILEEXT,  // default extension
                                        NULL,       // default file name
                                        defDLGFLAGS, // mode
                                        _(defCONFIGFILTER), // filter
                                        NULL        // parent wnd
                                       );
                oCfgFileDlg.m_ofn.lpstrTitle = _T(_("Save Configuration File"));
                if(oCfgFileDlg.DoModal() == IDOK)
                {
                    // get the name of the selected file
                    oCfgFilename = oCfgFileDlg.GetPathName();
                    // This call alone is sufficient to save the
                    // configuration file.
                    vSetFileStorageInfo(oCfgFilename);
                    if (SaveConfiguration() ==
                            defCONFIG_FILE_SUCCESS )
                    {
                        bWriteIntoRegistry(HKEY_CURRENT_USER, _(SECTION), defCONFIGFILENAME, REG_SZ, oCfgFilename);
                    }
                    bSaveConfig = true;
                }
            }
            else if ( unMsgRetVal == IDCANCEL )
            {
                return;
            }
            else
            {
                bSaveConfig = false;
            }
        }
    }
    else
    {
        if(theApp.m_bFromAutomation == FALSE)
        {
            if ( bIsConfigurationModified())
            {
                UINT unMsgRetVal  = AfxMessageBox(_T(_("You have made changes to the configuration.\nDo you want to save it?")),
                                                  MB_YESNOCANCEL);
                if ( unMsgRetVal == IDYES )
                {
                    // This call alone is sufficient to save the
                    // configuration file.
                    vSetFileStorageInfo(oCfgFilename);
                    if ( SaveConfiguration() == defCONFIG_FILE_SUCCESS )
                    {
                        bWriteIntoRegistry(HKEY_CURRENT_USER, _(SECTION), defCONFIGFILENAME, REG_SZ, oCfgFilename);
                    }
                }
                else if ( unMsgRetVal == IDCANCEL )
                {
                    return;
                }
                else
                {
                    bSaveConfig = false;
                }
            }
        }
    }

    // Writing in to Registry
    bWriteIntoRegistry(HKEY_CURRENT_USER, _(SECTION), defCONFIGFILENAME, REG_SZ, oCfgFilename);

    vREP_HandleConnectionStatusChange( FALSE ); //Close reply

    OnDllUnload(); //Unload all the loaded dlls
    //GetICANNodeSim()->NS_SetSimSysConfigData(NULL, 0); // Reset SimSysConfig
    GetICANNodeSim()->NS_PerformApplicationClosureOperation(bSaveConfig);

    //Unload J1939 Node sim dll
    OnDllUnloadJ1939();
    //GetIJ1939NodeSim()->NS_SetSimSysConfigData(NULL, 0);
    GetIJ1939NodeSim()->NS_PerformApplicationClosureOperation(bSaveConfig);

    OnDllUnloadLIN();
    GetILINNodeSim()->NS_PerformApplicationClosureOperation(bSaveConfig);

    if(m_unTimerSB != 0)
    {
        ::KillTimer(NULL, m_unTimerSB);
    }
    // PTV [1.6.4]
    if(m_unTimerSBLog != 0)
    {
        ::KillTimer(NULL, m_unTimerSBLog);
        m_unTimerSBLog = NULL;
        // Update Status bar
        //m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
        m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
    }
    if(m_unJ1939TimerSBLog != 0)
    {
        ::KillTimer(NULL, m_unJ1939TimerSBLog);
        m_unJ1939TimerSBLog = NULL;
        // Update Status bar
        //m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
        m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_J1939_LOG_ICON, m_hLogOffIcon);
    }
    // PTV [1.6.4]
    if(m_podUIThread != NULL)
    {
        m_podUIThread->PostThreadMessage(WM_QUIT,0,0);
    }
    //SGW Code commented by Arun 21-10-2010
    /*if(m_pomGraphThread != NULL)
    {
        m_pomGraphThread->PostThreadMessage(WM_QUIT,0,0);
    }*/
    //SGW Code commented by Arun 21-10-2010
    m_bIsStatWndCreated = FALSE;
    m_bIsFlexRayStatWndCreated == FALSE;
    // Get appropriate data structure
    // Call Close Database to take app. action
    CFlags* pFlags = NULL;
    pFlags = theApp.pouGetFlagsPtr();
    if( pFlags != NULL)
    {
        // Close DB Editor if it is visible
        if( pFlags->nGetFlagStatus(DBOPEN) == TRUE )
        {
            OnCloseDatabase();
        }
    }

    // Stop Logging if it is enabled
    BOOL bLogON = FALSE;
    if (NULL != sg_pouFrameProcCAN)
    {
        bLogON = sg_pouFrameProcCAN->FPC_IsLoggingON();
    }
    if ( bLogON )
    {
        bLogON = bLogON ? FALSE : TRUE;
        vStartStopLogging( bLogON );
    }

    // Stop J1939 Logging if it is enabled
    if (NULL != sg_pouIJ1939Logger)
    {
        sg_pouIJ1939Logger->FPJ1_EnableLogging(FALSE);
    }



    if (NULL != sg_pouFrameProcLIN)
    {
        bLogON = sg_pouFrameProcLIN->FPL_IsLoggingON();
    }
    if ( bLogON )
    {
        bLogON = bLogON ? FALSE : TRUE;
        vStartStopLogging_LIN( bLogON );
    }



    if (g_pouDIL_CAN_Interface != NULL)
    {
        g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();
    }

    if ( g_pouDIL_FLEXRAY_Interface != NULL )
    {
        g_pouDIL_FLEXRAY_Interface->DILF_PerformClosureOperations();
    }

    //OnFlexRayConnect();
    vCloseFormatconverters();

    SaveBarState(PROFILE_CAN_MONITOR);

    //--------storing toolbar position--------------------------------------
    //string                path;
    //char              cBuffer[_MAX_PATH];
    //size_t                size;
    //CFileException        fileException;
    //CFileFind         finder;
    //xmlDocPtr         pXMLDocPtr = NULL;       /* document pointer */
    //xmlNodePtr            pRtNodePtr = NULL;
    //xmlDtdPtr         dtd = NULL;       /* DTD pointer */
    //xmlNodePtr            pToolBarPos;

    //// Get the working directory
    //CString           strPath;
    //char* pstrExePath = strPath.GetBuffer (MAX_PATH);
    //::GetModuleFileName (0, pstrExePath, MAX_PATH);
    //strPath.ReleaseBuffer ();
    //strPath = strPath.Left(strPath.ReverseFind(92));

    //path = strPath;
    //path.append("\\BUSMASTER_Init_Config.xml");           //get the file from current working folder
    //bool bFound = finder.FindFile((LPCTSTR)path.c_str());

    //CRect         rMainFrmRect;
    //GetWindowRect(&rMainFrmRect);
    ////get the file
    //if(bFound)
    //{
    //  //clear user defined nodes
    //  ClearUserDefinedNodes(path);

    //  pXMLDocPtr = xmlParseFile(path.c_str());
    //  xmlChar* pXpath = (xmlChar*)"//Busmaster_Init_Config/Toolbar_Position_User_Defined";
    //  xmlXPathObjectPtr pObjectPath = xmlUtils::pGetNodes(pXMLDocPtr, pXpath);

    //  if( NULL != pObjectPath )
    //  {
    //      xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
    //      if( NULL != pNodeSet )
    //      {
    //          pToolBarPos = pNodeSet->nodeTab[0];             //Take First One only
    //      }
    //  }
    //  else                                        //if this section doesnt exist then create it
    //  {
    //      if(pXMLDocPtr == NULL)
    //      {
    //          // Create the document with version 1.0
    //          pXMLDocPtr = xmlNewDoc(BAD_CAST "1.0");
    //      }

    //      xmlChar* pXpath = (xmlChar*)"//Busmaster_Init_Config";
    //      xmlXPathObjectPtr pObjectPath = xmlUtils::pGetNodes(pXMLDocPtr, pXpath);

    //      if(pObjectPath == NULL)                 //if root node doean't exist, then create it
    //      {
    //          // Creating the Root node
    //          pRtNodePtr = xmlNewNode(NULL, BAD_CAST DEF_BUSMASTER_INIT_CONFIG);
    //          xmlDocSetRootElement(pXMLDocPtr, pRtNodePtr);
    //      }
    //
    //      //create the user defined node
    //      pToolBarPos = xmlNewNode(NULL, BAD_CAST DEF_TOOLBAR_POS_USER_DEFINED    );
    //      xmlAddChild(pRtNodePtr, pToolBarPos);
    //  }
    //}
    //else      //if file not found, create a new one
    //{
    //  xmlKeepBlanksDefault(0);

    //  // Create the document with version 1.0
    //  pXMLDocPtr = xmlNewDoc(BAD_CAST "1.0");

    //  // Creating the Root node
    //  pRtNodePtr = xmlNewNode(NULL, BAD_CAST DEF_BUSMASTER_INIT_CONFIG);
    //  xmlDocSetRootElement(pXMLDocPtr, pRtNodePtr);

    //  //creating user define node
    //  pToolBarPos = xmlNewNode(NULL, BAD_CAST DEF_TOOLBAR_POS_USER_DEFINED    );
    //  xmlAddChild(pRtNodePtr, pToolBarPos);
    //}
    //
    //if(pToolBarPos != NULL)
    //{
    //  //store the positions of the toolbars in the file
    //  CreateToolBarPosInGlobalFile(pToolBarPos);
    //}

    //xmlIndentTreeOutput = 1;
    //xmlThrDefIndentTreeOutput(TRUE);

    //if(pXMLDocPtr != NULL)
    //{
    //  xmlSaveFormatFileEnc(path.c_str(), pXMLDocPtr, "UTF-8", 1);

    //  xmlFreeDoc(pXMLDocPtr);
    //}
    //

    // xmlCleanupParser();

    //----------------------------------------------------------------------
    if( NULL != m_pCopyBusStsticsNode )
    {
        xmlFreeNode(m_pCopyBusStsticsNode);
        m_pCopyBusStsticsNode = NULL;
    }

    CMDIFrameWnd::OnClose();
}

/**
* \brief         Writes a value under BUSMASTER registry share
* \param         void
* \return        true for SUCCESS, else FALSE
* \authors       Arunkumar Karri
* \date          07.09.2013 Created
*/
bool CMainFrame::bWriteIntoRegistry(HKEY hRootKey, CString strSubKey, CString strName,  BYTE bytType, CString strValue , DWORD dwValue)
{
    HKEY hKey;
    DWORD dwDisp = 0;
    LPDWORD lpdwDisp = &dwDisp;
    CString strCompleteSubKey;
    strCompleteSubKey.Format("Software\\RBEI-ETAS\\BUSMASTER_v%d.%d.%d\\%s",VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,strSubKey);

    LONG iSuccess = RegCreateKeyEx( HKEY_CURRENT_USER, strCompleteSubKey, 0L,NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,lpdwDisp);
    LSTATUS ls = 0;

    if(iSuccess == ERROR_SUCCESS)
    {
        if ( bytType == REG_SZ )
        {
            ls = RegSetValueEx (hKey, strName.GetBuffer(MAX_PATH), 0L, REG_SZ,(CONST BYTE*) strValue.GetBuffer(MAX_PATH), strValue.GetLength());
        }
        else if ( bytType == REG_DWORD )
        {
            ls = RegSetValueEx (hKey, strName.GetBuffer(MAX_PATH), 0L, REG_DWORD,(CONST BYTE*) &dwValue, sizeof(dwValue));
        }
        RegCloseKey(hKey);
        return true;
    }
    else
    {
        return false;
    }
}
/*******************************************************************************
  Function Name  : ClearUserDefinedNodes
  Input(s)       : string& - path to global file
  Output         : bool - failure/ success
  Description    : Clears all the child nodes from the user_defined_node
  Member of      : CMainFrame
  Functionality  : -
  Author(s)      : Ashwin R Uchil
  Date Created   : 6.5.2012
  Modifications  :
*******************************************************************************/
//bool CMainFrame::ClearUserDefinedNodes(string& strPath)
//{
//xmlNodePtr parentNode, NxtParentNode, NxtChildNode;
//xmlDocPtr xmlToolbarPosDoc = xmlParseFile(strPath.c_str());

////get the user defined section
//xmlChar* pXpath = (xmlChar*)"//Busmaster_Init_Config/Toolbar_Position_User_Defined";  //set the path
//xmlXPathObjectPtr pObjectPath = xmlUtils::pGetNodes(xmlToolbarPosDoc, pXpath);

//if( NULL != pObjectPath )
//{
//  xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
//  if( NULL != pNodeSet )
//  {
//      parentNode = pNodeSet->nodeTab[0];              //Take First One only
//  }
//  if( NULL != parentNode )
//  {
//      parentNode = parentNode->xmlChildrenNode;       //ToolBar_main or ToolBar_MsgWnd etc
//  }

//  //delete parent and their children nodes
//  while (parentNode != NULL)
//  {
//      xmlNodePtr childNode = parentNode->children;
//      while (childNode)
//      {
//          NxtChildNode = childNode->next;
//          xmlUnlinkNode(childNode);
//          xmlFree(childNode);
//          childNode = NxtChildNode;
//      }
//      //first save the next node only then delete the current node
//      NxtParentNode = parentNode->next;
//      xmlUnlinkNode(parentNode);
//      xmlFree(parentNode);
//      parentNode = NxtParentNode;
//  }
//  if(xmlToolbarPosDoc != NULL)
//  {
//      //save the file else the changes will not be reflected
//      xmlSaveFormatFileEnc(strPath.c_str(), xmlToolbarPosDoc, "UTF-8", 1);

//      xmlFreeDoc(xmlToolbarPosDoc);
//  }
//  return true;
//}
//else
//{
//  return false;
//}
//}

/*******************************************************************************
  Function Name  : CreateToolBarPosInGlobalFile
  Input(s)       : xmlNodePtr
  Output         : BOOL
  Description    : Creates nodes in global file to store the toolbar postions
  Member of      : CMainFrame
  Functionality  : -
  Author(s)      : Ashwin R Uchil
  Date Created   : 6.5.2012
  Modifications  :
*******************************************************************************/
//BOOL CMainFrame::CreateToolBarPosInGlobalFile(xmlNodePtr pNodePtr)
//{
//int           nToolBarHeight = 0;         //stores the maximum offset taht need to be subtracted to get the correct position
//string        strAlignementMain, strAlignementConfig,strAlignementMsgWin;
//string        strAlignementJ1939,strAlignementCANDB, strAlignementNodeSim;
//CRect     rTBRectMain,rTBRectMsgWdw, rTBRectConfig,rTBRectJ1939,rTBRectCANDB,rTBRectNodeSim;

//m_wndToolBar.GetWindowRect(&rTBRectMain);
////To align all the toolbars in proper position, we need to first get the alignment of all the toolbars and then
////calculate the exact position. In case of left or right alignment the toolbar will be pushed down when the
////toolbars at with top alignment are created, so the top postion of left aligned toolbar will not be stored as 0,
////we need to calculate the offset and subtract it.

//if(m_wndToolBar.IsFloating() == 0)        //do not calculate the offset in case of floating toolbars
//{
//  ScreenToClient(&rTBRectMain);
//
//  strAlignementMain = GetToolBarStyle( m_wndToolBar);
//  if(strAlignementMain == "TOP" )     //if the toolbar is TOP aligned then set the offset
//  {
//      nToolBarHeight = rTBRectMain.bottom;
//  }
//}

//m_wndToolbarMsgWnd.GetWindowRect(&rTBRectMsgWdw);
////m_wndToolbarMsgWnd.GetClientRect(&rTBRectMsgWdw);
//WINDOWPLACEMENT       wPlace;
//m_wndToolbarMsgWnd.GetWindowPlacement(&wPlace);
//if(m_wndToolbarMsgWnd.IsFloating() == 0)
//{
//  CPoint          ptP;
//  ptP.x = rTBRectMsgWdw.left;
//  ptP.y = rTBRectMsgWdw.top;
//  ::ScreenToClient(this->m_hWnd, &ptP);

//  ScreenToClient(&rTBRectMsgWdw);
//  strAlignementMsgWin = GetToolBarStyle( m_wndToolbarMsgWnd);
//  if(strAlignementMsgWin == "TOP" && rTBRectMsgWdw.bottom > nToolBarHeight) //get the max offset required
//  {
//      nToolBarHeight = rTBRectMsgWdw.bottom;
//  }
//}

//m_wndToolbarConfig.GetWindowRect(&rTBRectConfig);
//if(m_wndToolbarConfig.IsFloating() == 0)
//{
//  ScreenToClient(&rTBRectConfig);
//  strAlignementConfig = GetToolBarStyle( m_wndToolbarConfig);
//  if(strAlignementConfig == "TOP" && rTBRectConfig.bottom > nToolBarHeight)
//  {
//      nToolBarHeight = rTBRectConfig.bottom;
//  }
//}

//m_wndToolbarJ1939.GetWindowRect(&rTBRectJ1939);
//if(m_wndToolbarJ1939.IsFloating() == 0)
//{
//  ScreenToClient(&rTBRectJ1939);
//  strAlignementJ1939 = GetToolBarStyle( m_wndToolbarJ1939);
//  if(strAlignementJ1939 == "TOP" && rTBRectJ1939.bottom > nToolBarHeight)
//  {
//      nToolBarHeight = rTBRectJ1939.bottom;
//  }
//}

//m_wndToolbarCANDB.GetWindowRect(&rTBRectCANDB);
//if(m_wndToolbarCANDB.IsFloating() == 0)
//{
//  ScreenToClient(&rTBRectCANDB);
//  strAlignementCANDB = GetToolBarStyle( m_wndToolbarCANDB);
//  if(strAlignementCANDB == "TOP" && rTBRectCANDB.bottom > nToolBarHeight)
//  {
//      nToolBarHeight = rTBRectCANDB.bottom;
//  }
//}

//m_wndToolbarNodeSimul.GetWindowRect(&rTBRectNodeSim);
//if(m_wndToolbarNodeSimul.IsFloating() == 0)
//{
//  ScreenToClient(&rTBRectNodeSim);
//  strAlignementNodeSim = GetToolBarStyle( m_wndToolbarNodeSimul);
//  if(strAlignementNodeSim == "TOP" && rTBRectNodeSim.bottom > nToolBarHeight)
//  {
//      nToolBarHeight = rTBRectNodeSim.bottom;
//  }
//}

////=============================Main=====================================
//xmlNodePtr pToolBarMain = xmlNewNode(NULL, BAD_CAST DEF_TOOLBAR_MAIN);
//xmlAddChild(pNodePtr, pToolBarMain);
//
//CreationOfToolBarNode(pToolBarMain,DEF_NAME,"Main");
//if(strAlignementMain == "LEFT" || strAlignementMain == "RIGHT")
//{
//  //if right or left aligned then subtract the offset.
//  //its only for TOP position
//  CreationOfToolBarNode(pToolBarMain,DEF_TOP,rTBRectMain.top - nToolBarHeight);
//}
//else
//{
//  CreationOfToolBarNode(pToolBarMain,DEF_TOP,rTBRectMain.top);
//}
//CreationOfToolBarNode(pToolBarMain,DEF_Left,rTBRectMain.left);
//CreationOfToolBarNode(pToolBarMain,DEF_Bottom,rTBRectMain.bottom);
//CreationOfToolBarNode(pToolBarMain,DEF_Right,rTBRectMain.right);
//
//CreationOfToolBarNode(pToolBarMain,DEF_TOOLBAR_ALIGNMENT,strAlignementMain);
//if(m_wndToolBar.IsFloating())
//  CreationOfToolBarNode(pToolBarMain,DEF_TOOLBAR_DOCKING,0);
//else
//  CreationOfToolBarNode(pToolBarMain,DEF_TOOLBAR_DOCKING,1);
//CreationOfToolBarNode(pToolBarMain,DEF_IS_VISIBLE,m_wndToolBar.IsWindowVisible());
////=======================================================================

////=====================Message window toolbar============================
//xmlNodePtr pMsgWnd = xmlNewNode(NULL, BAD_CAST DEF_TOOLBAR_MSG_WND);
//xmlAddChild(pNodePtr, pMsgWnd);

//CreationOfToolBarNode(pMsgWnd,DEF_NAME,"Message Window");
//if(strAlignementMsgWin == "LEFT" || strAlignementMsgWin == "RIGHT")
//{
//  //if right or left aligned then subtract the offset.
//  //its only for TOP position
//  CreationOfToolBarNode(pMsgWnd,DEF_TOP,rTBRectMsgWdw.top - nToolBarHeight);
//}
//else
//{
//  CreationOfToolBarNode(pMsgWnd,DEF_TOP,rTBRectMsgWdw.top);
//}
//CreationOfToolBarNode(pMsgWnd,DEF_Left,rTBRectMsgWdw.left);
//CreationOfToolBarNode(pMsgWnd,DEF_Bottom,rTBRectMsgWdw.bottom);
//CreationOfToolBarNode(pMsgWnd,DEF_Right,rTBRectMsgWdw.right);
//
//CreationOfToolBarNode(pMsgWnd,DEF_TOOLBAR_ALIGNMENT,strAlignementMsgWin);
//if(m_wndToolbarMsgWnd.IsFloating())
//  CreationOfToolBarNode(pMsgWnd,DEF_TOOLBAR_DOCKING,0);
//else
//  CreationOfToolBarNode(pMsgWnd,DEF_TOOLBAR_DOCKING,1);
//CreationOfToolBarNode(pMsgWnd,DEF_IS_VISIBLE,m_wndToolbarMsgWnd.IsWindowVisible());
////========================================================================

////=======================Configuration toolbar ===========================
//xmlNodePtr pConfigure = xmlNewNode(NULL, BAD_CAST DEF_TOOLBAR_CONFIG);
//xmlAddChild(pNodePtr, pConfigure);


//CreationOfToolBarNode(pConfigure,DEF_NAME,"Configure");
//if(strAlignementConfig == "LEFT" || strAlignementConfig == "RIGHT")
//{
//  //if right or left aligned then subtract the offset.
//  //its only for TOP position
//  CreationOfToolBarNode(pConfigure,DEF_TOP,rTBRectConfig.top - nToolBarHeight);
//}
//else
//{
//  CreationOfToolBarNode(pConfigure,DEF_TOP,rTBRectConfig.top);
//}
//CreationOfToolBarNode(pConfigure,DEF_Left,rTBRectConfig.left);
//CreationOfToolBarNode(pConfigure,DEF_Bottom,rTBRectConfig.bottom);
//CreationOfToolBarNode(pConfigure,DEF_Right,rTBRectConfig.right);
//
//CreationOfToolBarNode(pConfigure,DEF_TOOLBAR_ALIGNMENT,strAlignementConfig);
//if(m_wndToolbarConfig.IsFloating())
//  CreationOfToolBarNode(pConfigure,DEF_TOOLBAR_DOCKING,0);
//else
//  CreationOfToolBarNode(pConfigure,DEF_TOOLBAR_DOCKING,1);
//CreationOfToolBarNode(pConfigure,DEF_IS_VISIBLE,m_wndToolbarConfig.IsWindowVisible());
////=======================================================================

////==========================J1939 Toolbar================================
//xmlNodePtr pJ1939 = xmlNewNode(NULL, BAD_CAST DEF_TOOLBAR_J1939);
//xmlAddChild(pNodePtr, pJ1939);

//CreationOfToolBarNode(pJ1939,DEF_NAME,"J1939");
//if(strAlignementJ1939 == "LEFT" || strAlignementJ1939 == "RIGHT")
//{
//  //if right or left aligned then subtract the offset.
//  //its only for TOP position
//  CreationOfToolBarNode(pJ1939,DEF_TOP,rTBRectJ1939.top - nToolBarHeight);
//}
//else
//{
//  CreationOfToolBarNode(pJ1939,DEF_TOP,rTBRectJ1939.top);
//}
//CreationOfToolBarNode(pJ1939,DEF_Left,rTBRectJ1939.left);
//CreationOfToolBarNode(pJ1939,DEF_Bottom,rTBRectJ1939.bottom);
//CreationOfToolBarNode(pJ1939,DEF_Right,rTBRectJ1939.right);

//CreationOfToolBarNode(pJ1939,DEF_TOOLBAR_ALIGNMENT,strAlignementJ1939);
//if(m_wndToolbarJ1939.IsFloating())
//  CreationOfToolBarNode(pJ1939,DEF_TOOLBAR_DOCKING,0);
//else
//  CreationOfToolBarNode(pJ1939,DEF_TOOLBAR_DOCKING,1);
//CreationOfToolBarNode(pJ1939,DEF_IS_VISIBLE,m_wndToolbarJ1939.IsWindowVisible());
////======================================================================

////=======================CAN database Toolbar===========================
//xmlNodePtr pCANDB = xmlNewNode(NULL, BAD_CAST DEF_TOOLBAR_CAN_DB);
//xmlAddChild(pNodePtr, pCANDB);


//CreationOfToolBarNode(pCANDB,DEF_NAME,"CAN Database");
//if(strAlignementCANDB == "LEFT" || strAlignementCANDB == "RIGHT")
//{
//  //if right or left aligned then subtract the offset.
//  //its only for TOP position
//  CreationOfToolBarNode(pCANDB,DEF_TOP,rTBRectCANDB.top - nToolBarHeight);
//}
//else
//{
//  CreationOfToolBarNode(pCANDB,DEF_TOP,rTBRectCANDB.top);
//}
//CreationOfToolBarNode(pCANDB,DEF_Left,rTBRectCANDB.left);
//CreationOfToolBarNode(pCANDB,DEF_Bottom,rTBRectCANDB.bottom);
//CreationOfToolBarNode(pCANDB,DEF_Right,rTBRectCANDB.right);
//
//CreationOfToolBarNode(pCANDB,DEF_TOOLBAR_ALIGNMENT,strAlignementCANDB);
//if(m_wndToolbarCANDB.IsFloating())
//  CreationOfToolBarNode(pCANDB,DEF_TOOLBAR_DOCKING,0);
//else
//  CreationOfToolBarNode(pCANDB,DEF_TOOLBAR_DOCKING,1);
//CreationOfToolBarNode(pCANDB,DEF_IS_VISIBLE,m_wndToolbarCANDB.IsWindowVisible());
////======================================================================

////========================Node Simulation===============================
//xmlNodePtr pNodeSim = xmlNewNode(NULL, BAD_CAST DEF_TOOLBAR_NODE_SIM);
//xmlAddChild(pNodePtr, pNodeSim);


//CreationOfToolBarNode(pNodeSim,DEF_NAME,"Node Simulation");
//if(strAlignementNodeSim == "LEFT" || strAlignementNodeSim == "RIGHT" /*||m_wndToolbarNodeSimul.IsFloating()*/)
//{
//  //if right or left aligned then subtract the offset.
//  //its only for TOP position
//  CreationOfToolBarNode(pNodeSim,DEF_TOP,rTBRectNodeSim.top - nToolBarHeight);
//}
//else
//{
//  CreationOfToolBarNode(pNodeSim,DEF_TOP,rTBRectNodeSim.top);
//}

//CreationOfToolBarNode(pNodeSim,DEF_Left,rTBRectNodeSim.left);
//CreationOfToolBarNode(pNodeSim,DEF_Bottom,rTBRectNodeSim.bottom);
//CreationOfToolBarNode(pNodeSim,DEF_Right,rTBRectNodeSim.right);
//
//CreationOfToolBarNode(pNodeSim,DEF_TOOLBAR_ALIGNMENT,strAlignementNodeSim);
//if(m_wndToolbarNodeSimul.IsFloating())
//  CreationOfToolBarNode(pNodeSim,DEF_TOOLBAR_DOCKING,0);
//else
//  CreationOfToolBarNode(pNodeSim,DEF_TOOLBAR_DOCKING,1);
//CreationOfToolBarNode(pNodeSim,DEF_IS_VISIBLE,m_wndToolbarNodeSimul.IsWindowVisible());
////======================================================================
//return true;
//}

/*******************************************************************************
  Function Name  : GetToolBarStyle
  Input(s)       : CToolBar object
  Output         : string containing the name
  Description    : Gets the current toolbar style(Docked side like TOP, LEFT, RIGHT, BOTTOM)
  Member of      : CMainFrame
  Functionality  : -
  Author(s)      : Ashwin R Uchil
  Date Created   : 6.5.2012
  Modifications  :
*******************************************************************************/
//string CMainFrame::GetToolBarStyle(CToolBar& wndToolbar)
//{
//DWORD dwStyle = wndToolbar.GetBarStyle();         //get the current style of toolbar
//UINT unBarID = 0;
//string            strStyle;
////get the current alignment
//   unBarID = (dwStyle & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : unBarID;
//   unBarID = (dwStyle & CBRS_ALIGN_BOTTOM && unBarID == 0) ? AFX_IDW_DOCKBAR_BOTTOM : unBarID;
//   unBarID = (dwStyle & CBRS_ALIGN_LEFT && unBarID == 0) ? AFX_IDW_DOCKBAR_LEFT : unBarID;
//   unBarID = (dwStyle & CBRS_ALIGN_RIGHT && unBarID == 0) ? AFX_IDW_DOCKBAR_RIGHT : unBarID;

//if(unBarID == AFX_IDW_DOCKBAR_TOP)
//{
//  strStyle = "TOP";
//}
//else if(unBarID == AFX_IDW_DOCKBAR_BOTTOM)
//{
//  strStyle = "BOTTOM";
//}
//else if(unBarID == AFX_IDW_DOCKBAR_LEFT)
//{
//  strStyle = "LEFT";
//}
//else if(unBarID == AFX_IDW_DOCKBAR_RIGHT)
//{
//  strStyle = "RIGHT";
//}
//return strStyle;
//}
/*******************************************************************************
  Function Name  : CreationOfToolBarNode
  Input(s)       : xmlNodePtr   - parent node for which child can be created
                   string       - contains the name of the node
                   string       - data in the node
  Output         : BOOL - success/ failure
  Description    : creates new child node for the given node with the tag and data
  Member of      : CMainFrame
  Functionality  : -
  Author(s)      : Ashwin R Uchil
  Date Created   : 6.5.2012
  Modifications  :
*******************************************************************************/
//BOOL CMainFrame::CreationOfToolBarNode(xmlNodePtr pNodePtr, string strTag, string strData)
//{
/*const char *omcVarChar ;
omcVarChar = strData.c_str();
xmlNodePtr pNode = xmlNewChild(pNodePtr, NULL, BAD_CAST strTag.c_str(),BAD_CAST omcVarChar);
xmlAddChild(pNodePtr, pNode);
return true;*/
//}
/*******************************************************************************
  Function Name  : CreationOfToolBarNode
  Input(s)       : xmlNodePtr   - parent node for which child can be created
                   string       - contains the name of the node
                   int          - data in the node
  Output         : BOOL - success/ failure
  Description    : creates new child node for the given node with the tag and data
  Member of      : CMainFrame
  Functionality  : -
  Author(s)      : Ashwin R Uchil
  Date Created   : 6.5.2012
  Modifications  :
*******************************************************************************/

//BOOL CMainFrame::CreationOfToolBarNode(xmlNodePtr pNodePtr, string strTag, int nData)
//{
/*const char *omcVarChar ;
CString         csData;
csData.Format("%d", nData);
omcVarChar = csData.GetBuffer(0);
xmlNodePtr pNode = xmlNewChild(pNodePtr, NULL, BAD_CAST strTag.c_str(),BAD_CAST omcVarChar);
xmlAddChild(pNodePtr, pNode);
return true;*/
//}
/******************************************************************************
    Function Name    :  bCreateMsgWindow

    Input(s)         :      -
    Output           :  BOOL
    Functionality    :  Creates the message window (an object of
                        CMsgMDIChildWnd class)
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Ratnadip Choudhury
    Date Created     :  05.03.2002
    Modifications    :
                        Arunkumar K on 15.06.2010, New Message Window Thread
                        Raja N on 19.07.2005, Removed code related to log file
                        triggering
******************************************************************************/

BOOL CMainFrame::bCreateMsgWindow()
{
    BOOL bReturn=TRUE;
    if (m_podMsgWndThread == NULL)
    {
        m_podMsgWndThread = new CMsgWndThread;
        if (m_podMsgWndThread != NULL)
        {
            // Set specific parameters on successful creation

            m_podMsgWndThread->CreateThread();
            //CAN Message Window
            m_podMsgWndThread->CreateMsgWnd(m_hWnd, CAN,
                                            0, NULL);
            m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_SHOW, (LONG)CAN);

            //LIN Message Window
            m_podMsgWndThread->CreateMsgWnd(m_hWnd, LIN,
                                            0, NULL);
            m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                          WM_NOTIFICATION_FROM_OTHER,
                          eLOAD_DATABASE,
                          (LPARAM)&(m_ouClusterConfig[LIN]));

            //J1939 Message Window
            m_podMsgWndThread->CreateMsgWnd(m_hWnd, J1939,
                                            0, NULL);
            m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)J1939);
            bCreateFlexRayMsgWindow();
        }
        else
        {
            bReturn = FALSE;
        }
    }
    else
    {
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_SHOW, (LONG)CAN);
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)J1939);
    }

    return TRUE;
}

/******************************************************************************
    Function Name    :  OnHex_DecButon

    Input(s)         :      -
    Output           :  void
    Functionality    :  Toggles hexadecimal button and switches between decimal
                        state to hexadecimal state
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Ratnadip Choudhury
    Date Created     :  15.03.2002
    Modifications    :  Raja N on 26.04.2005, modified code to refer new Tx Msg
                        configuration window for posting message about numeric
                        mode change
******************************************************************************/
void CMainFrame::OnHex_DecButon()
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        if (IS_NUM_HEX_SET(byGetDispFlag))
        {
            CLEAR_EXPR_NUM_BITS(byGetDispFlag);
            SET_NUM_DEC(byGetDispFlag);
            theApp.pouGetFlagsPtr()->vSetFlagStatus(HEX, FALSE);
        }
        else
        {
            CLEAR_EXPR_NUM_BITS(byGetDispFlag);
            SET_NUM_HEX(byGetDispFlag);
            theApp.pouGetFlagsPtr()->vSetFlagStatus(HEX, TRUE);
        }

        hWnd = m_podMsgWndThread->hGetHandleMsgWnd(LIN);
        byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        if (IS_NUM_HEX_SET(byGetDispFlag))
        {
            CLEAR_EXPR_NUM_BITS(byGetDispFlag);
            SET_NUM_DEC(byGetDispFlag);
            theApp.pouGetFlagsPtr()->vSetFlagStatus(HEX, FALSE);
        }
        else
        {
            CLEAR_EXPR_NUM_BITS(byGetDispFlag);
            SET_NUM_HEX(byGetDispFlag);
            theApp.pouGetFlagsPtr()->vSetFlagStatus(HEX, TRUE);
        }

        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
            //Update Message Window
            if(hWnd)
            {
                BYTE bModes = NUMERIC;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }

    BOOL bHexON = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);
    if(m_objTxHandler.hConfigWindowShown() == S_OK)
    {
        eUSERSELCTION eUserSel = eHEXDECCMD;
        m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bHexON);
    }
    m_objFlexTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eHEXDECCMD, bHexON);
    m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eHEXDECCMD, bHexON);
    if (sg_pouSWInterface[CAN] != NULL)
    {
        sg_pouSWInterface[CAN]->SW_SetDisplayMode(bHexON);
    }
    if (sg_pouSWInterface[J1939] != NULL)
    {
        sg_pouSWInterface[J1939]->SW_SetDisplayMode(bHexON);
    }
    if (sg_pouSWInterface[LIN] != NULL)
    {
        sg_pouSWInterface[LIN]->SW_SetDisplayMode(bHexON);
    }
}

/**
* \brief         Enabling or disabling Hex/Numeric mode
* \param[in]     bHexEnabled is bool variable, if true Hex mode is enabled
* \return        void
* \authors       Prathiba P
* \date          06.18.2013 Created
*/
void CMainFrame::bSetHexDecFlags(BOOL bHexEnabled)
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        if (bHexEnabled == FALSE)
        {
            CLEAR_EXPR_NUM_BITS(byGetDispFlag);
            SET_NUM_DEC(byGetDispFlag);
            theApp.pouGetFlagsPtr()->vSetFlagStatus(HEX, FALSE);
        }
        else
        {
            CLEAR_EXPR_NUM_BITS(byGetDispFlag);
            SET_NUM_HEX(byGetDispFlag);
            theApp.pouGetFlagsPtr()->vSetFlagStatus(HEX, TRUE);
        }
        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
            //Update Message Window
            if(hWnd)
            {
                BYTE bModes = NUMERIC;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }

    BOOL bHexON = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);
    if(m_objTxHandler.hConfigWindowShown() == S_OK)
    {
        eUSERSELCTION eUserSel = eHEXDECCMD;
        m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bHexON);
    }
    //Flexray Window.
    m_objFlexTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eHEXDECCMD, bHexON);


    if (sg_pouSWInterface[CAN] != NULL)
    {
        sg_pouSWInterface[CAN]->SW_SetDisplayMode(bHexON);
    }
    if (sg_pouSWInterface[J1939] != NULL)
    {
        sg_pouSWInterface[J1939]->SW_SetDisplayMode(bHexON);
    }
}
/**
 * \brief Display Message window Overwrite
 * \req RS_16_01 - Two display modes namely, overwrite and append shall be supported.
 *
 * Toggles overwrite button and switches between
 * overwriting state to appending state.
 */
void CMainFrame::ApplyMessagewindowOverwrite()
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);

        BOOL bIsOverwrite = FALSE;
        bIsOverwrite = theApp.pouGetFlagsPtr()->nGetFlagStatus(OVERWRITE);
        if (bIsOverwrite == TRUE)
        {
            CLEAR_EXPR_DISP_BITS(byGetDispFlag);

            //Verifying the Interpretation Check condition.
            CStringArray aomstrTempDBFiles;
            theApp.m_pouMsgSignal->vGetDataBaseNames(&aomstrTempDBFiles);
            if (!(aomstrTempDBFiles.GetSize() > 0))
            {
                m_bInterPretMsg = FALSE;
            }
            if (m_bInterPretMsg)
            {
                SET_MODE_INTRP(byGetDispFlag);
            }
            else
            {
                SET_MODE_OVER(byGetDispFlag);
            }
            theApp.pouGetFlagsPtr()->vSetFlagStatus(OVERWRITE, TRUE);
        }
        else
        {
            CLEAR_EXPR_DISP_BITS(byGetDispFlag);
            SET_MODE_APPEND(byGetDispFlag);
            theApp.pouGetFlagsPtr()->vSetFlagStatus(OVERWRITE, FALSE);
        }
        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
            //Update Message Window
            if(hWnd)
            {
                BYTE bModes = DISPLAY_MODE;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }
    /* BOOL bOverwriteON = theApp.pouGetFlagsPtr()->nGetFlagStatus(OVERWRITE);
     bOverwriteON = bOverwriteON ? FALSE : TRUE;
     theApp.pouGetFlagsPtr()->vSetFlagStatus(OVERWRITE, bOverwriteON);
     PostThreadMessage(GUI_dwThread_MsgDisp, TM_OVERWRITE_MESSAGE, bOverwriteON,
         0);*/
}
/**
 * \brief Display Message window Overwrite
 * \req RS_16_01 - Two display modes namely, overwrite and append shall be supported.
 *
 * Toggles overwrite button and switches between
 * overwriting state to appending state.
 */
void CMainFrame::OnDisplayMessagewindowOverwrite()
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);



        if (IS_MODE_APPEND(byGetDispFlag))
        {
            CLEAR_EXPR_DISP_BITS(byGetDispFlag);

            //Verifying the Interpretation Check condition.
            CStringArray aomstrTempDBFiles;
            theApp.m_pouMsgSignal->vGetDataBaseNames(&aomstrTempDBFiles);
            if (!(aomstrTempDBFiles.GetSize() > 0))
            {
                m_bInterPretMsg = FALSE;
            }
            if (m_bInterPretMsg)
            {
                SET_MODE_INTRP(byGetDispFlag);
            }
            else
            {
                SET_MODE_OVER(byGetDispFlag);
            }
            theApp.pouGetFlagsPtr()->vSetFlagStatus(OVERWRITE, TRUE);
        }
        else
        {
            CLEAR_EXPR_DISP_BITS(byGetDispFlag);
            SET_MODE_APPEND(byGetDispFlag);
            theApp.pouGetFlagsPtr()->vSetFlagStatus(OVERWRITE, FALSE);
        }
        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
            //Update Message Window
            if(hWnd)
            {
                BYTE bModes = DISPLAY_MODE;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }
    /* BOOL bOverwriteON = theApp.pouGetFlagsPtr()->nGetFlagStatus(OVERWRITE);
     bOverwriteON = bOverwriteON ? FALSE : TRUE;
     theApp.pouGetFlagsPtr()->vSetFlagStatus(OVERWRITE, bOverwriteON);
     PostThreadMessage(GUI_dwThread_MsgDisp, TM_OVERWRITE_MESSAGE, bOverwriteON,
         0);*/
}


/******************************************************************************
    Function Name    :  bSetPressStatus

    Input(s)         :  int nButtonID, eCANMONITORFLAG eFlag
    Output           :  BOOL
    Functionality    :  Retrieves status of eFlag from global CFlags object, a
                        placeholder for all main program state flags and
                        modifies press status of nButtonID accordingly.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Ratnadip Choudhury
    Date Created     :  15.03.2002
    Modifications    :
******************************************************************************/
BOOL CMainFrame::bSetPressStatus(int nButtonID, eCANMONITORFLAG eFlag)
{
    // Get reference to Tool Bar ctrl
    BOOL bStatus = theApp.pouGetFlagsPtr()->nGetFlagStatus(eFlag);
    CToolBarCtrl& omRefToolBarCtrl = vGetReferenceToToolBarCtrl();
    return omRefToolBarCtrl.PressButton(nButtonID, bStatus);
}

/******************************************************************************
    Function Name    :  OnUpdateMessageInterpret

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Checks or Unchecks the Interpretation menu option
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  20.03.2002
    Modifications    :  Amitesh Bharti
                        18.06.2003, Uncheck the toolbar when it is disable.
    Modifications    :  Ratnadip Choudhury
                        10.01.2004, toolbar is only pressed/unpressed.
    Modifications    :  Raja N
                        23.06.2004, Disabled the button in Append mode to
                        prevent interpretation in Append mode
******************************************************************************/
void CMainFrame::OnUpdateMessageInterpret(CCmdUI* pCmdUI)
{
    BYTE byGetDispFlag = 0;
    if (m_podMsgWndThread != NULL)
    {
        //First check if any DB file is present or not
        //Store database file names
        CStringArray aomstrTempDBFilesCAN;
        theApp.m_pouMsgSignal->vGetDataBaseNames(&aomstrTempDBFilesCAN);

        CStringArray aomstrTempDBFilesJ1939;
        if(m_pouMsgSigJ1939)
        {
            m_pouMsgSigJ1939->vGetDataBaseNames(&aomstrTempDBFilesJ1939);
        }

        if (aomstrTempDBFilesCAN.GetSize() > 0 ||
                aomstrTempDBFilesJ1939.GetSize() > 0 ||
                m_ouClusterConfig[FLEXRAY].m_nChannelsConfigured > 0||
                m_ouClusterConfig[LIN].m_nChannelsConfigured > 0)
            //!m_acFlexDBConfigInfo.m_acConfigFileName[0].empty() )
        {
            //If fil epresent then check for other status
            HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
            ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
            if (IS_MODE_INTRP(byGetDispFlag))
            {
                pCmdUI->Enable(TRUE);   // Enabled
                pCmdUI->SetCheck(TRUE); // Pressed state
            }
            else if (IS_MODE_APPEND(byGetDispFlag))
            {
                // Disable the button
                pCmdUI->Enable(FALSE);  // Disabled
                pCmdUI->SetCheck(m_bInterPretMsg); // Retain pressed state
            }
            else if (IS_MODE_OVER(byGetDispFlag))
            {
                // Enable the button
                pCmdUI->Enable(TRUE);   // Enabled
                pCmdUI->SetCheck(FALSE);// Unpressed
            }
        }
        else
        {
            pCmdUI->Enable(FALSE);   // disable
            pCmdUI->SetCheck(FALSE); // UnPressed state
        }
    }
}
/******************************************************************************
    Function Name    :  OnUpdateExecuteMessagehandlers

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Enable or disable execute message handler
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modification By  :  Amitesh Bharti
    Modification on  :  23.10.2002,Enable menu/button if tool is connected
    Modification By  :  Amitesh Bharti
    Modification on  :  31.10.2002,Review comment incorporated.
/******************************************************************************/
void CMainFrame::OnUpdateExecuteMessagehandlers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetICANNodeSim()->NS_GetHandlerStatus(H_MSGHANDLERBUTTON));
    BOOL bEnable = GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

/******************************************************************************
    Function Name    :  OnUpdateExecuteMessagehandlers

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Enable or disable execute message handler
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modification By  :  Amitesh Bharti
    Modification on  :  23.10.2002,Enable menu/button if tool is connected
    Modification By  :  Amitesh Bharti
    Modification on  :  31.10.2002,Review comment incorporated.
/******************************************************************************/
void CMainFrame::OnUpdateExecuteMessagehandlersLIN(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetILINNodeSim()->NS_GetHandlerStatus(H_MSGHANDLERBUTTON));
    BOOL bEnable = GetILINNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}
/******************************************************************************
    Function Name    :  OnUpdateLogFilter

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Check or uncheck log filter
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :  Amitesh Bharti, 26.12.2002, File name is changed
******************************************************************************/
void CMainFrame::OnUpdateLogFilter(CCmdUI* pCmdUI)
{
    // Check or uncheck log filter
    // menu item
    pCmdUI->SetCheck(
        theApp.pouGetFlagsPtr()->nGetFlagStatus( LOGFILTER ) );
}
void CMainFrame::OnUpdateReplayFilter(CCmdUI* pCmdUI)
{
    // Check or uncheck replay filter
    // menu item
    pCmdUI->SetCheck(
        theApp.pouGetFlagsPtr()->nGetFlagStatus( REPLAYFILTER ) );
}
/******************************************************************************
    Function Name    :  OnUpdateMessageFilter

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Check or uncheck message filter
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :  Amitesh Bharti, 26.12.2002, File name is changed
******************************************************************************/
//void CMainFrame::OnUpdateMessageFilter(CCmdUI* pCmdUI)
//{
//    // Check or uncheck message filter
//    // menu item
//    pCmdUI->SetCheck(
//        theApp.pouGetFlagsPtr()->nGetFlagStatus( DISPLAYFILTERON ) );
//}
/******************************************************************************
    Function Name    :  OnUpdateExecuteMessagehandlersButton

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Toggle execute message handler button status
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modification By  :  Amitesh Bharti
    Modification on  :  23.10.2002,Enable menu/button if tool is connected
    Modification By  :  Amitesh Bharti
    Modification on  :  31.10.2002, Review comment incorporated.
/******************************************************************************/
void CMainFrame::OnUpdateExecuteMessagehandlersButton(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetICANNodeSim()->NS_GetHandlerStatus(H_MSGHANDLERBUTTON));
    BOOL bEnable = GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}
/******************************************************************************
    Function Name    :  OnExecuteMessagehandlersButton

    Input(s)         :      -
    Output           :      -
    Functionality    :  Calls OnActivateMessageHandler function
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnExecuteMessagehandlersButton()
{
    //Get present status
    BOOL bEnable = GetICANNodeSim()->NS_GetHandlerStatus(H_MSGHANDLERBUTTON);
    //Change the status of handler
    GetICANNodeSim()->NS_EnableAllMessageHandler( !bEnable );
}
/******************************************************************************
    Function Name    :  OnLogFilterButton

    Input(s)         :      -
    Output           :      -
    Functionality    :  Calls OnFilterLog function
    Member of        :  CMainFrame
    Friend of        :

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :  Amitesh Bharti, 26.12.2002, File name is changed
******************************************************************************/
//void CMainFrame::OnLogFilterButton()
//{
//    OnLogFilter();
//}
/******************************************************************************
    Function Name    :  OnUpdateLogFilterButton

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Toggle log filter button status
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :
******************************************************************************/
//void CMainFrame::OnUpdateLogFilterButton(CCmdUI* pCmdUI)
//{
//    // Toggle log filter button status
//    pCmdUI->SetCheck(theApp.pouGetFlagsPtr()->nGetFlagStatus(LOGFILTER));
//}
/******************************************************************************
    Function Name    :  OnMessageFilterButton

    Input(s)         :      -
    Output           :      -
    Functionality    :  Calls OnActivateTimerHandler function
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modified by      :  Ratnadip Choudhury
    Modifications    :  Added code to post filter message to display thread
    Date Modified    :  27.03.2002
    Modified by      :  Amitesh Bharti
    Modifications    :  Function name is changed and CFlag pointer is validated.
    Date Modified    :  26.12.2002
******************************************************************************/
void CMainFrame::OnMessageFilterButton()
{
    CFlags* pouFlags = NULL;
    BOOL bMessageFilterStatus = FALSE;

    pouFlags = theApp.pouGetFlagsPtr();
    if(pouFlags != NULL )
    {
        bMessageFilterStatus = pouFlags->nGetFlagStatus(DISPLAYFILTERON);
        bMessageFilterStatus = bMessageFilterStatus ? FALSE : TRUE;
        pouFlags->vSetFlagStatus(DISPLAYFILTERON, bMessageFilterStatus);

        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_ENABLE_FILTER_APPLIED, (WPARAM)bMessageFilterStatus, NULL);
        /* Modify filter icon accordingly in Main toolbar*/
        BYTE bytTbrItemIndex = 6;
        vModifyToolbarIcon( m_wndToolBar, bytTbrItemIndex, bMessageFilterStatus, IDI_ICON_MSG_FILTER_ON, IDI_ICON_MSG_FILTER );
    }
}

/**
* \brief         Handles the J1939 tx window close notification
* \param[in]     WPARAM, LPARAM
* \return        LRESULT
* \authors       Arunkumar Karri
* \date          28.02.2013 Created
*/
LRESULT CMainFrame::onJ1939TxWndClose(WPARAM wParam, LPARAM lParam)
{
    /* Modify filter icon accordingly in J1939 toolbar*/
    /*BYTE bytTbrItemIndex = 4;
    vSetToolBarIcon( m_wndToolbarJ1939, bytTbrItemIndex, IDI_ICON_J1939_TRANSMIT_OFF, IDI_ICON_J1939_TRANSMIT_OFF, IDI_ICON_J1939_TRANSMIT_DISABLED );*/
    return S_OK;
}

/**
* \brief         Sets the icon of a particular toolbar item
* \param[in]     Toolbar object, Item Index, Icon resource IDs for normal, hot and disabled options
* \param[out]    void
* \return        void
* \authors       Arunkumar Karri
* \date          28.02.2013 Created
*/
void CMainFrame::vSetToolBarIcon(CNVTCToolBar& objToolbar, BYTE bytItemIndex, UINT nTBIDNormal, UINT nTBIDHot, UINT nTBIDDisabled)
{
    /* Perform this only if resource dll is available */
    if ( m_hModAdvancedUILib )
    {
        /* Get hold of toolbar control */
        CToolBarCtrl& Toolbar       = objToolbar.GetToolBarCtrl();

        /* Get image list objects */
        CImageList* pList           = Toolbar.GetImageList();
        CImageList* pListHot        = Toolbar.GetHotImageList();
        CImageList* pListDisabled   = Toolbar.GetDisabledImageList();

        /* If successfully got all the image list pointers, proceed further */
        if ( pList && pListHot && pListDisabled )
        {
            /* Load the icon based on resource ID */
            HICON hIconNormal, hIconHot, hIconDisabled;

            hIconNormal     = LoadIcon(m_hModAdvancedUILib, MAKEINTRESOURCE(nTBIDNormal));
            hIconHot        = LoadIcon(m_hModAdvancedUILib, MAKEINTRESOURCE(nTBIDHot));
            hIconDisabled   = LoadIcon(m_hModAdvancedUILib, MAKEINTRESOURCE(nTBIDDisabled));
            /* Replace the respective Icon images if icons are successfully loaded */
            if ( hIconNormal &&  hIconHot && hIconDisabled )
            {
                pList->Replace(bytItemIndex, hIconNormal);
                pListHot->Replace(bytItemIndex, hIconHot);
                pListDisabled->Replace(bytItemIndex, hIconDisabled);

                objToolbar.Invalidate();
            }
        }
    }
}

/**
* \brief         Modifies the toolbar button size
* \param[in]     Toolbar object, Size object
* \return        void
* \authors       Arunkumar Karri
* \date          01.03.2013 Created
*/
void CMainFrame::vSetToolbarButtonSize(CNVTCToolBar& objToolbar, CSize& objSize)
{
    /* Get hold of toolbar control */
    CToolBarCtrl& Toolbar       = objToolbar.GetToolBarCtrl();

    /* Set the toolbar button size */
    Toolbar.SetButtonSize(objSize);
}

/**
* \brief         Modifies the icon of a particular toolbar item
* \param[in]     Toolbar object, Item Index, Icon ON and OFF resource IDs and item ON status
* \param[out]    void
* \return        void
* \authors       Arunkumar Karri
* \date          28.02.2013 Created
*/
void CMainFrame::vModifyToolbarIcon(CNVTCToolBar& objToolbar, BYTE bytItemIndex, BOOL bItemON, UINT nTBIDON, UINT nTBIDOFF)
{
    /* Perform this only if resource dll is available */
    if ( m_hModAdvancedUILib )
    {
        /* Get hold of toolbar control */
        CToolBarCtrl& Toolbar       = objToolbar.GetToolBarCtrl();

        /* Get image list objects */
        CImageList* pList           = Toolbar.GetImageList();
        CImageList* pListHot        = Toolbar.GetHotImageList();
        CImageList* pListDisabled   = Toolbar.GetDisabledImageList();

        /* Load the icon based on resource ID */
        HICON hIcon;
        if ( bItemON )
        {
            hIcon = LoadIcon(m_hModAdvancedUILib, MAKEINTRESOURCE(nTBIDON));
        }
        else
        {
            hIcon = LoadIcon(m_hModAdvancedUILib, MAKEINTRESOURCE(nTBIDOFF));
        }

        /* Replace the respective Icon images if icon is successfully loaded */
        if ( hIcon )
        {
            pList->Replace(bytItemIndex, hIcon);
            pListHot->Replace(bytItemIndex, hIcon);
            //pListDisabled->Replace(bytItemIndex, hIcon);

            objToolbar.Invalidate();
        }
    }
    /* Handle connect icon change for default icons */
    else if ( nTBIDOFF == IDI_ICON_CAN_CONNECT )
    {
        /* Get hold of toolbar control */
        CToolBarCtrl& Toolbar       = objToolbar.GetToolBarCtrl();

        /* Get image list objects */
        CImageList* pList           = Toolbar.GetImageList();
        CImageList* pListHot        = Toolbar.GetHotImageList();
        CImageList* pListDisabled   = Toolbar.GetDisabledImageList();

        /* If successfully got all the image list pointers, proceed further */
        if ( pList && pListHot && pListDisabled )
        {
            /* Load the icon based on resource ID */
            HICON hIcon;
            if ( bItemON )
            {
                hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_DISCONNECT));
            }
            else
            {
                hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_CONNECT));
            }

            /* Replace the respective Icon images if icon is successfully loaded */
            if ( hIcon )
            {
                pList->Replace(bytItemIndex, hIcon);
                pListHot->Replace(bytItemIndex, hIcon);
                pListDisabled->Replace(bytItemIndex, hIcon);

                objToolbar.Invalidate();
            }
        }
    }
}

/******************************************************************************
    Function Name    :  OnReplayFilter
    Input(s)         :      -
    Output           :      -
    Functionality    :  To handle(enable/disable) filter for replay when user
                        selects from the main menu
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Ashwin R Uchil
    Date Created     :  3.1.2013
******************************************************************************/
void CMainFrame::OnReplayFilter()
{
    CFlags* pouFlags = NULL;
    BOOL bReplayFilterStatus = FALSE;

    pouFlags = theApp.pouGetFlagsPtr();
    if(pouFlags != NULL )
    {
        bReplayFilterStatus = pouFlags->nGetFlagStatus(REPLAYFILTER);
        bReplayFilterStatus = bReplayFilterStatus ? FALSE : TRUE;
        pouFlags->vSetFlagStatus(REPLAYFILTER, bReplayFilterStatus);

        vREP_EnableFilters(bReplayFilterStatus);

        /* Modify filter icon accordingly in Main toolbar*/
        BYTE bytTbrItemIndex = 7;
        vModifyToolbarIcon( m_wndToolBar, bytTbrItemIndex, bReplayFilterStatus, IDI_ICON_REPLAY_FILTER_ON, IDI_ICON_REPLAY_FILTER );
    }
}

void CMainFrame::ApplyMessageFilterButton()
{
    CFlags* pouFlags = NULL;
    BOOL bMessageFilterStatus = FALSE;

    pouFlags = theApp.pouGetFlagsPtr();
    if(pouFlags != NULL )
    {
        bMessageFilterStatus = pouFlags->nGetFlagStatus(DISPLAYFILTERON);
        //bMessageFilterStatus = bMessageFilterStatus ? FALSE : TRUE;
        pouFlags->vSetFlagStatus(DISPLAYFILTERON, bMessageFilterStatus);

        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_ENABLE_FILTER_APPLIED, (WPARAM)bMessageFilterStatus, NULL);
        /* Modify filter icon accordingly in Main toolbar*/
        BYTE bytTbrItemIndex = 6;
        vModifyToolbarIcon( m_wndToolBar, bytTbrItemIndex, bMessageFilterStatus, IDI_ICON_MSG_FILTER_ON, IDI_ICON_MSG_FILTER );
    }
}



/******************************************************************************
    Function Name    :  OnUpdateMessageFilterButton

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Toggle message filter button status
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :  Amitesh Bharti, 26.12.2002, File name is changed
******************************************************************************/
void CMainFrame::OnUpdateMessageFilterButton(CCmdUI* pCmdUI)
{
    // Toggle message filter button status
    pCmdUI->SetCheck(theApp.pouGetFlagsPtr()->nGetFlagStatus(DISPLAYFILTERON));

}
/******************************************************************************
    Function Name    :  OnUpdateExecuteTimerhandler

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Enable or disable execute timer handler
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modification By  :  Amitesh Bharti
    Modification on  :  23.10.2002,Enable menu/button if tool is connected
    Modification By  :  Amitesh Bharti
    Modification on  :  31.10.2002,Review comment incorporated.
/******************************************************************************/
void CMainFrame::OnUpdateExecuteTimerhandler(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetICANNodeSim()->NS_GetHandlerStatus(H_TIMERBUTTON));
    BOOL bEnable = GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

/******************************************************************************
    Function Name    :  OnUpdateExecuteTimerhandler

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Enable or disable execute timer handler
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modification By  :  Amitesh Bharti
    Modification on  :  23.10.2002,Enable menu/button if tool is connected
    Modification By  :  Amitesh Bharti
    Modification on  :  31.10.2002,Review comment incorporated.
/******************************************************************************/
void CMainFrame::OnUpdateExecuteTimerhandlerLIN(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetILINNodeSim()->NS_GetHandlerStatus(H_TIMERBUTTON));
    BOOL bEnable = GetILINNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}
/******************************************************************************
    Function Name    :  OnClearMsgWindow

    Input(s)         :  -
    Output           :  -
    Functionality    :  Clears the contents of the mesage window
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  26.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnClearMsgWindow()
{
    // Post a message to clear the contents of the
    // message window
    for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
    {
        HWND hWnd;
        hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
        if(hWnd)
        {
            ::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, NULL, NULL);
        }
    }
}

/*******************************************************************************
  Function Name  : OnShowHideMessageWindow
  Input(s)       : UINT nID
  Output         : LRESULT
  Functionality  : Shows/hides Message Window based on the menu click.
  Member of      : CMainFrame
  Author(s)      : Arunkumar K
  Date Created   : 28-03-2011
  Modifications  :
*******************************************************************************/
void CMainFrame::OnShowHideMessageWindow(UINT nID)
{
    switch(nID)
    {
        case ID_SHOWMESSAGEWINDOW_CAN:
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_SHOW_MESSAGE_WINDOW, (WPARAM)TRUE, NULL);
            }
        }
        break;
        case ID_SHOWMESSAGEWINDOW_J1939:
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(J1939);
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_SHOW_MESSAGE_WINDOW, (WPARAM)TRUE, NULL);
            }
        }
        break;
        case ID_SHOWMESSAGEWINDOW_FLEXRAY:
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY);
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_SHOW_MESSAGE_WINDOW, (WPARAM)TRUE, NULL);
            }
        }
        break;
        case ID_SHOWMESSAGEWINDOW_LIN:
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(LIN);
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_SHOW_MESSAGE_WINDOW, (WPARAM)TRUE, NULL);
            }
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
}

/*******************************************************************************
  Function Name  : OnUpdateShowHideMessageWindow
  Input(s)       : pCmdUI
  Output         : -
  Functionality  : Shows the status of Message Window visibility in Menu Item.
  Member of      : CMainFrame
  Author(s)      : Arunkumar K
  Date Created   : 28-03-2011
  Modifications  :
*******************************************************************************/
void CMainFrame::OnUpdateShowHideMessageWindow(CCmdUI* pCmdUI)
{
    switch(pCmdUI->m_nID)
    {
        case ID_SHOWMESSAGEWINDOW_CAN:
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_SHOW_MESSAGE_WINDOW, (WPARAM)FALSE, (LPARAM)pCmdUI);    //Set WPARAM FALSE for update UI
            }
        }
        break;
        case ID_SHOWMESSAGEWINDOW_J1939:
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(J1939);
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_SHOW_MESSAGE_WINDOW, (WPARAM)FALSE, (LPARAM)pCmdUI);    //Set WPARAM FALSE for update UI
            }
        }
        break;
        case ID_SHOWMESSAGEWINDOW_FLEXRAY:
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY);
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_SHOW_MESSAGE_WINDOW, (WPARAM)FALSE, (LPARAM)pCmdUI);    //Set WPARAM FALSE for update UI
            }
        }
        break;
        case ID_SHOWMESSAGEWINDOW_LIN:
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(LIN);
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_SHOW_MESSAGE_WINDOW, (WPARAM)FALSE, (LPARAM)pCmdUI);    //Set WPARAM FALSE for update UI
            }
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
}

/******************************************************************************
    Function Name    :  OnUpdateDllBuildAll

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables or Disables the menu option
                        depending on the FUNCEDITOR flag.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateDllBuildAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_BUILDLOAD));
}

/******************************************************************************
    Function Name    :  OnUpdateDllBuildAll

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables or Disables the menu option
                        depending on the FUNCEDITOR flag.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateDllBuildAllLIN(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetILINNodeSim()->NS_ShouldToolBarBeEnabled(TB_BUILDLOAD));
}
/******************************************************************************
    Function Name    :  OnUpdateDllBuildloadAll

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables or Disables the menu option
                        depending on the FUNCEDITOR flag
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateDllBuildloadAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_BUILDLOAD));
}

/******************************************************************************
    Function Name    :  OnUpdateDllBuildloadAll

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables or Disables the menu option
                        depending on the FUNCEDITOR flag
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateDllBuildloadAllLIN(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetILINNodeSim()->NS_ShouldToolBarBeEnabled(TB_BUILDLOAD));
}
/******************************************************************************
    Function Name    :  poGetSignalPointer

    Input(s)         :  const CString &omStrSignalName
    Output           :  sSIGNALS*
    Functionality    :  Returns pointer to signal if the signal is found
                        otherwise NULL
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :  psGetMessagePointer() call is changed to psGetMessagePointerInactive()
                        to remove the bug while editing a signal of new Message created
                        in database.
    Modifications by :  Bhagyalakshmi & anish
    date             :  19.03.2008
******************************************************************************/
sSIGNALS* CMainFrame::poGetSignalPointer(
    BOOL bActive,
    const UINT& unMsgID,
    const CString& omStrSignalName)
{
    BOOL bFound = FALSE;
    sSIGNALS* pSignal = NULL;

    if ( !omStrSignalName.IsEmpty() )
    {
        sMESSAGE* pMsg = NULL;

        if ( bActive )
        {
            // Get message pointer from the data structure
            pMsg =
                theApp.m_pouMsgSignal->psGetMessagePointer( unMsgID );
        }
        else
        {
            pMsg =
                theApp.m_pouMsgSgInactive->psGetMessagePointerInactive( unMsgID );
        }

        if ( pMsg != NULL )
        {
            pSignal = pMsg->m_psSignals;

            //Loop through the signals list and get the matching signal pointer
            while ( (pSignal != NULL) && (!bFound) )
            {
                if ( pSignal->m_omStrSignalName == omStrSignalName )
                {
                    bFound = TRUE;
                }
                else
                {
                    pSignal = pSignal->m_psNextSignalList;
                }
            }
        }
    }
    return pSignal;
}
/******************************************************************************
    Function Name    :  OnUpdateDllUnloadAll

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables the menu if a DLL is loaded
                        and vice-versa
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateDllUnloadAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD));
}

/******************************************************************************
    Function Name    :  OnUpdateDllUnloadAll

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables the menu if a DLL is loaded
                        and vice-versa
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateDllUnloadAllLIN(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetILINNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD));
}
/******************************************************************************
    Function Name    :  OnUpdateDisplayMessagewindowOverwrite

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables the menu if OVERWRITE flag is true
                        and vice-versa
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateDisplayMessagewindowOverwrite(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(theApp.pouGetFlagsPtr()->nGetFlagStatus(OVERWRITE));
}
/******************************************************************************
    Function Name    :  OnUpdateToolHexdec

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables the menu if HEX flag is true
                        and vice-versa
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :  Arun Kumar K
                        04.06.2010 - commented old method and added new code.
******************************************************************************/
void CMainFrame::OnUpdateToolHexdec(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX));
}
/******************************************************************************
    Function Name    :  OnUpdateLogOnOff

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables the menu if LOGTOFILE is true
                        and vice-versa
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateLogOnOff(CCmdUI* pCmdUI)
{
    if (NULL != sg_pouFrameProcCAN)
    {
        //pCmdUI->SetCheck(sg_pouFrameProcCAN->FPC_IsLoggingON());
        USHORT ushCount =  sg_pouFrameProcCAN->FPC_GetLoggingBlockCount();
        ushCount = vCheckValidLogFiles(ushCount);
        BOOL bEnableLogTB = bIsAtleastOneLoggingBlockEnabled(ushCount);
        if(bEnableLogTB == TRUE)//log files found and if checked
        {
            pCmdUI->Enable(TRUE);
            pCmdUI->SetCheck(sg_pouFrameProcCAN->FPC_IsLoggingON());
        }
        else //log files don't found
        {
            pCmdUI->Enable(FALSE);
            pCmdUI->SetCheck(FALSE);
        }
    }
    else
    {
        //desable the menu item
        pCmdUI->Enable(FALSE);
        pCmdUI->SetCheck(FALSE);
    }
    //pCmdUI->SetCheck(theApp.pouGetFlagsPtr()->nGetFlagStatus(LOGTOFILE));
}

/******************************************************************************
    Function Name    :  OnUpdateLogOnOff_LIN

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables the menu if LOGTOFILE is true
                        and vice-versa
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Shashank Vernekar
    Date Created     :  29.01.2014
    Modifications    :
******************************************************************************/

void CMainFrame::OnUpdateLogOnOff_LIN(CCmdUI* pCmdUI)
{
    if (NULL != sg_pouFrameProcLIN)
    {
        //pCmdUI->SetCheck(sg_pouFrameProcCAN->FPC_IsLoggingON());
        USHORT ushCount =  sg_pouFrameProcLIN->FPL_GetLoggingBlockCount();
        ushCount = vCheckValidLogFiles_LIN(ushCount);
        BOOL bEnableLogTB = bIsAtleastOneLoggingBlockEnabled_LIN(ushCount);
        if(bEnableLogTB == TRUE )//log files found and if checked
        {
            pCmdUI->Enable(TRUE);
            pCmdUI->SetCheck(sg_pouFrameProcLIN->FPL_IsLoggingON());
        }
        else //log files don't found
        {
            pCmdUI->Enable(FALSE);
            pCmdUI->SetCheck(FALSE);
        }
    }
    else
    {
        //desable the menu item
        pCmdUI->Enable(FALSE);
        pCmdUI->SetCheck(FALSE);
    }
    //pCmdUI->SetCheck(theApp.pouGetFlagsPtr()->nGetFlagStatus(LOGTOFILE));
}
BOOL FilePathExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

USHORT CMainFrame::vCheckValidLogFiles(USHORT LogBlocks)
{
    if (LogBlocks > 0)
    {
        for (USHORT i = 0; i < LogBlocks; i++)
        {
            SLOGINFO sLogObject;
            HANDLE hFind;
            WIN32_FIND_DATA FindData;
            CStdioFile omStdiofile;

            sg_pouFrameProcCAN->FPC_GetLoggingBlock(i, sLogObject);

            // check for valid log file path
            CString strTempLog = sLogObject.m_sLogFileName;

            CString omLogFilePath = strTempLog.Left(strTempLog.ReverseFind('\\'));

            if ( !FilePathExists(omLogFilePath) )//file path not found
            {
                sg_pouFrameProcCAN->FPC_RemoveLoggingBlock(i--); //remove the old log data
                LogBlocks = sg_pouFrameProcCAN->FPC_GetLoggingBlockCount();; //refresh the log count
            }
        }
    }
    return LogBlocks;
}

USHORT CMainFrame::vCheckValidLogFiles_LIN(USHORT LogBlocks)
{
    if (LogBlocks > 0)
    {
        for (USHORT i = 0; i < LogBlocks; i++)
        {
            SLOGINFO sLogObject;
            HANDLE hFind;
            WIN32_FIND_DATA FindData;
            CStdioFile omStdiofile;

            sg_pouFrameProcLIN->FPL_GetLoggingBlock(i, sLogObject);

            // check for valid log file path
            CString strTempLog = sLogObject.m_sLogFileName;

            CString omLogFilePath = strTempLog.Left(strTempLog.ReverseFind('\\'));

            if ( !FilePathExists(omLogFilePath) )//file path not found
            {
                sg_pouFrameProcLIN->FPL_RemoveLoggingBlock(i--); //remove the old log data
                LogBlocks = sg_pouFrameProcLIN->FPL_GetLoggingBlockCount();; //refresh the log count
            }
        }
    }
    return LogBlocks;
}
BOOL CMainFrame::bIsAtleastOneLoggingBlockEnabled(USHORT LogBlocks)
{
    BOOL bEnabled = FALSE; // Assume no log files are configured or enabled

    if (LogBlocks > 0)
    {
        for (USHORT i = 0; i < LogBlocks; i++)
        {
            SLOGINFO sLogObject;
            sg_pouFrameProcCAN->FPC_GetLoggingBlock(i, sLogObject);
            if (sLogObject.m_bEnabled == TRUE)
            {
                bEnabled = TRUE;
                break;
            }
        }
    }
    return bEnabled;
}


BOOL CMainFrame::bIsAtleastOneLoggingBlockEnabled_LIN(USHORT LogBlocks)
{
    BOOL bEnabled = FALSE; // Assume no log files are configured or enabled

    if (LogBlocks > 0)
    {
        for (USHORT i = 0; i < LogBlocks; i++)
        {
            SLOGINFO sLogObject;
            sg_pouFrameProcLIN->FPL_GetLoggingBlock(i, sLogObject);
            if (sLogObject.m_bEnabled == TRUE)
            {
                bEnabled = TRUE;
                break;
            }
        }
    }
    return bEnabled;
}
/******************************************************************************/
/*  Function Name    :  PreTranslateMessage                                   */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework to translate     */
/*                      before they are dispathed.WM_CHAR message is captured */
/*                      to execute the key handlers.                          */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  01.04.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  21.10.2002, added connect and disconnect on key down  */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  24.10.2002, Key Hanlder on/off check added            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modifications on :  31.10.2002, Review comment incorporated               */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.01.2004, Interface for transmission of message on  */
/*                                  press of a key.                           */
/******************************************************************************/
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    vProcessKeyPress(pMsg);
    //CFlags* pouFlag = NULL;
    //pouFlag     = theApp.pouGetFlagsPtr();
    //if(pouFlag != NULL)
    //{
    //if (pMsg->message == WM_CHAR)
    //{
    //    // Check for key a-z or A-Z, if any of these are press
    //    // call member function of CExecuteFunc class for key handler
    //    if(   ( pMsg->wParam >= 'A' && pMsg->wParam<='Z' )||
    //          ( pMsg->wParam >= 'a' && pMsg->wParam<='z' )||
    //          ( pMsg->wParam >= '0' && pMsg->wParam<='9' ) )
    //    {
    //
    //                // Execute key hanlder only if execution is selected by user
    //                GetICANNodeSim()->NS_ManageOnKeyHandler((UCHAR)pMsg->wParam);
    //                GetIJ1939NodeSim()->NS_ManageOnKeyHandler((UCHAR)pMsg->wParam);
    //                PostMessage(WM_KEY_PRESSED_MSG_WND, pMsg->wParam, 0);

    //    }
    //}//if (pMsg->message == WM_CHAR)
    //if (pMsg->message == WM_KEYDOWN)
    //{
    //    BOOL bConnected = FALSE;
    //    // Get the current status of Connected/Disconnected state
    //    bConnected  = pouFlag->nGetFlagStatus(CONNECTED);
    //    //Procees the key "F2" and "ESC"
    //    if (pMsg->wParam == VK_F2)
    //    {
    //        if(bConnected == FALSE)
    //        {
    //            OnFileConnect();
    //        }
    //    }//if (pMsg->wParam == VK_F2)
    //    else if (pMsg->wParam == VK_ESCAPE)
    //    {
    //        if(bConnected == TRUE)
    //        {
    //            OnFileConnect();
    //        }
    //    }//else if (pMsg->wParam == VK_ESCAPE)
    //}//if (pMsg->message == WM_KEYDOWN)
    //}//   if(pouFlag != NULL)
    return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

/******************************************************************************/
/*  Function Name    :  OnUpdateReplayGo                                      */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework. This will enable*/
/*                      or disable the  menu item.                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.04.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  23.10.2002,Enable menu/button if tool is connected    */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002,Review comment incorporated                */
/*  Modifications    :  Raja N on 19.07.2005, Changed the implimentation to   */
/*                      call replay manager                                   */
/******************************************************************************/
void CMainFrame::OnUpdateReplayGo(CCmdUI* pCmdUI)
{
    // Dispatch it to Replay Manager
    pCmdUI->Enable( bREP_GetUIStateCmdGo());
}
/******************************************************************************/
/*  Function Name    :  OnUpdateReplaySkip                                    */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework. This will enable*/
/*                      or disable the  menu item.                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.04.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  23.10.2002,Enable menu/button if tool is connected    */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002,Review comment incorporated                */
/*  Modifications    :  Raja N on 19.07.2005, Changed the implimentation to   */
/*                      call replay manager                                   */
/******************************************************************************/
void CMainFrame::OnUpdateReplaySkip(CCmdUI* pCmdUI)
{
    // Dispatch it to Replay Manager
    pCmdUI->Enable( bREP_GetUIStateCmdSkip());

}
/******************************************************************************/
/*  Function Name    :  OnUpdateReplayStep                                    */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework. This will enable*/
/*                      or disable the  menu item.                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.04.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  23.10.2002,Enable menu/button if tool is connected    */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002,Review comment incorporated                */
/*  Modifications    :  Raja N on 19.07.2005, Changed the implimentation to   */
/*                      call replay manager                                   */
/******************************************************************************/
void CMainFrame::OnUpdateReplayStep(CCmdUI* pCmdUI)
{
    // Dispatch it to Replay Manager
    pCmdUI->Enable( bREP_GetUIStateCmdStep());
}
/******************************************************************************/
/*  Function Name    :  OnUpdateReplayStop                                    */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework. This will enable*/
/*                      or disable the  menu item.                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.04.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  23.10.2002,Enable menu/button if tool is connected    */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002,Review comment incorporated                */
/*  Modifications    :  Raja N on 19.07.2005, Changed the implimentation to   */
/*                      call replay manager                                   */
/******************************************************************************/
void CMainFrame::OnUpdateReplayStop(CCmdUI* pCmdUI)
{
    // Dispatch it to Replay Manager
    pCmdUI->Enable( bREP_GetUIStateCmdStop());
}

/******************************************************************************/
/*  Function Name    :  vSaveWinStatus                                        */
/*  Input(s)         :  WINDOWPLACEMENT WinCurrStatus                         */
/*  Output           :                                                        */
/*  Functionality    :  This function is called to save the current status of */
/*                      window to be used at the next time window is shown.   */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.04.2002                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CMainFrame::vSaveWinStatus(WINDOWPLACEMENT WinCurrStatus)
{
    // Write the Window Flag
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_FLAG, REG_DWORD,"",WinCurrStatus.flags);
    // Write the show wnd command
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_SHOWCMD, REG_DWORD,"",WinCurrStatus.showCmd);
    // Write the X, Y position when window is in minimum position
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MINPOS_X, REG_DWORD,"",WinCurrStatus.ptMinPosition.x);
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MINPOS_Y, REG_DWORD,"",WinCurrStatus.ptMinPosition.y);
    // Write the X, Y position when window is in maximum position
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MAXPOS_X, REG_DWORD,"",WinCurrStatus.ptMaxPosition.x);
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MAXPOS_Y, REG_DWORD,"",WinCurrStatus.ptMaxPosition.y);
    // Write the Window co-ordinates
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_LEFT, REG_DWORD,"",WinCurrStatus.rcNormalPosition.left);
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_RIGHT, REG_DWORD,"",WinCurrStatus.rcNormalPosition.right);
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_TOP, REG_DWORD,"",WinCurrStatus.rcNormalPosition.top);
    bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_BOTTOM, REG_DWORD,"",WinCurrStatus.rcNormalPosition.bottom);
}
/******************************************************************************/
/*  Function Name    :  vGetWinStatus                                         */
/*  Input(s)         :  WINDOWPLACEMENT& WinCurrStatus                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called to get the status of window   */
/*                      last stored.                                          */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.04.2002                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CMainFrame::vGetWinStatus(WINDOWPLACEMENT& WinCurrStatus)
{
    CString strTemp;
    // Set the length of the structure
    WinCurrStatus.length = sizeof(WINDOWPLACEMENT);
    // Get the Window Flag
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_FLAG, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.flags))
    {
        WinCurrStatus.flags = WPF_RESTORETOMAXIMIZED;
    }
    // Get the show wnd command
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_SHOWCMD, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.showCmd) )
    {
        WinCurrStatus.showCmd = SW_SHOWMAXIMIZED;
    }
    // Get the X, Y position when window is in minimum position
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MINPOS_X, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.ptMinPosition.x) )
    {
        WinCurrStatus.ptMinPosition.x = 200;
    }

    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MINPOS_Y, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.ptMinPosition.y) )
    {
        WinCurrStatus.ptMinPosition.y = 200;
    }
    // Get the X, Y position when window is in maximum position
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MAXPOS_X, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.ptMaxPosition.x) )
    {
        WinCurrStatus.ptMaxPosition.x = 0;
    }

    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MAXPOS_Y, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.ptMaxPosition.y) )
    {
        WinCurrStatus.ptMaxPosition.y = 0;
    }
    // Get the Window co-ordinates
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_LEFT, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.rcNormalPosition.left) )
    {
        WinCurrStatus.rcNormalPosition.left = 0;
    }
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_RIGHT, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.rcNormalPosition.right) )
    {
        WinCurrStatus.rcNormalPosition.right = 400;
    }
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_TOP, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.rcNormalPosition.top) )
    {
        WinCurrStatus.rcNormalPosition.top = 0;
    }
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_BOTTOM, REG_DWORD, strTemp, (DWORD&)WinCurrStatus.rcNormalPosition.bottom) )
    {
        WinCurrStatus.rcNormalPosition.bottom = 350;
    }
}
/******************************************************************************/
/*  Function Name    :  OnShowWindow                                          */
/*  Input(s)         :  BOOL bShow, UINT nStatus                              */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework. This will       */
/*                      show the window and set the status to last saved one  */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.04.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  14.03.2003                                            */
/*  Modifications    :  Raja N on 07.09.2004 Added code to disable Bus Status */
/*                      Icon by using a transperent Icon. This is for USB     */
/*                      build only                                            */
/*  Modifications    :  Raja N on 7.3.2005                                    */
/*                      Removed the code to create bus statistics dialog. Now */
/*                      this dialog will be created during first show request */
/*                      also removed the code to update status bar Icon       */
/******************************************************************************/
void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CMDIFrameWnd::OnShowWindow(bShow, nStatus);
    // If it is to show the window
    if (bShow == TRUE)
    {
        // Update the window placement
        SetWindowPlacement(&m_WinCurrStatus);
        // Update global window handle
        g_hMainGUI = m_hWnd;
        // Update the window
        InvalidateRect(NULL);
    }
}
/******************************************************************************/
/*  Function Name    :  OnDestroy                                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework to indicate that */
/*                      window is about to destroyed. Save the current status */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  05.04.2002                                            */
/*  Modification By  :  27.12.2002, Gopi, Added code to save the configuration*/
/*                      file.                                                 */
/*  Modification By  :  29.12.2002, Amarnath S,                               */
/*                      Call to SaveConfiguration(...) function removed       */
/*  Modification By  :  30.12.2002, Amitesh Bharti                            */
/*                      Destroy all thread if it is not able to exit normally */
/*                      Kill timer after waiting for the call back function to*/
/*                      exit.                                                 */
/*                      02.01.2003, One more parameter, unMaxWaitTime, added  */
/*                      in function vInitialiseTimerStruct. Loaded libray is  */
/*                      freed.                                                */
/*  Modification By  :  08.01.2004, Amitesh Bharti                            */
/*                      Added bits for selected msg and tx msg block thread   */
/*                      temination                                            */
/*  Modification By  :  05.04.2004, Raja N                                    */
/*                      Added call to free the Signal Watch list memory       */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  10.05.2004 Added function call                        */
/*                      gvStopMessageHandlerThreads to stop message handler   */
/*                      threads and to clear threads and thread block memory  */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  22.07.2004 DLL Unload handler is moved to a seprate   */
/*                      thread                                                */
/*  Modification on  :  14.03.2005 Added code to delete memory allocated for  */
/*                      Bus statistics dialog                                 */
/*  Modifications    :  Raja N on 19.07.2005, Changed the code to call replay */
/*                      manager to send stop signal to replay threads         */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :  27.12.2005,                                           */
/*                   :  Inserted CExecute manger for unloding DLLS            */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :  29.01.2007,                                           */
/*                   :  added g_bReadDllMsg=FALSE to avoid mem leak           */
//*****************************************************************************/
void CMainFrame::OnDestroy()
{
    vREP_SetThreadStopSignal();
    m_objTxHandler.vSetTxStopFlag(TRUE);
    //g_bStopMsgBlockTx    = TRUE;
    g_bStopSelectedMsgTx = TRUE;

    BYTE byStopThread = 0;

    byStopThread |= BIT_TX_SEL_MSG_THREAD  ;
    byStopThread |= BIT_REPLAY_THREAD  ;
    byStopThread |= BIT_MULTI_MSG_THREAD  ;

    // Destroy all running thread
    theApp.vDestroyUtilThreads(defMAX_WAIT_UTIL_THREAD,byStopThread);
    //Stop the dll read thread
    g_bReadDllMsg=FALSE;
    // Destroy Message Handler Thread
    // If DLL is loaded, then delete threads if anything is running/waiting
    //CExecuteManager::ouGetExecuteManager().vDeleteAllNode();
    // Write the MRU file list into the registry
    bWriteIntoRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE1), REG_SZ, m_omStrMRU_ConfigurationFiles[0]);
    bWriteIntoRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE2), REG_SZ, m_omStrMRU_ConfigurationFiles[1]);
    bWriteIntoRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE3), REG_SZ, m_omStrMRU_ConfigurationFiles[2]);
    bWriteIntoRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE4), REG_SZ, m_omStrMRU_ConfigurationFiles[3]);
    bWriteIntoRegistry(HKEY_CURRENT_USER, _(defSECTION_MRU), _(defSECTION_MRU_FILE5), REG_SZ, m_omStrMRU_ConfigurationFiles[4]);
    // Delete memory associated with signal watch window
    vReleaseSignalWatchListMemory(m_psSignalWatchList[CAN]);
    vReleaseSignalWatchListMemory(m_psSignalWatchList[J1939]);
    vEmptySimsysList();

    // Clean Network statistics dialog
    if( m_podBusStatistics != NULL )
    {
        // Delete the memory associated with this member
        delete m_podBusStatistics;
        m_podBusStatistics = NULL;
    }

    // Clean FlexRay Network statistics dialog
    if( m_podFlexRayBusStatistics != NULL )
    {
        // Delete the memory associated with this member
        delete m_podFlexRayBusStatistics;
        m_podFlexRayBusStatistics = NULL;
    }

    CMDIFrameWnd::OnDestroy();

    // Check for the current status of the window
    if (GetWindowPlacement(&m_WinCurrStatus))
    {
        vSaveWinStatus(m_WinCurrStatus);    // save in the registry
    }

    if (g_pouDIL_CAN_Interface != NULL)
    {
        g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();
    }

    //Destruction of Menu Pointer--by Arun
    /*if (pMyMenu)
    {
        pMyMenu->Detach();
        delete pMyMenu;
        pMyMenu = 0;
    }*/
}

/******************************************************************************/
/*  Function Name    :  podSetFunctionViewPtr
/*  Input(s)         :  Pointer to CFunctionEditorDoc class
/*  Output           :
/*  Functionality    :  Sets Pointer to CFunctionEditorDoc class
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  05.03.2002
/******************************************************************************/
//void CMainFrame::podSetFunctionViewPtr(CFunctionEditorDoc* pDoc)
//{
//    m_pomFunctionEditorDoc = pDoc;
//}
/******************************************************************************/
/*  Function Name    :  podGetFunctionEditorDoc
/*  Input(s)         :
/*  Output           :  Pointer to CFunctionEditorDoc class
/*  Functionality    :  Reutns Pointer to CFunctionEditorDoc class
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  05.03.2002
/******************************************************************************/
//CFunctionEditorDoc* CMainFrame::podGetFunctionEditorDoc()
//{
//    return m_pomFunctionEditorDoc;
//}
/******************************************************************************/
/*  Function Name    :  OnUpdateFileNew
/*  Input(s)         :  CCmdUI* pCmdUI
/*  Output           :
/*  Functionality    :  Enables or disables menu if FUNCEDITOR flag is true
                        and viceversa
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  05.03.2002
/******************************************************************************/
void CMainFrame::OnUpdateFileNew(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}
/******************************************************************************/
/*  Function Name    :  OnUpdateFileOpen
/*  Input(s)         :  CCmdUI* pCmdUI
/*  Output           :
/*  Functionality    :  Enables or disables menu if FUNCEDITOR flag is true
                        and viceversa
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  05.03.2002
/******************************************************************************/
void CMainFrame::OnUpdateFileOpen(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}
/******************************************************************************/
/*  Function Name    :  OnCfgLogFile                                          */
/*  Input(s)         :                                                        */
/*  Output           :  -                                                     */
/*  Functionality    :  o Displays congiuration setting dialog for Log        */
/*                      and Replay.                                           */
/*                      o The selected log file if not empty                  */
/*                      is put into the registry.                             */
/*                      o The selected replay file and mode are               */
/*                      stored and flags are set for enableing the            */
/*                      tool bar buttons.                                     */
/*                      o Replay window is display and filled with            */
/*                      messages.                                             */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  30.04.2002                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      16.08.2002, interface for Replay is changed to take   */
/*                      care of time delay ( fixed  and replay file ).        */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      27.12.2002, memory allocated for CMsgReplayWnd class  */
/*                      object only if replay file is selected. ( Not Empty ).*/
/*                      This will avoid delete if replay file is empty.       */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      17.06.2003, The logging is stopped if it is ON and a  */
/*                      new logfile name is selected.                         */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      19.01.2004, The replay window will not be displayed   */
/*                      even if replay is in progress,i.e. thread is running. */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004 Removed disabling logging and added code to*/
/*                      Apply changes with user confirmation                  */
/*  Modifications    :  Raja N on 19.07.2005
                        Changed function name appropriatly
/******************************************************************************/
void CMainFrame::OnCfgLogFile()
{
    if (sg_pouFrameProcCAN != NULL)
    {
        //CConfigMsgLogDlg omDlg(this, sg_pouFrameProcCAN, &m_sFilterAppliedCAN);
        CConfigMsgLogDlg omDlg(CAN, (void* ) sg_pouFrameProcCAN, m_abLogOnConnect[CAN], this,
                               (void*) &m_sFilterAppliedCAN);
        sg_pouFrameProcCAN->FPC_StartEditingSession(); // Start the editing session
        omDlg.vSetLogFileONOFF(sg_pouFrameProcCAN->FPC_IsLoggingON());
        BOOL bToConfirm = (omDlg.DoModal() == IDOK);
        sg_pouFrameProcCAN->FPC_StopEditingSession(bToConfirm);

        if(bToConfirm == IDOK)
        {
            // Check if the filter for log is enabled
            ApplyLogFilter();
        }
    }
}

/******************************************************************************/
/*  Function Name    :  OnCfgLogFile_LIN                                      */
/*  Input(s)         :                                                        */
/*  Output           :  -                                                     */
/*  Functionality    :  o Displays congiuration setting dialog for Log        */
/*                      and Replay.                                           */
/*                      o The selected log file if not empty                  */
/*                      is put into the registry.                             */
/*                                                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Shashank Vernekar                                     */
/*  Date Created     :  30.04.2002                                            */
/******************************************************************************/

void CMainFrame::OnCfgLogFile_LIN()
{
    if (sg_pouFrameProcLIN != NULL)

    {

        //CConfigMsgLogDlg omDlg(this, sg_pouFrameProcCAN, &m_sFilterAppliedCAN);
        CConfigMsgLogDlg omDlg(LIN, (void* ) sg_pouFrameProcLIN, m_abLogOnConnect[LIN], this,
                               (void*) &m_sFilterAppliedLIN);
        sg_pouFrameProcLIN->FPL_StartEditingSession(); // Start the editing session
        omDlg.vSetLogFileONOFF(sg_pouFrameProcLIN->FPL_IsLoggingON());
        BOOL bToConfirm = (omDlg.DoModal() == IDOK);
        sg_pouFrameProcLIN->FPL_StopEditingSession(bToConfirm);

        if(bToConfirm == IDOK)
        {
            // Check if the filter for log is enabled
            // ApplyLogFilter();
        }
    }
}
/******************************************************************************
Function Name    :  OnUpdateCfgnLog
Input(s)         :  pCmdUI
Output           :  -
Functionality    :  Enable / disable log configuration button.
Member of        :  CMainFrame
Friend of        :  -
Author(s)        :  Ratnadip Choudhury
Date Created     :  1.07.2010
******************************************************************************/
void CMainFrame::OnUpdateCfgnLog(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(sg_pouFrameProcCAN != NULL);
    /*  if (NULL != sg_pouFrameProcCAN)
        {
            pCmdUI->Enable(!sg_pouFrameProcCAN->FPC_IsLoggingON());
        }
        else
        {
            pCmdUI->Enable(sg_pouFrameProcCAN != NULL);
        }*/
}

/******************************************************************************
Function Name    :  OnUpdateCfgnLog_LIN
Input(s)         :  pCmdUI
Output           :  -
Functionality    :  Enable / disable log configuration button.
Member of        :  CMainFrame
Friend of        :  -
Author(s)        :  Shashank Vernekar
Date Created     :  29.01.2014
******************************************************************************/


void CMainFrame::OnUpdateCfgnLog_LIN(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(sg_pouFrameProcLIN != NULL);

    /* if ( m_shLINDriverId != DAL_NONE )
    {
        pCmdUI->Enable();
    }
    else
    {
        pCmdUI->Enable(0);
    }*/

    /*if (NULL != sg_pouFrameProcLIN)
       {
           pCmdUI->Enable(!sg_pouFrameProcLIN->FPL_IsLoggingON());
       }
       else
       {
           pCmdUI->Enable(sg_pouFrameProcLIN != NULL);
       }*/
}

/******************************************************************************/
/*  Function Name    :  OnUpdateToolButtonMsgDisp
/*  Input(s)         :  CCmdUI* pCmdUI
/*  Output           :  -
/*  Functionality    :  Disable the button as this functionality
                        is not used.
/*  Member of        :  CMainFrame
/*  Friend of        :      -
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  15.05.2002
/******************************************************************************/
void CMainFrame::OnUpdateToolButtonMsgDisp(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( FALSE );
}

/*****************************************************************************/
/*  Function Name    :  OnEndSession
/*  Description      :  Called by the framework when the current windows
                        session (log off or shutdown) is going to exit.
/*  Input(s)         :  bEnding - TRUE if the session is being ended
/*  Output           :  -
/*  Functionality    :  Perform all application closure related actions viz.
                        exiting of all different threads, freeing of memory
                        allocated etc.
/*  Member of        :  CMainFrame
/*  Author(s)        :  Ratnadip Choudhury
/*  Date Created     :  18.06.2002
/*****************************************************************************/
void CMainFrame::OnEndSession(BOOL bEnding)
{
    CMDIFrameWnd::OnEndSession(bEnding);

    if (bEnding == TRUE)
    {
        // Initiate window closing process.
        DestroyWindow();
    }
}
/*****************************************************************************/
/*  Function Name    :  OnUpdateConfigurationFileName
/*  Description      :  Message handler to update configuration file name
                        in the status bar
/*  Input(s)         :  CCmdUI *pCmdUI
/*  Output           :  -
/*  Functionality    :  Message handler to update configuration file name
                        in the status bar
/*  Member of        :  CMainFrame
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  25.06.2002
/*  Modifications    :  Gopi
/*                      12.11.2002, Changed usage from registry to config
/*                      28.11.2002, Changed the function name and replace
/*                                  database file name with configuration file
/*                                  name.
/*  Modifications    :  Amitesh Bharti
/*                      20.01.2004, update the configuration file name in case
/*                                  user is trying to save the default config
/*                                  is a file. The file name will be intialise
/*                                  to null if a new configuration is created
/*                                  or any other configuration file is created.
/*****************************************************************************/
void CMainFrame::OnUpdateConfigurationFileName(CCmdUI* pCmdUI)
{
    pCmdUI->Enable();
    CString omStrPage;

    CString omStrConfigurationFilename = STR_EMPTY;
    CString omStrDBName = STR_EMPTY;

    vGetLoadedCfgFileName(omStrConfigurationFilename);

    if ( omStrConfigurationFilename.IsEmpty())
    {
        if(m_omStrSavedConfigFile.IsEmpty())
        {
            omStrPage =
                _T(_("Default Configuration loaded"));
        }
        else
        {
            omStrPage.
            Format( _("Config File: %s"), m_omStrSavedConfigFile);
        }
    }
    else
    {
        omStrPage.
        Format( _("Config File: %s"), omStrConfigurationFilename);
    }

    pCmdUI->SetText( omStrPage );

    m_wndStatusBar.GetStatusBarCtrl().SetTipText( INDEX_DB_NAME, omStrPage);
}

/*****************************************************************************/
/*  Function Name    :  vWriteNewLogFilenameInRegistry
/*  Description      :  // Updates new log file name into the registry
                        // from the old log file name
                        // Generates an incremental number which
                        // will be appended to the current filename. And
                        // this new file name will be the log file name for
                        // the next session of logging.
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Updates new log file name into the registry
                        given the old log file name
/*  Member of        :  CMainFrame
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  23.09.2002
/*  Modifications    :  11.12.2002, Amarnath S
/*                      Integration with config class included
/*****************************************************************************/
void CMainFrame::vWriteNewLogFilenameInRegistry( CString omLogFilename )
{
    // Check if the file name has any number appended
    // "XXX10.log"
    int nDotIndex = omLogFilename.ReverseFind('.');
    // Get left of "." character
    CString omStrTemp = omLogFilename.Left( nDotIndex );// "XXX10"
    CString omStrDummy = omStrTemp;// "XXX10"
    BOOL bAlphaCharFound = FALSE;

    UINT unCount = 0;
    char tChar  = ' ';

    while ( bAlphaCharFound != TRUE &&
            (unCount < (UINT)omStrDummy.GetLength()))
    {
        // check if the selected character is a valid numeric character
        tChar = omStrDummy.GetAt( omStrDummy.GetLength() - unCount - 1);

        if( tChar >= '0' && tChar <= '9' )
        {
            unCount++;
        }
        else
        {
            bAlphaCharFound = TRUE;
        }
    }
    if (bAlphaCharFound)// There is atleast one alpha character
    {
        // "XXX10" - index is at last X char
        int nCharIndex = omStrDummy.ReverseFind(tChar);
        omStrTemp = omStrDummy.Left( nCharIndex + 1);//"XXX10" - "XXX"
        //"XXX10" - "10"
        omStrDummy = omStrDummy.Right( omStrDummy.GetLength() - nCharIndex -1);
    }
    else// There is no alpha character
    {
        omStrTemp = STR_EMPTY;
    }

    //"10" - 10
    int nNum = atoi( omStrDummy );
    // 10 - "11"
    omStrDummy.Format( "%d", ++nNum );
    // "XXX" - "XXX11"
    omStrTemp += omStrDummy;
    // "XXX11" - "XXX11.log"
    omStrTemp += ".log";
    // write the log file information into the configuration file
    //theApp.bSetData(LOG_FILE_NAME, &omStrTemp);

}
/******************************************************************************
    Function Name    :  OnCfgSendMsgs

    Input(s)         :  -
    Output           :  void
    Functionality    :  Called by the framework when user wants to configure
                        messages to send the same on the CAN-bus
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath S
    Date Created     :  10.10.2002
    Modifications    :  Amitesh Bharti
                        08.01.2004, Dialog is made modalless and it is restored
                                    is dialog is minimised and user selects
                                    this menu.
    Modifications    :  Raja N
                        26.04.2005, Modified to create new Tx Child window
                        for Tx Message Configuration
******************************************************************************/
void CMainFrame::OnCfgSendMsgs()
{
    BOOL bFirstTime = !(m_objTxHandler.hConfigWindowShown() == S_OK);
    m_objTxHandler.vShowConfigureMsgWindow((void*)this);
    m_objTxHandler.vSetMsgDBPtrInDetailsView((void*)theApp.m_pouMsgSignal);
    if(bFirstTime == TRUE)
    {
        eUSERSELCTION eUserSel;
        //Inform the connect status
        CFlags* pouFlags   = theApp.pouGetFlagsPtr();
        if(pouFlags != NULL)
        {
            // Toggle connect/disconnect flag
            eUserSel = eCONNECTCMD;
            BOOL bConnected = pouFlags->nGetFlagStatus(CONNECTED);
            m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bConnected );
            // Hex/Dec format
            BOOL bHexON = pouFlags->nGetFlagStatus(HEX);
            eUSERSELCTION eUserSel = eHEXDECCMD;
            m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bHexON);
            // Check for transmission on/off
            BOOL bTxOn = pouFlags->nGetFlagStatus(SENDMESG);
            eUserSel     = eTXMSGCMD;
            m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bTxOn);

        }
        // Inform about the database change.
        eUserSel = eDATABASEIMPORTCMD;
        m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel,0);
    }
}

/******************************************************************************
    Function Name    :  OnUpdateToolSendmsg
    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  - Called by the framework when the current GUI state of
                        toolbar button needs to be updated.
                        - Enables the toolbar if the number of configured
                        mesages is more than zero, disables if zero
                        - Press / Unpresses the button if Txing / Not-Txing
                        resply
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Ashwin R Uchil
    Date Created     :  26.3.2013
******************************************************************************/
void CMainFrame::OnUpdateToolFlexRaySendmsg(CCmdUI* pCmdUI)
{
    BOOL bIsConnected = theApp.pouGetFlagsPtr()->nGetFlagStatus(FLEX_CONNECTED);
    BOOL bEnable = bIsConnected && (m_objFlexTxHandler.unGetTxBlockCount() > 0);
    BOOL bStopFlag = !(theApp.pouGetFlagsPtr()->nGetFlagStatus(FLEXRAYSENDMSG));
    pCmdUI->Enable(bEnable);// && (m_bEnableSendToolbarButton));
    if (bIsConnected)
    {
        pCmdUI->SetCheck(!bStopFlag);
    }
    else // Disconnected state
    {
        ASSERT(bStopFlag); // In this state bStopFlag is supposed to be TRUE
        pCmdUI->SetCheck(FALSE);
    }
}
/******************************************************************************
    Function Name    :  OnUpdateToolSendmsg
    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  - Called by the framework when the current GUI state of
                        toolbar button needs to be updated.
                        - Enables the toolbar if the number of configured
                        mesages is more than zero, disables if zero
                        - Press / Unpresses the button if Txing / Not-Txing
                        resply
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath S
    Date Created     :  16.10.2002
    Modifications    :  18.12.2002, Amarnath S
                        Uses m_bEnableSendToolbarButton member varaible
                        instead of getting the data from Config details
******************************************************************************/
void CMainFrame::OnUpdateToolSendmsg(CCmdUI* pCmdUI)
{
    // Enable the toolbar if the number of configured
    // mesages is more than zero, disable if zero
    // in connect mode
    BOOL bIsConnected = theApp.pouGetFlagsPtr()->nGetFlagStatus(CONNECTED);
    BOOL bEnable = bIsConnected && (m_objTxHandler.unGetTxBlockCount() > 0);
    BOOL bStopFlag = m_objTxHandler.bGetTxStopFlag();

    pCmdUI->Enable(bEnable);// && (m_bEnableSendToolbarButton));
    if (bIsConnected)
    {
        pCmdUI->SetCheck(!bStopFlag);
    }
    else // Disconnected state
    {
        ASSERT(bStopFlag); // In this state bStopFlag is supposed to be TRUE
        pCmdUI->SetCheck(FALSE);
    }
}

/* Read thread function for graph display*/
DWORD WINAPI InterpretThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == NULL)
    {
        return (DWORD)-1;
    }

    CMainFrame* pouMainFrm = static_cast<CMainFrame*> (pThreadParam->m_pBuffer);

    if (pouMainFrm == NULL)
    {
        return (DWORD)-1;
    }

    pThreadParam->m_unActionCode = CREATE_TIME_MAP;
    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                pouMainFrm->ReadGraphDataBuffer(FALSE); // Retrieve message from the driver
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                pThreadParam->m_unActionCode = INVOKE_FUNCTION;
                pouMainFrm->ReadGraphDataBuffer(TRUE);
                SetEvent(pThreadParam->m_hActionEvent);
            }
            break;
            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

/* Function to start Msg read thread*/
BOOL CMainFrame::bStartGraphReadThread()
{
    BOOL bReturn = FALSE;
    //First stop the thread if running
    bStopGraphReadThread();
    m_ouGraphReadThread.m_pBuffer = this;
    m_ouGraphReadThread.m_hActionEvent = m_ouCanBuf.hGetNotifyingEvent();
    bReturn = m_ouGraphReadThread.bStartThread(InterpretThreadProc);

    return bReturn;
}
/* Function to stop msg read thread*/
BOOL CMainFrame::bStopGraphReadThread()
{
    BOOL bReturn = FALSE;
    bReturn = m_ouGraphReadThread.bTerminateThread();
    m_ouGraphReadThread.m_hActionEvent = NULL;
    m_ouGraphReadThread.m_unActionCode = IDLE;
    return bReturn;
}

/* routine to read the data from buffer */
INT CMainFrame::ReadGraphDataBuffer(BOOL bCalcTime)
{
    static STCANDATA sCanData;
    if (bCalcTime == TRUE)
    {
        if (m_ouCanBuf.ReadFromBuffer(&sCanData) == CALL_SUCCESS)
        {
            m_nTimeStamp = sCanData.m_lTickCount.QuadPart;
            vUpdateGraphData(sCanData);
        }
    }
    else
    {
        while (m_ouCanBuf.GetMsgCount())
        {
            if (m_ouCanBuf.ReadFromBuffer(&sCanData) == CALL_SUCCESS)
            {
                vUpdateGraphData(sCanData);
            }
        }
    }
    return 0;
}

/*******************************************************************************
  Function Name  : vUpdateGraphData
  Input(s)       : sMessage - CAN Message Frame
  Output         : -
  Functionality  : This function will update interpreted data for use by
                   signal graph window
  Member of      : CMainFrame
  Author(s)      : K ArunKumar
  Date Created   : 25/11/2010
  Modifications  :
*******************************************************************************/
void CMainFrame::vUpdateGraphData(const STCANDATA& sCanData)
{
    SINTERPRETDATA_LIST sInterpretList;

    STCAN_MSG sMessage = sCanData.m_uDataInfo.m_sCANMsg;
    // Iterate through list

    // Get List pointer
    CGraphList* pList = &(m_odGraphList[CAN]);

    // Get Database object pointer
    CMsgSignal* pomDataBase = theApp.m_pouMsgSignal;
    sMESSAGE* psMessage;
    sSIGNALS* psSignal;

    // Byte Array for populating the message bytes
    CByteArray omMsgByte;
    BOOL bByteArrayPopulated = FALSE;

    // If list and control are valid
    if( pList != NULL && pomDataBase != NULL )
    {
        // Get the list item count
        int nCount = pList->m_omElementList.GetSize();
        CGraphElement odElement;
        // Iterate through each items
        for( register int nIndex = 0; nIndex < nCount; nIndex++ )
        {
            // Init Msg and Sig pointers
            psSignal = NULL;
            psMessage = NULL;
            // get Current item
            odElement = pList->m_omElementList.GetAt( nIndex );
            // If the item is enabled and not of type Statistics
            // and the message id is same
            if( odElement.m_bEnabled == TRUE &&
                    odElement.m_nValueType != eSTAT_PARAM &&
                    (UINT)odElement.m_nMsgID == sMessage.m_unMsgID )
            {
                // Get the signal Pointer
                psMessage =
                    pomDataBase->psGetMessagePointer( odElement.m_nMsgID );
                if( psMessage != NULL )
                {
                    psSignal = psMessage->m_psSignals;
                }
                // Iterate throuch list of signals
                while( psSignal != NULL &&
                        psSignal->m_omStrSignalName != odElement.m_omStrElementName)
                {
                    // Jump to next signal
                    psSignal = psSignal->m_psNextSignalList;
                }
                // Got Match
                if( psSignal != NULL )
                {
                    if( bByteArrayPopulated == FALSE )
                    {
                        // Populate Byte Array
                        // if the data format is Motorola, reverse the byte
                        if ( psMessage->m_nMsgDataFormat == DATA_FORMAT_INTEL)
                        {
                            for ( register INT nCount = 0;
                                    nCount < (INT)psMessage->m_unMessageLength;
                                    nCount++ )
                            {
                                omMsgByte.Add(sMessage.m_ucData[nCount]);
                            }
                        }
                        else
                        {
                            for ( register INT nCount =
                                        (INT)(psMessage->m_unMessageLength - 1);
                                    nCount >= 0;
                                    nCount-- )
                            {
                                omMsgByte.Add(sMessage.m_ucData[nCount]);
                            }
                        }
                        bByteArrayPopulated = TRUE;
                    }
                    // Get Interpreted value
                    __int64 nRawValue = m_odIntMsg.n64GetSignalValue(
                                            &omMsgByte,             // Byte Array
                                            psSignal->m_unStartByte, // Start Byte
                                            psSignal->m_byStartBit,      // Start Bit
                                            psSignal->m_unSignalLength,        // Sig Len
                                            psSignal->m_bySignalType,         // Sig Type
                                            psSignal->m_eFormat);   // Signal byte order intel or motorola

                    sInterpretList.unMsgID = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;
                    sInterpretList.m_nTimeStamp = sCanData.m_lTickCount.QuadPart;
                    //Tobias - venkat
                    strcpy_s(sInterpretList.m_acSigName,64, odElement.m_omStrElementName.GetBuffer(64));
                    switch( odElement.m_nValueType )
                    {
                        case eRAW_VALUE:
                        {
                            // Update signal data into interpret structure
                            sInterpretList.m_unValue.m_nRawValue = nRawValue;
                            sInterpretList.m_shType = eRAW_VALUE;
                            break;
                        }
                        case ePHY_VALUE:
                        {
                            // Find Physical Value
                            // Calculate Phy Value
                            double dPhysical =
                                static_cast<double>( nRawValue );
                            dPhysical *= psSignal->m_fSignalFactor;
                            dPhysical += psSignal->m_fSignalOffset;
                            // Update Graph Control
                            sInterpretList.m_unValue.m_dPhysical = dPhysical;
                            sInterpretList.m_shType = ePHY_VALUE;
                            break;
                        }
                    }// Switch
                    m_pouMsgInterpretBuffer->WriteIntoBuffer(CAN,(BYTE*)&sInterpretList,SIZE_INTRP_DATA);
                } // If Signal != NULL
            } // If Message ID matches
        } // For loop of List Elements
    } // If list and control and dbase pointers are valid
}
/******************************************************************************/
/*  Function Name    :  OnFileConnect                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework when user selects*/
/*                      connect/disconnect menu/toolbar button. This will be  */
/*                     also called from PreTranslateMsg() function when ESC/F2*/
/*                     key is pressed. It will load the bitmap for connect and*/
/*                     disconnect depending upon the state of this tool.      */
/*                     All global flags are toggled when state of connected or*/
/*                     disconnected toggles. Msg is posted to msg window to   */
/*                      reset handler flags.                                  */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti, Amarnath Shastry                      */
/*  Date Created     :  21.10.2002                                            */
/*  Modification By  :  Amarnath S                                            */
/*  Modification on  :  28.10.2002                                            */
/*                      Initialised all the global thread flags to TRUE       */
/*                      instead of "!bConnected"                              */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002, Review comment incorporated               */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  26.11.2002, Order of connect and disconnect toolbar   */
/*                                  bitmap have been revered                  */
/*                      02.01.2003, One more parameter, unMaxWaitTime, added  */
/*                      in function vInitialiseTimerStruct.                   */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  08.01.2004, Changes for terminating running thread for*/
/*                                  message blocks.                           */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  23.02.2004, Added PostThreadMessage to ack. msg thread*/
/*                                  to update the time reference used for     */
/*                                  absolute time calculation                 */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  29.04.2004 Replaced PostMessage with direct assignment*/
/*                      of global variable g_bMsgHandlerON Msg Handler Enable */
/*                      flag. And moved PostThreadMessage upwards to avoide   */
/*                      negative timings and added call to clear message      */
/*                      window on connect - CRH0022                           */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  10.05.2004 Removed init of g_bStopMessageHandlers flag*/
/*                      as it is not required and removed from the global list*/
/*  Modifications    :  Raja N                                                */
/*                      23.06.2004, Modified PostMessage with CTimeManager::  */
/*                      vInitAbsoluteTime to init Abs time on connect         */
/*  Modifications    :  Raja N                                                */
/*                      22.07.2004, Modifications because of new DLL unload   */
/*                      handler moved in to a seprate thread and message      */
/*                      buffer creation creation in case of size of change    */
/*  Modifications    :  Raja N                                                */
/*                      01.08.2004, Added postmessage to indicate disconnect  */
/*                      to send multimessage dialog to enable controls        */
/*  Modifications    :  Raja N                                                */
/*                      07.09.2004, Modified code to refer HI layer functions */
/*  Modifications    :  Raja N                                                */
/*                      07.09.2004, Modified code to reflect changes in the   */
/*                      bus statistics dialog                                 */
/*  Modifications    :  Raja N on 26.04.2005, Modification code to refer new  */
/*                      Tx Msg configuration window for posting message about */
/*                      connection status change                              */
/*  Modifications    :  Raja N on 19.07.2005, Changed to code to use new      */
/*                      architecture for Log and Replay                       */
/*  Modifications    :  ArunKumar K on 09.11.2010, Added code for updating    */
/*                      connect status to Signal Graph Window module.         */
/******************************************************************************/
void CMainFrame::OnFileConnect()
{
    CFlags* pouFlags = NULL;

    // Hour glass cursor
    CWaitCursor omWait;

    pouFlags   = theApp.pouGetFlagsPtr();
	// If LIN or FlexRay is connected, stop connecting to the network and display information
	if(pouFlags != NULL)
	{
        if (pouFlags->nGetFlagStatus(LIN_CONNECTED) || pouFlags->nGetFlagStatus(FLEX_CONNECTED))
		{
			theApp.bWriteIntoTraceWnd(_("Please disconnect LIN / FlexRay networks and connect CAN"));
			return;
		}
	}

    if(pouFlags != NULL)
    {
        // Toggle connect/disconnect flag
        BOOL bConnected = pouFlags->nGetFlagStatus(CONNECTED);
        BOOL bReturn    = FALSE;
        bConnected = !bConnected;
        // Reset flag for stop running of thread function
        g_bStopTimerHandlers    = TRUE;
        g_bStopKeyHandlers      = TRUE;
        g_bStopErrorHandlers    = TRUE;
        if(bConnected)
        {
            m_objTxHandler.vSetTxStopFlag(TRUE);
            //Shailesh +
            //sg_pouFrameProcCAN->IncrementConnectionCount();
            //Shailesh -

        }
        //g_bStopMsgBlockTx       = TRUE;
        g_bStopSelectedMsgTx    = TRUE;
        // If Tx Msg window is active post a message about connection change

        vUpdateMainEntryListInWaveDataHandler();
        eUSERSELCTION eUserSel = eCONNECTCMD;
        for(short eBusType =0; eBusType <MAX_PROTOCOL; eBusType++)
        {
            m_objSigGrphHandler.vPostMessageToSGWnd(eBusType,WM_USER_CMD,
                                                    (WPARAM)eUserSel,bConnected);
        }

        if(m_objTxHandler.hConfigWindowShown() == S_OK)
        {
            // Send Message to Indicate connection change
            m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD,(WPARAM)eUserSel,bConnected);
            // Send message to indicate message transmission stop event if
            // the tool is disconnected
            if( bConnected == FALSE )
            {
                eUserSel = eTXMSGCMD;
                m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, 0 );
                m_objTxHandler.vStopTransmission(0);
            }
        }
        // Update Replay Manager to Stop running replay threads
        if( bConnected == FALSE )
        {
            //Stop transmission of J1939 message if any
            ::SendMessage(m_pouTxMsgWndJ1939->GetSafeHwnd(),
                          WM_CONNECT_CHANGE, (WPARAM)FALSE, 0);
            //Handle Replay
            vREP_HandleConnectionStatusChange( FALSE );
        }
        // If message transmission thread is waiting for key event then reset it
        // so that thread is terminated. Delete the event.
        theApp.vDestroyUtilThreads(500, BIT_MULTI_MSG_THREAD);
        // Pre Connect Activities

        if (m_podMsgWndThread != NULL)
        {
            HWND hWnd = NULL;
            for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
            {
                if (shBusID == FLEXRAY)
                {
                    continue;
                }
                hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
                //Update Message Window
                if(hWnd)
                {
                    ::SendMessage(hWnd, WM_UPDATE_CONNECT_STATUS, bConnected, NULL);
                }
            }
        }
        CBaseNodeSim* pNodeSim = NULL;
        if( bConnected == TRUE)
        {
            //Inform J1939TxWindow about connect change
            ::SendMessage(m_pouTxMsgWndJ1939->GetSafeHwnd(),
                          WM_CONNECT_CHANGE, (WPARAM)TRUE, 0);
            //Start Graph Interpret Thread
            bStartGraphReadThread();

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_START_READ, 0);

            // Update Absolute time value
            CTimeManager::vInitAbsoluteTime();
            if (m_podMsgWndThread != NULL)
            {
                for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
                {
                    if (shBusID == FLEXRAY)
                    {
                        continue;
                    }
                    HWND hWnd;
                    hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
                    if(hWnd)
                    {
                        ::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, NULL, NULL);
                        //Clear Sort Arrow in Message Window
                        ::SendMessage(hWnd, WM_CLEAR_SORT_COLUMN, NULL, NULL);
                    }
                }
            }
            GetICANBusStat()->BSC_bStartUpdation(FALSE);
            GetICANBusStat()->BSC_ResetBusStatistic();
            for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
            {
#ifdef BOA_FD_VERSION
                GetICANBusStat()->BSC_SetBaudRate(i, m_asControllerDetails[i].m_unDataBitRate);
#else
                GetICANBusStat()->BSC_SetBaudRate(i, _tstof(m_asControllerDetails[i].m_omStrBaudrate.c_str()));
#endif
            }
            GetICANBusStat()->BSC_bStartUpdation(TRUE);



            //send time to nodesim for calculation
            if (NS_GetInterface(CAN, (void**) &pNodeSim) == S_OK)
            {
                pNodeSim->NS_nOnBusConnected(TRUE);
            }

        }
        else
        {
            // PTV [1.6.4]
            if (NULL != sg_pouFrameProcCAN)
            {
                sg_pouFrameProcCAN->FPC_DisableDataLogFlag();
            }
            if(NULL != sg_pouIJ1939Logger)
            {
                sg_pouIJ1939Logger->FPJ1_DisableJ1939DataLogFlag();
            }
            // PTV [1.6.4]
            //Stop Graph Interpret Thread
            bStopGraphReadThread();
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_STOP_READ, 0);





            //m_n64TimeElapsedSinceConnection =0;

            //send time to nodesim for calculation
            if (NS_GetInterface(CAN, (void**) &pNodeSim) == S_OK)
            {
                pNodeSim->NS_nOnBusConnected(FALSE);
            }
        }
        // Use HI layer function to Connect/Disconnect
        if (bConnected == TRUE)
        {
            CWaitIndicator ouWaitIndicator;
            ouWaitIndicator.DisplayWindow(
                _T(_("Trying to connect the hardware ... Please wait")), this);
            {
                if (g_pouDIL_CAN_Interface->DILC_StartHardware() == S_OK)
                {
                    ouWaitIndicator.SetWindowText(_T(_("Connected... ")));
                    bReturn = TRUE;
                }
                else
                {
                    ouWaitIndicator.SetWindowText(_T(_("Failed to connect the hardware... ")));
                    theApp.bWriteIntoTraceWnd(_("Failed to start the hardware"));
                }
            }
            // Commented to avoid Timedelay in log file
            //Sleep (200);
            ouWaitIndicator.CloseWindow();
        }
        else
        {
            if (g_pouDIL_CAN_Interface->DILC_StopHardware() == S_OK)
            {
                bReturn = TRUE;
            }
        }
        //// Use HI layer function to Connect/Disconnect
        //      if (bConnected == TRUE)
        //      {
        //          CWaitIndicator ouWaitIndicator;
        //          ouWaitIndicator.DisplayWindow(
        //              _T(_("Trying to connect the LINhardware ... Please wait")), this);
        //          {
        //              if (g_pouDIL_LIN_Interface->DILL_StartHardware() == S_OK)
        //              {
        //                  ouWaitIndicator.SetWindowText(_T(_("LIN Connected... ")));
        //                  bReturn = TRUE;
        //              }
        //              else
        //              {
        //                  ouWaitIndicator.SetWindowText(_T(_("Failed to connect the LIN hardware... ")));
        //                  theApp.bWriteIntoTraceWnd(_("Failed to start the LIN hardware"));
        //              }
        //          }
        //          // Commented to avoid Timedelay in log file
        //          //Sleep (200);
        //          ouWaitIndicator.CloseWindow();
        //      }
        //      else
        //      {
        //          if (g_pouDIL_LIN_Interface->DILL_StopHardware() == S_OK)
        //          {
        //              bReturn = TRUE;
        //          }
        //      }
        if(bReturn == TRUE )
        {
            BOOL bLogIsON = FALSE, bJ1939LogON = FALSE, bLinLogON = FALSE;

            CFlags* pFlagLog = theApp.pouGetFlagsPtr();
            if(pFlagLog != NULL)
            {
                bLogIsON = pFlagLog->nGetFlagStatus(LOGTOFILE);
            }

            //Handle start/stop logging
            if (m_abLogOnConnect[CAN] == TRUE)
            {
                vStartStopLogging(bConnected);
            }
            // PTV [1.6.6]
            else if(bLogIsON == TRUE && bConnected == TRUE)
            {
                vStartStopLogging(bLogIsON);
            }




            if (m_abLogOnConnect[J1939] == TRUE)
            {
                // Enable Logging or stop logging
                if (NULL != sg_pouIJ1939Logger)
                {
                    if (bConnected)
                    {
                        vSetAssociatedDatabaseFiles(J1939); // Update the db file names associated
                        vSetBaudRateInfo(J1939);                // Update the baud rate details
                    }
                    sg_pouIJ1939Logger->FPJ1_EnableLogging(bConnected);

                    BYTE bytTbrItemIndex = 3;
                    vModifyToolbarIcon( m_wndToolbarJ1939, bytTbrItemIndex, (bool)sg_pouIJ1939Logger->FPJ1_IsLoggingON(), IDI_ICON_J1939_LOG_ON, IDI_ICON_J1939_LOG_OFF );
                }

            }
            else
            {
                // PTV [1.6.6]
                if (NULL != sg_pouIJ1939Logger)
                {
                    if(sg_pouIJ1939Logger->FPJ1_IsLoggingON() == TRUE)
                    {
                        bJ1939LogON = TRUE;
                        sg_pouIJ1939Logger->FPJ1_EnableLogging(TRUE);
                    }
                }
            }
            //SGW Code commented by Arun 21-10-2010
            pouFlags->vSetFlagStatus(CONNECTED, bConnected);

            // Post Connect Activities
            if( bConnected == TRUE )
            {
                // PTV[1.6.4]
                // If Connect and Log both are enabled

                BOOL bLogON = FALSE;
                CFlags* pFlag = theApp.pouGetFlagsPtr();
                if(pFlag != NULL)
                {
                    bLogON = pFlag->nGetFlagStatus(LOGTOFILE);
                }

                if (bLogON == TRUE || m_abLogOnConnect[CAN] == TRUE)
                {
                    //if(theApp.m_pouMsgSignal != NULL)
                    {
                        //if(theApp.m_pouMsgSignal->unGetNumerOfMessages() > 0)
                        //if (NULL != sg_pouFrameProcCAN)
                        {
                            //if(sg_pouFrameProcCAN->FPC_IsDataLogged() == TRUE)
                            {
                                m_unTimerSBLog = SetTimer(TIMER_REFRESH_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, NULL);
                            }
                        }
                    }
                }
                /*if(bLinLogON == TRUE || m_abLogOnConnect[LIN] == TRUE)
                {
                    m_unLinTimerSBLog = SetTimer(TIMER_REFRESH_LIN_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, NULL);
                }*/
                if(bJ1939LogON == TRUE || m_abLogOnConnect[J1939] == TRUE)
                {
                    m_unJ1939TimerSBLog = SetTimer(TIMER_REFRESH_J1939_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, NULL);
                }
                // PTV[1.6.4]

                vREP_HandleConnectionStatusChange( TRUE );
                //send connection statud to replay window
                BOOL bReplayFilterStatus = FALSE;
                bReplayFilterStatus = pouFlags->nGetFlagStatus(REPLAYFILTER);
                vREP_EnableFilters(bReplayFilterStatus);
            }
            // On Disconnect Kill the timer
            // PTV[1.6.4]
            else
            {
                if(m_unTimerSBLog != NULL)
                {
                    ::KillTimer(NULL, m_unTimerSBLog);
                    m_unTimerSBLog = NULL;
                    // Update Status bar
                    //m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
                    m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
                }

                //if(m_unJ1939TimerSBLog != NULL)
                //{
                //  ::KillTimer(NULL, m_unJ1939TimerSBLog);
                //  m_unJ1939TimerSBLog = NULL;
                //  // Update Status bar
                //  //m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
                //  m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_J1939_LOG_ICON, m_hLogIcon1);
                //}
            }
            // PTV[1.6.4]

            // Enable / disable signal transmission block
            m_ouWaveTransmitter.bUpdateBlock(bConnected);

            //venkat
            m_objTSExecutorHandler.vStartStopReadThread(CAN, bConnected);
            m_objTSExecutorHandler.vBusConnected(bConnected);

            // Reset the flags for handlers
            if(bConnected == FALSE )
            {

                pouFlags->vSetFlagStatus(MSGHANDLERBUTTON,bConnected);
                // Post a message to message display window to reset the flag.
                // Changed to assignment to global variable
                g_bMsgHandlerON = bConnected;

                pouFlags->vSetFlagStatus(KEY_HANDLER_ON,bConnected);
                pouFlags->vSetFlagStatus(ERROR_HANDLER, bConnected);
                pouFlags->vSetFlagStatus(ALL_HANDLER, bConnected);

                // Let the user configure the mgs
                pouFlags->vSetFlagStatus( SENDMESG, FALSE );

                // Enable the user to load or create another cfg file
                if(pouFlags->nGetFlagStatus(FLEX_CONNECTED) == FALSE)
                {
                    m_bCfgLoadMenuOption    = TRUE;
                    m_bCfgNewMenuOption     = TRUE;
                }
            }
            else
            {
                // Disable the user to load or create another cfg file
                m_bCfgLoadMenuOption    = FALSE;
                m_bCfgNewMenuOption     = FALSE;
            }

            g_bStopSendMultMsg  = TRUE;
            // Get reference to toolbar control
            CToolBarCtrl& omRefToolBarCtrl = vGetReferenceToToolBarCtrl();

            // Load the required bitmap to
            // show connect/disconnect state
            /* Modify connect icon accordingly in Main toolbar*/
            BYTE bytTbrItemIndex = 1;
            vModifyToolbarIcon( m_wndToolBar, bytTbrItemIndex, bConnected, IDI_ICON_CAN_DISCONNECT, IDI_ICON_CAN_CONNECT );

            // Press / Unpress the button if Connected / Disconnected
            omRefToolBarCtrl.PressButton(IDM_FILE_CONNECT, bConnected);
        }
        if(!bConnected)//for proper logging of data even if connection stopped
        {
            if (NULL != sg_pouFrameProcCAN)
            {
                USHORT ushCount =   sg_pouFrameProcCAN->FPC_GetLoggingBlockCount();
                if(ushCount>0)//check for log file count
                {
                    if(sg_pouFrameProcCAN->FPC_IsLoggingON())//if logging is on
                    {
                        sg_pouFrameProcCAN->FPC_vCloseLogFile();
                    }
                }
            }

            //shashank


            if(NULL != sg_pouIJ1939Logger)
            {
                //USHORT ushCount =   sg_pouFrameProcCAN->FPJ1_GetLoggingBlockCount();
                //if(ushCount>0)//check for log file count
                {
                    if(sg_pouIJ1939Logger->FPJ1_IsLoggingON())//if logging is on
                    {
                        sg_pouIJ1939Logger->FPJ1_vCloseLogFile();
                    }
                }
            }
        }
        if ( bConnected == TRUE )
        {
            GetICANNodeSim()->NS_ManageBusEventHandler(BUS_CONNECT);
            GetIJ1939NodeSim()->NS_ManageBusEventHandler(BUS_CONNECT);

            BOOL bIsJ1939Active = theApp.pouGetFlagsPtr()->nGetFlagStatus(ACTIVATED_J1939);

            if(bIsJ1939Active == TRUE)
            {
                BOOL bIsAddClaimed = theApp.pouGetFlagsPtr()->nGetFlagStatus(ADRESSCLAIM_J1939);
                if(bIsAddClaimed == FALSE)
                {
                    CFlags* pouFlags = theApp.pouGetFlagsPtr();
                    if (pouFlags != NULL)
                    {
                        GetIJ1939DIL()->DILIJ_NM_GetByteAddres(m_sJ1939ClientParam.m_byAddress,
                                                               m_sJ1939ClientParam.m_dwClientId);
                    }
                    GetIJ1939DIL()->DILIJ_ClaimAdress();
                    theApp.pouGetFlagsPtr()->vSetFlagStatus(ADRESSCLAIM_J1939, TRUE);
                }
            }
        }
        else
        {
            GetICANNodeSim()->NS_ManageBusEventHandler(BUS_DISCONNECT);
            GetIJ1939NodeSim()->NS_ManageBusEventHandler(BUS_DISCONNECT);
        }
    }
}

void CMainFrame::OnLINConnect()
{
    HRESULT hResult = S_OK;
    CFlags* pouFlag  = theApp.pouGetFlagsPtr();

	// If LIN or FlexRay is connected, stop connecting to the network and display information
	if (NULL != pouFlag)
	{
        if (pouFlag->nGetFlagStatus(CONNECTED) || pouFlag->nGetFlagStatus(FLEX_CONNECTED))
		{
			theApp.bWriteIntoTraceWnd(_("Please disconnect CAN / FlexRay networks and connect LIN"));
			return;
		}
	}

    /* Toggle connect/disconnect flag */
    BOOL bConnected = pouFlag->nGetFlagStatus(LIN_CONNECTED);
    bConnected = !bConnected;

    //m_objFlexTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eCONNECTCMD, bConnected);

    //m_objTxHandlerLin.vShowConfigureMsgWindow((void*)this, SW_HIDE);

    /*if(m_objTxHandlerLin.hConfigWindowShown() == S_OK)
    {
        // Send Message to Indicate connection change
       // m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD,(WPARAM)eCONNECTCMD,bConnected);
        // Send message to indicate message transmission stop event if
        // the tool is disconnected
        if( bConnected == FALSE )
        {
            m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eTXMSGCMD, 0 );
            m_objTxHandlerLin.vStopTransmission(0);
        }
    }
    else
    {
        // Send Message to Indicate connection change
        m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD,(WPARAM)eCONNECTCMD,bConnected);
    }*/

    CBaseNodeSim* pNodeSim = NULL;
    /* If connecton is required */
    if ( bConnected )
    {
        hResult = g_pouDIL_LIN_Interface->DILL_SetConfigData(m_ouClusterConfig[LIN]);
        hResult = g_pouDIL_LIN_Interface->DILL_PreStartHardware();
        g_pouDIL_LIN_Interface->DILL_GetConfiguration(m_sControllterlin,nsize);


        m_objFlexTxHandler.vBusStatusChanged(LIN, BUS_PRECONNECT);
        GetILINNodeSim()->NS_ManageBusEventHandler(BUS_PRE_CONNECT);
        m_objFlexTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eCONNECTCMD, BUS_PRE_CONNECT);

        hResult = g_pouDIL_LIN_Interface->DILL_StartHardware();

        /* If connect fails, return from here */
        if ( hResult == S_FALSE )
        {
            m_objFlexTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eCONNECTCMD, BUS_DISCONNECTED);

            m_objFlexTxHandler.vBusStatusChanged(LIN, BUS_DISCONNECTED);

            theApp.bWriteIntoTraceWnd("Error while accessing Hardware.");

            return;
        }

        BOOL bLogIsON;

        CFlags* pFlagLog = theApp.pouGetFlagsPtr();
        if(pFlagLog != NULL)
        {
            bLogIsON = pFlagLog->nGetFlagStatus(LOGTOFILE_LIN);
        }


        if (m_abLogOnConnect[LIN] == TRUE)
        {
            vStartStopLogging_LIN(bConnected);
        }
        // PTV [1.6.6]
        else if(bLogIsON == TRUE && bConnected == TRUE)
        {
            vStartStopLogging_LIN(bLogIsON);
        }


        m_objFlexTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eCONNECTCMD, BUS_CONNECTED);

        m_objFlexTxHandler.vBusStatusChanged(LIN, BUS_CONNECTED);
        GetILINNodeSim()->NS_ManageBusEventHandler(BUS_CONNECT);
        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_NOTIFICATION_FROM_OTHER,
                      eWINID_START_READ, 0);
        // Clear message window on connect
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(LIN);
        if(NULL != hWnd)
        {
            ::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, NULL, NULL);
            ::SendMessage(hWnd, WM_CLEAR_SORT_COLUMN, NULL, NULL);
        }


        if (NS_GetInterface(LIN, (void**) &pNodeSim) == S_OK)
        {
            pNodeSim->NS_nOnBusConnected(TRUE);
        }
    }
    /* If disconnecton is required */
    else
    {

        if (NULL != sg_pouFrameProcLIN)
        {
            sg_pouFrameProcLIN->FPL_DisableLINDataLogFlag();
        }


        g_pouDIL_LIN_Interface->DILL_ResetSlaveRespData();
        hResult = g_pouDIL_LIN_Interface->DILL_StopHardware();
        m_objFlexTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eCONNECTCMD, BUS_DISCONNECTED);
        m_objFlexTxHandler.vBusStatusChanged(LIN, BUS_DISCONNECTED);
        //m_objLINTxHandler.vStopTransmission(0);
        if (NULL != m_podMsgWndThread)
        {
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_STOP_READ, 0);
        }
        GetILINNodeSim()->NS_ManageBusEventHandler(BUS_DISCONNECT);

        if (NS_GetInterface(LIN, (void**) &pNodeSim) == S_OK)
        {
            pNodeSim->NS_nOnBusConnected(FALSE);
        }
    }
    //added to disable loading config while LIN transmission

    if(!bConnected)
    {
        if (NULL != sg_pouFrameProcLIN)
        {
            USHORT ushCount =   sg_pouFrameProcLIN->FPL_GetLoggingBlockCount();
            if(ushCount>0)//check for log file count
            {
                if(sg_pouFrameProcLIN->FPL_IsLoggingON())//if logging is on
                {

                    vStartStopLogging_LIN(FALSE);


                    sg_pouFrameProcLIN->FPL_vCloseLogFile();
                }
            }
        }
    }

    if(bConnected)
    {
        m_bCfgLoadMenuOption    = FALSE;
        m_bCfgNewMenuOption     = FALSE;
    }
    else
    {
        m_bCfgLoadMenuOption    = TRUE;
        m_bCfgNewMenuOption     = TRUE;
    }

    pouFlag->vSetFlagStatus(LIN_CONNECTED, bConnected);
    BYTE bytTbrItemIndex = 1;
    vModifyToolbarIcon( m_wndToolbarLIN, bytTbrItemIndex, bConnected, IDI_ICON_CAN_DISCONNECT, IDI_ICON_CAN_CONNECT );
}

void CMainFrame::OnUpdateLINConnect(CCmdUI* pCmdUI)
{
    // Select the current menu text based on which one between Connect
    // or Disconnect is currently selected. The menu text should show
    // the next available state i.e., if currently "Connect" is displayed,
    // next available state is Disconnect and vice versa. Texts are taken from
    // the string table
    CFlags* pouFlag  = theApp.pouGetFlagsPtr();
    if(pouFlag != NULL)
    {
        BOOL bConnected  = pouFlag->nGetFlagStatus(LIN_CONNECTED);
        UINT unConnected = bConnected ? IDS_DISCONNECT_LIN : IDS_CONNECT_LIN;

        // And initialise a CString with the string.
        CString omMenuItemText("");
        omMenuItemText.Format(unConnected);

        // And finally set the menu text.
        if(pCmdUI !=NULL)
        {
            pCmdUI->SetText(omMenuItemText);
        }
    }
    if ( m_shLINDriverId != DAL_NONE )
    {
        pCmdUI->Enable();
    }
    else
    {
        pCmdUI->Enable(0);
    }
}

/******************************************************************************/
/*  Function Name    :  OnUpdateFileConnect                                   */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  21.10.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002, Review comment incorporated               */
/*  Modification on  :  26.11.2002, Order of connect and disconnect menu text */
/*                                  have been revered                         */
/******************************************************************************/
void CMainFrame::OnUpdateFileConnect(CCmdUI* pCmdUI)
{
    // Select the current menu text based on which one between Connect
    // or Disconnect is currently selected. The menu text should show
    // the next available state i.e., if currently "Connect" is displayed,
    // next available state is Disconnect and vice versa. Texts are taken from
    // the string table
    CFlags* pouFlag  = theApp.pouGetFlagsPtr();
    if(pouFlag != NULL)
    {
        BOOL bConnected  = pouFlag->nGetFlagStatus(CONNECTED);
        UINT unConnected = bConnected ? IDS_DISCONNECT : IDS_CONNECT;

        // And initialise a CString with the string.
        CString omMenuItemText("");
        omMenuItemText.Format(unConnected);
        // And finally set the menu text.
        if(pCmdUI !=NULL)
        {
            pCmdUI->SetText(omMenuItemText);
        }
    }

}
/******************************************************************************/
/*  Function Name    :  OnExecuteKeyhandlers                                  */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework when user select */
/*                      toolbar button/menu for key handlers                  */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  24.10.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002, Review comment incorporated               */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :   05/01/06                                             */
/*                   :  Inserted CExecute manger for executing handlers       */
/******************************************************************************/
void CMainFrame::OnExecuteKeyhandlers()
{
    //Get present status
    BOOL bEnable = GetICANNodeSim()->NS_GetHandlerStatus(H_KEY_HANDLER_ON);
    //Change the status of handler
    GetICANNodeSim()->NS_EnableAllKeyHandler( !bEnable );
}

/******************************************************************************/
/*  Function Name    :  OnExecuteKeyhandlers                                  */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework when user select */
/*                      toolbar button/menu for key handlers                  */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  24.10.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002, Review comment incorporated               */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :   05/01/06                                             */
/*                   :  Inserted CExecute manger for executing handlers       */
/******************************************************************************/
void CMainFrame::OnExecuteKeyhandlersLIN()
{
    //Get present status
    BOOL bEnable = GetILINNodeSim()->NS_GetHandlerStatus(H_KEY_HANDLER_ON);
    //Change the status of handler
    GetILINNodeSim()->NS_EnableAllKeyHandler( !bEnable );
}
/******************************************************************************/
/*  Function Name    :  OnUpdateExecuteKeyhandlers                            */
/*  Input(s)         :  CCmdUI* pCmdU                                         */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  24.10.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002, Review comment incorporated               */
/******************************************************************************/
void CMainFrame::OnUpdateExecuteKeyhandlers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetICANNodeSim()->NS_GetHandlerStatus(H_KEY_HANDLER_ON));
    BOOL bEnable = GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

/******************************************************************************/
/*  Function Name    :  OnUpdateExecuteKeyhandlers                            */
/*  Input(s)         :  CCmdUI* pCmdU                                         */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  24.10.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  31.10.2002, Review comment incorporated               */
/******************************************************************************/
void CMainFrame::OnUpdateExecuteKeyhandlersLIN(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetILINNodeSim()->NS_GetHandlerStatus(H_KEY_HANDLER_ON));
    BOOL bEnable = GetILINNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

/******************************************************************************/
/*  Function Name    :  OnLoadConfigFile                                      */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user wants to load a configuration    */
/*                      file. The user chooses the file through the Open File */
/*                      Dialog and then calls the App object's                */
/*                      nLoadConfiguration(..) method to load the information */
/*                      from the file. If there is any error, then a message  */
/*                      box is displayed.                                     */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications    :  29.11.2002, Amarnath S                                */
/*                      Commented AfxMessageBox(...) function call            */
/*  Modifications    :  02.05.2003, Amitesh Bharti                            */
/*                      used CSplFileDlg class to customise the buttong text. */
/*                      Also the flag is changed to close the dialog only if  */
/*                      file selected exists.                                 */
/*  Modifications    :  06.05.2003, Amitesh Bharti                            */
/*                      Update all window coordinates from configuration file */
/*  Modifications    :  20.01.2004, Amitesh Bharti                            */
/*                      configuration file name in m_omStrSavedConfigFile is  */
/*                      set to null and before loading if any database file is*/
/*                      it is close after prompt of save.Win coordinate is    */
/*                      updated in any case of failure or success.            */
/*  Modifications    :  16.03.2004, Raja N                                    */
/*                      Seprated config load functionality in to a seprate    */
/*                      function m_omStrSavedConfigFile. And removed call to  */
/*                      close the database                                    */
/*  Modifications    :  28.04.2004, Raja N                                    */
/*                      Replaced hardcoded string with                        */
/*                      defSTR_CONF_SAVE_CONFIRM                              */
/*  Modifications    :  02.05.2005, Raja N
                        Moved window save functionality to the function
                        vSaveWindowPostion and made a call to this function
    Modifications    :  Raja N on 19.07.2005, Added code to close logging before
                        changing the configuration file
/******************************************************************************/
void CMainFrame::OnLoadConfigFile()
{
    if ( bIsConfigurationModified() == TRUE )
    {
        // Yes, there is a loaded config and changes have been made
        // to it.
        // Hence display save confirmation message dialog
        if (AfxMessageBox(_(defSTR_CONF_SAVE_CONFIRM), MB_YESNO | MB_ICONQUESTION )
                == IDYES )
        {
            // Save
            OnSaveConfigFile();
        }
    }
    CSplFileDlg oCfgFileDlg(TRUE,       // Open dialog as Open File dlg
                            defFILEEXT,  // default extension
                            NULL,       // default file name
                            OFN_HIDEREADONLY
                            |OFN_EXTENSIONDIFFERENT
                            |OFN_FILEMUSTEXIST, // mode
                            _(defCONFIGFILTER), // filter
                            NULL,        // parent wnd
                            _("Load"));
    oCfgFileDlg.m_ofn.lpstrTitle = _T(_("Load Configuration Filename..."));

    if(oCfgFileDlg.DoModal() == IDOK)
    {
        // get the name of the selected file
        CString oCfgFilename = oCfgFileDlg.GetPathName();
        nLoadConfigFile(oCfgFilename);

        // Close the J1939 and FlexRay message windows
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)J1939);
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)FLEXRAY);
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);


    }
}

/******************************************************************************/
/*  Function Name    :  OnNewConfigFile                                       */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user wants to create a new config file*/
/*                      This file will have default values.                   */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications    :  29.11.2002, Amarnath S                                */
/*                      Commented AfxMessageBox(...) function call            */
/*  Modifications    :  02.05.2003, Amitesh Bharti                            */
/*                      used CSplFileDlg class to customise the buttong text  */
/*  Modifications    :  06.05.2003, Amitesh Bharti                            */
/*                      Update all window coordinates from configuration file */
/*  Modifications    :  20.01.2004, Amitesh Bharti                            */
/*                      configuration file name in m_omStrSavedConfigFile is  */
/*                      set to null and before loading if any database file is*/
/*                      it is close after prompt of save.Win coordinate is    */
/*                      updated in any case of failure or success.            */
/*  Modifications    :  14.04.2004, Raja N                                    */
/*                   :  Incleded code to save Signal Watch Window and Message */
/*                      Inpterpretation Window coordinates. Also added code to*/
/*                      Clear Signal Watch Window, Message Interpretation     */
/*                      Window and Message Window                             */
/*  Modifications    :  07.09.2004, Raja N                                    */
/*                   :  Modified code to refer HI layer for device independent*/
/*                      API calls. HI layer will keep controller information  */
/*                      Previously ChangeRegister class will hold this data   */
/*  Modifications    :  02.05.2005, Raja N
                        Moved window save functionality to the function
                        vSaveWindowPostion and made a call to this function
    Modifications    :  Raja N on 19.07.2005, Changed the code to remove log
                        trigger update
/******************************************************************************/
void CMainFrame::OnNewConfigFile()
{
    if ( bIsConfigurationModified() == TRUE )
    {
        // Yes, there is a loaded config and changes have been made
        // to it.
        // Hence display save confirmation message dialog
        if (AfxMessageBox( _(defSTR_CONF_SAVE_CONFIRM), MB_YESNO | MB_ICONQUESTION)
                == IDYES )
        {
            // Save
            OnSaveConfigFile();
        }
    }

    CSplFileDlg oCfgFileDlg(FALSE,       // Save  as dialog
                            defFILEEXT,  // default extension
                            NULL,       // default file name
                            defDLGFLAGS, // mode
                            _(defCONFIGFILTER), // filter
                            NULL,        // parent wnd
                            _("Open"));

    oCfgFileDlg.m_ofn.lpstrTitle = _T(_("New Configuration Filename..."));

    if(oCfgFileDlg.DoModal() == IDOK)
    {
        // PTV [1.6.5] 23
        CConfigData::ouGetConfigDetailsObject().vCloseConfigFile();
        // PTV [1.6.5] 23

        // get the name of the selected file
        CString oCfgFilename = oCfgFileDlg.GetPathName();
        vSetFileStorageInfo(oCfgFilename);
        vSetCurrProjInfo((FLOAT)BUSMASTER_APPN_VERSION_LATEST);
        CConfigData::ouGetConfigDetailsObject().vSaveConfigFile();
        CConfigData::ouGetConfigDetailsObject().vReadConfigFile();

        for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
        {
            vSetCurrentSessionData(eSecId, NULL, 0);
            eSecId = static_cast<eSECTION_ID>(eSecId + 1);
        }
        SaveConfiguration();

        // On New Configuration Stop Logging if it is enabled for CAN
        BOOL bLogON = FALSE;
        if (NULL != sg_pouFrameProcCAN)
        {
            bLogON = sg_pouFrameProcCAN->FPC_IsLoggingON();
        }
        if ( bLogON )
        {
            bLogON = bLogON ? FALSE : TRUE;
            vStartStopLogging( bLogON );
        }


        BOOL bLogONl = FALSE;
        if (NULL != sg_pouFrameProcLIN)
        {
            bLogONl = sg_pouFrameProcLIN->FPL_IsLoggingON();
        }
        if ( bLogONl )
        {
            bLogONl = bLogONl ? FALSE : TRUE;
            vStartStopLogging_LIN( bLogONl );
        }


        // setting by default Hex mode on new configuration
        bSetHexDecFlags(TRUE);
        // On New Configuration Stop Logging if it is enabled for J1939
        vJ1939StartStopLogging();

        /* Update checked state for the filters(Log, Message) */
        ApplyLogFilter();
        ApplyMessageFilterButton();

        /* Switch off Replay filter and update*/
        CFlags* pouFlags = NULL;
        pouFlags = theApp.pouGetFlagsPtr();
        pouFlags->vSetFlagStatus(REPLAYFILTER, FALSE);
        ApplyReplayFilter();
        // Clear message window on connect
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY);
        if(NULL != hWnd)
        {
            ::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, NULL, NULL);
            ::SendMessage(hWnd, WM_CLEAR_SORT_COLUMN, NULL, NULL);
        }

        /*Unload FLEXRAY DIL */
        m_shFLEXRAYDriverId = DAL_NONE;
        InitializeFLEXRAYDIL();
        m_ouClusterConfig[FLEXRAY].InitialiseConfig();

        /*LIN*/
        m_shLINDriverId = DAL_LIN_NONE;
        IntializeDILL();
        m_ouClusterConfig[LIN].InitialiseConfig();

        // Close the J1939 and FlexRay message windows
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)J1939);
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)FLEXRAY);
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);




        /* Clear FlexRay Database/configuration files */
        m_acFlexDBConfigInfo.m_acConfigFileName[0] = "";
        m_acFlexDBConfigInfo.m_acConfigFileName[1] = "";
        m_acFlexDBConfigInfo.m_nFilesCount = 0;
        m_acFlexDBConfigInfo.m_nSelectedIndex = 0;
    }
}
void CMainFrame::vInitialiaseLINConfig(int nChannel)
{
    IntializeDILL();
    m_ouClusterConfig[LIN].InitialiseConfig();
    m_ouClusterConfig[LIN].m_nChannelsConfigured = nChannel;
    for ( int i = 0 ; i < nChannel; i++ )
    {
        m_ouClusterConfig[LIN].m_nChannelsConfigured = 1;
        m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_bOverWriteSettings = true;
        m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_nBaudRate = 19200;
        m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_strProtocolVersion = "LIN 2.0";
    }
    m_shLINDriverId = DAL_LIN_NONE;



}


void CMainFrame::vGetLoadedCfgFileName(CString& omFileName)
{
    DATASTORAGEINFO sDataStorageInfo;
    CConfigData::ouGetConfigDetailsObject().GetConfigDatastorage(&sDataStorageInfo);
    if (sDataStorageInfo.FSInfo != NULL)
    {
        omFileName.Format("%s", T2A(sDataStorageInfo.FSInfo->m_FilePath));
        delete sDataStorageInfo.FSInfo;
    }
}
/******************************************************************************/
/*  Function Name    :  OnSaveConfigFile                                      */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user wants to load a configuration    */
/*                      file. The user chooses the file through the Open File */
/*                      Dialog and then calls the App object's                */
/*                      nLoadConfiguration(..) method to load the information */
/*                      from the file. If there is any error, then a message  */
/*                      box is displayed.                                     */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications    :  29.11.2002, Amarnath S                                */
/*                      Commented AfxMessageBox(...) function call            */
/*  Modifications    :  20.01.2004, Amitesh Bharti                            */
/*                      configuration file name in m_omStrSavedConfigFile is  */
/*                      set to selected file name before saving it . This is  */
/*                      when user is trying to save default configuration and */
/*                      there is no file selected. Set all coordinates before */
/*                      save.                                                 */
/*  Modifications    :  14.04.2004, Raja N                                    */
/*                   :  Incleded code to save Signal Watch Window and Message */
/*                      Inpterpretation Window coordinates.                   */
/*  Modifications    :  02.05.2005, Raja N                                    */
/*                      Moved window save functionality to the function       */
/*                      vSaveWindowPostion and made a call to this function and
/*                      to restore window position using the function         */
/*                      vRestoreWindowPostion                                 */
/******************************************************************************/
void CMainFrame::OnSaveConfigFile()
{
    // get the name of the loaded config file only if no filename has
    // been specified until now..
    CString omStrCfgFilename;
    vGetLoadedCfgFileName(omStrCfgFilename);

    if(omStrCfgFilename.IsEmpty() == TRUE && bIsConfigurationModified())
    {
        CFileDialog oCfgFileDlg(FALSE,      // Open dialog as Save as File dlg
                                defFILEEXT,  // default extension
                                NULL,       // default file name
                                defDLGFLAGS, // mode
                                _(defCONFIGFILTER), // filter
                                NULL        // parent wnd
                               );
        oCfgFileDlg.m_ofn.lpstrTitle = _T(_("Save Configuration File"));
        if(oCfgFileDlg.DoModal() == IDOK)
        {
            // get the name of the selected file
            omStrCfgFilename = oCfgFileDlg.GetPathName();
            // This call alone is sufficient to save the
            // configuration file.
            vSetFileStorageInfo(omStrCfgFilename);
            if ( SaveConfiguration() ==
                    defCONFIG_FILE_SUCCESS )
            {
                vPushConfigFilenameDown(omStrCfgFilename);
                bWriteIntoRegistry(HKEY_CURRENT_USER, _(SECTION), defCONFIGFILENAME, REG_SZ, omStrCfgFilename);
                m_omStrSavedConfigFile = omStrCfgFilename;
            }
        }
    }
    else // ostrCfgFilename has some value...
    {
        if(SaveConfiguration() == defCONFIG_FILE_SUCCESS)
        {
            vPushConfigFilenameDown(omStrCfgFilename);

            // PTV [1.6.4] 6
            bWriteIntoRegistry(HKEY_CURRENT_USER, _(SECTION), defCONFIGFILENAME, REG_SZ, omStrCfgFilename);
            m_omStrSavedConfigFile = omStrCfgFilename;
            // PTV [1.6.4] 6
        }
    }
}

/******************************************************************************/
/*  Function Name    :  OnSaveAsConfigFile                                    */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user wants to save the configuration  */
/*                      information into a new file. The method calls the     */
/*                      App's nSaveConfiguration(..) method to save           */
/*                      information.                                          */
/*                      If there is any error, then a message box is displayed*/
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  12.11.2002                                            */
/*  Modifications    :  29.11.2002, Amarnath S                                */
/*                      Commented AfxMessageBox(...) function call            */
/******************************************************************************/
void CMainFrame::OnSaveAsConfigFile()
{
    CFileDialog oCfgFileDlg(FALSE,      // Open dialog as Save as File dlg
                            defFILEEXT,  // default extension
                            NULL,       // default file name
                            defDLGFLAGS , // mode
                            _(defCONFIGFILTER), // filter
                            NULL        // parent wnd
                           );
    oCfgFileDlg.m_ofn.lpstrTitle = _T(_("SaveAs Configuration Filename..."));

    if(oCfgFileDlg.DoModal() == IDOK)
    {
        // get the name of the selected file
        CString oCfgFilename = oCfgFileDlg.GetPathName();
        // Save the file
        CConfigData::ouGetConfigDetailsObject().vCloseConfigFile();
        vSetFileStorageInfo(oCfgFilename);
        SaveConfiguration();
    }
}

/******************************************************************************
 Function Name    : OnClickMruList
 Input(s)         : UINT unID - ID of the selected menu item
 Output           : void
 Functionality    : - Called by the framework when the user selects
                    one of the MRU filename
 Member of        : CMainFrame
 Friend of        :     -
 Author(s)        : Amarnath S
 Date Created     : 18.11.2002
 Modifications    : 29.11.2002, Amarnath S
                    Commented AfxMessageBox(...) function call
 Modifications    : 16.03.2004, Raja N
                    Introduced the call to nLoadConfigurationFile to load
                    the selected configuration file & checked for active
                    connection to prevent loading during transmission
 Modifications    : 28.04.2004, Raja N
                    Removed the check for connection state as it is not possible
                    now. The MRU items will be disabled if the tool is connected
 Modifications    : 02.05.2005, Raja N
                    Added call to the function vSaveWindowPostion to save window
                    position before loading a new configuration file
 Modifications    : Raja N on 19.07.2005, Added code to stop logging before
                    loading the new configuration file
*******************************************************************************/
void CMainFrame::OnClickMruList (UINT unID)
{
    // Get the main menu
    CFlags* pFlags = NULL;
    pFlags = theApp.pouGetFlagsPtr();
    if( pFlags != NULL)
    {
        if(pFlags->nGetFlagStatus(DLLLOADED) == FALSE)
        {
            CMenu* pMenu = GetMenu();
            if ( pMenu != NULL)
            {
                // Get the string associated with the selected menu item
                CString omStrName(STR_EMPTY);
                pMenu->GetMenuString( unID, omStrName, MF_BYCOMMAND);
                // Let the selected filename be the first file name in the MRU
                if (!omStrName.IsEmpty() && omStrName != _(defSTR_DEFAULT_MRU_ITEM) )
                {
                    // Check if any previous configuration file is loaded
                    // and changes are being made to it
                    if ( bIsConfigurationModified() == TRUE )
                    {
                        // Yes, there is a loaded config and changes have been made to
                        // it Hence display save confirmation message dialog
                        if (AfxMessageBox( _(defSTR_CONF_SAVE_CONFIRM),
                                           MB_YESNO | MB_ICONQUESTION ) == IDYES )
                        {
                            // Save
                            OnSaveConfigFile();
                        }
                    }
                    // Get the flag status and stop logging if it already started
                    BOOL bLogON = FALSE;
                    // Get the main menu
                    CFlags* pFlag = NULL;
                    pFlag = theApp.pouGetFlagsPtr();
                    if(pFlag != NULL)
                    {
                        bLogON = pFlag->nGetFlagStatus(LOGTOFILE);
                        if( bLogON == TRUE )
                        {
                            vStartStopLogging( FALSE );
                            //pFlag->vSetFlagStatus( LOGTOFILE, FALSE );
                        }
                    }

                    // Call Configuration Load
                    nLoadConfigFile(omStrName);
                }
            }
        }
        else
            AfxMessageBox(_(defIMPORT_DLLLOAD_WARNNING),
                          MB_OK|MB_ICONINFORMATION);
    }
}

/******************************************************************************
 Function Name    : OnUpdateMruList
 Input(s)         : -
 Output           : void
 Functionality    : - Equivalent to OnUpdateCommandUI
                    - Modify's the text associated with the MRU menu items
 Member of        : CMainFrame
 Friend of        :     -
 Author(s)        : Amarnath S
 Date Created     : 18.11.2002
 Modifications    : Raja N on 28.04.2004
                    Modified the UI Update logic to use the pCmdUI object and
                    included connect status to disable the menu item.
 Modifications    : Raja N on 11.08.2004
                    Modified the UI Update to check whether Tx dialog is active
                    or not. If it is active the config can not be loaded.
 Modifications    : Raja N on 26.04.2005
                    Modified code to refer new Tx message configuration window
                    pointer to check the presence of Tx configuration window
******************************************************************************/
void CMainFrame::OnUpdateMruList (CCmdUI* pCmdUI)
{
    CFlags* pouFlag = NULL;
    BOOL bConnect = FALSE;
    // Get MRU item index
    UINT unIndex = pCmdUI->m_nID - IDM_REC_CFG_FILE1;
    // Get MRU file name from configuration module
    pouFlag     = theApp.pouGetFlagsPtr();
    pCmdUI->SetText(m_omStrMRU_ConfigurationFiles[unIndex]);
    if(pouFlag != NULL)
    {
        // Check whether Tx msg window is present or not
        //BOOL bSendMsgDialog = ( m_pomTxMsgChildWindow != NULL );
        BOOL bSendMsgDialog = FALSE;;

        if(m_objTxHandler.hConfigWindowShown() == S_OK)
        {
            bSendMsgDialog = TRUE;
        }

        // Check connection status
        bConnect    = pouFlag->nGetFlagStatus(CONNECTED);
        BOOL bFlexConnect =  pouFlag->nGetFlagStatus(FLEX_CONNECTED);
        //BOOL bLINConnect =  pouFlag->nGetFlagStatus(LIN_CONNECTED);
        BOOL bFlexConfigShown = FALSE;
        if(m_objFlexTxHandler.hConfigWindowShown(FLEXRAY) == S_OK)
        {
            bFlexConfigShown = TRUE;
        }

        BOOL bLINConnect =  pouFlag->nGetFlagStatus(LIN_CONNECTED);
        BOOL bLINTxConfigWndShown = FALSE;
        if (m_objFlexTxHandler.hConfigWindowShown(LIN)== S_OK)
        {
            bLINTxConfigWndShown = TRUE;
        }
        // Disable menu if the tool is connected or Tx msg window is active or
        // MRU list is empty
        if( bConnect == TRUE ||
                bSendMsgDialog == TRUE ||
                m_omStrMRU_ConfigurationFiles[0] == _(defSTR_DEFAULT_MRU_ITEM) ||
                bFlexConnect == TRUE || bFlexConfigShown == TRUE ||
                bLINConnect == TRUE || bLINTxConfigWndShown == TRUE)
        {
            pCmdUI->Enable(FALSE);
        }
    }
}
/*******************************************************************************
 Function Name    : vPushConfigFilenameDown
 Input(s)         : omStrConfigFilename - file name at the top of the list
 Output           : void
 Functionality    : Simulates MRU functionality
 Member of        : CMainFrame
 Friend of        :    -
 Author(s)        : Amarnath S
 Date Created     : 18.11.2002
 Modifications    : 27.11.2002, Amarnath Shastry
                    Added "pSubSubMenu->GetMenuItemCount()"
 Modifications    : 02.01.2003, Amarnath S
                    Empty text introduced if no MRU are found
 Modifications    : 28.04.2004, Raja N
                    Use of pomGetMRUMenuPointer function to get the pointer of
                    "Recent Files" menu item instead of directly getting the
                    pointer from menubar.
*******************************************************************************/
void CMainFrame::vPushConfigFilenameDown(CString omStrConfigFilename )
{
    // The MRU list should have "omStrConfigFilename" file name
    // as the first element.
    // Hence u need to push down rest of the elements by one
    // position
    // If the filename is new then, the last element in the
    // list will be lost
    // Perform only for valid config file
    if ( omStrConfigFilename != STR_EMPTY )
    {
        if (m_omStrMRU_ConfigurationFiles[0] == defSTR_DEFAULT_MRU_ITEM)
        {
            m_omStrMRU_ConfigurationFiles[0] = omStrConfigFilename;
        }
        else
        {
            CString omStrNames[5];
            INT unIndex = -1;
            UINT unMaxCountIndex = 5;// Push all the elements down

            // Make a temp copy of all the MRU's
            UINT unCount;
            for (unCount = 0;                   //for (UINT unCount = 0; unCount defined out of for loop
                    unCount < 5; unCount++)
            {
                omStrNames[unCount] = m_omStrMRU_ConfigurationFiles[unCount];
                // Store the count
                if ( omStrNames[unCount] == omStrConfigFilename )
                {
                    unIndex = unCount;
                }
            }

            if ( unIndex != -1 )// If this is not newly loaded file
            {
                // Push down the mru files untill saved index
                unMaxCountIndex = unIndex;
            }

            // MRU files gets shuffled here
            for ( unCount = 0;
                    unCount < unMaxCountIndex; unCount++)
            {
                m_omStrMRU_ConfigurationFiles[unCount + 1] =
                    omStrNames[unCount];
            }

            // index zero always has selected file
            m_omStrMRU_ConfigurationFiles[0] =
                omStrConfigFilename;

            // if this is new file name, create the
            // menu for the same
            // Get application main menu
            if ( unIndex == -1 )
            {
                // Get the menu pointer of "Recent Configuration"
                CMenu* pMenu = pomGetMRUMenuPointer();

                if ( pMenu != NULL )// Verify
                {
                    INT nMenuCount = pMenu->GetMenuItemCount();
                    if ( nMenuCount < 5 )
                    {
                        // Create an item with empty string. UI Update will
                        // set the text appropriatly
                        UINT unMenuID = IDM_REC_CFG_FILE1;
                        pMenu->AppendMenu( MF_STRING | MF_ENABLED,
                                           unMenuID + nMenuCount, STR_EMPTY);
                    }
                }
            }
        }
    }
}

/******************************************************************************
    Function Name    :  OnUpdateConfigSaveAs
    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  Enables/Disables Load menu option for configuration
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath S
    Date Created     :  22.11.2002
    Modifications    :  Raja N on 26.04.2005, Modified code refer new Tx window
                        pointer to check Tx msg window presence
******************************************************************************/
void CMainFrame::OnUpdateConfigLoad(CCmdUI* pCmdUI)
{
    if ( pCmdUI != NULL )
    {
        BOOL bEnable = FALSE;
        // Check whether Tx msg window is visible or not
        //BOOL bSendMsgDialog = (m_pomTxMsgChildWindow != NULL );
        BOOL bSendMsgDialog = FALSE;;

        if(m_objTxHandler.hConfigWindowShown() == S_OK)
        {
            bSendMsgDialog = TRUE;
        }
        if(m_objFlexTxHandler.hConfigWindowShown(FLEXRAY) == S_OK)
        {
            bSendMsgDialog = TRUE;
        }
        if (m_objFlexTxHandler.hConfigWindowShown(LIN) == S_OK)
        {
            bSendMsgDialog = TRUE;
        }

        // Enable only if Tx window is not present
        bEnable = m_bCfgLoadMenuOption && !bSendMsgDialog;
        pCmdUI->Enable( bEnable );
    }
}
/******************************************************************************
    Function Name    :  OnUpdateConfigSaveAs
    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  Enables/Disables New menu option for configuration
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath S
    Date Created     :  22.11.2002
    Modifications    :  Raja N on 26.04.2005, Modified code refer new Tx window
                        pointer to check Tx msg window presence
******************************************************************************/
void CMainFrame::OnUpdateConfigNew(CCmdUI* pCmdUI)
{
    if ( pCmdUI != NULL )
    {
        BOOL bEnable = FALSE;
        //BOOL bSendMsgDialog = (m_pomTxMsgChildWindow != NULL );
        BOOL bSendMsgDialog = FALSE;;

        if(m_objTxHandler.hConfigWindowShown() == S_OK)
        {
            bSendMsgDialog = TRUE;
        }
        if(m_objFlexTxHandler.hConfigWindowShown(FLEXRAY) == S_OK)
        {
            bSendMsgDialog = TRUE;
        }
        if (m_objFlexTxHandler.hConfigWindowShown(LIN) == S_OK)
        {
            bSendMsgDialog = TRUE;
        }

        bEnable = m_bCfgNewMenuOption && !bSendMsgDialog;
        pCmdUI->Enable( bEnable );
    }
}
/******************************************************************************
    Function Name    :  OnUpdateConfigSaveAs
    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  Enables/Disables Save menu option for configuration
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath S
    Date Created     :  22.11.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateConfigSave(CCmdUI* pCmdUI)
{
    if ( pCmdUI != NULL )
    {
        pCmdUI->Enable( m_bCfgSaveMenuOption );
    }
}
/******************************************************************************
    Function Name    :  OnUpdateConfigSaveAs
    Input(s)         :  CCmdUI* pCmdUI
    Output           :  void
    Functionality    :  Enables/Disables SaveAs menu option for configuration
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amarnath S
    Date Created     :  22.11.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateConfigSaveAs(CCmdUI* pCmdUI)
{
    if ( pCmdUI != NULL )
    {
        pCmdUI->Enable( m_bCfgSaveAsMenuOption );
    }
}
/******************************************************************************
    Function Name    :  OnFilePropeties
    Input(s)         :
    Output           :
    Functionality    :  Called by the framework when the user selects
                        File->Propeties. It will display the name and location
                        of files( Active Database File ,DLL Loaded,
                        Configuration File, Baud Rate,Selected Log File,Selected
                        Replay File.
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  18.11.2002
    Modifications    :  Amitesh Bharti
                        14.12.2002, All pointers are check before using. Proper
                        error message is formatted if pointer are not valid.
    Modifications    :  Raja N
                        07.03.2004, Modified to support multi channel support
                        in the properties dialog.
    Modifications    :  Raja N
                        07.03.2004, Modified to support multiple log and replay
                        file list
******************************************************************************/
void CMainFrame::OnFilePropeties()
{
    CStringArray omStrArray;
    SPROPERTIESLIST sProperties;
    CString omStrFomat( STR_EMPTY );
    BOOL bDLLLoaded = FALSE;
    CFlags* pouFlag = NULL;

    // Get the CFlag Object pointer
    pouFlag     = theApp.pouGetFlagsPtr();
    if(pouFlag != NULL)
    {
        // Get the DLL load status
        bDLLLoaded = pouFlag->nGetFlagStatus(DLLLOADED);
    }

    // Get Controller information

    PSCONTROLLER_DETAILS  pBaudDetails = NULL;
    UINT nHardware = 0;
    LONG lParam = 0;
    if (g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
    {
        nHardware = (UINT)lParam;
    }
    pBaudDetails = m_asControllerDetails;
    // If valid
    if(pBaudDetails != NULL)
    {
        for( UINT unIndex = 0; unIndex < nHardware; unIndex++ )
        {
            // Format baud rate information
            float fBaudRate = atof(pBaudDetails[ unIndex ].m_omStrBaudrate.c_str());
            if( fBaudRate > 1000)
            {
                fBaudRate = fBaudRate/1000;
            }
            std::stringstream ss;
            ss << fBaudRate;
            omStrFomat.Format( _(defSTR_CHANNEL_ITEM_FORMAT),
                               unIndex + 1,
                               (ss.str()).c_str());
            // Append "*" if the channel is not present
            if( static_cast<int>(unIndex) > ( nHardware - 1 ) )
            {
                omStrFomat += _(defSTR_CHANNEL_NOT_PRESENT);
            }
            // Add the formatted entry
            sProperties.m_omBaudRate.Add(omStrFomat);
        }
    }
    else
    {
        // Add Error Information
        omStrFomat = _(defSTR_ERROR_GETTTING_BAUDRATE);
        sProperties.m_omBaudRate.Add(omStrFomat);
    }

    // Get Log file information
    if (sg_pouFrameProcCAN != NULL)
    {
        UINT unCount = sg_pouFrameProcCAN->FPC_GetLoggingBlockCount();
        for (UINT i = 0; i < unCount; i++)
        {
            SLOGINFO sLogInfo;
            if (sg_pouFrameProcCAN->FPC_GetLoggingBlock((USHORT)i, sLogInfo) == S_OK)
            {
                sProperties.m_omLogFile.Add(sLogInfo.m_sLogFileName);
            }

        }
    }
    // If no log file is selection then add not selected string to the list
    if( sProperties.m_omLogFile.GetSize() == 0)
    {
        sProperties.m_omLogFile.Add( _(defSTR_NOT_SELECTED) );
    }

    // Get Replay File List
    vREP_GetReplayFileNameList(sProperties.m_omReplayFile );
    if( sProperties.m_omReplayFile.GetSize() == 0)
    {
        sProperties.m_omReplayFile.Add( _(defSTR_NOT_SELECTED) );
    }

    sProperties.m_omDatabaseFile.RemoveAll();
    CStringArray aomDataBaseFiles;
    // Get Database File Name
    theApp.m_pouMsgSignal->vGetDataBaseNames(&aomDataBaseFiles);
    // If Success
    // If It is a valid file name
    if( aomDataBaseFiles.GetSize() != 0 )
    {
        // Assign the file name
        sProperties.m_omDatabaseFile.Append(aomDataBaseFiles);
    }
    else
    {
        // Else use not imported
        sProperties.m_omDatabaseFile.Add(_(defSTR_NOT_IMPORTED));
    }

    // Get the DLL information
    omStrFomat = _(defSTR_DLL_NOT_LOADED);
    if( bDLLLoaded == TRUE )
    {
        // Get the loaded DLL names
        /* CSimSysNodeInfo* pSimSysNodeInfo = theApp.pomGetSimSysNodeInfo();
         if( pSimSysNodeInfo != NULL)
         {
             pSimSysNodeInfo->vGetLoadedDlls( sProperties.m_omDLLFile );

         }*/

        /*  theApp.bGetData( MRU_DLL_FILE_NAME, (VOID**)&pomStrFileName );
          // IF success
          if( pomStrFileName != NULL )
          {
              // It is a valid name
              if( (*pomStrFileName).IsEmpty() == FALSE )
              {
                  // Assign the file name
                  omStrFomat = *pomStrFileName;
              }
              // Clear the memory allocated to get DLL file name
              theApp.vRelease(MRU_DLL_FILE_NAME,(VOID**)&pomStrFileName);
          }*/
        /*else
         {
             omStrFomat = defSTR_ERROR_GETTTING_DLL_INFO;
             sProperties.m_omDLLFile.Add(omStrFomat);
         }*/
    }
    // Add the DLL information
    // sProperties.m_omDLLFile.Add(omStrFomat);

    // Create properties dialog and
    CProperties odProperties( sProperties );
    // Show the dialog
    odProperties.DoModal();
}

/******************************************************************************
 Function Name    : vCreateMRU_Menus
 Input(s)         : -
 Output           : void
 Functionality    : Set the text for the first sub menu item under
                    "File -> Recent Configurations -> Empty" menu option
                    The first sub menu removed.Then create menu for the rest
                    of the MRU files.
 Member of        : CMainFrame
 Friend of        :     -
 Author(s)        : Amarnath S
 Date Created     : 14.12.2002
 Modifications    : Raja N on 28.04.2004
                    Modified the getting menu pointer portion as it has been
                    moved to a seprate function pomGetMRUMenuPointer();
******************************************************************************/
void CMainFrame::vCreateMRU_Menus()
{
    // Get application main menu
    CMenu* pMenu = pomGetMRUMenuPointer();
    if ( pMenu != NULL )// Verify
    {
        // if the first MRU file is empty then, "Empty" text shud be displayed
        if ( m_omStrMRU_ConfigurationFiles[0] != STR_EMPTY &&
                m_omStrMRU_ConfigurationFiles[0] != _(defSTR_DEFAULT_MRU_ITEM) )
        {
            // Remove the static item
            UINT unMenuID = IDM_REC_CFG_FILE1;
            // Create menu's to hold rest of MRU configuration files
            for (UINT unCount = 0; unCount < 5; unCount++ )
            {
                // Create menu only if string is not empty
                if ( m_omStrMRU_ConfigurationFiles[unCount] != STR_EMPTY )
                {
                    pMenu->RemoveMenu( unMenuID + unCount, MF_BYCOMMAND);
                    pMenu->AppendMenu(MF_STRING, unMenuID + unCount,
                                      m_omStrMRU_ConfigurationFiles[unCount] );
                }
                else
                {
                    //break the loop
                    unCount = 5;
                }
            }
        }
    }
}


/******************************************************************************
    Function Name    :  OnTimer

    Input(s)         :  nIDEvent - Identifier of the timer.
    Output           :  -
    Functionality    :  This timer handler is used to update the status bar to
                        show the current error states and values of both Rx and
                        Tx error values. This also updates the bus statistics
                        window.
    Member of        :  CMainFrame

    Author(s)        :  Ratnadip Choudhury
    Date Created     :
    Modifications    :  Raja N on 22.07.2004
                        Added check for connection status before updating the
                        Network statistics dialog
    Modifications    :  Raja N on 08.09.2004
                        Modified code to refer HI layer function to get error
                        counter value.As of now error counters are not supported
                        by USB driver. So this function call will return -1 to
                        avoid further processing.
    Modifications    :  Raja N on 10.12.2004
                        Added code to Update Graph control with network
                        statistics parameters
    Modifications    :  Raja N on 14.03.2005
                        Added code to support multi channel in the error counter
                        update procedure
******************************************************************************/
void CMainFrame::OnTimer(UINT nIDEvent)
{
    if (nIDEvent == m_unTimerSB)
    {
        if ((theApp.pouGetFlagsPtr()->nGetFlagStatus(CONNECTED)) == TRUE)
        {
            vUpdateGraphStatsData();
            // Update the bus statistics window if it exists.
            if ( m_podBusStatistics != NULL &&
                    m_podBusStatistics->IsWindowVisible( ) == TRUE )
            {
                // Perform network statistics calculation and update of
                // network statistics window
                m_podBusStatistics->SendMessage(WM_DISPLAY_MESSAGE, 1, 0);
            }

            // Get Present error counter value and take
            // appropriate actions for error handler execution
            // Get the number of channels available
            UINT unTotalChannels = 0;
            LONG lParam = 0;
            if (g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
            {
                unTotalChannels = (UINT)lParam;
            }
            for( UINT unChannel = 0; unChannel < unTotalChannels; unChannel++ )
            {
                // Get the error counter status from the HI layer
                if( g_pouDIL_CAN_Interface->DILC_GetErrorCount( m_sErrorCount, unChannel, ERR_CNT) == S_OK)
                {
                    // Make Channel specific error code
                    WORD nErrorWord = MAKEWORD( ERROR_BUS , unChannel );
                    // Call error handler function to process
                    OnErrorMessageProc( nErrorWord,
                                        MAKEWORD(m_sErrorCount.m_ucRxErrCount,
                                                 m_sErrorCount.m_ucTxErrCount));
                }
            }
        }
    }
    if(nIDEvent == m_unTimerSBLog)
    {
        if (NULL != sg_pouFrameProcCAN)
        {
            if(sg_pouFrameProcCAN->FPC_IsDataLogged() == TRUE)
            {
                /*m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");*/
                if(m_bIconSetFlag == 1)
                {
                    m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogIcon2);
                    m_bIconSetFlag = 2;
                }
                else if(m_bIconSetFlag == 2)
                {
                    m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogIcon1);
                    m_bIconSetFlag = 1;
                }

                for ( int i = 0 ; i < 100 ; i++ )
                {
                    if ( sg_pouFrameProcCAN->FPC_IsThreadBlocked() == TRUE )
                    {
                        m_nSendMsgLogCnt++;
                    }
                    else
                    {
                        m_nSendMsgLogCnt = 0;
                        break;
                    }
                }
                if ( m_nSendMsgLogCnt > 98 )
                {
                    m_nSendMsgLogCnt = 0;
                    sg_pouFrameProcCAN->FPC_DisableDataLogFlag();
                }
            }
            else
            {
                m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
            }
        }
        else
        {
            m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
        }
    }
    if(nIDEvent == m_unJ1939TimerSBLog)
    {
        if (NULL != sg_pouIJ1939Logger)
        {
            if(sg_pouIJ1939Logger->FPJ1_IsJ1939DataLogged() == TRUE)
            {
                if(m_bJ1939IconSetFlag == 1)
                {
                    m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_J1939_LOG_ICON, m_hLogIcon2);
                    m_bJ1939IconSetFlag = 2;
                }
                else if(m_bJ1939IconSetFlag == 2)
                {
                    m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_J1939_LOG_ICON, m_hLogIcon1);
                    m_bJ1939IconSetFlag = 1;
                }

                for ( int i = 0 ; i < 100 ; i++ )
                {
                    if ( sg_pouIJ1939Logger->FPJ1_IsJ1939ThreadBlocked() == TRUE )
                    {
                        m_nSendMsgJ1939LogCnt++;
                    }
                    else
                    {
                        m_nSendMsgJ1939LogCnt = 0;
                        break;
                    }
                }
                if ( m_nSendMsgJ1939LogCnt > 98 )
                {
                    m_nSendMsgJ1939LogCnt = 0;
                    sg_pouIJ1939Logger->FPJ1_DisableJ1939DataLogFlag();
                }
            }
            else
            {
                m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_J1939_LOG_ICON, m_hLogOffIcon);
            }
        }
        else
        {
            m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_J1939_LOG_ICON, m_hLogOffIcon);
        }
    }
    if(nIDEvent == m_unFlexRayNSTimer)
    {
        // Update the bus statistics window if it exists.
        if ( m_podFlexRayBusStatistics != NULL &&
                m_podFlexRayBusStatistics->IsWindowVisible( ) == TRUE )
        {
            // Perform network statistics calculation and update of
            // network statistics window
            m_podFlexRayBusStatistics->SendMessage(WM_HW_FLEXRAY_NETWORK_STATISTICS, 1, 0);
        }
    }
    // Call Parent class member for default action
    CMDIFrameWnd::OnTimer(nIDEvent);
}
/******************************************************************************
    Function Name    :  OnDropFiles
    Input(s)         :  hDropInfo : array of timer structure
    Output           :
    Functionality    :  This function will be called by frame work when user
                        drops a file in frame window. Only file with .c
                        extension is allowed to processed. Also if already
                        function editor is open the command is not processed.
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  27.02.2003
    Modifications    :
******************************************************************************/
void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
    CString omStrFileName = "";
    CString omStrFileExt  = "";
    BOOL bEditOn          = FALSE;
    CHAR acFileName[1024];
    CFlags* pouFlag = theApp.pouGetFlagsPtr();
    if(pouFlag != NULL )
    {
        INT nNumberOfFiles = 0;
        // Only open file should be dragged and dropped.
        // Check for the number of files dragged and dropped.
        nNumberOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF,acFileName,1024);
        if(nNumberOfFiles == 1 )
        {
            // Get the file name.
            DragQueryFile(hDropInfo,0,acFileName,1024);
            omStrFileName = acFileName;
            // get the file extention
            omStrFileExt  = omStrFileName.Right(omStrFileName.GetLength() -
                                                omStrFileName.ReverseFind('.') );

            bEditOn = pouFlag->nGetFlagStatus( FUNCEDITOR);
            // only file with extension ".c"  will be opened when it is dragged
            // Also if function editor is already opened it will not allow the
            // second function editor window to open.
            if(omStrFileExt.CompareNoCase(defDOT_C) == 0 && bEditOn == FALSE)
            {
                CMDIFrameWnd::OnDropFiles(hDropInfo);
                // Save the selected filename into the configuration details
                // if it is is not the same C file
                m_omMRU_C_Filename = omStrFileName;
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnExecuteAllHandlers                                  */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects all     */
/*                      handler menu item / toolbar button. The all handler   */
/*                      will be enable/disable based on the state of this menu*/
/*                      /toolbar. if handlers are already enable and the req. */
/*                      is to enable again will have no effect. Similarly the */
/*                      vice versa.                                           */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  05.02.2004.Added function call vEnableDisalbeAllTimers*/
/*                      to toogle timers when all handlers activated or       */
/*                      deactivated                                           */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  29.04.2004 Replaced PostMessage with direct assignment*/
/*                      of global variable g_bMsgHandlerON Msg Handler Enable */
/*                      flag
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :   05/01/06                                             */
/*                   :  Inserted CExecute manger for executing handlers       */
/******************************************************************************/
void CMainFrame::OnExecuteAllHandlers()
{
    //Get present status
    BOOL bEnable = GetICANNodeSim()->NS_GetHandlerStatus(H_ALL_HANDLER);
    //Change the status of handler
    GetICANNodeSim()->NS_EnableAllHandlers( !bEnable );
}

/******************************************************************************/
/*  Function Name    :  OnExecuteAllHandlers                                  */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects all     */
/*                      handler menu item / toolbar button. The all handler   */
/*                      will be enable/disable based on the state of this menu*/
/*                      /toolbar. if handlers are already enable and the req. */
/*                      is to enable again will have no effect. Similarly the */
/*                      vice versa.                                           */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  05.02.2004.Added function call vEnableDisalbeAllTimers*/
/*                      to toogle timers when all handlers activated or       */
/*                      deactivated                                           */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  29.04.2004 Replaced PostMessage with direct assignment*/
/*                      of global variable g_bMsgHandlerON Msg Handler Enable */
/*                      flag
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :   05/01/06                                             */
/*                   :  Inserted CExecute manger for executing handlers       */
/******************************************************************************/
void CMainFrame::OnExecuteAllHandlersLIN()
{
    //Get present status
    BOOL bEnable = GetILINNodeSim()->NS_GetHandlerStatus(H_ALL_HANDLER);
    //Change the status of handler
    GetILINNodeSim()->NS_EnableAllHandlers( !bEnable );
}

/******************************************************************************/
/*  Function Name    :  OnUpdateExecuteAllHandlers                            */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CMainFrame::OnUpdateExecuteAllHandlers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetICANNodeSim()->NS_GetHandlerStatus(H_ALL_HANDLER));
    BOOL bEnable = GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

/******************************************************************************/
/*  Function Name    :  OnUpdateExecuteAllHandlers                            */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CMainFrame::OnUpdateExecuteAllHandlersLIN(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetILINNodeSim()->NS_GetHandlerStatus(H_ALL_HANDLER));
    BOOL bEnable = GetILINNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}
/******************************************************************************/
/*  Function Name    :  OnExecuteErrorHandlers                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects Error   */
/*                      handler menu item / toolbar button. The error handler */
/*                      will be enable/disable based on the state of this menu*/
/*                      /toolbar. if handlers are already enable it will be   */
/*                      disable and vice versa.                               */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :   05/01/06                                             */
/*                   :  Inserted CExecute manger for executing handlers       */
/******************************************************************************/
void CMainFrame::OnExecuteErrorHandlers()
{
    //Get present status
    BOOL bEnable = GetICANNodeSim()->NS_GetHandlerStatus(H_ERROR_HANDLER);
    //Change the status of handler
    GetICANNodeSim()->NS_EnableAllErrorHandler( !bEnable );
}

/******************************************************************************/
/*  Function Name    :  OnExecuteErrorHandlers                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects Error   */
/*                      handler menu item / toolbar button. The error handler */
/*                      will be enable/disable based on the state of this menu*/
/*                      /toolbar. if handlers are already enable it will be   */
/*                      disable and vice versa.                               */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :  Anish kumar                                           */
/*  Modification on  :   05/01/06                                             */
/*                   :  Inserted CExecute manger for executing handlers       */
/******************************************************************************/
void CMainFrame::OnExecuteErrorHandlersLIN()
{
    //Get present status
    BOOL bEnable = GetILINNodeSim()->NS_GetHandlerStatus(H_ERROR_HANDLER);
    //Change the status of handler
    GetILINNodeSim()->NS_EnableAllErrorHandler( !bEnable );
}

/******************************************************************************/
/*  Function Name    :  OnUpdateExecuteErrorHandlers                          */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CMainFrame::OnUpdateExecuteErrorHandlers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetICANNodeSim()->NS_GetHandlerStatus(H_ERROR_HANDLER));
    BOOL bEnable = GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

/******************************************************************************/
/*  Function Name    :  OnUpdateExecuteErrorHandlers                          */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CMainFrame::OnUpdateExecuteErrorHandlersLIN(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetILINNodeSim()->NS_GetHandlerStatus(H_ERROR_HANDLER));
    BOOL bEnable = GetILINNodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

/******************************************************************************/
/*  Function Name    :  OnConfigureModeActive                                 */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects Active  */
/*                      mode menu item . The controller will set to active    */
/*                      mode if the tool is in connected state                */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modifications    :  Raja N on 08.09.2004, Modified the code to refer HI   */
/*                      layer function to set Active mode                     */
/*  Modifications    :  Raja N on 09.03.2005, Modified the code to support    */
/*                      multi channel architecture                            */
/******************************************************************************/
void CMainFrame::OnConfigureModeActive()
{
    g_pouDIL_CAN_Interface->DILC_SetControllerParams(defMODE_ACTIVE, HW_MODE);
    //g_pouDIL_CAN_Interface->DILC_GetControllerParams(l, 0, ECONTR_PARAM(700));
}
/******************************************************************************/
/*  Function Name    :  OnUpdateConfigureModeActive                           */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modifications    :  Raja N on 08.09.2004, Added code to disable this menu */
/*                      incase of no USB hardware present. Added code to work */
/*                      with both USB and CAN builds                          */
/******************************************************************************/
void CMainFrame::OnUpdateConfigureModeActive(CCmdUI* pCmdUI)
{
    CFlags* pouFlag = NULL;
    pouFlag     = theApp.pouGetFlagsPtr();
    pCmdUI->Enable(FALSE);
    pCmdUI->SetCheck(FALSE);
    BOOL bConnect  = pouFlag->nGetFlagStatus(CONNECTED);
    if(pCmdUI != NULL && bConnect == TRUE)
    {
        //Tested with these hardwares only
        if( m_dwDriverId == DRIVER_CAN_VECTOR_XL || m_dwDriverId == DRIVER_CAN_ICS_NEOVI || m_dwDriverId == DRIVER_CAN_ETAS_ES581 )
        {
            pCmdUI->Enable(TRUE);
        }
    }
    /*if( pCmdUI != NULL)
    {
        CFlags* podFlags = theApp.pouGetFlagsPtr();
        if( podFlags != NULL )
        {
            BOOL bConnect = podFlags->nGetFlagStatus(CONNECTED);
            if(bConnect == FALSE )
            {
                pCmdUI->Enable(TRUE);
            }
            else
            {
                pCmdUI->Enable(FALSE);
            }
        }
        LONG lParam = 0;
        if (g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, HW_MODE) == S_OK)
        {
            if (lParam == defMODE_ACTIVE)
            {
                pCmdUI->SetCheck(TRUE);
            }
            else
            {
                pCmdUI->SetCheck(FALSE);
            }
        }
        else
        {
            pCmdUI->SetCheck(FALSE);
        }
    }*/
}
/******************************************************************************/
/*  Function Name    :  OnNetworkStatisticsWnd                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects to      */
/*                      Display statistics window.                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  14.03.2005, Modified code to create statistics dialog */
/*                      only during first show                                */
/******************************************************************************/
void CMainFrame::OnNetworkStatisticsWnd()
{
    // If statistics dialog is not created
    if(m_bIsStatWndCreated == FALSE )
    {
        // Create statistics dialog

        LONG lParam = 0;
        int unTotalChannels = defNO_OF_CHANNELS;
        if (g_pouDIL_CAN_Interface && g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
        {
            unTotalChannels = (INT)lParam;
        }

        // Create memory for the dialog
        m_podBusStatistics = new CBusStatisticsDlg(GetICANBusStat(), NULL, unTotalChannels);
        // If success
        if( m_podBusStatistics != NULL )
        {
            // Create the dialog
            int nReturn =
                m_podBusStatistics->Create(IDD_DLG_NETWORK_STATISTICS, this);

            if(nReturn != 0 )
            {
                // Update the statistics dialog creation status
                m_bIsStatWndCreated = TRUE;


            }
            else
            {
                // Show the error information to the user
                if(theApp.m_bFromAutomation == FALSE)
                {
                    AfxMessageBox( _(defSTR_STAT_WINDOW_CREATION_FAILED) );
                }
            }
        }
    }
    // If statistics window is created
    if(m_bIsStatWndCreated == TRUE )
    {
        // Toggle window visiblity
        if(!m_podBusStatistics->IsWindowVisible() )
        {

            //m_podBusStatistics->vLoadDataFromStore();
            if(m_pCopyBusStsticsNode != NULL)
            {
                m_podBusStatistics->vLoadDataFromStore(m_pCopyBusStsticsNode);
            }
            m_podBusStatistics->ShowWindow(SW_SHOW);
        }
        else
        {
            m_podBusStatistics->ShowWindow(SW_HIDE);
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnUpdateNetworkStatisticsWnd                          */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  14.03.2005, Added check before accessing the member of*/
/*                      statistics dialog                                     */
/******************************************************************************/
void CMainFrame::OnUpdateNetworkStatisticsWnd(CCmdUI* pCmdUI)
{
    if(pCmdUI != NULL)
    {
        // Check for window cration
        if( m_podBusStatistics != NULL &&
                m_podBusStatistics->IsWindowVisible() == TRUE )
        {
            pCmdUI->SetCheck(TRUE);
        }
        else
        {
            pCmdUI->SetCheck(FALSE);
        }
    }
}

/******************************************************************************/
/*  Function Name    :  OnFlexRayNetworkStatisticsWnd                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects to      */
/*                      Display statistics window.                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ashwin R Uchil                                        */
/*  Date Created     :  13.08.2013                                            */
/******************************************************************************/
void CMainFrame::OnFlexRayNetworkStatisticsWnd()
{
    // If statistics dialog is not created
    if (m_bIsFlexRayStatWndCreated == FALSE)
    {
        m_podFlexRayBusStatistics = new CFlexRayNetworkStatsDlg(GetIFlexRayBusStat(), this);
        int nReturn = m_podFlexRayBusStatistics->Create(IDD_DLG_FLEXRAY_NETWORK_STATISTICS, this);
        if(nReturn != 0 )
        {
            // Update the statistics dialog creation status
            m_bIsFlexRayStatWndCreated = TRUE;


        }
        else
        {
            // Show the error information to the user
            if(theApp.m_bFromAutomation == FALSE)
            {
                AfxMessageBox( _(defSTR_STAT_WINDOW_CREATION_FAILED) );
            }
        }
    }
    else
    {
        int nShowMode = m_podFlexRayBusStatistics->IsWindowVisible() ? SW_HIDE : SW_SHOW;
        m_podFlexRayBusStatistics->ShowWindow(nShowMode);
    }
}
/******************************************************************************/
/*  Function Name    :  OnUpdateFlexRayNetworkStatisticsWnd                   */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ashwin R Uchil                                        */
/*  Date Created     :  13.08.2013                                            */
/******************************************************************************/
void CMainFrame::OnUpdateFlexRayNetworkStatisticsWnd(CCmdUI* pCmdUI)
{
    if(pCmdUI != NULL)
    {
        // Check for window cration
        if( m_podBusStatistics != NULL &&
                m_podBusStatistics->IsWindowVisible() == TRUE )
        {
            pCmdUI->SetCheck(TRUE);
        }
        else
        {
            pCmdUI->SetCheck(FALSE);
        }
    }
}



/******************************************************************************
    Function Name    :  bEnableDisableLog
    Input(s)         :  bStart, indicating Start\Stop of Logging
    Output           :
    Functionality    :  This function calls OnLogEnable() function to
                        Enable\Disable Logging
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Ravikumar Patil
    Date Created     :  24.02.2003
    Modifications    :  21.03.2002, Ravikumar Patil, code review comment
*****************************************************************************/
BOOL CMainFrame::bEnableDisableLog(BOOL bStart)
{
    BOOL bRetval = TRUE ;
    BOOL bLogging = theApp.pouGetFlagsPtr()->nGetFlagStatus(LOGTOFILE);

    if ( (bStart == TRUE) && (bLogging == TRUE) ) //If logging enabled and trying
    {
        //to enable
        bRetval = FALSE ;
    }
    else if ( (bStart == FALSE) && (bLogging == FALSE) ) //If logging disabled and trying
    {
        //to disable
        bRetval = FALSE ;
    }
    else
    {
        OnLogEnable() ;
    }
    return bRetval ;
}

/******************************************************************************
    Function Name    :  bWriteToLog
    Input(s)         :  pcOutStrLog, string to be logged
    Output           :
    Functionality    :  This function will Send a message to Log the string
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Ravikumar Patil
    Date Created     :  28.02.2003
    Modifications    :  21.03.2002, Ravikumar Patil, code review comment
******************************************************************************/
BOOL CMainFrame::bWriteToLog(char* pcOutStrLog)
{
    BOOL bRetval = TRUE ;
    BOOL bLogON = TRUE ;
    bLogON = theApp.pouGetFlagsPtr()->nGetFlagStatus(LOGTOFILE);

    if ( (bLogON == TRUE) && (pcOutStrLog != NULL) )
    {
        //::SendMessage(GUI_hDisplayWindow,WM_LOG_STRING,0,(LPARAM)pcOutStrLog) ;
    }
    else
    {
        bRetval = FALSE ;
    }

    return bRetval ;
}

/******************************************************************************
    Function Name    :  bSetControllerMode
    Input(s)         :  bbMode, New controller Mode
    Output           :
    Functionality    :  This function will set the Controller mode. It calls
                        OnConfigurePassive() or OnConfigurePassive() member
                        function depending on the current mode
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Ravikumar Patil
    Date Created     :  28.02.2003
    Modifications    :
******************************************************************************/
BOOL CMainFrame::bSetControllerMode(BOOL bMode)
{
    BOOL bRetval = TRUE ;
    BOOL bControllerMode = theApp.pouGetFlagsPtr()->nGetFlagStatus(CONTROLLER_MODE);
    BOOL bConnect = theApp.pouGetFlagsPtr()->nGetFlagStatus(CONNECTED);

    if(bConnect == TRUE ) //If connected
    {
        bRetval = FALSE ;
    }
    else //If not connected
    {
        if ( (bMode == TRUE ) && (bControllerMode != 1))
        {
            OnConfigureModeActive() ;
        }
        else if( (bMode == FALSE ) && (bControllerMode != 2))
        {
            OnConfigurePassive() ;
        }
        else
        {
            bRetval = FALSE ;
        }
    }
    return bRetval ;
}



/******************************************************************************
    Function Name    :  vDisconnect
    Input(s)         :  bState
    Output           :
    Functionality    :  This function will call OnFileConnect() member function
                        to connect\disconnect
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Ravikumar Patil
    Date Created     :  24.02.2003
    Modifications    :  21.03.2002, Ravikumar Patil, code review comment
******************************************************************************/
LRESULT CMainFrame::vDisconnect(WPARAM , LPARAM lParam)
{
    //bState is TRUE if Disconnect
    BOOL bRetval = TRUE ;
    BOOL bState = (WPARAM) lParam ;

    BOOL bActiveState = theApp.pouGetFlagsPtr()->nGetFlagStatus(CONNECTED);
    if ( (bActiveState == TRUE) && (bState == FALSE) ) //If already connected and again
    {
        //trying to connect
        bRetval = FALSE ;
    }
    else if ( (bActiveState == FALSE) && (bState == TRUE) ) //If already disconnected and
    {
        //again trying to disconnect
        bRetval = FALSE ;
    }
    else
    {
        OnFileConnect() ;
    }

    return bRetval ;
}

/******************************************************************************

/******************************************************************************
    Function Name    :  gbSendStrToTrace
    Input(s)         :  pcOutStrTrace, string to be sent to trace
    Output           :  BOOL value (True always)
                        function will be returned
    Functionality    :  This function will Send a message and pass the string
                        to be printed
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  25-Feb-2003
    Modifications    :  Raja N on 10.03.2004
                        Changed the wait delay from infinite to 1000 to avoide
                        hang while unloading the DLL
    Modifications    :  Anish on 30.01.2007
                        Moved creation of p_acTraceStr after waitforsingleobject
                        to avoid any memory leak
******************************************************************************/
BOOL gbSendStrToTrace(CHAR* pcOutStrTrace)
{
    BOOL bResult = TRUE;

    if (g_dwUIThread == 0)
    {
        CMainFrame* pMainFrm = static_cast<CMainFrame*> (theApp.GetMainWnd());

        if (pMainFrm != NULL)
        {
            pMainFrm->SendMessage(WM_CREATE_TRACEWIN, 0, 0);
            g_hSemaphore = CreateSemaphore(NULL,1,1,NULL);
        }
    }

    if (g_dwUIThread != 0)
    {
        if ( pcOutStrTrace != NULL)
        {
            static CHAR s_acTraceStr[1024] = {""};
            WaitForSingleObject(g_hSemaphore, MAX_TIME_LIMIT);
            //CHAR *p_acTraceStr = new CHAR[1024];
            strncpy_s(s_acTraceStr,pcOutStrTrace,1024);
            bResult = ::PostThreadMessage(g_dwUIThread, WM_WRITE_TO_TRACE, 0,
                                          (LPARAM) s_acTraceStr);
            Sleep(0);
        }
    }

    return bResult;
}

/******************************************************************************/
/*  Function Name    :  OnConfigurePassive                                    */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects Passive */
/*                      menu item. The controller will set to Passive mode if */
/*                      the tool is in connected state.                       */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modifications    :  Raja N on 08.09.2004, Modified to refer HI layer      */
/*                      Functions.                                            */
/*  Modifications    :  Raja N on 09.03.2005, Modified the code to support    */
/*                      multi channel architecture                            */
/******************************************************************************/
void CMainFrame::OnConfigurePassive()
{
    g_pouDIL_CAN_Interface->DILC_SetControllerParams(defMODE_PASSIVE, HW_MODE);
    //g_pouDIL_CAN_Interface->DILC_SetControllerParams(lValue, unChannel, (ECONTR_PARAM)70);
}
/******************************************************************************/
/*  Function Name    :  OnUpdateConfigurePassive                              */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modifications    :  Raja N on 08.09.2004, Modified to refer HI layer      */
/*                      Functions. This menu item will be disabled if there is*/
/*                      no real USB hardware found during startup             */
/******************************************************************************/
void CMainFrame::OnUpdateConfigurePassive(CCmdUI* pCmdUI)
{
    CFlags* pouFlag = NULL;
    pouFlag     = theApp.pouGetFlagsPtr();
    BOOL bConnect  = pouFlag->nGetFlagStatus(CONNECTED);
    pCmdUI->Enable(FALSE);
    pCmdUI->SetCheck(FALSE);
    if(pCmdUI != NULL && bConnect == TRUE)
    {
        //Tested with these hardwares only
        if( m_dwDriverId == DRIVER_CAN_VECTOR_XL || m_dwDriverId == DRIVER_CAN_ICS_NEOVI || m_dwDriverId == DRIVER_CAN_ETAS_ES581 )
        {
            pCmdUI->Enable(TRUE);
        }
    }

    /*if( pCmdUI != NULL)
    {
        CFlags* podFlags = theApp.pouGetFlagsPtr();
        if( podFlags != NULL )
        {
            // Get the connection status
            BOOL bConnect = podFlags->nGetFlagStatus(CONNECTED);
            if(bConnect == FALSE )
            {
                pCmdUI->Enable(TRUE);
            }
            else
            {
                pCmdUI->Enable(FALSE);
            }
        }
        LONG lParam = 0;
        if (g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, HW_MODE) == S_OK)
        {
            if (lParam == defMODE_PASSIVE)
            {
                pCmdUI->SetCheck(TRUE);
            }
            else
            {
                pCmdUI->SetCheck(FALSE);
            }
        }
        else
        {
            pCmdUI->SetCheck(FALSE);
        }
    }*/
}
/******************************************************************************/
/*  Function Name    :  OnTraceWnd                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects to      */
/*                      display trace window. If the window is not created,it */
/*                      it will create it.                                    */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :  Ravikumar Patil                                       */
/*  Modification on  :  21.03.2003, changes due to change in type of window   */
/*  Modified On      :  18.04.2003, new member function  bCreateOutputWindow  */
/*                                of CNotificWnd class is called to create it.*/
/*  Modification By  :  Saravanan                                             */
/*  Modified On      :  16.11.2011, Window is made visible for first time call*/
/******************************************************************************/
void CMainFrame::OnTraceWnd()
{
    if (m_podUIThread == NULL)
    {
        bCreateTraceWindow();
        //Window should be displayed if thread is created newly
        m_bNotificWndVisible = FALSE;
    }
    if (m_podUIThread != NULL)
    {
        m_bNotificWndVisible = !m_bNotificWndVisible;
        UINT nShow = m_bNotificWndVisible? SW_SHOW : SW_HIDE;
        m_podUIThread->PostThreadMessage(WM_THREADMSG_PROC, WM_MODIFY_VISIBILITY, nShow);
        //m_bNotificWndVisible = m_podUIThread->bIsNotificWndVisible();
    }
}
/******************************************************************************/
/*  Function Name    :  OnUpdateTraceWnd                                      */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  03.03.2003                                            */
/*  Modification By  :  Ravikumar Patil                                       */
/*  Modification on  :  21.03.2003, changes due to change in type of window   */
/******************************************************************************/
void CMainFrame::OnUpdateTraceWnd(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bNotificWndVisible);
}

/******************************************************************************/
/*  Function Name    :  OnUpdateConfigureBaudrate                             */
/*  Input(s)         :  CCmdUI* pCmdUI                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the current GUI state of */
/*                      the menu item / toolbar button needs to be updated,   */
/*                      either as a result of pulling down the menu item or   */
/*                      whatever else.                                        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  01.04.2003                                            */
/*  Modifications    :  Raja N on 08.09.2004, Modified code to disable this   */
/*                      menuitem if the tool is in simulate mode              */
/******************************************************************************/
void CMainFrame::OnUpdateConfigureBaudrate(CCmdUI* pCmdUI)
{
    if(pCmdUI != NULL)
    {
        CFlags* podFlag = theApp.pouGetFlagsPtr();
        if( podFlag != NULL )
        {
            // Disable if it is connected
            BOOL bDisable = podFlag->nGetFlagStatus(CONNECTED);
            // In Simulation mode is selected then disable Controller option
            LONG lParam= 0;
            if( g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, HW_MODE) == S_OK)
            {
                if (lParam == defMODE_SIMULATE)
                {
                    bDisable = TRUE;
                }
            }
            // If bDisable = TRUE then set the Enable to FALSE
            pCmdUI->Enable(!bDisable);
        }
    }
}
/******************************************************************************/
/*  Function Name    :  vSetWarningLimitVar                                   */
/*  Input(s)         :  WPARAM wParam = 0                                     */
/*                      LPARAM lParam = warning limit value                   */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when a                        */
/*                      WM_SET_WARNING_LIMIT_VAR message is posted. The lParam*/
/*                      will contain warning limit value. It will set  the    */
/*                      member variable m_unWarningLimit to this value        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  01.04.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
LRESULT CMainFrame::vSetWarningLimitVar(WPARAM , LPARAM lParam)
{
    UINT unWarningLimtMin = 1;
    UINT unWarningLimtMax = 127;
    m_unWarningLimit = static_cast <UINT> (lParam);
    // Validate the warning limit for maximum and minimum value.
    if(   ( m_unWarningLimit < unWarningLimtMin )
            || ( m_unWarningLimit > unWarningLimtMax ) )
    {

        m_unWarningLimit = unWarningLimtMin;
    }
    return 0;
}

/******************************************************************************/
/*  Function Name    :  vKeyPressedInMsgWnd                                   */
/*  Input(s)         :  WPARAM wParam = Key Value                             */
/*                      LPARAM lParam = 0                                     */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when a                        */
/*                      WM_KEY_PRESSED_MSG_WND message is posted. The wParam  */
/*                      will contain key value which is pressed. It will call */
/*                      vExecuteOnKeyHandler((CHAR)wParam) member function of */
/*                      CExecuteFunc class, with is key value passed as       */
/*                      parameter.                                            */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  23.04.2003                                            */
/*  Modification By  :                                                        */
/*  Modification on  :  Raja N on 26.04.2005, Modified code to refer Tx msg   */
/*                      manager to handle key press event                     */
/*  Modification By  :  Anish kumar                                           */
/*  Modification On  :  21/12/05 ,modified to refer cExecuteManager for       */
/*                      multinode                                             */
/******************************************************************************/
LRESULT CMainFrame::vKeyPressedInMsgWnd(WPARAM wParam, LPARAM )
{
    // Check for key a-z or A-Z, if any of these are press
    // call member function of CExecuteManager class for key handler
    if(   ( wParam >= 'A' && wParam<='Z' )||
            ( wParam >= 'a' && wParam<='z' )||
            ( wParam >= '0' && wParam<='9' ) )
    {
        //vManageOnKeyHandler(UCHAR(wParam));
        BOOL bStopMsgBlockTx = m_objTxHandler.bGetTxStopFlag();
        if(bStopMsgBlockTx == FALSE )
        {
            m_objTxHandler.vStartTransmission((UCHAR)wParam);
        }
        m_objFlexTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eKeyPress, (UCHAR)wParam);
    }
    return 0;
}

/******************************************************************************/
/*  Function Name    :  GetSubMenu                                            */
/*  Input(s)         :  CString MenuName                                      */
/*  Output           :  Pointer to CMenu                                      */
/*  Functionality    :  Function takes the Root Menu and returns the          */
/*                      SubMenu Pointer                                       */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Arun Kumar K                                          */
/*  Date Created     :  07.05.2010                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
CMenu* CMainFrame::GetSubMenu(CString MenuName)
{
    CMenu* mmenu = GetMenu();
    CMenu* Submenu = NULL;
    BOOL bFound = FALSE;
    int i = -1;
    CString Str;
    int nCnt = mmenu->GetMenuItemCount();
    for (i = 0; i < nCnt; i++)
    {
        mmenu->GetMenuString(i, Str, MF_BYPOSITION);
        if (Str == MenuName)
        {
            bFound = TRUE;
            break;
        }
    }
    if (bFound)
    {
        Submenu = mmenu->GetSubMenu(i);
    }
    return Submenu;
}

/******************************************************************************/
/*  Function Name    : vToolBarDropDownMenu                                   */
/*  Input(s)         : UINT unControlID, int nButtonIndex                     */
/*  Output           :                                                        */
/*  Functionality    : Pops up context menu  for drop down toolbar button     */
/*  Member of        : CMainFrame                                             */
/*  Friend of        :      -                                                 */
/*  Author(s)        : Krishnaswamy B.N.                                      */
/*  Date Created     : 03.06.2003                                             */
/*  Modifications    : Arun Kumar K,                                          */
/*                     20.05.2010, m_wndToolBar.ClientToScreen(..) used       */
/*                                 instead of this->ClientToScreen(..).       */
/*                     Amitesh Bharti,                                        */
/*                     21.01.2004, Instead of index toolbar button ID is      */
/*                                 passed as parameter.                       */
/******************************************************************************/
void CMainFrame::vToolBarDropDownMenu(UINT unControlID, int nButtonID)
{
    CMenu* m_pomContextMenu = new CMenu;
    RECT rRect = {0,0,0,0};
    // Load the Menu from the resource
    if ( m_pomContextMenu != NULL )
    {
        m_pomContextMenu->DestroyMenu();
        m_pomContextMenu->LoadMenu(unControlID);

        CMenu* pomSubMenu = m_pomContextMenu->GetSubMenu(0);

        if(nButtonID == IDM_DISPLAY_MESSAGE_DISPLAYRELATIVETIME)
        {
            m_wndToolbarMsgWnd.GetToolBarCtrl().GetRect( nButtonID, &rRect );
            m_wndToolbarMsgWnd.ClientToScreen(&rRect);
        }
        else
        {
            m_wndToolBar.GetToolBarCtrl().GetRect( nButtonID, &rRect );
            m_wndToolBar.ClientToScreen(&rRect);
        }

        rRect.left    = rRect.left + SHIFT_X;
        //    rRect.bottom  = rRect.bottom + SHIFT_Y;
        pomSubMenu->TrackPopupMenu( TPM_LEFTALIGN  |TPM_RIGHTBUTTON,
                                    rRect.left ,
                                    rRect.bottom ,
                                    this,
                                    NULL);
        if (m_pomContextMenu != NULL )
        {
            delete m_pomContextMenu;
        }
    }
}
/******************************************************************************
    Function Name    :  OnDisplayAbsoluteTime

    Input(s)         :      -
    Output           :  void
    Functionality    :  Changes the Display mode to Absolute mode
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Krishnaswamy B.N.
    Date Created     :  03.06.2003
    Modification By  :  ArunKumar K
    Modification on  :  08.06.2010, Setting Check Status in Message Window.
******************************************************************************/
void CMainFrame::OnDisplayAbsoluteTime()
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        CLEAR_EXPR_TM_BITS(byGetDispFlag);
        SET_TM_ABS(byGetDispFlag);
        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
            if(hWnd)
            {
                BYTE bModes = TIME_MODE;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }
}
/******************************************************************************
    Function Name    :  OnUpdateDisplayAbsolutetime

    Input(s)         :      -
    Output           :  void
    Functionality    :  Puts a check mark to the menu
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Krishnaswamy B.N.
    Date Created     :  03.06.2003
    Modification By  :  ArunKumar K
    Modification on  :  08.06.2010, Retreiving Check Status from Message Window.
    Modification By  :  Amitesh Bharti
    Modification on  :  01.07.2003, This menu is enable even tool is connected.
******************************************************************************/
void CMainFrame::OnUpdateDisplayAbsolutetime(CCmdUI* pCmdUI)
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        if (IS_TM_ABS_SET(byGetDispFlag))
        {
            pCmdUI->SetCheck();
        }
        else
        {
            pCmdUI->SetCheck(FALSE);
        }
    }
}
/******************************************************************************
    Function Name    :  OnDisplayRelativetime

    Input(s)         :      -
    Output           :  void
    Functionality    :  Changes the display mode to relative mode
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Krishnaswamy B.N.
    Date Created     :  03.06.2003
    Modification By  :  ArunKumar K
    Modification on  :  08.06.2010, Setting Check Status in Message Window.
******************************************************************************/
void CMainFrame::OnDisplayRelativetime()
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        CLEAR_EXPR_TM_BITS(byGetDispFlag);
        SET_TM_REL(byGetDispFlag);
        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
            if(hWnd)
            {
                BYTE bModes = TIME_MODE;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }
}
/******************************************************************************
    Function Name    :  OnUpdateDisplayRelativetime

    Input(s)         :      -
    Output           :  void
    Functionality    :  Puts a check mark to the menu
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Krishnaswamy B.N.
    Date Created     :  03.06.2003
    Modification By  :  ArunKumar K
    Modification on  :  08.06.2010, Retreiving Check Status from Message Window.
    Modification By  :  Amitesh Bharti
    Modification on  :  01.07.2003, This menu is enable even tool is connected.
******************************************************************************/
void CMainFrame::OnUpdateDisplayRelativetime(CCmdUI* pCmdUI)
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        if (IS_TM_REL_SET(byGetDispFlag))
        {
            pCmdUI->SetCheck();
        }
        else
        {
            pCmdUI->SetCheck(FALSE);
        }
    }
}
/******************************************************************************
    Function Name    :  OnDisplaySystemTime

    Input(s)         :      -
    Output           :  void
    Functionality    :  Changes the display mode to relative mode
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Krishnaswamy B.N.
    Date Created     :  03.06.2003
    Modification By  :  ArunKumar K
    Modification on  :  08.06.2010, Setting Check Status in Message Window.
******************************************************************************/
void CMainFrame::OnDisplaySystemTime()
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        CLEAR_EXPR_TM_BITS(byGetDispFlag);
        SET_TM_SYS(byGetDispFlag);
        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
            if(hWnd)
            {
                BYTE bModes = TIME_MODE;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }
}
/******************************************************************************
    Function Name    :  OnUpdateDisplaySystemTime

    Input(s)         :      -
    Output           :  void
    Functionality    :  Puts a check mark to the menu
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Krishnaswamy B.N.
    Date Created     :  03.06.2003
    Modification By  :  ArunKumar K
    Modification on  :  08.06.2010, Retreiving Check Status from Message Window.
    Modification By  :  Amitesh Bharti
    Modification on  :  01.07.2003, This menu is enable even tool is connected.
******************************************************************************/
void CMainFrame::OnUpdateDisplaySystemTime(CCmdUI* pCmdUI)
{
    if (m_podMsgWndThread != NULL)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), NULL);
        if (IS_TM_SYS_SET(byGetDispFlag))
        {
            pCmdUI->SetCheck();
        }
        else
        {
            pCmdUI->SetCheck(FALSE);
        }
    }
}
/******************************************************************************
    Function Name    :  OnEnableTimeStampButton

    Input(s)         :      -
    Output           :  void
    Functionality    :  Dummy Handler to enable the time mode toolbar button
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Krishnaswamy B.N.
    Date Created     :  03.06.2003
******************************************************************************/
void CMainFrame::OnEnableTimeStampButton()
{
    // Required to enable the toolbar button
}

/******************************************************************************
 Function Name  :   vPostMsgToSendMsgDlg

 Input(s)       :
 Output         :   -
 Functionality  :   This function is called at start up or during loading the
                    new configuration file to set log trigger status in MsgWnd
                    class
 Member of      :   CMainFrame

 Author(s)      :   Amitesh Bharti
 Date Created   :   18/06/2003
 Modifications  :   Raja N on 26.04.2005, Modified code refer new Tx window
                    pointer to post database import message
******************************************************************************/
VOID CMainFrame::vPostMsgToSendMsgDlg(ETYPE_BUS eBus)
{

    // Send a message to Tx Window to update the database messages.
    switch (eBus)
    {
        case CAN:
        {
            if(m_objTxHandler.hConfigWindowShown() == S_OK)
            {
                eUSERSELCTION eUserSel = eDATABASEIMPORTCMD;
                m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD,(WPARAM)eUserSel,0) ;
            }
        }
        break;
        case J1939:
        {
            SMSGENTRY* psMsgEntry = NULL;
            vPopulateMsgEntryFromDB(psMsgEntry, m_pouMsgSigJ1939);
            if (m_pouTxMsgWndJ1939 != NULL)
            {
                m_pouTxMsgWndJ1939->vSetDatabaseInfo(psMsgEntry);
            }
            else //J1939 Transmit window is not created just update datastore
            {

                CTxMsgWndJ1939::vUpdateDataStore(psMsgEntry);
            }
            SMSGENTRY::vClearMsgList(psMsgEntry);
            psMsgEntry = NULL;
        }
        break;
    }
}

/******************************************************************************
 Function Name  :   vDeleteGlobalTxMemory

 Input(s)       :
 Output         :   -
 Functionality  :   This function is called at start up to delete the global
                    pointer for message  block. It will be done by calling
                    CSendMultiMsgDlg member function.
 Member of      :   CMainFrame

 Author(s)      :   Amitesh Bharti
 Date Created   :   18/06/2003
 Modifications  :   Raja N
                    26.04.2005, Modified to refer CTxMsgManager class for
                    clearing global Tx list memory
******************************************************************************/
VOID CMainFrame::vDeleteGlobalTxMemory()
{
    // Delete global memory alloted for Tx List
    //m_podTxMsgManager->vDeleteGlobalTxMemory();
    m_objTxHandler.vDeleteTxBlockMemory();
}

/******************************************************************************
 Function Name  :   vAssignMsgBlockList

 Input(s)       :
 Output         :   -
 Functionality  :   This function is called at start up or during loading the
                    new configuration file to set global pointer for message
                    block .It will be done by calling CSendMultiMsgDlg member
                    function.
 Member of      :   CMainFrame

 Author(s)      :   Amitesh Bharti
 Date Created   :   14/01/2004
 Modifications  :   Raja N
                    26.04.2005, Modified to refer CTxMsgManager class for
                    assigning global Tx list memory
******************************************************************************/
VOID CMainFrame::vAssignMsgBlockList()
{
    //m_podTxMsgManager->vAssignMsgBlockList();
    m_objTxHandler.vAssignMsgBlockList();
}
/******************************************************************************/
/*  Function Name    :  bAllocateMemoryForGlobalTxList                        */
/*  Input(s)         :  TRUE or FALSE                                         */
/*  Output           :                                                        */
/*  Functionality    :  This function allocate memory to global list. Memory  */
/*                      will be allocated only if the list is not already     */
/*                      It will be done by calling CSendMultiMsgDlg member    */
/*                      function bAllocateMemoryForGlobalTxList().            */
/*  Member of        :  CSendMultiMsgDlg                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  14.01.2004                                            */
/*  Modifications    :  Raja N                                                */
/*                      26.04.2005, Modified to refer CTxMsgManager class for */
/*                      clearing global Tx list memory                        */
/******************************************************************************/
BOOL CMainFrame::bAllocateMemoryForGlobalTxList()
{
    //return m_podTxMsgManager->bAllocateMemoryForGlobalTxList();
    HRESULT hResult = S_FALSE;
    hResult = m_objTxHandler.hAllocateMemoryForGlobalTxList();

    if(hResult == S_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/******************************************************************************
    Function Name    :  vNotificationFromOtherWin
    Input(s)         :  wParam - ID of the window that posts the message
                        lParam - Information passed. Interpretation of this is
                                 subject to the window ID
    Output           :  -
    Functionality    :  Handler of WM_NOTIFICATION_FROM_OTHER posted from other
                        windows
    Member of        :  CMainFrame
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :  Raja N on 02/12/2004
    Modification     :  Added graphics window status flag update code.
/*****************************************************************************/
LRESULT CMainFrame::vNotificationFromOtherWin(WPARAM wParam, LPARAM lParam)
{
    // wParam tells which window posts this message
    // lParam contains the different informations been passed
    if (wParam == eWINID_TRACEWND)
    {
        m_bNotificWndVisible = (BOOL) lParam;
    }
    //SGW Code commented by Arun 21-10-2010
    // Update Graph window status
    //if( wParam == eWNDID_GRAPHWND)
    //{
    //    m_bGraphWindowVisible = (BOOL) lParam;
    //    // Update Msg Wnd
    //    //::PostMessage( GUI_hDisplayWindow, WM_NOTIFICATION_FROM_OTHER,
    //    //          eWNDID_GRAPHWND, lParam );
    //}
    //SGW Code commented by Arun 21-10-2010
    return 0;
}
/******************************************************************************
    Function Name    :  vCreateTraceWindow
    Input(s)         :  wParam - Unused
                        lParam - Unused
    Output           :  -
    Functionality    :  Handler of WM_CREATE_TRACEWIN posted to create trace
                        window
    Member of        :  CMainFrame
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :
    Modification     :
/*****************************************************************************/
LRESULT CMainFrame::vCreateTraceWindow(WPARAM , LPARAM )
{
    bCreateTraceWindow();
    return 0;
}
/******************************************************************************
    Function Name    :  bCreateTraceWindow
    Input(s)         :  -
    Output           :  TRUE - If trace window is created successfully.
                        FALSE - If window creation is unsuccessful or it is
                        already existing
    Functionality    :  Called to create the trace window
    Member of        :  CMainFrame
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :
    Modification     :
/*****************************************************************************/
BOOL CMainFrame::bCreateTraceWindow(void)
{
    BOOL bResult = FALSE;

    if (m_podUIThread == NULL)
    {
        m_podUIThread = static_cast<CUIThread*> (AfxBeginThread(RUNTIME_CLASS(CUIThread),
                        THREAD_PRIORITY_NORMAL, 0, 0));

        if (m_podUIThread != NULL)
        {
            bResult = m_podUIThread->bCreateTraceWindow(this);
            m_sNotificWndPlacement.showCmd = SW_SHOW;
            m_podUIThread->vUpdateWndCo_Ords(m_sNotificWndPlacement, TRUE);
            sg_ouAppServiceObj.m_podUIThread = m_podUIThread;
            vREP_InitReplay(&sg_ouAppServiceObj, eINIT_TRACE);
            CMsgSignal::vSetTraceWndObj(m_podUIThread);//trace wnd ptr to MsgSignal
        }
    }

    return bResult;
}
/******************************************************************************
    Function Name    :  OnSize
    Input(s)         :  -
    Output           :
    Functionality    :  Called to by frame work when main window is resized.
    Member of        :  CMainFrame
    Author(s)        :  Raja N.
    Date Created     :  29-01-2004
    Modified by      :
    Modification     :
/*****************************************************************************/
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    CMDIFrameWnd::OnSize(nType, cx, cy);

    // Workaruond for Toolbar size problem
    SendMessage(WM_COMMAND,ID_VIEW_TOOLBAR);
    SendMessage(WM_COMMAND,ID_VIEW_TOOLBAR);
}


/******************************************************************************
Function Name    : vEmptySignalWatchList
Input(s)         :
Output           :
Functionality    : Called to clear signal watch list. This will be called while
                   importing a database or while changing the confuguration
Member of        : CMainFrame
Author(s)        : Raja N
Date Created     : 05-03-2004
Modified by      : Raja N on 05/04/2004
Modification     : Modified to refer the latest data structure
/*****************************************************************************/
//void CMainFrame::vEmptySignalWatchList()
//{
//    // Enter Critical Section
//    //EnterCriticalSection(&g_CritSectionForSW);
//    theApp.vRelease(SIGNAL_WATCH_LIST,NULL);
//    m_psSignalWatchList = NULL;
//    // Leave Critical Section
//    //LeaveCriticalSection(&g_CritSectionForSW);
//
//    //::PostMessage(GUI_hDisplayWindow, WM_REMOVE_SIGNAL, 0, 0);
//    //::PostMessage(GUI_hDisplayWindow, WM_CLEAR_SIGNAL_WATCH, 0, 0);
//}

void CMainFrame::vReleaseSignalWatchListMemory(SMSGENTRY*& psSignalWatchList)
{
    if (psSignalWatchList != NULL)
    {
        SMSGENTRY::vClearMsgList(psSignalWatchList);
    }
}
void CMainFrame::vEmptySimsysList()
{
    GetICANNodeSim()->NS_SetSimSysConfigData(NULL, 0);
}
/******************************************************************************
Function Name    : nLoadConfigurationFile
Input(s)         :
Output           :
Functionality    : Called to load a configuration file. This will be called from
                   Configuration Load and while selecting MRU config files
Member of        : CMainFrame
Author(s)        : Raja N
Date Created     : 16.03.2004
Modified by      :
Modifications    : Raja N on 14.04.02004
                 : Added code to clear Signal Watch, Message Window and Message
                   Interpretation Window.
Modifications    : Raja N on 08.09.02004
                 : Modified the code to refer HIL function to set the baud rate
                   and to init controller after that
Modifications    : Raja N on 08.09.02004
                   Added call to vRestoreWindowPostion which will restore
                   all window postions with the configuration data
Modifications    : Anish on 05.02.2007
                   Added code to initialize simsyslist as null before loading
                   another configuration.
/*****************************************************************************/
//int CMainFrame::nLoadConfigurationFile(CString omConfigFile,BOOL bFromCom)
//{
//  //theApp.bSetData(SIMSYS_LIST,reinterpret_cast<void *>(-1));
//  int nReturn = nLoadConfigFile(omConfigFile,bFromCom) ;
//    // load the file
//    if(nReturn == defCONFIG_FILE_SUCCESS)
//    {
//        vPushConfigFilenameDown(omConfigFile);
//    }
//    // restore window postion from configuration file
//    vRestoreWindowPostion();
//
//    if( m_pomGraphThread != NULL )
//    {
//        ::PostThreadMessage( m_pomGraphThread->m_nThreadID,
//                             WM_CONFIG_CHANGE , NULL, NULL );
//    }
//    //::PostThreadMessage(GUI_dwThread_MsgDisp,TM_SET_FLAG_STATUS,0,0);
//    BOOL bReturn = FALSE;
//    // Fill the controller configuration information
//
//    PSCONTROLLER_DETAILS pControllerDetails = NULL;
//    //theApp.bGetData(CONTROLLER_DETAILS, (void**) &pControllerDetails);
//    //HRESULT hResult = S_FALSE;
//    if (pControllerDetails != NULL)
//    {
//        if (g_pouDIL_CAN_Interface->DILC_SetConfigData((PCHAR)pControllerDetails,
//                        sizeof(SCONTROLLER_DETAILS)) == S_OK)
//        {
////            theApp.vRelease(CONTROLLER_DETAILS, (void**) &pControllerDetails);
//            bReturn = TRUE;
//        }
//    }
//
//    if(bReturn == TRUE )
//    {
//        // Init controller parameters
//        vSetControllerParameters();
//        // Clear UI Components
//        vClearOnConfLoad();
//        vInitCFileFunctPtrs();
//        vUpdateFuncStructsNodeSimEx((PVOID)&m_sExFuncPtr, UPDATE_ALL);
//    }
//  if(bFromCom == TRUE )
//  {
//      if( nReturn==defFILE_LOADED ||nReturn==defCONFIG_FILE_OPEN_FAILED)
//      //if called from COM interface return false if DLLs are loaded
//      bReturn= FALSE;
//  }
//    return bReturn;
//}

/******************************************************************************
 Function Name    : OnUpdateSignalWatchWnd

 Input(s)         : -
 Output           : -
 Functionality    : Updates th emenu item Window -> Signal Watch Window
 Member of        : CMainFrame
 Friend of        :     -

 Author(s)        : Raja N
 Date Created     : 13.04.2004
 Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateSignalWatchWnd(CCmdUI* pCmdUI)
{
    BOOL bShowHideFlag = FALSE;
    if (sg_pouSWInterface[CAN] != NULL)
    {
        bShowHideFlag = sg_pouSWInterface[CAN]->SW_IsWindowVisible()? TRUE : FALSE;
    }
    // Set Check as per the show status
    pCmdUI->SetCheck(bShowHideFlag);
}

/******************************************************************************
 Function Name    : pomGetMRUMenuPointer

 Input(s)         : -
 Output           : CMenu * - pointer to MRU menu item
 Functionality    : Returns the pointer of the MRU menu item "Recent Files"
                    or NULL in case of failure
 Member of        : CMainFrame
 Friend of        :     -

 Author(s)        : Raja N
 Date Created     : 28.04.2004
 Modifications    :
******************************************************************************/
CMenu* CMainFrame::pomGetMRUMenuPointer()
{
    // Get application main menu or NULL in case of failure
    CMenu* pMenu = GetMenu();

    if ( pMenu != NULL )// Verify
    {
        // Reach "File" menu option
        INT nPos = nFindMenuItem(pMenu, _(defSTR_FILE_MENU_TEXT));
        pMenu = pMenu->GetSubMenu( nPos );
        if ( pMenu != NULL )
        {
            // Reach "Recent Configurations" menu option
            nPos = nFindMenuItem(pMenu, _(defSTR_RECENT_MENU_TEXT));
            pMenu = pMenu->GetSubMenu( nPos );
        }
    }
    // Return the pointer or NULL
    return pMenu;
}

/******************************************************************************
    Function Name    :  vEnableDisableHandlers
    Input(s)         :  WPARAM wParam, LPARAM lParam
    Output           :
    Functionality    :  This function will call OnExecuteMessagehandlersButton()
                        OnExecuteKeyhandlers() or OnExecuteErrorHandlers member
                        function to enable or disable respective handlers. This
                        is an user defined message mapped function.
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  14.07.2004
    Modifications    :
******************************************************************************/
LRESULT CMainFrame::vEnableDisableHandlers(WPARAM wParam, LPARAM )
{
    eCANMONITORFLAG eWhichFlag = (eCANMONITORFLAG)wParam;
    switch(eWhichFlag)
    {
        case MSGHANDLERBUTTON:
            OnExecuteMessagehandlersButton();
            break;
        case KEY_HANDLER_ON:
            OnExecuteKeyhandlers();
            break;
        case ERROR_HANDLER:
            OnExecuteErrorHandlers();
            break;
    }
    return 0;
}





/******************************************************************************/
/*  Function Name    :  vSetControllerParameters                              */
/*  Input(s)         :    -                                                   */
/*  Output           :    -                                                   */
/*  Functionality    :  This function will set the controller parameters. This*/
/*                      will be called after updating the configuration info. */
/*                      This will take care of calling the HIL functions in a */
/*                      sequence depending upon the build                     */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  03.09.2004                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  Condition changed from || to && to solve problem of   */
/*                      controller mode gettting set to simulation always     */
/*                      while loading the configuration file.                 */
/*  Modifications    :  Raja N on 09.03.2005                                  */
/*                   :  Added code to support multiple contoller information  */
/*                      in the configuration module                           */
/******************************************************************************/
void CMainFrame::vSetControllerParameters()
{
    CFlags* podFlag = NULL;
    podFlag   = theApp.pouGetFlagsPtr();
    UCHAR ucControllerMode = defMODE_SIMULATE;  //venkat
    // Get the Controller mode
    LONG lParam = 0;
    if (g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, HW_MODE) == S_OK)
    {
        ucControllerMode = (UCHAR)lParam;
    }

    INT nNoOfHardware = 0;
    if (g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
    {
        nNoOfHardware = (INT)lParam;
    }
    // If it is active or passive then check the No. of Hw Available
    // If no real Hw available then switch to Simulation
    if( ucControllerMode != defMODE_SIMULATE && // Check for Simulation
            nNoOfHardware == 0 )
    {
        // Inform the user about this
        if(theApp.m_bFromAutomation == FALSE)
        {
            AfxMessageBox( _(defSTR_MODE_MISMATCH) );
        }

        // Set the mode to Simulation
        m_byControllerMode = defMODE_SIMULATE;
        IntializeDIL();
        // Update the Flag Status
        if( podFlag != NULL )
        {
            podFlag->vSetFlagStatus(CONTROLLER_MODE, defMODE_SIMULATE );
        }
    }
    // Not Simulation mode or enough hardware available
    else
    {
        // Update the controller mode
        if(podFlag != NULL )
        {
            UCHAR ucControllerMode = defMODE_SIMULATE;
            LONG lParam = 0;
            if (g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, HW_MODE) == S_OK)
            {
                ucControllerMode = (UCHAR)lParam;
            }
            // Set the flag to last used controller mode
            podFlag->vSetFlagStatus(CONTROLLER_MODE,
                                    static_cast <WORD>(ucControllerMode));
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vClearOnConfLoad                                      */
/*  Input(s)         :    -                                                   */
/*  Output           :    -                                                   */
/*  Functionality    :  This function will be called after configuration load.*/
/*                      This will take care of clearing messages/settings     */
/*                      that needs to be done after configuration load        */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  03.09.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CMainFrame::vClearOnConfLoad()
{
    if (m_podMsgWndThread != NULL)
    {
        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)shBusID);
            if(hWnd)
            {
                ::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, NULL, NULL);
            }
        }
    }

    // Clear Message Interpretation Window Content
    //vClearInterpretationWindow();
    // Clear Signal Watch Window
    //::PostMessage(GUI_hDisplayWindow, WM_REMOVE_SIGNAL, 0, 0);
    //::PostMessage(GUI_hDisplayWindow, WM_CLEAR_SIGNAL_WATCH, 0, 0);
}

/*******************************************************************************
  Function Name  : OnGraphWindow
  Input(s)       : -
  Output         : -
  Functionality  : Creates Graph UI thread for the first time. After that shows
                   or hides the window.
  Member of      : CMainFrame
  Author(s)      : Raja N
  Date Created   : 02/12/2004
  Modifications  :
*******************************************************************************/
void CMainFrame::OnGraphWindow()
{
    m_objSigGrphHandler.CreateGraphWindow(this, CAN);

    //SGW Code commented by Arun 21-10-2010
    //// Get the show status
    //int nShowStatus = m_bGraphWindowVisible ? SW_HIDE : SW_SHOW;
    //// Create UI thread only if it is not created and show flag is true
    //if( m_pomGraphThread == NULL && (nShowStatus == SW_SHOW) )
    //{
    //    // This function creates UI thread and window object
    //    bCreateGraphWindow();
    //}
    //// If thread is active then post the show message to the thread
    //// Note::
    //// The thread will update m_bGraphWindowVisible variable after displaying
    //// the window by a notification message to this class
    //if( m_pomGraphThread != NULL )
    //{
    //    m_pomGraphThread->PostThreadMessage( WM_MODIFY_VISIBILITY,
    //                                         nShowStatus, 0);
    //}
    //SGW Code commented by Arun 21-10-2010
}

/*******************************************************************************
  Function Name  : bCreateGraphWindow
  Input(s)       : -
  Output         : BOOL - TRUE on success. FALSE otherwise
  Functionality  : Creates Graph MDI child window.
  Member of      : CMainFrame
  Author(s)      : Raja N
  Date Created   : 02/12/2004
  Modifications  :
*******************************************************************************/
//SGW Code commented by Arun 21-10-2010
//BOOL CMainFrame::bCreateGraphWindow()
//{
//    BOOL bResult = FALSE;
//    // If thread doesn't exist
//    if (m_pomGraphThread == NULL)
//    {
//        // Create Graph UI Thread
//        m_pomGraphThread = (CGraphUIThread *)
//            AfxBeginThread( RUNTIME_CLASS(CGraphUIThread),
//                            THREAD_PRIORITY_NORMAL, 0, 0 );
//        // If success
//        if (m_pomGraphThread != NULL)
//        {
//            // Create Graph Window
//            bResult = m_pomGraphThread->nCreateGraphWindow( this,
//                                                            g_pouDIL_CAN_Interface,
//                                                            m_sGraphWndPlacement,
//                                                            m_sGraphSplitterPos);
//
//            if (bResult != 0)
//            {
//                // Terminate UI thread
//                TerminateThread(m_pomGraphThread->m_hThread, 0);
//                // Invalidate the pointer
//                m_pomGraphThread = NULL;
//            }
//            else
//            {
//                // Successfully created
//                m_pomGraphThread->bStartGraphReadThread();
//                bResult = TRUE;
//
//            }
//        }
//    }
//    // Return the result
//    return bResult;
//}
//SGW Code commented by Arun 21-10-2010

/*******************************************************************************
  Function Name  : OnUpdateGraphWnd
  Input(s)       : pCmdUI - UI context
  Output         : -
  Functionality  : Updates menuitem as per graph window status
  Member of      : CMainFrame
  Author(s)      : Raja N
  Date Created   : 02/12/2004
  Modifications  :
*******************************************************************************/
void CMainFrame::OnUpdateGraphWnd(CCmdUI* /*pCmdUI*/)
{
    // Set check if the window is visible
    //SGW Code commented by Arun 21-10-2010
    //pCmdUI->SetCheck( m_bGraphWindowVisible );
    //SGW Code commented by Arun 21-10-2010
}

/*******************************************************************************
  Function Name  : vUpdateGraphControl
  Input(s)       : -
  Output         : -
  Functionality  : This will update graph control with statistics parameters.
  Member of      : CMainFrame
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  : Arunkumar Karri on 09/02/2012
                   Moved the Bus statistics code into the If condition.
                   Raja N on 14.3.2005
                   Added logic to check the channel ID of statistics parameter.
*******************************************************************************/
void CMainFrame::vUpdateGraphStatsData()
{
    SINTERPRETDATA_LIST sInterpretList;

    for(int nBusType = CAN; nBusType<AVAILABLE_PROTOCOLS; nBusType++)
    {
        if(m_objSigGrphHandler.bIsWindowVisible((SHORT)nBusType) == FALSE)
        {
            continue;
        }
        CGraphList* pList = &(m_odGraphList[nBusType]);
        // Get the element count
        int nCount = pList->m_omElementList.GetSize();
        // Iterate through element list
        for( int nIndex = 0; nIndex < nCount; nIndex++ )
        {
            // Get Current element
            CGraphElement odTemp =
                pList->m_omElementList.GetAt( nIndex );
            // If the element is enabled and of type statistics
            if( odTemp.m_bEnabled == TRUE &&
                    odTemp.m_nValueType == eSTAT_PARAM )
            {
                // Channel Index starts from 0. So sub 1 from the index value
                odTemp.m_nFrameFormat -= 1;
                SBUSSTATISTICS sBusStats;
                GetICANBusStat()->BSC_GetBusStatistics(odTemp.m_nFrameFormat, sBusStats);

                strcpy_s(sInterpretList.m_acSigName , 64, "");
                sInterpretList.m_shType = eSTAT_PARAM;

                sInterpretList.m_unValue.m_nRawValue = -1;
                sInterpretList.m_unValue.m_dPhysical = -1.0;
                sInterpretList.unMsgID = nIndex;
                sInterpretList.m_nTimeStamp = 0;
                // switch the stat item
                switch( odTemp.m_nMsgID )
                {
                    case 0: // "Total Messages"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unTotalMsgCount;
                        break;
                    case 1: // "Message Rate"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unMsgPerSecond;
                        break;
                    case 2: // "Total Errors"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unErrorTotalCount;
                        break;
                    case 3: // "Error Rate"
                        sInterpretList.m_unValue.m_dPhysical = sBusStats.m_dErrorRate;
                        break;
                    case 4: // "Network Load"
                        sInterpretList.m_unValue.m_dPhysical = sBusStats.m_dBusLoad;
                        break;
                    case 5: // "Peak Network Load"
                        sInterpretList.m_unValue.m_dPhysical = sBusStats.m_dPeakBusLoad;
                        break;
                    case 6: // "Tx Message Total"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unTotalTxMsgCount;
                        break;
                    case 7: // "Standard Tx Message"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unTxSTDMsgCount;
                        break;
                    case 8: // "Standard Tx Rate"
                        sInterpretList.m_unValue.m_dPhysical = sBusStats.m_dTxSTDMsgRate;
                        break;
                    case 9: // "Extended Tx Message"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unTxEXTDMsgCount;
                        break;
                    case 10: // "Extended Tx Rate"
                        sInterpretList.m_unValue.m_dPhysical = sBusStats.m_dTxEXTMsgRate;
                        break;
                    case 11: // "Standard.Tx RTR"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unTxSTD_RTRMsgCount;
                        break;
                    case 12: // "Extended Tx RTR"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unTxEXTD_RTRMsgCount;
                        break;
                    case 13: // "Tx Errors"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unErrorTxCount;
                        break;
                    case 14: // "Tx Error Rate"
                        sInterpretList.m_unValue.m_dPhysical = sBusStats.m_dErrorTxRate;
                        break;
                    case 15: // "Rx Message Total"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unTotalRxMsgCount;
                        break;
                    case 16: // "Standard Rx Message"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unRxSTDMsgCount;
                        break;
                    case 17: // "Standard Rx Rate"
                        sInterpretList.m_unValue.m_dPhysical = sBusStats.m_dRxSTDMsgRate;
                        break;
                    case 18: // "Extended Rx Message"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unRxEXTDMsgCount;
                        break;
                    case 19: // "Extended Rx Rate"
                        sInterpretList.m_unValue.m_dPhysical = sBusStats.m_dRxEXTMsgRate;
                        break;
                    case 20: // "Standard.Rx RTR"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unRxSTD_RTRMsgCount;
                        break;
                    case 21: // "Extended Rx RTR"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unRxEXTD_RTRMsgCount;
                        break;
                    case 22: // "Rx Errors"
                        sInterpretList.m_unValue.m_nRawValue = sBusStats.m_unErrorRxCount;
                        break;
                    case 23: // "Rx Error Rate"
                        sInterpretList.m_unValue.m_dPhysical = sBusStats.m_dErrorRxRate;
                        break;
                    default:
                        // Invalid stat parameter index.
                        ASSERT( FALSE );
                }//switch
                m_pouMsgInterpretBuffer->WriteIntoBuffer(nBusType,(BYTE*)&sInterpretList,SIZE_INTRP_DATA);
            }
        }
    }
}

/*******************************************************************************
  Function Name  : vRestoreWindowPostion
  Input(s)       : -
  Output         : -
  Functionality  : This function will call each window that has window postion
                   save/restore functinality to restore postion from
                   configuration module
  Member of      : CMainFrame
  Author(s)      : Raja N
  Date Created   : 18.4.2005
  Modifications  : Raja N on 19.07.2005, Removed support for Replay Window
  Modifications  : Anish on 26.10.2006, Buildprogram pointer accessed
                    from EXecutemanager
*******************************************************************************/
void CMainFrame::vRestoreWindowPostion()
{
    //m_pouBuildProgram = CExecuteManager::ouGetExecuteManager().m_pouBuildProgram;
    //       // Output window coordinates
    //   if(m_pouBuildProgram != NULL)
    //   {
    //       if(m_pouBuildProgram->m_podOutWnd != NULL)
    //       {
    //           if(m_pouBuildProgram->m_podOutWnd->m_hWnd != NULL)
    //           {
    //               // so that only the data for the output window is set..
    //               m_pouBuildProgram->m_podOutWnd->vUpdateWinStatus();
    //           }
    //       }
    //   }
    // Get Message window, signal watch and message interptetation window
    // postions
    /*if (m_podMsgMDIChild != NULL)
    {
        if (::IsWindow(m_podMsgMDIChild->m_hWnd))
        {
            m_podMsgMDIChild->vUpdateWinStatus();
        }
        // Save Signal Watch Window position
        m_podMsgMDIChild->vUpdateSignalWatchWindow(FALSE);
        // Save Message Interpretation window position
        m_podMsgMDIChild->vUpdateInterpretationWindow(FALSE);
    }*/
    // Get Trace Window Postion
    if ((m_podUIThread != NULL) && (m_bNotificWndVisible == TRUE))
    {
        m_podUIThread->PostThreadMessage(WM_LOAD_SAVE_WINSTATUS, TRUE, 0);
    }

    //SGW Code commented by Arun 21-10-2010
    // Update Graph window
    /*if( m_pomGraphThread != NULL )
    {
        m_pomGraphThread->pomGetChildFramePointer()->vUpdateWinStatus();
    }*/
    //SGW Code commented by Arun 21-10-2010

    // Update Tx Window
    /*if( m_pomTxMsgChildWindow != NULL )
    {
        m_pomTxMsgChildWindow->vUpdateWinStatus();
    }*/
}

/*******************************************************************************
  Function Name  : vStartStopLogging
  Input(s)       : bStart - TRUE to start and FALSE to stop logging
  Output         : -
  Functionality  : To start/stop logging
  Member of      : CMainFrame
  Author(s)      : Raja N
  Date Created   : 19.7.2005
  Modifications  :
*******************************************************************************/
void CMainFrame::vStartStopLogging(BOOL bStart)
{
    // Enable Logging or stop logging
    if (NULL != sg_pouFrameProcCAN)
    {
        // To reduce the unwanted functional call incase of stop logging
        if (bStart)
        {
            vSetAssociatedDatabaseFiles(CAN);   // Update the db file names associated
            vSetBaudRateInfo(CAN);              // Update the baud rate details
        }

        sg_pouFrameProcCAN->FPC_EnableLogging(bStart);
    }
    //LogKadoor CLogManager::ouGetLogManager().vStartStopLogging( bStart );
}

/*******************************************************************************
  Function Name  : vStartStopLogging_LIN
  Input(s)       : bStart - TRUE to start and FALSE to stop logging
  Output         : -
  Functionality  : To start/stop logging
  Member of      : CMainFrame
  Author(s)      : Shashank Vernekar
  Date Created   : 29.01.2014
  Modifications  :
*******************************************************************************/
//[RS_LIN_06_14]
void CMainFrame::vStartStopLogging_LIN(BOOL bStart)
{
    // Enable Logging or stop logging
    if (NULL != sg_pouFrameProcLIN)
        //  if (NULL != sg_pouFrameProcLIN)
    {
        // To reduce the unwanted functional call incase of stop logging
        if (bStart)
        {
            vSetAssociatedDatabaseFiles(LIN);   // Update the db file names associated

            //  vSetAssociatedDatabaseFiles(LIN);

            vSetBaudRateInfo(LIN);              // Update the baud rate details

            //vSetBaudRateInfo(LIN);
        }

        sg_pouFrameProcLIN->FPL_EnableLogging(bStart);
    }
    //LogKadoor CLogManager::ouGetLogManager().vStartStopLogging( bStart );
}

/*******************************************************************************
  Function Name  : vSetAssociatedDatabaseFiles
  Input(s)       : -
  Output         : -
  Functionality  : To update the associated database file names for logging
  Member of      : CMainFrame
  Author(s)      : Saravanan K S
  Date Created   : 22.03.2012
  Modifications  :
*******************************************************************************/
void CMainFrame::vSetAssociatedDatabaseFiles(ETYPE_BUS eBus)
{
    // Get the list of files associated to the application
    CStringArray aomDataBaseFiles;



    if (CAN == eBus)
    {
        theApp.m_pouMsgSignal->vGetDataBaseNames(&aomDataBaseFiles);
    }
    else if (J1939 == eBus)
    {
        m_pouMsgSigJ1939->vGetDataBaseNames(&aomDataBaseFiles);
    }
    else if(LIN==eBus)
    {

        //m_pouMsgSigLIN->vGetDataBaseNames(&linLDF);
    }

    if (NULL != sg_pouFrameProcCAN)
    {
        if (CAN == eBus)
        {
            sg_pouFrameProcCAN->FPC_SetDatabaseFiles(aomDataBaseFiles);
        }
        else if (J1939 == eBus)
        {
            sg_pouIJ1939Logger->FPJ1_SetDatabaseFiles(aomDataBaseFiles);
        }
        else if (LIN == eBus)
        {
            CStringArray m_saLDFPaths;
            string sLDFPath;

            for(int i = 0; i < defNO_OF_LIN_CHANNELS; i++)
            {
                sLDFPath=m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_strDataBasePath;
                if((sLDFPath !=STR_EMPTY))
                {
                    m_saLDFPaths.Add(sLDFPath.c_str());
                }
            }

            sg_pouFrameProcLIN->FPL_SetDatabaseFiles(m_saLDFPaths);
        }
    }
}

/*******************************************************************************
  Function Name  : vSetBaudRateInfo
  Input(s)       : -
  Output         : -
  Functionality  : To update the channel baudrate information for logging
  Member of      : CMainFrame
  Author(s)      : Saravanan K S
  Date Created   : 22.03.2012
  Modifications  :
*******************************************************************************/
void CMainFrame::vSetBaudRateInfo(ETYPE_BUS eBus)
{
    if (CAN == eBus)
    {
        if (NULL != sg_pouFrameProcCAN)
        {
            // Update the baudrate info and the number of channels used
            sg_pouFrameProcCAN->FPC_SetChannelBaudRateDetails
            (m_asControllerDetails, m_nNumChannels,eBus);
        }
    }
    else if (J1939 == eBus)
    {
        if (NULL != sg_pouIJ1939Logger)
        {
            // Update the baudrate info and the number of channels used
            sg_pouIJ1939Logger->FPJ1_SetChannelBaudRateDetails
            (m_asControllerDetails, m_nNumChannels,eBus);
        }
    }

    else if(LIN == eBus)
    {
        if(NULL != sg_pouFrameProcLIN)
        {

            if ( g_pouDIL_LIN_Interface != NULL )
            {

                int nCount = 0;
                g_pouDIL_LIN_Interface->DILL_GetConfiguration(m_asControllerDetailsLIN, nCount);
                sg_pouFrameProcLIN->FPL_SetChannelBaudRateDetails(m_asControllerDetailsLIN,nCount,eBus);
            }
        }

    }
}

/*******************************************************************************
  Function Name  : vComStartStopLog
  Input(s)       : bStart - TRUE to start and FALSE to stop logging
  Output         : -
  Functionality  : to be called from COM functionTo start/stop logging
                   inline function gives linking error
  Member of      : CMainFrame
  Author(s)      : Raja N
  Date Created   : 19.7.2005
  Modifications  :
*******************************************************************************/
void CMainFrame::vComStartStopLog(BOOL bStart)
{
    vStartStopLogging(bStart);
}

void CMainFrame::vComStartStopLog_LIN(BOOL bStart)
{
    vStartStopLogging_LIN(bStart);
}

/*******************************************************************************
  Function Name  : OnCfgnReplay
  Input(s)       : -
  Output         : -
  Functionality  : Message handler for configuring replay files
  Member of      : CMainFrame
  Author(s)      : Raja N
  Date Created   : 19.7.2005
  Modifications  :
*******************************************************************************/
void CMainFrame::OnCfgnReplay()
{
    // Show configuration dialog
    vREP_DisplayReplayConfigDlg(CAN, &m_sFilterAppliedCAN);
}

/*******************************************************************************
  Function Name  : OnUpdateCfgnReplay
  Input(s)       : -
  Output         : -
  Functionality  : UI Update handler for Replay menu. This will be disabled if
                   tool is connected
  Member of      : CMainFrame
  Author(s)      : Raja N
  Date Created   : 19.7.2005
  Modifications  :
*******************************************************************************/
void CMainFrame::OnUpdateCfgnReplay(CCmdUI* pCmdUI)
{
    if( pCmdUI != NULL )
    {
        // Connection status
        pCmdUI->Enable( m_bCfgNewMenuOption );
    }
}

/*******************************************************************************
  Function Name  : omGetDllName
  Input(s)       : -
  Output         : Dll path with .c ext
  Functionality  : It sends the path of compiled file which is assigned after
                    building from CBuildProgram
  Member of      : CMainFrame
  Author(s)      : Anish kumar
  Date Created   : 27.12.2005
  Modifications  :
*******************************************************************************/
CString CMainFrame::omGetDllName()
{
    return m_omStrSourceFilePathName;
}

/******************************************************************************/
/*  Function Name    :  OnConfigureSimulatedsystems                                    */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects Simulated */
/*                      system menu item.                                     */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  22.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
void CMainFrame::OnConfigureSimulatedsystems()
{
    vInitCFileFunctPtrs();
    GetICANNodeSim()->FE_CreateFuncEditorTemplate(this->GetSafeHwnd(), m_sExFuncPtr[CAN]);
}

/******************************************************************************/
/*  Function Name    :  OnConfigureSimulatedsystemsLin                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when the user selects Simulated */
/*                      system menu item.                                     */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :                                                        */
/*  Modifications    :                                                        */
/******************************************************************************/
void CMainFrame::OnConfigureSimulatedsystemsLin()
{
    vNS_LINInitCFileFunctPtrs();
    GetILINNodeSim()->FE_CreateFuncEditorTemplate(this->GetSafeHwnd(), m_sExFuncPtr[LIN]);
}

/******************************************************************************/
/*  Function Name    :  App_SendMsgLIN                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Transmitting Lin message, directing to resp. DIL      */
/*                      system menu item.                                     */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :                                                        */
/*  Modifications    :                                                        */
/******************************************************************************/
static int App_SendMsgLIN(void* pMsg, HMODULE /*hModule*/)
{
    STLIN_MSG* psMsg = (STLIN_MSG*)pMsg;
    return g_pouDIL_LIN_Interface->DILL_SendMsg(g_dwClientID,*psMsg);
}

/******************************************************************************/
/*  Function Name    :  vNS_LINInitCFileFunctPtrs                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Transmitting Lin message, directing to resp. DIL      */
/*                      system menu item.                                     */
/*  Member of        :  CMainFrame                                            */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :                                                        */
/*  Modifications    :                                                        */
/******************************************************************************/
void CMainFrame::vNS_LINInitCFileFunctPtrs()
{
    //vSetAppService((CBaseAppServices *) &sg_ouAppServiceObj);

    m_sExFuncPtr[LIN].m_hWmdMDIParentFrame = this->GetSafeHwnd();
    //m_sExFuncPtr.m_pouTraceWnd = m_podUIThread;
    m_sExFuncPtr[LIN].m_omAPIList.RemoveAll();
    for (int nIndex = 0; nIndex < TOTAL_API_COUNT_LIN; nIndex++)
    {
        m_sExFuncPtr[LIN].m_omAPIList.Add(sg_omAPIFuncListLIN[nIndex]);
    }

    m_sExFuncPtr[LIN].m_omAPINames.RemoveAll();
    for (int nIndex = 0; nIndex < TOTAL_API_COUNT_LIN; nIndex++)
    {
        m_sExFuncPtr[LIN].m_omAPINames.Add(sg_omAPIFuncNamesLIN[nIndex]);
    }

    m_sExFuncPtr[LIN].m_omErrorHandlerList.RemoveAll();
    for (int nIndex = 0; nIndex < TOTAL_ERROR_COUNT; nIndex++)
    {
        m_sExFuncPtr[LIN].m_omErrorHandlerList.Add(sg_omMcNetErrorHandlerList[nIndex]);
    }

    m_sExFuncPtr[LIN].m_omDefinedMsgHeaders.RemoveAll();
    CStringArray omDatabaseNames;
    // TODO: LIN Commented generation of unions.h file
    /*if( NULL != m_pouMsgSigLIN)
    {
        m_pouMsgSigLIN->vGetDataBaseNames(&omDatabaseNames);
    }*/

    //theApp.m_pouMsgSignal->vGetDataBaseNames(&omDatabaseNames);
    GetILINNodeSim()->NS_SetLINConfig(m_ouClusterConfig[LIN]);

    for (int nIndex = 0; nIndex < m_ouClusterConfig[LIN].m_nChannelsConfigured; nIndex++)
    {
        CString omHeaderPath = omStrGetUnionFilePath(m_ouClusterConfig[LIN].m_ouFlexChannelConfig[nIndex].m_strDataBasePath.c_str());
        m_sExFuncPtr[LIN].m_omDefinedMsgHeaders.Add(omHeaderPath);
    }

    // vUpdateMsgNameCodeList(theApp.m_pouMsgSignal, m_sExFuncPtr[LIN].m_odMsgNameMsgCodeListDB);
    // Send KeyPanel list pointer
    //m_sExFuncPtr.m_podNodeToDllMap = &g_odNodeToDllMap;
    //m_sExFuncPtr.m_podKeyPanelEntryList = &g_odKeyPanelEntryList;
    m_sExFuncPtr[LIN].m_omAppDirectory = m_omAppDirectory;
    m_sExFuncPtr[LIN].m_omObjWrapperName = WRAPPER_NAME_LIN;
    m_sExFuncPtr[LIN].m_omStructFile = STRUCT_FILE_LIN;
    m_sExFuncPtr[LIN].m_omStructName = MSG_STRUCT_NAME_LIN;
    m_sExFuncPtr[LIN].Send_Msg = App_SendMsgLIN;
    m_sExFuncPtr[LIN].DisConnectTool = NULL;
    m_sExFuncPtr[LIN].EnDisableLog = NULL;
    m_sExFuncPtr[LIN].WriteToLog = NULL;
    m_sExFuncPtr[LIN].RestController = NULL;
    m_sExFuncPtr[LIN].m_pouITraceWndPtr = &sg_ouAppServiceObj;
    //m_sExFuncPtr.m_pNetWorkMcNet = &m_odNetwork;

}
/******************************************************************************
    Function Name    :  OnUpdateDllLoadAll

    Input(s)         :  CCmdUI* pCmdUI
    Output           :  -
    Functionality    :  Enables or Disables the menu option
                        depending on the FUNCEDITOR flag.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  29.03.2002
    Modifications    :
******************************************************************************/
void CMainFrame::OnUpdateDllLoadAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetICANNodeSim()->NS_ShouldToolBarBeEnabled(TB_LOAD));
}

void CMainFrame::OnUpdateLINDllLoadAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetILINNodeSim()->NS_ShouldToolBarBeEnabled(TB_LOAD));
}

/******************************************************************************
    Function Name    :  OnHelpInfo

    Input(s)         :  HELPINFO* pHelpInfo
    Output           :  -
    Functionality    :  -
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Anish kumar
    Date Created     :  27.04.06
    Modifications    :
******************************************************************************/
BOOL CMainFrame::OnHelpInfo(HELPINFO* pHelpInfo)
{
    return CMDIFrameWnd::OnHelpInfo(pHelpInfo);
}

/******************************************************************************
    Function Name    :  WinHelp

    Input(s)         :  -
    Output           :  -
    Functionality    :  sets the helpId of controls for which user has
                        requested for help and style of help window
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Anish kumar
    Date Created     :  27.04.06
    Modifications    :
******************************************************************************/
void CMainFrame::WinHelp(DWORD , UINT )
{
    // Get Application Help File Path
    CString omStrPath = theApp.m_pszHelpFilePath;
    // Replace .hlp with .chm
    int nIndex = omStrPath.ReverseFind( PERIOD );
    // Extract string before the extension
    omStrPath = omStrPath.Mid(0, nIndex );
    // Add New Extension
    omStrPath = omStrPath + ".chm";
    // Make it as content display always
    ::HtmlHelp(NULL, omStrPath, HH_DISPLAY_TOPIC, 0);
}

/******************************************************************************
    Function Name    :  OnDissociateDatabase

    Input(s)         :  -
    Output           :  -
    Functionality    :  Display the Dissociatedatabase dialog
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Anish kumar
    Date Created     :  06.12.06
    Modifications    :
******************************************************************************/
void CMainFrame::OnDissociateDatabase()
{
    sg_asDbParams[CAN].m_ppvActiveDB = (void**)&(theApp.m_pouMsgSgInactive);
    sg_asDbParams[CAN].m_ppvImportedDBs = (void**)&(theApp.m_pouMsgSignal);
    CDatabaseDissociateDlg odDBDialog(sg_asDbParams[CAN]);
    odDBDialog.DoModal();
    //Update Defined Msg Headers
    m_sExFuncPtr[CAN].m_omDefinedMsgHeaders.RemoveAll();
    CStringArray omDatabaseNames;
    theApp.m_pouMsgSignal->vGetDataBaseNames(&omDatabaseNames);
    for (int i = 0; i < omDatabaseNames.GetSize(); i++)
    {
        CString omHeaderPath = omStrGetUnionFilePath(omDatabaseNames.GetAt(i));
        m_sExFuncPtr[CAN].m_omDefinedMsgHeaders.Add(omHeaderPath);
    }
    //Update in NodeSimEx
    GetICANNodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[CAN].m_omDefinedMsgHeaders), UPDATE_UNIONS_HEADER_FILES);
    //Update Msg Name<-->Msg Code List
    vUpdateMsgNameCodeList(theApp.m_pouMsgSignal, m_sExFuncPtr[CAN].m_odMsgNameMsgCodeListDB);
    //Update in NodeSimEx
    GetICANNodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[CAN].m_odMsgNameMsgCodeListDB), UPDATE_DATABASE_MSGS);
}

/******************************************************************************
    Function Name    :  vUpdateAllMsgWndInterpretStatus
    Input(s)         :  bAssociate
    Output           :  -
    Functionality    :  Updates the interpret status of message windows.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Anish kumar
    Date Created     :  05.04.11
    Modifications    :
******************************************************************************/
void CMainFrame::vUpdateAllMsgWndInterpretStatus(BOOL /*bAssociate*/)
{
    HWND hWnd = NULL;
    //Update MsgWnds
    for(register int nBusID = CAN; nBusID < AVAILABLE_PROTOCOLS; nBusID++)
    {
        hWnd = m_podMsgWndThread->hGetHandleMsgWnd((eTYPE_BUS)nBusID);
        if(hWnd)
        {
            ::SendMessage(hWnd, WM_DATABASE_CHANGE, (WPARAM)FALSE, NULL);
        }
    }
}

/******************************************************************************
    Function Name    :  OnFileConverter

    Input(s)         :  -
    Output           :  -
    Functionality    :  Called from frame work. This function will execute
                        CAPL_2_C.exe which resides in the application
                        directory.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Venkatanarayana Makam
    Date Created     :  18.12.2011
    Modifications    :
******************************************************************************/
void CMainFrame::OnFileConverter()
{
    try
    {
        // If window is already created and displayed then just bring it to front
        //m_hProcess = NULL; //KSS
        CWnd* pWndCreated = IsWindowCreated();
        if (NULL != pWndCreated && pWndCreated->GetSafeHwnd())
        {
            pWndCreated->SetForegroundWindow();
            return;
        }

        // Get the working directory
        char acPath[MAX_PATH] = "";
        GetModuleFileName( NULL, acPath, MAX_PATH );
        PathRemoveFileSpec(acPath);
        CString strPath = acPath;
        strPath += "\\FormatConverter.exe";

        if(PathFileExists(strPath) == TRUE)
        {
            // Launch the converter utility
            PROCESS_INFORMATION sProcessInfo;
            STARTUPINFO sStartInfo;

            memset(&sProcessInfo, 0, sizeof(PROCESS_INFORMATION));
            memset(&sStartInfo, 0, sizeof(STARTUPINFO));

            INT nSuccess = CreateProcess(   strPath.GetBuffer(MAX_PATH), "", NULL, NULL,
                                            true, CREATE_NO_WINDOW, NULL, NULL,
                                            &sStartInfo, &sProcessInfo);
            if (nSuccess != 0)
            {
                m_hProcess = sProcessInfo.hProcess;
            }
        }
    }
    catch(...)
    {
    }

}
CWnd* CMainFrame::IsWindowCreated()
{
    if (NULL == m_hProcess) //KSS
    {
        return NULL;
    }
    // Check if the window is already created
    CWnd* pWndCreated = NULL;
    try
    {
        CWnd* pWnd = FindWindow(NULL, _("BUSMASTER Format Conversions"));

        // Following check is necessay to get the process created by a particular instance
        // of the application and to avoid unncessary closing or showing the process created
        // by another instance of the application
        if (NULL != pWnd)
        {
            // Get the process id of the process created by using the process handle
            DWORD dwId = GetProcessId(m_hProcess);
            // Open the process
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwId);

            if (NULL != hProcess)
            {
                // Get the first window
                HWND hWnd = ::GetTopWindow(0);
                DWORD dwWndProcId = 0;
                DWORD dwTheardProcId = 0;
                HANDLE hThreadProc = NULL;

                // Loop through all the windows and get the window handle
                while ( NULL != hWnd )
                {
                    dwTheardProcId = GetWindowThreadProcessId(hWnd, &dwWndProcId);
                    hThreadProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwWndProcId);
                    if (NULL != hThreadProc && dwWndProcId == dwId)
                    {
                        pWndCreated = CWnd::FromHandle(hWnd);
                        break;
                    }
                    hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
                }
            }
            else
            {
                DWORD dwErr = GetLastError();
            }
        }
    }
    catch(...)
    {
    }

    return pWndCreated;
}

void CMainFrame::vCloseFormatconverters()
{
    // Close format converters window if it is already displayed
    ////Method 1
    //CWnd* pWndCreated = IsWindowCreated();
    //if (NULL != pWndCreated && pWndCreated->GetSafeHwnd())
    //{
    //  pWndCreated->SendMessageA(WM_CLOSE);
    //}

    try
    {
        //Method 2
        CWnd* pWnd = NULL;
        DWORD dwExitCode = 0;

        if (NULL != m_hProcess) //KSS
        {
            pWnd = FindWindow(NULL, _("BUSMASTER Format Conversions"));
            if (NULL != pWnd)
            {
                if (GetExitCodeProcess (m_hProcess, &dwExitCode))
                {
                    TerminateProcess(m_hProcess, dwExitCode);
                }
                else
                {
                    TerminateProcess(m_hProcess, 0);
                }
            }
        }
    }
    catch (...)
    {
    }

    ////Method 3
    //HANDLE   hProc ;
    //DWORD   dwRet ;
    //DWORD dwPID = (DWORD)m_hProcess;
    //DWORD dwTimeout = 1000;

    //// If we can't open the process with PROCESS_TERMINATE rights,
    //// then we give up immediately.
    //hProc = OpenProcess(PROCESS_TERMINATE, FALSE, dwPID);
    //if(hProc == NULL)
    //{
    //  DWORD dwErr = GetLastError();
    //  return;
    //}

    //// TerminateAppEnum() posts WM_CLOSE to all windows whose PID
    //// matches your process's.
    //EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM) dwPID) ;

    //// Wait on the handle. If it signals, great. If it times out,
    //// then you kill it.
    //if(WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0)
    //  TerminateProcess(hProc,0);
    //else
    //  return;

    //CloseHandle(hProc) ;
}

BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam )
{
    DWORD dwID ;

    GetWindowThreadProcessId(hwnd, &dwID) ;

    if(dwID == (DWORD)lParam)
    {
        PostMessage(hwnd, WM_CLOSE, 0, 0) ;
    }

    return TRUE ;
}


/******************************************************************************
    Function Name    :  OnSaveImportDatabase

    Input(s)         :  -
    Output           :  -
    Functionality    :  This function will save and import currently edited(open)
                        database file.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Pradeep Kadoor
    Date Created     :  012.06.2009
    Modifications    :
******************************************************************************/
void CMainFrame::OnSaveImportDatabase()
{
    OnConfigDatabaseSave();
    dLoadDataBaseFile(m_omStrDatabaseName, FALSE);
}

void CMainFrame::OnUpdateSaveImportDatabase(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ));
}

/**
* \brief         This function will save and import currently edited(open)
                 database file.
* \param[in]     NULL
* \return        void
* \authors       Arunkumar Karri
* \date          12.06.2013 Created
*/
void CMainFrame::OnSaveImportJ1939Database()
{
    OnJ1939DBSave();
    dLoadJ1939DBFile(m_podMsgSgWndJ1939->m_sDbParams.m_omDBPath,FALSE);
}

/**
* \brief         This function will update the menu status of J1939 Save
                 and Import
* \param[in]     pCmdUI, is pointer to CCmdUI
* \return        void
* \authors       Arunkumar Karri
* \date          12.06.2013 Created
*/
void CMainFrame::OnUpdateSaveImportJ1939Database(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN_J1939 ));
}

/******************************************************************************
    Function Name    :  OnMessageTraceWnd

    Input(s)         :  -
    Output           :  -
    Functionality    :  This function will show the trace window. Trace window
                        will be created if it does not exists.
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Pradeep Kadoor
    Date Created     :  012.06.2009
    Modifications    :
******************************************************************************/
LRESULT CMainFrame::OnMessageTraceWnd(WPARAM /*wPAram*/, LPARAM /*lParam*/)
{
    if ((m_podUIThread == NULL) && (!m_bNotificWndVisible))
    {
        bCreateTraceWindow();
    }

    if (m_podUIThread != NULL)
    {
        m_podUIThread->PostThreadMessage(WM_THREADMSG_PROC, WM_MODIFY_VISIBILITY, SW_SHOW);
        m_bNotificWndVisible = TRUE;
    }
    return S_OK;
}

void CMainFrame::vReRegisterAllCANNodes(void)
{
    //Problem here
    xmlNodePtr pCANSimSys = NULL;
    GetICANNodeSim()->NS_GetSimSysConfigData(pCANSimSys);
    CStringArray pomErrorFiles;
    GetICANNodeSim()->NS_DllUnloadAll(&pomErrorFiles);
    GetICANNodeSim()->NS_SetSimSysConfigData(pCANSimSys);
    vInitCFileFunctPtrs();
    GetICANNodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[CAN]), UPDATE_ALL);
    xmlFreeNode(pCANSimSys);
}
void CMainFrame::vReRegisterAllLINNodes(void)
{
    xmlNodePtr pLinSimSys = NULL;
    GetILINNodeSim()->NS_GetSimSysConfigData(pLinSimSys);
    CStringArray pomErrorFiles;
    GetILINNodeSim()->NS_DllUnloadAll(&pomErrorFiles);
    GetILINNodeSim()->NS_SetSimSysConfigData(pLinSimSys);
    vInitCFileFunctPtrs();
    GetILINNodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[LIN]), UPDATE_ALL);
    xmlFreeNode(pLinSimSys);
}

void CMainFrame::vReRegisterAllJ1939Nodes(void)
{
    xmlNodePtr pJ1939SimSys = NULL;
    GetIJ1939NodeSim()->NS_GetSimSysConfigData(pJ1939SimSys);
    CStringArray pomErrorFiles;
    GetIJ1939NodeSim()->NS_DllUnloadAll(&pomErrorFiles);
    GetIJ1939NodeSim()->NS_SetSimSysConfigData(pJ1939SimSys);
    vInitCFileFunctPtrs();
    NS_InitJ1939SpecInfo();
    GetIJ1939NodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[J1939]), UPDATE_ALL);
    xmlFreeNode(pJ1939SimSys);
}
HRESULT CMainFrame::InitializeFLEXRAYDIL()
{
    HRESULT hResult = S_OK;
    m_bNoHardwareFound = true;
    string  strLastError;
    HRESULT hLastError;
    static char chError[256]; //TODO::For trace window
    if (g_pouDIL_FLEXRAY_Interface == NULL)
    {
        hResult = DIL_GetInterface(FLEXRAY, (void**)&g_pouDIL_FLEXRAY_Interface);
    }
    else
    {
        g_pouDIL_FLEXRAY_Interface->DILF_PerformClosureOperations();
        //m_objFlexTxHandler.vFlexFileChanged();
    }
    if (hResult == S_OK)
    {
        if ((hResult = g_pouDIL_FLEXRAY_Interface->DILF_SelectDriver(m_shFLEXRAYDriverId, m_hWnd, &m_ouWrapperLogger)) == S_OK)
        {
            if ( ( hResult = g_pouDIL_FLEXRAY_Interface->DILF_ListHwInterfaces( m_sSelFlxHwInterface[0], m_acFlexDBConfigInfo )) == S_OK )
            {
                if ( ( hResult = g_pouDIL_FLEXRAY_Interface->DILF_SelectHwInterfaces( m_sSelFlxHwInterface )) == S_OK )
                {
                    hResult = g_pouDIL_FLEXRAY_Interface->DILF_SetConfigData(m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[0]) ;


                    if ( hResult == S_OK )
                    {
                        /* Register the Monitor client buffer */
                        hResult = g_pouDIL_FLEXRAY_Interface->DILF_RegisterClient(TRUE, m_dwFLEXClientID, _T("CAN_MONITOR"));

                        // Create the message window
                        m_podMsgWndThread->vUpdateClientID(FLEXRAY, m_dwFLEXClientID);
                        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_SHOW, (LONG)FLEXRAY);
                        m_objFlexTxHandler.vSetDILInterfacePtr(FLEXRAY, NULL);
                        vInitializeBusStatFlexRay();
                        //string strConfigName = (PCHAR)m_acFlexDBConfigInfo.m_acConfigFileName[m_acFlexDBConfigInfo.m_nSelectedIndex];
                        m_objFlexTxHandler.vFlexFileChanged();
                        //bCreateFlexRayMsgWindow();
                    }
                    else
                    {
                        g_pouDIL_FLEXRAY_Interface->DILF_GetLastErrorString(strLastError, hLastError);
                        sprintf_s(chError, "Error From Driver :: 0x%X", hLastError);
                        theApp.bWriteIntoTraceWnd("FLEXRAY DIL: Setting Initialization configuration failed!");
                        theApp.bWriteIntoTraceWnd(chError);
                    }
                }
                else
                {
                    g_pouDIL_FLEXRAY_Interface->DILF_GetLastErrorString(strLastError, hLastError);
                    sprintf_s(chError, "Error From Driver :: 0x%X", hLastError);
                    theApp.bWriteIntoTraceWnd("FLEXRAY DIL: Selection of Hardware Interface failed!");
                    theApp.bWriteIntoTraceWnd(chError);
                }
            }
            else
            {
                g_pouDIL_FLEXRAY_Interface->DILF_GetLastErrorString(strLastError, hLastError);
                sprintf_s(chError, "Error From Driver :: 0x%X", hLastError);
                theApp.bWriteIntoTraceWnd("FLEXRAY DIL: Channel selection failed. Please check if your hardware is connected.");
                theApp.bWriteIntoTraceWnd(chError);
            }
        }
        else
        {
            if ( m_shFLEXRAYDriverId != DAL_NONE && m_shFLEXRAYDriverId !=FLEXRAY_DRIVER_STUB )
            {
                g_pouDIL_FLEXRAY_Interface->DILF_GetLastErrorString(strLastError, hLastError);
                sprintf_s(chError, "Error From Driver :: 0x%X", hLastError);
                theApp.bWriteIntoTraceWnd("FLEXRAY DIL: Driver selection failed. Please check the Hardware support wiki page on https://github.com/rbei-etas/busmaster/wiki/Hardware-support ");
                theApp.bWriteIntoTraceWnd(chError);
            }
        }
    }
    else
    {
        g_pouDIL_FLEXRAY_Interface->DILF_GetLastErrorString(strLastError, hLastError);
        sprintf_s(chError, "Error From Driver :: 0x%X", hLastError);
        theApp.bWriteIntoTraceWnd("FLEXRAY DIL: Failed to get DIL Interface object!");
        theApp.bWriteIntoTraceWnd(chError);

    }
    return hResult;
}
void CMainFrame::OnFlexRayConnect()
{
	CFlags* pouFlag  = theApp.pouGetFlagsPtr();
	// If LIN or FlexRay is connected, stop connecting to the network and display information
	if (NULL != pouFlag)
	{
        if (pouFlag->nGetFlagStatus(CONNECTED) || pouFlag->nGetFlagStatus(LIN_CONNECTED))
		{
			theApp.bWriteIntoTraceWnd(_("Please disconnect CAN / LIN networks and connect FlexRay"));
			return;
		}
	}

    if ( !m_bFlxDILChanging )
    {
        char chError[256];
        HRESULT hResult = S_OK;
        HRESULT hLastError;
        string strLastError;
        CFlags* pouFlag  = theApp.pouGetFlagsPtr();

        /* Toggle connect/disconnect flag */
        BOOL bConnected = pouFlag->nGetFlagStatus(FLEX_CONNECTED);
        bConnected = !bConnected;

        /* If connecton is required */
        if ( bConnected )
        {
            hResult = g_pouDIL_FLEXRAY_Interface->DILF_SetConfigData(m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[0]) ;

            if ( hResult != S_OK )
            {
                g_pouDIL_FLEXRAY_Interface->DILF_GetLastErrorString(strLastError, hLastError);
                sprintf_s(chError, "Error From Driver :: 0x%X", hLastError);
                theApp.bWriteIntoTraceWnd("FLEXRAY DIL: Setting configuration failed!");
                theApp.bWriteIntoTraceWnd(chError);
                return;
            }
            //CTimeManager::vInitAbsoluteTime();
            hResult = g_pouDIL_FLEXRAY_Interface->DILF_StartHardware();

            /* If connect fails, return from here */
            if ( hResult == S_FALSE )
            {
                return;
            }

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_START_READ, 0);
            // Clear message window on connect
            HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY);
            if(NULL != hWnd)
            {
                ::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, NULL, NULL);
                ::SendMessage(hWnd, WM_CLEAR_SORT_COLUMN, NULL, NULL);
            }
            m_objFlexTxHandler.vBusStatusChanged(FLEXRAY, BUS_CONNECTED);

            theApp.pouGetFlagsPtr()->vSetFlagStatus( FLEXRAYSENDMSG, FALSE );
        }
        /* If disconnecton is required */
        else
        {
            if(g_pouDIL_FLEXRAY_Interface != NULL)
            {
                hResult = g_pouDIL_FLEXRAY_Interface->DILF_StopHardware();
            }
            m_objFlexTxHandler.vStopTransmission(0);
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_STOP_READ, 0);
            m_objFlexTxHandler.vBusStatusChanged(FLEXRAY, BUS_DISCONNECTED);
        }
        //added to disable loading config while flexray transmission

        if(bConnected)
        {
            m_bCfgLoadMenuOption    = FALSE;
            m_bCfgNewMenuOption     = FALSE;
            m_unFlexRayNSTimer = SetTimer(TIMER_REFRESH_FLEXRAY_STATUS, STSBAR_REFRESH_TIME_PERIOD, NULL);
        }
        else
        {
            if(pouFlag->nGetFlagStatus(CONNECTED) == FALSE)
            {
                m_bCfgLoadMenuOption    = TRUE;
                m_bCfgNewMenuOption     = TRUE;
            }
        }
        pouFlag->vSetFlagStatus(FLEX_CONNECTED, bConnected);
        // Get reference to toolbar control
        CToolBarCtrl& omRefToolBarCtrl = m_wndToolbarFlexRay.GetToolBarCtrl();

        // Load the required bitmap to
        // show connect/disconnect state
        /* Modify connect icon accordingly in Main toolbar*/
        BYTE bytTbrItemIndex = 1;
        vModifyToolbarIcon( m_wndToolbarFlexRay, bytTbrItemIndex, bConnected, IDI_ICON_CAN_DISCONNECT, IDI_ICON_CAN_CONNECT );

        // Press / Unpress the button if Connected / Disconnected
        omRefToolBarCtrl.PressButton(ID_FLEXRAY_CONNECT, bConnected);
    }
}
void CMainFrame::OnUpdateFlexRayConnect(CCmdUI* pCmdUI)
{
    // Select the current menu text based on which one between Connect
    // or Disconnect is currently selected. The menu text should show
    // the next available state i.e., if currently "Connect" is displayed,
    // next available state is Disconnect and vice versa. Texts are taken from
    // the string table
    CFlags* pouFlag  = theApp.pouGetFlagsPtr();
    if(pouFlag != NULL)
    {
        BOOL bConnected  = pouFlag->nGetFlagStatus(FLEX_CONNECTED);
        UINT unConnected = bConnected ? IDS_DISCONNECT_FLEXRAY : IDS_CONNECT_FLEXRAY;

        // And initialise a CString with the string.
        CString omMenuItemText("");
        omMenuItemText.Format(unConnected);
        // And finally set the menu text.
        if(pCmdUI !=NULL)
        {
            pCmdUI->SetText(omMenuItemText);
        }
    }
    if ( m_shFLEXRAYDriverId != DAL_NONE )
    {
        pCmdUI->Enable();
    }
    else
    {
        pCmdUI->Enable(0);
    }
}
HRESULT CMainFrame::IntializeDIL(UINT unDefaultChannelCnt)
{
    HRESULT hResult = S_OK;
    m_bNoHardwareFound = true;
    if (g_pouDIL_CAN_Interface == NULL)
    {
        hResult = DIL_GetInterface(CAN, (void**)&g_pouDIL_CAN_Interface);
    }
    else
    {
        //g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();
        //DeselectJ1939Interfaces();
    }
    if (hResult == S_OK)
    {
        if ((hResult = g_pouDIL_CAN_Interface->DILC_SelectDriver(m_dwDriverId, m_hWnd, &m_ouWrapperLogger)) == S_OK)
        {
            g_pouDIL_CAN_Interface->DILC_PerformInitOperations();
            INT nCount = unDefaultChannelCnt;
            if ((hResult = g_pouDIL_CAN_Interface->DILC_ListHwInterfaces(m_asINTERFACE_HW, nCount)) == S_OK)
            {
                DeselectJ1939Interfaces();
                // On Deactivate, deactivating J1939 and hiding the message window
                GetIJ1939NodeSim()->NS_SetJ1939ActivationStatus(false);
                m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)J1939);
                HRESULT hResult = g_pouDIL_CAN_Interface->DILC_SelectHwInterfaces(m_asINTERFACE_HW, nCount);
                if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
                {
                    hResult = g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _T("CAN_MONITOR"));
                    if ((hResult == S_OK)|| (hResult == ERR_CLIENT_EXISTS))
                    {
                        m_bNoHardwareFound = false;
                        g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails, nCount);
                        bInitFrameProcCAN(); // Initialize logger module
                        vReRegisterAllCANNodes();//Reinitialize node simulation
                        if (sg_pouSWInterface[CAN] == NULL)//Signal watch
                        {
                            if (SW_GetInterface(CAN, (void**)&sg_pouSWInterface[CAN]) == S_OK)
                            {
                                sg_pouSWInterface[CAN]->SW_DoInitialization();
                            }
                        }
                        else
                        {
                            sg_pouSWInterface[CAN]->SW_DoInitialization();
                        }
                        m_podMsgWndThread->vUpdateClientID(CAN, g_dwClientID);
                        if (m_podMsgWndThread != NULL)//Msg window
                        {
                            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                                          eWINID_MSG_WND_GET_BUFFER_DETAILS, (LPARAM)m_anMsgBuffSize);
                        }
                        //initialize graph window read buffer
                        vInitializeGraphWndReadBuffer();
                        //re initialize statistics module
                        vInitializeBusStatCAN();
                        //venkat - Initialize Read Thread of TSExecutor.
                        m_objTSExecutorHandler.vDoInitailization(CAN);

                        /* Updates the number of channels selected */
                        m_nNumChannels = nCount;

                        /*Update hardware info in status bar*/
                        vUpdateHWStatusInfo();
                    }
                    else
                    {
                        theApp.bWriteIntoTraceWnd(_("registering client failed"));
                        m_dwDriverId = DRIVER_CAN_STUB;          //select simulation
                        IntializeDIL();
                    }
                }
                else
                {
                    theApp.bWriteIntoTraceWnd(_("Selecting hardware interface failed"));
                    m_dwDriverId = DRIVER_CAN_STUB;          //select simulation
                    IntializeDIL();
                }
            }
            else
            {
                if ( hResult == HW_INTERFACE_NO_SEL )
                {
                    theApp.bWriteIntoTraceWnd(_("hardware selection Cancelled. Retaining previous hardware settings.."));
                    /* Retain previous DIL selection */
                    m_dwDriverId = g_pouDIL_CAN_Interface->DILC_GetSelectedDriver();
                    m_bNoHardwareFound = false;
                }
                else
                {
                    theApp.bWriteIntoTraceWnd(_("Listing hardware interfaces failed"));
                    m_dwDriverId = DRIVER_CAN_STUB;          //select simulation
                    IntializeDIL();
                }
            }
        }
        else
        {
            if ( hResult!=DAL_ALREADY_SELECTED )
            {
                theApp.bWriteIntoTraceWnd(_("Driver selection failed"));
                m_dwDriverId = DRIVER_CAN_STUB;          //select simulation
                IntializeDIL();

            }
        }
        m_objTxHandler.vSetDILInterfacePtr((void*)g_pouDIL_CAN_Interface);
        m_objTxHandler.vSetClientID(g_dwClientID);
        vUpdateChannelInfo();
    }
    return hResult;
}

HRESULT CMainFrame::IntializeDILL(UINT unDefaultChannelCnt)
{
    if ( m_podMsgWndThread != NULL )
    {
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_SHOW, (LONG)LIN);
    }
    HRESULT hResult = S_OK;
    m_bNoHardwareFound = true;
    if (g_pouDIL_LIN_Interface == NULL)
    {
        hResult = DIL_GetInterface(LIN, (void**)&g_pouDIL_LIN_Interface);
    }
    else
    {
        //g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();
        //DeselectJ1939Interfaces();
    }
    if (hResult == S_OK)
    {
        if ((hResult = g_pouDIL_LIN_Interface->DILL_SelectDriver(m_shLINDriverId, m_hWnd, &m_ouWrapperLogger)) == S_OK)
        {
            g_pouDIL_LIN_Interface->DILL_PerformInitOperations();
            INT nCount = unDefaultChannelCnt;
            if ((hResult = g_pouDIL_LIN_Interface->DILL_ListHwInterfaces(m_asINTERFACE_HW, nCount)) == S_OK)
            {
                //DeselectJ1939Interfaces();
                HRESULT hResult = g_pouDIL_LIN_Interface->DILL_SelectHwInterfaces(m_asINTERFACE_HW, nCount);
                if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
                {
                    hResult = g_pouDIL_LIN_Interface->DILL_RegisterClient(TRUE, g_dwClientID, _T("LIN_MONITOR"));
                    if ((hResult == S_OK)|| (hResult == ERR_CLIENT_EXISTS))
                    {
                        m_bNoHardwareFound = false;
                        g_pouDIL_LIN_Interface->DILL_SetConfigData(m_ouClusterConfig[LIN]);
                        bInitFrameProcLIN(); // Initialize logger module
                        vReRegisterAllLINNodes();//Reinitialize node simulation
                        if (sg_pouSWInterface[LIN] == NULL)//Signal watch
                        {
                            if (SW_GetInterface(LIN, (void**)&sg_pouSWInterface[LIN]) == S_OK)
                            {
                                sg_pouSWInterface[LIN]->SW_DoInitialization();
                            }
                        }
                        else
                        {
                            sg_pouSWInterface[LIN]->SW_DoInitialization();
                        }
                        m_podMsgWndThread->vUpdateClientID(LIN, g_dwClientID);
                        if (m_podMsgWndThread != NULL)//Msg window
                        {
                            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                                          eWINID_MSG_WND_GET_BUFFER_DETAILS, (LPARAM)m_anMsgBuffSize);
                        }
                        //initialize graph window read buffer
                        /*vInitializeGraphWndReadBuffer();*/
                        //re initialize statistics module
                        /*vInitializeBusStatLIN();*/
                        //venkat - Initialize Read Thread of TSExecutor.
                        m_objTSExecutorHandler.vDoInitailization(LIN);

                        /* Updates the number of channels selected */
                        m_nNumChannelsLIN = nCount;

                        /*Update hardware info in status bar*/
                        vUpdateHWStatusInfo();

                        //Update TxWindow
                        eUSERSELCTION eUserSel = eCHANNELCOUNTUPDATED;
                        m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel,0);

                        // Enable the Deactivate menu as a valid driver is selected
                        m_pDILSubMenuLin->EnableMenuItem(IDC_SELECT_LIN_DRIVER + 0, true);

                        if(m_shLINDriverId == DRIVER_LIN_ETAS_BOA)
                        {
                            BOOL bIsLDFAssociated = FALSE;
                            for(INT nChnlIndex = 0; nChnlIndex < m_ouClusterConfig[LIN].m_nChannelsConfigured; nChnlIndex++)
                            {
                                if(m_ouClusterConfig[LIN].m_ouFlexChannelConfig[nChnlIndex].m_strDataBasePath != "")
                                {
                                    bIsLDFAssociated = TRUE;
                                }
                            }

                            if(bIsLDFAssociated == FALSE)
                            {
                                theApp.bWriteIntoTraceWnd(_("Information: To monitor messages associating LDF file is required for LIN ETAS BOA"));
                            }
                        }
                    }
                    else
                    {
                        theApp.bWriteIntoTraceWnd(_("registering client failed"));
                        m_shLINDriverId = DAL_NONE;          //select simulation
                        IntializeDILL();
                        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);
                    }
                }
                else
                {
                    theApp.bWriteIntoTraceWnd(_("Selecting hardware interface failed"));
                    m_shLINDriverId = DAL_NONE;          //select simulation
                    IntializeDILL();
                    m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);
                }
            }
            else
            {
                if ( hResult == HW_INTERFACE_NO_SEL )
                {
                    theApp.bWriteIntoTraceWnd(_("hardware selection Cancelled. Retaining previous hardware settings.."));
                    /* Retain previous DIL selection */
                    m_shLINDriverId = g_pouDIL_LIN_Interface->DILL_GetSelectedDriver();
                    m_bNoHardwareFound = false;
                    m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);
                }
                else
                {
                    theApp.bWriteIntoTraceWnd(_("Listing hardware interfaces failed"));
                    m_shLINDriverId = DAL_NONE;          //select simulation
                    IntializeDILL();
                    m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);
                }
            }
        }
        else
        {
            if (m_shLINDriverId <= 0) // Dummy Driver for Deactvation
            {
                if ( m_podMsgWndThread != NULL )
                {
                    m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);
                }
                // Disable the Deactivate menu as a valid driver is selected
                if ( m_pDILSubMenuLin != NULL )
                {
                    m_pDILSubMenuLin->EnableMenuItem(IDC_SELECT_LIN_DRIVER + 0, false);
                }
            }
        }
        m_objFlexTxHandler.vSetDILInterfacePtr(LIN, (void*)g_pouDIL_LIN_Interface);
        m_objFlexTxHandler.vSetClientID(LIN, g_dwClientID);
    }
    return hResult;
}

/*******************************************************************************
  Function Name  : vUpdateChannelInfo
  Input(s)       : -
  Output         : -
  Functionality  : Updates the channel count info to other windows.
  Member of      : CMainFrame
  Author(s)      : Arunkumar K
  Date Created   : 07-04-2011
  Modifications  :
*******************************************************************************/
void CMainFrame::vUpdateChannelInfo(void)
{
    LONG lParam = 0;
    UINT nHardware = defNO_OF_CHANNELS;
    if(g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
    {
        nHardware = (UINT)lParam;
        //update Bus Stats Dlg.
        if(m_bIsStatWndCreated)
        {
            m_podBusStatistics->vUpdateChannelCountInfo(nHardware);
        }
        //Update TxWindow
        eUSERSELCTION eUserSel = eCHANNELCOUNTUPDATED;
        m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel,0);

        //Update J1939 Tx Window
        if ( m_pouTxMsgWndJ1939 )
        {
            m_pouTxMsgWndJ1939->vUpdateChannelIDInfo();
        }
    }
}

/*******************************************************************************
  Function Name  : vUpdateHWStatusInfo
  Input(s)       : -
  Output         : -
  Functionality  : Updates the hardware selection and baud rate inforation in
                   status bar.
  Member of      : CMainFrame
  Author(s)      : Arunkumar K
  Date Created   : 04-04-2012
  Modifications  :
*******************************************************************************/
void CMainFrame::vUpdateHWStatusInfo(void)
{
    // Set number of channels, hardware selected and supported channels.
    CString omStrChannels;
    CString omStrChannelDriver;
    omStrChannels.Format(   _(defSTR_CHANNELS_SUPPORTED),
                            m_nNumChannels,
                            _(defSTR_CHANNEL_NAME) );

    for (int i = 0 ; i < m_nDILCount ; i++)
    {
        if ( m_dwDriverId == m_ouList[i].m_dwDriverID )
        {
            if ( m_asControllerDetails->m_omStrBaudrate== "" )
            {
                m_asControllerDetails->m_omStrBaudrate = "500";
            }
            // PTV
            // Added Shortcut key for the hardwared device, Removing '&'
            //venkat
            CString strDriverName = m_ouList[i].m_acName.c_str();
            strDriverName.Replace("&", "");

            float fBaudRate = atof(m_asControllerDetails->m_omStrBaudrate.c_str());
            if( fBaudRate >1000 )
            {
                fBaudRate /= 1000;
            }
            omStrChannelDriver.Format(  _T("%s - %s - %.3f Kbps (Allowed channels:%d)"),
                                        omStrChannels, strDriverName,
                                        fBaudRate, CHANNEL_ALLOWED);
            break;
        }
    }

    // Set Channel string
    m_wndStatusBar.SetPaneText(INDEX_CHANNELS, omStrChannelDriver );
}

LRESULT CMainFrame::OnProvideMsgDBPtr(WPARAM wParam, LPARAM /*lParam*/)
{
    CMsgSignal** pSignal = (CMsgSignal**)wParam;
    *pSignal = theApp.m_pouMsgSignal;
    return 0;
}

LRESULT CMainFrame::OnProvideMsgNameFromCode(WPARAM wParam, LPARAM lParam)
{
    CString* strMsgName = (CString*)lParam;
    if (NULL != theApp.m_pouMsgSignal)
    {
        theApp.m_pouMsgSignal->bMessageNameFromMsgCode((UINT)wParam, *strMsgName);
    }

    return 0;
}

LRESULT CMainFrame::OnProvidePGNNameFromCode(WPARAM wParam, LPARAM lParam)
{
    CString* strMsgName = (CString*)lParam;
    if (NULL != m_pouMsgSigJ1939)
    {
        m_pouMsgSigJ1939->bMessageNameFromMsgCode((UINT)wParam, *strMsgName);
    }

    return 0;
}

BOOL CMainFrame::bInitFrameProcCAN(void)
{
    BOOL bReturn = FALSE;
    if (FP_GetInterface(FRAMEPROC_CAN, (void**)&sg_pouFrameProcCAN) == S_OK)
    {
        CString omVerStr("");
        SCANPROC_PARAMS sCANProcParams;

        omVerStr.Format(IDS_VERSION);
        strncpy_s(sCANProcParams.m_acVersion, 64, omVerStr, omVerStr.GetLength());
        sCANProcParams.dwClientID = g_dwClientID;
        sCANProcParams.m_pouCANBuffer = &g_ouCANBufFSE;
        sCANProcParams.m_pILog = &m_ouWrapperLogger;

        if (sg_pouFrameProcCAN->FPC_DoInitialisation(&sCANProcParams) == S_OK)
        {
            bReturn = TRUE;
        }
    }
    return bReturn;
}

BOOL CMainFrame::bInitFrameProcLIN(void)
{
    BOOL bReturn = FALSE;
    if (FP_GetInterface(FRAMEPROC_LIN, (void**)&sg_pouFrameProcLIN) == S_OK)
    {
        CString omVerStr("");
        SLINPROC_PARAMS sLINProcParams;

        omVerStr.Format(IDS_VERSION);
        strncpy_s(sLINProcParams.m_acVersion, 64, omVerStr, omVerStr.GetLength());
        sLINProcParams.dwClientID = g_dwClientID;
        sLINProcParams.m_pouLINBuffer = &g_ouLINBufFSE;
        sLINProcParams.m_pILog = &m_ouWrapperLogger;

        if (sg_pouFrameProcLIN->FPL_DoInitialisation(&sLINProcParams) == S_OK)
        {
            bReturn = TRUE;
        }
    }
    return bReturn;
}

DWORD CMainFrame::dwGetMonitorNodeClientID()
{
    return g_dwClientID;
}

int CMainFrame::COM_ConnectTool()
{
    OnFileConnect();
    return 0;
}

BOOL CMainFrame::COM_SaveConfigFile()
{
    OnSaveConfigFile();
    return TRUE;
}

BOOL CMainFrame::COM_SaveConfigFileAs(CString omCfgFilePath)
{
    // Save the file
    CConfigData::ouGetConfigDetailsObject().vCloseConfigFile();
    vSetFileStorageInfo(omCfgFilePath);
    SaveConfiguration();
    return TRUE;
}

BOOL CMainFrame::COM_bDLLBuild(CStringArray* omStrBuildFiles)
{
    BOOL bReturn            = FALSE;
    //BOOL bSuccess           = FALSE;
    int nFailure            = 0;
    CString omStrFileName   = STR_EMPTY;
    //int nNodeCount          = 0;

    //CSimSysNodeInfo::PSSIMSYSINFO pTempSimsys = theApp.psReturnSimsysInfoPtr();
    omStrBuildFiles->RemoveAll();

    //while(pTempSimsys != NULL)
    /*{
        nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
        CSimSysNodeInfo::PSNODELIST pTempNode = pTempSimsys->m_psNodesList;
        while(pTempNode != NULL)
        {
            if(pTempNode->m_sNodeInfo.m_omStrFileName != "")
            {
                bSuccess = CExecuteManager::ouGetExecuteManager().
                    bExecuteDllBuild(&pTempNode->m_sNodeInfo);
                if(!bSuccess)
                {
                    nFailure++;
                    omStrBuildFiles->Add(pTempNode->m_sNodeInfo.m_omStrFileName);
                }
            }
            pTempNode = pTempNode->m_psNextNode;
        }
        pTempSimsys = pTempSimsys->m_psSimsysNext;
    }*/
    if(nFailure == 0)
    {
        bReturn= TRUE;
    }
    return bReturn;
}


BOOL CMainFrame::COM_bDLLBuildLoad(CStringArray* omStrBuildFiles)
{
    BOOL bReturn    = FALSE;
    //BOOL bSuccess   = FALSE;
    int nFailure    = 0;
    //int nNodeCount  = 0;
    /*CSimSysNodeInfo::PSSIMSYSINFO pTempSimsys = theApp.psReturnSimsysInfoPtr();*/
    //CExecuteManager::ouGetExecuteManager().vStopTimer();

    omStrBuildFiles->RemoveAll();

    //while(pTempSimsys != NULL)
    /*{
        nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;
        CSimSysNodeInfo::PSNODELIST pTempNode = pTempSimsys->m_psNodesList;

        while(pTempNode != NULL)
        {
            if(pTempNode->m_sNodeInfo.m_omStrFileName != "")
            {
                bSuccess = CExecuteManager::ouGetExecuteManager().
                    bExecuteDllBuildLoad(&pTempNode->m_sNodeInfo);
                if(!bSuccess)
                {
                    nFailure++;
                    omStrBuildFiles->Add(pTempNode->m_sNodeInfo.m_omStrFileName);
                }
            }
            pTempNode = pTempNode->m_psNextNode;
        }
        pTempSimsys = pTempSimsys->m_psSimsysNext;
    }*/
    if(nFailure == 0)
    {
        bReturn= TRUE;
    }

    //CExecuteManager::ouGetExecuteManager().vStartTimer();
    return bReturn;
}

BOOL CMainFrame::COM_bDllLoad(CStringArray* /*omStrBuildFiles*/)
{
    OnDllLoad();
    return TRUE;
}

BOOL CMainFrame::COM_bDllUnload(CStringArray* /*omStrBuildFiles*/)
{
    OnDllUnload();
    return TRUE;
}

void CMainFrame::COM_SendMessage()
{
    OnSendMessage();
}

void CMainFrame::COM_EnableAllHandlers(BOOL /*bEnable*/)
{
    OnExecuteAllHandlers();
}

/*  function to get _Unions.h header filename from Database name*/
CString CMainFrame::omStrGetUnionFilePath(CString omStrTemp)
{
    CString omStrHeaderFileName = "";
    if(omStrTemp.IsEmpty() == FALSE)
    {
        omStrHeaderFileName = omStrTemp.Left(omStrTemp.ReverseFind('.'));
        omStrHeaderFileName += defHEADER_FILE_NAME;
    }
    return omStrHeaderFileName;
}
static int App_SendMsg(void* pMsg, HMODULE /*hModule*/)
{
    STCAN_MSG* psMsg = (STCAN_MSG*)pMsg;
    return g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID,*psMsg);
}

void CMainFrame::vInitCFileFunctPtrs()
{
    //vSetAppService((CBaseAppServices *) &sg_ouAppServiceObj);

    m_sExFuncPtr[CAN].m_hWmdMDIParentFrame = this->GetSafeHwnd();
    //m_sExFuncPtr.m_pouTraceWnd = m_podUIThread;
    m_sExFuncPtr[CAN].m_omAPIList.RemoveAll();
    for (int i = 0; i < TOTAL_API_COUNT; i++)
    {
        m_sExFuncPtr[CAN].m_omAPIList.Add(sg_omAPIFuncList[i]);
    }

    m_sExFuncPtr[CAN].m_omAPINames.RemoveAll();
    for (int i = 0; i < TOTAL_API_COUNT; i++)
    {
        m_sExFuncPtr[CAN].m_omAPINames.Add(sg_omAPIFuncNames[i]);
    }

    m_sExFuncPtr[CAN].m_omErrorHandlerList.RemoveAll();
    for (int i = 0; i < TOTAL_ERROR_COUNT; i++)
    {
        m_sExFuncPtr[CAN].m_omErrorHandlerList.Add(sg_omMcNetErrorHandlerList[i]);
    }

    m_sExFuncPtr[CAN].m_omDefinedMsgHeaders.RemoveAll();
    CStringArray omDatabaseNames;
    theApp.m_pouMsgSignal->vGetDataBaseNames(&omDatabaseNames);
    for (int i = 0; i < omDatabaseNames.GetSize(); i++)
    {
        CString omHeaderPath = omStrGetUnionFilePath(omDatabaseNames.GetAt(i));
        m_sExFuncPtr[CAN].m_omDefinedMsgHeaders.Add(omHeaderPath);
    }

    vUpdateMsgNameCodeList(theApp.m_pouMsgSignal, m_sExFuncPtr[CAN].m_odMsgNameMsgCodeListDB);
    // Send KeyPanel list pointer
    //m_sExFuncPtr.m_podNodeToDllMap = &g_odNodeToDllMap;
    //m_sExFuncPtr.m_podKeyPanelEntryList = &g_odKeyPanelEntryList;
    m_sExFuncPtr[CAN].m_omAppDirectory = m_omAppDirectory;
    m_sExFuncPtr[CAN].m_omObjWrapperName = WRAPPER_NAME;
    m_sExFuncPtr[CAN].m_omStructFile = STRUCT_FILE;
    m_sExFuncPtr[CAN].m_omStructName = MSG_STRUCT_NAME;
    m_sExFuncPtr[CAN].Send_Msg = App_SendMsg;
    m_sExFuncPtr[CAN].DisConnectTool = NULL;
    m_sExFuncPtr[CAN].EnDisableLog = NULL;
    m_sExFuncPtr[CAN].WriteToLog = NULL;
    m_sExFuncPtr[CAN].RestController = NULL;
    m_sExFuncPtr[CAN].m_pouITraceWndPtr = &sg_ouAppServiceObj;
    //m_sExFuncPtr.m_pNetWorkMcNet = &m_odNetwork;

}
void CMainFrame::NS_InitJ1939SpecInfo()
{
    m_sExFuncPtr[J1939].m_hWmdMDIParentFrame = this->GetSafeHwnd();
    //m_sExFuncPtr.m_pouTraceWnd = m_podUIThread;
    m_sExFuncPtr[J1939].m_omAPIList.RemoveAll();
    for (int i = 0; i < TOTAL_API_COUNT_J1939; i++)
    {
        m_sExFuncPtr[J1939].m_omAPIList.Add(sg_omAPIFuncListJ1939[i]);
    }

    m_sExFuncPtr[J1939].m_omAPINames.RemoveAll();
    for (int i = 0; i < TOTAL_API_COUNT_J1939; i++)
    {
        m_sExFuncPtr[J1939].m_omAPINames.Add(sg_omAPIFuncNamesJ1939[i]);
    }

    m_sExFuncPtr[J1939].m_omErrorHandlerList.RemoveAll();

    m_sExFuncPtr[J1939].m_omDefinedMsgHeaders.RemoveAll();
    CStringArray omDatabaseNames;
    if( NULL != m_pouMsgSigJ1939)
    {
        m_pouMsgSigJ1939->vGetDataBaseNames(&omDatabaseNames);
    }
    for (int i = 0; i < omDatabaseNames.GetSize(); i++)
    {
        CString omHeaderPath = omStrGetUnionFilePath(omDatabaseNames.GetAt(i));
        m_sExFuncPtr[J1939].m_omDefinedMsgHeaders.Add(omHeaderPath);
    }

    vUpdateMsgNameCodeList(m_pouMsgSigJ1939, m_sExFuncPtr[J1939].m_odMsgNameMsgCodeListDB);
    // Send KeyPanel list pointer
    //m_sExFuncPtr.m_podNodeToDllMap = &g_odNodeToDllMap;
    //m_sExFuncPtr.m_podKeyPanelEntryList = &g_odKeyPanelEntryList;
    m_sExFuncPtr[J1939].m_omAppDirectory = m_omAppDirectory;
    m_sExFuncPtr[J1939].m_omObjWrapperName = WRAPPER_NAME_J1939;
    m_sExFuncPtr[J1939].m_omStructFile = STRUCT_FILE_J1939;
    m_sExFuncPtr[J1939].m_omStructName = MSG_STRUCT_NAME_J1939;
    m_sExFuncPtr[J1939].Send_Msg = App_SendMsg;
    m_sExFuncPtr[J1939].DisConnectTool = NULL;
    m_sExFuncPtr[J1939].EnDisableLog = NULL;
    m_sExFuncPtr[J1939].WriteToLog = NULL;
    m_sExFuncPtr[J1939].RestController = NULL;
    m_sExFuncPtr[J1939].m_pouITraceWndPtr = &sg_ouAppServiceObj;
    //m_sExFuncPtr.m_pNetWorkMcNet = &m_odNetwork;

}
void CMainFrame::vUpdateMsgNameCodeList(CMsgSignal* pMsgSig, CMsgNameMsgCodeListDataBase& odMsgNameMsgCodeListDB)
{
    if(pMsgSig != NULL)
    {
        pMsgSig->bFillDbStructure(odMsgNameMsgCodeListDB);
    }
    //odMsgNameMsgCodeList.RemoveAll();
    //int nDefMsgCount = 0;
    //if (pMsgSig != NULL)
    //{
    //    nDefMsgCount = pMsgSig->unGetNumerOfMessages();
    //}
    //if (nDefMsgCount > 0)
    //{
    //    CStringList omDefMsgs;
    //    pMsgSig->omStrListGetMessageNames(omDefMsgs);
    //    POSITION pos = omDefMsgs.GetHeadPosition();
    //    // Insert every message name into the message list box
    //    CString omStrMsgName = _T("");
    //    while ( pos != NULL )
    //    {
    //        SMSG_NAME_CODE sMsgNameCode;
    //        omStrMsgName = omDefMsgs.GetNext(pos);
    //        sMsgNameCode.m_omMsgName = omStrMsgName;
    //        sMESSAGE* psDBMsg =  pMsgSig->psGetMessagePointer(omStrMsgName);
    //        if (psDBMsg != NULL)
    //        {
    //            sMsgNameCode.m_dwMsgCode = psDBMsg->m_unMessageCode;
    //            sMsgNameCode.m_unMsgLen  = psDBMsg->m_unMessageLength;
    //            sSIGNALS* psSignal = psDBMsg->m_psSignals;
    //            while (psSignal != NULL)
    //            {
    //                sMsgNameCode.m_omSignalNames.AddTail(psSignal->m_omStrSignalName);
    //                psSignal = psSignal->m_psNextSignalList;
    //            }
    //        }
    //        odMsgNameMsgCodeList.AddTail(sMsgNameCode);
    //    }
    //}
}

void CMainFrame::ToggleView(CToolBar& omToolbar)
{
    BOOL bToShow = !bIsToolbarVisible(omToolbar);

    ShowControlBar(&omToolbar, bToShow, FALSE);
    Invalidate();
}

BOOL CMainFrame::bIsToolbarVisible(CToolBar& omToolbar)
{
    return ((omToolbar.GetStyle() & WS_VISIBLE) != 0);
}

void CMainFrame::OnDisplayMain()
{
    ToggleView(m_wndToolBar);
}

void CMainFrame::OnUpdateDisplayMain(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(bIsToolbarVisible(m_wndToolBar));
}

void CMainFrame::OnDisplayMsgWnd()
{
    ToggleView(m_wndToolbarMsgWnd);
}

void CMainFrame::OnUpdateDisplayMsgWnd(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(bIsToolbarVisible(m_wndToolbarMsgWnd));
}

void CMainFrame::OnDisplayNodeSim()
{
    ToggleView(m_wndToolbarNodeSimul);
}

void CMainFrame::OnUpdateDisplayNodeSim(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(bIsToolbarVisible(m_wndToolbarNodeSimul));
}

void CMainFrame::OnDisplayConfig()
{
    ToggleView(m_wndToolbarConfig);
}

void CMainFrame::OnUpdateDisplayConfig(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(bIsToolbarVisible(m_wndToolbarConfig));
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
    if(nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
    {
        CMDIChildWnd* pChild = this->MDIGetActive();
        if(pChild)
        {
            CRuntimeClass* pRunTimeClass = pChild->GetRuntimeClass();
            if(strcmp(pRunTimeClass->m_lpszClassName,_T("CMsgFrmtWnd"))==0)
            {
                if(m_podMsgWndThread!=NULL)
                {
                    HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
                    ::SendMessage(hWnd, WM_SETFOCUS_MSGWND_LIST, (WPARAM)true, NULL);
                }
            }
        }
    }
}

INT CMainFrame::nLoadConfigFile(CString omConfigFileName)
{
    m_bIsXmlConfig = TRUE;
    int nRetValue = S_OK;
    if (!omConfigFileName.IsEmpty())
    {
        CConfigData::ouGetConfigDetailsObject().vCloseConfigFile();
        vSetFileStorageInfo(omConfigFileName);
        //MVN
        string strCfxFile = (LPCSTR)omConfigFileName;
        nRetValue = nLoadXMLConfiguration(strCfxFile);
        if (nRetValue == S_OK )         //1. Try to Load as a XML file
        {
            m_bIsXmlConfig = TRUE;
            ApplyLogFilter();
            ApplyMessageFilterButton();
            ApplyReplayFilter();
            ApplyMessagewindowOverwrite();
            nRetValue = S_OK;
            vCreateMRU_Menus();
            m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)J1939);
            m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)FLEXRAY);
            m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);



        }
        //~MVN
        else                            //2. Load as a Binary File
        {
            CConfigData::ouGetConfigDetailsObject().vCloseConfigFile();
            vSetFileStorageInfo(omConfigFileName);
            CConfigAdapter ouConfigAdapter;
            INT nError = ouConfigAdapter.nLoadConfigFile(omConfigFileName);
            if (nError == defCONFIG_FILE_SUCCESS)
            {
                vSetCurrProjInfo(BUSMASTER_APPN_VERSION_6_0);
                for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
                {
                    BYTE* pbyConfigData = NULL;
                    INT nSize = 0;
                    if (ouConfigAdapter.bGetConfigData(pbyConfigData, nSize, eSecId))
                    {
                        CConfigData::ouGetConfigDetailsObject().bSetData(pbyConfigData, nSize, SectionName[eSecId]);
                        delete[] pbyConfigData;
                    }
                    eSecId = static_cast<eSECTION_ID>( eSecId + 1);
                }
            }
            else
            {
                CConfigData::ouGetConfigDetailsObject().vReadConfigFile();
            }
            nRetValue = LoadConfiguration();

            ApplyLogFilter();
            ApplyMessageFilterButton();
            ApplyReplayFilter();
        }
    }
    else            //If Default Configuration
    {
        for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
        {
            //TODO:: this has to bo moved to xml config
            /* call with NULL to load the default configuration*/
            vSetCurrentSessionData(eSecId, NULL, 0);
            eSecId = static_cast<eSECTION_ID>(eSecId + 1);
        }
        bCreateTraceWindow();

    }
    return nRetValue;
}


INT CMainFrame::LoadConfiguration(void)
{
    INT nReturn = defCONFIG_FILE_SUCCESS;
    CConfigData::ouGetConfigDetailsObject().bGetCurrProjInfo(&m_sProjData);
    for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
    {
        BYTE* pbyConfigData = NULL;
        INT nSize = 0;
        CConfigData::ouGetConfigDetailsObject().bGetData((void*&)pbyConfigData, nSize, SectionName[eSecId]);

        /* call even if pbyConfigData is NULL to load the default configuration*/
        vSetCurrentSessionData(eSecId, pbyConfigData, nSize);
        //All done now release the memory
        try
        {
            delete[] pbyConfigData;
        }
        catch(...)
        {
        }

        pbyConfigData = NULL;

        eSecId = static_cast<eSECTION_ID>(eSecId + 1);
    }

    bCreateTraceWindow();
    return nReturn;
}

//MVN
int CMainFrame::nLoadXMLConfiguration(string& m_strCfxFile)
{
    int nRetValue = S_FALSE;

    m_xmlConfigFiledoc = xmlReadFile(m_strCfxFile.c_str(), "UTF-8", 0);
    //m_xmlConfigFiledoc = xmlParseFile(m_strCfxFile.c_str());

    CString strEncoding = "UTF-8";

    //m_xmlConfigFiledoc->encoding = (BAD_CAST strEncoding.GetBuffer(strEncoding.GetLength()));

    if ( NULL != m_xmlConfigFiledoc )           //1. Try to Load as a XML file
    {
        //AfxMessageBox("Loaded");
        nRetValue = nLoadXMLConfiguration();
        xmlFreeDoc(m_xmlConfigFiledoc);
        m_xmlConfigFiledoc = NULL;
    }
    return nRetValue;
}

INT CMainFrame::vSaveXMLConfiguration(const char* filename)
{

    xmlDocPtr pXMLDocPtr = NULL;       /* document pointer */
    xmlNodePtr pRtNodePtr = NULL;
    xmlDtdPtr dtd = NULL;       /* DTD pointer */

    xmlKeepBlanksDefault(0);

    // Create the document with version 1.0
    pXMLDocPtr = xmlNewDoc(BAD_CAST "1.0");

    CString strEncoding = "UTF-8";

    // Creating the Root node "BUSMAST
    pRtNodePtr = xmlNewNode(NULL, BAD_CAST DEF_BUSMASTER_CONFIGURATION);

    xmlDocSetRootElement(pXMLDocPtr, pRtNodePtr);

    // GLOBAL Configuration
    xmlNodePtr pGblCnfgPtr = xmlNewNode(NULL, BAD_CAST DEF_GLOBAL_CONFIGURATION);
    xmlAddChild(pRtNodePtr, pGblCnfgPtr);

    // MODULE Configuration
    xmlNodePtr pModConfigNdPtr = xmlNewNode(NULL, BAD_CAST DEF_MODULE_CONFIGURATION);
    xmlAddChild(pRtNodePtr, pModConfigNdPtr);

    int nRetValue = S_OK;

    for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
    {
        BYTE* pbyConfigData = NULL;
        UINT nSize = 0;

        if(eSecId == MAINFRAME_SECTION_ID)
        {
            vGetCurrentSessionData(eSecId, pbyConfigData, nSize, pGblCnfgPtr);
        }
        else
        {
            vGetCurrentSessionData(eSecId, pbyConfigData, nSize, pModConfigNdPtr);
        }

        if (pbyConfigData != NULL)
        {
            // CConfigData::ouGetConfigDetailsObject().bSetData((void*)pbyConfigData, nSize, SectionName[eSecId]);
            //All done now release the memory
            delete[] pbyConfigData;
            pbyConfigData = NULL;
        }

        eSecId = static_cast<eSECTION_ID>(eSecId + 1);
    }

    xmlIndentTreeOutput = 1;
    xmlThrDefIndentTreeOutput(TRUE);

    if(pXMLDocPtr != NULL)
    {
        xmlSaveFormatFileEnc(filename, pXMLDocPtr, "UTF-8", 1);

        //xmlSaveToFilename(filename, "UTF-8", NULL);

        //xmlSaveFileEnc(filename, pXMLDocPtr, "UTF-8");

        xmlFreeDoc(pXMLDocPtr);
    }

    xmlCleanupParser();

    return nRetValue;
}


void CMainFrame::vGetCurrentSessionData(eSECTION_ID eSecId, BYTE*& pbyConfigData, UINT& nSize, xmlNodePtr pNodePtr)
{
    switch (eSecId)
    {
        case MAINFRAME_SECTION_ID:
        {
            // Saving Application Version in to cfx file
            CString strVersion;

            // Application version
            strVersion.Format(_("%d.%d.%d"), VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

            xmlNodePtr pAppVersion = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_APPLICATION_VERSION
                                                 , BAD_CAST strVersion.GetBuffer(strVersion.GetLength()));
            xmlAddChild(pNodePtr, pAppVersion);

            BYTE byVersion = 0x2;

            char acName[MAX_PATH] = {_T('\0')};
            strcpy_s(acName, MAX_PATH, m_omMRU_C_Filename.GetBuffer(MAX_PATH));


            theApp.pouGetFlagsPtr()->vGetToolbarButtonStatus(&m_sToolBarInfo);

            // Writing IsMsgFilterEnabled in to xml
            CString strIsMsgFilterEnabled = "";
            if(m_sToolBarInfo.m_byMsgFilter == TRUE)
            {
                strIsMsgFilterEnabled = _("TRUE");
            }
            else if(m_sToolBarInfo.m_byMsgFilter == FALSE)
            {
                strIsMsgFilterEnabled = _("FALSE");
            }

            xmlNodePtr pMsgFilterEnbld = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_IsMsgFilterEnabled
                                         , BAD_CAST strIsMsgFilterEnabled.GetBuffer(strIsMsgFilterEnabled.GetLength()));
            xmlAddChild(pNodePtr, pMsgFilterEnbld);

            CString strIsReplayFilterEnabled = "";
            if(m_sToolBarInfo.m_byReplayFilter == TRUE)
            {
                strIsReplayFilterEnabled = _("TRUE");
            }
            else if(m_sToolBarInfo.m_byReplayFilter == FALSE)
            {
                strIsReplayFilterEnabled = _("FALSE");
            }
            xmlNodePtr pReplayFilterEnbld = xmlNewChild(pNodePtr, NULL, BAD_CAST "IsReplayFilterEnabled"
                                            , BAD_CAST strIsReplayFilterEnabled.GetBuffer(strIsReplayFilterEnabled.GetLength()));
            xmlAddChild(pNodePtr, pReplayFilterEnbld);
            // Writing IsLogFilterEnabled in to xml
            CString strIsLogFilterEnabled = "";
            if(m_sToolBarInfo.m_byLogFilter == TRUE)
            {
                strIsLogFilterEnabled = _("TRUE");
            }
            else if(m_sToolBarInfo.m_byLogFilter == FALSE)
            {
                strIsLogFilterEnabled = _("FALSE");
            }

            xmlNodePtr pLogFilterEnbld = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_IsLogFilterEnabled
                                         , BAD_CAST strIsLogFilterEnabled.GetBuffer(strIsLogFilterEnabled.GetLength()));
            xmlAddChild(pNodePtr, pLogFilterEnbld);

            // Writing IsLoggingEnabled in to xml
            CString strIsLoggingEnabled = "";
            if(m_sToolBarInfo.m_byLogging == TRUE)
            {
                strIsLoggingEnabled = _("TRUE");
            }
            else if(m_sToolBarInfo.m_byLogging == FALSE)
            {
                strIsLoggingEnabled = _("FALSE");
            }

            xmlNodePtr pLoggingEnbld = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_IsLoggingEnabled
                                                   , BAD_CAST strIsLoggingEnabled.GetBuffer(strIsLoggingEnabled.GetLength()));
            xmlAddChild(pNodePtr, pLoggingEnbld);

            // Writing IsMsgIntepretationEnabled in to xml
            CString strIsMsgIntepretationEnabled = "";
            if(m_sToolBarInfo.m_byMsgInterpret == TRUE)
            {
                strIsMsgIntepretationEnabled = _("TRUE");
            }
            else if(m_sToolBarInfo.m_byMsgInterpret == FALSE)
            {
                strIsMsgIntepretationEnabled = _("FALSE");
            }

            xmlNodePtr pMsgIntrEnbld = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_IsMsgIntepretationEnabled
                                                   , BAD_CAST strIsMsgIntepretationEnabled.GetBuffer(strIsMsgIntepretationEnabled.GetLength()));
            xmlAddChild(pNodePtr, pMsgIntrEnbld);

            // Writing IsOverWriteEnabled in to xml
            CString strIsOverWriteEnabled = "";
            if(m_sToolBarInfo.m_byOverwrite == TRUE)
            {
                strIsOverWriteEnabled = _("TRUE");
            }
            else if(m_sToolBarInfo.m_byOverwrite == FALSE)
            {
                strIsOverWriteEnabled = _("FALSE");
            }

            xmlNodePtr pOverwriteEnbld = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_IsOverWriteEnabled
                                         , BAD_CAST strIsOverWriteEnabled.GetBuffer(strIsOverWriteEnabled.GetLength()));
            xmlAddChild(pNodePtr, pOverwriteEnbld);

            // Writing DisplayTimeMode in to xml
            CString strDisplayTimeMode = "";
            if(m_sToolBarInfo.m_byDisplayTimeMode == eSYSTEM_MODE)
            {
                strDisplayTimeMode = _("SYSTEM");
            }
            else if(m_sToolBarInfo.m_byDisplayTimeMode == eABSOLUTE_MODE)
            {
                strDisplayTimeMode = _("ABSOLUTE");
            }
            else if(m_sToolBarInfo.m_byDisplayTimeMode == eRELATIVE_MODE)
            {
                strDisplayTimeMode = _("RELATIVE");
            }

            xmlNodePtr pDisplayTimeMode = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_DisplayTimeMode
                                          , BAD_CAST strDisplayTimeMode.GetBuffer(strDisplayTimeMode.GetLength()));
            xmlAddChild(pNodePtr, pDisplayTimeMode);

            // Writing DisplayNumericMode in to xml
            CString strDisplayNumericMode = "";
            if(m_sToolBarInfo.m_byDisplayHexON == TRUE)
            {
                strDisplayNumericMode = _("FALSE");
            }
            else if(m_sToolBarInfo.m_byDisplayHexON == FALSE)
            {
                strDisplayNumericMode = _("TRUE");
            }

            xmlNodePtr pDisplayNumericMode = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_DisplayNumericMode
                                             , BAD_CAST strDisplayNumericMode.GetBuffer(strDisplayNumericMode.GetLength()));
            xmlAddChild(pNodePtr, pDisplayNumericMode);

            // Writing LogOnConnect in to xml
            CString strLogOnConnectCAN = "";
            if(m_abLogOnConnect[CAN] == TRUE)
            {
                strLogOnConnectCAN = _("TRUE");
            }
            else if(m_abLogOnConnect[CAN] == FALSE)
            {
                strLogOnConnectCAN = _("FALSE");
            }

            xmlNodePtr pLogOnConnectCAN = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_LogOnConnect_For_CAN
                                          , BAD_CAST strLogOnConnectCAN.GetBuffer(strLogOnConnectCAN.GetLength()));
            xmlAddChild(pNodePtr, pLogOnConnectCAN);

            // Writing LogOnConnect in to xml
            CString strLogOnConnectJ1939 = "";
            if(m_abLogOnConnect[J1939] == TRUE)
            {
                strLogOnConnectJ1939 = _("TRUE");
            }
            else if(m_abLogOnConnect[J1939] == FALSE)
            {
                strLogOnConnectJ1939 = _("FALSE");
            }

            xmlNodePtr pLogOnConnectJ1939 = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_LogOnConnect_For_J1939
                                            , BAD_CAST strLogOnConnectJ1939.GetBuffer(strLogOnConnectJ1939.GetLength()));
            xmlAddChild(pNodePtr, pLogOnConnectJ1939);


            CString strLogOnConnectLIN = "";
            if(m_abLogOnConnect[LIN] == TRUE)
            {
                strLogOnConnectLIN = _("TRUE");
            }
            else if(m_abLogOnConnect[LIN] == FALSE)
            {
                strLogOnConnectLIN = _("FALSE");
            }

            xmlNodePtr pLogOnConnectLIN = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_LogOnConnect_For_LIN
                                          , BAD_CAST strLogOnConnectLIN.GetBuffer(strLogOnConnectLIN.GetLength()));
            xmlAddChild(pNodePtr, pLogOnConnectLIN);





            //COPY_DATA(pbyTemp, &m_sToolBarInfo, sizeof(STOOLBARINFO));

            if (m_podUIThread != NULL)
            {
                m_podUIThread->vUpdateWndCo_Ords(m_sNotificWndPlacement, FALSE);
            }

            CString strVisibility = xmlUtils::nSetWindowVisibility( m_sNotificWndPlacement.showCmd );
            CString strWndPlacement = xmlUtils::nSetWindowVisibility( m_sNotificWndPlacement.flags );
            CString strTop = "", strBottom = "", strLeft = "", strRight = "";

            strTop.Format("%d", m_sNotificWndPlacement.rcNormalPosition.top);
            strBottom.Format("%d", m_sNotificWndPlacement.rcNormalPosition.bottom);
            strLeft.Format("%d", m_sNotificWndPlacement.rcNormalPosition.left);
            strRight.Format("%d", m_sNotificWndPlacement.rcNormalPosition.right);

            // Writing Window Position in to xml
            xmlNodePtr pWndPosition = xmlNewNode(NULL, BAD_CAST DEF_WINDOW_POSITION);
            xmlAddChild(pNodePtr, pWndPosition);

            // Writing Visibility in to xml
            xmlNodePtr pVisibility = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_VISIBILITY, BAD_CAST strVisibility.GetBuffer(strVisibility.GetLength()));
            xmlAddChild(pWndPosition, pVisibility);

            // Writing window placement in to xml
            xmlNodePtr pWndPlcmnt = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_WINDOW_PLACEMENT, BAD_CAST strWndPlacement.GetBuffer(strWndPlacement.GetLength()));
            xmlAddChild(pWndPosition, pWndPlcmnt);

            // Writing Top bottom left and right position in to xml
            xmlNodePtr pTopPtr = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_TOP, BAD_CAST strTop.GetBuffer(strTop.GetLength()));
            xmlAddChild(pWndPosition, pTopPtr);

            xmlNodePtr pLeftPtr = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_Left, BAD_CAST strLeft.GetBuffer(strLeft.GetLength()));
            xmlAddChild(pWndPosition, pLeftPtr);

            xmlNodePtr pBottomPtr = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_Bottom, BAD_CAST strBottom.GetBuffer(strBottom.GetLength()));
            xmlAddChild(pWndPosition, pBottomPtr);

            xmlNodePtr pRightPtr = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_Right, BAD_CAST strRight.GetBuffer(strRight.GetLength()));
            xmlAddChild(pWndPosition, pRightPtr);
        }
        break;
        case LOG_SECTION_J1939_ID:
        {
            /*if (GetIJ1939Logger() != NULL)
            {
                GetIJ1939Logger()->FPJ1_GetConfigData(&pbyConfigData, nSize);
            }*/
            if (GetIJ1939Logger() != NULL)
            {
                xmlNodePtr pJ1939LogPtr = xmlNewNode(NULL, BAD_CAST DEF_J1939_LOG);
                xmlAddChild(pNodePtr, pJ1939LogPtr);

                GetIJ1939Logger()->FPJ1_GetConfigData(pJ1939LogPtr);
            }
        }
        break;
        case LOG_SECTION_ID:
        {
            /* if (sg_pouFrameProcCAN != NULL)
             {
                 sg_pouFrameProcCAN->FPC_GetConfigData(&pbyConfigData, nSize);
             }*/

            if (sg_pouFrameProcCAN != NULL)
            {
                xmlNodePtr pCanLogPtr = xmlNewNode(NULL, BAD_CAST DEF_CAN_LOG);
                xmlAddChild(pNodePtr, pCanLogPtr);

                sg_pouFrameProcCAN->FPC_GetConfigData(pCanLogPtr);
            }
        }
        break;

        case LOG_SECTION_LIN_ID:
        {


            if (sg_pouFrameProcLIN != NULL)
            {
                xmlNodePtr pLinLogPtr = xmlNewNode(NULL, BAD_CAST DEF_LIN_LOG);
                xmlAddChild(pNodePtr, pLinLogPtr);

                sg_pouFrameProcLIN->FPL_GetConfigData(pLinLogPtr);
            }
        }
        break;
        case SIMSYS_SECTION_ID:
        {
            xmlNodePtr pCanSimSys = NULL;
            GetICANNodeSim()->NS_GetSimSysConfigData(pCanSimSys);
            xmlAddChild(pNodePtr, pCanSimSys);
        }
        break;
        case SIMSYS_SECTION_LIN_ID:
        {
            xmlNodePtr pLinSimSys = NULL;
            GetILINNodeSim()->NS_GetSimSysConfigData(pLinSimSys);
            xmlAddChild(pNodePtr, pLinSimSys);
        }
        break;
        case SIMSYS_SECTION_J1939_ID:
        {
            xmlNodePtr pJ1939SimSys = NULL;
            GetIJ1939NodeSim()->NS_GetSimSysConfigData(pJ1939SimSys);
            xmlAddChild(pNodePtr, pJ1939SimSys);
        }
        break;
        case REPLAY_SECTION_ID:
        {

            xmlNodePtr pCANReplay = xmlNewNode(NULL, BAD_CAST DEF_CAN_REPLAY);
            xmlAddChild(pNodePtr, pCANReplay);
            vREP_GetReplayConfigData(pCANReplay);
        }
        break;
        case MSGWND_SECTION_ID:
        {
            SMESSAGE_ATTRIB sMsgAttrib;
            sMsgAttrib.m_psMsgAttribDetails = NULL;
            sMsgAttrib.m_usMsgCount = 0;
            CMessageAttrib::ouGetHandle(CAN).vGetMessageAttribData(sMsgAttrib);
            UINT nCount = sMsgAttrib.m_usMsgCount;
            SFILTERAPPLIED_CAN sMsgWndFilter;

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_GET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);

            xmlNodePtr pCANMsgWindow = xmlNewNode(NULL, BAD_CAST DEF_MSG_WINDOW);
            xmlAddChild(pNodePtr, pCANMsgWindow);

            INT nAppndBuffrSize = m_anMsgBuffSize[defAPPEND_DATA_INDEX];
            INT nOverwritBffrSize = m_anMsgBuffSize[defOVERWRITE_DATE_INDEX];
            INT nDisplayUpdateTime = m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX];

            CString strAppndBffrSize = "";
            strAppndBffrSize.Format("%d", nAppndBuffrSize);

            CString strOverwritBffrSize = "";
            strOverwritBffrSize.Format("%d", nOverwritBffrSize);

            CString strDisplayUpdateTime = "";
            strDisplayUpdateTime.Format("%d", nDisplayUpdateTime);

            xmlNodePtr pAppndBffrPtr = xmlNewChild(pCANMsgWindow, NULL, BAD_CAST DEF_APPEND_BUFFER_SIZE, BAD_CAST strAppndBffrSize.GetBuffer(strAppndBffrSize.GetLength()));
            xmlAddChild(pCANMsgWindow, pAppndBffrPtr);

            xmlNodePtr pOvrwriteBffrPtr = xmlNewChild(pCANMsgWindow, NULL, BAD_CAST DEF_OVERWRITE_BUFFER_SIZE, BAD_CAST strOverwritBffrSize.GetBuffer(strOverwritBffrSize.GetLength()));
            xmlAddChild(pCANMsgWindow, pOvrwriteBffrPtr);

            xmlNodePtr pDispBffrPtr = xmlNewChild(pCANMsgWindow, NULL, BAD_CAST DEF_DISPLAY_UPDATE_TIME, BAD_CAST strDisplayUpdateTime.GetBuffer(strDisplayUpdateTime.GetLength()));
            xmlAddChild(pCANMsgWindow, pDispBffrPtr);

            const char* omcVarChar ;

            for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
            {
                if ( sMsgAttrib.m_psMsgAttribDetails[i].sColor != RGB(0,0,0))
                {
                    xmlNodePtr pCANMsgAttribute = xmlNewNode(NULL, BAD_CAST DEF_MSG_ATTRIBUTE);
                    xmlAddChild(pCANMsgWindow, pCANMsgAttribute);

                    omcVarChar = sMsgAttrib.m_psMsgAttribDetails[i].omStrMsgname;
                    xmlNodePtr pMsgName = xmlNewChild(pCANMsgAttribute, NULL, BAD_CAST DEF_NAME,BAD_CAST omcVarChar);
                    xmlAddChild(pCANMsgAttribute, pMsgName);

                    CString     csMsgID;
                    csMsgID.Format("%u",sMsgAttrib.m_psMsgAttribDetails[i].unMsgID);
                    omcVarChar = csMsgID;
                    xmlNodePtr pMsgID = xmlNewChild(pCANMsgAttribute, NULL, BAD_CAST DEF_MSG_ID,BAD_CAST omcVarChar);
                    xmlAddChild(pCANMsgAttribute, pMsgID);

                    CString csColor;
                    int nR, nB, nG;
                    nR = GetRValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nG = GetGValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nB = GetBValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    //csColor.Format("%x%x%x",nR,nG,nB);

                    char ch[10];

                    sprintf(ch, "%02x%02x%02x", nR,nG,nB);


                    omcVarChar = csColor;
                    xmlNodePtr pMsgColor = xmlNewChild(pCANMsgAttribute, NULL, BAD_CAST DEF_MSG_COLOR,BAD_CAST ch);
                    xmlAddChild(pCANMsgAttribute, pMsgColor);
                }
            }

            CString     csFilter;
            for(int iCnt=0; iCnt < sMsgWndFilter.m_ushTotal; iCnt++)
            {
                csFilter.Format("%s",((sMsgWndFilter.m_psFilters)+iCnt)->m_sFilterName.m_acFilterName);
                omcVarChar = csFilter;
                xmlNodePtr pFilter = xmlNewChild(pCANMsgWindow, NULL, BAD_CAST DEF_FILTER,BAD_CAST omcVarChar);
                xmlAddChild(pCANMsgWindow, pFilter);
            }

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                          WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_GET_CONFIG_DATA,
                          (LPARAM)pCANMsgWindow);

            DELETE_ARRAY(sMsgAttrib.m_psMsgAttribDetails);
        }
        break;
        case MSGWND_SECTION_LIN_ID:
        {
            SMESSAGE_ATTRIB sMsgAttrib;
            sMsgAttrib.m_psMsgAttribDetails = NULL;
            sMsgAttrib.m_usMsgCount = 0;
            CMessageAttrib::ouGetHandle(LIN).vGetMessageAttribData(sMsgAttrib);
            UINT nCount = sMsgAttrib.m_usMsgCount;
            SFILTERAPPLIED_LIN sMsgWndFilter;

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_GET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);

            xmlNodePtr pLINMsgWindow = xmlNewNode(NULL, BAD_CAST DEF_LIN_MESS_WINDOW);
            xmlAddChild(pNodePtr, pLINMsgWindow);

            INT nAppndBuffrSize = m_anMsgBuffSize[defAPPEND_DATA_INDEX];
            INT nOverwritBffrSize = m_anMsgBuffSize[defOVERWRITE_DATE_INDEX];
            INT nDisplayUpdateTime = m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX];

            CString strAppndBffrSize = "";
            strAppndBffrSize.Format("%d", nAppndBuffrSize);

            CString strOverwritBffrSize = "";
            strOverwritBffrSize.Format("%d", nOverwritBffrSize);

            CString strDisplayUpdateTime = "";
            strDisplayUpdateTime.Format("%d", nDisplayUpdateTime);

            xmlNodePtr pAppndBffrPtr = xmlNewChild(pLINMsgWindow, NULL, BAD_CAST DEF_APPEND_BUFFER_SIZE, BAD_CAST strAppndBffrSize.GetBuffer(strAppndBffrSize.GetLength()));
            xmlAddChild(pLINMsgWindow, pAppndBffrPtr);

            xmlNodePtr pOvrwriteBffrPtr = xmlNewChild(pLINMsgWindow, NULL, BAD_CAST DEF_OVERWRITE_BUFFER_SIZE, BAD_CAST strOverwritBffrSize.GetBuffer(strOverwritBffrSize.GetLength()));
            xmlAddChild(pLINMsgWindow, pOvrwriteBffrPtr);

            xmlNodePtr pDispBffrPtr = xmlNewChild(pLINMsgWindow, NULL, BAD_CAST DEF_DISPLAY_UPDATE_TIME, BAD_CAST strDisplayUpdateTime.GetBuffer(strDisplayUpdateTime.GetLength()));
            xmlAddChild(pLINMsgWindow, pDispBffrPtr);

            const char* omcVarChar ;

            for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
            {
                if ( sMsgAttrib.m_psMsgAttribDetails[i].sColor != RGB(0,0,0))
                {
                    xmlNodePtr pLINMsgAttribute = xmlNewNode(NULL, BAD_CAST DEF_MSG_ATTRIBUTE);
                    xmlAddChild(pLINMsgWindow, pLINMsgAttribute);

                    omcVarChar = sMsgAttrib.m_psMsgAttribDetails[i].omStrMsgname;
                    xmlNodePtr pMsgName = xmlNewChild(pLINMsgAttribute, NULL, BAD_CAST DEF_NAME,BAD_CAST omcVarChar);
                    xmlAddChild(pLINMsgAttribute, pMsgName);

                    CString     csMsgID;
                    csMsgID.Format("%u",sMsgAttrib.m_psMsgAttribDetails[i].unMsgID);
                    omcVarChar = csMsgID;
                    xmlNodePtr pMsgID = xmlNewChild(pLINMsgAttribute, NULL, BAD_CAST DEF_MSG_ID,BAD_CAST omcVarChar);
                    xmlAddChild(pLINMsgAttribute, pMsgID);

                    CString csColor;
                    int nR, nB, nG;
                    nR = GetRValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nG = GetGValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nB = GetBValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    //csColor.Format("%x%x%x",nR,nG,nB);

                    char ch[10];

                    sprintf(ch, "%02x%02x%02x", nR,nG,nB);


                    omcVarChar = csColor;
                    xmlNodePtr pMsgColor = xmlNewChild(pLINMsgAttribute, NULL, BAD_CAST DEF_MSG_COLOR,BAD_CAST ch);
                    xmlAddChild(pLINMsgAttribute, pMsgColor);
                }
            }

            CString     csFilter;
            for(int iCnt=0; iCnt < sMsgWndFilter.m_ushTotal; iCnt++)
            {
                csFilter.Format("%s",((sMsgWndFilter.m_psFilters)+iCnt)->m_sFilterName.m_acFilterName);
                omcVarChar = csFilter;
                xmlNodePtr pFilter = xmlNewChild(pLINMsgWindow, NULL, BAD_CAST DEF_FILTER,BAD_CAST omcVarChar);
                xmlAddChild(pLINMsgWindow, pFilter);
            }

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                          WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_GET_CONFIG_DATA,
                          (LPARAM)pLINMsgWindow);

            DELETE_ARRAY(sMsgAttrib.m_psMsgAttribDetails);
        }
        break;
        case MSGWND_SECTION_J1939_ID:
        {
            SMESSAGE_ATTRIB sMsgAttrib;
            sMsgAttrib.m_psMsgAttribDetails = NULL;
            sMsgAttrib.m_usMsgCount = 0;
            CMessageAttrib::ouGetHandle(J1939).vGetMessageAttribData(sMsgAttrib);
            UINT nCount = sMsgAttrib.m_usMsgCount;
            UINT unMsgFrmtWndCfgSize = 0;
            ASSERT(m_podMsgWndThread != NULL);

            if(m_podMsgWndThread->hGetHandleMsgWnd(J1939))
            {
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                              WM_NOTIFICATION_FROM_OTHER, eWINID_MSG_WND_GET_CONFIG_SIZE, (LPARAM)&unMsgFrmtWndCfgSize);
            }
            //CALC SIZE ENDS
            xmlNodePtr pJ1939MsgWindow = xmlNewNode(NULL, BAD_CAST DEF_J1939_MESS_WINDOW);
            xmlAddChild(pNodePtr, pJ1939MsgWindow);

            const char* omcVarChar ;

            for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
            {
                if( sMsgAttrib.m_psMsgAttribDetails[i].sColor != RGB( 0, 0, 0 ) )
                {
                    xmlNodePtr pJ1939MsgAttribute = xmlNewNode(NULL, BAD_CAST DEF_MSG_ATTRIBUTE);
                    xmlAddChild(pJ1939MsgWindow, pJ1939MsgAttribute);

                    omcVarChar = sMsgAttrib.m_psMsgAttribDetails[i].omStrMsgname.GetBuffer(MAX_PATH);
                    xmlNodePtr pMsgName = xmlNewChild(pJ1939MsgAttribute, NULL, BAD_CAST DEF_NAME,BAD_CAST omcVarChar);
                    xmlAddChild(pJ1939MsgAttribute, pMsgName);

                    CString     csMsgID;
                    csMsgID.Format("%u",sMsgAttrib.m_psMsgAttribDetails[i].unMsgID);
                    omcVarChar = csMsgID.GetBuffer(MAX_PATH);
                    xmlNodePtr pMsgID = xmlNewChild(pJ1939MsgAttribute, NULL, BAD_CAST DEF_MSG_ID,BAD_CAST omcVarChar);
                    xmlAddChild(pJ1939MsgAttribute, pMsgID);

                    CString     csColor;
                    int nR, nB, nG;
                    nR = GetRValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nG = GetGValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nB = GetBValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    //csColor.Format("%2x%2x%2x",nR,nG,nB);

                    char ch[7] = {0};

                    sprintf(ch, "%02x%02x%02x", nR,nG,nB);

                    omcVarChar = csColor.GetBuffer(MAX_PATH);
                    xmlNodePtr pMsgColor = xmlNewChild(pJ1939MsgAttribute, NULL, BAD_CAST DEF_MSG_COLOR,BAD_CAST ch);
                    xmlAddChild(pJ1939MsgAttribute, pMsgColor);
                }
            }


            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                          WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_GET_CONFIG_DATA,
                          (LPARAM)pJ1939MsgWindow);
        }
        break;
        case SIGWATCH_SECTION_J1939_ID:
        {
            CMainEntryList odMainEntryList;
            vPopulateMainEntryList(&odMainEntryList, m_psSignalWatchList[J1939], m_pouMsgSigJ1939);

            ////CALCULATE SIZE REQUIRED
            //nSize += sizeof(BYTE); //Configuration version

            POSITION pos = odMainEntryList.GetHeadPosition();

            pos = odMainEntryList.GetHeadPosition();
            UINT nMainCount = odMainEntryList.GetCount();

            xmlNodePtr pNodeSigWtchPtr = xmlNewNode(NULL, BAD_CAST DEF_J1939_SIGNAL_WATCH);
            xmlAddChild(pNodePtr, pNodeSigWtchPtr);

            //COPY_DATA(pbyTemp, &nMainCount, sizeof(UINT));

            while (pos)
            {
                // Adding Message


                SMAINENTRY& sMainEntry = odMainEntryList.GetNext(pos);
                //COPY_DATA(pbyTemp, &(sMainEntry.m_unMainEntryID), sizeof(UINT));

                //COPY_DATA(pbyTemp, acName, (sizeof(char) * MAX_PATH));
                UINT unSelCount = sMainEntry.m_odSelEntryList.GetCount();
                //MVN only if any signal is selected
                if( unSelCount > 0 )
                {
                    char acName[MAX_PATH] = {_T('\0')};
                    strcpy_s(acName, MAX_PATH, sMainEntry.m_omMainEntryName.GetBuffer(MAX_CHAR));

                    xmlNodePtr pMsgTagPtr = xmlNewNode(NULL, BAD_CAST DEF_MESSAGE);
                    xmlAddChild(pNodeSigWtchPtr, pMsgTagPtr);

                    CString strMessageId = "";
                    strMessageId.Format("%d", sMainEntry.m_unMainEntryID);
                    /* Generating Message names */
                    xmlNodePtr pMsgPtr = xmlNewChild(pMsgTagPtr, NULL, BAD_CAST DEF_MSGID
                                                     , BAD_CAST strMessageId.GetBufferSetLength(strMessageId.GetLength()));

                    xmlAddChild(pMsgTagPtr, pMsgPtr);

                    //COPY_DATA(pbyTemp, &unSelCount, sizeof(UINT));
                    POSITION SelPos = sMainEntry.m_odSelEntryList.GetHeadPosition();
                    while (SelPos != NULL)
                    {
                        SSUBENTRY sSubEntry = sMainEntry.m_odSelEntryList.GetNext(SelPos);
                        //COPY_DATA(pbyTemp, &(sSubEntry.m_unSubEntryID), sizeof(UINT));
                        strcpy_s(acName, MAX_PATH, sSubEntry.m_omSubEntryName.GetBuffer(MAX_CHAR));
                        //COPY_DATA(pbyTemp, acName, (sizeof(char) * MAX_PATH));

                        /* Generating signals in the Message */
                        xmlNodePtr pSignalPtr = xmlNewChild(pMsgTagPtr, NULL, BAD_CAST DEF_SIGNAL
                                                            , BAD_CAST sSubEntry.m_omSubEntryName.GetBufferSetLength(sSubEntry.m_omSubEntryName.GetLength()));
                        xmlAddChild(pMsgTagPtr, pSignalPtr);
                    }
                }
            }

            if (sg_pouSWInterface[J1939] != NULL)
            {
                UINT nSWSize = 0;
                sg_pouSWInterface[J1939]->SW_GetConfigData(pNodeSigWtchPtr);
                //pbyTemp += nSWSize;
            }
        }
        break;
        case SIGWATCH_SECTION_ID:
        {
            CMainEntryList odMainEntryList;
            vPopulateMainEntryList(&odMainEntryList, m_psSignalWatchList[CAN], theApp.m_pouMsgSignal);

            //CALCULATE SIZE REQUIRED
            //nSize += sizeof(BYTE); //Configuration version

            POSITION pos = odMainEntryList.GetHeadPosition();
            //nSize += sizeof (UINT); //To store the count of main entry
            //while (pos)
            //{
            //    nSize += sizeof (UINT);
            //    nSize += (sizeof (char) * MAX_PATH);
            //    SMAINENTRY& sMainEntry = odMainEntryList.GetNext(pos);
            //    nSize += (sizeof (char) * MAX_PATH);//To store number of selected entries

            //    for (UINT nSelIndex = 0; nSelIndex < (UINT)sMainEntry.m_odSelEntryList.GetCount(); nSelIndex++)
            //    {
            //        nSize += sizeof (UINT);
            //        nSize += (sizeof (char) * MAX_PATH);
            //    }
            //}
            //BYTE* pbySWWndPlacement = NULL;
            //UINT unSWSize = 0;
            //nSize += sg_pouSWInterface[CAN]->SW_GetConfigSize();
            ////ALLOCATE MEMORY
            //pbyConfigData = new BYTE[nSize];
            //BYTE* pbyTemp = pbyConfigData;

            ////UPDATE THE DATA NOW
            //BYTE byVersion = 0x1;
            //COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

            pos = odMainEntryList.GetHeadPosition();
            UINT nMainCount = odMainEntryList.GetCount();

            //COPY_DATA(pbyTemp, &nMainCount, sizeof(UINT));

            xmlNodePtr pNodeSigWtchPtr = xmlNewNode(NULL, BAD_CAST DEF_SIGNAL_WATCH);
            xmlAddChild(pNodePtr, pNodeSigWtchPtr);

            while (pos)
            {
                char acName[MAX_PATH] = {_T('\0')};

                SMAINENTRY& sMainEntry = odMainEntryList.GetNext(pos);

                UINT unSelCount = sMainEntry.m_odSelEntryList.GetCount();
                //MVN only if any signal is selected
                if( unSelCount > 0)
                {
                    // Adding Message
                    xmlNodePtr pMsgTagPtr = xmlNewNode(NULL, BAD_CAST DEF_MESSAGE);
                    xmlAddChild(pNodeSigWtchPtr, pMsgTagPtr);

                    CString strMessageId = "";
                    strMessageId.Format("%d", sMainEntry.m_unMainEntryID);
                    /* Generating Message names */
                    xmlNodePtr pMsgPtr = xmlNewChild(pMsgTagPtr, NULL, BAD_CAST DEF_MSGID
                                                     , BAD_CAST strMessageId.GetBufferSetLength(strMessageId.GetLength()));

                    xmlAddChild(pMsgTagPtr, pMsgPtr);

                    POSITION SelPos = sMainEntry.m_odSelEntryList.GetHeadPosition();

                    while (SelPos != NULL)
                    {
                        SSUBENTRY& sSubEntry = sMainEntry.m_odSelEntryList.GetNext(SelPos);

                        /* Generating signals in the Message */
                        xmlNodePtr pSignalPtr = xmlNewChild(pMsgTagPtr, NULL, BAD_CAST DEF_SIGNAL
                                                            , BAD_CAST sSubEntry.m_omSubEntryName.GetBufferSetLength(sSubEntry.m_omSubEntryName.GetLength()));
                        xmlAddChild(pMsgTagPtr, pSignalPtr);
                    }
                }
            }

            if (sg_pouSWInterface[CAN] != NULL)
            {
                UINT nSWSize = 0;
                sg_pouSWInterface[CAN]->SW_GetConfigData(pNodeSigWtchPtr);
                //sg_pouSWInterface[CAN]->SW_GetConfigData((void*)pbyTemp);
                //pbyTemp += nSWSize;
            }
        }
        break;
        case DIL_SECTION_ID:
        {
            //nSize = sizeof(BYTE);//configuration version
            //nSize += sizeof(DWORD);// Driver Id
            //nSize += sizeof(BYTE); // Controller mode

            ////nSize += sizeof(SCONTROLLER_DETAILS) * CHANNEL_ALLOWED;
            //for(int i = 0; i < CHANNEL_ALLOWED; i++)
            //{
            //    int nTemp = 0;
            //    m_asControllerDetails[i].GetControllerConfigSize(nTemp);
            //    nSize += nTemp;
            //}


            //pbyConfigData = new BYTE[nSize];

            //if (pbyConfigData != NULL)
            {
                //BYTE* pbyTemp = pbyConfigData;

                //BYTE byVersion = DIL_CFX_CURRENT_VERSION;
                //COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));
                //COPY_DATA(pbyTemp, &m_dwDriverId, sizeof(DWORD));
                //COPY_DATA(pbyTemp, &m_byControllerMode, sizeof(BYTE));
                //COPY_DATA(pbyTemp, m_asControllerDetails, (sizeof(SCONTROLLER_DETAILS) * CHANNEL_ALLOWED));

                if(pNodePtr != NULL)
                {
                    xmlNodePtr pNodeDBFilePtr = xmlNewNode(NULL, BAD_CAST DEF_CAN_DIL_SECTION);
                    xmlAddChild(pNodePtr, pNodeDBFilePtr);

                    // Above lines have to be changed.
                    CString omstrDriverName = vGetControllerName(m_dwDriverId);

                    if(pNodeDBFilePtr != NULL)
                    {
                        // Get the selected driver Name
                        //CString omstrDriverName = m_ouList[nDriverId].m_acName.c_str();

                        //omstrDriverName.Replace("&", "");
                        const char* strDriverName = omstrDriverName;

                        const char* strControllerMode = "";

                        /* Saving controller mode in English to config */
                        if(m_byControllerMode == defCONTROLLER_ACTIVE)
                        {
                            strControllerMode = defSTR_ACTIVE_STATE;
                        }
                        else if(m_byControllerMode == defCONTROLLER_PASSIVE)
                        {
                            strControllerMode = defSTR_PASSIVE_STATE;
                        }
                        else if(m_byControllerMode == defCONTROLLER_BUSOFF)
                        {
                            strControllerMode = defSTR_BUSOFF_STATE;
                        }
                        else
                        {
                            strControllerMode = defSTR_UNKNOWN_STATE;
                        }

                        xmlNodePtr pDNPtr = xmlNewChild(pNodeDBFilePtr, NULL, BAD_CAST DEF_DRIVER_NAME, BAD_CAST strDriverName);
                        xmlAddChild(pNodeDBFilePtr, pDNPtr);

                        xmlNodePtr pCntrlModePtr = xmlNewChild(pNodeDBFilePtr, NULL, BAD_CAST DEF_CONTROLLER_MODE, BAD_CAST strControllerMode);
                        xmlAddChild(pNodeDBFilePtr, pCntrlModePtr);

                        xmlNodePtr pCntrlSettngsPtr = xmlNewNode(NULL, BAD_CAST DEF_CONTROLLER_SETTINGS);
                        xmlAddChild(pNodeDBFilePtr, pCntrlSettngsPtr);

                        INT nNumOfChannelsSel = 0;

                        if(pCntrlSettngsPtr != NULL)
                        {
                            INT nCount = 0;
                            HRESULT hResult;

                            LONG lParam = 0;

                            if(g_pouDIL_CAN_Interface != NULL)
                            {
                                if(g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
                                {
                                    nNumOfChannelsSel = (UINT)lParam;
                                }
                            }

                            for(int nChnlIndex = 0; nChnlIndex < nNumOfChannelsSel; nChnlIndex++)
                            {
                                //xmlNodePtr pChnlPtr = xmlNewChild(pNodeDBFilePtr, NULL, BAD_CAST DEF_CHANNEL, BAD_CAST "");
                                xmlNodePtr pChnlPtr = xmlNewNode(NULL, (xmlChar*)DEF_CHANNEL);
                                xmlAddChild(pCntrlSettngsPtr, pChnlPtr);

                                m_asControllerDetails[nChnlIndex].SaveConfigDataToXML(pChnlPtr);
                            }
                            //xmlNewChild(pCntrlSettngsPtr, NULL, BAD_CAST DEF_DRIVER_NAME, BAD_CAST strDriverName);
                        }

                        //xmlTextWriterSetIndent(pNodeDBFilePtr, 1);
                    }
                }
                /*for(int i = 0; i < CHANNEL_ALLOWED; i++)
                {
                    INT nsize = 0;
                    m_asControllerDetails[i].GetControllerConfigData(pbyTemp, nsize);
                }*/
            }
        }
        break;
        case GRAPH_SECTION_ID:
        {
            // Generating Signal Graph in to xml
            xmlNodePtr pCanSigGrphPtr = xmlNewNode(NULL, BAD_CAST DEF_CAN_SIGNAL_GRAPH);
            xmlAddChild(pNodePtr, pCanSigGrphPtr);

            BYTE byVersion = 0x2;

            // CAN PROTOCOL
            int nBUSID=0;
            m_odGraphList[nBUSID].pbyGetConfigData(pCanSigGrphPtr, byVersion);

            m_objSigGrphHandler.GetWindowSplitterPos((SHORT)nBUSID, m_sGraphWndPlacement[nBUSID],
                    m_sGraphSplitterPos[nBUSID]);

            CString strVisibility = xmlUtils::nSetWindowVisibility( m_sGraphWndPlacement[nBUSID].showCmd );
            CString strWndPlacement = xmlUtils::nSetWindowVisibility( m_sGraphWndPlacement[nBUSID].flags );
            CString strTop = "", strBottom = "", strLeft = "", strRight = "";

            strTop.Format("%d", m_sGraphWndPlacement[nBUSID].rcNormalPosition.top);
            strBottom.Format("%d", m_sGraphWndPlacement[nBUSID].rcNormalPosition.bottom);
            strLeft.Format("%d", m_sGraphWndPlacement[nBUSID].rcNormalPosition.left);
            strRight.Format("%d", m_sGraphWndPlacement[nBUSID].rcNormalPosition.right);

            // Writing Window Position in to xml
            xmlNodePtr pWndPosition = xmlNewNode(NULL, BAD_CAST DEF_WINDOW_POSITION);
            xmlAddChild(pCanSigGrphPtr, pWndPosition);

            // Writing Visibility in to xml
            xmlNodePtr pVisibility = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_VISIBILITY, BAD_CAST strVisibility.GetBuffer(strVisibility.GetLength()));
            xmlAddChild(pWndPosition, pVisibility);

            // Writing window placement in to xml
            xmlNodePtr pWndPlcmnt = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_WINDOW_POSITION, BAD_CAST strWndPlacement.GetBuffer(strWndPlacement.GetLength()));
            xmlAddChild(pWndPosition, pWndPlcmnt);

            // Writing Top bottom left and right position in to xml
            xmlNodePtr pTopPtr = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_TOP, BAD_CAST strTop.GetBuffer(strTop.GetLength()));
            xmlAddChild(pWndPosition, pTopPtr);

            xmlNodePtr pLeftPtr = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_Left, BAD_CAST strLeft.GetBuffer(strLeft.GetLength()));
            xmlAddChild(pWndPosition, pLeftPtr);

            xmlNodePtr pBottomPtr = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_Bottom, BAD_CAST strBottom.GetBuffer(strBottom.GetLength()));
            xmlAddChild(pWndPosition, pBottomPtr);

            xmlNodePtr pRightPtr = xmlNewChild(pWndPosition, NULL, BAD_CAST DEF_Right, BAD_CAST strRight.GetBuffer(strRight.GetLength()));
            xmlAddChild(pWndPosition, pRightPtr);

            // Writing splitter details in to xml
            // Writing Column 0 details
            INT nColumnzeroCXIdeal = m_sGraphSplitterPos[nBUSID].m_nRootSplitterData[0][0];
            INT nColumnzeroCXMin = m_sGraphSplitterPos[nBUSID].m_nRootSplitterData[0][1];

            CString strColZeroCxIdeal = "", strColZeroCxMin = "";
            strColZeroCxIdeal.Format("%d", nColumnzeroCXIdeal);
            strColZeroCxMin.Format("%d", nColumnzeroCXMin);

            // Writing Column 1 details
            INT nColumnOneCXIdeal = m_sGraphSplitterPos[nBUSID].m_nRootSplitterData[1][0];
            INT nColumnOneCXMin = m_sGraphSplitterPos[nBUSID].m_nRootSplitterData[1][1];

            CString strColOneCxIdeal = "", strColOneCxMin = "";
            strColOneCxIdeal.Format("%d", nColumnOneCXIdeal);
            strColOneCxMin.Format("%d", nColumnOneCXMin);

            // Writing Row 0 details
            INT nRowzeroCyIdeal = m_sGraphSplitterPos[nBUSID].m_nRightViewSplitterData[0][0];
            INT nRowzeroCyMin = m_sGraphSplitterPos[nBUSID].m_nRightViewSplitterData[0][1];

            CString strRowZeroCyIdeal = "", strRowZeroCyMin = "";
            strRowZeroCyIdeal.Format("%d", nRowzeroCyIdeal);
            strRowZeroCyMin.Format("%d", nRowzeroCyMin);

            // Writing Row 1 details
            INT nRowOneCyIdeal = m_sGraphSplitterPos[nBUSID].m_nRightViewSplitterData[1][0];
            INT nRowOneCyMin = m_sGraphSplitterPos[nBUSID].m_nRightViewSplitterData[1][1];

            CString strRowOneCyIdeal = "", strRowOneCyMin = "";
            strRowOneCyIdeal.Format("%d", nRowOneCyIdeal);
            strRowOneCyMin.Format("%d", nRowOneCyMin);

            // Column zero
            xmlNodePtr pColZero = xmlNewNode(NULL, BAD_CAST DEF_Splitter_Window_Col_0);
            xmlAddChild(pCanSigGrphPtr, pColZero);

            xmlNodePtr pColZeroCxIdeal = xmlNewChild(pColZero, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST strColZeroCxIdeal.GetBuffer(strColZeroCxIdeal.GetLength()));
            xmlAddChild(pColZero, pColZeroCxIdeal);

            xmlNodePtr pColZeroCxMin = xmlNewChild(pColZero, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST strColZeroCxMin.GetBuffer(strColZeroCxMin.GetLength()));
            xmlAddChild(pColZero, pColZeroCxMin);

            // Column One
            xmlNodePtr pColOne = xmlNewNode(NULL, BAD_CAST DEF_Splitter_Window_Col_1);
            xmlAddChild(pCanSigGrphPtr, pColOne);

            xmlNodePtr pColOneCxIdeal = xmlNewChild(pColOne, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST strColOneCxIdeal.GetBuffer(strColOneCxIdeal.GetLength()));
            xmlAddChild(pColOne, pColOneCxIdeal);

            xmlNodePtr pColOneCxMin = xmlNewChild(pColOne, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST strColOneCxMin.GetBuffer(strColOneCxMin.GetLength()));
            xmlAddChild(pColOne, pColOneCxMin);

            // Row zero
            xmlNodePtr pRowZero = xmlNewNode(NULL, BAD_CAST DEF_Splitter_Window_Row_0);
            xmlAddChild(pCanSigGrphPtr, pRowZero);

            xmlNodePtr pRowZeroCxIdeal = xmlNewChild(pRowZero, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST strRowZeroCyIdeal.GetBuffer(strRowZeroCyIdeal.GetLength()));
            xmlAddChild(pRowZero, pRowZeroCxIdeal);

            xmlNodePtr pRowZeroCxMin = xmlNewChild(pRowZero, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST strRowZeroCyMin.GetBuffer(strRowZeroCyMin.GetLength()));
            xmlAddChild(pRowZero, pRowZeroCxMin);

            // Row One
            xmlNodePtr pRowOne = xmlNewNode(NULL, BAD_CAST DEF_Splitter_Window_Row_1);
            xmlAddChild(pCanSigGrphPtr, pRowOne);

            xmlNodePtr pRowOneCxIdeal = xmlNewChild(pRowOne, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST strRowOneCyIdeal.GetBuffer(strRowOneCyIdeal.GetLength()));
            xmlAddChild(pRowOne, pRowOneCxIdeal);

            xmlNodePtr pRowOneCxMin = xmlNewChild(pRowOne, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST strRowOneCyMin.GetBuffer(strRowOneCyMin.GetLength()));
            xmlAddChild(pRowOne, pRowOneCxMin);
        }
        break;
        case TXWND_SECTION_ID:
        {
            //changes done for XML configuration
            xmlNodePtr pCANTxPtr = xmlNewNode(NULL, BAD_CAST DEF_CAN_TX_WINDOW);
            xmlAddChild(pNodePtr, pCANTxPtr);
            m_objTxHandler.vGetTxWndConfigData(pCANTxPtr);
        }
        break;
        case FILTER_SECTION_ID:
        {
            nSize = m_sFilterAppliedCAN.unGetSize();

            if(pNodePtr != NULL)
            {
                m_sFilterAppliedCAN.pbGetConfigData(pNodePtr);
            }

            //        pbyConfigData = new BYTE[nSize];

            //        if (pbyConfigData != NULL)
            //        {
            //            BYTE* pbyTemp = pbyConfigData;
            //            //bool bResult = false;
            //            pbyTemp = m_sFilterAppliedCAN.pbGetConfigData(pbyTemp);

            ////m_sFilterAppliedCAN.pbGetConfigDataForXML();
            //        }

            //pbyConfigData = new BYTE[nSize];

            /*if (pbyConfigData != NULL)
            {*/
            //BYTE* pbyTemp = pbyConfigData;
            //bool bResult = false;



            //m_sFilterAppliedCAN.pbGetConfigDataForXML();
            //}
        }
        break;
        case DATABASE_SECTION_J1939_ID:
        {
            nSize += sizeof(BYTE);//configuration version

            CStringArray omDbNames;
            if (m_pouMsgSigJ1939 != NULL)
            {
                string omStrBasePath;
                CString omConfigFileName;
                vGetLoadedCfgFileName(omConfigFileName);
                CUtilFunctions::nGetBaseFolder(omConfigFileName.GetBuffer(MAX_PATH), omStrBasePath);
                m_pouMsgSigJ1939->vGetRelativeDataBaseNames(omStrBasePath, &omDbNames);
            }

            if(pNodePtr != NULL)
            {
                xmlNodePtr pNodeDBFilePtr = xmlNewNode(NULL, BAD_CAST DEF_J1939_DATABASE_FILES);
                xmlAddChild(pNodePtr, pNodeDBFilePtr);

                if(pNodeDBFilePtr != NULL)
                {
                    for(INT nDbFileIndex = 0; nDbFileIndex < omDbNames.GetSize(); nDbFileIndex++)
                    {
                        const char* strFilePath = omDbNames[nDbFileIndex];
                        xmlChar* pDBFFilePath = (xmlChar*)strFilePath;

                        xmlNodePtr pFPPtr = xmlNewChild(pNodeDBFilePtr, NULL, BAD_CAST DEF_FILE_PATH, BAD_CAST pDBFFilePath);
                        xmlAddChild(pNodeDBFilePtr, pFPPtr);
                    }

                    //xmlTextWriterSetIndent(pNodeDBFilePtr, 1);
                }
            }

            //nSize += sizeof(UINT) + ((sizeof(char) * MAX_PATH) * omDbNames.GetSize());
            //pbyConfigData = new BYTE[nSize];

            //if (pbyConfigData != NULL)
            //{
            //    BYTE* pbyTemp = pbyConfigData;

            //    BYTE byVersion = 0x1;
            //    COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));
            //    //CAN DB NAMES
            //    UINT unCount = omDbNames.GetSize();
            //    COPY_DATA(pbyTemp, &unCount,  sizeof (UINT));
            //    for (UINT i = 0; i < unCount; i++)
            //    {
            //        CString omDbName = omDbNames.GetAt(i);
            //        char acName[MAX_PATH] = {_T('\0')};
            //        strcpy_s(acName, MAX_PATH, omDbName.GetBuffer(MAX_CHAR));
            //        COPY_DATA(pbyTemp, acName, (sizeof (char) * MAX_PATH));
            //    }
            //}
        }
        break;
        case DATABASE_SECTION_ID:
        {
            nSize += sizeof(BYTE);//configuration version

            CStringArray omDbNames;
            if (theApp.m_pouMsgSignal != NULL)
            {
                string omStrBasePath;
                CString omConfigFileName;
                vGetLoadedCfgFileName(omConfigFileName);
                CUtilFunctions::nGetBaseFolder(omConfigFileName.GetBuffer(MAX_PATH), omStrBasePath);
                theApp.m_pouMsgSignal->vGetRelativeDataBaseNames(omStrBasePath, &omDbNames);
            }

            if(pNodePtr != NULL)
            {
                //xmlNodePtr pNodeDBFilePtr = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_CAN_DATABASE_FILES, BAD_CAST "");

                xmlNodePtr pNodeDBFilePtr = xmlNewNode(NULL, BAD_CAST DEF_CAN_DATABASE_FILES);

                xmlAddChild(pNodePtr, pNodeDBFilePtr);

                if(pNodeDBFilePtr != NULL)
                {
                    for(INT nDbFileIndex = 0; nDbFileIndex < omDbNames.GetSize(); nDbFileIndex++)
                    {
                        const char* strFilePath = omDbNames[nDbFileIndex];
                        xmlChar* pDBFFilePath = (xmlChar*)strFilePath;

                        xmlNodePtr pNewChildPtr = xmlNewChild(pNodeDBFilePtr, NULL, BAD_CAST DEF_FILE_PATH, BAD_CAST pDBFFilePath);
                        xmlAddChild(pNodeDBFilePtr, pNewChildPtr);
                    }
                }
            }

        }
        break;
        case LIN_CLUSTER_CONFIG:
        {
            if(pNodePtr != NULL)
            {
                //xmlNodePtr pNodeDBFilePtr = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_CAN_DATABASE_FILES, BAD_CAST "");

                xmlNodePtr pNodeDBFilePtr = xmlNewNode(NULL, BAD_CAST DEF_LIN_CLUSTER_CONFIG);



                if(pNodeDBFilePtr != NULL)
                {
                    for ( int i = 0; i < 1; i++ )
                    {

                        //Channel
                        xmlNodePtr pNodeChannelPtr = xmlNewNode(NULL, BAD_CAST DEF_FLEX_CHANNEL_CHANNEL);


                        char chValue[256];

                        //Index
                        sprintf(chValue, "%d", i);
                        xmlNodePtr pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_INDEX ,BAD_CAST chValue);
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //HWUri

                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_HWURI ,BAD_CAST "Hardware" );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //DbPath
                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_DBPATH, BAD_CAST  m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_strDataBasePath.c_str() );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //ClusterId
                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_CLUSTERID, BAD_CAST  m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouClusterInfo.m_strName.c_str() );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //BaudRate
                        sprintf(chValue, "%d", m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_nBaudRate);
                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_LIN_BAUDRATE, BAD_CAST chValue  );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //ProtocolVersion
                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_LIN_PROTOCOLVER, BAD_CAST m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_strProtocolVersion.c_str());
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);


                        sprintf(chValue, "%d", m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_bOverWriteSettings);
                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_LIN_OVERWRITE_SETTINGS, BAD_CAST chValue );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);



                        //ECU
                        xmlNodePtr pNodeECUPtr = xmlNewNode(NULL, BAD_CAST DEF_FLEX_CHANNEL_ECU);
                        list<string>::iterator itrList;
                        for ( itrList = m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_strSlectedEculist.begin();
                                itrList != m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_strSlectedEculist.end(); itrList++)
                        {
                            pIndex= xmlNewChild(pNodeECUPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_NAME, BAD_CAST  itrList->c_str() );
                        }
                        xmlAddChild(pNodeChannelPtr, pNodeECUPtr);

                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);



                    }
                }
                xmlAddChild(pNodePtr, pNodeDBFilePtr);
            }
        }
        break;
        case FLEXRAY_CLUSTER_CONFIG:
        {
            if(pNodePtr != NULL)
            {
                //xmlNodePtr pNodeDBFilePtr = xmlNewChild(pNodePtr, NULL, BAD_CAST DEF_CAN_DATABASE_FILES, BAD_CAST "");

                xmlNodePtr pNodeDBFilePtr = xmlNewNode(NULL, BAD_CAST DEF_FLEXRAY_CLUSTER_CONFIG);



                if(pNodeDBFilePtr != NULL)
                {
                    for ( int i = 0; i < 1; i++ )
                    {

                        //Channel
                        xmlNodePtr pNodeChannelPtr = xmlNewNode(NULL, BAD_CAST DEF_FLEX_CHANNEL_CHANNEL);


                        char chValue[256];

                        //Index
                        sprintf(chValue, "%d", i);
                        xmlNodePtr pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_INDEX ,BAD_CAST chValue);
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //HWUri

                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_HWURI ,BAD_CAST "Hardware" );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //DbPath
                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_DBPATH, BAD_CAST  m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_strDataBasePath.c_str() );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //ClusterId
                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_CLUSTERID, BAD_CAST  m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_ouClusterInfo.m_strName.c_str() );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //ECU
                        xmlNodePtr pNodeECUPtr = xmlNewNode(NULL, BAD_CAST DEF_FLEX_CHANNEL_ECU);
                        list<string>::iterator itrList;
                        for ( itrList = m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_strSlectedEculist.begin();
                                itrList != m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_strSlectedEculist.end(); itrList++)
                        {
                            pIndex= xmlNewChild(pNodeECUPtr, NULL, BAD_CAST DEF_FLEX_CHANNEL_NAME, BAD_CAST  itrList->c_str() );
                        }
                        xmlAddChild(pNodeChannelPtr, pNodeECUPtr);

                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);



                    }
                }
                xmlAddChild(pNodePtr, pNodeDBFilePtr);
            }
        }
        break;
        case DATABASE_SECTION_FLEXRAY:
        {
            /*if(pNodePtr != NULL)
            {
                xmlNodePtr pNodeDBFilePtr = xmlNewNode(NULL, BAD_CAST DEF_FLEX_DATABASE);
                xmlAddChild(pNodePtr, pNodeDBFilePtr);
                if(pNodeDBFilePtr != NULL)
                {
                    const char* strFilePath = (m_acFlexDBConfigInfo.m_acConfigFileName[0]).c_str();
                    xmlChar* pDBFFilePath = (xmlChar*)strFilePath;
                    xmlNodePtr pNewChildPtr = xmlNewChild(pNodeDBFilePtr, NULL, BAD_CAST DEF_FILE_PATH, BAD_CAST pDBFFilePath);
                    xmlAddChild(pNodeDBFilePtr, pNewChildPtr);

                    strFilePath = (m_acFlexDBConfigInfo.m_acConfigFileName[1]).c_str();
                    pDBFFilePath = (xmlChar*)strFilePath;
                    pNewChildPtr = xmlNewChild(pNodeDBFilePtr, NULL, BAD_CAST DEF_FILE_PATH, BAD_CAST pDBFFilePath);
                    xmlAddChild(pNodeDBFilePtr, pNewChildPtr);
                }
            }*/
            //    {
            //        CString omDbName = omDbNames.GetAt(i);
            //        char acName[MAX_PATH] = {_T('\0')};
            //        strcpy_s(acName, MAX_PATH, omDbName.GetBuffer(MAX_PATH));
            //        COPY_DATA(pbyTemp, acName, (sizeof (char) * MAX_PATH));
            //    }
            //}
        }
        break;
        case WAVEFORMDATA_SECTION_ID:
        {
            xmlNodePtr pNodeWaveFormGen = xmlNewNode(NULL, BAD_CAST DEF_WF_GENERATOR);
            xmlAddChild(pNodePtr, pNodeWaveFormGen);
            m_objWaveformDataHandler.GetConfigData(pNodeWaveFormGen);
        }
        break;
        case BUS_STATISTICS_SECTION_ID:
        {
            if(m_bIsStatWndCreated)
            {
                xmlNodePtr pNodeWaveFormGen = xmlNewNode(NULL, BAD_CAST DEF_CAN_BUS_STATS);
                xmlAddChild(pNodePtr, pNodeWaveFormGen);

                m_podBusStatistics->GetConfigData(pNodeWaveFormGen);
            }
            else
            {
                if( m_pCopyBusStsticsNode != NULL )
                {
                    xmlNode* pTempNodePtr = xmlCopyNode(m_pCopyBusStsticsNode, 1);
                    xmlAddChild(pNodePtr, pTempNodePtr);
                }
                else //Store Default or Binary Config
                {
                    xmlNodePtr pNodeWaveFormGen = xmlNewNode(NULL, BAD_CAST DEF_CAN_BUS_STATS);
                    xmlAddChild(pNodePtr, pNodeWaveFormGen);
                    CBusStatisticsDlg::vGetDataFromStore(pNodeWaveFormGen);
                }
            }

        }
        break;
        //venkat
        case TEST_SETUP_EDITOR_SECTION_ID:
        {
            xmlNodePtr pNodeTSEditor = NULL;
            m_objTSEditorHandler.vGetConfigurationData(pNodeTSEditor);
            xmlAddChild(pNodePtr, pNodeTSEditor);
        }
        break;
        case TEST_SUITE_EXECUTOR_SECTION_ID:
        {
            xmlNodePtr pNodeExecutor = NULL;
            m_objTSExecutorHandler.vGetConfigurationData(pNodeExecutor);
            xmlAddChild(pNodePtr, pNodeExecutor);
        }
        break;
        case FLEXRAY_TXWND:
        {
            xmlNodePtr pFLEXTxPtr = xmlNewNode(NULL, BAD_CAST DEF_FLEX_TX_WINDOW);
            xmlAddChild(pNodePtr, pFLEXTxPtr);
            m_objFlexTxHandler.vGetTxWndConfigData( FLEXRAY, pFLEXTxPtr);
        }
        break;
        case LIN_TXWND:
        {
            xmlNodePtr pFLEXTxPtr = xmlNewNode(NULL, BAD_CAST DEF_LIN_TX_WINDOW);
            xmlAddChild(pNodePtr, pFLEXTxPtr);
            m_objFlexTxHandler.vGetTxWndConfigData(LIN, pFLEXTxPtr);
        }

        case MSGWND_SECTION_FLEXRAY_ID:
        {
            // Save FlexRay Message Window contents
            SMESSAGE_ATTRIB sMsgAttrib;
            sMsgAttrib.m_psMsgAttribDetails = NULL;
            sMsgAttrib.m_usMsgCount = 0;
            CMessageAttrib::ouGetHandle(FLEXRAY).vGetMessageAttribData(sMsgAttrib);
            UINT nCount = sMsgAttrib.m_usMsgCount;
            UINT unMsgFrmtWndCfgSize = 0;
            ASSERT(m_podMsgWndThread != NULL);

            if(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY))
            {
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                              WM_NOTIFICATION_FROM_OTHER, eWINID_MSG_WND_GET_CONFIG_SIZE, (LPARAM)&unMsgFrmtWndCfgSize);
            }
            //CALC SIZE ENDS
            xmlNodePtr pFlexRayMsgWindow = xmlNewNode(NULL, BAD_CAST DEF_FLEXRAY_MESS_WINDOW);
            xmlAddChild(pNodePtr, pFlexRayMsgWindow);

            const char* omcVarChar ;

            for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
            {
                if( sMsgAttrib.m_psMsgAttribDetails[i].sColor != RGB( 0, 0, 0 ) )
                {
                    xmlNodePtr pFlexRayMsgAttribute = xmlNewNode(NULL, BAD_CAST DEF_MSG_ATTRIBUTE);
                    xmlAddChild(pFlexRayMsgWindow, pFlexRayMsgAttribute);

                    omcVarChar = sMsgAttrib.m_psMsgAttribDetails[i].omStrMsgname.GetBuffer(MAX_PATH);
                    xmlNodePtr pMsgName = xmlNewChild(pFlexRayMsgAttribute, NULL, BAD_CAST DEF_NAME,BAD_CAST omcVarChar);
                    xmlAddChild(pFlexRayMsgAttribute, pMsgName);

                    CString     csMsgID;
                    csMsgID.Format("%u",sMsgAttrib.m_psMsgAttribDetails[i].unMsgID);
                    omcVarChar = csMsgID.GetBuffer(MAX_PATH);
                    xmlNodePtr pMsgID = xmlNewChild(pFlexRayMsgAttribute, NULL, BAD_CAST DEF_MSG_ID,BAD_CAST omcVarChar);
                    xmlAddChild(pFlexRayMsgAttribute, pMsgID);

                    CString     csColor;
                    int nR, nB, nG;
                    nR = GetRValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nG = GetGValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nB = GetBValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    //csColor.Format("%2x%2x%2x",nR,nG,nB);

                    char ch[7] = {0};

                    sprintf(ch, "%02x%02x%02x", nR,nG,nB);

                    omcVarChar = csColor.GetBuffer(MAX_PATH);
                    xmlNodePtr pMsgColor = xmlNewChild(pFlexRayMsgAttribute, NULL, BAD_CAST DEF_MSG_COLOR,BAD_CAST ch);
                    xmlAddChild(pFlexRayMsgAttribute, pMsgColor);
                }
            }

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                          WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_GET_CONFIG_DATA,
                          (LPARAM)pFlexRayMsgWindow);
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
}

INT CMainFrame::vSaveXMLConfiguration()
{
    CString filename;
    vGetLoadedCfgFileName(filename);

    vSaveXMLConfiguration(filename);
    return S_OK;
}

void CMainFrame::vSetGlobalConfiguration(xmlNodePtr& pNodePtr)
{
    pNodePtr = pNodePtr->xmlChildrenNode;

    while(pNodePtr != NULL)
    {
        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsMsgFilterEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strMsgFilter = ptext;

                if(strMsgFilter == _("FALSE"))
                {
                    m_sToolBarInfo.m_byMsgFilter = FALSE;
                }
                else if(strMsgFilter == _("TRUE"))
                {
                    m_sToolBarInfo.m_byMsgFilter = TRUE;
                }
                xmlFree(ptext);
            }
        }
        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsReplayFilterEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strReplayFilter = ptext;
                if(strReplayFilter == _("FALSE"))
                {
                    m_sToolBarInfo.m_byReplayFilter = FALSE;
                }
                else if(strReplayFilter == _("TRUE"))
                {
                    m_sToolBarInfo.m_byReplayFilter = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsLogFilterEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strLogFilter = ptext;

                if(strLogFilter == _("FALSE"))
                {
                    m_sToolBarInfo.m_byLogFilter = FALSE;
                }
                else if(strLogFilter == _("TRUE"))
                {
                    m_sToolBarInfo.m_byLogFilter = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsLoggingEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strLogging = ptext;

                if(strLogging == _("FALSE"))
                {
                    m_sToolBarInfo.m_byLogging = FALSE;
                }
                else if(strLogging == _("TRUE"))
                {
                    m_sToolBarInfo.m_byLogging = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsMsgIntepretationEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strMsgInterpret = ptext;

                if(strMsgInterpret == _("FALSE"))
                {
                    m_sToolBarInfo.m_byMsgInterpret = FALSE;
                    m_bInterPretMsg = FALSE;
                }
                else if(strMsgInterpret == _("TRUE"))
                {
                    m_sToolBarInfo.m_byMsgInterpret = TRUE;
                    m_bInterPretMsg = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsOverWriteEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strOverwrite = ptext;

                if(strOverwrite == _("FALSE"))
                {
                    m_sToolBarInfo.m_byOverwrite = FALSE;
                    theApp.pouGetFlagsPtr()->vSetFlagStatus(OVERWRITE, FALSE);
                }
                else if(strOverwrite == _("TRUE"))
                {
                    m_sToolBarInfo.m_byOverwrite = TRUE;
                    theApp.pouGetFlagsPtr()->vSetFlagStatus(OVERWRITE, TRUE);
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"DisplayTimeMode")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strDisplayTimeMode = ptext;

                if(strDisplayTimeMode == _("SYSTEM"))
                {
                    m_sToolBarInfo.m_byDisplayTimeMode = eSYSTEM_MODE;
                }
                else if(strDisplayTimeMode == _("ABSOLUTE"))
                {
                    m_sToolBarInfo.m_byDisplayTimeMode = eABSOLUTE_MODE;
                }
                else if(strDisplayTimeMode == _("RELATIVE"))
                {
                    m_sToolBarInfo.m_byDisplayTimeMode = eRELATIVE_MODE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"DisplayNumericMode")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strDisplayNumericON = ptext;

                if(strDisplayNumericON == _("TRUE"))
                {
                    // Setting decimal mode
                    bSetHexDecFlags(FALSE);
                    m_sToolBarInfo.m_byDisplayHexON = FALSE;
                }
                else if(strDisplayNumericON == _("FALSE"))
                {
                    // Setting Hex mode
                    bSetHexDecFlags(TRUE);
                    m_sToolBarInfo.m_byDisplayHexON = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"LogOnConnect_CAN")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strLogOnConnect = ptext;

                if(strLogOnConnect == _("TRUE"))
                {
                    m_abLogOnConnect[CAN] = TRUE;
                }
                else if(strLogOnConnect == _("FALSE"))
                {
                    m_abLogOnConnect[CAN] = FALSE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"LogOnConnect_J1939")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strLogOnConnect = ptext;
                if(strLogOnConnect == _("TRUE"))
                {
                    m_abLogOnConnect[J1939] = TRUE;
                }
                else if(strLogOnConnect == _("FALSE"))
                {
                    m_abLogOnConnect[J1939] = FALSE;
                }
                xmlFree(ptext);
            }
        }


        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"LogOnConnect_LIN")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strLogOnConnect = ptext;

                if(strLogOnConnect == _("TRUE"))
                {
                    m_abLogOnConnect[LIN] = TRUE;
                }
                else if(strLogOnConnect == _("FALSE"))
                {
                    m_abLogOnConnect[LIN] = FALSE;
                }
                xmlFree(ptext);
            }
        }


        pNodePtr = pNodePtr->next;
    }
}

void CMainFrame::vSetWindowPositionForGraph(xmlNodePtr pNodePtr, xmlDocPtr pDocPtr)
{
    pNodePtr = pNodePtr->xmlChildrenNode;

    while(pNodePtr != NULL)
    {
        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Visibility"))))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                m_sGraphWndPlacement[CAN].showCmd = xmlUtils::nGetWindowVisibility((char*)ptext);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"WindowPlacement")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                m_sGraphWndPlacement[CAN].flags = xmlUtils::nGetWindowVisibility((char*)ptext);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Top"))))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strTop = ptext;
                m_sGraphWndPlacement[CAN].rcNormalPosition.top = atoi(strTop);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Bottom"))))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strBottom = ptext;
                m_sGraphWndPlacement[CAN].rcNormalPosition.bottom = atoi(strBottom);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Left"))))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strLeft = ptext;
                m_sGraphWndPlacement[CAN].rcNormalPosition.left = atoi(strLeft);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Right"))))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(NULL != ptext)
            {
                CString strRight = ptext;
                m_sGraphWndPlacement[CAN].rcNormalPosition.right = atoi(strRight);
                xmlFree(ptext);
            }
        }

        pNodePtr = pNodePtr->next;
    }
}

int CMainFrame::nLoadXMLConfiguration()
{
    int nRetValue = S_OK;
    xmlNodeSetPtr pNodeSet;
    xmlXPathObjectPtr pPathObject;

    for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
    {
        switch(eSecId)
        {
            case MAINFRAME_SECTION_ID:
            {
                BYTE byVersion = 0;

                // Updating Global configuration parameters in to xml
                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Global_Configuration";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    INT nCount = pNodeSet->nodeNr;
                    for(int i = 0; i < nCount; i++)
                    {
                        vSetGlobalConfiguration(pNodeSet->nodeTab[i]);
                    }
                }

                theApp.pouGetFlagsPtr()->vSetToolbarButtonStatus(&m_sToolBarInfo);

                // Updating Toolbar Window position in to xml
                xmlChar* pchWndPosPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Global_Configuration/Window_Position";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchWndPosPath);
                if( NULL != pPathObject && pPathObject->nodesetval != NULL )
                {
                    if ( S_OK != xmlUtils::ParseWindowsPlacement(pPathObject->nodesetval->nodeTab[0], m_sNotificWndPlacement) )
                    {
                        m_sNotificWndPlacement.rcNormalPosition.top  = 459;
                        m_sNotificWndPlacement.rcNormalPosition.left = 0;
                        m_sNotificWndPlacement.rcNormalPosition.right = 1089;
                        m_sNotificWndPlacement.rcNormalPosition.bottom = 612;
                        m_sNotificWndPlacement.showCmd = SW_NORMAL;
                    }
                    if (m_podUIThread != NULL)
                    {
                        m_podUIThread->vUpdateWndCo_Ords(m_sNotificWndPlacement, TRUE);
                        m_podUIThread->vClearTraceContents();
                    }
                }
            }
            break;
            case DIL_SECTION_LIN_ID:
            {
                m_shLINDriverId = DAL_LIN_NONE;
                IntializeDILL();
                m_ouClusterConfig[LIN].InitialiseConfig();
            }
            break;
            case DATABASE_SECTION_ID:
            {
                vClearDbInfo(CAN);

                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Database_Files/FilePath";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            if (  NULL != pNodeSet->nodeTab[i]->xmlChildrenNode )
                            {
                                xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);
                                if ( ( NULL != ptext ) && ( theApp.m_pouMsgSignal != NULL ) )
                                {
                                    dLoadDataBaseFile(ptext, TRUE);
                                    xmlFree(ptext);
                                }
                            }
                        }
                    }
                    else
                    {
                        nRetValue = S_OK;
                    }
                    xmlXPathFreeObject (pPathObject);
                }
                else
                {
                    nRetValue = S_OK;
                }
            }
            break;
            case DATABASE_SECTION_J1939_ID:
            {
                //Clear all databases
                if (m_pouMsgSigJ1939 == NULL)
                {
                    m_pouMsgSigJ1939 = new CMsgSignal(sg_asDbParams[J1939], FALSE);
                }

                vClearDbInfo(J1939);

                CStringArray omDBNames;
                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/J1939_Database_Files/FilePath";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            if (NULL != pNodeSet->nodeTab[i]->xmlChildrenNode )
                            {
                                xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);
                                if ( NULL != ptext )
                                {
                                    omDBNames.Add(ptext);
                                    nRetValue = S_OK;
                                    xmlFree(ptext);
                                }
                            }
                        }
                        xmlXPathFreeObject (pPathObject);
                    }
                    else
                    {
                        nRetValue = S_OK ;
                    }
                }
                else
                {
                    nRetValue = S_OK;
                }
                if ( ( nRetValue == S_OK ) && ( m_pouMsgSigJ1939  != NULL ) )
                {
                    // m_pouMsgSigJ1939->vSetDataBaseNames(&omDBNames);
                    for (INT i = 0; i < omDBNames.GetSize(); i++)
                    {
                        //No need to check return value. Error message will be displayed
                        // in trace window
                        dLoadJ1939DBFile(omDBNames.GetAt(i), TRUE);
                    }
                    SMSGENTRY* psMsgEntry = NULL;
                    vPopulateMsgEntryFromDB(psMsgEntry, m_pouMsgSigJ1939);
                    if (m_pouTxMsgWndJ1939 != NULL)
                    {
                        m_pouTxMsgWndJ1939->vSetDatabaseInfo(psMsgEntry);
                    }
                    else
                    {
                        m_pouTxMsgWndJ1939->vUpdateDataStore(psMsgEntry);
                    }
                }
            }
            break;
            case LIN_CLUSTER_CONFIG:
            {
                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Cluster_Config/CHANNEL";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);


                /*LIN*/
                vInitialiaseLINConfig(1);





                for ( int i = 0; i < m_ouClusterConfig[LIN].m_nChannelsConfigured; i++ )
                {
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_eBusChannelType = LIN;
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_nBaudRate = 19200;
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_strProtocolVersion = "LIN 2.1";
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_strDataBasePath = "";
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_bOverWriteSettings = true;
                    //m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouClusterInfo.clear();
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_strSlectedEculist.clear();
                }

                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlNodePtr xmlChannel = pNodeSet->nodeTab[i]->xmlChildrenNode;
                            if (NULL != pNodeSet->nodeTab[i] )
                            {
                                xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                                sLinConfigContainer ouFibexContainer;
                                INT nCount = 0;
                                while ( pNodePtr != NULL )
                                {
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Index")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_nChannel = atoi((char*)key);
                                            nCount++;
                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"HWUri")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_strHWUri = (char*)key;
                                            nCount++;
                                        }
                                    }
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"DbPath")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_strDbPath = (char*)key;
                                            nCount++;

                                        }
                                    }
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"ClusterId")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_strClusterId = (char*)key;
                                            nCount++;

                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"BaudRate")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_nBaudRate = atoi((char*)key);
                                            nCount++;

                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"ProtocolVersion")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_srtProtocolVerson = (char*)key;
                                            nCount++;

                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)DEF_LIN_OVERWRITE_SETTINGS)))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_bOverwrite = atoi((char*)key);
                                            nCount++;
                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"ECU")))
                                    {
                                        //Nodes
                                        xmlChar* pchPath = (xmlChar*)"Name";
                                        pPathObject = xmlUtils::pGetChildNodes(pNodePtr, pchPath);
                                        if( NULL != pPathObject )
                                        {
                                            xmlNodeSetPtr pNodeSetEcu = pPathObject->nodesetval;
                                            if(NULL != pNodeSetEcu)
                                            {
                                                nCount++;

                                                for(int i=0; i < pNodeSetEcu->nodeNr; i++)
                                                {
                                                    xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodeSetEcu->nodeTab[i]->xmlChildrenNode , 1);
                                                    ouFibexContainer.m_strECUList.push_back((char*)key);
                                                }
                                            }
                                        }
                                    }
                                    pNodePtr = pNodePtr->next;
                                }   //while

                                vVlaidateAndLoadFibexConfig(ouFibexContainer);

                            }   //If

                        }   //Channel Configurations


                    }
                }
                else
                {

                }
                m_objFlexTxHandler.SetFibexConfig(LIN, m_ouClusterConfig[LIN]);

                m_objFlexTxHandler.vSetTxWndConfigData( LIN, m_xmlConfigFiledoc);
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                              WM_NOTIFICATION_FROM_OTHER,
                              eLOAD_DATABASE,
                              (LPARAM)&m_ouClusterConfig[FLEXRAY]);






            }
            break;
            case FLEXRAY_CLUSTER_CONFIG:
            {
                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/FLEX_Cluster_Config/CHANNEL";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                m_ouClusterConfig[FLEXRAY].m_nChannelsConfigured = 0;

                /*Unload FLEXRAY DIL */
                m_shFLEXRAYDriverId = DAL_NONE;
                InitializeFLEXRAYDIL();
                m_ouClusterConfig[FLEXRAY].InitialiseConfig();


                for ( int i = 0; i < m_ouClusterConfig[FLEXRAY].m_nChannelsConfigured; i++ )
                {
                    m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_ouFlexRayParams.m_nKetSlot = -1;
                    m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_ouFlexRayParams.m_nSecondKeySlot = -1;
                    m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_strDataBasePath = "";
                    //m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_ouClusterInfo.clear();
                    m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_strSlectedEculist.clear();
                }

                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlNodePtr xmlChannel = pNodeSet->nodeTab[i]->xmlChildrenNode;
                            if (NULL != pNodeSet->nodeTab[i] )
                            {
                                xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                                sFibexConfigContainer ouFibexContainer;
                                INT nCount = 0;
                                while ( pNodePtr != NULL )
                                {
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Index")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_nChannel = atoi((char*)key);
                                            nCount++;
                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"HWUri")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_strHWUri = (char*)key;
                                            nCount++;
                                        }
                                    }
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"DbPath")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_strDbPath = (char*)key;
                                            nCount++;

                                        }
                                    }
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"ClusterId")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_strClusterId = (char*)key;
                                            nCount++;

                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Key_Slot")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_nKeySlot = atoi((char*)key);
                                            nCount++;

                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Second_Key_Slt")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_nSecondKeySlot = atoi((char*)key);
                                            nCount++;

                                        }
                                    }


                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"ECU")))
                                    {
                                        //Nodes
                                        xmlChar* pchPath = (xmlChar*)"Name";
                                        pPathObject = xmlUtils::pGetChildNodes(pNodePtr, pchPath);
                                        if( NULL != pPathObject )
                                        {
                                            xmlNodeSetPtr pNodeSetEcu = pPathObject->nodesetval;
                                            if(NULL != pNodeSetEcu)
                                            {
                                                nCount++;

                                                for(int i=0; i < pNodeSetEcu->nodeNr; i++)
                                                {
                                                    xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodeSetEcu->nodeTab[i]->xmlChildrenNode , 1);
                                                    ouFibexContainer.m_strECUList.push_back((char*)key);
                                                }
                                            }
                                        }
                                    }
                                    pNodePtr = pNodePtr->next;
                                }   //while

                                vVlaidateAndLoadFibexConfig(ouFibexContainer);

                            }   //If

                        }   //Channel Configurations


                    }
                }
                else
                {

                }

                m_objFlexTxHandler.SetFibexConfig(FLEXRAY, m_ouClusterConfig[FLEXRAY]);
                m_objFlexTxHandler.vSetTxWndConfigData( FLEXRAY, m_xmlConfigFiledoc);
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                              WM_NOTIFICATION_FROM_OTHER,
                              eLOAD_DATABASE,
                              (LPARAM)&m_ouClusterConfig[FLEXRAY]);

            }
            break;
            case DATABASE_SECTION_FLEXRAY:
            {
                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/FLEX_Database/FilePath";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(NULL != pNodeSet)
                    {
                        m_acFlexDBConfigInfo.m_nFilesCount = 0;
                        if ( pNodeSet->nodeNr == 2 )
                        {
                            m_acFlexDBConfigInfo.m_nSelectedIndex = 1;
                        }
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            if (NULL != pNodeSet->nodeTab[i]->xmlChildrenNode )
                            {
                                xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);
                                if ( NULL != ptext )
                                {
                                    m_acFlexDBConfigInfo.m_acConfigFileName[i] = (char*)ptext;
                                    m_acFlexDBConfigInfo.m_nFilesCount++;
                                    nRetValue = S_OK;
                                    xmlFree(ptext);
                                }
                            }
                        }
                        xmlXPathFreeObject (pPathObject);
                    }
                    else
                    {
                        nRetValue = S_OK ;
                    }
                }
                else
                {
                    nRetValue = S_OK;
                }
            }
            break;
            case DIL_SECTION_ID:
            {

                xmlChar* pchPathChannel = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_DIL_Section/ControllerSettings/Channel";
                xmlChar* pchPathDriver = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_DIL_Section/DriverName";
                xmlChar* pchPathControllerMode = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_DIL_Section/ControllerMode";
                BOOL bProper = TRUE;

                UINT unChannelCount = CHANNEL_ALLOWED;

                //COPY_DATA_2(&m_dwDriverId, pbyTemp, sizeof(DWORD));
                if(m_xmlConfigFiledoc != NULL)
                {
                    pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPathDriver);
                    if( NULL != pPathObject )
                    {
                        nRetValue = S_FALSE;
                        pNodeSet = pPathObject->nodesetval;
                        if ( pNodeSet != NULL &&  pNodeSet->nodeTab[0] != NULL && pNodeSet->nodeTab[0]->xmlChildrenNode != NULL )
                            //Tight Checking
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[0]->xmlChildrenNode, 1);
                            if ( NULL != ptext)
                            {
                                m_dwDriverId = nGetControllerID((char*)ptext);
                                //cleaning
                                xmlFree(ptext);
                            }
                        }
                        else
                        {
                            m_dwDriverId = DIL_CAN_STUB;
                        }
                        xmlXPathFreeObject (pPathObject);
                    }
                    else
                    {
                        m_dwDriverId = DIL_CAN_STUB;
                    }

                    //COPY_DATA_2(&m_byControllerMode, pbyTemp, sizeof(BYTE));
                    pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPathControllerMode);
                    if( NULL != pPathObject )
                    {
                        pNodeSet = pPathObject->nodesetval;
                        if ( pNodeSet != NULL &&  pNodeSet->nodeTab[0] != NULL && pNodeSet->nodeTab[0]->xmlChildrenNode != NULL )
                            //Tight Checking
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[0]->xmlChildrenNode, 1);
                            if ( NULL != ptext)
                            {
                                m_byControllerMode = 0;//we are not using
                                xmlFree(ptext);
                            }
                        }
                        else
                        {
                            m_byControllerMode = 0;
                        }
                        xmlXPathFreeObject (pPathObject);
                    }
                    else
                    {
                        m_byControllerMode = 0;
                    }

                    //COPY_DATA_2(m_asControllerDetails, pbyTemp, (sizeof(SCONTROLLER_DETAILS) * unChannelCount));
                    for(int i = unChannelCount; i < defNO_OF_CHANNELS; i++)
                    {
                        m_asControllerDetails[i].vInitialize(TRUE);
                    }
                    pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPathChannel);
                    if( NULL != pPathObject )
                    {
                        pNodeSet = pPathObject->nodesetval;
                        if ( pNodeSet != NULL )
                            //Tight Checking
                        {
                            unChannelCount = pNodeSet->nodeNr;
                            for(int i = 0; i < unChannelCount; i++)
                            {
                                LoadControllerConfigData(m_asControllerDetails[i], pNodeSet->nodeTab[i]);
                            }
                        }
                        xmlXPathFreeObject (pPathObject);
                    }


                    IntializeDIL(unChannelCount);
                    ASSERT(g_pouDIL_CAN_Interface != NULL);
                    g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails,
                            defNO_OF_CHANNELS);
                }

                if ((NULL == m_xmlConfigFiledoc) || ( bProper ==  FALSE))
                {
                    m_dwDriverId = DRIVER_CAN_STUB;
                    m_byControllerMode = defMODE_SIMULATE;
                    IntializeDIL();
                    for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
                    {
                        m_asControllerDetails[i].vInitialize(TRUE);
                    }

                    g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails,
                            sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS);
                }
            }
            break;
            case FILTER_SECTION_ID:
            {
                nRetValue = m_sFilterAppliedCAN.nSetXMLConfigData(m_xmlConfigFiledoc);
            }
            break;

            case SIMSYS_SECTION_ID:
            {
                if (GetICANNodeSim() != NULL)
                {
                    GetICANNodeSim()->NS_SetSimSysConfigData(m_xmlConfigFiledoc);
                }
            }
            break;

            case SIMSYS_SECTION_J1939_ID:
            {
                if (GetIJ1939NodeSim() != NULL)
                {
                    GetIJ1939NodeSim()->NS_SetSimSysConfigData(m_xmlConfigFiledoc);
                }
            }
            case SIMSYS_SECTION_LIN_ID:
            {
                if (GetILINNodeSim() != NULL)
                {
                    GetILINNodeSim()->NS_SetSimSysConfigData(m_xmlConfigFiledoc);
                }
            }
            break;
            case REPLAY_SECTION_ID:
            {
                vREP_SetXMLReplayConfigData(m_xmlConfigFiledoc);
            }
            break;
            case MSGWND_SECTION_ID:
            {
                BYTE byVersion = 0;

                //Msg Attributes
                SMESSAGE_ATTRIB sMsgAttrib;
                sMsgAttrib.m_psMsgAttribDetails = NULL;
                sMsgAttrib.m_usMsgCount = 0;

                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Message_Attribute";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(NULL != pNodeSet)
                    {
                        // Get the Message Count from xml
                        sMsgAttrib.m_usMsgCount = pNodeSet->nodeNr;
                        PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                        for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != NULL)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Name"))))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        pMessageAtt[i].omStrMsgname = ((CString)ptext);
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Message_ID"))))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        pMessageAtt[i].unMsgID = atoi(((CString)ptext));
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Color"))))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strColor = ptext;
                                        DWORD dwColor = strtoul(strColor, NULL, 16);

                                        COLORREF rgbTreeItem = RGB(GetBValue(dwColor),GetGValue(dwColor),GetRValue(dwColor));

                                        pMessageAtt[i].sColor = rgbTreeItem;
                                        xmlFree(ptext);
                                    }
                                }

                                pNodePtr = pNodePtr->next;
                            }
                        }
                        sMsgAttrib.m_psMsgAttribDetails = pMessageAtt;
                        CMessageAttrib::ouGetHandle(CAN).vSetMessageAttribData(&sMsgAttrib);
                        theApp.vPopulateCANIDList();
                    }
                }

                // Copying Message Buffer Details
                /* Append buffer size */
                xmlChar* pchAppBuffSizePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Append_Buffer_Size";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchAppBuffSizePath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( NULL != ptext ))
                            {
                                CString strAppndBufferSize = ptext;
                                m_anMsgBuffSize[defAPPEND_DATA_INDEX] = atoi(strAppndBufferSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                /* Overwrite buffer size */
                xmlChar* pchOvrBuffSizePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Overwrite_Buffer_Size";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchOvrBuffSizePath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( NULL != ptext ))
                            {
                                CString strOvrwriteBufferSize = ptext;
                                m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = atoi(strOvrwriteBufferSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                /* Display update time */
                xmlChar* pchDispUpTimePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Display_Update_Time";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchDispUpTimePath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( NULL != ptext ))
                            {
                                CString strDispUpdTimeSize = ptext;
                                m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = atoi(strDispUpdTimeSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                //Msg Filter
                bool bResult = false;
                SFILTERAPPLIED_CAN sMsgWndFilter;
                //pbyTemp = sMsgWndFilter.pbSetConfigData(pbyTemp, bResult);

                // Get the Filter data from xml
                pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Filter";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(NULL != pNodeSet)
                    {
                        //for(INT nIndex =0; nIndex < pNodeSet->nodeNr; nIndex++)
                        {
                            sMsgWndFilter.pbSetConfigData(m_sFilterAppliedCAN, pNodeSet, m_xmlConfigFiledoc, bResult);
                            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                          WM_SET_FILTER_DETAILS, (WPARAM)&m_sFilterAppliedCAN, NULL);
                        }
                    }
                }

                if(NULL != m_xmlConfigFiledoc)
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA_XML,
                                  (LPARAM)m_xmlConfigFiledoc);


                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eLOAD_DATABASE,
                                  (LPARAM)&(theApp.m_pouMsgSignal));
                }

                if(sMsgAttrib.m_usMsgCount > 0)
                {
                    //clear msg attributes
                    DELETE_ARRAY(sMsgAttrib.m_psMsgAttribDetails);
                    sMsgAttrib.m_usMsgCount = 0;
                }
            }
            break;
            case MSGWND_SECTION_LIN_ID:
            {
                BYTE byVersion = 0;

                //Msg Attributes
                SMESSAGE_ATTRIB sMsgAttrib;
                sMsgAttrib.m_psMsgAttribDetails = NULL;
                sMsgAttrib.m_usMsgCount = 0;

                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Message_Window/Message_Attribute";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(NULL != pNodeSet)
                    {
                        // Get the Message Count from xml
                        sMsgAttrib.m_usMsgCount = pNodeSet->nodeNr;
                        PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                        for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != NULL)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Name"))))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        pMessageAtt[i].omStrMsgname = ((CString)ptext);
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Message_ID"))))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        pMessageAtt[i].unMsgID = atoi(((CString)ptext));
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Color"))))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strColor = ptext;
                                        DWORD dwColor = strtoul(strColor, NULL, 16);

                                        COLORREF rgbTreeItem = RGB(GetBValue(dwColor),GetGValue(dwColor),GetRValue(dwColor));

                                        pMessageAtt[i].sColor = rgbTreeItem;
                                        xmlFree(ptext);
                                    }
                                }

                                pNodePtr = pNodePtr->next;
                            }
                        }
                        sMsgAttrib.m_psMsgAttribDetails = pMessageAtt;
                        CMessageAttrib::ouGetHandle(LIN).vSetMessageAttribData(&sMsgAttrib);
                        //theApp.vPopulateLINIDList();
                    }
                }

                // Copying Message Buffer Details
                /* Append buffer size */
                xmlChar* pchAppBuffSizePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Message_Window/Append_Buffer_Size";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchAppBuffSizePath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( NULL != ptext ))
                            {
                                CString strAppndBufferSize = ptext;
                                m_anMsgBuffSize[defAPPEND_DATA_INDEX] = atoi(strAppndBufferSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                /* Overwrite buffer size */
                xmlChar* pchOvrBuffSizePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Message_Window/Overwrite_Buffer_Size";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchOvrBuffSizePath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( NULL != ptext ))
                            {
                                CString strOvrwriteBufferSize = ptext;
                                m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = atoi(strOvrwriteBufferSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                /* Display update time */
                xmlChar* pchDispUpTimePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Message_Window/Display_Update_Time";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchDispUpTimePath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(NULL != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( NULL != ptext ))
                            {
                                CString strDispUpdTimeSize = ptext;
                                m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = atoi(strDispUpdTimeSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                if(NULL != m_xmlConfigFiledoc)
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA_LIN_XML,
                                  (LPARAM)m_xmlConfigFiledoc);


                    /*::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eLOAD_DATABASE,
                                  (LPARAM)&(theApp.m_pouMsgSignal));*/
                }

                if(sMsgAttrib.m_usMsgCount > 0)
                {
                    //clear msg attributes
                    DELETE_ARRAY(sMsgAttrib.m_psMsgAttribDetails);
                    sMsgAttrib.m_usMsgCount = 0;
                }
            }
            break;
            case LOG_SECTION_ID:
            {
                INT nRetVal = S_OK;
                if (m_xmlConfigFiledoc != NULL)
                {
                    if (sg_pouFrameProcCAN != NULL)
                    {
                        sg_pouFrameProcCAN->FPC_StartEditingSession();
                        nRetVal = sg_pouFrameProcCAN->FPC_SetConfigData(m_xmlConfigFiledoc);
                        sg_pouFrameProcCAN->FPC_StopEditingSession(TRUE);
                    }
                    //Start logging if toolbar status is enabled.
                    BOOL bLogON = FALSE;
                    CFlags* pFlag = theApp.pouGetFlagsPtr();
                    if(pFlag != NULL)
                    {
                        bLogON = pFlag->nGetFlagStatus(LOGTOFILE);
                        BOOL bFilterON = FALSE;
                        bFilterON = pFlag->nGetFlagStatus(LOGFILTER);
                        vStartStopLogging( bLogON );
                    }
                }
                else if ( (m_xmlConfigFiledoc != NULL) || (nRetVal == S_FALSE) )
                {
                    sg_pouFrameProcCAN = GetICANLogger();
                    sg_pouFrameProcCAN->FPC_StartEditingSession();
                    sg_pouFrameProcCAN->FPC_ClearLoggingBlockList();
                    sg_pouFrameProcCAN->FPC_StopEditingSession(TRUE);
                }
            }
            break;
            case LOG_SECTION_J1939_ID:
            {
                if (m_xmlConfigFiledoc != NULL)
                {
                    if (GetIJ1939Logger() != NULL)
                    {
                        GetIJ1939Logger()->FPJ1_StartEditingSession();
                        GetIJ1939Logger()->FPJ1_SetConfigData(m_xmlConfigFiledoc);
                        GetIJ1939Logger()->FPJ1_StopEditingSession(TRUE);
                    }
                }
                else
                {
                    if (GetIJ1939Logger() != NULL)
                    {
                        GetIJ1939Logger()->FPJ1_StartEditingSession();
                        GetIJ1939Logger()->FPJ1_ClearLoggingBlockList();
                        GetIJ1939Logger()->FPJ1_StopEditingSession(TRUE);
                    }
                }
            }
            break;
            case LOG_SECTION_LIN_ID:
            {
                INT nRetVal = S_OK;
                if (m_xmlConfigFiledoc != NULL)
                {
                    if (sg_pouFrameProcLIN != NULL)
                    {
                        sg_pouFrameProcLIN->FPL_StartEditingSession();
                        nRetVal = sg_pouFrameProcLIN->FPL_SetConfigData(m_xmlConfigFiledoc);
                        sg_pouFrameProcLIN->FPL_StopEditingSession(TRUE);
                    }
                    //Start logging if toolbar status is enabled.
                    BOOL bLogON = FALSE;
                    CFlags* pFlag = theApp.pouGetFlagsPtr();
                    if(pFlag != NULL)
                    {
                        bLogON = pFlag->nGetFlagStatus(LOGTOFILE_LIN);
                        BOOL bFilterON = FALSE;
                        // bFilterON = pFlag->nGetFlagStatus(LOGFILTER);
                        vStartStopLogging_LIN( bLogON );
                    }
                }
                else if ( (m_xmlConfigFiledoc != NULL) || (nRetVal == S_FALSE) )
                {
                    sg_pouFrameProcLIN = GetILINLogger();
                    sg_pouFrameProcLIN->FPL_StartEditingSession();
                    sg_pouFrameProcLIN->FPL_ClearLoggingBlockList();
                    sg_pouFrameProcLIN->FPL_StopEditingSession(TRUE);
                }
            }
            break;


            case SIGWATCH_SECTION_ID:
            {

                if (sg_pouSWInterface[CAN] == NULL)
                {
                    if (SW_GetInterface(CAN, (void**)&sg_pouSWInterface[CAN]) == S_OK)
                    {
                        sg_pouSWInterface[CAN]->SW_DoInitialization();
                    }
                }


                CMainEntryList odMainEntryList;
                BOOL bProper = bParseSignalWatchXMLconfig(CAN, odMainEntryList);

                xmlXPathObjectPtr pOjectPath = NULL;
                xmlNodePtr pNodePtr = NULL;

                xmlChar* pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Watch";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);

                if(pOjectPath != NULL)
                {
                    bProper = TRUE;
                }

                if(bProper == TRUE)
                {
                    vPopulateSigWatchList(odMainEntryList, m_psSignalWatchList[CAN], theApp.m_pouMsgSignal);
                    m_ouMsgInterpretSW_C.vSetMessageList(m_psSignalWatchList[CAN]);
                    sg_pouSWInterface[CAN]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_C);


                    if(pOjectPath != NULL)
                    {
                        xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                        if(pNodeSet != NULL)
                        {
                            pNodePtr = pNodeSet->nodeTab[0];
                            if(pNodePtr != NULL)
                            {
                                sg_pouSWInterface[CAN]->SW_SetConfigData((xmlNodePtr)(pNodePtr->children));
                            }
                        }
                        xmlXPathFreeObject(pOjectPath);
                        //bProper = FALSE;
                    }

                }
                if (bProper == FALSE)
                {
                    //Set default settings
                    vReleaseSignalWatchListMemory(m_psSignalWatchList[CAN]);

                    m_ouMsgInterpretSW_C.vSetMessageList(m_psSignalWatchList[CAN]);
                    sg_pouSWInterface[CAN]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_C);
                    sg_pouSWInterface[CAN]->SW_SetConfigData((void*)NULL);
                }
                bProper = FALSE;
            }
            break;
            case SIGWATCH_SECTION_J1939_ID:
            {
                if (sg_pouSWInterface[J1939] == NULL)
                {
                    if (SW_GetInterface(J1939, (void**)&sg_pouSWInterface[J1939]) == S_OK)
                    {
                        sg_pouSWInterface[J1939]->SW_DoInitialization();
                    }
                }

                CMainEntryList odMainEntryList;
                BOOL bProper = bParseSignalWatchXMLconfig(J1939, odMainEntryList);

                xmlXPathObjectPtr pOjectPath = NULL;
                xmlNodePtr pNodePtr = NULL;

                xmlChar* pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/J1939_Signal_Watch";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);

                if(pOjectPath != NULL)
                {
                    bProper = TRUE;
                }

                if(bProper == TRUE && sg_pouSWInterface[J1939] != NULL)
                {
                    vPopulateSigWatchList(odMainEntryList, m_psSignalWatchList[J1939], m_pouMsgSigJ1939);
                    m_ouMsgInterpretSW_J.vSetJ1939Database(m_psSignalWatchList[J1939]);
                    sg_pouSWInterface[J1939]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_J);

                    if(pOjectPath != NULL)
                    {
                        xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                        if(pNodeSet != NULL)
                        {
                            pNodePtr = pNodeSet->nodeTab[0];
                            if(pNodePtr != NULL)
                            {
                                sg_pouSWInterface[J1939]->SW_SetConfigData((xmlNodePtr)(pNodePtr->children));
                            }
                        }
                        xmlXPathFreeObject(pOjectPath);
                        //bProper = FALSE;
                    }
                }
                if (bProper == FALSE && sg_pouSWInterface[J1939] != NULL)
                {
                    //Set default settings
                    vReleaseSignalWatchListMemory(m_psSignalWatchList[J1939]);

                    m_ouMsgInterpretSW_J.vSetJ1939Database(m_psSignalWatchList[J1939]);
                    sg_pouSWInterface[J1939]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_J);
                    sg_pouSWInterface[J1939]->SW_SetConfigData((void*)NULL);
                }
                bProper = FALSE;
            }
            break;
            case WAVEFORMDATA_SECTION_ID:
            {
                m_objWaveformDataHandler.SetConfigData((xmlDocPtr)m_xmlConfigFiledoc);
            }
            break;
            case BUS_STATISTICS_SECTION_ID:
            {
                if(m_pCopyBusStsticsNode != NULL)
                {
                    xmlFreeNode(m_pCopyBusStsticsNode);
                    m_pCopyBusStsticsNode = NULL;
                }
                xmlXPathObjectPtr pTempNode = xmlUtils::pGetNodes(m_xmlConfigFiledoc, (xmlChar*)("//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Bus_Statistics"));

                if(pTempNode != NULL && pTempNode->nodesetval != NULL && pTempNode->nodesetval->nodeTab[0] != NULL)
                {
                    m_pCopyBusStsticsNode = xmlCopyNode(pTempNode->nodesetval->nodeTab[0], 1);
                }
                else
                {
                    m_pCopyBusStsticsNode = NULL;
                }

                int nRetVal = S_OK;
                if(m_bIsStatWndCreated)
                {
                    if ( NULL != pTempNode )
                    {
                        nRetVal = m_podBusStatistics->SetConfigData(m_pCopyBusStsticsNode);
                        // On Load Config hiding Network statistics window
                        m_podBusStatistics->ShowWindow(SW_HIDE);
                    }
                    else
                    {
                        CBusStatisticsDlg::vSetDefaultsToStore();
                        m_podBusStatistics->vLoadDataFromStore();
                    }
                }
                else
                {
                    if (m_pCopyBusStsticsNode != NULL)
                    {
                        //BYTE* pbyTemp = pbyConfigData;
                        CBusStatisticsDlg::vSaveDataToStore();
                    }
                    else
                    {
                        CBusStatisticsDlg::vSetDefaultsToStore();
                    }
                }
            }
            break;
            case TXWND_SECTION_ID:
            {
                m_objTxHandler.vSetTxWndConfigData(m_xmlConfigFiledoc);
            }
            break;

            case MSGWND_SECTION_J1939_ID:
            {
                xmlNodePtr pNode = NULL;
                xmlNodePtr pMsgAttibNode = NULL;
                xmlNodePtr pChildNode = NULL;
                //Msg Attributes
                SMESSAGE_ATTRIB sMsgAttrib;
                sMsgAttrib.m_psMsgAttribDetails = NULL;
                sMsgAttrib.m_usMsgCount = 0;

                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/J1939_Message_Window/Message_Attribute";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( NULL != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(NULL != pNodeSet)
                    {
                        // Get the Message Count from xml
                        sMsgAttrib.m_usMsgCount = pNodeSet->nodeNr;
                        //COPY_DATA_2(&(sMsgAttrib.m_usMsgCount), pbyTemp, sizeof(UINT));
                        PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                        for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != NULL)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Name"))))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        pMessageAtt[i].omStrMsgname = ((CString)ptext);
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Message_ID"))))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        pMessageAtt[i].unMsgID = atoi(((CString)ptext));
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)_("Color"))))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strColor = ptext;
                                        DWORD dwColor = strtoul(strColor, NULL, 16);

                                        COLORREF rgbTreeItem = RGB(GetBValue(dwColor),GetGValue(dwColor),GetRValue(dwColor));

                                        pMessageAtt[i].sColor = rgbTreeItem;
                                        xmlFree(ptext);
                                    }
                                }

                                pNodePtr = pNodePtr->next;
                            }
                        }
                        sMsgAttrib.m_psMsgAttribDetails = pMessageAtt;
                        CMessageAttrib::ouGetHandle(J1939).vSetMessageAttribData(&sMsgAttrib);
                        vPopulateJ1939PGNList();
                    }
                }

                if(m_xmlConfigFiledoc != NULL)
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA_J1939_XML,
                                  (LPARAM)m_xmlConfigFiledoc);
                }
            }
            break;
            case TEST_SETUP_EDITOR_SECTION_ID:
            {
                m_objTSEditorHandler.vSetConfigurationData(m_xmlConfigFiledoc);
            }
            break;
            case TEST_SUITE_EXECUTOR_SECTION_ID:
            {
                m_objTSExecutorHandler.vSetConfigurationData(m_xmlConfigFiledoc);
            }
            break;
            case GRAPH_SECTION_ID:
            {
                xmlXPathObjectPtr pOjectPath = NULL;
                xmlChar* pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/GRAPH_PARAMETERS";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != NULL)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != NULL)
                    {
                        /* Clear Graph List before setting with current configuration values */
                        if(m_odGraphList[CAN].m_omElementList.GetSize()>0)
                        {
                            m_odGraphList[CAN].m_omElementList.RemoveAll();
                        }
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            m_odGraphList[CAN].pbySetConfigData(pNodeSet->nodeTab[i], m_xmlConfigFiledoc);
                        }
                    }
                }


                pOjectPath = NULL;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Window_Position";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != NULL)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != NULL)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            vSetWindowPositionForGraph(pNodeSet->nodeTab[i], m_xmlConfigFiledoc);
                        }
                    }
                }

                pOjectPath = NULL;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Splitter_Window_Col_0";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != NULL)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != NULL)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != NULL)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxIdeal")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strCxIdeal = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRootSplitterData[0][0] = atoi(strCxIdeal);

                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxMin")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strCxMin = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRootSplitterData[0][1] = atoi(strCxMin);

                                        xmlFree(ptext);
                                    }
                                }
                                pNodePtr = pNodePtr->next;
                            }
                        }
                    }
                }

                pOjectPath = NULL;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Splitter_Window_Col_1";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != NULL)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != NULL)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != NULL)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxIdeal")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strCxIdeal = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRootSplitterData[1][0] = atoi(strCxIdeal);

                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxMin")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strCxMin = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRootSplitterData[1][1] = atoi(strCxMin);

                                        xmlFree(ptext);
                                    }
                                }
                                pNodePtr = pNodePtr->next;
                            }
                        }
                    }
                }

                pOjectPath = NULL;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Splitter_Window_Row_0";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != NULL)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != NULL)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != NULL)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxIdeal")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strCxIdeal = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRightViewSplitterData[0][0] = atoi(strCxIdeal);

                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxMin")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strCxMin = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRightViewSplitterData[0][1] = atoi(strCxMin);

                                        xmlFree(ptext);
                                    }
                                }
                                pNodePtr = pNodePtr->next;
                            }
                        }
                    }
                }

                pOjectPath = NULL;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Splitter_Window_Row_1";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != NULL)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != NULL)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != NULL)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxIdeal")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strCxIdeal = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRightViewSplitterData[1][0] = atoi(strCxIdeal);

                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxMin")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(NULL != ptext)
                                    {
                                        CString strCxMin = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRightViewSplitterData[1][1] = atoi(strCxMin);

                                        xmlFree(ptext);
                                    }
                                }

                                pNodePtr = pNodePtr->next;
                            }
                        }
                    }
                }

                for(int nBUSID=0; nBUSID<AVAILABLE_PROTOCOLS; nBUSID++)
                {
                    m_objSigGrphHandler.SetSignalListDetails((SHORT)nBUSID, &m_odGraphList[nBUSID]);
                    m_objSigGrphHandler.SetWindowSplitterPos((SHORT)nBUSID, m_sGraphWndPlacement[nBUSID],
                            m_sGraphSplitterPos[nBUSID]);
                }
                //Arun
                vPostConfigChangeCmdToSigGrphWnds();
                break;
            }
            case FLEXRAY_TXWND:
            {
                //called from fibex config
                //m_objFlexTxHandler.vSetTxWndConfigData(m_xmlConfigFiledoc);
            }
            break;
            case MSGWND_SECTION_FLEXRAY_ID:
            {
                // Load FlexRay Message Window contents
                xmlNodePtr pNode = NULL;
                xmlNodePtr pMsgAttibNode = NULL;
                xmlNodePtr pChildNode = NULL;
                //Msg Attributes
                SMESSAGE_ATTRIB sMsgAttrib;
                sMsgAttrib.m_psMsgAttribDetails = NULL;
                sMsgAttrib.m_usMsgCount = 0;

                if(m_xmlConfigFiledoc != NULL)
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA_FLEXRAY_XML,
                                  (LPARAM)m_xmlConfigFiledoc);

                    list<Cluster> ouCluster;
                    m_pouMsgSigFLEXRAY->hLoadFibexDBFile(m_acFlexDBConfigInfo.m_acConfigFileName[0].c_str(), ouCluster);

                    /* ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                                   WM_NOTIFICATION_FROM_OTHER,
                                   eLOAD_DATABASE,
                                   (LPARAM)&(m_pouMsgSigFLEXRAY));*/
                }
            }
            break;
        } //switch(eSecId)
        eSecId = static_cast<eSECTION_ID>(eSecId + 1);
    }//for
    nRetValue = S_OK;
    return nRetValue;
}
void CMainFrame::vVlaidateAndLoadFibexConfig(sLinConfigContainer& ouFibexContainer)
{
    if ( m_pouMsgSigLIN != NULL && ouFibexContainer.m_nChannel >= 0 && ouFibexContainer.m_nChannel < CHANNEL_ALLOWED )
    {
        bool bValid = false;
        CHANNEL_CONFIG ouFlexChannel;
        std::list<Cluster> ouClusterList;
        std::list<LinChannelParam> ouLinParams;
        if ( S_OK == m_pouMsgSigFLEXRAY->hLoadLdfFile(ouFibexContainer.m_strDbPath.c_str(), ouClusterList, ouLinParams) )
        {
            list<Cluster>::iterator itrCluster;
            for ( itrCluster = ouClusterList.begin(); itrCluster != ouClusterList.end(); itrCluster++ )
            {
                if ( itrCluster->m_strName == ouFibexContainer.m_strClusterId )
                {
                    ouFlexChannel.m_strDataBasePath = ouFibexContainer.m_strDbPath;
                    ouFlexChannel.m_strSlectedEculist = ouFibexContainer.m_strECUList;
                    ouFlexChannel.m_ouClusterInfo = *itrCluster;
                    ouFlexChannel.m_ouLinParams.m_nBaudRate = ouFibexContainer.m_nBaudRate;
                    ouFlexChannel.m_ouLinParams.m_strProtocolVersion = ouFibexContainer.m_srtProtocolVerson;
                    ouFlexChannel.m_ouLinParams.m_bOverWriteSettings = ouFibexContainer.m_bOverwrite;
                    bValid = true;
                    break;
                }
            }
            if ( true == bValid )
            {
                m_ouClusterConfig[LIN].m_ouFlexChannelConfig[ouFibexContainer.m_nChannel] = ouFlexChannel;
                //m_ouClusterConfig[LIN].m_nChannelsConfigured++;
            }
            else
            {
                //TODO::
            }
        }
    }
}

void CMainFrame::vVlaidateAndLoadFibexConfig(sFibexConfigContainer& ouFibexContainer)
{
    if ( m_pouMsgSigFLEXRAY != NULL && ouFibexContainer.m_nChannel >= 0 && ouFibexContainer.m_nChannel < CHANNEL_ALLOWED )
    {
        bool bValid = false;
        CHANNEL_CONFIG ouFlexChannel;
        std::list<Cluster> ouClusterList;
        if ( S_OK == m_pouMsgSigFLEXRAY->hLoadFibexDBFile(ouFibexContainer.m_strDbPath.c_str(), ouClusterList) )
        {
            list<Cluster>::iterator itrCluster;
            for ( itrCluster = ouClusterList.begin(); itrCluster != ouClusterList.end(); itrCluster++ )
            {
                if ( itrCluster->m_strName == ouFibexContainer.m_strClusterId )
                {
                    ouFlexChannel.m_strDataBasePath = ouFibexContainer.m_strDbPath;
                    ouFlexChannel.m_strSlectedEculist = ouFibexContainer.m_strECUList;
                    ouFlexChannel.m_ouClusterInfo = *itrCluster;
                    ouFlexChannel.m_ouFlexRayParams.m_nKetSlot = ouFibexContainer.m_nKeySlot;
                    ouFlexChannel.m_ouFlexRayParams.m_nSecondKeySlot = ouFibexContainer.m_nSecondKeySlot;
                    bValid = true;
                    break;
                }
            }
            if ( true == bValid )
            {
                m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[ouFibexContainer.m_nChannel] = ouFlexChannel;
                m_ouClusterConfig[FLEXRAY].m_nChannelsConfigured++;
            }
            else
            {
                //TODO::
            }
        }
    }
}
void CMainFrame::LoadControllerConfigData(SCONTROLLER_DETAILS& sController, xmlNodePtr& pNodePtr)
{
    sController.vInitialize(TRUE);
    if ( pNodePtr == NULL )
    {
        return;
    }

    pNodePtr = pNodePtr->xmlChildrenNode;
    string strVar;
    UINT unTemp = 0;
    float fBaudRate = 0.00;
    //sController.LoadControllerConfigData(pNodePtr);
    while(NULL != pNodePtr)
    {
        if (xmlUtils::GetDataFrmNode(pNodePtr,"BaudRate",strVar))
        {
            fBaudRate = atof(strVar.c_str());
            if( fBaudRate != 0 && fBaudRate <= 1000 )
            {
                fBaudRate *= 1000;
                std::stringstream ss;
                ss << (UINT)fBaudRate;
                sController.m_omStrBaudrate  =  ss.str();
            }
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"CNF1",strVar))
        {
            unTemp = atoi(strVar.c_str());
            if (unTemp > 0 )
            {
                sController.m_omStrCNF1  =  strVar;
            }
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"CNF2",strVar))
        {
            unTemp = atoi(strVar.c_str());
            if (unTemp > 0 )
            {
                sController.m_omStrCNF2  =  strVar;
            }
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"CNF3",strVar))
        {
            unTemp = atoi(strVar.c_str());
            if (unTemp > 0 )
            {
                sController.m_omStrCNF3  =  strVar;
            }
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"BTR0",strVar))
        {
            sController.m_omStrBTR0 = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"BTR1",strVar))
        {
            sController.m_omStrBTR1 = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"Clock",strVar))
        {
            sController.m_omStrClock = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"SamplePerc",strVar))
        {
            sController.m_omStrSamplePercentage = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"Sampling",strVar))
        {
            sController.m_omStrSampling = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"Warning_Limit",strVar))
        {
            sController.m_omStrWarningLimit = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"Propagation_Delay",strVar))
        {
            sController.m_omStrPropagationDelay = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"SJW",strVar))
        {
            sController.m_omStrSjw = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccCodeByte1_0",strVar))
        {
            sController.m_omStrAccCodeByte1[0] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccCodeByte1_1",strVar))
        {
            sController.m_omStrAccCodeByte1[1] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccCodeByte2_0",strVar))
        {
            sController.m_omStrAccCodeByte2[0] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccCodeByte2_1",strVar))
        {
            sController.m_omStrAccCodeByte2[1] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccCodeByte3_0",strVar))
        {
            sController.m_omStrAccCodeByte3[0] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccCodeByte3_1",strVar))
        {
            sController.m_omStrAccCodeByte3[1] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccCodeByte4_0",strVar))
        {
            sController.m_omStrAccCodeByte4[0] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccCodeByte4_1",strVar))
        {
            sController.m_omStrAccCodeByte4[1] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccMaskByte1_0",strVar))
        {
            sController.m_omStrAccMaskByte1[0] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccMaskByte1_1",strVar))
        {
            sController.m_omStrAccMaskByte1[1] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccMaskByte2_0",strVar))
        {
            sController.m_omStrAccMaskByte2[0] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccMaskByte2_1",strVar))
        {
            sController.m_omStrAccMaskByte2[1] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccMaskByte3_0",strVar))
        {
            sController.m_omStrAccMaskByte3[0] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccMaskByte3_1",strVar))
        {
            sController.m_omStrAccMaskByte3[1] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccMaskByte4_0",strVar))
        {
            sController.m_omStrAccMaskByte4[0] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccMaskByte4_1",strVar))
        {
            sController.m_omStrAccMaskByte4[1] = strVar.c_str();
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"ItemUnderFocus",strVar))
        {
            sController.m_nItemUnderFocus = atoi(strVar.c_str());
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"BTR0BTR1",strVar))
        {
            sController.m_nBTR0BTR1 = atoi(strVar.c_str());
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"AccFilterMode",strVar))
        {
            sController.m_bAccFilterMode = atoi(strVar.c_str());
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"ControllerMode",strVar))
        {
            sController.m_ucControllerMode = atoi(strVar.c_str());
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"SelfReception",strVar))
        {
            sController.m_bSelfReception = atoi(strVar.c_str());
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"HWFilterType_0",strVar))
        {
            sController.m_enmHWFilterType[0] = (eHW_FILTER_TYPES)atoi(strVar.c_str());
        }

        if (xmlUtils::GetDataFrmNode(pNodePtr,"HWFilterType_1",strVar))
        {
            sController.m_enmHWFilterType[1] = (eHW_FILTER_TYPES)atoi(strVar.c_str());
        }
        pNodePtr = pNodePtr->next;
    }
}
INT CMainFrame::nGetControllerID(string strDriverName)
{
    INT nDriverID = -1;
    if(strDriverName == "ETAS BOA")
    {
        nDriverID = DRIVER_CAN_ETAS_BOA;
    }
    else if (strDriverName == "Vector XL" )
    {
        nDriverID = DRIVER_CAN_VECTOR_XL;
    }
    else if(strDriverName == "ETAS ES581")
    {
        nDriverID = DRIVER_CAN_ETAS_ES581;
    }
    else if(strDriverName == "IntrepidCS neoVI")
    {
        nDriverID = DRIVER_CAN_ICS_NEOVI;
    }
    else if(strDriverName == "IXXAT VCI")
    {
        nDriverID = DRIVER_CAN_IXXAT;
    }
    else if(strDriverName == "Kvaser CAN")
    {
        nDriverID = DRIVER_CAN_KVASER_CAN;
    }
    else if(strDriverName == "MHS Tiny-CAN")
    {
        nDriverID = DRIVER_CAN_MHS;
    }
    else if(strDriverName == "PEAK USB")
    {
        nDriverID = DRIVER_CAN_PEAK_USB;
    }
    else
    {
        nDriverID = DRIVER_CAN_STUB;
    }
    return nDriverID;
}
//~MVN

CString CMainFrame::vGetControllerName(UINT nDriverId)
{
    for (int i = 0 ; i < m_nDILCount ; i++)
    {
        if ( m_dwDriverId == m_ouList[i].m_dwDriverID )
        {
            if ( m_asControllerDetails->m_omStrBaudrate== "" )
            {
                m_asControllerDetails->m_omStrBaudrate = "500";
            }
            // PTV
            // Added Shortcut key for the hardwared device, Removing '&'
            //venkat
            CString strDriverName = m_ouList[i].m_acName.c_str();
            strDriverName.Replace("&", "");

            return strDriverName;
        }
    }
}

void CMainFrame::vSetFileStorageInfo(CString oCfgFilename)
{
    USES_CONVERSION;
    DATASTORAGEINFO stempDataInfo;
    FILESTORAGEINFO FileStoreInfo;
    strcpy_s(FileStoreInfo.m_FilePath, _MAX_PATH, T2A(oCfgFilename.GetBuffer(MAX_PATH)));
    stempDataInfo.FSInfo = &FileStoreInfo;
    stempDataInfo.m_Datastore = FILEMODE;
    CConfigData::ouGetConfigDetailsObject().SetConfigDatastorage(&stempDataInfo);
    CConfigData::ouGetConfigDetailsObject().vSetCurrProjName(_(DEFAULT_PROJECT_NAME));
    vPushConfigFilenameDown(oCfgFilename);
}
void CMainFrame::vSetCurrProjInfo(float fAppVersion)
{
    PROJECTDATA ProjData;
    ProjData.m_dwAppUniqueId = BUSMASTER_UNIQUE_ID;
    ProjData.m_fAppVersion = fAppVersion;
    ProjData.m_Language = _("English");
    ProjData.m_ProjectName = _(DEFAULT_PROJECT_NAME);
    GetLocalTime(&ProjData.m_ProjSysTime );
    CConfigData::ouGetConfigDetailsObject().bSetCurrProjInfo(&ProjData);
}

INT CMainFrame::SaveConfiguration(void)
{
    INT nReturn = defCONFIG_FILE_SUCCESS;
    // Save Node Simulation changes
    GetICANNodeSim()->NS_SaveSimulationSystem();
    GetIJ1939NodeSim()->NS_SaveSimulationSystem();

    vSetCurrProjInfo((FLOAT)BUSMASTER_APPN_VERSION_LATEST);

    vSaveXMLConfiguration();
    //for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
    //{
    //    BYTE* pbyConfigData = NULL;
    //    UINT nSize = 0;
    //    vGetCurrentSessionData(eSecId, pbyConfigData, nSize);

    //    if (pbyConfigData != NULL)
    //    {
    //        CConfigData::ouGetConfigDetailsObject().bSetData((void*)pbyConfigData, nSize, SectionName[eSecId]);
    //All done now release the memory
    //        delete[] pbyConfigData;
    //        pbyConfigData = NULL;
    //    }

    //    eSecId = static_cast<eSECTION_ID>(eSecId + 1);
    //}

    //CConfigData::ouGetConfigDetailsObject().vSaveConfigFile();

    return nReturn;
}




void CMainFrame::vClearDbInfo(ETYPE_BUS eBus)
{
    switch (eBus)
    {
        case CAN:
        {
            CFlags* pFlags = theApp.pouGetFlagsPtr();
            BOOL bDatabaseOpen = FALSE;
            if( pFlags != NULL)
            {
                bDatabaseOpen = pFlags->nGetFlagStatus(DBOPEN) ;
            }
            if(bDatabaseOpen == TRUE)
            {
                OnCloseDatabase();
            }
            theApp.m_pouMsgSignal->bDeAllocateMemory(STR_EMPTY);
            CStringArray omDatabase;
            omDatabase.RemoveAll();
            theApp.m_pouMsgSignal->vSetDataBaseNames(&omDatabase);
        }
        break;
        case J1939:
        {
            if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
            {
                OnJ1939DBClose();
            }
            if (m_pouMsgSigJ1939 != NULL)
            {
                m_pouMsgSigJ1939->bDeAllocateMemory(STR_EMPTY);
                CStringArray omDatabase;
                omDatabase.RemoveAll();
                m_pouMsgSigJ1939->vSetDataBaseNames(&omDatabase);
            }
        }
        break;
        case BUS_TOTAL:
        {
            vClearDbInfo(CAN);
            vClearDbInfo(J1939);
        }
        break;
    }
}

BOOL CMainFrame::CompareFile(CString FirstFile,CString SecFile)
{
    int nSizeF, nSizeS;
    CStdioFile InFile, InFile2;
    CString strTemp;
    BOOL bAllFine = TRUE;
    CStringArray ArrFFile, ArrSFile, ArrDisC;
    //reads First XML file data\

    InFile.Open(FirstFile,CFile::modeRead);
    InFile2.Open(SecFile,CFile::modeRead);
    if( NULL == InFile.m_hFile || NULL == InFile2.m_hFile )
    {
        return FALSE;
    }
    else
    {
        if(InFile.m_pStream == NULL)
        {
            return TRUE;
        }

        while(InFile.ReadString(strTemp))
        {
            strTemp.TrimLeft();
            strTemp.TrimRight();
            ArrFFile.Add(strTemp);
        }
        InFile.Close();


        if(InFile2.m_pStream == NULL)
        {
            return TRUE;
        }

        while(InFile2.ReadString(strTemp))
        {
            strTemp.TrimLeft();
            strTemp.TrimRight();
            ArrSFile.Add(strTemp);
        }
        InFile2.Close();

        //Gets the total number of tags in firstfile
        nSizeF = (int)ArrFFile.GetSize();
        //Gets the total number of tags in secondfile
        nSizeS = (int)ArrSFile.GetSize();

        INT m_nMaxSize = 0;

        if(nSizeF != nSizeS)
        {
            return TRUE;
        }

        int nSizeT;
        if(nSizeF < nSizeS)
        {
            nSizeT = nSizeF;
            m_nMaxSize = nSizeS;
        }
        else
        {
            nSizeT = nSizeS;
            m_nMaxSize = nSizeF;
        }
        for(int ni=0; ni<nSizeT; ni++)
        {
            CString SideF, SideS;
            SideF = ArrFFile.GetAt(ni);
            SideS = ArrSFile.GetAt(ni);
            if(SideF!=SideS)
            {
                strTemp.Format(_T("%d"),ni);
                //Adds unmatching tags in an array
                ArrDisC.Add(strTemp);
                return TRUE;
                break;
            }
        }

        return FALSE;
    }
    /* for(int ni=nSizeT;ni<m_nMaxSize;ni++)
     {
         strTemp.Format(_T("%d"),ni);
         ArrDisC.Add(strTemp);
     }*/
}

BOOL CMainFrame::bIsConfigurationModified(void)
{
    BOOL bResult = TRUE;
    CString oCfgFilename;
    vGetLoadedCfgFileName(oCfgFilename);

    if ( oCfgFilename.IsEmpty() == FALSE)
    {
        INT nRevPos = oCfgFilename.ReverseFind('\\');

        CString strTempPath = "";

        if(nRevPos != -1)
        {
            strTempPath = oCfgFilename.Left(nRevPos);
        }
        char chTempPath[MAX_PATH+1] = {0};
        GetTempPath(MAX_PATH+1, chTempPath);
        strTempPath = chTempPath;
        strTempPath +=  "\\busmaster.tempcfx";
        vSaveXMLConfiguration(strTempPath);
        bResult = CompareFile(strTempPath, oCfgFilename);

        bResult |= GetICANNodeSim()->NS_IsSimSysConfigChanged();
        bResult |= GetIJ1939NodeSim()->NS_IsSimSysConfigChanged();

    }

    return bResult;


    //for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
    //{
    //    BYTE* pbyCurrData   = NULL;
    //    UINT  nCurrSize     = 0;
    //    BYTE* pbyConfigData = NULL;
    //    INT  nCfgSize      = 0;

    //    //Get current data
    //    vGetCurrentSessionData(eSecId, pbyCurrData, nCurrSize);
    //    // Get Config data
    //    CConfigData::ouGetConfigDetailsObject().bGetData((void*&)pbyConfigData, nCfgSize, SectionName[eSecId]);
    //    //comparison part
    //    bResult = !(nCurrSize == (UINT)nCfgSize);
    //    if (!bResult) // If config is still same compare the whole memory
    //    {
    //        bResult = !(memcmp(pbyCurrData, pbyConfigData, nCfgSize) == 0);
    //    }
    //    if (pbyCurrData != NULL)
    //    {
    //        delete[] pbyCurrData;
    //    }
    //    if (pbyConfigData != NULL)
    //    {
    //        delete[] pbyConfigData;
    //    }
    //    if (bResult == TRUE)
    //    {
    //        break;
    //    }
    //    eSecId = static_cast<eSECTION_ID>(eSecId + 1);
    //}
    //return bResult;
}
void CMainFrame::vSetCurrentSessionData(eSECTION_ID eSecId, BYTE* pbyConfigData, UINT nSize)
{
    CString omVerStr("");
    omVerStr.Format(IDS_VERSION);

    switch (eSecId)
    {
        case MAINFRAME_SECTION_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));
                char acName[MAX_PATH] = {_T('\0')};
                COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
                m_omMRU_C_Filename.Format("%s", acName);

                COPY_DATA_2(&m_sToolBarInfo, pbyTemp, sizeof(STOOLBARINFO));
                theApp.pouGetFlagsPtr()->vSetToolbarButtonStatus(&m_sToolBarInfo);

                COPY_DATA_2(&m_sNotificWndPlacement, pbyTemp, sizeof(WINDOWPLACEMENT));
                if (m_podUIThread != NULL)
                {
                    m_podUIThread->vUpdateWndCo_Ords(m_sNotificWndPlacement, TRUE);
                    m_podUIThread->vClearTraceContents();
                }
                if (byVersion == 0x2)//LogOnConnet option is introduced
                {
                    COPY_DATA_2(m_abLogOnConnect, pbyTemp, sizeof(BOOL) * BUS_TOTAL);
                }
            }
            else
            {
                theApp.pouGetFlagsPtr()->vInitializeFlags();
                // Setting Hex mode
                bSetHexDecFlags(TRUE);
                m_sNotificWndPlacement.length = 0;
                m_sNotificWndPlacement.rcNormalPosition.top = -1;
                if (m_podUIThread != NULL)
                {
                    m_podUIThread->vUpdateWndCo_Ords(m_sNotificWndPlacement, TRUE);
                    m_podUIThread->vClearTraceContents();
                }
                //Set default settings
            }
        }
        break;
        case LOG_SECTION_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;
                if (sg_pouFrameProcCAN != NULL)
                {
                    sg_pouFrameProcCAN->FPC_StartEditingSession();
                    sg_pouFrameProcCAN->FPC_SetConfigData(pbyTemp, omVerStr);
                    sg_pouFrameProcCAN->FPC_StopEditingSession(TRUE);
                }
                //Start logging if toolbar status is enabled.
                BOOL bLogON = FALSE;
                CFlags* pFlag = theApp.pouGetFlagsPtr();
                if(pFlag != NULL)
                {
                    bLogON = pFlag->nGetFlagStatus(LOGTOFILE);
                    vStartStopLogging( bLogON );
                }
            }
            else
            {
                sg_pouFrameProcCAN = GetICANLogger();
                sg_pouFrameProcCAN->FPC_StartEditingSession();
                sg_pouFrameProcCAN->FPC_ClearLoggingBlockList();
                sg_pouFrameProcCAN->FPC_StopEditingSession(TRUE);
            }
        }
        break;
        case LOG_SECTION_J1939_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;
                if (GetIJ1939Logger() != NULL)
                {
                    GetIJ1939Logger()->FPJ1_StartEditingSession();
                    GetIJ1939Logger()->FPJ1_SetConfigData(pbyTemp, omVerStr);
                    GetIJ1939Logger()->FPJ1_StopEditingSession(TRUE);
                }
            }
            else
            {
                if (GetIJ1939Logger() != NULL)
                {
                    GetIJ1939Logger()->FPJ1_StartEditingSession();
                    GetIJ1939Logger()->FPJ1_ClearLoggingBlockList();
                    GetIJ1939Logger()->FPJ1_StopEditingSession(TRUE);
                }
            }
        }
        break;
        case LOG_SECTION_LIN_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;
                if (sg_pouFrameProcLIN != NULL)
                {
                    sg_pouFrameProcLIN->FPL_StartEditingSession();
                    sg_pouFrameProcLIN->FPL_SetConfigData(pbyTemp, omVerStr);
                    sg_pouFrameProcLIN->FPL_StopEditingSession(TRUE);
                }
                //Start logging if toolbar status is enabled.
                BOOL bLogON = FALSE;
                CFlags* pFlag = theApp.pouGetFlagsPtr();
                if(pFlag != NULL)
                {
                    bLogON = pFlag->nGetFlagStatus(LOGTOFILE_LIN);
                    vStartStopLogging_LIN( bLogON );
                }
            }
            else
            {
                sg_pouFrameProcLIN = GetILINLogger();
                sg_pouFrameProcLIN->FPL_StartEditingSession();
                sg_pouFrameProcLIN->FPL_ClearLoggingBlockList();
                sg_pouFrameProcLIN->FPL_StopEditingSession(TRUE);
            }
        }
        break;

        case SIMSYS_SECTION_ID:
        {
            if (GetICANNodeSim() != NULL)
            {
                GetICANNodeSim()->NS_SetSimSysConfigData(pbyConfigData, nSize);
            }
        }
        break;
        case SIMSYS_SECTION_J1939_ID:
        {
            if (GetIJ1939NodeSim() != NULL)
            {
                GetIJ1939NodeSim()->NS_SetSimSysConfigData(pbyConfigData, nSize);
            }
        }
        break;
        case SIMSYS_SECTION_LIN_ID:
        {
            if (GetILINNodeSim() != NULL)
            {
                GetILINNodeSim()->NS_SetSimSysConfigData(pbyConfigData, nSize);
            }
        }
        break;
        case REPLAY_SECTION_ID:
        {
            vREP_SetReplayConfigData(pbyConfigData, nSize);
        }
        break;
        case MSGWND_SECTION_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));

                //Msg Attributes
                SMESSAGE_ATTRIB sMsgAttrib;
                sMsgAttrib.m_psMsgAttribDetails = NULL;
                sMsgAttrib.m_usMsgCount = 0;
                COPY_DATA_2(&(sMsgAttrib.m_usMsgCount), pbyTemp, sizeof(UINT));
                PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                {
                    char acName[MAX_PATH] = {_T('\0')};
                    COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
                    pMessageAtt[i].omStrMsgname.Format("%s", acName);

                    COPY_DATA_2(&(pMessageAtt[i].unMsgID), pbyTemp, sizeof(UINT));
                    COPY_DATA_2(&(pMessageAtt[i].sColor), pbyTemp, sizeof(COLORREF));
                }
                sMsgAttrib.m_psMsgAttribDetails = pMessageAtt;
                CMessageAttrib::ouGetHandle(CAN).vSetMessageAttribData(&sMsgAttrib);
                theApp.vPopulateCANIDList();
                //Msg buffer size
                COPY_DATA_2(m_anMsgBuffSize, pbyTemp, sizeof(UINT) * defDISPLAY_CONFIG_PARAM);

                //Msg Filter
                bool bResult = false;
                SFILTERAPPLIED_CAN sMsgWndFilter;
                pbyTemp = sMsgWndFilter.pbSetConfigData(pbyTemp, bResult);

                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                              WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);
                //Msg FormatWnd Details

                if((pbyTemp - pbyConfigData) < (INT)nSize)              //VENKAT
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA,
                                  (LPARAM)pbyTemp);
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eLOAD_DATABASE,
                                  (LPARAM)&(theApp.m_pouMsgSignal));
                }
                //clear msg attributes
                DELETE_ARRAY(sMsgAttrib.m_psMsgAttribDetails);
                sMsgAttrib.m_usMsgCount = 0;
            }
            else
            {
                CMessageAttrib::ouGetHandle(CAN).vSetMessageAttribData(NULL);
                m_anMsgBuffSize[defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
                m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
                m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;

                SFILTERAPPLIED_CAN sMsgWndFilter;
                sMsgWndFilter.vClear();
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                              WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);

                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                              WM_NOTIFICATION_FROM_OTHER,
                              eWINID_MSG_WND_SET_CONFIG_DATA,
                              NULL);
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                              WM_NOTIFICATION_FROM_OTHER,
                              eLOAD_DATABASE,
                              (LPARAM)&(theApp.m_pouMsgSignal));
            }
        }
        break;
        case MSGWND_SECTION_FLEXRAY_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));

                //Msg Attributes
                SMESSAGE_ATTRIB sMsgAttrib;
                sMsgAttrib.m_psMsgAttribDetails = NULL;
                sMsgAttrib.m_usMsgCount = 0;
                COPY_DATA_2(&(sMsgAttrib.m_usMsgCount), pbyTemp, sizeof(UINT));
                PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                {
                    char acName[MAX_PATH] = {_T('\0')};
                    COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
                    pMessageAtt[i].omStrMsgname.Format("%s", acName);

                    COPY_DATA_2(&(pMessageAtt[i].unMsgID), pbyTemp, sizeof(UINT));
                    COPY_DATA_2(&(pMessageAtt[i].sColor), pbyTemp, sizeof(COLORREF));
                }
                sMsgAttrib.m_psMsgAttribDetails = pMessageAtt;
                CMessageAttrib::ouGetHandle(CAN).vSetMessageAttribData(&sMsgAttrib);
                theApp.vPopulateCANIDList();
                //Msg buffer size
                COPY_DATA_2(m_anMsgBuffSize, pbyTemp, sizeof(UINT) * defDISPLAY_CONFIG_PARAM);

                //Msg Filter
                bool bResult = false;
                SFILTERAPPLIED_CAN sMsgWndFilter;
                pbyTemp = sMsgWndFilter.pbSetConfigData(pbyTemp, bResult);

                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                              WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);
                //Msg FormatWnd Details

                if((pbyTemp - pbyConfigData) < (INT)nSize)              //VENKAT
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA,
                                  (LPARAM)pbyTemp);
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eLOAD_DATABASE,
                                  (LPARAM)&(theApp.m_pouMsgSignal));
                }
                //clear msg attributes
                DELETE_ARRAY(sMsgAttrib.m_psMsgAttribDetails);
                sMsgAttrib.m_usMsgCount = 0;
            }
            else
            {
                CMessageAttrib::ouGetHandle(FLEXRAY).vSetMessageAttribData(NULL);
                m_anMsgBuffSize[defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
                m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
                m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;

                /* SFILTERAPPLIED_CAN sMsgWndFilter;
                 sMsgWndFilter.vClear();
                 ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                               WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);*/

                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                              WM_NOTIFICATION_FROM_OTHER,
                              eWINID_MSG_WND_SET_CONFIG_DATA,
                              NULL);
            }
        }
        break;
        case MSGWND_SECTION_LIN_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));

                //Msg Attributes
                SMESSAGE_ATTRIB sMsgAttrib;
                sMsgAttrib.m_psMsgAttribDetails = NULL;
                sMsgAttrib.m_usMsgCount = 0;
                COPY_DATA_2(&(sMsgAttrib.m_usMsgCount), pbyTemp, sizeof(UINT));
                PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                {
                    char acName[MAX_PATH] = {_T('\0')};
                    COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
                    pMessageAtt[i].omStrMsgname.Format("%s", acName);

                    COPY_DATA_2(&(pMessageAtt[i].unMsgID), pbyTemp, sizeof(UINT));
                    COPY_DATA_2(&(pMessageAtt[i].sColor), pbyTemp, sizeof(COLORREF));
                }
                sMsgAttrib.m_psMsgAttribDetails = pMessageAtt;
                CMessageAttrib::ouGetHandle(LIN).vSetMessageAttribData(&sMsgAttrib);
                //theApp.vPopulateLINIDList();
                //Msg buffer size
                COPY_DATA_2(m_anMsgBuffSize, pbyTemp, sizeof(UINT) * defDISPLAY_CONFIG_PARAM);

                //Msg Filter
                bool bResult = false;
                SFILTERAPPLIED_LIN sMsgWndFilter;
                pbyTemp = sMsgWndFilter.pbSetConfigData(pbyTemp, bResult);

                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                              WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);
                //Msg FormatWnd Details

                if((pbyTemp - pbyConfigData) < (INT)nSize)              //VENKAT
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA,
                                  (LPARAM)pbyTemp);
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eLOAD_DATABASE,
                                  (LPARAM)&(theApp.m_pouMsgSignal));
                }
                //clear msg attributes
                DELETE_ARRAY(sMsgAttrib.m_psMsgAttribDetails);
                sMsgAttrib.m_usMsgCount = 0;
            }
            else
            {
                CMessageAttrib::ouGetHandle(LIN).vSetMessageAttribData(NULL);
                m_anMsgBuffSize[defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
                m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
                m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;

                SFILTERAPPLIED_LIN sMsgWndFilter;
                sMsgWndFilter.vClear();
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                              WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, NULL);

                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                              WM_NOTIFICATION_FROM_OTHER,
                              eWINID_MSG_WND_SET_CONFIG_DATA,
                              NULL);
                /*::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                              WM_NOTIFICATION_FROM_OTHER,
                              eLOAD_DATABASE,
                              (LPARAM)&(theApp.m_pouMsgSignal));*/
            }
        }
        break;
        case MSGWND_SECTION_J1939_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));

                //Msg Attributes
                SMESSAGE_ATTRIB sMsgAttrib;
                sMsgAttrib.m_psMsgAttribDetails = NULL;
                sMsgAttrib.m_usMsgCount = 0;
                COPY_DATA_2(&(sMsgAttrib.m_usMsgCount), pbyTemp, sizeof(UINT));

                PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                {
                    char acName[MAX_PATH] = {_T('\0')};
                    COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
                    pMessageAtt[i].omStrMsgname.Format("%s", acName);

                    COPY_DATA_2(&(pMessageAtt[i].unMsgID), pbyTemp, sizeof(UINT));
                    COPY_DATA_2(&(pMessageAtt[i].sColor), pbyTemp, sizeof(COLORREF));
                }
                sMsgAttrib.m_psMsgAttribDetails = pMessageAtt;
                CMessageAttrib::ouGetHandle(J1939).vSetMessageAttribData(&sMsgAttrib);

                vPopulateJ1939PGNList();

                //Msg FormatWnd Details
                if((pbyTemp - pbyConfigData) < (INT)nSize)          //VENKAT
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA,
                                  (LPARAM)pbyTemp);
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eLOAD_DATABASE,
                                  (LPARAM)&(m_pouMsgSigJ1939));
                }
                DELETE_ARRAY(sMsgAttrib.m_psMsgAttribDetails);
                sMsgAttrib.m_usMsgCount = 0;
            }
            else
            {
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                              WM_NOTIFICATION_FROM_OTHER,
                              eWINID_MSG_WND_SET_CONFIG_DATA,
                              NULL);
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                              WM_NOTIFICATION_FROM_OTHER,
                              eLOAD_DATABASE,
                              (LPARAM)&(m_pouMsgSigJ1939));
            }
        }
        break;
        case SIGWATCH_SECTION_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));

                CMainEntryList odMainEntryList;
                UINT nMainCount = 0;
                COPY_DATA_2(&nMainCount,pbyTemp, sizeof (UINT));
                for (UINT i = 0; i < nMainCount; i++)
                {
                    SMAINENTRY sMainEntry;
                    COPY_DATA_2(&(sMainEntry.m_unMainEntryID),pbyTemp, (sizeof (UINT)));
                    char acName[MAX_PATH] = {_T('\0')};
                    COPY_DATA_2(acName, pbyTemp, (sizeof (char) * MAX_PATH));
                    sMainEntry.m_omMainEntryName.Format("%s", acName);
                    UINT nSelCount = 0;
                    COPY_DATA_2(&nSelCount,pbyTemp, sizeof (UINT));

                    for (UINT nSelIndex = 0; nSelIndex < nSelCount; nSelIndex++)
                    {
                        SSUBENTRY sSelEntry;
                        COPY_DATA_2(&(sSelEntry.m_unSubEntryID),pbyTemp, (sizeof (UINT)));
                        COPY_DATA_2(acName, pbyTemp, (sizeof (char) * MAX_PATH));
                        sSelEntry.m_omSubEntryName.Format("%s", acName);
                        sMainEntry.m_odSelEntryList.AddTail(sSelEntry);
                    }
                    odMainEntryList.AddTail(sMainEntry);
                }
                vPopulateSigWatchList(odMainEntryList, m_psSignalWatchList[CAN], theApp.m_pouMsgSignal);
                if (sg_pouSWInterface[CAN] == NULL)
                {
                    if (SW_GetInterface(CAN, (void**)&sg_pouSWInterface[CAN]) == S_OK)
                    {
                        sg_pouSWInterface[CAN]->SW_DoInitialization();
                    }
                }
                m_ouMsgInterpretSW_C.vSetMessageList(m_psSignalWatchList[CAN]);
                sg_pouSWInterface[CAN]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_C);
                sg_pouSWInterface[CAN]->SW_SetConfigData(pbyTemp);
            }
            else
            {
                //Set default settings
                vReleaseSignalWatchListMemory(m_psSignalWatchList[CAN]);
                if (sg_pouSWInterface[CAN] == NULL)
                {
                    if (SW_GetInterface(CAN, (void**)&sg_pouSWInterface[CAN]) == S_OK)
                    {
                        sg_pouSWInterface[CAN]->SW_DoInitialization();
                    }
                }
                m_ouMsgInterpretSW_C.vSetMessageList(m_psSignalWatchList[CAN]);
                sg_pouSWInterface[CAN]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_C);
                sg_pouSWInterface[CAN]->SW_SetConfigData((void*)NULL);
            }
        }
        break;
        case SIGWATCH_SECTION_J1939_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));

                CMainEntryList odMainEntryList;
                UINT nMainCount = 0;
                COPY_DATA_2(&nMainCount,pbyTemp, sizeof (UINT));
                for (UINT i = 0; i < nMainCount; i++)
                {
                    SMAINENTRY sMainEntry;
                    COPY_DATA_2(&(sMainEntry.m_unMainEntryID),pbyTemp, (sizeof (UINT)));
                    char acName[MAX_PATH] = {_T('\0')};
                    COPY_DATA_2(acName, pbyTemp, (sizeof (char) * MAX_PATH));
                    sMainEntry.m_omMainEntryName.Format("%s", acName);
                    UINT nSelCount = 0;
                    COPY_DATA_2(&nSelCount,pbyTemp, sizeof (UINT));

                    for (UINT nSelIndex = 0; nSelIndex < nSelCount; nSelIndex++)
                    {
                        SSUBENTRY sSelEntry;
                        COPY_DATA_2(&(sSelEntry.m_unSubEntryID),pbyTemp, (sizeof (UINT)));
                        COPY_DATA_2(acName, pbyTemp, (sizeof (char) * MAX_PATH));
                        sSelEntry.m_omSubEntryName.Format("%s", acName);
                        sMainEntry.m_odSelEntryList.AddTail(sSelEntry);
                    }
                    odMainEntryList.AddTail(sMainEntry);
                }
                vPopulateSigWatchList(odMainEntryList, m_psSignalWatchList[J1939], m_pouMsgSigJ1939);
                if (sg_pouSWInterface[J1939] == NULL)
                {
                    if (SW_GetInterface(J1939, (void**)&sg_pouSWInterface[J1939]) == S_OK)
                    {
                        sg_pouSWInterface[J1939]->SW_DoInitialization();
                    }
                }
                m_ouMsgInterpretSW_J.vSetJ1939Database(m_psSignalWatchList[J1939]);
                sg_pouSWInterface[J1939]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_J);
                sg_pouSWInterface[J1939]->SW_SetConfigData(pbyTemp);
            }
            else
            {
                //Set default settings
                vReleaseSignalWatchListMemory(m_psSignalWatchList[J1939]);
                if (sg_pouSWInterface[J1939] == NULL)
                {
                    if (SW_GetInterface(J1939, (void**)&sg_pouSWInterface[J1939]) == S_OK)
                    {
                        sg_pouSWInterface[J1939]->SW_DoInitialization();
                    }
                }
                m_ouMsgInterpretSW_J.vSetJ1939Database(m_psSignalWatchList[J1939]);
                sg_pouSWInterface[J1939]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_J);
                sg_pouSWInterface[J1939]->SW_SetConfigData((void*)NULL);
            }
        }
        break;
        case DIL_SECTION_ID:
        {
            BYTE byVersion = 0;
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                UINT unChannelCount = CHANNEL_ALLOWED;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));
                if (byVersion == DIL_CFX_TWO_CHANNEL) // Only two channels supported in version 0x1
                {
                    unChannelCount = 2;
                }
                if(byVersion >= DIL_CFX_STRUCTURE_UPDATE)
                {
                    COPY_DATA_2(&m_dwDriverId, pbyTemp, sizeof(DWORD));
                    COPY_DATA_2(&m_byControllerMode, pbyTemp, sizeof(BYTE));
                    //COPY_DATA_2(m_asControllerDetails, pbyTemp, (sizeof(SCONTROLLER_DETAILS) * unChannelCount));
                    for(int i = 0; i < unChannelCount; i++)
                    {
                        m_asControllerDetails[i].LoadControllerConfigData(pbyTemp);
                    }
                    IntializeDIL();
                    ASSERT(g_pouDIL_CAN_Interface != NULL);
                    //HRESULT hResult =
                    /*g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails,
                                                        sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS);*/
                    g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails,
                            defNO_OF_CHANNELS);
                }

            }
            if ((NULL == pbyConfigData) || ( byVersion < DIL_CFX_CURRENT_VERSION))
            {
                m_dwDriverId = DRIVER_CAN_STUB;
                m_byControllerMode = defMODE_SIMULATE;
                IntializeDIL();
                for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
                {
                    m_asControllerDetails[i].vInitialize(FALSE);
                }

                //HRESULT hResult =
                g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails,
                        sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS);
                //venkat
                /*g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails,
                        sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS);*/
                //Set default settings
            }
        }
        break;
        case GRAPH_SECTION_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));

                if(byVersion == 2)
                {
                    for(int nBUSID=0; nBUSID<AVAILABLE_PROTOCOLS; nBUSID++)
                    {
                        pbyTemp = m_odGraphList[nBUSID].pbySetConfigData(pbyTemp, byVersion);

                        COPY_DATA_2(&m_sGraphWndPlacement[nBUSID], pbyTemp, sizeof(WINDOWPLACEMENT));
                        COPY_DATA_2(&m_sGraphSplitterPos[nBUSID], pbyTemp, sizeof(SGRAPHSPLITTERDATA));
                        m_objSigGrphHandler.SetWindowSplitterPos((SHORT)nBUSID, m_sGraphWndPlacement[nBUSID],
                                m_sGraphSplitterPos[nBUSID]);
                        m_objSigGrphHandler.SetSignalListDetails((SHORT)nBUSID, &m_odGraphList[nBUSID]);
                    }
                }
                else if(byVersion == 1)
                {
                    pbyTemp = m_odGraphList[CAN].pbySetConfigData(pbyTemp, byVersion);
                    COPY_DATA_2(&m_sGraphWndPlacement, pbyTemp, sizeof(WINDOWPLACEMENT));
                    COPY_DATA_2(&m_sGraphSplitterPos, pbyTemp, sizeof(SGRAPHSPLITTERDATA));
                    m_objSigGrphHandler.SetSignalListDetails(CAN, &m_odGraphList[CAN]);
                }
            }
            else
            {
                for(int nBUSID=0; nBUSID<AVAILABLE_PROTOCOLS; nBUSID++)
                {
                    m_odGraphList[nBUSID].m_odGraphParameters.vInitialize();
                    if(m_odGraphList[nBUSID].m_omElementList.GetSize()>0)
                    {
                        m_odGraphList[nBUSID].m_omElementList.RemoveAll();
                    }
                    m_sGraphWndPlacement[nBUSID].showCmd = SW_HIDE;
                    m_sGraphWndPlacement[nBUSID].length = 0;
                    m_sGraphWndPlacement[nBUSID].rcNormalPosition.top = -1;
                    m_sGraphSplitterPos[nBUSID].m_nRootSplitterData[0][0] = -1;
                    m_objSigGrphHandler.SetWindowSplitterPos((SHORT)nBUSID, m_sGraphWndPlacement[nBUSID],
                            m_sGraphSplitterPos[nBUSID]);
                }
            }
            //Arun
            vPostConfigChangeCmdToSigGrphWnds();
        }
        break;
        case TXWND_SECTION_ID:
        {
            if (pbyConfigData != NULL)
            {
                m_objTxHandler.vSetTxWndConfigData(pbyConfigData, nSize);
            }
            else
            {
                m_objTxHandler.vSetTxWndConfigData(NULL);
            }
        }
        break;

        case FILTER_SECTION_ID:
        {
            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                bool bResult = false;
                pbyTemp = m_sFilterAppliedCAN.pbSetConfigData(pbyTemp, bResult);
            }
            else
            {
                m_sFilterAppliedCAN.vClear();
                //Set default settings
            }
        }
        break;
        case DATABASE_SECTION_J1939_ID:
        {
            //Clear all databases
            if (m_pouMsgSigJ1939 == NULL)
            {
                m_pouMsgSigJ1939 = new CMsgSignal(sg_asDbParams[J1939], FALSE);
            }
            vClearDbInfo(J1939);

            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));

                UINT unCount = 0;
                CStringArray omDBNames;

                COPY_DATA_2(&unCount, pbyTemp, sizeof (UINT));
                for (UINT i = 0; i < unCount; i++)
                {
                    char acName[MAX_PATH] = {_T('\0')};
                    COPY_DATA_2(acName, pbyTemp, (sizeof (char) * MAX_PATH));
                    CString omDbName;
                    omDbName.Format("%s", acName);
                    omDBNames.Add(omDbName);
                }
                if (m_pouMsgSigJ1939 != NULL)
                {
                    //m_pouMsgSigJ1939->vSetDataBaseNames(&omDBNames);
                    for (INT i = 0; i < omDBNames.GetSize(); i++)
                    {
                        //No need to check return value. Error message will be displayed
                        // in trace window
                        dLoadJ1939DBFile(omDBNames.GetAt(i), TRUE);
                    }
                    SMSGENTRY* psMsgEntry = NULL;
                    vPopulateMsgEntryFromDB(psMsgEntry, m_pouMsgSigJ1939);
                    if (m_pouTxMsgWndJ1939 != NULL)
                    {
                        m_pouTxMsgWndJ1939->vSetDatabaseInfo(psMsgEntry);
                    }
                    else
                    {
                        m_pouTxMsgWndJ1939->vUpdateDataStore(psMsgEntry);
                    }
                }
            }
        }
        break;
        case DATABASE_SECTION_ID:
        {
            //Clear all databases
            vClearDbInfo(CAN);

            if (pbyConfigData != NULL)
            {
                BYTE* pbyTemp = pbyConfigData;

                BYTE byVersion = 0;
                COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));

                UINT unCount = 0;
                CStringArray omDBNames;
                COPY_DATA_2(&unCount, pbyTemp, sizeof (UINT));
                for (UINT i = 0; i < unCount; i++)
                {
                    char acName[MAX_PATH] = {_T('\0')};
                    COPY_DATA_2(acName, pbyTemp, (sizeof (char) * MAX_PATH));
                    CString omDbName;
                    omDbName.Format("%s", acName);
                    omDBNames.Add(omDbName);
                }

                if (theApp.m_pouMsgSignal != NULL)
                {
                    for (INT i = 0; i < omDBNames.GetSize(); i++)
                    {
                        //No need to check return value. Error message will be displayed
                        // in trace window
                        //venkat:dLoadDataBaseFile will associate the database file if.
                        //it is valid.
                        dLoadDataBaseFile(omDBNames.GetAt(i), TRUE);
                    }
                }
            }
        }
        break;
        case WAVEFORMDATA_SECTION_ID:
        {
            m_objWaveformDataHandler.SetConfigData(pbyConfigData);
        }
        break;
        case BUS_STATISTICS_SECTION_ID:
        {
            if(m_bIsStatWndCreated)
            {
                if (pbyConfigData != NULL)
                {
                    BYTE* pbyTemp = pbyConfigData;
                    m_podBusStatistics->SetConfigData(pbyTemp);
                }
                else
                {
                    CBusStatisticsDlg::vSetDefaultsToStore();
                    m_podBusStatistics->vLoadDataFromStore();
                }
            }
            else
            {
                if (pbyConfigData != NULL)
                {
                    BYTE* pbyTemp = pbyConfigData;
                    CBusStatisticsDlg::vSaveDataToStore(pbyTemp);
                }
                else
                {
                    CBusStatisticsDlg::vSetDefaultsToStore();
                    CBusStatisticsDlg::vSaveDataToStore(NULL);
                }
            }
        }
        break;
        //venkat
        case TEST_SETUP_EDITOR_SECTION_ID:
        {
            m_objTSEditorHandler.vSetConfigurationData(pbyConfigData, nSize);
        }
        break;
        case FLEXRAY_TXWND:
        {
            if (pbyConfigData != NULL)
            {
                m_objFlexTxHandler.vSetTxWndConfigData(pbyConfigData, nSize);
            }
            else
            {
                m_objFlexTxHandler.vSetTxWndConfigData(FLEXRAY, NULL);
            }
        }
        break;
        case LIN_TXWND:
        {
            m_objFlexTxHandler.vSetTxWndConfigData(LIN, NULL);
        }
        case TEST_SUITE_EXECUTOR_SECTION_ID:
        {
            m_objTSExecutorHandler.vSetConfigurationData(pbyConfigData, nSize);
        }
        break;
        case FLEXRAY_CLUSTER_CONFIG:
        {
            /*Unload FLEXRAY DIL */
            m_shFLEXRAYDriverId = DAL_NONE;
            InitializeFLEXRAYDIL();
            m_ouClusterConfig[FLEXRAY].InitialiseConfig();


            for ( int i = 0; i < m_ouClusterConfig[FLEXRAY].m_nChannelsConfigured; i++ )
            {
                m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_ouFlexRayParams.m_nKetSlot = -1;
                m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_ouFlexRayParams.m_nSecondKeySlot = -1;
                m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_strDataBasePath = "";
                //m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_ouClusterInfo.clear();
                m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i].m_strSlectedEculist.clear();
            }
        }
        break;
        case LIN_CLUSTER_CONFIG:
        {
            vInitialiaseLINConfig(1);
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
}

void CMainFrame::vGetCurrentSessionData(eSECTION_ID eSecId, BYTE*& pbyConfigData, UINT& nSize)
{
    switch (eSecId)
    {
    }
}

void CMainFrame::OnSelectFLEXRAYDriver(UINT nID)
{
    m_bFlxDILChanging = TRUE;
    FLEXRAY_DILINFO* psCurrDIL = psGetFLEXRAYDILEntry(nID);

    //TODO::FIBEX Validation is Required
    if ( m_ouClusterConfig[FLEXRAY].m_nChannelsConfigured < 0 ||m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[0].m_strDataBasePath.empty())
    {
        int nSelection = MessageBox("Please Configure the Cluster Parameters", "Invalid Cluster Configuration", MB_OK);
        if ( nSelection == IDOK )
        {
            OnFlexRayDBAssociate();
        }
    }

    if ( m_ouClusterConfig[FLEXRAY].m_nChannelsConfigured < 0 ||m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[0].m_strDataBasePath.empty())
    {
        return;
    }

    if (psCurrDIL != NULL)
    {
        m_shFLEXRAYDriverId =  psCurrDIL->m_dwDriverID;

        HRESULT hResult = InitializeFLEXRAYDIL();

        if ( hResult != S_OK )
        {

            /* Resetting DIL to NONE */
            if ( g_pouDIL_FLEXRAY_Interface )
            {
                m_shFLEXRAYDriverId = DAL_NONE;
                g_pouDIL_FLEXRAY_Interface->DILF_SelectDriver(m_shFLEXRAYDriverId, m_hWnd, &m_ouWrapperLogger);
            }
        }
    }
    m_bFlxDILChanging = FALSE;
}

void CMainFrame::OnUpdateSelectFLEXRAYDriver(CCmdUI* pCmdUI)
{
    BOOL bSelected = FALSE;
    // Search for the associated item in the DIL list
    FLEXRAY_DILINFO* psCurrDIL = psGetFLEXRAYDILEntry(pCmdUI->m_nID);
    if (psCurrDIL != NULL)
    {
        if (g_pouDIL_FLEXRAY_Interface != NULL)
        {
            bSelected = (psCurrDIL->m_dwDriverID == g_pouDIL_FLEXRAY_Interface->DILF_GetSelectedDriver());
        }
    }
    CFlags* pFlag = theApp.pouGetFlagsPtr();
    if (pFlag != NULL)
    {
        BOOL bConnected = pFlag->nGetFlagStatus(FLEX_CONNECTED);
        if (bConnected == FALSE)
        {
            pCmdUI->Enable(!bSelected);
        }
        else
        {
            pCmdUI->Enable(FALSE);
        }
    }

    if(psCurrDIL->m_dwDriverID != FLEXRAY_DAL_NONE)
    {
        pCmdUI->SetCheck(bSelected);
    }
}

void CMainFrame::OnSelectDriver(UINT nID)
{
    DILINFO* psCurrDIL = psGetDILEntry(nID);
    // Above lines have to be changed.

    if (psCurrDIL != NULL)
    {
        m_dwDriverId =  psCurrDIL->m_dwDriverID;

        HRESULT hResult = IntializeDIL();

        /* Check if user intentionally not cancelled hardware selection */
        if ( hResult != HW_INTERFACE_NO_SEL )
        {
            //Retain default values for all channels
            for (int i = 0; i < defNO_OF_CHANNELS; i++)
            {
                m_asControllerDetails[i].vInitialize(FALSE);
            }
        }
        /*Update hardware info in status bar*/
        vUpdateHWStatusInfo();

        if(m_bNoHardwareFound == true)
        {
            m_dwDriverId = 0;  //resetting to simulation
        }
    }
}

void CMainFrame::OnSelectLINDriver(UINT nID)
{
    //DILINFO* psCurrDIL = psGetDILLINEntry(nID);
    // Above lines have to be changed.


    /*if (psCurrDIL != NULL)
    {
        m_dwLINDriverId =  psCurrDIL->m_dwDriverID;

        HRESULT hResult = IntializeDILL();
    }*/

    //m_bFlxDILChanging = TRUE;
    DILINFO* psCurrDIL = psGetDILLINEntry(nID);

    //TODO::FIBEX Validation is Required
    if ( m_ouClusterConfig[LIN].m_nChannelsConfigured < 0 ||( m_ouClusterConfig[LIN].m_ouFlexChannelConfig[0].m_strDataBasePath.empty() && m_ouClusterConfig[LIN].m_ouFlexChannelConfig[0].m_ouLinParams.m_bOverWriteSettings == false ) )
    {
        int nSelection = MessageBox("Please Configure the Cluster Parameters", "Invalid Cluster Configuration", MB_OK);
        if ( nSelection == IDOK )
        {
            OnLinClusterConfig();
        }
    }

    if ( m_ouClusterConfig[LIN].m_nChannelsConfigured < 0 ||( m_ouClusterConfig[LIN].m_ouFlexChannelConfig[0].m_strDataBasePath.empty() && m_ouClusterConfig[LIN].m_ouFlexChannelConfig[0].m_ouLinParams.m_bOverWriteSettings == false ) )
    {
        return;
    }

    if (psCurrDIL != NULL)
    {
        m_shLINDriverId =  psCurrDIL->m_dwDriverID;

        HRESULT hResult = IntializeDILL();

        if ( hResult != S_OK )
        {

            /* Resetting DIL to NONE */
            if ( g_pouDIL_LIN_Interface )
            {
                m_shLINDriverId = DAL_NONE;
                g_pouDIL_LIN_Interface->DILL_SelectDriver(m_shLINDriverId, m_hWnd, &m_ouWrapperLogger);
            }
        }
    }
    // m_bFlxDILChanging = FALSE;
}
void CMainFrame::OnUpdateSelectDriver(CCmdUI* pCmdUI)
{
    BOOL bSelected = FALSE;
    // Search for the associated item in the DIL list
    DILINFO* psCurrDIL = psGetDILEntry(pCmdUI->m_nID);
    if (psCurrDIL != NULL)
    {
        if (g_pouDIL_CAN_Interface != NULL)
        {
            bSelected = (psCurrDIL->m_dwDriverID == g_pouDIL_CAN_Interface->DILC_GetSelectedDriver());
        }
    }
    CFlags* pFlag = theApp.pouGetFlagsPtr();
    if (pFlag != NULL)
    {
        BOOL bConnected = pFlag->nGetFlagStatus(CONNECTED);
        if (bConnected == FALSE)
        {
            pCmdUI->Enable(!bSelected);
        }
        else
        {
            pCmdUI->Enable(FALSE);
        }
    }

    pCmdUI->SetCheck(bSelected);
}

void CMainFrame::OnUpdateSelectLINDriver(CCmdUI* pCmdUI)
{
    BOOL bSelected = FALSE;
    // Search for the associated item in the DIL list
    DILINFO* psCurrDIL = psGetDILLINEntry(pCmdUI->m_nID);
    if (psCurrDIL != NULL)
    {
        if (g_pouDIL_LIN_Interface != NULL)
        {
            bSelected = (psCurrDIL->m_dwDriverID == g_pouDIL_LIN_Interface->DILL_GetSelectedDriver());
        }
    }
    CFlags* pFlag = theApp.pouGetFlagsPtr();
    if (pFlag != NULL)
    {
        BOOL bConnected = pFlag->nGetFlagStatus(LIN_CONNECTED);
        if (bConnected == FALSE)
        {
            pCmdUI->Enable(!bSelected);
        }
        else
        {
            pCmdUI->Enable(FALSE);
        }
    }

    if(psCurrDIL->m_dwDriverID != DAL_LIN_NONE)
    {
        pCmdUI->SetCheck(bSelected);
    }

}

BOOL CMainFrame::bUpdatePopupMenuFLEXRAYDIL(void)
{
    USES_CONVERSION;

    BOOL bResult = TRUE;

    if (m_pFlxDILSubMenu == NULL)
    {
        /* Create a new popup Menu */
        if (bResult == ((m_pFlxDILSubMenu = new CMenu()) != NULL))
        {
            if ((bResult = m_pFlxDILSubMenu->CreatePopupMenu()) == TRUE)
            {
                // Add the FlexRay DIL list
                for (int i = 0; (i < m_nFlexRayDILCount) && bResult; i++)
                {
                    bResult = m_pFlxDILSubMenu->AppendMenu(MF_STRING,
                                                           IDC_SELECT_FLEX_DRIVER + i, _((char*)m_ouFlexRayList[i].m_acName.c_str()));
                    if (bResult == TRUE)
                    {
                        m_ouFlexRayList[i].m_ResourceID = IDC_SELECT_FLEX_DRIVER + i;
                    }
                }
            }
            else
            {
                theApp.bWriteIntoTraceWnd(_("bUpdatePopupMenuFLEXRAYDIL for driver selection failed!"));
            }
        }
        else
        {
            theApp.bWriteIntoTraceWnd(_("new CMenu() for FlexRay failed!"));
            ASSERT(FALSE);
        }
    }

    if (bResult == TRUE)
    {
        CMenu* pConfigMenu = GetSubMenu(_T(_("Flex&Ray"))); // Get the Menu "F&LEXRAY"
        ASSERT(pConfigMenu != NULL);
        if (pConfigMenu == NULL)
        {
            theApp.bWriteIntoTraceWnd(_("GetSubMenu(\"Flex&Ray\") failed"));
        }
        if(pConfigMenu != NULL)
        {
            pConfigMenu->InsertMenu(1, MF_BYPOSITION | MF_POPUP, (UINT_PTR) (m_pFlxDILSubMenu->m_hMenu), _T(_("Dri&ver Selection")));
        }
    }
    if (bResult == FALSE)
    {
        theApp.bWriteIntoTraceWnd(_("Could not create DIL menu items"));
    }

    return bResult;
}

BOOL CMainFrame::bUpdatePopupMenuDIL(void)
{
    USES_CONVERSION;

    BOOL bResult = TRUE;

    if (m_pDILSubMenu == NULL)
    {
        /* Create a new popup Menu */
        if (bResult == ((m_pDILSubMenu = new CMenu()) != NULL))     //venkat
        {
            if ((bResult = m_pDILSubMenu->CreatePopupMenu()) == TRUE)
            {
                // Add the DIL list
                for (int i = 0; (i < m_nDILCount) && bResult; i++)
                {
                    bResult = m_pDILSubMenu->AppendMenu(MF_STRING,
                                                        IDC_SELECT_DRIVER + i, _((char*)m_ouList[i].m_acName.c_str()));
                    if (bResult == TRUE)
                    {
                        m_ouList[i].m_ResourceID = IDC_SELECT_DRIVER + i;
                    }
                }
            }
            else
            {
                theApp.bWriteIntoTraceWnd(_("CreatePopupMenu for driver selection failed"));
            }
        }
        else
        {
            theApp.bWriteIntoTraceWnd(_("new CMenu() failed"));
            ASSERT(FALSE);
        }
    }

    if (bResult == TRUE)
    {
        CMenu* pConfigMenu = GetSubMenu(_T(_("&CAN"))); // Get the Menu "&Hardware"
        ASSERT(pConfigMenu != NULL);
        if (pConfigMenu == NULL)
        {
            theApp.bWriteIntoTraceWnd(_("GetSubMenu(\"&CAN\") failed"));
        }
        /*  PTV[1.6.4] */
        // Added shortcut key

        if(pConfigMenu != NULL)
        {
            pConfigMenu->InsertMenu(1, MF_BYPOSITION | MF_POPUP, (UINT_PTR) (m_pDILSubMenu->m_hMenu), _T(_("Dri&ver Selection")));
        }
    }
    if (bResult == FALSE)
    {
        theApp.bWriteIntoTraceWnd(_("Could not create DIL menu items"));
    }

    return bResult;
}
FLEXRAY_DILINFO* CMainFrame::psGetFLEXRAYDILEntry(UINT unKeyID, BOOL bKeyMenuItem)
{
    FLEXRAY_DILINFO* psResult = NULL;
    for (int i = 0; i < m_nFlexRayDILCount; i++)
    {
        if (bKeyMenuItem == TRUE)
        {
            if (m_ouFlexRayList[i].m_ResourceID == unKeyID)
            {
                psResult = &(m_ouFlexRayList[i]);
                break;
            }
        }
        else
        {
            if (m_ouFlexRayList[i].m_dwDriverID == unKeyID)
            {
                psResult = &(m_ouFlexRayList[i]);
                break;
            }
        }
    }
    return psResult;
}

BOOL CMainFrame::bUpdatePopupMenuDILL(void)
{

    USES_CONVERSION;

    BOOL bResult = TRUE;

    if (m_pDILSubMenuLin == NULL)
    {
        /* Create a new popup Menu */
        if (bResult == ((m_pDILSubMenuLin = new CMenu()) != NULL))     //venkat
        {
            if ((bResult = m_pDILSubMenuLin->CreatePopupMenu()) == TRUE)
            {
                // Add the DIL list
                for (int i = 0; (i < m_nDILCountLin) && bResult; i++)
                {
                    bResult = m_pDILSubMenuLin->AppendMenu(MF_STRING,
                                                           IDC_SELECT_LIN_DRIVER + i, _((char*)m_ouListLin[i].m_acName.c_str()));
                    if (bResult == TRUE)
                    {
                        m_ouListLin[i].m_ResourceID = IDC_SELECT_LIN_DRIVER + i;
                        if (i == 0) //Disable the Deactivate menu item by default
                        {
                            m_pDILSubMenuLin->EnableMenuItem(IDC_SELECT_LIN_DRIVER + i, false);
                        }
                    }
                }
            }
            else
            {
                theApp.bWriteIntoTraceWnd(_("CreatePopupMenu for driver selection failed"));
            }
        }
        else
        {
            theApp.bWriteIntoTraceWnd(_("new CMenu() failed"));
            ASSERT(FALSE);
        }
    }

    if (bResult == TRUE)
    {
        CMenu* pConfigMenu = GetSubMenu(_T(_("&LIN"))); // Get the Menu "&Hardware"
        ASSERT(pConfigMenu != NULL);
        if (pConfigMenu == NULL)
        {
            theApp.bWriteIntoTraceWnd(_("GetSubMenu(\"&LIN\") failed"));
        }
        /*  PTV[1.6.4] */
        // Added shortcut key

        if(pConfigMenu != NULL)
        {
            pConfigMenu->InsertMenu(2, MF_BYPOSITION | MF_POPUP, (UINT_PTR) (m_pDILSubMenuLin->m_hMenu), _T(_("&Driver Selection")));
        }
    }
    if (bResult == FALSE)
    {
        theApp.bWriteIntoTraceWnd(_("Could not create DIL menu items"));
    }

    return bResult;
}

DILINFO* CMainFrame::psGetDILEntry(UINT unKeyID, BOOL bKeyMenuItem)
{
    DILINFO* psResult = NULL;
    for (int i = 0; i < m_nDILCount; i++)
    {
        if (bKeyMenuItem == TRUE)
        {
            if (m_ouList[i].m_ResourceID == unKeyID)
            {
                psResult = &(m_ouList[i]);
                break;
            }
        }
        else
        {
            if (m_ouList[i].m_dwDriverID == unKeyID)
            {
                psResult = &(m_ouList[i]);
                break;
            }
        }
    }
    return psResult;
}
DILINFO* CMainFrame::psGetDILLINEntry(UINT unKeyID, BOOL bKeyMenuItem)
{
    DILINFO* psResult = NULL;
    for (int i = 0; i < m_nDILCountLin; i++)
    {
        if (bKeyMenuItem == TRUE)
        {
            if (m_ouListLin[i].m_ResourceID == unKeyID)
            {
                psResult = &(m_ouListLin[i]);
                break;
            }
        }
        else
        {
            if (m_ouListLin[i].m_dwDriverID == unKeyID)
            {
                psResult = &(m_ouListLin[i]);
                break;
            }
        }
    }
    return psResult;
}

void CMainFrame::OnConfigureWaveformMessages(void)
{
    CMsgSignal* pomDatabase = NULL;
    pomDatabase = theApp.m_pouMsgSignal;
    if( pomDatabase != NULL )
    {
        if( pomDatabase->unGetNumerOfMessages() > 0)
        {
            vUpdateMainEntryListInWaveDataHandler();

            LONG lParam = 0;
            UINT nHardware = 0;
            if(g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
            {
                nHardware = (UINT)lParam;
            }

            CWaveformSelectionDlg dlgWaveForm(this, &m_objWaveformDataHandler, nHardware);
            dlgWaveForm.DoModal();
            //Added by Arun to eliminate the RADAR Defect Seq. No 106. (28/06/2011)
            //If in Connected state, update the enable state of signal transmission.
            CFlags* pouFlags = theApp.pouGetFlagsPtr();
            BOOL bConnected = pouFlags->nGetFlagStatus(CONNECTED);
            if( bConnected == TRUE )
            {
                m_ouWaveTransmitter.bUpdateBlock(bConnected);
            }
        }
        else
        {
            // Database is not imported!!
            AfxMessageBox( _(defSTR_EMPTY_ACTIVE_DATABASE) );
        }
    }
}

void CMainFrame::vUpdateMainEntryListInWaveDataHandler()
{
    m_odResultingList.RemoveAll();
    vPopulateMainEntryList(&m_odResultingList, NULL, theApp.m_pouMsgSignal);
    m_objWaveformDataHandler.vSetCompleteMsgList(&m_odResultingList);

    //If in Connected state, update the enable state of signal transmission.
    CFlags* pouFlags = theApp.pouGetFlagsPtr();
    BOOL bConnected = pouFlags->nGetFlagStatus(CONNECTED);
    if( bConnected == TRUE )
    {
        m_ouWaveTransmitter.bUpdateBlock(bConnected);
    }
}

/*******************************************************************************
  Function Name  : vClearSignalInfoList
  Input(s)       : -
  Output         : -
  Functionality  : Clears signal list data in m_objWaveformDataHandler object
  Member of      : CMainFrame
  Author(s)      : Arunkumar K
  Date Created   : 01-02-2011
  Modifications  :
*******************************************************************************/
void CMainFrame::vClearSignalInfoList(void)
{
    m_objWaveformDataHandler.vClearSignalInfoList();
}

/*******************************************************************************
  Function Name  : OnStartSignalTransmission
  Input(s)       : pCmdUI
  Output         : -
  Functionality  : Updates the check status of Start Signal Transmission menu item.
  Member of      : CMainFrame
  Author(s)      : Arunkumar K
  Date Created   : 27-08-2010
  Modifications  :
*******************************************************************************/
void CMainFrame::OnStartSignalTransmission(void)
{
    if (m_ouWaveTransmitter.bIsWaveformTxON())
    {
        // Waveform transmission is ON. Hence stop the transmission
        m_ouWaveTransmitter.vStopSignalTransmission();
        // update flag.
        theApp.pouGetFlagsPtr()->vSetFlagStatus( SEND_SIGNAL_MSG, FALSE );
        // PTV [1.6.4]
        // Disabling Data logging Flag
        if (NULL != sg_pouFrameProcCAN)
        {
            sg_pouFrameProcCAN->FPC_DisableDataLogFlag();
        }
    }
    else
    {
        // Now start sending waveform messages, update flag
        theApp.pouGetFlagsPtr()->vSetFlagStatus( SEND_SIGNAL_MSG, TRUE );
        // Waveform transmission is OFF. Hence start the transmission
        m_ouWaveTransmitter.vStartSignalTransmission(g_dwClientID);
    }
}

/*******************************************************************************
  Function Name  : OnUpdateStartSignalTransmission
  Input(s)       : pCmdUI
  Output         : -
  Functionality  : Start signal transmission menu item updation routine.
  Member of      : CMainFrame
  Author(s)      : Arunkumar K
  Date Created   : 27-08-2010
  Modifications  : Simplified updation code - Ratnadip Choudhury, 30/08/2010
*******************************************************************************/
void CMainFrame::OnUpdateStartSignalTransmission(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_ouWaveTransmitter.bIsBlockEnabled());
    pCmdUI->SetCheck(m_ouWaveTransmitter.bIsWaveformTxON());
}

LRESULT CMainFrame::OnMessageFromUserDll(WPARAM wParam, LPARAM lParam)
{
    USES_CONVERSION;
    LRESULT hRetVal = S_OK;
    CFlags* pouFlags = theApp.pouGetFlagsPtr();
    switch (wParam)
    {
        case LOG_ENABLE_DISABLE:
        {
            if (pouFlags != NULL)
            {
                BOOL bEnable = pouFlags->nGetFlagStatus(LOGTOFILE);
                BOOL bRequest = (BOOL)lParam;
                if (bRequest != bEnable)
                {
                    OnLogEnable();
                }
            }
        }
        break;
        case DIS_CONNECT:
        {
            if (pouFlags != NULL)
            {
                BOOL bConnect = pouFlags->nGetFlagStatus(CONNECTED);
                BOOL bRequest = (BOOL)lParam;
                if (bRequest != bConnect)
                {
                    OnFileConnect();
                }
            }
        }
        break;
        case LIN_DIS_CONNECT:
        {
            if (pouFlags != NULL)
            {
                BOOL bConnect = pouFlags->nGetFlagStatus(LIN_CONNECTED);
                BOOL bRequest = (BOOL)lParam;
                if (bRequest != bConnect)
                {
                    OnLINConnect();
                }
            }
        }
        break;
        case WRITE_TO_LOGFILE:
        {
            char* pacText = (char*)lParam;
            if( NULL != pacText )
            {
                CString omText;
                omText.Format("%s", A2T(pacText));
                if (sg_pouFrameProcCAN != NULL)
                {
                    sg_pouFrameProcCAN->FPC_LogString(omText);
                }
            }
            else
            {
                hRetVal = S_FALSE;
            }
        }
        break;
        case WRITE_TO_LOGFILE_J1939:
        {
            char* pacText = (char*)lParam;
            if( NULL != pacText )
            {
                CString omText;
                omText.Format("%s", A2T(pacText));
                if (GetIJ1939Logger() != NULL)
                {
                    GetIJ1939Logger()->FPJ1_LogString(omText);
                }
            }
            else
            {
                hRetVal = S_FALSE;
            }
        }
        break;
        default:
        {
        }
        break;
    }
    return hRetVal;
}
//initializes Busstatistics for can where user selects different driver.
void CMainFrame::vInitializeBusStatCAN(void)
{
    GetICANBusStat()->BSC_DoInitialization();
    for (int i = 0; i < defNO_OF_CHANNELS; i++)
    {
#ifdef BOA_FD_VERSION
        GetICANBusStat()->BSC_SetBaudRate(i, m_asControllerDetails[i].m_unDataBitRate);
#else
        GetICANBusStat()->BSC_SetBaudRate(i, _tstof(m_asControllerDetails[i].m_omStrBaudrate.c_str()));
#endif
    }
}


//initializes FlexRay Busstatistics for can where user selects different driver.
void CMainFrame::vInitializeBusStatFlexRay(void)
{
    GetIFlexRayBusStat()->BSC_DoInitialization();
    /*for (int i = 0; i < defNO_OF_CHANNELS; i++)
    {
    #ifdef BOA_FD_VERSION
        GetICANBusStat()->BSC_SetBaudRate(i, m_asControllerDetails[i].m_unDataBitRate);
    #else
        GetICANBusStat()->BSC_SetBaudRate(i, _tstof(m_asControllerDetails[i].m_omStrBaudrate.c_str()));
    #endif
    }*/
}

void CMainFrame::vInitializeGraphWndReadBuffer()
{
    if (g_pouDIL_CAN_Interface != NULL)
    {
        DWORD dwClientId = 0;
        g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, dwClientId, CAN_MONITOR_NODE);
        if (g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_ADD, dwClientId, &m_ouCanBuf) != S_OK)
        {
            TRACE(_("The function vInitializeGraphWndReadBuffer() failed."));
        }
    }
}
void CMainFrame::OnConfigureSignalgraphwindow()
{
    LONG lParam = 0;
    UINT nHardware = 0;
    if(g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
    {
        nHardware = (UINT)lParam;
    }

    CSigGrphConfigDlg objSigGrphCnfg(nHardware);
    objSigGrphCnfg.m_pMainFrame = this;
    objSigGrphCnfg.DoModal();
    for(short eBusType = CAN; eBusType< MAX_PROTOCOL; eBusType++)
    {
        m_objSigGrphHandler.SetSignalListDetails(eBusType, &m_odGraphList[eBusType]);
    }
}

void CMainFrame::OnUpdateConfigureSignalgraphwindow(CCmdUI* /*pCmdUI*/)
{
}

void CMainFrame::OnSignalgraphwindowCAN()
{
    if ( m_objSigGrphHandler.bIsWindowVisible(CAN) == FALSE )
    {
        m_objSigGrphHandler.CreateGraphWindow(this, CAN);
        m_objSigGrphHandler.SetSignalListDetails(CAN, &m_odGraphList[CAN]);

        CFlags* pouFlags = NULL;
        pouFlags   = theApp.pouGetFlagsPtr();
        if(pouFlags != NULL)
        {
            BOOL bConnected = pouFlags->nGetFlagStatus(CONNECTED);
            eUSERSELCTION eUserSel = eCONNECTCMD;
            m_objSigGrphHandler.vPostMessageToSGWnd(CAN, WM_USER_CMD,
                                                    (WPARAM)eUserSel,bConnected);
        }
        m_objSigGrphHandler.ShowGraphWindow(CAN, SW_SHOW);
    }
    else
    {
        m_objSigGrphHandler.ShowGraphWindow(CAN, SW_HIDE);
    }
}

void CMainFrame::OnUpdateSignalgraphwindowCAN(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_objSigGrphHandler.bIsWindowVisible(CAN));
}

void CMainFrame::OnSignalgraphwindowMcnet()
{
    m_objSigGrphHandler.CreateGraphWindow(this, MCNET);
    m_objSigGrphHandler.SetSignalListDetails(MCNET, &m_odGraphList[MCNET]);
    CFlags* pouFlags = NULL;
    pouFlags   = theApp.pouGetFlagsPtr();
    if(pouFlags != NULL)
    {
        BOOL bConnected = pouFlags->nGetFlagStatus(CONNECTED);
        eUSERSELCTION eUserSel = eCONNECTCMD;
        m_objSigGrphHandler.vPostMessageToSGWnd(MCNET, WM_USER_CMD,
                                                (WPARAM)eUserSel,bConnected);
    }
    m_objSigGrphHandler.ShowGraphWindow(MCNET, SW_SHOW);
}

void CMainFrame::OnUpdateSignalgraphwindowMcnet(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_objSigGrphHandler.bIsWindowVisible(MCNET));
}

void CMainFrame::vPostConfigChangeCmdToSigGrphWnds(BOOL bHideGraphWnd)
{
    for(register int nBusID = CAN; nBusID < AVAILABLE_PROTOCOLS; nBusID++)
    {
        m_objSigGrphHandler.vPostMessageToSGWnd((SHORT)nBusID, WM_USER_CMD,
                                                (WPARAM)eCONFIGCHANGECMD, (LPARAM)bHideGraphWnd);
    }
}

// START J1939 RELATED HANDLERS
void CMainFrame::OnActivateJ1939()
{
    HRESULT Result = S_FALSE;
    bool bActivateStatus = false;
    if ((NULL == sg_pouIJ1939DIL) && (NULL == sg_pouIJ1939Logger))
    {
        Result = ProcessJ1939Interfaces();
        GetIJ1939NodeSim()->NS_SetJ1939ActivationStatus(true);
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_SHOW, (LONG)J1939);
        bActivateStatus = true;
    }
    else
    {
        Result = DeselectJ1939Interfaces();
        GetIJ1939NodeSim()->NS_SetJ1939ActivationStatus(false);
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)J1939);
    }
    /* Modify filter icon accordingly in J1939 toolbar*/
    BYTE bytTbrItemIndex = 1;
    vModifyToolbarIcon( m_wndToolbarJ1939, bytTbrItemIndex, bActivateStatus, IDI_ICON_J1939_DEACTIVATE, IDI_ICON_J1939_ACTIVATE );

    /* If J1939 is deactivated, retain the old icon set and hide J1939 Tx window */
    if ( !bActivateStatus )
    {
        if(m_pouTxMsgWndJ1939 != NULL)
        {
            m_pouTxMsgWndJ1939->ShowWindow(SW_HIDE);
        }
        m_wndToolbarJ1939.bLoadCNVTCToolBar(m_bytIconSize, IDB_TLB_J1939,IDB_TLB_J1939_HOT, IDB_TLB_J1939_DISABLED);
    }
    else
    {
        // Go online or offline
        OnActionJ1939Online();
    }

    ASSERT(Result == S_OK);
}

void CMainFrame::OnUpdateActivateJ1939(CCmdUI* pCmdUI)
{
    /* Enable this menu item ONLY when CAN interface is available. The handler
    is NOT to be invoked after successful querying of J1939 interfaces. So
    disable it if the former is successful */
    if (NULL != g_pouDIL_CAN_Interface)
    {
        if ((NULL == sg_pouIJ1939DIL) && (NULL == sg_pouIJ1939Logger))
        {
            pCmdUI->SetText(_T(_("&Activate")));
        }
        else
        {
            pCmdUI->SetText(_T(_("D&eactivate")));
        }

        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}

//shashank

void CMainFrame::OnUpdateActivateLIN(CCmdUI* pCmdUI)
{
    /* Enable this menu item ONLY when LIN interface is available. The handler
    is NOT to be invoked after successful querying of LIN interfaces. So
    disable it if the former is successful */
    if ((NULL == sg_pouLinDIL) && (NULL == sg_pouLinLogger))
    {
        pCmdUI->SetText(_T(_("&Activate")));
    }
    else
    {
        pCmdUI->SetText(_T(_("D&eactivate")));
    }

    pCmdUI->Enable(TRUE);


}

void CMainFrame::OnJ1939ConfigLog()
{
    vConfigureLogFile(J1939);
}

void CMainFrame::OnLINConfigLog()
{
    vConfigureLogFile(LIN);
}

void CMainFrame::OnUpdateJ1939ConfigLog(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(NULL != sg_pouIJ1939Logger);
}

//shashank
void CMainFrame::OnUpdateLinConfigLog(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(NULL != sg_pouLinLogger);
}

void CMainFrame::OnActionJ1939Online()
{
    bool bOnlineStatus = false;
    if (NULL != sg_pouIJ1939DIL && sg_pouIJ1939DIL->DILIJ_bIsOnline() == FALSE)
    {
        if (sg_pouIJ1939DIL->DILIJ_GoOnline() == S_OK)
        {
            bOnlineStatus = true;
            theApp.bWriteIntoTraceWnd(_("DIL.J1939 network started..."));

            CFlags* pouFlags = theApp.pouGetFlagsPtr();
            BOOL bConnected = pouFlags->nGetFlagStatus(CONNECTED);
            if (bConnected == TRUE)
            {
                if (pouFlags != NULL)
                {
                    GetIJ1939DIL()->DILIJ_NM_GetByteAddres(m_sJ1939ClientParam.m_byAddress,
                                                           m_sJ1939ClientParam.m_dwClientId);
                }

                if(GetIFlags()->nGetFlagStatus(ADRESSCLAIM_J1939) == FALSE)
                {
                    GetIJ1939DIL()->DILIJ_ClaimAdress();
                    GetIFlags()->vSetFlagStatus(ADRESSCLAIM_J1939, TRUE);
                }
            }
            if (m_pouTxMsgWndJ1939 != NULL)
            {
                m_pouTxMsgWndJ1939->vSetJ1939ClientParam(m_sJ1939ClientParam);
                CFlags* pouFlags = theApp.pouGetFlagsPtr();
                if (pouFlags != NULL)
                {
                    BOOL bConnected = pouFlags->nGetFlagStatus(CONNECTED);
                    if (bConnected == TRUE)
                    {
                        ::SendMessage(m_pouTxMsgWndJ1939->GetSafeHwnd(),
                                      WM_CONNECT_CHANGE, (WPARAM)TRUE, (LPARAM)TRUE);
                    }
                }
            }
        }
        else
        {
            theApp.bWriteIntoTraceWnd(_("Network startup failed. Select a valid hardware interface."));
        }
    }
    else
    {
        if (m_pouTxMsgWndJ1939 != NULL)
        {
            ::SendMessage(m_pouTxMsgWndJ1939->GetSafeHwnd(),
                          WM_CONNECT_CHANGE, (WPARAM)FALSE, 0);
        }
        if (NULL != sg_pouIJ1939DIL && sg_pouIJ1939DIL->DILIJ_GoOffline() == S_OK)
        {
            theApp.bWriteIntoTraceWnd(_("DIL.J1939 network stopped..."));
        }
        // PTV [1.6.4]
        if(NULL != sg_pouIJ1939Logger)
        {
            sg_pouIJ1939Logger->FPJ1_DisableJ1939DataLogFlag();
        }
    }
    /* Modify filter icon accordingly in J1939 toolbar*/
    //BYTE bytTbrItemIndex = 1;
    //vModifyToolbarIcon( m_wndToolbarJ1939, bytTbrItemIndex, bOnlineStatus, IDI_ICON_J1939_OFFLINE, IDI_ICON_J1939_ONLINE );

    /* If J1939 goes offline, Switch off J1939 Transmit Item */
    if ( !bOnlineStatus )
    {
        if(m_pouTxMsgWndJ1939 != NULL)
        {
            m_pouTxMsgWndJ1939->ShowWindow(SW_HIDE);
        }
        /*bytTbrItemIndex = 4;
        vSetToolBarIcon( m_wndToolbarJ1939, bytTbrItemIndex,  IDI_ICON_J1939_TRANSMIT_OFF, IDI_ICON_J1939_TRANSMIT_OFF, IDI_ICON_J1939_TRANSMIT_DISABLED);*/
    }
}

void CMainFrame::OnUpdateActionJ1939Online(CCmdUI* pCmdUI)
{
    if (NULL != sg_pouIJ1939DIL)
    {
        pCmdUI->Enable(TRUE);
        if (sg_pouIJ1939DIL->DILIJ_bIsOnline() == TRUE)
        {
            pCmdUI->SetText(_T(_("&Go Offline")));
        }
        else
        {
            pCmdUI->SetText(_T(_("&Go Online")));
        }
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}

void CMainFrame::OnActionJ1939TxMessage()
{
    if (m_pouTxMsgWndJ1939 == NULL)
    {
        m_pouTxMsgWndJ1939 = new CTxMsgWndJ1939(this, m_sJ1939ClientParam);
        m_pouTxMsgWndJ1939->Create(IDD_DLG_TX);
    }

    // Hide the J1939 Transmit window if it is visible
    if(m_pouTxMsgWndJ1939->IsWindowVisible() == TRUE)
    {
        m_pouTxMsgWndJ1939->ShowWindow(SW_HIDE);
    }
    else // Show the J1939 Transmit window if it is hidden
    {
        m_pouTxMsgWndJ1939->ShowWindow(SW_SHOW);
    }
}

void CMainFrame::OnUpdateActionJ1939TxMessage(CCmdUI* pCmdUI)
{
    BOOL bCheck = sg_pouIJ1939DIL && sg_pouIJ1939DIL->DILIJ_bIsOnline();
    pCmdUI->Enable(bCheck);

    if(m_pouTxMsgWndJ1939 != NULL)
    {
        BOOL bIsWindowEnabled = m_pouTxMsgWndJ1939->IsWindowVisible();

        pCmdUI->SetCheck(bIsWindowEnabled);
    }
}

void CMainFrame::vJ1939StartStopLogging()
{
    // Enable Logging or stop logging
    if (NULL != sg_pouIJ1939Logger)
    {
        BOOL bEnable = !sg_pouIJ1939Logger->FPJ1_IsLoggingON();
        if (bEnable)
        {
            vSetAssociatedDatabaseFiles(J1939); // Update the db file names associated
            vSetBaudRateInfo(J1939);                // Update the baud rate details
            // PTV [1.6.4]
            m_unJ1939TimerSBLog = SetTimer(TIMER_REFRESH_J1939_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, NULL);
        }
        sg_pouIJ1939Logger->FPJ1_EnableLogging(bEnable);
    }

}


void CMainFrame::OnActionJ1939Log()
{
    vJ1939StartStopLogging();
    /* Modify filter icon accordingly in J1939 toolbar*/
    BYTE bytTbrItemIndex = 3;
    vModifyToolbarIcon( m_wndToolbarJ1939, bytTbrItemIndex, (bool)sg_pouIJ1939Logger->FPJ1_IsLoggingON(), IDI_ICON_J1939_LOG_ON, IDI_ICON_J1939_LOG_OFF );
    /*sg_pouIJ1939Logger->FPJ1_EnableLogging(!sg_pouIJ1939Logger->FPJ1_IsLoggingON());*/
}



void CMainFrame::OnUpdateActionJ1939Log(CCmdUI* pCmdUI)
{
    if (NULL != sg_pouIJ1939Logger)
    {
        USHORT ushCount = sg_pouIJ1939Logger->FPJ1_GetLoggingBlockCount();
        if(ushCount>0)//log file found
        {
            pCmdUI->Enable(TRUE); //enable the menu item
            pCmdUI->SetCheck(sg_pouIJ1939Logger->FPJ1_IsLoggingON());
        }
        else//log file don't found
        {
            //desable the menu item
            pCmdUI->Enable(FALSE);
            pCmdUI->SetCheck(FALSE);
        }
    }
    else
    {
        pCmdUI->Enable(FALSE);
        pCmdUI->SetCheck(FALSE);
    }
}

//shashank
void CMainFrame::OnUpdateActionLINLog(CCmdUI* pCmdUI)
{
    if (NULL != sg_pouLinLogger)
    {
        USHORT ushCount = sg_pouLinLogger->FPL_GetLoggingBlockCount();
        if(ushCount>0)//log file found
        {
            pCmdUI->Enable(TRUE); //enable the menu item
            pCmdUI->SetCheck(sg_pouLinLogger->FPL_IsLoggingON());
        }
        else//log file don't found
        {
            //desable the menu item
            pCmdUI->Enable(FALSE);
            pCmdUI->SetCheck(FALSE);
        }
    }
    else
    {
        pCmdUI->Enable(FALSE);
        pCmdUI->SetCheck(FALSE);
    }
}

void CMainFrame::OnToolbarJ1939()
{
    ToggleView(m_wndToolbarJ1939);
}

void CMainFrame::OnUpdateToolbarJ1939(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(bIsToolbarVisible(m_wndToolbarJ1939));
}

// END J1939 RELATED HANDLERS
void CMainFrame::OnToolbarFlexRay()
{
    ToggleView(m_wndToolbarFlexRay);
}

void CMainFrame::OnUpdateToolbarFlexRay(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(bIsToolbarVisible(m_wndToolbarFlexRay));
}

void CMainFrame::OnToolbarConfiguration()
{
    ToggleView(m_wndToolbarConfiguration);
}

void CMainFrame::OnUpdateToolbarConfiguration(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(bIsToolbarVisible(m_wndToolbarConfiguration));
}

void CMainFrame::OnToolbarLIN()
{
    ToggleView(m_wndToolbarLIN);
}

void CMainFrame::OnUpdateToolbarLIN(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(bIsToolbarVisible(m_wndToolbarLIN));
}

// START J1939 RELATED HELPER FUNCTIONS
HRESULT CMainFrame::ProcessJ1939Interfaces(void)
{
    HRESULT Result = S_FALSE;

    theApp.bWriteIntoTraceWnd(_("Querying DIL.J1939 ..."));
    if (NULL != (sg_pouIJ1939DIL = GetIJ1939DIL())) // Successfully get the DIL interface
    {
        theApp.bWriteIntoTraceWnd(_("DIL.J1939 query successful..."));
        // Now update the global status
        GetIFlags()->vSetFlagStatus(ACTIVATED_J1939, (int) TRUE);

        // Initialise the interface and register
        Result = sg_pouIJ1939DIL->DILIJ_Initialise(&m_ouWrapperLogger, GetICANDIL());
        if (S_OK == Result)
        {
            theApp.bWriteIntoTraceWnd(_("DIL.J1939 initialisation successful..."));
            Result = sg_pouIJ1939DIL->DILIJ_RegisterClient(TRUE,
                     J1939_MONITOR_NODE, J1939_ECU_NAME, 0, m_sJ1939ClientParam.m_dwClientId);
            if (Result == S_OK || Result == ERR_CLIENT_EXISTS)
            {
                strcpy_s(m_sJ1939ClientParam.m_acName, MAX_PATH, J1939_MONITOR_NODE);
                m_sJ1939ClientParam.m_unEcuName = J1939_ECU_NAME;
                m_podMsgWndThread->vUpdateClientID(J1939, m_sJ1939ClientParam.m_dwClientId);
                m_podMsgWndThread->vSetDILInterfacePointer(J1939, (void**)&(sg_pouIJ1939DIL));
                vReRegisterAllJ1939Nodes();
                if (sg_pouSWInterface[J1939] == NULL)//Signal watch J1939
                {
                    if (SW_GetInterface(J1939, (void**)&sg_pouSWInterface[J1939]) == S_OK)
                    {
                        sg_pouSWInterface[J1939]->SW_DoInitialization();
                    }
                }
                else
                {
                    sg_pouSWInterface[J1939]->SW_DoInitialization();
                }
            }
        }
        else
        {
            theApp.bWriteIntoTraceWnd(_("DIL.J1939 initialisation failed..."));
        }
        // If everything so far is successful, get hold of the logger interface
        if (S_OK == Result)
        {
            theApp.bWriteIntoTraceWnd(_("DIL.J1939 client registration successful..."));
            sg_pouIJ1939Logger = GetIJ1939Logger(); // First query logger interface
            if (NULL != sg_pouIJ1939Logger)
            {
                theApp.bWriteIntoTraceWnd(_("J1939 logger query successful..."));

                // Next initialise the interface
                CParamLoggerJ1939 ouParam;
                CString omVerStr("");       // First get the version information
                omVerStr.Format(IDS_VERSION);   // string from the rsource
                strcpy_s(ouParam.m_acVersion, MAX_PATH, omVerStr.GetBuffer(MAX_CHAR));

                ouParam.m_pILog = &m_ouWrapperLogger;
                ouParam.dwClientID = m_sJ1939ClientParam.m_dwClientId;
                Result = sg_pouIJ1939Logger->FPJ1_DoInitialisation(&ouParam);

                if (S_OK == Result)
                {
                    theApp.bWriteIntoTraceWnd(_("J1939 logger successfully initialised..."));
                    // That's all. Now return the result.
                }
                else
                {
                    theApp.bWriteIntoTraceWnd(_("J1939 logger initialisation failed..."));
                }
            }
            else
            {
                theApp.bWriteIntoTraceWnd(_("J1939 logger query failed..."));
            }
        }
        else
        {
            theApp.bWriteIntoTraceWnd(_("DIL.J1939 client registration failed..."));
        }
    }
    else
    {
        theApp.bWriteIntoTraceWnd(_("Query of DIL.J1939 failed..."));
    }
    return Result;
}

HRESULT CMainFrame::DeselectJ1939Interfaces(void)
{
    HRESULT Result = S_OK;

    // First stop J1939 logging if it is ON
    if (NULL != sg_pouIJ1939Logger)
    {
        if (sg_pouIJ1939Logger->FPJ1_IsLoggingON() == TRUE)
        {
            theApp.bWriteIntoTraceWnd(_("Stopping J1939 logging..."));
            if (sg_pouIJ1939Logger->FPJ1_EnableLogging(FALSE) != S_OK)
            {
                theApp.bWriteIntoTraceWnd(_("Stopping J1939 logging failed..."));
                Result = S_FALSE;
            }
        }
        // Logger interface needs a reinitialisation next time. Set the pointer to
        // NULL to update the GUI accordingly.
        sg_pouIJ1939Logger = NULL;
    }

    if (NULL != sg_pouIJ1939DIL)
    {
        if (m_pouTxMsgWndJ1939 != NULL)
        {
            ::SendMessage(m_pouTxMsgWndJ1939->GetSafeHwnd(), WM_CONNECT_CHANGE, (WPARAM)FALSE, 0);
        }
        if (sg_pouIJ1939DIL->DILIJ_bIsOnline() == TRUE)
        {
            theApp.bWriteIntoTraceWnd(_("Going Offline..."));
            if (sg_pouIJ1939DIL->DILIJ_GoOffline() != S_OK)
            {
                theApp.bWriteIntoTraceWnd(_("Going Offline failed..."));
                Result = S_FALSE;
            }
        }
        theApp.bWriteIntoTraceWnd(_("Uninitialising DIL.J1939..."));
        /* PTV[1.6.4] */
        // On Disassociate of J1939 Unload all the Simulated systems
        CStringArray omStrBuildFiles;
        GetIJ1939NodeSim()->NS_DllUnloadAll(&omStrBuildFiles);
        /* PTV[1.6.4] */
        if (sg_pouIJ1939DIL->DILIJ_Uninitialise() != S_OK)
        {
            theApp.bWriteIntoTraceWnd(_("Uninitialising DIL.J1939 failed..."));
            Result = S_FALSE;
        }
        else
        {
            theApp.bWriteIntoTraceWnd(_("Done..."));
        }
        // Update the global status and reset the inteface pointer to NULL.
        GetIFlags()->vSetFlagStatus(ACTIVATED_J1939, (int) FALSE);
        GetIFlags()->vSetFlagStatus(ADRESSCLAIM_J1939, FALSE);
        sg_pouIJ1939DIL = NULL;
    }

    return Result;
}

void CMainFrame::vConfigureLogFile(ETYPE_BUS eCurrBus)
{
    if (J1939 == eCurrBus)
    {
        if (NULL != sg_pouIJ1939Logger)
        {
            CConfigMsgLogDlg omDlg(J1939, (void*) sg_pouIJ1939Logger, m_abLogOnConnect[J1939], this,
                                   (void*) &m_sFilterAppliedJ1939);
            sg_pouIJ1939Logger->FPJ1_StartEditingSession(); // Start the editing session
            omDlg.vSetLogFileONOFF(sg_pouIJ1939Logger->FPJ1_IsLoggingON());
            BOOL bToConfirm = (omDlg.DoModal() == IDOK);
            sg_pouIJ1939Logger->FPJ1_StopEditingSession(bToConfirm);
        }
    }


    if (LIN == eCurrBus)
    {
        if (NULL != sg_pouLinLogger )
        {
            CConfigMsgLogDlg omDlg(LIN, (void*) sg_pouLinLogger, m_abLogOnConnect[LIN], this,
                                   (void*) &m_sFilterAppliedLIN);
            sg_pouLinLogger->FPL_StartEditingSession(); // Start the editing session
            omDlg.vSetLogFileONOFF(sg_pouLinLogger->FPL_IsLoggingON());
            BOOL bToConfirm = (omDlg.DoModal() == IDOK);
            sg_pouLinLogger->FPL_StopEditingSession(bToConfirm);
        }
    }
}
// END J1939 RELATED HELPER FUNCTIONS

void CMainFrame::OnJ1939ConfigureTimeouts()
{
    CJ1939TimeOutCfg omDlg;
    omDlg.DoModal();
}

void CMainFrame::OnUpdateJ1939Timeouts(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(NULL != sg_pouIJ1939DIL);
}

void CMainFrame::OnUpdateJ1939DBNew(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( !CMsgSignalDBWnd::sm_bValidJ1939Wnd );
}

static void vGetNewJ1939DBName(CString& omString)
{
    UINT unCount = 1;
    BOOL bContinue = TRUE;
    while (bContinue == TRUE)
    {
        CString omStrDbName = NEW_DATABASE_NAME_J1939;
        CString omStr = STR_EMPTY;
        omStr.Format( "%d", unCount++);
        omStr += ".";
        omStr += DATABASE_EXTN;
        omStrDbName += omStr;
        // file-attribute information
        struct _finddata_t fileinfo;
        // Auto Select DB File
        if (_findfirst( omStrDbName, &fileinfo)== -1L)
        {
            omString = omStrDbName;
            bContinue = FALSE;
        }
    }
}
static BOOL bCreateStudioFile(CString& omDefaultFileName)
{
    BOOL bResult = FALSE;
    CStdioFile om_File;
    TRY
    {
        CFileDialog fileDlg(FALSE, DATABASE_EXTN, omDefaultFileName.GetBuffer(MAX_CHAR),
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, DATABASE_FILTER);
        if (fileDlg.DoModal() == IDOK)
        {
            omDefaultFileName = fileDlg.GetPathName();
            bResult = TRUE;

            // create the selected file
            if (om_File.Open(omDefaultFileName,
            CFile::modeCreate | CFile::modeRead |
            CFile::typeText))
            {
                om_File.Close();
            }
        }
    }
    CATCH_ALL(om_Fe)
    {
        if(om_Fe != NULL )
        {
            LPTSTR lpszError = NULL;
            // Get error
            om_Fe->GetErrorMessage( lpszError, 255);

            AfxMessageBox( lpszError, NULL, MB_OK );

            om_Fe->Delete();
        }
    }
    END_CATCH_ALL

    return bResult;
}

void CMainFrame::OnJ1939DBNew()
{

    // Check if any database is already open
    if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
    {
        // Some database is open
        // Flash a message as to whether the user
        // wants to open another database
        INT nReturn =
            AfxMessageBox( _("Are you sure you want to close the \ndatabase that is already open?"), MB_YESNO, MB_ICONINFORMATION);
        if ( nReturn == IDYES)
        {
            // Close the database that was open
            OnJ1939DBClose();
        }
    }
    if (m_pouActiveDbJ1939 == NULL)
    {
        m_pouActiveDbJ1939 = new CMsgSignal(sg_asDbParams[J1939], FALSE);
    }

    if (NULL != m_podMsgSgWndJ1939 && !IsWindow(m_podMsgSgWndJ1939->m_hWnd))
    {
        m_podMsgSgWndJ1939 = NULL;
    }

    if (m_podMsgSgWndJ1939 == NULL)
    {
        sg_asDbParams[J1939].m_ppvActiveDB = (void**)&m_pouActiveDbJ1939;
        sg_asDbParams[J1939].m_ppvImportedDBs = (void**)&m_pouMsgSigJ1939;
        m_podMsgSgWndJ1939 = new CMsgSignalDBWnd(sg_asDbParams[J1939]);
    }

    if (m_podMsgSgWndJ1939 != NULL)
    {
        vGetNewJ1939DBName(m_omJ1939DBName);
        if (bCreateStudioFile(m_omJ1939DBName) == TRUE)
        {
            sg_asDbParams[J1939].m_omDBPath = m_omJ1939DBName;
            m_podMsgSgWndJ1939->vSetDBName(m_omJ1939DBName);

            if ( !m_podMsgSgWndJ1939->Create(  NULL, _("Database Editor"), WS_CHILD |
                                               WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION |
                                               WS_THICKFRAME, rectDefault, this ))
            {
                MessageBox( _("Create BUSMASTER Database Window Failed!"),
                            NULL, MB_OK|MB_ICONERROR );
                return;
            }

            CMsgSignalDBWnd::sm_bValidJ1939Wnd = TRUE;
            m_podMsgSgWndJ1939->ShowWindow( SW_SHOWMAXIMIZED );
            m_podMsgSgWndJ1939->UpdateWindow();

            // Set the flag to indicate the opening of database window
            CFlags* pFlags = theApp.pouGetFlagsPtr();

            if(pFlags != NULL)
            {
                pFlags->vSetFlagStatus( DBOPEN_J1939, TRUE );
            }
        }
    }

}

void CMainFrame::OnJ1939DBOpen()
{
    // Check if any database is already open
    if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
    {
        // Some database is open
        // Flash a message as to whether the user
        // wants to open another database
        INT nReturn =
            AfxMessageBox( _("Are you sure you want to close the \ndatabase that is already open?"), MB_YESNO, MB_ICONINFORMATION);
        if ( nReturn == IDYES)
        {
            // Close the database that was open
            OnJ1939DBClose();
        }
        else
        {
            return;
        }
    }
    // Display a open file dialog
    CFileDialog fileDlg( TRUE,      // Open File dialog
                         "dbf",     // Default Extension,
                         NULL,
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         _("J1939 Database File(s)(*.dbf)|*.dbf||"),
                         NULL );
    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _T(_("Select J1939 Database Filename..."));

    if ( IDOK == fileDlg.DoModal() )
    {
        CString strExtName  = fileDlg.GetFileExt();
        CString strDbName   = fileDlg.GetPathName();
        if ( strDbName.ReverseFind('.') )
        {
            strDbName = strDbName.Left( strDbName.ReverseFind('.') + 1);
            strDbName.TrimRight();
            strDbName += strExtName;
            m_omJ1939DBName = strDbName;
        }
        BOOL bDisplayEditor = FALSE;
        // file-attribute information
        struct _finddata_t fileinfo;
        // Auto Select DB File
        if (_findfirst( strDbName, &fileinfo)!= -1L)
        {
            // Load the File & fill the Structure
            if (m_pouActiveDbJ1939 == NULL)
            {
                sg_asDbParams[J1939].m_ppvActiveDB = (void**)&m_pouActiveDbJ1939;
                sg_asDbParams[J1939].m_ppvImportedDBs = (void**)&m_pouMsgSigJ1939;
                m_pouActiveDbJ1939 = new CMsgSignal(sg_asDbParams[J1939], FALSE);
            }
            if (m_pouActiveDbJ1939->
                    bFillDataStructureFromDatabaseFile(strDbName, PROTOCOL_J1939))
            {
                // No corruption in database, display the editor
                bDisplayEditor = TRUE;
            }
        }
        else
        {
            AfxMessageBox(_("Specified database file is not found.\nOperation unsuccessful."), MB_OK|MB_ICONINFORMATION);
        }

        if ( bDisplayEditor == TRUE )
        {
            if ( m_podMsgSgWndJ1939 != NULL )
            {
                m_podMsgSgWndJ1939 = NULL;
            }

            m_podMsgSgWndJ1939 = new CMsgSignalDBWnd(sg_asDbParams[J1939]);

            if ( m_podMsgSgWndJ1939 != NULL )
            {
                sg_asDbParams[J1939].m_omDBPath = m_omJ1939DBName;
                m_podMsgSgWndJ1939->vSetDBName(m_omJ1939DBName);
                // Create child window
                if ( !m_podMsgSgWndJ1939->Create(NULL,
                                                 _("Database Editor"),
                                                 WS_CHILD | WS_VISIBLE |
                                                 WS_OVERLAPPED | WS_CAPTION |
                                                 WS_THICKFRAME, rectDefault,
                                                 this ) )
                {
                    MessageBox( _("Create J1939 Database Window Failed!"),
                                NULL, MB_OK|MB_ICONERROR );
                    return;
                }
                CMsgSignalDBWnd::sm_bValidJ1939Wnd = TRUE;
                m_podMsgSgWndJ1939->ShowWindow( SW_SHOWMAXIMIZED );
                m_podMsgSgWndJ1939->UpdateWindow();

                // Set the flag to indicate the opening of database window
                CFlags* pFlags = theApp.pouGetFlagsPtr();
                if(pFlags != NULL)
                {
                    pFlags->vSetFlagStatus( DBOPEN_J1939, TRUE );
                }
            }
            else
            {
                AfxMessageBox(_(MSG_MEMORY_CONSTRAINT),
                              MB_OK | MB_ICONINFORMATION);
            }
        }
    }
}

void CMainFrame::OnJ1939DBClose()
{
    if (m_podMsgSgWndJ1939 != NULL)
    {
        if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
        {
            m_podMsgSgWndJ1939->SendMessage(WM_CLOSE, NULL, NULL);
            m_podMsgSgWndJ1939 = NULL;
        }
        else
        {
            m_podMsgSgWndJ1939 = NULL;
        }
        // Set the flag to indicate the opening of database window
        CFlags* pFlags = theApp.pouGetFlagsPtr();

        if(pFlags != NULL)
        {
            pFlags->vSetFlagStatus( DBOPEN_J1939, FALSE );
        }
    }
}

void CMainFrame::OnJ1939DBSave()
{
    if (m_podMsgSgWndJ1939 != NULL)
    {
        if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
        {
            //send save message
            m_podMsgSgWndJ1939->SendMessage(WM_SAVE_DBJ1939, NULL, NULL);

            // Set all the items in the tree view to normal font
            if ( m_pomMsgSgTreeViews[J1939] != NULL)
            {
                m_pomMsgSgTreeViews[J1939]->vSetAllItemsNormal();
            }
        }
    }
}

void CMainFrame::OnUpdateJ1939DBSave(CCmdUI* pCmdUI)
{
    BOOL bResult = FALSE;
    if (m_podMsgSgWndJ1939 != NULL)
    {
        if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
        {
            // Get appropriate data structure
            CMsgSignal** ppTempMsgSg = NULL;
            ppTempMsgSg = (CMsgSignal**)m_podMsgSgWndJ1939->m_sDbParams.m_ppvActiveDB;
            if((*ppTempMsgSg) != NULL)
            {
                if ((*ppTempMsgSg)->bGetModifiedFlag() == FALSE)
                {
                    bResult = TRUE;
                }
            }
        }
    }

    pCmdUI->Enable(bResult);
}

void CMainFrame::OnUpdateJ1939DBSaveAs(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN_J1939 ));
}

void CMainFrame::OnUpdateJ1939DBClose(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN_J1939 ));
}

void CMainFrame::OnJ1939DBAssociate()
{
    CStringArray strFilePathArray;
    // Display a open file dialog
    char szFilters[] = _T("BUSMASTER J1939 Database File(s)(*.dbf)|*.dbf||");
    CFileDialog fileDlg( TRUE,      // Open File dialog
                         "dbf",     // Default Extension,
                         NULL,
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         _(szFilters),
                         NULL );

    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _T(_("Select Active Database Filename..."));

    if ( IDOK == fileDlg.DoModal() )
    {
        POSITION pos = NULL;
        pos = fileDlg.GetStartPosition();
        while(NULL != pos)
        {
            CString strTempFile = fileDlg.GetNextPathName(pos);
            strFilePathArray.Add(strTempFile);
        }
        CString omStrMsg = _("Database File: \n ");
        BOOL bAllFilesImported = TRUE;
        int nFileCount = strFilePathArray.GetSize();
        for(int nCount = 0; nCount < nFileCount; nCount++)
        {
            CString strTempFileName = strFilePathArray.GetAt(nCount);
            //FALSE because it is not called using COM
            DWORD dError = dLoadJ1939DBFile(strTempFileName,FALSE);
            if(E_INVALIDARG == dError)
            {
                bAllFilesImported = FALSE;
                //Add the file name for warning display
                omStrMsg += strTempFileName;
                omStrMsg += defNEW_LINE;
            }
        }
        if(bAllFilesImported == FALSE)
        {
            omStrMsg += _(" not found!");
            MessageBox(omStrMsg,_("BUSMASTER"),MB_OK|MB_ICONERROR);
        }
        else
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(J1939);
            //Set the J1939 DB pointer in MsgFrmtWnd class
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_NOTIFICATION_FROM_OTHER,
                              eLOAD_DATABASE,
                              (LPARAM)&(m_pouMsgSigJ1939));
                ::SendMessage(hWnd, WM_DATABASE_CHANGE, (WPARAM)TRUE, NULL);
            }
            vPostMsgToSendMsgDlg(J1939);
        }
    }
}

void CMainFrame::OnJ1939DBDissociate()
{
    if (m_pouMsgSigJ1939 == NULL)
    {
        m_pouMsgSigJ1939 = new CMsgSignal(sg_asDbParams[J1939], FALSE);
    }
    sg_asDbParams[J1939].m_ppvActiveDB = (void**)&(m_pouActiveDbJ1939);
    sg_asDbParams[J1939].m_ppvImportedDBs = (void**)&(m_pouMsgSigJ1939);
    CDatabaseDissociateDlg odDBDialog(sg_asDbParams[J1939]);
    odDBDialog.DoModal();
    //Update J1939 TxWnd
    SMSGENTRY* psMsgEntry = NULL;
    vPopulateMsgEntryFromDB(psMsgEntry, m_pouMsgSigJ1939);
    if (m_pouTxMsgWndJ1939 != NULL)
    {
        m_pouTxMsgWndJ1939->vSetDatabaseInfo(psMsgEntry);
    }
    else
    {
        CTxMsgWndJ1939::vUpdateDataStore(psMsgEntry);
    }

    vUpdateMsgNameCodeList( m_pouMsgSigJ1939, m_sExFuncPtr[J1939].m_odMsgNameMsgCodeListDB);

    //Update in J1939 NodeSimEx
    if ( NULL != GetIJ1939NodeSim() )
    {
        GetIJ1939NodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[J1939].m_omDefinedMsgHeaders), UPDATE_UNIONS_HEADER_FILES);

        GetIJ1939NodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[J1939].m_odMsgNameMsgCodeListDB), UPDATE_DATABASE_MSGS);
    }
}

void CMainFrame::OnJ1939CfgSimSys()
{
    NS_InitJ1939SpecInfo();
    GetIJ1939NodeSim()->FE_CreateFuncEditorTemplate(this->GetSafeHwnd(), m_sExFuncPtr[J1939]);
}

void CMainFrame::OnJ1939LoadAll()
{
    CStringArray omStrBuildFiles;

    BOOL bSucces = GetIJ1939NodeSim()->NS_DllLoadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the load is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are not properly loaded:"));
        CString omStrErrorMsgDummy="";
        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format("\n%s", omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

void CMainFrame::OnUpdateJ1939LoadAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetIJ1939NodeSim()->NS_ShouldToolBarBeEnabled(TB_LOAD));
}

void CMainFrame::OnJ1939UnloadAll()
{
    CStringArray omStrBuildFiles;
    BOOL bSucces = GetIJ1939NodeSim()->NS_DllUnloadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the unload is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are not properly unloaded:"));
        CString omStrErrorMsgDummy="";
        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format("\n%s", omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

void CMainFrame::OnUpdateJ1939UnloadAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetIJ1939NodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD));
}

void CMainFrame::OnJ1939BuildAndLoadAll()
{
    CStringArray omStrBuildFiles;

    BOOL bSucces = GetIJ1939NodeSim()->NS_DLLBuildLoadAll(&omStrBuildFiles);
    if(!bSucces)  // if the build is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are either not properly build or loaded:"));
        CString omStrErrorMsgDummy="";
        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

void CMainFrame::OnUpdateJ1939BuildAndLoadAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetIJ1939NodeSim()->NS_ShouldToolBarBeEnabled(TB_BUILDLOAD));
}

void CMainFrame::OnJ1939BuildAll()
{
    CStringArray omStrBuildFiles;

    BOOL bSucces = GetIJ1939NodeSim()->NS_DLLBuildAll(&omStrBuildFiles);
    if(!bSucces)  // if the build is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg =_T(_("Following file(s) are either not properly build or loaded:"));
        CString omStrErrorMsgDummy="";
        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

void CMainFrame::OnUpdateJ1939BuildAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetIJ1939NodeSim()->NS_ShouldToolBarBeEnabled(TB_BUILD));
}

void CMainFrame::OnJ1939AllMessageHandlers()
{
    //Get present status
    BOOL bEnable = GetIJ1939NodeSim()->NS_GetHandlerStatus(H_MSGHANDLERBUTTON);
    //Change the status of handler
    GetIJ1939NodeSim()->NS_EnableAllMessageHandler( !bEnable );
}

void CMainFrame::OnUpdateJ1939AllMessageHandlers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetIJ1939NodeSim()->NS_GetHandlerStatus(H_MSGHANDLERBUTTON));
    BOOL bEnable = GetIJ1939NodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

void CMainFrame::OnJ1939AllKeyHandlers()
{
    //Get present status
    BOOL bEnable = GetIJ1939NodeSim()->NS_GetHandlerStatus(H_KEY_HANDLER_ON);
    //Change the status of handler
    GetIJ1939NodeSim()->NS_EnableAllKeyHandler( !bEnable );
}

void CMainFrame::OnUpdateJ1939AllKeyHandlers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetIJ1939NodeSim()->NS_GetHandlerStatus(H_KEY_HANDLER_ON));
    BOOL bEnable = GetIJ1939NodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

void CMainFrame::OnJ1939AllTimerHandlers()
{
    //Get present status
    BOOL bEnable = GetIJ1939NodeSim()->NS_GetHandlerStatus(H_TIMERBUTTON);
    //Change the status of handler
    GetIJ1939NodeSim()->NS_EnableAllTimers( !bEnable );
}

/**
* \brief         This function will enable all the event handlers
* \return        void
* \authors       Arunkumar Karri
* \date          11.06.2012 Created
*/
void CMainFrame::OnJ1939AllEventHandlers()
{
    //Get present status
    BOOL bEnable = GetIJ1939NodeSim()->NS_GetHandlerStatus(H_EVENT_HANDLER);
    //Change the status of handler
    GetIJ1939NodeSim()->NS_EnableAllEventHandler( !bEnable );
}

void CMainFrame::OnUpdateJ1939AllTimerHandlers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetIJ1939NodeSim()->NS_GetHandlerStatus(H_TIMERBUTTON));
    BOOL bEnable = GetIJ1939NodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

/**
* \brief         This function will update the enable status of event handlers
* \param[in]     CCmdUI *pCmdUI
* \return        void
* \authors       Arunkumar Karri
* \date          11.06.2012 Created
*/
void CMainFrame::OnUpdateJ1939AllEventHandlers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetIJ1939NodeSim()->NS_GetHandlerStatus(H_EVENT_HANDLER));
    BOOL bEnable = GetIJ1939NodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

void CMainFrame::OnJ1939AllHandlers()
{
    //Get present status
    BOOL bEnable = GetIJ1939NodeSim()->NS_GetHandlerStatus(H_ALL_HANDLER);
    //Change the status of handler
    GetIJ1939NodeSim()->NS_EnableAllHandlers( !bEnable );
}
void CMainFrame::OnUpdateJ1939AllHandlers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetIJ1939NodeSim()->NS_GetHandlerStatus(H_ALL_HANDLER));
    BOOL bEnable = GetIJ1939NodeSim()->NS_ShouldToolBarBeEnabled(TB_UNLOAD);
    pCmdUI->Enable(bEnable);
}

void CMainFrame::OnUpdateJ1939CfgSimSys(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(NULL != sg_pouIJ1939DIL);
}

void CMainFrame::OnJ1939SignalwatchAdd()
{
    CMsgSignal* pomDatabase = NULL;
    pomDatabase = m_pouMsgSigJ1939;
    if( pomDatabase != NULL )
    {
        if( pomDatabase->unGetNumerOfMessages() > 0)
        {
            /* Test code starts*/
            CMainEntryList odResultingList;
            vPopulateMainEntryList(&odResultingList, m_psSignalWatchList[J1939], m_pouMsgSigJ1939);
            if (sg_pouSWInterface[J1939] == NULL)
            {
                if (SW_GetInterface(J1939, (void**)&sg_pouSWInterface[J1939]) == S_OK)
                {
                    sg_pouSWInterface[J1939]->SW_DoInitialization();
                }
            }
            if (sg_pouSWInterface[J1939] != NULL)
            {
                if (sg_pouSWInterface[J1939]->SW_ShowAddDelSignalsDlg(this, &odResultingList) == IDOK)
                {
                    vPopulateSigWatchList(odResultingList, m_psSignalWatchList[J1939], m_pouMsgSigJ1939);// This populates m_psSignalWatchList
                    m_ouMsgInterpretSW_J.vSetJ1939Database(m_psSignalWatchList[J1939]);
                    sg_pouSWInterface[J1939]->SW_UpdateMsgInterpretObj(&m_ouMsgInterpretSW_J);
                }
            }
        }
        else
        {
            // Database is not imported!!
            AfxMessageBox( _(defSTR_EMPTY_ACTIVE_DATABASE) );
        }
    }
}

void CMainFrame::OnJ1939SignalwatchShow()
{
    if (sg_pouSWInterface[J1939] == NULL)
    {
        if (SW_GetInterface(J1939, (void**)&sg_pouSWInterface[J1939]) == S_OK)
        {
            sg_pouSWInterface[J1939]->SW_DoInitialization();
        }
    }
    if (sg_pouSWInterface[J1939] != NULL)
    {
        INT nCmd = sg_pouSWInterface[J1939]->SW_IsWindowVisible() ? SW_HIDE : SW_SHOW;
        sg_pouSWInterface[J1939]->SW_ShowSigWatchWnd(this, nCmd);
        BOOL bHexON = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);
        sg_pouSWInterface[J1939]->SW_SetDisplayMode(bHexON);
    }
}
void CMainFrame::OnUpdateJ1939SignalwatchShow(CCmdUI* pCmdUI)
{
    if(pCmdUI != NULL )
    {
        if(sg_pouSWInterface[J1939] != NULL)
        {
            if (sg_pouSWInterface[J1939]->SW_IsWindowVisible() == TRUE)
            {
                pCmdUI->SetCheck(TRUE);
            }
            else
            {
                pCmdUI->SetCheck(FALSE);
            }
        }
    }
}
void CMainFrame::OnConfigureMessagedisplayJ1939()
{
    CPPageMessage odDBMsg(TRUE, J1939, m_pouMsgSigJ1939), odNDBMsg(FALSE, J1939, m_pouMsgSigJ1939);
    CPropertySheet omAllMessages(_("Configure Message Display - J1939"));
    //CMsgFilterConfigPage omFilter(&m_sFilterAppliedJ1939, m_podMsgWndThread->hGetHandleMsgWnd(J1939));

    /*BOOL bConnected = FALSE;
    CFlags* pouFlag      = theApp.pouGetFlagsPtr();
    // Get the connection status
    if(pouFlag != NULL)
    {
        bConnected   = pouFlag->nGetFlagStatus(CONNECTED);
    }*/

    omAllMessages.m_psh.dwFlags |= PSH_NOAPPLYNOW;
    omAllMessages.m_psh.dwFlags &= ~PSH_HASHELP;
    // Add Filter Page
    //omAllMessages.AddPage(&omFilter);

    // Add DB & NDB pages
    omAllMessages.AddPage(&odDBMsg);
    omAllMessages.AddPage(&odNDBMsg);

    omAllMessages.DoModal();
}

void CMainFrame::OnConfigureMessageDisplayFlexRay()
{
    CPropertySheet omAllMessages(_("Configure Message Display - FlexRay"));
    CConfigMsgDispPropPg odMsgDisplay;

    omAllMessages.m_psh.dwFlags |= PSH_NOAPPLYNOW;
    omAllMessages.m_psh.dwFlags &= ~PSH_HASHELP;

    omAllMessages.AddPage(&odMsgDisplay);
    omAllMessages.DoModal();
}

void CMainFrame::OnToolbarCandatabase()
{
    ToggleView(m_wndToolbarCANDB);
}

void CMainFrame::OnUpdateToolbarCanDatabase(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(bIsToolbarVisible(m_wndToolbarCANDB));
}

//venkat
void CMainFrame::OnAutomationTSEditor(void)
{
    m_objTSEditorHandler.vShowTSEditorWindow((void*)this);
}
void CMainFrame::OnFlexRayTxWindow()
{
    m_objFlexTxHandler.vShowConfigureMsgWindow(this, FLEXRAY);
}
void CMainFrame::OnUpdateFlexrayAssociate(CCmdUI* pCmdUI)
{
    CFlags* pouFlag  = theApp.pouGetFlagsPtr();
    if ( (pouFlag!= NULL) && (TRUE == pouFlag->nGetFlagStatus(FLEX_CONNECTED)))
    {
        pCmdUI->Enable(FALSE);
    }
    else
    {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrame::OnFlexRayDBAssociate()
{
    string          strFilePath;
    CStringArray strFilePathArray;
    strFilePath = m_acFlexDBConfigInfo.m_acConfigFileName[0];


    CFibexConfigDlg* ouConfig = new CFibexConfigDlg(m_pouMsgSigFLEXRAY,m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig , m_nMaxFlexChannels , FLEXRAY, NULL);

    if ( ouConfig == NULL )
    {
        return;
    }

    if ( ouConfig->DoModal() == IDOK)
    {
        m_ouClusterConfig[FLEXRAY].m_nChannelsConfigured = 1;
        for ( int i = 0 ; i < CHANNEL_ALLOWED; i++ )
        {
            m_ouClusterConfig[FLEXRAY].m_ouFlexChannelConfig[i] = ouConfig->m_ouFlexrayChannelConfig[i];

        }
        delete ouConfig;
        m_objFlexTxHandler.SetFibexConfig(FLEXRAY, m_ouClusterConfig[FLEXRAY]);


        //Venkat
        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                      WM_NOTIFICATION_FROM_OTHER,
                      eLOAD_DATABASE,
                      (LPARAM)&m_ouClusterConfig[FLEXRAY]);

    }
}



void CMainFrame::OnFlexRayDBDisociate()
{
    CFlexRayDatabaseDissociateDlg odDBDialog(m_acFlexDBConfigInfo);
    odDBDialog.DoModal();

    if ( m_acFlexDBConfigInfo.m_acConfigFileName[0].empty() )
    {
        m_pouMsgSigFLEXRAY->vClearFIBEXContainers();
        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(FLEXRAY),
                      WM_NOTIFICATION_FROM_OTHER,
                      eLOAD_DATABASE,
                      (LPARAM)&(m_pouMsgSigFLEXRAY));
        /* Update message window */
        vUpdateAllMsgWndInterpretStatus(FALSE);
    }
}
void CMainFrame::OnAutomationTSExecutor(void)
{
    m_objTSExecutorHandler.vShowTSExecutorWindow((void*)this);
    m_objTSExecutorHandler.vSetTSEVersionInfo(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
}

LRESULT CMainFrame::OnReceiveKeyBoardData(WPARAM wParam, LPARAM lParam)
{
    MSG msg;
    msg.message = WM_CHAR;
    msg.wParam = wParam;
    msg.lParam = lParam;

    vProcessKeyPress(&msg);

    /*PostMessage(WM_KEY_PRESSED_MSG_WND, wParam, 0);*/
    return S_OK;
}


LRESULT CMainFrame::onGetConfigPath(WPARAM wParam, LPARAM lParam)
{
    char* pchPath = (char*)wParam;
    if(pchPath != NULL)
    {
        CString strPath;
        vGetLoadedCfgFileName(strPath);
        strcpy(pchPath, strPath.GetBuffer(MAX_PATH));
    }
    return S_OK;
}
LRESULT CMainFrame::OnReceiveKeyDown(WPARAM wParam, LPARAM lParam)
{
    MSG msg;
    msg.message = WM_KEYDOWN;
    msg.wParam = wParam;
    msg.lParam = lParam;

    vProcessKeyPress(&msg);

    /*PostMessage(WM_KEY_PRESSED_MSG_WND, wParam, 0);*/
    return S_OK;
}

void CMainFrame::vProcessKeyPress(MSG* pMsg)
{
    if (pMsg->message == WM_CHAR)
    {
        // Check for key a-z or A-Z, if any of these are press
        // call member function of CExecuteFunc class for key handler
        if( pMsg->wParam >= 0x20 && pMsg->wParam<= 0x7E )
        {

            // Execute key hanlder only if execution is selected by user
            GetICANNodeSim()->NS_ManageOnKeyHandler((UCHAR)pMsg->wParam);
            GetIJ1939NodeSim()->NS_ManageOnKeyHandler((UCHAR)pMsg->wParam);
            GetILINNodeSim()->NS_ManageOnKeyHandler((UCHAR)pMsg->wParam);
            PostMessage(WM_KEY_PRESSED_MSG_WND,
                        pMsg->wParam, 0);

        }

    }
    if (pMsg->message == WM_KEYUP)
    {
        // Check for key a-z or A-Z, if any of these are press
        // call member function of CExecuteFunc class for key handler
        UINT unKey;
        if(GetKeyState(VK_CONTROL) & 0x8000)
        {
        }
        else
        {
            switch(pMsg->wParam)
            {
                case VK_TAB:
                    unKey = BMKEY_TAB;
                    break;
                case VK_RETURN:
                    unKey = BMKEY_RETURN;
                    break;
                case VK_BACK:
                    unKey = BMKEY_BACK;
                    break;
                case VK_PRIOR:
                    unKey = BMKEY_PAGEUP;
                    break;
                case VK_NEXT:
                    unKey = BMKEY_PAGEDOWN;
                    break;
                case VK_END:
                    unKey = BMKEY_END;
                    break;
                case VK_HOME:
                    unKey = BMKEY_HOME;
                    break;
                case VK_LEFT:
                    unKey = BMKEY_LEFTARROW;
                    break;
                case VK_UP:
                    unKey = BMKEY_UPARROW;
                    break;
                case VK_RIGHT:
                    unKey = BMKEY_RIGHTARROW;
                    break;
                case VK_DOWN:
                    unKey = BMKEY_DOWNARROW;
                    break;
                case VK_INSERT:
                    unKey = BMKEY_INSERT;
                    break;
                case VK_DELETE:
                    unKey = BMKEY_DELETE;
                    break;
                default:
                    if(pMsg->wParam >= VK_F1 && pMsg->wParam <= VK_F12)
                    {
                        unKey = (pMsg->wParam - VK_F1) + BMKEY_F1;
                    }
                    else
                    {
                        unKey = 0xFF;
                    }
                    break;
            }
            if(unKey != 0xFF)
            {
                GetICANNodeSim()->NS_ManageOnKeyHandler(unKey);
            }

        }
        // Execute key hanlder only if execution is selected by user

    }
    if (pMsg->message == WM_KEYDOWN)
    {
        CFlags* pouFlag = NULL;
        pouFlag     = theApp.pouGetFlagsPtr();
        if(pouFlag != NULL)
        {
            BOOL bConnected = FALSE;
            BOOL bConnectedFlexRay = FALSE;
            BOOL bConnectedLIN = FALSE;
            // Get the current status of Connected/Disconnected state
            bConnected  = pouFlag->nGetFlagStatus(CONNECTED);
            // Get the current status of Connected/Disconnected state for FlexRay
            bConnectedFlexRay  = pouFlag->nGetFlagStatus(FLEX_CONNECTED);

            bConnectedLIN  = pouFlag->nGetFlagStatus(LIN_CONNECTED);
            //Procees the key "F2" and "ESC"
            if (pMsg->wParam == VK_F2)
            {
                CFrameWnd*   pFrmWnd = GetActiveFrame() ;
                if(m_podMsgSgWnd == pFrmWnd)//check for CAN Database
                {
                    CWnd* pViewWnd = m_podMsgSgWnd->GetWorkingView();
                    if(m_pomMsgSgDetViews[CAN] == pViewWnd)//check for Signal view
                    {
                        m_pomMsgSgDetViews[CAN]->vEditSignalName();
                    }
                    else if(m_pomMsgSgTreeViews[CAN] == pViewWnd)//check for Treeview
                    {
                        m_pomMsgSgTreeViews[CAN]->SendMessage(WM_COMMAND, IDM_EDIT_MSG,0);
                    }
                }
                //else if(m_podMsgSgWndLin == pFrmWnd)//check for LIN Database
                //            {
                //                CWnd* pViewWnd = m_podMsgSgWndLin->GetWorkingView();
                //                if(m_pomMsgSgDetViews[LIN] == pViewWnd)//check for Signal view
                //                {
                //                    m_pomMsgSgDetViews[LIN]->vEditSignalName();
                //                }
                //                else if(m_pomMsgSgTreeViews[LIN] == pViewWnd)//check for Treeview
                //                {
                //                    m_pomMsgSgTreeViews[LIN]->SendMessage(WM_COMMAND, IDM_EDIT_MSG,0);
                //                }
                //            }
                else if (m_podMsgSgWndJ1939 == pFrmWnd)//check for J1939 Database
                {
                    //if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
                    CWnd* pViewWnd = m_podMsgSgWndJ1939->GetWorkingView();
                    if(m_pomMsgSgDetViews[J1939] == pViewWnd)//check for Signal view
                    {
                        m_pomMsgSgDetViews[J1939]->vEditSignalName();
                    }
                    else if(m_pomMsgSgTreeViews[J1939] == pViewWnd)//check for Treeview
                    {
                        m_pomMsgSgTreeViews[J1939]->SendMessage(WM_COMMAND, IDM_EDIT_MSG,0);
                    }

                }
                else if(bConnected == FALSE)
                {
                    OnFileConnect();
                }
            }//if (pMsg->wParam == VK_F2)
            else if (pMsg->wParam == VK_ESCAPE)
            {
                if(bConnected == TRUE)
                {
                    OnFileConnect();
                }
            }//else if (pMsg->wParam == VK_ESCAPE)
            //Procees the key "F3" and "F12" for FlexRay
            else if (pMsg->wParam == VK_F3)
            {
                if( m_shFLEXRAYDriverId != DAL_NONE && bConnectedFlexRay == FALSE )
                {
                    OnFlexRayConnect();
                }
            }
            else if (pMsg->wParam == VK_F12)
            {
                if( m_shFLEXRAYDriverId != DAL_NONE && bConnectedFlexRay == TRUE )
                {
                    OnFlexRayConnect();
                }
            }
            // For LIN F4 - Connect and F11 - Disconnect
            else if (pMsg->wParam == VK_F4)
            {
                if( m_shLINDriverId != DAL_NONE && bConnectedLIN == FALSE )
                {
                    OnLINConnect();
                }
            }
            else if (pMsg->wParam == VK_F11)
            {
                if( m_shLINDriverId != DAL_NONE && bConnectedLIN == TRUE )
                {
                    OnLINConnect();
                }
            }
        }
    }
}
BOOL CMainFrame::bParseSignalWatchXMLconfig(ETYPE_BUS eBus, CMainEntryList& odMainEntryList)
{
    BOOL bProper = TRUE;
    if (m_xmlConfigFiledoc != NULL)
    {
        UINT nMainCount = 0;
        xmlXPathObjectPtr pOjectPath = NULL;
        xmlNodePtr pNodePtr = NULL;
        CMsgSignal* pMsgSignal = NULL;
        if(eBus == J1939)
        {
            xmlChar* pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/J1939_Signal_Watch/Message";
            pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
            pMsgSignal = m_pouMsgSigJ1939;
        }
        else
        {
            xmlChar* pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Watch/Message";
            pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
            pMsgSignal = theApp.m_pouMsgSignal;
        }

        if(pOjectPath != NULL)
        {
            xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
            if(pNodeSet != NULL)
            {
                nMainCount = pNodeSet->nodeNr;
                for (UINT i = 0; i < nMainCount; i++)
                {
                    pNodePtr = pNodeSet->nodeTab[i];
                    if(pNodePtr != NULL)
                    {
                        SMAINENTRY sMainEntry;
                        pNodePtr = pNodePtr->children;
                        BOOL bNameFound = FALSE;
                        while(pNodePtr != NULL)
                        {
                            if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Id")) && bNameFound == FALSE)
                            {
                                xmlChar* key = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                if(NULL != key)
                                {
                                    sMainEntry.m_unMainEntryID = atoi((char*)key);
                                    if ( NULL != pMsgSignal && ( TRUE == pMsgSignal->bMessageNameFromMsgCode(sMainEntry.m_unMainEntryID, sMainEntry.m_omMainEntryName)))
                                    {
                                        bNameFound = TRUE;
                                    }
                                    xmlFree(key);

                                }
                            }
                            if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Signal"))&& bNameFound == TRUE)
                            {
                                xmlChar* key = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                if(NULL != key)
                                {
                                    SSUBENTRY sSelEntry;
                                    sSelEntry.m_omSubEntryName = (char*)key;
                                    sMainEntry.m_odSelEntryList.AddTail(sSelEntry);
                                    xmlFree(key);
                                }
                            }

                            pNodePtr = pNodePtr->next;
                        }
                        if(bNameFound == TRUE)
                        {
                            odMainEntryList.AddTail(sMainEntry);
                        }
                    }
                }
            }
            else
            {
                bProper = FALSE;
            }
            xmlXPathFreeObject(pOjectPath);
        }
        else
        {
            bProper = FALSE;
        }
    }
    else
    {
        bProper = FALSE;
    }
    return bProper;
}

// START LIN RELATED HANDLERS


/******************************************************************************/
/*  Function Name    :  OnConfigBaudrateLin                                   */
/*  Input(s)         :                                                        */
/*  Output           :  baudrate                                              */
/*  Functionality    :  This function is called to invoke the CChangeRegisters*/
/*                   dialog box. On selection of menu, this function is called*/
/*  Member of        :  CChangeRegisters                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Manohar H                                             */
/*  Date Created     :  02.05.2013                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CMainFrame::OnConfigBaudrateLIN()
{
    PCHAR pInitData = (PCHAR)m_asControllerDetailsLIN;
    int nSize = sizeof(SCONTROLLER_DETAILS_LIN) * defNO_OF_LIN_CHANNELS;
    //if (g_pouDIL_LIN_Interface->DILL_DisplayConfigDlg(m_asControllerDetailsLIN, nSize) == S_OK)
    //{
    //    //Set Controller to ConfigDetails
    //    //memcpy(m_asControllerDetails, pInitData, nSize);
    //}
    //Update hardware info in status bar
    vUpdateHWStatusInfo();

    //update baudrate details in global statistics buffer
    /* for (int i = 0; i < defNO_OF_LIN_CHANNELS; i++)
    {
        GetILINBusStat()->BSC_SetBaudRate(i, _tstof(m_asControllerDetails[i].m_omStrBaudrate.c_str()));
    } */
}


/**
* \brief         This function pops out the hardware selection dialog
* \param         void
* \return        void
* \authors       Arunkumar Karri
* \date          13.12.2011 Created
*/
void CMainFrame::OnConfigChannelSelectionLIN()
{
    INT nCount = CHANNEL_ALLOWED;
    HRESULT hResult = S_FALSE;

    /* Deselect hardware interfaces if selected */
    hResult = g_pouDIL_LIN_Interface->DILL_DeselectHwInterfaces();

    if (g_pouDIL_LIN_Interface->DILL_ListHwInterfaces(m_asINTERFACE_HW, nCount) == S_OK)
    {
        hResult = g_pouDIL_LIN_Interface->DILL_SelectHwInterfaces(m_asINTERFACE_HW, nCount);
        if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
        {
            /* Updates the number of channels selected */
            m_nNumChannels = nCount;

            //Update hardware info in status bar
            vUpdateHWStatusInfo();

            //Update NW statistics window channel information
            vUpdateChannelInfo();

            // Update controller information
            // g_pouDIL_LIN_Interface->DILL_SetConfigData(m_asControllerDetails, nCount);
        }
    }
    else
    {
        /* Select previously available channels */
        // g_pouDIL_LIN_Interface->DILL_SelectHwInterfaces(m_asINTERFACE_HW, nCount);
    }
}
//void CMainFrame::OnActivateLIN()
//{
//    HRESULT Result = S_FALSE;
//    bool bActivateStatus = false;
//    if ((NULL == sg_pouLinDIL) && (NULL == sg_pouLinLogger))
//    {
//        Result = ProcessLINInterfaces();
//        GetILINNodeSim()->NS_SetLINActivationStatus(true);
//        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_SHOW, (LONG)LIN);
//        bActivateStatus = true;
//    }
//    else
//    {
//        Result = DeselectLINInterfaces();
//        GetILINNodeSim()->NS_SetLINActivationStatus(false);
//        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)LIN);
//    }
//    /* Modify filter icon accordingly in LIN toolbar*/
//    BYTE bytTbrItemIndex = 0;
//    vModifyToolbarIcon( m_wndToolbarLIN, bytTbrItemIndex, bActivateStatus, IDI_ICON_LIN_DEACTIVATE, IDI_ICON_LIN_ACTIVATE );
//
//    ASSERT(Result == S_OK);
//}

void CMainFrame::OnSendMessageLIN()
{
    // Check for SENDMESG flag, If true,
    // make it false and stop sending messages
    // If false, make it true and start sending messages
    CToolBarCtrl& omRefToolBarCtrl = vGetReferenceToToolBarCtrl();
    // Use of this flag can be avoided by using "g_bStopSendMultMsg"
    if ( !theApp.pouGetFlagsPtr()->nGetFlagStatus( SENDMESGLIN ) )
    {

        // Now start sending messages, update flag
        theApp.pouGetFlagsPtr()->vSetFlagStatus( SENDMESGLIN, TRUE );

        // Set the tool bar button to pressed state
        omRefToolBarCtrl.PressButton(ID_TRANSMIT_ACTIVATE_LIN, TRUE);

        m_objTxHandlerLin.vSetTxStopFlag(FALSE);
        //g_bStopMsgBlockTx       = FALSE;
        // Call Message Manager function to start message transmission
        //m_podTxMsgManager->vStartTransmission(0);
        m_objTxHandlerLin.vStartTransmission(0);
    }
    else // Started, Stop the same
    {
        //m_objTxHandler.vSetTxStopFlag(TRUE);
        m_objTxHandlerLin.vStopTransmission(0);
        //g_bStopMsgBlockTx       = TRUE;
        theApp.vDestroyUtilThreads(500, BIT_MULTI_MSG_THREAD );
        // Now start sending messages, update flag
        theApp.pouGetFlagsPtr()->vSetFlagStatus( SENDMESGLIN, FALSE );
        // ReSet the tool bar button to pressed state
        omRefToolBarCtrl.PressButton(ID_TRANSMIT_ACTIVATE_LIN, FALSE);
        // PTV [1.6.4]
        // Disabling Data logging Flag
        /*if (NULL != sg_pouFrameProcCAN)
        {
            sg_pouFrameProcCAN->FPC_DisableDataLogFlag();
        }*/
    }
    // Inform Tx Window about message transmission start if it exists
    if(m_objTxHandlerLin.hConfigWindowShown() == S_OK)
    {
        CFlags* pouFlag = theApp.pouGetFlagsPtr();
        BOOL bTxON = FALSE;
        if(pouFlag != NULL )
        {
            bTxON = static_cast<BOOL> (pouFlag->nGetFlagStatus(SENDMESGLIN));
        }
        eUSERSELCTION eUserSel     = eTXMSGCMD;
        m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bTxON);
    }
}

/******************************************************************************
    Function Name    :  OnCfgSendMsgsLIN

    Input(s)         :  -
    Output           :  void
    Functionality    :  Called by the framework when user wants to configure
                        messages to send the same on the LIN-bus
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Manohar H
    Date Created     :  29.07.2013
******************************************************************************/
void CMainFrame::OnCfgSendMsgsLIN()
{
    m_objFlexTxHandler.vShowConfigureMsgWindow(this, LIN);

    //BOOL bFirstTime = !(m_objTxHandlerLin.hConfigWindowShown() == S_OK);
    //m_objTxHandlerLin.vShowConfigureMsgWindow((void*)this, SW_SHOW);
    //m_objTxHandlerLin.vSetMsgDBPtrInDetailsView((void*)theApp.m_pouMsgSignal);
    //if(bFirstTime == TRUE)
    //{
    //    eUSERSELCTION eUserSel;
    //    //Inform the connect status
    //    CFlags* pouFlags   = theApp.pouGetFlagsPtr();
    //    if(pouFlags != NULL)
    //    {
    //        // Toggle connect/disconnect flag
    //        eUserSel = eCONNECTCMD;
    //        BOOL bConnected = pouFlags->nGetFlagStatus(LIN_CONNECTED);
    //        m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bConnected );
    //        // Hex/Dec format
    //        BOOL bHexON = pouFlags->nGetFlagStatus(HEX);
    //        eUSERSELCTION eUserSel = eHEXDECCMD;
    //        m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bHexON);
    //        // Check for transmission on/off
    //        BOOL bTxOn = pouFlags->nGetFlagStatus(SENDMESG);
    //        eUserSel     = eTXMSGCMD;
    //        m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, bTxOn);
    //    }
    //    // Inform about the database change.
    //    eUserSel = eDATABASEIMPORTCMD;
    //    m_objTxHandlerLin.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel,0);
    //}
}

// START LIN RELATED HELPER FUNCTIONS
HRESULT CMainFrame::ProcessLINInterfaces(void)
{
    HRESULT Result = S_FALSE;

    return Result;
}

HRESULT CMainFrame::DeselectLINInterfaces(void)
{
    HRESULT Result = S_OK;

    return Result;
}
void CMainFrame::OnUpdateLinClusterConfig(CCmdUI* pCmdUI)
{
    CFlags* pouFlag  = theApp.pouGetFlagsPtr();
    if ( (pouFlag!= NULL) && (TRUE == pouFlag->nGetFlagStatus(LIN_CONNECTED)))
    {
        pCmdUI->Enable(FALSE);
    }
    else
    {
        pCmdUI->Enable(TRUE);
    }
}

void CMainFrame::OnLinClusterConfig()
{
    string          strFilePath;
    CStringArray strFilePathArray;
    strFilePath = m_acFlexDBConfigInfo.m_acConfigFileName[0];


    CFibexConfigDlg ouConfig(m_pouMsgSigLIN,m_ouClusterConfig[LIN].m_ouFlexChannelConfig , m_nMaxFlexChannels , LIN, NULL);

    if ( ouConfig.DoModal() == IDOK)
    {

        m_ouClusterConfig[LIN].m_nChannelsConfigured = 1;
        for ( int i = 0 ; i < CHANNEL_ALLOWED; i++ )
        {
            m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i] = ouConfig.m_ouFlexrayChannelConfig[i];
        }
        m_objFlexTxHandler.SetFibexConfig(LIN, m_ouClusterConfig[LIN]);
    }
}

// END LIN RELATED HELPER FUNCTIONS
#ifdef _FLEXRAY_
/******************************************************************************
    Function Name    :  bCreateFlexRayMsgWindow

    Input(s)         :      -
    Output           :  BOOL
    Functionality    :  Creates the message window (an object of
                        CMsgMDIChildWnd class)
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Saravanan
    Date Created     :  20.03.2013
    Modifications    :
******************************************************************************/
BOOL CMainFrame::bCreateFlexRayMsgWindow()
{
    BOOL bReturn = TRUE;

    if (m_podMsgWndThread == NULL)
    {
        m_podMsgWndThread = new CMsgWndThread;
    }

    if (m_podMsgWndThread != NULL)
    {
        //FlexRay Message Window
        m_podMsgWndThread->CreateMsgWnd(m_hWnd, FLEXRAY,
                                        0, NULL);
        m_podMsgWndThread->PostThreadMessage(WM_MODIFY_VISIBILITY, SW_HIDE, (LONG)FLEXRAY);

        //m_podMsgWndThread->vUpdateClientID(FLEXRAY, 0);
    }
    else
    {
        bReturn = FALSE;
    }

    return TRUE;
}
#endif