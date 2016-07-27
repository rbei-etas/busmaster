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
 * \file      MainFrm.h
 * \brief     This file contain the definition of CMainFrame class
 * \authors   RBIN/EMC2 - Amarnath Shastry, Amitesh Bharti, Ratnadip Choudhury, Ravikumar Patil, Anish kumar, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CMainFrame class
 */

#pragma once


//libxml file includes


#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "Flags.h"                  // Definition of CFlags class
#include "SectionNames.h"           // Different section names and section ID
#include "MsgSignalDBWnd.h"         // Database editor window defintion file
#include "MsgSgTreeView.h"          // Left pane for DB editor definition file
#include "MsgSgDetView.h"           // Right pane for DB editor definition file
#include "MsgSignal.h"              // Msg-Sig data structure definition file
#include "SignalWatch/SignalWatch_extern.h" // Signal watch DLL
#include "struct.h"                 // Structure definition
#include "Utility/RadixEdit.h"              // For the RAdix Edit control definition
#include "Utility/ComboItem.h"              // For Custom Combobox Implementation
#include "Utility/EditItem.h"               // For Custom Editbox Implementation
#include "Utility/numSpinCtrl.h"            // For the custom spin control
#include "Utility/NumEdit.h"                // For Custom Numeric Edit control Impl
#include "Utility/FFListctrl.h"             // For Flicker Free List class definition
#include "Utility/NumSpinCtrl.h"        // For the custom spin control
#include "Utility/flexlistctrl.h"           // For editable list control implementation
#include "BusStatisticsDlg.h"       // BusStatisticsDlg Dialog class for bus 
#include "LINBusStatisticsDlg.h"
// statistics
#include "GraphParameters.h"        // For Graph Parameter Class
#include "GraphElement.h"           // For Graph Element class
#include "GraphList.h"              // For CGraphList class declaration
//#include "GraphChildframe.h"        // For Graph Child Window
////#include "GraphUIThread.h"          // For Graph UI Thread Definition

#include "UIThread.h"               // For Trace window UI Thread definition
#include "Utility/FlexListCtrl.h"   // For editable list control implementation
#include "Utility/Utility_Thread.h"
#include "DataTypes/J1939_Datatypes.h"  //J1939 related structures
#include "WrapperErrorLogger.h" //For Logging Error in DIL Interface 
#include "CFilesDefs_CAN.h"
//For node simulation interface
#include "NodeSimEx/BaseNodeSim.h"
#include "NodeSimEx/NodeSimEx_Extern.h"

#include "MsgWndThread.h"
#include "CNVTCToolBar.h"
#include "ImportLogToolbar.h"
#include "FlexTxHandler.h"
#include "WaveFormDataHandler.h"
#include "WaveformTransmitter.h"
#include "TxMsgWndJ1939.h"
#include "SigGrphWndHandler.h"
#include "MsgBufVSE.h"
#include "ConfigData.h"
#include "NetworkStatistics.h"
#include "BusStatisticLIN.h"
#include "TSExecutorHandler.h"
#include "TSEditorHandler.h"
#include "Utility/XMLUtils.h"
#include "IBMNetWorkService.h"
#include "IBusmasterPluginInterface.h"
#include "frChannelSettingsHandler.h"

#include "IBusmasterPlugInManager.h"
#define WM_SAVE_DBJ1939        (WM_USER + 108)
#define WM_USER_NSCODEGEN      (WM_USER + 107)



// Forward reference to Message Child Window
class CMsgMDIChildWnd;

class CMainFrame : public CMDIFrameWnd ,
    public IBusmasterPluginInterface
{
    DECLARE_DYNAMIC(CMainFrame)
public:
    CMainFrame();

    // Attributes
    HANDLE m_hProcess;
    // Flag specifies whether to enable
    INTERFACE_HW_LIST m_asINTERFACE_HW;
    //controller mode
    BYTE m_byControllerMode;
    DWORD m_dwDriverId;
    SHORT m_shFLEXRAYDriverId;
    SHORT m_shLINDriverId;
    WrapperErrorLogger m_ouWrapperLogger;
    // send toolbar button
    //bool m_bEnableSendToolbarButton;
    // MRU list
    CString m_omStrMRU_ConfigurationFiles[6];

    // Pointer to new Simulated system array Struct
    PSSIMSYSARRAY m_psSimSysArray;
    //CSimSysNodeInfo::PSSIMSYSINFO m_psSimSysInfo;

    // Holds opened document name
    CString m_omStrSourceFilePathName;
    // Holds database name for CAN
    CString m_omStrDatabaseName;
    // Holds database name for J1939
    CString m_omJ1939DBName;
    // Holds the timer delay in msec for the replay
    UINT m_unReplayTimeDelay;
    // Holds the time delay in msec for the replay cycle
    UINT m_unCycleTimeDelay;
    //Notification window (Trace window) object pointer
    CUIThread* m_podUIThread;
    // Flag to indicate that notific window is created.
    bool    m_bNotificWndVisible ;
    //flag whether the current hardware is available or not
    bool    m_bNoHardwareFound;
    // Graph Control data members
    // Pointer to Graph List
    CGraphList         m_odGraphList[MAX_PROTOCOL];
    // Flag to Indicate Graph Window status
    bool                m_bGraphWindowVisible;

    TSEditorHandler m_objTSEditorHandler;
    TSExecutorHandler m_objTSExecutorHandler;

    CMsgWndThread* m_podMsgWndThread;
    bool m_bInterPretMsg;

    CFlexTxHandler  m_objFlexTxHandler;

    CSigGrphHandler m_objSigGrphHandler;
    S_EXFUNC_PTR    m_sExFuncPtr[BUS_TOTAL];
    CTxMsgWndJ1939* m_pouTxMsgWndJ1939;
    SJ1939CLIENTPARAM m_sJ1939ClientParam;
    CNetworkStatistics* m_oNetworkStatistics;
    CBusStatisticsDlg* m_bsCAN;
    CLINBusStatisticsDlg* m_bsLIN;

    bool CompareFile(CString FirstFile,CString SecFile);

    int             m_nNumChannels;
    int             m_nNumChannelsLIN;
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainFrame)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    //}}AFX_VIRTUAL

    // Implementation
public:

    void vGettextBusmaster();
    HICON m_hLogIcon1, m_hLogIcon2, m_hLogOffIcon;
    int m_nIconSetFlag;
    int m_nJ1939IconSetFlag;
    int m_nLinIconSetFlag;
    int m_nFlexRayIconSetFlag;

    INT m_nSendMsgLogCnt;
    INT m_nSendMsgJ1939LogCnt;
    INT m_nSendMsgFlexRayLogCnt;
    CWaveformTransmitter m_ouWaveTransmitter;
    USHORT vCheckValidLogFiles(USHORT iCount);
    bool bIsAtleastOneLoggingBlockEnabled(USHORT LogBlocks);


    USHORT vCheckValidLogFiles_LIN(USHORT iCount);
    bool bIsAtleastOneLoggingBlockEnabled_LIN(USHORT LogBlocks);

    //Get Message Window Thread
    inline CMsgWndThread* pGetMessageWndThread()
    {
        return m_podMsgWndThread;
    }
    //To initialize DIL
    HRESULT IntializeDIL(UINT unDefaultChannelCnt = 0);

    //To initialize LIN DIL
    HRESULT IntializeDILL(UINT unDefaultChannelCnt = 0);
    //To initialize FLEXRAY DIL
    HRESULT InitializeFLEXRAYDIL();
    DWORD m_dwFLEXClientID;



    //BMClusterConfig m_ouBmClusterConfig[BUS_TOTAL];

    //Generalised New Structures.


    INT nsize;
    bool m_bFRConfigModified;



    /* Buffer for FLEXRAY */
    INTERFACE_HW_LIST m_sSelFlxHwInterface;
    // To Create Graph UI thread and graph Window
    bool bCreateGraphWindow();
    // Function to club activities needs to be done after conf load.
    void vClearOnConfLoad();
    // Function to init controller after configuration load
    void vSetControllerParameters();

    // Function to clear Simulated system List
    void vEmptySimsysList();

    void vCreateMRU_Menus();

    bool bWriteIntoRegistry(HKEY hRootKey, CString strSubKey, CString strName,  BYTE bytType, CString strValue = "" , DWORD dwValue = 0);
    // Delete memory allocated by signal watch
    void vFreeSignalWatchMemorySpace();

    // Gets pointer to specified signal
    sSIGNALS* poGetSignalPointer( bool, const UINT& unMsgID,
                                  const CString& omStrSignalName);

    void vSetMessageData(BYTE*  pbMessageData);
    // Creates message window
    bool bCreateMsgWindow(void);

    //Create toolbar nodes
    //bool CreationOfToolBarNode(xmlNodePtr pNodePtr, string strTag, string strData);
    //bool CreationOfToolBarNode(xmlNodePtr pNodePtr, string strTag, int nData);
    // Returns reference to CByteArray
    CByteArray& pomGetMsgByteArrayReference();
    // Fills the tool bar combo box with message names

    bool bIsHexNumber(CString omStrHexNumber);
    // COnvert integer to hex
    CString omStrConvertIntegerToHex( CString omStrInt );
    // Set new database indication flag
    void vSetNewDatabaseFlag(bool bValue);
    // Get new database indication flag
    bool vGetNewDatabaseFlag();
    // Return reference to toolbar control
    CToolBarCtrl& vGetReferenceToToolBarCtrl();
    // Convert string to byte array
    void vConvStrtoByteArray(CByteArray*, char*);
    // Convert byte array to string
    // Convert string in hex to integer
    __int64 nConvertStringToInt( CString omStrHexNo);
    // Set pointer to CMsgSgDetView class
    void podSetMsgSgDetView(CMsgSgDetView* pDetView, ETYPE_BUS eBus);
    // Set pointer to CMsgSgTreeView class
    void podSetMsgSgTreeView(CMsgSgTreeView* pTreeView, ETYPE_BUS eBus);
    // Get pointer to CMsgSgTreeView class
    CMsgSgTreeView* podGetMsgSgTreeView(ETYPE_BUS eBus);
    // Get pointer to CMsgSgDetView class
    CMsgSgDetView* podGetMsgSgDetView(ETYPE_BUS eBus);

    // Get pointer to CMsgSignalDBWnd class
    CMsgSignalDBWnd* pomGetMsgSgWnd();

    // Get pointer to CSimSysWnd class
    //CSimSysWnd* pomGetSimSysWnd();

    virtual ~CMainFrame();
    //Called by wrapper function to Start\Stop logging$Log:$Log:
    bool bEnableDisableLog(bool bStart) ;
    //Called by wrapper function to Log a string $Log:$Log:
    bool bWriteToLog(char* pcOutStrLog) ;
    //Called by wrapper function to change the controller mode $Log:$Log:
    bool bSetControllerMode(bool bMode) ;
    //To display the context menu when user selects the time mode drop down menu
    void vToolBarDropDownMenu( UINT unControlID, int nButtonIndex);
    // To create the trace window
    bool bCreateTraceWindow();
    DWORD dLoadJ1939DBFile(CString omStrActiveDataBase, bool bFrmCom);
    // To load specified configuration File
    DWORD dLoadDataBaseFile(CString omStrActiveDataBase, bool bFrmCom);
    // To clear Message Interpretation Window Content on change of Config. File
    //void vClearInterpretationWindow();
    // To get the menu pointer of the MRU list
    CMenu* pomGetMRUMenuPointer();

    // To get Dll path of recent compiled file
    CString omGetDllName();
    //Calling build and load from com interface function also
    bool bDLLBuildLoad(CStringArray* omStrBuildFiles);
    bool bDLLBuild(CStringArray* omStrBuildFiles) ;
    bool bDllLoad(CStringArray* omStrBuildFiles) ;
    bool bDllUnload(CStringArray* omStrBuildFiles) ;
    // To stop or start logging during configuration change
    inline void vStartStopLogging(bool bStart);
    inline void vStartStopLogging_LIN(bool bStart);

    // To stop or start logging during configuration change
    inline void vJ1939StartStopLogging();

    // To set the associated database file names for logging
    void vSetAssociatedDatabaseFiles(ETYPE_BUS eBus);
    // To set the baudrate for the selected channels
    void vSetBaudRateInfo(ETYPE_BUS eBus);
    //Wrapper function around the inline function,to be called from com function
    void vComStartStopLog(bool bStart);
    void vComStartStopLog_LIN(bool bStart);
    //Function takes the Root Menu and returns the SubMenu Pointer
    CMenu* GetSubMenu(CString MenuName);

    bool bInitFrameProcCAN(void);
    bool bInitFrameProcLIN(void);

    DWORD dwGetMonitorNodeClientID();

    CWnd* IsWindowCreated();
    void vCloseFormatconverters();
    void vProcessKeyPress(MSG* pMsg);

    void OnHex_DecButon();

    void bSetHexDecFlags(bool bHexOn);

    /* API to modify icon for a particular item in toolbar */
    void vModifyToolbarIcon(CNVTCToolBar& objToolbar, BYTE bytItemIndex, bool bItemON, UINT nTBIDON, UINT nTBIDOFF);
    /* API to set icon for a particular item in toolbar */
    void vSetToolBarIcon(CNVTCToolBar& objToolbar, BYTE bytItemIndex, UINT nTBIDNormal, UINT nTBIDHot, UINT nTBIDDisabled);
    /* API to set toolbar button size */
    void vSetToolbarButtonSize(CNVTCToolBar& objToolbar, CSize& objSize);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    // control bar embedded members
    CStatusBar      m_wndStatusBar; // Status bar
    CNVTCToolBar    m_wndToolBar;   // Tool bar/*WrapFixed*/
    CNVTCToolBar    m_wndToolbarMsgWnd;
    CNVTCToolBar    m_wndToolbarConfig;
    CNVTCToolBar    m_wndToolbarJ1939;
    CNVTCToolBar    m_wndToolbarCANDB;
    CNVTCToolBar    m_wndToolbarFlexRay;
    CNVTCToolBar    m_wndToolbarConfiguration;
    CNVTCToolBar    m_wndToolbarLIN;

    // Generated message map functions
protected:
    //{{AFX_MSG(CMainFrame)
    afx_msg BOOL PreCreateWindow(CREATESTRUCT&);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnOpenDatabase();
    afx_msg void OnCloseDatabase();
    afx_msg LRESULT OnCloseDatabase(WPARAM,LPARAM);
    afx_msg void OnImportDatabase();
    afx_msg void OnConfigBaudrate();
    afx_msg void OnConfigLinChannel();
    afx_msg void OnNewDatabase();
    afx_msg void OnConfigDatabaseSaveAs();
    afx_msg void OnConfigDatabaseSave();
    afx_msg void OnConfigMessageDisplay();
    afx_msg void OnConfigMessageDisplayLin();
    afx_msg void OnReplaySingleSkip();
    afx_msg void OnReplaySingleStep();
    afx_msg void OnReplayStop();
    afx_msg void OnReplayGo();
    afx_msg void OnDllUnload();
    afx_msg void OnDllUnloadLIN();
    afx_msg void OnDllUnloadJ1939();
    afx_msg void OnMessageInterpretation();
    afx_msg void OnAddSignalToSignalWindow();

    afx_msg void OnAddSignalToSignalWindow_LIN();

    afx_msg void vOnLINScheduleTableConfig();

    //afx_msg void OnMessageFilter();
    afx_msg void OnLogFilter();
    afx_msg void OnLogFilterLIN();

    afx_msg void OnSelectMessage();
    afx_msg void OnLINFilter();
    afx_msg void OnAboutApplication();
    afx_msg void OnButtonSignalWatchButton();
    afx_msg void OnButtonSignalWatchButton_LIN();


    afx_msg void OnUpdateConfigureDatabaseClose(CCmdUI* pCmdUI);
    afx_msg void OnUpdateConfigureDatabaseNew(CCmdUI* pCmdUI);
    afx_msg void OnUpdateConfigureDatabaseSave(CCmdUI* pCmdUI);
    afx_msg void OnUpdateConfigureDatabaseSaveas(CCmdUI* pCmdUI);
    afx_msg void OnClose();
    afx_msg void OnUpdateMessageInterpret(CCmdUI* pCmdUI);
    afx_msg void OnUpdateLogFilter(CCmdUI* pCmdUI);
    afx_msg void OnUpdateLogFilterLIN(CCmdUI* pCmdUI);

    afx_msg void OnUpdateLINScheduleTableConfig(CCmdUI* pCmdUI);
    afx_msg void OnUpdateReplayFilter(CCmdUI* pCmdUI);


    afx_msg void OnMessageFilterButton();
    afx_msg void OnMessageFilterButtonLin();
    afx_msg void OnUpdateMessageFilterButtonLin(CCmdUI* pCmdUI);

    afx_msg void OnReplayFilter();
    void ApplyMessageFilterButton();
    afx_msg void OnUpdateMessageFilterButton(CCmdUI* pCmdUI);
    afx_msg void OnDisplayMessagewindowOverwrite();
    void ApplyMessagewindowOverwrite();
    afx_msg void OnClearMsgWindow();
    afx_msg void OnUpdateDisplayMessagewindowOverwrite(CCmdUI* pCmdUI);
    afx_msg void OnUpdateToolHexdec(CCmdUI* pCmdUI);
    afx_msg void OnUpdateLogOnOff(CCmdUI* pCmdUI);
    afx_msg void OnUpdateLogOnOff_LIN(CCmdUI* pCmdUI);
    afx_msg void OnUpdateReplayGo(CCmdUI* pCmdUI);
    afx_msg void OnUpdateReplaySkip(CCmdUI* pCmdUI);
    afx_msg void OnUpdateReplayStep(CCmdUI* pCmdUI);
    afx_msg void OnUpdateReplayStop(CCmdUI* pCmdUI);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnDestroy();
    afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
    afx_msg void OnCfgLogFile();
    afx_msg void OnCfgLogFile_LIN();

    afx_msg void OnUpdateToolButtonMsgDisp(CCmdUI* pCmdUI);
    afx_msg void OnEndSession(BOOL bEnding);
    afx_msg void OnCfgSendMsgs();

    afx_msg void OnFileConnect();
    afx_msg void OnUpdateFileConnect(CCmdUI* pCmdUI);
    afx_msg void OnFlexRayConnect();
    afx_msg void OnUpdateFlexRayConnect(CCmdUI* pCmdUI);
    afx_msg void OnLINConnect();
    afx_msg void OnUpdateLINConnect(CCmdUI* pCmdUI);
    afx_msg void OnLoadConfigFile();
    afx_msg void OnNewConfigFile();
    afx_msg void OnSaveConfigFile();
    afx_msg void OnSaveAsConfigFile();
    afx_msg void OnUpdateConfigLoad(CCmdUI* pCmdUI);
    afx_msg void OnUpdateConfigNew(CCmdUI* pCmdUI);
    afx_msg void OnUpdateConfigSave(CCmdUI* pCmdUI);
    afx_msg void OnUpdateConfigSaveAs(CCmdUI* pCmdUI);
    afx_msg void OnFilePropeties();
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnUpdateConfigureModeActive(CCmdUI* pCmdUI);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnNetworkStatisticsWnd();
    afx_msg void OnUpdateNetworkStatisticsWnd(CCmdUI* pCmdUI);



    afx_msg void OnUpdateConfigurePassive(CCmdUI* pCmdUI);
    afx_msg void OnTraceWnd();
    afx_msg LRESULT OnMessageTraceWnd(WPARAM wParam, LPARAM lParam);
    afx_msg void OnUpdateTraceWnd(CCmdUI* pCmdUI);
    afx_msg void OnUpdateConfigureBaudrate(CCmdUI* pCmdUI);
    afx_msg void OnDisplayAbsoluteTime();
    afx_msg void OnUpdateDisplayAbsolutetime(CCmdUI* pCmdUI);
    afx_msg void OnDisplayRelativetime();
    afx_msg void OnUpdateDisplayRelativetime(CCmdUI* pCmdUI);
    afx_msg void OnDisplaySystemTime();
    afx_msg void OnUpdateDisplaySystemTime(CCmdUI* pCmdUI);
    afx_msg void OnEnableTimeStampButton();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnUpdateSignalWatchWnd(CCmdUI* pCmdUI);
    afx_msg void OnUpdateSignalWatchWnd_LIN(CCmdUI* pCmdUI);
    afx_msg void OnUpdateSignalWatchWnd_FlexRay(CCmdUI* pCmdUI);
    afx_msg void OnGraphWindow();
    afx_msg void OnUpdateGraphWnd(CCmdUI* pCmdUI);
    afx_msg void OnCfgnReplay();

    afx_msg void OnUpdateCfgnReplay(CCmdUI* pCmdUI);
    afx_msg void OnConfigureSimulatedsystems();

    //LIN
    afx_msg void OnConfigureSimulatedsystemsLin();
    afx_msg void OnUpdateLinClusterConfig(CCmdUI* pCmdUI);
    //UDS
    afx_msg void OnCfgnUdsMainWnd();
    afx_msg void OnCfgnUdsSettingsWnd();
    afx_msg void OnAutomationTSEditor();
    afx_msg void OnAutomationTSExecutor();

    afx_msg void OnFlexRayDBAssociate();
    afx_msg void OnUpdateFlexrayAssociate(CCmdUI* pCmdUI);

    afx_msg void OnConfigChannelSelection();
    afx_msg void OnConfigChannelSelectionLIN();
    afx_msg void OnUpdateConfigChannelSelection(CCmdUI* pCmdUI);


    afx_msg void OnUpdateFrame(CCmdUI* pCmdUI);
    afx_msg void OnUpdatePdu(CCmdUI* pCmdUI);

    afx_msg LRESULT OnReceiveKeyBoardData(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnReceiveKeyDown(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT onGetConfigPath(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT onJ1939TxWndClose(WPARAM wParam, LPARAM lParam);




    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    afx_msg void OnClickMruList( UINT unID );
    afx_msg void OnUpdateMruList(CCmdUI* pCmdUI);
    afx_msg LRESULT OnErrorMessageProc(WPARAM wpParam, LPARAM lParam);
    afx_msg void OnUpdateStatusBar(WPARAM wpParam, LPARAM lParam);
    afx_msg void OnUpdateConfigurationFileName(CCmdUI* pCmdUI);
    afx_msg LRESULT vDisconnect(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT vSetWarningLimitVar(WPARAM wParam, LPARAM lParam);
    // Notification from trace window
    afx_msg LRESULT vNotificationFromOtherWin(WPARAM wParam, LPARAM lParam);
    // Handler for creating trace window
    afx_msg LRESULT vCreateTraceWindow(WPARAM wParam, LPARAM lParam);
    int nCreateToolbar(CWnd* pomParent, CToolBar& omToolbar, UINT unID,CString omTitle);
    void DockControlBarLeftOf(CToolBar* pomCurrBar,CToolBar* pomLeftOf);


public:
    // To update graph data with statistics parameters
    void vUpdateGraphStatsData();
    //To send configuration change command to all signal graph windows
    void vPostConfigChangeCmdToSigGrphWnds(bool bHideGraphWnd = TRUE);

    void vClearSignalInfoList(void);
    void vUpdateChannelInfo(void);
    void vUpdateHWStatusInfo(void);
    bool bFillDbStructure(CMsgNameMsgCodeListDataBase& odMsgNameMsgCodeListDB);
    void vInitialiaseLINConfig(int nChannel = 0);
private:
    std::string strGetCurrentConfigFileName();
    //Spin Control Page No. for Import of Log File.
    CSpinButtonCtrl m_omSpinPageNo;
    CRadixEdit m_omEditPageNo;
    CSliderCtrl m_omSliderMsgs;

    bool m_bUpdateNetworkStatistics;
    bool m_bLINDisconnect;
    HMODULE m_hModAdvancedUILib;
    bool    m_bUseAdvancedUILib;
    BYTE    m_bytIconSize;
    PROJECTDATA m_sProjData;
    CMenu* m_pExternalTools; // External Tools menu is dynamically created.
    DILLIST m_ouList;// List of the driver interface layers supported
    DILLIST m_ouListLin;// List of the driver interface layers supported
    INT m_nDILCount; //Count of the driver interface layers supported
    INT m_nDILCountLin; //Count of the driver interface layers supported
    CMenu* m_pDILSubMenu;
    DILLIST m_ouFlexRayList;    // List of the FlexRay driver interface layers supported
    INT             m_nFlexRayDILCount; //Count of the FlexRay driver interface layers supported
    CMenu*          m_pFlxDILSubMenu;
    CMenu* m_pDILSubMenuLin;
    CWaveFormDataHandler m_objWaveformDataHandler;
    CMainEntryList m_odResultingList;
    //CMainEntryList m_odResultingList;

    INT m_anMsgBuffSize[BUS_TOTAL][defDISPLAY_CONFIG_PARAM];
    WINDOWPLACEMENT m_sMsgWndPlacement;//MSG_WND_PLACEMENT
    WINDOWPLACEMENT m_sMsgInterpretPlacement;//MSGINTERP_WND_PLACEMENT

    CString m_omMRU_C_Filename;
    STOOLBARINFO m_sToolBarInfo;
    WINDOWPLACEMENT m_sNotificWndPlacement;

    WINDOWPLACEMENT m_sGraphWndPlacement[AVAILABLE_PROTOCOLS];
    SGRAPHSPLITTERDATA m_sGraphSplitterPos[AVAILABLE_PROTOCOLS];

    WINDOWPLACEMENT m_sNetworkWndPlacement;

    SCONTROLLER_DETAILS m_asControllerDetails[defNO_OF_CHANNELS];
    SCONTROLLER_DETAILS_LIN m_asControllerDetailsLIN[defNO_OF_LIN_CHANNELS];
    std::list<FlexRayControllerParams> m_lstControllerDetailsFlexRay;
    //sCONTROLLERDETAILSLIN sControllerDetailsLIN[defNO_OF_LIN_CHANNELS];

    SFILTERAPPLIED_CAN m_sFilterAppliedCAN; // Filter applied struct for CAN
    SFILTERAPPLIED_J1939 m_sFilterAppliedJ1939; // Filter applied struct for J1939
    SFILTERAPPLIED_LIN m_sFilterAppliedLIN;     // Filter applied struct for LIN



    CString m_omStrSavedConfigFile;
    STCAN_MSG m_sRxMsgInfo;
    STLIN_MSG m_sRxMsgInfoLin;

    bool m_bMsgHandlerRxDataByte;
    bool m_bAbortMsgHandler;
    //   void vInitialiseInterfaceFunctionPointers();
    // To apply window postion from configuration module
    void vRestoreWindowPostion();
    // To find state transtition
    bool bIsTransitionInState(UINT unChannel, BYTE byRxError, BYTE byTxError);

    //To initialize Read Buffer for Graph window purpose
    void vInitializeGraphWndReadBuffer();

    // Menu options for configuration file
    bool m_bCfgNewMenuOption;
    bool m_bCfgLoadMenuOption;
    bool m_bCfgSaveMenuOption;
    bool m_bCfgSaveAsMenuOption;
    // Returns the position of the menu item asked for
    INT nFindMenuItem(CMenu* Menu, LPCTSTR MenuString);
    // Shuffles MRU filenames
    //gets the toolbar style of a toolbar in terms of TOP, BOTTOM, LEFT, RIGHT
    //string GetToolBarStyle(CToolBar& wndToolbar);

    bool m_bIsSendingMsg;
    // Holds Previously loaded DLL name
    CString m_omStrPrevLoadedDll;
    // Data bytes for sending message
    BYTE m_abyMessageData[8];

    // Pointer to CMsgMDIChildWnd class
    CMsgMDIChildWnd* m_podMsgMDIChild;
    // Flag to indicate creation of new database
    bool m_bIsNewDatabase;
    // Pointer to CMsgSgDetView class
    CMsgSgDetView* m_pomMsgSgDetViews[BUS_TOTAL];
    // Pointer to CMsgSgTreeView class
    CMsgSgTreeView* m_pomMsgSgTreeViews[BUS_TOTAL];
    // Pointer to CMsgSignalDBWnd class
    CMsgSignalDBWnd* m_podMsgSgWnd;
    CMsgSignalDBWnd* m_podMsgSgWndJ1939;
    // To set window place every time at the same
    // place as it was last closed at.
    WINDOWPLACEMENT m_WinCurrStatus;
    // To remember the current error state
    ERROR_STATE m_eCurrErrorState[ defNO_OF_CHANNELS ];

    // Timer to update status bar
    UINT_PTR m_unTimerSB;
    UINT_PTR m_unTimerSBLog;
    UINT_PTR m_unJ1939TimerSBLog;
    UINT_PTR m_unLINTimerSBLog;

    // Transmission and reception error counters
    SERROR_CNT m_sErrorCount;
    // Flag to indicate which one between message ID and name is selected
    bool m_bMessageName;
    // Get new log filename from old
    void vWriteNewLogFilenameInRegistry( CString omLogFilename );
    // Get window status
    void vGetWinStatus(WINDOWPLACEMENT&);
    // Set window status
    void vSaveWinStatus(WINDOWPLACEMENT);

    bool bSetDefaultToolbarPosition();
    bool bDestroyMsgWindow(void);
    // Change status of tool bar button
    bool bSetPressStatus(int, eCANMONITORFLAG);
    // To stop or start logging during configuration change
    //inline void vStartStopLogging(bool bStart);
    // To process J1939 DIL and logger interfaces
    HRESULT ProcessJ1939Interfaces(void);
    // To deselect J1939 interfaces
    HRESULT DeselectJ1939Interfaces(BOOL bTrace = TRUE);
    // To process LIN DIL and logger interfaces
    HRESULT ProcessLINInterfaces(void);
    // To deselect LIN interfaces
    HRESULT DeselectLINInterfaces(void);

    // The bus statistics modeless dialog box
    CBusStatisticsDlg* m_podBusStatistics;
    CLINBusStatisticsDlg* m_podLBusStatistics;

    CNetworkStatistics* m_cnsTester;

    bool m_bIsStatWndCreated;
    UINT m_unWarningLimit;

    CString         m_omAppDirectory;
    bool            m_bFlxDILChanging;

    void ToggleView(CToolBar& omToolbar);
    bool bIsToolbarVisible(CToolBar& omToolbar);


    void vPopulateIDList(ETYPE_BUS);

    /* Helper function to re register all the nodes when driver changes */
    void vReRegisterAllCANNodes(void);
    void vReRegisterAllLINNodes(void);

    void vReRegisterAllJ1939Nodes(void);

    void vGetLoadedCfgFileName(CString& omFileName);
    bool bIsConfigurationModified(void);
    void vGetCurrentSessionData(eSECTION_ID eSecId, xmlNodePtr pNodePtr);
    void vManageOnKeyHandler(CBaseNodeSim* pBaseNodeSim, UINT unKey);

    //void vSetCurrentSessionData(eSECTION_ID eSecId, BYTE* pbyConfigData, UINT nSize);
    void vSetDefaultConfiguration(eSECTION_ID eSecId);

    INT SaveConfiguration(void);
    int nLoadXMLConfiguration(std::string& m_strCfxFile);
    int nLoadXMLConfiguration();

    void vSetWindowPositionForGraph(xmlNodePtr pNodePtr, xmlDocPtr pDocPtr);

    INT vSaveXMLConfiguration();
    INT vSaveXMLConfiguration(const char* filename);

    INT nGetControllerID(std::string ptext);
    void LoadControllerConfigData(SCONTROLLER_DETAILS& sController, xmlNodePtr& pNodePtr);
    std::string m_omStrCurrentConfigFile;

    CString vGetControllerName(UINT nDriverId);

    void vSetFileStorageInfo(CString omCfgFileName);
    void vSetCurrProjInfo(float fAppVersion);

    DILINFO* psGetDILEntry(UINT unKeyID, bool bKeyMenuItem = TRUE);
    DILINFO* psGetFLEXRAYDILEntry(UINT unKeyID, bool bKeyMenuItem = TRUE);
    void vInitializeBusStatCAN(void);


    DILINFO* psGetDILLINEntry(UINT unKeyID, bool bKeyMenuItem = TRUE);
    void vInitializeBusStatLIN(void);

    bool bStartGraphReadThread();
    bool bStopGraphReadThread();
    void vUpdateGraphData(STCANDATA& sCanData);
    void vClearDbInfo(ETYPE_BUS eBus);
    CPARAM_THREADPROC m_ouGraphReadThread;
    CMsgBufFSE<STCANDATA> m_ouCanBuf;
    CMsgBufFSE<STLINDATA> m_ouLinBuf;

    CMsgBufVSE* m_pouMsgInterpretBuffer;
    LONGLONG m_nTimeStamp;

    CMsgSignal* m_pouMsgSigJ1939;
    CMsgSignal* m_pouActiveDbJ1939;
    BOOL m_abLogOnConnect[BUS_TOTAL];

    xmlNodePtr m_pXmlNodeBusStats;
    void vSetGlobalConfiguration(xmlNodePtr& pNodePtr);
    void vSW_DoInitialization(ETYPE_BUS eBusType);
    void OnClusterChanged(ETYPE_BUS eBusType);
    void GenerateNodeSimObjFiles(ETYPE_BUS eBusType);
    static DWORD WINAPI NSCodeGenerationThread(LPVOID pVoid);
    CPARAM_THREADPROC m_ouNodeSimCodeGenThread;
    HANDLE m_NSCodeGenThreads[MAX_PROTOCOL];

    LRESULT OnNodeSimCodeGenStatus(WPARAM, LPARAM);
    bool bWaitForNSCodeGenStatus(ETYPE_BUS );
    struct NSCodeGenThreadParam
    {
        CMainFrame* m_pMainFrame;
        ETYPE_BUS m_eBusType;
    };

    void ApplyFilterConfigToMsgWnd(std::map<std::string, bool>& filtersApplied, SFILTERAPPLIED_CAN&);
    void ApplyFilterConfigToMsgWnd(std::map<std::string, bool>& filtersApplied, SFILTERAPPLIED_LIN&);
public:
    void vPopulateJ1939PGNList();
    INT ReadGraphDataBuffer(bool bCalcTime);
    INT nLoadConfigFile(CString omConfigFileName);
    void BuildAllNodes();
    int  COM_ConnectTool();
    bool COM_SaveConfigFile();
    bool COM_SaveConfigFileAs(CString omCfgFilePath);


    bool bUpdatePopupMenuDIL(void);
    bool bUpdatePopupMenuFLEXRAYDIL(void);
    bool bUpdatePopupMenuDILL(void);


    void vNS_LINInitCFileFunctPtrs();

    void vInitCFileFunctPtrs();
    void NS_InitJ1939SpecInfo();
    void vUpdateMsgNameCodeList(CMsgSignal* pMsgSig, CMsgNameMsgCodeListDataBase& odMsgNameMsgCodeListDB);
    void vPushConfigFilenameDown ( CString omStrConfigFilename );
    void vUpdateMainEntryListInWaveDataHandler();
    void vUpdateAllMsgWndInterpretStatus(ETYPE_BUS eBusType, bool bAssociate);


    int getDilService( ETYPE_BUS, IBusService** );
    int getDbService( IBMNetWorkGetService** );
    int displayMessage( char* message );
    HWND getBusmasterHandle();
    int getInfo(int infoType, EXT_INFO_PARAM, /*IN, OUT*/ OUT_INFO_PARAM infoData);
    int getPluginConnectionPoint(const char* pluginId, IBusmasterPluginConnection**);
    void provideBusmasterConfiguredPaths(int pathType, char* infoData);
public:
    afx_msg void OnUpdateSelectDriver(CCmdUI* pCmdUI);
    afx_msg void OnSelectDriver(UINT nID);

    afx_msg void OnUpdateSelectFLEXRAYDriver(CCmdUI* pCmdUI);
    afx_msg void OnSelectFLEXRAYDriver(UINT nID);

    afx_msg void OnUpdateSelectLINDriver(CCmdUI* pCmdUI);
    afx_msg void OnSelectLINDriver(UINT nID);
    afx_msg LRESULT vKeyPressedInMsgWnd(WPARAM wParam, LPARAM lParam);
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
    afx_msg void OnDissociateDatabase();
    afx_msg void OnSaveImportDatabase();
    afx_msg void OnUpdateSaveImportDatabase(CCmdUI* pCmdUI);
    afx_msg void OnSaveImportJ1939Database();
    afx_msg void OnUpdateSaveImportJ1939Database(CCmdUI* pCmdUI);
    //To update the Message DB Pointer
    afx_msg LRESULT OnProvideMsgDBPtr(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnProvideMsgNameFromCode(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnProvidePGNNameFromCode(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnMessageFromUserDll(WPARAM wParam, LPARAM lParam);

    afx_msg void OnConfigureModeActive();
    afx_msg void OnConfigurePassive();
    afx_msg void OnLogEnable();
    afx_msg void OnLog_LIN_Enable();

    afx_msg void OnRestartController();
    afx_msg void OnUpdateCfgnLog(CCmdUI* pCmdUI);
    afx_msg void OnUpdateCfgnLog_LIN(CCmdUI* pCmdUI);



    afx_msg void OnStatisticsCAN();
    afx_msg void OnStatisticsLIN();
    afx_msg void OnStatisticsFlexRay();
    afx_msg void OnStatistics(ETYPE_BUS ebus);

    afx_msg void OnDisplayEdit();
    afx_msg void OnUpdateDisplayEdit(CCmdUI* pCmdUI);
    afx_msg void OnDisplayMain();
    afx_msg void OnUpdateDisplayMain(CCmdUI* pCmdUI);
    afx_msg void OnDisplayMsgWnd();
    afx_msg void OnUpdateDisplayMsgWnd(CCmdUI* pCmdUI);



    afx_msg void OnDisplayConfig();
    afx_msg void OnUpdateDisplayConfig(CCmdUI* pCmdUI);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnConfigureWaveformMessages();
    afx_msg void OnStartSignalTransmission();
    afx_msg void OnUpdateStartSignalTransmission(CCmdUI* pCmdUI);
    afx_msg void OnConfigureSignalgraphwindow();
    afx_msg void OnUpdateConfigureSignalgraphwindow(CCmdUI* pCmdUI);
    afx_msg void OnSignalgraphwindowCAN();
    afx_msg void OnUpdateSignalgraphwindowCAN(CCmdUI* pCmdUI);
    afx_msg void OnSignalgraphwindowMcnet();
    afx_msg void OnUpdateSignalgraphwindowMcnet(CCmdUI* pCmdUI);
    afx_msg void OnActivateJ1939();
    afx_msg void OnUpdateActivateJ1939(CCmdUI* pCmdUI);
    afx_msg void OnUpdateActivateLIN(CCmdUI* pCmdUI);
    afx_msg void OnJ1939ConfigLog();
    afx_msg void OnUpdateJ1939ConfigLog(CCmdUI* pCmdUI);
    afx_msg void OnUpdateLinConfigLog(CCmdUI* pCmdUI);
    afx_msg void OnActionJ1939Online();
    afx_msg void OnUpdateActionJ1939Online(CCmdUI* pCmdUI);
    afx_msg void OnActionJ1939TxMessage();
    afx_msg void OnUpdateActionJ1939TxMessage(CCmdUI* pCmdUI);
    afx_msg void OnActionJ1939Log();
    afx_msg void OnUpdateActionJ1939Log(CCmdUI* pCmdUI);
    afx_msg void OnUpdateActionLINLog(CCmdUI* pCmdUI);
    afx_msg void OnToolbarJ1939();
    afx_msg void OnUpdateToolbarJ1939(CCmdUI* pCmdUI);
    afx_msg void OnToolbarFlexRay();
    afx_msg void OnUpdateToolbarFlexRay(CCmdUI* pCmdUI);
    afx_msg void OnToolbarConfiguration();
    afx_msg void OnUpdateToolbarConfiguration(CCmdUI* pCmdUI);
    afx_msg void OnToolbarLIN();
    afx_msg void OnUpdateToolbarLIN(CCmdUI* pCmdUI);
    afx_msg void OnJ1939ConfigureTimeouts();
    afx_msg void OnUpdateJ1939Timeouts(CCmdUI* pCmdUI);
    afx_msg void OnUpdateJ1939DBNew(CCmdUI* pCmdUI);
    afx_msg void OnJ1939DBNew();
    afx_msg void OnJ1939DBOpen();
    afx_msg void OnJ1939DBClose();
    afx_msg LRESULT OnJ1939DBClose(WPARAM wParam, LPARAM lParam);
    afx_msg void OnUpdateJ1939DBClose(CCmdUI* pCmdUI);
    afx_msg void OnJ1939DBSave();
    afx_msg void OnJ1939DBSaveAs();
    afx_msg void OnUpdateJ1939DBSave(CCmdUI* pCmdUI);
    afx_msg void OnUpdateJ1939DBSaveAs(CCmdUI* pCmdUI);
    afx_msg void OnJ1939DBAssociate();
    afx_msg void OnJ1939DBDissociate();
    afx_msg void OnJ1939CfgSimSys();
    afx_msg void OnUpdateJ1939CfgSimSys(CCmdUI* pCmdUI);
    afx_msg void OnJ1939SignalwatchAdd();
    afx_msg void OnJ1939SignalwatchShow();
    afx_msg void OnUpdateJ1939SignalwatchShow(CCmdUI* pCmdUI);
    afx_msg void OnConfigureMessagedisplayJ1939();

    afx_msg void OnJ1939Exportlog();
    afx_msg void OnShowHideMessageWindow(UINT nID);
    afx_msg void OnUpdateShowHideMessageWindow(CCmdUI* pCmdUI);
    afx_msg void OnToolbarCandatabase();
    afx_msg void OnUpdateToolbarCanDatabase(CCmdUI* pCmdUI);
    afx_msg LRESULT OnMessageFromUserDllGetAbsoluteTime(WPARAM wParam, LPARAM lParam);
    afx_msg void OnFileConverter();

    //afx_msg void OnActivateLIN();
    afx_msg void OnCfgSendMsgsLIN();
    afx_msg void OnLinClusterConfig();
    afx_msg void OnLDFEditor();



    afx_msg void onPluginMenuClicked(UINT id);
    afx_msg void onPluginMenuUpadate( CCmdUI* pCmdUI );


    void ApplyLogFilter();
    void ApplyLINLogFilter();

    void ApplyReplayFilter();
    xmlDocPtr m_xmlConfigFiledoc;
    bool m_bIsXmlConfig;

private:
    void vLoadBusmasterKernel();

    void vVlaidateAndLoadFibexConfig(sLinConfigContainer& ouFibexContainer);


    void SaveConfigDataToXML( SCONTROLLER_DETAILS&, xmlNodePtr pNodePtr );
    void vInitialize( SCONTROLLER_DETAILS& controller, BOOL bUpdateHWDesc );

    int m_nMaxFlexChannels;
    int m_nMaxLinChannels;

    IBMNetWorkService* m_ouBusmasterNetwork = nullptr;
    IBusMasterKernel* mBusmasterKernel = nullptr;
    IBusmasterPluginManager* mPluginManager = nullptr;
    frChannelSettingsHandler mFrChannelSettingsHandler;

public:
	afx_msg void OnCanSaintTool();
};
