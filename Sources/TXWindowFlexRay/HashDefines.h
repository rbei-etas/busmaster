/**********************************************************************************
    Project         :   Frame
    FileName        :   HashDefines.h
    Description     :   Declaration of Hash defines
    Directory       :
    Version         :   V1.0
    Authors         :   Ravi D
    Date Created    :   19/10/2006
    Date Modified   :
    Modifications   :
**********************************************************************************/

#pragma once

#define BIT_TM_ABS              0x01
#define BIT_TM_REL              0x02
#define BIT_TM_SYS              0x04
#define BIT_NUM_HEX             0x08
#define BIT_NUM_DEC             0x10

#define SET_TM_ABS(Flag)        (Flag |= BIT_TM_ABS)
#define SET_TM_REL(Flag)        (Flag |= BIT_TM_REL)
#define SET_TM_SYS(Flag)        (Flag |= BIT_TM_SYS)
#define SET_NUM_HEX(Flag)       (Flag |= BIT_NUM_HEX)
#define SET_NUM_DEC(Flag)       (Flag |= BIT_NUM_DEC)

#define IS_TM_ABS_SET(Flag)     (Flag & BIT_TM_ABS)
#define IS_TM_REL_SET(Flag)     (Flag & BIT_TM_REL)
#define IS_TM_SYS_SET(Flag)     (Flag & BIT_TM_SYS)
#define IS_NUM_HEX_SET(Flag)    (Flag & BIT_NUM_HEX)
#define IS_NUM_DEC_SET(Flag)    (Flag & BIT_NUM_DEC)

#define CLEAR_EXPR_FLAG(Flag)   (Flag &= 0x0)


#define DISP_MODE_OVER          0x20
#define DISP_MODE_APPEND        0x40
#define DISP_MODE_INTRP         0x80

#define CLEAR_EXPR_NUM_BITS(Flag)    (Flag &= (~BITS_NUM))
#define CLEAR_EXPR_DISP_BITS(Flag)   (Flag &= (~BITS_DISP))
#define CLEAR_EXPR_TM_BITS(Flag)     (Flag &= (~BITS_TM))


// In any scenario these two bits are mutually exclusive, as far as the
// current implementation is concerned.
#define SET_MODE_OVER(Flag)     (Flag = CLEAR_EXPR_DISP_BITS(Flag) | DISP_MODE_OVER)
#define SET_MODE_APPEND(Flag)   (Flag = CLEAR_EXPR_DISP_BITS(Flag) | DISP_MODE_APPEND)
#define SET_MODE_INTRP(Flag)    (Flag = CLEAR_EXPR_DISP_BITS(Flag) | DISP_MODE_INTRP)

#define IS_MODE_OVER(Flag)      (Flag & DISP_MODE_OVER)
#define IS_MODE_APPEND(Flag)    (Flag & DISP_MODE_APPEND)
#define IS_MODE_INTRP(Flag)     (Flag & DISP_MODE_INTRP)

#define defBITS_IN_FOUR_BYTE              32
const int TX_MESSAGE   = 0x10000000;  // bitwise OR to make it a Tx message
const int RX_MESSAGE   = 0x20000000;  // bitwise OR to make it a Rx message
const int CH_A_MESSAGE = 0x40000000;  // bitwise OR to make it a Channnel A message
const int CH_B_MESSAGE = 0x80000000;  // bitwise OR to make it a Channnel B  message



#define FIRST_COL       0       // First column of the List control.
#define SECOND_COL      1       // Second column of the List control.
#define THIRD_COL       2       // Third column of the List control.
#define FOURTH_COL      3
#define FIFTH_COL       4
#define SIXTH_COL       5
#define SEVENTH_COL     6       //Data Bytes
#define EIGHT_COL       7
#define NINTH_COL       8
#define TENTH_COL       9


#define LEN_10          10
#define LEN_256         256
#define LEN_1024        1024

#define TX_DATA_LENGTH  0
#define TX_DATA_BYTE    1

#define defSTR_FRAME_NAME           "Table/Frame"
#define defSTR_FRAME_ID             "ID(hex)"
#define defSTR_FRAME_TYPE           "Type"
#define defSTR_DELAY                "Delay[ms]"

#define INITIAL_DB_INDEX 1
#define FIBEXTREE_ROOTNAME _("Fibex Databases...")
#define ERROR_IN_OBJECTCREATION _("Exception in Fibex object Creation\nPlease Reload the DLL")
#define SIG_VALUEDESC_ERROR _("Already value description exists!")
#define CONFIRM_MSG _("Do you want to delete?")
#define SIG_DATA_MSG _("Data Generating...")
#define SIG_DATA_EXCEL_ERROR _("Couldn't CreateDispatch on Excel")
#define DLL_MEMORY_CREATION_ERROR _(" Dll Memory Creation Error!")
#define NODE_ERROR _("Node Already Exists!")
#define LIST_ERROR _("Please Select the item!")
#define FRAME_SUCCESS_MSG _(" Frame Added Successfully ")
#define FRAME_FAIL_MSG _(" Frame Already Exists!")
#define SIG_ADDITION_ERROR _("Frame Length is not Sufficient!")
#define FRAMEID_EMPTY _("Enter Frame Id!")
//#define CONFIGTOOL_PATH _("..\\FIBEXEDITOR\\ConfigurationTool\\FlexRayConfigurationTool.exe")
#define CONFIGTOOL_PATH _("\\ConfigurationTool\\FlexRayConfigurationTool.exe")
#define CONFIGTOOL_CFGPATH _("\\ConfigurationTool\\DefaultFlexRayConfiguration.cfg")
#define CONFIGTOOL_CFGNAME _("DefaultFlexRayConfiguration.cfg")
#define CUSTOMCFGSELECTION_EMPTY _("Enter Flexray Configuration file")
#define FRAME_MAX_LENGTH 4
#define SIGNAL_MAX_LENGTH 4
#define SIGNAL_FAIL_MSG _("Signal Already Exists!")
#define SIG_DATA_ERROR _(" Max and Min Value is Zero  ")
#define DB_CLOSE_MSG _(" Do you want to close active DB?")
#define DB_SAVE_SUCCESS_MSG _T (" Saved Successfully!")
#define DB_ERROR_MSG _T (" Fibex Tree is Empty !")
#define SNAME_MSG _T ( " Enter Short Name ")
#define SIG_FRAMEREF_ERROR _( "This Signal is referred By Frame\n You Can't Delete the Signal ")
#define SIG_EMPTY _("Signal Collection is empty!")
#define MOTO_BYTEORDER "Motorola"
#define INTEL_BYTEORDER "Intel"
#define SIGNED_MAXBITLENGTH 8
#define UNSIGNED_MAXBITLENGTH 8
#define BASE10 10
#define BASE2 2
#define MINFRAMEBYTE 2
#define MAXFRAMEBYTE 255
#define DEFAULT_SIGTYPE 2
#define DEFAULT_SIGLEN 8
#define DEFAULT_MAXLEN 255
#define DEFAULT_MINLEN 0
#define DEFAULT_UNSIGLEN 8
#define DEFAULT_SNSIGLEN 8
#define DEFAULT_UNMAXLEN 255
#define DEFAULT_UNMINLEN 0
#define DEFAULT_SNMAXLEN 255
#define DEFAULT_SNMINLEN 256
#define SIG_MAXLENGTH_ERROR _("Max Value is incorrect")
#define SIG_MINLENGTH_ERROR _("Min Value is incorrect")
#define HEADER_PROJECT _("Project")
#define HEADER_FRAMES _("Frames")
#define HEADER_SIGNALS _("Signals")
#define HEADER_FIBEX _("Fibex")
#define FRAMESIGINSTANCE_OFFSET_OVERLAP_ERROR _("Given Offset is Overlapped!")
#define BYTELEN_ERROR _("Enter Byte Length between 1 and 255!")
#define FRAMESIG_ERROR _("Select Signal Id!")
#define FRAMESIG_EMPTY _("Signal Collection is Empty!")
#define BITPOSITION_LIMIT 8
#define FRAME_FULL _("Frame is  occupied!")
#define BITPOSITION_OCCUPY _(" Position is occupied by other Signal !")
#define BITPOSITION_SPACE  _(" Space is not available!")
#define FRAMELEN_ERROR _(" Some Signal is available in existing position.\n You can't reduce the size!")
#define SIG_DEFAULTVALUE_MINERROR _(" Default value is greater than Minimum Value ")
#define SIG_DEFAULTVALUE_MAXERROR _(" Default value is lesser than Maximum Value ")
#define DEFAULTVALUE_ASSIGNMESSAGE _(" assigned to Default Value ")
#define RESOURCE_ERROR _(" Resource Error \n Try Again!")
#define FRAME_EMPTY _("Frame Collection is Empty!")
#define SENDVALUE_FLEXCOM _(" sent to Flexcom Application")
#define SIGMAX_LENGTH 32 //2048 
#define SIGMAX_LENGTHERROR _("Maximum Signal length is less than 32")
#define SIGNALID_EMPTY _("Enter Signal Id!")
#define SIGNALSHORTNAME_EMPTY _("Enter Signal Short Name")
#define PROJECTSHORTNAME_EMPTY _("Enter Project Short Name")
#define FRAMESIGINS_EMPTY _(" Frame is not having any Signals\n Please add Signal reference!")
#define FRAME_ERROR _("Please Save the Frame before sending it FlexCom")
#define CURRENT_FIBEX_VERSION "1.0"
#define FIBEXVERSION_ERROR _("Enter Fibex Version!")
#define FIBEXSTRUCTURE_EMPTY _("Fibex Structure is empty!")
#define FILE_NOTEXISTERROR _(" File does not exist!")
#define XMLFILE_EXTNERROR _(" Please give xml file name with .xml extension!")
#define FIBEXSTRUCTURE_EMPTY _("Fibex Structure is empty!")
#define FILE_NOTEXISTERROR _(" File does not exist!")
#define XMLFILE_EXTNERROR _(" Please give xml file name with .xml extension!")
#define NODENAME_EMPTY _("Enter Node Name!")
#define NODEVALUE_EMPTY _("Enter Node Description!")
#define SIGVALDESC_EMPTY _("Enter Signal Value Description!")
#define SIGVALVALUE_EMPTY _("Enter Signal value for description!")
#define PROJECTCHANGES_ERROR _("Can't Save Modification Details without Change Value!")
#define COMPANYDATAREF_ERROR _("Can't Save the Company Data Details without Company Data Ref Value!")
#define SAVE_MSG _("Successfully FIBEX information Saved!")
#define SAVEDB_ERROR _("You can't save FIBEX information without Project information!")
#define XMLREAD_ERROR _("Error in openning xml file!\nplease load correct xml file")
#define VERSION_INFOERR _("Version info is not available!")
#define PROJECT_INFOERR _("Project info is not available!")
#define DBERR_MSG _("Enter Database Name!")
#define RECENTCOLLECTIONS_MAXLENGTH 5
#define RECENTDATABASEs_INIFILE_PATH _("\\RecentDatabases.ini")
#define RECENTDATABASEs_INIFILE_ERROR _("Error in opening ini file!")
#define RECENTFILE_ERROR _("Error in opening recent file!")
#define HEADER_PROPERTY _("Properties")
#define FILE_MINLENGTH 5
#define FILE_MAXLENGTH 30
#define COMMON_TBARTITLE _("FIBEXEdit-Common")
#define PROJECT_TBARTITLE _("FIBEXEdit-Project")
#define FRAME_TBARTITLE _("FIBEXEdit-Frame")
#define SIGNAL_TBARTITLE _("FIBEXEdit-Signal")
#define GET_FORMULA 1 // Temp Value
#define GET_DEFVALUE 2 // Temp Value
#define UPDATE_FIBEXTREE 18 //  any Unique vale
#define UPDATE_LIST 17 //  any Unique vale
#define SIGVALUE_EXISTS _(" Already Value Exists!")
#define ZERO 0
#define FRAMECYCLENUM_MIN 0
#define FRAMECYCLENUM_MAX 63
#define TRANSMISSION_MSG _("Transmission is going on...")
#define CONFIG_MSG _("H/W Configuration Level is  not proper(Level = Red)\nPlease configure once again!")
#define CONFIGLEVEL_RED 2
#define CONFIGLEVEL_UNKNOWN 4

#define PROJECTVIEW _("PROJECT VIEW")
#define FRAMEVIEW _("FRAME VIEW")
#define SIGNALVIEW _("SIGNAL VIEW")
#define PROPERTYVIEW _("PROPERTY VIEW")
#define TESTREPORTVIEW _("TEST REPORT VIEW")
#define FILELENGTH_MIN 5
#define DBINVALIDERR_MSG _("Invalid Fibex Database!")
#define FRAME_MINLEN 2
#define FRAME_MAXLEN 255

// update hints
#define UPDATE_FONT     0
#define UPDATE_MSGWND   1
#define UPDATE_SAVE     2

#define APPLICATION_NAME _("FRAME")
#define EXPORTTOEXCEL_SYNTAXERROR _("Could not convert!")
#define EXPORTTOEXCEL_SUCCESSMSG _("Successfully converted text file to Excel File!")
#define EXPORTTOEXCEL_FIELDSELECTIONERROR _("No fields selected!")
#define EXPORTTOEXCEL_LOGFILEERROR _("Enter Log file details!")
#define EXPORTTOEXCEL_CSVFILEERROR _("Enter CSV file details!")
#define EXPORTTOEXCEL_FILEERROR _("Enter Log and CSV file details!")
#define EXPORTTOEXCEL_LOGFILEOPENERROR _("Log file does not exist!")
#define EXPORTTOEXCEL_CSVFILEOPENERROR _("CSV file does not exist!")

// User defined Window messages
#define WM_LSTC_DBLCLK              (WM_USER + 8)
#define WM_OWNER_DATA_NOTIFICATION  (WM_USER + 48)

#define WM_RX_COMPLETE              (WM_USER + 10)
#define WM_TX_COMPLETE              (WM_USER + 11)
#define WM_HW_STATUS                (WM_USER + 12)
#define WM_HW_NETWORK_STATISTICS    (WM_USER + 13)
#define WM_MODIFY_VISIBILITY        (WM_USER + 14)
#define WM_UPDATE_FONT              (WM_USER + 15)
#define WM_UPDATE_MSGWNDCFG         (WM_USER + 16)
#define WM_WND_PROP_MODIFY          (WM_USER + 17)
#define WM_WND_LOG_PROPERTY         (WM_USER + 18)
#define WM_WND_HW_ACTION            (WM_USER + 19)
#define WM_LOG_STATUS_MODIFY        (WM_USER + 20)
#define WM_START_STOP_CYCLIC_TX     (WM_USER + 21)
#define WM_HW_CONFIGURE             (WM_USER + 22)
#define WM_UPDATE_SENDBUTTON        (WM_USER + 24)
#define WM_CONFIG_QUESTION          (WM_USER + 25)
#define WM_PROVIDE_WND_PROP         (WM_USER + 26)
#define WM_TX_EDIT_COMPLETE         (WM_USER + 27)
#define WM_LOAD_HW_ICON             (WM_USER + 28)
//To manage msg colour
#define WM_MSG_COLOR_MODIFY         (WM_USER + 29)
//#define WM_MSG_FILTER_STATUS        (WM_USER + 30)
#define WM_IS_HW_ACTIVE             (WM_USER + 31)
#define WM_MSG_INTERPRETATION       (WM_USER + 32)
#define WM_IS_SENDMSG_PRESENT       (WM_USER + 33)
#define WM_NOTIFICATION_FROM_OTHER  (WM_USER + 34)
#define WM_LOAD_SAVE_WINSTATUS      (WM_USER + 35)
#define WM_THREADMSG_PROC           (WM_USER + 36)
#define WM_TRACE_WND_DISP           (WM_USER + 37)
#define ON_LIST_ITEM_CLICK          (WM_USER + 38)

#define IDC_LSTB_OUTPUT             300

typedef enum
{
    LOAD_CONFIG,
    SAVE_CONFIG,
    NEW_CONFIG,
    IS_CONFIG_CHANGED
};

typedef enum eWINDOWID
{
    eWINID_INVALID = 0,
    eWINID_MSGWND,
    eWINID_MSG_INTERPRETATION,
    eWINID_TRACEWND,
    eWNDID_GRAPHWND,
    eWINID_INTERPRET_WND_SAVE,
    eWINID_INTERPRET_WND_LOAD,
    eLOAD_DATASET_FRAME,
    eWINID_NETWORK_STATISTICS,
};

#define defSTR_CONF_SAVE_CONFIRM     _("Configuration File is modified \nDo you want to save this configuration?")
#define defSTR_CONF_CLOSE_CONFIRM    _("Do you want to close the application?")
#define VALUE_COLUMN    1

// For Log Header; added by Arun on 12th Jan, 2007
#define FLX_LOGDATE_FORMAT                  "%c%02d:%02d:%04d::%02d:%02d:%02d:%04d%c"
#define FRAME_VERSION                       "***FRAME v1.0***"
#define FRAME_LOG_HEADER                    "***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***"
#define FRAME_LOG_START                     "***[START LOG SESSION]***"
#define FRAME_LOG_STOP                      "***[STOP LOG SESSION]***"
#define FRAME_LOG_HEXFORMAT                 "***HEX***"
#define FRAME_LOG_DECFORMAT                 "***DEC***"
#define FRAME_LOG_ABSMODE                   "***ABSOLUTE MODE***"
#define FRAME_LOG_RELMODE                   "***RELATIVE MODE***"
#define FRAME_LOG_SYSMODE                   "***SYSTEM MODE***"
#define FRAME_CHANNEL_A                     "A"
#define FRAME_CHANNEL_B                     "B"
#define FRAME_CHANNEL_AB                    "AB"
#define FRAME_LOG_COLUMNS                   "***<Time><Tx/Rx><FrameID><FrameLength><Cycle><Channel><DataBytes>***"
#define FRAME_LOG_DIRECTORY                 "\\Sample\\Log\\"
#define FRAME_LOG_BACKSLASH                 "\\"
#define FRAME_LOG_REMOVE                    _("Do you want to remove selected log file entry?")
#define FRAME_LOG_ALREADYEXISTS             _("Log file: %s is already added to the log configuration list!")
#define FRAME_REPLAYLOG_ALREADYEXISTS       _("Replay log: %s already exists!")

#define FLX_MAX_LOG_FILE_NAME_ITERATION     250
#define FLX_NEW_LOGFILENAME                 "FlexRayLogFile%d.log"
#define FLX_LOG_FILE_EXTENSION              "log"
#define FLX_LOG_FILTER                      "*.log|*.log||"
#define FLX_MAX_FILE_PATH                   300
#define FLX_LOG_SELECTION_TITLE             _("Select a Log file")
#define FLX_LOG_COL_NAME                    _("Log File")
#define FLX_LOG_FILE_COL_WIDTH              0.95

#define ENTER_PERIOD_MESSAGE                _("Enter Period between 1 to 65535")
#define HARDWARE_NOT_CONNECTED              _("Hardware not connected!")
#define NO_MESSAGE_SELECTED                 _("No message selected in Transmit window!")
#define NO_ROW_PRESENT                      _("No Message present in Transmit window!")

#define FLX_INI_FILE_EXTENSION              "ini"
#define FLX_INI_FILTER                      _("INI Files(*.ini)|*.ini||")
#define FLX_CHI_FILE_EXTENSION              "chi"
#define FLX_CHI_FILTER                      _("CHI Files(*.chi)|*.chi||")
#define FLX_MDB_FILE_EXTENSION              "mdb"
#define FLX_MDB_FILTER                      _("Access Database Files(*.mdb)|*.mdb||")
#define FLX_XML_FILE_EXTENSION              "xml"
#define FLX_XML_FILTER                      _("XML Database Files(*.xml)|*.xml||")

#define STATIC_MSG_DATA_LENGTH              _("Data length of static messages can not be changed")
#define DYNAMIC_MSG_DATA_LENGTH             _("Dynamic data length is more than the set value")

//----------------------------------------------------------------------------
// IUciFlxCtrl Interface dependant constants and macros
//----------------------------------------------------------------------------
//*** Header Info Flag Mask

#define UCI_FLXHDRINFO_CHANNELID                      0x00000001
#define UCI_FLXHDRINFO_PAYLOADPREAMBLE                0x00000002
#define UCI_FLXHDRINFO_NULLFRAME                      0x00000004
#define UCI_FLXHDRINFO_SYNCFRAME                      0x00000008
#define UCI_FLXHDRINFO_STARTUPFRAME                   0x00000010
#define UCI_FLXHDRINFO_VALIDCOMMUNICATIONEVENT        0x00000020
#define UCI_FLXHDRINFO_SYNTAXERROR                    0x00000040
#define UCI_FLXHDRINFO_CONTENTERROR                   0x00000080
#define UCI_FLXHDRINFO_BOUNDARYVIOLATION              0x00000100
#define UCI_FLXHDRINFO_TXCONFLICT                     0x00000200
#define UCI_FLXHDRINFO_FRAMETYPE                      0x00000400
#define UCI_FLXHDRINFO_SELFRECEPTION                  0x00000800
#define UCI_FLXHDRINFO_BOTHCHANNELS                   0x00001000
#define UCI_FLXHDRINFO_ASYNCMODE                      0x00002000

#define UCI_FLXHDRINFO_TXSTATICMSG                    0x00000001
#define UCI_FLXHDRINFO_TXDYNAMICMSG                   0x00000002
#define UCI_FLXHDRINFO_CHANNELAENABLE                 0x00000004
#define UCI_FLXHDRINFO_CHANNELBENABLE                 0x00000008

#define UCI_FLXHDRINFO_CHANNELID_A                    0x00000000
#define UCI_FLXHDRINFO_CHANNELID_B                    0x00000001


#define TIMER_ID_DISPLAY_UPDATE           102
#define defDEF_DISPLAY_UPDATE_TIME        100
#define def_MSG_BUFFER_LENGTH             2000
typedef struct
{
    char m_omStrBlockName[260];
    BOOL m_bType;
    BOOL m_bActive;
    UINT m_unTimeInterval;
    char m_chblock_GUIID[100];
} sFrameMsgManager;

#define DEFAULT_PROJECT_NAME                         "Project_Name"
#define SECTION                                      _("Files")
#define defCONFIGFILENAME                            _("ConfigFile")
#define defFORMAT_MSGID_DECIMAL "%d"
#define defFORMAT_MSGID_HEX     "%X"
#define defFORMAT_SIG_VAL_DEC   "%I64d"
#define defFORMAT_SIG_VAL_HEX   "%I64X"

#define defCOLOR_WHITE           RGB(255,255,255)
#define defSIGNAL_LIST_STRING_MAX_LENGTH     50
#define defSIGNAL_FRAME_COLUMN      9
#define defSTR_COL_SIGNAL_NAME          _("Signal Name")
#define defSTR_FILTER_DETAILS_ID_FROM           _("From")
#define defSTR_FILTER_DETAILS_ID_TO             _("To")
#define defSTR_FILTER_DETAILS_ID_TYPE           _("ID Type")
#define defSTR_FILTER_DETAILS_MSG_TYPE          _("Msg Type")
#define defSTR_FILTER_DETAILS_MSG_DIRECTION     _("Direction")
#define defSTR_FILTER_DETAILS_MSG_CHANNEL       _("Channel")
#define defSTR_FILTER_NAME          _("Name")
#define defSTR_FILTER_TYPE          _("Type")
#define defSTR_MSG_DIR_TX                       _("Tx")
#define defSTR_MSG_DIR_RX                       _("Rx")
#define defSTR_SELECTION_ALL                    _("All")
#define defSTR_MSG_CHANNEL_A                    "A"
#define defSTR_MSG_CHANNEL_B                    "B"
#define defFILTER_IMAGE_INDEX_DB                0
#define defFILTER_IMAGE_INDEX_NDB               1
#define defFILTER_IMAGE_INDEX_FILTER            2
#define defFILTER_IMAGE_INDEX_ID_RANGE          3
#define defFILTER_TYPE_SINGLE_ID                0
#define defFILTER_TYPE_ID_RANGE                 1
#define defSTR_FILTER_TYPE_PASS                 _("Pass")
#define defSTR_FILTER_TYPE_STOP                 _("Stop")
#define defSTR_MSG_ID_FORMAT_DB                 "%x [%s]"
#define defSTR_MSG_ID_FORMAT_NDB                "%x"
#define defSTR_DEFAULT_MRU_ITEM                 _("Empty")
#define defSTR_FILE_MENU_TEXT                   _("&File")
#define defSTR_RECENT_MENU_TEXT                 _("Recent File")

#define defSECTION_MRU           _("MRU List")
#define defSECTION_MRU_FILE1     _("MRU File1")
#define defSECTION_MRU_FILE2     _("MRU File2")
#define defSECTION_MRU_FILE3     _("MRU File3")
#define defSECTION_MRU_FILE4     _("MRU File4")
#define defSECTION_MRU_FILE5     _("MRU File5")

//For adding MSgID to message name
#define defSTR_MSG_ID_IN_HEX          "[0x%x]"
#define MAX_LMT_FOR_MSG_ID          1064

enum eMSG_CHANGES_IN
{
    MSG_NAME    =   0,
    MSG_CODE,
    MSG_LENGTH,
    MSG_FRAME_FORMAT,
    MSG_DATA_FORMAT

};

#define defMSGID_NAME_DELIMITER  "["
#define defMSGID_NAME_START_CHAR '['
#define defMSG_NAME_END_CHAR     ']'
#define defNUMBER_OF_FILTERS_SUPPORTED          100
#define defSTR_DEFAULT_FILTER_NAME              _("Filter")
#define defSTR_DEFAULT_FILTER_FORMAT            "%s_%d"
#define TX_FLAG                 0x01
#define RX_FLAG                 0x02
#define BLUE_COLOR              RGB(0,0,255)
#define defSIGNAL_ICON_SIZE               16
#define defSTR_FILTER_NAME_COLUMN       _("Filter Name")
#define defBASE_DEC                 10
#define defBASE_HEX                 16
#define defFORMAT_INT64_DECIMAL "%I64d"
#define defFORMAT_INT64_HEX     "%I64X"
#define defSTR_FORMAT_PHY_VALUE  "%.3f"
#define defCHAR_PATH_SEPRATER    '\\'
#define defMAX_BITS             64
#define defCRETATE_SUCCESS                  0
#define defCREATE_FAILED                    -1

#define SIGNED_VAL  1
#define FLOAT_VAL   2
#define BUDDY_CTRL  4

#define defNO_OF_CHANNELS           2


#define IS_BUDDY_ENABLED(x)  ((x) & (BUDDY_CTRL) ? TRUE : FALSE )
#define IS_FLOAT_ENABLED(x)  ((x) & (FLOAT_VAL) ? TRUE : FALSE )
#define IS_SIGNED_NUMBER(x)  ((x) & (SIGNED_VAL) ? TRUE : FALSE )

#define defFLC_CREATE_FAILED                _("Failed to create %s Control")
#define defCOMBO_ITEM                       _("ComboItem")
#define defCOMBO_LIST                       _("ComboLsit")
#define defEDIT_ITEM                        _("EditItem")
#define defNUM_ITEM                         _("NumItem")
// Item type and Numeric details table sizes
#define defLIST_ITEM_TYPE_TABLE_SIZE        131
#define defLIST_NUM_DETAILS_TABLE_SIZE      67
// Virtual column count. This should be greater then the actual column count
#define defVIRTUAL_COL_COUNT                50
#define defNO_OF_MSG_COLORS                 4
// Version info ::
// 1.0 = saving of log info,filter info
// 1.1 = saving of log and filter enable flag,msg color
#define FRAME_APPN_VERSION       1.1
#define FRAME_UNIQUE_ID          11235
#define defSTR_COLLAPSE_MENU_TEXT    _("&Collapse")
#define DB_MSG_NAME_SIZE         64
#define MIN_FILE_NAME_SIZE       3

#define defMSG_IPET_WND_HEIGHT_RATIO        0.35

#define defMSG_IPET_WND_BORDER_WIDTH        1


#define COLOUR_NULL_FRAME       RGB(  0,   0,   0)
#define COLOUR_DYNC_FRAME       RGB(  0, 128,   0)
#define COLOUR_STAT_FRAME       RGB(128,   0,   0)
#define COLOUR_SYNC_FRAME       RGB(  0,   0, 128)

enum eMESSAGEFROM
{
    FNTREEVIEW = 0,
    OUTWND,
    FNVIEW,
    SENDMESGBUTTON
};

#define INT_INIT                (INT)0
#define LONG_INIT               (LONG)0
#define DWORD_INIT              (DWORD)0
#define CHAR_INIT               (CHAR)0
#define defCOUNT_INIT              0//initial value of a counter

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

#define CHAR_WIDTH_MULTIPLY     100

#define EDIT_CAPTION            _("Enter Valid Integer :")
#define LIST_CAPTION            _("Select from the List below :")
#define COMBO_CAPTION           _("Select from the Combo box :")

// default no of chars in a tab
#define defNO_OF_CHARS_IN_TAB   4
#define MARGIN_FOR_FILE_VIEW    5
#define TAB_POSITION            1
#define TAB_ORIGIN              0
// for creating new fonts
#define DEFAULT_FONT            "Courier"
#define DEFAULT_FONT_ESCAPEMENT  0
#define DEFAULT_FONT_ORIENTATION 0
#define NOT_ITALIC               0
#define NO_UNDERLINE             0
#define NO_STRIKEOUT             0

#define NEXT_POSITION           1
#define START_POSITION          0


#define GET_CONTEXT_WND_CHAR_HEIGHT() \
    ((::GetDeviceCaps(hDeviceContext,LOGPIXELSY) * 10) / 72)
#define GET_CONTEXT_WND_CHAR_WIDTH() \
    ((::GetDeviceCaps(hDeviceContext,LOGPIXELSX) * 6) / 72)
#define GET_FILE_VIEW_CHAR_HEIGHT() \
    ((omDeviceContext.GetDeviceCaps (LOGPIXELSY) * 10) / 72)
#define GET_FILE_VIEW_CHAR_WIDTH() \
    ((omDeviceContext.GetDeviceCaps (LOGPIXELSX) * 6) / 72)

#define SPACE_BET_CHAR_IN_FILE_VIEW 20
#define SPACE_BET_LINE_IN_FILE_VIEW 5

#define SPLT_ONE_ROWS   1
#define SPLT_ONE_COLS   2

#define SPLT_TWO_ROWS   2
#define SPLT_TWO_COLS   1

#define FRAME_FN_HDR           _("/* Start FRAME generated function - \
PLACE_HODLER_FOR_FUNCTIONNAME */")

#define FRAME_FN_FOOTER        _("}/* End FRAME generated function - \
PLACE_HODLER_FOR_FUNCTIONNAME */")

#define FRAME_FN_PARTIAL_HDR   _("/* Start FRAME generated function - ")

#define FRAME_VAR_HDR          _("/* Start FRAME global variable */")
#define FRAME_VAR_FOOTER       _("/* End FRAME global variable */")

#define FRAME_INCLUDE_HDR      _("/* Start FRAME include header */")
#define FRAME_INCLUDE_FOOTER   _("/* End FRAME include header */")

#define FRAME_FN_PROTOTYPE_HDR _("/* Start FRAME Function Prototype  */")
#define FRAME_FN_PROTOTYPE_FOOTER   _("/* End FRAME Function Prototype  */")

#define UTILS_PREFIX            "Utils_"
#define OPEN_PARENTHESIS        "("
#define NO_OF_CHAR_IN_UTILS     6

#define GLOBAL_VAR_PREFIX         "g_"
#define MIN_CHAR_IN_GLOBAL_VAR    2
#define FILENAME_NOT_FOUND        _("File not found!")

#define defDOT_C                    ".C"
#define defDOT_DEF                  ".def"
#define defDOT_EXP                  ".exp"
#define defDOT_DLL                  ".dll"

#define MESSAGE_HANDLERS            _("Message Handlers")
#define TIMER_HANDLERS              _("Timer Handlers")
#define KEY_HANDLERS                _("Key Handlers")
#define defERROR_HANDLERS           _("Error Handlers")
#define defDLL_HANDLERS             _("DLL Handlers")
#define UTILITY_FUNCS               _("Utility Functions")
#define GLOBAL_VARIABLES            _("Global Variables")
#define INCLUDE_HEADERS             _("Include Headers")

#define SPACE                   ' '
#define SEMI_COLON              ';'
#define defDEFAULT_KEY_HANDLER_CODE "void OnKey_KEYNAME(unsigned char KeyValue)"

/*#define defDEFAULT_TIMER_HANDLER_CODE   _("void __stdcall OnTimer_TIMERNAME_TIMERVALUE(unsigned int uTimerID,\
unsigned int uMsg, unsigned long dwUser, unsigned long dw1, unsigned long dw2)")*/
#define defDEFAULT_TIMER_HANDLER_CODE _("void OnTimer_TIMERNAME_TIMERVALUE( )")

#define defTODO_FUNCTION_INSERT     _("/* TODO: Define and Initialise the following variables*/")
#define defNEW_LINE                 "\n"
#define defRETURN_TYPE_NAME         _("ReturnValue")
#define defPARAMETER_NAME           _("Para%d")
#define defTODO                     "/* TODO */"
#define defMSG_DUPL_MSG_HANDLER     _("Message handler for TYPE MESSAGENAME already exists!")
#define defDUPL_CORR_MSG_HANDLER     _("Corresponding Message handler for TYPE \"MESSAGENAME\" already exists!")
#define defMSG_DUPL_TMR_HANDLER     _("Timer handler already exists!")
#define defMSG_DUPL_KEY_HANDLER     _("Key handler for the key \"KEYNAME\" already exists!")
#define defMSG_DUPL_UTIL_FUNC       _("Utility function \"FUNCTIONNAME\" already exists!")
#define defMSG_DUPL_GLO_VAR         _("Global variable is already defined!")
#define defINIT_HANDLER_STRUCT_MSG  _("Unable to initialise handler functions for execution.")
#define defDLL_UNLOADED_MSG         _(" The DLL \"FILENAME\" is unloaded")
#define defOBJECT_BUILDPROGRAM_MSG  _("Unable to allocate memory for BuildProgram module")
#define defSTRMSG_RANGE_INVALID     _("Message ID range:\"FROM\" field has value greater then or equal to \"TO\" field")
#define defMESSAGE_HANDLER      "OnMsg"
#define defKEY_HANDLER          "OnKey_"
#define defTIMER_HANDLER        "OnTimer_"
#define defERROR_HANDLER_FN     "OnError_"
#define defDLL_HANDLER_FN       "OnDLL_"
#define defUTILS_FUNCTION_FN    "Utils_"
#define defMSG_NAME_HANDLER     "Name_"
#define defMSG_ID_HANDLER       "ID_"
#define defMSG_ALL              "_All"
#define defMSG_IDRANGE_HANDLER  "IDRange_"
// Function Editor
#define defCONFIRM_DELETE_HANDLER   _("You are about to delete the function FUNCTION_NAME(...).\n\
Are you sure you want to delete?")

#define defVOID                     "void"
#define defCONFIRM_DELETE_GLOVAR    _("You are about to delete the variable.\n\
Do you wish to proceed?")

#define defCONFIRM_DELETE_INCHDR    _("You are about to delete the #include.\n\
Are you sure you want to delete?")
#define defDEFAULT_MSG_HANDLER_CODE _("void OnMsg%s(%s RxMsg)")

#define defDEFAULT_KEY_HANDLER_CODE _("void OnKey_KEYNAME(unsigned char KeyValue)")

#define defDEFAULT_TIMER_HANDLER_CODE _("void OnTimer_TIMERNAME_TIMERVALUE( )")

#define defERROR_HANDLER_NUMBER  5
#define defDLL_HANDLER_NUMBER    2
#define defERROR_HANDLER_TEXT_ADD "Add Error Handler(s)"
#define defERROR_HANDLER_TEXT_DEL "Delete Error Handler(s)"
#define defDLL_HANDLER_TEXT_ADD "Add DLL Handler(s)"
#define defDLL_HANDLER_TEXT_DEL "Delete DLL Handler(s)"
#define defGENERIC_KEY          '*'
#define defGENERIC_KEY_HANDLER_TEXT "All"
// Signal Type
#define defBOOLEAN          "bool"
#define defUNSIGNED_INT     "unsigned int"
#define defUNSIGNED_CHAR    "unsigned char"
#define defUNSIGNED_LONG    "unsigned long"
#define defUNSIGNED_SHORT   "unsigned short"
#define defSIGNED_SHORT     "short"
#define defSIGNED_CHAR      "char"
#define defSIGNED_INT       "int"
#define defSIGNED_LONG      "long"
#define defFLOAT            "float"

#define defMSG_NAME_TEXT        "message name"
#define defMSG_ID_TEXT          "message id"
#define defMSG_IDRANGE_TEXT     "message id range"
#define defMSG_ALL_TEXT         "all"
#define defSTR_FLEX_MSG         _("SFLEXMSG")
#define defSTR_MSG_SPECIFIC_HANDLER _("Name_")
#define defMESSAGE_NAME_INDEX       5
#define defDEFAULT_MSG_HANDLER_CODE _("void OnMsg%s(%s RxMsg)")

#define defDEFAULT_KEY_HANDLER_CODE _("void OnKey_KEYNAME(unsigned char KeyValue)")
#define MSG_RET_TYPE_EMPTY      _("Please select the return type for the function.")
#define WRAPPER_H               "Wrapper.h"
#define WRAPPER_H_STARTID       _("// Prototypes Start")
#define WRAPPER_H_ENDID         _("// Prototypes End")

enum eSELTYPE
{
    SEL_MESSAGE = 0,
    SEL_SIGNAL,
    SEL_GLOBAL_MESSAGE
};
#define defNUMBER_OF_TIMER           100
#define defFNS_INIT_SIG_FORMAT              "%s\n\n%s.%s"
#define defHASHINCLUDE                      "#include "
#define defSIGNALMEMBER                     "m_sWhichBit"
#define defSTR_INSERT_GLOBAL_DLG_CAPTION    _("Message Variable List")
#define defUNDERSCORE                       "_"
#define MSG_STRUCT_VAR                      "sMsgStruct"
#define PERIOD                              '.'
#define NEW_LINE                            "\n"
#define TVOID                               "void"
#define defMSG_DUP_TIMER_HANDLER            _("Duplicate timer handler name!")
#define defTIMER_FN_NAME                    "OnTimer_TIMERNAME_TIMERVALUE"

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


#define defSIMVALIDEXTN        ".sim"
#define defDOT_SMALL_C         ".c"
#define defDOT                 '.'

//Parameters for the CEvent objects array used in CExecuteFunc
#define defEVENT_EXFUNC_TOTAL        5
//define for CExecute utility thread
#define defKEY_HANDLER_THREAD        0
#define defERROR_HANDLER_THREAD      1
#define defDLL_LOAD_HANDLER_THREAD   2
#define defDLL_UNLOAD_HANDLER_THREAD 3
#define defMSG_HANDLER_THREAD        4

//no. of entries in CExecute func message handler buffer
#define defMAX_FUNC_MSG                     1000
//no. of entries in global message handler structure buffer
#define defMAX_HMODULE_MSG                  1000

#define defNUMEXTNCHARS     3

#define defMASK_CHECK_UNCHECK    0x00003000
#define defMASK_LIST_CTRL        0x00003003
// This enumeration defines current error state
//enum eERROR_STATE
//{
//    ERROR_ACTIVE  =   0,
//    ERROR_WARNING_LIMIT,
//    ERROR_PASSIVE,
//    ERROR_BUS_OFF,
//    ERROR_FRAME
//};
#define COPYFILE(SRC,DESTN)         ::CopyFile(SRC,DESTN,FALSE);
#define defCHANGEDSIMSYS           0x0000000000000001

#define defTMPFILEATTRIB    FILE_ATTRIBUTE_HIDDEN
#define defEMPTYSTR         ""
#define defSAVECONFIGERRSTR _("Error while saving file %s. Restoring old data")// for sim sys 
#define defCONFIGSAVEERR    _("File %s not saved properly. Please try again")// for sim sys 

// enumerators for obtaining information from the configuration file
typedef enum eCONFIGDETAILS
{
    MRU_C_FILE_NAME,    //  MRU C File
    LOG_FILE_NAME,      // log file information
    REPLAY_FILE_NAME,   // replay file information
    DATABASE_FILE_NAME, // database filename
    MRU_DLL_FILE_NAME,  // dll filename
    MESSAGE_NAME,           // message name
    MESSAGE_ID,             // message id
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
    SIMSYS_LIST  // pointer to the array of simulated system configuration paths
};
#define defBORDER_WIDTH_FACTOR              5
#define defCHILD_FRAME_PROP_H               0.75
#define defOUTPUT_WND_PROP_X                0.5
#define defREPLAY_WND_PROP_X                0.8
#define defNOTIFIC_WND_PROP_X               0.5

#define defMSG_WND_HEIGHT_PROPOTION         0.5

#define defSIGNAL_WATCH_WND_WIDTH_RATIO     0.65
#define defSIGNAL_WATCH_WND_HEIGHT_RATIO    0.35

#define defMSG_IPET_WND_WIDTH_RATIO         0.65
#define defMSG_IPET_WND_HEIGHT_RATIO        0.35

#define defGRAPH_WND_TOP_MARGIN            0.05
#define defGRAPH_WND_BOTTOM_MARGIN         0.25
#define defGRAPH_WND_LEFT_MARGIN           0.05
#define defGRAPH_WND_RIGHT_MARGIN          0.05

#define defTX_MSG_WND_BOTTOM_MARGIN        0.16
#define defTX_MSG_WND_RIGHT_MARGIN         0.17

#define defTX_SIM_WND_BOTTOM_MARGIN        0.05
#define defTX_SIM_WND_RIGHT_MARGIN         0.05

#define ALL_NODE_EDIT_CONFMN     _("You are about to change all the Node details!\n\nAre you sure you want to Continue?")
#define ALL_SIMSYS_DELETE_CONFMN _("You are about to Delete all the Simulated Systems.\n\nAre you sure you want to Delete?")
#define ASK_SIMSYS_SAVE_PROMPT   _("Do you want to save changes to the simulated system file(s)?")
#define defSIZE_OF_ERROR_BUFFER  1024
#define defSIZE_OF_IMAGE         25
#define defSTR_BUILD_TRACE_LINE_MARK       " "
#define defTIMER_HEADER_NAME               _("Timer Handler Name")
#define defTMPFILEEXTN                     "tmp"
#define MSG_MEMORY_CONSTRAINT              _("Insufficient Memory available")
#define SIMSYS_DELETE_CONFMN     _("You are about to Delete a Simulated System.\n\nAre you sure you want to Delete?")
#define ALL_SIMSYS_DELETE_CONFMN _("You are about to Delete all the Simulated Systems.\n\nAre you sure you want to Delete?")
#define ALL_NODE_EDIT_CONFMN     _("You are about to change all the Node details!\n\nAre you sure you want to Continue?")
#define NODE_DELETE_CONFMN       _("You are about to Delete a Node.\n\nAre you sure you want to Delete?")
#define ALL_NODE_DELETE_CONFMN   _("You are about to Delete all the Nodes.\n\nAre you sure you want to Delete?")


// Replay type of logged messages
enum eREPLAYTYPE
{
    eTX_MESSAGE = 0,
    eRX_MESSAGE,
    eALL_MESSAGE
};
enum eTIMEMODE
{
    eSYSTEM_MODE = 0,
    eABSOLUTE_MODE,
    eRELATIVE_MODE
};
#define FLEXRAY_BUS                  _("FLEXRAY Bus")
#define defERROR_IN_REGISTRY      _("Unable to obtain path of GCC from registry key")
#define defERROR_OPEN_REGISTRY    _("The registry value is corrupted. Please re-install.")
#define defERROR_OPEN_REGISTRY_FN    _("The registry value is corrupted. The interface function is not available.\nPlease re-install.")
#define defIMPORT_DLLLOAD_WARNNING   _("Please Unload all the Dlls and try again")
//Definitions for Major and Minor Versions

#define defMAJOR_VERSION                    1
#define defMINOR_VERSION                    0
//To get the Version Number from the application DLL
#define defUNSUPPORTED_VER                  _("Unsupported DLL Version %d.%d")
#define defOLD_VER                          _("%s is created with older version of the tool. Please recompile the source code to get latest version of DLL")

#define def_MSG_MAP_HASH_SIZE     257
#define defSTR_ERROR_IN_MSG_PROG   _("Exception occured in Message Handler ID : 0x%X")
#define defSTR_ERROR_IN_KEY_PROG   _("Exception occured in Key Handler of '%c'")
#define defSTR_ERROR_IN_ERR_PROG   _("Exception occured in Error Handler")
#define defSTR_ERROR_IN_DLL_LOAD   _("Exception occured in DLL Load Handler")
#define defSTR_ERROR_IN_DLL_UNLOAD _("Exception occured in DLL UnLoad Handler")
#define defSTR_ERROR_IN_TIMER_PROG _("Exception occured in Timer Handler \"%s\"")
const BYTE BIT_MSG_HANDLER_THREAD       = 0x01; // First bit
const BYTE BIT_KEY_HANDLER_THREAD       = 0x02; // Second bit
const BYTE BIT_TX_SEL_MSG_THREAD        = 0x04; // Third bit
const BYTE BIT_REPLAY_THREAD            = 0x08; // fourth bit
const BYTE BIT_DLL_LOAD_HANDLER_THREAD   = 0x10; // fifth bit
const BYTE BIT_DLL_UNLOAD_HANDLER_THREAD = 0x20; // sixth bit
const BYTE BIT_ERROR_HANDLER_THREAD      = 0x40; // Seventh bit
const BYTE BIT_MULTI_MSG_THREAD          = 0x80; // Eighth bit
#define defSTR_ERROR_LOADING_HANDLER    _("Failed to get address of %s Handlers: %s")
#define defSTR_DLL_HANDLER  "DLL"
#define defSTR_KEY_HANDLER  _("Key")
#define defSTR_MESSAGE_HANDLER  _("Message")
#define defSTR_ERROR_HANDLER  _("Error")
#define defSTR_TIMER_HANDLER  _("Timer")
// DLL unload handler wait time
#define defWAIT_DELAY_FOR_DLL_UNLOAD        500
#define MAX_TIME_LIMIT                      1000    // in miliseconds
#define defHEADER_FILE_NAME                 "_Unions.h"
// Simulated system Configuration window
#define defSTR_SIMSYS_WINDOW_TITLE              _("Configure simulated systems")
#define defSTR_SIMSYS_SAVE_CONFIRMATION         _("Do you want to save changes?")
