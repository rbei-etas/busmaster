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
 * \file      Defines.h
 * \brief     Definitions
 * \authors   Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definitions.
 */

#pragma once

/* MFC includes */
#include "afxwin.h"
#include "../../Utility/MultiLanguageSupport.h"
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

#define WM_RX_COMPLETE              (WM_USER + 61)
#define WM_TX_COMPLETE              (WM_USER + 62)
#define WM_HW_STATUS                (WM_USER + 63)
#define WM_HW_NETWORK_STATISTICS    (WM_USER + 64)
//#define WM_MODIFY_VISIBILITY      (WM_USER + 65)
#define WM_UPDATE_FONT              (WM_USER + 66)
#define WM_UPDATE_MSGWNDCFG         (WM_USER + 67)
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
#define WM_MSG_COLOR_MODIFY         (WM_USER + 79)
//#define WM_MSG_FILTER_STATUS        (WM_USER + 30)
#define WM_IS_HW_ACTIVE             (WM_USER + 80)
#define WM_MSG_INTERPRETATION       (WM_USER + 81)
#define WM_IS_SENDMSG_PRESENT       (WM_USER + 82)
//#define WM_NOTIFICATION_FROM_OTHER    (WM_USER + 83)
//#define WM_LOAD_SAVE_WINSTATUS      (WM_USER + 84)
#define WM_THREADMSG_PROC           (WM_USER + 85)
#define WM_TRACE_WND_DISP           (WM_USER + 86)
#define WM_GET_DB_PTR               (WM_USER + 87)
#define WM_LOAD_UNLOAD              (WM_USER + 88) //If you are changing this change in NodeSimEx/HashDefines.h accordingly
#define WM_DISPLAY_MSGWND           (WM_USER + 90)
#define WM_PARENT_UPDATE_MSGPTR     (WM_USER + 91)
#define WM_PARENT_UPDATE_MSG_CLR    (WM_USER + 92)
#define WM_INVALIDATE_LIST_DISPLAY  (WM_USER + 93)
#define WM_GET_INTERPRET_STATE      (WM_USER + 94)
#define WM_PARENT_SORT_COLUMN       (WM_USER + 95)
#define WM_CLEAR_SORT_COLUMN        (WM_USER + 96)
#define WM_UPDATE_CONNECT_STATUS    (WM_USER + 97)
#define WM_EXPAND_COLLAPSE_MSG      (WM_USER + 98)
#define WM_SETFOCUS_MSGWND_LIST     (WM_USER + 99)
#define WM_GET_NEXT_PREV_MSG_INDEX  (WM_USER + 100)
#define WM_SET_FILTER_DETAILS       (WM_USER + 101)
#define WM_GET_FILTER_DETAILS       (WM_USER + 102)
#define WM_MSG_ITEM_DBL_CLICK       (WM_USER + 103)
#define WM_ENABLE_FILTER_APPLIED    (WM_USER + 104)
#define WM_UPDATE_MSG_INTRP_WND_PLC (WM_USER + 105)
#define WM_UPDATE_TREE_ITEMS_POS    (WM_USER + 106)
#define WM_SHOW_MESSAGE_WINDOW      (WM_USER + 107)


#define APPLICATION_NAME             "BUSMASTER"
#define DEFAULT_PROJECT_NAME         "BUSMASTER"

#define BUSMASTER_APPN_VERSION_6_0          6.0
#define BUSMASTER_APPN_VERSION_6_1          6.1
#define BUSMASTER_APPN_VERSION_LATEST       6.1
#define BUSMASTER_UNIQUE_ID             133146
#define defCONFIGFILENAME           "ConfigFile"
#define defSTR_CONF_CLOSE_CONFIRM   _("Do you want to close the application?")
#define SECTION                     _("Files")


#define defSECTION_MRU           _("MRU list")
#define defSECTION_MRU_FILE1     _("MRU file1")
#define defSECTION_MRU_FILE2     _("MRU file2")
#define defSECTION_MRU_FILE3     _("MRU file3")
#define defSECTION_MRU_FILE4     _("MRU file4")
#define defSECTION_MRU_FILE5     _("MRU file5")
#define defSTR_DEFAULT_MRU_ITEM  _("Empty")
#define defSTR_FILE_MENU_TEXT    _("&File")
#define STR_EMPTY                ""

#define RULE_FILE_EXTENSION      "xml"
#define RULE_FILTER              "*.xml|*.xml||"
#define RULE_SELECTION_TITLE     _("Select rule file")
#define EXPORTTOEXCEL_SUCCESSMSG            _("Successfully converted text file to Excel file!")
#define EXPORTTOEXCEL_FAILUREMSG            _("The log file selected doesn't match the bus type selection. Please check.")

#define EXPORTTOEXCEL_SYNTAXERROR           _("Could not convert!")
#define EXPORTTOEXCEL_FILESNOTOPEN          _("Error in opening input files");
#define EXPORTTOEXCEL_FIELDSELECTIONERROR   _("No fields selected!")
#define EXPORTTOEXCEL_LOGFILEERROR          _("Enter log file details!")
#define EXPORTTOEXCEL_CSVFILEERROR          _("Enter CSV file details!")
#define EXPORTTOEXCEL_FILEERROR             _("Enter log and CSV file details!")
#define EXPORTTOEXCEL_LOGFILEOPENERROR      _("Log file doesn't exist!")
#define EXPORTTOEXCEL_CSVFILEOPENERROR      _("CSV file doesn't exist!")

#define ERR_EXPORTTOEXCEL_SYNTAXERROR           0xF000
#define ERR_EXPORTTOEXCEL_FILESNOTOPEN          ERR_EXPORTTOEXCEL_SYNTAXERROR + 1
#define ERR_EXPORTTOEXCEL_FIELDSELECTIONERROR   ERR_EXPORTTOEXCEL_SYNTAXERROR + 2
#define ERR_EXPORTTOEXCEL_LOGFILEERROR          ERR_EXPORTTOEXCEL_SYNTAXERROR + 3
#define ERR_EXPORTTOEXCEL_CSVFILEERROR          ERR_EXPORTTOEXCEL_SYNTAXERROR + 4
#define ERR_EXPORTTOEXCEL_FILEERROR             ERR_EXPORTTOEXCEL_SYNTAXERROR + 5
#define ERR_EXPORTTOEXCEL_LOGFILEOPENERROR      ERR_EXPORTTOEXCEL_SYNTAXERROR + 6
#define ERR_EXPORTTOEXCEL_CSVFILEOPENERROR      ERR_EXPORTTOEXCEL_SYNTAXERROR + 7

#define MSG_MEMORY_CONSTRAINT              _("Insufficient memory available")
#define ASK_SAVE_PROMPT         _("Do you want to save changes to the database file?")
#define defSTR_EMPTY_ACTIVE_DATABASE _("There is no active database. Please import database")
#define defNEW_LINE                 "\n"
#define DEFAULT_RULE_FILE               "MCNet_Example.xml"



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
