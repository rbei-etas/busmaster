#pragma once


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

#define APPLICATION_NAME _("FRAME")
#define TX_DATA_LENGTH  0
#define TX_DATA_BYTE    1
#define WM_TX_EDIT_COMPLETE         (WM_USER + 27)


#define defSTR_FRAME_NAME           "Table/Frame"
#define defSTR_FRAME_ID             "ID(hex)"
#define defSTR_FRAME_TYPE           "Type"
#define defSTR_DELAY                "Delay[ms]"

#define defCOLOR_WHITE           RGB(255,255,255)
#define defSIGNAL_LIST_STRING_MAX_LENGTH     50
#define defSIGNAL_FRAME_COLUMN      9
#define defSTR_COL_SIGNAL_NAME          _("Signal Name")


#define defMSGID_NAME_DELIMITER  "["
#define defMSGID_NAME_START_CHAR '['


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

#define STATIC_MSG_DATA_LENGTH              _("Data length of static messages can not be changed")
#define DYNAMIC_MSG_DATA_LENGTH             _("Dynamic data length is more than the set value")