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
 * \file      MsgFrmtWnd.cpp
 * \authors   Ratnadip Choudhury, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// MsgFrmtWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BUSMASTER.h"
#include "include/utils_macro.h"
#include "common.h"
#include "MsgFrmtWnd.h"
#include "Include/CAN_Error_Defs.h"
// Time calculation related functionality
#include "TimeManager.h"


// CMsgFrmtWnd

const int SIZE_APP_CAN_BUFFER       = 5000;
/* Error message related definitions and declarations : BEGIN */
static CString sg_omColmStr;
const DWORD COLOUR_ERROR_MSG    = RGB(255, 0, 0);
const int ERRORS_DEFINED = 13;

const BYTE MSG_FRMT_WND_VERSION = 0x1;

// Dummy structure to get rid of Reference init
struct sERRORMSGINFO
{
    unsigned short m_usErrorCode; // Error code
    char* m_ptcErorMsg;          // Error message
};
typedef sERRORMSGINFO SERRORMSGINFO;
typedef sERRORMSGINFO* PERRORMSGINFO;


static SERRORMSGINFO sg_asErrorEntry[ERRORS_DEFINED] =
{
    {ERROR_UNKNOWN,       _T("Unknown Error")},
    {BIT_ERROR_RX,        _T("Bus Error - Bit Error(Rx)")},
    {FORM_ERROR_RX,       _T("Bus Error - Form Error(Rx)")},
    {STUFF_ERROR_RX,      _T("Error Frame - Stuff error(Rx)")},
    {OTHER_ERROR_RX,      _T("Bus Error - Other Error(Rx)")},
    {BIT_ERROR_TX,        _T("Bus Error - Bit Error(Tx)")},
    {FORM_ERROR_TX,       _T("Bus Error - Form Error(Tx)")},
    {STUFF_ERROR_TX,      _T("Bus Error - Stuff Error(Tx)")},
    {OTHER_ERROR_TX,      _T("Bus Error - Other Error(Tx)")},
    {ERROR_DEVICE_BUFF_OVERFLOW,        _T("Controller Buffer Overflow")},
    {ERROR_DRIVER_BUFF_OVERFLOW,        _T("Driver Buffer Overflow")},
    {ERROR_APPLICATION_BUFF_OVERFLOW,   _T("Application Buffer Overflow")},
    {ERROR_DLLMSG_BUFF_OVERFLOW,        _T("DLL message buffer Overflow")}
};

//Structure for SubErrors
struct sSUBERRORMSGINFO
{
    char* m_ptcSubErorMsg;          // SubError message
};
typedef sSUBERRORMSGINFO SSUBERRORMSGINFO;
typedef sSUBERRORMSGINFO* PSUBERRORMSGINFO;
//max no. of error bit defined is 28,one more element so that the
//error names can be placed at the error bit no.
const int SUB_ERRORS_DEFINED = 29;
//Static array having definitions of all sub array
static SSUBERRORMSGINFO sg_asSubErrorEntry[SUB_ERRORS_DEFINED] =
{
    //for 0 and 1 no error defined
    _T(" "),
    _T(" "),
    _T("Other Errors"),
    _T("SOF bit error"),
    _T("Other Errors"),
    _T("Other Errors"),
    _T("Other Errors"),
    _T("Other Errors"),
    _T("CRC error"),
    _T("Other Errors"),
    _T("Other Errors"),
    _T("DLC error"),
    _T("Other Errors"),
    _T("Other Errors"),
    _T("Other Errors"),
    _T("Other Errors"),
    _T(" "),
    _T("local error "),
    _T("Intermission field error"),
    _T("Tolerate Dominant Bits Error"),
    _T(" "),
    _T(" "),
    _T(" "),
    _T("Other Errors"),
    _T("Other Errors"),
    _T("Acknowledgement error"),
    _T("EOF error"),
    _T("Other Errors"),
    _T("over load condition")
};

/* Error message related definitions and declarations : END */

CMsgFrmtWnd* CMsgFrmtWnd::pThisPtr[nMaxProtocol];
CMenu CMsgFrmtWnd::menu;
extern SBUSSTATISTICS g_sBusStatistics[ defNO_OF_CHANNELS ];
extern CCANMonitorApp theApp;
extern CBaseDIL_CAN* g_pouDIL_CAN_Interface;
// DIL CAN interface

#define defDEF_DISPLAY_UPDATE_TIME          100
#define TIMER_ID_DISPLAY_UPDATE             102

#define MAX_LINE_LENGTH_TOOL_TIP            624

STCAN_MSG sDummy0002;

extern HWND g_hMainGUI;

const __int64 nInvalidKey        = 0;

#define SIGNAL_FORMAT  _T("%-30s %-16s %-30s %-16s")

#define defMSG_IPET_WND_BORDER_WIDTH        1
#define defMSG_IPET_WND_HEIGHT_RATIO        0.35
#define defMSG_IPET_WND_WIDTH_RATIO         0.65
#define defMSG_IPET_WND_HEIGHT_RATIO        0.35
#define defSTR_COLLAPSE_MENU_TEXT           _T("&Collapse")

// Start sMsgDispMapEntry implementation
sMsgDispMapEntry::sMsgDispMapEntry()
{
    // Init to invalid value
    m_mListIndex     = -1;
    m_nBufferIndex   = -1;
    m_nSignalCnt     = 0;
    m_eInterpretMode = NON_INTERPRETABLE ;
    m_nTimeStamp     = 0;
    m_nTimeOffset    = 0;
    m_opTreeWndParam = NULL;
}

sMsgDispMapEntry::sMsgDispMapEntry(const sMsgDispMapEntry& sRhsObj)
{
    // Init to invalid value
    m_mListIndex     = sRhsObj.m_mListIndex;
    m_nBufferIndex   = sRhsObj.m_nBufferIndex;
    m_nSignalCnt     = sRhsObj.m_nSignalCnt;
    m_eInterpretMode = sRhsObj.m_eInterpretMode;
    m_nTimeStamp     = sRhsObj.m_nTimeStamp;
    m_nTimeOffset    = sRhsObj.m_nTimeOffset;
    m_opTreeWndParam = sRhsObj.m_opTreeWndParam;
}

sMsgDispMapEntry& sMsgDispMapEntry::operator=(const sMsgDispMapEntry& sRhsObj)
{
    // Init to invalid value
    m_mListIndex     = sRhsObj.m_mListIndex;
    m_nBufferIndex   = sRhsObj.m_nBufferIndex;
    m_nSignalCnt     = sRhsObj.m_nSignalCnt;
    m_eInterpretMode = sRhsObj.m_eInterpretMode;
    m_nTimeStamp     = sRhsObj.m_nTimeStamp;
    m_nTimeOffset    = sRhsObj.m_nTimeOffset;
    m_opTreeWndParam = sRhsObj.m_opTreeWndParam;
    return *this;
}
// Ends sMsgDispMapEntry implementation

// MsgContainerBase INTERFACE



IMPLEMENT_DYNAMIC(CMsgFrmtWnd, CMDIChildWnd)
CMsgFrmtWnd::CMsgFrmtWnd(): m_sCurrEntry(sDummy0002), m_ouMsgAttr(CMessageAttrib::ouGetHandle(CAN))
{
}

CMsgFrmtWnd::CMsgFrmtWnd(ETYPE_BUS eBusType): m_sCurrEntry(sDummy0002), m_ouMsgAttr(CMessageAttrib::ouGetHandle(eBusType))
{
    m_pouMsgContainerIntrf = NULL;
    m_bInterPretMsg = FALSE;
    m_hMainWnd =NULL;
    m_bSignalWatchON = FALSE;
    m_unDispUpdateTimerId = 0;
    m_bUpdate = FALSE;
    m_nField =-1;
    m_bAscending =true;
    CLEAR_EXPR_FLAG(m_bExprnFlag_Disp);
    SET_TM_SYS(m_bExprnFlag_Disp);
    SET_NUM_HEX(m_bExprnFlag_Disp);
    SET_MODE_APPEND(m_bExprnFlag_Disp);
    InitializeCriticalSection(&m_CritSec1);
    InitializeCriticalSection(&m_omCritSecForMapArr);
    InitializeCriticalSection(&m_omCritSecClearAll);
    m_omMgsIndexVec.reserve(nOverWriteMaxMsg);
    m_eBusType = eBusType;
    CMsgFrmtWnd::pThisPtr[m_eBusType] = this;
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);
    //m_ouPsdiHandler.vLoadPSDI_Dll(m_eBusType);
    m_pouMsgContainerIntrf->vSetRxMsgCallBkPtr(CMsgFrmtWnd::vRxMsgCallBk);
    //m_ouPsdiHandler.vSetRxMsgCallBkPtr(CMsgFrmtWnd::vRxMsgCallBk);
    m_bConnected = FALSE;
    m_pExpandedMapIndexes = NULL;
    m_ppMsgDB = NULL;

    for(int i=0; i<MAX_MSG_WND_COL_CNT; i++)
    {
        m_pomDataPtrArr[i] = NULL;
    }

    m_nIndex = 0;
    m_unCurrInterpretedMsgID = static_cast < UINT > (-1);
    m_podMsgIntprtnDlg = NULL;
    // Append Buffer Size
    m_anMsgBuffSize[defAPPEND_DATA_INDEX] = defDEF_APPEND_BUFFER_SIZE;
    // Overwrite Buffer Size
    m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = defDEF_OVERWRITE_BUFFER_SIZE;
    // Display Update Rate
    m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = defDEF_DISPLAY_UPDATE_TIME;
    m_nPrevToolCol = m_nPrevToolRow = -1;
}
CMsgFrmtWnd::~CMsgFrmtWnd()
{
    DeleteCriticalSection(&m_omCritSecForMapArr);
    DeleteCriticalSection(&m_omCritSecClearAll);
    DeleteCriticalSection(&m_CritSec1);

    if(m_podMsgIntprtnDlg)
    {
        m_podMsgIntprtnDlg->DestroyWindow();
        m_podMsgIntprtnDlg = NULL;
    }
}


BEGIN_MESSAGE_MAP(CMsgFrmtWnd, CWnd)
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_MESSAGE(WM_UPDATE_FONT, OnUpdateFont)
    ON_MESSAGE(WM_NOTIFICATION_FROM_OTHER, vNotificationFromOtherWin)
    ON_MESSAGE(WM_PARENT_UPDATE_MSGPTR,vUpdateFormattedMsgStruct)
    ON_MESSAGE(WM_PARENT_SORT_COLUMN,vSortMsgWndColumn)
    ON_MESSAGE(WM_PARENT_UPDATE_MSG_CLR,vUpdateMsgClr)
    ON_WM_TIMER()
    ON_MESSAGE(WM_WND_PROP_MODIFY, ModifyMsgWndProperty)
    ON_MESSAGE(WM_PROVIDE_WND_PROP, ProvideMsgWndProperty)
    ON_MESSAGE(WM_SET_FILTER_DETAILS, SetFilterDetails)
    ON_MESSAGE(WM_GET_FILTER_DETAILS, GetFilterDetails)
    ON_MESSAGE(WM_ENABLE_FILTER_APPLIED, EnableFilterApplied)
    ON_MESSAGE(IDM_CLEAR_MSG_WINDOW, OnClearAll)
    ON_MESSAGE(WM_SHOW_MESSAGE_WINDOW, OnShowHideMessageWindow)
    //ON_MESSAGE(WM_OPEN_CLOSE_SIG_WATCH, vOpenCloseSignalWatchWnd)
    ON_MESSAGE(WM_INVALIDATE_LIST_DISPLAY, vInvalidateListDisplay)
    ON_MESSAGE(WM_GET_INTERPRET_STATE, vOnGetInterpretState)
    ON_MESSAGE(WM_CLEAR_SORT_COLUMN, vOnClearSortColumns)
    ON_MESSAGE(WM_UPDATE_CONNECT_STATUS, vOnUpdateConnectionStatus)
    ON_MESSAGE(WM_EXPAND_COLLAPSE_MSG, vOnExpandCollapseMsg)
    ON_MESSAGE(WM_SETFOCUS_MSGWND_LIST, vOnSetFocusMsgList)
    ON_MESSAGE(WM_GET_NEXT_PREV_MSG_INDEX, vOnGetNextPrevMsgIndex)
    ON_MESSAGE(WM_LSTC_DBLCLK, OnListCtrlMsgDblClick)
    ON_MESSAGE(WM_UPDATE_MSG_INTRP_WND_PLC, OnUpdateMsgIntrpWndPlcmnt)
    ON_MESSAGE(WM_UPDATE_TREE_ITEMS_POS, OnUpdateMsgTreeItemsPositions)
    ON_WM_PARENTNOTIFY()
    ON_WM_CLOSE()
    ON_COMMAND(IDM_MSGWND_RESET_COLUMNS, OnMsgwndResetColumns)
    ON_COMMAND(IDM_MESSAGE_SEND, OnSendSelectedMessageEntry)
    ON_COMMAND(IDM_MESSAGE_EXPAND, OnExpandSelectedMessageEntry)
    ON_MESSAGE(WM_DATABASE_CHANGE, OnToggleInterpretStatusAllEntries)
    ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()


// CMsgFrmtWnd message handlers

/*******************************************************************************
  Function Name  : Create
  Input(s)       : szTitle, style, rect, parent
  Output         : BOOL
  Functionality  : Creation and Registration of MsgFrmtWnd class go here.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
BOOL CMsgFrmtWnd::Create(LPCTSTR szTitle, LONG style, const RECT& rect,
                         CMDIFrameWnd* parent)
{
    // Setup the shared menu
    if (menu.m_hMenu == NULL)
    {
        menu.LoadMenu(IDR_MAINFRAME);
    }

    m_hMenuShared = menu.m_hMenu;
    // Register a custom WndClass and create a window.
    // This must be done because CHelloWnd has a custom icon.
    LPCTSTR lpszReceiveWndClass =
        AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
                            LoadCursor(NULL, IDC_ARROW),
                            (HBRUSH) (COLOR_WINDOW+1),
                            LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_MSGWND)));
    m_hWndOwner = parent->m_hWnd;
    return CMDIChildWnd::Create(lpszReceiveWndClass, szTitle, style, rect, parent);
}

/*******************************************************************************
  Function Name  : OnCreate
  Input(s)       : LPCREATESTRUCT
  Output         : returns 0 as int
  Functionality  : Creation of List Control and all its initializations go here.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
int CMsgFrmtWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    RECT ClientRect;
    GetClientRect(&ClientRect);
    BOOL bResult = FALSE;
    bResult = m_lstMsg.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS |
                              LVS_SINGLESEL | LVS_ALIGNLEFT | LVS_OWNERDATA | WS_CLIPCHILDREN|
                              WS_BORDER | WS_TABSTOP, ClientRect, this, IDC_LSTC_MESSAGE);

    if(bResult)
    {
        m_lstMsg.vDoInitialization();
        m_lstMsg.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );
        //m_wndHeader.SubclassWindow(m_lstMsg.GetHeaderCtrl()->m_hWnd);
        VERIFY(m_lstMsg.m_wndHeader.SubclassWindow(m_lstMsg.GetHeaderCtrl()->GetSafeHwnd()));
        m_hMainWnd = g_hMainGUI;
        SendMessage(WM_UPDATE_FONT, 0, 0);
        vSetDefaultHeaders();
        m_MsgHdrInfo.vGetHdrColStruct(m_sHdrColStruct);
        m_lstMsg.vSetSortableMsgColumns(m_sHdrColStruct, m_eBusType);
        m_lstMsg.vShowHideBlankcolumn(m_bInterPretMsg);
    }
    else
    {
        AfxMessageBox(_T("Not able to create the list control."));
    }

    m_podMsgIntprtnDlg = new CMessageInterpretation(this);
    m_podMsgIntprtnDlg->vCreateMsgIntrprtnDlg(this, FALSE);

    if(m_eBusType == J1939)
    {
        m_podMsgIntprtnDlg->vSetCaption(_T("PGN"));
    }

    m_objToolTip.Create(this);
    m_objToolTip.Activate(TRUE);
    m_objToolTip.SetMaxTipWidth(MAX_LINE_LENGTH_TOOL_TIP);
    CWnd* pWndChild = GetWindow(GW_CHILD);
    CString strToolTip;

    while (pWndChild)
    {
        if (strToolTip.LoadString(IDR_MESSAGE_WINDOW_TEXT))
        {
            m_objToolTip.AddTool(pWndChild, strToolTip);
        }

        pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
    }

    return 0;
}

/*******************************************************************************
  Function Name  : OnSize
  Input(s)       : nType, cx, cy
  Output         : -
  Functionality  : Resize the List Control based on the new size of MsgFrmtWnd.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (m_lstMsg.m_hWnd != NULL)
    {
        CRect rctClient;
        GetClientRect(&rctClient);
        m_lstMsg.SetWindowPos(&wndTop, 0, 0, rctClient.Width(),
                              rctClient.Height(), SWP_NOZORDER);
        ::SendMessage( m_hWnd, WM_UPDATE_TREE_ITEMS_POS, 0, 0);
    }
}
/*******************************************************************************
  Function Name  : GetFilterDetails
  Input(s)       : -
  Output         : -
  Functionality  : Set Filter details
  Member of      : CMsgFrmtWnd
  Author(s)      : Pradeep Kadoor
  Date Created   :
  Modifications  :
*******************************************************************************/

LRESULT CMsgFrmtWnd::GetFilterDetails(WPARAM wParam, LPARAM /*lParam*/)
{
    //  ETYPE_BUS eBus = (ETYPE_BUS)lParam;
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

    switch (m_eBusType)
    {
        case CAN:
        {
            SFILTERAPPLIED_CAN* psFilterAppliedParam =
                (SFILTERAPPLIED_CAN*)wParam;
            m_pouMsgContainerIntrf->GetFilterScheme((void*)psFilterAppliedParam);
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }

    return 0;
}
/*******************************************************************************
  Function Name  : SetFilterDetails
  Input(s)       : -
  Output         : -
  Functionality  : Set Filter details
  Member of      : CMsgFrmtWnd
  Author(s)      : Pradeep Kadoor
  Date Created   :
  Modifications  :
*******************************************************************************/

LRESULT CMsgFrmtWnd::SetFilterDetails(WPARAM wParam, LPARAM /*lParam*/)
{
    //    ETYPE_BUS eBus = (ETYPE_BUS)lParam;
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

    switch (m_eBusType)
    {
        case CAN:
        {
            SFILTERAPPLIED_CAN* psFilterAppliedParam =
                (SFILTERAPPLIED_CAN*)wParam;
            m_pouMsgContainerIntrf->ApplyFilterScheme((void*)psFilterAppliedParam);
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }

    return 0;
}
/*******************************************************************************
  Function Name  : EnableFilterApplied
  Input(s)       : -
  Output         : -
  Functionality  : Enables filter applied
  Member of      : CMsgFrmtWnd
  Author(s)      : Pradeep Kadoor
  Date Created   :
  Modifications  :
*******************************************************************************/

LRESULT CMsgFrmtWnd::EnableFilterApplied(WPARAM wParam, LPARAM /*lParam*/)
{
    //    ETYPE_BUS eBus = (ETYPE_BUS)lParam;
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

    switch (m_eBusType)
    {
        case CAN:
        {
            m_pouMsgContainerIntrf->EnableFilterApplied((BOOL)wParam);
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }

    return 0;
}
/*******************************************************************************
  Function Name  : ProvideMsgWndProperty
  Input(s)       : -
  Output         : -
  Functionality  : Give Msg Wnd Property
  Member of      : CMsgFrmtWnd
  Author(s)      : Anish
  Date Created   :
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::ProvideMsgWndProperty(WPARAM wParam, LPARAM /*lParam*/)
{
    BYTE* tempDispFlag = (BYTE*)wParam;
    *tempDispFlag  = m_bExprnFlag_Disp;
    return 0L;
}

/*******************************************************************************
  Function Name  : vOnGetInterpretState
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : Handler for message event WM_GET_INTERPRET_STATE.
                   Sends the Interpret state through wParam.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vOnGetInterpretState(WPARAM wParam, LPARAM /*lParam*/)
{
    BOOL* pbInterpret = NULL;
    pbInterpret = (BOOL*) wParam;
    *pbInterpret = m_bInterPretMsg;
    return 0;
}

/*******************************************************************************
  Function Name  : vOnClearSortColumns
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : Handler for message event WM_CLEAR_SORT_COLUMN.
                   Clears the current sorting in List Control.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vOnClearSortColumns(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    m_lstMsg.m_wndHeader.ClearArrowDirection();
    return 0;
}

/*******************************************************************************
  Function Name  : vOnUpdateConnectionStatus
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : Handler for message event WM_UPDATE_CONNECT_STATUS.
                   Updates the List Control with current Connect status.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vOnUpdateConnectionStatus(WPARAM wParam, LPARAM /*lParam*/)
{
    m_bConnected = wParam!=0 ? true:false;
    m_lstMsg.SetConnectionStatus(m_bConnected);
    return 0;
}

/*******************************************************************************
  Function Name  : vOnExpandCollapseMsg
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : Handler for message event WM_EXPAND_COLLAPSE_MSG.
                   Expands or collapses the Message on Arrows/Enter key presses.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 13-07-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vOnExpandCollapseMsg(WPARAM wParam, LPARAM lParam)
{
    int nIndex = (int)wParam;
    eTreeItemStates eTreeState =(eTreeItemStates)lParam;

    if (IS_MODE_INTRP(m_bExprnFlag_Disp))
    {
        vExpandContractMsg( nIndex, eTreeState);
    }

    return 0;
}

/*******************************************************************************
  Function Name  : vOnSetFocusMsgList
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : Handler for message event WM_SETFOCUS_MSGWND_LIST.
                   Sets the Focus of List Ctrl in Message Window based on WParam.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 15-07-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vOnSetFocusMsgList(WPARAM wParam, LPARAM /*lParam*/)
{
    bool bSetFocus = wParam!=0 ? true:false;

    if(bSetFocus)
    {
        m_lstMsg.SetFocus();
    }

    return 0;
}

/*******************************************************************************
  Function Name  : vInvalidateListDisplay
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : Handler for message event WM_INVALIDATE_LIST_DISPLAY.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vInvalidateListDisplay(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    m_lstMsg.Invalidate();
    return 0;
}

/*******************************************************************************
  Function Name  : OnClearAll
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : Handler for message event IDM_CLEAR_MSG_WINDOW.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::OnClearAll(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    OnEditClearAll();
    return 0;
}

/*******************************************************************************
  Function Name  : OnEditClearAll
  Input(s)       : _
  Output         : -
  Functionality  : Clears all the Message Window contents and PSDI_CAN DLL buffers.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::OnEditClearAll()
{
    //Remove all the Msg signal tree
    vRemoveAllMsgTree();
    m_omMsgDispMap.RemoveAll();
    m_omMgsIndexVec.clear();
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

    if(m_pouMsgContainerIntrf != NULL)
    {
        m_pouMsgContainerIntrf->vEditClearAll();
    }

    m_lstMsg.DeleteAllItems();
    m_lstMsg.Invalidate();
}

/*******************************************************************************
  Function Name  : vRemoveAllMsgTree
  Input(s)       : _
  Output         : -
  Functionality  : Clears all the Message Window contents.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
//This will be called on clear window
void CMsgFrmtWnd::vRemoveAllMsgTree()
{
    // Variable to keep entry details
    SMSGDISPMAPENTRY sTemp;
    // Variable to hold message ID
    __int64 n64Temp = 0;
    POSITION pos = m_omMsgDispMap.GetStartPosition();

    while( pos != NULL )
    {
        m_omMsgDispMap.GetNextAssoc( pos, n64Temp, sTemp );

        if (sTemp.m_opTreeWndParam != NULL)
        {
            delete sTemp.m_opTreeWndParam;
            sTemp.m_opTreeWndParam = NULL;
            m_omMsgDispMap [n64Temp] = sTemp;
        }
    }

    m_omMsgDispMap.RemoveAll();
}

/*******************************************************************************
  Function Name  : OnParentNotify
  Input(s)       : message, lParam
  Output         : -
  Functionality  : Handles the WM_LBUTTONDOWN event for handling Message
                   exapansion\collapsing signal information on single clicks.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::OnParentNotify(UINT message, LPARAM lParam)
{
    int nEvent = LOWORD(message);

    if (nEvent == WM_LBUTTONDOWN)
    {
        int nIndex;
        UINT nFlag;
        // Make sure this event occurs for a valid message item
        nIndex = m_lstMsg.HitTest(CPoint(LOWORD(lParam),
                                         HIWORD(lParam)),
                                  &nFlag);

        if ( nIndex >= 0 )
        {
            if (TRUE && (nFlag & LVHT_ONITEM))
            {
                //vHandleInterpretation( nIndex );
            }

            //if (bHasEntryIconClicked(nIndex, CPoint(LOWORD(lParam),HIWORD(lParam))))
            if (nFlag == LVHT_ONITEMICON && IS_MODE_INTRP(m_bExprnFlag_Disp))
            {
                vExpandContractMsg( nIndex );
            }

            if(m_podMsgIntprtnDlg->IsWindowVisible())
            {
                if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
                {
                    ::PostMessage(this->m_hWnd, WM_LSTC_DBLCLK, nIndex, NULL);
                }
                else
                {
                    __int64 nMapIndex = m_omMgsIndexVec[nIndex];

                    if (nMapIndex != nInvalidKey)
                    {
                        ::PostMessage(this->m_hWnd, WM_LSTC_DBLCLK, nIndex, NULL);
                    }
                }
            }
        }
    }
    else if (nEvent == WM_RBUTTONDOWN)
    {
        int nIndex = -1;
        // Get the mouse click coordinates and get the list ctrl header rectangle
        CPoint tmpPoint = CPoint(LOWORD(lParam), HIWORD(lParam));
        CRect headerRect;

        if (m_lstMsg.m_hWnd != NULL)
        {
            m_lstMsg.GetHeaderCtrl()->GetClientRect(&headerRect);
        }

        // Check if the mouse click point is within the header rectangle
        if (!headerRect.PtInRect(tmpPoint))
        {
            POSITION Pos = m_lstMsg.GetFirstSelectedItemPosition();

            if (Pos != NULL)
            {
                nIndex = m_lstMsg.GetNextSelectedItem(Pos);
            }

            UINT nFlag;
            // Make sure this event occurs for a valid message item
            int nItem = m_lstMsg.HitTest(
                            CPoint(LOWORD(lParam), HIWORD(lParam)),
                            &nFlag);

            if (nItem == -1)
            {
                int nToStop = 0;
            }

            m_lstMsg.SetItemState( nItem,
                                   LVIS_SELECTED | LVIS_FOCUSED,
                                   LVIS_SELECTED | LVIS_FOCUSED);
            // Take this as selected item
            nIndex = nItem;

            if ((nIndex >= 0) && (nFlag & LVHT_ONITEM))
            {
                CMenu* pomContextMenu = new CMenu;

                if (pomContextMenu != NULL)
                {
                    // Load the Menu from the resource
                    pomContextMenu->DestroyMenu();
                    pomContextMenu->LoadMenu(IDM_MENU_MSG_OPRN);
                    CMenu* pomSubMenu = pomContextMenu->GetSubMenu(0);

                    if( IS_MODE_APPEND(m_bExprnFlag_Disp) || !IS_MODE_INTRP(m_bExprnFlag_Disp))
                        pomContextMenu->EnableMenuItem(IDM_MESSAGE_EXPAND,
                                                       MF_DISABLED |MF_GRAYED);
                    else
                    {
                        SMSGDISPMAPENTRY sTemp;
                        __int64 nMapIndex = m_omMgsIndexVec[nIndex];

                        if (nMapIndex != nInvalidKey)
                        {
                            if (m_omMsgDispMap.Lookup(nMapIndex, sTemp))
                            {
                                if (sTemp.m_eInterpretMode == INTERPRETING)
                                {
                                    pomContextMenu->ModifyMenu( IDM_MESSAGE_EXPAND,
                                                                MF_BYCOMMAND,
                                                                IDM_MESSAGE_EXPAND,
                                                                defSTR_COLLAPSE_MENU_TEXT);
                                }
                            }
                        }
                    }

                    // Enable/Disable Send Item as per connection status
                    BOOL bConnected = TRUE;
                    // Get flag object
                    CFlags* pouFlags = theApp.pouGetFlagsPtr();

                    // If valid
                    if(pouFlags != NULL )
                    {
                        bConnected = pouFlags->nGetFlagStatus( CONNECTED );
                    }

                    // If it is not connected then disable it other wise enable
                    if( bConnected == FALSE )
                    {
                        pomContextMenu->EnableMenuItem( IDM_MESSAGE_SEND,
                                                        MF_DISABLED |MF_GRAYED);
                    }

                    if (pomSubMenu != NULL)
                    {
                        CPoint omSrcPt(lParam);
                        ClientToScreen(&omSrcPt);
                        pomSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
                                                   omSrcPt.x, omSrcPt.y, this, NULL);
                    }

                    delete pomContextMenu;
                    pomContextMenu = NULL;
                }
            }
        }
    }

    CMDIChildWnd::OnParentNotify(message, lParam);
}

/******************************************************************************
 Function Name  :   OnSendSelectedMessageEntry

 Input(s)       :   -
 Output         :   -
 Functionality  :   Transmits the selected message entry from the message list
 Member of      :   CMsgFrmtWnd

 Author(s)      :   ArunKumar K
 Date Created   :   09/09/2010
******************************************************************************/
const int SIZE_STCAN_MSG = sizeof(STCAN_MSG);
void CMsgFrmtWnd::OnSendSelectedMessageEntry()
{
    POSITION Pos = m_lstMsg.GetFirstSelectedItemPosition();

    if (Pos != NULL)
    {
        PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);
        int nItem = m_lstMsg.GetNextSelectedItem(Pos);

        //For CAN Messages
        if(m_eBusType == CAN)
        {
            STCANDATA sInfo;
            HRESULT hResult;

            if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
            {
                hResult = m_pouMsgContainerIntrf->hReadFromAppendBuffer(&sInfo, nItem);
            }
            else
            {
                __int64 nMsgKey = m_omMgsIndexVec[nItem];
                hResult = m_pouMsgContainerIntrf->hReadFromOWBuffer(&sInfo, nMsgKey);
            }

            // Send only valid messages
            if( hResult == S_OK && IS_A_MESSAGE( sInfo.m_ucDataType ) )
            {
                //HRESULT hRet =
                g_pouDIL_CAN_Interface->DILC_SendMsg(m_dwClientID, sInfo.m_uDataInfo.m_sCANMsg);
            }
        }
        //For J1939 Messages
        else if(m_eBusType == J1939)
        {
            STJ1939_MSG sJ1939Msg;
            HRESULT hResult;

            if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
            {
                hResult = m_pouMsgContainerIntrf->hReadFromAppendBuffer(&sJ1939Msg, nItem);
            }
            else
            {
                __int64 nMsgKey = m_omMgsIndexVec[nItem];
                hResult = m_pouMsgContainerIntrf->hReadFromOWBuffer(&sJ1939Msg, nMsgKey);
            }

            // Send only valid messages
            //Arun 14-02-2011 -- Need to check for J1939 messages' validness
            if( hResult == S_OK /*&& IS_A_MESSAGE( sInfo.m_ucDataType )*/ )
            {
                //HRESULT hRet =
                (*m_ppouIJ1939DIL)->DILIJ_SendJ1939Msg(m_dwClientID,
                                                       sJ1939Msg.m_sMsgProperties.m_byChannel,
                                                       sJ1939Msg.m_sMsgProperties.m_eType,
                                                       sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(),
                                                       sJ1939Msg.m_pbyData, sJ1939Msg.m_unDLC,
                                                       sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority,
                                                       sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress,
                                                       sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
            }
        }
    }
}

/******************************************************************************
 Function Name  : OnExpandSelectedMessageEntry

 Input(s)       : -
 Output         : -
 Functionality  : This function will be called to Expand/Collapse the selected
                  message with the interpretted values.
 Member of      : CMsgFrmtWnd

 Author(s)      : ArunKumar K
 Date Created   : 09/09/2010
******************************************************************************/
void CMsgFrmtWnd::OnExpandSelectedMessageEntry()
{
    POSITION Pos = m_lstMsg.GetFirstSelectedItemPosition();

    if (Pos != NULL)
    {
        int nItem = m_lstMsg.GetNextSelectedItem(Pos);
        vExpandContractMsg( nItem );
    }
}
/*******************************************************************************
  Function Name  : vInit
  Input(s)       : pParam
  Output         : -
  Functionality  : Initialize the PSDI DLL.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vInit(void* pParam)
{
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);
    m_pouMsgContainerIntrf->vInit(pParam);
    m_pouMsgContainerIntrf->bGetDilInterFace();
}

/*******************************************************************************
  Function Name  : OnTimer
  Input(s)       : nIDEvent
  Output         : -
  Functionality  : Called during Timer events and updates the List Control.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::OnTimer(UINT nIDEvent)
{
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

    if (nIDEvent == m_unDispUpdateTimerId)
    {
        if (m_bUpdate == TRUE)
        {
            //Get count of list control
            int nBuffMsgCnt = 0;

            //Now for upadating no. of items in list ctrl
            if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
            {
                nBuffMsgCnt = m_pouMsgContainerIntrf->nGetAppendBufferCount();
            }
            else
            {
                //For overwrite the no. of item in list control is
                //equal to no. of item in array and not equal to the
                //item count in VFSE buffer
                nBuffMsgCnt = m_omMgsIndexVec.size();
            }

            m_lstMsg.SetItemCountEx(nBuffMsgCnt);
            //vDoInterpretation();
            m_lstMsg.Invalidate(FALSE);
            m_bUpdate = FALSE;

            if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
            {
                m_lstMsg.SetItemState( nBuffMsgCnt-1,
                                       LVIS_SELECTED | LVIS_FOCUSED,
                                       LVIS_SELECTED | LVIS_FOCUSED);
                m_lstMsg.EnsureVisible( nBuffMsgCnt-1, TRUE);
            }

            //Update Message Interpretation Window.
            if( m_podMsgIntprtnDlg->IsWindowVisible() )
            {
                m_nIndex = nBuffMsgCnt-1;
                SSignalInfoArray SigInfoArray;
                vGetSignalInfoArray(m_unCurrInterpretedMapIndex, SigInfoArray);
                CString strName;
                strName = strGetMsgNameOrCode(m_unCurrInterpretedMsgID);
                int nCnt = SigInfoArray.GetSize();
                SSignalInfo sTempSignal;
                CStringArray arrSignals;
                CStringArray arrSigRawValues;
                CStringArray arrSigPhyValues;
                CStringArray arrSigUnits;

                for (int i = 0; i < nCnt; i++)
                {
                    sTempSignal = SigInfoArray.GetAt(i);
                    arrSignals.Add(sTempSignal.m_omSigName);
                    arrSigRawValues.Add(sTempSignal.m_omRawValue);
                    arrSigPhyValues.Add(sTempSignal.m_omEnggValue);
                    arrSigUnits.Add(sTempSignal.m_omUnit);
                }

                m_podMsgIntprtnDlg->vUpdateMessageData(m_unCurrInterpretedMsgID
                                                       ,strName,arrSignals,arrSigRawValues,arrSigPhyValues,
                                                       arrSigUnits, IS_NUM_HEX_SET(m_bExprnFlag_Disp));
            }

            //Update Signal Tree items.
            EnterCriticalSection(&m_CritSec1);

            if(IS_MODE_INTRP(m_bExprnFlag_Disp))
            {
                vUpdateAllTreeWnd();
            }

            LeaveCriticalSection(&m_CritSec1);
        }
    }

    CMDIChildWnd::OnTimer(nIDEvent);
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

/*******************************************************************************
  Function Name  : vNotificationFromOtherWin
  Input(s)       : wParam
  Output         : Returns 0 as LRESULT
  Functionality  : Handles Notifications from other windows.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vNotificationFromOtherWin(WPARAM wParam, LPARAM lParam)
{
    // wParam tells which window posts this message
    // lParam contains the different informations been passed
    switch (wParam)
    {
        case eLOAD_DATABASE:
        {
            m_ppMsgDB = (CMsgSignal**)lParam;

            if(m_eBusType == J1939)
            {
                SMSGENTRY* psMsgEntry = NULL;
                vPopulateMsgEntryFromDB(psMsgEntry, *m_ppMsgDB);
                m_ouMsgInterpretJ1939.vSetJ1939Database(psMsgEntry);
            }
        }
        break;

        case eWINID_START_READ:
        {
            bStartReadThread();
        }
        break;

        case eWINID_STOP_READ:
        {
            bStopReadThread();
        }
        break;

        case eWINID_MSG_WND_GET_CONFIG_SIZE:
        {
            UINT unSize = nGetMsgFrmtWndConfigSize();
            UINT* pSize = (UINT*)lParam;
            *pSize = unSize;
        }
        break;

        case eWINID_MSG_WND_GET_CONFIG_DATA:
        {
            BYTE* pbyData = (BYTE*)lParam;
            GetConfigData(pbyData);
        }
        break;

        case eWINID_MSG_WND_SET_CONFIG_DATA:
        {
            BYTE* pbyData = (BYTE*)lParam;
            SetConfigData(pbyData);
        }
        break;

        case eWINID_MSG_WND_GET_BUFFER_DETAILS:
        {
            INT* pMsgBuffSize = (INT*)lParam;

            //If it is NULL values.i.e user did not configure buffer details.
            if(pMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] == 0)
            {
                break;
            }

            vUpdateMsgBufferDetails(pMsgBuffSize);
        }
        break;
    }

    return 0;
}

/*******************************************************************************
  Function Name  : vUpdateMsgBufferDetails
  Input(s)       : -
  Output         : -
  Functionality  : CMsgFrmtWnd
                   Updates the Message Buffer details of Append and overwrite buffers.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 06-09-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vUpdateMsgBufferDetails(INT* pMsgBuffSize)
{
    // Append Buffer Size
    m_anMsgBuffSize[defAPPEND_DATA_INDEX] = pMsgBuffSize[defAPPEND_DATA_INDEX];
    // Overwrite Buffer Size
    m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = pMsgBuffSize[defOVERWRITE_DATE_INDEX];
    // Display Update Rate
    m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = pMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX];

    if ((m_unDispUpdateTimerId = SetTimer(TIMER_ID_DISPLAY_UPDATE,
                                          m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX], NULL)) == 0)
    {
        // Log error message
    }

    //RADAR Defect Seq No.52:
    //Need to set the Append and Overwrite Buffer sizes in PSDI_CAN module.
}


/*******************************************************************************
  Function Name  : OnUpdateMsgIntrpWndPlcmnt
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : CMsgFrmtWnd
                   Handler for message event WM_UPDATE_MSG_INTRP_WND_PLC.
                   Updates the Message Interpretation window placement details.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 06-09-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::OnUpdateMsgIntrpWndPlcmnt(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    m_podMsgIntprtnDlg->GetWindowPlacement(&m_sMsgIntrpWndPlacement);
    return 0;
}

/*******************************************************************************
  Function Name  : OnListCtrlMsgDblClick
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : CMsgFrmtWnd
                   Handler for message event WM_LSTC_DBLCLK.
                   Shows the Message Interpretation window on Message double click.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 06-08-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::OnListCtrlMsgDblClick(WPARAM wParam, LPARAM lParam)
{
    __int64 nMapIndex = 0;
    CPoint* pPoint = (CPoint*)lParam;

    if(lParam!=NULL)
    {
        UINT nFlag;
        // Make sure this event occurs for a valid message item
        int nIndex = m_lstMsg.HitTest(*pPoint,&nFlag);

        if ( nIndex >= 0 )
        {
            //Return if clicked on first column.
            if (nFlag == LVHT_ONITEMICON)
            {
                return 0;
            }
        }
    }

    if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
    {
        PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

        if(m_eBusType == CAN)
        {
            static STCANDATA sCANMsg;
            m_pouMsgContainerIntrf->hReadFromAppendBuffer(&sCANMsg, (int)wParam);
            m_pouMsgContainerIntrf->nCreateMapIndexKey((LPVOID)&sCANMsg);
        }
        else if(m_eBusType == J1939)
        {
            static STJ1939_MSG sJ1939Msg;
            m_pouMsgContainerIntrf->hReadFromAppendBuffer(&sJ1939Msg, (int)wParam);
            m_pouMsgContainerIntrf->nCreateMapIndexKey((LPVOID)&sJ1939Msg);
        }
    }
    else
    {
        nMapIndex = m_omMgsIndexVec[(int)wParam];
    }

    m_nIndex =  wParam;
    vShowUpdateMsgIntrpDlg(nMapIndex);
    return 0;
}

/*******************************************************************************
  Function Name  : vShowUpdateMsgIntrpDlg
  Input(s)       : nMapIndex
  Output         : Returns 0 as LRESULT
  Functionality  : Updates the Color of a particular message based on index.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vShowUpdateMsgIntrpDlg(__int64 nMapIndex)
{
    int nMsgCode;
    nMsgCode = nGetCodefromMapKey(nMapIndex);
    SSignalInfoArray SigInfoArray;
    vGetSignalInfoArray(nMapIndex, SigInfoArray);
    CString strName;
    strName = strGetMsgNameOrCode(nMsgCode);
    int nCnt = SigInfoArray.GetSize();
    SSignalInfo sTempSignal;
    CStringArray arrSignals;
    CStringArray arrSigRawValues;
    CStringArray arrSigPhyValues;
    CStringArray arrSigUnits;

    for (int i = 0; i < nCnt; i++)
    {
        sTempSignal = SigInfoArray.GetAt(i);
        arrSignals.Add(sTempSignal.m_omSigName);
        arrSigRawValues.Add(sTempSignal.m_omRawValue);
        arrSigPhyValues.Add(sTempSignal.m_omEnggValue);
        arrSigUnits.Add(sTempSignal.m_omUnit);
    }

    m_unCurrInterpretedMsgID = nMsgCode;
    m_unCurrInterpretedMapIndex = nMapIndex;

    if(!m_podMsgIntprtnDlg->IsWindowVisible())
    {
        m_podMsgIntprtnDlg->vClearWindowContent();
        m_podMsgIntprtnDlg->DestroyWindow();
        m_podMsgIntprtnDlg->vCreateMsgIntrprtnDlg(this, FALSE);
    }

    m_podMsgIntprtnDlg->SetWindowPlacement(&m_sMsgIntrpWndPlacement);
    m_podMsgIntprtnDlg->vUpdateMessageData(nMsgCode,strName,arrSignals,arrSigRawValues,arrSigPhyValues,
                                           arrSigUnits, IS_NUM_HEX_SET(m_bExprnFlag_Disp));
    // Send NC paint Message
    m_podMsgIntprtnDlg->SendMessage(WM_NCPAINT, 1, 0);
    // Repaint the client area
    m_podMsgIntprtnDlg->Invalidate();
}
/*******************************************************************************
  Function Name  : vUpdateMsgClr
  Input(s)       : wParam contains Index.
  Output         : Returns 0 as LRESULT
  Functionality  : Updates the Color of a particular message based on index.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  : Arunkumar K,
                   07-09-2010,
                   Updated funtion to support erroneous message display in RED.
*******************************************************************************/
LRESULT CMsgFrmtWnd::vUpdateMsgClr(WPARAM wParam, LPARAM /*lParam*/)
{
    int nMsgCode = 0;
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

    if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
    {
        HRESULT hResult = m_pouMsgContainerIntrf->hUpdateFormattedMsgStruct((int)wParam,
                          nMsgCode,
                          m_bExprnFlag_Disp);

        if(nMsgCode!=-1 && hResult == S_FALSE)  //Erroneous Message
        {
            m_lstMsg.vSetMsgColor(COLOUR_ERROR_MSG);
        }
        else
        {
            m_lstMsg.vSetMsgColor(m_ouMsgAttr.GetCanIDColour(nMsgCode));
        }
    }
    else if(m_omMgsIndexVec.size() > wParam)
    {
        __int64 nMsgKey = m_omMgsIndexVec[(int)wParam];

        if (nMsgKey != 0)
        {
            SMSGDISPMAPENTRY sDispEntry;
            BOOL bFound;
            bFound = m_omMsgDispMap.Lookup(nMsgKey, sDispEntry );

            if (bFound)
            {
                HRESULT hResult = m_pouMsgContainerIntrf->hUpdateFormattedMsgStruct(
                                      (int)sDispEntry.m_nBufferIndex,
                                      nMsgCode,
                                      m_bExprnFlag_Disp);

                if(nMsgCode!=-1 && hResult == S_FALSE)  //Erroneous Message
                {
                    m_lstMsg.vSetMsgColor(COLOUR_ERROR_MSG);
                }
                else
                {
                    m_lstMsg.vSetMsgColor(m_ouMsgAttr.GetCanIDColour(nMsgCode));
                }
            }
        }
    }

    return 0;
}

/*******************************************************************************
  Function Name  : vOnGetNextPrevMsgIndex
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : CMsgFrmtWnd
                   Handler for message event WM_GET_NEXT_PREV_MSG_INDEX.
                   Returns the Next/Previous Message Index.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 15-07-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vOnGetNextPrevMsgIndex(WPARAM wParam, LPARAM lParam)
{
    bool bNext = wParam!=0 ? true:false;
    int* pIndex = (int*)lParam;

    if(bNext)
    {
        for(UINT i= *pIndex+1; i < m_omMgsIndexVec.size(); i++)
        {
            if(m_omMgsIndexVec[i] == 0) //If the Index contains Signal Interpretation, Skip the Index
            {
                *pIndex = *pIndex +1;
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        for(int i= *pIndex-1; i>=0; i--)
        {
            if(m_omMgsIndexVec[i] == 0) //If the Index contains Signal Interpretation, Skip the Index
            {
                *pIndex = *pIndex -1;
            }
            else
            {
                break;
            }
        }
    }

    return 0;
}

/*******************************************************************************
  Function Name  : vUpdateFormattedMsgStruct
  Input(s)       : wParam contains SWMUPDATEPTRPARA
  Output         : Returns 0 as LRESULT
  Functionality  : Formats the Message Data Buffer in PSDI_CAN DLL for required
                   message as requested by List Control.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vUpdateFormattedMsgStruct(WPARAM wParam, LPARAM /*lParam*/)
{
    SWMUPDATEPTRPARA* psParam = NULL;
    HRESULT hResult = S_FALSE;
    int nMsgCode    = 0;
    psParam = (SWMUPDATEPTRPARA*) wParam;
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

    if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
    {
        hResult = m_pouMsgContainerIntrf->hUpdateFormattedMsgStruct(psParam->m_nListIndex,
                  nMsgCode,
                  m_bExprnFlag_Disp);

        if( hResult == S_FALSE && nMsgCode!= -1)
        {
            //Handle Error Msg display
            vFormatCurrErrorEntry((USHORT)nMsgCode, 0);
            m_pouMsgContainerIntrf->vSetCurrMsgName(sg_omColmStr);
        }
        else
        {
            m_pouMsgContainerIntrf->vSetCurrMsgName(strGetMsgNameOrCode(nMsgCode));
        }
    }
    else if(m_omMgsIndexVec.size() > psParam->m_nListIndex)
    {
        //The Items interpret state will only be supplied if the display mode is
        //either overwrite or overwrite interpret mode
        //Over write mode
        EnterCriticalSection(&m_omCritSecForMapArr);
        __int64 nMsgKey = m_omMgsIndexVec[psParam->m_nListIndex];

        if (nMsgKey != 0)
        {
            SMSGDISPMAPENTRY sDispEntry;
            BOOL bFound;
            bFound = m_omMsgDispMap.Lookup(nMsgKey, sDispEntry );

            if (bFound)
            {
                hResult = m_pouMsgContainerIntrf->hUpdateFormattedMsgStruct(sDispEntry.m_nBufferIndex,
                          nMsgCode,
                          m_bExprnFlag_Disp,
                          sDispEntry.m_nTimeOffset);

                if( hResult == S_FALSE && nMsgCode!= -1)
                {
                    //Handle Error Msg display
                    vFormatCurrErrorEntry((USHORT)nMsgCode, 0);
                    m_pouMsgContainerIntrf->vSetCurrMsgName(sg_omColmStr);
                }
                else
                {
                    m_pouMsgContainerIntrf->vSetCurrMsgName(strGetMsgNameOrCode(nMsgCode));
                }

                if (hResult == S_OK)
                {
                    // if it is overwrite mode then even for interprting mode
                    // return interpretable so that it will show + sign
                    // Bcoz in order to remember the mode while interpreting
                    // while switchhing to appent/OW mode of that entry is not changed
                    if ((IS_MODE_OVER(m_bExprnFlag_Disp)) &&
                            ( INTERPRETING == sDispEntry.m_eInterpretMode))
                    {
                        psParam->m_eInPretMode = INTERPRETABLE;
                    }
                    else
                    {
                        psParam->m_eInPretMode = sDispEntry.m_eInterpretMode;
                    }
                }
            }
        }
        else
        {
            m_pouMsgContainerIntrf->vClearFormattedMsgStruct();
        }

        LeaveCriticalSection(&m_omCritSecForMapArr);
    }

    psParam->m_bResult = TRUE;
    return 0;
}

/*******************************************************************************
  Function Name  : vSortMsgWndColumn
  Input(s)       : wParam contains Sort Field value, lParam contains Sort order.
  Output         : Returns 0 as LRESULT
  Functionality  : Sorts the Buffers in PSDI_CAN DLL based on Sort field
                   specified. Also, handles the reordering of m_omMgsIndexVec
                   in case of INTERPRET mode.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::vSortMsgWndColumn(WPARAM wParam, LPARAM lParam)
{
    m_nField = (int)wParam;
    m_bAscending = lParam!=0 ? true:false;
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);
    m_pouMsgContainerIntrf->DoSortBuffer(m_nField,m_bAscending);
    m_pExpandedMapIndexes = (__int64*)malloc(sizeof(__int64) * m_omMgsIndexVec.size());

    if( m_pExpandedMapIndexes == NULL)
    {
        AfxMessageBox(_T("Unable to Allocate Memory."));
        return 0;
    }

    if (IS_MODE_INTRP(m_bExprnFlag_Disp))
    {
        int nExpCnt = 0;

        for(UINT i = 0; i < m_omMgsIndexVec.size(); i++)    //Collapse message entries which are expanded.
        {
            SMSGDISPMAPENTRY sTemp;
            __int64 nMapIndex = m_omMgsIndexVec[i];

            if (nMapIndex != nInvalidKey)
            {
                if (m_omMsgDispMap.Lookup(nMapIndex, sTemp))
                {
                    if (sTemp.m_eInterpretMode == INTERPRETING)
                    {
                        sTemp.m_eInterpretMode = INTERPRETABLE;
                        vExpandContractMsg(i);
                        m_pExpandedMapIndexes[nExpCnt++] = nMapIndex;
                    }
                }
            }
        }

        //Sort the m_omMgsIndexVec Array
        for(UINT i = 0; i < m_omMgsIndexVec.size(); i++)
        {
            __int64 nMapIndex;
            m_pouMsgContainerIntrf->GetMapIndexAtID(i, nMapIndex);
            m_omMgsIndexVec[i] = nMapIndex;
            SMSGDISPMAPENTRY sDispEntry;

            if (m_omMsgDispMap.Lookup(nMapIndex, sDispEntry ))
            {
                sDispEntry.m_nBufferIndex = i;
                m_omMsgDispMap.SetAt(nMapIndex, sDispEntry);
            }
        }

        vCreateAllMsgTree();

        //Expand the Messages which are previously in expandede state.
        for(int i = 0; i<nExpCnt; i++)
        {
            __int64 nMapIndex;
            nMapIndex = m_pExpandedMapIndexes[i];

            for(UINT j =0; j < m_omMgsIndexVec.size(); j++)
            {
                if(nMapIndex == m_omMgsIndexVec[j])
                {
                    vExpandContractMsg(j);
                    break;
                }
            }
        }

        free(m_pExpandedMapIndexes);
    }

    m_lstMsg.Invalidate();
    return 0;
}

/*******************************************************************************
  Function Name  : vUpdateStatistics
  Input(s)       : cTxMode
  Output         : -
  Functionality  : Updates the g_sBusStatistics array for updations in
                   Network statistics Window.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vUpdateStatistics( char cTxMode )
{
    int nChannel = m_sCurrEntry.m_ucChannel - 1;

    if( nChannel < 0 )
    {
        ASSERT( FALSE );
    }

    // Check the Message Type and Update the statistics
    if (cTxMode == 'R') // received message
    {
        if (m_sCurrEntry.m_ucRTR == 0) // Non RTR message
        {
            if (m_sCurrEntry.m_ucEXTENDED == 0)
            {
                g_sBusStatistics[ nChannel ].m_unRxSTDMsgCount++;
            }
            else
            {
                g_sBusStatistics[ nChannel ].m_unRxEXTDMsgCount++;
            }
        }
        else // RTR message
        {
            if (m_sCurrEntry.m_ucEXTENDED == 0)
            {
                g_sBusStatistics[ nChannel ].m_unRxSTD_RTRMsgCount++;
            }
            else
            {
                g_sBusStatistics[ nChannel ].m_unRxEXTD_RTRMsgCount++;
            }
        }
    }
    else // transmitted message
    {
        if (m_sCurrEntry.m_ucRTR == 0) // Non RTR message
        {
            if (m_sCurrEntry.m_ucEXTENDED == 0)
            {
                g_sBusStatistics[ nChannel ].m_unTxSTDMsgCount++;
            }
            else
            {
                g_sBusStatistics[ nChannel ].m_unTxEXTDMsgCount++;
            }
        }
        else // RTR message
        {
            if (m_sCurrEntry.m_ucEXTENDED == 0)
            {
                g_sBusStatistics[ nChannel ].m_unTxSTD_RTRMsgCount++;
            }
            else
            {
                g_sBusStatistics[ nChannel ].m_unTxEXTD_RTRMsgCount++;
            }
        }
    }
}

/*******************************************************************************
  Function Name  : strGetMsgNameOrCode
  Input(s)       : nMsgCode
  Output         : CString containg Message Name or Code.
  Functionality  : Retreives the Message Name of nMsgCode from Database and
                   returns it. If not found, returns the message code.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
CString CMsgFrmtWnd::strGetMsgNameOrCode(UINT nMsgCode)
{
    CString omName = "";

    if (NULL != m_ppMsgDB)
    {
        if ( !(*m_ppMsgDB)->bMessageNameFromMsgCode(nMsgCode, omName))
        {
            //If the message is not found in the database then format the
            //msg code and return
            if (IS_NUM_DEC_SET(m_bExprnFlag_Disp))
            {
                omName.Format(_T("%d"), nMsgCode);
            }
            else
            {
                omName.Format(_T("%x"), nMsgCode);
            }
        }
    }

    return omName;
}

/*******************************************************************************
  Function Name  : vSetDefaultHeaders
  Input(s)       : -
  Output         : -
  Functionality  : Sets the Default Headers based on the BUS type(m_eBusType).
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vSetDefaultHeaders()
{
    SMSGWNDHDRCOL sHdrCtrlPos;
    CString somArrColTitle[MAX_MSG_WND_COL_CNT];

    switch (m_eBusType)
    {
        case CAN:
        {
            int nColCount = 8;
            //Set the positions for coloumns
            sHdrCtrlPos.m_byTimePos     = 0;
            sHdrCtrlPos.m_byRxTxPos     = 1;
            sHdrCtrlPos.m_byChannel     = 2;
            sHdrCtrlPos.m_byMsgTypePos  = 3;
            sHdrCtrlPos.m_byIDPos       = 4;
            sHdrCtrlPos.m_byCodeNamePos = 5;
            sHdrCtrlPos.m_byDLCPos      = 6;
            sHdrCtrlPos.m_byDataPos     = 7;
            //Set the col string
            somArrColTitle[sHdrCtrlPos.m_byTimePos]     = _T("Time              ");
            somArrColTitle[sHdrCtrlPos.m_byRxTxPos]     = _T("Tx/Rx          ");
            somArrColTitle[sHdrCtrlPos.m_byChannel]     = _T("Channel        ");
            somArrColTitle[sHdrCtrlPos.m_byMsgTypePos]  = _T("Type");
            somArrColTitle[sHdrCtrlPos.m_byIDPos]       = _T("ID            ");
            somArrColTitle[sHdrCtrlPos.m_byCodeNamePos] = _T("Message       ");
            somArrColTitle[sHdrCtrlPos.m_byMsgTypePos]  = _T("Msg Type");
            somArrColTitle[sHdrCtrlPos.m_byDLCPos]      = _T("DLC ");
            somArrColTitle[sHdrCtrlPos.m_byDataPos]     = _T("Data Byte(s)                                     ");
            m_MsgHdrInfo.vInitializeColDetails(sHdrCtrlPos, somArrColTitle, nColCount);
            vRearrangeCols();
        }
        break;

        case FLEXRAY:
        {
        }
        break;

        case MCNET:
        {
        }
        break;

        case J1939:
        {
            int nColCount = 12;
            //Set the positions for coloumns
            sHdrCtrlPos.m_byTimePos     = 0;
            sHdrCtrlPos.m_byChannel     = 1;
            sHdrCtrlPos.m_byIDPos       = 2;
            sHdrCtrlPos.m_byPGNPos      = 3;
            sHdrCtrlPos.m_byCodeNamePos = 4;
            sHdrCtrlPos.m_byMsgTypePos  = 5;
            sHdrCtrlPos.m_bySrcPos      = 6;
            sHdrCtrlPos.m_byDestPos     = 7;
            sHdrCtrlPos.m_byPriorityPos = 8;
            sHdrCtrlPos.m_byRxTxPos     = 9;
            sHdrCtrlPos.m_byDLCPos      = 10;
            sHdrCtrlPos.m_byDataPos     = 11;
            //Set the col string
            somArrColTitle[sHdrCtrlPos.m_byTimePos]     = _T("Time             ");
            somArrColTitle[sHdrCtrlPos.m_byChannel]     = _T("Channel    ");
            somArrColTitle[sHdrCtrlPos.m_byIDPos]       = _T("CAN ID      ");
            somArrColTitle[sHdrCtrlPos.m_byPGNPos]      = _T("PGN        ");
            somArrColTitle[sHdrCtrlPos.m_byCodeNamePos] = _T("PGN Name   ");
            somArrColTitle[sHdrCtrlPos.m_byMsgTypePos]  = _T("Type       ");
            somArrColTitle[sHdrCtrlPos.m_bySrcPos]      = _T("Src    ");
            somArrColTitle[sHdrCtrlPos.m_byDestPos]     = _T("Dest       ");
            somArrColTitle[sHdrCtrlPos.m_byPriorityPos] = _T("Priority");
            somArrColTitle[sHdrCtrlPos.m_byRxTxPos]     = _T("Tx/Rx    ");
            somArrColTitle[sHdrCtrlPos.m_byDLCPos]      = _T("DLC ");
            somArrColTitle[sHdrCtrlPos.m_byDataPos]     = _T("Data Byte(s)                                     ");
            m_MsgHdrInfo.vInitializeColDetails(sHdrCtrlPos, somArrColTitle, nColCount);
            vRearrangeCols();
        }
        break;
    }
}

/*******************************************************************************
  Function Name  : vRearrangeCols
  Input(s)       : -
  Output         : -
  Functionality  : Rearranges the message header columns.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vRearrangeCols()
{
    CString omArrColTitle[MAX_MSG_WND_COL_CNT] = {""};
    int nCount = 0;
    m_MsgHdrInfo.vGetHdrColNames(omArrColTitle, nCount);
    vSetColTitles(omArrColTitle, nCount);
    vCreateCols();
}

/*******************************************************************************
  Function Name  : vSetColTitles
  Input(s)       : Array omArrColTitle, nColCount
  Output         : -
  Functionality  : Sets the member column array m_omArrColTitle with omArrColTitle.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vSetColTitles(CString omArrColTitle[], int nColCount)
{
    CString omTmpColName;
    m_omArrColTitle.RemoveAll();

    for (int i = 0; i < nColCount; i++)
    {
        omTmpColName = omArrColTitle[i];

        if ( !omTmpColName.IsEmpty() )
        {
            m_omArrColTitle.Add(omTmpColName);
        }
    }

    m_nColCount = m_omArrColTitle.GetSize();
}

/*******************************************************************************
  Function Name  : vCreateCols
  Input(s)       : -
  Output         : -
  Functionality  : Creation of message header columns in Message Window.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vCreateCols()
{
    //First delete all coloumns
    int nColcnt = m_lstMsg.GetHeaderCtrl()->GetItemCount();

    for (int i = nColcnt-1; i >= 0; i--)
    {
        m_lstMsg.DeleteColumn(i);
    }

    //Now create all the coloumns
    LVCOLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));
    lvcolumn.mask =  LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
    lvcolumn.fmt = LVCFMT_LEFT;
    CString omTmpColTitle;
    // add columns
    omTmpColTitle = _T("bl");
    lvcolumn.iSubItem = 0;
    lvcolumn.pszText = "";
    lvcolumn.cchTextMax = _tcslen(omTmpColTitle.GetBuffer(MAX_PATH));
    lvcolumn.cx = nGetListCtrlTextExtent(omTmpColTitle);
    m_lstMsg.InsertColumn(0, &lvcolumn);

    for (UINT i = 1; i <= m_nColCount; i++)
    {
        omTmpColTitle = m_omArrColTitle[i-1];
        lvcolumn.iSubItem = i;
        lvcolumn.pszText = omTmpColTitle.GetBuffer(MAX_PATH);
        //lvcolumn.cchTextMax = (int)wcslen(omTmpColTitle);
        lvcolumn.cchTextMax = _tcslen(omTmpColTitle.GetBuffer(MAX_PATH));
        lvcolumn.cx = nGetListCtrlTextExtent(omTmpColTitle);
        m_lstMsg.InsertColumn(i, &lvcolumn);
        /* CHeaderCtrlEx::CItemData *pData = new CHeaderCtrlEx::CItemData(lvcolumn.cx, TRUE, TRUE);
         m_wndHeader.SetItemData((int)i, (DWORD_PTR)pData);*/
    }

    vUpdatePtrInLstCtrl();
}

/*******************************************************************************
  Function Name  : nGetListCtrlTextExtent
  Input(s)       : omColTitle
  Output         : Returns the Text Extent.
  Functionality  : Gets the Text Extent of omColTitle and returns the same.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
int CMsgFrmtWnd::nGetListCtrlTextExtent(CString omColTitle)
{
    CSize sz;
    TEXTMETRIC tm;
    int nDx = 0;
    CDC*  pDC = m_lstMsg.GetDC();
    pDC->GetTextMetrics(&tm);
    sz = pDC->GetTextExtent(omColTitle);
    //Add a char avg. width to remove wrapping
    nDx = tm.tmAveCharWidth;
    nDx += sz.cx;
    return nDx;
}

/*******************************************************************************
  Function Name  : vRxMsgCallBk
  Input(s)       : pMsg, eBusType
  Output         : -
  Functionality  : Callback function which receives the Message and dispatches
                   to the BUS of type eBusType.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vRxMsgCallBk(void* pMsg, ETYPE_BUS eBusType)
{
    if (NULL != pThisPtr[eBusType])
    {
        pThisPtr[eBusType]->vOnRxMsg(pMsg);
    }
}

/*******************************************************************************
  Function Name  : bStopReadThread
  Input(s)       : -
  Output         : Boolean value indicating SUCCESS/FAILURE of Thread stop.
  Functionality  : Used to stop the Data Read thread in PSDI_CAN DLL.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
BOOL CMsgFrmtWnd:: bStopReadThread()
{
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);
    return m_pouMsgContainerIntrf->bStopReadThread();
}

/*******************************************************************************
  Function Name  : bStartReadThread
  Input(s)       : -
  Output         : Boolean value indicating SUCCESS/FAILURE of Thread start.
  Functionality  : Used to start the Data Read thread in PSDI_CAN DLL and to
                   set the timer locally for data updation.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
BOOL CMsgFrmtWnd:: bStartReadThread()
{
    int bResult = TRUE;
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);
    m_pouMsgContainerIntrf->SetClientID(m_dwClientID);

    if (m_pouMsgContainerIntrf->bStartReadThread())
    {
        if ((m_unDispUpdateTimerId = SetTimer(TIMER_ID_DISPLAY_UPDATE,
                                              m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX], NULL)) == 0)
        {
            // Log error message
            bResult = FALSE;
        }
    }

    return bResult;
}

/*******************************************************************************
  Function Name  : vOnRxMsg
  Input(s)       : pMsg containing the dispatched Message on transmission/reception.
  Output         : -
  Functionality  : Processes the transmitted/dispatched message and stores it in
                   PSDI_CAN DLL.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vOnRxMsg(void* pMsg)
{
    UINT nMsgCode       = 0;
    __int64 dwTimeStamp = 0;
    __int64 dwMapIndex  = 0;
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);
    SMSGDISPMAPENTRY sDispEntry;
    int nBufIndex = 0;
    m_pouMsgContainerIntrf->vSaveOWandGetDetails(pMsg, dwMapIndex, dwTimeStamp, nMsgCode, nBufIndex);

    //EnterCriticalSection(&m_omCritSecForMapArr);
    if (m_omMsgDispMap.Lookup(dwMapIndex, sDispEntry))
    {
        //Time offset will be previous msg time stamp
        sDispEntry.m_nTimeOffset = sDispEntry.m_nTimeStamp;
        sDispEntry.m_nTimeStamp  = dwTimeStamp;
    }
    else
    {
        //Get the interpretable state
        memset(&sDispEntry, 0, sizeof(sDispEntry));
        sDispEntry.m_nBufferIndex = nBufIndex;
        CString msgName;

        if (m_ppMsgDB != NULL)
        {
            if ((*m_ppMsgDB)->bMessageNameFromMsgCode(nMsgCode, msgName))
            {
                sDispEntry.m_eInterpretMode = INTERPRETABLE;
            }
        }

        //Both times will be same so that rel. time = 0
        sDispEntry.m_nTimeOffset = dwTimeStamp;
        sDispEntry.m_nTimeStamp  = dwTimeStamp;
        //Update array list
        m_omMgsIndexVec.push_back(dwMapIndex);
    }

    //LeaveCriticalSection(&m_omCritSecForMapArr);
    //Update the map
    m_omMsgDispMap[dwMapIndex] = sDispEntry;

    if(m_eBusType == J1939)
    {
        static STJ1939_MSG sJ1939Msg;
        //HRESULT hResult =
        m_pouMsgContainerIntrf->hReadFromOWBuffer(&sJ1939Msg, dwMapIndex);
        m_bUpdate = TRUE;
        return;
    }

    static STCANDATA sCANMsg;
    static char s_cTxMode;
    HRESULT hResult = m_pouMsgContainerIntrf->hReadFromOWBuffer(&sCANMsg, dwMapIndex);
    m_sCurrEntry = sCANMsg.m_uDataInfo.m_sCANMsg;

    if(hResult == S_OK)
    {
        if (IS_ERR_MESSAGE(sCANMsg.m_ucDataType))
        {
            m_sErrorInfo = sCANMsg.m_uDataInfo.m_sErrInfo;
            // Update current absolute time stamp
            int nTimeStamp  = CTimeManager::nCalculateElapsedTimeFromTick(
                                  sCANMsg.m_lTickCount);
            // Take appropriate actions
            USHORT usErrCode = usProcessCurrErrorEntry();

            if( usErrCode != ERROR_UNKNOWN )
            {
                // Format error message
                vFormatCurrErrorEntry(usErrCode,nTimeStamp);
            }
        }
        else
        {
            // Set a few flags and modify the message entry according to
            // its mode (Tx or Rx message).
            if (IS_RX_MESSAGE(sCANMsg.m_ucDataType))
            {
                s_cTxMode = 'R';
                // Update bus statistics object.
                g_sBusStatistics[ m_sCurrEntry.m_ucChannel - 1].
                m_unTotalRxMsgCount++;
            }
            else if (IS_TX_MESSAGE(sCANMsg.m_ucDataType))
            {
                // Transmitted message
                s_cTxMode = 'T';
                g_sBusStatistics[ m_sCurrEntry.m_ucChannel - 1].
                m_unTotalTxMsgCount++;
            }
            else
            {
                TRACE(_T("vOnRxMsg... undefined\n"));
            }

            if (m_sCurrEntry.m_ucRTR)
                // Update Global DLC count
                g_sBusStatistics[ m_sCurrEntry.m_ucChannel - 1].
                m_unDLCCount += m_sCurrEntry.m_ucDataLen;

            // Update Global Statistics
            vUpdateStatistics( s_cTxMode );
        }
    }

    if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
    {
        if( m_unCurrInterpretedMsgID == nMsgCode &&
                m_unCurrInterpretedMapIndex == dwMapIndex)
        {
            m_nIndex = m_lstMsg.GetItemCount();
        }
    }

    m_bUpdate = TRUE;
}

/*******************************************************************************
  Function Name  : usProcessCurrErrorEntry
  Input(s)       : -
  Output         : Returns Error ID as USHORT.
  Functionality  : Processes the current Error entry and returns the Error code.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
USHORT CMsgFrmtWnd::usProcessCurrErrorEntry(void)
{
    // Get the Error code
    USHORT usErrorID;
    // Get the channel number
    CHAR nChannel = m_sErrorInfo.m_ucChannel - 1;

    if( nChannel < 0 || nChannel >= defNO_OF_CHANNELS )
    {
        ASSERT( FALSE );
        // Take prevension
        nChannel = 0;
    }

    // Decide which module(s) to notify by analysing the error code
    // Accordingly notify the modules by sending/posting message
    if (m_sErrorInfo.m_ucErrType == ERROR_BUS)
    {
        // Update Statistics information
        g_sBusStatistics[ nChannel ].m_unErrorTotalCount++;
        usErrorID = m_sErrorInfo.m_ucReg_ErrCap & 0xE0;

        // Received message
        if (usErrorID & 0x20)
        {
            g_sBusStatistics[ nChannel ].m_unErrorRxCount++;
        }
        else
        {
            g_sBusStatistics[ nChannel ].m_unErrorTxCount++;
        }
    }
    else if (m_sErrorInfo.m_ucErrType == ERROR_WARNING_LIMIT_REACHED)
    {
        // Reaching warning limit isn't considered as an error.
        // In case of this interrupt there is no need for display to
        // be updated.
        // Use Channel ID as High Byte of WPARAM
        usErrorID = ERROR_UNKNOWN;
    }
    // Handle interrupts. This is not error message and assign error code
    // to unknown
    // Interrupts are state transition indicators but not real error message
    else if (m_sErrorInfo.m_ucErrType == ERROR_INTERRUPT)
    {
        // Use Channel ID as High Byte of WPARAM
        usErrorID = ERROR_UNKNOWN;
    }
    else
    {
        usErrorID = m_sErrorInfo.m_ucErrType;
    }

    return usErrorID;
}

/*******************************************************************************
  Function Name  : vUpdatePtrInLstCtrl
  Input(s)       : -
  Output         : -
  Functionality  : Gets the pointer to Message data Buffer from PSDI_CAN DLL
                   and updates the same in List Control.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vFormatCurrErrorEntry(USHORT usErrorID, INT /*nTimeStamp*/)
{
    CString omErrMsg("");
    BOOL bErrProcessed = FALSE;
    int nCount = 0;

    while ((nCount < ERRORS_DEFINED) && (bErrProcessed == FALSE))
    {
        if (usErrorID == sg_asErrorEntry[nCount].m_usErrorCode)
        {
            sg_omColmStr = sg_asErrorEntry[nCount].m_ptcErorMsg;
            bErrProcessed = TRUE;
        }

        nCount++;
    }

    if (bErrProcessed == FALSE)
    {
        sg_omColmStr.Format(_T("Error code: %X"), usErrorID);
    }
}

/*******************************************************************************
  Function Name  : vUpdatePtrInLstCtrl
  Input(s)       : -
  Output         : -
  Functionality  : Gets the pointer to Message data Buffer from PSDI_CAN DLL
                   and updates the same in List Control.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vUpdatePtrInLstCtrl()
{
    SMSGWNDHDRCOL sHdrColStruct;
    m_MsgHdrInfo.vGetHdrColStruct(m_sHdrColStruct);
    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);
    //*************************************TBD the header details 4 diff. protocol
    m_pouMsgContainerIntrf->vGetUpdatedCurrDataPtrArray(m_sHdrColStruct, m_pomDataPtrArr, m_bExprnFlag_Disp);
    //****************************************
    m_lstMsg.vSetDsipItemDataPtrArr(m_pomDataPtrArr);
}

/*******************************************************************************
  Function Name  : OnUpdateFont
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : Updates the Font of List Control on message WM_UPDATE_FONT.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::OnUpdateFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    if (m_lstMsg.m_hWnd != NULL)
    {
        CRect rc;
        m_lstMsg.GetWindowRect(&rc);
        WINDOWPOS wp;
        wp.hwnd = m_hWnd;
        wp.cx = rc.Width();
        wp.cy = rc.Height();
        wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
        m_lstMsg.SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM) &wp);
    }

    return 0;
}

/*******************************************************************************
  Function Name  : ModifyMsgWndProperty
  Input(s)       : wParam containing Modes, lParam containing arguments
  Output         : Returns 0 as LRESULT
  Functionality  : Handles the events from MainFrm for modifying the Message Window
                   properties.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::ModifyMsgWndProperty(WPARAM wParam, LPARAM lParam)
{
    BYTE byModes = (BYTE) wParam;
    WORD wArguments = (WORD) lParam;
    BOOL bToUpdate = FALSE;
    EnterCriticalSection(&m_CritSec1);

    if (byModes & NUMERIC)
    {
        // Extract out bits for numeric mode to compare to the current setting
        if ((m_bExprnFlag_Disp & BITS_NUM) != (wArguments & BITS_NUM))
        {
            CLEAR_EXPR_NUM_BITS(m_bExprnFlag_Disp);
            m_bExprnFlag_Disp |= (wArguments & BITS_NUM);
            //m_ouMsgStore.vFormatDisplayBuffers(NUMERIC, m_bExprnFlag_Disp);
            bToUpdate = TRUE;
        }

        vUpdateAllTreeWnd();

        if(m_podMsgIntprtnDlg->IsWindowVisible())
        {
            vShowUpdateMsgIntrpDlg(m_unCurrInterpretedMapIndex);
        }
    }

    if (byModes & TIME_MODE)
    {
        // Extract out bits for time mode to compare to the current setting
        if ((m_bExprnFlag_Disp & BITS_TM) != (wArguments & BITS_TM))
        {
            CLEAR_EXPR_TM_BITS(m_bExprnFlag_Disp);
            m_bExprnFlag_Disp |= (wArguments & BITS_TM);
            //m_ouMsgStore.vFormatDisplayBuffers(TIME_MODE, m_bExprnFlag_Disp);
            //Now chnage the Numeric format of InterpretMsg wnd and SigTree windows
            //vRefreshInterpretation();
            bToUpdate = TRUE;
        }
    }

    if (byModes & DISPLAY_MODE)
    {
        PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

        // Extract out bits for display mode to compare to the current setting
        if ((m_bExprnFlag_Disp & BITS_DISP) != (wArguments & BITS_DISP))
        {
            m_bInterPretMsg = FALSE;

            //If previous mode was interpret then collapse the expanded items
            if (IS_MODE_INTRP(m_bExprnFlag_Disp))
            {
                vExpandContractAllEntries(FALSE, FALSE);
            }

            CLEAR_EXPR_DISP_BITS(m_bExprnFlag_Disp);
            m_bExprnFlag_Disp |= (wArguments & BITS_DISP);
            int nMsgCount = 0;

            if (IS_MODE_APPEND(m_bExprnFlag_Disp))
            {
                // Remove all msg signal tree wnd and then change the mode
                vExpandContractAllEntries(FALSE, FALSE);
                nMsgCount = m_pouMsgContainerIntrf->nGetAppendBufferCount();
                /*if (IS_TM_REL_SET(m_bExprnFlag_Disp))
                {
                    //If append mode set rel base time equal to the time stamp of first
                    //msg in VSE buffer
                    if( m_omMgsIndexVec.size() > 0 )
                    {
                        __int64 ntemp = m_omMgsIndexVec[0];
                        SMSGDISPMAPENTRY sTemp;
                        m_omMsgDispMap.Lookup(ntemp, sTemp);
                    }
                }*/
                //m_lstMsg.vSetDispMode(APPEND);
            }
            else if (IS_MODE_INTRP(m_bExprnFlag_Disp))
            {
                // First change the mode and then create all the sig tree window
                //m_lstMsg.vSetDispMode(OVERWRITE_INTERPRET);
                vChangeStateOfInterpretableMsgs();
                vCreateAllMsgTree();
                m_bInterPretMsg = TRUE;
                nMsgCount = m_omMgsIndexVec.size();

                if( m_nField != -1)
                {
                    ::SendMessage( m_hWnd, WM_PARENT_SORT_COLUMN, (WPARAM)m_nField, (LPARAM)m_bAscending);
                }

                m_nIndex = 0;
                ::SendMessage( m_hWnd, WM_UPDATE_TREE_ITEMS_POS, 0, 0);
            }
            else // if (IS_MODE_OVER(m_bExprnFlag_Disp))
            {
                // Remove all msg signal tree wnd and then change the mode
                vExpandContractAllEntries(FALSE, FALSE);
                nMsgCount = m_omMgsIndexVec.size();
                //m_lstMsg.vSetDispMode(OVERWRITE);
                m_nIndex = 0;
            }

            m_lstMsg.vShowHideBlankcolumn(m_bInterPretMsg);
            m_lstMsg.SetItemCountEx(nMsgCount);
            bToUpdate = TRUE;
        }
    }

    m_bUpdate = bToUpdate;
    vUpdatePtrInLstCtrl();
    LeaveCriticalSection(&m_CritSec1);
    return 0L;
}

/*******************************************************************************
  Function Name  : vExpandContractAllEntries
  Input(s)       : bInterpretationON, bChangeIntptnState
  Output         : -
  Functionality  : Expands/ collapses all message entries based on  bInterpretationON.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vExpandContractAllEntries (BOOL bInterpretationON,
        BOOL bChangeIntptnState)
{
    if (bInterpretationON) //Expand all entries
    {
        //Keep the key of map
        __int64 n64Temp;
        SMSGDISPMAPENTRY sEntry;
        // Now shift the elements down Shift the map
        POSITION pos = m_omMsgDispMap.GetStartPosition();

        while( pos != NULL )
        {
            m_omMsgDispMap.GetNextAssoc( pos, n64Temp, sEntry );

            //If entry is expandable and not already expanded then expand
            if (sEntry.m_eInterpretMode == INTERPRETABLE)
            {
                SSignalInfoArray SigInfoArray;
                vGetSignalInfoArray(n64Temp, SigInfoArray);

                if (0 < SigInfoArray.GetSize())
                {
                    //Change the state and update the list
                    if (bChangeIntptnState)
                    {
                        sEntry.m_eInterpretMode = INTERPRETING;
                    }

                    int nMsgListIndex = nGetMsgListIndexFromMapIndex(n64Temp);

                    if (-1 != nMsgListIndex)
                    {
                        //Expand and update the map
                        vExpandMsgEntry (sEntry, SigInfoArray, nMsgListIndex);
                        m_omMsgDispMap[n64Temp] = sEntry;
                    }
                }
            }
        }
    }
    else  //Contract all entries
    {
        //Keep the key of map
        __int64 n64Temp;
        SMSGDISPMAPENTRY sEntry;
        // Now shift the elements down Shift the map
        POSITION pos = m_omMsgDispMap.GetStartPosition();

        while( pos != NULL )
        {
            m_omMsgDispMap.GetNextAssoc( pos, n64Temp, sEntry );

            //If entry is expanded then contract
            if (sEntry.m_eInterpretMode == INTERPRETING)
            {
                if (bChangeIntptnState)
                {
                    //Change the state and update the map
                    sEntry.m_eInterpretMode = INTERPRETABLE;
                }

                int nMsgListIndex = nGetMsgListIndexFromMapIndex(n64Temp);

                if (-1 != nMsgListIndex)
                {
                    //Contraction and updation of map
                    vContractMsgEntry (sEntry, nMsgListIndex);
                    m_omMsgDispMap[n64Temp] = sEntry;
                }
            }
        }
    }

    m_lstMsg.Invalidate();
}

/*******************************************************************************
  Function Name  : vGetSignalInfoArray
  Input(s)       : nMapIndex, SSignalInfoArray
  Output         : -
  Functionality  : Gets the SSignalInfoArray for a message at nMapIndex.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  : Arunkumar K,
                   07-09-2010,
                   Updated the functionality for supporting signal info of
                   append message types.
*******************************************************************************/
void CMsgFrmtWnd::vGetSignalInfoArray(__int64 nMapIndex, SSignalInfoArray& SigInfoArray)
{
    if(m_eBusType == CAN)
    {
        static CMsgInterpretation ouMsgInterpret;
        static EFORMAT eNumFormat;
        static STCANDATA sCANMsg;
        sMESSAGE* pMsg = NULL;
        SigInfoArray.RemoveAll();
        int nID = nGetCodefromMapKey(nMapIndex);

        if (NULL != m_ppMsgDB)
        {
            pMsg = (*m_ppMsgDB)->psGetMessagePointer(nID);

            if (NULL != pMsg)
            {
                if (IS_NUM_HEX_SET(m_bExprnFlag_Disp))
                {
                    eNumFormat = HEXADECIMAL;
                }
                else
                {
                    eNumFormat = DEC;
                }

                PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

                if(IS_MODE_APPEND(m_bExprnFlag_Disp))
                {
                    int nBuffMsgCnt;
                    nBuffMsgCnt = m_nIndex;
                    HRESULT hResult = m_pouMsgContainerIntrf->hReadFromAppendBuffer(&sCANMsg, nBuffMsgCnt);

                    if(sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID == (UINT)nID && hResult == S_OK)
                    {
                        ouMsgInterpret.bInterpretMsgs(eNumFormat, pMsg,
                                                      sCANMsg.m_uDataInfo.m_sCANMsg.m_ucData, SigInfoArray);
                    }
                    else if( (theApp.pouGetFlagsPtr()->nGetFlagStatus( SENDMESG )
                              /*|| theApp.pouGetFlagsPtr()->nGetFlagStatus(SEND_SIGNAL_MSG)*/)
                             && sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID != (UINT)nID)
                    {
                        HRESULT hResult = m_pouMsgContainerIntrf->hReadFromOWBuffer(&sCANMsg, nMapIndex);

                        if(hResult == S_OK)
                            ouMsgInterpret.bInterpretMsgs(eNumFormat, pMsg, sCANMsg.m_uDataInfo.m_sCANMsg.m_ucData
                                                          , SigInfoArray);
                    }
                }
                else
                {
                    HRESULT hResult = m_pouMsgContainerIntrf->hReadFromOWBuffer(&sCANMsg, nMapIndex);

                    if(hResult == S_OK)
                        ouMsgInterpret.bInterpretMsgs(eNumFormat, pMsg, sCANMsg.m_uDataInfo.m_sCANMsg.m_ucData
                                                      , SigInfoArray);
                }
            }
        }
    }
    else if(m_eBusType == J1939)
    {
        static EFORMAT eNumFormat;
        static STJ1939_MSG sJ1939Msg;
        CString strMsgName;
        SigInfoArray.RemoveAll();
        UINT nID = nGetCodefromMapKey(nMapIndex);

        if (NULL != m_ppMsgDB)
        {
            if (IS_NUM_HEX_SET(m_bExprnFlag_Disp))
            {
                eNumFormat = HEXADECIMAL;
            }
            else
            {
                eNumFormat = DEC;
            }

            PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

            if(IS_MODE_APPEND(m_bExprnFlag_Disp))
            {
                int nBuffMsgCnt;
                nBuffMsgCnt = m_nIndex;
                HRESULT hResult = m_pouMsgContainerIntrf->hReadFromAppendBuffer(&sJ1939Msg, nBuffMsgCnt);

                if(sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN() == nID && hResult == S_OK)
                {
                    m_ouMsgInterpretJ1939.bInterPretJ1939_MSGS(eNumFormat, nID,
                            sJ1939Msg.m_unDLC, sJ1939Msg.m_pbyData, strMsgName, SigInfoArray);
                }
                else if( (theApp.pouGetFlagsPtr()->nGetFlagStatus( SENDMESG )
                          || theApp.pouGetFlagsPtr()->nGetFlagStatus(SEND_SIGNAL_MSG))
                         && sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN() != nID)
                {
                    HRESULT hResult = m_pouMsgContainerIntrf->hReadFromOWBuffer(&sJ1939Msg, nMapIndex);

                    if(hResult == S_OK)
                        m_ouMsgInterpretJ1939.bInterPretJ1939_MSGS(eNumFormat,
                                sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(),
                                sJ1939Msg.m_unDLC, sJ1939Msg.m_pbyData, strMsgName, SigInfoArray);
                }
            }
            else
            {
                HRESULT hResult = m_pouMsgContainerIntrf->hReadFromOWBuffer(&sJ1939Msg, nMapIndex);

                if(hResult == S_OK)
                    m_ouMsgInterpretJ1939.bInterPretJ1939_MSGS(eNumFormat,
                            sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(),
                            sJ1939Msg.m_unDLC, sJ1939Msg.m_pbyData, strMsgName, SigInfoArray);
            }
        }
    }
}

/*******************************************************************************
  Function Name  : vExpandMsgEntry
  Input(s)       : SMSGDISPMAPENTRY, SSignalInfoArray, nMsgIndex
  Output         : -
  Functionality  : Creates a new Tree Item for a particular message with signal data.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vExpandMsgEntry( SMSGDISPMAPENTRY& sEntry,
                                   SSignalInfoArray& SigInfoArray,
                                   int nMsgIndex)
{
    //Insert invalid mapIds (n = no. of signals) after the msg. in array
    //Create the tree, move other trees after this down by no. of signals
    //Setitemcountex = no. of elements in the array
    int nIndexShift = SigInfoArray.GetSize();
    sEntry.m_nSignalCnt = nIndexShift;
    // Insert invalid elements after the message in array
    EnterCriticalSection(&m_omCritSecForMapArr);
    m_omMgsIndexVec.insert((m_omMgsIndexVec.begin() + nMsgIndex + 1),
                           nIndexShift, nInvalidKey);
    //Shift the tree windows which are after the entry
    //if last entry then not reqd.
    int nArrSize = m_omMgsIndexVec.size();
    //Get the TreeCtrl Wnd rect for shifting the already present TreeCtrl Wnd
    // Calculate dimensions of the rectangle
    CRect omTreeRect;
    vGetTreeRect(nMsgIndex, nIndexShift, omTreeRect);

    if(m_nIndex > nMsgIndex)
    {
        m_nIndex += nIndexShift;
    }

    //Keep the key of map
    if( nMsgIndex  < (nArrSize - nIndexShift))
    {
        __int64 n64Temp;
        SMSGDISPMAPENTRY sTemp;

        // Now shift the elements down Shift the map
        for (int i = nMsgIndex + nIndexShift + 1; i < nArrSize; i++)
        {
            n64Temp = m_omMgsIndexVec[i];

            if (n64Temp != nInvalidKey)
            {
                if (m_omMsgDispMap.Lookup(n64Temp, sTemp))
                {
                    //Shift the Tree window
                    if (sTemp.m_opTreeWndParam != NULL)
                    {
                        sTemp.m_opTreeWndParam->vShift (omTreeRect.Height() +
                                                        defMSG_IPET_WND_BORDER_WIDTH);
                    }
                }
                else
                {
                    ASSERT(FALSE);
                }
            }
        }
    }

    LeaveCriticalSection(&m_omCritSecForMapArr);
    // Now add interpretted data
    //Get the signal info in Formatted strings array
    CStringArray omSigStrArray;
    vFormatSignalInfo (SigInfoArray, omSigStrArray);
    // String to format interpretted data
    __int64 nMsgKey = m_omMgsIndexVec[nMsgIndex];
    COLORREF rgbTreeItem = RGB(0,0,0);      //venkat

    if (nMsgKey != nInvalidKey)
    {
        PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

        if(m_eBusType == CAN)
        {
            static STCANDATA sCANMsg;
            m_pouMsgContainerIntrf->hReadFromOWBuffer(&sCANMsg, nMsgKey);
            rgbTreeItem =m_ouMsgAttr.GetCanIDColour(sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID);
        }
        else if(m_eBusType == J1939)
        {
            static STJ1939_MSG sJ1939Msg;
            m_pouMsgContainerIntrf->hReadFromOWBuffer(&sJ1939Msg, nMsgKey);
            rgbTreeItem =m_ouMsgAttr.GetCanIDColour(
                             sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
        }
    }
    else
    {
        rgbTreeItem = RGB(0,0,0);
    }

    sEntry.m_opTreeWndParam = new CTreeItemParam (&m_lstMsg, omSigStrArray,
            omTreeRect, rgbTreeItem);
    m_lstMsg.SetItemCountEx(nArrSize);
}

/*******************************************************************************
  Function Name  : vGetTreeRect
  Input(s)       : nItem, nCount, omTreeRect
  Output         : -
  Functionality  : Retreives the Tree Rect dimensions of a particular message.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  : Arunkumar K, 18-07-2011,
                   Updated code to suite the functionality of MoveToY(..)
*******************************************************************************/
void CMsgFrmtWnd::vGetTreeRect(int nItem, int nCount, CRect& omTreeRect)
{
    CRect ItemRect;
    m_lstMsg.GetItemRect(nItem, &ItemRect, LVIR_BOUNDS);
    omTreeRect = ItemRect;
    //omTreeRect.MoveToY(omTreeRect.bottom);
    omTreeRect.top = ItemRect.bottom;
    omTreeRect.bottom += ItemRect.Height();
    omTreeRect.bottom += (nCount - 1) * ItemRect.Height()
                         - defMSG_IPET_WND_BORDER_WIDTH;
}

/*******************************************************************************
  Function Name  : OnUpdateMsgTreeItemsPositions
  Input(s)       : wParam, lParam
  Output         : Returns 0 as LRESULT
  Functionality  : CMsgFrmtWnd
                   Handler for message event WM_UPDATE_TREE_ITEMS_POS.
                   Updates the Message Interpretation Tree Items..
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 09-09-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::OnUpdateMsgTreeItemsPositions(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    __int64 n64Temp;
    SMSGDISPMAPENTRY sTemp;
    CRect omTreeRect;
    int nSize = m_omMgsIndexVec.size();

    for(int i = 0; i < nSize; i++)
    {
        n64Temp = m_omMgsIndexVec[i];

        if (n64Temp != nInvalidKey)
        {
            if (m_omMsgDispMap.Lookup(n64Temp, sTemp))
            {
                vGetTreeRect(i, sTemp.m_nSignalCnt, omTreeRect);

                //Update the Tree window
                if (sTemp.m_opTreeWndParam != NULL)
                {
                    sTemp.m_opTreeWndParam->vUpdateTreeItemRect(omTreeRect);
                }
            }
            else
            {
                ASSERT(FALSE);
            }
        }
    }

    return 0;
}


/*******************************************************************************
  Function Name  : nGetMsgListIndexFromMapIndex
  Input(s)       : nKey
  Output         : List ID
  Functionality  : Retreives the List index from Map Index.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
int CMsgFrmtWnd::nGetMsgListIndexFromMapIndex(__int64 nMapIndex)
{
    int nIndex  = -1;
    BOOL bFound = FALSE;
    int nCnt    = m_omMgsIndexVec.size();

    for (int i = 0; (i < nCnt) && (!bFound) ; i++)
    {
        if (nMapIndex == m_omMgsIndexVec[i])
        {
            nIndex = i;
            bFound = TRUE;
        }
    }

    return nIndex;
}

/*******************************************************************************
  Function Name  : nGetCodefromMapKey
  Input(s)       : nKey
  Output         : Message ID
  Functionality  : Retreives the Message ID from Map Index.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  : Arunkumar K, 04-03-2011
                   Updated AND operation value to 0x1FFFFFFF instead of 0x0FFFFFFF.
*******************************************************************************/
int CMsgFrmtWnd::nGetCodefromMapKey(__int64 nKey)
{
    int nID = (int)(nKey & 0x1FFFFFFF);
    return nID;
}

/*******************************************************************************
  Function Name  : vContractMsgEntry
  Input(s)       : SMSGDISPMAPENTRY, MsgIndex
  Output         : -
  Functionality  : Collapses the Message entry at a particular message Index.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vContractMsgEntry(SMSGDISPMAPENTRY& sEntry, int nMsgIndex)
{
    //Remove invalid mapIds inserted for interpnof the the msg. in array
    //move other trees after this up by no. of signals, Delete the tree
    //Setitemcountex = no. of elements in the array
    EnterCriticalSection(&m_CritSec1);

    if ( sEntry.m_opTreeWndParam != NULL )
    {
        int nIndexShift = sEntry.m_nSignalCnt;
        EnterCriticalSection(&m_omCritSecForMapArr);
        // Delete invalid elements after the message in array
        m_omMgsIndexVec.erase((m_omMgsIndexVec.begin() + nMsgIndex + 1),
                              (m_omMgsIndexVec.begin() + nMsgIndex + nIndexShift + 1));
        // Collapse Message
        int nSize = m_omMgsIndexVec.size();
        //Get the TreeCtrl Wnd rect for shfting the already present TreeCtrl Wnd
        // Calculate dimensions of the rectangle
        CRect omTreeRect;
        vGetTreeRect(nMsgIndex, nIndexShift, omTreeRect);

        if(m_nIndex > nMsgIndex)
        {
            m_nIndex -= nIndexShift;
        }

        //If it is not the last msg
        if( ( nMsgIndex + nIndexShift + 1 ) != nSize)
        {
            __int64 n64Temp;
            SMSGDISPMAPENTRY sTemp;

            // Now shift the elements down Shift the map
            for (int i = nMsgIndex + nIndexShift + 1; i < nSize; i++)
            {
                n64Temp = m_omMgsIndexVec[i];

                if (n64Temp != nInvalidKey)
                {
                    if (m_omMsgDispMap.Lookup(n64Temp, sTemp))
                    {
                        //Shift the Tree window
                        if (sTemp.m_opTreeWndParam != NULL)
                        {
                            sTemp.m_opTreeWndParam->vShift ( -(omTreeRect.Height() +
                                                               defMSG_IPET_WND_BORDER_WIDTH));
                        }
                    }
                    else
                    {
                        ASSERT(FALSE);
                    }
                }
            }
        }

        delete sEntry.m_opTreeWndParam;
        sEntry.m_opTreeWndParam = NULL;
        sEntry.m_nSignalCnt = 0;
        m_lstMsg.SetItemCountEx(nSize);
        LeaveCriticalSection(&m_omCritSecForMapArr);
    }

    LeaveCriticalSection(&m_CritSec1);
}

/*******************************************************************************
  Function Name  : vFormatSignalInfo
  Input(s)       : SSignalInfoArray, CStringArray
  Output         : -
  Functionality  : Formats the CStringArray with structure data containing
                   signal data.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vFormatSignalInfo (const SSignalInfoArray& omSigInfoArray,
                                     CStringArray& omSigStrArray)
{
    omSigStrArray.RemoveAll();
    CString omTemp;
    SSignalInfo sTempSignal;
    int nCnt = omSigInfoArray.GetSize();

    for (int i = 0; i < nCnt; i++)
    {
        sTempSignal = omSigInfoArray.GetAt(i);
        omTemp.Format(SIGNAL_FORMAT, sTempSignal.m_omSigName, sTempSignal.m_omRawValue,
                      sTempSignal.m_omEnggValue, sTempSignal.m_omUnit);
        omSigStrArray.Add(omTemp);
    }
}
//
///*******************************************************************************
//  Function Name  : vOpenCloseSignalWatchWnd
//  Input(s)       : wParam and lParam
//  Output         : Returns 0 as LRESULT
//  Functionality  : Toggles the between the state of Creation and showing/
//                 Destruction of Signal Watch Dialog.
//  Member of      : CMsgFrmtWnd
//  Author(s)      : Arunkumar K
//  Date Created   : 12-05-2010
//  Modifications  :
//*******************************************************************************/
//LRESULT CMsgFrmtWnd::vOpenCloseSignalWatchWnd(WPARAM wParam, LPARAM lParam)
//{
//    BOOL bCreateWnd = (BOOL) wParam;
//
//    if (bCreateWnd == TRUE)
//    {
//        if (m_podSignalWatchDlg == NULL)
//        {
//            m_podSignalWatchDlg = new CSigWatchDlg;
//        }
//
//        if (m_podSignalWatchDlg != NULL)
//        {
//            CWnd *pomGrandParent = GetParent()->GetParent();
//            m_podSignalWatchDlg->Create(IDD_DLG_SIGNAL_WATCH, pomGrandParent);
//
//            // Show the window
//            m_podSignalWatchDlg->ShowWindow(SW_SHOW);
//            // Size the list control
//            m_podSignalWatchDlg->SendMessage(WM_SIZE, nZERO, nZERO);
//            // Repaint Non-Client area
//            m_podSignalWatchDlg->SendMessage(WM_NCPAINT, nONE, nZERO);
//
//            m_bSignalWatchON = TRUE;
//        }
//    }
//    else // Window should be closed
//    {
//        if (m_podSignalWatchDlg != NULL)
//        {
//             // Set the Focus to Main Frame
//            //theApp.m_pMainWnd->SetFocus();
//          ::SetFocus(g_hMainGUI);
//            // This is required to update wnd co-ordinates
//            if( m_podSignalWatchDlg->GetSafeHwnd())
//            {
//                m_podSignalWatchDlg->ShowWindow(SW_HIDE);
//                m_podSignalWatchDlg->DestroyWindow();
//            }
//            m_bSignalWatchON = FALSE;
//        }
//    }
//return 0;
//}

/*******************************************************************************
  Function Name  : vChangeStateOfInterpretableMsgs
  Input(s)       : -
  Output         : -
  Functionality  : Changes the state of messages from MODE_NONE to INTERPRETABLE
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vChangeStateOfInterpretableMsgs()
{
    //Keep the key of map
    __int64 n64Temp;
    SMSGDISPMAPENTRY sEntry;
    POSITION pos = m_omMsgDispMap.GetStartPosition();

    while( pos != NULL )
    {
        m_omMsgDispMap.GetNextAssoc( pos, n64Temp, sEntry );

        //If entry is expanded then contract
        if (sEntry.m_eInterpretMode == MODE_NONE)
        {
            if (NULL != m_ppMsgDB)
            {
                int nID = nGetCodefromMapKey(n64Temp);
                sMESSAGE* psMsgStruct = (*m_ppMsgDB)->psGetMessagePointer(nID);

                if (psMsgStruct != NULL)
                {
                    if (psMsgStruct->m_unNumberOfSignals > 0)
                    {
                        //Change the state and update the map
                        sEntry.m_eInterpretMode = INTERPRETABLE;
                        //Save the change in the map
                        m_omMsgDispMap[ n64Temp ] = sEntry;
                    }
                }
            }
        }
    }

    m_lstMsg.Invalidate();
}

/*******************************************************************************
  Function Name  : vCreateAllMsgTree
  Input(s)       : -
  Output         : -
  Functionality  : Creates the Tree items of messages which are in INTERPRETING state.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
//Only for those messages which were in INTERPRETING state
void CMsgFrmtWnd::vCreateAllMsgTree()
{
    // Variable to keep entry details
    SMSGDISPMAPENTRY sEntry;
    // Variable to hold message ID
    __int64 n64Temp = 0;
    POSITION pos = m_omMsgDispMap.GetStartPosition();

    while( pos != NULL )
    {
        m_omMsgDispMap.GetNextAssoc( pos, n64Temp, sEntry );

        if (sEntry.m_eInterpretMode == INTERPRETING)
        {
            // Get the message details
            SSignalInfoArray SigInfoArray;
            //Get the signal details
            vGetSignalInfoArray(n64Temp, SigInfoArray);
            //Expand and update the map
            int nMsgListIndex = nGetMsgListIndexFromMapIndex(n64Temp);

            if (-1 != nMsgListIndex)
            {
                vExpandMsgEntry (sEntry, SigInfoArray,nMsgListIndex);
                //Update the map
                m_omMsgDispMap[n64Temp] = sEntry;
            }
        }
    }

    //m_omMsgDispMap.RemoveAll();
}

/*******************************************************************************
  Function Name  : vExpandContractMsg
  Input(s)       : Index of List Control position.
  Output         : -
  Functionality  : Expands/ Collapses Message Interpretation based on index of
                   list control.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vExpandContractMsg(int nIndex,eTreeItemStates eTreeItemState)
{
    SMSGDISPMAPENTRY sTemp;
    EnterCriticalSection(&m_omCritSecForMapArr);
    __int64 nMapIndex = m_omMgsIndexVec[nIndex];

    if (nMapIndex != nInvalidKey)
    {
        if (m_omMsgDispMap.Lookup(nMapIndex, sTemp))
        {
            if(eTreeItemState == TREESTATE_TOGGLE)
            {
                if (sTemp.m_eInterpretMode == INTERPRETING)
                {
                    sTemp.m_eInterpretMode = INTERPRETABLE;
                    vContractMsgEntry(sTemp, nIndex);
                }
                else if(sTemp.m_eInterpretMode == INTERPRETABLE)
                {
                    // get signal info set from the Data base and get it interpreted
                    SSignalInfoArray SigInfoArray;
                    vGetSignalInfoArray(nMapIndex, SigInfoArray);
                    sTemp.m_eInterpretMode  = INTERPRETING;
                    vExpandMsgEntry(sTemp, SigInfoArray, nIndex);
                }
            }
            else if(eTreeItemState == TREESTATE_EXPAND)
            {
                if(sTemp.m_eInterpretMode == INTERPRETABLE)
                {
                    // get signal info set from the Data base and get it interpreted
                    SSignalInfoArray SigInfoArray;
                    vGetSignalInfoArray(nMapIndex, SigInfoArray);
                    sTemp.m_eInterpretMode  = INTERPRETING;
                    vExpandMsgEntry(sTemp, SigInfoArray, nIndex);
                }
            }
            else if(eTreeItemState == TREESTATE_COLLAPSE)
            {
                if (sTemp.m_eInterpretMode == INTERPRETING)
                {
                    sTemp.m_eInterpretMode = INTERPRETABLE;
                    vContractMsgEntry(sTemp, nIndex);
                }
            }

            //Update entry with current interpretation status
            m_omMsgDispMap[nMapIndex] = sTemp;
            // Maintain the selection
            m_lstMsg.SetItemState( nIndex, LVIS_SELECTED, LVIS_SELECTED);
            // Update the List control
            m_lstMsg.Invalidate();
        }
    }

    LeaveCriticalSection(&m_omCritSecForMapArr);
    vArrangeAllTreeWnd();
}

/*******************************************************************************
  Function Name  : vArrangeAllTreeWnd
  Input(s)       : -
  Output         : -
  Functionality  : Rearranges all the Tree window items of Messages based on
                   new positions.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 12-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vArrangeAllTreeWnd()
{
    // Variable to keep entry details
    SMSGDISPMAPENTRY sTemp;
    // Variable to hold message ID
    __int64 n64Temp = 0;
    EnterCriticalSection(&m_omCritSecForMapArr);
    int nSize = m_omMgsIndexVec.size();

    for (int i = 0; i < nSize; i++)
    {
        n64Temp = m_omMgsIndexVec[i];

        if (m_omMsgDispMap.Lookup(n64Temp, sTemp))
        {
            if (sTemp.m_opTreeWndParam != NULL)
            {
                CRect rItem( -1, -1, -1, -1 );
                m_lstMsg.GetItemRect(i, &rItem, LVIR_BOUNDS);
                sTemp.m_opTreeWndParam->vShiftWindowTop(rItem.bottom);
            }
        }
    }

    LeaveCriticalSection(&m_omCritSecForMapArr);
}

/*******************************************************************************
  Function Name  : vUpdateAllTreeWnd
  Input(s)       : -
  Output         : -
  Functionality  : Updates the signal data for all the messages.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 07-09-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vUpdateAllTreeWnd()
{
    // Variable to keep entry details
    SMSGDISPMAPENTRY sTemp;
    // Variable to hold message ID
    __int64 n64Temp = 0;
    int nSize = m_omMgsIndexVec.size();

    for (int i = 0; i < nSize; i++)
    {
        n64Temp = m_omMgsIndexVec[i];

        if (m_omMsgDispMap.Lookup(n64Temp, sTemp))
        {
            if (sTemp.m_opTreeWndParam != NULL)
            {
                COLORREF rgbTreeItem = RGB(0,0,0);//venkat

                if (n64Temp != nInvalidKey)
                {
                    PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

                    if(m_eBusType == CAN)
                    {
                        static STCANDATA sCANMsg;
                        m_pouMsgContainerIntrf->hReadFromOWBuffer(&sCANMsg, n64Temp);
                        rgbTreeItem =m_ouMsgAttr.GetCanIDColour(sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID);
                    }
                    else if(m_eBusType == J1939)
                    {
                        static STJ1939_MSG sJ1939Msg;
                        m_pouMsgContainerIntrf->hReadFromOWBuffer(&sJ1939Msg, n64Temp);
                        rgbTreeItem =m_ouMsgAttr.GetCanIDColour(
                                         sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
                    }
                }
                else
                {
                    rgbTreeItem = RGB(0,0,0);
                }

                SSignalInfoArray SigInfoArray;
                vGetSignalInfoArray(n64Temp, SigInfoArray);
                //Get the signal info in Formatted strings array
                CStringArray omSigStrArray;
                vFormatSignalInfo (SigInfoArray, omSigStrArray);
                // String to format interpretted data
                sTemp.m_opTreeWndParam->vUpdateTreeValues(omSigStrArray, rgbTreeItem);
            }
        }
    }
}

/*******************************************************************************
  Function Name  : vUpdateMsgTreeWnd
  Input(s)       : -
  Output         : -
  Functionality  : Updates the signal data for a message.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 07-09-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vUpdateMsgTreeWnd(__int64 nMapIndex)
{
    // Variable to keep entry details
    SMSGDISPMAPENTRY sTemp;

    if (m_omMsgDispMap.Lookup(nMapIndex, sTemp))
    {
        if (sTemp.m_opTreeWndParam != NULL /*&& sTemp.m_opTreeWndParam->bVerifyTreeCtrl()*/)
        {
            COLORREF rgbTreeItem = RGB(0,0,0);  //venkat

            if (nMapIndex != nInvalidKey)
            {
                PSDI_GetInterface(m_eBusType, (void**)&m_pouMsgContainerIntrf);

                if(m_eBusType == CAN)
                {
                    static STCANDATA sCANMsg;
                    m_pouMsgContainerIntrf->hReadFromOWBuffer(&sCANMsg, nMapIndex);
                    rgbTreeItem =m_ouMsgAttr.GetCanIDColour(sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID);
                }
                else if(m_eBusType == J1939)
                {
                    static STJ1939_MSG sJ1939Msg;
                    m_pouMsgContainerIntrf->hReadFromOWBuffer(&sJ1939Msg, nMapIndex);
                    rgbTreeItem =m_ouMsgAttr.GetCanIDColour(
                                     sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
                }
            }
            else                            //may not be required
            {
                rgbTreeItem = RGB(0,0,0);
            }

            SSignalInfoArray SigInfoArray;
            vGetSignalInfoArray(nMapIndex, SigInfoArray);
            //Get the signal info in Formatted strings array
            CStringArray omSigStrArray;
            vFormatSignalInfo (SigInfoArray, omSigStrArray);
            // String to format interpretted data
            sTemp.m_opTreeWndParam->vUpdateTreeValues(omSigStrArray, rgbTreeItem);
        }
    }
}

/*******************************************************************************
  Function Name  : OnClose
  Input(s)       : -
  Output         : -
  Functionality  : Called when Close is clicked on Message Window.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 13-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::OnClose()
{
    return;
}

/*******************************************************************************
  Function Name  : OnShowHideMessageWindow
  Input(s)       : wParam, lParam
  Output         : LRESULT
  Functionality  : Shows/hides Message Window based on the menu click.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 17-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::OnShowHideMessageWindow(WPARAM wParam, LPARAM lParam)
{
    BOOL bShowUpdate = (BOOL)wParam;

    if(bShowUpdate) //If show/hide
    {
        if(IsWindowVisible())
        {
            ShowWindow(SW_HIDE);
            m_pouMsgContainerIntrf->hToggleDILBufferRead(FALSE);
        }
        else
        {
            ShowWindow(SW_SHOW);
            m_pouMsgContainerIntrf->hToggleDILBufferRead(TRUE);
        }
    }
    else //Update UI
    {
        CCmdUI* pCmdUI = (CCmdUI*)lParam;

        if(IsWindowVisible())
        {
            pCmdUI->SetCheck(1);
        }
        else
        {
            pCmdUI->SetCheck(0);
        }
    }

    return 0;
}

/*******************************************************************************
  Function Name  : OnUpdateShowHideMessageWindow
  Input(s)       : pCmdUI
  Output         : -
  Functionality  : Shows the status of Message Window visibility in Menu Item.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 17-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::OnUpdateShowHideMessageWindow(CCmdUI* pCmdUI)
{
    switch(pCmdUI->m_nID)
    {
        case ID_SHOWMESSAGEWINDOW_CAN:
        {
            if(m_eBusType == CAN)
            {
                if(IsWindowVisible())
                {
                    pCmdUI->SetCheck(1);
                }
                else
                {
                    pCmdUI->SetCheck(0);
                }
            }
        }
        break;

        case ID_SHOWMESSAGEWINDOW_J1939:
        {
            if(m_eBusType == J1939)
            {
                if(IsWindowVisible())
                {
                    pCmdUI->SetCheck(1);
                }
                else
                {
                    pCmdUI->SetCheck(0);
                }
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
  Function Name  : OnMsgwndResetColumns
  Input(s)       : -
  Output         : -
  Functionality  : Reorders the Message window
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::OnMsgwndResetColumns()
{
    CHeaderCtrl* pHeaderCtrl = m_lstMsg.GetHeaderCtrl();

    if (pHeaderCtrl != NULL)
    {
        int  nColumnCount = pHeaderCtrl->GetItemCount();
        LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));
        ASSERT(pnOrder != NULL);
        m_lstMsg.GetColumnOrderArray(pnOrder, nColumnCount);
        int i;

        for (i = 0 ; i < nColumnCount; i++)
        {
            pnOrder[i] = i;
        }

        m_lstMsg.SetColumnOrderArray(nColumnCount, pnOrder);
        free(pnOrder);
        m_lstMsg.Invalidate();
    }
}

/*******************************************************************************
  Function Name  : OnMDIActivate
  Input(s)       : bActivate, pActivateWnd, pDeactivateWnd
  Output         : -
  Functionality  : Set the Focus to List Control when ever MsgFrmtWnd gets activated.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 15-07-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
    CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

    if(bActivate)
    {
        m_lstMsg.SetFocus();
    }
}

/*******************************************************************************
  Function Name  : GetConfigData
  Input(s)       : ppvDataStream, unLength
  Output         : -
  Functionality  : Returns the Message Format Window Configuration in Byte Array.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 25-08-2010
  Modifications  :
*******************************************************************************/
HRESULT CMsgFrmtWnd::GetConfigData(BYTE* pvDataStream)
{
    BYTE* pByteTrgt = NULL;
    pByteTrgt = pvDataStream;
    BYTE byVer = MSG_FRMT_WND_VERSION;
    COPY_DATA(pByteTrgt, &byVer , sizeof(BYTE)); //Setting Version.
    UINT nSize= nGetMsgFrmtWndConfigSize();
    COPY_DATA(pByteTrgt, &nSize, sizeof(UINT)); //Setting Buffer Size.
    //Storing column Header Positions.
    CHeaderCtrl* pHeaderCtrl = m_lstMsg.GetHeaderCtrl();

    if (pHeaderCtrl != NULL)
    {
        int  nColumnCount = pHeaderCtrl->GetItemCount();
        COPY_DATA(pByteTrgt, &nColumnCount, sizeof(UINT)); //Storing column count.
        LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));
        ASSERT(pnOrder != NULL);
        m_lstMsg.GetColumnOrderArray(pnOrder, nColumnCount);

        for (int i = 0 ; i < nColumnCount; i++)
        {
            COPY_DATA(pByteTrgt, &pnOrder[i], sizeof(int));
            //Storing Column Header show/Hide.
            bool bColumnVisible = m_lstMsg.IsColumnVisible(i);
            COPY_DATA(pByteTrgt, &bColumnVisible, sizeof(bool));
            INT nColWidth = m_lstMsg.GetColumnWidth(i);
            COPY_DATA(pByteTrgt, &nColWidth, sizeof(INT));
        }

        free(pnOrder);
    }

    //Storing Hex/Dec Display.
    //Venkat
    bool bHexDec = IS_NUM_HEX_SET(m_bExprnFlag_Disp) > 0 ? true: false;
    COPY_DATA(pByteTrgt, &bHexDec, sizeof(bool));
    //Storing Overwrite/Append Mode.
    bool bOvrwAppend = IS_MODE_APPEND(m_bExprnFlag_Disp) > 0 ? true: false;
    COPY_DATA(pByteTrgt, &bOvrwAppend, sizeof(bool));
    //Storing Interpret Status if in overwrite Mode.
    bool bInterpret = IS_MODE_INTRP(m_bExprnFlag_Disp) > 0 ? true: false;
    COPY_DATA(pByteTrgt, &bInterpret, sizeof(bool));
    //Storing Time Display.
    BYTE byTimeDisplay;

    if(IS_TM_ABS_SET(m_bExprnFlag_Disp))
    {
        byTimeDisplay = BIT_TM_ABS;
    }
    else if(IS_TM_REL_SET(m_bExprnFlag_Disp))
    {
        byTimeDisplay = BIT_TM_REL;
    }
    else if(IS_TM_SYS_SET(m_bExprnFlag_Disp))
    {
        byTimeDisplay = BIT_TM_SYS;
    }

    COPY_DATA(pByteTrgt, &byTimeDisplay, sizeof(BYTE));
    WINDOWPLACEMENT sMsgWndPlacement;
    GetWindowPlacement(&sMsgWndPlacement);
    COPY_DATA(pByteTrgt, &sMsgWndPlacement, sizeof(WINDOWPLACEMENT));
    m_podMsgIntprtnDlg->GetWindowPlacement(&m_sMsgIntrpWndPlacement);
    COPY_DATA(pByteTrgt, &m_sMsgIntrpWndPlacement, sizeof(WINDOWPLACEMENT));
    return S_OK;
}

/*******************************************************************************
  Function Name  : SetConfigData
  Input(s)       : pvDataStream
  Output         : -
  Functionality  : Sets the Message Format Window Configuration from Byte Array.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 25-08-2010
  Modifications  : ArunKumar K, 27-04-2011,
                   Added the call CLEAR_EXPR_TM_BITS(m_bExprnFlag_Disp)
                   before setting the time mode.
*******************************************************************************/
HRESULT CMsgFrmtWnd::SetConfigData(BYTE* pvDataStream)
{
    BYTE* pByteSrc = NULL;
    pByteSrc = pvDataStream;

    if (pByteSrc != NULL)
    {
        //Reading Version.
        BYTE byVer = 0;
        COPY_DATA_2(&byVer, pByteSrc, sizeof(BYTE));
        UINT nSize= 0;
        //Reading Buffer Size.
        COPY_DATA_2(&nSize, pByteSrc, sizeof(UINT));

        if ((byVer == MSG_FRMT_WND_VERSION) && (nSize > 0))
        {
            //Reading column Header Positions.
            CHeaderCtrl* pHeaderCtrl = m_lstMsg.GetHeaderCtrl();

            if (pHeaderCtrl != NULL)
            {
                int  nColumnCount=0;
                //Reading column count.
                COPY_DATA_2(&nColumnCount, pByteSrc, sizeof(UINT));
                LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));

                for (int i = 0 ; i < nColumnCount; i++)
                {
                    COPY_DATA_2(&pnOrder[i], pByteSrc, sizeof(int));
                    bool bColumnVisible = false;
                    COPY_DATA_2(&bColumnVisible, pByteSrc, sizeof(bool));
                    m_lstMsg.ShowColumn(i, bColumnVisible);
                    INT nColWidth = 0;
                    COPY_DATA_2(&nColWidth, pByteSrc, sizeof(INT));
                    m_lstMsg.SetColumnWidth(i, nColWidth);
                }

                m_lstMsg.SetColumnOrderArray(nColumnCount, pnOrder);
                free(pnOrder);
            }

            //Reading Hex/Dec Display.
            bool bHexDec = false;
            COPY_DATA_2(&bHexDec, pByteSrc, sizeof(bool));

            if(bHexDec)
            {
                SET_NUM_HEX(m_bExprnFlag_Disp);
            }
            else
            {
                SET_NUM_DEC(m_bExprnFlag_Disp);
            }

            //Reading Overwrite/Append Mode.
            bool bOvrwAppend = false;
            COPY_DATA_2(&bOvrwAppend, pByteSrc, sizeof(bool));

            if(bOvrwAppend)
            {
                SET_MODE_APPEND(m_bExprnFlag_Disp);
            }
            else
            {
                SET_MODE_OVER(m_bExprnFlag_Disp);
            }

            //Reading Interpret Status if in overwrite Mode.
            COPY_DATA_2(&m_bInterPretMsg, pByteSrc, sizeof(bool));

            if(m_bInterPretMsg)
            {
                SET_MODE_INTRP(m_bExprnFlag_Disp);
            }

            //Reading Time Display.
            BYTE byTimeDisplay;
            COPY_DATA_2(&byTimeDisplay, pByteSrc, sizeof(bool));
            CLEAR_EXPR_TM_BITS(m_bExprnFlag_Disp);

            if(byTimeDisplay == BIT_TM_ABS)
            {
                SET_TM_ABS(m_bExprnFlag_Disp);
            }
            else if(byTimeDisplay == BIT_TM_REL)
            {
                SET_TM_REL(m_bExprnFlag_Disp);
            }
            else if(byTimeDisplay == BIT_TM_SYS)
            {
                SET_TM_SYS(m_bExprnFlag_Disp);
            }

            WINDOWPLACEMENT sMsgWndPlacement;
            COPY_DATA_2(&sMsgWndPlacement, pByteSrc, sizeof(WINDOWPLACEMENT));
            //Save the window visibility status.
            BOOL bVisible = IsWindowVisible();
            //SetWindowPlacement function call makes the window visible.
            SetWindowPlacement(&sMsgWndPlacement);

            //Regain the window's visibility status.
            if(!bVisible)
            {
                ShowWindow(SW_HIDE);
            }

            COPY_DATA_2(&m_sMsgIntrpWndPlacement, pByteSrc, sizeof(WINDOWPLACEMENT));
        }
        else
        {
            //load default values
            vSetDefaultConfigValues();
        }
    }
    else
    {
        //load default values
        vSetDefaultConfigValues();
    }

    vUpdatePtrInLstCtrl();
    m_lstMsg.vShowHideBlankcolumn(m_bInterPretMsg);
    //Clear all the previous messages.
    OnEditClearAll();
    return S_OK;
}

/*******************************************************************************
  Function Name  : vSetDefaultConfigValues
  Input(s)       : pvDataStream
  Output         : -
  Functionality  : Gets the Message Format Window Configuration size as UINT.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 25-08-2010
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::vSetDefaultConfigValues()
{
    //load default values
    vCreateCols();
    CLEAR_EXPR_FLAG(m_bExprnFlag_Disp);
    SET_TM_SYS(m_bExprnFlag_Disp);
    SET_NUM_DEC(m_bExprnFlag_Disp);
    SET_MODE_OVER(m_bExprnFlag_Disp);
    m_bInterPretMsg = FALSE;
    //Set default Msg Interpretation window placement.
    CRect omRect;
    ::GetWindowRect(g_hMainGUI, &omRect);
    int nBorderWidth = defBORDER_WIDTH_FACTOR *
                       GetSystemMetrics( SM_CXBORDER);
    // Make the top starts from 35% of main frame height
    omRect.top += static_cast<LONG> ( omRect.Height() *
                                      defMSG_IPET_WND_HEIGHT_RATIO);
    // Make the width 65% of main frame
    omRect.left += (LONG)( omRect.Width() * defMSG_IPET_WND_WIDTH_RATIO);
    // Shift the top down to avoid overlap with toolbar
    //omRect.top += omToolBarRect.Height();
    // Reduce the border size
    omRect.right -= nBorderWidth;
    m_sMsgIntrpWndPlacement.ptMaxPosition.x = -1;
    m_sMsgIntrpWndPlacement.ptMaxPosition.y = -1;
    m_sMsgIntrpWndPlacement.ptMinPosition.x = -1;
    m_sMsgIntrpWndPlacement.ptMinPosition.y = -1;
    m_sMsgIntrpWndPlacement.flags = 0;
    m_sMsgIntrpWndPlacement.length = 44;
    m_sMsgIntrpWndPlacement.rcNormalPosition = omRect;
    m_sMsgIntrpWndPlacement.showCmd = SW_SHOW;
}

/*******************************************************************************
  Function Name  : SetConfigData
  Input(s)       : pvDataStream
  Output         : -
  Functionality  : Gets the Message Format Window Configuration size as UINT.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 25-08-2010
  Modifications  :
*******************************************************************************/
UINT CMsgFrmtWnd::nGetMsgFrmtWndConfigSize()
{
    /*1. Column Header Positions.   : SHORT[10]
    2. Column Header Show/Hide. : BOOL[10]
    3. Hex/Dec Display. : BOOL
    4. Overwrite/Append Mode. : BOOL
    5. Interpret Status if in overwrite Mode. : BOOL
    6. Time Display. : BYTE
    7. windowPlacement*/
    UINT nSize = 0;
    nSize += sizeof(BYTE);  // 1 Byte for Version
    nSize += sizeof(UINT);  // Bytes for Buffer Size
    //Bytes for column Header Positions.
    nSize += sizeof(UINT); //to store the column count
    CHeaderCtrl* pHeaderCtrl = m_lstMsg.GetHeaderCtrl();

    if (pHeaderCtrl != NULL)
    {
        UINT unColCount = pHeaderCtrl->GetItemCount();
        nSize += (sizeof(int) * unColCount); //Column order
        nSize += (sizeof(bool) * unColCount);//Column visibility
        nSize += (sizeof(int) * unColCount); //Column width
    }

    //Bytes for Hex/Dec Display.
    nSize += sizeof(bool);
    //Bytes for Overwrite/Append Mode.
    nSize += sizeof(bool);
    //Bytes for Interpret Status if in overwrite Mode.
    nSize += sizeof(bool);
    //Byte for Time Display.
    nSize += sizeof(BYTE);
    nSize += sizeof(WINDOWPLACEMENT);//sizeof Msg window placement
    nSize += sizeof(WINDOWPLACEMENT);//sizeof interpretation window placement
    return nSize;
}

void CMsgFrmtWnd::vSetMsgDataBasePtr(void** ppvMsgDBPtr)
{
    m_ppMsgDB = (CMsgSignal**)ppvMsgDBPtr;
}

void CMsgFrmtWnd::vSetClientID(DWORD dwClientID)
{
    m_dwClientID = dwClientID;
}

void CMsgFrmtWnd::vSetDILInterfacePointer(void** ppvJ1939DIL)
{
    m_ppouIJ1939DIL = (CBaseDILI_J1939**)ppvJ1939DIL;
}
BOOL CMsgFrmtWnd::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if ( pMsg->message >= WM_MOUSEFIRST &&
            pMsg->message <= WM_MOUSELAST )
    {
        MSG msg;
        ::CopyMemory(&msg, pMsg, sizeof(MSG));
        HWND hWndPrnt = ::GetParent(msg.hwnd);

        while(hWndPrnt && m_hWnd != hWndPrnt)
        {
            msg.hwnd = hWndPrnt;
            hWndPrnt = ::GetParent(hWndPrnt);
        }

        if(msg.hwnd)
        {
            vDisplayToolTip(&msg);
            m_objToolTip.RelayEvent(&msg);
        }
    }

    return CMDIChildWnd::PreTranslateMessage(pMsg);
}

void CMsgFrmtWnd::vDisplayToolTip(MSG* pMsg)
{
    CString strClassName;
    const int nMaxCount = 20;
    LPTSTR lpszClassName = strClassName.GetBuffer(nMaxCount);
    GetClassName(pMsg->hwnd,lpszClassName,nMaxCount);
    strClassName.ReleaseBuffer();

    if (strClassName==_T("SysListView32"))
    {
        CRxMsgList* ptrListCtrl = static_cast<CRxMsgList*> (CRxMsgList::FromHandle(pMsg->hwnd));
        POINT pt(pMsg->pt);
        ptrListCtrl->ScreenToClient(&pt); // ItemFromPoint needs coords converted with 0,0 as upper left most of listbox
        UINT nFlag;
        int nIndex = ptrListCtrl->HitTest(pt,&nFlag);
        // Find the subitem
        LVHITTESTINFO sbItmHitInfo = {0};
        sbItmHitInfo.flags = nFlag;
        sbItmHitInfo.pt = pt;
        ptrListCtrl->SubItemHitTest(&sbItmHitInfo);

        if(sbItmHitInfo.iItem == -1 ||
                m_sHdrColStruct.m_byDataPos+1 != sbItmHitInfo.iSubItem )
        {
            m_objToolTip.Activate(FALSE);
            m_nPrevToolRow = -1;
            m_nPrevToolCol = -1;
        }
        else if( nIndex >=0 &&
                 sbItmHitInfo.iItem >= 0 &&
                 m_sHdrColStruct.m_byDataPos+1 == sbItmHitInfo.iSubItem &&
                 (sbItmHitInfo.iItem !=m_nPrevToolRow ||
                  sbItmHitInfo.iSubItem!=m_nPrevToolCol ))
        {
            m_objToolTip.Activate(TRUE);
            static SWMUPDATEPTRPARA sParam;
            sParam.m_nListIndex  = nIndex;
            sParam.m_eInPretMode = MODE_NONE;
            sParam.m_bResult     = FALSE;
            ::SendMessage( m_hWnd, WM_PARENT_UPDATE_MSGPTR, (WPARAM)&sParam, 0);
            CString strExpandText;
            strExpandText = m_pomDataPtrArr[m_sHdrColStruct.m_byDataPos];
            m_objToolTip.UpdateTipText(strExpandText, CWnd::FromHandle(pMsg->hwnd));
            // Update with new row,col values.
            m_nPrevToolRow = sbItmHitInfo.iItem;
            m_nPrevToolCol = sbItmHitInfo.iSubItem;
            m_objToolTip.Invalidate();
        }
    }
}

/*******************************************************************************
  Function Name  : OnToggleInterpretStatusAllEntries
  Input(s)       : bIsDBAssociate
  Output         : LRESULT
  Functionality  : Toggles the interpret state of all message entries based on
                   bIsDBAssociate.
  Member of      : CMsgFrmtWnd
  Author(s)      : Arunkumar K
  Date Created   : 05-04-2011
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::OnToggleInterpretStatusAllEntries (WPARAM wParam, LPARAM /*lParam*/)
{
    BOOL bIsDBAssociate = (BOOL)wParam;
    int nMsgCode;
    CString msgName;

    if(bIsDBAssociate)//If DB is associated
    {
        //Keep the key of map
        __int64 n64Temp;
        SMSGDISPMAPENTRY sEntry;
        // Now shift the elements down Shift the map
        POSITION pos = m_omMsgDispMap.GetStartPosition();

        while( pos != NULL )
        {
            m_omMsgDispMap.GetNextAssoc( pos, n64Temp, sEntry );
            //If entry is expandable and not already expanded then expand
            nMsgCode = nGetCodefromMapKey(n64Temp);

            if (m_ppMsgDB != NULL)
            {
                if ((*m_ppMsgDB)->bMessageNameFromMsgCode(nMsgCode, msgName))
                {
                    sEntry.m_eInterpretMode = INTERPRETABLE;
                }
                else
                {
                    sEntry.m_eInterpretMode = NON_INTERPRETABLE;
                }

                m_omMsgDispMap[n64Temp] = sEntry;
            }
            else
            {
                sEntry.m_eInterpretMode = NON_INTERPRETABLE;
                m_omMsgDispMap[n64Temp] = sEntry;
            }
        }
    }
    else //If DB is dissociated
    {
        //Keep the key of map
        __int64 n64Temp;
        SMSGDISPMAPENTRY sEntry;
        // Now shift the elements down Shift the map
        POSITION pos = m_omMsgDispMap.GetStartPosition();

        while( pos != NULL )
        {
            m_omMsgDispMap.GetNextAssoc( pos, n64Temp, sEntry );
            nMsgCode = nGetCodefromMapKey(n64Temp);

            if (m_ppMsgDB != NULL)
            {
                if ((*m_ppMsgDB)->bMessageNameFromMsgCode(nMsgCode, msgName))
                {
                    continue;
                }
            }

            //If entry is expanded then contract
            if (sEntry.m_eInterpretMode == INTERPRETING)
            {
                //Change the state and update the map
                sEntry.m_eInterpretMode = NON_INTERPRETABLE;
                int nMsgListIndex = nGetMsgListIndexFromMapIndex(n64Temp);

                if (-1 != nMsgListIndex)
                {
                    //Contraction and updation of map
                    vContractMsgEntry (sEntry, nMsgListIndex);
                    m_omMsgDispMap[n64Temp] = sEntry;
                }
            }
            else
            {
                sEntry.m_eInterpretMode = NON_INTERPRETABLE;
                m_omMsgDispMap[n64Temp] = sEntry;
            }
        }
    }

    m_lstMsg.Invalidate();
    return 0;
}

