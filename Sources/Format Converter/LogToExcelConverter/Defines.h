/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  common.h
  Description   :  
  $Log:   X:/Archive/Sources/Application/common.h_v  $
 * 
 *    Rev 1.10   06 Sep 2011 19:55:26   CANMNTTM
 * CANvas name is replaced with BUSMATER
 * 
 *    Rev 1.9   15 Apr 2011 20:02:22   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.8   29 Mar 2011 12:01:08   CANMNTTM
 * Updated to have show/hide functionality for all message windows and also disabling buffer updates when a message window is hidden.
 * 
 *    Rev 1.7   02 Mar 2011 15:38:32   CANMNTTM
 * 1. Support to J1939 Nodesimulation
 * 3. Support to J1939 Message window
 * 2. Removed unwanted macros
 * 
 *    Rev 1.6   22 Sep 2010 15:22:48   CANMNTTM
 * Old configuration is loaded according to the 
 * license validator.
 * 
 *    Rev 1.5   16 Sep 2010 15:51:56   CANMNTTM
 * Added new definition
 * eWINID_MSG_WND_GET_BUFFER_DETAILS.
 * 
 *    Rev 1.4   13 Sep 2010 10:30:20   CANMNTTM
 * Added new definition WM_UPDATE_TREE_ITEMS_POS.
 * 
 *    Rev 1.3   06 Sep 2010 19:56:16   CANMNTTM
 * Added new definition
 * WM_UPDATE_MSG_INTRP_WND_PLC
 * 
 *    Rev 1.2   03 Sep 2010 19:54:50   CANMNTTM
 * 1. Message WM_ENABLE_MSGWND_FILTER
 * has been added.
 * 
 *    Rev 1.1   01 Sep 2010 21:28:44   CANMNTTM
 * Three more message 
 * 1. eWINID_MSG_WND_SET_CONFIG_DATA
 * 2. eWINID_MSG_WND_GET_CONFIG_DATA
 * 3. eWINID_MSG_WND_GET_CONFIG_SIZE
 * are added to load/save msg window configuration.
 * 
 * 
 *    Rev 1.0   16 Aug 2010 21:20:50   rac2kor
 *  

  Author(s)     :  
  Date Created  :  
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#if !defined COMMON_DEFS_H__INCLUDED_
#define COMMON_DEFS_H__INCLUDED_
typedef enum
{
	LOAD_CONFIG,
	SAVE_CONFIG,
    NEW_CONFIG,
	IS_CONFIG_CHANGED
};

enum
{
    eWINID_INVALID = 0,
    eWINID_MSGWND,
    eWINID_MSG_INTERPRETATION,
    eWINID_TRACEWND,
    eWNDID_GRAPHWND,
	eWINID_MSG_WND_SET_CONFIG_DATA,
	eWINID_MSG_WND_GET_CONFIG_DATA,
    eLOAD_DATABASE,
    eDISSCOCIATE_DATABASE,
    eWINID_START_READ,
    eWINID_STOP_READ,
    eWINID_MSG_WND_GET_CONFIG_SIZE,
	eWINID_MSG_WND_GET_BUFFER_DETAILS, 
};

#define IDC_LSTB_OUTPUT             300

// User defined Window messages
//#define WM_LSTC_DBLCLK              (WM_USER + 59)
//#define WM_OWNER_DATA_NOTIFICATION  (WM_USER + 60)

#define WM_RX_COMPLETE				(WM_USER + 61)	
#define WM_TX_COMPLETE				(WM_USER + 62)	
#define WM_HW_STATUS				(WM_USER + 63)
#define WM_HW_NETWORK_STATISTICS	(WM_USER + 64)
//#define WM_MODIFY_VISIBILITY		(WM_USER + 65)
#define WM_UPDATE_FONT				(WM_USER + 66)
#define WM_UPDATE_MSGWNDCFG			(WM_USER + 67)
#define WM_WND_PROP_MODIFY          (WM_USER + 68)
#define WM_WND_LOG_PROPERTY         (WM_USER + 69)
#define WM_WND_HW_ACTION            (WM_USER + 70)
#define WM_LOG_STATUS_MODIFY        (WM_USER + 71)
#define WM_START_STOP_CYCLIC_TX     (WM_USER + 72)
#define WM_HW_CONFIGURE             (WM_USER + 73)
#define WM_UPDATE_SENDBUTTON        (WM_USER + 74)
#define WM_CONFIG_QUESTION          (WM_USER + 75)
#define WM_PROVIDE_WND_PROP         (WM_USER + 76)
#define WM_TX_EDIT_COMPLETE         (WM_USER + 77)
#define WM_LOAD_HW_ICON             (WM_USER + 78)
//To manage msg colour
#define WM_MSG_COLOR_MODIFY			(WM_USER + 79)
//#define WM_MSG_FILTER_STATUS        (WM_USER + 30)
#define WM_IS_HW_ACTIVE				(WM_USER + 80)
#define WM_MSG_INTERPRETATION		(WM_USER + 81)
#define WM_IS_SENDMSG_PRESENT		(WM_USER + 82)
//#define WM_NOTIFICATION_FROM_OTHER	(WM_USER + 83)
//#define WM_LOAD_SAVE_WINSTATUS      (WM_USER + 84)
#define WM_THREADMSG_PROC           (WM_USER + 85)
#define WM_TRACE_WND_DISP           (WM_USER + 86)
#define WM_GET_DB_PTR               (WM_USER + 87)
#define WM_LOAD_UNLOAD              (WM_USER + 88) //If you are changing this change in NodeSimEx/HashDefines.h accordingly
#define WM_DISPLAY_MSGWND           (WM_USER + 90)
#define WM_PARENT_UPDATE_MSGPTR		(WM_USER + 91)
#define WM_PARENT_UPDATE_MSG_CLR	(WM_USER + 92)
#define WM_INVALIDATE_LIST_DISPLAY	(WM_USER + 93)
#define WM_GET_INTERPRET_STATE		(WM_USER + 94)
#define WM_PARENT_SORT_COLUMN    	(WM_USER + 95)
#define WM_CLEAR_SORT_COLUMN    	(WM_USER + 96)
#define WM_UPDATE_CONNECT_STATUS   	(WM_USER + 97)
#define WM_EXPAND_COLLAPSE_MSG   	(WM_USER + 98)
#define WM_SETFOCUS_MSGWND_LIST   	(WM_USER + 99)
#define WM_GET_NEXT_PREV_MSG_INDEX 	(WM_USER + 100)
#define WM_SET_FILTER_DETAILS       (WM_USER + 101)
#define WM_GET_FILTER_DETAILS       (WM_USER + 102)
#define WM_MSG_ITEM_DBL_CLICK	 	(WM_USER + 103)
#define WM_ENABLE_FILTER_APPLIED    (WM_USER + 104)
#define WM_UPDATE_MSG_INTRP_WND_PLC (WM_USER + 105)
#define WM_UPDATE_TREE_ITEMS_POS    (WM_USER + 106)
#define WM_SHOW_MESSAGE_WINDOW      (WM_USER + 107)


#define APPLICATION_NAME             _T("BUSMASTER")
#define DEFAULT_PROJECT_NAME         "Auto-SAT_Tools"
//#define defSTR_CONF_SAVE_CONFIRM     _T("Configuration File is modified \n Do you want to save this configuration?")

#define BUSMASTER_APPN_VERSION_6_0          6.0
#define BUSMASTER_APPN_VERSION_6_1          6.1
#define BUSMASTER_APPN_VERSION_LATEST       6.1
#define BUSMASTER_UNIQUE_ID             133146
#define defCONFIGFILENAME           _T("ConfigFile")
#define defSTR_CONF_CLOSE_CONFIRM   _T("Do you want to close the application?")
#define SECTION                     _T("Files")


#define defSECTION_MRU           _T("MRU List")
#define defSECTION_MRU_FILE1     _T("MRU File1")
#define defSECTION_MRU_FILE2     _T("MRU File2")
#define defSECTION_MRU_FILE3     _T("MRU File3")
#define defSECTION_MRU_FILE4     _T("MRU File4")
#define defSECTION_MRU_FILE5     _T("MRU File5")
#define defSTR_DEFAULT_MRU_ITEM  _T("Empty")
#define defSTR_FILE_MENU_TEXT    _T("&File")
//#define defSTR_RECENT_MENU_TEXT  _T("Recent File")
#define STR_EMPTY                _T("")

#define RULE_FILE_EXTENSION      _T("xml")
#define RULE_FILTER              _T("*.xml|*.xml||")
#define RULE_SELECTION_TITLE     _T("Select Rule File")

#define EXPORTTOEXCEL_SYNTAXERROR _T("Could not convert!")
#define EXPORTTOEXCEL_SUCCESSMSG _T("Successfully converted text file to Excel File!")
#define EXPORTTOEXCEL_FIELDSELECTIONERROR _T("No fields selected!")
#define EXPORTTOEXCEL_LOGFILEERROR _T("Enter Log file details!") 
#define EXPORTTOEXCEL_CSVFILEERROR _T("Enter CSV file details!") 
#define EXPORTTOEXCEL_FILEERROR _T("Enter Log and CSV file details!") 
#define EXPORTTOEXCEL_LOGFILEOPENERROR _T("Log File doesnot Exist!")
#define EXPORTTOEXCEL_CSVFILEOPENERROR _T("CSV File doesnot Exist!")

#define MSG_MEMORY_CONSTRAINT              _T("Insufficient Memory available")
#define ASK_SAVE_PROMPT         _T("Do you want to save changes to the database file?")
#define defSTR_EMPTY_ACTIVE_DATABASE _T("There is no active database. Please import database")
#define defNEW_LINE                 _T("\n")
#define DEFAULT_RULE_FILE               _T("MCNet_Example.xml")



typedef enum eTYPE_BUS
{
    CAN     = 0,
    MCNET,
    J1939,
    FLEXRAY,
    LIN,
    MOST,
    BUS_TOTAL,
    BUS_INVALID
} ETYPE_BUS;


#endif // COMMON_DEFS_H__INCLUDED_