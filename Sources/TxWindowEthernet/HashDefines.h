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

#ifndef _HASH_DEFINES_H
#define _HASH_DEFINES_H
//#pragma pack(8)
//#pragma pack(show)

#define BIT_TM_ABS              0x01
#define BIT_TM_REL              0x02
#define BIT_TM_SYS              0x04
//const BYTE BITS_TM              = (BIT_TM_ABS | BIT_TM_REL | BIT_TM_SYS);
#define BIT_NUM_HEX             0x08
#define BIT_NUM_DEC             0x10
//const BYTE BITS_NUM             = (BIT_NUM_HEX | BIT_NUM_DEC);
//const BYTE BITS_EXPRN           = (BITS_TM | BITS_NUM);

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
//const BYTE BITS_DISP            = (DISP_MODE_OVER | DISP_MODE_APPEND |
//                                   DISP_MODE_INTRP);

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

#define INITIAL_DB_INDEX 1
#define FIBEXTREE_ROOTNAME _T("Fibex Databases...")
#define ERROR_IN_OBJECTCREATION _T("Exception in Fibex object Creation\nPlease Reload the DLL")
#define SIG_VALUEDESC_ERROR _T("Already value description exists!")
#define CONFIRM_MSG _T("Do you want to delete?")
#define SIG_DATA_MSG _T("Data Generating...")
#define SIG_DATA_EXCEL_ERROR _T("Couldn't CreateDispatch on Excel")
//#define APPLICATION_NAME _T("FIBEX EDIT")
#define DLL_MEMORY_CREATION_ERROR _T(" Dll Memory Creation Error!")
#define NODE_ERROR _T("Node Already Exists!")
#define LIST_ERROR _T("Please Select the item!")
#define FRAME_SUCCESS_MSG _T(" Frame Added Successfully ")
#define FRAME_FAIL_MSG _T(" Frame Already Exists!")
#define SIG_ADDITION_ERROR _T("Frame Length is not Sufficient!")
#define FRAMEID_EMPTY _T("Enter Frame Id!")
//#define CONFIGTOOL_PATH _T("..\\FIBEXEDITOR\\ConfigurationTool\\FlexRayConfigurationTool.exe")
#define CONFIGTOOL_PATH _T("\\ConfigurationTool\\FlexRayConfigurationTool.exe")
#define CONFIGTOOL_CFGPATH _T("\\ConfigurationTool\\DefaultFlexRayConfiguration.cfg")
#define CONFIGTOOL_CFGNAME _T("DefaultFlexRayConfiguration.cfg")
#define CUSTOMCFGSELECTION_EMPTY _T("Enter Flexray Configuration file")
#define FRAME_MAX_LENGTH 4
#define SIGNAL_MAX_LENGTH 4
#define SIGNAL_FAIL_MSG _T("Signal Already Exists!")
#define SIG_DATA_ERROR _T(" Max and Min Value is Zero  ")
#define DB_CLOSE_MSG _T(" Do you want to close active DB?")
#define DB_SAVE_SUCCESS_MSG _T (" Saved Successfully!")
#define DB_ERROR_MSG _T (" Fibex Tree is Empty !")
#define SNAME_MSG _T ( " Enter Short Name ")
#define SIG_FRAMEREF_ERROR _T( "This Signal is referred By Frame\n You Can't Delete the Signal ")
#define SIG_EMPTY _T("Signal Collection is empty!")
#define MOTO_BYTEORDER _T("Motorola")
#define INTEL_BYTEORDER _T("Intel")
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
#define SIG_MAXLENGTH_ERROR _T("Max Value is incorrect")
#define SIG_MINLENGTH_ERROR _T("Min Value is incorrect")
#define HEADER_PROJECT _T("Project")
#define HEADER_FRAMES _T("Frames")
#define HEADER_SIGNALS _T("Signals")
#define HEADER_FIBEX _T("Fibex")
#define FRAMESIGINSTANCE_OFFSET_OVERLAP_ERROR _T("Given Offset is Overlapped!")
#define BYTELEN_ERROR _T("Enter Byte Length between 1 and 255!")
#define FRAMESIG_ERROR _T("Select Signal Id!")
#define FRAMESIG_EMPTY _T("Signal Collection is Empty!")
#define BITPOSITION_LIMIT 8
#define FRAME_FULL _T("Frame is  occupied!")
#define BITPOSITION_OCCUPY _T(" Position is occupied by other Signal !")
#define BITPOSITION_SPACE  _T(" Space is not available!")
#define FRAMELEN_ERROR _T(" Some Signal is available in existing position.\n You can't reduce the size!")
#define SIG_DEFAULTVALUE_MINERROR _T(" Default value is greater than Minimum Value ")
#define SIG_DEFAULTVALUE_MAXERROR _T(" Default value is lesser than Maximum Value ")
#define DEFAULTVALUE_ASSIGNMESSAGE _T(" assigned to Default Value ")
#define RESOURCE_ERROR _T(" Resource Error \n Try Again!")
#define FRAME_EMPTY _T("Frame Collection is Empty!")
#define SENDVALUE_FLEXCOM _T(" sent to Flexcom Application")
#define SIGMAX_LENGTH 32 //2048 
#define SIGMAX_LENGTHERROR _T("Maximum Signal length is less than 32")
#define SIGNALID_EMPTY _T("Enter Signal Id!")
#define SIGNALSHORTNAME_EMPTY _T("Enter Signal Short Name")
#define PROJECTSHORTNAME_EMPTY _T("Enter Project Short Name")
#define FRAMESIGINS_EMPTY _T(" Frame is not having any Signals\n Please add Signal reference!")
#define FRAME_ERROR _T("Please Save the Frame before sending it FlexCom")
#define CURRENT_FIBEX_VERSION _T("1.0")
#define FIBEXVERSION_ERROR _T("Enter Fibex Version!")
#define FIBEXSTRUCTURE_EMPTY _T("Fibex Structure is empty!")
#define FILE_NOTEXISTERROR _T(" File does not exist!")
#define XMLFILE_EXTNERROR _T(" Please give xml file name with .xml extension!")
#define FIBEXSTRUCTURE_EMPTY _T("Fibex Structure is empty!")
#define FILE_NOTEXISTERROR _T(" File does not exist!")
#define XMLFILE_EXTNERROR _T(" Please give xml file name with .xml extension!")
#define NODENAME_EMPTY _T("Enter Node Name!")
#define NODEVALUE_EMPTY _T("Enter Node Description!")
#define SIGVALDESC_EMPTY _T("Enter Signal Value Description!")
#define SIGVALVALUE_EMPTY _T("Enter Signal value for description!")
#define PROJECTCHANGES_ERROR _T("Can't Save Modification Details without Change Value!")
#define COMPANYDATAREF_ERROR _T("Can't Save the Company Data Details without Company Data Ref Value!")
#define SAVE_MSG _T("Successfully FIBEX information Saved!")
#define SAVEDB_ERROR _T("You can't save FIBEX information without Project information!")
#define XMLREAD_ERROR _T("Error in openning xml file!\nplease load correct xml file")
#define VERSION_INFOERR _T("Version info is not available!")
#define PROJECT_INFOERR _T("Project info is not available!")
#define DBERR_MSG _T("Enter Database Name!")
#define RECENTCOLLECTIONS_MAXLENGTH 5
#define RECENTDATABASEs_INIFILE_PATH _T("\\RecentDatabases.ini")
#define RECENTDATABASEs_INIFILE_ERROR _T("Error in opening ini file!")
#define RECENTFILE_ERROR _T("Error in opening recent file!")
#define HEADER_PROPERTY _T("Properties")
#define FILE_MINLENGTH 5
#define FILE_MAXLENGTH 30
#define COMMON_TBARTITLE _T("FIBEXEdit-Common")
#define PROJECT_TBARTITLE _T("FIBEXEdit-Project")
#define FRAME_TBARTITLE _T("FIBEXEdit-Frame")
#define SIGNAL_TBARTITLE _T("FIBEXEdit-Signal")
#define GET_FORMULA 1 // Temp Value
#define GET_DEFVALUE 2 // Temp Value
#define UPDATE_FIBEXTREE 18 //  any Unique vale
#define UPDATE_LIST 17 //  any Unique vale
#define SIGVALUE_EXISTS _T(" Already Value Exists!")
#define ZERO 0
#define FRAMECYCLENUM_MIN 0
#define FRAMECYCLENUM_MAX 63
#define TRANSMISSION_MSG _T("Transmission is going on...")
#define CONFIG_MSG _T("H/W Configuration Level is  not proper(Level = Red)\nPlease configure once again!")
#define CONFIGLEVEL_RED 2
#define CONFIGLEVEL_UNKNOWN 4

#define PROJECTVIEW _T("PROJECT VIEW")
#define FRAMEVIEW _T("FRAME VIEW")
#define SIGNALVIEW _T("SIGNAL VIEW")
#define PROPERTYVIEW _T("PROPERTY VIEW")
#define TESTREPORTVIEW _T("TEST REPORT VIEW")
#define FILELENGTH_MIN 5
#define DBINVALIDERR_MSG _T("Invalid Fibex Database!")
#define FRAME_MINLEN 2
#define FRAME_MAXLEN 255


// update hints
#define UPDATE_FONT     0
#define UPDATE_MSGWND   1
#define UPDATE_SAVE     2

#define APPLICATION_NAME _T("FRAME")
#define EXPORTTOEXCEL_SYNTAXERROR _T("Could not convert!")
#define EXPORTTOEXCEL_SUCCESSMSG _T("Successfully converted text file to Excel File!")
#define EXPORTTOEXCEL_FIELDSELECTIONERROR _T("No fields selected!")
#define EXPORTTOEXCEL_LOGFILEERROR _T("Enter Log file details!")
#define EXPORTTOEXCEL_CSVFILEERROR _T("Enter CSV file details!")
#define EXPORTTOEXCEL_FILEERROR _T("Enter Log and CSV file details!")
#define EXPORTTOEXCEL_LOGFILEOPENERROR _T("Log File doesnot Exist!")
#define EXPORTTOEXCEL_CSVFILEOPENERROR _T("CSV File doesnot Exist!")

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

#define defSTR_CONF_SAVE_CONFIRM     _T("Configuration File is modified \n Do you want to save this configuration?")
#define defSTR_CONF_CLOSE_CONFIRM    _T("Do you want to close the application?")
#define VALUE_COLUMN    1

// For Log Header; added by Arun on 12th Jan, 2007
#define FLX_LOGDATE_FORMAT                  _T("%c%02d:%02d:%04d::%02d:%02d:%02d:%04d%c")
#define FRAME_VERSION                       _T("***FRAME v1.0***")
#define FRAME_LOG_HEADER                    _T("***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***")
#define FRAME_LOG_START                     _T("***[START LOG SESSION]***")
#define FRAME_LOG_STOP                      _T("***[STOP LOG SESSION]***")
#define FRAME_LOG_HEXFORMAT                 _T("***HEX***")
#define FRAME_LOG_DECFORMAT                 _T("***DEC***")
#define FRAME_LOG_ABSMODE                   _T("***ABSOLUTE MODE***")
#define FRAME_LOG_RELMODE                   _T("***RELATIVE MODE***")
#define FRAME_LOG_SYSMODE                   _T("***SYSTEM MODE***")
#define FRAME_CHANNEL_A                     _T("A")
#define FRAME_CHANNEL_B                     _T("B")
#define FRAME_CHANNEL_AB                    _T("AB")
#define FRAME_LOG_COLUMNS                   _T("***<Time><Tx/Rx><FrameID><FrameLength><Cycle><Channel><DataBytes>***")
#define FRAME_LOG_DIRECTORY                 _T("\\Sample\\Log\\")
#define FRAME_LOG_BACKSLASH                 _T("\\")
#define FRAME_LOG_REMOVE                    _T("Do you want to remove selected log file entry?")
#define FRAME_LOG_ALREADYEXISTS             _T("Log file: %s is already added to the log configuration list!")
#define FRAME_REPLAYLOG_ALREADYEXISTS       _T("Replay log: %s already exists!")

#define FLX_MAX_LOG_FILE_NAME_ITERATION     250
#define FLX_NEW_LOGFILENAME                 _T("FlexRayLogFile%d.log")
#define FLX_LOG_FILE_EXTENSION              _T("log")
#define FLX_LOG_FILTER                      _T("*.log|*.log||")
#define FLX_MAX_FILE_PATH                   300
#define FLX_LOG_SELECTION_TITLE             _T("Select a Log file")
#define FLX_LOG_COL_NAME                    _T("Log File")
#define FLX_LOG_FILE_COL_WIDTH              0.95

#define ENTER_PERIOD_MESSAGE                _T("Enter Period between 1 to 65535")
#define HARDWARE_NOT_CONNECTED              _T("Hardware not connected!")
#define NO_MESSAGE_SELECTED                 _T("No message selected in Transmit window!")
#define NO_ROW_PRESENT                      _T("No Message present in Transmit window!")

#define FLX_INI_FILE_EXTENSION              _T("ini")
#define FLX_INI_FILTER                      _T("INI Files(*.ini)|*.ini||")
#define FLX_CHI_FILE_EXTENSION              _T("chi")
#define FLX_CHI_FILTER                      _T("CHI Files(*.chi)|*.chi||")
#define FLX_MDB_FILE_EXTENSION              _T("mdb")
#define FLX_MDB_FILTER                      _T("Access Database Files(*.mdb)|*.mdb||")
#define FLX_XML_FILE_EXTENSION              _T("xml")
#define FLX_XML_FILTER                      _T("XML Database Files(*.xml)|*.xml||")

#define STATIC_MSG_DATA_LENGTH              _T("Data length of static messages can not be changed")
#define DYNAMIC_MSG_DATA_LENGTH             _T("Dynamic data length is more than the set value")



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
#define SECTION                                      _T("Files")
#define defCONFIGFILENAME                            _T("ConfigFile")
#define defFORMAT_MSGID_DECIMAL _T("%d")
#define defFORMAT_MSGID_HEX     _T("%X")
#define defFORMAT_SIG_VAL_DEC   _T("%I64d")
#define defFORMAT_SIG_VAL_HEX   _T("%I64X")

#define defCOLOR_WHITE           RGB(255,255,255)
#define defSIGNAL_LIST_STRING_MAX_LENGTH     50
#define defSIGNAL_FRAME_COLUMN      7
#define defSTR_COL_SIGNAL_NAME          _T("Signal Name")
#define defSTR_FILTER_DETAILS_ID_FROM           _T("From")
#define defSTR_FILTER_DETAILS_ID_TO             _T("To")
#define defSTR_FILTER_DETAILS_ID_TYPE           _T("ID Type")
#define defSTR_FILTER_DETAILS_MSG_TYPE          _T("Msg Type")
#define defSTR_FILTER_DETAILS_MSG_DIRECTION     _T("Direction")
#define defSTR_FILTER_DETAILS_MSG_CHANNEL       _T("Channel")
#define defSTR_FILTER_NAME          _T("Name")
#define defSTR_FILTER_TYPE          _T("Type")
#define defSTR_MSG_DIR_TX                       _T("Tx")
#define defSTR_MSG_DIR_RX                       _T("Rx")
#define defSTR_SELECTION_ALL                    _T("All")
#define defSTR_MSG_CHANNEL_A                    _T("A")
#define defSTR_MSG_CHANNEL_B                    _T("B")
#define defFILTER_IMAGE_INDEX_DB                0
#define defFILTER_IMAGE_INDEX_NDB               1
#define defFILTER_IMAGE_INDEX_FILTER            2
#define defFILTER_IMAGE_INDEX_ID_RANGE          3
#define defFILTER_TYPE_SINGLE_ID                0
#define defFILTER_TYPE_ID_RANGE                 1
#define defSTR_FILTER_TYPE_PASS                 _T("Pass")
#define defSTR_FILTER_TYPE_STOP                 _T("Stop")
#define defSTR_MSG_ID_FORMAT_DB                 _T("%x [%s]")
#define defSTR_MSG_ID_FORMAT_NDB                _T("%x")
#define STR_EMPTY                               _T("")
#define defSTR_DEFAULT_MRU_ITEM                 _T("Empty")
#define defSTR_FILE_MENU_TEXT                   _T("&File")
#define defSTR_RECENT_MENU_TEXT                 _T("Recent File")

#define defSECTION_MRU           _T("MRU List")
#define defSECTION_MRU_FILE1     _T("MRU File1")
#define defSECTION_MRU_FILE2     _T("MRU File2")
#define defSECTION_MRU_FILE3     _T("MRU File3")
#define defSECTION_MRU_FILE4     _T("MRU File4")
#define defSECTION_MRU_FILE5     _T("MRU File5")

//For adding MSgID to message name
#define defSTR_MSG_ID_IN_HEX          _T("[0x%x]")
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
#define defSTR_DEFAULT_FILTER_NAME              _T("Filter")
#define defSTR_DEFAULT_FILTER_FORMAT            _T("%s_%d")
#define TX_FLAG                 0x01
#define RX_FLAG                 0x02
#define BLUE_COLOR              RGB(0,0,255)
#define defSIGNAL_ICON_SIZE               16
#define defSTR_FILTER_NAME_COLUMN       _T("Filter Name")
#define defBASE_DEC                 10
#define defBASE_HEX                 16
#define defFORMAT_INT64_DECIMAL _T("%I64d")
#define defFORMAT_INT64_HEX     _T("%I64X")
#define defSTR_FORMAT_PHY_VALUE  _T("%.3f")
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

#define defFLC_CREATE_FAILED                _T("Failed to create %s Control")
#define defCOMBO_ITEM                       _T("ComboItem")
#define defCOMBO_LIST                       _T("ComboLsit")
#define defEDIT_ITEM                        _T("EditItem")
#define defNUM_ITEM                         _T("NumItem")
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
#define defSTR_COLLAPSE_MENU_TEXT    _T("&Collapse")
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

#define EDIT_CAPTION            _T("Enter Valid Integer :")
#define LIST_CAPTION            _T("Select from the List below :")
#define COMBO_CAPTION           _T("Select from the Combo box :")

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

#define FRAME_FN_HDR           _T("/* Start FRAME generated function - \
PLACE_HODLER_FOR_FUNCTIONNAME */")

#define FRAME_FN_FOOTER        _T("}/* End FRAME generated function - \
PLACE_HODLER_FOR_FUNCTIONNAME */")

#define FRAME_FN_PARTIAL_HDR   _T("/* Start FRAME generated function - ")

#define FRAME_VAR_HDR          _T("/* Start FRAME global variable */")
#define FRAME_VAR_FOOTER       _T("/* End FRAME global variable */")

#define FRAME_INCLUDE_HDR      _T("/* Start FRAME include header */")
#define FRAME_INCLUDE_FOOTER   _T("/* End FRAME include header */")

#define FRAME_FN_PROTOTYPE_HDR _T("/* Start FRAME Function Prototype  */")
#define FRAME_FN_PROTOTYPE_FOOTER   _T("/* End FRAME Function Prototype  */")

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

#define MESSAGE_HANDLERS            _T("Message Handlers")
#define TIMER_HANDLERS              _T("Timer Handlers")
#define KEY_HANDLERS                _T("Key Handlers")
#define defERROR_HANDLERS           _T("Error Handlers")
#define defDLL_HANDLERS             _T("DLL Handlers")
#define UTILITY_FUNCS               _T("Utility Functions")
#define GLOBAL_VARIABLES            _T("Global Variables")
#define INCLUDE_HEADERS             _T("Include Headers")

#define SPACE                   ' '
#define SEMI_COLON              ';'
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
#define defDLL_UNLOADED_MSG         _T(" The DLL \"FILENAME\" is unloaded")
#define defOBJECT_BUILDPROGRAM_MSG  _T("Unable to allocate memory for BuildProgram module")
#define defSTRMSG_RANGE_INVALID     _T("Message ID range:\"FROM\" field has value greater then or equal to \"TO\" field")
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
#define defCONFIRM_DELETE_HANDLER   _T("You are about to delete the function FUNCTION_NAME(...).\n\
Are you sure you want to delete?")

#define defVOID                     _T("void")
#define defCONFIRM_DELETE_GLOVAR    _T("You are about to delete the variable.\n\
Do you wish to proceed?")

#define defCONFIRM_DELETE_INCHDR    _T("You are about to delete the #include.\n\
Are you sure you want to delete?")
#define defDEFAULT_MSG_HANDLER_CODE _T("void OnMsg%s(%s RxMsg)")

#define defDEFAULT_KEY_HANDLER_CODE _T("void OnKey_KEYNAME(unsigned char KeyValue)")

#define defDEFAULT_TIMER_HANDLER_CODE _T("void OnTimer_TIMERNAME_TIMERVALUE( )")

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
#define defSTR_FLEX_MSG         _T("SFLEXMSG")
#define defSTR_MSG_SPECIFIC_HANDLER _T("Name_")
#define defMESSAGE_NAME_INDEX       5
#define defDEFAULT_MSG_HANDLER_CODE _T("void OnMsg%s(%s RxMsg)")

#define defDEFAULT_KEY_HANDLER_CODE _T("void OnKey_KEYNAME(unsigned char KeyValue)")
#define MSG_RET_TYPE_EMPTY      _T("Please select the return type for the function.")
#define WRAPPER_H               _T("Wrapper.h")
#define WRAPPER_H_STARTID       _T("// Prototypes Start")
#define WRAPPER_H_ENDID         _T("// Prototypes End")

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
#define PERIOD                              '.'
#define NEW_LINE                            _T("\n")
#define TVOID                               "void"
#define defMSG_DUP_TIMER_HANDLER            _T("Duplicate timer handler name!")
#define defTIMER_FN_NAME                    _T("OnTimer_TIMERNAME_TIMERVALUE")

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


#define defSIMVALIDEXTN        _T(".sim")
#define defDOT_SMALL_C         _T(".c")
#define defDOT                 _T('.')

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
#define defEMPTYSTR         _T("")
#define defSAVECONFIGERRSTR _T("Error while saving file %s. Restoring old data")// for sim sys 
#define defCONFIGSAVEERR    _T("File %s not saved properly. Please try again")// for sim sys 
#define defSIMSYSCOPYRIGHT  _T("RBEI_FRAME")
#define defSIMSYSVERSION    1.1
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
#define FLEXRAY_BUS                  _T("FLEXRAY Bus")
#define defERROR_IN_REGISTRY      _T("Unable to obtain path of GCC from registry key")
#define defERROR_OPEN_REGISTRY    _T("The registry value is corrupted. Please re-install.")
#define defERROR_OPEN_REGISTRY_FN    _T("The registry value is corrupted. The interface function is not available.\nPlease re-install.")
#define defIMPORT_DLLLOAD_WARNNING   _T("Please Unload all the Dlls and try again")
//Definitions for Major and Minor Versions

#define defMAJOR_VERSION                    1
#define defMINOR_VERSION                    0
//To get the Version Number from the application DLL
#define defUNSUPPORTED_VER                  _T("Unsupported DLL Version %d.%d")
#define defOLD_VER                          _T("%s is created with old version of FRAME. Please recompile the source code to get latest version of DLL")
#define defREGISTRY_LOCATION      _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\FRAME.exe")
#define def_MSG_MAP_HASH_SIZE     257
#define defSTR_ERROR_IN_MSG_PROG   _T("Exception occured in Message Handler ID : 0x%X")
#define defSTR_ERROR_IN_KEY_PROG   _T("Exception occured in Key Handler of '%c'")
#define defSTR_ERROR_IN_ERR_PROG   _T("Exception occured in Error Handler")
#define defSTR_ERROR_IN_DLL_LOAD   _T("Exception occured in DLL Load Handler")
#define defSTR_ERROR_IN_DLL_UNLOAD _T("Exception occured in DLL UnLoad Handler")
#define defSTR_ERROR_IN_TIMER_PROG _T("Exception occured in Timer Handler \"%s\"")
const BYTE BIT_MSG_HANDLER_THREAD       = 0x01; // First bit
const BYTE BIT_KEY_HANDLER_THREAD       = 0x02; // Second bit
const BYTE BIT_TX_SEL_MSG_THREAD        = 0x04; // Third bit
const BYTE BIT_REPLAY_THREAD            = 0x08; // fourth bit
const BYTE BIT_DLL_LOAD_HANDLER_THREAD   = 0x10; // fifth bit
const BYTE BIT_DLL_UNLOAD_HANDLER_THREAD = 0x20; // sixth bit
const BYTE BIT_ERROR_HANDLER_THREAD      = 0x40; // Seventh bit
const BYTE BIT_MULTI_MSG_THREAD          = 0x80; // Eighth bit
#define defSTR_ERROR_LOADING_HANDLER    _T("Failed to get address of %s Handlers: %s")
#define defSTR_DLL_HANDLER  _T("DLL")
#define defSTR_KEY_HANDLER  _T("Key")
#define defSTR_MESSAGE_HANDLER  _T("Message")
#define defSTR_ERROR_HANDLER  _T("Error")
#define defSTR_TIMER_HANDLER  _T("Timer")
// DLL unload handler wait time
#define defWAIT_DELAY_FOR_DLL_UNLOAD        500
#define MAX_TIME_LIMIT                      1000    // in miliseconds
#define defHEADER_FILE_NAME                 _T("_Unions.h")
// Simulated system Configuration window
#define defSTR_SIMSYS_WINDOW_TITLE              _T("Configure Simulated systems")
#define defSTR_SIMSYS_SAVE_CONFIRMATION         _T("Do you want to save changes?")

#endif //_HASH_DEFINES_H