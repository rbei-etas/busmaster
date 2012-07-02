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
 * \brief     This header file contains the hash define
 * \authors   Amarnath Shastry, Amitesh Bharti, Krishnaswamy B.N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the hash define
 */

#pragma once

//#define defNO_OF_CHANNELS           2
#define MAX_PROTOCOL               20
#define AVAILABLE_PROTOCOLS         4



#define defDRIVER_VERSION "CAN Driver Ver %d.%.2d.%c%x"

// Byte definitions

//Log Type
#define HEX_MODE    "***HEX***"
#define DEC_MODE    "***DEC***"

#define defTX       " Tx "
#define defRX       " Rx "
//Time mode
#define SYSTEM_MODE     "***SYSTEM MODE***"
#define ABSOLUTE_MODE   "***ABSOLUTE MODE***"
#define RELATIVE_MODE   "***RELATIVE MODE***"

#define END_SESSION     "***[STOP LOGGING SESSION]***"
#define START_SESSION   "***[START LOGGING SESSION]***"
#define defLOG_NOTE        "\n***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***"
#define defINVALID_HEADER "Invalid File Header. Replay File Operation Failed"
#define defSTR_REPLAY_FILE_MISSING      "The replay file \"%s\" does not exists !"

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
#define TVOID               "void"

//DIL Configuration Versions
#define DIL_CFX_TWO_CHANNEL             0X1
#define DIL_CFX_MULTI_CHANNEL           0X2
#define DIL_CFX_STRUCTURE_UPDATE        0X3
#define DIL_CFX_CURRENT_VERSION         DIL_CFX_STRUCTURE_UPDATE
// Replay type of logged messages
enum eREPLAYTYPE
{
    eTX_MESSAGE = 0,
    eRX_MESSAGE,
    eALL_MESSAGE
};

enum eMODES
{
    MD_ADD          =   0,
    MD_EDIT,
    MD_DELETE,
    MD_READ_ONLY,
    MD_NONE
};

enum MSG_SENT_BY
{
    MSG_NAME_EDIT_CTRL  =   0,
    MSG_PRMVAR_EDIT_CTRL,
    MSG_PRMTYP_EDIT_CTRL
};

enum eMSG_CHANGES_IN
{
    MSG_NAME    =   0,
    MSG_CODE,
    MSG_LENGTH,
    MSG_FRAME_FORMAT,
    MSG_DATA_FORMAT

};

enum eSELTYPE
{
    SEL_MESSAGE = 0,
    SEL_SIGNAL,
    SEL_GLOBAL_MESSAGE
};

enum eMESSAGEFROM
{
    FNTREEVIEW = 0,
    OUTWND,
    FNVIEW,
    SENDMESGBUTTON
};
enum eTIMEMODE
{
    eSYSTEM_MODE = 0,
    eABSOLUTE_MODE,
    eRELATIVE_MODE
};

typedef enum eREPLAYMODE
{
    REPLAY_OFF = 0,
    REPLAY_ON,
    RUN_CYCLIC,
    SINGLE_LAP_RUN
};

// For splitter row and column
#define FIRST_ROW   0
#define FIRST_COL   0
#define SECOND_ROW  1
#define SECOND_COL  1

#define SPLT_ONE_ROWS   1
#define SPLT_ONE_COLS   2

#define SPLT_TWO_ROWS   2
#define SPLT_TWO_COLS   1

#define ERROR_WND_LISTCTLR_ID 20002



// default fn names
#define DEFAULT_MSG_FN_NAME     "newMessage()"
#define DEFAULT_TIMER_FN_NAME   "newTimer()"
#define NEWTIMER                "newTimer"
#define NEW_FUNCTION            "newFuncName"

// fn types
#define ON_FN                   2
#define ON_TIMER_FN             1
#define ON_MSG_FN               0

// default codes for msg and timer fns
#define DEF_ON_MSG_FN_CODE      "/*******************************************************************************/\n\
void newMsgFuncName(unsigned short Can_ID, unsigned char DLC, unsigned char* strData)\n{\n\n}"

#define DEF_ON_TIMER_FN_CODE    "/*******************************************************************************/\n\
void newTimerFuncName( )\n{\n\n}"

#define DEF_ON__FN_CODE "/*******************************************************************************/\n\
void newFuncName( )\n{\n\n}"

// tree elements
#define FUNCTIONS               "Utility Functions()"
#define ON_MESG                 "ON_MESSAGE() Handler(s)"
#define ON_TIMER                "ON_TIMER() Handler(s)"
#define ON_KEY                  "ON_KEY() Handler(s)"

#define defMAX_TEXT_ACCEPTANCE  2
#define EXPORTS     "EXPORTS"

typedef VOID (*MSGFNPTR)(unsigned short, unsigned char, unsigned char*);
typedef VOID (*TIMERFNPTR)();
//For an integer list
typedef CList<int,int>      defomIntegerList;

#define TXT_FILE_NAME           "UserFnInfoFile.txt"
#define NEW_LINE                "\n"
#define START_ID                "[START]"
#define END_ID                  "[END]"


#define SENDMSG                 "SendMsg( unsigned short Can_ID, unsigned short DLC,unsigned char *Data )"
#define READMSG                 "ReadMsg( unsigned short *Can_ID, unsigned char *DLC, unsigned char *Data )"
#define RESET_MONITOR           "Reset( USHORT Baud )"
#define WRAPPER_HEADER_FILE     "#include \"Wrapper.h\"\n"

#define defHEADER_FILE_NAME        "_Unions.h"
#define defUNION                "typedef union \n{\n"

/* J1939 structures used in unions.h */
#define defMSGPROPS_J1939        "\tSTJ1939_MSG_PROPERTIES m_sMsgProperties;\n"
#define defDLCJ1939             "\tunsigned int m_unDLC;\n"

/* DEFAULT STRUCTURES USED IN UNIONS.H */
#define defSIGNALMEMBER         "m_sWhichBit"
#define defTYPEDEF_STRUCT       "\ntypedef struct \n{\n"
#define defDATATYPE_MSGID       "\tunsigned int  m_unMsgID       ;\n"
#define defDATATYPE_DLC         "\tunsigned char m_ucDLC         ;\n"
#define defDATATYPE_EXTD        "\tunsigned char m_ucEXTENDED    ;\n"
#define defDATATYPE_RTR         "\tunsigned char m_ucRTR         ;\n"
#define defDATATYPE_CHANNEL_NUM "\tunsigned char m_ucChannel	;\n"
#define defDATATYPE_MSG_DATA    "\t%-12s  %s     ;\n"
#define defSTART_OF_STRUCT_DEF  "\n\tstruct\n\t{\n"
#define defEND_OF_STRUCT_DEF    "\n} %s;\n"
#define defEND_OF_UNION_DEF     "\n\t};\n}%s_;"
#define defUNDERSCORE           "_"

#define defUINT_LENGTH           32
#define defUINT_SIZE             4
#define defSIGNED_INTEGER       "int"
#define defUNSIGNED_INTEGER     "unsigned int"
#define defINTEGER64            "__int64"
#define defUNION_FORMAT_STRING  "\t    %-12s %-16s : %2d ;\n"

#define defDATA_BYTE_ARRAY_J1939   "\tunsigned char      m_aucData[%d];\n"
#define defDATA_BYTE_ARRAY         "\tunsigned char      m_aucData[8];\n"
#define defDATA_WORD_ARRAY         "\tunsigned short int m_auwData[4];\n"
#define defDATA_LONG_ARRAY         "\tunsigned long  int m_aulData[2];\n"

#define H_FILE_HEADER_START     "#ifndef %s_UNIONS_H_\n#define %s_UNIONS_H_\n#pragma pack(push,8)\n"
#define H_FILE_HEADER_END       "\n\n#pragma pack(pop,8)\n#endif"
#define GLOBALS                 "Global Variable(s)"

// for creating new fonts
#define DEFAULT_FONT            "Courier"
#define DEFAULT_FONT_ESCAPEMENT  0
#define DEFAULT_FONT_ORIENTATION 0
#define NOT_ITALIC               0
#define NO_UNDERLINE             0
#define NO_STRIKEOUT             0

#define NEXT_POSITION           1
#define START_POSITION          0

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



// default no of chars in a tab
#define defNO_OF_CHARS_IN_TAB   4
#define MARGIN_FOR_FILE_VIEW    5
#define TAB_POSITION            1
#define TAB_ORIGIN              0

#define GET_CONTEXT_WND_CHAR_HEIGHT() \
    ((::GetDeviceCaps(hDeviceContext,LOGPIXELSY) * 10) / 72)
#define GET_CONTEXT_WND_CHAR_WIDTH() \
    ((::GetDeviceCaps(hDeviceContext,LOGPIXELSX) * 6) / 72)
#define GET_FILE_VIEW_CHAR_HEIGHT() \
    ((omDeviceContext.GetDeviceCaps (LOGPIXELSY) * 10) / 72)
#define GET_FILE_VIEW_CHAR_WIDTH() \
    ((omDeviceContext.GetDeviceCaps (LOGPIXELSX) * 6) / 72)



#define STR_EMPTY               ""
//Initializer constants
#define INT_INIT                (INT)0
#define LONG_INIT               (LONG)0
#define DWORD_INIT              (DWORD)0
#define CHAR_INIT               (CHAR)0
#define defCOUNT_INIT              0//initial value of a counter

#define DEFAULT_X_POS           0
#define DEFAULT_Y_POS           0
#define DEFAULT_HEIGHT          0
#define DEFAULT_WIDTH           0
#define INCR_LEN                1

#define CHAR_WIDTH_MULTIPLY     100

#define EDIT_CAPTION            "Enter Valid Integer :"
#define LIST_CAPTION            "Select from the List below :"
#define COMBO_CAPTION           "Select from the Combo box :"

#define CMD_SEND_MSG            0
#define CMD_OPEN_DB             1

#define ID_OF_EDIT_CTRL         "IDC_EDIT"

#define SHIFT_X                     6
#define SHIFT_Y                     3

#define SECTION                 "Files"
#define ENTRY_DATABASE          "DatabaseFile"

#define DATABASE_EXTN           "DBF"
#define PERIOD                  '.'
#define DATABASE_FILTER         "*.dbf|*.dbf||"

//CANoe Database Support
#define CANOE_DATABASE_EXTN     "DBC"

//For OLD Version Compatiability
#define CANVAS_DATABASE_HEADER         "//******************************CANvas Messages and signals Database Ver1.0******************************//"
#define CANVAS_DATABASE_HEADER_NEW     "//******************************CANvas Messages and signals Database ******************************//"

#define DATABASE_HEADER         "//******************************BUSMASTER Messages and signals Database Ver1.0******************************//"
#define DATABASE_HEADER_NEW     "//******************************BUSMASTER Messages and signals Database ******************************//"
#define DATABASE_HEADER_ALL     "//******************************BUSMASTER Messages and signals Database Ver1.0******************************//\n\n// Message Format: MSG,MSGID,MSG_LENGTH,NO_OF_SIGNALS,DATA_FORMAT\n// Signal Format: SIG_NAME,SIG_LENGTH,WHICH_BYTE_IN_MSG,START_BIT,SIG_TYPE,MAX_VAL,MIN_VAL,SIG_DATA_FORMAT,SIG_OFFSET,SIG_FACTOR,SIG_UNIT\n// Value Description Format: DESCRIPTOR,VALUE\n//************************************************************************************************************//\n"

#define defREPLY_IN_PROGRESS    "Replay is in progress!!\nPlease stop it for next session."
#define MSG_MEMORY_CONSTRAINT   "Insufficient Memory available"
#define MSG_MSG_NOT_FOUND       "Selected message not found"
#define SG_DELETE_CONFMN        "You are about to delete a signal.\n\nAre you sure you want to delete?"
#define SG_DELETE_FAILED        "Could not delete signal!"
#define MSG_DELETE_CONFMN       "You are about to Delete a Message.\n\nAre you sure you want to Delete?"

#define CAN_BUS                  "CAN Bus"
#define SIMSYS_DELETE_CONFMN     "You are about to Delete a Simulated System.\n\nAre you sure you want to Delete?"
#define ALL_SIMSYS_DELETE_CONFMN "You are about to Delete all the Simulated Systems.\n\nAre you sure you want to Delete?"
#define ALL_NODE_EDIT_CONFMN     "You are about to change all the Node details!\n\nAre you sure you want to Continue?"
#define NODE_DELETE_CONFMN       "You are about to Delete a Node.\n\nAre you sure you want to Delete?"
#define ALL_NODE_DELETE_CONFMN   "You are about to Delete all the Nodes.\n\nAre you sure you want to Delete?"

#define MSG_DEFAULT_DATABASE    "Currently the sample database file is loaded."
#define MSG_CREATE_UNLOAD_DATABASE "User can create the necessary database file and unload the sample database file if not required."
#define MSG_NO_DATABASE_SELECT  "Currently no database file is selected."
#define MSG_SELECT_DATABASE     "User can select database file from 'File->Database->Import Database' menu option."
#define MSG_SG_NAME_EMPTY       "Signal name cannot be empty!"
#define MSG_DUPLICATE_SG_NAME   "Signal name already exists!"
#define MSG_SG_SELECTED         "No item selected!"
#define MSG_INVALID_HEX_NO      "Invalid hexa-decimal number!"
#define MSG_MAX_VAL_LESS        "Maximum value cannot be less than minimum value!"
#define MSG_MIN_VAL_MORE        "Minimum value cannot be more than maximum value!"
#define MSG_SG_LEN_ZERO         "Invalid signal length value!"
#define MSG_DUP_SG_DESC_VAL     "Signal value already exists!"
#define MSG_DUP_SG_DESC         "Signal description already exists!"
#define MSG_SELECT_SIGNAL       "Please select the signal from the signal list!"
#define MSG_SG_VAL_OUT_OF_RANGE "Signal value is out of range!"
#define MSG_DELETE_DESC_CONMN   "You are about to delete a signal description and\
its value.\nAre you sure you want to delete?"

#define MSG_DEL_FAIL            "Delete operation failed!"

#define MSG_PROMPT_STD          "You are about to convert extended frame format to standard.\n\
This operation will truncate the message code from 29 to 11 bits.\nDo you want to continue?"

#define MSG_PROMPT_EXTD         "You are about to convert standard frame format to extended.\n\
This operation will convert the message code from 11 to 29 bits.\nDo you want to continue?"

#define MAX_LMT_FOR_PGN  0x3FFFF
#define MAX_LMT_FOR_STD_MSG_ID  2048
#define defMAX_LMT_EXTD_MSG_ID 0x20000000
#define defMAX_BITS             64
#define defMAX_SIGNALS          128
#define defBITS_IN_BYTE         8
#define defMAX_BYTE             8
#define defBITS_IN_FOUR_BYTE    32
#define defBITS_IN_FIVE_BYTE    40
#define defBITS_IN_SIX_BYTE     48
#define defBITS_IN_SEVEN_BYTE   56
#define defSIZE_OF_ERROR_BUFFER 1024

#define NEW_DATABASE_NAME       "New_Database"
#define NEW_DATABASE_NAME_J1939 "New_DatabaseJ1939"
#define CHAR_BOOL               'B'
#define CHAR_UINT               'U'
#define CHAR_INT                'I'
#define defMSGID_RTR            'r'
#define defMSGID_STD            's'
#define defMSGID_EXTENDED       'x'
#define defEMPTY_CHAR           ' '
#define defFORMAT_DATA_DECIMAL  "%03d"
#define defFORMAT_DATA_HEX      "%02X"
#define defFORMAT_MSGID_DECIMAL "%d"
#define defFORMAT_INT64_DECIMAL "%I64d"
#define defFORMAT_UINT64_DEC    "%I64u"
#define defFORMAT_INT64_HEX     "%I64X"
#define defFORMAT_MSGID_HEX     "0x%X"
#define defFORMAT_MSGID_HEX_STR "%X"
#define defFORMAT_DATA_FLOAT    "%f"

#define defMSGID_NAME_DELIMITER  "["
#define defMSGID_NAME_START_CHAR '['
#define defMSG_NAME_END_CHAR     ']'

#define FRAME_FORMAT_STD        0
#define FRAME_FORMAT_EXND       1

#define BASE_10                 10

#define RESET                   0
#define SET                     1

#define VALUE                   1
#define DESCRIPTION             0

#define defVALIDATION_MSG_FROM           "From::Invalid Message ID"
#define defVALIDATION_MSG_TO             "To::Invalid Message ID"

#define defSECTION_PARALLEL_PORT         "Parallel Port"
#define defPARALLEL_PORT_MODE            "Parallel port mode"

#define defSTR_SECTION_USB               "USB"
#define defSTR_DRIVER_INFO               "Driver Version"

#define defMSG_TIMER_START_FAILED  "Unable to start timer \"%s\" due to \
system resource problem !!"
#define defERROR_MSG_MAX_TIMER     "Maximum number of timer functions \
supported are %d, you have %d timer functions. The last %d timer \
function(s) is(are) ignored during initialisation."
#define defNUMBER_OF_TIMER        100
#define defTIMER_HEADER_NAME    "Timer Handler Name"
#define defSIZE_OF_IMAGE          25
#define defMASK_LIST_CTRL        0x00003003
#define defMASK_CHECK_UNCHECK    0x00003000
#define defMASK_NODE_COMBO       0xffffdfff

#define DefSPL_LINE              "***"
#define NUMBER_OF_MESSAGES      "NUMBER_OF_MESSAGES"
#define DATABASE_VERSION        "[DATABASE_VERSION]"
#define DATABASE_PREVIOUS_VERSION_NO  "1.2"

// Added protocol and application version information
#define DATABASE_VERSION_NO     "1.3"
#define DATABASE_PROTOCOL       "[PROTOCOL]"
#define APPLICATION_VERSION     "[BUSMASTER_VERSION]"
#define APPLICATION_VERSION_NO     "[PLACE_HOLDER_FOR_APPLICATION_VERSION]"
#define APPLICATION_VERSION_PLACE_HOLDER "PLACE_HOLDER_FOR_APPLICATION_VERSION"
#define DATABASE_PROTOCOL_CAN   "CAN"
#define DATABASE_PROTOCOL_J1939 "J1939"
#define MSG_START_ID            "[START_MSG] "
#define SG_START_ID             "[START_SIGNALS] "
#define SG_DESC_START_ID        "[VALUE_DESCRIPTION] "
#define MSG_END_ID              "[END_MSG]"
#define SPACE                   ' '
#define defSPACE_STR            " "
#define MSG_STRUCT_VAR          "sMsgStruct"
#define MSG_VAR_NAME_EMPTY      "Variable name cannot be empty."
#define MSG_VAR_TYPE_EMPTY      "Please select data type for the variable."
#define SEMI_COLON              ';'
#define SEMI_COLON_STR          ";"
#define MSG_PROTO_EMPTY         "Function prototype cannot be empty."
#define MSG_RET_TYPE_EMPTY      "Please select the return type for the function."
#define WRAPPER_H               "Wrapper.h"
#define WRAPPER_H_STARTID       "// Prototypes Start"
#define WRAPPER_H_ENDID         "// Prototypes End"
#define ASK_SAVE_PROMPT         "Do you want to save changes to the database file?"
#define ASK_SIMSYS_SAVE_PROMPT  "Do you want to save changes to the simulated system file(s)?"

#define MSG_DB_NOT_FOUND        "Database filename not found!\n\
Please select the database file from Import Database... file menu"
#define DATABASE_MISMATCH       "Database version mismatch!.\n\
Message data format will be ignored.\n\
Signal's byte order is set to Intel(Little-Endian) by default."
#define DATABASE_CORRUPT      "Database file is corrupted";


#define CANMON_FN_HDR           "/* Start BUSMASTER generated function - \
PLACE_HODLER_FOR_FUNCTIONNAME */"

#define CANMON_FN_FOOTER        "}/* End BUSMASTER generated function - \
PLACE_HODLER_FOR_FUNCTIONNAME */"

#define CANMON_FN_PARTIAL_HDR   "/* Start BUSMASTER generated function - "

#define CANMON_VAR_HDR          "/* Start BUSMASTER global variable */"
#define CANMON_VAR_FOOTER       "/* End BUSMASTER global variable */"

#define CANMON_INCLUDE_HDR      "/* Start BUSMASTER include header */"
#define CANMON_INCLUDE_FOOTER   "/* End BUSMASTER include header */"

#define CANMON_FN_PROTOTYPE_HDR "/* Start BUSMASTER Function Prototype  */"
#define CANMON_FN_PROTOTYPE_FOOTER   "/* End BUSMASTER Function Prototype  */"

#define UTILS_PREFIX           "Utils_"
#define OPEN_PARENTHESIS        "("
#define NO_OF_CHAR_IN_UTILS     6

#define GLOBAL_VAR_PREFIX         "g_"
#define MIN_CHAR_IN_GLOBAL_VAR    2

#define FILENAME_NOT_FOUND      "File not found!"

#define defDOT_C                    ".C"
#define defDOT_DEF                  ".def"
#define defDOT_EXP                  ".exp"
#define defDOT_DLL                  ".dll"

// Parameters for the CEvent object array used as state flag
#define defEVENT_TOTAL          2
#define UI_THREAD               0

//no. of entries in CExecute func message handler buffer
#define defMAX_FUNC_MSG                     1000
//no. of entries in global message handler structure buffer
#define defMAX_HMODULE_MSG                  1000

// Define for all Util Message Thread
#define defOFFSET_TXMSG              1
#define defTX_SEL_MSG_THREAD         0
//#define defKEY_HANDLER_THREAD        0
//#define defERROR_HANDLER_THREAD      3
//#define defDLL_LOAD_HANDLER_THREAD   4
//#define defDLL_UNLOAD_HANDLER_THREAD 5
//#define defMSG_HANDLER_THREAD        6

//Parameters for the CEvent objects array used in CExecuteFunc
#define defEVENT_EXFUNC_TOTAL        5
//define for CExecute utility thread
#define defKEY_HANDLER_THREAD        0
#define defERROR_HANDLER_THREAD      1
#define defDLL_LOAD_HANDLER_THREAD   2
#define defDLL_UNLOAD_HANDLER_THREAD 3
#define defMSG_HANDLER_THREAD        4




#define defMAX_WAIT_UTIL_THREAD 3000  // in miliseconds

#define MAX_TIME_LIMIT      1000    // in miliseconds


#define FLOOR_TX_MESSAGE_DLC          9   // 

// User defined messages defined
#define WM_STATUSBAR                WM_USER + 1
#define WM_USER_CREATE              WM_USER + 2
#define WM_USER_CLOSE               WM_USER + 3
#define WM_USER_TEST                WM_USER + 4
#define WM_USER_RESIZE_CLIENTWND    WM_USER + 5
#define WM_DISPLAY_MESSAGE          WM_USER + 6
#define WM_SCROLL_FLAG              WM_USER + 7
#define WM_LSTC_DBLCLK              WM_USER + 8
#define WM_USER_NUMERIC_MODE        WM_USER + 9
#define WM_USER_TIME_MODE           WM_USER + 10
#define WM_MSG_STEP                 WM_USER + 11
#define WM_MSG_SKIP                 WM_USER + 12
#define WM_MSG_GO                   WM_USER + 13
#define WM_MSG_STOP                 WM_USER + 14
#define TM_DISP_ON_ERRORMSG         WM_USER + 15
#define WM_DISP_ON_CONFIRM_MSG      WM_USER + 16
#define TM_OVERWRITE_MESSAGE        WM_USER + 17
#define TM_FILTER_DISPLAY           WM_USER + 18
#define TM_FILTER_LOGGING           WM_USER + 19
#define TM_UPDATE_FILTERLIST        WM_USER + 20
#define WM_INTERPRET_MESSAGE        WM_USER + 21
#define TM_ONLINE_INTERPRETATION    WM_USER + 22
#define TM_CLEAR_MESSAGE_WINDOW     WM_USER + 23
#define WM_DISPLAY_TX_MESSAGE       WM_USER + 25
#define WM_OPEN_CLOSE_SIG_WATCH     WM_USER + 26
#define WM_ENABLE_SIG_WATCH         WM_USER + 27
#define TM_FILTER_TX_MSG            WM_USER + 28
#define WM_SHOW_DATE                WM_USER + 29
#define TM_CLEAR_INTERPRETATION_WND WM_USER + 30
#define WM_CONDITIONAL_LOG          WM_USER + 31
#define WM_ERROR                    WM_USER + 32
#define WM_LOG_STRING               WM_USER + 33
#define WM_WRITE_TO_TRACE           WM_USER + 34
#define WM_PROCESS_ERROR_MESSAGE    WM_USER + 35
#define WM_FILE_DISCONNECT          WM_USER + 36
#define WM_RESET_CONTROLLER         WM_USER + 37
#define WM_SET_WARNING_LIMIT_VAR    WM_USER + 38
#define WM_KEY_PRESSED_MSG_WND      WM_USER + 39
#define TM_SET_FLAG_STATUS          WM_USER + 40
#define WM_NOTIFICATION_FROM_OTHER  WM_USER + 43
#define WM_USER_CMD                 WM_USER + 44
#define WM_LOAD_SAVE_WINSTATUS      WM_USER + 45
#define WM_MODIFY_VISIBILITY        WM_USER + 46
#define WM_CREATE_TRACEWIN          WM_USER + 47
#define WM_OWNER_DATA_NOTIFICATION  WM_USER + 48
#define WM_CLEAR_SIGNAL_WATCH       WM_USER + 49
#define WM_ENABLE_DISABLE_HANDLERS  WM_USER + 50
#define WM_SET_UPDATE_TIMER         WM_USER + 51
// CColourPopup messages
//#define WM_CPN_SELCHANGE            WM_USER + 52 // Colour Picker Selection change
//#define WM_CPN_DROPDOWN             WM_USER + 53 // Colour Picker drop down
//#define WM_CPN_CLOSEUP              WM_USER + 54 // Colour Picker close up
//#define WM_CPN_SELENDOK             WM_USER + 55 // Colour Picker end OK
//#define WM_CPN_SELENDCANCEL         WM_USER + 56 // Colour Picker end (cancelled)
#define WM_CONNECT_CHANGE           WM_USER + 57 // Connection Change Event
#define WM_CONFIG_CHANGE            WM_USER + 58 // Configuration Change Event
#define WM_DATABASE_CHANGE          WM_USER + 59 // Database associate/dissociate event
#define WM_KEYBOARD_CHAR            WM_USER + 60 // Capture char key press event from Signal Watch and NW Statistics
#define WM_KEYBOARD_KEYDOWN         WM_USER + 61 // Capture key down event to handle connect / disconnect from Signal Watch and NW Statistics

// Errors
#define defMSG_NOACK                "Message transmission\
acknowledgement could not be received for the message transmitted from\n\
MODULE_NAME module.\nMessage transmission aborted!"
#define defMSG_TIMEOUT              "Timeout occured waiting for acknowledgement\n\
for the message transmitted from MODULE_NAME module.!\n\n\
Select Yes to continue transmission. Further transmission may not be as\
expected.\nSelect No to abort transmission."
#define defMSG_MEMALLOC             "Memory allocation failure.\n\
Message transmission aborted!"
#define defWAIT_FAILED              "Wait for acknowledgement failed."
#define defTX_FAILED                "Unknown Error. Message transmission aborted!"
#define defWRITE_FAILED             "Write function failed. Message transmission aborted!"
//
#define defTX_SEND_MULTI_MSG           0x01
#define defTX_REPLAY                   0x02
// Module name
#define defSEND_MULTI_MSG_MODULE    "Send multiple message"
#define defREPLAY_MODULE            "Replay"
#define defINVALID_TIME_DELAY       "Time delay should be greater then zero"

#define defSTRING_SIZE              50      // Largest size of the following strings
#define defMESSAGE_ID               "Message ID/Name"
#define defMESSAGE_TYPE             "Type"
#define defMESSSAGE_DLC             "DLC"
#define defMESSAGE_DATA_BYTES       "Data Bytes"
#define defMESSAGE_BLOCK_NAME       "Name"
#define defMESSAGE_BLOCK_TRIGGER    "Type"
#define defMESSAGE_BLOCK_TRIG_VAL   "Value"
#define defMESSAGE_TRIG_INTERVAL    "Interval:"
#define defMESSAGE_TRIG_KEY         "     Key:"
#define defMESSAGE_TRIG_MS          "ms"
#define defMSG_CYCLIC               "CYCLIC"
#define defMSG_MONOSHOT             "MONOSHOT"
#define defMSG_WAVEFORM             "WAVEFORM"
#define defASSETRIC                 "*"
#define defDEFAULT_MSG_BLOCK_NAME   "OnTime"

#define defDEFAULT_SIMSYS_NAME   "SimSys"

#define defDEFAULT_TIME_INTERVAL    "100"
#define defDEFAULT_KEY_VAL_STR      "A"
#define defDEFAULT_KEY_VAL          'A'

#define defMAX_MSGINBLOCK           64
#define defMAX_MSGBLOCK             32
#define defDEFAULT_TIME_VAL         100
#define defMESSAGE_BLOCK_COLUMN     3
#define defMESSAGE_FRAME_COLUMN     5
#define defDATA_BYTES_EXTRA         22
#define defMAIN_ITEM                0
#define defSUBITEM_MSGBLOCK_TYPE    1
#define defSUBITEM_MSGBLOCK_TRG_VAL 2
#define defSUBITEM_MSGDETAILS_CHANNEL_ID    1
#define defSUBITEM_MSGDETAILS_TYPE          2
#define defSUBITEM_MSGDETAILS_DLC           3
#define defSUBITEM_MSGDETAILS_DATA          4
#define defMASK_DISPLAY             0x01
#define defMASK_TXON                0x02
#define defMASK_CONNECT             0x04

#define defFORMAT_FRAME_CONTENT_HEX "0x%-8X%6c%c         %-7d %-24s"
#define defFORMAT_FRAME_CONTENT_DEC "%-10d%6c%c         %-7d %-32s"
#define defFORMAT_DATA_BYTES_HEX    "%02X "
#define defFORMAT_DATA_BYTES_DEC    "%03d "

#define defBUTTON_LESS              "Less..."
#define defBUTTON_MORE              "More..."
#define defUPDATE_FAILED            "Could not update the data!"
#define defNOSG_DEFINED             "You cannot configure the selected message.\n\
No signals are defined for the message!"
#define defZERO_MSG_LENGTH          "You cannot configure the selected message.\n\
The message length is zero!"
#define defDEL_SEL_MSG_FRAME        "You are about to delete seleted message frame detail.\n\n\
Are you sure you want to delete?"
#define defDEL_ALL_MSG_FRAME        "You are about to delete all message frame detail of selected message block.\n\n\
Are you sure you want to delete?"
#define defDEL_MSG_BLOCK            "You are about to delete a message block.\n\n\
Are you sure you want to delete?"
#define defINVALID_DLC              "Invalid Data...!\n\
Data length count(DLC) should be less than 9."
#define defINVALID_DATABYTE         "Invalid Data...!\n\
Data byte value should be less than 256."
#define defNO_MSG_CODE              "Please enter valid message code"
#define defNO_DLC                   "Please enter valid DLC"

const int TX_MESSAGE = 0x20000000;  // bitwise OR to make it a Tx message
const int RX_MESSAGE = 0xdfffffff;  // bitwise AND to make it a Rx message

#define MAKE_RTR_MESSAGE_TYPE(MSGID)         (MSGID | 0x80000000)
#define MAKE_EXTENDED_MESSAGE_TYPE(MSGID)    (MSGID | 0x40000000)
#define MAKE_STANDARD_MESSAGE_TYPE(MSGID)    (MSGID & 0xBfffffff)
#define MAKE_DEFAULT_MESSAGE_TYPE(MSGID)     (MSGID & 0x3fffffff)
#define MAKE_RX_TX_MESSAGE(MSGID, MSGTYPE)   ((MSGTYPE) ? (MSGID) : ((MSGID) | TX_MESSAGE))
#define MAKE_CHANNEL_SPECIFIC_MESSAGE(MSGID, CHANNELID) ( ((unsigned __int64)((UINT)(MSGID))) | (((__int64)(CHANNELID)) << defBITS_IN_FOUR_BYTE) )
#define MAKE_SOURCE_SPECIFIC_MESSAGE(MSGID, SOURCE) ( ((unsigned __int64)((MSGID))) | (((__int64)(SOURCE)) << defBITS_IN_FIVE_BYTE) )
#define MAKE_DEST_SPECIFIC_MESSAGE(MSGID, DEST) ( ((unsigned __int64)((MSGID))) | (((__int64)(DEST)) << defBITS_IN_SIX_BYTE) )
#define MAKE_TYPE_SPECIFIC_MESSAGE(MSGID, TYPE) ( ((unsigned __int64)((MSGID))) | (((__int64)(TYPE)) << defBITS_IN_SEVEN_BYTE) )


const int BIT_FILTER_DISPLAY =  0x1;
const int BIT_FILTER_LOG     =  0x2;
const int BIT_FILTER_TX_MSG  =  0x4;
const int MASK_BIT_FILTER    =  0x7;

const BYTE BIT_MSG_HANDLER_THREAD       = 0x01; // First bit
const BYTE BIT_KEY_HANDLER_THREAD       = 0x02; // Second bit
const BYTE BIT_TX_SEL_MSG_THREAD        = 0x04; // Third bit
const BYTE BIT_REPLAY_THREAD            = 0x08; // fourth bit
const BYTE BIT_DLL_LOAD_HANDLER_THREAD   = 0x10; // fifth bit
const BYTE BIT_DLL_UNLOAD_HANDLER_THREAD = 0x20; // sixth bit
const BYTE BIT_ERROR_HANDLER_THREAD      = 0x40; // Seventh bit
const BYTE BIT_MULTI_MSG_THREAD          = 0x80; // Eighth bit

const int MESSAGE_BUFFER_SIZE = 512;
const int SIZE_HASH_TABLE = 37;
const int SIZE_HASH_TABLE_DBMSGS = 199;

// Dynamically created control identifiers defined
#define IDC_MESSAGE_WND             100
#define IDC_LSTC_MESSAGE            200
#define IDC_LSTB_OUTPUT             300
#define IDC_LSTB_REPLAY             400

// typedef
typedef CArray<INT, INT&> CARRAY;
typedef CARRAY* PCARRAY;


#define SPACE_BET_CHAR_IN_FILE_VIEW 20
#define SPACE_BET_LINE_IN_FILE_VIEW 5

// Function Editor
#define defCONFIRM_DELETE_HANDLER   "You are about to delete the function FUNCTION_NAME(...).\n\
Are you sure you want to delete?"

#define defVOID                     "void"
#define defCONFIRM_DELETE_GLOVAR    "You are about to delete the variable.\n\
Do you wish to proceed?"

#define defCONFIRM_DELETE_INCHDR    "You are about to delete the #include.\n\
Are you sure you want to delete?"

// Main Window
#define defSECTION_MAIN_WND              "MainWnd"
#define defITEM_MAIN_WND_LEFT            "MainWnd.left"
#define defITEM_MAIN_WND_RIGHT           "MainWnd.right"
#define defITEM_MAIN_WND_TOP             "MainWnd.top"
#define defITEM_MAIN_WND_BOTTOM          "MainWnd.bottom"

#define defITEM_MAIN_WND_FLAG            "MainWnd Flag"
#define defITEM_MAIN_WND_SHOWCMD         "MainWnd ShowCmd"
#define defITEM_MAIN_WND_MINPOS_X        "MainWnd MinPos.X"
#define defITEM_MAIN_WND_MINPOS_Y        "MainWnd MinPos.Y"
#define defITEM_MAIN_WND_MAXPOS_X        "MainWnd MaxPos.X"
#define defITEM_MAIN_WND_MAXPOS_Y        "MainWnd MaxPos.Y"

#define defDISPLAY_BUFFER_SIZE      100

#define defCAN_MONITOR_DATA_IND     0
#define defCAN_MONITOR_DAT_CON      1
#define defCAN_MONITOR_ERROR_IND    2
#define defAPP_BUFFER_CLEAR         3
#define defAPP_EXIT                 4
#define defTIMEOUT_FOR_USERSEND_MSG 5

#define defTRACE_WINDOW_TITLE       "Trace Window"

// No Signal Manipulation allowed
#define defNO_SG_MINUP              "You cannot manipulate signal \
details since message length\nfor the selected message is zero!"

#define defLOG_FILTER               "*.log|*.log||"
#define defSTR_LOG_FILE_EXTENSION   "log"
// Logging
#define defLOGDLG_CAPTION           "Select Message Log File..."
#define defLOGDLG_FILTER            "Message Log File(s)(*.log)|*.log||"
#define ENTRY_LOGFILE               "MessageLogFile"
#define defMSG_NAME                 "Message Name"
#define defMSG_ID                   "Message ID"
#define ENTRY_REPLAYFILE            "MessageReplayFile"
#define def_DEFAULT_LOGFILENAME     "BUSMASTERLogFile.log"
#define defNO_DATABASE_MSG_NAME     "The message \"%s\" is not defined  in database !!"

// Use these hashdefines to check the return value if u are
// tx'ing messages

// TX Success
#define TRANSMIT_SUCCESS            1
#define TRANSMIT_FAILURE            0
// Wait ReadyToTransmit event errors
#define WAIT_TIMEOUT_ERROR          -(TRANSMIT_SUCCESS + 1)
#define WAIT_FAILURE_ERROR          -(TRANSMIT_SUCCESS + 2)
#define WAIT_ABANDONED_ERROR        -(TRANSMIT_SUCCESS + 3)
// Write operation errors
#define WRITE_ERROR                 -(TRANSMIT_SUCCESS + 4)
#define WRITE_INCOMPLETE            -(TRANSMIT_SUCCESS + 5)
// Wait Ack event errors
#define WAIT_ACK_TIMEOUT_ERROR      -(TRANSMIT_SUCCESS + 6)
#define WAIT_ACK_FAILURE_ERROR      -(TRANSMIT_SUCCESS + 7)
#define WAIT_ACK_ABANDONED_ERROR    -(TRANSMIT_SUCCESS + 8)
#define MEM_ALLOC_FAILURE_ERROR     -(TRANSMIT_SUCCESS + 9)

// error defines for CConfigDetails class..
#define defCONFIG_FILE_ERROR                1000
#define defCONFIG_FILE_OPEN_FAILED          defCONFIG_FILE_ERROR+2
#define defCONFIG_FILE_CLOSE_FAILED         defCONFIG_FILE_ERROR+5
#define defCONFIG_FILE_INVALID_FILE_EXTN    defCONFIG_FILE_ERROR+6
#define defCONFIG_FILE_CORRUPT              defCONFIG_FILE_ERROR+7
#define defCONFIG_FILE_HDR_CORRUPT          defCONFIG_FILE_ERROR+8
#define defCONFIG_FILE_SUCCESS              10
#define defFILE_LOADED                      11

#define defCONFIG_FILE_LOADING              0
#define defCONFIG_FILE_SAVING               1
#define defCONFIG_FILE_NEW                  2

// More hash defines to make compatible with the
// error code returned by GetLastError() function
// NOTE : Do not change the values below

#define defCONFIG_FILE_NOT_FOUND            2
#define defCONFIG_PATH_NOT_FOUND            3
#define defCONFIG_FILE_ACCESS_DENIED        5
#define defCONFIG_FILE_HANDLE_INVALID       6
#define defCONFIG_DRIVE_NOT_FOUND           15
#define defCONFIG_FILE_READ_FAILED          30
#define defCONFIG_FILE_WRITE_FAILED         29

// Hash defines only for this class

#define defDEFAULTCFGFILE   "DefaultConfig.cfx"
#define defDOT              '.'
#define defNUMEXTNCHARS     3
#define defVALIDEXTN        ".cfx"
#define defFORWARDSLASH     "//"
#define defBUFLEN           64
#define defAPPVERSION           2.7
#define defBASE_CONF_VERSION    2.3
// for sim sys
//#define defSIMSYSVERSION           1.0

// Feature's starting version list
#define defSIGNAL_GRAPH_VERSION           2.4
#define defMULTI_CHANNEL_VERSION          2.5
#define defTX_MSG_ENABLE_OPTION_VERISION  2.5
#define defMULTI_FILTER_VERSION           2.5
#define defMULTI_NODE_VERSION             2.6
#define defMULTI_DATABASE_VERSION         2.7

#define defSIMVALIDEXTN        ".sim"
#define defDOT_SMALL_C         ".c"

#define defCOPYRIGHT        "(c)Robert Bosch India Limited, Bangalore"
#define defTMPFILEEXTN      "tmp"
#define defTMPFILEATTRIB    FILE_ATTRIBUTE_HIDDEN
#define defEMPTYSTR         ""
#define defSAVECONFIGERRSTR "Error while saving file. Restoring old data"

#define defCHILD_FRAME_PROP_H 0.75
#define defOUTPUT_WND_PROP_X  0.5
#define defREPLAY_WND_PROP_X  0.8
#define defNOTIFIC_WND_PROP_X 0.5
#define defMSGWNDFLAGS      WPF_RESTORETOMAXIMIZED
#define defMSGWNDSHOWCMD    SW_SHOWMAXIMIZED
#define defWNDSHOWCMD       SW_SHOWNORMAL
#define defMSGWNDPTMINPOSX  0x000000c8
#define defMSGWNDPTMINPOSY  0x000000c8
#define defMSGWNDPTMAXPOSX  0xfffffffa
#define defMSGWNDPTMAXPOSY  0xffffffe7
#define defMSGWNDRECTB      0x0000015e

#define defMSGWNDRECTL      0x00
#define defMSGWNDRECTT      0x00
#define defMSGWNDRECTR      0x00000190
#define defBTR0BTR1         0x00004914
#define defITEMINFOCUS      0x00000040
#define defACCCODE          "00"
#define defACCMASK          "FF"
#define defACCFILTERTYPE    0
#define defBAUDRATE         "100"
#define defPROPDELAY        "ALL"
#define defSJW              "ALL"
#define defBTR0             "49"
#define defBTR1             "14"
#define defCNF1             "7"
#define defCNF2             "BA"
#define defCNF3             "7"
#define defCLOCK            "32"
#define defSAMPLING         "1"
#define defWARNING_LIMIT    "96"
#define defWARNING_LIMIT_INT    96
//if external copy simulation mode is default mode
#ifdef EVAL_COPY_EXT
#define defCONTROLLER_MODE  3 // Simulation mode
#else
#define defCONTROLLER_MODE  1 // Active mode
# endif
#define defMSGFILTERTYPE    0x01
#define defDLCSIZE          8
#define defMAX_THREAD_COUNT 30
#define def_MSG_MAP_HASH_SIZE 257

#define defCHANGEDMRUCFILE          0x0000000000000001
#define defCHANGEDLOGFILE           0x0000000000000010
#define defCHANGEDREPLAYFILE        0x0000000000000100
#define defCHANGEDDATABASEFILE      0x0000000000001000
#define defCHANGEDDLLFILE           0x0000000000010000
#define defCHANGEDMSGNAME           0x0000000000100000
#define defCHANGEDMSGID             0x0000000001000000
#define defCHANGEDMULTIMSG          0x0000000010000000
#define defCHANGEDWNDCOORDS         0x0000000100000000
#define defCHANGEDCONTROLLERDETAILS 0x0000001000000000
#define defCHANGEDMSGATTRIBS        0x0000010000000000
#define defCHANGEDMSGFILTER         0x0000100000000000
#define defCHAGNEDTOOLBARINF        0x0001000000000000
#define defCHANGEDMSGBLOCKCOUNT     0x0010000000000000

#define COPYFILE(SRC,DESTN)         ::CopyFile(SRC,DESTN,FALSE);
#define defCHANGEDSIMSYS           0x0000000000000001

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

#define defSECTION_MRU           "MRU List"
#define defSECTION_MRU_FILE1     "MRU File1"
#define defSECTION_MRU_FILE2     "MRU File2"
#define defSECTION_MRU_FILE3     "MRU File3"
#define defSECTION_MRU_FILE4     "MRU File4"
#define defSECTION_MRU_FILE5     "MRU File5"
#define defCONFIGFILENAME        "ConfigFile"

#define defREGISTRY_LOCATION      "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\BUSMASTER.exe"
#define defERROR_IN_REGISTRY      "Unable to obtain path of GCC from registry key"
#define defERROR_OPEN_REGISTRY    "The registry value is corrupted. Please re-install."
#define defERROR_OPEN_REGISTRY_FN "The registry value is corrupted. The interface function is not available.\nPlease re-install."
#define defHARDWARE_ERROR_MSG     "Unable to detect hardware interface."
#define defSTR_APP_START_ERROR    "Unable to start the application.\n\
Either the application is already running or the BUSMASTER Device Driver is not yet started.\n\
If the BUSMASTER Device Driver is not started,"
#define defSTR_WINNT_INSTRUCTION  " please start it manually from Control Panel -> Devices -> Candriver \
and then restart the application."
#define defSTR_WIN2K_INSTRUCTION  " Please use BUSMASTER Driver Manager from Start-> Programs -> BUSMASTER -> BUSMASTER Driver Manager \
and then restart the application."

#define defSTR_CONNECT_ERROR    "Unable to connect. Please check the Dongle"

#define defSTR_MDI_CHILD_CREATION_FAILED          "Child window creation failed"
#define defSTR_MSGWND_UI_THREAD_CREATION_FAILED   "Display thread creation failed"
#define defSTR_MSG_READ_THREAD_CREATION_FAILED    "Message router thread creation failed"

#define defIMPORT_DLLLOAD_WARNNING "Due to change in configuration\nPlease unload the loaded DLL file"
#define defMSG_INIT_FORMAT       "= { 0x%X, %d, %d, %d"
#define defFNS_COMMA             ","
#define defDEFAULT_CHANNEL_ID    "1"
#define defFNS_INIT_VAL          " 0"
#define defOPEN_PARENTHESIS      ", {"
#define defCLOSE_PARENTHESIS     " }"
#define defFNS_INIT_FORMAT       "%s %s %s"
#define defFNS_INIT_SIG_FORMAT   "%s\n\n%s.%s"
#define defCHAR_EQUALS           '='
#define defSIG_WATCH_DELETE_ERROR   "Error deleting Signal Watch List!!"
#define defIMPORT_WARNING   "Dissociation will clear Signal Watch List.\nDo you want to Clear the List?"

#define defSTR_FORMAT_DISPLAY_DEC    "%-60s   [%16I64d]"
#define defSTR_FORMAT_DISPLAY_HEX    "%-60s   [%16I64X]"
#define defSTR_FORMAT_OL_INTRP   "         %-20s %-20s"
//#define defSTR_SIGNAL_WATCH_FMT  _T(" %-2s %-16s-> %-14s %-10s[%s]")
#define defSTR_FORMAT_PHY_VALUE  "%.3f"
#define defSTR_FORMAT_PHY_VALUE_WITH_UNIT  "%16s %s"
// To have a left justified format string for interpretation to utilist visible area
// of the list control

#define defCHAR_PATH_SEPRATER    '\\'
#define defSTR_DEFAULT_DBNAME    "None"
#define defCOLOR_WHITE           RGB(255,255,255)
#define defSTR_MSG_SIG_SEPERATER " -> "
//#define defSTR_FORMAT_SW_LIST    _T("%s%s%s")

//#define defSTR_SW_DELETE_ERROR   _T("Error while deleting temporary list!!!")
//#define defSTR_SW_DELETE_SIG_ERROR  _T("Error while deleting Signal %s from Message %s !!!")
//#define defSTR_SW_DELETE_SIG_MSGID_ERROR _T("Error while deleting Signal %s from Message ID: %x !!!")
//#define defSTR_SW_PARSE_ERROR    _T("Parse Error in %s ")
//#define defSTR_SW_SIGNAL_DETAIL_ERROR   _T("Signal Not Found in the Database!!\nPlease remove the Signal")
//#define defSTR_SW_SIGNAL_GENERIC_ERROR  _T("Error showing Signal Details!!")

#define defSTR_CLOSE_MENU_TEXT  "&Close"
#define defSTR_SIGNAL_NAME      "Name"
#define defSTR_PHYSICAL_COLUMN  "Physical Value"
#define defSTR_RAW_COLUMN       "Raw Value"
#define defSTR_UNITS            "Units"
#define defSTR_DEFAULT_MRU_ITEM "Empty"
#define defSTR_FILE_MENU_TEXT   "&File"
#define defSTR_RECENT_MENU_TEXT "Recent Configurations"
#define defSTR_CONF_SAVE_CONFIRM    "Configuration File is modified \n Do you want to save this configuration?"
#define defSTR_ERROR_IN_MSG_PROG   "Exception occured in Message Handler ID : 0x%X"
#define defSTR_ERROR_IN_KEY_PROG   "Exception occured in Key Handler of '%c'"
#define defSTR_ERROR_IN_ERR_PROG   "Exception occured in Error Handler"
#define defSTR_ERROR_IN_DLL_LOAD   "Exception occured in DLL Load Handler"
#define defSTR_ERROR_IN_DLL_UNLOAD   "Exception occured in DLL UnLoad Handler"
#define defSTR_ERROR_IN_TIMER_PROG "Exception occured in Timer Handler \"%s\""

#define defSTR_ERROR_IN_MSG_HANDLER "Exception in Message Handler Execution\nPlease Unload the DLL"
#define defSTR_THREAD_DEL_ERROR     "Exception while deleting Message Handler Thread!!"

#define defSTR_HEX_MODE "Name/ ID (in Hex)"
#define defSTR_DEC_MODE "Name/ ID (in Dec)"

#define defSTR_BUS_STAT_PASSIVE "Bus Status : Passive"
#define defSTR_BUS_STAT_ACTIVE  "Bus Status : Active"
#define defSTR_BUS_STAT_OFF     "Bus Status : Off"
#define defSTR_RX               "Rx"
#define defSTR_COLLAPSE_MENU_TEXT   "&Collapse"

#define defCHAR_OPEN_PARENTHESIS    '{'
#define defCHAR_CLOSE_PARENTHESIS   '}'

#define defALTERNATE_FOR_ERROR_ZERO 7

#define defSTR_ERROR_LOADING_HANDLER    "Failed to get address of %s Handlers: %s"
#define defSTR_DLL_HANDLER  "DLL"
#define defSTR_KEY_HANDLER  "Key"
#define defSTR_MESSAGE_HANDLER  "Message"
#define defSTR_ERROR_HANDLER  "Error"
#define defSTR_TIMER_HANDLER  "Timer"

#define defSTR_INSERT_GLOBAL_DLG_CAPTION    "Message Variable List"

#define defTIME_STAMP_FORMAT "%02d:%02d:%02d:%04d"

const int LENGTH_DATASTRING = 33;

#define defDATE_FIELD_FORMAT "%-11s "
#define defTIME_STAMP_FIELD_FORMAT "%-13s "
#define defMSG_TX_RX_FIELD_FORMAT " %cx   "
#define defMSG_TYPE_FIELD_FORMAT " %-4s"
#define defMSG_FIELD_FORMAT "%-4d%-28s "
#define defMSG_FIELD_HEX_FORMAT "%-4X%-28s "
#define defMSG_DLC_FIELD_FORMAT " %-3d"
#define defMSG_DATA_FIELD_FORMAT "%-16s"

// Display Formatting string

// Display with out date in decimal format
#define defMSG_DISPLAY_FORMAT   "%-13s  %cx      %-5d  %-4s%-9d%-23s  %-3d%-16s"
// Display with out date in Hex format
#define defMSG_DISPLAY_FORMAT_HEX   "%-13s  %cx      %-5d  %-4s%-8X%-24s  %-3d%-16s"
// Display with date in decimal format
#define defMSG_DISPLAY_FORMAT_WITH_DATE "%-11s %-13s  %cx      %-5d  %-4s%-9d%-23s  %-3d%-16s"
// Display with date in Hex format
#define defMSG_DISPLAY_FORMAT_WITH_DATE_HEX "%-11s %-13s  %cx      %-5d  %-4s%-8X%-24s  %-3d%-16s"
// Error message with date
#define defMSG_DISPLAY_ERROR_WITH_DATE  "%-11s %-13s %s : Channel %2d  "
// Error message with out date
#define defMSG_DISPLAY_ERROR            "%-13s %s : Channel %2d  "

// Define Message Display Header Format
#define defDISPLAY_HEADER_FORMAT_WITH_DATE "%-11s%-14s%-6s%-9s%-5s%-33s%-4s%-16s"
#define defDISPLAY_HEADER_FORMAT           "%-13s%-6s%-9s%-5s%-33s%-4s%-16s"

// Define Column names
#define defSTR_DATE     "Date"
#define defSTR_TIME     "Time"
#define defSTR_RXTX     "Tx/Rx"
#define defSTR_MSG_TYPE "Type"
#define defSTR_MESSAGE  "Message"
#define defSTR_DLC      "DLC"
#define defSTR_DATA     "Data Byte(s)"


#define defSTR_OW_BUFFER_OVERFLOW   "Overwrite Buffer overflow occured. Please increase the Overwrite Buffer size"

#define SIGNED_VAL  1
#define FLOAT_VAL   2
#define BUDDY_CTRL  4

#define IS_BUDDY_ENABLED(x)  ((x) & (BUDDY_CTRL) ? TRUE : FALSE )
#define IS_FLOAT_ENABLED(x)  ((x) & (FLOAT_VAL) ? TRUE : FALSE )
#define IS_SIGNED_NUMBER(x)  ((x) & (SIGNED_VAL) ? TRUE : FALSE )

// Send Multi Message Signal List definitions
#define defSIGNAL_LIST_STRING_MAX_LENGTH     50
#define defSIGNAL_FRAME_COLUMN      4
#define defSTR_COL_SIGNAL_NAME          "Signal Name"

// These are all not required as it is already defined as defSTR_PHYSICAL_COLUMN
// and defSTR_RAW_COLUMN
//#define defSTR_SIGNAL_RAW_VALUE     _T("Raw Value")
//#define defSTR_SIGNAL_PHY_VALUE     _T("Physical Value")
#define defSTR_SIGNAL_UNIT          "Unit"
#define def_SIG_NAME_COLUMN         0
#define def_RAW_VALUE_COLUMN        1
#define def_PHY_VALUE_COLUMN        2
#define def_SIG_UNIT_COLUMN         3

#define defSTR_TRUE                 "TRUE"
#define defSTR_FALSE                "FALSE"

#define defBASE_DEC                 10
#define defBASE_HEX                 16

#define defSIGN_MASK                0x8000000000000000

#define defSTR_INVALID_MESSAGE_ID   "Invalid Message ID or Name"
#define defSTR_INVALID_DLC          "Invalid DLC value"
#define defSTR_INVALID_DATA_BYTES   "Invalid data for byte : %d"

// Bit wise representation of trigger condition
// Bit 0        - Time Trigger
// Bit 1        - Key Trigger
// Bit 2        -
// Bit 3        -

// Bit 4        -
// Bit 5        -
// Bit 6        -
// Bit 7        -
#define defTIME_TRIGGER     0x1
#define MASK_DISABLE_TIME_TRIGGER ~defTIME_TRIGGER
#define defKEY_TRIGGER      0x2
#define MASK_DISABLE_KEY_TRIGGER ~defKEY_TRIGGER

// Macros to find the type
#define IS_TIME_TRIGGERED(x)    ( (x) & defTIME_TRIGGER ? TRUE : FALSE )
#define IS_KEY_TRIGGERED(x)     ( (x) & defKEY_TRIGGER ? TRUE : FALSE )

#define defSTR_LOG_FILE_CREATION_ERROR  "Log File Creation Failed : %s\nLogging is Disabled"
#define defSTR_LOG_FILE_DIR_MISSING     "Path \"%s\" not found. Do you Want to create it?"
#define defSTR_DIR_CREATE_FAILED        "Directory creation Failed. Please check the path: \"%s\""

#define defSTR_INVALID_SIGNAL_DEFINITION    "%s has invalid signal definition"
#define defSTR_DATABASE_CHECK_WARN          "Please check the database file!!"



#define defSIGNAL_ICON_SIZE               16
// Message Buffer Size Related definitions
#define defDEF_APPEND_BUFFER_SIZE         5000
#define defDEF_OVERWRITE_BUFFER_SIZE      2000
#define defMIN_BUFFER_SIZE                200
#define defMAX_BUFFER_SIZE                32500
#define defMIN_DISPLAY_UPDATE_TIME        50        // In ms
#define defDEF_DISPLAY_UPDATE_TIME        100
#define defMAX_DISPLAY_UPDATE_TIME        20000     // In ms
#define defSTR_BUFFER_CREATION_FAILURE    "Buffer Create Failed. Please check the buffer size in the Message Display Configuration"
#define defSTR_BUFFER_CREATION_GENERIC_FAILURE  "Message Buffer Creation Failed"
#define defSTR_BUFFER_DELETION_GENERIC_FAILURE  "Message Buffer deletion Failed"
#define defDISPLAY_CONFIG_PARAM         3
#define defAPPEND_DATA_INDEX            0
#define defOVERWRITE_DATE_INDEX         1
#define defDISPLAY_UPDATE_DATA_INDEX    2

// DLL unload handler wait time
#define defWAIT_DELAY_FOR_DLL_UNLOAD        500

#define defFLC_CREATE_FAILED                "Failed to create %s Control"
#define defCOMBO_ITEM                       "ComboItem"
#define defCOMBO_LIST                       "ComboLsit"
#define defEDIT_ITEM                        "EditItem"
#define defNUM_ITEM                         "NumItem"
#define defSTR_SIGNAL_MATRIX                "Signal Matrix"

#define defHEX_FORMAT                       "%X"
#define defFMT_ZERO_PRECESSION              "%.0f"

// Error codes and strings
// Data named event creation failure
#define defDATA_EVENT_CREATE_FAILURE              -1
// Senaphore for nWriteToCan function access is failed
#define defWRITE_TO_CAN_SEM_CREATE_FAILURE        -2
// Data named event creation failure
#define defSTR_DATA_EVENT_CREATE_FAILURE        "Unable to communicate with driver. Please check if your current login has administrative privilege";
// Senaphore for nWriteToCan function access is failed
#define defSTR_WRITE_TO_CAN_SEM_CREATE_FAILURE  "Bus Access semaphore creation failed. Please restart the application"
// Names of the Named Events. These named events are used to establish
// connection (only indication) with the driver
#define defSTR_DATA_EVENT_NAME              "CAN_MonitorDataInd"
#define defSTR_ERROR_EVENT_NAME             "CAN_MonitorErrorInd"
#define defSTR_DATA_CON_EVENT_NAME          "CAN_MonitorDataCon"

#define defSTR_LOG_APPLY_CONFORM            "Logging is currently on.Do you want to apply changes now or during next logging session?"
#define defSTR_LOG_APPLY_NOTIFICATION       "Log parameter changes are applied"

// Item type and Numeric details table sizes
#define defLIST_ITEM_TYPE_TABLE_SIZE        131
#define defLIST_NUM_DETAILS_TABLE_SIZE      67

// Virtual column count. This should be greater then the actual column count
#define defVIRTUAL_COL_COUNT                50

// Key Handler proc CMap Hash table size.
#define defKEY_HANDLER_HASH_TABLE_SIZE      17

#define defTX_BLOCK_SEM_MAX_COUNT           2

#define defSTR_SIGNAL_NAME_INVALID          "Signal name cannot be empty"
#define defSTR_SIGNAL_END_BIT_INVALID       "End bit of signal is out of message length"
#define defSTR_SIGNAL_DUP_START_BIT         "Duplicate signal start bit value"
#define defSTR_MAX_VAL_INVALID              "Maximum value is out of bounds!"
#define defSTR_MIN_VAL_INVALID              "Minimum value is out of bounds!"

#define defCRETATE_SUCCESS                  0
#define defCREATE_FAILED                    -1

#define defSTR_TIMER_VAL_FMT_SPECIFIER      "%d"
#define defSTR_KEY_VAL_FMT_SPECIFIER      "%c"
#define defSTR_MSG_BLOCK_TRIGGER_SEPRATOR   " | "

#define defSTR_FILTER_UPDATED               "Filter list has been updated"
#define defSTR_RELOAD_CONFIRMATION          "You have changed \"Message Block\" properties \
or deleted message(s) from \"Active Message Block(s)\"\nThese changes can not be saved while message \
transmission is ON.\nDo you want to revert the changes ?"
#define defSTR_IGNORE_CHANGES_CONFIRMATION  "You have changed \"Message Block\" properties \
or deleted message(s) from \"Active Message Block(s)\"\nThese changes can not be saved while message \
transmission is ON.\nDo you want to ignore the changes ?"

#define defSTR_EMPTY_ACTIVE_DATABASE "There is no active database. Please import database"

#define defBITS_ERR_FRAME                   20
#define defBITS_STD_FRAME                   48 //Without databytes.
#define defBITS_EXTD_FRAME                  68 //Without databytes.
#define defBITS_ERROR_FRAME                 15
#define defBITS_INTER_FRAME                 12
#define defTIME_INTERVAL                    1.0
#define defDIV_FACT_FOR_SECOND              10000.0
#define defMAX_PERCENTAGE_BUS_LOAD          100.00
#define defMAX_PERCENTAGE_BUS_LOAD_ALLOWED  99.99
#define defSTR_FORMAT_BUS_LOAD              _T("%-5.2f %%")
#define defBITS_KBUAD_RATE                  1000.0


// These are all USB Related Definitions
#define defSTR_DUAL_FILTERING               "\tDual Filtering\n"
#define defSTR_WARNING_LIMIT                "\tWarning Limit\n"
#define defSTR_UNSUPPORTED_OPTIONS_INFO     "The following unsupported setting found.\n%sDefault value will be used for these options.\nTo save changes please use File -> Configuration -> Save. Changes will be discarded otherwise."
#define defSTR_UNSUPPORTED_OPTIONS          "Found Unsupported Options"
#define defSTR_SOFTWARE_RESET_FAILED        "Software Reset failed"

#define defSTR_SIMULATE_MENU_TEXT           "Simulate"

#define defSTR_PEAK_USB_DEVICE              "pcan_usb"
//#define defSTR_PEAK_NPNP_DEVICE             _T("peakcan")
//#define defSTR_PEAK_USB_DEVICE_NAME         _T("PEAK USB-CAN")
// Maximum name length is 20
#define defSTR_PEAK_USB_HW_NET_NAME                "BUSMASTER USB Net"
#define defSTR_PEAK_USB_SIM_NET_NAME               "BUSMASTER Sim Net"
#define defPEAK_USB_SIM_HW_HANDLE              0
#define defPEAK_USB_SIM_NET_HANDLE             32
#define defPEAK_USB_MAX_HW_COUNT               32

#define defUSB_PARAM_BUFFER_SIZE               1024
#define defUSB_PARAM_READ_BUFFER_SIZE          128

#define defSTR_USB_CLIENT_NAME              "BUSMASTER USB"
#define defMAX_WAIT_TX                      2000

#define defUSB_MODE_ACTIVE                     1
#define defUSB_MODE_PASSIVE                    2
#define defUSB_MODE_SIMULATE                   3

#define defPP_MODE_ACTIVE                      1
#define defPP_MODE_PASSIVE                     2
#define defPP_MODE_SELF_RECEPTION              3

#define defMODE_ACTIVE                         1
#define defMODE_PASSIVE                        2
#define defMODE_SIMULATE                       3

// This is controller mode
#define defCONTROLLER_ACTIVE                   1
#define defCONTROLLER_PASSIVE                  2
#define defCONTROLLER_BUSOFF                   3


#define defSTR_MODE_MISMATCH                "No USB Hardware found. Switching to Simulation mode"
#define defSTR_HW_TEST_SUCCESS              "Hardware Interface test passed successfully"
#define defSTR_PP_MODE_EPP_FAILED           "Unable to set the parallel port to EPP mode"
#define defSTR_PP_MODE_NON_EPP_FAILED       "Unable to set the parallel port to Non EPP mode"
#define defSTR_USB_DRIVER_IS_NOT_RUNNING    "Unable to communicate with the driver or Unable to create client networks. Application will quit"

#define defSTR_NET_NAME_FORAMT              "%s_%d"
#define defMAX_VERSION_LEN                     256

#define defSTR_HW_DISPLAY_FORMAT            "CAN %d"
#define defCONNECTED_IMAGE_INDEX            1
#define defDISCONNECTED_IMAGE_INDEX         0

#define defSTR_HW_HANDLE_DISPLAY_FORMAT     "%d"
#define defSTR_FW_VERSION_DISPLAY_FORMAT    "%d.%d"
#define defSTR_NOT_CONNECTED                "Not Connected"

// For filter value. The init will block all messages
#define defINIT_CODE_VALUE                  0x00
#define defINIT_MASK_VALUE                  0xFF

#define defERR_OK                           0
//#define defBAUD_RATE                        0x4914 // 100 KBPS

// Bus Statistics Dialog
#define defSTR_FORMAT_UINT_DATA             "%u"
#define defSTR_FORMAT_FLOAT_DATA            "%.2f"
// Graph UI Thread
#define defSTR_GRAPH_WINDOW_NAME            "Graph Display"
#define defGRAPH_WINDOW_RATIO_X            0.9
#define defGRAPH_WINDOW_RATIO_Y            0.7

// Graph Left View
#define defSTR_LIST_HEADING_CATOGORY        "Category"
#define defSTR_LIST_HEADING_ELEMENT         "Element"
#define defSTR_LIST_HEADING_TYPE            "Type"

#define defCOL_CATOGORY_INDEX               0
#define defCOL_ELEMENT_INDEX                1
#define defCOL_TYPE_INDEX                   2

#define defCOL_CATOGORY_WIDTH               69
#define defCOL_ELEMENT_WIDTH                79
#define defCOL_TYPE_WIDTH                   50

#define defELEMENT_SELECTION_CATOGORY_WIDTH     95
#define defELEMENT_SELECTION_ELEMENT_WIDTH      105
#define defELEMENT_SELECTION_TYPE_WIDTH         65

#define desSTR_INVALID_MAINFRAME_POINTER    "Invalid main frame pointer"

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
#define defSTR_MAX_ELEMENTS_EXCEEDED_MSG    "Only 10 graph elements are supported. Please delete existing element to add new element"

#define defSTR_INVALID_RANGE        "Please enter valid range value"


#define defSTR_SHOW_GRID        "Show Grid"
#define defSTR_HIDE_GRID        "Hide Grid"
#define defUPDATE_TIMER_ID      100

#define defDEFAULT_XRANGE_MAX   10
#define defDEFAULT_XRANGE_MIN   0

#define defSTR_Y_AXIS_FORMAT        "%0.4f"
#define defSTR_X_AXIS_FORMAT        "%0.3fS"

// Track Modes
#define defTRACK_MODE_NONE              0
#define defTRACK_MODE_TOOLTIP           1
#define defTRACK_MODE_ZOOM              2
#define defTRACK_MODE_PAN_XY            3
#define defTRACK_MODE_PAN_X             4
#define defTRACK_MODE_PAN_Y             5

// Zoom Mode sub catogory
#define defZOOM_X_AXIS                  0
#define defZOOM_Y_AXIS                  1
#define defZOOM_XY_AXIS                 2

#define defTIMER_RESOLUTION_FACTOR     10000.0

#define defDEFAULT_LINE_WIDTH           1
#define defSELECTED_LINE_WIDTH          2

// Color Picker
//#define defSTR_DEFAULT_TEXT         _T("Automatic")
//#define defSTR_CUSTOM_TEXT          _T("More Colours...")

// Color Popup
//#define defCOLOR_BOX_SIZE           18

// Configuration
#define defIMPORT_WARNING_GRAPH   "Dissociation will clear Graph element List for all buses.\nDo you want to Clear the List?"
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

// Child Frame
#define defGRAPH_WINDOW_ROWS        1
#define defGRAPH_WINDOW_COLS        2

#define defLEFT_VIEW_PROPOTION      0.25
#define defRIGHT_VIEW_PROPOTION     0.75

#define defGRAPH_VIEW_PROPOSION     (2.0 / 3.0 )
#define defBOTTOM_VIEW_PROPOSION    (1.0 / 3.0 )

#define defGRAPH_WINDOW_RIGHT_ROWS  2
#define defGRAPH_WINDOW_RIGHT_COLS  1

// Export Dialog

#define defSTR_BAUD_RATE_FORMAT     "Channel %-2d -> %s kbps"

#define defSTR_EXPORT_DIALOG_CAPTION    "Select Export Filename..."
//#define defSTR_CSV_FORMAT_SPECIFIER _T("Comma Separated Values format (*.csv)|*.csv||")
//#define defSTR_EXPORT_CREATE_SUCCESS _T("CSV report successfully generated and saved at %s")
#define defSTR_CSV_FILE_PATH_EMPTY      "Please enter valid CSV report file path"

#define defSTR_REPORT_DIALOG_CAPTION    "Select Report Filename..."
#define defSTR_HTML_FORMAT_SPECIFIER "HTML Report Format (*.html)|*.html||"
#define defSTR_HTML_FORMAT           "html"
#define defSTR_REPORT_CREATE_SUCCESS "HTML report successfully generated and saved at %s"
#define defSTR_HTML_FILE_PATH_EMPTY      "Please enter valid HTML report file path"

#define defSTR_BITMAP_DIALOG_CAPTION    "Select Picture Filename..."
#define defSTR_BMP_FORMAT_SPECIFIER "Bitmap Picture Format (*.bmp)|*.bmp||"
#define defSTR_BMP_FORMAT           "bmp"
#define defSTR_bmp_FILE_PATH_EMPTY      "Please enter valid bitmap file path"

#define defSTR_DEFAULT_BROWSE_LOCATION  "about:blank"

// Bus Statistics Dialog
#define defHEADING_COLOR            RGB(150,0,0)
#define defITEM_COLOR               RGB(0,0,150)

#define defSTR_COL_PARAMETER        "Parameter"
#define defSTR_COL_PARAMETER_VAL    "Value"
#define defSTR_INITIAL_VAL          "-"
#define defPARAMETER_COL_WIDTH      200
#define defVALUE_COL_WIDTH          90
#define defDIALOG_BORDER_OFFSET     10
#define defLIST_BORDER_OFFSET       5

// Parameter List
#define defSTR_PARAMETER_MSG_TOTAL         "Messages      [Total]"
#define defSTR_PARAMETER_MSG_RATE          "Messages      [Msg/s]"
#define defSTR_PARAMETER_ERR_TOTAL         "Errors        [Total]"
#define defSTR_PARAMETER_ERR_RATE          "Errors        [Err/s]"
#define defSTR_PARAMETER_LOAD              "Load"
#define defSTR_PARAMETER_PEAK_LOAD         "Peak Load"
#define defSTR_PARAMETER_AVG_LOAD          "Average Load"

#define defSTR_TX_HEADING                  "          Transmitted    "

#define defSTR_PARAMETER_TX_TOTAL          "Total"

#define defSTR_PARAMETER_TX_STD_TOTAL      "Standard      [Total]"
#define defSTR_PARAMETER_TX_STD_RATE       "Standard      [Msg/s]"

#define defSTR_PARAMETER_TX_EXD_TOTAL      "Extended      [Total]"
#define defSTR_PARAMETER_TX_EXD_RATE       "Extended      [Msg/s]"
#define defSTR_PARAMETER_TX_STD_RTR        "Standard RTR"
#define defSTR_PARAMETER_TX_EXD_RTR        "Extended RTR"

#define defSTR_PARAMETER_TX_ERR_TOTAL      "Errors        [Total]"
#define defSTR_PARAMETER_TX_ERR_RATE       "Error         [Err/s]"

#define defSTR_RX_HEADING                  "          Received    "

#define defSTR_PARAMETER_RX_TOTAL          "Total"

#define defSTR_PARAMETER_RX_STD_TOTAL      "Standard      [Total]"
#define defSTR_PARAMETER_RX_STD_RATE       "Standard      [Msg/s]"

#define defSTR_PARAMETER_RX_EXD_TOTAL      "Extended      [Total]"
#define defSTR_PARAMETER_RX_EXD_RATE       "Extended      [Msg/s]"
#define defSTR_PARAMETER_RX_STD_RTR        "Standard.RTR"
#define defSTR_PARAMETER_RX_EXD_RTR        "Extended RTR"

#define defSTR_PARAMETER_RX_ERR_TOTAL      "Errors        [Total]"
#define defSTR_PARAMETER_RX_ERR_RATE       "Error         [Err/s]"

#define defSTR_STATUS_HEADING              "          Status    "
#define defSTR_PARAMETER_CONTROLLER_STATUS     "Controller"
#define defSTR_PARAMETER_TX_ERROR_COUNT        "Tx Error Counter"
#define defSTR_PARAMETER_PEAK_TX_ERROR_COUNT   "Peak Tx Error Counter"
#define defSTR_PARAMETER_RX_ERROR_COUNT        "Rx Error Counter"
#define defSTR_PARAMETER_PEAK_RX_ERROR_COUNT   "Peak Rx Error Counter"

#define defSTR_ACTIVE_STATE                 "Active"
#define defSTR_PASSIVE_STATE                "Passive"
#define defSTR_BUSOFF_STATE                 "Bus Off"
#define defSTR_UNKNOWN_STATE                "Unknown"

#define defSTR_CONTROL_NOT_REGISTERED_MSG  "Graph component is not installed properly. Please reinstall the application!!"

/* - Hardware Interface Layer - */
#define defSTR_MISSING_HARDWARE "Required number of hardware are not present\nRequired: %2d    Available: %2d"

#define defSTR_CHANNEL_NAME                "Channel"
#define defSTR_CHANNEL_NAME_FORMAT         "%s %d"

#define defSTR_HARDWARE_COL_NAME           "Hardware"
#define defSTR_CHANNEL_COL_WIDTH           90
#define defSTR_HARDWARE_COL_WIDTH          75

#define defCHANNEL_COL                     0
#define defHARDWARE_COL                    1

#define defSTR_TEMP_CLIENT_NAME            "TEMP_CLIENT"
#define defSTR_TEMP_NET_NAME               "TEMP_NET"

#define defSTR_NOT_FULLY_CONFIGURED        "Required number of hardware are not configured.\nApplication will use default channel association with hardware"
/* -------------- Properties Dialog ---------------- */
#define defICON_SIZE                        16
#define defICON_GROW                        1

#define defSTR_BUSMASTER_VERSION_HEADING      "BUSMASTER Ver "
#define defSTR_CHANNEL_HEADING             "Channels"
#define defSTR_LOGS_HEADING                "Log Files"
#define defSTR_REPLAY_HEADING              "Replay Files"
#define defSTR_DATABASE_HEADING            "Databases"
#define defSTR_USER_PROG_HEADING           "User Programs"


#define defSTR_CHANNEL_HEADING             "Channels"

#define defSTR_NOT_SELECTED                "Not Selected !!"
#define defSTR_NOT_IMPORTED                "Not Imported !!"
#define defSTR_DLL_NOT_LOADED              "No Functions DLL loaded !!"

#define defSTR_CHANNEL_ITEM_FORMAT         "Channel %-2d at %-9s Kbps"
#define defSTR_CHANNEL_NOT_PRESENT         " (Not Active)"
// Error strings
#define defSTR_ERROR_GETTTING_BAUDRATE     "Error in getting Baudrate information !!"
#define defSTR_ERROR_GETTTING_LOGINFO      "Error in getting Log file name !!"
#define defSTR_ERROR_GETTTING_REPLAYINFO   "Error in getting Replay file name !!"
#define defSTR_ERROR_GETTTING_DB_INFO      "Error in getting Active Database file name !!"
#define defSTR_ERROR_GETTTING_DLL_INFO     "Error in getting Functions DLL loaded name !!"

#define defVERSION_ICON_INDEX               0
#define defCHANNEL_ICON_INDEX               1
#define defLOGFILE_ICON_INDEX               2
#define defREPLAYFILE_ICON_INDEX            3
#define defDATABASE_ICON_INDEX              4
#define defUSERDLL_ICON_INDEX               5

/* ---------------- Controller Configuration ----------------- */
#define defSTR_CHANNEL_SELECTION_ERROR      "Unable to select the device"
#define defSTR_BTR0_COL_HEADING             "BTR0"
#define defSTR_BTR1_COL_HEADING             "BTR1"
#define defSTR_CNF1_COL_HEADING             "CNF1"
#define defSTR_CNF2_COL_HEADING             "CNF2"
#define defSTR_CNF3_COL_HEADING             "CNF3"
#define defSTR_SAMPLE_POINT_COL_HEADING     "Sampling Point"
#define defSTR_NBT_COL_HEADING              "NBT"
#define defSTR_SJW_COL_HEADING              "SJW"
#define defSTR_BRP_COL_HEADING              "BRP"

#define defCHANNEL_ICON_SIZE                24
#define defCHANNEL_LIST_INIT_SIZE           5
#define defCHANNEL_LIST_GROW_SIZE           1
/* ---------------------- Main Frame -----------------------*/
#define defSTR_CHANNEL_TEST_PASS_FORMAT     "Channel %-2d : Passed\n"
#define defSTR_CHANNEL_TEST_FAIL_FORMAT     "Channel %-2d : Failed\n"
#define defSTR_CHANNELS_SUPPORTED           "%d %s(s)"
#define defSTR_STAT_WINDOW_CREATION_FAILED  "Could not be able to create network statistics window"

/* ---------------- Configuration Module ---------------------- */
#define defSTR_MORE_CHANNEL_INFORMATION_FOUND       "%d channels information found in the configuration file.\nApplication will support only %d channel(s).\nTaking first %d channel(s) information"
#define defSTR_LESS_CHANNEL_INFORMATION_FOUND       "%d channel(s) information found in the configuration file.\nApplication will supports %d channels.\nTaking default value for unavailable channel(s) information"

/* -------------- Graph element selection ------------------- */
#define defSTR_INVALID_CHANNEL              "Channel not found. Do you want to remove this entry from the list?"

// USB Driver error codes and corresponfing error strings
#define defSTR_CAN_ERR_OK               "Success"      // No error
#define defSTR_CAN_ERR_XMTFULL          "Transmit buffer in CAN controller is full"
#define defSTR_CAN_ERR_OVERRUN          "CAN controller Transmit buffer overflow"
#define defSTR_CAN_ERR_BUSLIGHT         "Bus error: an error counter reached the 'light' limit"
#define defSTR_CAN_ERR_BUSHEAVY         "Bus error: an error counter reached the 'heavy' limit"
#define defSTR_CAN_ERR_BUSOFF           "Bus error: the CAN controller is in bus-off state"
#define defSTR_CAN_ERR_QRCVEMPTY        "Receive queue is empty"
#define defSTR_CAN_ERR_QOVERRUN         "Receive buffer overflow"
#define defSTR_CAN_ERR_QXMTFULL         "Transmit queue is full"
#define defSTR_CAN_ERR_REGTEST          "Test of the CAN controller hardware registers failed (no hardware found)"
#define defSTR_CAN_ERR_NOVXD            "USB Driver is not running"
#define defSTR_CAN_ERR_HWINUSE          "Hardware is in use by another Net"
#define defSTR_CAN_ERR_NETINUSE         "a Client is already connected to the Net"
#define defSTR_CAN_ERR_ILLHW            "Unable to connect to Hardware"
#define defSTR_CAN_ERR_ILLNET           "Unable to connect to Network"
#define defSTR_CAN_ERR_ILLCLIENT        "Client handle is wrong"
#define defSTR_CAN_ERR_RESOURCE         "Driver resource creation error"
#define defSTR_CAN_ERR_ILLPARAMTYPE     "Invalid parameter type"
#define defSTR_CAN_ERR_ILLPARAMVAL      "Invalid parameter value"
#define defSTR_CAN_ERR_UNKNOWN          "Unknown error %x"

// Error Result strings
#define defSTR_ERROR_REPORT_FORMAT          "Channel %-2d : %s"

#define defSTR_WARNING_LIMIT_SET_FAILED     "Setting warning limit failed"
#define defSTR_FILTER_SET_FAILED            "Setting hardware filter failed"
#define defSTR_CONNECT_FAILED               "Connect failed"
#define defSTR_RESET_FAILED                 "Hardware reset failed"
#define defSTR_CLIENT_RESET_FAILED          "Software reset failed"
#define defSTR_CONTROLLER_MODE_SET_FAILED   "Setting hardware mode failed"

//For adding MSgID to message name
#define defSTR_MSG_ID_IN_HEX          "[0x%x]"

// Configuration Module
#define defSTR_CONFIGURATION_TITLE          "Configuration"

// Tx Message Configuration window
#define defSTR_TX_WINDOW_TITLE              "Configure Transmission Messages"
#define defSTR_TX_SAVE_CONFIRMATION         "Do you want to save changes?"

// Simulated system Configuration window
#define defSTR_SIMSYS_WINDOW_TITLE              "Configure Simulated systems"
#define defSTR_SIMSYS_SAVE_CONFIRMATION         "Do you want to save changes?"

// Tx Window Splitter Position
#define defTX_WND_ROOT_SPLITTER_RATIO       0.865
#define defTX_WND_LEFT_SPLITTER_RATIO       0.27
#define defTX_WND_RIGHT_SPLITTER_RATIO      0.440

#define defMSG_WND_HEIGHT_PROPOTION         0.5

#define defSIGNAL_WATCH_WND_WIDTH_RATIO     0.65
#define defSIGNAL_WATCH_WND_HEIGHT_RATIO    0.35

#define defMSG_IPET_WND_WIDTH_RATIO         0.65
#define defMSG_IPET_WND_HEIGHT_RATIO        0.35

#define defGRAPH_WND_TOP_MARGIN            0.05
#define defGRAPH_WND_BOTTOM_MARGIN         0.25
#define defGRAPH_WND_LEFT_MARGIN           0.05
#define defGRAPH_WND_RIGHT_MARGIN          0.05

#define defTX_MSG_WND_BOTTOM_MARGIN        0.14
#define defTX_MSG_WND_RIGHT_MARGIN         0.17

#define defBORDER_WIDTH_FACTOR             5

// Log File Trigger
#define defLOG_TRIGGER_NONE                 0
#define defLOG_TRIGGER_START                1
#define defLOG_TRIGGER_STOP                 2
#define defLOG_TRIGGER_ALL                  3


// Filter
#define defFILTER_TYPE_SINGLE_ID                0
#define defFILTER_TYPE_ID_RANGE                 1

#define defFILTER_IGNORE_CHANNEL        0
#define defFILTER_IGNORE_DIRECTION      0
#define defFILTER_IGNORE_ID_TYPE        2
#define defFILTER_IGNORE_MSG_TYPE       2

#define defSTR_FILTER_NAME          "Name"
#define defSTR_FILTER_TYPE          "Type"

#define defSTR_FILTER_DETAILS_ID_FROM           "From"
#define defSTR_FILTER_DETAILS_ID_TO             "To"
#define defSTR_FILTER_DETAILS_ID_TYPE           "ID Type"
#define defSTR_FILTER_DETAILS_MSG_TYPE          "Msg Type"
#define defSTR_FILTER_DETAILS_MSG_DIRECTION     "Direction"
#define defSTR_FILTER_DETAILS_MSG_CHANNEL       "Channel"

#define defSTR_SELECTION_ALL                    "All"

#define defSTR_MSG_ID_TYPE_STD                  "Std"
#define defSTR_MSG_ID_TYPE_EXTD                 "Exd"

#define defSTR_MSG_TYPE_RTR                     "RTR"
#define defSTR_MSG_TYPE_NON_RTR                 "Non RTR"

#define defSTR_MSG_DIR_TX                       "Tx"
#define defSTR_MSG_DIR_RX                       "Rx"

#define defSTR_FILTER_TYPE_PASS                 "Pass"
#define defSTR_FILTER_TYPE_STOP                 "Stop"

#define defSTR_MSG_ID_FORMAT_DB                 "%x [%s]"
#define defSTR_MSG_ID_FORMAT_NDB                "%x"

#define defFILTER_IMAGE_INDEX_DB                0
#define defFILTER_IMAGE_INDEX_NDB               1
#define defFILTER_IMAGE_INDEX_FILTER            2
#define defFILTER_IMAGE_INDEX_ID_RANGE          3

#define defNUMBER_OF_FILTERS_SUPPORTED          100

#define defSTR_FILTER_NAME_COLUMN       "Filter Name"

#define defSTR_DEFAULT_FILTER_NAME              "Filter"
#define defSTR_DEFAULT_FILTER_FORMAT            "%s_%d"

// 0 - None, 1 - Start, 2 - Stop, 3 - Both
#define defLOG_TRIGGER_NONE             0
#define defLOG_TRIGGER_START            1
#define defLOG_TRIGGER_STOP             2
#define defLOG_TRIGGER_BOTH             3


#define defREPLAY_RETAIN_DELAY          0
#define defREPLAY_SPECIFIC_DELAY        1

#define defREPLAY_MODE_MONOSHOT          0
#define defREPLAY_MODE_CYCLIC            1

#define defSTR_REPLAY_FILE_SELECTION_TITLE  "Select Replay File Name"
#define defSTR_REPLAY_ERROR         "Replay failed for: %s, %s"
#define defSTR_REPLAY_FILE_EMPTY    "File is empty and all messages are filtered"

#define defSTR_REPLAY_WINDOW_TITLE  "Replay Window - "

#define defBREAK_POINT_MAP_SIZE         17
#define defSTR_BUSMASTER_VERSION_STRING    "BUSMASTER Ver "
#define defSTR_FILE_OPEN_ERROR          "Input file open error"
#define defSTR_LOG_FILE_UNSUPPORTED     "Unsupported version log file"
#define defSTR_LOG_PRTOCOL_MISMATCH     "Protocol Mismatch \nSession(s) cannot be replayed"
#define defSTR_MIXED_MODE_WARNING       "Mode Mismatch\n%d session(s) cannot be replayed"
#define defSTR_PROTOCOL_USED            "PROTOCOL "
#define defSTR_PROTOCOL_CAN             "PROTOCOL CAN"


#define defINTERACTIVE_THREAD_WAIT_TIME                 500
#define defNON_INTERACTIVE_THREAD_WAIT_TIME             250

#define defSTR_MSG_ID_STANDARD              "Standard"
#define defSTR_MSG_ID_EXTENDED              "Extended"

//Definitions for Major and Minor Versions

#define defMAJOR_VERSION                    1
#define defMINOR_VERSION                    0
//To get the Version Number from the application DLL
#define defUNSUPPORTED_VER                  "Unsupported DLL Version %d.%d"
#define defOLD_VER                          "%s is created with old version of BUSMASTER. Please recompile the source code to get latest version of DLL"

// Log File Parsing
// Major Version supported
#define defLOG_FILE_MAJOR_VERSION_SUPPORTED         1
// Position of Minor Version in Version String
#define defMINOR_VERSION_POSITION                   0
//This line is used to separate two files Build,Build&load messages on output window
#define defSTR_BUILD_TRACE_LINE_MARK               " "
