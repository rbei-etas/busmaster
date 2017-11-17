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
#include "BUSMASTER.h"        // App class definition file
#include "Utility\UtilFunctions.h"

#include "include/XMLDefines.h"
#include "MainFrm.h"            // Main frame class defintion file
#include "Properties.h"         // Properties dialog class definition file

// Special file dialog class to
#include "SplFileDlg.h"         // customise CSplFileDlg brower
#include "MessageAttrib.h"      // Message attribute dlg class defintion file
#include "DataType.h"           // Structure definition file for msg attribute
#include "DriverInfoDlg.h"      // Driver info class definition file
#include "Splash.h"             // splash screen implementation file
#include "Flags.h"              // CFlags class

// UDS Protocol
//#include "DIL_Interface/DILI_UDS_Protocol.h"
#include "UDS_Protocol/UDS_Extern.h"

// For getting name of a permanent exported function from the user-defined DLL.
#include "MessageList.h"
#include "PPageMessage.h"
#include "common.h"
#include "Utility\TimeManager.h"
#include "MsgBufferConfigPage.h"    // For Message Buffer Configuration PPage
#include "MsgFilterConfigPage.h"    // For Filter configuration page
#include "DatabaseDissociateDlg.h"
#include "AppServices_Impl.h"
#include "include/utils_macro.h"
#include "Include/BaseDefs.h"
#include "InterfaceGetter.h"
//#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "Utility/Utility.h"
#include "Utility/Utility_Thread.h"
#include "Utility/WaitIndicator.h"
//#include "DIL_Interface_extern.h"
#include "BaseDIL_CAN.h"
#include "BaseDIL_LIN.h"
#include "BaseDIL_J1939.h"
#include "include/ModuleID.h"
#include "FrameProcessor/FrameProcessor_extern.h"
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "FrameProcessor/BaseFrameProcessor_LIN.h"
#include "FrameProcessor/BaseFrameProcessor_J1939.h"


#include "Replay/Replay_Extern.h"

#include "SignalWatch/SignalWatch_extern.h"
#include "SignalWatch/BaseSignalWatch.h"
#include "SignalWatch/SignalWatch_LIN.h"
#include "Filter/Filter_extern.h"
#include ".\mainfrm.h"
#include "WaveformSelectionDlg.h"

#include "BusStatistics.h"
#include "SigGrphConfigDlg.h"
#include <DataTypes/SigGrphWnd_Datatypes.h>
#include "J1939TimeOutCfg.h"
#include "include/XMLDefines.h"
#include "Utility\MultiLanguageSupport.h"
#include "ConfigMsgDispPropPg.h"
#include "ChannelConfigurationDlg.h"
#include "Include\BaseDefs.h"
#include "IBusmasterPluginInterface.h"
#include "Error.h"
#include "BusMasterNetWork.h"
#include "LinChannelParam.h"
#include "J1939DriverDefines.h"
#include "BusmasterPlugInManager.h"
#include "DataTypes\NodeSimCodeGenerator.h"
#include "MessageWindowSettingsDialog.h"
#include "UI\BusmasterMenuItem.h"
#include "DataTypes\NSCodeGenHelperFactory.h"
#define MSG_GET_CONFIGPATH  10000
#define FromXMLFile 1 //To show the called InitializeDil() is from XmlConfig


// For bus statistics information

extern CCANMonitorApp theApp;       // Application object
CMsgBufFSE<STCANDATA> g_ouCANBufFSE;    //Global CAN buffer
CMsgBufFSE<STLINDATA> g_ouLINBufFSE;    //Global LIN buffer


HWND g_hMainGUI;                     // Main GUI

DWORD g_dwUIThread = 0;

extern BOOL g_bStopSendMultMsg;     // Flag - carried from Utility.h
extern BOOL g_bStopTimerHandlers;   // Flag - carried from Utility.h
extern BOOL g_bStopKeyHandlers;     // Flag - carried from Utility.h
extern BOOL g_bStopErrorHandlers;   // Flag - carried from Utility.h
extern BOOL g_bStopDLLHandlers;
extern BOOL g_bStopMsgHandlers;


extern BOOL gbMsgTransmissionOnOff(BOOL bOnOff,HMODULE hModule);

BOOL g_bStopSelectedMsgTx ;

// Flag for Msg Handler status
extern BOOL g_bMsgHandlerON;
// Message Handler Related
// To stop all message handler threads
extern void gvStopMessageHandlerThreads();
extern UINT g_unDllMsgCnt;
CRITICAL_SECTION g_omDllMsgCritiSec;
extern UINT g_unWriteDllMsg;

static CBaseFrameProcessor_CAN* sg_pouFrameProcCAN = nullptr; // CAN logger interface
CBaseDIL_CAN* g_pouDIL_CAN_Interface = nullptr; // CAN DIL interface

static CBaseSignalWatch* sg_pouSWInterface[BUS_TOTAL] = {nullptr}; // SIGNAL WATCH INTERFACE

static CBaseFrameProcessor_LIN* sg_pouFrameProcLIN = nullptr; // CAN logger interface
CBaseDIL_LIN* g_pouDIL_LIN_Interface = nullptr; // LIN DIL interface



DWORD g_dwClientID = 0;
extern BOOL g_bReadDllMsg;
HANDLE g_hSemaphore = nullptr;

// Related to J1939
static CBaseFrameProcessor_J1939* sg_pouIJ1939Logger = nullptr; // Logger interface
static CBaseDILI_J1939* sg_pouIJ1939DIL = nullptr; // DIL interface

// Related to LIN
static CBaseFrameProcessor_LIN* sg_pouLinLogger = nullptr; // Logger interface
static CBaseDIL_LIN* sg_pouLinDIL = nullptr; // DIL interface

//J1939 Database editor window
BOOL CMsgSignalDBWnd::sm_bValidJ1939Wnd = FALSE;
SMSGENTRY* CTxMsgWndJ1939::m_psMsgRoot = nullptr;

#define defCONFIGFILTER         _("BUSMASTER Configuration files(*.cfx)|*.cfx||")
#define defFILEEXT              "cfx"
#define defDLGFLAGS             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST
#define defDEFAULTDLLFILENAME   "*.dll"
#define TIMER_REFRESH_MAINGUI   0x100
#define TIMER_REFRESH_LOG_STATUS 0x101
#define TIMER_REFRESH_J1939_LOG_STATUS 0x102
#define TIMER_REFRESH_LIN_LOG_STATUS 0x103
#define STSBAR_REFRESH_TIME_PERIOD      1000  // in milliseconds
#define STSBAR_REFRESH_TIME_PERIOD_LOG  1000  // in milliseconds
#define PROFILE_CAN_MONITOR "Screen"


#define defCONFIG_MSG_DISPLAY_CAN "CAN"
#define defCONFIG_MSG_DISPLAY_LIN "LIN"
#define defCONFIG_MSG_DISPLAY_J1939 "J1939"

#define	USE_MSG_AUTOIT_REQUEST				WM_USER + 1000

enum
{
    DRIVER_CAN_STUB = 0,
    DRIVER_CAN_PEAK_USB,
    DRIVER_CAN_ICS_NEOVI,
    DRIVER_CAN_ETAS_BOA,
    DRIVER_CAN_ETAS_ES581,
    DRIVER_CAN_ETAS_ES5814,
	DRIVER_CAN_ETAS_ES5821,
    DRIVER_CAN_VECTOR_XL,
    DRIVER_CAN_KVASER_CAN,
    DRIVER_CAN_MHS,
    DRIVER_CAN_NSI,
    DRIVER_CAN_IXXAT,
    DRIVER_CAN_VSCOM,
    DRIVER_CAN_IVIEW,
    DRIVER_CAN_ISOLAR,
    DIL_TOTAL,          // Its value must be <= MAX_DILS
    DAL_NONE = ~0x0
};

enum
{
    DRIVER_LIN_ISOLAR_EVE_VLIN = 0,
    DRIVER_LIN_VECTOR_XL,
    DRIVER_LIN_ETAS_BOA,
    DRIVER_LIN_PEAK_USB,
    DRIVER_LIN_KVASER,
    DIL_LIN_TOTAL,
    DAL_LIN_NONE = ~0x0
};



IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
    ON_WM_CREATE()
    ON_COMMAND(IDM_CONFIGURE_DATABASE_OPENACTIVE, OnOpenDatabase)
    ON_COMMAND(IDM_CONFIGURE_DATABASE_CLOSE, OnCloseDatabase)
    ON_MESSAGE(IDM_CONFIGURE_DATABASE_CLOSE, OnCloseDatabase)
	ON_COMMAND(IDM_FILE_IMPORT_DATABASE, OnImportDatabase)

    ON_COMMAND(IDM_CONFIGURE_LIN_CHANNEL, OnConfigLinChannel)
    ON_COMMAND(IDM_CONFIGURE_DATABASE_NEW, OnNewDatabase)
    ON_COMMAND(IDM_CONFIGURE_DATABASE_SAVEAS, OnConfigDatabaseSaveAs)
    ON_COMMAND(IDM_CONFIGURE_DATABASE_SAVE, OnConfigDatabaseSave)
    ON_COMMAND(IDM_CONFIGURE_MESSAGE, OnConfigMessageDisplay)
    ON_COMMAND(ID_CONFIGURE_MESSAGEDISPLAY_LIN, OnConfigMessageDisplayLin)
    ON_COMMAND(IDM_REPLAY_SKIP, OnReplaySingleSkip)
    ON_COMMAND(IDM_REPLAY_STEP, OnReplaySingleStep)
    ON_COMMAND(IDM_REPLAY_STOP, OnReplayStop)
    ON_COMMAND(IDM_REPLAY_GO, OnReplayGo)
    
    ON_COMMAND(IDM_SIGNALWATCH_ADDSIGNAL, OnAddSignalToSignalWindow)
    ON_COMMAND(IDM_SIGNALWATCH_ADDSIGNAL_LIN, OnAddSignalToSignalWindow_LIN)

    ON_COMMAND(IDM_SCHEDULE_CONFIG_LIN, vOnLINScheduleTableConfig)
    ON_UPDATE_COMMAND_UI(IDM_SCHEDULE_CONFIG_LIN, OnUpdateLINScheduleTableConfig)
    ON_COMMAND(IDM_FILTER_MESSAGEFILTEROFF, OnMessageFilterButton)
    ON_COMMAND(IDM_FILTER_REPLAYFILTEROFF, OnReplayFilter)
    ON_COMMAND(IDM_FILTER_LOGFILTEROFF, OnLogFilter)
    ON_COMMAND(IDM_FILTER_LOGLINOFF, OnLogFilterLIN)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_LOGLINOFF, OnUpdateLogFilterLIN)
    ON_COMMAND(IDM_FILTER_MESSAGE_SELECTMESSAGES, OnSelectMessage)
    ON_COMMAND(IDM_CONFIGURE_FILTER_LIN, OnLINFilter)
    ON_COMMAND(IDM_APP_ABOUT, OnAboutApplication)
    ON_COMMAND(IDM_LOG_ON_OFF, OnLogEnable)
    ON_COMMAND(IDM_LOG_LIN_ON_OFF, OnLog_LIN_Enable)
    ON_UPDATE_COMMAND_UI(IDM_LOG_LIN_ON_OFF, OnUpdateLogOnOff_LIN)
    ON_COMMAND(IDR_TOOL_HEXDEC, OnHex_DecButon)
    ON_COMMAND(IDR_TOOL_BUTTON_SIGNAL_WATCH, OnButtonSignalWatchButton)
    ON_COMMAND(IDR_TOOL_BUTTON_SIGNAL_WATCH_LIN, OnButtonSignalWatchButton_LIN)

    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_DATABASE_CLOSE, OnUpdateConfigureDatabaseClose)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_DATABASE_NEW, OnUpdateConfigureDatabaseNew)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_DATABASE_SAVE, OnUpdateConfigureDatabaseSave)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_DATABASE_SAVEAS, OnUpdateConfigureDatabaseSaveas)
    ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(IDM_FILTER_LOGFILTEROFF, OnUpdateLogFilter)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_REPLAYFILTEROFF, OnUpdateReplayFilter)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_MESSAGEFILTEROFF, OnUpdateMessageFilterButton)
    ON_COMMAND(IDM_FILTER_LOGFILTEROFF_BUTTON, OnLogFilter)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_LOGFILTEROFF_BUTTON, OnUpdateLogFilter)
    ON_COMMAND(IDM_FILTER_MESSAGEFILTEROFF_BUTTON, OnMessageFilterButton)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_MESSAGEFILTEROFF_BUTTON, OnUpdateMessageFilterButton)
    
	//Clear
	ON_COMMAND_RANGE(IDM_CLEAR_MSG_WINDOW_START, IDM_CLEAR_MSG_WINDOW_END, OnClearMsgWindow)
	//Absolute
	ON_COMMAND_RANGE(IDM_ABSOLUTE_TIME_MSG_WINDOW_START, IDM_ABSOLUTE_TIME_MSG_WINDOW_END, OnDisplayAbsoluteTime)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_ABSOLUTE_TIME_MSG_WINDOW_START, IDM_ABSOLUTE_TIME_MSG_WINDOW_END, OnUpdateDisplayAbsolutetime)
	
	//Relative
	ON_COMMAND_RANGE(IDM_RELATIVE_TIME_MSG_WINDOW_START, IDM_RELATIVE_TIME_MSG_WINDOW_END, OnDisplayRelativetime)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_RELATIVE_TIME_MSG_WINDOW_START, IDM_RELATIVE_TIME_MSG_WINDOW_END, OnUpdateDisplayRelativetime)
	
	//System
	ON_COMMAND_RANGE(IDM_SYSTEM_TIME_MSG_WINDOW_START, IDM_SYSTEM_TIME_MSG_WINDOW_END, OnDisplaySystemTime)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_SYSTEM_TIME_MSG_WINDOW_START, IDM_SYSTEM_TIME_MSG_WINDOW_END, OnUpdateDisplaySystemTime)
	
	//Overwrite
	ON_COMMAND_RANGE(IDM_OVERWRITE_MSG_WINDOW_START, IDM_OVERWRITE_MSG_WINDOW_END, OnDisplayMessagewindowOverwrite)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_OVERWRITE_MSG_WINDOW_START, IDM_OVERWRITE_MSG_WINDOW_END, OnUpdateDisplayMessagewindowOverwrite)
    
	//Interpret
	ON_COMMAND_RANGE(IDM_INTERPRETE_MSG_WINDOW_START, IDM_INTERPRETE_MSG_WINDOW_END, OnMessageInterpretation)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_INTERPRETE_MSG_WINDOW_START, IDM_INTERPRETE_MSG_WINDOW_END, OnUpdateMessageInterpret)

	ON_UPDATE_COMMAND_UI(IDR_TOOL_HEXDEC, OnUpdateToolHexdec)
    ON_UPDATE_COMMAND_UI(IDM_LOG_ON_OFF, OnUpdateLogOnOff)
    ON_UPDATE_COMMAND_UI(IDM_REPLAY_GO, OnUpdateReplayGo)
    ON_UPDATE_COMMAND_UI(IDM_REPLAY_SKIP, OnUpdateReplaySkip)
    ON_UPDATE_COMMAND_UI(IDM_REPLAY_STEP, OnUpdateReplayStep)
    ON_UPDATE_COMMAND_UI(IDM_REPLAY_STOP, OnUpdateReplayStop)
    //UDS
    ON_COMMAND(IDM_UDS,OnCfgnUdsMainWnd)
    ON_COMMAND(IDD_SETTINGS_UDS,OnCfgnUdsSettingsWnd)
    ON_WM_SHOWWINDOW()
    ON_WM_DESTROY()
    ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
    ON_COMMAND(IDM_CFGN_LOG, OnCfgLogFile)
    ON_COMMAND(IDM_CFGN_LIN_LOG, OnCfgLogFile_LIN)
    ON_UPDATE_COMMAND_UI(IDR_TOOL_BUTTON_MSG_DISP, OnUpdateToolButtonMsgDisp)
    ON_WM_ENDSESSION()
    ON_COMMAND(IDM_CFGN_SEND_MSGS, OnCfgSendMsgs)
    ON_COMMAND(IDM_FILE_CONNECT, OnFileConnect)
    ON_UPDATE_COMMAND_UI(IDM_FILE_CONNECT, OnUpdateFileConnect)
    
    
    ON_COMMAND(IDM_LIN_CONNECT, OnLINConnect)
    ON_UPDATE_COMMAND_UI(IDM_LIN_CONNECT, OnUpdateLINConnect)
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
    ON_COMMAND(IDM_CONFIGURE_ACTIVE, OnConfigureModeActive)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_ACTIVE, OnUpdateConfigureModeActive)
    ON_WM_TIMER()
    ON_COMMAND(IDM_NETWORK_STATISTICS_WND_CAN,OnStatisticsCAN)
    ON_COMMAND(IDM_NETWORK_STATISTICS_WND_LIN,OnStatisticsLIN)

    ON_COMMAND(IDM_CONFIGURE_PASSIVE, OnConfigurePassive)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_PASSIVE, OnUpdateConfigurePassive)
    ON_COMMAND(IDM_TRACE_WND, OnTraceWnd)
    ON_MESSAGE(IDM_TRACE_WND, OnMessageTraceWnd)
    ON_UPDATE_COMMAND_UI(IDM_TRACE_WND, OnUpdateTraceWnd)
    ON_UPDATE_COMMAND_UI(IDM_CONFIGURE_BAUDRATE, OnUpdateConfigureBaudrate)
    
    
    ON_WM_SIZE()
    ON_UPDATE_COMMAND_UI(IDR_TOOL_BUTTON_SIGNAL_WATCH, OnUpdateSignalWatchWnd)
    ON_UPDATE_COMMAND_UI(IDR_TOOL_BUTTON_SIGNAL_WATCH_LIN, OnUpdateSignalWatchWnd_LIN)

    ON_COMMAND(IDM_GRAPH_WND, OnGraphWindow)
    ON_UPDATE_COMMAND_UI(IDM_GRAPH_WND, OnUpdateGraphWnd)
    ON_COMMAND(IDM_CFGN_REPLAY, OnCfgnReplay)
    ON_UPDATE_COMMAND_UI(IDM_CFGN_REPLAY, OnUpdateCfgnReplay)
    ON_COMMAND(IDM_CONFIGURE_SIMULATEDSYSTEMS, OnConfigureSimulatedsystems)
    ON_COMMAND(IDM_CONFIGURE_SIMULATEDSYSTEMS_LIN, OnConfigureSimulatedsystemsLin)
    ON_COMMAND_RANGE(IDC_SELECT_DRIVER,IDC_SELECT_DRIVER + 14, OnSelectDriver)
    ON_UPDATE_COMMAND_UI_RANGE(IDC_SELECT_DRIVER,IDC_SELECT_DRIVER + 14, OnUpdateSelectDriver)
    
    
    ON_COMMAND_RANGE(IDC_SELECT_LIN_DRIVER,IDC_SELECT_LIN_DRIVER + 5, OnSelectLINDriver)
    ON_UPDATE_COMMAND_UI_RANGE(IDC_SELECT_LIN_DRIVER,IDC_SELECT_LIN_DRIVER + 5, OnUpdateSelectLINDriver)
    ON_COMMAND(ID_HELP_FINDER, CMDIFrameWndEx::OnHelpFinder)
    ON_COMMAND(ID_HELP, CMDIFrameWndEx::OnHelp)
    ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWndEx::OnContextHelp)
    ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWndEx::OnHelpFinder)
    ON_MESSAGE(WM_PROCESS_ERROR_MESSAGE, OnErrorMessageProc)
    ON_UPDATE_COMMAND_UI(ID_ACTIVE_DATABASE_NAME, OnUpdateConfigurationFileName)
    ON_MESSAGE(WM_FILE_DISCONNECT,vDisconnect)
    ON_MESSAGE(WM_SET_WARNING_LIMIT_VAR,vSetWarningLimitVar)
    ON_MESSAGE(WM_KEY_PRESSED_MSG_WND,vKeyPressedInMsgWnd)
    ON_MESSAGE(WM_NOTIFICATION_FROM_OTHER, vNotificationFromOtherWin)
    ON_MESSAGE(WM_CREATE_TRACEWIN, vCreateTraceWindow)
    ON_WM_HELPINFO()
    ON_COMMAND(IDM_DATABASE_DISSOCIATE_DB, OnDissociateDatabase)
    ON_COMMAND(IDM_SAVE_IMPORT, OnSaveImportDatabase)
    ON_UPDATE_COMMAND_UI(IDM_SAVE_IMPORT, OnUpdateSaveImportDatabase)
    ON_MESSAGE(WM_GET_DB_PTR, OnProvideMsgDBPtr)
    ON_MESSAGE(WM_GET_MSG_NAME_FROM_CODE, OnProvideMsgNameFromCode)
    ON_MESSAGE(WM_GET_PGN_NAME_FROM_CODE, OnProvidePGNNameFromCode)

    ON_UPDATE_COMMAND_UI(IDM_CFGN_LOG, OnUpdateCfgnLog)
    ON_UPDATE_COMMAND_UI(IDM_CFGN_LIN_LOG, OnUpdateCfgnLog_LIN)
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
    ON_COMMAND(33077, OnJ1939ConfigureTimeouts)
    ON_UPDATE_COMMAND_UI(33077, OnUpdateJ1939Timeouts)
    ON_COMMAND(33079, OnJ1939DBNew)
    ON_UPDATE_COMMAND_UI(33079, OnUpdateJ1939DBNew)
    ON_COMMAND(33080, OnJ1939DBOpen)
    ON_COMMAND(ID_DATABASE_CLOSE, OnJ1939DBClose)
    ON_UPDATE_COMMAND_UI(ID_DATABASE_CLOSE, OnUpdateJ1939DBClose)
    ON_MESSAGE(ID_DATABASE_CLOSE, OnJ1939DBClose)
    ON_COMMAND(ID_DATABASE_SAVE, OnJ1939DBSave)
    ON_UPDATE_COMMAND_UI(ID_DATABASE_SAVE, OnUpdateJ1939DBSave)
	ON_COMMAND(ID_J1939_DATABASE_ASSOCIATE, OnJ1939DBAssociate)
	ON_COMMAND(ID_J1939_DATABASE_DISSOCIATE, OnJ1939DBDissociate)
    ON_COMMAND(ID_CONFIGURE_SIMULATEDSYSTEMS, OnJ1939CfgSimSys)
    ON_UPDATE_COMMAND_UI(33085, OnUpdateJ1939CfgSimSys)
    ON_COMMAND(ID_SIGNALWATCH_ADD, OnJ1939SignalwatchAdd)
    ON_COMMAND(ID_SIGNALWATCH_SHOWWINDOW, OnJ1939SignalwatchShow)
    ON_UPDATE_COMMAND_UI(ID_SIGNALWATCH_SHOWWINDOW, OnUpdateJ1939SignalwatchShow)
    ON_COMMAND(ID_CONFIGURE_MESSAGEDISPLAY_J1939, OnConfigureMessagedisplayJ1939)

    ON_COMMAND_RANGE(ID_SHOWMESSAGEWINDOW_CAN,ID_SHOWMESSAGEWINDOW_LIN, OnShowHideMessageWindow)
    ON_UPDATE_COMMAND_UI_RANGE(ID_SHOWMESSAGEWINDOW_CAN,ID_SHOWMESSAGEWINDOW_LIN, OnUpdateShowHideMessageWindow)
    ON_COMMAND(ID_TESTAUTOMATION_EDITOR, OnAutomationTSEditor)
    ON_COMMAND(ID_TESTAUTOMATION_EXECUTOR, OnAutomationTSExecutor)
    ON_COMMAND(ID_UTILITY_FILE_CONVERTER, OnFileConverter)
    ON_COMMAND(ID_CONFIGURE_CHANNELSELECTION, OnConfigChannelSelection)
    ON_UPDATE_COMMAND_UI(ID_CONFIGURE_CHANNELSELECTION, OnUpdateConfigChannelSelection)
    ON_MESSAGE(WM_KEYBOARD_CHAR, OnReceiveKeyBoardData)
    ON_MESSAGE(WM_KEYBOARD_KEYDOWN, OnReceiveKeyDown)
    ON_MESSAGE(MSG_GET_CONFIGPATH, onGetConfigPath)
    ON_MESSAGE(WM_J1939_TX_CLOSE_MSG, onJ1939TxWndClose)
   
    
    ON_COMMAND(ID_TRANSMIT_CONFIGURE_LIN, OnCfgSendMsgsLIN)
    ON_COMMAND(ID_LIN_CLUSTER_CONFIG, OnLinClusterConfig)
    ON_UPDATE_COMMAND_UI(ID_LIN_CLUSTER_CONFIG, OnUpdateLinClusterConfig)
    ON_COMMAND(IDM_LDF_EDITOR_LIN, OnLDFEditor)
    ON_COMMAND(IDM_FILTER_MSGLINOFF, OnMessageFilterButtonLin)
    ON_UPDATE_COMMAND_UI(IDM_FILTER_MSGLINOFF, OnUpdateMessageFilterButtonLin)

    ON_MESSAGE(WM_USER_NSCODEGEN, OnNodeSimCodeGenStatus)


	
	ON_REGISTERED_MESSAGE(AFX_WM_ON_RIBBON_CUSTOMIZE, OnRibbonCustomize)

    //Plugin
    ON_COMMAND_RANGE( IDR_PLUGIN_MENU_START, IDR_PLUGIN_MENU_END, onPluginMenuClicked )
    ON_UPDATE_COMMAND_UI_RANGE( IDR_PLUGIN_MENU_START, IDR_PLUGIN_MENU_END, onPluginMenuUpadate )

	//Temp - For Autoit scripts.
	ON_MESSAGE(USE_MSG_AUTOIT_REQUEST, OnAutoItRequest)
	//ON_UPDATE_COMMAND_UI(ID_RIBBON_HIDE, &CMainFrame::OnUpdateRibbonHide)
    ON_COMMAND(ID_BUTTON_TOGGLERIBBON, &CMainFrame::OnButtonToggleribbon)
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

CMainFrame::CMainFrame()
{

    m_oNetworkStatistics = nullptr;
    GetCurrentDirectory(MAX_PATH, theApp.m_acApplicationDirectory);// Get application directory
    
    m_nMaxLinChannels = 1;
    m_podMsgSgWnd                   = nullptr;
    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        m_pomMsgSgDetViews[i]       = nullptr;
        m_pomMsgSgTreeViews[i]      = nullptr;
    }

    m_podMsgWndThread               = nullptr;

    m_bIsNewDatabase                = FALSE;
    m_nIconSetFlag = 0;
    m_nJ1939IconSetFlag = 0;
    m_nSendMsgLogCnt = 0;
    m_nSendMsgJ1939LogCnt = 0;

    m_unReplayTimeDelay             = 50;
    m_unCycleTimeDelay              = 0;
    m_unTimerSB                     = 0;
    m_omStrPrevLoadedDll            = "";
    m_bIsSendingMsg                 = FALSE;
    m_bMessageName                  = TRUE;
    //m_bDateActive                   = FALSE;
    m_bAbortMsgHandler              = FALSE;
    m_bMsgHandlerRxDataByte         = FALSE;
    m_bNoHardwareFound              = true;
    // Clear all MRU strings

    // Config menu option - default
    m_bCfgNewMenuOption     = TRUE;
    m_bCfgLoadMenuOption    = TRUE;
    m_bCfgSaveMenuOption    = TRUE;
    m_bCfgSaveAsMenuOption  = TRUE;
    
    m_bIsStatWndCreated = FALSE;
    m_bNotificWndVisible       = FALSE;
    // Initially the controller will be in error active mode.
    for( UINT unIndex = 0; unIndex < defNO_OF_CHANNELS; unIndex++)
    {
        m_eCurrErrorState[ unIndex ] = ERROR_ACTIVE;
    }

    m_unWarningLimit  = 96;
    m_podUIThread = nullptr;
    m_omStrSavedConfigFile = "";
    m_unTimerSBLog = 0;
    m_unJ1939TimerSBLog = 0;

    // Graph related pointers
    // Graph window status
    for(int nBUSID = CAN; nBUSID<AVAILABLE_PROTOCOLS; nBUSID++)
    {
        if(nBUSID == 5)
        {
            //TODO: When Signal graph for ETHERNET is implemented remove this check
            //Skip Ethernet protocol
            continue;
        }
        m_sGraphWndPlacement[nBUSID].length = 0;
        m_sGraphWndPlacement[nBUSID].rcNormalPosition.top = -1;
        m_sGraphSplitterPos[nBUSID].m_nRootSplitterData[0][0] = -1;
    }

    // Init Bus Statistics pointer
    m_podBusStatistics = nullptr;

    //Initialize notific window position
    m_sNotificWndPlacement.length = 0;
    m_sNotificWndPlacement.rcNormalPosition.top = -1;

    //Default Hw mode
    m_byControllerMode = defMODE_ACTIVE;
    char acTmp[MAX_PATH] = {'\0'};
    GetCurrentDirectory(MAX_PATH, acTmp);
    m_omAppDirectory = acTmp;
    m_bInterPretMsg = FALSE;
    m_pouMsgInterpretBuffer = nullptr;
    m_objTxHandler.vLoadTx_DLL();
    m_objSigGrphHandler.vLoadSigGrph_DLL();
    m_pouMsgInterpretBuffer = m_objSigGrphHandler.vGetGraphBuffer();


    for ( ETYPE_BUS eBus = CAN; eBus != BUS_TOTAL; )
    {
        // Append Buffer Size
        m_anMsgBuffSize[eBus][defAPPEND_DATA_INDEX] = 0;
        // Overwrite Buffer Size
        m_anMsgBuffSize[eBus][defOVERWRITE_DATE_INDEX] = 0;
        // Display Update Rate
        m_anMsgBuffSize[eBus][defDISPLAY_UPDATE_DATA_INDEX] = 0;
        eBus = static_cast<ETYPE_BUS>( eBus + 1 );
    }

    m_sMsgWndPlacement.length = 0;//MSG_WND_PLACEMENT
    m_sMsgInterpretPlacement.length = 0;
    m_dwDriverId = 0; //Plugin;
    m_pouTxMsgWndJ1939 = nullptr;
    m_sJ1939ClientParam.m_byAddress = ADDRESS_NULL;
    m_ouGraphReadThread.m_hActionEvent = nullptr;
    m_ouGraphReadThread.m_unActionCode = IDLE;
    m_nTimeStamp = 0;
    m_podMsgSgWndJ1939 = nullptr;
    m_pouMsgSigJ1939   = nullptr;
    m_pouActiveDbJ1939 = nullptr;
    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        m_abLogOnConnect[i] = FALSE;
    }

    m_objTSEditorHandler.vLoadTSEditor_DLL();
    m_objTSExecutorHandler.vLoadTSExecutor_DLL();

    m_xmlConfigFiledoc = nullptr;
    m_bIsXmlConfig = TRUE;
    m_pXmlNodeBusStats = nullptr;

    // language support
    vGettextBusmaster();

    m_bLINDisconnect = false;
   
    m_bFlxDILChanging = FALSE;

    m_shLINDriverId = -1;

    for ( int i =0; i < defNO_OF_LIN_CHANNELS; i++ )
    {
        m_asControllerDetailsLIN[i].m_strHwUri.reserve(MAX_PATH);
    }
    vLoadBusmasterKernel();

    //m_ouBusmasterNetwork = new BMNetwork();
}

CMainFrame::~CMainFrame()
{
    DELETE_PTR(m_podMsgWndThread);
    if (m_pouMsgSigJ1939 != nullptr)
    {
        m_pouMsgSigJ1939->bDeAllocateMemory("");
    }
    DELETE_PTR(m_pouMsgSigJ1939);

    if (m_pouActiveDbJ1939 != nullptr)
    {
        m_pouActiveDbJ1939->bDeAllocateMemoryInactive();
    }
    DELETE_PTR(m_pouActiveDbJ1939);

    if (m_pouTxMsgWndJ1939 != nullptr)
    {
        m_pouTxMsgWndJ1939->DestroyWindow();
    }
    DELETE_PTR(m_pouTxMsgWndJ1939);
    CTxMsgWndJ1939::vClearDataStore();

    
    ReleaseBusStat(CAN);
    ReleaseBusStat(LIN);

    ReleaseLogger(eID_COMPONENT::FRAMEPROC_CAN);
    ReleaseLogger(eID_COMPONENT::FRAMEPROC_J1939);
    ReleaseLogger(eID_COMPONENT::FRAMEPROC_LIN);

    m_objTSExecutorHandler.vDoDeInitailization(CAN);
    if (nullptr != mPluginManager)
    {
        delete mPluginManager;
        mPluginManager = nullptr;
    }
    if (nullptr != m_ouBusmasterNetwork)
    {
        delete m_ouBusmasterNetwork;
    }
    if (nullptr != mBusmasterKernel)
    {
        delete mBusmasterKernel;
    }
}



/******************************************************************************
    Functionality    :  National Language Support
******************************************************************************/
void CMainFrame::vGettextBusmaster()
{
    setlocale(LC_ALL, "");
    bindtextdomain("BUSMASTER", "./Localization/locale/");
    textdomain("BUSMASTER");
}
static bool bIsMsgExist(UINT MsgId, const SMSGENTRY* psFromList, sMESSAGE*& psMsg)
{
    bool bResult = false;
    if (psFromList != nullptr)
    {
        const SMSGENTRY* pTempMsgEntry = psFromList;
        while ((pTempMsgEntry != nullptr))
        {
            if (pTempMsgEntry->m_psMsg->m_unMessageCode == MsgId)
            {
                psMsg = pTempMsgEntry->m_psMsg;
                bResult = true;
                break;
            }
            pTempMsgEntry = pTempMsgEntry->m_psNext;
        }
    }
    return bResult;
}

static bool bIsSigExist(const CString& omSigName, sSIGNALS* pSigList, sSIGNALS*& pSig)
{
    bool bResult = false;
    while (pSigList != nullptr)
    {
        if (omSigName == pSigList->m_omStrSignalName)
        {
            pSig = pSigList;
            bResult = true;
            break;
        }
        pSigList = pSigList->m_psNextSignalList;
    }
    return bResult;
}
static void vPopulateMainEntryList( CMainEntryList* podResultingList, const SMSGENTRY* psExistingList,
                                    IBMNetWorkGetService* pouDatabase )
{
    if ( ( podResultingList != nullptr ) && ( pouDatabase != nullptr ) )
    {
        std::list<IFrame*> frameList;
        std::string name;
        pouDatabase->GetFrameList( CAN, 0, frameList );

for ( auto itrFrame : frameList )
        {
            SMAINENTRY sMainEntry;
            {
                itrFrame->GetFrameId( sMainEntry.m_unMainEntryID );
                //sMainEntry.m_unMainEntryID = pMsg->m_unMessageCode;
                itrFrame->GetName( name );
                sMainEntry.m_omMainEntryName = name.c_str();
                sMESSAGE* pListMsg = nullptr;
                if ( bIsMsgExist( sMainEntry.m_unMainEntryID, psExistingList, pListMsg ) )
                {
                    std::map<ISignal*, SignalInstanse> signalList;
                    itrFrame->GetSignalList( signalList );
                    //sSIGNALS* pSig = pMsg-> m_psSignals;
for ( auto pSig : signalList )
                    {
                        SSUBENTRY sSubEntry;
                        pSig.first->GetName( name );
                        sSubEntry.m_omSubEntryName = name.c_str();
                        sSIGNALS* pJunk = nullptr;
                        if ( bIsSigExist( name.c_str(), pListMsg->m_psSignals, pJunk ) )
                        {
                            sMainEntry.m_odSelEntryList.AddTail( sSubEntry );
                        }
                        else
                        {
                            sMainEntry.m_odUnSelEntryList.AddTail( sSubEntry );
                        }
                    }
                }
                else //If Msg does not exist copy the full list directly
                {
                    std::map<ISignal*, SignalInstanse> signalList;
                    itrFrame->GetSignalList( signalList );
for ( auto pSig : signalList )
                    {
                        SSUBENTRY sSubEntry;
                        pSig.first->GetName( name );
                        sSubEntry.m_omSubEntryName = name.c_str();
                        sMainEntry.m_odUnSelEntryList.AddTail( sSubEntry );
                    }
                }
                podResultingList->AddTail( sMainEntry );
            }
        }//End of msg for loop
        //delete the ID array
    }
}

static void vPopulateMainEntryList(CMainEntryList* podResultingList, const SMSGENTRY* psExistingList,
                                   CMsgSignal* pouDatabase)
{
    if ((podResultingList != nullptr) && (pouDatabase != nullptr))
    {
        UINT nNoMsgs = pouDatabase->unGetNumerOfMessages();
        UINT* pMsgIds = new UINT[nNoMsgs];
        pouDatabase->unListGetMessageIDs(pMsgIds);
        for (UINT i = 0; i < nNoMsgs; i++)
        {
            SMAINENTRY sMainEntry;
            sMESSAGE* pMsg = pouDatabase->psGetMessagePointer(pMsgIds[i]);
            if (pMsg != nullptr)
            {
                sMainEntry.m_unMainEntryID = pMsg->m_unMessageCode;
                sMainEntry.m_omMainEntryName = pMsg->m_omStrMessageName;
                sMESSAGE* pListMsg = nullptr;
                if (bIsMsgExist(pMsg->m_unMessageCode, psExistingList, pListMsg))
                {
                    sSIGNALS* pSig = pMsg->m_psSignals;
                    while (pSig != nullptr)
                    {
                        SSUBENTRY sSubEntry;
                        sSubEntry.m_omSubEntryName = pSig->m_omStrSignalName;
                        sSIGNALS* pJunk = nullptr;
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
                    while (pSig != nullptr)
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
        pMsgIds = nullptr;
    }
}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& createStruct)
{
    createStruct.style |= WS_VSCROLL | WS_HSCROLL;
    if (!CMDIFrameWndEx::PreCreateWindow(createStruct))
    {
        return FALSE;
    }
    return TRUE;
}
/******************************************************************************
 Functionality    : #Called by the framework while a window is being created
                    #Creates toolbar, status bar.
                    #Creates combo box in the menu.
                    #Creates Dropdwon menu for signal watch window.
                    #Creates Dropdwon menu for time mode.
 * \req      RS_FLX_14
 * \design   DS_FLX_14
 * \codetag  CT_FLX_14
/******************************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    OnApplicationLook(ID_VIEW_APPLOOK_OFF_2007_BLUE);
    if ( nullptr != m_ouBusmasterNetwork && ( false == m_ouBusmasterNetwork->isDbManagerAvailable()) )
    {
        MessageBox("Unable to Load Database Manager.\nPlease Reinstall BUSMASTER", "Error", MB_OK|MB_ICONERROR);
        return -1;
    }

    //TestMain();


    vGetWinStatus(m_WinCurrStatus);
    // Update the window placement
    SetWindowPlacement(&m_WinCurrStatus);
	//mMenuBar.Create(this);
	//mDefaultMenu = mMenuBar.GetHMenu();
	//mMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	mRibbonBar.Create(this);
	mRibbonBar.LoadFromResource(IDR_BUSMASTER_RIBBON);
    mRibbonBar.UpdateRibbonBarMinimisedIcon();
    
    
    EnableDocking(CBRS_ALIGN_ANY);

    //To Initilaise GCC Version
    std::string strVersion;
    GetCurrentGccVersion(strVersion);

	EnableDocking(CBRS_ALIGN_ANY);

    if (!m_wndStatusBar.CreateEx(this,SBT_TOOLTIPS) ||
            !m_wndStatusBar.SetIndicators(indicators,
                                          sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0(_("Failed to create status bar\n"));
        return -1;      // fail to create
    }
    
    // Set pane info for displaying active database name
    m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 340);

    m_wndStatusBar.SetPaneInfo( INDEX_CHANNELS, IDS_CHANNELS,
                                SBPS_NOBORDERS, 320);
    m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 340);

    // For Can Logging status
    m_wndStatusBar.SetPaneInfo( INDEX_CAN_LOG_ICON, ID_LOG_RECORD_CAN_ICON,
                                SBPS_NOBORDERS, 100);

    // For J1939 Logging status
    m_wndStatusBar.SetPaneInfo( INDEX_J1939_LOG_ICON, ID_LOG_RECORD_J1939_ICON,
                                SBPS_NOBORDERS, 110);

    m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 340);

    m_wndStatusBar.SetPaneInfo( INDEX_DB_NAME, ID_ACTIVE_DATABASE_NAME,
                                SBPS_STRETCH, 50);

    m_hLogIcon1 = (HICON)::LoadImage(AfxGetInstanceHandle(),
                                     MAKEINTRESOURCE(IDI_ICON_LOG_ON),
                                     IMAGE_ICON, 16, 16, LR_SHARED);

    m_hLogIcon2 = (HICON)::LoadImage(AfxGetInstanceHandle(),
                                     MAKEINTRESOURCE(IDI_ICON_LOG_ONLIGHT),
                                     IMAGE_ICON, 16, 16, LR_SHARED);

    m_hLogOffIcon = (HICON)::LoadImage(AfxGetInstanceHandle(),
                                       MAKEINTRESOURCE(IDI_ICON_LOG_OFF),
                                       IMAGE_ICON, 16, 16, LR_SHARED);

    m_nIconSetFlag = 1;
    m_nJ1939IconSetFlag = 1;
    m_wndStatusBar.SetPaneIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
	m_wndStatusBar.SetPaneIcon(INDEX_J1939_LOG_ICON, m_hLogOffIcon);
	
    // Set number of channels supported
    CString omStrChannels;
    omStrChannels.Format( _(defSTR_CHANNELS_SUPPORTED),
                          CHANNEL_ALLOWED,
                          _(defSTR_CHANNEL_NAME) );
    // Set Channel string
    m_wndStatusBar.SetPaneText(INDEX_CHANNELS, omStrChannels );

    // Create the timer which will be fired to update the status bar.
    m_unTimerSB = SetTimer(TIMER_REFRESH_MAINGUI, STSBAR_REFRESH_TIME_PERIOD,
                           nullptr);

    //EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);
	
	theApp.ReadRecentFileList();
    //vCreateMRU_Menus();

    //Update DIL List
    if (g_pouDIL_CAN_Interface == nullptr)
    {
        //IBusService* service;
        DIL_GetInterface( CAN, (void**)&g_pouDIL_CAN_Interface );
        //g_pouDIL_CAN_Interface = (CBaseDIL_CAN*)service;
    }
    m_nDILCount = g_pouDIL_CAN_Interface->DILC_GetDILList(false, &m_ouList);

    
    if (g_pouDIL_LIN_Interface == nullptr)
    {
        DIL_GetInterface( LIN, (void**)&g_pouDIL_LIN_Interface );
    }
    m_nDILCountLin = g_pouDIL_LIN_Interface->DILL_GetDILList(false, &m_ouListLin);



    mPluginManager = new BusmasterPluginManager();; // TODO::
    mPluginManager->init( this );

	mPluginManager->loadBusPlugins( nullptr );
	//variablelayer
	BusmasterPluginConfiguration info;
	mVariableLayer.setBusmasterInterface(this);
	info.mPluginInterface = &mVariableLayer;
	mVariableLayer.getNotifySink(&info.mNotifyEvent);
	mPluginManager->addPlugin(info);

	/*BusmasterPluginConfiguration info1;
	mLINVariableLayer.setBusmasterInterface(this);
	info1.mPluginInterface = &mLINVariableLayer;
	mLINVariableLayer.getNotifySink(&info1.mNotifyEvent);
	mPluginManager->addPlugin(info1);
*/
	//Load Other Plugins
	mPluginManager->loadPlugins( nullptr );
    mPluginManager->notifyPlugins(eBusmaster_Event::plugin_load_completed, nullptr);



    // Do initialisation for the waveform transmission object
    m_ouWaveTransmitter.vDoInitialisation(&m_objWaveformDataHandler,
                                          g_pouDIL_CAN_Interface, m_ouBusmasterNetwork);

    // CG: The following line was added by the Splash Screen component.
    CSplashScreen::DisplaySplashScreen(this, SW_SHOW);
    Sleep(1500);
    bCreateMsgWindow();
    m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUSERSELCTION::eCREATEDESTROYCMD, TRUE);
    // Setting Hex Mode by default
    bSetHexDecFlags(TRUE);


    vInitialiaseLINConfig(1);
    m_objTxHandler.SetNetworkConfig(LIN, m_ouBusmasterNetwork);
    m_objTxHandler.SetNetworkConfig(CAN, m_ouBusmasterNetwork);
    //TODO Appropriate Place has to be decided.
    bInitFrameProcLIN(); // Initialize logger module

    // In-Active Database
    theApp.m_pouMsgSgInactive = new CMsgSignal( sg_asDbParams[CAN], false );



    bUpdatePopupMenuDIL();
    
    bUpdatePopupMenuDILL(); //LIN modify


    BusmasterPluginConfiguration pluginInfo;
    UIElements uiElements;
	uiElements.mRibbonBar = &mRibbonBar;
	
	mPluginManager->drawUI(uiElements);
    UpdateWindow();
	
	mRibbonBar.ForceRecalcLayout();
	return 0;
}


#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG


/******************************************************************************
    Input(s)         :  WPARAM wParam, LPARAM lParam, LRESULT* pResult
    Output           :  BOOL
    Functionality    :  #Called by the framework when the user clicks on
                         drop down of signal watch button
                        #Called by the framework when the user clicks on
                         drop down of time mode button
******************************************************************************/
BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    return CMDIFrameWndEx::OnNotify(wParam, lParam, pResult);
}

/******************************************************************************
DESCRIPTION:        Opens a database file for editing
AUTHOR:             RBIN/EMC2 - Amarnath Shastry
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
                             nullptr,
                             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                             _("BUSMASTER Database File(s)(*.dbf)|*.dbf||"),
                             nullptr );

        // Set Title
        fileDlg.m_ofn.lpstrTitle  = _("Select BUSMASTER Database Filename...");

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
                    m_bIsNewDatabase = FALSE;      //To declare that database as existing one
                }

            }
            else
            {
                AfxMessageBox(_("Specified database file is not found.\n\
								Operation unsuccessful."), MB_OK | MB_ICONINFORMATION);
            }

            if ( bDisplayEditor == TRUE )
            {
                if ( m_podMsgSgWnd != nullptr )
                {
                    m_podMsgSgWnd = nullptr;
                }
                sg_asDbParams[CAN].m_pouMsgSignalActiveDB = theApp.m_pouMsgSgInactive;
                sg_asDbParams[CAN].m_pouMsgSignalImportedDBs = theApp.m_pouMsgSignal;
                m_podMsgSgWnd = new CMsgSignalDBWnd(sg_asDbParams[CAN]);

                if ( m_podMsgSgWnd != nullptr )
                {
                    sg_asDbParams[CAN].m_omDBPath = m_omStrDatabaseName;
                    m_podMsgSgWnd->vSetDBName(m_omStrDatabaseName);
                    // Create child window
                    if ( !m_podMsgSgWnd->Create(nullptr,
                                                _("Database Editor"),
                                                WS_CHILD | WS_VISIBLE |
                                                WS_OVERLAPPED | WS_CAPTION |
                                                WS_THICKFRAME, rectDefault,
                                                this ) )
                    {
                        MessageBox( _("Create BUSMASTER Database Window Failed!"),
                                    nullptr, MB_OK|MB_ICONERROR );
                        return;
                    }

                    m_podMsgSgWnd->ShowWindow( SW_SHOWNORMAL );
                    m_podMsgSgWnd->UpdateWindow();

                    // Set the flag to indicate the opening of database window
                    CFlags* pFlags = theApp.pouGetFlagsPtr();
                    pFlags->vSetFlagStatus( DBOPEN, TRUE );
                    // Enable 'Save' & 'Save Import' menu item
					CMenu* pMenu = nullptr;// CMenu::FromHandle(mMenuBar.GetHMenu());
                    if (pMenu != nullptr)
                    {
                        HMENU hMenu = pMenu->GetSafeHmenu();
                        if (hMenu != nullptr)
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
DESCRIPTION:    Opens a database file for editing
RETURN VALUE:   Pointer to "CMsgSignalDBWnd" class
*******************************************************************************/
CMsgSignalDBWnd* CMainFrame::pomGetMsgSgWnd()
{
    return ( m_podMsgSgWnd );
}

/******************************************************************************
DESCRIPTION:    Closes a database file
*******************************************************************************/
LRESULT CMainFrame::OnCloseDatabase(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    OnCloseDatabase();
    return S_OK;
}
void CMainFrame::OnCloseDatabase()
{
    if (nullptr == m_podMsgSgWnd)
    {
        return;
    }
    // Get appropriate data structure
    CMsgSignal* pTempMsgSg = m_podMsgSgWnd->m_sDbParams.m_pouMsgSignalActiveDB;
    if (nullptr == pTempMsgSg)
    {
        return;
    }
    if (pTempMsgSg->bGetModifiedFlag() == FALSE)
    {
        UINT bRetVal = AfxMessageBox(_(ASK_SAVE_PROMPT),
                                     MB_YESNOCANCEL | MB_ICONQUESTION);
        if (bRetVal == IDYES)
        {
            //save the database modificatins.
            m_podMsgSgWnd->vSaveModifiedDBs(pTempMsgSg);
        }
        else if (bRetVal == IDNO)
        {
            // if this is new database
            // then delete the memory and the file itself
            if (vGetNewDatabaseFlag())
            {
                // This file is no longer required
                CFile::Remove(m_podMsgSgWnd->m_sDbParams.m_omDBPath);
                vSetNewDatabaseFlag(FALSE);
            }
        }
        else if (bRetVal == IDCANCEL)
        {
            return;
        }

    }
    else
    {
        // If the user just creates new database,
        // and closes the window
        // delete the file
        if (vGetNewDatabaseFlag())
        {
            // This file is no longer required
            CFile::Remove(m_podMsgSgWnd->m_sDbParams.m_omDBPath);
            vSetNewDatabaseFlag(FALSE);
        }

    }

    // delete previously allocated memory if any
    pTempMsgSg->bDeAllocateMemoryInactive();

    CFlags* pFlags = theApp.pouGetFlagsPtr();
    if (pFlags != nullptr)
    {
        pFlags->vSetFlagStatus(DBOPEN, FALSE);
    }


    m_podMsgSgWnd->MDIDestroy();
    m_podMsgSgWnd = nullptr;
}

/******************************************************************************
DESCRIPTION:    Returns pointer to CMsgSgDetView class
RETURN VALUE:   Pointer to "CMsgSgDetView" class
*******************************************************************************/
CMsgSgDetView* CMainFrame::podGetMsgSgDetView(ETYPE_BUS eBus)
{
    return m_pomMsgSgDetViews[eBus];
}

/******************************************************************************
DESCRIPTION:    Returns pointer to CMsgSgTreeView class
RETURN VALUE:   Pointer to "CMsgSgTreeView" class
*******************************************************************************/
CMsgSgTreeView* CMainFrame::podGetMsgSgTreeView(ETYPE_BUS eBus)
{
    return(m_pomMsgSgTreeViews[eBus]);
}

/******************************************************************************
DESCRIPTION:    Sets pointer to CMsgSgTreeView class
PARAMETERS:     Pointer to "CMsgSgTreeView" class
*******************************************************************************/
void CMainFrame::podSetMsgSgTreeView(CMsgSgTreeView* pTreeView, ETYPE_BUS eBus)
{
    m_pomMsgSgTreeViews[eBus] = pTreeView;
}

/******************************************************************************
DESCRIPTION:    Sets pointer to CMsgSgDetView class
PARAMETERS:     Pointer to "CMsgSgDetView" class
*******************************************************************************/
void CMainFrame::podSetMsgSgDetView(CMsgSgDetView* pDetView, ETYPE_BUS eBus)
{
    m_pomMsgSgDetViews[eBus] = pDetView;
}

/******************************************************************************
DESCRIPTION:    Loads a database file selected by the user
AUTHOR:         RBIN/EMC2 - Amarnath Shastry
*******************************************************************************/
void CMainFrame::OnImportDatabase()
{
    CStringArray strFilePathArray;
    // Display a open file dialog
    CFileDialog fileDlg( TRUE,      // Open File dialog
                         "dbf",     // Default Extension,
                         nullptr,// Initial database file name.
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,
                         _("BUSMASTER Datatbase File(*.dbf)|*.dbf||"),
                         nullptr );

    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _("Select Active Database Filename...");

    if ( IDOK == fileDlg.DoModal() )
    {
        POSITION pos = nullptr;
        pos = fileDlg.GetStartPosition();
        while(nullptr != pos)
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
            MessageBox(omStrMsg,"BUSMASTER",MB_OK|MB_ICONERROR);
        }

		

        HWND hWnd;
        hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);

        if(hWnd)
        {
            ::SendMessage(hWnd, WM_DATABASE_CHANGE, (WPARAM)TRUE, 0);
        }

        //Added by Arun to update Data Handler Main entry list.
        vUpdateMainEntryListInWaveDataHandler();

    }
}

DWORD CMainFrame::dLoadJ1939DBFile(CString omStrActiveDataBase, bool /*bFrmCom*/)
{
    DWORD dReturn = (DWORD)E_FAIL;
    //Check for same DB path......
    //TODO::Remove
    m_ouBusmasterNetwork->SetChannelCount( J1939, 1 );
    if ( TRUE == PathIsRelative( omStrActiveDataBase ) )
    {
        std::string omStrBasePath;
        CString omConfigFileName;
        vGetLoadedCfgFileName( omConfigFileName );
        CUtilFunctions::nGetBaseFolder( omConfigFileName.GetBuffer( MAX_PATH ), omStrBasePath );
        char chAbsPath[MAX_PATH];
        PathCombine( chAbsPath, omStrBasePath.c_str(), omStrActiveDataBase.GetBuffer( MAX_PATH ) );
        omStrActiveDataBase = chAbsPath;
    }

    int channel = 0;
    ERRORCODE ecError = m_ouBusmasterNetwork->LoadDb( J1939, channel, omStrActiveDataBase.GetBuffer( 0 ) );
    if ( ecError == EC_SUCCESS )
    {
        //Signal watch
        sg_pouSWInterface[J1939]->SW_SetClusterInfo( m_ouBusmasterNetwork );

        //TODO CAN Nodesim
        //Update in NodeSimEx
        GetIJ1939NodeSim()->NS_UpdateFuncStructsNodeSimEx( (PVOID)&( m_sExFuncPtr[J1939].m_omDefinedMsgHeaders ), UPDATE_UNIONS_HEADER_FILES );
        //Update Msg Name<-->Msg Code List
        vUpdateMsgNameCodeList( theApp.m_pouMsgSignal, m_sExFuncPtr[J1939].m_odMsgNameMsgCodeListDB );
        //Update in NodeSimEx
        GetIJ1939NodeSim()->NS_UpdateFuncStructsNodeSimEx( (PVOID)&( m_sExFuncPtr[J1939].m_odMsgNameMsgCodeListDB ), UPDATE_DATABASE_MSGS );


        CBaseNodeSim* pNodeSim = nullptr;
        pNodeSim = GetIJ1939NodeSim();
        if ( pNodeSim != nullptr )
        {
            pNodeSim->NS_SetBmNetworkConfig( m_ouBusmasterNetwork, true );
        }

        // User can open the active DB
        theApp.pouGetFlagsPtr()->vSetFlagStatus( SELECTDATABASEFILE, TRUE );

        vPopulateIDList( J1939 );


        // Delete all the memory associated with the data structure.
        CMessageAttrib::ouGetHandle( J1939 ).vSaveMessageAttribData();


        HWND hWnd;
        hWnd = m_podMsgWndThread->hGetHandleMsgWnd( J1939 );
        //Set the J1939 DB pointer in MsgFrmtWnd class
        if ( hWnd )
        {
            ::SendMessage( hWnd, WM_NOTIFICATION_FROM_OTHER,
                           eLOAD_DATABASE,
                           (LPARAM)(m_ouBusmasterNetwork));
            ::SendMessage( hWnd, WM_DATABASE_CHANGE, (WPARAM)TRUE, 0 );
        }
        if (m_pouTxMsgWndJ1939 != nullptr)
        {
            m_pouTxMsgWndJ1939->vSetDatabaseInfo(m_ouBusmasterNetwork);
        }
        vPopulateIDList( J1939 );
        if (nullptr != m_pouMsgSigJ1939)
        {
            m_pouMsgSigJ1939->bAddDbNameEntry(omStrActiveDataBase);
        }
        OnClusterChanged(J1939);

        dReturn = S_OK;
    }
    else
    {
        static CString errText; //TODO::FOR TRACE WINDOW
        if ( ecError == ERR_INVALID_DATABASE )
        {
            errText = omStrActiveDataBase + _( " is not created for J1939. Please load J1939 related dbf file." );
        }
        else if ( ecError == EC_FILE_INVALID )
        {
            errText = omStrActiveDataBase + _( " INvalid DatabaseFile." );
        }
        else if ( ecError == EC_FILE_INVALID )
        {
            errText = "Database file corrupted";
        }
        else
        {
            errText = _( "Error While Importing the File" );
        }
        theApp.bWriteIntoTraceWnd( errText.GetBuffer( 0 ) );
    }

    return dReturn;
}

/******************************************************************************
DESCRIPTION:    Loads a database file selected by the user
PARAMETERS:     file name ,bFrmCom
RETURN VALUE:   bool
*******************************************************************************/
DWORD CMainFrame::dLoadDataBaseFile(CString omStrActiveDataBase, bool /* bFrmCom */)
{
    DWORD dReturn= (DWORD)E_FAIL;
    //Check for same DB path......
    //TODO::Remove
    m_ouBusmasterNetwork->SetChannelCount( CAN, 1 );
    if ( TRUE == PathIsRelative( omStrActiveDataBase ) )
    {
        std::string omStrBasePath;
        CString omConfigFileName;
        vGetLoadedCfgFileName( omConfigFileName );
        CUtilFunctions::nGetBaseFolder( omConfigFileName.GetBuffer( MAX_PATH ), omStrBasePath );
        char chAbsPath[MAX_PATH];
        PathCombine( chAbsPath, omStrBasePath.c_str(), omStrActiveDataBase.GetBuffer( MAX_PATH ) );
        omStrActiveDataBase = chAbsPath;
    }

    int channel = 0;
    ERRORCODE ecError = m_ouBusmasterNetwork->LoadDb( CAN, channel, omStrActiveDataBase.GetBuffer( 0 ) );
    if ( ecError == EC_SUCCESS )
    {
        //Signal watch
        m_objTxHandler.SetNetworkConfig(CAN, m_ouBusmasterNetwork);
        sg_pouSWInterface[CAN]->SW_SetClusterInfo( m_ouBusmasterNetwork );

        //TODO CAN Nodesim
        //Update in NodeSimEx
        GetICANNodeSim()->NS_UpdateFuncStructsNodeSimEx( (PVOID)&( m_sExFuncPtr[CAN].m_omDefinedMsgHeaders ), UPDATE_UNIONS_HEADER_FILES );
        //Update Msg Name<-->Msg Code List
        vUpdateMsgNameCodeList( theApp.m_pouMsgSignal, m_sExFuncPtr[CAN].m_odMsgNameMsgCodeListDB );
        //Update in NodeSimEx
        GetICANNodeSim()->NS_UpdateFuncStructsNodeSimEx( (PVOID)&( m_sExFuncPtr[CAN].m_odMsgNameMsgCodeListDB ), UPDATE_DATABASE_MSGS );


        CBaseNodeSim* pNodeSim = nullptr;
        pNodeSim = GetICANNodeSim();
        if ( pNodeSim != nullptr )
        {
            pNodeSim->NS_SetBmNetworkConfig( m_ouBusmasterNetwork, true );
        }

        // User can open the active DB
        theApp.pouGetFlagsPtr()->vSetFlagStatus( SELECTDATABASEFILE, TRUE );

        vPopulateIDList( CAN );


        // Delete all the memory associated with the data structure.
        CMessageAttrib::ouGetHandle( CAN ).vSaveMessageAttribData();

        vPopulateIDList( CAN );

        if (nullptr != theApp.m_pouMsgSignal)
        {
            theApp.m_pouMsgSignal->bAddDbNameEntry(omStrActiveDataBase);
        }

        OnClusterChanged(CAN);
        dReturn = S_OK;
    }
    else
    {
        static CString errText; //TODO::FOR TRACE WINDOW
        if ( ecError == ERR_INVALID_DATABASE )
        {
            errText = omStrActiveDataBase + _( " is not created for CAN. Please load CAN related dbf file." );
        }
        else if ( ecError == EC_FILE_INVALID )
        {
            errText = omStrActiveDataBase + _( " INvalid DatabaseFile." );
        }
        else if ( ecError == EC_FILE_INVALID )
        {
            errText = "Database file corrupted";
        }
        else
        {
            errText = _( "Error While Importing the File" );
        }
        theApp.bWriteIntoTraceWnd( errText.GetBuffer( 0 ) );
    }
    return dReturn;
}

void CMainFrame::vPopulateIDList( ETYPE_BUS  bus)
{
    CMessageAttrib& ouMsgAttr = CMessageAttrib::ouGetHandle( bus );

    {
        CStringList omStrMsgNameList;
        UINT unNoOfMsgs = 0;
        UINT* pIDArray = nullptr;

        std::list<IFrame*> ouFrames;

        m_ouBusmasterNetwork->GetFrameList( bus, 0, ouFrames );
        //m_ouClusterConfig[LIN].m_ouFlexChannelConfig[0].m_ouClusterInfo.GetFrames(ouFrames);

        {
            SCanIDList sList;
            sList.nCANID = 0;
            std::list<IFrame*>::iterator itrStart = ouFrames.begin();

            while( ouFrames.end() != itrStart )
            {
                std::string omFrameName;
                (*itrStart)->GetFrameId(sList.nCANID);
                (*itrStart)->GetName(omFrameName);
                //sList.nCANID        = itrStart->m_nSlotId;
                sList.omCANIDName   = omFrameName.c_str();

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
                itrStart++;
            }

            ouMsgAttr.vDoCommit();
            delete [] pIDArray;
            pIDArray = nullptr;
        }
    }
}

/******************************************************************************
DESCRIPTION:    Closes a database file
PARAMETERS:     CString omStrHexNo -- Hex number
RETURN VALUE:   Equivalent decimal number
*******************************************************************************/
__int64 CMainFrame::nConvertStringToInt(CString omStrHexNo)
{
    __int64 n64RetVal = 0;

    bool b_IsNegative = false;

    for (int nCount = 0; nCount < omStrHexNo.GetLength(); nCount++)
    {
        char cChar = omStrHexNo.GetAt( nCount);

        if ((cChar >= '0') && (cChar <= '9'))
        {
            n64RetVal = n64RetVal * 16 + (cChar - '0');
        }
        else if ((cChar >= 'A') && (cChar <= 'F'))
        {
            n64RetVal = n64RetVal * 16 + ((cChar - 'A') + 10);
        }
        else if ((cChar >= 'a') && (cChar <= 'f'))
        {
            n64RetVal = n64RetVal * 16 + ((cChar - 'a') + 10);
        }
        else if ( cChar == '-' )
        {
            b_IsNegative = true;
        }
    }
    if ( b_IsNegative )
    {
        n64RetVal = -n64RetVal;
    }
    return n64RetVal;
}

/******************************************************************************/
/*  Input(s)         :  BTR0 and BTR1                                         */
/*  Output           :  baudrate                                              */
/*  Functionality    :  This function is called to invoke the CChangeRegisters*/
/*                   dialog box. On selection of menu, this function is called*/
/******************************************************************************/

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

	if (g_pouDIL_CAN_Interface->DILC_ListHwInterfaces(m_asINTERFACE_HW, nCount, m_asControllerDetails) == S_OK)
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

            for(int nIndex = 0; nIndex < nCount; nIndex++)
            {
                m_asControllerDetails[nIndex].m_omHardwareDesc = m_asINTERFACE_HW[nIndex].m_acNameInterface;
            }
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
    if(pCmdUI != nullptr)
    {
        CFlags* podFlag = theApp.pouGetFlagsPtr();
        if( podFlag != nullptr )
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
DESCRIPTION:    #Called by the framework when user selects new database
                option from the menu
                #Creates a new database file name and displays
                database editior
*******************************************************************************/
void CMainFrame::OnNewDatabase()
{
    UINT unCount = 1;
    BOOL bResult = FALSE;
    while (TRUE)
    {
        CString omStrDbName = NEW_DATABASE_NAME;

        CString omStr = "";

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
                fileDlg.m_ofn.lpstrTitle  = _("New Database file name...");
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
                if(om_Fe != nullptr )
                {
                    LPTSTR lpszError = nullptr;
                    // Get error
                    om_Fe->GetErrorMessage( lpszError, 255);

                    MessageBox( lpszError, nullptr, MB_OK );

                    om_Fe->Delete();
                }
            }
            END_CATCH_ALL
            break;
        }

        unCount++;
    }
    if (bResult == TRUE)
    {

        if ( m_podMsgSgWnd != nullptr )
        {
            m_podMsgSgWnd = nullptr;
        }
        sg_asDbParams[CAN].m_pouMsgSignalActiveDB = theApp.m_pouMsgSgInactive;
        sg_asDbParams[CAN].m_pouMsgSignalImportedDBs = theApp.m_pouMsgSignal;
        m_podMsgSgWnd = new CMsgSignalDBWnd(sg_asDbParams[CAN]);

        if ( m_podMsgSgWnd != nullptr )
        {
            sg_asDbParams[CAN].m_omDBPath = m_omStrDatabaseName;
            m_podMsgSgWnd->vSetDBName(m_omStrDatabaseName);
            m_bIsNewDatabase = TRUE;

            // Create child window
            if ( !m_podMsgSgWnd->Create( nullptr,
                                         _("Database Editor"),
                                         WS_CHILD|WS_VISIBLE |WS_OVERLAPPED|
                                         WS_CAPTION|WS_THICKFRAME,
                                         rectDefault, this ) )
            {
                MessageBox( _("Create BUSMASTER Database Window Failed!"),
                            nullptr, MB_OK|MB_ICONERROR );
                return;
            }

            m_podMsgSgWnd->ShowWindow( SW_SHOWNORMAL );

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
DESCRIPTION:    #Called by the framework when user selects SaveAs...
                option from the menu for the database editor
                #Dispalys Save file dialog
                #Saves the datastructure contents into the selected file
*******************************************************************************/
void CMainFrame::OnConfigDatabaseSaveAs()
{
    // Display a save file dialog
    CFileDialog fileDlg( FALSE,     // Save File dialog
                         "dbf",     // Default Extension,
                         "NewDB.dbf",
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         _("CAN Datatbase File(*.dbf)|*.dbf||"),
                         nullptr );

    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _("Save As...");

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

        CMsgSignal* pTempMsgSg = nullptr;
        // Get the pointer to the editor database data structure
        pTempMsgSg = theApp.m_pouMsgSgInactive;

        if ( pTempMsgSg != nullptr )
        {
            BeginWaitCursor( );
            // save into the file
            pTempMsgSg->bWriteIntoDatabaseFileFromDataStructure( strDbName, PROTOCOL_CAN );

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
            if ( m_pomMsgSgTreeViews[CAN] != nullptr)
            {
                m_pomMsgSgTreeViews[CAN]->vSetRootItemText(m_omStrDatabaseName);
            }

            // Set all the items in the tree view to normal font
            if ( m_pomMsgSgTreeViews[CAN] != nullptr)
            {
                m_pomMsgSgTreeViews[CAN]->vSetAllItemsNormal();
            }
        }
    }
}

/**
* \brief         Called by the framework when user selects SaveAs...
                 option from the menu for the database editor
* \param[in]     nullptr
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
                         nullptr );

    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _("Save As...");

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

        CMsgSignal* pTempMsgSg = m_podMsgSgWndJ1939->m_sDbParams.m_pouMsgSignalActiveDB;

        if ( pTempMsgSg != nullptr )
        {
            BeginWaitCursor( );
            // save into the file
            pTempMsgSg->bWriteIntoDatabaseFileFromDataStructure( strDbName, PROTOCOL_J1939 );
            if (pTempMsgSg->bGetDBAcitveFlagStatus() == TRUE)
            {
                if (m_pouTxMsgWndJ1939 != nullptr)
                {
                    m_pouTxMsgWndJ1939->vSetDatabaseInfo(m_ouBusmasterNetwork);
                }
            }
            EndWaitCursor( );
            // Now set the root item in the tree view to
            // selected name
            if ( m_pomMsgSgTreeViews[J1939] != nullptr)
            {
                m_pomMsgSgTreeViews[J1939]->vSetRootItemText(strDbName);
            }

            // Set all the items in the tree view to normal font
            if ( m_pomMsgSgTreeViews[J1939] != nullptr)
            {
                m_pomMsgSgTreeViews[J1939]->vSetAllItemsNormal();
            }
        }
    }
}

/******************************************************************************
DESCRIPTION:    #Called by the framework when user selects Save...
                option from the menu for the database editor
                #Saves the datastructure contents into the selected file
*******************************************************************************/
void CMainFrame::OnConfigDatabaseSave()
{

    // Get appropriate message data structure
    CMsgSignal* pTempMsgSg = nullptr;
    // Get the data structure pointer
    pTempMsgSg = theApp.m_pouMsgSgInactive;

    // Save into the database
    if ( pTempMsgSg != nullptr )
    {
        CMsgSgDetView* podMsgSg = podGetMsgSgDetView(CAN);

        // Special case:
        // If the user edits message details and
        // directly clicks the "save" tool bar button
        // or menu options, save the edited values to
        // the data structure first
        if (podMsgSg != nullptr)
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

                    // Check if the modified file is being loaded or not.
                    //If yes then prompt the user whether he wants to
                    //import it or not.
                    CStringArray omImportedDBNames;
                    if (theApp.m_pouMsgSignal != nullptr)
                    {
                        theApp.m_pouMsgSignal->vGetDataBaseNames(&omImportedDBNames);
                        for (INT nDBCount = 0; nDBCount < omImportedDBNames.GetSize();
                                nDBCount++)
                        {
                            if (m_omStrDatabaseName ==
                                    omImportedDBNames.GetAt(nDBCount))
                            {
                                CString omText;
                                omText.Format(_("File  \"%s\"  has been modified which is currently being loaded.\nDo you want to re-import it to reflect the changes?"),
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
                    if ( m_pomMsgSgTreeViews[CAN] != nullptr)
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
DESCRIPTION:    #Called by the framework when user selects Configure Message...
                option from the menu.
                #Displays message attribute dialog
*******************************************************************************/
void CMainFrame::OnConfigMessageDisplay()
{
    if ( nullptr == m_podMsgWndThread )
    {
        //if Msg Window is not created no need to configure its display.Since there
        //is no way to get the existing details and to save the configured values
        return;
    }

    MessageWindowSettings settings;


    settings.mBusmasterIsOnline = true;

    CFlags* pouFlag = theApp.pouGetFlagsPtr();
    // Get the connection status
    if (pouFlag != nullptr)
    {
        settings.mBusmasterIsOnline = (pouFlag->nGetFlagStatus(CONNECTED)!=0);
    }


    //Buffer
    settings.mBufferSettings.mISValidSettings = !settings.mBusmasterIsOnline;
    if (settings.mBusmasterIsOnline == FALSE)
    {
        if (m_podMsgWndThread != nullptr)//Msg window
        {
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_GET_BUFFER_DETAILS, (LPARAM)m_anMsgBuffSize[CAN]);
        }
        settings.mBufferSettings.mAppendSize = m_anMsgBuffSize[CAN][defAPPEND_DATA_INDEX];
        settings.mBufferSettings.mOverWriteSize = m_anMsgBuffSize[CAN][defOVERWRITE_DATE_INDEX];
        settings.mBufferSettings.mDisplayUpdateRate = m_anMsgBuffSize[CAN][defDISPLAY_UPDATE_DATA_INDEX];
    }

    //Filters
    settings.mFilterDetails.mISValidSettings = true;

    for (int i = 0; i < m_sFilterAppliedCAN.m_ushTotal; i++)
    {
        settings.mFilterDetails.mFiltersConfigured.push_back(m_sFilterAppliedCAN.m_psFilters[i].m_sFilterName.m_acFilterName);
    }

    SFILTERAPPLIED_CAN sFilterAppliedCan;
    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_GET_FILTER_DETAILS, (WPARAM)&sFilterAppliedCan, (LPARAM)CAN);
    std::map<std::string, bool> filtersApplied;
    for (int i = 0; i < sFilterAppliedCan.m_ushTotal; i++)
    {
        settings.mFilterDetails.mFitersApplied[sFilterAppliedCan.m_psFilters[i].m_sFilterName.m_acFilterName] = (sFilterAppliedCan.m_psFilters[i].m_bEnabled!=0);
    }

    //message attributes
    settings.mMessageAttribute.mValidSettings = true;
    CMessageAttrib& attributes = CMessageAttrib::ouGetHandle(CAN);
    settings.mMessageAttribute.mMsgAttributes = &(attributes);

    ICluster* pCluster = nullptr;
    std::list<IFrame*> lstFrames;
    m_ouBusmasterNetwork->GetFrameList(CAN, 0, lstFrames);
    settings.mMessageAttribute.mMessageIDs = new UINT[lstFrames.size()];
    unsigned int unIndex = 0, unFrameId;
    for (auto itrFrame : lstFrames)
    {
        itrFrame->GetFrameId(unFrameId);
        settings.mMessageAttribute.mMessageIDs[unIndex] = unFrameId;
        unIndex++;
    }

    settings.mMessageAttribute.mMsgCount = unIndex;


    //Invoke Settings Dialog
    MessageWindowSettingsDialog settingsDlg(defCONFIG_MSG_DISPLAY_CAN, &settings);
    BOOL bReturnVal = settingsDlg.DoModal();
    if (bReturnVal == IDOK)
    {
        ApplyFilterConfigToMsgWnd(settings.mFilterDetails.mFitersApplied, m_sFilterAppliedCAN);

        m_anMsgBuffSize[CAN][defAPPEND_DATA_INDEX] = settings.mBufferSettings.mAppendSize;
        m_anMsgBuffSize[CAN][defOVERWRITE_DATE_INDEX] = settings.mBufferSettings.mOverWriteSize;
        m_anMsgBuffSize[CAN][defDISPLAY_UPDATE_DATA_INDEX] = settings.mBufferSettings.mDisplayUpdateRate;

        if (nullptr != m_podMsgWndThread && settings.mBusmasterIsOnline == false)
        {
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_GET_BUFFER_DETAILS, (LPARAM)m_anMsgBuffSize[CAN]);
        }
        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_INVALIDATE_LIST_DISPLAY, 0, 0);
    }
}



void CMainFrame::OnConfigMessageDisplayLin()
{
    if (nullptr == m_podMsgWndThread)
    {
        //if Msg Window is not created no need to configure its display.Since there
        //is no way to get the existing details and to save the configured values
        return;
    }

    MessageWindowSettings settings;

    CFlags* pouFlag = theApp.pouGetFlagsPtr();
    // Get the connection status
    if (pouFlag != nullptr)
    {
        settings.mBusmasterIsOnline = (pouFlag->nGetFlagStatus(LIN_CONNECTED)!=0);
    }


    //Buffer
    settings.mBufferSettings.mISValidSettings = !settings.mBusmasterIsOnline;
    if (settings.mBusmasterIsOnline == FALSE)
    {
        if (m_podMsgWndThread != nullptr)//Msg window
        {
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_GET_BUFFER_DETAILS, (LPARAM)m_anMsgBuffSize[LIN]);
        }
        settings.mBufferSettings.mAppendSize = m_anMsgBuffSize[LIN][defAPPEND_DATA_INDEX];
        settings.mBufferSettings.mOverWriteSize = m_anMsgBuffSize[LIN][defOVERWRITE_DATE_INDEX];
        settings.mBufferSettings.mDisplayUpdateRate = m_anMsgBuffSize[LIN][defDISPLAY_UPDATE_DATA_INDEX];
    }

    //Filters
    settings.mFilterDetails.mISValidSettings = true;
    for (int i = 0; i < m_sFilterAppliedLIN.m_ushTotal; i++)
    {
        settings.mFilterDetails.mFiltersConfigured.push_back(m_sFilterAppliedLIN.m_psFilters[i].m_sFilterName.m_acFilterName);
    }

    SFILTERAPPLIED_LIN sFilterAppliedLin;
    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_GET_FILTER_DETAILS, (WPARAM)&sFilterAppliedLin, (LPARAM)LIN);
    std::map<std::string, bool> filtersApplied;
    for (int i = 0; i < sFilterAppliedLin.m_ushTotal; i++)
    {
        settings.mFilterDetails.mFitersApplied[sFilterAppliedLin.m_psFilters[i].m_sFilterName.m_acFilterName] = static_cast<bool>(sFilterAppliedLin.m_psFilters[i].m_bEnabled);
    }

    //message attributes
    settings.mMessageAttribute.mValidSettings = true;
    CMessageAttrib& attributes = CMessageAttrib::ouGetHandle(LIN);
    settings.mMessageAttribute.mMsgAttributes = &(attributes);

    ICluster* pCluster = nullptr;
    std::list<IFrame*> lstFrames;
    m_ouBusmasterNetwork->GetFrameList(LIN, 0, lstFrames);
    settings.mMessageAttribute.mMessageIDs = new UINT[lstFrames.size()];
    unsigned int unIndex = 0, unFrameId;
for (auto itrFrame : lstFrames)
    {
        itrFrame->GetFrameId(unFrameId);
        settings.mMessageAttribute.mMessageIDs[unIndex] = unFrameId;
        unIndex++;
    }

    settings.mMessageAttribute.mMsgCount = unIndex;


    //Invoke Settings Dialog
    MessageWindowSettingsDialog settingsDlg(defCONFIG_MSG_DISPLAY_LIN, &settings);
    BOOL bReturnVal = settingsDlg.DoModal();
    if (bReturnVal == IDOK)
    {
        ApplyFilterConfigToMsgWnd(settings.mFilterDetails.mFitersApplied, m_sFilterAppliedLIN);

        m_anMsgBuffSize[LIN][defAPPEND_DATA_INDEX] = settings.mBufferSettings.mAppendSize;
        m_anMsgBuffSize[LIN][defOVERWRITE_DATE_INDEX] = settings.mBufferSettings.mOverWriteSize;
        m_anMsgBuffSize[LIN][defDISPLAY_UPDATE_DATA_INDEX] = settings.mBufferSettings.mDisplayUpdateRate;

        if (nullptr != m_podMsgWndThread && settings.mBusmasterIsOnline == false)
        {
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_GET_BUFFER_DETAILS, (LPARAM)m_anMsgBuffSize[LIN]);
        }
        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_INVALIDATE_LIST_DISPLAY, 0, 0);
    }
}

void CMainFrame::ApplyFilterConfigToMsgWnd(std::map<std::string, bool>& filtersApplied, SFILTERAPPLIED_LIN& canFilters)
{
    SFILTERAPPLIED_LIN    sTempFilter;
    sTempFilter.vClear();
    sTempFilter.m_psFilters = new SFILTERSET[filtersApplied.size()];

for (auto filter : filtersApplied)
    {
        const PSFILTERSET psTemp = SFILTERSET::psGetFilterSetPointer(canFilters.m_psFilters, canFilters.m_ushTotal, filter.first.c_str());
        if (psTemp != nullptr)
        {
            sTempFilter.m_psFilters[sTempFilter.m_ushTotal].bClone(*psTemp);
            sTempFilter.m_psFilters[sTempFilter.m_ushTotal].m_bEnabled = filter.second;
            sTempFilter.m_ushTotal++;
        }
    }

    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_SET_FILTER_DETAILS, (WPARAM)&sTempFilter, 0);


    // Check if filter for message display is enabled
    ApplyMessageFilterButton();
}

void CMainFrame::ApplyFilterConfigToMsgWnd(std::map<std::string, bool>& filtersApplied, SFILTERAPPLIED_CAN& canFilters)
{
    SFILTERAPPLIED_CAN    sTempFilter;
    sTempFilter.vClear();
    sTempFilter.m_psFilters = new SFILTERSET[filtersApplied.size()];

for (auto filter : filtersApplied)
    {
        const PSFILTERSET psTemp = SFILTERSET::psGetFilterSetPointer(canFilters.m_psFilters, canFilters.m_ushTotal, filter.first.c_str());
        if (psTemp != nullptr)
        {
            sTempFilter.m_psFilters[sTempFilter.m_ushTotal].bClone(*psTemp);
            sTempFilter.m_psFilters[sTempFilter.m_ushTotal].m_bEnabled = filter.second;
            sTempFilter.m_ushTotal++;
        }
    }

    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_SET_FILTER_DETAILS, (WPARAM)&sTempFilter, 0);


    // Check if filter for message display is enabled
    ApplyMessageFilterButton();
}

/******************************************************************************/
/*  Functionality    :  Called by frame work when user select command skip    */
/******************************************************************************/
void CMainFrame::OnReplaySingleSkip()
{
    // Dispatch it to Replay Manager
    vREP_CmdSkip();
}

/******************************************************************************/
/*  Functionality    :  Called by frame work when user select command step    */
/******************************************************************************/
void CMainFrame::OnReplaySingleStep()
{
    // Dispatch it to Replay Manager
    vREP_CmdStep();
}

/******************************************************************************/
/*  Functionality    :  Called by frame work when user select command stop    */
/******************************************************************************/
void CMainFrame::OnReplayStop()
{
    // Dispatch it to Replay Manager
    vREP_CmdStop();
}

/******************************************************************************/
/*  Functionality    :  Called by frame work when user select go command      */
/******************************************************************************/
void CMainFrame::OnReplayGo()
{
    // Dispatch it to Replay Manager
    vREP_CmdGo();
}

extern BOOL gbStopActiveThreads(int);


/******************************************************************************/
/*  Input(s)         :  Stringarray                                           */
/*  Output           :  array of files not built properly                     */
/*  Functionality    :  It builds and load                                    */
/*                       all the DLL under the configuration.                 */
/******************************************************************************/
bool CMainFrame::bDLLBuildLoad(CStringArray* /* omStrBuildFiles */)
{
    return false;
}


/******************************************************************************/
/*  Functionality    :   to build                                             */
/*                      all the C files associated with all the simulated     */
/******************************************************************************/
bool CMainFrame::bDLLBuild(CStringArray* /* omStrBuildFiles */)
{
    return false;
}


/******************************************************************************/
/*  Functionality    :  Called by frame work when user want to  load          */
/*                       all the DLLs under the configuration.                */
/******************************************************************************/
bool CMainFrame::bDllLoad(CStringArray* /*omStrBuildFiles*/)
{
    return false;
}

/******************************************************************************/
/*  Functionality    :   to  unload                                           */
/*                       all the DLLs under the configuration.                */
/******************************************************************************/
bool CMainFrame::bDllUnload(CStringArray* /*omStrBuildFiles*/)
{
    return false;
}

/******************************************************************************/
/*  Functionality    :  Called by frame work when user want to  unload        */
/*                       all the DLLs under the configuration.                */
/******************************************************************************/
void CMainFrame::OnDllUnload()
{
    CStringArray omStrBuildFiles;
    BOOL bSucces = GetICANNodeSim()->NS_DllUnloadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the unload is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg = _("Following file(s) are not properly unloaded:");
        CString omStrErrorMsgDummy = "";

        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format("\n%s", omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

/******************************************************************************/
/*  Functionality    :  Called by frame work when user want to  unload        */
/*                       all the DLLs under the configuration.                */
/******************************************************************************/
void CMainFrame::OnDllUnloadLIN()
{
    CStringArray omStrBuildFiles;
    BOOL bSucces = GetILINNodeSim()->NS_DllUnloadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the unload is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg = _("Following file(s) are not properly unloaded:");
        CString omStrErrorMsgDummy = "";

        for(int i = 0 ; i < nFailure; i++)
        {

            omStrErrorMsgDummy.Format("\n%s", omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

/******************************************************************************/
/*  Functionality    :  Called by frame work when user want to  unload        */
/*                       all the DLLs under the configuration.                */
/******************************************************************************/
void CMainFrame::OnDllUnloadJ1939()
{
    CStringArray omStrBuildFiles;
    BOOL bSucces = GetIJ1939NodeSim()->NS_DllUnloadAll(&omStrBuildFiles);
    if(bSucces!=TRUE)  // if the unload is not successfull
    {
        int nFailure = omStrBuildFiles.GetSize();
        CString omStrErrorMsg = _("Following file(s) are not properly unloaded:");
        CString omStrErrorMsgDummy = "";

        for(int i = 0 ; i < nFailure; i++)
        {
            omStrErrorMsgDummy.Format("\n%s", omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }
        AfxMessageBox(omStrErrorMsg);
    }
}

/******************************************************************************/
/*  Functionality    :  Fills the J1939PGNList struct with dtabase message ID,
                        name and color for future use.
/******************************************************************************/
void CMainFrame::vPopulateJ1939PGNList()
{
    if ( m_pouMsgSigJ1939 != nullptr )
    {
        CMessageAttrib& ouMsgAttr = CMessageAttrib::ouGetHandle(J1939);
        CStringList omStrMsgNameList;
        UINT unNoOfMsgs =
            m_pouMsgSigJ1939->unGetNumerOfMessages();

        UINT* pIDArray = new UINT[unNoOfMsgs];

        m_pouMsgSigJ1939->omStrListGetMessageNames(omStrMsgNameList);

        if (pIDArray != nullptr )
        {
            m_pouMsgSigJ1939->unListGetMessageIDs( pIDArray );

            SCanIDList sList;

            POSITION pos = omStrMsgNameList.GetHeadPosition();

            INT unCount = (INT)((INT)unNoOfMsgs - 1);
            POSITION pos1 = pos;

            for ( pos1 = pos; ((pos1 != nullptr) && (unCount >= 0)); unCount--)
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
            pIDArray = nullptr;
        }
    }
}

/******************************************************************************
    Functionality    :  Changes the press state of Interpretation
                        tool bar button
******************************************************************************/
void CMainFrame::OnMessageInterpretation(UINT id)
{
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	id = id - IDM_INTERPRETE_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
		HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(bus);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
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
            ASSERT(false); // Must not arise
        }
		::SendMessage(hWnd, WM_WND_PROP_MODIFY, DISPLAY_MODE, byGetDispFlag);
    }
}

/*******************************************************************************
 Functionality    : This function is called by framework when user wants to
                    watch a signal value. This function will display the
                    database messages to select signals. The selected signal
                    names will be copied into Signal watch data structure.user
                    can also remove added signal from the Signal Watch List
*******************************************************************************/
void CMainFrame::OnAddSignalToSignalWindow()
{
    vSW_DoInitialization(CAN);
    if ( sg_pouSWInterface[CAN] != nullptr )
    {
        sg_pouSWInterface[CAN]->SW_ShowAddDelSignalsDlg( this, m_ouBusmasterNetwork );
    }
    return;
}

/******************************************************************************
    Function Name    :  vOnLINScheduleTableConfig
    Input(s)         :  void
    Output           :  void
    Functionality    :  Called by the framework when user select Schedule Table
                        Config
    Member of        :  CMainFrame
    Friend of        :      -
    Author(s)        :  Prathiba P
    Date Created     :  12.05.2014
******************************************************************************/
void CMainFrame::vOnLINScheduleTableConfig()
{
    //if(m_ouClusterConfig[LIN].m_ouFlexChannelConfig[0].m_strDataBasePath.empty() == false) pending
    if(nullptr != m_ouBusmasterNetwork)
    {
        m_objTxHandler.vCreateLINScheduleConfigDlg(this, m_ouBusmasterNetwork);
    }
    else
    {
        AfxMessageBox( _(defSTR_EMPTY_ACTIVE_DATABASE) );
        this->SetFocus();
    }
}
void CMainFrame::OnAddSignalToSignalWindow_LIN()
{
    vSW_DoInitialization(LIN);
    if (sg_pouSWInterface[LIN] != nullptr)
    {
        sg_pouSWInterface[LIN]->SW_ShowAddDelSignalsDlg(this, m_ouBusmasterNetwork);
    }
}

/******************************************************************************
    Functionality    :  Called by the framework when user select Log Filter
******************************************************************************/
void CMainFrame::OnLogFilter()
{
    CFlags* pouFlags = nullptr;
    bool bLogFilterStatus = false;

    pouFlags = theApp.pouGetFlagsPtr();
    if(pouFlags != nullptr )
    {
        bLogFilterStatus = pouFlags->nGetFlagStatus(LOGFILTER) ? true : false;
        bLogFilterStatus = !bLogFilterStatus;
        pouFlags->vSetFlagStatus(LOGFILTER, bLogFilterStatus);
        if (sg_pouFrameProcCAN != nullptr)
        {
            INT Count = sg_pouFrameProcCAN->GetLoggingBlockCount();
            for (INT i = 0; i < Count; i++)
            {
                sg_pouFrameProcCAN->EnableFilter((USHORT)i, bLogFilterStatus);
            }
        }
    }
}

/******************************************************************************
    Functionality    :  Called by the framework when user select Log Filter
******************************************************************************/
void CMainFrame::OnLogFilterLIN()
{
    CFlags* pouFlags = nullptr;
    bool bLogFilterStatus = false;

    pouFlags = theApp.pouGetFlagsPtr();
	if (pouFlags != nullptr)
	{
		bLogFilterStatus = (pouFlags->nGetFlagStatus(LOGFILTER_LIN) != 0);
		bLogFilterStatus = bLogFilterStatus ? FALSE : TRUE;
		pouFlags->vSetFlagStatus(LOGFILTER_LIN, bLogFilterStatus);
		if (sg_pouFrameProcLIN != nullptr)
		{
			INT Count = sg_pouFrameProcLIN->GetLoggingBlockCount();
			for (INT i = 0; i < Count; i++)
			{
				sg_pouFrameProcLIN->EnableFilter((USHORT)i, bLogFilterStatus);
			}
		}
	}
}

void CMainFrame::ApplyReplayFilter()
{
    CFlags* pouFlags = nullptr;
    bool bReplayFilterStatus = false;

    pouFlags = theApp.pouGetFlagsPtr();
	if (pouFlags != nullptr)
	{
		bReplayFilterStatus = (pouFlags->nGetFlagStatus(REPLAYFILTER) != 0);
		pouFlags->vSetFlagStatus(REPLAYFILTER, bReplayFilterStatus);

		vREP_EnableFilters(bReplayFilterStatus);
	}
}

void CMainFrame::ApplyLogFilter()
{
    CFlags* pouFlags = nullptr;
    bool bLogFilterStatus = false;

    pouFlags = theApp.pouGetFlagsPtr();
	if (pouFlags != nullptr)
	{
		bLogFilterStatus = (pouFlags->nGetFlagStatus(LOGFILTER) != 0);
		//bLogFilterStatus = bLogFilterStatus ? FALSE : TRUE;
		pouFlags->vSetFlagStatus(LOGFILTER, bLogFilterStatus);
		if (sg_pouFrameProcCAN != nullptr)
		{
			INT Count = sg_pouFrameProcCAN->GetLoggingBlockCount();
			for (INT i = 0; i < Count; i++)
			{
				sg_pouFrameProcCAN->EnableFilter((USHORT)i, bLogFilterStatus);
			}
		}
	}
}

/******************************************************************************
    Functionality    :  Applying Filter to logging module
******************************************************************************/
void CMainFrame::ApplyLINLogFilter()
{
    CFlags* pouFlags = nullptr;
    bool bLogFilterStatus = false;

    pouFlags = theApp.pouGetFlagsPtr();
	if (pouFlags != nullptr)
	{
		bLogFilterStatus = (pouFlags->nGetFlagStatus(LOGFILTER_LIN) != 0);
		//bLogFilterStatus = bLogFilterStatus ? FALSE : TRUE;
		pouFlags->vSetFlagStatus(LOGFILTER_LIN, bLogFilterStatus);
		if (sg_pouFrameProcLIN != nullptr)
		{
			INT Count = sg_pouFrameProcLIN->GetLoggingBlockCount();
			for (INT i = 0; i < Count; i++)
			{
				sg_pouFrameProcLIN->EnableFilter((USHORT)i, bLogFilterStatus);
			}
		}
	}
}

/******************************************************************************
    Functionality    :  Called by the framework when user selects About
                        Application... menu option
                        Displays a dialog with program information.
******************************************************************************/
void CMainFrame::OnAboutApplication()
{
    theApp.OnAppAbout();

}

static void vPopulateMsgEntryFromDB(SMSGENTRY*& psMsgEntry,
                                    CMsgSignal* pouMsgSig)
{
    if (pouMsgSig != nullptr)
    {
        UINT nCount = pouMsgSig->unGetNumerOfMessages();
        UINT* punMsgIds = new UINT[nCount];
        pouMsgSig->unListGetMessageIDs(punMsgIds);
        for (UINT i = 0; i < nCount; i++)
        {
            sMESSAGE* pMsg = pouMsgSig->
                             psGetMessagePointer(punMsgIds[i]);
            if (pMsg != nullptr)
            {
                SMSGENTRY::bUpdateMsgList(psMsgEntry, pMsg);
            }
        }
        delete[] punMsgIds;
    }
}

/******************************************************************************/
/*  Functionality    :  This function is called by framework when user wants  */
/*                      to select message filter list.A dialog box will be    */
/*                      for user to select the message list the choose options*/
/******************************************************************************/
void CMainFrame::OnSelectMessage()
{
    // Ask Filter Manager to show filter configuration dialog
    // This will update configuration module if user makes some
    // Modification
    SMSGENTRY* psMsgEntry = nullptr;
    vPopulateMsgEntryFromDB(psMsgEntry, theApp.m_pouMsgSignal);
    LPARAM lParam = 0;
    if (nullptr != GetICANDIL())
    {
        GetICANDIL()->DILC_GetControllerParams(lParam, 0, NUMBER_HW);
    }

    HRESULT hResult = Filter_ShowConfigDlg( (void*)&m_sFilterAppliedCAN, m_ouBusmasterNetwork, CAN, (UINT)lParam, this );

    SMSGENTRY::vClearMsgList(psMsgEntry);
    psMsgEntry = nullptr;

    if(hResult == S_OK)//if some changes occurs in filters, update it in all related modules
    {
        //Indicate to the modules
        //1. Log
        if (sg_pouFrameProcCAN != nullptr)
        {
            UINT unLogCount = sg_pouFrameProcCAN->GetLoggingBlockCount();

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
        if (m_podMsgWndThread != nullptr)
        {
            SFILTERAPPLIED_CAN sMsgWndFilter;
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_GET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, 0);

            //store the previous filter
            SFILTERAPPLIED_CAN      sTempAppliedFilter;
            sTempAppliedFilter.bClone(sMsgWndFilter);
            Filter_ReUpdateAppliedFilter(&sMsgWndFilter, &m_sFilterAppliedCAN, CAN);

            //restore the enable flag for all filters
            for(int nFilterCnt = 0; nFilterCnt < sTempAppliedFilter.m_ushTotal; nFilterCnt++)
            {
                if((sTempAppliedFilter.m_psFilters) != nullptr)
                {
                    ((sMsgWndFilter.m_psFilters)+ nFilterCnt) ->m_bEnabled
                        =  ((sTempAppliedFilter.m_psFilters)+ nFilterCnt) ->m_bEnabled;
                }
            }
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, 0);

            //if msg filter is enable, disable it and then re-enable it to affect the changes
			if (theApp.pouGetFlagsPtr()->nGetFlagStatus(DISPLAYFILTERON))
			{
				//Disable the msg filter, so it will update the modified changes
				theApp.pouGetFlagsPtr()->vSetFlagStatus(DISPLAYFILTERON, FALSE);
				::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_ENABLE_FILTER_APPLIED, (WPARAM)FALSE, 0);

				//re-enable the msg filter, so it will show the modified changes
				theApp.pouGetFlagsPtr()->vSetFlagStatus(DISPLAYFILTERON, TRUE);
				::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_ENABLE_FILTER_APPLIED, (WPARAM)TRUE, 0);

			}
        }
    }
}

/******************************************************************************/
/*  Functionality    :  This function is called by framework when user wants  */
/*                      to select message filter list.A dialog box will be    */
/*                      for user to select the message list the choose options*/
/******************************************************************************/
void CMainFrame::OnLINFilter()
{
    // Ask Filter Manager to show filter configuration dialog
    // This will update configuration module if user makes some
    // Modification
    /*SMSGENTRY* psMsgEntry = nullptr;
    vPopulateMsgEntryFromDB(psMsgEntry, theApp.m_pouMsgSignal);*/
    LPARAM lParam = 0;
    if (nullptr != GetILINDIL())
    {
        GetILINDIL()->DILL_GetControllerParams(lParam, 0, NUMBER_HW);
    }

    HRESULT hResult = Filter_ShowConfigDlg((void*)&m_sFilterAppliedLIN,  m_ouBusmasterNetwork, LIN, (UINT)lParam, this);

    /*SMSGENTRY::vClearMsgList(psMsgEntry);
    psMsgEntry = nullptr;*/

    if(hResult == S_OK)//if some changes occurs in filters, update it in all related modules
    {
        //Indicate to the modules
        //1. Log
        if (sg_pouFrameProcLIN != nullptr)
        {
            UINT unLogCount = sg_pouFrameProcLIN->GetLoggingBlockCount();

            for (USHORT i = 0; i < unLogCount; i++)
            {
                SFILTERAPPLIED_LIN sFilterLin;
                sg_pouFrameProcLIN->FPL_GetFilteringScheme(i, sFilterLin);
                Filter_ReUpdateAppliedFilter(&sFilterLin, &m_sFilterAppliedLIN, LIN);
                //            sg_pouFrameProcCAN->FPC_StartEditingSession(); //editing is not required here
                sg_pouFrameProcLIN->FPL_ApplyFilteringScheme(i, sFilterLin);
                //            sg_pouFrameProcCAN->FPC_StopEditingSession(TRUE);
            }
        }
        //2. Replay Currently no Replay for LIN
        // vREP_SetConfiguredFilter(&m_sFilterAppliedLIN);
        //3. Message window
        if (m_podMsgWndThread != nullptr)
        {
            SFILTERAPPLIED_LIN sMsgWndFilter;
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_GET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, 0);

            //store the previous filter
            SFILTERAPPLIED_LIN      sTempAppliedFilter;
            sTempAppliedFilter.bClone(sMsgWndFilter);
            Filter_ReUpdateAppliedFilter(&sMsgWndFilter, &m_sFilterAppliedLIN, LIN);

            //restore the enable flag for all filters
            for(int nFilterCnt = 0; nFilterCnt < sTempAppliedFilter.m_ushTotal; nFilterCnt++)
            {
                if((sTempAppliedFilter.m_psFilters) != nullptr)
                {
                    ((sMsgWndFilter.m_psFilters)+ nFilterCnt) ->m_bEnabled
                        =  ((sTempAppliedFilter.m_psFilters)+ nFilterCnt) ->m_bEnabled;
                }
            }
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, 0);

            //if msg filter is enable, disable it and then re-enable it to affect the changes
			if (theApp.pouGetFlagsPtr()->nGetFlagStatus(DISPLAYFILTERON))
			{
				//Disable the msg filter, so it will update the modified changes
				theApp.pouGetFlagsPtr()->vSetFlagStatus(DISPLAYFILTERON, FALSE);
				::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_ENABLE_FILTER_APPLIED, (WPARAM)FALSE, 0);

				//re-enable the msg filter, so it will show the modified changes
				theApp.pouGetFlagsPtr()->vSetFlagStatus(DISPLAYFILTERON, TRUE);
				::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_ENABLE_FILTER_APPLIED, (WPARAM)TRUE, 0);

			}
        }
    }
}

/******************************************************************************/
/*  Functionality    :  This function is called by framework when user wants  */
/*                      a log file. This will set the status of corresponding
                        toolbar button.
                        Gets the log file from the registry.
                        Post a thread message to perform logging to the specifi
                        -ed log file name.
/******************************************************************************/
void CMainFrame::OnLogEnable()
{
    // TODO: Add your command handler code here
    BOOL bLogON = FALSE;
    if (nullptr != sg_pouFrameProcCAN)
    {
        bLogON = sg_pouFrameProcCAN->IsLoggingON();
    }
    bLogON = bLogON ? FALSE : TRUE;

    if(bLogON == FALSE)
    {
        if (nullptr != sg_pouFrameProcCAN)
        {
            sg_pouFrameProcCAN->DisableDataLogFlag();
        }
    }

    // Set the status of logging
    BOOL bIsConnected = FALSE;
    CFlags* pouFlags = nullptr;
    pouFlags = theApp.pouGetFlagsPtr();
    if (nullptr != pouFlags)
    {
        pouFlags->vSetFlagStatus(LOGTOFILE, bLogON);
        bIsConnected = pouFlags->nGetFlagStatus(CONNECTED);
    }

    // If Connected and Log is enabled
    if(bLogON == TRUE && bIsConnected == TRUE)
    {
        //if(theApp.m_pouMsgSignal != nullptr)
        {
            //if(theApp.m_pouMsgSignal->unGetNumerOfMessages() > 0)
            //if (nullptr != sg_pouFrameProcCAN)
            {
                //if(sg_pouFrameProcCAN->FPC_IsDataLogged() == TRUE)
                {
                    m_unTimerSBLog = SetTimer(TIMER_REFRESH_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, nullptr);
                }
            }
        }
    }
    // If not connected or log is disabled
    else
    {
        if(m_unTimerSBLog != 0)
        {
            ::KillTimer(nullptr, m_unTimerSBLog);
            m_unTimerSBLog = 0;
            // Update Status bar
            //m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
            m_wndStatusBar.SetPaneIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
        }
    }

    vStartStopLogging(bLogON == TRUE );
}

/******************************************************************************/
/*  Functionality    :  This function is called by framework when user wants  */
/*                      a log file. This will set the status of corresponding
                        toolbar button.
                        Gets the log file from the registry.
                        Post a thread message to perform logging to the specifi
                        -ed log file name.
/******************************************************************************/

void CMainFrame::OnLog_LIN_Enable()
{
    //shashank
    // TODO: Add your command handler code here
    BOOL bLogON = FALSE;
    if (nullptr != sg_pouFrameProcLIN )
        //if (nullptr != sg_pouFrameProcLIN)
    {
        bLogON = sg_pouFrameProcLIN->IsLoggingON();
    }
    bLogON = bLogON ? FALSE : TRUE;


    if(bLogON == FALSE)
    {
        if (nullptr != sg_pouFrameProcLIN)
            //if (nullptr != sg_pouFrameProcLIN)
        {
            sg_pouFrameProcLIN->DisableDataLogFlag();
        }
    }

    // Set the status of logging
    BOOL bIsConnected = FALSE;
    CFlags* pouFlags = nullptr;
    pouFlags = theApp.pouGetFlagsPtr();
    if (nullptr != pouFlags)
    {
        pouFlags->vSetFlagStatus(LOGTOFILE_LIN, bLogON);
        bIsConnected = pouFlags->nGetFlagStatus(LIN_CONNECTED);
    }

    // If Connected and Log is enabled
    if(bLogON == TRUE && bIsConnected == TRUE)
    {
        //if(theApp.m_pouMsgSignal != nullptr)
        {
            //if(theApp.m_pouMsgSignal->unGetNumerOfMessages() > 0)
            //if (nullptr != sg_pouFrameProcCAN)
            {
                //if(sg_pouFrameProcCAN->FPC_IsDataLogged() == TRUE)
                {
                    m_unTimerSBLog = SetTimer(TIMER_REFRESH_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, nullptr);
                }
            }
        }
    }
    // If not connected or log is disabled
    else
    {
        if(m_unTimerSBLog != 0)
        {
            ::KillTimer(nullptr, m_unTimerSBLog);
            m_unTimerSBLog = 0;
            // Update Status bar
            //m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
            //shashank
            //m_wndStatusBar.GetStatusBarCtrl().SetIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
			m_wndStatusBar.SetPaneIcon(INDEX_LIN_LOG_ICON, m_hLogOffIcon);
        }
    }
	vStartStopLogging_LIN(bLogON == TRUE);
}


/******************************************************************************/
/*  Functionality    :  This function is called by framework when user wants  */
/*                      clicks the CAN network statistics
/******************************************************************************/
void CMainFrame::OnStatisticsCAN()
{
    m_bUpdateNetworkStatistics = true;
    OnStatistics(CAN);
}

/******************************************************************************/
/*  Functionality    :  This function is called by framework when user        */
/*                      clicks the LIN network statistics
/******************************************************************************/
void CMainFrame::OnStatisticsLIN()
{
    OnStatistics(LIN);
}


/******************************************************************************/
/*  Functionality    :  This function is called by framework when user clicks */
/*                      on the network statistics window . Depending on the
                        BUS selected the corrsponding tab will be made active
/******************************************************************************/
void CMainFrame::OnStatistics(ETYPE_BUS ebus)
{
    BOOL bRet = FALSE;
    if(m_bIsStatWndCreated == FALSE )
    {
        LONG lParam = 0;
        int unTotalChannels = defNO_OF_CHANNELS;
        if (g_pouDIL_CAN_Interface && g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
        {
            unTotalChannels = (INT)lParam;
        }


        int unTotalChannelsLIN = 0;
        m_ouBusmasterNetwork->GetChannelCount(LIN, unTotalChannelsLIN);
        if ( m_oNetworkStatistics == nullptr )
        {
            UINT unChnl[BUS_TOTAL];

            unChnl[CAN] = unTotalChannels;
            unChnl[LIN] = unTotalChannelsLIN;
            m_oNetworkStatistics = new CNetworkStatistics(unChnl);
            m_oNetworkStatistics->SetTitle("Network Statistics");
            if ( nullptr != m_oNetworkStatistics )
            {
                bRet = m_oNetworkStatistics->Create(this, WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME);
                m_oNetworkStatistics->SetParent(AfxGetMainWnd());
                if(ebus == CAN)
                {
                    m_oNetworkStatistics->SetActivePage(CAN_STAT_PAGE);
                }
                else if(ebus == LIN)
                {
                    m_oNetworkStatistics->SetActivePage(LIN_STAT_PAGE);
                }
            }

            if(bRet)
            {
                // Update the statistics dialog creation status
                m_bIsStatWndCreated = TRUE;

                if(m_pXmlNodeBusStats != nullptr)
                {
                    m_oNetworkStatistics->hSetConfigData((xmlNodePtr)m_pXmlNodeBusStats, TRUE);
                    xmlFreeNode(m_pXmlNodeBusStats);
                    m_pXmlNodeBusStats = nullptr;

                }
                else
                {
                    m_oNetworkStatistics->hSetConfigData(nullptr, FALSE);
                }

                m_oNetworkStatistics->ShowWindow(SW_SHOW);
            }
        }
    }
    else if (m_bIsStatWndCreated == TRUE)
    {
        if(m_pXmlNodeBusStats != nullptr)
        {
            m_oNetworkStatistics->hSetConfigData((xmlNodePtr)m_pXmlNodeBusStats, TRUE);
            xmlFreeNode(m_pXmlNodeBusStats);
            m_pXmlNodeBusStats = nullptr;
        }

        if(ebus == CAN)
        {
            m_oNetworkStatistics->SetActivePage(CAN_STAT_PAGE);
        }
        else if(ebus == LIN)
        {
            m_oNetworkStatistics->SetActivePage(LIN_STAT_PAGE);
        }

        m_oNetworkStatistics->ShowWindow(SW_SHOW);
    }
}



void CMainFrame::OnButtonSignalWatchButton()
{
    vSW_DoInitialization(CAN);
    if (sg_pouSWInterface[CAN] != nullptr)
    {
        INT nCmd = sg_pouSWInterface[CAN]->SW_IsWindowVisible() ? SW_HIDE : SW_SHOW;
        sg_pouSWInterface[CAN]->SW_ShowSigWatchWnd(this, GetSafeHwnd(), nCmd);
        BOOL bHexON = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);
        sg_pouSWInterface[CAN]->SW_SetDisplayMode(bHexON);
    }
}


void CMainFrame::OnButtonSignalWatchButton_LIN()
{
    vSW_DoInitialization(LIN);
    if (sg_pouSWInterface[LIN] != nullptr)
    {
        INT nCmd = sg_pouSWInterface[LIN]->SW_IsWindowVisible() ? SW_HIDE : SW_SHOW;
        sg_pouSWInterface[LIN]->SW_ShowSigWatchWnd(this, GetSafeHwnd(), nCmd);
        BOOL bHexON = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);
        sg_pouSWInterface[LIN]->SW_SetDisplayMode(bHexON);
    }
}

/******************************************************************************
    Functionality    :  Function will convert a string to equivalent
                        byte array.
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
}

/******************************************************************************/
/*  Functionality    :  Returns the value of m_bIsNewDatabase
/******************************************************************************/
bool CMainFrame::vGetNewDatabaseFlag()
{
    return m_bIsNewDatabase;
}

/******************************************************************************/
/*  Functionality    :  Sets the value of m_bIsNewDatabase to "bValue"
                        passed as parameter
/******************************************************************************/
void CMainFrame::vSetNewDatabaseFlag(bool bValue)
{
    m_bIsNewDatabase = bValue;
}

/******************************************************************************/
/*  Input(s)         :  "wpParam" contains the string to be displayed         */
/*  Output           :  baudrate                                              */
/*  Functionality    :  This function is by frame work when a WM_STATUSBAR    */
/*                      message is posted. It displayed on status bar text    */
/*                      passed a parameter "wpParam"                          */
/******************************************************************************/
LRESULT CMainFrame::OnErrorMessageProc(WPARAM wpParam, LPARAM lParam)
{
    //TRACE("POSTED ERROR MSG");
    SCAN_ERR ErrorMsg ;
    BOOL bToCallHandler = FALSE;
    ERROR_STATE eCurrError;
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
        if( pouFlag != nullptr )
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
    Input(s)         :  unChannel - Channel ID
                        byRxError - Rx Error counter value
                        byTxError - Tx Error counter value
    Output           :  TRUE if the function finds out analysing byRxError and
                        byTxError that
    Functionality    :  This analyses byRxError and byTxError to find out the
                        current error state. If it is different than what is
                        indicated currently with m_eCurrErrorState, this
                        returns TRUE, else FALSE.
******************************************************************************/
bool CMainFrame::bIsTransitionInState( UINT unChannel,
                                       BYTE byRxError,
                                       BYTE byTxError )
{
    bool bIsTransition = false;
    // Based on the value of transmit and receive error counter decide
    // the current error state

    if ((byTxError <= 127) && (byRxError <= 127))
    {
        // Error Active Mode
        if (m_eCurrErrorState[ unChannel ] != ERROR_ACTIVE)
        {
            bIsTransition = true;
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
            bIsTransition = true;
            m_eCurrErrorState[ unChannel ] = ERROR_BUS_OFF;
        }
    }
    else
    {
        // Error passive
        if (m_eCurrErrorState[ unChannel ] != ERROR_PASSIVE)
        {
            bIsTransition = true;
            m_eCurrErrorState[ unChannel ] = ERROR_PASSIVE;
        }
    }

    return bIsTransition;
}

/******************************************************************************/
/*  Functionality    :  This function will convert a integer passed as CString
                        to hexadecimal no.
/******************************************************************************/
CString CMainFrame::omStrConvertIntegerToHex(CString omStrInt)
{
    CString omStrHexNo = "";
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
    Functionality    :  Checks whether the value passed as parameter is valid
                        Hexadecimal number. If yes, the function returns TRUE,
                        otherwise FALSE
******************************************************************************/
bool CMainFrame::bIsHexNumber(CString omStrHexNumber)
{
    bool b_RetVal = false;

    omStrHexNumber.MakeUpper();

    for ( int nCount = 0; nCount < omStrHexNumber.GetLength(); nCount++ )
    {
        char t_cChar = omStrHexNumber.GetAt(nCount);

        if ( ( t_cChar >= 'A' && t_cChar <= 'F' ) ||
                ( t_cChar >= '0' && t_cChar <= '9' ) ||
                ( t_cChar == '-') )
        {
            b_RetVal = true;
        }
        else
        {
            b_RetVal = false;
            break;
        }
    }

    return b_RetVal;
}

/******************************************************************************
    Functionality    :  Called by the framework to enable or disable
                        Database Close menu option.
******************************************************************************/
void CMainFrame::OnUpdateConfigureDatabaseClose(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ));
}

/******************************************************************************
    Functionality    :  Called by the framework to enable or disable
                        Database New menu option.
******************************************************************************/
void CMainFrame::OnUpdateConfigureDatabaseNew(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( !theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ));
}

/******************************************************************************
    Functionality    :  Called by the framework to enable or disable
                        Database Save menu option.
******************************************************************************/
void CMainFrame::OnUpdateConfigureDatabaseSave(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ));
}

/******************************************************************************
    Functionality    :  Called by the framework to enable or disable
                        Database SaveAs menu option.
******************************************************************************/
void CMainFrame::OnUpdateConfigureDatabaseSaveas(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( theApp.pouGetFlagsPtr()->nGetFlagStatus( DBOPEN ));
}

/******************************************************************************
    Functionality    :  Called by the framework when mainframe receives WM_CLOSE
                        message.
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
            UINT unMsgRetVal = AfxMessageBox(_("You have made changes to the configuration.\nDo you want to save it?"),
                                             MB_YESNOCANCEL);

            if ( unMsgRetVal == IDYES )
            {
                CFileDialog oCfgFileDlg(FALSE,   // Open dialog as Save as File dlg
                                        defFILEEXT,  // default extension
                                        nullptr,       // default file name
                                        defDLGFLAGS, // mode
                                        _(defCONFIGFILTER), // filter
                                        nullptr        // parent wnd
                                       );
                oCfgFileDlg.m_ofn.lpstrTitle = _("Save Configuration File");
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
						theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, SECTION, defCONFIGFILENAME, REG_SZ, oCfgFilename);
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
                UINT unMsgRetVal  = AfxMessageBox(_("You have made changes to the configuration.\nDo you want to save it?"),
                                                  MB_YESNOCANCEL);
                if ( unMsgRetVal == IDYES )
                {
                    // This call alone is sufficient to save the
                    // configuration file.
                    vSetFileStorageInfo(oCfgFilename);
                    if ( SaveConfiguration() == defCONFIG_FILE_SUCCESS )
                    {
						theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, SECTION, defCONFIGFILENAME, REG_SZ, oCfgFilename);
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
    theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, SECTION, defCONFIGFILENAME, REG_SZ, oCfgFilename);

	if (g_pouDIL_CAN_Interface != nullptr)
	{
		g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();
	}
	
	if (g_pouDIL_LIN_Interface != NULL)
	{
		g_pouDIL_LIN_Interface->DILL_StopHardware();
		g_pouDIL_LIN_Interface->DILL_PerformClosureOperations();
	}
	mPluginManager->notifyAppClose();
    vREP_HandleConnectionStatusChange( FALSE ); //Close reply

    OnDllUnload(); //Unload all the loaded dlls

    //Unload J1939 Node sim dll
    OnDllUnloadJ1939();

    OnDllUnloadLIN();

    if(m_unTimerSB != 0)
    {
        ::KillTimer(nullptr, m_unTimerSB);
    }
    if(m_unTimerSBLog != 0)
    {
        ::KillTimer(nullptr, m_unTimerSBLog);
        m_unTimerSBLog = 0;
        // Update Status bar
        //m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
		m_wndStatusBar.SetPaneIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
    }
    if(m_unJ1939TimerSBLog != 0)
    {
        ::KillTimer(nullptr, m_unJ1939TimerSBLog);
        m_unJ1939TimerSBLog = 0;
        // Update Status bar
		m_wndStatusBar.SetPaneIcon(INDEX_J1939_LOG_ICON, m_hLogOffIcon);
    }
	//now unloadplugins;
	mPluginManager->unLoadPlugins();

    if(m_podUIThread != nullptr)
    {
        m_podUIThread->PostThreadMessage(WM_QUIT,0,0);
    }
    //SGW Code commented by Arun 21-10-2010
    /*if(m_pomGraphThread != nullptr)
    {
        m_pomGraphThread->PostThreadMessage(WM_QUIT,0,0);
    }*/
    //SGW Code commented by Arun 21-10-2010
    m_bIsStatWndCreated = FALSE;
    // Get appropriate data structure
    // Call Close Database to take app. action
    CFlags* pFlags = nullptr;
    pFlags = theApp.pouGetFlagsPtr();
    if( pFlags != nullptr)
    {
        // Close DB Editor if it is visible
        if( pFlags->nGetFlagStatus(DBOPEN) == TRUE )
        {
            OnCloseDatabase();
        }
    }

    // Stop Logging if it is enabled
    BOOL bLogON = FALSE;
    if (nullptr != sg_pouFrameProcCAN)
    {
        bLogON = sg_pouFrameProcCAN->IsLoggingON();
    }
    if ( bLogON )
    {
        bLogON = bLogON ? FALSE : TRUE;
        vStartStopLogging(bLogON == TRUE);
    }

    // Stop J1939 Logging if it is enabled
    if (nullptr != sg_pouIJ1939Logger)
    {
        sg_pouIJ1939Logger->EnableLogging(FALSE);
    }



    if (nullptr != sg_pouFrameProcLIN)
    {
        bLogON = sg_pouFrameProcLIN->IsLoggingON();
    }
    if ( bLogON )
    {
        bLogON = bLogON ? FALSE : TRUE;
        vStartStopLogging_LIN(bLogON == TRUE);
    }

    //m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eCREATEDESTROYCMD, FALSE);
    m_objTxHandler.vPostMessageToTxWnd ( WM_USER_CLOSE, FALSE, FALSE );

    

    vCloseFormatconverters();

    SaveBarState(PROFILE_CAN_MONITOR);

    if(nullptr != m_pXmlNodeBusStats)
    {
        xmlFreeNode(m_pXmlNodeBusStats);
        m_pXmlNodeBusStats = nullptr;
    }


    //send busmaster_exit event to plugin
    //eBusmaster_Event event = eBusmaster_Event::busmaster_exit;
    //mPluginManager->notifyPlugins(event, nullptr);


	

	afxKeyboardManager->SaveState(theApp.GetRegSectionPath());
    CMDIFrameWnd::OnClose();
}



/******************************************************************************
    Functionality    :  Creates the message window (an object of
                        CMsgMDIChildWnd class)
******************************************************************************/
bool CMainFrame::bCreateMsgWindow()
{
    bool bReturn=true;
    if (m_podMsgWndThread == nullptr)
    {
        m_podMsgWndThread = new CMsgWndThread;
        if (m_podMsgWndThread != nullptr)
        {
            // Set specific parameters on successful creation

            //m_podMsgWndThread->CreateThread();
            //CAN Message Window
            m_podMsgWndThread->CreateMsgWnd(m_hWnd, CAN,
                                            0, nullptr);
            m_podMsgWndThread->vModifyVisibilityStatus(SW_SHOW, CAN);

            //LIN Message Window
            m_podMsgWndThread->CreateMsgWnd(m_hWnd, LIN,
                                            0, nullptr);
            m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, LIN);

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                          WM_NOTIFICATION_FROM_OTHER,
                          eLOAD_DATABASE,
                          (LPARAM)m_ouBusmasterNetwork);

            //J1939 Message Window
            m_podMsgWndThread->CreateMsgWnd(m_hWnd, J1939,
                                            0, nullptr);
            m_podMsgWndThread->vModifyVisibilityStatus( SW_HIDE, J1939);
        }
        else
        {
            bReturn = false;
        }
    }
    else
    {
        m_podMsgWndThread->vModifyVisibilityStatus(SW_SHOW, CAN);
        m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, LIN);
        m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, J1939);
    }

    return true;
}

/******************************************************************************
    Functionality    :  Toggles hexadecimal button and switches between decimal
                        state to hexadecimal state
******************************************************************************/
void CMainFrame::OnHex_DecButon()
{
    if (m_podMsgWndThread != nullptr)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
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
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
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
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)shBusID);
            //Update Message Window
            if(hWnd)
            {
                BYTE bModes = NUMERIC;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }

    BOOL bHexON = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);

    m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eHEXDECCMD, bHexON);


    if (sg_pouSWInterface[CAN] != nullptr)
    {
        sg_pouSWInterface[CAN]->SW_SetDisplayMode(bHexON);
    }
    if (sg_pouSWInterface[J1939] != nullptr)
    {
        sg_pouSWInterface[J1939]->SW_SetDisplayMode(bHexON);
    }
    if (sg_pouSWInterface[LIN] != nullptr)
    {
        sg_pouSWInterface[LIN]->SW_SetDisplayMode(bHexON);
    }

    if(nullptr != m_podMsgSgWnd)
    {
        m_podMsgSgWnd->SendMessage(WM_UPDATE_DATABASEEDITOR ,(WPARAM)(bHexON), 0);

    }
    if (nullptr != m_podMsgSgWndJ1939)
    {
        m_podMsgSgWndJ1939->SendMessage(WM_UPDATE_DATABASEEDITOR, (WPARAM)(bHexON), 0);
    }
    eNumeric_mode mode = (bHexON == TRUE) ? eNumeric_mode::HEXADECIMAL_MODE : eNumeric_mode::DECICAL_MODE;
    mPluginManager->notifyPlugins(eBusmaster_Event::display_numeric_mode_changed, &mode);

}

/**
* \brief         Enabling or disabling Hex/Numeric mode
* \param[in]     bHexEnabled is bool variable, if true Hex mode is enabled
* \return        void
* \authors       Prathiba P
* \date          06.18.2013 Created
*/
void CMainFrame::bSetHexDecFlags(bool bHexEnabled)
{
    if (m_podMsgWndThread != nullptr)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
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
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)shBusID);
            //Update Message Window
            if(hWnd)
            {
                BYTE bModes = NUMERIC;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }

    BOOL bHexON = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);

    m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eHEXDECCMD, bHexON);


    if (sg_pouSWInterface[CAN] != nullptr)
    {
        sg_pouSWInterface[CAN]->SW_SetDisplayMode(bHexON);
    }
    if (sg_pouSWInterface[J1939] != nullptr)
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
    if (m_podMsgWndThread != nullptr)
    {
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);

        BOOL bIsOverwrite = FALSE;
        bIsOverwrite = theApp.pouGetFlagsPtr()->nGetFlagStatus(OVERWRITE);
        if (bIsOverwrite == TRUE)
        {
            CLEAR_EXPR_DISP_BITS(byGetDispFlag);

            int nDbCanCount = 0, nDbJ1939Count = 0, nDbLinCount = 0;
            m_ouBusmasterNetwork->GetDBServiceCount(CAN, 0, nDbCanCount);
            m_ouBusmasterNetwork->GetDBServiceCount(J1939, 0, nDbJ1939Count);
            m_ouBusmasterNetwork->GetDBServiceCount(LIN, 0, nDbLinCount);


            if (nDbCanCount <= 0 && nDbJ1939Count <= 0 && nDbLinCount <= 0 )
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
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)shBusID);
            //Update Message Window
            if(hWnd)
            {
                BYTE bModes = DISPLAY_MODE;
                ::SendMessage(hWnd, WM_WND_PROP_MODIFY, bModes, byGetDispFlag);
            }
        }
    }
}

/**
 * \brief Display Message window Overwrite
 * \req RS_16_01 - Two display modes namely, overwrite and append shall be supported.
 *
 * Toggles overwrite button and switches between
 * overwriting state to appending state.
 */
void CMainFrame::OnDisplayMessagewindowOverwrite(UINT id)
{

	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	id = id - IDM_OVERWRITE_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
		HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)bus);
        BYTE byGetDispFlag = 0;
        ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
		
		if (IS_MODE_APPEND(byGetDispFlag))
        {
            CLEAR_EXPR_DISP_BITS(byGetDispFlag);
	
            int nDbCount = 0;
			m_ouBusmasterNetwork->GetDBServiceCount(bus, 0, nDbCount);
            

			if (nDbCount <= 0)
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
		
		::SendMessage(hWnd, WM_WND_PROP_MODIFY, DISPLAY_MODE, byGetDispFlag);
    }
}


/******************************************************************************
    Functionality    :  Checks or Unchecks the Interpretation menu option
******************************************************************************/
void CMainFrame::OnUpdateMessageInterpret(CCmdUI* pCmdUI)
{
    if ( nullptr == theApp.m_pouMsgSignal )
    {
        return;
    }
    BYTE byGetDispFlag = 0;
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	UINT id = pCmdUI->m_nID - IDM_INTERPRETE_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
		HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(bus);
        int nDbLinCount = 0;

        int nDbCount = 0;
		m_ouBusmasterNetwork->GetDBServiceCount(bus, 0, nDbCount);
        

		if ( nDbCount > 0 )
        {
            ::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
			if (IS_MODE_INTRP(byGetDispFlag))
            {
                pCmdUI->Enable(TRUE);   // Enabled
                pCmdUI->SetCheck(TRUE); // Pressed state
            }
            else if (IS_MODE_APPEND(byGetDispFlag))
            {
                // Disable the button
                pCmdUI->Enable(FALSE);  // Disabled
                pCmdUI->SetCheck(0); // Retain pressed state
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
    Functionality    :  Check or uncheck log filter
                        menu item
******************************************************************************/
void CMainFrame::OnUpdateLogFilter(CCmdUI* pCmdUI)
{
    // Check or uncheck log filter
    // menu item
    pCmdUI->SetCheck(
        theApp.pouGetFlagsPtr()->nGetFlagStatus( LOGFILTER ) );
}

/******************************************************************************
    Functionality    :  Check or uncheck log filter
                        menu item
******************************************************************************/
void CMainFrame::OnUpdateLogFilterLIN(CCmdUI* pCmdUI)
{
    // Check or uncheck log filter
    // menu item
    pCmdUI->SetCheck(
        theApp.pouGetFlagsPtr()->nGetFlagStatus( LOGFILTER_LIN ) );
}


/******************************************************************************
    Function Name    :  OnUpdateLINScheduleTableConfig

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Check or uncheck Schedule Table Config
                        menu item
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Prathiba P
    Date Created     :  12.05.2014
******************************************************************************/
void CMainFrame::OnUpdateLINScheduleTableConfig(CCmdUI* pCmdUI)
{
    int nLinChannels;
    m_ouBusmasterNetwork->GetChannelCount(LIN, nLinChannels);
    if(nLinChannels > 0)
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}
void CMainFrame::OnUpdateReplayFilter(CCmdUI* pCmdUI)
{
    // Check or uncheck replay filter
    // menu item
    pCmdUI->SetCheck(
        theApp.pouGetFlagsPtr()->nGetFlagStatus( REPLAYFILTER ) );
}


/******************************************************************************
    Functionality    :  Calls OnActivateTimerHandler function
******************************************************************************/
void CMainFrame::OnMessageFilterButton()
{
    CFlags* pouFlags = nullptr;
    bool bMessageFilterStatus = false;

    pouFlags = theApp.pouGetFlagsPtr();
	if (pouFlags != nullptr)
	{
		bMessageFilterStatus = (pouFlags->nGetFlagStatus(DISPLAYFILTERON) != 0);
		bMessageFilterStatus = bMessageFilterStatus ? FALSE : TRUE;
		pouFlags->vSetFlagStatus(DISPLAYFILTERON, bMessageFilterStatus);

		::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_ENABLE_FILTER_APPLIED, (WPARAM)bMessageFilterStatus, 0);
	}
}


/******************************************************************************
    Function Name    :  OnMessageFilterButtonLin

    Input(s)         :      -
    Output           :      -
    Functionality    :
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Venkatanarayana Makam
    Date Created     :  21.03.2014
******************************************************************************/
void CMainFrame::OnMessageFilterButtonLin()
{
    CFlags* pouFlags = nullptr;
    bool bMessageFilterStatus = false;

    pouFlags = theApp.pouGetFlagsPtr();
	if (pouFlags != nullptr)
	{
		bMessageFilterStatus = (pouFlags->nGetFlagStatus(DISPLAYFILTERONLIN) != 0);
		bMessageFilterStatus = bMessageFilterStatus ? FALSE : TRUE;
		pouFlags->vSetFlagStatus(DISPLAYFILTERONLIN, bMessageFilterStatus);

		::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_ENABLE_FILTER_APPLIED, (WPARAM)bMessageFilterStatus, 0);
	}
}

/**
* \brief         Handles the J1939 tx window close notification
* \param[in]     WPARAM, LPARAM
* \return        LRESULT
* \authors       Arunkumar Karri
* \date          28.02.2013 Created
*/
LRESULT CMainFrame::onJ1939TxWndClose(WPARAM /* wParam */, LPARAM /* lParam */)
{
    /* Modify filter icon accordingly in J1939 toolbar */
    return S_OK;
}


/******************************************************************************
    Functionality    :  To handle(enable/disable) filter for replay when user
                        selects from the main menu
******************************************************************************/
void CMainFrame::OnReplayFilter()
{
    CFlags* pouFlags = nullptr;
	bool bReplayFilterStatus = false;

    pouFlags = theApp.pouGetFlagsPtr();
	if (pouFlags != nullptr)
	{
		bReplayFilterStatus = (pouFlags->nGetFlagStatus(REPLAYFILTER) != 0);
		bReplayFilterStatus = !bReplayFilterStatus;
		pouFlags->vSetFlagStatus(REPLAYFILTER, bReplayFilterStatus);

		vREP_EnableFilters(bReplayFilterStatus);
	}
}

void CMainFrame::ApplyMessageFilterButton()
{
    CFlags* pouFlags = nullptr;
    bool bMessageFilterStatus = false;

    pouFlags = theApp.pouGetFlagsPtr();
    if(pouFlags != nullptr )
    {
        bMessageFilterStatus = (pouFlags->nGetFlagStatus(DISPLAYFILTERON) != 0);
        //bMessageFilterStatus = bMessageFilterStatus ? FALSE : TRUE;
        pouFlags->vSetFlagStatus(DISPLAYFILTERON, bMessageFilterStatus);

        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_ENABLE_FILTER_APPLIED, (WPARAM)bMessageFilterStatus, 0);
        
        bMessageFilterStatus = (pouFlags->nGetFlagStatus(DISPLAYFILTERONLIN) != 0);
        //bMessageFilterStatus = bMessageFilterStatus ? FALSE : TRUE;
        pouFlags->vSetFlagStatus(DISPLAYFILTERONLIN, bMessageFilterStatus);

        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_ENABLE_FILTER_APPLIED, (WPARAM)bMessageFilterStatus, 0);
    }
}

/******************************************************************************
    Function Name    :  OnUpdateMessageFilterButtonLin

    Input(s)         :  CCmdUI* pCmdUI
    Output           :      -
    Functionality    :  Toggle message filter button status
    Member of        :  CMainFrame
    Friend of        :      -

    Author(s)        :  Amarnath Shastry
    Date Created     :  23.03.2002
    Modifications    :  Amitesh Bharti, 26.12.2002, File name is changed
******************************************************************************/
void CMainFrame::OnUpdateMessageFilterButtonLin(CCmdUI* pCmdUI)
{
    // Toggle message filter button status
    pCmdUI->SetCheck(theApp.pouGetFlagsPtr()->nGetFlagStatus(DISPLAYFILTERONLIN));

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
void CMainFrame::OnClearMsgWindow(UINT id)
{
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	id = id - IDM_CLEAR_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
		// Post a message to clear the contents of the
		// message window
		HWND hWnd;
		hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)bus);
		if (hWnd)
		{
			::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, 0, 0);
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
            m_podMsgWndThread->toggleMessageWindowDisplay(ETYPE_BUS::CAN);
        }
        break;
        case ID_SHOWMESSAGEWINDOW_J1939:
        {
            m_podMsgWndThread->toggleMessageWindowDisplay(ETYPE_BUS::J1939);
        }
        break;

        case ID_SHOWMESSAGEWINDOW_LIN:
        {
            m_podMsgWndThread->toggleMessageWindowDisplay(ETYPE_BUS::LIN);
        }
        break;
        default:
        {
            ASSERT(false);
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
            ASSERT(false);
        }
        break;
    }
}

/******************************************************************************
    Function Name    :  poGetSignalPointer

    Input(s)         :  const CString &omStrSignalName
    Output           :  sSIGNALS*
    Functionality    :  Returns pointer to signal if the signal is found
                        otherwise nullptr
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
    bool bActive,
    const UINT& unMsgID,
    const CString& omStrSignalName)
{
    BOOL bFound = FALSE;
    sSIGNALS* pSignal = nullptr;

    if ( !omStrSignalName.IsEmpty() )
    {
        sMESSAGE* pMsg = nullptr;

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

        if ( pMsg != nullptr )
        {
            pSignal = pMsg->m_psSignals;

            //Loop through the signals list and get the matching signal pointer
            while ( (pSignal != nullptr) && (!bFound) )
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
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	UINT id = pCmdUI->m_nID - IDM_OVERWRITE_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
		HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)bus);
		BYTE byGetDispFlag = 0;
		::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
		pCmdUI->SetCheck(!((bool)IS_MODE_APPEND(byGetDispFlag)));
	}
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
    auto hexStatus = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);
    pCmdUI->SetCheck(hexStatus);
    pCmdUI->SetText(hexStatus ? "Hex" : "Dec");
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
    if (nullptr != sg_pouFrameProcCAN)
    {
        //pCmdUI->SetCheck(sg_pouFrameProcCAN->FPC_IsLoggingON());
        USHORT ushCount =  sg_pouFrameProcCAN->GetLoggingBlockCount();
        ushCount = vCheckValidLogFiles(ushCount);
        BOOL bEnableLogTB = bIsAtleastOneLoggingBlockEnabled(ushCount);
        if(bEnableLogTB == TRUE)//log files found and if checked
        {
            pCmdUI->Enable(TRUE);
            pCmdUI->SetCheck(sg_pouFrameProcCAN->IsLoggingON());
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
    if (nullptr != sg_pouFrameProcLIN)
    {
        //pCmdUI->SetCheck(sg_pouFrameProcCAN->FPC_IsLoggingON());
        USHORT ushCount =  sg_pouFrameProcLIN->GetLoggingBlockCount();
        ushCount = vCheckValidLogFiles_LIN(ushCount);
        BOOL bEnableLogTB = bIsAtleastOneLoggingBlockEnabled_LIN(ushCount);
        if(bEnableLogTB == TRUE )//log files found and if checked
        {
            pCmdUI->Enable(TRUE);
            pCmdUI->SetCheck(sg_pouFrameProcLIN->IsLoggingON());
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
            CStdioFile omStdiofile;

            sg_pouFrameProcCAN->GetLoggingBlock(i, sLogObject);

            // check for valid log file path
            CString strTempLog = sLogObject.m_sLogFileName;

            CString omLogFilePath = strTempLog.Left(strTempLog.ReverseFind('\\'));

            if ( !FilePathExists(omLogFilePath) )//file path not found
            {
                sg_pouFrameProcCAN->RemoveLoggingBlock(i--); //remove the old log data
                LogBlocks = sg_pouFrameProcCAN->GetLoggingBlockCount();; //refresh the log count
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
            CStdioFile omStdiofile;

            sg_pouFrameProcLIN->GetLoggingBlock(i, sLogObject);

            // check for valid log file path
            CString strTempLog = sLogObject.m_sLogFileName;

            CString omLogFilePath = strTempLog.Left(strTempLog.ReverseFind('\\'));

            if ( !FilePathExists(omLogFilePath) )//file path not found
            {
                sg_pouFrameProcLIN->RemoveLoggingBlock(i--); //remove the old log data
                LogBlocks = sg_pouFrameProcLIN->GetLoggingBlockCount();; //refresh the log count
            }
        }
    }
    return LogBlocks;
}


bool CMainFrame::bIsAtleastOneLoggingBlockEnabled(USHORT LogBlocks)
{
    bool bEnabled = false; // Assume no log files are configured or enabled

    if (LogBlocks > 0)
    {
        for (USHORT i = 0; i < LogBlocks; i++)
        {
            SLOGINFO sLogObject;
            sg_pouFrameProcCAN->GetLoggingBlock(i, sLogObject);
            if (sLogObject.m_bEnabled)
            {
                bEnabled = true;
                break;
            }
        }
    }
    return bEnabled;
}

bool CMainFrame::bIsAtleastOneLoggingBlockEnabled_LIN(USHORT LogBlocks)
{
    bool bEnabled = false; // Assume no log files are configured or enabled

    if (LogBlocks > 0)
    {
        for (USHORT i = 0; i < LogBlocks; i++)
        {
            SLOGINFO sLogObject;
            sg_pouFrameProcLIN->GetLoggingBlock(i, sLogObject);
            if (sLogObject.m_bEnabled)
            {
                bEnabled = true;
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
    return CMDIFrameWndEx::PreTranslateMessage(pMsg);
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
    HMONITOR hMonitor = MonitorFromWindow(AfxGetMainWnd()->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST);
    MONITORINFO ouMonitorInfo;
    GetMonitorInfo(hMonitor, &ouMonitorInfo);
    int nMonitors = GetSystemMetrics(SM_CMONITORS);
	theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_DISPLAY_LEFT, REG_DWORD, "", ouMonitorInfo.rcWork.left);
	theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_DISPLAY_MONITOR, REG_DWORD, "", nMonitors);
    // Write the Window Flag
	theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_FLAG, REG_DWORD, "", WinCurrStatus.flags);
    // Write the show wnd command
    if (WinCurrStatus.showCmd == SW_SHOWMINIMIZED)
    {
        WinCurrStatus.showCmd = SW_SHOWNORMAL;
    }
	theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_SHOWCMD, REG_DWORD, "", WinCurrStatus.showCmd);
    // Write the X, Y position when window is in minimum position
	theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MINPOS_X, REG_DWORD, "", WinCurrStatus.ptMinPosition.x);
	theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MINPOS_Y, REG_DWORD, "", WinCurrStatus.ptMinPosition.y);
    // Write the X, Y position when window is in maximum position
	theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MAXPOS_X, REG_DWORD, "", WinCurrStatus.ptMaxPosition.x);
	theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_MAXPOS_Y, REG_DWORD, "", WinCurrStatus.ptMaxPosition.y);
    // Write the Window co-ordinates
    theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_LEFT, REG_DWORD,"",WinCurrStatus.rcNormalPosition.left);
    theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_RIGHT, REG_DWORD,"",WinCurrStatus.rcNormalPosition.right);
    theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_TOP, REG_DWORD,"",WinCurrStatus.rcNormalPosition.top);
    theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_BOTTOM, REG_DWORD,"",WinCurrStatus.rcNormalPosition.bottom);
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
    // Check if secondary monmitor is connected and check if the application is svaed in that monitor space
    BOOL bRet = TRUE;
    int nAvailableMonitors = GetSystemMetrics(SM_CMONITORS);
    int nMonitorsFromConfig = 0;
    HMONITOR hMonitor = MonitorFromWindow(AfxGetMainWnd()->GetSafeHwnd(), MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO ouMonitorInfo;
    GetMonitorInfo(hMonitor, &ouMonitorInfo);
    RECT ouDisplayRect;
    CString strTemp;
    // Set the length of the structure
    WinCurrStatus.length = sizeof(WINDOWPLACEMENT);
    ouDisplayRect.left = 0;
    ouDisplayRect.bottom = 0;
    ouDisplayRect.right = 0;
    ouDisplayRect.top = 0;
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_DISPLAY_LEFT, REG_DWORD, strTemp, (DWORD&)ouDisplayRect.left))
    {
        ouDisplayRect.left = 0;
    }
    if ( !theApp.bReadFromRegistry(HKEY_CURRENT_USER, defSECTION_MAIN_WND, defITEM_MAIN_WND_DISPLAY_MONITOR, REG_DWORD, strTemp, (DWORD&)nMonitorsFromConfig))
    {
        ouDisplayRect.top = 0;
    }
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
    if ((nAvailableMonitors <= 1) && (nMonitorsFromConfig > nAvailableMonitors) && (ouDisplayRect.left > ouMonitorInfo.rcWork.left))
    {
        WinCurrStatus.rcNormalPosition.left = 0;
        WinCurrStatus.rcNormalPosition.top = 0;
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
    CMDIFrameWndEx::OnShowWindow(bShow, nStatus);
    // If it is to show the window
    if (bShow == TRUE)
    {
        // Update the window placement
        SetWindowPlacement(&m_WinCurrStatus);
        // Update global window handle
        g_hMainGUI = m_hWnd;
        // Update the window
        InvalidateRect(nullptr);
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

    g_bStopSelectedMsgTx = TRUE;

    BYTE byStopThread = 0;

    byStopThread |= BIT_TX_SEL_MSG_THREAD  ;
    byStopThread |= BIT_REPLAY_THREAD  ;
    byStopThread |= BIT_MULTI_MSG_THREAD  ;

    // Destroy all running thread
    //theApp.vDestroyUtilThreads(defMAX_WAIT_UTIL_THREAD,byStopThread);
    //Stop the dll read thread
    g_bReadDllMsg=FALSE;
    // Destroy Message Handler Thread
    // If DLL is loaded, then delete threads if anything is running/waiting
    //CExecuteManager::ouGetExecuteManager().vDeleteAllNode();
    // Write the MRU file list into the registry
	//bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MRU, defSECTION_MRU_FILE1, REG_SZ, m_omStrMRU_ConfigurationFiles[0]);
    //bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MRU, defSECTION_MRU_FILE2, REG_SZ, m_omStrMRU_ConfigurationFiles[1]);
    //bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MRU, defSECTION_MRU_FILE3, REG_SZ, m_omStrMRU_ConfigurationFiles[2]);
    //bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MRU, defSECTION_MRU_FILE4, REG_SZ, m_omStrMRU_ConfigurationFiles[3]);
    //bWriteIntoRegistry(HKEY_CURRENT_USER, defSECTION_MRU, defSECTION_MRU_FILE5, REG_SZ, m_omStrMRU_ConfigurationFiles[4]);


    vEmptySimsysList();

    // Clean Network statistics dialog
    if( m_podBusStatistics != nullptr )
    {
        // Delete the memory associated with this member
        delete m_podBusStatistics;
        m_podBusStatistics = nullptr;
    }



    CMDIFrameWndEx::OnDestroy();

    // Check for the current status of the window
    if (GetWindowPlacement(&m_WinCurrStatus))
    {
        vSaveWinStatus(m_WinCurrStatus);    // save in the registry
    }

    // The closure operation is done in the ::OnClose() so commenting the below
    // performClosureOperations() for CAN interface. This is causing application
    // crash on closure
    /*if (g_pouDIL_CAN_Interface != nullptr)
    {
        g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();
    }*/

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
    if (sg_pouFrameProcCAN != nullptr)
    {
        if (ConfigureCANLog(sg_pouFrameProcCAN, m_abLogOnConnect[CAN], this, &m_sFilterAppliedCAN) == S_OK)
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
    if (sg_pouFrameProcLIN != nullptr)
    {
        if (ConfigureLINLog(sg_pouFrameProcLIN, m_abLogOnConnect[LIN], this, &m_sFilterAppliedLIN) == S_OK)
        {
            // Check if the filter for log is enabled
            ApplyLINLogFilter();
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
    pCmdUI->Enable(sg_pouFrameProcCAN != nullptr);
    /*  if (nullptr != sg_pouFrameProcCAN)
        {
            pCmdUI->Enable(!sg_pouFrameProcCAN->FPC_IsLoggingON());
        }
        else
        {
            pCmdUI->Enable(sg_pouFrameProcCAN != nullptr);
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
    pCmdUI->Enable(sg_pouFrameProcLIN != nullptr);
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
    CMDIFrameWndEx::OnEndSession(bEnding);

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
    CString omStrPage = strGetCurrentConfigFileName();

    pCmdUI->SetText( omStrPage );

    m_wndStatusBar.SetTipText( INDEX_DB_NAME, omStrPage);
}

CString CMainFrame::strGetCurrentConfigFileName()
{
    CString omStrConfigurationFilename = "";
    CString omStrDBName = "";
    CString omStrPage;

    vGetLoadedCfgFileName(omStrConfigurationFilename);

    if ( omStrConfigurationFilename.IsEmpty())
    {
        if(m_omStrSavedConfigFile.IsEmpty())
        {
            omStrPage =
                _("Default Configuration loaded");
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

    return( omStrPage );
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
        omStrTemp = "";
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
    m_objTxHandler.vShowConfigureMsgWindow(this, CAN);
}

/* Read thread function for graph display*/
DWORD WINAPI InterpretThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return (DWORD)-1;
    }

    CMainFrame* pouMainFrm = static_cast<CMainFrame*> (pThreadParam->m_pBuffer);

    if (pouMainFrm == nullptr)
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
bool CMainFrame::bStartGraphReadThread()
{
    bool bReturn = false;
    //First stop the thread if running
    bStopGraphReadThread();
    m_ouGraphReadThread.m_pBuffer = this;
    m_ouGraphReadThread.m_hActionEvent = m_ouCanBuf.hGetNotifyingEvent();
    bReturn = (m_ouGraphReadThread.bStartThread(InterpretThreadProc) == TRUE);

    return bReturn;
}

/* Function to stop msg read thread*/
bool CMainFrame::bStopGraphReadThread()
{
    bool bReturn = false;
    bReturn = (m_ouGraphReadThread.bTerminateThread() == TRUE);
    m_ouGraphReadThread.m_hActionEvent = nullptr;
    m_ouGraphReadThread.m_unActionCode = IDLE;
    return bReturn;
}

/* routine to read the data from buffer */
INT CMainFrame::ReadGraphDataBuffer(bool bCalcTime)
{
    static STCANDATA sCanData;
    if (bCalcTime)
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
void CMainFrame::vUpdateGraphData(STCANDATA& sCanData)
{
    SINTERPRETDATA_LIST sInterpretList;

    STCAN_MSG sMessage = sCanData.m_uDataInfo.m_sCANMsg;
    // Iterate through list

    // Get List pointer
    CGraphList* pList = &(m_odGraphList[CAN]);

    // Get Database object pointer
    IBMNetWorkGetService* pomDataBase = m_ouBusmasterNetwork;
    IFrame* psMessage;
    InterpreteSignals psSignal;
    std::string name;
    // Byte Array for populating the message bytes
    CByteArray omMsgByte;
    BOOL bByteArrayPopulated = FALSE;

    // If list and control are valid
    if( pList != nullptr && pomDataBase != nullptr )
    {
        // Get the list item count
        int nCount = pList->m_omElementList.GetSize();
        CGraphElement odElement;
        std::list<InterpreteSignals> signalList;
        // Iterate through each items
        bool isValid = false;
        for( register int nIndex = 0; nIndex < nCount; nIndex++ )
        {
            isValid = false;
            // Init Msg and Sig pointers
            psMessage = nullptr;
            // get Current item
            odElement = pList->m_omElementList.GetAt( nIndex );
            // If the item is enabled and not of type Statistics
            // and the message id is same
            if( odElement.m_bEnabled == TRUE &&
                    odElement.m_nValueType != eSTAT_PARAM &&
                    (UINT)odElement.m_nMsgID == sMessage.m_unMsgID )
            {
                // Get the signal Pointer

                pomDataBase->GetFrame( CAN, 0, odElement.m_nMsgID, nullptr, &psMessage );
                if( psMessage != nullptr )
                {
                    psMessage->InterpretSignals( sCanData.m_uDataInfo.m_sCANMsg.m_ucData, sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen, signalList, false );
                    std::list<InterpreteSignals>::iterator itrSignal = signalList.begin();
                    while (itrSignal != signalList.end())
                    {
                        if ( itrSignal->m_omSigName.c_str() == odElement.m_omStrElementName )
                        {
                            psSignal = *itrSignal;
                            isValid = true;
                            break;
                        }
                        itrSignal++;
                    }
                }

                // Got Match
                if (isValid == true)
                {
                    // Get Interpreted value
                    __int64 nRawValue = strtoul(psSignal.m_omRawValue.c_str(), nullptr, 10);

                    sInterpretList.unMsgID = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;
                    sInterpretList.m_nTimeStamp = sCanData.m_lTickCount.QuadPart;
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
                            // Update Graph Control
                            sInterpretList.m_unValue.m_dPhysical = strtod(psSignal.m_omEnggValue.c_str(), nullptr);
                            sInterpretList.m_shType = ePHY_VALUE;
                            break;
                        }
                    }// Switch
                    m_pouMsgInterpretBuffer->WriteIntoBuffer(CAN,(BYTE*)&sInterpretList,SIZE_INTRP_DATA);
                } // If Signal != nullptr
            } // If Message ID matches
        } // For loop of List Elements
    } // If list and control and dbase pointers are valid
}
/******************************************************************************/
/*  Functionality    :  This function is called by framework when user selects*/
/*                      connect/disconnect menu/toolbar button. This will be  */
/*                     also called from PreTranslateMsg() function when ESC/F2*/
/*                     key is pressed. It will load the bitmap for connect and*/
/*                     disconnect depending upon the state of this tool.      */
/*                     All global flags are toggled when state of connected or*/
/*                     disconnected toggles. Msg is posted to msg window to   */
/*                      reset handler flags.                                  */
/******************************************************************************/
void CMainFrame::OnFileConnect()
{
    CFlags* pouFlags = nullptr;

    // Hour glass cursor
    CWaitCursor omWait;

    pouFlags   = theApp.pouGetFlagsPtr();


    if(pouFlags != nullptr)
    {
        // Toggle connect/disconnect flag
        bool bConnected = (pouFlags->nGetFlagStatus(CONNECTED) != 0);
        bool bReturn = false;
        bConnected = !bConnected;
        // Reset flag for stop running of thread function
        g_bStopTimerHandlers    = TRUE;
        g_bStopKeyHandlers      = TRUE;
        g_bStopErrorHandlers    = TRUE;
		
        if(bConnected)
        {
            //Build and Load Node Simulation
            if(FALSE == GetICANNodeSim()->NS_DLLBuildLoadAllEnabled())
            {
                return;
            }
            BOOL bIsJ1939Active = theApp.pouGetFlagsPtr()->nGetFlagStatus(ACTIVATED_J1939);
            if(bIsJ1939Active)
            {
                if(FALSE == GetIJ1939NodeSim()->NS_DLLBuildLoadAllEnabled())
                {
                    return;
                }
            }

            if((pouFlags->nGetFlagStatus(LOGTOFILE) != 0 || m_abLogOnConnect[CAN] == TRUE) && sg_pouFrameProcCAN != nullptr)
            {
                sg_pouFrameProcCAN->vSetMeasurementFileName();
            }
        }
        else
        {

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


        if( bConnected == FALSE )
        {
            m_objTxHandler.vBusStatusChanged(CAN, BUS_DISCONNECTED);
        }
        else
        {
            m_objTxHandler.vBusStatusChanged(CAN, BUS_CONNECTED);
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
        //theApp.vDestroyUtilThreads(500, BIT_MULTI_MSG_THREAD);
        // Pre Connect Activities

        if (m_podMsgWndThread != nullptr)
        {
            HWND hWnd = nullptr;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
            if(hWnd)
            {
                ::SendMessage(hWnd, WM_UPDATE_CONNECT_STATUS, bConnected, 0);
            }
        }
        CBaseNodeSim* pNodeSim = nullptr;
        if( bConnected == TRUE)
        {
            //Inform J1939TxWindow about connect change
            if ( nullptr != m_pouTxMsgWndJ1939 )
            {
                ::SendMessage(m_pouTxMsgWndJ1939->GetSafeHwnd(),
                              WM_CONNECT_CHANGE, (WPARAM)TRUE, 0);
            }
            //Start Graph Interpret Thread
            bStartGraphReadThread();

            // Update Absolute time value
            CTimeManager::vInitAbsoluteTime();
            if (m_podMsgWndThread != nullptr)
            {
                HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
                if(hWnd)
                {
                    ::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, 0, 0);
                    //Clear Sort Arrow in Message Window
                    ::SendMessage(hWnd, WM_CLEAR_SORT_COLUMN, 0, 0);
                    ::SendMessage(hWnd, WM_NOTIFICATION_FROM_OTHER, eBusStatusChanged, true);
                    ::SendMessage(hWnd, WM_NOTIFICATION_FROM_OTHER, eWINID_START_READ, 0);

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
            if (nullptr != sg_pouIJ1939Logger && theApp.pouGetFlagsPtr()->nGetFlagStatus(ACTIVATED_J1939) == TRUE
                    && (m_abLogOnConnect[J1939] == TRUE || true == sg_pouIJ1939Logger->IsLoggingON() ))
            {
                sg_pouIJ1939Logger->vSetMeasurementFileName();
            }
        }
        else
        {
            if (nullptr != sg_pouFrameProcCAN)
            {
                sg_pouFrameProcCAN->DisableDataLogFlag();
            }
            if(nullptr != sg_pouIJ1939Logger)
            {
                sg_pouIJ1939Logger->DisableDataLogFlag();
            }
            //Stop Graph Interpret Thread
            bStopGraphReadThread();
            if ( nullptr != m_podMsgWndThread )
            {
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER, eBusStatusChanged, false);
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                              eWINID_STOP_READ, 0);
            }
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
                _("Trying to connect the hardware ... Please wait"), this);
            {
				Event_Bus_Staus busStatus;
				busStatus.mBus = ETYPE_BUS::CAN;
				busStatus.mEventType = eBUSEVENT::ON_PRE_CONNECT;
				mPluginManager->notifyPlugins(eBusmaster_Event::Bus_Status, &busStatus);

				if (g_pouDIL_CAN_Interface->DILC_StartHardware() == S_OK)
                {
                    ouWaitIndicator.SetWindowText(_("Connected... "));
					busStatus.mBus = ETYPE_BUS::CAN;
					busStatus.mEventType = eBUSEVENT::ON_CONNECT;
					mPluginManager->notifyPlugins(eBusmaster_Event::Bus_Status, &busStatus);
                    bReturn = true;
                }
                else
                {
                    ouWaitIndicator.SetWindowText(_("Failed to connect the hardware... "));
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
				Event_Bus_Staus busStatus;
				busStatus.mBus = ETYPE_BUS::CAN;
				busStatus.mEventType = eBUSEVENT::ON_DISCONNECT;
				mPluginManager->notifyPlugins(eBusmaster_Event::Bus_Status, &busStatus);
                bReturn = true;
            }
        }

		if (bReturn)
		{
            setConnectState(ETYPE_BUS::CAN, bConnected);

			BOOL bLogIsON = FALSE, bJ1939LogON = FALSE, bLinLogON = FALSE;

			CFlags* pFlagLog = theApp.pouGetFlagsPtr();
			if (pFlagLog != nullptr)
			{
				bLogIsON = pFlagLog->nGetFlagStatus(LOGTOFILE);
			}

			//Handle start/stop logging
			if (m_abLogOnConnect[CAN] == TRUE)
			{
				vStartStopLogging(bConnected);
			}
			else if (bLogIsON == TRUE && bConnected == TRUE)
			{
				vStartStopLogging(bLogIsON == TRUE);
			}

			if (m_abLogOnConnect[J1939] == TRUE)
			{
				// Enable Logging or stop logging
				if (nullptr != sg_pouIJ1939Logger)
				{
					if (bConnected)
					{
						vSetAssociatedDatabaseFiles(J1939); // Update the db file names associated
						vSetBaudRateInfo(J1939);                // Update the baud rate details
					}
					sg_pouIJ1939Logger->EnableLogging(bConnected);
				}

			}
			else
			{
				if (nullptr != sg_pouIJ1939Logger)
				{
					if (sg_pouIJ1939Logger->IsLoggingON() == TRUE)
					{
						bJ1939LogON = TRUE;
						sg_pouIJ1939Logger->EnableLogging(TRUE);
					}
				}
			}
			//SGW Code commented by Arun 21-10-2010
			pouFlags->vSetFlagStatus(CONNECTED, bConnected);

			// Post Connect Activities
			if (bConnected == TRUE)
			{
				// If Connect and Log both are enabled

				BOOL bLogON = FALSE;
				CFlags* pFlag = theApp.pouGetFlagsPtr();
				if (pFlag != nullptr)
				{
					bLogON = pFlag->nGetFlagStatus(LOGTOFILE);
				}

				if (bLogON == TRUE || m_abLogOnConnect[CAN] == TRUE)
				{
					//if(theApp.m_pouMsgSignal != nullptr)
					{
						//if(theApp.m_pouMsgSignal->unGetNumerOfMessages() > 0)
						//if (nullptr != sg_pouFrameProcCAN)
						{
							//if(sg_pouFrameProcCAN->FPC_IsDataLogged() == TRUE)
							{
								m_unTimerSBLog = SetTimer(TIMER_REFRESH_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, nullptr);
							}
						}
					}
				}
				/*if(bLinLogON == TRUE || m_abLogOnConnect[LIN] == TRUE)
				{
				m_unLinTimerSBLog = SetTimer(TIMER_REFRESH_LIN_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, nullptr);
				}*/
				if (bJ1939LogON == TRUE || m_abLogOnConnect[J1939] == TRUE)
				{
					m_unJ1939TimerSBLog = SetTimer(TIMER_REFRESH_J1939_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, nullptr);
				}

				vREP_HandleConnectionStatusChange(TRUE);
				//send connection statud to replay window
				BOOL bReplayFilterStatus = FALSE;
				bReplayFilterStatus = pouFlags->nGetFlagStatus(REPLAYFILTER);
				vREP_EnableFilters(bReplayFilterStatus);
			}
			// On Disconnect Kill the timer
			else
			{
				if (m_unTimerSBLog != 0)
				{
					::KillTimer(nullptr, m_unTimerSBLog);
					m_unTimerSBLog = 0;
					// Update Status bar
					//m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");
					m_wndStatusBar.SetPaneIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
				}
			}

			// Enable / disable signal transmission block
			m_ouWaveTransmitter.bUpdateBlock(bConnected);

			m_objTSExecutorHandler.vStartStopReadThread(CAN, bConnected);
			m_objTSExecutorHandler.vBusConnected(bConnected);

            if (bConnected == FALSE)
			{

				// Let the user configure the mgs
				pouFlags->vSetFlagStatus(SENDMESG, FALSE);
                m_bCfgLoadMenuOption = TRUE;
                m_bCfgNewMenuOption = TRUE;
			}
			else
			{
				// Disable the user to load or create another cfg file
				m_bCfgLoadMenuOption = FALSE;
				m_bCfgNewMenuOption = FALSE;
			}

			g_bStopSendMultMsg = TRUE;
		}
        if(!bConnected)//for proper logging of data even if connection stopped
        {
            if (nullptr != sg_pouFrameProcCAN)
            {
                USHORT ushCount =   sg_pouFrameProcCAN->GetLoggingBlockCount();
                if(ushCount>0)//check for log file count
                {
                    if(sg_pouFrameProcCAN->IsLoggingON())//if logging is on
                    {
                        sg_pouFrameProcCAN->vCloseLogFile();
                    }
                }
            }
            //shashank

            if(nullptr != sg_pouIJ1939Logger)
            {
                if(sg_pouIJ1939Logger->IsLoggingON())//if logging is on
                {
                    sg_pouIJ1939Logger->vCloseLogFile();
                }
            }
        }
        if ( bConnected == TRUE )
        {
            GetICANNodeSim()->NS_ManageBusEventHandler( eBUSEVEHANDLER::BUS_CONNECT);
            GetIJ1939NodeSim()->NS_ManageBusEventHandler( eBUSEVEHANDLER::BUS_CONNECT );

            BOOL bIsJ1939Active = theApp.pouGetFlagsPtr()->nGetFlagStatus(ACTIVATED_J1939);

            if(bIsJ1939Active == TRUE)
            {
                BOOL bIsAddClaimed = theApp.pouGetFlagsPtr()->nGetFlagStatus(ADRESSCLAIM_J1939);
                if(bIsAddClaimed == FALSE)
                {
                    CFlags* pouFlags = theApp.pouGetFlagsPtr();
                    if (pouFlags != nullptr)
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
            GetICANNodeSim()->NS_ManageBusEventHandler( eBUSEVEHANDLER::BUS_DISCONNECT );
            GetIJ1939NodeSim()->NS_ManageBusEventHandler( eBUSEVEHANDLER::BUS_DISCONNECT );
            //Unload Node Simulation dlls.
            GetICANNodeSim()->NS_DLLUnloadAllEnabled();
            GetIJ1939NodeSim()->NS_DLLUnloadAllEnabled();
        }
    }
}

void CMainFrame::OnLINConnect()
{
    HRESULT hResult = S_OK;
    CFlags* pouFlag  = theApp.pouGetFlagsPtr();

    /* Toggle connect/disconnect flag */
    bool bConnected = (pouFlag->nGetFlagStatus(LIN_CONNECTED) != 0);
    bConnected = !bConnected;

    CBaseNodeSim* pNodeSim = nullptr;
    /* If connecton is required */
    if ( bConnected )
    {
        if(NULL != g_pouDIL_LIN_Interface && g_pouDIL_LIN_Interface->DILL_GetSelectedDriver() == DRIVER_LIN_ISOLAR_EVE_VLIN)
        {
            BOOL bIsMasterMode = FALSE;
            int nLinChannels = 0;
            m_ouBusmasterNetwork->GetChannelCount(LIN, nLinChannels);
            ChannelSettings ouChannelSettings;
            for(INT nChnlIndex = 0; nChnlIndex < nLinChannels; nChnlIndex++)
            {
                m_ouBusmasterNetwork->GetChannelSettings(LIN, nChnlIndex, &ouChannelSettings);
                bIsMasterMode = ouChannelSettings.m_ouLINSettings.m_bIsMasterMode;
            }

            if( bIsMasterMode == TRUE)
            {
                AfxMessageBox("Master mode is not supported for LIN ETAS ISOLAR-EVE. Disable Master mode from LIN->Cluster Configuration");
                return;
            }
        }
        hResult = g_pouDIL_LIN_Interface->DILL_SetConfigData(m_ouBusmasterNetwork);
        //sg_pouSWInterface[LIN]->SW_SetClusterInfo(m_ouBusmasterNetwork);
        hResult = g_pouDIL_LIN_Interface->DILL_PreStartHardware();

        m_objTxHandler.vBusStatusChanged(LIN, BUS_PRECONNECT);
        if(FALSE == GetILINNodeSim()->NS_DLLBuildLoadAllEnabled())
        {
            return;
        }
        pNodeSim = GetILINNodeSim();
        if(pNodeSim != nullptr)
        {
            pNodeSim->NS_ManageBusEventHandler(BUS_PRE_CONNECT);
        }
        m_objTxHandler.vBusStatusChanged(LIN, BUS_PRECONNECT);

        hResult = g_pouDIL_LIN_Interface->DILL_StartHardware();

        /* If connect fails, return from here */
        if ( hResult == S_FALSE )
        {
            m_objTxHandler.vBusStatusChanged(LIN, BUS_DISCONNECTED);
            theApp.bWriteIntoTraceWnd("Error while accessing Hardware.");
            GetILINNodeSim()->NS_DLLUnloadAllEnabled();
            return;
        }

        BOOL bLogIsON = FALSE;
        CFlags* pFlagLog = theApp.pouGetFlagsPtr();
        if(pFlagLog != nullptr)
        {
            bLogIsON = pFlagLog->nGetFlagStatus(LOGTOFILE_LIN);
        }
        if((bLogIsON == TRUE || m_abLogOnConnect[LIN] == TRUE) && sg_pouFrameProcLIN != nullptr)
        {
            sg_pouFrameProcLIN->vSetMeasurementFileName();
        }

        if (m_abLogOnConnect[LIN] == TRUE)
        {
            vStartStopLogging_LIN(bConnected);
        }
        else if(bLogIsON == TRUE && bConnected == TRUE)
        {
            vStartStopLogging_LIN(bLogIsON == TRUE);
        }

        m_objTxHandler.vBusStatusChanged(LIN, BUS_CONNECTED);
        GetILINNodeSim()->NS_ManageBusEventHandler(BUS_CONNECT);
        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_NOTIFICATION_FROM_OTHER, eBusStatusChanged, true);
        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_NOTIFICATION_FROM_OTHER,
                      eWINID_START_READ, 0);
        // Clear message window on connect
        HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(LIN);
        if(nullptr != hWnd)
        {
            ::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, 0, 0);
            ::SendMessage(hWnd, WM_CLEAR_SORT_COLUMN, 0, 0);
        }

        GetILINBusStat()->BSL_bStartUpdation(FALSE);
        GetILINBusStat()->BSL_ResetBusStatistic();

        vSetBaudRateInfo(LIN);

        for (UINT i = 0; i < defNO_OF_LIN_CHANNELS; i++)
        {
#ifdef BOA_FD_VERSION
            GetILINBusStat()->BSL_SetBaudRate(i, m_asControllerDetailsLIN[i].m_unDataBitRate);
#else
            GetILINBusStat()->BSL_SetBaudRate(i, (m_asControllerDetailsLIN[i].m_BaudRate));
#endif
        }
        GetILINBusStat()->BSL_bStartUpdation(TRUE);

        if (NS_GetInterface(LIN, (void**) &pNodeSim) == S_OK)
        {
            pNodeSim->NS_nOnBusConnected(TRUE);
        }
        m_bLINDisconnect = false;

    }
    /* If disconnecton is required */
    else
    {
        if (nullptr != sg_pouFrameProcLIN)
        {
            sg_pouFrameProcLIN->DisableDataLogFlag();
        }

        g_pouDIL_LIN_Interface->DILL_ResetSlaveRespData();
        hResult = g_pouDIL_LIN_Interface->DILL_StopHardware();
        m_objTxHandler.vBusStatusChanged(LIN, BUS_DISCONNECTED);
        //m_objLINTxHandler.vStopTransmission(0);
        if (nullptr != m_podMsgWndThread)
        {
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_NOTIFICATION_FROM_OTHER, eBusStatusChanged, false);
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_NOTIFICATION_FROM_OTHER,
                          eWINID_STOP_READ, 0);
        }
        GetILINNodeSim()->NS_ManageBusEventHandler( eBUSEVEHANDLER::BUS_DISCONNECT );
        GetILINNodeSim()->NS_DLLUnloadAllEnabled();
        if (NS_GetInterface(LIN, (void**) &pNodeSim) == S_OK)
        {
            pNodeSim->NS_nOnBusConnected(FALSE);
        }
        m_bLINDisconnect = true;


    }
    //added to disable loading config while LIN transmission

    if(!bConnected)
    {
        if (nullptr != sg_pouFrameProcLIN)
        {
            USHORT ushCount =   sg_pouFrameProcLIN->GetLoggingBlockCount();
            if(ushCount>0)//check for log file count
            {
                if(sg_pouFrameProcLIN->IsLoggingON())//if logging is on
                {
                    vStartStopLogging_LIN(FALSE);
                    sg_pouFrameProcLIN->vCloseLogFile();
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
	setConnectState(ETYPE_BUS::LIN, bConnected);
	
    pouFlag->vSetFlagStatus(LIN_CONNECTED, bConnected);
    BYTE bytTbrItemIndex = 1;
}

void CMainFrame::OnUpdateLINConnect(CCmdUI* pCmdUI)
{
    // Select the current menu text based on which one between Connect
    // or Disconnect is currently selected. The menu text should show
    // the next available state i.e., if currently "Connect" is displayed,
    // next available state is Disconnect and vice versa. Texts are taken from
    // the string table
    CFlags* pouFlag  = theApp.pouGetFlagsPtr();
    if(pouFlag != nullptr)
    {
        BOOL bConnected  = pouFlag->nGetFlagStatus(LIN_CONNECTED);
        UINT unConnected = bConnected ? IDS_DISCONNECT_LIN : IDS_CONNECT_LIN;

        // And initialise a CString with the string.
        CString omMenuItemText("");
        omMenuItemText.Format(unConnected);

        // And finally set the menu text.
        if(pCmdUI !=nullptr)
        {
            pCmdUI->SetText(omMenuItemText);
        }
    }

    if(nullptr != pCmdUI)
    {
        if ( m_shLINDriverId != DAL_NONE )
        {
            pCmdUI->Enable();
        }
        else
        {
            pCmdUI->Enable(0);
        }
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
    if(pouFlag != nullptr)
    {
        BOOL bConnected  = pouFlag->nGetFlagStatus(CONNECTED);
        UINT unConnected = bConnected ? IDS_DISCONNECT : IDS_CONNECT;

        // And initialise a CString with the string.
        CString omMenuItemText("");
        omMenuItemText.Format(unConnected);
        // And finally set the menu text.
        if(pCmdUI !=nullptr)
        {
            pCmdUI->SetText(omMenuItemText);
        }
    }

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
                            nullptr,       // default file name
                            OFN_HIDEREADONLY
                            |OFN_EXTENSIONDIFFERENT
                            |OFN_FILEMUSTEXIST, // mode
                            _(defCONFIGFILTER), // filter
                            nullptr,        // parent wnd
                            _("Load"));
    oCfgFileDlg.m_ofn.lpstrTitle = _("Load Configuration Filename...");

    if(oCfgFileDlg.DoModal() == IDOK)
    {
        // get the name of the selected file
        CString oCfgFilename = oCfgFileDlg.GetPathName();
        nLoadConfigFile(oCfgFilename);

        // Close the J1939 and lin message windows
        m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, J1939);
        m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, LIN);

        // Deactivate J1939
        DeselectJ1939Interfaces(FALSE);
        GetIJ1939NodeSim()->NS_SetJ1939ActivationStatus(false);
        m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, J1939);
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
                            nullptr,       // default file name
                            defDLGFLAGS, // mode
                            _(defCONFIGFILTER), // filter
                            nullptr,        // parent wnd
                            _("Open"));

    oCfgFileDlg.m_ofn.lpstrTitle = _("New Configuration Filename...");

    if(oCfgFileDlg.DoModal() == IDOK)
    {

        CConfigData::ouGetConfigDetailsObject().vCloseConfigFile();

        // get the name of the selected file
        CString oCfgFilename = oCfgFileDlg.GetPathName();
        vSetFileStorageInfo(oCfgFilename);
        vSetCurrProjInfo((FLOAT)BUSMASTER_APPN_VERSION_LATEST);
        CConfigData::ouGetConfigDetailsObject().vSaveConfigFile();
        CConfigData::ouGetConfigDetailsObject().vReadConfigFile();

        for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
        {
            vSetDefaultConfiguration(eSecId);
            eSecId = static_cast<eSECTION_ID>(eSecId + 1);
        }

        mPluginManager->setPluginConfiguration(nullptr);

        SaveConfiguration();

        // On New Configuration Stop Logging if it is enabled for CAN
        BOOL bLogON = FALSE;
        if (nullptr != sg_pouFrameProcCAN)
        {
            bLogON = sg_pouFrameProcCAN->IsLoggingON();
        }
        if ( bLogON )
        {
            bLogON = bLogON ? FALSE : TRUE;
            vStartStopLogging(bLogON == TRUE);
        }

        BOOL bLogONl = FALSE;
        if (nullptr != sg_pouFrameProcLIN)
        {
            bLogONl = sg_pouFrameProcLIN->IsLoggingON();
        }
        if ( bLogONl )
        {
            bLogONl = bLogONl ? FALSE : TRUE;
            vStartStopLogging_LIN(bLogONl == TRUE);
        }


        // setting by default Hex mode on new configuration
        bSetHexDecFlags(TRUE);
        // On New Configuration Stop Logging if it is enabled for J1939
        vJ1939StartStopLogging();

        /* Update checked state for the filters(Log, Message) */
        ApplyLogFilter();
        ApplyMessageFilterButton();

        /* Switch off Replay filter and update*/
        CFlags* pouFlags = nullptr;
        pouFlags = theApp.pouGetFlagsPtr();
        pouFlags->vSetFlagStatus(REPLAYFILTER, FALSE);
        ApplyReplayFilter();
        
        /*LIN*/
        m_shLINDriverId = DAL_NONE;
        IntializeDILL();
        //m_ouClusterConfig[LIN].InitialiseConfig();
        m_ouBusmasterNetwork->ReSetNetwork(LIN);
        m_ouBusmasterNetwork->SetChannelCount(LIN, 1);

        //TODO::CAN Update
        m_ouBusmasterNetwork->SetChannelCount( CAN, 1 );

        ApplyLINLogFilter();

        // Close the J1939 and LIN message windows
        m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, J1939);
        m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, LIN);

        // Deactivate J1939
        DeselectJ1939Interfaces(FALSE);
        GetIJ1939NodeSim()->NS_SetJ1939ActivationStatus(false);
        m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, J1939);
    }
}
void CMainFrame::vInitialiaseLINConfig(int nChannel)
{
    IntializeDILL();

    m_ouBusmasterNetwork->SetChannelCount(LIN, nChannel);
    //m_ouClusterConfig[LIN].m_nChannelsConfigured = nChannel;

    //m_ouClusterConfig[LIN].InitialiseConfig();

    for ( int i = 0 ; i < nChannel; i++ )
    {
        ChannelSettings ouSettings;
        ouSettings.m_ouLINSettings.m_bOverWriteSettings = true;
        ouSettings.m_ouLINSettings.m_nBaudRate = 19200;
        ouSettings.m_ouLINSettings.m_strProtocolVersion = "LIN 2.0";
        m_ouBusmasterNetwork->SetChannelSettings(LIN, nChannel, &ouSettings);
    }
    m_shLINDriverId = DAL_NONE;
}


void CMainFrame::vGetLoadedCfgFileName(CString& omFileName)
{
    DATASTORAGEINFO sDataStorageInfo;
    CConfigData::ouGetConfigDetailsObject().GetConfigDatastorage(&sDataStorageInfo);
    if (sDataStorageInfo.FSInfo != nullptr)
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
                                nullptr,       // default file name
                                defDLGFLAGS, // mode
                                _(defCONFIGFILTER), // filter
                                nullptr        // parent wnd
                               );
        oCfgFileDlg.m_ofn.lpstrTitle = _("Save Configuration File");
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
				theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, SECTION, defCONFIGFILENAME, REG_SZ, omStrCfgFilename);
                m_omStrSavedConfigFile = omStrCfgFilename;
            }
        }
    }
    else // ostrCfgFilename has some value...
    {
        if(SaveConfiguration() == defCONFIG_FILE_SUCCESS)
        {
            vPushConfigFilenameDown(omStrCfgFilename);

			theApp.bWriteIntoRegistry(HKEY_CURRENT_USER, SECTION, defCONFIGFILENAME, REG_SZ, omStrCfgFilename);
            m_omStrSavedConfigFile = omStrCfgFilename;
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
                            nullptr,       // default file name
                            defDLGFLAGS , // mode
                            _(defCONFIGFILTER), // filter
                            nullptr        // parent wnd
                           );
    oCfgFileDlg.m_ofn.lpstrTitle = _("SaveAs Configuration Filename...");

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
void CMainFrame::OnClickMruList(CString omStrName)
{
	if (!omStrName.IsEmpty() && omStrName != _(defSTR_DEFAULT_MRU_ITEM))
	{
		// Check if any previous configuration file is loaded
		// and changes are being made to it
		if (bIsConfigurationModified() == TRUE)
		{
			// Yes, there is a loaded config and changes have been made to
			// it Hence display save confirmation message dialog
			if (AfxMessageBox(_(defSTR_CONF_SAVE_CONFIRM),
				MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				// Save
				OnSaveConfigFile();
			}
		}
		// Get the flag status and stop logging if it already started
		BOOL bLogON = FALSE;
		// Get the main menu
		CFlags* pFlag = nullptr;
		pFlag = theApp.pouGetFlagsPtr();
		if (pFlag != nullptr)
		{
			bLogON = pFlag->nGetFlagStatus(LOGTOFILE);
			if (bLogON == TRUE)
			{
				vStartStopLogging(FALSE);
				//pFlag->vSetFlagStatus( LOGTOFILE, FALSE );
			}
		}
		// Call Configuration Load
		nLoadConfigFile(omStrName);
	}
}
/*******************************************************************************
 Function Name    : vPushConfigFilenameDown
 Input(s)         : omStrConfigFilename - file name at the top of the list
 Output           : void
 Functionality    : Simulates MRU functionality
 Member of        : CMainFrame
 Friend of        :    -
*******************************************************************************/
void CMainFrame::vPushConfigFilenameDown(CString omStrConfigFilename )
{
    return theApp.AddToRecentFileList(omStrConfigFilename);
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
    if ( pCmdUI != nullptr )
    {
        BOOL bEnable = FALSE;
        // Check whether Tx msg window is visible or not
        //BOOL bSendMsgDialog = (m_pomTxMsgChildWindow != nullptr );
        BOOL bSendMsgDialog = FALSE;;

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
    if ( pCmdUI != nullptr )
    {
        pCmdUI->Enable(m_bCfgNewMenuOption);
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
    if ( pCmdUI != nullptr )
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
    if ( pCmdUI != nullptr )
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
    CString omStrFomat( "" );

    CFlags* pouFlag = nullptr;

    // Get Controller information

    PSCONTROLLER_DETAILS  pBaudDetails = nullptr;
    UINT nHardware = 0;
    LONG lParam = 0;
    if (g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
    {
        nHardware = (UINT)lParam;
    }
    pBaudDetails = m_asControllerDetails;
    // If valid
    if(pBaudDetails != nullptr)
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
    if (sg_pouFrameProcCAN != nullptr)
    {
        UINT unCount = sg_pouFrameProcCAN->GetLoggingBlockCount();
        for (UINT i = 0; i < unCount; i++)
        {
            SLOGINFO sLogInfo;
            if (sg_pouFrameProcCAN->GetLoggingBlock((USHORT)i, sLogInfo) == S_OK)
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



    // Create properties dialog and
    CProperties odProperties( sProperties );
    // Show the dialog
    odProperties.DoModal();
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

        //Update the bus statistics window once if it opened when CAN is in disconnnected state.
        if ( m_oNetworkStatistics != nullptr &&
                m_oNetworkStatistics->IsWindowVisible( ) == TRUE /*&& theApp.pouGetFlagsPtr()->nGetFlagStatus(CONNECTED) == FALSE && m_bUpdateNetworkStatistics*/)
        {

            m_oNetworkStatistics->vSendMessage(CAN);
            m_bUpdateNetworkStatistics = false;
        }
        if (((theApp.pouGetFlagsPtr()->nGetFlagStatus(CONNECTED)) == TRUE))
        {
            vUpdateGraphStatsData();
            //Update the bus statistics window if it exists.
            if ( m_oNetworkStatistics != nullptr &&
                    m_oNetworkStatistics->IsWindowVisible( ) == TRUE)
            {
                // Perform network statistics calculation and update of
                // network statistics window
                m_oNetworkStatistics->vSendMessage(CAN);
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
        if((m_oNetworkStatistics != nullptr &&
                m_oNetworkStatistics->IsWindowVisible( ) == TRUE ))
        {
            m_oNetworkStatistics->vSendMessage(LIN);
        }

        if (m_bLINDisconnect == TRUE)
        {
            if ( m_oNetworkStatistics != nullptr &&
                    m_oNetworkStatistics->IsWindowVisible( ) == TRUE )
            {
                m_oNetworkStatistics->vSendMessage(LIN, m_bLINDisconnect);
            }
        }
    }
    if(nIDEvent == m_unTimerSBLog)
    {
        if (nullptr != sg_pouFrameProcCAN)
        {
            if(sg_pouFrameProcCAN->IsDataLogged() == TRUE)
            {
                /*m_wndStatusBar.SetPaneText(INDEX_LOG_RECORD, "CAN");*/
                if(m_nIconSetFlag == 1)
                {
					m_wndStatusBar.SetPaneIcon(INDEX_CAN_LOG_ICON, m_hLogIcon2);
                    m_nIconSetFlag = 2;
                }
                else if(m_nIconSetFlag == 2)
                {
					m_wndStatusBar.SetPaneIcon(INDEX_CAN_LOG_ICON, m_hLogIcon1);
                    m_nIconSetFlag = 1;
                }

                for ( int i = 0 ; i < 100 ; i++ )
                {
                    if ( sg_pouFrameProcCAN->IsThreadBlocked() == TRUE )
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
                    sg_pouFrameProcCAN->DisableDataLogFlag();
                }
            }
            else
            {
                m_wndStatusBar.SetPaneIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
            }
        }
        else
        {
			m_wndStatusBar.SetPaneIcon(INDEX_CAN_LOG_ICON, m_hLogOffIcon);
        }
    }
    if(nIDEvent == m_unJ1939TimerSBLog)
    {
        if (nullptr != sg_pouIJ1939Logger)
        {
            if (sg_pouIJ1939Logger->IsDataLogged() == TRUE)
            {
                if(m_nJ1939IconSetFlag == 1)
                {
					m_wndStatusBar.SetPaneIcon(INDEX_J1939_LOG_ICON, m_hLogIcon2);
                    m_nJ1939IconSetFlag = 2;
                }
                else if(m_nJ1939IconSetFlag == 2)
                {
					m_wndStatusBar.SetPaneIcon(INDEX_J1939_LOG_ICON, m_hLogIcon1);
                    m_nJ1939IconSetFlag = 1;
                }

                for ( int i = 0 ; i < 100 ; i++ )
                {
                    if ( sg_pouIJ1939Logger->IsThreadBlocked() == TRUE )
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
                    sg_pouIJ1939Logger->DisableDataLogFlag();
                }
            }
            else
            {
				m_wndStatusBar.SetPaneIcon(INDEX_J1939_LOG_ICON, m_hLogOffIcon);
            }
        }
        else
        {
			m_wndStatusBar.SetPaneIcon(INDEX_J1939_LOG_ICON, m_hLogOffIcon);
        }
    }

    // Call Parent class member for default action
    CMDIFrameWndEx::OnTimer(nIDEvent);
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
    if(pouFlag != nullptr )
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
                CMDIFrameWndEx::OnDropFiles(hDropInfo);
                // Save the selected filename into the configuration details
                // if it is is not the same C file
                m_omMRU_C_Filename = omStrFileName;
            }
        }
    }
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
    CFlags* pouFlag = nullptr;
    pouFlag     = theApp.pouGetFlagsPtr();

    if(nullptr != pCmdUI)
    {
        pCmdUI->Enable(FALSE);
        pCmdUI->SetCheck(FALSE);
    }

    BOOL bConnect  = pouFlag->nGetFlagStatus(CONNECTED);
    if(pCmdUI != nullptr && bConnect == TRUE)
    {
        //Tested with these hardwares only
        if( m_dwDriverId == DRIVER_CAN_VECTOR_XL || m_dwDriverId == DRIVER_CAN_ICS_NEOVI || m_dwDriverId == DRIVER_CAN_ETAS_ES581 )
        {
            pCmdUI->Enable(TRUE);
        }
    }
    /*if( pCmdUI != nullptr)
    {
        CFlags* podFlags = theApp.pouGetFlagsPtr();
        if( podFlags != nullptr )
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
bool CMainFrame::bEnableDisableLog(bool bStart)
{
    bool bRetval = true ;
    BOOL bLogging = theApp.pouGetFlagsPtr()->nGetFlagStatus(LOGTOFILE);

    if (bStart && (bLogging == TRUE)) //If logging enabled and trying
    {
        //to enable
        bRetval = false;
    }
    else if (!bStart && (bLogging == FALSE)) //If logging disabled and trying
    {
        //to disable
        bRetval = false;
    }
    else
    {
        OnLogEnable();
    }
    return bRetval;
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
bool CMainFrame::bWriteToLog(char* pcOutStrLog)
{
    bool bRetval = true;
    BOOL bLogON = TRUE ;
    bLogON = theApp.pouGetFlagsPtr()->nGetFlagStatus(LOGTOFILE);

    if ((bLogON == TRUE) && (pcOutStrLog != nullptr))
    {
        //::SendMessage(GUI_hDisplayWindow,WM_LOG_STRING,0,(LPARAM)pcOutStrLog) ;
    }
    else
    {
        bRetval = false;
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
bool CMainFrame::bSetControllerMode(bool bMode)
{
    bool bRetval = true;
    int bControllerMode = theApp.pouGetFlagsPtr()->nGetFlagStatus(CONTROLLER_MODE);
    BOOL bConnect = theApp.pouGetFlagsPtr()->nGetFlagStatus(CONNECTED);

    if(bConnect == TRUE) //If connected
    {
        bRetval = false;
    }
    else //If not connected
    {
        if (bMode && (bControllerMode != 1))
        {
            OnConfigureModeActive() ;
        }
        else if(!bMode && (bControllerMode != 2))
        {
            OnConfigurePassive();
        }
        else
        {
            bRetval = false;
        }
    }
    return bRetval;
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

        if (pMainFrm != nullptr)
        {
            pMainFrm->SendMessage(WM_CREATE_TRACEWIN, 0, 0);
            g_hSemaphore = CreateSemaphore(nullptr,1,1,nullptr);
        }
    }

    if (g_dwUIThread != 0)
    {
        if ( pcOutStrTrace != nullptr)
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
    CFlags* pouFlag = nullptr;
    pouFlag     = theApp.pouGetFlagsPtr();
    BOOL bConnect  = pouFlag->nGetFlagStatus(CONNECTED);

    if(nullptr != pCmdUI)
    {
        pCmdUI->Enable(FALSE);
        pCmdUI->SetCheck(FALSE);
    }

    if(pCmdUI != nullptr && bConnect == TRUE)
    {
        //Tested with these hardwares only
        if( m_dwDriverId == DRIVER_CAN_VECTOR_XL || m_dwDriverId == DRIVER_CAN_ICS_NEOVI || m_dwDriverId == DRIVER_CAN_ETAS_ES581 )
        {
            pCmdUI->Enable(TRUE);
        }
    }
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
    if (m_podUIThread == nullptr)
    {
        bCreateTraceWindow();
        //Window should be displayed if thread is created newly
        m_bNotificWndVisible = FALSE;
    }
    if (m_podUIThread != nullptr)
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



/*******************************************************************************
  Function Name  : OnCfgnUdsMainWnd
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CMainFrame
  Author(s)      : Alejandra Sanchez
  Date Created   :
  Modifications  :
*******************************************************************************/
typedef HRESULT (*DIL_UDS_SHOWWND)(HWND hParent);
static DIL_UDS_SHOWWND pfUDS_ShowWnd;

void CMainFrame::OnCfgnUdsMainWnd()
{
    /*if((ptrDILI_UDS_Protocol = GetIUDSDIL())!= NULL){
        ptrDILI_UDS_Protocol->DisplayUdsMainWnd(this->m_hWnd);
    }*/
    //pfUDS_ShowWnd = NULL;
    //HMODULE hMod= ::LoadLibrary("UDS_Protocol.dll");
    //pfUDS_ShowWnd = (DIL_UDS_SHOWWND)::GetProcAddress(hMod, "DIL_UDS_ShowWnd");
    //pfUDS_ShowWnd(m_hWnd);
    LPARAM lParam;
    GetICANDIL()->DILC_GetControllerParams(lParam, 0, NUMBER_HW);
    HRESULT Temp = DIL_UDS_ShowWnd(this->m_hWnd,lParam);
}
/*******************************************************************************
  Function Name  : OnCfgnUdsSettingsWnd
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CMainFrame
  Author(s)      : Alejandra Sanchez
  Date Created   :
  Modifications  :
*******************************************************************************/
void CMainFrame::OnCfgnUdsSettingsWnd()
{
    HRESULT hola = DIL_UDS_ShowSettingWnd(this->m_hWnd);
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
    if(pCmdUI != nullptr)
    {
        CFlags* podFlag = theApp.pouGetFlagsPtr();
        if( podFlag != nullptr )
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
        m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eKeyPress, (UCHAR)wParam);
    }
    return 0;
}


/******************************************************************************
Function Name    :  OnDisplayRelativetime

Input(s)         :      -
Output           :  void
Functionality    :  Changes the display mode to relative mode
Member of        :  CMainFrame
Friend of        :      -
******************************************************************************/
void CMainFrame::OnDisplayRelativetime(UINT id)
{
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	id = id - IDM_RELATIVE_TIME_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
		HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)bus);
		if (hWnd)
		{
			BYTE byGetDispFlag = 0;
			::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
			CLEAR_EXPR_TM_BITS(byGetDispFlag);
			SET_TM_REL(byGetDispFlag);
			::SendMessage(hWnd, WM_WND_PROP_MODIFY, TIME_MODE, byGetDispFlag);
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
void CMainFrame::OnUpdateDisplayRelativetime(CCmdUI* pCmdUI)
{
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	auto id = pCmdUI->m_nID - IDM_RELATIVE_TIME_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
		HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(bus);
		BYTE byGetDispFlag = 0;
		::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
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
Function Name    :  OnDisplayAbsoluteTime

Input(s)         :      -
Output           :  void
Functionality    :  Changes the display mode to relative mode
Member of        :  CMainFrame
Friend of        :      -
******************************************************************************/
void CMainFrame::OnDisplayAbsoluteTime(UINT id)
{
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	id = id - IDM_ABSOLUTE_TIME_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
		HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)bus);
		if (hWnd)
		{
			BYTE byGetDispFlag = 0;
			::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
			CLEAR_EXPR_TM_BITS(byGetDispFlag);
			SET_TM_ABS(byGetDispFlag);
			::SendMessage(hWnd, WM_WND_PROP_MODIFY, TIME_MODE, byGetDispFlag);
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
******************************************************************************/
void CMainFrame::OnUpdateDisplayAbsolutetime(CCmdUI* pCmdUI)
{
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	auto id = pCmdUI->m_nID - IDM_ABSOLUTE_TIME_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		if (id >= 0 && id < sizeof(msgWndcommandToBus))
		{
			ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
			HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(bus);
			BYTE byGetDispFlag = 0;
			::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
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
void CMainFrame::OnDisplaySystemTime(UINT id)
{
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	id = id - IDM_SYSTEM_TIME_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
		HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)bus);
		if (hWnd)
		{
			BYTE byGetDispFlag = 0;
			::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
			CLEAR_EXPR_TM_BITS(byGetDispFlag);
			SET_TM_SYS(byGetDispFlag);
			::SendMessage(hWnd, WM_WND_PROP_MODIFY, TIME_MODE, byGetDispFlag);
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
	if (nullptr == m_podMsgWndThread)
	{
		return;
	}

	static int msgWndcommandToBus[] = { CAN, J1939, LIN };
	auto id = pCmdUI->m_nID - IDM_SYSTEM_TIME_MSG_WINDOW_START;
	if (id >= 0 && id < sizeof(msgWndcommandToBus))
	{
		if (id >= 0 && id < sizeof(msgWndcommandToBus))
		{
			ETYPE_BUS bus = static_cast<ETYPE_BUS>(msgWndcommandToBus[id]);
			HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(bus);
			BYTE byGetDispFlag = 0;
			::SendMessage(hWnd, WM_PROVIDE_WND_PROP, (WPARAM)(&byGetDispFlag), 0);
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
        m_bNotificWndVisible = (lParam != 0);
    }

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
bool CMainFrame::bCreateTraceWindow(void)
{
    BOOL bResult = FALSE;

    if (m_podUIThread == nullptr)
    {
        m_podUIThread = static_cast<CUIThread*> (AfxBeginThread(RUNTIME_CLASS(CUIThread),
                        THREAD_PRIORITY_NORMAL, 0, 0));

        if (m_podUIThread != nullptr)
        {
            bResult = m_podUIThread->bCreateTraceWindow(this);
            m_sNotificWndPlacement.showCmd = SW_SHOW;
            m_podUIThread->vUpdateWndCo_Ords(m_sNotificWndPlacement, TRUE);
            sg_ouAppServiceObj.m_podUIThread = m_podUIThread;
            vREP_InitReplay(&sg_ouAppServiceObj, eINIT_TRACE);
            CMsgSignal::vSetTraceWndObj(m_podUIThread);//trace wnd ptr to MsgSignal
        }
    }

    return (bResult == TRUE);
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
	CMDIFrameWndEx::OnSize(nType, cx, cy);
}


void CMainFrame::vEmptySimsysList()
{
    GetICANNodeSim()->NS_SetSimSysConfigData(nullptr, 0);
}


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
    if (sg_pouSWInterface[CAN] != nullptr)
    {
        bShowHideFlag = sg_pouSWInterface[CAN]->SW_IsWindowVisible()? TRUE : FALSE;
    }
    // Set Check as per the show status
    pCmdUI->SetCheck(bShowHideFlag);
}

void CMainFrame::OnUpdateSignalWatchWnd_LIN(CCmdUI* pCmdUI)
{
    BOOL bShowHideFlag = FALSE;
    if (sg_pouSWInterface[LIN] != nullptr)
    {
        bShowHideFlag = sg_pouSWInterface[LIN]->SW_IsWindowVisible()? TRUE : FALSE;
    }
    // Set Check as per the show status
    pCmdUI->SetCheck(bShowHideFlag);
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
    CFlags* podFlag = nullptr;
    podFlag   = theApp.pouGetFlagsPtr();
    UCHAR ucControllerMode = defMODE_SIMULATE;
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
        if( podFlag != nullptr )
        {
            podFlag->vSetFlagStatus(CONTROLLER_MODE, defMODE_SIMULATE );
        }
    }
    // Not Simulation mode or enough hardware available
    else
    {
        // Update the controller mode
        if(podFlag != nullptr )
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
    if (m_podMsgWndThread != nullptr)
    {
        for(short shBusID = CAN; shBusID < AVAILABLE_PROTOCOLS; shBusID++)
        {
            HWND hWnd;
            hWnd = m_podMsgWndThread->hGetHandleMsgWnd((ETYPE_BUS)shBusID);
            if(hWnd)
            {
                ::SendMessage(hWnd, IDM_CLEAR_MSG_WINDOW, 0, 0);
            }
        }
    }
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
    //if( m_pomGraphThread == nullptr && (nShowStatus == SW_SHOW) )
    //{
    //    // This function creates UI thread and window object
    //    bCreateGraphWindow();
    //}
    //// If thread is active then post the show message to the thread
    //// Note::
    //// The thread will update m_bGraphWindowVisible variable after displaying
    //// the window by a notification message to this class
    //if( m_pomGraphThread != nullptr )
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
//    if (m_pomGraphThread == nullptr)
//    {
//        // Create Graph UI Thread
//        m_pomGraphThread = (CGraphUIThread *)
//            AfxBeginThread( RUNTIME_CLASS(CGraphUIThread),
//                            THREAD_PRIORITY_NORMAL, 0, 0 );
//        // If success
//        if (m_pomGraphThread != nullptr)
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
//                m_pomGraphThread = nullptr;
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
        if(nBusType == 5)
        {
            //TODO: When Signal graph for ETHERNET is implemented remove this check
            //Skip Ethernet protocol
            continue;
        }
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
    //   if(m_pouBuildProgram != nullptr)
    //   {
    //       if(m_pouBuildProgram->m_podOutWnd != nullptr)
    //       {
    //           if(m_pouBuildProgram->m_podOutWnd->m_hWnd != nullptr)
    //           {
    //               // so that only the data for the output window is set..
    //               m_pouBuildProgram->m_podOutWnd->vUpdateWinStatus();
    //           }
    //       }
    //   }
    // Get Message window, signal watch and message interptetation window
    // postions
    /*if (m_podMsgMDIChild != nullptr)
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
    if ((m_podUIThread != nullptr) && (m_bNotificWndVisible == TRUE))
    {
        m_podUIThread->PostThreadMessage(WM_LOAD_SAVE_WINSTATUS, TRUE, 0);
    }

    //SGW Code commented by Arun 21-10-2010
    // Update Graph window
    /*if( m_pomGraphThread != nullptr )
    {
        m_pomGraphThread->pomGetChildFramePointer()->vUpdateWinStatus();
    }*/
    //SGW Code commented by Arun 21-10-2010

    // Update Tx Window
    /*if( m_pomTxMsgChildWindow != nullptr )
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
void CMainFrame::vStartStopLogging(bool bStart)
{
    CFlags* pouFlags = nullptr;

    pouFlags   = theApp.pouGetFlagsPtr();
    // Enable Logging or stop logging
    if (nullptr != sg_pouFrameProcCAN && pouFlags != nullptr)
    {
        // To reduce the unwanted functional call incase of stop logging
        if (bStart)
        {
            /*if(pouFlags->nGetFlagStatus(CONNECTED) != 0)
            {
                sg_pouFrameProcCAN->vSetMeasurementFileName();
            }*/
            vSetAssociatedDatabaseFiles(CAN);   // Update the db file names associated
            vSetBaudRateInfo(CAN);              // Update the baud rate details
        }

        sg_pouFrameProcCAN->EnableLogging(bStart);
    }
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
void CMainFrame::vStartStopLogging_LIN(bool bStart)
{
    // Enable Logging or stop logging
    if (nullptr != sg_pouFrameProcLIN)
        //  if (nullptr != sg_pouFrameProcLIN)
    {
        // To reduce the unwanted functional call incase of stop logging
        if (bStart)
        {

            vSetAssociatedDatabaseFiles(LIN);   // Update the db file names associated

            //  vSetAssociatedDatabaseFiles(LIN);

            vSetBaudRateInfo(LIN);              // Update the baud rate details

            //vSetBaudRateInfo(LIN);
        }

        sg_pouFrameProcLIN->EnableLogging(bStart);
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
    CStringArray omStrArrDBPaths;
    std::string strDBPath;

    int nChannelCount = 0;
    if (nullptr == m_ouBusmasterNetwork)
    {
        return;
    }
    m_ouBusmasterNetwork->GetChannelCount(eBus, nChannelCount);
    std::list<ICluster*> pClusterList;
    for (int i = 0; i < nChannelCount; i++)
    {
        m_ouBusmasterNetwork->GetDBServiceList(eBus, i, pClusterList);
for (auto itrpCluster : pClusterList)
        {
            itrpCluster->GetDBFilePath(strDBPath);
            if ((strDBPath != ""))
            {
                omStrArrDBPaths.Add(strDBPath.c_str());
            }
        }
    }
    switch (eBus)
    {
        case CAN:
            if(nullptr != sg_pouFrameProcCAN)
            {
                sg_pouFrameProcCAN->SetDatabaseFiles(omStrArrDBPaths);
            }
            break;
        case J1939:
            if (nullptr != sg_pouIJ1939Logger)
            {
                sg_pouIJ1939Logger->SetDatabaseFiles(omStrArrDBPaths);
            }
            break;
        case LIN:
            if (nullptr != sg_pouFrameProcLIN)
            {
                sg_pouFrameProcLIN->SetDatabaseFiles(omStrArrDBPaths);
            }
            break;
        default:
            break;
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
        if (nullptr != sg_pouFrameProcCAN)
        {
            // Update the baudrate info and the number of channels used
            sg_pouFrameProcCAN->FPC_SetChannelBaudRateDetails
            (m_asControllerDetails, m_nNumChannels);
        }
    }
    else if (J1939 == eBus)
    {
        if (nullptr != sg_pouIJ1939Logger)
        {
            // Update the baudrate info and the number of channels used
            sg_pouIJ1939Logger->FPJ1_SetChannelBaudRateDetails
            (m_asControllerDetails, m_nNumChannels);
        }
    }

    else if(LIN == eBus)
    {
        if(nullptr != sg_pouFrameProcLIN)
        {

            if ( g_pouDIL_LIN_Interface != nullptr )
            {

                int nCount = 0;
                g_pouDIL_LIN_Interface->DILL_GetConfiguration(m_asControllerDetailsLIN, nCount);
                sg_pouFrameProcLIN->FPL_SetChannelBaudRateDetails(m_asControllerDetailsLIN,nCount);
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
void CMainFrame::vComStartStopLog(bool bStart)
{
    vStartStopLogging(bStart);
}

void CMainFrame::vComStartStopLog_LIN(bool bStart)
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
    if( pCmdUI != nullptr )
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


    CBaseNodeSim* pNodeSim = nullptr;
    pNodeSim = GetILINNodeSim();

    if(pNodeSim != nullptr)
    {
        pNodeSim->NS_SetBmNetworkConfig( m_ouBusmasterNetwork );
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
    m_sExFuncPtr[LIN].DisConnectTool = nullptr;
    m_sExFuncPtr[LIN].EnDisableLog = nullptr;
    m_sExFuncPtr[LIN].WriteToLog = nullptr;
    m_sExFuncPtr[LIN].RestController = nullptr;
    m_sExFuncPtr[LIN].m_pouITraceWndPtr = &sg_ouAppServiceObj;
    //m_sExFuncPtr.m_pNetWorkMcNet = &m_odNetwork;

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
    return CMDIFrameWndEx::OnHelpInfo(pHelpInfo);
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
    ::HtmlHelp(nullptr, omStrPath, HH_DISPLAY_TOPIC, 0);
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

    std::list<ICluster*> clusterList;
    m_ouBusmasterNetwork->GetDBServiceList( CAN, 0, clusterList );
    std::string path;
    std::list<std::string> dbPathList;
for ( auto cluster : clusterList )
    {
        cluster->GetDBFilePath( path );
        dbPathList.push_back( path );
    }

    CDatabaseDissociateDlg odDBDialog( dbPathList );
    if ( IDOK == odDBDialog.DoModal() )
    {
        dbPathList = odDBDialog.GetDissociatedFiles();
for ( auto dbPath : dbPathList )
        {
            m_ouBusmasterNetwork->DeleteDBService( CAN, 0, dbPath );
        }

        ////////////////////////////////////////////////////////////

        m_ouBusmasterNetwork->SetChannelCount( CAN, 1 );

        m_objTxHandler.SetNetworkConfig(CAN, m_ouBusmasterNetwork);

        ::SendMessage( m_podMsgWndThread->hGetHandleMsgWnd( CAN ),
                       WM_NOTIFICATION_FROM_OTHER,
                       eLOAD_DATABASE,
                       (LPARAM)m_ouBusmasterNetwork );


        CBaseNodeSim* pNodeSim = nullptr;
        pNodeSim = GetICANNodeSim();
        if ( pNodeSim != nullptr )
        {
            pNodeSim->NS_SetBmNetworkConfig( m_ouBusmasterNetwork, true );
        }

        sg_pouSWInterface[CAN]->SW_ClearSigWatchWnd();
        sg_pouSWInterface[CAN]->SW_SetClusterInfo( m_ouBusmasterNetwork );

        ////////////////////////////


        vUpdateMainEntryListInWaveDataHandler();
        vClearSignalInfoList();
        if (!m_ouWaveTransmitter.bIsBlockEnabled())
        {
            theApp.pouGetFlagsPtr()->vSetFlagStatus(SEND_SIGNAL_MSG, FALSE);
        }

        //Update Message windows
        vUpdateAllMsgWndInterpretStatus(CAN, FALSE);

        // Check for Graph list
        for (register int nBusID = CAN; nBusID < AVAILABLE_PROTOCOLS; nBusID++)
        {
            if (m_odGraphList[nBusID].m_omElementList.GetSize() > 0)
            {
                // Clear Graph List for all buses.
                for (register int nID = CAN; nID < AVAILABLE_PROTOCOLS; nID++)
                {
                    m_odGraphList[nID].m_omElementList.RemoveAll();
                }

                // Send the Message to the Left View to Update List for all buses
                vPostConfigChangeCmdToSigGrphWnds(FALSE);


                break;
            }
        }

		mPluginManager->notifyPlugins(eBusmaster_Event::database_dissociated, nullptr);
    }

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
void CMainFrame::vUpdateAllMsgWndInterpretStatus(ETYPE_BUS eBusType, bool /* bAssociate */)
{
    // This is onvoked only for CAN and J1939 database disassociate calls
    HWND hWnd = nullptr;
    //Update MsgWnds
    hWnd = m_podMsgWndThread->hGetHandleMsgWnd(eBusType);
    if(hWnd)
    {
        ::SendMessage(hWnd, WM_DATABASE_CHANGE, (WPARAM)FALSE, 0);
    }
}

/**
 * Called from frame work. This function will execute
 * CAPL_2_C.exe which resides in the application
 * directory.
 */
void CMainFrame::OnFileConverter()
{
    try
    {
        // If window is already created and displayed then just bring it to front
        //m_hProcess = nullptr; //KSS
        CWnd* pWndCreated = IsWindowCreated();
        if (nullptr != pWndCreated && pWndCreated->GetSafeHwnd())
        {
            pWndCreated->SetForegroundWindow();
            return;
        }

        // Get the working directory
        char acPath[MAX_PATH] = "";
        GetModuleFileName( nullptr, acPath, MAX_PATH );
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

            INT nSuccess = CreateProcess(   strPath.GetBuffer(MAX_PATH), "", nullptr, nullptr,
                                            true, CREATE_NO_WINDOW, nullptr, nullptr,
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
void CMainFrame::OnLDFEditor()
{
    try
    {
        // Get the working directory
        char acPath[MAX_PATH] = "";
        GetModuleFileName( nullptr, acPath, MAX_PATH );
        PathRemoveFileSpec(acPath);
        CString strPath = acPath;
        strPath += "\\LDFEditor.exe";

        if(PathFileExists(strPath) == TRUE)
        {
            // Launch the converter utility
            PROCESS_INFORMATION sProcessInfo;
            STARTUPINFO sStartInfo;

            memset(&sProcessInfo, 0, sizeof(PROCESS_INFORMATION));
            memset(&sStartInfo, 0, sizeof(STARTUPINFO));

            int nSuccess = CreateProcess(strPath.GetBuffer(MAX_PATH),"",
                                         nullptr, nullptr, false, CREATE_NO_WINDOW, nullptr, nullptr,
                                         &sStartInfo, &sProcessInfo);
            if(!nSuccess)
            {
                AfxMessageBox("Unable to launch LDF Editor.",MB_ICONSTOP|MB_OK);
            }
        }
    }
    catch(...)
    {
    }

}
CWnd* CMainFrame::IsWindowCreated()
{
    if (nullptr == m_hProcess) //KSS
    {
        return nullptr;
    }
    // Check if the window is already created
    CWnd* pWndCreated = nullptr;
    try
    {
        CWnd* pWnd = FindWindow(nullptr, _("BUSMASTER Format Conversions"));

        // Following check is necessay to get the process created by a particular instance
        // of the application and to avoid unncessary closing or showing the process created
        // by another instance of the application
        if (nullptr != pWnd)
        {
            // Get the process id of the process created by using the process handle
            DWORD dwId = GetProcessId(m_hProcess);
            // Open the process
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwId);

            if (nullptr != hProcess)
            {
                // Get the first window
                HWND hWnd = ::GetTopWindow(0);
                DWORD dwWndProcId = 0;
                DWORD dwTheardProcId = 0;
                HANDLE hThreadProc = nullptr;

                // Loop through all the windows and get the window handle
                while ( nullptr != hWnd )
                {
                    dwTheardProcId = GetWindowThreadProcessId(hWnd, &dwWndProcId);
                    hThreadProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwWndProcId);
                    if (nullptr != hThreadProc && dwWndProcId == dwId)
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
    //if (nullptr != pWndCreated && pWndCreated->GetSafeHwnd())
    //{
    //  pWndCreated->SendMessageA(WM_CLOSE);
    //}

    try
    {
        //Method 2
        CWnd* pWnd = nullptr;
        DWORD dwExitCode = 0;

        if (nullptr != m_hProcess) //KSS
        {
            pWnd = FindWindow(nullptr, _("BUSMASTER Format Conversions"));
            if (nullptr != pWnd)
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
* \param[in]     nullptr
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
    if ((m_podUIThread == nullptr) && (!m_bNotificWndVisible))
    {
        bCreateTraceWindow();
    }

    if (m_podUIThread != nullptr)
    {
        m_podUIThread->PostThreadMessage(WM_THREADMSG_PROC, WM_MODIFY_VISIBILITY, SW_SHOW);
        m_bNotificWndVisible = TRUE;
    }
    return S_OK;
}

void CMainFrame::vReRegisterAllCANNodes(void)
{
    xmlNodePtr pCANSimSys = nullptr;
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
    xmlNodePtr pLinSimSys = nullptr;
    GetILINNodeSim()->NS_GetSimSysConfigData(pLinSimSys);
    CStringArray pomErrorFiles;
    GetILINNodeSim()->NS_DllUnloadAll(&pomErrorFiles);
    GetILINNodeSim()->NS_SetSimSysConfigData(pLinSimSys);
    vNS_LINInitCFileFunctPtrs();
    GetILINNodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[LIN]), UPDATE_ALL);
    xmlFreeNode(pLinSimSys);
}

void CMainFrame::vReRegisterAllJ1939Nodes(void)
{
    xmlNodePtr pJ1939SimSys = nullptr;
    GetIJ1939NodeSim()->NS_GetSimSysConfigData(pJ1939SimSys);
    CStringArray pomErrorFiles;
    GetIJ1939NodeSim()->NS_DllUnloadAll(&pomErrorFiles);
    GetIJ1939NodeSim()->NS_SetSimSysConfigData(pJ1939SimSys);
    NS_InitJ1939SpecInfo();
    GetIJ1939NodeSim()->NS_UpdateFuncStructsNodeSimEx((PVOID)&(m_sExFuncPtr[J1939]), UPDATE_ALL);
    xmlFreeNode(pJ1939SimSys);
}

HRESULT CMainFrame::IntializeDIL(UINT unDefaultChannelCnt, bool bLoadedFromXml)
{
    HRESULT hResult = S_OK;
    m_bNoHardwareFound = true;
    if (g_pouDIL_CAN_Interface == nullptr)
    {
        hResult = DIL_GetInterface( CAN, (void**)&g_pouDIL_CAN_Interface );
    }
    else
    {
        //g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();
        //DeselectJ1939Interfaces();
    }
    if (hResult == S_OK)
    {
        if ((hResult = g_pouDIL_CAN_Interface->DILC_SelectDriver(m_dwDriverId, m_hWnd)) == S_OK)
        {
            g_pouDIL_CAN_Interface->DILC_PerformInitOperations();
            INT nCount = unDefaultChannelCnt;
            if(S_FALSE == g_pouDIL_CAN_Interface->DILC_SetHardwareChannel(m_asControllerDetails,m_dwDriverId, false))
            {
                theApp.bWriteIntoTraceWnd(_("Channel Selection failed. Setting to default channel selection"));
            }
			if ((hResult = g_pouDIL_CAN_Interface->DILC_ListHwInterfaces(m_asINTERFACE_HW, nCount, m_asControllerDetails, bLoadedFromXml)) == S_OK)
            {
                DeselectJ1939Interfaces();
                // On Deactivate, deactivating J1939 and hiding the message window
                GetIJ1939NodeSim()->NS_SetJ1939ActivationStatus(false);

                if ( nullptr != m_podMsgWndThread )
                {
                    m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, J1939);
                }
                if (m_pouTxMsgWndJ1939 != nullptr)
                {
                    m_pouTxMsgWndJ1939->ShowWindow(SW_HIDE);
                }
                HRESULT hResult = g_pouDIL_CAN_Interface->DILC_SelectHwInterfaces(m_asINTERFACE_HW, nCount);
                if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
                {
                    hResult = g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, "CAN_MONITOR");
                    if ((hResult == S_OK)|| (hResult == ERR_CLIENT_EXISTS))
                    {
                        m_bNoHardwareFound = false;
                        HRESULT hResultCntrl = g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails, nCount);
                        if(hResultCntrl == DILC_WARN_PARAM_ADAPTED)
                        {
                            theApp.bWriteIntoTraceWnd("Warning: One of the CAN channel's supplied configuration parameter(s) are adapted in a compatible manner.");
                        }
                        bInitFrameProcCAN(); // Initialize logger module
                        vReRegisterAllCANNodes();
                        vSW_DoInitialization(CAN);
                        if ( nullptr != m_podMsgWndThread )
                        {
                            m_podMsgWndThread->vUpdateClientID(CAN, g_dwClientID);
                            if (m_podMsgWndThread != nullptr)//Msg window
                            {
                                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                                              eWINID_MSG_WND_GET_BUFFER_DETAILS, (LPARAM)m_anMsgBuffSize[CAN]);
                            }
                        }
                        //initialize graph window read buffer
                        vInitializeGraphWndReadBuffer();
                        //re initialize statistics module
                        vInitializeBusStatCAN();
                        // Initialize Read Thread of TSExecutor.
                        m_objTSExecutorHandler.vDoInitailization(CAN);

                        /* Updates the number of channels selected */
                        m_nNumChannels = nCount;

                        /*Update hardware info in status bar*/
                        vUpdateHWStatusInfo();

						ETYPE_BUS busType = ETYPE_BUS::CAN;
						mPluginManager->notifyPlugins(eBusmaster_Event::driver_selection_changed, &busType);
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
                    // Setting Previous hardware selection
                    g_pouDIL_CAN_Interface->DILC_SetHardwareChannel(m_asControllerDetails,m_dwDriverId);
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
            else
            {
                if(S_FALSE == g_pouDIL_CAN_Interface->DILC_SetHardwareChannel(m_asControllerDetails,m_dwDriverId))
                {
                    theApp.bWriteIntoTraceWnd(_("Channel Selection failed. Setting to default channel selection"));
                }
            }
        }
        m_objTxHandler.vSetDILInterfacePtr(CAN,(void*)g_pouDIL_CAN_Interface);
        m_objTxHandler.vSetClientID(CAN,g_dwClientID);
		vUpdateChannelInfo(); 
    }
    return hResult;
}

HRESULT CMainFrame::IntializeDILL(UINT unDefaultChannelCnt)
{
    /* if ( m_podMsgWndThread != nullptr )
     {
         m_podMsgWndThread->vModifyVisibilityStatus( SW_SHOW, (LONG)LIN);
     }*/
    HRESULT hResult = S_OK;
    WPARAM wMsgWndState = SW_HIDE;
    m_bNoHardwareFound = true;
    if (g_pouDIL_LIN_Interface == nullptr)
    {
        hResult = DIL_GetInterface( LIN, (void**)&g_pouDIL_LIN_Interface );
    }
    else
    {
        //g_pouDIL_CAN_Interface->DILC_PerformClosureOperations();
        //DeselectJ1939Interfaces();
    }
    if (hResult == S_OK)
    {
        if ((hResult = g_pouDIL_LIN_Interface->DILL_SelectDriver(m_shLINDriverId, m_hWnd)) == S_OK)
        {
            g_pouDIL_LIN_Interface->DILL_PerformInitOperations();
            INT nCount = unDefaultChannelCnt;
            if ((hResult = g_pouDIL_LIN_Interface->DILL_ListHwInterfaces(m_asINTERFACE_HW, nCount)) == S_OK)
            {
                //DeselectJ1939Interfaces();
                HRESULT hResult = g_pouDIL_LIN_Interface->DILL_SelectHwInterfaces(m_asINTERFACE_HW, nCount);
                if ((hResult == HW_INTERFACE_ALREADY_SELECTED) || (hResult == S_OK))
                {
                    hResult = g_pouDIL_LIN_Interface->DILL_RegisterClient(TRUE, g_dwClientID, "LIN_MONITOR");
                    if ((hResult == S_OK)|| (hResult == ERR_CLIENT_EXISTS))
                    {
                        m_bNoHardwareFound = false;
                        g_pouDIL_LIN_Interface->DILL_SetConfigData(m_ouBusmasterNetwork);
                        bInitFrameProcLIN(); // Initialize logger module
                        vReRegisterAllLINNodes();//Reinitialize node simulation

                        wMsgWndState = SW_SHOW;
                        vSW_DoInitialization(LIN);
                        if ( nullptr != m_podMsgWndThread )
                        {
                            m_podMsgWndThread->vUpdateClientID(LIN, g_dwClientID);
                            if (m_podMsgWndThread != nullptr)//Msg window
                            {
                                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_NOTIFICATION_FROM_OTHER,
                                              eWINID_MSG_WND_GET_BUFFER_DETAILS, (LPARAM)m_anMsgBuffSize[LIN]);
                            }
                        }

                        m_objTSExecutorHandler.vDoInitailization(LIN);

                        /* Updates the number of channels selected */
                        m_nNumChannelsLIN = nCount;

                        /*Update hardware info in status bar*/
                        vUpdateHWStatusInfo();
                        vInitializeBusStatLIN();
                        //Update TxWindow
                        eUSERSELCTION eUserSel = eCHANNELCOUNTUPDATED;
                        m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, 0);

                        
                        if(m_shLINDriverId == DRIVER_LIN_ETAS_BOA)
                        {
                            BOOL bIsLDFAssociated = FALSE;
                            int nLinChannels = 0;
                            m_ouBusmasterNetwork->GetChannelCount(LIN, nLinChannels);
                            for(INT nChnlIndex = 0; nChnlIndex < nLinChannels; nChnlIndex++)
                            {
                                int nCount = 0;
                                m_ouBusmasterNetwork->GetDBServiceCount(LIN, nChnlIndex, nCount);
                                if(nCount == 0)
                                {
                                    bIsLDFAssociated = TRUE;
                                }
                            }

                            if(bIsLDFAssociated == FALSE)
                            {
                                theApp.bWriteIntoTraceWnd(_("Information: To monitor messages associating LDF file is required for LIN ETAS BOA"));
                            }
                        }

						ETYPE_BUS busType = ETYPE_BUS::LIN;
						mPluginManager->notifyPlugins(eBusmaster_Event::driver_selection_changed, &busType);
                    }
                    else
                    {
                        theApp.bWriteIntoTraceWnd(_("registering client failed"));
                        m_shLINDriverId = DAL_NONE;          //select simulation
                        IntializeDILL();
                        wMsgWndState = SW_HIDE;
                    }
                }
                else
                {
                    theApp.bWriteIntoTraceWnd(_("Selecting hardware interface failed"));
                    m_shLINDriverId = DAL_NONE;          //select simulation
                    IntializeDILL();
                    wMsgWndState = SW_HIDE;
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
                    wMsgWndState = SW_HIDE;
                }
                else
                {
                    theApp.bWriteIntoTraceWnd(_("Listing hardware interfaces failed"));
                    m_shLINDriverId = DAL_NONE;          //select simulation
                    IntializeDILL();
                    wMsgWndState = SW_HIDE;
                }
            }
        }
        else
        {
            if ( ERR_LOAD_DRIVER == hResult )
            {
                theApp.bWriteIntoTraceWnd(_("Driver Selection Failed"));
                m_shLINDriverId = DAL_NONE;          //select simulation
                wMsgWndState = SW_HIDE;
            }
			else if (m_shLINDriverId <= 0) // Dummy Driver for Deactvation
			{
				wMsgWndState = SW_HIDE;
				// Disable the Deactivate menu as a valid driver is selected
				/*if ( m_pDILSubMenuLin != nullptr )
				{
				m_pDILSubMenuLin->EnableMenuItem(IDC_SELECT_LIN_DRIVER + 0, false);
				}*/
			}
        }
        m_objTxHandler.vSetDILInterfacePtr(LIN, (void*)g_pouDIL_LIN_Interface);
        m_objTxHandler.vSetClientID(LIN, g_dwClientID);

        if ( nullptr != m_podMsgWndThread )
        {
            m_podMsgWndThread->vModifyVisibilityStatus(wMsgWndState, LIN);
        }

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
            if(m_oNetworkStatistics != nullptr)
            {
                m_oNetworkStatistics->vUpdateChannelInfo(CAN, nHardware);
            }
        }
        //Update TxWindow
        eUSERSELCTION eUserSel = eCHANNELCOUNTUPDATED;
        m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, 0);

        //Update J1939 Tx Window
        if ( m_pouTxMsgWndJ1939 )
        {
            m_pouTxMsgWndJ1939->vUpdateChannelIDInfo();
        }
    }

    if(g_pouDIL_LIN_Interface->DILL_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
    {
        nHardware = (UINT)lParam;
        //update Bus Stats Dlg.
        if(m_bIsStatWndCreated)
        {
            if(m_oNetworkStatistics != nullptr)
            {
                m_oNetworkStatistics->vUpdateChannelInfo(LIN, nHardware);
            }
        }
        //Update TxWindow
        eUSERSELCTION eUserSel = eCHANNELCOUNTUPDATED;
        m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel, 0);


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
            // Added Shortcut key for the hardwared device, Removing '&'
            CString strDriverName = m_ouList[i].m_acName.c_str();
            strDriverName.Replace("&", "");

            double fBaudRate = atof(m_asControllerDetails->m_omStrBaudrate.c_str());
            if(fBaudRate > 1000)
            {
                fBaudRate /= 1000;
            }
            omStrChannelDriver.Format(  _("%s - %s - %.3f Kbps (Allowed channels:%d)"),
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
    if (nullptr != theApp.m_pouMsgSignal)
    {
        IFrame* frame;
        m_ouBusmasterNetwork->GetFrame(CAN, 0, (UINT)wParam, nullptr, &frame);
        if(nullptr != frame)
        {
            std::string name;
            frame->GetName(name);
            *strMsgName = name.c_str();
        }

    }

    return 0;
}

LRESULT CMainFrame::OnProvidePGNNameFromCode(WPARAM wParam, LPARAM lParam)
{
    CString* strMsgName = (CString*)lParam;
    if (nullptr != theApp.m_pouMsgSignal)
    {
        IFrame* frame;
        m_ouBusmasterNetwork->GetFrame(J1939, 0, (UINT)wParam, nullptr, &frame);
        if (nullptr != frame)
        {
            std::string name;
            frame->GetName(name);
            *strMsgName = name.c_str();
        }
    }

    return 0;


}

bool CMainFrame::bInitFrameProcCAN(void)
{
    bool bReturn = false;
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
            bReturn = true;
        }
    }
    return bReturn;
}

bool CMainFrame::bInitFrameProcLIN(void)
{
    bool bReturn = false;
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
            bReturn = true;
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

bool CMainFrame::COM_SaveConfigFile()
{
    OnSaveConfigFile();
    return true;
}

bool CMainFrame::COM_SaveConfigFileAs(CString omCfgFilePath)
{
    // Save the file
    CConfigData::ouGetConfigDetailsObject().vCloseConfigFile();
    vSetFileStorageInfo(omCfgFilePath);
    SaveConfiguration();
    return true;
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

    vUpdateMsgNameCodeList(theApp.m_pouMsgSignal, m_sExFuncPtr[CAN].m_odMsgNameMsgCodeListDB);
    // Send KeyPanel list pointer
    //m_sExFuncPtr.m_podNodeToDllMap = &g_odNodeToDllMap;
    //m_sExFuncPtr.m_podKeyPanelEntryList = &g_odKeyPanelEntryList;
    m_sExFuncPtr[CAN].m_omAppDirectory = m_omAppDirectory;
    m_sExFuncPtr[CAN].m_omObjWrapperName = WRAPPER_NAME;
    m_sExFuncPtr[CAN].m_omStructFile = STRUCT_FILE;
    m_sExFuncPtr[CAN].m_omStructName = MSG_STRUCT_NAME;
    m_sExFuncPtr[CAN].Send_Msg = App_SendMsg;
    m_sExFuncPtr[CAN].DisConnectTool = nullptr;
    m_sExFuncPtr[CAN].EnDisableLog = nullptr;
    m_sExFuncPtr[CAN].WriteToLog = nullptr;
    m_sExFuncPtr[CAN].RestController = nullptr;
    m_sExFuncPtr[CAN].m_pouITraceWndPtr = &sg_ouAppServiceObj;

    CBaseNodeSim* pNodeSim = nullptr;
    pNodeSim = GetICANNodeSim();

    if (pNodeSim != nullptr)
    {
        pNodeSim->NS_SetBmNetworkConfig(m_ouBusmasterNetwork, false);
    }
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


    vUpdateMsgNameCodeList(m_pouMsgSigJ1939, m_sExFuncPtr[J1939].m_odMsgNameMsgCodeListDB);
    // Send KeyPanel list pointer
    //m_sExFuncPtr.m_podNodeToDllMap = &g_odNodeToDllMap;
    //m_sExFuncPtr.m_podKeyPanelEntryList = &g_odKeyPanelEntryList;
    m_sExFuncPtr[J1939].m_omAppDirectory = m_omAppDirectory;
    m_sExFuncPtr[J1939].m_omObjWrapperName = WRAPPER_NAME_J1939;
    m_sExFuncPtr[J1939].m_omStructFile = STRUCT_FILE_J1939;
    m_sExFuncPtr[J1939].m_omStructName = MSG_STRUCT_NAME_J1939;
    m_sExFuncPtr[J1939].Send_Msg = App_SendMsg;
    m_sExFuncPtr[J1939].DisConnectTool = nullptr;
    m_sExFuncPtr[J1939].EnDisableLog = nullptr;
    m_sExFuncPtr[J1939].WriteToLog = nullptr;
    m_sExFuncPtr[J1939].RestController = nullptr;
    m_sExFuncPtr[J1939].m_pouITraceWndPtr = &sg_ouAppServiceObj;

    CBaseNodeSim* pNodeSim = nullptr;
    pNodeSim = GetIJ1939NodeSim();
    if (pNodeSim != nullptr)
    {
        pNodeSim->NS_SetBmNetworkConfig(m_ouBusmasterNetwork, false);
    }
    //m_sExFuncPtr.m_pNetWorkMcNet = &m_odNetwork;

}
void CMainFrame::vUpdateMsgNameCodeList(CMsgSignal* pMsgSig, CMsgNameMsgCodeListDataBase& odMsgNameMsgCodeListDB)
{
    if(pMsgSig != nullptr)
    {
        pMsgSig->bFillDbStructure(odMsgNameMsgCodeListDB);
    }
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CMDIFrameWndEx::OnActivate(nState, pWndOther, bMinimized);
    if(nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
    {
        CMDIChildWnd* pChild = this->MDIGetActive();
        if(pChild)
        {
            CRuntimeClass* pRunTimeClass = pChild->GetRuntimeClass();
            if(strcmp(pRunTimeClass->m_lpszClassName, "CMsgFrmtWnd")==0)
            {
                if(m_podMsgWndThread!=nullptr)
                {
                    HWND hWnd = m_podMsgWndThread->hGetHandleMsgWnd(CAN);
                    ::SendMessage(hWnd, WM_SETFOCUS_MSGWND_LIST, (WPARAM)true, 0);
                }
            }
        }
    }
}

INT CMainFrame::nLoadConfigFile(CString omConfigFileName)
{
    m_bIsXmlConfig = TRUE;
    int nRetValue = S_FALSE;
    if (!omConfigFileName.IsEmpty())
    {
        CConfigData::ouGetConfigDetailsObject().vCloseConfigFile();
        vSetFileStorageInfo(omConfigFileName);

        std::string strCfxFile = (LPCSTR)omConfigFileName;
        nRetValue = nLoadXMLConfiguration(strCfxFile);
        if ( nRetValue != S_OK )
        {
            AfxMessageBox (_("The Configuration file is not Valid\nLoading Default Configuration."),
                           MB_OK | MB_ICONERROR);
        }
    }
    if ( nRetValue != S_OK )            //If Default Configuration
    {
        for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
        {
            //TODO:: this has to bo moved to xml config
            /* call with nullptr to load the default configuration*/
            vSetDefaultConfiguration(eSecId);
            eSecId = static_cast<eSECTION_ID>(eSecId + 1);
        }
        bCreateTraceWindow();
    }
    m_bIsXmlConfig = TRUE;



    ApplyLogFilter();
    ApplyMessageFilterButton();
    ApplyReplayFilter();
    ApplyMessagewindowOverwrite();
    ApplyLINLogFilter();
    //vCreateMRU_Menus();
    m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, J1939);

    m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, LIN);

    // Deactivate J1939
    DeselectJ1939Interfaces(FALSE);
    GetIJ1939NodeSim()->NS_SetJ1939ActivationStatus(false);
    m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, J1939);
    return nRetValue;
}

void CMainFrame::BuildAllNodes()
{
    GetICANNodeSim()->NS_DLLBuildAll();


    vReRegisterAllJ1939Nodes();
    GetIJ1939NodeSim()->NS_DLLBuildAll();


    vReRegisterAllLINNodes();
    GetILINNodeSim()->NS_DLLBuildAll();
}


int CMainFrame::nLoadXMLConfiguration(std::string& m_strCfxFile)
{
    int nRetValue = S_FALSE;

    m_xmlConfigFiledoc = xmlReadFile(m_strCfxFile.c_str(), "UTF-8", 0);
    //m_xmlConfigFiledoc = xmlParseFile(m_strCfxFile.c_str());

    CString strEncoding = "UTF-8";

    //m_xmlConfigFiledoc->encoding = (BAD_CAST strEncoding.GetBuffer(strEncoding.GetLength()));

    if ( nullptr != m_xmlConfigFiledoc )           //1. Try to Load as a XML file
    {
        //AfxMessageBox("Loaded");
        mPluginManager->notifyPlugins(eBusmaster_Event::new_configuration_loading, nullptr);

        nRetValue = nLoadXMLConfiguration();

        mPluginManager->setPluginConfiguration(m_xmlConfigFiledoc);

        mPluginManager->notifyPlugins(eBusmaster_Event::new_configuration_loaded, nullptr);

        xmlFreeDoc(m_xmlConfigFiledoc);
        m_xmlConfigFiledoc = nullptr;
    }
    return nRetValue;
}

INT CMainFrame::vSaveXMLConfiguration(const char* filename)
{

    xmlDocPtr pXMLDocPtr = nullptr;       /* document pointer */
    xmlNodePtr pRtNodePtr = nullptr;
    xmlDtdPtr dtd = nullptr;       /* DTD pointer */

    xmlKeepBlanksDefault(0);

    // Create the document with version 1.0
    pXMLDocPtr = xmlNewDoc(BAD_CAST "1.0");

    CString strEncoding = "UTF-8";

    // Creating the Root node "BUSMAST
    pRtNodePtr = xmlNewNode(nullptr, BAD_CAST DEF_BUSMASTER_CONFIGURATION);

    xmlDocSetRootElement(pXMLDocPtr, pRtNodePtr);

    // GLOBAL Configuration
    xmlNodePtr pGblCnfgPtr = xmlNewNode(nullptr, BAD_CAST DEF_GLOBAL_CONFIGURATION);
    xmlAddChild(pRtNodePtr, pGblCnfgPtr);

    // MODULE Configuration
    xmlNodePtr pModConfigNdPtr = xmlNewNode(nullptr, BAD_CAST DEF_MODULE_CONFIGURATION);
    xmlAddChild(pRtNodePtr, pModConfigNdPtr);

    int nRetValue = S_OK;

    for (eSECTION_ID eSecId = DATABASE_SECTION_ID; eSecId < SECTION_TOTAL;)
    {
        if(eSecId == MAINFRAME_SECTION_ID)
        {
            vGetCurrentSessionData(eSecId,  pGblCnfgPtr);
        }
        else
        {
            vGetCurrentSessionData(eSecId, pModConfigNdPtr);
        }

        eSecId = static_cast<eSECTION_ID>(eSecId + 1);
    }


    mPluginManager->getPluginConfiguration(pModConfigNdPtr);

    xmlIndentTreeOutput = 1;
    xmlThrDefIndentTreeOutput(TRUE);

    if(pXMLDocPtr != nullptr)
    {
        xmlSaveFormatFileEnc(filename, pXMLDocPtr, "UTF-8", 1);

        //xmlSaveToFilename(filename, "UTF-8", nullptr);

        //xmlSaveFileEnc(filename, pXMLDocPtr, "UTF-8");

        xmlFreeDoc(pXMLDocPtr);
    }

    xmlCleanupParser();

    return nRetValue;
}

void CMainFrame::vGetCurrentSessionData(eSECTION_ID eSecId, xmlNodePtr pNodePtr)
{
    switch (eSecId)
    {
        case MAINFRAME_SECTION_ID:
        {
            // Saving Application Version in to cfx file
            CString strVersion;

            // Application version
            strVersion.Format("%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

            xmlNodePtr pAppVersion = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_APPLICATION_VERSION
                                                 , BAD_CAST strVersion.GetBuffer(strVersion.GetLength()));
            xmlAddChild(pNodePtr, pAppVersion);

            BYTE byVersion = 0x2;

            char acName[MAX_PATH] = { '\0' };
            strcpy_s(acName, MAX_PATH, m_omMRU_C_Filename.GetBuffer(MAX_PATH));


            theApp.pouGetFlagsPtr()->vGetToolbarButtonStatus(&m_sToolBarInfo);

            // Writing IsMsgFilterEnabled in to xml
            CString strIsMsgFilterEnabled = "";
            if(m_sToolBarInfo.m_byMsgFilter == TRUE)
            {
                strIsMsgFilterEnabled = "TRUE";
            }
            else if(m_sToolBarInfo.m_byMsgFilter == FALSE)
            {
                strIsMsgFilterEnabled = "FALSE";
            }

            xmlNodePtr pMsgFilterEnbld = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_IsMsgFilterEnabled
                                         , BAD_CAST strIsMsgFilterEnabled.GetBuffer(strIsMsgFilterEnabled.GetLength()));
            xmlAddChild(pNodePtr, pMsgFilterEnbld);

            // Writing IsMsgFilterEnabledLin in to xml
            strIsMsgFilterEnabled = "";
            if(m_sToolBarInfo.m_byMsgFilterLin == TRUE)
            {
                strIsMsgFilterEnabled = "TRUE";
            }
            else if(m_sToolBarInfo.m_byMsgFilterLin == FALSE)
            {
                strIsMsgFilterEnabled = "FALSE";
            }

            xmlNodePtr pMsgFilterEnbldLin = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_IsMsgFilterEnabledLin
                                            , BAD_CAST strIsMsgFilterEnabled.GetBuffer(strIsMsgFilterEnabled.GetLength()));
            xmlAddChild(pNodePtr, pMsgFilterEnbldLin);

            //CAN-ReplyFilter
            CString strIsReplayFilterEnabled = "";
            if(m_sToolBarInfo.m_byReplayFilter == TRUE)
            {
                strIsReplayFilterEnabled = "TRUE";
            }
            else if(m_sToolBarInfo.m_byReplayFilter == FALSE)
            {
                strIsReplayFilterEnabled = "FALSE";
            }
            xmlNodePtr pReplayFilterEnbld = xmlNewChild(pNodePtr, nullptr, BAD_CAST "IsReplayFilterEnabled"
                                            , BAD_CAST strIsReplayFilterEnabled.GetBuffer(strIsReplayFilterEnabled.GetLength()));
            xmlAddChild(pNodePtr, pReplayFilterEnbld);
            // Writing IsLogFilterEnabled in to xml
            CString strIsLogFilterEnabled = "";
            if(m_sToolBarInfo.m_byLogFilter == TRUE)
            {
                strIsLogFilterEnabled = "TRUE";
            }
            else if(m_sToolBarInfo.m_byLogFilter == FALSE)
            {
                strIsLogFilterEnabled = "FALSE";
            }

            xmlNodePtr pLogFilterEnbld = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_IsLogFilterEnabled
                                         , BAD_CAST strIsLogFilterEnabled.GetBuffer(strIsLogFilterEnabled.GetLength()));
            xmlAddChild(pNodePtr, pLogFilterEnbld);

            // Writing IsLogFilterEnabledLIN in to xml
            CString strIsLogFilterLINEnabled = "";
            if(m_sToolBarInfo.m_byLogFilterLIN == TRUE)
            {
                strIsLogFilterLINEnabled = "TRUE";
            }
            else if(m_sToolBarInfo.m_byLogFilterLIN == FALSE)
            {
                strIsLogFilterLINEnabled = "FALSE";
            }

            xmlNodePtr pLogFilterLINEnbld = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_IsLogFilterLINEnabled
                                            , BAD_CAST strIsLogFilterLINEnabled.GetBuffer(strIsLogFilterLINEnabled.GetLength()));
            xmlAddChild(pNodePtr, pLogFilterLINEnbld);

            // Writing IsLoggingEnabled in to xml
            CString strIsLoggingEnabled = "";
            if(m_sToolBarInfo.m_byLogging == TRUE)
            {
                strIsLoggingEnabled = "TRUE";
            }
            else if(m_sToolBarInfo.m_byLogging == FALSE)
            {
                strIsLoggingEnabled = "FALSE";
            }

            xmlNodePtr pLoggingEnbld = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_IsLoggingEnabled
                                                   , BAD_CAST strIsLoggingEnabled.GetBuffer(strIsLoggingEnabled.GetLength()));
            xmlAddChild(pNodePtr, pLoggingEnbld);

            // Writing IsMsgIntepretationEnabled in to xml
            CString strIsMsgIntepretationEnabled = "";
            if(m_sToolBarInfo.m_byMsgInterpret == TRUE)
            {
                strIsMsgIntepretationEnabled = "TRUE";
            }
            else if(m_sToolBarInfo.m_byMsgInterpret == FALSE)
            {
                strIsMsgIntepretationEnabled = "FALSE";
            }

            xmlNodePtr pMsgIntrEnbld = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_IsMsgIntepretationEnabled
                                                   , BAD_CAST strIsMsgIntepretationEnabled.GetBuffer(strIsMsgIntepretationEnabled.GetLength()));
            xmlAddChild(pNodePtr, pMsgIntrEnbld);

            // Writing IsOverWriteEnabled in to xml
            CString strIsOverWriteEnabled = "";
            if(m_sToolBarInfo.m_byOverwrite == TRUE)
            {
                strIsOverWriteEnabled = "TRUE";
            }
            else if(m_sToolBarInfo.m_byOverwrite == FALSE)
            {
                strIsOverWriteEnabled = "FALSE";
            }

            xmlNodePtr pOverwriteEnbld = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_IsOverWriteEnabled
                                         , BAD_CAST strIsOverWriteEnabled.GetBuffer(strIsOverWriteEnabled.GetLength()));
            xmlAddChild(pNodePtr, pOverwriteEnbld);

            // Writing DisplayTimeMode in to xml
            CString strDisplayTimeMode = "";
            if(m_sToolBarInfo.m_byDisplayTimeMode == eSYSTEM_MODE)
            {
                strDisplayTimeMode = "SYSTEM";
            }
            else if(m_sToolBarInfo.m_byDisplayTimeMode == eABSOLUTE_MODE)
            {
                strDisplayTimeMode = "ABSOLUTE";
            }
            else if(m_sToolBarInfo.m_byDisplayTimeMode == eRELATIVE_MODE)
            {
                strDisplayTimeMode = "RELATIVE";
            }

            xmlNodePtr pDisplayTimeMode = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_DisplayTimeMode
                                          , BAD_CAST strDisplayTimeMode.GetBuffer(strDisplayTimeMode.GetLength()));
            xmlAddChild(pNodePtr, pDisplayTimeMode);

            // Writing DisplayNumericMode in to xml
            CString strDisplayNumericMode = "";
            if(m_sToolBarInfo.m_byDisplayHexON == TRUE)
            {
                strDisplayNumericMode = "FALSE";
            }
            else if(m_sToolBarInfo.m_byDisplayHexON == FALSE)
            {
                strDisplayNumericMode = "TRUE";
            }

            xmlNodePtr pDisplayNumericMode = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_DisplayNumericMode
                                             , BAD_CAST strDisplayNumericMode.GetBuffer(strDisplayNumericMode.GetLength()));
            xmlAddChild(pNodePtr, pDisplayNumericMode);

            // Writing LogOnConnect in to xml
            CString strLogOnConnectCAN = "";
            if(m_abLogOnConnect[CAN] == TRUE)
            {
                strLogOnConnectCAN = "TRUE";
            }
            else if(m_abLogOnConnect[CAN] == FALSE)
            {
                strLogOnConnectCAN = "FALSE";
            }

            xmlNodePtr pLogOnConnectCAN = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_LogOnConnect_For_CAN
                                          , BAD_CAST strLogOnConnectCAN.GetBuffer(strLogOnConnectCAN.GetLength()));
            xmlAddChild(pNodePtr, pLogOnConnectCAN);

            // Writing LogOnConnect in to xml
            CString strLogOnConnectJ1939 = "";
            if(m_abLogOnConnect[J1939] == TRUE)
            {
                strLogOnConnectJ1939 = "TRUE";
            }
            else if(m_abLogOnConnect[J1939] == FALSE)
            {
                strLogOnConnectJ1939 = "FALSE";
            }

            xmlNodePtr pLogOnConnectJ1939 = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_LogOnConnect_For_J1939
                                            , BAD_CAST strLogOnConnectJ1939.GetBuffer(strLogOnConnectJ1939.GetLength()));
            xmlAddChild(pNodePtr, pLogOnConnectJ1939);

            // Writing LogOnConnect For LIN in to xml
            CString strLogOnConnectLIN = "";
            if(m_abLogOnConnect[LIN] == TRUE)
            {
                strLogOnConnectLIN = "TRUE";
            }
            else if(m_abLogOnConnect[LIN] == FALSE)
            {
                strLogOnConnectLIN = "FALSE";
            }

            xmlNodePtr pLogOnConnectLIN = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_LogOnConnect_For_LIN
                                          , BAD_CAST strLogOnConnectLIN.GetBuffer(strLogOnConnectLIN.GetLength()));
            xmlAddChild(pNodePtr, pLogOnConnectLIN);


            if (m_podUIThread != nullptr)
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
            xmlNodePtr pWndPosition = xmlNewNode(nullptr, BAD_CAST DEF_WINDOW_POSITION);
            xmlAddChild(pNodePtr, pWndPosition);

            // Writing Visibility in to xml
            xmlNodePtr pVisibility = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_VISIBILITY, BAD_CAST strVisibility.GetBuffer(strVisibility.GetLength()));
            xmlAddChild(pWndPosition, pVisibility);

            // Writing window placement in to xml
            xmlNodePtr pWndPlcmnt = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_WINDOW_PLACEMENT, BAD_CAST strWndPlacement.GetBuffer(strWndPlacement.GetLength()));
            xmlAddChild(pWndPosition, pWndPlcmnt);

            // Writing Top bottom left and right position in to xml
            xmlNodePtr pTopPtr = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_TOP, BAD_CAST strTop.GetBuffer(strTop.GetLength()));
            xmlAddChild(pWndPosition, pTopPtr);

            xmlNodePtr pLeftPtr = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_Left, BAD_CAST strLeft.GetBuffer(strLeft.GetLength()));
            xmlAddChild(pWndPosition, pLeftPtr);

            xmlNodePtr pBottomPtr = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_Bottom, BAD_CAST strBottom.GetBuffer(strBottom.GetLength()));
            xmlAddChild(pWndPosition, pBottomPtr);

            xmlNodePtr pRightPtr = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_Right, BAD_CAST strRight.GetBuffer(strRight.GetLength()));
            xmlAddChild(pWndPosition, pRightPtr);
        }
        break;
        case LOG_SECTION_J1939_ID:
        {
            /*if (GetIJ1939Logger() != nullptr)
            {
                GetIJ1939Logger()->FPJ1_GetConfigData(&pbyConfigData, nSize);
            }*/
            if (GetIJ1939Logger() != nullptr)
            {
                xmlNodePtr pJ1939LogPtr = xmlNewNode(nullptr, BAD_CAST DEF_J1939_LOG);
                xmlAddChild(pNodePtr, pJ1939LogPtr);

                GetIJ1939Logger()->GetConfigData(pJ1939LogPtr);
            }
        }
        break;
        case LOG_SECTION_ID:
        {
            /* if (sg_pouFrameProcCAN != nullptr)
             {
                 sg_pouFrameProcCAN->FPC_GetConfigData(&pbyConfigData, nSize);
             }*/

            if (sg_pouFrameProcCAN != nullptr)
            {
                xmlNodePtr pCanLogPtr = xmlNewNode(nullptr, BAD_CAST DEF_CAN_LOG);
                xmlAddChild(pNodePtr, pCanLogPtr);

                sg_pouFrameProcCAN->GetConfigData(pCanLogPtr);
            }
        }
        break;

        case LOG_SECTION_LIN_ID:
        {


            if (sg_pouFrameProcLIN != nullptr)
            {
                xmlNodePtr pLinLogPtr = xmlNewNode(nullptr, BAD_CAST DEF_LIN_LOG);
                xmlAddChild(pNodePtr, pLinLogPtr);

                sg_pouFrameProcLIN->GetConfigData(pLinLogPtr);
            }
        }
        break;

        case SIMSYS_SECTION_ID:
        {
            xmlNodePtr pCanSimSys = nullptr;
            GetICANNodeSim()->NS_GetSimSysConfigData(pCanSimSys);
            xmlAddChild(pNodePtr, pCanSimSys);
        }
        break;
        case SIMSYS_SECTION_LIN_ID:
        {
            xmlNodePtr pLinSimSys = nullptr;
            GetILINNodeSim()->NS_GetSimSysConfigData(pLinSimSys);
            xmlAddChild(pNodePtr, pLinSimSys);
        }
        break;

        case SIMSYS_SECTION_J1939_ID:
        {
            xmlNodePtr pJ1939SimSys = nullptr;
            GetIJ1939NodeSim()->NS_GetSimSysConfigData(pJ1939SimSys);
            xmlAddChild(pNodePtr, pJ1939SimSys);
        }
        break;
        case REPLAY_SECTION_ID:
        {

            xmlNodePtr pCANReplay = xmlNewNode(nullptr, BAD_CAST DEF_CAN_REPLAY);
            xmlAddChild(pNodePtr, pCANReplay);
            vREP_GetReplayConfigData(pCANReplay);
        }
        break;
        case MSGWND_SECTION_ID:
        {
            SMESSAGE_ATTRIB sMsgAttrib;
            sMsgAttrib.m_psMsgAttribDetails = nullptr;
            sMsgAttrib.m_usMsgCount = 0;
            CMessageAttrib::ouGetHandle(CAN).vGetMessageAttribData(sMsgAttrib);
            UINT nCount = sMsgAttrib.m_usMsgCount;
            SFILTERAPPLIED_CAN sMsgWndFilter;

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN), WM_GET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, 0);

            xmlNodePtr pCANMsgWindow = xmlNewNode(nullptr, BAD_CAST DEF_MSG_WINDOW);
            xmlAddChild(pNodePtr, pCANMsgWindow);

            INT nAppndBuffrSize = m_anMsgBuffSize[CAN][defAPPEND_DATA_INDEX];
            INT nOverwritBffrSize = m_anMsgBuffSize[CAN][defOVERWRITE_DATE_INDEX];
            INT nDisplayUpdateTime = m_anMsgBuffSize[CAN][defDISPLAY_UPDATE_DATA_INDEX];

            CString strAppndBffrSize = "";
            strAppndBffrSize.Format("%d", nAppndBuffrSize);

            CString strOverwritBffrSize = "";
            strOverwritBffrSize.Format("%d", nOverwritBffrSize);

            CString strDisplayUpdateTime = "";
            strDisplayUpdateTime.Format("%d", nDisplayUpdateTime);

            xmlNodePtr pAppndBffrPtr = xmlNewChild(pCANMsgWindow, nullptr, BAD_CAST DEF_APPEND_BUFFER_SIZE, BAD_CAST strAppndBffrSize.GetBuffer(strAppndBffrSize.GetLength()));
            xmlAddChild(pCANMsgWindow, pAppndBffrPtr);

            xmlNodePtr pOvrwriteBffrPtr = xmlNewChild(pCANMsgWindow, nullptr, BAD_CAST DEF_OVERWRITE_BUFFER_SIZE, BAD_CAST strOverwritBffrSize.GetBuffer(strOverwritBffrSize.GetLength()));
            xmlAddChild(pCANMsgWindow, pOvrwriteBffrPtr);

            xmlNodePtr pDispBffrPtr = xmlNewChild(pCANMsgWindow, nullptr, BAD_CAST DEF_DISPLAY_UPDATE_TIME, BAD_CAST strDisplayUpdateTime.GetBuffer(strDisplayUpdateTime.GetLength()));
            xmlAddChild(pCANMsgWindow, pDispBffrPtr);

            const char* omcVarChar ;

            for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
            {
                if ( sMsgAttrib.m_psMsgAttribDetails[i].sColor != RGB(0,0,0))
                {
                    xmlNodePtr pCANMsgAttribute = xmlNewNode(nullptr, BAD_CAST DEF_MSG_ATTRIBUTE);
                    xmlAddChild(pCANMsgWindow, pCANMsgAttribute);

                    omcVarChar = sMsgAttrib.m_psMsgAttribDetails[i].omStrMsgname;
                    xmlNodePtr pMsgName = xmlNewChild(pCANMsgAttribute, nullptr, BAD_CAST DEF_NAME,BAD_CAST omcVarChar);
                    xmlAddChild(pCANMsgAttribute, pMsgName);

                    CString     csMsgID;
                    csMsgID.Format("%u",sMsgAttrib.m_psMsgAttribDetails[i].unMsgID);
                    omcVarChar = csMsgID;
                    xmlNodePtr pMsgID = xmlNewChild(pCANMsgAttribute, nullptr, BAD_CAST DEF_MSG_ID,BAD_CAST omcVarChar);
                    xmlAddChild(pCANMsgAttribute, pMsgID);

                    CString csColor;
                    int nR, nB, nG;
                    nR = GetRValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nG = GetGValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nB = GetBValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    //csColor.Format("%x%x%x",nR,nG,nB);

                    char ch[10];

                    sprintf_s(ch, sizeof(ch), "%02x%02x%02x", nR,nG,nB);


                    omcVarChar = csColor;
                    xmlNodePtr pMsgColor = xmlNewChild(pCANMsgAttribute, nullptr, BAD_CAST DEF_MSG_COLOR,BAD_CAST ch);
                    xmlAddChild(pCANMsgAttribute, pMsgColor);
                }
            }

            CString     csFilter;
            for(int iCnt=0; iCnt < sMsgWndFilter.m_ushTotal; iCnt++)
            {
                csFilter.Format("%s",((sMsgWndFilter.m_psFilters)+iCnt)->m_sFilterName.m_acFilterName.c_str());
                omcVarChar = csFilter;
                xmlNodePtr pFilter = xmlNewChild(pCANMsgWindow, nullptr, BAD_CAST DEF_FILTER,BAD_CAST omcVarChar);
                xmlAddChild(pCANMsgWindow, pFilter);
                csFilter.Format("%d", ((sMsgWndFilter.m_psFilters)+iCnt)->m_bEnabled);
                xmlNewProp(pFilter, (const xmlChar*)"IsEnabled", (const xmlChar*)csFilter.GetBuffer(MAX_PATH));
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
            sMsgAttrib.m_psMsgAttribDetails = nullptr;
            sMsgAttrib.m_usMsgCount = 0;
            CMessageAttrib::ouGetHandle(LIN).vGetMessageAttribData(sMsgAttrib);
            UINT nCount = sMsgAttrib.m_usMsgCount;
            SFILTERAPPLIED_LIN sMsgWndFilter;

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN), WM_GET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, 0);

            xmlNodePtr pLINMsgWindow = xmlNewNode(nullptr, BAD_CAST DEF_LIN_MESS_WINDOW);
            xmlAddChild(pNodePtr, pLINMsgWindow);

            INT nAppndBuffrSize = m_anMsgBuffSize[LIN][defAPPEND_DATA_INDEX];
            INT nOverwritBffrSize = m_anMsgBuffSize[LIN][defOVERWRITE_DATE_INDEX];
            INT nDisplayUpdateTime = m_anMsgBuffSize[LIN][defDISPLAY_UPDATE_DATA_INDEX];

            CString strAppndBffrSize = "";
            strAppndBffrSize.Format("%d", nAppndBuffrSize);

            CString strOverwritBffrSize = "";
            strOverwritBffrSize.Format("%d", nOverwritBffrSize);

            CString strDisplayUpdateTime = "";
            strDisplayUpdateTime.Format("%d", nDisplayUpdateTime);

            xmlNodePtr pAppndBffrPtr = xmlNewChild(pLINMsgWindow, nullptr, BAD_CAST DEF_APPEND_BUFFER_SIZE, BAD_CAST strAppndBffrSize.GetBuffer(strAppndBffrSize.GetLength()));
            xmlAddChild(pLINMsgWindow, pAppndBffrPtr);

            xmlNodePtr pOvrwriteBffrPtr = xmlNewChild(pLINMsgWindow, nullptr, BAD_CAST DEF_OVERWRITE_BUFFER_SIZE, BAD_CAST strOverwritBffrSize.GetBuffer(strOverwritBffrSize.GetLength()));
            xmlAddChild(pLINMsgWindow, pOvrwriteBffrPtr);

            xmlNodePtr pDispBffrPtr = xmlNewChild(pLINMsgWindow, nullptr, BAD_CAST DEF_DISPLAY_UPDATE_TIME, BAD_CAST strDisplayUpdateTime.GetBuffer(strDisplayUpdateTime.GetLength()));
            xmlAddChild(pLINMsgWindow, pDispBffrPtr);

            const char* omcVarChar ;

            for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
            {
                if ( sMsgAttrib.m_psMsgAttribDetails[i].sColor != RGB(0,0,0))
                {
                    xmlNodePtr pLINMsgAttribute = xmlNewNode(nullptr, BAD_CAST DEF_MSG_ATTRIBUTE);
                    xmlAddChild(pLINMsgWindow, pLINMsgAttribute);

                    omcVarChar = sMsgAttrib.m_psMsgAttribDetails[i].omStrMsgname;
                    xmlNodePtr pMsgName = xmlNewChild(pLINMsgAttribute, nullptr, BAD_CAST DEF_NAME,BAD_CAST omcVarChar);
                    xmlAddChild(pLINMsgAttribute, pMsgName);

                    CString     csMsgID;
                    csMsgID.Format("%u",sMsgAttrib.m_psMsgAttribDetails[i].unMsgID);
                    omcVarChar = csMsgID;
                    xmlNodePtr pMsgID = xmlNewChild(pLINMsgAttribute, nullptr, BAD_CAST DEF_MSG_ID,BAD_CAST omcVarChar);
                    xmlAddChild(pLINMsgAttribute, pMsgID);

                    CString csColor;
                    int nR, nB, nG;
                    nR = GetRValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nG = GetGValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nB = GetBValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    //csColor.Format("%x%x%x",nR,nG,nB);

                    char ch[10];

                    sprintf_s(ch, sizeof(ch), "%02x%02x%02x", nR,nG,nB);


                    omcVarChar = csColor;
                    xmlNodePtr pMsgColor = xmlNewChild(pLINMsgAttribute, nullptr, BAD_CAST DEF_MSG_COLOR,BAD_CAST ch);
                    xmlAddChild(pLINMsgAttribute, pMsgColor);
                }
            }

            CString     csFilter;
            for(int iCnt=0; iCnt < sMsgWndFilter.m_ushTotal; iCnt++)
            {
                csFilter.Format("%s",((sMsgWndFilter.m_psFilters)+iCnt)->m_sFilterName.m_acFilterName.c_str());
                omcVarChar = csFilter;
                xmlNodePtr pFilter = xmlNewChild(pLINMsgWindow, nullptr, BAD_CAST DEF_FILTER,BAD_CAST omcVarChar);
                xmlAddChild(pLINMsgWindow, pFilter);
                csFilter.Format("%d", ((sMsgWndFilter.m_psFilters)+iCnt)->m_bEnabled);
                xmlNewProp(pFilter, (const xmlChar*)"IsEnabled", (const xmlChar*)csFilter.GetBuffer(MAX_PATH));
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
            sMsgAttrib.m_psMsgAttribDetails = nullptr;
            sMsgAttrib.m_usMsgCount = 0;
            CMessageAttrib::ouGetHandle(J1939).vGetMessageAttribData(sMsgAttrib);
            UINT nCount = sMsgAttrib.m_usMsgCount;
            UINT unMsgFrmtWndCfgSize = 0;
            ASSERT(m_podMsgWndThread != nullptr);

            if(m_podMsgWndThread->hGetHandleMsgWnd(J1939))
            {
                ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                              WM_NOTIFICATION_FROM_OTHER, eWINID_MSG_WND_GET_CONFIG_SIZE, (LPARAM)&unMsgFrmtWndCfgSize);
            }
            //CALC SIZE ENDS
            xmlNodePtr pJ1939MsgWindow = xmlNewNode(nullptr, BAD_CAST DEF_J1939_MESS_WINDOW);
            xmlAddChild(pNodePtr, pJ1939MsgWindow);

            const char* omcVarChar ;

            for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
            {
                if( sMsgAttrib.m_psMsgAttribDetails[i].sColor != RGB( 0, 0, 0 ) )
                {
                    xmlNodePtr pJ1939MsgAttribute = xmlNewNode(nullptr, BAD_CAST DEF_MSG_ATTRIBUTE);
                    xmlAddChild(pJ1939MsgWindow, pJ1939MsgAttribute);

                    omcVarChar = sMsgAttrib.m_psMsgAttribDetails[i].omStrMsgname.GetBuffer(MAX_PATH);
                    xmlNodePtr pMsgName = xmlNewChild(pJ1939MsgAttribute, nullptr, BAD_CAST DEF_NAME,BAD_CAST omcVarChar);
                    xmlAddChild(pJ1939MsgAttribute, pMsgName);

                    CString     csMsgID;
                    csMsgID.Format("%u",sMsgAttrib.m_psMsgAttribDetails[i].unMsgID);
                    omcVarChar = csMsgID.GetBuffer(MAX_PATH);
                    xmlNodePtr pMsgID = xmlNewChild(pJ1939MsgAttribute, nullptr, BAD_CAST DEF_MSG_ID,BAD_CAST omcVarChar);
                    xmlAddChild(pJ1939MsgAttribute, pMsgID);

                    CString     csColor;
                    int nR, nB, nG;
                    nR = GetRValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nG = GetGValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    nB = GetBValue(sMsgAttrib.m_psMsgAttribDetails[i].sColor);
                    //csColor.Format("%2x%2x%2x",nR,nG,nB);

                    char ch[7] = {0};

                    sprintf_s(ch, sizeof(ch), "%02x%02x%02x", nR,nG,nB);

                    omcVarChar = csColor.GetBuffer(MAX_PATH);
                    xmlNodePtr pMsgColor = xmlNewChild(pJ1939MsgAttribute, nullptr, BAD_CAST DEF_MSG_COLOR,BAD_CAST ch);
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
            xmlNodePtr pNodeSigWtchPtr = xmlNewNode( nullptr, BAD_CAST DEF_J1939_SIGNAL_WATCH );
            xmlAddChild( pNodePtr, pNodeSigWtchPtr );
            if ( sg_pouSWInterface[J1939] != nullptr )
            {
                sg_pouSWInterface[J1939]->SW_GetConfigData( pNodeSigWtchPtr );
            }
        }
        break;
        case SIGWATCH_SECTION_ID:
        {
            xmlNodePtr pNodeSigWtchPtr = xmlNewNode(nullptr, BAD_CAST DEF_SIGNAL_WATCH);
            xmlAddChild( pNodePtr, pNodeSigWtchPtr );
            if ( sg_pouSWInterface[CAN] != nullptr )
            {
                sg_pouSWInterface[CAN]->SW_GetConfigData( pNodeSigWtchPtr );
            }
        }
        break;
        case SIGWATCH_SECTION_LIN_ID:
        {
            xmlNodePtr pNodeSigWtchPtr = xmlNewNode(nullptr, BAD_CAST DEF_LIN_SIGNAL_WATCH);
            xmlAddChild(pNodePtr, pNodeSigWtchPtr);
            if (sg_pouSWInterface[LIN] != nullptr)
            {
                sg_pouSWInterface[LIN]->SW_GetConfigData(pNodeSigWtchPtr);
            }
        }
        break;

        break;
        case DIL_SECTION_ID:
        {
            if(pNodePtr != nullptr)
            {
                xmlNodePtr pNodeDBFilePtr = xmlNewNode(nullptr, BAD_CAST DEF_CAN_DIL_SECTION);
                xmlAddChild(pNodePtr, pNodeDBFilePtr);

                // Above lines have to be changed.
                CString omstrDriverName = vGetControllerName(m_dwDriverId);

                if(pNodeDBFilePtr != nullptr)
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

                    xmlNodePtr pDNPtr = xmlNewChild(pNodeDBFilePtr, nullptr, BAD_CAST DEF_DRIVER_NAME, BAD_CAST strDriverName);
                    xmlAddChild(pNodeDBFilePtr, pDNPtr);

                    xmlNodePtr pCntrlModePtr = xmlNewChild(pNodeDBFilePtr, nullptr, BAD_CAST DEF_CONTROLLER_MODE, BAD_CAST strControllerMode);
                    xmlAddChild(pNodeDBFilePtr, pCntrlModePtr);

                    xmlNodePtr pCntrlSettngsPtr = xmlNewNode(nullptr, BAD_CAST DEF_CONTROLLER_SETTINGS);
                    xmlAddChild(pNodeDBFilePtr, pCntrlSettngsPtr);

                    INT nNumOfChannelsSel = 0;

                    if(pCntrlSettngsPtr != nullptr)
                    {
                        INT nCount = 0;
                        LONG lParam = 0;

                        if(g_pouDIL_CAN_Interface != nullptr)
                        {
                            if(g_pouDIL_CAN_Interface->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
                            {
                                nNumOfChannelsSel = (UINT)lParam;
                            }
                        }

                        for(int nChnlIndex = 0; nChnlIndex < nNumOfChannelsSel; nChnlIndex++)
                        {
                            //xmlNodePtr pChnlPtr = xmlNewChild(pNodeDBFilePtr, nullptr, BAD_CAST DEF_CHANNEL, BAD_CAST "");
                            xmlNodePtr pChnlPtr = xmlNewNode(nullptr, (xmlChar*)DEF_CHANNEL);
                            xmlAddChild(pCntrlSettngsPtr, pChnlPtr);

                            SaveConfigDataToXML( m_asControllerDetails[nChnlIndex], pChnlPtr );
                        }
                        //xmlNewChild(pCntrlSettngsPtr, nullptr, BAD_CAST DEF_DRIVER_NAME, BAD_CAST strDriverName);
                    }

                    //xmlTextWriterSetIndent(pNodeDBFilePtr, 1);
                }
            }
        }
        break;
        case GRAPH_SECTION_ID:
        {
            // Generating Signal Graph in to xml
            xmlNodePtr pCanSigGrphPtr = xmlNewNode(nullptr, BAD_CAST DEF_CAN_SIGNAL_GRAPH);
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
            xmlNodePtr pWndPosition = xmlNewNode(nullptr, BAD_CAST DEF_WINDOW_POSITION);
            xmlAddChild(pCanSigGrphPtr, pWndPosition);

            // Writing Visibility in to xml
            xmlNodePtr pVisibility = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_VISIBILITY, BAD_CAST strVisibility.GetBuffer(strVisibility.GetLength()));
            xmlAddChild(pWndPosition, pVisibility);

            // Writing window placement in to xml
            xmlNodePtr pWndPlcmnt = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_WINDOW_POSITION, BAD_CAST strWndPlacement.GetBuffer(strWndPlacement.GetLength()));
            xmlAddChild(pWndPosition, pWndPlcmnt);

            // Writing Top bottom left and right position in to xml
            xmlNodePtr pTopPtr = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_TOP, BAD_CAST strTop.GetBuffer(strTop.GetLength()));
            xmlAddChild(pWndPosition, pTopPtr);

            xmlNodePtr pLeftPtr = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_Left, BAD_CAST strLeft.GetBuffer(strLeft.GetLength()));
            xmlAddChild(pWndPosition, pLeftPtr);

            xmlNodePtr pBottomPtr = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_Bottom, BAD_CAST strBottom.GetBuffer(strBottom.GetLength()));
            xmlAddChild(pWndPosition, pBottomPtr);

            xmlNodePtr pRightPtr = xmlNewChild(pWndPosition, nullptr, BAD_CAST DEF_Right, BAD_CAST strRight.GetBuffer(strRight.GetLength()));
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
            xmlNodePtr pColZero = xmlNewNode(nullptr, BAD_CAST DEF_Splitter_Window_Col_0);
            xmlAddChild(pCanSigGrphPtr, pColZero);

            xmlNodePtr pColZeroCxIdeal = xmlNewChild(pColZero, nullptr, BAD_CAST DEF_CX_IDEAL, BAD_CAST strColZeroCxIdeal.GetBuffer(strColZeroCxIdeal.GetLength()));
            xmlAddChild(pColZero, pColZeroCxIdeal);

            xmlNodePtr pColZeroCxMin = xmlNewChild(pColZero, nullptr, BAD_CAST DEF_CX_MIN, BAD_CAST strColZeroCxMin.GetBuffer(strColZeroCxMin.GetLength()));
            xmlAddChild(pColZero, pColZeroCxMin);

            // Column One
            xmlNodePtr pColOne = xmlNewNode(nullptr, BAD_CAST DEF_Splitter_Window_Col_1);
            xmlAddChild(pCanSigGrphPtr, pColOne);

            xmlNodePtr pColOneCxIdeal = xmlNewChild(pColOne, nullptr, BAD_CAST DEF_CX_IDEAL, BAD_CAST strColOneCxIdeal.GetBuffer(strColOneCxIdeal.GetLength()));
            xmlAddChild(pColOne, pColOneCxIdeal);

            xmlNodePtr pColOneCxMin = xmlNewChild(pColOne, nullptr, BAD_CAST DEF_CX_MIN, BAD_CAST strColOneCxMin.GetBuffer(strColOneCxMin.GetLength()));
            xmlAddChild(pColOne, pColOneCxMin);

            // Row zero
            xmlNodePtr pRowZero = xmlNewNode(nullptr, BAD_CAST DEF_Splitter_Window_Row_0);
            xmlAddChild(pCanSigGrphPtr, pRowZero);

            xmlNodePtr pRowZeroCxIdeal = xmlNewChild(pRowZero, nullptr, BAD_CAST DEF_CX_IDEAL, BAD_CAST strRowZeroCyIdeal.GetBuffer(strRowZeroCyIdeal.GetLength()));
            xmlAddChild(pRowZero, pRowZeroCxIdeal);

            xmlNodePtr pRowZeroCxMin = xmlNewChild(pRowZero, nullptr, BAD_CAST DEF_CX_MIN, BAD_CAST strRowZeroCyMin.GetBuffer(strRowZeroCyMin.GetLength()));
            xmlAddChild(pRowZero, pRowZeroCxMin);

            // Row One
            xmlNodePtr pRowOne = xmlNewNode(nullptr, BAD_CAST DEF_Splitter_Window_Row_1);
            xmlAddChild(pCanSigGrphPtr, pRowOne);

            xmlNodePtr pRowOneCxIdeal = xmlNewChild(pRowOne, nullptr, BAD_CAST DEF_CX_IDEAL, BAD_CAST strRowOneCyIdeal.GetBuffer(strRowOneCyIdeal.GetLength()));
            xmlAddChild(pRowOne, pRowOneCxIdeal);

            xmlNodePtr pRowOneCxMin = xmlNewChild(pRowOne, nullptr, BAD_CAST DEF_CX_MIN, BAD_CAST strRowOneCyMin.GetBuffer(strRowOneCyMin.GetLength()));
            xmlAddChild(pRowOne, pRowOneCxMin);
        }
        break;
        case TXWND_SECTION_ID:
        {
            //changes done for XML configuration
            xmlNodePtr pCANTxPtr = xmlNewNode(nullptr, BAD_CAST DEF_CAN_TX_WINDOW);
            xmlAddChild(pNodePtr, pCANTxPtr);
            m_objTxHandler.vGetTxWndConfigData(CAN, pCANTxPtr);
        }
        break;
        case FILTER_SECTION_ID:
        {
            if(pNodePtr != nullptr)
            {
                m_sFilterAppliedCAN.pbGetConfigData(pNodePtr);
            }
        }
        break;
        case FILTER_SECTION_LIN_ID:
            if(pNodePtr != nullptr)
            {
                m_sFilterAppliedLIN.pbGetConfigData(pNodePtr);
            }
            break;

        case DATABASE_SECTION_J1939_ID:
        {
            std::list<ICluster*> clusterList;
            m_ouBusmasterNetwork->GetDBServiceList(J1939, 0, clusterList);
            CStringArray omDbNames;
            CString omConfigFileName;
            std::string omStrBasePath, omStrRelativePath;
            vGetLoadedCfgFileName(omConfigFileName);
            CUtilFunctions::nGetBaseFolder(omConfigFileName.GetBuffer(MAX_PATH), omStrBasePath);

            std::string path;
            CString temp;
for (auto cluster : clusterList)
            {
                cluster->GetDBFilePath(path);
                temp = path.c_str();
                CUtilFunctions::MakeRelativePath(omStrBasePath.c_str(), temp.GetBuffer(0), omStrRelativePath);
                omDbNames.Add(omStrRelativePath.c_str());
            }


            if (pNodePtr != nullptr)
            {
                xmlNodePtr pNodeDBFilePtr = xmlNewNode(nullptr, BAD_CAST DEF_J1939_DATABASE_FILES);

                xmlAddChild(pNodePtr, pNodeDBFilePtr);

                if (pNodeDBFilePtr != nullptr)
                {
                    for (INT nDbFileIndex = 0; nDbFileIndex < omDbNames.GetSize(); nDbFileIndex++)
                    {
                        const char* strFilePath = omDbNames[nDbFileIndex];
                        xmlChar* pDBFFilePath = (xmlChar*)strFilePath;

                        xmlNodePtr pNewChildPtr = xmlNewChild(pNodeDBFilePtr, nullptr, BAD_CAST DEF_FILE_PATH, BAD_CAST pDBFFilePath);
                        xmlAddChild(pNodeDBFilePtr, pNewChildPtr);
                    }
                }
            }

        }
        break;
        case DATABASE_SECTION_ID:
        {
            std::list<ICluster*> clusterList;
            m_ouBusmasterNetwork->GetDBServiceList( CAN, 0, clusterList );
            CStringArray omDbNames;
            CString omConfigFileName;
            std::string omStrBasePath, omStrRelativePath;
            vGetLoadedCfgFileName( omConfigFileName );
            CUtilFunctions::nGetBaseFolder( omConfigFileName.GetBuffer( MAX_PATH ), omStrBasePath );

            std::string path;
            CString temp;
for ( auto cluster : clusterList )
            {
                cluster->GetDBFilePath( path );
                temp = path.c_str();
                CUtilFunctions::MakeRelativePath( omStrBasePath.c_str(), temp.GetBuffer(0), omStrRelativePath );
                omDbNames.Add( omStrRelativePath.c_str() );
            }


            if(pNodePtr != nullptr)
            {
                //xmlNodePtr pNodeDBFilePtr = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_CAN_DATABASE_FILES, BAD_CAST "");

                xmlNodePtr pNodeDBFilePtr = xmlNewNode(nullptr, BAD_CAST DEF_CAN_DATABASE_FILES);

                xmlAddChild(pNodePtr, pNodeDBFilePtr);

                if(pNodeDBFilePtr != nullptr)
                {
                    for(INT nDbFileIndex = 0; nDbFileIndex < omDbNames.GetSize(); nDbFileIndex++)
                    {
                        const char* strFilePath = omDbNames[nDbFileIndex];
                        xmlChar* pDBFFilePath = (xmlChar*)strFilePath;

                        xmlNodePtr pNewChildPtr = xmlNewChild(pNodeDBFilePtr, nullptr, BAD_CAST DEF_FILE_PATH, BAD_CAST pDBFFilePath);
                        xmlAddChild(pNodeDBFilePtr, pNewChildPtr);
                    }
                }
            }

        }
        break;
        case LIN_CLUSTER_CONFIG:
        {
            if(pNodePtr != nullptr)
            {
                //xmlNodePtr pNodeDBFilePtr = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_CAN_DATABASE_FILES, BAD_CAST "");

                xmlNodePtr pNodeDBFilePtr = xmlNewNode(nullptr, BAD_CAST DEF_LIN_CLUSTER_CONFIG);



                if(pNodeDBFilePtr != nullptr)
                {
                    for ( int i = 0; i < 1; i++ )
                    {

                        //Channel
                        xmlNodePtr pNodeChannelPtr = xmlNewNode(nullptr, BAD_CAST DEF_FLEX_CHANNEL_CHANNEL);


                        char chValue[256];

                        //Index
                        sprintf_s(chValue, sizeof(chValue), "%d", i);
                        xmlNodePtr pIndex= xmlNewChild(pNodeChannelPtr, nullptr, BAD_CAST DEF_CHANNEL_INDEX ,BAD_CAST chValue);
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //HWUri

                        pIndex= xmlNewChild(pNodeChannelPtr, nullptr, BAD_CAST DEF_CHANNEL_HWURI ,BAD_CAST "Hardware" );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //DbPath
                        ICluster* pCluster;
                        m_ouBusmasterNetwork->GetDBService(LIN, i, 0, &pCluster);

                        std::string strName = "";
                        if ( nullptr != pCluster )
                        {
                            pCluster->GetDBFilePath(strName);
                            pIndex= xmlNewChild(pNodeChannelPtr, nullptr, BAD_CAST DEF_CHANNEL_DBPATH, BAD_CAST  strName.c_str() );
                            xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                            //ClusterId
                            pCluster->GetName(strName);
                            pIndex= xmlNewChild(pNodeChannelPtr, nullptr, BAD_CAST DEF_CHANNEL_CLUSTERID, BAD_CAST  strName.c_str() );
                            xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);
                        }
                        //BaudRate
                        ChannelSettings ouChannelSettings;
                        m_ouBusmasterNetwork->GetChannelSettings(LIN, i, &ouChannelSettings);
                        sprintf_s(chValue, sizeof(chValue), "%d", ouChannelSettings.m_ouLINSettings.m_nBaudRate);
                        pIndex= xmlNewChild(pNodeChannelPtr, nullptr, BAD_CAST DEF_LIN_BAUDRATE, BAD_CAST chValue  );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //ProtocolVersion
                        pIndex= xmlNewChild(pNodeChannelPtr, nullptr, BAD_CAST DEF_LIN_PROTOCOLVER, BAD_CAST ouChannelSettings.m_ouLINSettings.m_strProtocolVersion.c_str());
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);


                        sprintf_s(chValue, sizeof(chValue), "%d", ouChannelSettings.m_ouLINSettings.m_bOverWriteSettings);
                        pIndex= xmlNewChild(pNodeChannelPtr, nullptr, BAD_CAST DEF_LIN_OVERWRITE_SETTINGS, BAD_CAST chValue );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);

                        //Master Mode
                        sprintf(chValue, "%d", ouChannelSettings.m_ouLINSettings.m_bIsMasterMode);
                        pIndex= xmlNewChild(pNodeChannelPtr, NULL, BAD_CAST DEF_LIN_MASTER_MODE, BAD_CAST chValue );
                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);


                        //ECU
                        xmlNodePtr pNodeECUPtr = xmlNewNode(nullptr, BAD_CAST DEF_CHANNEL_ECU);

                        xmlAddChild(pNodeChannelPtr, pNodeECUPtr);

                        xmlAddChild(pNodeDBFilePtr, pNodeChannelPtr);



                    }
                }
                xmlAddChild(pNodePtr, pNodeDBFilePtr);
            }
        }
        break;
        
        case WAVEFORMDATA_SECTION_ID:
        {
            xmlNodePtr pNodeWaveFormGen = xmlNewNode(nullptr, BAD_CAST DEF_WF_GENERATOR);
            xmlAddChild(pNodePtr, pNodeWaveFormGen);
            m_objWaveformDataHandler.GetConfigData(pNodeWaveFormGen);
        }
        break;
        case BUS_STATISTICS_SECTION_ID:
        {
            if(m_oNetworkStatistics != nullptr)
            {
                xmlNodePtr pNodeBusStat = nullptr;
                m_oNetworkStatistics->vGetConfigData(pNodeBusStat);
                xmlAddChild(pNodePtr, pNodeBusStat);
            }
            else
            {
                if(nullptr != m_pXmlNodeBusStats)
                {
                    xmlNodePtr pBUSStatNode = nullptr;
                    pBUSStatNode = xmlCopyNode(m_pXmlNodeBusStats, 1);
                    xmlAddChild(pNodePtr, pBUSStatNode);
                }
            }
        }
        break;
        case TEST_SETUP_EDITOR_SECTION_ID:
        {
            xmlNodePtr pNodeTSEditor = nullptr;
            m_objTSEditorHandler.vGetConfigurationData(pNodeTSEditor);
            xmlAddChild(pNodePtr, pNodeTSEditor);
        }
        break;
        case TEST_SUITE_EXECUTOR_SECTION_ID:
        {
            xmlNodePtr pNodeExecutor = nullptr;
            m_objTSExecutorHandler.vGetConfigurationData(pNodeExecutor);
            xmlAddChild(pNodePtr, pNodeExecutor);
        }
        break;

        case LIN_TXWND:
        {
            xmlNodePtr pFLEXTxPtr = xmlNewNode(nullptr, BAD_CAST DEF_LIN_TX_WINDOW);
            xmlAddChild(pNodePtr, pFLEXTxPtr);
            m_objTxHandler.vGetTxWndConfigData(LIN, pFLEXTxPtr);
        }
        break;
        case LIN_SCHEDULE_CONFIG:
        {
            xmlNodePtr pScheduleConfig = xmlNewNode(NULL, BAD_CAST DEF_LIN_SCHEDULE_TABLE);
            xmlAddChild(pNodePtr, pScheduleConfig);
            m_objTxHandler.vGetScheduleConfig(pScheduleConfig);
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

    while(pNodePtr != nullptr)
    {
        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsMsgFilterEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strMsgFilter = ptext;

                if(strMsgFilter == "FALSE")
                {
                    m_sToolBarInfo.m_byMsgFilter = FALSE;
                }
                else if(strMsgFilter == "TRUE")
                {
                    m_sToolBarInfo.m_byMsgFilter = TRUE;
                }
                xmlFree(ptext);
            }
        }
        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsMsgFilterEnabledLin")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strMsgFilter = ptext;

                if(strMsgFilter == "FALSE")
                {
                    m_sToolBarInfo.m_byMsgFilterLin = FALSE;
                }
                else if(strMsgFilter == "TRUE")
                {
                    m_sToolBarInfo.m_byMsgFilterLin = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsReplayFilterEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strReplayFilter = ptext;
                if(strReplayFilter == "FALSE")
                {
                    m_sToolBarInfo.m_byReplayFilter = FALSE;
                }
                else if(strReplayFilter == "TRUE")
                {
                    m_sToolBarInfo.m_byReplayFilter = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsLogFilterEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strLogFilter = ptext;

                if(strLogFilter == "FALSE")
                {
                    m_sToolBarInfo.m_byLogFilter = FALSE;
                }
                else if(strLogFilter == "TRUE")
                {
                    m_sToolBarInfo.m_byLogFilter = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsLogFilterLINEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strLogFilter = ptext;

                if(strLogFilter == "FALSE")
                {
                    m_sToolBarInfo.m_byLogFilterLIN = FALSE;
                }
                else if(strLogFilter == "TRUE")
                {
                    m_sToolBarInfo.m_byLogFilterLIN = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsLoggingEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strLogging = ptext;

                if(strLogging == "FALSE")
                {
                    m_sToolBarInfo.m_byLogging = FALSE;
                }
                else if(strLogging == "TRUE")
                {
                    m_sToolBarInfo.m_byLogging = TRUE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"IsMsgIntepretationEnabled")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strMsgInterpret = ptext;

                if(strMsgInterpret == "FALSE")
                {
                    m_sToolBarInfo.m_byMsgInterpret = FALSE;
                    m_bInterPretMsg = FALSE;
                }
                else if(strMsgInterpret == "TRUE")
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
            if(nullptr != ptext)
            {
                CString strOverwrite = ptext;

                if(strOverwrite == "FALSE")
                {
                    m_sToolBarInfo.m_byOverwrite = FALSE;
                    theApp.pouGetFlagsPtr()->vSetFlagStatus(OVERWRITE, FALSE);
                }
                else if(strOverwrite == "TRUE")
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
            if(nullptr != ptext)
            {
                CString strDisplayTimeMode = ptext;

                if(strDisplayTimeMode == "SYSTEM")
                {
                    m_sToolBarInfo.m_byDisplayTimeMode = eSYSTEM_MODE;
                }
                else if(strDisplayTimeMode == "ABSOLUTE")
                {
                    m_sToolBarInfo.m_byDisplayTimeMode = eABSOLUTE_MODE;
                }
                else if(strDisplayTimeMode == "RELATIVE")
                {
                    m_sToolBarInfo.m_byDisplayTimeMode = eRELATIVE_MODE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"DisplayNumericMode")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strDisplayNumericON = ptext;

                if(strDisplayNumericON == "TRUE")
                {
                    // Setting decimal mode
                    bSetHexDecFlags(FALSE);
                    m_sToolBarInfo.m_byDisplayHexON = FALSE;
                }
                else if(strDisplayNumericON == "FALSE")
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
            if(nullptr != ptext)
            {
                CString strLogOnConnect = ptext;

                if(strLogOnConnect == "TRUE")
                {
                    m_abLogOnConnect[CAN] = TRUE;
                }
                else if(strLogOnConnect == "FALSE")
                {
                    m_abLogOnConnect[CAN] = FALSE;
                }
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"LogOnConnect_J1939")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strLogOnConnect = ptext;
                if(strLogOnConnect == "TRUE")
                {
                    m_abLogOnConnect[J1939] = TRUE;
                }
                else if(strLogOnConnect == "FALSE")
                {
                    m_abLogOnConnect[J1939] = FALSE;
                }
                xmlFree(ptext);
            }
        }


        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"LogOnConnect_LIN")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strLogOnConnect = ptext;

                if(strLogOnConnect == "TRUE")
                {
                    m_abLogOnConnect[LIN] = TRUE;
                }
                else if(strLogOnConnect == "FALSE")
                {
                    m_abLogOnConnect[LIN] = FALSE;
                }
                xmlFree(ptext);
            }
        }

        pNodePtr = pNodePtr->next;
    }
}

void CMainFrame::vSetWindowPositionForGraph(xmlNodePtr pNodePtr, xmlDocPtr /* pDocPtr */)
{
    pNodePtr = pNodePtr->xmlChildrenNode;

    while(pNodePtr != nullptr)
    {
        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Visibility")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                m_sGraphWndPlacement[CAN].showCmd = xmlUtils::nGetWindowVisibility((char*)ptext);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"WindowPlacement")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                m_sGraphWndPlacement[CAN].flags = xmlUtils::nGetWindowVisibility((char*)ptext);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Top")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strTop = ptext;
                m_sGraphWndPlacement[CAN].rcNormalPosition.top = atoi(strTop);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Bottom")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strBottom = ptext;
                m_sGraphWndPlacement[CAN].rcNormalPosition.bottom = atoi(strBottom);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Left")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strLeft = ptext;
                m_sGraphWndPlacement[CAN].rcNormalPosition.left = atoi(strLeft);
                xmlFree(ptext);
            }
        }

        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Right")))
        {
            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
            if(nullptr != ptext)
            {
                CString strRight = ptext;
                m_sGraphWndPlacement[CAN].rcNormalPosition.right = atoi(strRight);
                xmlFree(ptext);
            }
        }

        pNodePtr = pNodePtr->next;
    }
}
void CMainFrame::vSW_DoInitialization(ETYPE_BUS eBusType)
{
    if (sg_pouSWInterface[eBusType] == nullptr)//Signal watch
    {
        if (SW_GetInterface(eBusType, (void**)&sg_pouSWInterface[eBusType]) == S_OK)
        {
            sg_pouSWInterface[eBusType]->SW_DoInitialization(m_ouBusmasterNetwork);
        }
    }
    else
    {
        sg_pouSWInterface[eBusType]->SW_DoInitialization(m_ouBusmasterNetwork);
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
                if( nullptr != pPathObject )
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
                if( nullptr != pPathObject && pPathObject->nodesetval != nullptr )
                {
                    if ( S_OK != xmlUtils::ParseWindowsPlacement(pPathObject->nodesetval->nodeTab[0], m_sNotificWndPlacement) )
                    {
                        m_sNotificWndPlacement.rcNormalPosition.top  = 459;
                        m_sNotificWndPlacement.rcNormalPosition.left = 0;
                        m_sNotificWndPlacement.rcNormalPosition.right = 1089;
                        m_sNotificWndPlacement.rcNormalPosition.bottom = 612;
                        m_sNotificWndPlacement.showCmd = SW_NORMAL;
                    }
                    if (m_podUIThread != nullptr)
                    {
                        m_sNotificWndPlacement.showCmd = SW_NORMAL;
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
                //m_ouClusterConfig[LIN].InitialiseConfig();
                m_ouBusmasterNetwork->ReSetNetwork(LIN);
                m_ouBusmasterNetwork->SetChannelCount(LIN, 1);
            }
            break;
            case DATABASE_SECTION_ID:
            {
                vClearDbInfo(CAN);

                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Database_Files/FilePath";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(nullptr != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            if (  nullptr != pNodeSet->nodeTab[i]->xmlChildrenNode )
                            {
                                xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);
                                if ( ( nullptr != ptext ) && ( theApp.m_pouMsgSignal != nullptr ) )
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
                if (m_pouMsgSigJ1939 == nullptr)
                {
                    m_pouMsgSigJ1939 = new CMsgSignal(sg_asDbParams[J1939], FALSE);
                }

                vClearDbInfo(J1939);

                CStringArray omDBNames;
                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/J1939_Database_Files/FilePath";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(nullptr != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            if (nullptr != pNodeSet->nodeTab[i]->xmlChildrenNode )
                            {
                                xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);
                                if ( nullptr != ptext )
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
                if ( ( nRetValue == S_OK ) && ( m_pouMsgSigJ1939  != nullptr ) )
                {
                    // m_pouMsgSigJ1939->vSetDataBaseNames(&omDBNames);
                    for (INT i = 0; i < omDBNames.GetSize(); i++)
                    {
                        //No need to check return value. Error message will be displayed
                        // in trace window
                        dLoadJ1939DBFile(omDBNames.GetAt(i), TRUE);
                    }
                    SMSGENTRY* psMsgEntry = nullptr;
                    vPopulateMsgEntryFromDB(psMsgEntry, m_pouMsgSigJ1939);
                    if (m_pouTxMsgWndJ1939 != nullptr)
                    {
                        m_pouTxMsgWndJ1939->vSetDatabaseInfo(m_ouBusmasterNetwork);
                    }
                    else
                    {
                        //venkat
                        //m_pouTxMsgWndJ1939->vUpdateDataStore( m_ouBusmasterNetwork );
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





                /*for ( int i = 0; i < m_ouClusterConfig[LIN].m_nChannelsConfigured; i++ )
                {
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_eBusChannelType = LIN;
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_nBaudRate = 19200;
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_strProtocolVersion = "LIN 2.1";
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_strDataBasePath = "";
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouLinParams.m_bOverWriteSettings = true;
                    //m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_ouClusterInfo.clear();
                    m_ouClusterConfig[LIN].m_ouFlexChannelConfig[i].m_strSlectedEculist.clear();
                }*/

                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(nullptr != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlNodePtr xmlChannel = pNodeSet->nodeTab[i]->xmlChildrenNode;
                            if (nullptr != pNodeSet->nodeTab[i] )
                            {
                                xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                                sLinConfigContainer ouFibexContainer;
                                INT nCount = 0;
                                while ( pNodePtr != nullptr )
                                {
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Index")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(nullptr != key)
                                        {
                                            ouFibexContainer.m_nChannel = atoi((char*)key);
                                            nCount++;
                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"HWUri")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(nullptr != key)
                                        {
                                            ouFibexContainer.m_strHWUri = (char*)key;
                                            nCount++;
                                        }
                                    }
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"DbPath")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(nullptr != key)
                                        {
                                            ouFibexContainer.m_strDbPath = (char*)key;
                                            nCount++;

                                        }
                                    }
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"ClusterId")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(nullptr != key)
                                        {
                                            ouFibexContainer.m_strClusterId = (char*)key;
                                            nCount++;

                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"BaudRate")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(nullptr != key)
                                        {
                                            ouFibexContainer.m_nBaudRate = atoi((char*)key);
                                            nCount++;

                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"ProtocolVersion")))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(nullptr != key)
                                        {
                                            ouFibexContainer.m_srtProtocolVerson = (char*)key;
                                            nCount++;

                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)DEF_LIN_OVERWRITE_SETTINGS)))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(nullptr != key)
                                        {
                                            ouFibexContainer.m_bOverwrite = (atoi((char*)key) != 0);
                                            nCount++;
                                        }
                                    }

                                    //Master Mode
                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)DEF_LIN_MASTER_MODE)))
                                    {
                                        xmlChar* key = xmlNodeListGetString(pNodePtr->doc, pNodePtr->xmlChildrenNode, 1);
                                        if(NULL != key)
                                        {
                                            ouFibexContainer.m_bIsMasterMode = atoi((char*)key);
                                            nCount++;
                                        }
                                    }

                                    if ((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"ECU")))
                                    {
                                        //Nodes
                                        xmlChar* pchPath = (xmlChar*)"Name";
                                        pPathObject = xmlUtils::pGetChildNodes(pNodePtr, pchPath);
                                        if( nullptr != pPathObject )
                                        {
                                            xmlNodeSetPtr pNodeSetEcu = pPathObject->nodesetval;
                                            if(nullptr != pNodeSetEcu)
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
                m_objTxHandler.SetNetworkConfig(LIN, m_ouBusmasterNetwork);

                m_objTxHandler.vSetTxWndConfigData( LIN, m_xmlConfigFiledoc);

                m_objTxHandler.vSetScheduleConfig(m_xmlConfigFiledoc);
                m_objTxHandler.vShowLINScheduleConfigDlg(false);
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
                if(m_xmlConfigFiledoc != nullptr)
                {
                    pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPathDriver);
                    if( nullptr != pPathObject )
                    {
                        nRetValue = S_FALSE;
                        pNodeSet = pPathObject->nodesetval;
                        if ( pNodeSet != nullptr &&  pNodeSet->nodeTab[0] != nullptr && pNodeSet->nodeTab[0]->xmlChildrenNode != nullptr )
                            //Tight Checking
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[0]->xmlChildrenNode, 1);
                            if ( nullptr != ptext)
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
                    if( nullptr != pPathObject )
                    {
                        pNodeSet = pPathObject->nodesetval;
                        if ( pNodeSet != nullptr &&  pNodeSet->nodeTab[0] != nullptr && pNodeSet->nodeTab[0]->xmlChildrenNode != nullptr )
                            //Tight Checking
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[0]->xmlChildrenNode, 1);
                            if ( nullptr != ptext)
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
                    for(int i = 0; i < defNO_OF_CHANNELS; i++)
                    {
                        vInitialize( m_asControllerDetails[i], TRUE );
                    }
                    pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPathChannel);
                    if( nullptr != pPathObject )
                    {
                        pNodeSet = pPathObject->nodesetval;
                        if ( pNodeSet != nullptr )
                            //Tight Checking
                        {
                            unChannelCount = pNodeSet->nodeNr;
                            for(unsigned int i = 0; i < unChannelCount; i++)
                            {
                                LoadControllerConfigData(m_asControllerDetails[i], pNodeSet->nodeTab[i]);
                            }
                        }
                        xmlXPathFreeObject (pPathObject);
                    }


				IntializeDIL(unChannelCount, FromXMLFile);
                    ASSERT(g_pouDIL_CAN_Interface != nullptr);
                    g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails,
                            defNO_OF_CHANNELS);
                    // Set hardware selection as per configuration
                    g_pouDIL_CAN_Interface->DILC_SetHardwareChannel(m_asControllerDetails,m_dwDriverId, true, unChannelCount);
                }

                if ((nullptr == m_xmlConfigFiledoc) || ( bProper ==  FALSE))
                {
                    m_dwDriverId = DRIVER_CAN_STUB;
                    m_byControllerMode = defMODE_SIMULATE;
                    IntializeDIL();
                    for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
                    {
                        vInitialize( m_asControllerDetails[i], TRUE );
                    }

                    g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails,
                            sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS);
                }
            }
            break;
            case FILTER_SECTION_ID:
            {
                nRetValue = m_sFilterAppliedCAN.nSetXMLConfigData(m_xmlConfigFiledoc, CAN);
            }
            break;
            case FILTER_SECTION_LIN_ID:
            {
                nRetValue = m_sFilterAppliedLIN.nSetXMLConfigData(m_xmlConfigFiledoc, LIN);
            }
            break;

            case SIMSYS_SECTION_ID:
            {
                if (GetICANNodeSim() != nullptr)
                {
                    GetICANNodeSim()->NS_SetSimSysConfigData(m_xmlConfigFiledoc);
                }
            }
            break;

            case SIMSYS_SECTION_J1939_ID:
            {
                if (GetIJ1939NodeSim() != nullptr)
                {
                    GetIJ1939NodeSim()->NS_SetSimSysConfigData(m_xmlConfigFiledoc);
                }
            }
            break;

            case SIMSYS_SECTION_LIN_ID:
            {
                if (GetILINNodeSim() != nullptr)
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
                sMsgAttrib.m_psMsgAttribDetails = nullptr;
                sMsgAttrib.m_usMsgCount = 0;

                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Message_Attribute";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(nullptr != pNodeSet)
                    {
                        // Get the Message Count from xml
                        sMsgAttrib.m_usMsgCount = pNodeSet->nodeNr;
                        PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                        for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != nullptr)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Name")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        pMessageAtt[i].omStrMsgname = ((CString)ptext);
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Message_ID")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        pMessageAtt[i].unMsgID = atoi(((CString)ptext));
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Color")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        CString strColor = ptext;
                                        DWORD dwColor = strtoul(strColor, nullptr, 16);

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
                        vPopulateIDList( CAN );
                    }
                }

                // Copying Message Buffer Details
                /* Append buffer size */
                xmlChar* pchAppBuffSizePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Append_Buffer_Size";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchAppBuffSizePath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(nullptr != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( nullptr != ptext ))
                            {
                                CString strAppndBufferSize = ptext;
                                m_anMsgBuffSize[CAN][defAPPEND_DATA_INDEX] = atoi(strAppndBufferSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[CAN][defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                /* Overwrite buffer size */
                xmlChar* pchOvrBuffSizePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Overwrite_Buffer_Size";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchOvrBuffSizePath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(nullptr != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( nullptr != ptext ))
                            {
                                CString strOvrwriteBufferSize = ptext;
                                m_anMsgBuffSize[CAN][defOVERWRITE_DATE_INDEX] = atoi(strOvrwriteBufferSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[CAN][defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                /* Display update time */
                xmlChar* pchDispUpTimePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Display_Update_Time";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchDispUpTimePath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(nullptr != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( nullptr != ptext ))
                            {
                                CString strDispUpdTimeSize = ptext;
                                m_anMsgBuffSize[CAN][defDISPLAY_UPDATE_DATA_INDEX] = atoi(strDispUpdTimeSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[CAN][defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;
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
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(nullptr != pNodeSet)
                    {
                        //for(INT nIndex =0; nIndex < pNodeSet->nodeNr; nIndex++)
                        {
                            sMsgWndFilter.pbSetConfigData(m_sFilterAppliedCAN, pNodeSet, m_xmlConfigFiledoc, bResult);
                            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                          WM_SET_FILTER_DETAILS, (WPARAM)&m_sFilterAppliedCAN, 0);
                        }
                    }
                }

                if(nullptr != m_xmlConfigFiledoc)
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA_XML,
                                  (LPARAM)m_xmlConfigFiledoc);


                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eLOAD_DATABASE,
                                  (LPARAM)m_ouBusmasterNetwork);
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
                sMsgAttrib.m_psMsgAttribDetails = nullptr;
                sMsgAttrib.m_usMsgCount = 0;

                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Message_Window/Message_Attribute";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(nullptr != pNodeSet)
                    {
                        // Get the Message Count from xml
                        sMsgAttrib.m_usMsgCount = pNodeSet->nodeNr;
                        PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                        for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != nullptr)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Name")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        pMessageAtt[i].omStrMsgname = ((CString)ptext);
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Message_ID")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        pMessageAtt[i].unMsgID = atoi(((CString)ptext));
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Color")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        CString strColor = ptext;
                                        DWORD dwColor = strtoul(strColor, nullptr, 16);

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
                        //theApp.vPopulateIDList();
                    }
                }

                // Copying Message Buffer Details
                /* Append buffer size */
                xmlChar* pchAppBuffSizePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Message_Window/Append_Buffer_Size";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchAppBuffSizePath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(nullptr != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( nullptr != ptext ))
                            {
                                CString strAppndBufferSize = ptext;
                                m_anMsgBuffSize[LIN][defAPPEND_DATA_INDEX] = atoi(strAppndBufferSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[LIN][defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                /* Overwrite buffer size */
                xmlChar* pchOvrBuffSizePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Message_Window/Overwrite_Buffer_Size";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchOvrBuffSizePath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(nullptr != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( nullptr != ptext ))
                            {
                                CString strOvrwriteBufferSize = ptext;
                                m_anMsgBuffSize[LIN][defOVERWRITE_DATE_INDEX] = atoi(strOvrwriteBufferSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[LIN][defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                /* Display update time */
                xmlChar* pchDispUpTimePath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Message_Window/Display_Update_Time";

                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchDispUpTimePath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;

                    if(nullptr != pNodeSet)
                    {
                        for(int i=0; i < pNodeSet->nodeNr; i++)
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodeSet->nodeTab[i]->xmlChildrenNode, 1);

                            if ( ( nullptr != ptext ))
                            {
                                CString strDispUpdTimeSize = ptext;
                                m_anMsgBuffSize[LIN][defDISPLAY_UPDATE_DATA_INDEX] = atoi(strDispUpdTimeSize);
                                xmlFree(ptext);
                            }
                        }
                    }
                    else
                    {
                        m_anMsgBuffSize[LIN][defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;
                    }
                    xmlXPathFreeObject (pPathObject);
                }

                //Msg Filter
                bool bResult = false;
                SFILTERAPPLIED_LIN sMsgWndFilter;
                //pbyTemp = sMsgWndFilter.pbSetConfigData(pbyTemp, bResult);

                // Get the Filter data from xml
                pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Message_Window/Filter";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(nullptr != pNodeSet)
                    {
                        //for(INT nIndex =0; nIndex < pNodeSet->nodeNr; nIndex++)
                        {
                            sMsgWndFilter .pbSetConfigData( m_sFilterAppliedLIN, pNodeSet, m_xmlConfigFiledoc, bResult, LIN);
                            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                                          WM_SET_FILTER_DETAILS, (WPARAM)&m_sFilterAppliedLIN, 0);
                        }
                    }
                }


                if(nullptr != m_xmlConfigFiledoc)
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA_XML,
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
                if (m_xmlConfigFiledoc != nullptr)
                {
                    if (sg_pouFrameProcCAN != nullptr)
                    {
                        sg_pouFrameProcCAN->StartEditingSession();
                        nRetVal = sg_pouFrameProcCAN->SetConfigData(m_xmlConfigFiledoc);
                        sg_pouFrameProcCAN->StopEditingSession(TRUE);
                    }
                    //Start logging if toolbar status is enabled.
                    BOOL bLogON = FALSE;
                    CFlags* pFlag = theApp.pouGetFlagsPtr();
                    if(pFlag != nullptr)
                    {
                        bLogON = pFlag->nGetFlagStatus(LOGTOFILE);
                        BOOL bFilterON = FALSE;
                        bFilterON = pFlag->nGetFlagStatus(LOGFILTER);
                        vStartStopLogging(bLogON == TRUE);
                    }
                }
                else if ( (m_xmlConfigFiledoc != nullptr) || (nRetVal == S_FALSE) )
                {
                    sg_pouFrameProcCAN = GetICANLogger();
                    sg_pouFrameProcCAN->StartEditingSession();
                    sg_pouFrameProcCAN->ClearLoggingBlockList();
                    sg_pouFrameProcCAN->StopEditingSession(TRUE);
                }
            }
            break;
            case LOG_SECTION_J1939_ID:
            {
                if (m_xmlConfigFiledoc != nullptr)
                {
                    if (GetIJ1939Logger() != nullptr)
                    {
                        GetIJ1939Logger()->StartEditingSession();
                        GetIJ1939Logger()->SetConfigData(m_xmlConfigFiledoc);
                        GetIJ1939Logger()->StopEditingSession(true);
                    }
                }
                else
                {
                    if (GetIJ1939Logger() != nullptr)
                    {
                        GetIJ1939Logger()->StartEditingSession();
                        GetIJ1939Logger()->ClearLoggingBlockList();
                        GetIJ1939Logger()->StopEditingSession(true);
                    }
                }
            }
            break;
            case LOG_SECTION_LIN_ID:
            {
                INT nRetVal = S_OK;
                if (m_xmlConfigFiledoc != nullptr)
                {
                    if (sg_pouFrameProcLIN != nullptr)
                    {
                        sg_pouFrameProcLIN->StartEditingSession();
                        nRetVal = sg_pouFrameProcLIN->SetConfigData(m_xmlConfigFiledoc);
                        sg_pouFrameProcLIN->StopEditingSession(TRUE);
                    }
                    //Start logging if toolbar status is enabled.
                    BOOL bLogON = FALSE;
                    CFlags* pFlag = theApp.pouGetFlagsPtr();
                    if(pFlag != nullptr)
                    {
                        bLogON = pFlag->nGetFlagStatus(LOGTOFILE_LIN);
                        BOOL bFilterON = FALSE;
                        // bFilterON = pFlag->nGetFlagStatus(LOGFILTER);
                        vStartStopLogging_LIN(bLogON == TRUE);
                    }
                }
                else if ( (m_xmlConfigFiledoc != nullptr) || (nRetVal == S_FALSE) )
                {
                    sg_pouFrameProcLIN = GetILINLogger();
                    sg_pouFrameProcLIN->StartEditingSession();
                    sg_pouFrameProcLIN->ClearLoggingBlockList();
                    sg_pouFrameProcLIN->StopEditingSession(TRUE);
                }
            }
            break;

            case SIGWATCH_SECTION_ID:
            {
                if ( sg_pouSWInterface[CAN] == nullptr )
                {
                    if ( SW_GetInterface( CAN, (void**)&sg_pouSWInterface[CAN] ) == S_OK )
                    {
                        sg_pouSWInterface[CAN]->SW_DoInitialization( m_ouBusmasterNetwork );
                    }
                }

                BOOL bProper = FALSE;
                xmlXPathObjectPtr pOjectPath = nullptr;
                xmlNodePtr pNodePtr = nullptr;

                xmlChar* pXmlPath = ( xmlChar* )"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Watch";
                pOjectPath = xmlUtils::pGetNodes( m_xmlConfigFiledoc, pXmlPath );

                if ( pOjectPath != nullptr )
                {
                    bProper = TRUE;
                }

                if ( bProper == TRUE )
                {
                    sg_pouSWInterface[CAN]->SW_ClearSigWatchWnd();
                    if ( pOjectPath != nullptr )
                    {
                        xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                        if ( pNodeSet != nullptr )
                        {
                            pNodePtr = pNodeSet->nodeTab[0];
                            if ( pNodePtr != nullptr )
                            {
                                sg_pouSWInterface[CAN]->SW_SetConfigData( (xmlNodePtr)( pNodePtr->children ) );
                            }
                        }
                        xmlXPathFreeObject( pOjectPath );
                        //bProper = FALSE;
                    }

                }
                if ( bProper == FALSE )
                {
                    //Set default settings
                    sg_pouSWInterface[CAN]->SW_SetConfigData( nullptr );
                    sg_pouSWInterface[CAN]->SW_ClearSigWatchWnd();
                }
                bProper = FALSE;
            }
            break;

            case SIGWATCH_SECTION_LIN_ID:
            {
                if (sg_pouSWInterface[LIN] == nullptr)
                {
                    if (SW_GetInterface(LIN, (void**)&sg_pouSWInterface[LIN]) == S_OK)
                    {
                        sg_pouSWInterface[LIN]->SW_DoInitialization( m_ouBusmasterNetwork);
                    }
                }

                BOOL bProper = FALSE;
                xmlXPathObjectPtr pOjectPath = nullptr;
                xmlNodePtr pNodePtr = nullptr;

                xmlChar* pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Signal_Watch";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);

                if(pOjectPath != nullptr)
                {
                    bProper = TRUE;
                }

                if(bProper == TRUE)
                {
                    sg_pouSWInterface[LIN]->SW_ClearSigWatchWnd();
                    if(pOjectPath != nullptr)
                    {
                        xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                        if(pNodeSet != nullptr)
                        {
                            pNodePtr = pNodeSet->nodeTab[0];
                            if(pNodePtr != nullptr)
                            {
                                sg_pouSWInterface[LIN]->SW_SetConfigData((xmlNodePtr)(pNodePtr->children));
                            }
                        }
                        xmlXPathFreeObject(pOjectPath);
                        //bProper = FALSE;
                    }

                }
                if (bProper == FALSE)
                {
                    //Set default settings
                    sg_pouSWInterface[LIN]->SW_SetConfigData(nullptr);
                    sg_pouSWInterface[LIN]->SW_ClearSigWatchWnd();
                }
                bProper = FALSE;
            }
            break;

            case SIGWATCH_SECTION_J1939_ID:
            {
                if ( sg_pouSWInterface[J1939] == nullptr )
                {
                    if ( SW_GetInterface( J1939, (void**)&sg_pouSWInterface[J1939] ) == S_OK )
                    {
                        sg_pouSWInterface[J1939]->SW_DoInitialization( m_ouBusmasterNetwork );
                    }
                }

                BOOL bProper = FALSE;
                xmlXPathObjectPtr pOjectPath = nullptr;
                xmlNodePtr pNodePtr = nullptr;

                xmlChar* pXmlPath = ( xmlChar* )"//BUSMASTER_CONFIGURATION/Module_Configuration/J1939_Signal_Watch";
                pOjectPath = xmlUtils::pGetNodes( m_xmlConfigFiledoc, pXmlPath );

                if ( pOjectPath != nullptr )
                {
                    bProper = TRUE;
                }

                if ( bProper == TRUE )
                {
                    sg_pouSWInterface[J1939]->SW_ClearSigWatchWnd();
                    if ( pOjectPath != nullptr )
                    {
                        xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                        if ( pNodeSet != nullptr )
                        {
                            pNodePtr = pNodeSet->nodeTab[0];
                            if ( pNodePtr != nullptr )
                            {
                                sg_pouSWInterface[J1939]->SW_SetConfigData( (xmlNodePtr)( pNodePtr->children ) );
                            }
                        }
                        xmlXPathFreeObject( pOjectPath );
                        //bProper = FALSE;
                    }

                }
                if ( bProper == FALSE )
                {
                    //Set default settings
                    sg_pouSWInterface[J1939]->SW_SetConfigData( nullptr );
                    sg_pouSWInterface[J1939]->SW_ClearSigWatchWnd();
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
                m_pXmlNodeBusStats = nullptr;
                xmlXPathObjectPtr pTempNode = nullptr;
                pTempNode = xmlUtils::pGetNodes(m_xmlConfigFiledoc, (xmlChar*)("//BUSMASTER_CONFIGURATION/Module_Configuration/Bus_Statistics"));

                if(pTempNode != nullptr && pTempNode->nodesetval != nullptr && pTempNode->nodesetval->nodeTab[0] != nullptr)
                {
                    m_pXmlNodeBusStats = xmlCopyNode(pTempNode->nodesetval->nodeTab[0], 1);
                }
                if(m_bIsStatWndCreated == TRUE)
                {
                    if(m_pXmlNodeBusStats != nullptr)
                    {
                        m_oNetworkStatistics->hSetConfigData((xmlNodePtr)m_pXmlNodeBusStats);
                    }
                }
            }
            break;
            case TXWND_SECTION_ID:
            {
                m_objTxHandler.vSetTxWndConfigData(CAN,m_xmlConfigFiledoc);
            }
            break;
            case LIN_SCHEDULE_CONFIG:
            {
                m_objTxHandler.vSetScheduleConfig(m_xmlConfigFiledoc);
            }
            break;
            case MSGWND_SECTION_J1939_ID:
            {
                xmlNodePtr pNode = nullptr;
                xmlNodePtr pMsgAttibNode = nullptr;
                xmlNodePtr pChildNode = nullptr;
                //Msg Attributes
                SMESSAGE_ATTRIB sMsgAttrib;
                sMsgAttrib.m_psMsgAttribDetails = nullptr;
                sMsgAttrib.m_usMsgCount = 0;

                xmlChar* pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/J1939_Message_Window/Message_Attribute";
                pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
                if( nullptr != pPathObject )
                {
                    pNodeSet = pPathObject->nodesetval;
                    if(nullptr != pNodeSet)
                    {
                        // Get the Message Count from xml
                        sMsgAttrib.m_usMsgCount = pNodeSet->nodeNr;
                        //COPY_DATA_2(&(sMsgAttrib.m_usMsgCount), pbyTemp, sizeof(UINT));
                        PSMESSAGEATTR pMessageAtt = new SMESSAGEATTR[sMsgAttrib.m_usMsgCount];
                        for (UINT i = 0; i < sMsgAttrib.m_usMsgCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != nullptr)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Name")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        pMessageAtt[i].omStrMsgname = ((CString)ptext);
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Message_ID")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        pMessageAtt[i].unMsgID = atoi(((CString)ptext));
                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Color")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        CString strColor = ptext;
                                        DWORD dwColor = strtoul(strColor, nullptr, 16);

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

                if(m_xmlConfigFiledoc != nullptr)
                {
                    ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                                  WM_NOTIFICATION_FROM_OTHER,
                                  eWINID_MSG_WND_SET_CONFIG_DATA_XML,
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
                xmlXPathObjectPtr pOjectPath = nullptr;
                xmlChar* pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/GRAPH_PARAMETERS";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != nullptr)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != nullptr)
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


                pOjectPath = nullptr;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Window_Position";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != nullptr)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != nullptr)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            vSetWindowPositionForGraph(pNodeSet->nodeTab[i], m_xmlConfigFiledoc);
                        }
                    }
                }

                pOjectPath = nullptr;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Splitter_Window_Col_0";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != nullptr)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != nullptr)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != nullptr)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxIdeal")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        CString strCxIdeal = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRootSplitterData[0][0] = atoi(strCxIdeal);

                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxMin")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
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

                pOjectPath = nullptr;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Splitter_Window_Col_1";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != nullptr)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != nullptr)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != nullptr)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxIdeal")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        CString strCxIdeal = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRootSplitterData[1][0] = atoi(strCxIdeal);

                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxMin")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
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

                pOjectPath = nullptr;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Splitter_Window_Row_0";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != nullptr)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != nullptr)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != nullptr)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxIdeal")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        CString strCxIdeal = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRightViewSplitterData[0][0] = atoi(strCxIdeal);

                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxMin")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
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

                pOjectPath = nullptr;
                pXmlPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Signal_Graph/Splitter_Window_Row_1";
                pOjectPath = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pXmlPath);
                if(pOjectPath != nullptr)
                {
                    xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
                    if(pNodeSet != nullptr)
                    {
                        INT nCount = pNodeSet->nodeNr;
                        for(int i = 0; i < nCount; i++)
                        {
                            xmlNodePtr pNodePtr = pNodeSet->nodeTab[i]->xmlChildrenNode;

                            while(pNodePtr != nullptr)
                            {
                                if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxIdeal")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
                                    {
                                        CString strCxIdeal = ptext;
                                        m_sGraphSplitterPos[CAN].m_nRightViewSplitterData[1][0] = atoi(strCxIdeal);

                                        xmlFree(ptext);
                                    }
                                }
                                else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"CxMin")))
                                {
                                    xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                                    if(nullptr != ptext)
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
                    if(nBUSID == 5)
                    {
                        //TODO: When Signal graph for ETHERNET is implemented remove this check
                        //Skip Ethernet protocol
                        continue;
                    }
                    m_objSigGrphHandler.SetSignalListDetails((SHORT)nBUSID, &m_odGraphList[nBUSID]);
                    m_objSigGrphHandler.SetWindowSplitterPos((SHORT)nBUSID, m_sGraphWndPlacement[nBUSID],
                            m_sGraphSplitterPos[nBUSID]);
                }
                //Arun
                vPostConfigChangeCmdToSigGrphWnds();
                break;
            }

        } //switch(eSecId)
        eSecId = static_cast<eSECTION_ID>(eSecId + 1);
    }//for
    nRetValue = S_OK;
    return nRetValue;
}
void CMainFrame::vVlaidateAndLoadFibexConfig(sLinConfigContainer& ouFibexContainer)
{
    if (  ouFibexContainer.m_nChannel >= 0 && ouFibexContainer.m_nChannel < CHANNEL_ALLOWED )
    {
        bool bValid = false;
        std::list<LinChannelParam> ouLinParams;
        std::list<LinChannelParam>::iterator itrChannelParam;
        std::list<ICluster*> ouClusterList;
        std::string strName;
        m_ouBusmasterNetwork->ParseDbFile( ouFibexContainer.m_strDbPath, LIN, ouClusterList );

        if ( ouClusterList.size() > 0 )
        {
            itrChannelParam = ouLinParams.begin();
            int nIndex = 0;
            ChannelSettings ouChannelSettings;
            for ( auto itrCluster = ouClusterList.begin(); itrCluster != ouClusterList.end(); itrCluster++ )
            {
                (*itrCluster)->GetName(strName);
				if (strName == ouFibexContainer.m_strClusterId || ouFibexContainer.m_strClusterId == "")
                {
                    m_ouBusmasterNetwork->SetDBService(LIN, ouFibexContainer.m_nChannel, 0, *itrCluster);

                    ouChannelSettings.m_ouLINSettings.m_nBaudRate = ouFibexContainer.m_nBaudRate;
                    ouChannelSettings.m_ouLINSettings.m_strProtocolVersion = ouFibexContainer.m_srtProtocolVerson;
                    ouChannelSettings.m_ouLINSettings.m_bOverWriteSettings = ouFibexContainer.m_bOverwrite;
                    ouChannelSettings.m_ouLINSettings.m_bIsMasterMode = ouFibexContainer.m_bIsMasterMode;


                    std::map<UID_ELEMENT, IElement*> mapScheduleTables;
                    std::list<IScheduleTable*> lstScheduleTables;
                    (*itrCluster)->GetElementList(eScheduleTableElement, mapScheduleTables);

for(auto itrScheduleTable : mapScheduleTables)
                    {
                        lstScheduleTables.push_back((IScheduleTable*)itrScheduleTable.second);
                    }

                    //pCluster->GetProperties(eLINScheduleTableProps, (void *)&lstScheduleTables);
                    vUpdateScheduleTableList(ouChannelSettings, ouFibexContainer.m_nChannel, m_ouBusmasterNetwork);
                    //ouChannelSettings.m_ouLINSettings.m_ouScheduleTableList = lstScheduleTables;


                    bValid = true;
                    m_ouBusmasterNetwork->SetChannelSettings(LIN, ouFibexContainer.m_nChannel, &ouChannelSettings);

                    std::list<IEcu*> ouSelectedEcuList;
                    (*itrCluster)->GetEcuList(ouSelectedEcuList);
                    m_ouBusmasterNetwork->SetSimulatedEcuList(LIN, ouFibexContainer.m_nChannel,ouSelectedEcuList);

                    break;
                }
                nIndex++;
            }
            if ( true == bValid )
            {
                vPopulateIDList( LIN );
				OnClusterChanged(LIN);
            }
            else
            {
                //TODO::
            }
        }
        else
        {
            ChannelSettings ouChannelSettings;
            ouChannelSettings.m_ouLINSettings.m_nBaudRate = ouFibexContainer.m_nBaudRate;
            ouChannelSettings.m_ouLINSettings.m_strProtocolVersion = ouFibexContainer.m_srtProtocolVerson;
            ouChannelSettings.m_ouLINSettings.m_bOverWriteSettings = ouFibexContainer.m_bOverwrite;
            ouChannelSettings.m_ouLINSettings.m_bIsMasterMode = ouFibexContainer.m_bIsMasterMode;
            m_ouBusmasterNetwork->SetChannelSettings(LIN, ouFibexContainer.m_nChannel, &ouChannelSettings);
        }
    }
}

void CMainFrame::LoadControllerConfigData(SCONTROLLER_DETAILS& sController, xmlNodePtr& pNodePtr)
{
    vInitialize( sController, TRUE );
    if ( pNodePtr == nullptr )
    {
        return;
    }

    pNodePtr = pNodePtr->xmlChildrenNode;
    std::string strVar;
    UINT unTemp = 0;
    double fBaudRate = 0.0;
    //sController.LoadControllerConfigData(pNodePtr);
    while(nullptr != pNodePtr)
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

        if (xmlUtils::GetDataFrmNode(pNodePtr,"BTLCycles",strVar))
        {
            sController.m_unBTL_Cycles = atoi(strVar.c_str());
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

        if (xmlUtils::GetDataFrmNode(pNodePtr,"HardwareDesc",strVar))
        {
            sController.m_omHardwareDesc = strVar.c_str();
        }
        //CANFD
        if (xmlUtils::GetDataFrmNode(pNodePtr,"CANFD_BaudRate",strVar))
        {
            sController.m_unDataBitRate = atoi(strVar.c_str());
        }
        if (xmlUtils::GetDataFrmNode(pNodePtr,"CANFD_SamplePoint",strVar))
        {
            sController.m_unDataSamplePoint = atoi(strVar.c_str());
        }
        if (xmlUtils::GetDataFrmNode(pNodePtr,"CANFD_BTLCycles",strVar))
        {
            sController.m_unDataBTL_Cycles = atoi(strVar.c_str());
        }
        if (xmlUtils::GetDataFrmNode(pNodePtr,"CANFD_SJW",strVar))
        {
            sController.m_unDataSJW = atoi(strVar.c_str());
        }
        if (xmlUtils::GetDataFrmNode(pNodePtr,"CANFD_TxDelayCompensation",strVar))
        {
            sController.m_bTxDelayCompensationControl = (atoi(strVar.c_str()) != 0);
        }
        if (xmlUtils::GetDataFrmNode(pNodePtr,"CANFD_TxSecondarySamplePoint",strVar))
        {
            sController.m_unTxSecondarySamplePointOffset = atoi(strVar.c_str());
        }
      



        pNodePtr = pNodePtr->next;
    }
}

INT CMainFrame::nGetControllerID(std::string strDriverName)
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
    else if(strDriverName == "ETAS ES581" || strDriverName == "ETAS ES581.3")
    {
        nDriverID = DRIVER_CAN_ETAS_ES581;
    }
    else if (strDriverName == "ETAS ES581.4")
    {
        nDriverID = DRIVER_CAN_ETAS_ES5814;
    }
	else if (strDriverName == "ETAS ES582.1")
	{
		nDriverID = DRIVER_CAN_ETAS_ES5821;
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
	else if (strDriverName == "NSI CAN-API")
	{
		nDriverID = DRIVER_CAN_NSI;
	}
    else
    {
        nDriverID = DRIVER_CAN_STUB;
    }
    return nDriverID;
}

CString CMainFrame::vGetControllerName(UINT /* nDriverId */)
{
    for (int i = 0 ; i < m_nDILCount ; i++)
    {
        if ( m_dwDriverId == m_ouList[i].m_dwDriverID )
        {
            if ( m_asControllerDetails->m_omStrBaudrate== "" )
            {
                m_asControllerDetails->m_omStrBaudrate = "500";
            }
            // Added Shortcut key for the hardwared device, Removing '&'
            CString strDriverName = m_ouList[i].m_acName.c_str();
            strDriverName.Replace("&", "");

            return strDriverName;
        }
    }

    return "";
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

    vSetCurrProjInfo((FLOAT)BUSMASTER_APPN_VERSION_LATEST);

    vSaveXMLConfiguration();
    return nReturn;
}




void CMainFrame::vClearDbInfo(ETYPE_BUS eBus)
{
    switch (eBus)
    {
        case CAN:
        {
            m_ouBusmasterNetwork->ReSetNetwork( CAN );
            m_ouBusmasterNetwork->SetChannelCount( CAN, 1 );
            /*CFlags* pFlags = theApp.pouGetFlagsPtr();
            BOOL bDatabaseOpen = FALSE;
            if( pFlags != nullptr)
            {
                bDatabaseOpen = pFlags->nGetFlagStatus(DBOPEN) ;
            }
            if(bDatabaseOpen == TRUE)
            {
                OnCloseDatabase();
            }
            theApp.m_pouMsgSignal->bDeAllocateMemory("");
            CStringArray omDatabase;
            omDatabase.RemoveAll();
            theApp.m_pouMsgSignal->vSetDataBaseNames(&omDatabase);*/
        }
        break;
        case J1939:
        {
            m_ouBusmasterNetwork->ReSetNetwork( J1939 );
            m_ouBusmasterNetwork->SetChannelCount( J1939, 1 );
            /*if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
            {
                OnJ1939DBClose();
            }
            if (m_pouMsgSigJ1939 != nullptr)
            {
                m_pouMsgSigJ1939->bDeAllocateMemory("");
                CStringArray omDatabase;
                omDatabase.RemoveAll();
                m_pouMsgSigJ1939->vSetDataBaseNames(&omDatabase);
            }*/
        }
        break;
        default:
            break;
    }
}

bool CMainFrame::CompareFile(CString FirstFile, CString SecFile)
{
    int nSizeF, nSizeS;
    CStdioFile InFile, InFile2;
    CString strTemp;
    BOOL bAllFine = TRUE;
    CStringArray ArrFFile, ArrSFile, ArrDisC;
    //reads First XML file data\

    InFile.Open(FirstFile,CFile::modeRead);
    InFile2.Open(SecFile,CFile::modeRead);
    if( 0 == InFile.m_hFile || 0 == InFile2.m_hFile )
    {
        return false;
    }
    else
    {
        if(InFile.m_pStream == nullptr)
        {
            return true;
        }

        while(InFile.ReadString(strTemp))
        {
            strTemp.TrimLeft();
            strTemp.TrimRight();
            ArrFFile.Add(strTemp);
        }
        InFile.Close();


        if(InFile2.m_pStream == nullptr)
        {
            return true;
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
            return true;
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
                strTemp.Format("%d", ni);
                //Adds unmatching tags in an array
                ArrDisC.Add(strTemp);
                return true;
                break;
            }
        }

        return false;
    }
}

bool CMainFrame::bIsConfigurationModified(void)
{
    bool bResult = true;
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

        /*bResult |= (GetICANNodeSim()->NS_IsSimSysConfigChanged() == TRUE);
        bResult |= (GetIJ1939NodeSim()->NS_IsSimSysConfigChanged() == TRUE);*/

    }

    return bResult;
}

void CMainFrame::vSetDefaultConfiguration(eSECTION_ID eSecId)
{
    CString omVerStr("");
    omVerStr.Format(IDS_VERSION);

    switch (eSecId)
    {
        case MAINFRAME_SECTION_ID:
        {
            theApp.pouGetFlagsPtr()->vInitializeFlags();
            // Setting Hex mode
            bSetHexDecFlags(TRUE);
            m_sNotificWndPlacement.length = 0;
            m_sNotificWndPlacement.rcNormalPosition.top = -1;
            if (m_podUIThread != nullptr)
            {
                m_podUIThread->vUpdateWndCo_Ords(m_sNotificWndPlacement, TRUE);
                m_podUIThread->vClearTraceContents();
            }
            //Set default settings
        }
        break;
        case LOG_SECTION_ID:
        {
            sg_pouFrameProcCAN = GetICANLogger();
            sg_pouFrameProcCAN->StartEditingSession();
            sg_pouFrameProcCAN->ClearLoggingBlockList();
            sg_pouFrameProcCAN->StopEditingSession(TRUE);
        }
        break;
        case LOG_SECTION_J1939_ID:
        {
            if (GetIJ1939Logger() != nullptr)
            {
                GetIJ1939Logger()->StartEditingSession();
                GetIJ1939Logger()->ClearLoggingBlockList();
                GetIJ1939Logger()->StopEditingSession(true);
            }
        }
        break;
        case LOG_SECTION_LIN_ID:
        {
            sg_pouFrameProcLIN = GetILINLogger();
            sg_pouFrameProcLIN->StartEditingSession();
            sg_pouFrameProcLIN->ClearLoggingBlockList();
            sg_pouFrameProcLIN->StopEditingSession(TRUE);
        }
        break;
        case SIMSYS_SECTION_ID:
        {
            if (GetICANNodeSim() != nullptr)
            {
                GetICANNodeSim()->NS_SetSimSysConfigData(nullptr, 0);
            }
        }
        break;
        case SIMSYS_SECTION_J1939_ID:
        {
            if (GetIJ1939NodeSim() != nullptr)
            {
                GetIJ1939NodeSim()->NS_SetSimSysConfigData(nullptr, 0);
            }
        }
        break;
        case SIMSYS_SECTION_LIN_ID:
        {
            if (GetILINNodeSim() != nullptr)
            {
                GetILINNodeSim()->NS_SetSimSysConfigData(nullptr, 0);
            }
        }
        break;

        case REPLAY_SECTION_ID:
        {
            vREP_SetReplayConfigData(nullptr, 0);
        }
        break;
        case MSGWND_SECTION_ID:
        {
            CMessageAttrib::ouGetHandle(CAN).vSetMessageAttribData(nullptr);
            m_anMsgBuffSize[CAN][defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
            m_anMsgBuffSize[CAN][defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
            m_anMsgBuffSize[CAN][defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;

            SFILTERAPPLIED_CAN sMsgWndFilter;
            sMsgWndFilter.vClear();
            if (nullptr == m_podMsgWndThread)
            {
                return;
            }
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                          WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, 0);

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                          WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_SET_CONFIG_DATA,
                          0);
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(CAN),
                          WM_NOTIFICATION_FROM_OTHER,
                          eLOAD_DATABASE,
                          (LPARAM)m_ouBusmasterNetwork );
        }
        break;
        case MSGWND_SECTION_LIN_ID:
        {
            CMessageAttrib::ouGetHandle(LIN).vSetMessageAttribData(nullptr);
            m_anMsgBuffSize[LIN][defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
            m_anMsgBuffSize[LIN][defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
            m_anMsgBuffSize[LIN][defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;

            SFILTERAPPLIED_LIN sMsgWndFilter;
            sMsgWndFilter.vClear();
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                          WM_SET_FILTER_DETAILS, (WPARAM)&sMsgWndFilter, 0);

            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(LIN),
                          WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_SET_CONFIG_DATA,
                          0);
        }
        break;
        case MSGWND_SECTION_J1939_ID:
        {
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                          WM_NOTIFICATION_FROM_OTHER,
                          eWINID_MSG_WND_SET_CONFIG_DATA,
                          0);
            ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                          WM_NOTIFICATION_FROM_OTHER,
                          eLOAD_DATABASE,
                          (LPARAM)(m_ouBusmasterNetwork));
        }
        break;
        case SIGWATCH_SECTION_ID:
        {
            vSW_DoInitialization(CAN);

            sg_pouSWInterface[CAN]->SW_SetConfigData(nullptr);
            sg_pouSWInterface[CAN]->SW_ClearSigWatchWnd();
        }
        break;

        case SIGWATCH_SECTION_LIN_ID:
        {
            //Set default settings
            if (sg_pouSWInterface[LIN] == nullptr)
            {
                if (SW_GetInterface(LIN, (void**)&sg_pouSWInterface[LIN]) == S_OK)
                {
                    sg_pouSWInterface[LIN]->SW_DoInitialization( m_ouBusmasterNetwork);
                }
            }
            sg_pouSWInterface[LIN]->SW_SetConfigData(nullptr);
            sg_pouSWInterface[LIN]->SW_ClearSigWatchWnd();
        }
        break;

        case SIGWATCH_SECTION_J1939_ID:
        {
            vSW_DoInitialization(J1939);


            sg_pouSWInterface[J1939]->SW_SetConfigData(nullptr);
            sg_pouSWInterface[J1939]->SW_ClearSigWatchWnd();
        }
        break;
        case DIL_SECTION_ID:
        {
            m_dwDriverId = DRIVER_CAN_STUB;
            m_byControllerMode = defMODE_SIMULATE;
            IntializeDIL();
            for (UINT i = 0; i < defNO_OF_CHANNELS; i++)
            {
                vInitialize( m_asControllerDetails[i], FALSE );
            }

            g_pouDIL_CAN_Interface->DILC_SetConfigData(m_asControllerDetails,
                    sizeof(SCONTROLLER_DETAILS) * defNO_OF_CHANNELS);
            //Set default settings
        }
        break;
        case GRAPH_SECTION_ID:
        {
            for(int nBUSID=0; nBUSID<AVAILABLE_PROTOCOLS; nBUSID++)
            {
                if(nBUSID == 5)
                {
                    //TODO: When Signal graph for ETHERNET is implemented remove this check
                    //Skip Ethernet protocol
                    continue;
                }
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
            //Arun
            vPostConfigChangeCmdToSigGrphWnds();
        }
        break;
        case TXWND_SECTION_ID:
        {
            m_objTxHandler.vSetTxWndConfigData(CAN,nullptr);
        }
        break;

        case FILTER_SECTION_ID:
        {
            m_sFilterAppliedCAN.vClear();
        }
        break;
        case FILTER_SECTION_LIN_ID:
        {
            m_sFilterAppliedLIN.vClear();
        }
        break;

        case DATABASE_SECTION_J1939_ID:
        {
            //Clear all databases
            if (m_pouMsgSigJ1939 == nullptr)
            {
                m_pouMsgSigJ1939 = new CMsgSignal(sg_asDbParams[J1939], FALSE);
            }
            vClearDbInfo(J1939);
        }
        break;
        case DATABASE_SECTION_ID:
        {
            //Clear all databases
            vClearDbInfo(CAN);
        }
        break;
        case WAVEFORMDATA_SECTION_ID:
        {
            m_objWaveformDataHandler.SetConfigData((BYTE*)nullptr);
        }
        break;
        case BUS_STATISTICS_SECTION_ID:
        {
            GetICANBusStat()->BSC_ResetBusStatistic();
            GetILINBusStat()->BSL_ResetBusStatistic();

            // Update the bus statistics window if it exists.
            if ( m_oNetworkStatistics != nullptr &&
                    m_oNetworkStatistics->IsWindowVisible( ) == TRUE )
            {
                // Perform network statistics calculation and update of
                // network statistics window
                m_oNetworkStatistics->vSendMessage(CAN);
                m_oNetworkStatistics->vSendMessage(LIN);
            }

            if(m_bIsStatWndCreated == TRUE && m_oNetworkStatistics != nullptr)
            {
                m_oNetworkStatistics->hSetConfigData(nullptr);
            }
            if(nullptr != m_pXmlNodeBusStats)
            {
                xmlFreeNode(m_pXmlNodeBusStats);
                m_pXmlNodeBusStats = nullptr;
            }
        }
        break;
        case TEST_SETUP_EDITOR_SECTION_ID:
        {
            BYTE* pTempData = nullptr;
            unsigned int nData = 0;
            m_objTSEditorHandler.vSetConfigurationData(pTempData, nData);
        }
        break;

        case LIN_TXWND:
        {
            m_objTxHandler.vSetTxWndConfigData(LIN, nullptr);
        }
        break;
        case TEST_SUITE_EXECUTOR_SECTION_ID:
        {
            BYTE* pTempData = nullptr;
            unsigned int nData = 0;
            m_objTSExecutorHandler.vSetConfigurationData(pTempData, nData);
        }
        break;
        
        case LIN_CLUSTER_CONFIG:
        {
            vInitialiaseLINConfig(1);
            m_objTxHandler.SetNetworkConfig(LIN, m_ouBusmasterNetwork);
            m_objTxHandler.vSetScheduleConfig(NULL);
            m_objTxHandler.vShowLINScheduleConfigDlg(false);
        }
        break;

        default:
        {

        }
        break;
    }
}


void CMainFrame::OnSelectDriver(UINT nID)
{
    DILINFO* psCurrDIL = psGetDILEntry(nID);
    // Above lines have to be changed.

    if (psCurrDIL != nullptr)
    {
        m_dwDriverId =  psCurrDIL->m_dwDriverID;

        HRESULT hResult = IntializeDIL();


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


    /*if (psCurrDIL != nullptr)
    {
        m_dwLINDriverId =  psCurrDIL->m_dwDriverID;

        HRESULT hResult = IntializeDILL();
    }*/

    //m_bFlxDILChanging = TRUE;
    DILINFO* psCurrDIL = psGetDILLINEntry(nID);

    //TODO::FIBEX Validation is Required

    ICluster* pCluster = nullptr;
    ChannelSettings ouChannelSetting;

    m_ouBusmasterNetwork->GetDBService(LIN, 0, 0, &pCluster);

    if (  pCluster == nullptr  )
    {
        m_ouBusmasterNetwork->GetChannelSettings(LIN, 0, &ouChannelSetting);
        if ( ouChannelSetting.m_ouLINSettings.m_bOverWriteSettings == false )
        {
            int nSelection = MessageBox("Please Configure the Cluster Parameters", "Invalid Cluster Configuration", MB_OK);
            if ( nSelection == IDOK )
            {
                OnLinClusterConfig();
            }
        }
    }

    m_ouBusmasterNetwork->GetDBService(LIN, 0, 0, &pCluster);

    if (  pCluster == nullptr  )
    {
        m_ouBusmasterNetwork->GetChannelSettings(LIN, 0, &ouChannelSetting);
        if ( ouChannelSetting.m_ouLINSettings.m_bOverWriteSettings == false )
        {
            return;
        }
    }

    if (psCurrDIL != nullptr)
    {
        m_shLINDriverId =  psCurrDIL->m_dwDriverID;

        HRESULT hResult = IntializeDILL();

        if ( hResult != S_OK )
        {

            /* Resetting DIL to NONE */
            if ( g_pouDIL_LIN_Interface )
            {
                m_shLINDriverId = DAL_NONE;
                g_pouDIL_LIN_Interface->DILL_SelectDriver(m_shLINDriverId, m_hWnd);
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
    if (psCurrDIL != nullptr)
    {
        if (g_pouDIL_CAN_Interface != nullptr)
        {
            bSelected = (psCurrDIL->m_dwDriverID == g_pouDIL_CAN_Interface->DILC_GetSelectedDriver());
        }
    }
    CFlags* pFlag = theApp.pouGetFlagsPtr();
    if (pFlag != nullptr)
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
    if (psCurrDIL != nullptr)
    {
        if (g_pouDIL_LIN_Interface != nullptr)
        {
            bSelected = (psCurrDIL->m_dwDriverID == g_pouDIL_LIN_Interface->DILL_GetSelectedDriver());
        }
    }
    CFlags* pFlag = theApp.pouGetFlagsPtr();
    if (pFlag != nullptr)
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

    if(nullptr != psCurrDIL && psCurrDIL->m_dwDriverID != DAL_LIN_NONE)
    {
        pCmdUI->SetCheck(bSelected);
    }
}

CMFCRibbonCategory* CMainFrame::GetCategory(const char* categoryName)
        {
	auto count = mRibbonBar.GetCategoryCount();
	for (int i = 0; i < count; i++)

    {
		auto category = mRibbonBar.GetCategory(i);
		if (nullptr != category)
        {
			std::string name = category->GetName();
			if (categoryName == name)
        {
				return category;
        }
    }
    }

	return nullptr;
}
CMFCRibbonPanel* CMainFrame::GetRibbonPanel(CMFCRibbonCategory* category, const char* panelName)
{
	if(category == nullptr)
	{
		return nullptr;
	}
	int panelCount = category->GetPanelCount();
	for (int i = 0; i < panelCount; i++)
	{
		auto panel = category->GetPanel(i);
		if (nullptr != panel)
		{
			std::string name = panel->GetName();
			if (name == panelName)
			{
				return panel;
			}
		}
	}
	
	
	CMFCRibbonPanel* newPanel = category->AddPanel(panelName);
	newPanel->SetJustifyColumns(FALSE);
	return newPanel;
}

bool CMainFrame::bUpdatePopupMenuDIL()
{
	auto canCategory = GetCategory("CAN");
	if (canCategory == nullptr) return false;

	auto panel = GetRibbonPanel(canCategory, "Hardware Configuration");
	if (nullptr == panel) return false;

        /* Create a new popup Menu */
	
	auto control = static_cast<CMFCRibbonButton*>(panel->GetElement(2));
	
                // Add the DIL list
	for (int i = 0; (i < m_nDILCount) ; i++)
                {
		CMFCRibbonButton* newButton = new CMFCRibbonButton();
		
		newButton->SetText(m_ouList[i].m_acName.c_str());
                        m_ouList[i].m_ResourceID = IDC_SELECT_DRIVER + i;

		newButton->SetVisible(TRUE);
		newButton->SetID(IDC_SELECT_DRIVER + i);
		control->AddSubItem(newButton);
        }
        // Added shortcut key


	return true;
}

bool CMainFrame::bUpdatePopupMenuDILL(void)
{
	auto canCategory = GetCategory("LIN");
	if (canCategory == nullptr) return false;

	auto panel = GetRibbonPanel(canCategory, "Hardware Configuration");
	if (nullptr == panel) return false;

        /* Create a new popup Menu */

	auto control = static_cast<CMFCRibbonButton*>(panel->GetElement(2));

                // Add the DIL list
	for (int i = 0; (i < m_nDILCountLin); i++)
                {
		CMFCRibbonButton* newButton = new CMFCRibbonButton();
		
		newButton->SetText(m_ouListLin[i].m_acName.c_str());
		m_ouListLin[i].m_ResourceID = IDC_SELECT_LIN_DRIVER + i;

		newButton->SetVisible(TRUE);
		newButton->SetID(IDC_SELECT_LIN_DRIVER + i);
		control->AddSubItem(newButton);
                    }

        // Added shortcut key


	return true;
}

DILINFO* CMainFrame::psGetDILEntry(UINT unKeyID, bool bKeyMenuItem)
{
    DILINFO* psResult = nullptr;
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

DILINFO* CMainFrame::psGetDILLINEntry(UINT unKeyID, bool bKeyMenuItem)
{
    DILINFO* psResult = nullptr;
    for (int i = 0; i < m_nDILCountLin; i++)
    {
        if (bKeyMenuItem == true)
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
    IBMNetWorkGetService* pomDatabase = m_ouBusmasterNetwork;
    if( pomDatabase != nullptr )
    {
        std::list<IFrame*> frameList;
        m_ouBusmasterNetwork->GetFrameList( CAN, 0, frameList );
        if ( frameList.size() > 0 )
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
    vPopulateMainEntryList( &m_odResultingList, nullptr, m_ouBusmasterNetwork );
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
        // Disabling Data logging Flag
        if (nullptr != sg_pouFrameProcCAN)
        {
            sg_pouFrameProcCAN->DisableDataLogFlag();
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
            if (pouFlags != nullptr)
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
        case LOG_ENABLE_DISABLE_LIN:
        {
            if (pouFlags != nullptr)
            {
                BOOL bEnable = pouFlags->nGetFlagStatus(LOGTOFILE_LIN);
                BOOL bRequest = (BOOL)lParam;
                if (bRequest != bEnable)
                {
                    OnLog_LIN_Enable();
                }
            }
        }
        break;

        case DIS_CONNECT:
        {
            if (pouFlags != nullptr)
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
            if (pouFlags != nullptr)
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
            if( nullptr != pacText )
            {
                CString omText;
                omText.Format("%s", A2T(pacText));
                if (sg_pouFrameProcCAN != nullptr)
                {
                    sg_pouFrameProcCAN->LogString(omText);
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
            if( nullptr != pacText )
            {
                CString omText;
                omText.Format("%s", A2T(pacText));
                if (GetIJ1939Logger() != nullptr)
                {
                    GetIJ1939Logger()->LogString(omText);
                }
            }
            else
            {
                hRetVal = S_FALSE;
            }
        }
        break;
        case WRITE_TO_LOGFILE_LIN:
        {
            char* pacText = (char*)lParam;
            if( nullptr != pacText )
            {
                CString omText;
                omText.Format("%s", A2T(pacText));
                if (GetILINLogger() != nullptr)
                {
                    GetILINLogger()->LogString(omText);
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


void CMainFrame::vInitializeBusStatLIN(void)
{
    GetILINBusStat()->BSL_DoInitialization();
    //sCONTROLLERDETAILSLIN sControllerDetails[defNO_OF_LIN_CHANNELS];
    //          int nCount = 0;
    //g_pouDIL_LIN_Interface->DILL_GetConfiguration(sControllerDetails, nCount);

    vSetBaudRateInfo(LIN);
    for (int i = 0; i < defNO_OF_LIN_CHANNELS; i++)
    {
        GetILINBusStat()->BSL_SetBaudRate(i, m_asControllerDetailsLIN[i].m_BaudRate);
    }
}


void CMainFrame::vInitializeGraphWndReadBuffer()
{
    if (g_pouDIL_CAN_Interface != nullptr)
    {
        DWORD dwClientId = 0;
        g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, dwClientId, CAN_MONITOR_NODE);
        if (g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_ADD, dwClientId, &m_ouCanBuf) != S_OK)
        {
            TRACE("The function vInitializeGraphWndReadBuffer() failed.");
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

    CSigGrphConfigDlg objSigGrphCnfg(m_ouBusmasterNetwork, nHardware);
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

        CFlags* pouFlags = nullptr;
        pouFlags   = theApp.pouGetFlagsPtr();
        if(pouFlags != nullptr)
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
    CFlags* pouFlags = nullptr;
    pouFlags   = theApp.pouGetFlagsPtr();
    if(pouFlags != nullptr)
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

void CMainFrame::vPostConfigChangeCmdToSigGrphWnds(bool bHideGraphWnd)
{
    for(register int nBusID = CAN; nBusID < AVAILABLE_PROTOCOLS; nBusID++)
    {
        if(nBusID == 5)
        {
            //TODO: When Signal graph for ETHERNET is implemented remove this check
            //Skip Ethernet protocol
            continue;
        }
        m_objSigGrphHandler.vPostMessageToSGWnd((SHORT)nBusID, WM_USER_CMD,
                                                (WPARAM)eCONFIGCHANGECMD, (LPARAM)bHideGraphWnd);
    }
}

// START J1939 RELATED HANDLERS
void CMainFrame::OnActivateJ1939()
{
    HRESULT Result = S_FALSE;
    bool bActivateStatus = false;
    if ((nullptr == sg_pouIJ1939DIL) && (nullptr == sg_pouIJ1939Logger))
    {
        Result = ProcessJ1939Interfaces();
        GetIJ1939NodeSim()->NS_SetJ1939ActivationStatus(true);
        m_podMsgWndThread->vModifyVisibilityStatus(SW_SHOW, J1939);
        bActivateStatus = true;
    }
    else
    {
        Result = DeselectJ1939Interfaces();
        GetIJ1939NodeSim()->NS_SetJ1939ActivationStatus(false);
        m_podMsgWndThread->vModifyVisibilityStatus(SW_HIDE, J1939);
    }

	setConnectStateJ1939(bActivateStatus);

    /* If J1939 is deactivated, retain the old icon set and hide J1939 Tx window */
    if ( !bActivateStatus )
    {
        if(m_pouTxMsgWndJ1939 != nullptr)
        {
            m_pouTxMsgWndJ1939->ShowWindow(SW_HIDE);
        }
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
    if (nullptr != g_pouDIL_CAN_Interface)
    {
        if ((nullptr == sg_pouIJ1939DIL) && (nullptr == sg_pouIJ1939Logger))
        {
            pCmdUI->SetText(_("Activate"));
        }
        else
        {
            pCmdUI->SetText(_("Deactivate"));
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
    if ((nullptr == sg_pouLinDIL) && (nullptr == sg_pouLinLogger))
    {
        pCmdUI->SetText(_("&Activate"));
    }
    else
    {
        pCmdUI->SetText(_("D&eactivate"));
    }

    pCmdUI->Enable(TRUE);


}

void CMainFrame::OnJ1939ConfigLog()
{
    if (nullptr != sg_pouIJ1939Logger)
    {
        ConfigureJ1939Log(sg_pouIJ1939Logger, m_abLogOnConnect[J1939], this, &m_sFilterAppliedJ1939);
    }
}


void CMainFrame::OnUpdateJ1939ConfigLog(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(nullptr != sg_pouIJ1939Logger);
}

//shashank
void CMainFrame::OnUpdateLinConfigLog(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(nullptr != sg_pouLinLogger);
}

void CMainFrame::OnActionJ1939Online()
{
    bool bOnlineStatus = false;
    if (nullptr != sg_pouIJ1939DIL && sg_pouIJ1939DIL->DILIJ_bIsOnline() == FALSE)
    {
        if (sg_pouIJ1939DIL->DILIJ_GoOnline() == S_OK)
        {
            bOnlineStatus = true;
            theApp.bWriteIntoTraceWnd(_("DIL.J1939 network started..."));

            CFlags* pouFlags = theApp.pouGetFlagsPtr();
            if (nullptr != pouFlags)
            {
                BOOL bConnected = pouFlags->nGetFlagStatus(CONNECTED);
                if (bConnected == TRUE)
                {
                    if (pouFlags != nullptr)
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
            }
            if (m_pouTxMsgWndJ1939 != nullptr)
            {
                m_pouTxMsgWndJ1939->vSetJ1939ClientParam(m_sJ1939ClientParam);
                CFlags* pouFlags = theApp.pouGetFlagsPtr();
                if (pouFlags != nullptr)
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
        if (m_pouTxMsgWndJ1939 != nullptr)
        {
            ::SendMessage(m_pouTxMsgWndJ1939->GetSafeHwnd(),
                          WM_CONNECT_CHANGE, (WPARAM)FALSE, 0);
        }
        if (nullptr != sg_pouIJ1939DIL && sg_pouIJ1939DIL->DILIJ_GoOffline() == S_OK)
        {
            theApp.bWriteIntoTraceWnd(_("DIL.J1939 network stopped..."));
        }
        if(nullptr != sg_pouIJ1939Logger)
        {
            sg_pouIJ1939Logger->DisableDataLogFlag();
        }
    }
    
    /* If J1939 goes offline, Switch off J1939 Transmit Item */
    if ( !bOnlineStatus )
    {
        if(m_pouTxMsgWndJ1939 != nullptr)
        {
            m_pouTxMsgWndJ1939->ShowWindow(SW_HIDE);
        }
    }
}

void CMainFrame::OnUpdateActionJ1939Online(CCmdUI* pCmdUI)
{
    if (nullptr != sg_pouIJ1939DIL)
    {
        pCmdUI->Enable(TRUE);
        if (sg_pouIJ1939DIL->DILIJ_bIsOnline() == TRUE)
        {
            pCmdUI->SetText(_("&Go Offline"));
        }
        else
        {
            pCmdUI->SetText(_("&Go Online"));
        }
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}

void CMainFrame::OnActionJ1939TxMessage()
{
    if (m_pouTxMsgWndJ1939 == nullptr)
    {
        m_pouTxMsgWndJ1939 = new CTxMsgWndJ1939(this, m_sJ1939ClientParam);
        if (m_pouTxMsgWndJ1939 != nullptr)
        {
            m_pouTxMsgWndJ1939->Create(IDD_DLG_TX);
            m_pouTxMsgWndJ1939->vSetDatabaseInfo(m_ouBusmasterNetwork);
        }
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

    if(m_pouTxMsgWndJ1939 != nullptr)
    {
        BOOL bIsWindowEnabled = m_pouTxMsgWndJ1939->IsWindowVisible();

        pCmdUI->SetCheck(bIsWindowEnabled);
    }
}

void CMainFrame::vJ1939StartStopLogging()
{
    // Enable Logging or stop logging
    if (nullptr != sg_pouIJ1939Logger)
    {
        bool bEnable = !sg_pouIJ1939Logger->IsLoggingON();
        if (bEnable)
        {
            vSetAssociatedDatabaseFiles(J1939); // Update the db file names associated
            vSetBaudRateInfo(J1939);                // Update the baud rate details
            m_unJ1939TimerSBLog = SetTimer(TIMER_REFRESH_J1939_LOG_STATUS, STSBAR_REFRESH_TIME_PERIOD_LOG, nullptr);
            /* if(nullptr != sg_pouIJ1939Logger && theApp.pouGetFlagsPtr()->nGetFlagStatus(ACTIVATED_J1939) == TRUE && theApp.pouGetFlagsPtr()->nGetFlagStatus(CONNECTED) == TRUE)
             {
                 sg_pouIJ1939Logger->vSetMeasurementFileName();
             }*/
        }
        sg_pouIJ1939Logger->EnableLogging(bEnable);
    }

}

void CMainFrame::OnActionJ1939Log()
{
    vJ1939StartStopLogging();
}

void CMainFrame::OnUpdateActionJ1939Log(CCmdUI* pCmdUI)
{
    if (nullptr != sg_pouIJ1939Logger)
    {
        USHORT ushCount = sg_pouIJ1939Logger->GetLoggingBlockCount();
        if(ushCount>0)//log file found
        {
            pCmdUI->Enable(TRUE); //enable the menu item
            pCmdUI->SetCheck(sg_pouIJ1939Logger->IsLoggingON());
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
    if (nullptr != sg_pouLinLogger)
    {
        USHORT ushCount = sg_pouLinLogger->GetLoggingBlockCount();
        if(ushCount>0)//log file found
        {
            pCmdUI->Enable(TRUE); //enable the menu item
            pCmdUI->SetCheck(sg_pouLinLogger->IsLoggingON());
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

// START J1939 RELATED HELPER FUNCTIONS
HRESULT CMainFrame::ProcessJ1939Interfaces(void)
{
    HRESULT Result = S_FALSE;

    theApp.bWriteIntoTraceWnd(_("Querying DIL.J1939 ..."));
    if (nullptr != (sg_pouIJ1939DIL = GetIJ1939DIL())) // Successfully get the DIL interface
    {
        theApp.bWriteIntoTraceWnd(_("DIL.J1939 query successful..."));
        // Now update the global status
        GetIFlags()->vSetFlagStatus(ACTIVATED_J1939, (int) TRUE);

        // Initialise the interface and register
        Result = sg_pouIJ1939DIL->DILIJ_Initialise(GetICANDIL());
        if (S_OK == Result)
        {
            theApp.bWriteIntoTraceWnd(_("DIL.J1939 initialisation successful..."));
            Result = sg_pouIJ1939DIL->DILIJ_RegisterClient(TRUE,
                     J1939_MONITOR_NODE, J1939_ECU_NAME, 0, m_sJ1939ClientParam.m_dwClientId );
            if (Result == S_OK || Result == ERR_CLIENT_EXISTS)
            {
                strcpy_s(m_sJ1939ClientParam.m_acName, MAX_PATH, J1939_MONITOR_NODE);
                m_sJ1939ClientParam.m_unEcuName = J1939_ECU_NAME;
                m_podMsgWndThread->vUpdateClientID(J1939, m_sJ1939ClientParam.m_dwClientId);
                m_podMsgWndThread->vSetDILInterfacePointer(J1939, (void**)&(sg_pouIJ1939DIL));
                vReRegisterAllJ1939Nodes();
                vSW_DoInitialization(J1939);

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
            if (nullptr != sg_pouIJ1939Logger)
            {
                theApp.bWriteIntoTraceWnd(_("J1939 logger query successful..."));

                // Next initialise the interface
                CParamLoggerJ1939 ouParam;
                CString omVerStr("");       // First get the version information
                omVerStr.Format(IDS_VERSION);   // string from the rsource
                strcpy_s(ouParam.m_acVersion, MAX_PATH, omVerStr.GetBuffer(0));

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

HRESULT CMainFrame::DeselectJ1939Interfaces(BOOL bTrace)
{
    HRESULT Result = S_OK;

    // First stop J1939 logging if it is ON
    if (nullptr != sg_pouIJ1939Logger)
    {
        if (sg_pouIJ1939Logger->IsLoggingON() == TRUE)
        {
            theApp.bWriteIntoTraceWnd(_("Stopping J1939 logging..."), bTrace);
            if (sg_pouIJ1939Logger->EnableLogging(FALSE) != S_OK)
            {
                theApp.bWriteIntoTraceWnd(_("Stopping J1939 logging failed..."), bTrace);
                Result = S_FALSE;
            }
        }
        // Logger interface needs a reinitialisation next time. Set the pointer to
        // nullptr to update the GUI accordingly.
        sg_pouIJ1939Logger = nullptr;
    }

    if (nullptr != sg_pouIJ1939DIL)
    {
        if (m_pouTxMsgWndJ1939 != nullptr)
        {
            ::SendMessage(m_pouTxMsgWndJ1939->GetSafeHwnd(), WM_CONNECT_CHANGE, (WPARAM)FALSE, 0);
        }
        if (sg_pouIJ1939DIL->DILIJ_bIsOnline() == TRUE)
        {
            theApp.bWriteIntoTraceWnd(_("Going Offline..."), bTrace);
            if (sg_pouIJ1939DIL->DILIJ_GoOffline() != S_OK)
            {
                theApp.bWriteIntoTraceWnd(_("Going Offline failed..."), bTrace);
                Result = S_FALSE;
            }
        }
        theApp.bWriteIntoTraceWnd(_("Uninitialising DIL.J1939..."), bTrace);
        // On Disassociate of J1939 Unload all the Simulated systems
        CStringArray omStrBuildFiles;
        GetIJ1939NodeSim()->NS_DllUnloadAll(&omStrBuildFiles);
        if (sg_pouIJ1939DIL->DILIJ_Uninitialise() != S_OK)
        {
            theApp.bWriteIntoTraceWnd(_("Uninitialising DIL.J1939 failed..."), bTrace);
            Result = S_FALSE;
        }
        else
        {
            theApp.bWriteIntoTraceWnd(_("Done..."),bTrace);
        }
        // Update the global status and reset the inteface pointer to nullptr.
        GetIFlags()->vSetFlagStatus(ACTIVATED_J1939, (int) FALSE);
        GetIFlags()->vSetFlagStatus(ADRESSCLAIM_J1939, FALSE);
        sg_pouIJ1939DIL = nullptr;
    }

    return Result;
}

// END J1939 RELATED HELPER FUNCTIONS

void CMainFrame::OnJ1939ConfigureTimeouts()
{
    CJ1939TimeOutCfg omDlg;
    omDlg.DoModal();
}

void CMainFrame::OnUpdateJ1939Timeouts(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(nullptr != sg_pouIJ1939DIL);
}

void CMainFrame::OnUpdateJ1939DBNew(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( !CMsgSignalDBWnd::sm_bValidJ1939Wnd );
}

static void vGetNewJ1939DBName(CString& omString)
{
    UINT unCount = 1;
    for(;;)
    {
        CString omStrDbName = NEW_DATABASE_NAME_J1939;
        CString omStr = "";
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
            break;
        }
    }
}

static BOOL bCreateStudioFile(CString& omDefaultFileName)
{
    BOOL bResult = FALSE;
    CStdioFile om_File;
    TRY
    {
        CFileDialog fileDlg(FALSE, DATABASE_EXTN, omDefaultFileName.GetBuffer(0),
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
        if(om_Fe != nullptr )
        {
            LPTSTR lpszError = nullptr;
            // Get error
            om_Fe->GetErrorMessage( lpszError, 255);

            AfxMessageBox( lpszError, 0, MB_OK );

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
    if (m_pouActiveDbJ1939 == nullptr)
    {
        m_pouActiveDbJ1939 = new CMsgSignal(sg_asDbParams[J1939], FALSE);
    }

    if (nullptr != m_podMsgSgWndJ1939 && !IsWindow(m_podMsgSgWndJ1939->m_hWnd))
    {
        m_podMsgSgWndJ1939 = nullptr;
    }
    sg_asDbParams[J1939].m_pouMsgSignalActiveDB = m_pouActiveDbJ1939;
    sg_asDbParams[J1939].m_pouMsgSignalImportedDBs = m_pouMsgSigJ1939;
    if (m_podMsgSgWndJ1939 == nullptr)
    {
        m_podMsgSgWndJ1939 = new CMsgSignalDBWnd(sg_asDbParams[J1939]);
    }

    if (m_podMsgSgWndJ1939 != nullptr)
    {
        vGetNewJ1939DBName(m_omJ1939DBName);
        if (bCreateStudioFile(m_omJ1939DBName) == TRUE)
        {
            sg_asDbParams[J1939].m_omDBPath = m_omJ1939DBName;
            m_podMsgSgWndJ1939->vSetDBName(m_omJ1939DBName);

            if ( !m_podMsgSgWndJ1939->Create(  nullptr, _("Database Editor"), WS_CHILD |
                                               WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION |
                                               WS_THICKFRAME, rectDefault, this ))
            {
                MessageBox( _("Create BUSMASTER Database Window Failed!"),
                            nullptr, MB_OK|MB_ICONERROR );
                return;
            }

            CMsgSignalDBWnd::sm_bValidJ1939Wnd = TRUE;
            m_podMsgSgWndJ1939->ShowWindow( SW_SHOWNORMAL );
            m_podMsgSgWndJ1939->UpdateWindow();

            // Set the flag to indicate the opening of database window
            CFlags* pFlags = theApp.pouGetFlagsPtr();

            if(pFlags != nullptr)
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
                         nullptr,
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         _("J1939 Database File(s)(*.dbf)|*.dbf||"),
                         nullptr );
    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _("Select J1939 Database Filename...");

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
            if (m_pouActiveDbJ1939 == nullptr)
            {
                sg_asDbParams[J1939].m_pouMsgSignalActiveDB = m_pouActiveDbJ1939;
                sg_asDbParams[J1939].m_pouMsgSignalImportedDBs = m_pouMsgSigJ1939;
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
            if ( m_podMsgSgWndJ1939 != nullptr )
            {
                m_podMsgSgWndJ1939 = nullptr;
            }

            sg_asDbParams[J1939].m_pouMsgSignalActiveDB = m_pouActiveDbJ1939;
            sg_asDbParams[J1939].m_pouMsgSignalImportedDBs = m_pouMsgSigJ1939;

            m_podMsgSgWndJ1939 = new CMsgSignalDBWnd(sg_asDbParams[J1939]);

            if ( m_podMsgSgWndJ1939 != nullptr )
            {
                sg_asDbParams[J1939].m_omDBPath = m_omJ1939DBName;
                m_podMsgSgWndJ1939->vSetDBName(m_omJ1939DBName);
                // Create child window
                if ( !m_podMsgSgWndJ1939->Create(nullptr,
                                                 _("Database Editor"),
                                                 WS_CHILD | WS_VISIBLE |
                                                 WS_OVERLAPPED | WS_CAPTION |
                                                 WS_THICKFRAME, rectDefault,
                                                 this ) )
                {
                    MessageBox( _("Create J1939 Database Window Failed!"),
                                nullptr, MB_OK|MB_ICONERROR );
                    return;
                }
                CMsgSignalDBWnd::sm_bValidJ1939Wnd = TRUE;
                m_podMsgSgWndJ1939->ShowWindow( SW_SHOWNORMAL );
                m_podMsgSgWndJ1939->UpdateWindow();

                // Set the flag to indicate the opening of database window
                CFlags* pFlags = theApp.pouGetFlagsPtr();
                if(pFlags != nullptr)
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
    if (nullptr == m_podMsgSgWndJ1939)
    {
        return;
    }
    // Get appropriate data structure
    CMsgSignal* pTempMsgSg = m_podMsgSgWndJ1939->m_sDbParams.m_pouMsgSignalActiveDB;
    if (nullptr == pTempMsgSg)
    {
        return;
    }
    if (pTempMsgSg->bGetModifiedFlag() == FALSE)
    {
        UINT bRetVal = AfxMessageBox(_(ASK_SAVE_PROMPT),
                                     MB_YESNOCANCEL | MB_ICONQUESTION);
        if (bRetVal == IDYES)
        {
            //save the database modificatins.
            m_podMsgSgWndJ1939->vSaveModifiedDBs(pTempMsgSg);
        }
        else if (bRetVal == IDNO)
        {
            // if this is new database
            // then delete the memory and the file itself
            if (vGetNewDatabaseFlag())
            {
                // This file is no longer required
                CFile::Remove(m_podMsgSgWndJ1939->m_sDbParams.m_omDBPath);
                vSetNewDatabaseFlag(FALSE);
            }
        }
        else if (bRetVal == IDCANCEL)
        {
            return;
        }

    }
    else
    {
        // If the user just creates new database,
        // and closes the window
        // delete the file
        if (vGetNewDatabaseFlag())
        {
            // This file is no longer required
            CFile::Remove(m_podMsgSgWndJ1939->m_sDbParams.m_omDBPath);
            vSetNewDatabaseFlag(FALSE);
        }

    }

    // delete previously allocated memory if any
    pTempMsgSg->bDeAllocateMemoryInactive();

    CMsgSignalDBWnd::sm_bValidJ1939Wnd = FALSE;
    CFlags* pFlags = theApp.pouGetFlagsPtr();
    if (pFlags != nullptr)
    {
        pFlags->vSetFlagStatus(DBOPEN_J1939, FALSE);
    }

    m_podMsgSgWndJ1939->MDIDestroy();
    m_podMsgSgWndJ1939 = nullptr;
}
LRESULT CMainFrame::OnJ1939DBClose(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    OnJ1939DBClose();
    return S_OK;
}

void CMainFrame::OnJ1939DBSave()
{
    if (m_podMsgSgWndJ1939 != nullptr)
    {
        if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
        {
            //send save message
            m_podMsgSgWndJ1939->SendMessage(WM_SAVE_DBJ1939, 0, 0);

            // Set all the items in the tree view to normal font
            if ( m_pomMsgSgTreeViews[J1939] != nullptr)
            {
                m_pomMsgSgTreeViews[J1939]->vSetAllItemsNormal();
            }
        }
    }
}

void CMainFrame::OnUpdateJ1939DBSave(CCmdUI* pCmdUI)
{
    BOOL bResult = FALSE;
    if (m_podMsgSgWndJ1939 != nullptr)
    {
        if (CMsgSignalDBWnd::sm_bValidJ1939Wnd == TRUE)
        {
            // Get appropriate data structure
            CMsgSignal* pTempMsgSg = m_podMsgSgWndJ1939->m_sDbParams.m_pouMsgSignalActiveDB;
            if(pTempMsgSg != nullptr)
            {
                if (pTempMsgSg->bGetModifiedFlag() == FALSE)
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
    CFileDialog fileDlg(TRUE,      // Open File dialog
                        "dbf",     // Default Extension,
                        nullptr,
                        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                        _("BUSMASTER J1939 Database File(s)(*.dbf)|*.dbf||"),
                        nullptr );

    // Set Title
    fileDlg.m_ofn.lpstrTitle = _("Select Active Database Filename...");

    if ( IDOK == fileDlg.DoModal() )
    {
        POSITION pos = nullptr;
        pos = fileDlg.GetStartPosition();
        while(nullptr != pos)
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
            MessageBox(omStrMsg,"BUSMASTER",MB_OK|MB_ICONERROR);
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
                              (LPARAM)(m_ouBusmasterNetwork));
                ::SendMessage(hWnd, WM_DATABASE_CHANGE, (WPARAM)TRUE, 0);
            }
            if (m_pouTxMsgWndJ1939 != nullptr)
            {
                m_pouTxMsgWndJ1939->vSetDatabaseInfo(m_ouBusmasterNetwork);
            }
        }
    }
}

void CMainFrame::OnJ1939DBDissociate()
{
    std::list<ICluster*> clusterList;
    m_ouBusmasterNetwork->GetDBServiceList(J1939, 0, clusterList);
    std::string path;
    std::list<std::string> dbPathList;
for (auto cluster : clusterList)
    {
        cluster->GetDBFilePath(path);
        dbPathList.push_back(path);
    }

    CDatabaseDissociateDlg odDBDialog(dbPathList);
    if (IDOK == odDBDialog.DoModal())
    {
        dbPathList = odDBDialog.GetDissociatedFiles();
for (auto dbPath : dbPathList)
        {
            m_ouBusmasterNetwork->DeleteDBService(J1939, 0, dbPath);
        }

        ////////////////////////////////////////////////////////////

        m_ouBusmasterNetwork->SetChannelCount(J1939, 1);

        //1. Tx Window
        if (nullptr != m_pouTxMsgWndJ1939)
        {
            m_pouTxMsgWndJ1939->vSetDatabaseInfo(m_ouBusmasterNetwork);
        }
        //2. Message Window
        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939),
                      WM_NOTIFICATION_FROM_OTHER,
                      eLOAD_DATABASE,
                      (LPARAM)m_ouBusmasterNetwork);

        //3. Node Simulation
        CBaseNodeSim* pNodeSim = nullptr;
        pNodeSim = GetIJ1939NodeSim();
        if (pNodeSim != nullptr)
        {
            pNodeSim->NS_SetBmNetworkConfig(m_ouBusmasterNetwork, true);
        }

        //4. Signal Watch
        sg_pouSWInterface[J1939]->SW_ClearSigWatchWnd();
        sg_pouSWInterface[J1939]->SW_SetClusterInfo(m_ouBusmasterNetwork);

        ////////////////////////////////////////////////////////////////////

        vClearSignalInfoList();

        //Update Message windows
        vUpdateAllMsgWndInterpretStatus(J1939, FALSE);



    }
}

void CMainFrame::OnJ1939CfgSimSys()
{
    NS_InitJ1939SpecInfo();
    GetIJ1939NodeSim()->FE_CreateFuncEditorTemplate(this->GetSafeHwnd(), m_sExFuncPtr[J1939]);
}


void CMainFrame::OnUpdateJ1939CfgSimSys(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(nullptr != sg_pouIJ1939DIL);
}

void CMainFrame::OnJ1939SignalwatchAdd()
{
    vSW_DoInitialization(J1939);
    if ( sg_pouSWInterface[J1939] != nullptr )
    {
        sg_pouSWInterface[J1939]->SW_ShowAddDelSignalsDlg( this, m_ouBusmasterNetwork );
    }
    return;
}

void CMainFrame::OnJ1939SignalwatchShow()
{

    vSW_DoInitialization(J1939);
    if (sg_pouSWInterface[J1939] != nullptr)
    {
        INT nCmd = sg_pouSWInterface[J1939]->SW_IsWindowVisible() ? SW_HIDE : SW_SHOW;
        sg_pouSWInterface[J1939]->SW_ShowSigWatchWnd(this, GetSafeHwnd(), nCmd);
        BOOL bHexON = theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX);
        sg_pouSWInterface[J1939]->SW_SetDisplayMode(bHexON);
    }
}
void CMainFrame::OnUpdateJ1939SignalwatchShow(CCmdUI* pCmdUI)
{
    if(pCmdUI != nullptr )
    {
        if(sg_pouSWInterface[J1939] != nullptr)
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
    if (nullptr == m_podMsgWndThread)
    {
        //if Msg Window is not created no need to configure its display.Since there
        //is no way to get the existing details and to save the configured values
        return;
    }

    MessageWindowSettings settings;

    //Buffer
    settings.mBufferSettings.mISValidSettings = false;
    settings.mBusmasterIsOnline = true;


    //Filters
    settings.mFilterDetails.mISValidSettings = false;

    //message attributes
    CMessageAttrib& attributes = CMessageAttrib::ouGetHandle(J1939);
    settings.mMessageAttribute.mValidSettings = true;
    settings.mMessageAttribute.mMsgAttributes = &(attributes);

    ICluster* pCluster = nullptr;
    std::list<IFrame*> lstFrames;
    m_ouBusmasterNetwork->GetFrameList(J1939, 0, lstFrames);
    settings.mMessageAttribute.mMessageIDs = new UINT[lstFrames.size()];
    unsigned int unIndex = 0, unFrameId;
for (auto itrFrame : lstFrames)
    {
        itrFrame->GetFrameId(unFrameId);
        settings.mMessageAttribute.mMessageIDs[unIndex] = unFrameId;
        unIndex++;
    }

    settings.mMessageAttribute.mMsgCount = unIndex;


    //Invoke Settings Dialog
    MessageWindowSettingsDialog settingsDlg(defCONFIG_MSG_DISPLAY_J1939, &settings);
    if (IDOK == settingsDlg.DoModal())
    {
        ::SendMessage(m_podMsgWndThread->hGetHandleMsgWnd(J1939), WM_INVALIDATE_LIST_DISPLAY, 0, 0);
    }


}

void CMainFrame::OnAutomationTSEditor(void)
{
	try
	{
		//m_objTSEditorHandler.vShowTSEditorWindow((void*)this);

		// Get the working directory
		char acPath[MAX_PATH] = "";
		GetModuleFileName(nullptr, acPath, MAX_PATH);
		PathRemoveFileSpec(acPath);
		CString strPath = acPath;
		strPath += "\\TestSetupEditorGUI.exe";

		if (PathFileExists(strPath) == TRUE)
		{
			// Launch the converter utility
			PROCESS_INFORMATION sProcessInfo;
			STARTUPINFO sStartInfo;

			memset(&sProcessInfo, 0, sizeof(PROCESS_INFORMATION));
			memset(&sStartInfo, 0, sizeof(STARTUPINFO));

			int nSuccess = CreateProcess(strPath.GetBuffer(MAX_PATH), "",
				nullptr, nullptr, false, CREATE_NO_WINDOW, nullptr, nullptr,
				&sStartInfo, &sProcessInfo);
			if (!nSuccess)
			{
				AfxMessageBox("Unable to launch Test Automation Editor.", MB_ICONSTOP | MB_OK);
			}
		}
	}
	catch (...)
	{
	}
}

bool CMainFrame::bWaitForNSCodeGenStatus(ETYPE_BUS eBusType)
{
    DWORD result = WaitForSingleObject( m_NSCodeGenThreads[eBusType], 0);
    CWaitIndicator ouWaitIndicator;

    if ( result == WAIT_FAILED || result == WAIT_OBJECT_0)
    {
        return false;
    }
    ouWaitIndicator.DisplayWindow("Please Wait.. Dependency files are getting generated.", this);

    do
    {

        result = WaitForSingleObject( m_NSCodeGenThreads[eBusType], 500);

    }
    while ( result != WAIT_OBJECT_0  );

    return true;
}


LRESULT CMainFrame::OnNodeSimCodeGenStatus(WPARAM ouWparam, LPARAM oulParam)
{
    static int nCurrentThreads = 0;
    ETYPE_BUS eBus = (ETYPE_BUS)ouWparam;
    bool bStatusCompleted = (oulParam != 0);

    bStatusCompleted ? nCurrentThreads-- : nCurrentThreads++;

    CString omStrPage = "";
    if ( nCurrentThreads > 0 )
    {
        omStrPage = "Dependency Files are Getting Generated...";
    }
    m_wndStatusBar.SetPaneText ( 0, omStrPage);

    return S_FALSE;
}


DWORD WINAPI CMainFrame::NSCodeGenerationThread(LPVOID pVoid)
{
    NSCodeGenThreadParam ouThreadParam = *((NSCodeGenThreadParam*)pVoid);
    //We should use mainframe handle instead of pointer
    ouThreadParam.m_pMainFrame->PostMessage(WM_USER_NSCODEGEN, ouThreadParam.m_eBusType, false);
    ouThreadParam.m_pMainFrame->GenerateNodeSimObjFiles(ouThreadParam.m_eBusType);
    ouThreadParam.m_pMainFrame->PostMessage(WM_USER_NSCODEGEN, ouThreadParam.m_eBusType, true);

    ouThreadParam.m_pMainFrame->mPluginManager->notifyPlugins(eBusmaster_Event::network_config_changed, &ouThreadParam.m_eBusType);

    return 0;
}
void CMainFrame::GenerateNodeSimObjFiles(ETYPE_BUS eBusType)
{
    if (nullptr != m_ouBusmasterNetwork)
    {
        int nChannels = 0;

        ICluster* pCluster = nullptr;
        m_ouBusmasterNetwork->GetChannelCount(eBusType, nChannels);

        for (int i = 0; i < nChannels; i++)
        {
            int nDBCount = 0;
            m_ouBusmasterNetwork->GetDBServiceCount(eBusType, i, nDBCount);
            for (int nDBIndex = 0; nDBIndex < nDBCount; nDBIndex++)
            {
                m_ouBusmasterNetwork->GetDBService(eBusType, i, nDBIndex, &pCluster);
                if (nullptr != pCluster)
                {
                    //m_ouBusmasterNetwork->GenerateCode(pCluster);
                    NodeSimCodeGenerator ouCodeGenrator;
                    NSCodeGenHelperFactory helperFactory;
                    ouCodeGenrator.CompileCode(pCluster, false, true, helperFactory.GetNsCodeGenHelper(eBusType));
                }
            }
        }
    }
}


void CMainFrame::OnClusterChanged(ETYPE_BUS eBusType)
{
    DWORD result = WaitForSingleObject( m_NSCodeGenThreads[eBusType], INFINITE);

    if (result == WAIT_OBJECT_0 || result == WAIT_FAILED)
    {
        NSCodeGenThreadParam* ouNSCodeGenThreads = new NSCodeGenThreadParam();
        ouNSCodeGenThreads->m_pMainFrame = this;
        ouNSCodeGenThreads->m_eBusType = eBusType;
        m_NSCodeGenThreads[eBusType] = CreateThread(nullptr, 0, NSCodeGenerationThread, ouNSCodeGenThreads, 0, nullptr);
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


LRESULT CMainFrame::onGetConfigPath(WPARAM wParam, LPARAM /* lParam */)
{
    char* pchPath = (char*) wParam;

    if(pchPath != nullptr)
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
            mPluginManager->notifyPlugins(eBusmaster_Event::key_pressed, &(pMsg->wParam));

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
                vManageOnKeyHandler(GetICANNodeSim(),unKey);
                vManageOnKeyHandler(GetIJ1939NodeSim(),unKey);
                vManageOnKeyHandler(GetILINNodeSim(),unKey);
                mPluginManager->notifyPlugins(eBusmaster_Event::key_pressed, &(unKey));

            }

        }
        // Execute key hanlder only if execution is selected by user

    }
    if (pMsg->message == WM_KEYDOWN)
    {
        CFlags* pouFlag = nullptr;
        pouFlag     = theApp.pouGetFlagsPtr();
        if(pouFlag != nullptr)
        {
            BOOL bConnected = FALSE;
            BOOL bConnectedLIN = FALSE;
            // Get the current status of Connected/Disconnected state
            bConnected  = pouFlag->nGetFlagStatus(CONNECTED);
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
void CMainFrame::vManageOnKeyHandler(CBaseNodeSim* pBaseNodeSim, UINT unKey)
{
    if(nullptr != pBaseNodeSim)
    {
        pBaseNodeSim->NS_ManageOnKeyHandler(unKey);
    }
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
    m_objTxHandler.vShowConfigureMsgWindow(this, LIN);
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
    if ( (pouFlag!= nullptr) && (TRUE == pouFlag->nGetFlagStatus(LIN_CONNECTED)))
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
    CStringArray strFilePathArray;

    CChannelConfigurationDlg ouConfig( m_ouBusmasterNetwork, LIN, nullptr);

    if ( ouConfig.DoModal() == IDOK)
    {
        m_ouBusmasterNetwork->SetChannelCount(LIN, 1);

        m_objTxHandler.SetNetworkConfig(LIN, m_ouBusmasterNetwork);

        vPopulateIDList(LIN); //For Message window configuration
        //Generate unions.h file.
        OnClusterChanged(LIN);
    }
}


void CMainFrame::SaveConfigDataToXML( SCONTROLLER_DETAILS& controller, xmlNodePtr pNodePtr )
{
    double fBaudRate = atof( controller.m_omStrBaudrate.c_str() );
    // if( m_omHardwareDesc.find("Vector") == -1)      //if its not VECTOR then convert to Kbps
    {
        fBaudRate = fBaudRate / 1000;    //convert to Kbps before saving to XML
    }
    std::stringstream ss;
    ss << fBaudRate;
    std::string omStrBaudrate = ss.str();
    const char* strBaudRate = omStrBaudrate.c_str();
    const char* strCNF1 = controller.m_omStrCNF1.c_str();
    const char* strCNF2 = controller.m_omStrCNF2.c_str();
    const char* strCNF3 = controller.m_omStrCNF3.c_str();
    const char* strVar;
    const char* strLocation = controller.m_omStrLocation.c_str();

    xmlNewChild( pNodePtr, nullptr, BAD_CAST "BaudRate", BAD_CAST strBaudRate );
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "CNF1", BAD_CAST strCNF1 );
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "CNF2", BAD_CAST strCNF2 );
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "CNF3", BAD_CAST strCNF3 );

    strVar = controller.m_omStrBTR0.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "BTR0", BAD_CAST strVar );

    strVar = controller.m_omStrBTR1.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "BTR1", BAD_CAST strVar );

    strVar = controller.m_omStrClock.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "Clock", BAD_CAST strVar );

    strVar = controller.m_omStrSamplePercentage.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "SamplePerc", BAD_CAST strVar );

    strVar = controller.m_omStrSampling.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "Sampling", BAD_CAST strVar );

    std::stringstream stream16a;
    std::string strBTLCycles;
    stream16a << controller.m_unBTL_Cycles;
    strBTLCycles = stream16a.str();
    strVar = strBTLCycles.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "BTLCycles", BAD_CAST strVar );

    strVar = controller.m_omStrWarningLimit.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "Warning_Limit", BAD_CAST strVar );

    strVar = controller.m_omStrPropagationDelay.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "Propagation_Delay", BAD_CAST strVar );

    strVar = controller.m_omStrSjw.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "SJW", BAD_CAST strVar );

    strVar = controller.m_omStrAccCodeByte1[0].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccCodeByte1_0", BAD_CAST strVar );

    strVar = controller.m_omStrAccCodeByte1[1].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccCodeByte1_1", BAD_CAST strVar );

    strVar = controller.m_omStrAccCodeByte2[0].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccCodeByte2_0", BAD_CAST strVar );

    strVar = controller.m_omStrAccCodeByte2[1].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccCodeByte2_1", BAD_CAST strVar );

    strVar = controller.m_omStrAccCodeByte3[0].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccCodeByte3_0", BAD_CAST strVar );

    strVar = controller.m_omStrAccCodeByte3[1].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccCodeByte3_1", BAD_CAST strVar );

    strVar = controller.m_omStrAccCodeByte4[0].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccCodeByte4_0", BAD_CAST strVar );

    strVar = controller.m_omStrAccCodeByte4[1].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccCodeByte4_1", BAD_CAST strVar );

    //------------------------
    strVar = controller.m_omStrAccMaskByte1[0].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccMaskByte1_0", BAD_CAST strVar );

    strVar = controller.m_omStrAccMaskByte1[1].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccMaskByte1_1", BAD_CAST strVar );

    strVar = controller.m_omStrAccMaskByte2[0].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccMaskByte2_0", BAD_CAST strVar );

    strVar = controller.m_omStrAccMaskByte2[1].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccMaskByte2_1", BAD_CAST strVar );

    strVar = controller.m_omStrAccMaskByte3[0].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccMaskByte3_0", BAD_CAST strVar );

    strVar = controller.m_omStrAccMaskByte3[1].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccMaskByte3_1", BAD_CAST strVar );

    strVar = controller.m_omStrAccMaskByte4[0].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccMaskByte4_0", BAD_CAST strVar );

    strVar = controller.m_omStrAccMaskByte4[1].c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccMaskByte4_1", BAD_CAST strVar );

    strVar = controller.m_omHardwareDesc.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "HardwareDesc", BAD_CAST strVar );

    std::string strData;

    std::stringstream stream;
    stream << controller.m_nItemUnderFocus;
    strData = stream.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "ItemUnderFocus", BAD_CAST strVar );

    std::stringstream stream1;
    stream1 << controller.m_nBTR0BTR1;
    strData = stream1.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "BTR0BTR1", BAD_CAST strVar );

    std::stringstream stream2;
    stream2 << controller.m_bAccFilterMode;
    strData = stream2.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "AccFilterMode", BAD_CAST strVar );

    std::stringstream stream3;
    stream3 << controller.m_ucControllerMode;
    strData = stream3.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "ControllerMode", BAD_CAST strVar );

    std::stringstream stream4;
    stream4 << controller.m_bSelfReception;
    strData = stream4.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "SelfReception", BAD_CAST strVar );

    std::stringstream stream5;
    stream5 << (int)controller.m_enmHWFilterType[0];
    strData = stream5.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "HWFilterType_0", BAD_CAST strVar );

    std::stringstream stream6;
    stream6 << (int)controller.m_enmHWFilterType[1];
    strData = stream6.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "HWFilterType_1", BAD_CAST strVar );

    std::stringstream stream7;
    stream7 << controller.m_bDebug;
    strData = stream7.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "Debug", BAD_CAST strVar );

    std::stringstream stream8;
    stream8 << controller.m_bPassiveMode;
    strData = stream8.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "PassiveMode", BAD_CAST strVar );

    std::stringstream stream9;
    stream9 << controller.m_bHWTimestamps;
    strData = stream9.str();

    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "HWTimestamps", BAD_CAST strVar );

    xmlNewChild( pNodePtr, nullptr, BAD_CAST "Location", BAD_CAST strLocation );

    std::stringstream stream10;
    stream10 << controller.m_bLowSpeed;
    strData = stream10.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "LowSpeed", BAD_CAST strVar );

    std::stringstream stream11;
    stream11 << controller.m_unDataBitRate;
    strData = stream11.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "CANFD_BaudRate", BAD_CAST strVar );

    std::stringstream stream12;
    stream12 << controller.m_unDataSamplePoint;
    strData = stream12.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "CANFD_SamplePoint", BAD_CAST strVar );

    std::stringstream stream13;
    stream13 << controller.m_unDataBTL_Cycles;
    strData = stream13.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "CANFD_BTLCycles", BAD_CAST strVar );

    std::stringstream stream14;
    stream14 << controller.m_unDataSJW;
    strData = stream14.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "CANFD_SJW", BAD_CAST strVar );

    std::stringstream stream15;
    stream15 << controller.m_bTxDelayCompensationControl;
    strData = stream15.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "CANFD_TxDelayCompensation", BAD_CAST strVar );

    std::stringstream stream16;
    stream16 << controller.m_unTxSecondarySamplePointOffset;
    strData = stream16.str();
    strVar = strData.c_str();
    xmlNewChild( pNodePtr, nullptr, BAD_CAST "CANFD_TxSecondarySamplePoint", BAD_CAST strVar );

    
}
void CMainFrame::vInitialize( SCONTROLLER_DETAILS& controller, BOOL bUpdateHWDesc )
{
    // The default baudrate is taken as 500 kbps
    controller.m_nItemUnderFocus = 64;
    controller.m_nBTR0BTR1 = 49210;
    controller.m_omStrCNF1 = "7";
    controller.m_omStrCNF2 = "B8";
    controller.m_omStrCNF3 = "5";
    controller.m_omStrBTR0 = "C0";
    controller.m_omStrBTR1 = "3A";
    controller.m_omStrBaudrate = "500000";
    controller.m_omStrClock = "16";
    controller.m_omStrSamplePercentage = "75";
    controller.m_omStrSampling = "1";
    controller.m_omStrWarningLimit = "96";
    controller.m_omStrPropagationDelay = "ALL";
    controller.m_omStrSjw = "4";
    controller.m_omStrAccCodeByte1[0] = "0";
    controller.m_omStrAccCodeByte2[0] = "0";
    controller.m_omStrAccCodeByte3[0] = "0";
    controller.m_omStrAccCodeByte4[0] = "0";
    controller.m_omStrAccCodeByte1[1] = "0";
    controller.m_omStrAccCodeByte2[1] = "0";
    controller.m_omStrAccCodeByte3[1] = "0";
    controller.m_omStrAccCodeByte4[1] = "0";
    controller.m_omStrAccMaskByte1[0] = "FF";
    controller.m_omStrAccMaskByte2[0] = "FF";
    controller.m_omStrAccMaskByte3[0] = "FF";
    controller.m_omStrAccMaskByte4[0] = "FF";
    controller.m_omStrAccMaskByte1[1] = "FF";
    controller.m_omStrAccMaskByte2[1] = "FF";
    controller.m_omStrAccMaskByte3[1] = "FF";
    controller.m_omStrAccMaskByte4[1] = "FF";

    if ( bUpdateHWDesc )
    {
        controller.m_omHardwareDesc = "";
    }
    controller.m_bAccFilterMode = FALSE;
    controller.m_ucControllerMode = 0x1;
    controller.m_enmHWFilterType[0] = HW_FILTER_ACCEPT_ALL;
    controller.m_enmHWFilterType[1] = HW_FILTER_ACCEPT_ALL;
    controller.m_bSelfReception = TRUE;
    controller.m_bLowSpeed = FALSE;
    controller.m_unBTL_Cycles = 10;

    /* CAN FD related parameters */
	controller.m_bcanFDEnabled = FALSE;
	controller.m_bSupportCANFD = FALSE;
    controller.m_unDataBitRate = 2000000;
    controller.m_unDataSamplePoint = 70;
    controller.m_unDataBTL_Cycles = 10;
    controller.m_unDataSJW = 03;
    controller.m_bTxDelayCompensationControl = 0;   // OCI_CANFD_TX_DELAY_COMPENSATION_OFF
    controller.m_unTxSecondarySamplePointOffset = 0;
    
}

void CMainFrame::vLoadBusmasterKernel()
{
    getBusmasterKernel( &mBusmasterKernel );
    mBusmasterKernel->getDatabaseService( &m_ouBusmasterNetwork );
    //return S_OK;
}
int CMainFrame::getDilService( ETYPE_BUS bus, IBusService** busService )
{
    return mBusmasterKernel->getBusService( bus, busService );
}
int CMainFrame::getDbService( IBMNetWorkGetService** dbService )
{
    *dbService = m_ouBusmasterNetwork;
    return S_OK;
}
int CMainFrame::getDbSetService(IBMNetWorkService** dbService)
{
    *dbService = m_ouBusmasterNetwork;
    return S_OK;
}
int CMainFrame::notifyPlugins(eBusmaster_Event event, void* data)
{
    return mPluginManager->notifyPlugins(event, data);
}
int CMainFrame::displayMessage( char* message )
{
    if (nullptr != m_podUIThread)
    {
        CString omStr(message);
        m_podUIThread->vAddString(omStr);
    }

    return S_OK;
}

HWND CMainFrame::getBusmasterHandle()
{
    return GetSafeHwnd();
}

int CMainFrame::getInfo(int infoType, EXT_INFO_PARAM, /*IN, OUT*/ OUT_INFO_PARAM infoData)
{
    switch ( infoType )
    {
        case busmaster_window:
        {
            *( (HWND*)infoData ) = GetSafeHwnd();
        }
        break;
		case 6:
		{
			*((HWND*)infoData) = m_hWndMDIClient;
		}
		break;
        case busmaster_version:
        {
            char* retVal = (char*)infoData;
            CString str;
            str.LoadString(IDS_VERSION);
            strcpy_s(retVal, 25, str.GetBuffer(0));
        }
        break;
        case numeric_mode:
        {
            eNumeric_mode* retVal = (eNumeric_mode*)infoData;
            if (nullptr != retVal)
            {
                *retVal = eNumeric_mode::DECICAL_MODE;
                if (TRUE == theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX))
                {
                    *retVal = eNumeric_mode::HEXADECIMAL_MODE;
                }
            }
        }
        break;
        case busmaster_install_path:
        case busmaster_default_path:
        {
            provideBusmasterConfiguredPaths(infoType, (char*)infoData);
        }
        break;
    }
    return S_OK;
}
void CMainFrame::provideBusmasterConfiguredPaths(int pathType, char* infoData)
{
    std::string path;
    switch (pathType)
    {
        case busmaster_install_path:
        {
            GetBusmasterInstalledFolder(path);
        }
        break;
        case busmaster_default_path:
        {
            GetCurrentVerBusMasterUserDataPath(path);
        }
        break;
    }
    char* retVal = (char*)infoData;
    strcpy_s(retVal, 1024, path.c_str());
}
int CMainFrame::getPluginConnectionPoint(const char* pluginId, IBusmasterPluginConnection** connectionPoint)
{
    *connectionPoint = mPluginManager->getPluginConnectionPoint(pluginId);
    if (*connectionPoint == nullptr)
    {
        return S_FALSE;
    }
    return S_OK;
}
int CMainFrame::getVariableCommunicationLayer(IVariableLayer** variableLayer)
{
	*variableLayer = &mVariableLayer;
	return S_OK;
}
afx_msg void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;
	switch (id)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;
	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;
	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;
	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;
	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;
	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;
	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;
	default:
		switch (id)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;
		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;
		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;
		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
}
//plugin

void CMainFrame::onPluginMenuClicked(UINT menuId)
{
    if( S_FALSE == mPluginManager->noifyMenuClick( menuId ))
    {
        //TODO::Display Error Info
    }
}
void CMainFrame::onPluginMenuUpadate( CCmdUI* pCmdUI )
{
    auto buttonCmd = dynamic_cast<CMFCRibbonCmdUI*>(pCmdUI);
    if (buttonCmd != nullptr)
    {
        CMFCRibbonButtonEx* btn = dynamic_cast<CMFCRibbonButtonEx*>(buttonCmd->m_pUpdated);
        if (nullptr != btn)
        {
            BusmasterMenuItem menuItem(btn, pCmdUI);
            mPluginManager->noifyMenuUpdate(pCmdUI->m_nID, &menuItem);
        }
        else
        {
            CMFCRibbonButtonEx* btn = dynamic_cast<CMFCRibbonButtonEx*>(buttonCmd->m_pUpdated->GetOriginal());
            if (nullptr != btn)
            {
                BusmasterMenuItem menuItem(btn, pCmdUI);
                mPluginManager->noifyMenuUpdate(pCmdUI->m_nID, &menuItem);
            }
        }
    }
}

void CMainFrame::setConnectStateJ1939(bool connected)
{
	CMFCRibbonCategory* canCategory = GetCategory("J1939");

	if (canCategory == nullptr) return;
	auto panel = GetRibbonPanel(canCategory, "Activate");
	if (nullptr == panel) return;
	int count = panel->GetCount();
	auto control = static_cast<CMFCRibbonButton*>(panel->GetElement(0));
	if (connected == true)
	{
		control->SetImageIndex(19, TRUE);
		control->SetImageIndex(3, FALSE);
		control->SetText("Deactivate");
	}
	else
	{
		control->SetImageIndex(18, TRUE);
		control->SetImageIndex(0, FALSE);
		control->SetText("Activate");
	}
	mRibbonBar.ForceRecalcLayout();
}
void CMainFrame::setConnectState(ETYPE_BUS busType, bool connected)
{
	CMFCRibbonCategory* canCategory = GetCategory(getBusInString(busType).c_str());
	
	if (canCategory == nullptr) return;
	auto panel = GetRibbonPanel(canCategory, "Hardware Configuration");
	if (nullptr == panel) return;
	int count = panel->GetCount();
	auto control = static_cast<CMFCRibbonButton*>(panel->GetElement(0));
    CString text;
    if (connected == true)
	{
		control->SetImageIndex(11, TRUE);
		control->SetImageIndex(3, FALSE);
        text.LoadString(IDS_DISCONNECT);
        control->SetText((LPCSTR)text);
	}
	else
	{
		control->SetImageIndex(0, TRUE);
		control->SetImageIndex(2, FALSE);
        text.LoadString(IDS_CONNECT);
        control->SetText((LPCSTR)text);
	}
	mRibbonBar.ForceRecalcLayout();
}

LRESULT CMainFrame::OnAutoItRequest(WPARAM wparam, LPARAM lparam)
{
	//MessageBox("Request");
	if (wparam == 0)
	{
		if (lparam != theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX))
		{
			OnHex_DecButon();
		}
		return wparam;
	}
	return 1;
}

LRESULT CMainFrame::OnRibbonCustomize(WPARAM lparam, LPARAM wparam)
{
    return 0;
}


void CMainFrame::OnButtonToggleribbon()
{
    mRibbonBar.ToggleWindowDisplay();

    
}
int CMainFrame::GetLicenseDetails(std::string strAddOn, CLicenseDetails &licenseDetails)
{
	if (nullptr != mPluginManager)
	{
		return mPluginManager->getLicenseDetails(strAddOn, licenseDetails);
	}	
}
// This function is used to import a license from About Box
int CMainFrame::ImportLicense()
{
	int nResult = -1;
	CFileDialog fileDlg(TRUE, "lic", nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_("License File(s)(*.lic)|*.lic||"), nullptr);

	// Set Title
	fileDlg.m_ofn.lpstrTitle = _("Select License File...");

	if (fileDlg.DoModal() == IDOK)
	{
		std::string strLicenseFile = fileDlg.GetPathName();
		std::string strLicenseFileExt = fileDlg.GetFileExt();

		if (strLicenseFileExt != "lic")
		{
			AfxMessageBox("Invalid license file format");
			return -1;
		}

		if (TRUE == PathFileExists(strLicenseFile.c_str()))
		{
			LONG lError = 0;
			HKEY hKey;
			BYTE acRegValue[1024] = { 0 };
			DWORD dwSize = sizeof(BYTE[1024]);

			HKEY hRootKey = HKEY_LOCAL_MACHINE;
			CString strSubKey = "";
			CString strName = "Install_Dir";
			DWORD dwType = REG_SZ;
			CString strValue = "";
			DWORD dwValue = 0;

			CString strCompleteSubKey;
			strCompleteSubKey.Format("Software\\BUSMASTER_v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
			strValue = "";
			lError = RegOpenKeyEx(hRootKey, strCompleteSubKey, 0, KEY_READ, &hKey);
			// If the registry key open successfully, get the value in "path"
			// sub key
			if (lError == ERROR_SUCCESS)
			{
				if (dwType == REG_SZ)
				{
					lError = RegQueryValueEx(hKey, strName, 0, &dwType, acRegValue, &dwSize);
					strValue.Format("%s", acRegValue);
				}

				RegCloseKey(hKey);
			}

			BOOL bStatus = FALSE;
			strLicenseFile += '\0';
			strValue += "\0";
			SHFILEOPSTRUCT fileInfo;
			fileInfo.hwnd = this->m_hWnd;
			fileInfo.wFunc = FO_COPY;
			fileInfo.pFrom = strLicenseFile.c_str();
			fileInfo.pTo = strValue.GetBuffer();

			fileInfo.fFlags = FOF_NOCONFIRMATION;
			fileInfo.fAnyOperationsAborted = bStatus;

			if (strValue != "" && TRUE == PathFileExists(strLicenseFile.c_str()))
			{
				int nRetVal = SHFileOperation(&fileInfo);
				if (nRetVal != 0)
				{
					MessageBox("Importing License file failed", "BUSMASTER", MB_OK | MB_ICONERROR);
					nResult = - 1;
				}
				else
				{
					//MessageBox("License file is copied successfully.\nRestart BUSMASTER to start using Add-On features.", "BUSMASTER", MB_OK | MB_ICONINFORMATION);
					nResult = 0;
				}
				if (fileInfo.fAnyOperationsAborted)
				{
					nResult = -1;
				}
			}
		}
	}

	return nResult;
}
