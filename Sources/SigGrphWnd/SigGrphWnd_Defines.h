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
 * \file      SigGrphWnd_Defines.h
 * \brief     Contains all the associated macro definitions
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains all the associated macro definitions
 */

#pragma once

#define STR_EMPTY ""
#define MAX_PROTOCOLS 20
#define AVAILABLE_PROTOCOLS         4

// Export Dialog

#define defSTR_BAUD_RATE_FORMAT     _T("Channel %-2d -> %s kbps")

#define defSTR_EXPORT_DIALOG_CAPTION    _T("Select Export Filename...")
#define defSTR_CSV_FORMAT_SPECIFIER _T("Comma Separated Values format (*.csv)|*.csv||")
#define defSTR_CSV_FORMAT           _T("csv")
#define defSTR_EXPORT_CREATE_SUCCESS _T("CSV report successfully generated and saved at %s")
#define defSTR_CSV_FILE_PATH_EMPTY      _T("Please enter valid CSV report file path")

#define defSTR_REPORT_DIALOG_CAPTION    _T("Select Report Filename...")
#define defSTR_HTML_FORMAT_SPECIFIER _T("HTML Report Format (*.html)|*.html||")
#define defSTR_HTML_FORMAT           _T("html")
#define defSTR_REPORT_CREATE_SUCCESS _T("HTML report successfully generated and saved at %s")
#define defSTR_HTML_FILE_PATH_EMPTY      _T("Please enter valid HTML report file path")

#define defSTR_BITMAP_DIALOG_CAPTION    _T("Select Picture Filename...")
#define defSTR_BMP_FORMAT_SPECIFIER _T("Bitmap Picture Format (*.bmp)|*.bmp||")
#define defSTR_BMP_FORMAT           _T("bmp")
#define defSTR_bmp_FILE_PATH_EMPTY      _T("Please enter valid bitmap file path")
#define defSPACE_STR            _T(" ")

#define defSTR_DEFAULT_BROWSE_LOCATION  _T("about:blank")

// To copy the data and advance the pointer of the target data stream
#define COPY_DATA(TgtStream, SrcStream, TotBytes) {memcpy(TgtStream, SrcStream, TotBytes); TgtStream += TotBytes;}

// To copy the data and advance the pointer of the source data stream
#define COPY_DATA_2(TgtStream, SrcStream, TotBytes) {memcpy(TgtStream, SrcStream, TotBytes); SrcStream += TotBytes;}

// Configuration
#define defIMPORT_WARNING_GRAPH   _T("Dissociation will clear Graph element List for all buses.\nDo you want to Clear the List?")
// Default values
#define defDEFAULT_BUFFER_SIZE      5000   // 5000 Entries
#define defDEFAULT_UPDATE_RATE      1000    // One Second

#define defDEFAULT_FRAME_COLOR      ( RGB( 0, 85, 0 ) )
#define defDEFAULT_PLOT_AREA_COLOR  ( RGB( 0, 0, 0 ) )
#define defDEFAULT_GRID_COLOR       ( RGB( 192, 192, 192 ) )
#define defDEFAULT_AXIS_COLOR      ( RGB( 255, 0, 0) )

#define defDEFAULT_FRAME_TYPE       1

#define defDEFAULT_X_GRID_LINES     10
#define defDEFAULT_Y_GRID_LINES     5

// Zoom Mode sub catogory
#define defZOOM_X_AXIS                  0
#define defZOOM_Y_AXIS                  1
#define defZOOM_XY_AXIS                 2

// Track Modes
#define defTRACK_MODE_NONE              0
#define defTRACK_MODE_TOOLTIP           1
#define defTRACK_MODE_ZOOM              2
#define defTRACK_MODE_PAN_XY            3
#define defTRACK_MODE_PAN_X             4
#define defTRACK_MODE_PAN_Y             5

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

// Graph Left View
#define defSTR_LIST_HEADING_CATOGORY        _T("Category")
#define defSTR_LIST_HEADING_ELEMENT         _T("Element")
#define defSTR_LIST_HEADING_TYPE            _T("Type")

#define defCOL_CATOGORY_INDEX               0
#define defCOL_ELEMENT_INDEX                1
#define defCOL_TYPE_INDEX                   2

#define defCOL_CATOGORY_WIDTH               69
#define defCOL_ELEMENT_WIDTH                79
#define defCOL_TYPE_WIDTH                   50

#define defELEMENT_SELECTION_CATOGORY_WIDTH     95
#define defELEMENT_SELECTION_ELEMENT_WIDTH      105
#define defELEMENT_SELECTION_TYPE_WIDTH         65

#define desSTR_INVALID_MAINFRAME_POINTER    _T("Invalid main frame pointer")
#define MSG_MEMORY_CONSTRAINT   _T("Insufficient Memory available")
#define defSTR_GRAPH_WINDOW_NAME            _T("Graph Display")

#define defSTR_DB_MSG_NAME      "Database Messages"
#define defSTR_STATISTICS_NAME  "Statistics"
#define defSTR_RAW_VALUE        "Raw"
#define defSTR_PHY_VALUE        "Physical"

#define defSTR_SHOW             "Show"
#define defSTR_HIDE             "Hide"

#define defSTR_ENABLE             "Enable"
#define defSTR_DISABLE            "Disable"
#define defSTR_ELEMENT_NOT_FOUND    "Element not found in the list. Please remove dead elements!!"

#define defMAX_ELEMENTS_COUNT       10
#define defSTR_MAX_ELEMENTS_EXCEEDED_MSG    _T("Only 10 graph elements are supported. Please delete existing element to add new element")

#define defSTR_INVALID_RANGE        _T("Please enter valid range value")


#define defSTR_SHOW_GRID        _T("Show Grid")
#define defSTR_HIDE_GRID        _T("Hide Grid")
#define defUPDATE_TIMER_ID      100

#define defDEFAULT_XRANGE_MAX   10
#define defDEFAULT_XRANGE_MIN   0

#define defSTR_Y_AXIS_FORMAT        _T("%0.4f")
#define defSTR_X_AXIS_FORMAT        _T("%0.3fS")

#define WM_CONFIG_CHANGE            WM_USER + 58 // Configuration Change Event
#define WM_NOTIFICATION_FROM_OTHER  WM_USER + 43

#define defDEFAULT_LINE_WIDTH           1
#define defSELECTED_LINE_WIDTH          2

#define defSIGNAL_ICON_SIZE               16

#define defTIME_STAMP_FORMAT "%02d:%02d:%02d:%04d"

// Graph window splitter details
struct tagGraphSplitterData
{
    int m_nRootSplitterData[2][2];
    int m_nRightViewSplitterData[2][2];
};

typedef tagGraphSplitterData sGraphSplitterData;
typedef sGraphSplitterData SGRAPHSPLITTERDATA;
typedef SGRAPHSPLITTERDATA* PSGRAPHSPLITTERDATA;

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

enum eTIMEMODE
{
    eSYSTEM_MODE = 0,
    eABSOLUTE_MODE,
    eRELATIVE_MODE
};

typedef enum eELEMENT_TYPE
{
    eSTAT_PARAM,
    eRAW_VALUE,
    ePHY_VALUE
};

typedef enum eDISPLAY_TYPE
{
    eDISPLAY_NORMAL,
    eDISPLAY_STEPMODE_XY,
    eDISPLAY_STEPMODE_YX
};

// enumerators for obtaining information from the configuration file
typedef enum eCONFIGDETAILS
{
    MRU_C_FILE_NAME,    //  MRU C File
    LOG_FILE_NAME,      // log file information
    REPLAY_FILE_NAME,   // replay file information
    DATABASE_FILE_NAME, // database filename
    MRU_DLL_FILE_NAME,  // dll filename
    //MESSAGE_NAME,           // message name
    //MESSAGE_ID,             // message id
    SEND_MULTI_MSGS,    // multiple message
    CONTROLLER_DETAILS, // Controller information
    MSG_ATTRIBUTES,     // attributes of all the messages
    OLD_FILTER_DETAILS, // filter details
    MSG_DISPLAY_FILTER_DETAILS, // filter details of message Display
    TOOLBAR_DETAILS,     // toolbar status information
    MSG_BLOCK_COUNT,    // Message Block count
    SIGNAL_WATCH_LIST,   // Signal Watch List Pointer
    MSG_BUFFER_SIZE,    // Message Buffer Size
    SIGNAL_GRAPH_LIST,   // Graph List
    REPLAY_WND_PLACEMENT,
    OUT_WND_PLACEMENT,
    NOTIFICATION_WND_PLACEMENT,
    MSG_WND_PLACEMENT,
    SIGWATCH_WND_PLACEMENT,
    MSGINTERP_WND_PLACEMENT,
    GRAPH_WND_PLACEMENT,
    TX_WND_PLACEMENT,
    TX_MSG_WND_SPLITTER_DATA,    // Tx Msg window Spliter Data
    GRAPH_WND_SPLITTER_DATA,
    CONFIG_FILE_VERSION,
    SIMSYS_WND_PLACEMENT,
    SIMSYS_LIST,  // pointer to the array of simulated system configuration paths
    LOG_CONFIG_DETS,
    FILTER_CONFIG_DETS,
    REPLAY_CONFIG_DETS
};

// Child Frame
#define defGRAPH_WINDOW_ROWS        1
#define defGRAPH_WINDOW_COLS        2

#define defLEFT_VIEW_PROPOTION      0.25
#define defRIGHT_VIEW_PROPOTION     0.75

#define defGRAPH_VIEW_PROPOSION     (2.0 / 3.0 )
#define defBOTTOM_VIEW_PROPOSION    (1.0 / 3.0 )

#define defGRAPH_WINDOW_RIGHT_ROWS  2
#define defGRAPH_WINDOW_RIGHT_COLS  1

#define defTIMER_RESOLUTION_FACTOR     10000.0

#define defGRAPH_WND_TOP_MARGIN            0.05
#define defGRAPH_WND_BOTTOM_MARGIN         0.25
#define defGRAPH_WND_LEFT_MARGIN           0.05
#define defGRAPH_WND_RIGHT_MARGIN          0.05

#define defSTR_CHANNEL_NAME                _T("Channel")
#define defSTR_CHANNEL_NAME_FORMAT         _T("%s %d")

#define WM_USER_CMD                 WM_USER + 44

typedef enum eUSERSELCTION
{
    eHEXDECCMD = 0,
    eTXMSGCMD,
    eCONNECTCMD,
    eDATABASEIMPORTCMD,
    eCONFIGCHANGECMD,
    eCHANNELCOUNTUPDATED
};
