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
#include "../DataTypes/Filter_Datatypes.h"
#include "include/utils_macro.h"
#include "common.h"
#include "MsgFrmtWnd.h"
#include "Error.h"
#include "utility\MultiLanguageSupport.h"
#include "Resource.h"

// CMsgFrmtWnd

const int SIZE_APP_CAN_BUFFER       = 5000;
const int SIZE_APP_LIN_BUFFER       = 5000;
/* Error message related definitions and declarations : BEGIN */
static CString sg_omColmStr;



const BYTE MSG_FRMT_WND_VERSION = 0x1;

#define def_XPATH_COLUMN        "//BUSMASTER_CONFIGURATION/Module_Configuration/%s_Message_Window/COLUMN"
#define def_XPATH_ISHEX         "//BUSMASTER_CONFIGURATION/Module_Configuration/%s_Message_Window/IsHex"
#define def_XPATH_ISAPPENDED    "//BUSMASTER_CONFIGURATION/Module_Configuration/%s_Message_Window/IsAppend"
#define def_XPATH_ISINTERPRET   "//BUSMASTER_CONFIGURATION/Module_Configuration/%s_Message_Window/IsInterpret"
#define def_XPATH_TIMEMODE      "//BUSMASTER_CONFIGURATION/Module_Configuration/%s_Message_Window/Time_Mode"
#define def_XPATH_WNDPOS        "//BUSMASTER_CONFIGURATION/Module_Configuration/%s_Message_Window/Window_Position"
#define def_XPATH_INT_WND_POS   "//BUSMASTER_CONFIGURATION/Module_Configuration/%s_Message_Window/Interpretation_Window_Position"
#define defSTR_ERR_INVALID_FILE "Invalid File. Please load valid(.log) File."

/* Error message related definitions and declarations : END */


CMenu CMsgFrmtWnd::menu;

// DIL CAN interface

#define defDEF_DISPLAY_UPDATE_TIME          100
#define TIMER_ID_DISPLAY_UPDATE             102

#define MAX_LINE_LENGTH_TOOL_TIP            624



const __int64 nInvalidKey        = 0;

#define SIGNAL_FORMAT  "%-30s %-16s %-30s %-16s"

#define defMSG_IPET_WND_BORDER_WIDTH        1
#define defMSG_IPET_WND_HEIGHT_RATIO        0.35
#define defMSG_IPET_WND_WIDTH_RATIO         0.65
#define defMSG_IPET_WND_HEIGHT_RATIO        0.35
#define defSTR_COLLAPSE_MENU_TEXT           "&Collapse"
#define defIMPORT_LOG_FILTER "Log File (*.log)|*.log||"

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
    m_opTreeWndParam = nullptr;
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

CMsgFrmtWnd::CMsgFrmtWnd(ETYPE_BUS eBusType, CMsgContainerBase* msgContainer, HWND parentWnd) : m_ouMsgAttr(CMessageAttrib::ouGetHandle(eBusType))
{
    mBusConnectedState = false;
    m_bUnloadOnClear=false;
    m_omStrWindowTitle="";
    //changes done for CCP checker AUC

    m_bMsgIntrprtnDlgShown = 0;
    m_dwClientID = 0;
    m_nColCount = 0;
    m_bIsLogFileImported = false;
    InitializeCriticalSection(&m_ouCriticalSection);
    m_hMainWnd = parentWnd;
    //------------------------------------

    m_pouMsgContainerIntrf = nullptr;
    m_bInterPretMsg = FALSE;

    m_bSignalWatchON = FALSE;
    m_unDispUpdateTimerId = 0;
    m_bUpdate = FALSE;
    m_nField =-1;
    m_bAscending =true;

    m_pBmNetWork = nullptr;

    CLEAR_EXPR_FLAG(m_bExprnFlag_Disp);
    SET_TM_SYS(m_bExprnFlag_Disp);
    SET_NUM_HEX(m_bExprnFlag_Disp);
    SET_MODE_APPEND(m_bExprnFlag_Disp);
    InitializeCriticalSection(&m_CritSec1);
    InitializeCriticalSection(&m_omCritSecForMapArr);
    InitializeCriticalSection(&m_omCritSecClearAll);
    m_omMgsIndexVec.reserve(nOverWriteMaxMsg);

    m_eBusType = eBusType;

    m_pouMsgContainerIntrf = msgContainer;
    if ( nullptr != m_pouMsgContainerIntrf )
    {
        m_pouMsgContainerIntrf->vSetRxMsgCallBkPtr(this);
    }

    m_bConnected = FALSE;
    m_pExpandedMapIndexes = nullptr;

    for(int i=0; i<MAX_MSG_WND_COL_CNT; i++)
    {
        m_pomDataPtrArr[i] = nullptr;
    }
    m_nIndex = 0;
    m_unCurrInterpretedMsgID = static_cast < UINT > (-1);
    m_podMsgIntprtnDlg = nullptr;


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
    DeleteCriticalSection(&m_ouCriticalSection);
    if(m_podMsgIntprtnDlg)
    {
        delete m_podMsgIntprtnDlg;
        m_podMsgIntprtnDlg = nullptr;
    }
    m_bIsLogFileImported = false;
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
    ON_MESSAGE(WM_UPDATE_DISPLAY_FRAME_PDU, OnDisplaySettingUpdated)
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
    ON_COMMAND(IDM_MESSAGE_IMPORTLOGFILE, OnImportLogFile)
    ON_MESSAGE(WM_DATABASE_CHANGE, OnToggleInterpretStatusAllEntries)

    ON_EN_UPDATE(ID_BTN_EDIT_PAGENO, OnImportLogPageNoChange)


    ON_MESSAGE(WM_IMPORTLOG_OVERWRITE_SCROLL,OnImportLogOverwriteScroll)
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
    /*if (menu.m_hMenu == nullptr)
    {
        menu.LoadMenu(IDR_MAINFRAME);
    }
    m_hMenuShared = menu.m_hMenu;*/
    m_hMenuShared = nullptr;
    // Register a custom WndClass and create a window.
    // This must be done because CHelloWnd has a custom icon.
    LPCTSTR lpszReceiveWndClass =
        AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
                            LoadCursor(nullptr, IDC_ARROW),
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

    vSetDefaultWindowPosition(ClientRect);

    if(bResult)
    {
        m_lstMsg.vDoInitialization();

        m_lstMsg.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );

        //m_wndHeader.SubclassWindow(m_lstMsg.GetHeaderCtrl()->m_hWnd);
        VERIFY(m_lstMsg.m_wndHeader.SubclassWindow(m_lstMsg.GetHeaderCtrl()->GetSafeHwnd()));

        SendMessage(WM_UPDATE_FONT, 0, 0);
        vSetDefaultHeaders();

        m_MsgHdrInfo.vGetHdrColStruct(m_sHdrColStruct);
        m_lstMsg.vSetSortableMsgColumns(m_sHdrColStruct, m_eBusType);

        m_lstMsg.vShowHideBlankcolumn(m_bInterPretMsg);
    }
    else
    {
        AfxMessageBox(_("Not able to create the list control."));
    }

    m_podMsgIntprtnDlg = new CMessageInterpretation(this);
    m_podMsgIntprtnDlg->vCreateMsgIntrprtnDlg(this, FALSE);


    m_podMsgIntprtnDlg->vSetCaption(GetProtocolStringName().c_str());

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

    //createToolBar();

    return 0;
}
BOOL CMsgFrmtWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* /*pContext*/)
{
    createToolBar();



    return TRUE;
}
void CMsgFrmtWnd::createToolBar()
{
    if (!m_wndToolbarImportLog.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT,
                                        WS_BORDER | WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
                                        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0, 0, 0, 0), IDR_TLB_IMPORT_LOG) ||
            !m_wndToolbarImportLog.LoadToolBar(IDR_TLB_IMPORT_LOG))
    {
        return;
    }


    m_wndToolbarImportLog.bLoadCNVTCToolBar(72, IDR_TLB_IMPORT_LOG, IDR_TLB_IMPORT_LOG, IDR_TLB_IMPORT_LOG);

    //2. Add Edit Ctrl Page No. for Import Log Toolbar
    CRect rect;
    m_wndToolbarImportLog.GetItemRect(1, &rect);
    rect.bottom -= 50;
    if (!m_omEditPageNo.Create(
                WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT,
                rect, &m_wndToolbarImportLog, ID_BTN_EDIT_PAGENO))
    {
        TRACE("Failed to create Page no. Edit box\n");
        return ;
    }

    m_omEditPageNo.vSetBase(BASE_DECIMAL);
    if (!m_omSpinPageNo.Create(WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS | UDS_WRAP | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_NOTHOUSANDS,
                               rect, &m_wndToolbarImportLog, 1))
    {
        TRACE("Failed to create Spin control for Page No.\n");
        return ;
    }

    //3. Add Slider Ctrl for Import Log Toolbar
    m_wndToolbarImportLog.GetItemRect(3, &rect);
    rect.right = rect.right + 2 * (rect.right - rect.left);
    if (!m_omSliderMsgs.Create(WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT, rect, &m_wndToolbarImportLog, ID_BTN_MSG_SLIDER))
    {
        TRACE("Failed to create Slider Ctrl for Msgs\n");
        return;
    }

    m_omSpinPageNo.SetBuddy(&m_omEditPageNo);
    m_omSpinPageNo.SetRange(1, 100);
    m_omSpinPageNo.SetPos(1);



    ShowControlBar(&m_wndToolbarImportLog, FALSE, 0);

    m_omSliderMsgs.SetRange(0, defDEFAULT_PAGE_LENGTH - 1);
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

    if (m_lstMsg.m_hWnd != nullptr)
    {
        vFitListCtrlToWindow();

        ::SendMessage( m_hWnd, WM_UPDATE_TREE_ITEMS_POS, 0, 0);
    }
}

void CMsgFrmtWnd::vFitListCtrlToWindow()
{
    {
        CRect sClientRect;
        GetClientRect(&sClientRect);
        int ClientWidth = 0;
        if ( sClientRect.left - sClientRect.right < 0 )
        {
            ClientWidth = sClientRect.right - sClientRect.left;
        }

        else
        {
            ClientWidth = abs(sClientRect.left - sClientRect.right);
        }


        int nTotalWidth = 0;
        int columnCount = m_lstMsg.GetHeaderCtrl()->GetItemCount();
        for (int nIdx = 0; nIdx < columnCount-1; nIdx++)
        {
            nTotalWidth += m_lstMsg.GetColumnWidth(nIdx);
        }
        int nLastColWidth = ClientWidth - nTotalWidth;
        m_lstMsg.SetColumnWidth(columnCount-1, nLastColWidth);

        CRect  toolbarRect;
        m_wndToolbarImportLog.GetWindowRect(toolbarRect);

        if (m_wndToolbarImportLog.IsVisible())
        {
            CRect rect;
            rect.top = 0 + toolbarRect.Height();
            rect.left = 0;
            rect.right = sClientRect.right;
            rect.bottom = sClientRect.bottom + toolbarRect.Height();
            m_lstMsg.MoveWindow(rect);
        }
        else
        {
            m_lstMsg.MoveWindow(&sClientRect);
            m_lstMsg.SetWindowPos(&wndTop, 0, 0, sClientRect.Width(),
                                  sClientRect.Height(), SWP_NOZORDER);
        }
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
    m_pouMsgContainerIntrf->GetFilterScheme((void*)wParam);
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
    m_pouMsgContainerIntrf->ApplyFilterScheme((void*)wParam);
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
    if (nullptr != m_pouMsgContainerIntrf)
    {
        m_pouMsgContainerIntrf->EnableFilterApplied((BOOL)wParam);
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
    BOOL* pbInterpret = nullptr;
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
    if (m_lstMsg.GetSafeHwnd() == nullptr)
    {
        return;
    }
    //Remove all the Msg signal tree
    vRemoveAllMsgTree();
    EnterCriticalSection(&m_ouCriticalSection);
    m_omMsgDispMap.RemoveAll();
    LeaveCriticalSection(&m_ouCriticalSection);
    m_omMgsIndexVec.clear();
    if(m_pouMsgContainerIntrf != nullptr)
    {
        m_pouMsgContainerIntrf->vEditClearAll();
        UnloadFile(true);
    }
    m_lstMsg.DeleteAllItems();
    m_lstMsg.Invalidate();
    if (m_podMsgIntprtnDlg->IsWindowVisible())
    {
        m_podMsgIntprtnDlg->vClearWindowContent();
        m_podMsgIntprtnDlg->DestroyWindow();
        m_podMsgIntprtnDlg->vCreateMsgIntrprtnDlg(this, FALSE);
    }
}

void CMsgFrmtWnd::UnloadFile(bool bFileUnload)
{
    if(m_bUnloadOnClear)
    {
        if(m_omStrWindowTitle!="")
        {
            this->SetWindowTextA(m_omStrWindowTitle);
        }
        if(bFileUnload)
        {
            if(S_OK == m_pouMsgContainerIntrf->UnloadLogFile())
            {
                m_bIsLogFileImported = false;
            }
        }
        ShowImportLogToolbar(FALSE);
        m_bUnloadOnClear = false;
        bStopReadThread();
    }
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
    while( pos != nullptr )
    {
        m_omMsgDispMap.GetNextAssoc( pos, n64Temp, sTemp );
        if (sTemp.m_opTreeWndParam != nullptr)
        {
            delete sTemp.m_opTreeWndParam;
            sTemp.m_opTreeWndParam = nullptr;
            m_omMsgDispMap [n64Temp] = sTemp;
        }
    }
    //m_omMsgDispMap.RemoveAll();
}
/*******************************************************************************
  Function Name  : CmdRouteMenu
  Input(s)       : CWnd* pWnd,CMenu* pPopupMenu
  Output         : -
  Functionality  : To Call ON_UPDATE_COMMAND_UI from Context menu to
                   Check/uncheck Show Toolbar.
  Member of      : CMsgFrmtWnd
  Author(s)      :
  Date Created   : 18-05-2015
  Modifications  :
*******************************************************************************/
void CMsgFrmtWnd::CmdRouteMenu(CWnd* pWnd,CMenu* pPopupMenu)
{
    CCmdUI state;
    state.m_pMenu = pPopupMenu;
    state.m_pParentMenu = pPopupMenu;
    state.m_nIndexMax = pPopupMenu->GetMenuItemCount();

    for (state.m_nIndex = 0;
            state.m_nIndex < state.m_nIndexMax;
            state.m_nIndex++)
    {
        state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);

        // menu separator or invalid cmd - ignore it
        if (state.m_nID == 0)
        {
            continue;
        }

        if (state.m_nID == (UINT)-1)
        {
            // possibly a popup menu, route to child menu if so
            CMenu* pSub=pPopupMenu->GetSubMenu(state.m_nIndex);
            if(pSub)
            {
                CmdRouteMenu(pWnd,pSub);
            }
        }
        else
        {
            // normal menu item, Auto disable if command is
            // _not_ a system command.
            state.m_pSubMenu = NULL;
            state.DoUpdate(pWnd, FALSE);
        }
    }
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
    CRect rect;
    CPoint hitPoint(LOWORD(lParam), HIWORD(lParam));
    if (TRUE == m_wndToolbarImportLog.IsVisible())
    {
        m_wndToolbarImportLog.GetWindowRect(&rect);
        hitPoint.y -= rect.Height();
    }

    if (nEvent == WM_LBUTTONDOWN)
    {
        int nIndex;
        UINT nFlag;



        // Make sure this event occurs for a valid message item
        nIndex = m_lstMsg.HitTest(hitPoint, &nFlag);

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
                    ::PostMessage(this->m_hWnd, WM_LSTC_DBLCLK, nIndex, 0);
                }
                else
                {
                    __int64 nMapIndex = m_omMgsIndexVec[nIndex];
                    if (nMapIndex != nInvalidKey)
                    {
                        ::PostMessage(this->m_hWnd, WM_LSTC_DBLCLK, nIndex, 0);
                    }
                }
            }
        }
    }
    else if (nEvent == WM_RBUTTONDOWN)
    {
        int nIndex = -1;

        // Get the mouse click coordinates and get the list ctrl header rectangle
        CRect rect;
        m_wndToolbarImportLog.GetWindowRect(&rect);
        CPoint tmpPoint = CPoint(LOWORD(lParam), HIWORD(lParam) - rect.Height());
        CRect headerRect;
        if (m_lstMsg.m_hWnd != nullptr)
        {
            m_lstMsg.GetHeaderCtrl()->GetClientRect(&headerRect);
        }

        // Check if the mouse click point is within the header rectangle
        if (!headerRect.PtInRect(hitPoint))
        {
            POSITION Pos = m_lstMsg.GetFirstSelectedItemPosition();
            if (Pos != nullptr)
            {
                nIndex = m_lstMsg.GetNextSelectedItem(Pos);
            }

            UINT nFlag;
            // Make sure this event occurs for a valid message item
            int nItem = m_lstMsg.HitTest(hitPoint, &nFlag);

            if (nItem == -1)
            {
                int nToStop = 0;
            }

            m_lstMsg.SetItemState( nItem,
                                   LVIS_SELECTED | LVIS_FOCUSED,
                                   LVIS_SELECTED | LVIS_FOCUSED);

            // Take this as selected item
            nIndex = nItem;
            // Enable/Disable Send Item as per connection status
            BOOL bConnected = TRUE;
            // Get flag object
            bConnected = mBusConnectedState;
            // If it is not connected then disable it other wise enable

            CMenu* pomContextMenu = new CMenu;
            if (pomContextMenu != nullptr)
            {
                // Load the Menu from the resource
                pomContextMenu->DestroyMenu();
                CMenu* pomSubMenu = nullptr;
                pomContextMenu->LoadMenu(IDM_MENU_MSG_OPRN);
                pomSubMenu = pomContextMenu->GetSubMenu(0);

                //1. Import log file menu
                pomContextMenu->EnableMenuItem(IDM_MESSAGE_IMPORTLOGFILE, MF_DISABLED | MF_GRAYED);

                //2. Message Expand menu
                if (nIndex < 0)
                {
                    pomContextMenu->EnableMenuItem(IDM_MESSAGE_EXPAND, MF_DISABLED | MF_GRAYED);
                }
                else if((nIndex >= 0) && (nFlag & LVHT_ONITEM))
                {
                    if (IS_MODE_APPEND(m_bExprnFlag_Disp) || !IS_MODE_INTRP(m_bExprnFlag_Disp))
                    {
                        pomContextMenu->EnableMenuItem(IDM_MESSAGE_EXPAND, MF_DISABLED | MF_GRAYED);
                    }
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
                                    pomContextMenu->ModifyMenu(IDM_MESSAGE_EXPAND,
                                                               MF_BYCOMMAND,
                                                               IDM_MESSAGE_EXPAND,
                                                               _(defSTR_COLLAPSE_MENU_TEXT));
                                }
                                else  if (sTemp.m_eInterpretMode == NON_INTERPRETABLE)
                                {
                                    pomContextMenu->EnableMenuItem(IDM_MESSAGE_EXPAND,
                                                                   MF_DISABLED | MF_GRAYED);
                                }
                            }
                        }
                        else
                        {
                            return;
                        }
                    }
                }

                //3. Send Message
                if( bConnected == FALSE )
                {
                    pomContextMenu->EnableMenuItem( IDM_MESSAGE_SEND,
                                                    MF_DISABLED |MF_GRAYED);

                }


                if( bConnected == TRUE )
                {
                    if(m_eBusType == LIN)
                    {
                        pomContextMenu->EnableMenuItem( IDM_MESSAGE_SEND,
                                                        MF_DISABLED |MF_GRAYED);
                    }
                }


                if (pomSubMenu != nullptr)
                {
                    CPoint omSrcPt(lParam);
                    ClientToScreen(&omSrcPt);
                    CWnd* pWndPopupOwner = this;
                    CmdRouteMenu(pWndPopupOwner,pomContextMenu->GetSubMenu(0));
                    pomSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
                                               omSrcPt.x, omSrcPt.y, pWndPopupOwner, nullptr);

                }
                delete pomContextMenu;
                pomContextMenu = nullptr;
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
const int SIZE_STLIN_MSG = sizeof(STLIN_MSG);
void CMsgFrmtWnd::OnSendSelectedMessageEntry()
{
    POSITION Pos = m_lstMsg.GetFirstSelectedItemPosition();
    if (Pos != nullptr)
    {
        int nItem = m_lstMsg.GetNextSelectedItem(Pos);

        __int64 nMsgKey = nItem;
        if (!IS_MODE_APPEND(m_bExprnFlag_Disp))
        {
            nMsgKey = m_omMgsIndexVec[nItem];
        }
        m_pouMsgContainerIntrf->sendMessage(nMsgKey, IS_MODE_APPEND(m_bExprnFlag_Disp));
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
    if (Pos != nullptr)
    {
        int nItem = m_lstMsg.GetNextSelectedItem(Pos);
        vExpandContractMsg( nItem );
    }
}


/******************************************************************************
 Function Name  : OnImportLogFile

 Input(s)       : -
 Output         : -
 Functionality  : This function will be called to Import Log File.
 Member of      : CMsgFrmtWnd

 Author(s)      : Robin G.K.
 Date Created   : 27-05-2015
******************************************************************************/
void CMsgFrmtWnd::OnImportLogFile()
{
    CFileDialog om_Dlg( TRUE,  //open an existing file
                        ".log",  //extension to file
                        nullptr,  //initial file name
                        OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
                        defIMPORT_LOG_FILTER,
                        this);
    // Set caption text
    om_Dlg.m_ofn.lpstrTitle = "Select the File";

    if ( om_Dlg.DoModal() == IDOK )
    {
        CString strExtName  = om_Dlg.GetFileExt();
        strExtName.MakeLower();
        if(strExtName == "log")
        {
            OnClearAll(0,0);
            m_bUnloadOnClear = true;
            std::string strFilePath = om_Dlg.GetPathName();
            //Set File Path to Window Text
            this->GetWindowText(m_omStrWindowTitle);
            this->SetWindowTextA(m_omStrWindowTitle+"  "+strFilePath.c_str());
            sLoadFile structLoadFile;
            structLoadFile.m_strFileName = strFilePath;
            structLoadFile.m_pouMsgContainerIntrf = m_pouMsgContainerIntrf;
            structLoadFile.m_oupMsgFrmtWnd = this;
            CProgressBarDlg odProgressCtrlDlg(structLoadFile);
            /*m_odProgressCtrlDlg.LoadFile(structLoadFile);*/
            if(odProgressCtrlDlg.DoModal()==IDCANCEL)
            {
                UnloadFile(FALSE);
            }
            else
            {
                unsigned long nPageCount=0;
                if(m_pouMsgContainerIntrf!=nullptr)
                {
                    m_bIsLogFileImported = true;
                    m_pouMsgContainerIntrf->GetTotalPages(nPageCount);
                    OnImportLogSetTotalPages(nPageCount);
                    ShowImportLogToolbar(TRUE);
                }
            }
        }
        else
        {
            AfxMessageBox(defSTR_ERR_INVALID_FILE ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
        }
    }
}

void CMsgFrmtWnd::OnImportLogSetTotalPages(int pageNumber)
{
    m_wndToolbarImportLog.SetSliderHandleWnd(this->GetSafeHwnd());
    m_omSpinPageNo.SetRange(1, pageNumber);
    m_omSpinPageNo.SetPos(1);
}
void CMsgFrmtWnd::ShowImportLogToolbar(BOOL bShow)
{
    ShowControlBar(&m_wndToolbarImportLog, bShow, 0);
    vFitListCtrlToWindow();
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
    if ( nullptr != m_pouMsgContainerIntrf )
    {
        m_pouMsgContainerIntrf->vInit(pParam);
        m_pouMsgContainerIntrf->bGetDilInterFace();
    }
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
    if (nIDEvent == m_unDispUpdateTimerId)
    {
        if (m_bUpdate == TRUE)
        {
            //Get count of list control
            int nBuffMsgCnt = 0;
            //Now for upadating no. of items in list ctrl
            if( IS_MODE_APPEND(m_bExprnFlag_Disp) && m_pouMsgContainerIntrf != nullptr )
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
                unsigned int msgId;
                CString strName = "";
                m_nIndex = nBuffMsgCnt-1;
                SSignalInfoArray SigInfoArray;
                vGetSignalInfoArray(m_unCurrInterpretedMapIndex, SigInfoArray);
                bool bHex = IS_NUM_DEC_SET(m_bExprnFlag_Disp);
                m_pouMsgContainerIntrf->GetMessageDetails(m_unCurrInterpretedMapIndex, msgId, strName, bHex);


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
            m_pBmNetWork = (IBMNetWorkGetService*)lParam;
            m_pouMsgContainerIntrf->SetIBMNetWorkGetService( m_pBmNetWork );
        }
        break;
        case eBusStatusChanged:
        {
            mBusConnectedState = (lParam != 0);
        }
        break;
        case eWINID_START_READ:
        {
            OnClearAll(0,0);
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
            xmlNodePtr pxmlNodePtr = (xmlNodePtr)lParam;
            //BYTE* pbyData = (BYTE*)lParam;
            //GetConfigData(pbyData);
            GetConfigData(pxmlNodePtr);
        }
        break;

        case eWINID_MSG_WND_SET_CONFIG_DATA:
        {
            BYTE* pbyData = (BYTE*)lParam;
            SetConfigData(pbyData);

            //xmlDocPtr pNodePtr = (xmlDocPtr)lParam;

            //if(pNodePtr != nullptr)
            //{
            //  //BYTE* pbyData = (BYTE*)lParam;
            //  SetConfigData(pNodePtr);
            //}
        }
        break;

        case eWINID_MSG_WND_SET_CONFIG_DATA_XML:
        {
            xmlDocPtr pNodePtr = (xmlDocPtr)lParam;
            if(pNodePtr != nullptr)
            {
                SetConfigData(pNodePtr);
            }
        }
        break;

        case eWINID_MSG_WND_GET_BUFFER_DETAILS:
        {
            INT* pMsgBuffSize = (INT*)lParam;
            //If it is nullptr values.i.e user did not configure buffer details.
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

    {
        // Append Buffer Size
        m_anMsgBuffSize[defAPPEND_DATA_INDEX] = pMsgBuffSize[defAPPEND_DATA_INDEX];
        // Overwrite Buffer Size
        m_anMsgBuffSize[defOVERWRITE_DATE_INDEX] = pMsgBuffSize[defOVERWRITE_DATE_INDEX];
        // Display Update Rate
        m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX] = pMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX];
    }

    if ((m_unDispUpdateTimerId = SetTimer(TIMER_ID_DISPLAY_UPDATE,
                                          m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX], nullptr)) == 0)
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

    if(lParam!=0)
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

    m_nIndex = wParam;
    if (!IS_MODE_APPEND(m_bExprnFlag_Disp))
    {
        nMapIndex = m_omMgsIndexVec[(int)wParam];
        int nMsgCode = nMapIndex & 0xFFFF;
        WORD wDirId = HIWORD(nMsgCode);
        WORD wEventMsgType = LOWORD(nMsgCode);
        INT nMsgId = HIBYTE(wDirId);
        int nEventType = LOBYTE(wEventMsgType);
        vShowUpdateMsgIntrpDlg(nMapIndex);
    }

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
    //int nMsgCode = nMapIndex & 0xFFFF;
    //nMsgCode = nGetCodefromMapKey(nMapIndex);

    unsigned int nMsgCode = 0;

    //Message Name
    CString strName;
    bool isHex = IS_NUM_DEC_SET(m_bExprnFlag_Disp);
    m_pouMsgContainerIntrf->GetMessageDetails(nMapIndex, nMsgCode, strName, isHex);

    //Signal List
    SSignalInfoArray SigInfoArray;
    if (S_OK != vGetSignalInfoArray(nMapIndex, SigInfoArray))
    {
        return;
    }


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
    if ( nullptr == m_pouMsgContainerIntrf )
    {
        return S_FALSE;
    }
    long long indexId = 0;
    indexId = nMsgCode;
    if (!IS_MODE_APPEND(m_bExprnFlag_Disp) && m_omMgsIndexVec.size() > (int)wParam )
    {
        indexId = m_omMgsIndexVec[(int)wParam];
    }
    else
    {
        indexId = (int)wParam;
    }
    m_lstMsg.vSetMsgColor(m_pouMsgContainerIntrf->getMessageColor(indexId, IS_MODE_APPEND(m_bExprnFlag_Disp), &m_ouMsgAttr));



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
    SWMUPDATEPTRPARA* psParam = nullptr;
    HRESULT hResult = S_FALSE;
    int nMsgCode    = 0;
    psParam = (SWMUPDATEPTRPARA*) wParam;

    if(nullptr != m_pouMsgContainerIntrf)
    {
        if( IS_MODE_APPEND(m_bExprnFlag_Disp) )
        {
            //TODO::hUpdateFormattedMsgStruct has to update the Message Name inside
            hResult = m_pouMsgContainerIntrf->hUpdateFormattedMsgStruct(psParam->m_nListIndex,
                      nMsgCode,
                      m_bExprnFlag_Disp);
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

    if ( nullptr == m_pouMsgContainerIntrf )
    {
        return S_FALSE;
    }


    m_pouMsgContainerIntrf->DoSortBuffer(m_nField,m_bAscending);

    m_pExpandedMapIndexes = (__int64*)malloc(sizeof(__int64) * m_omMgsIndexVec.size());
    if( m_pExpandedMapIndexes == nullptr)
    {
        AfxMessageBox(_("Unable to Allocate Memory."));
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

            if(nullptr != m_pouMsgContainerIntrf)
            {
                m_pouMsgContainerIntrf->GetMapIndexAtID(i, nMapIndex);
                m_omMgsIndexVec[i] = nMapIndex;
                SMSGDISPMAPENTRY sDispEntry;
                if (m_omMsgDispMap.Lookup(nMapIndex, sDispEntry ))
                {
                    sDispEntry.m_nBufferIndex = i;
                    m_omMsgDispMap.SetAt(nMapIndex, sDispEntry);
                }
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
  Function Name  : vSetDefaultHeaders
  Input(s)       : -
  Output         : -
  Functionality  : Sets the Default Headers based on the BUS type.
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
            somArrColTitle[sHdrCtrlPos.m_byTimePos]     = _("Time              ");
            somArrColTitle[sHdrCtrlPos.m_byRxTxPos]     = _("Tx/Rx          ");
            somArrColTitle[sHdrCtrlPos.m_byChannel]     = _("Channel        ");
            somArrColTitle[sHdrCtrlPos.m_byMsgTypePos]  = _("Type");
            somArrColTitle[sHdrCtrlPos.m_byIDPos]       = _("ID            ");
            somArrColTitle[sHdrCtrlPos.m_byCodeNamePos] = _("Message       ");
            somArrColTitle[sHdrCtrlPos.m_byMsgTypePos]  = _("Msg Type");
            somArrColTitle[sHdrCtrlPos.m_byDLCPos]      = _("DLC        ");
            somArrColTitle[sHdrCtrlPos.m_byDataPos]     = _("Data Byte(s)                                     ");

            m_MsgHdrInfo.vInitializeColDetails(sHdrCtrlPos, somArrColTitle, nColCount);

            vRearrangeCols();
        }
        break;
        break;
        case LIN:
        {
            int nColCount = 9;
            //Set the positions for coloumns
            sHdrCtrlPos.m_byTimePos     = 0;
            sHdrCtrlPos.m_byCodeNamePos = 1;
            sHdrCtrlPos.m_byMsgTypePos  = 2;
            sHdrCtrlPos.m_byRxTxPos     = 3;
            sHdrCtrlPos.m_byChannel     = 4;
            sHdrCtrlPos.m_byDLCPos      = 5;
            sHdrCtrlPos.m_byIDPos       = 6;
            sHdrCtrlPos.m_byDataPos     = 7;
            sHdrCtrlPos.m_byChecksumPos = 8;


            //Set the col string
            somArrColTitle[sHdrCtrlPos.m_byTimePos]     = _("Time              ");
            somArrColTitle[sHdrCtrlPos.m_byRxTxPos]     = _("Tx/Rx          ");
            somArrColTitle[sHdrCtrlPos.m_byChannel]     = _("Channel     ");
            somArrColTitle[sHdrCtrlPos.m_byIDPos]       = _("ID           ");
            somArrColTitle[sHdrCtrlPos.m_byCodeNamePos] = _("Message        ");
            somArrColTitle[sHdrCtrlPos.m_byMsgTypePos]  = _("Message Type        ");
            somArrColTitle[sHdrCtrlPos.m_byDLCPos]      = _("DLC         ");
            somArrColTitle[sHdrCtrlPos.m_byDataPos]     = _("Data Byte(s)                                     ");
            somArrColTitle[sHdrCtrlPos.m_byChecksumPos] = _("Checksum ");

            m_MsgHdrInfo.vInitializeColDetails(sHdrCtrlPos, somArrColTitle, nColCount);

            vRearrangeCols();
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
            somArrColTitle[sHdrCtrlPos.m_byTimePos]     = _("Time             ");
            somArrColTitle[sHdrCtrlPos.m_byChannel]     = _("Channel       ");
            somArrColTitle[sHdrCtrlPos.m_byIDPos]       = _("CAN ID         ");
            somArrColTitle[sHdrCtrlPos.m_byPGNPos]      = _("PGN            ");
            somArrColTitle[sHdrCtrlPos.m_byCodeNamePos] = _("PGN Name          ");
            somArrColTitle[sHdrCtrlPos.m_byMsgTypePos]  = _("Type         ");
            somArrColTitle[sHdrCtrlPos.m_bySrcPos]      = _("Src        ");
            somArrColTitle[sHdrCtrlPos.m_byDestPos]     = _("Dest     ");
            somArrColTitle[sHdrCtrlPos.m_byPriorityPos] = _("Priority     ");
            somArrColTitle[sHdrCtrlPos.m_byRxTxPos]     = _("Tx/Rx       ");
            somArrColTitle[sHdrCtrlPos.m_byDLCPos]      = _("DLC         ");
            somArrColTitle[sHdrCtrlPos.m_byDataPos]     = _("Data Byte(s)                                     ");

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

    omTmpColTitle = _("bl");
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
    if ( nullptr == m_pouMsgContainerIntrf )
    {
        return FALSE;
    }

    int bResult = TRUE;
    m_pouMsgContainerIntrf->SetClientID(m_dwClientID);
    if (m_pouMsgContainerIntrf->bStartReadThread())
    {
        if ((m_unDispUpdateTimerId = SetTimer(TIMER_ID_DISPLAY_UPDATE,
                                              m_anMsgBuffSize[defDISPLAY_UPDATE_DATA_INDEX], nullptr)) == 0)
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
void CMsgFrmtWnd::onRxMsg(void* pMsg)
{
    if (m_pouMsgContainerIntrf == nullptr)
    {
        return;
    }


    EINTERPRET_MODE eInterpreteMode = EINTERPRET_MODE::MODE_NONE;
    SMSGDISPMAPENTRY sDispEntry;
    int nBufIndex = 0;
    UINT nMsgCode = 0;
    __int64 dwTimeStamp = 0;
    __int64 dwMapIndex = 0;

    m_pouMsgContainerIntrf->vSaveOWandGetDetails(pMsg, dwMapIndex, dwTimeStamp, nMsgCode, nBufIndex, eInterpreteMode);

    EnterCriticalSection(&m_ouCriticalSection);
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

        sDispEntry.m_eInterpretMode = eInterpreteMode;
        //Both times will be same so that rel. time = 0
        sDispEntry.m_nTimeOffset = dwTimeStamp;
        sDispEntry.m_nTimeStamp  = dwTimeStamp;
        //Update array list
        m_omMgsIndexVec.push_back(dwMapIndex);
    }
    //LeaveCriticalSection(&m_omCritSecForMapArr);
    //Update the map
    m_omMsgDispMap[dwMapIndex] = sDispEntry;
    LeaveCriticalSection(&m_ouCriticalSection);


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
    if ( nullptr != m_pouMsgContainerIntrf )
    {
        //*************************************TBD the header details 4 diff. protocol
        m_pouMsgContainerIntrf->vGetUpdatedCurrDataPtrArray(m_sHdrColStruct, m_pomDataPtrArr, m_bExprnFlag_Disp);
        //****************************************
        m_lstMsg.vSetDsipItemDataPtrArr(m_pomDataPtrArr);
    }
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
    if (m_lstMsg.m_hWnd != nullptr)
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

LRESULT CMsgFrmtWnd::OnDisplaySettingUpdated(WPARAM wParam, LPARAM /*lParam*/)
{
    if ( wParam != 0 )
    {
        DisplaySettings* ouSettings = (DisplaySettings*)wParam;
        m_pouMsgContainerIntrf->SetDisplaySettings(*ouSettings);
    }
    return S_OK;
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
        if(m_podMsgIntprtnDlg != NULL)
        {
            if(m_podMsgIntprtnDlg->IsWindowVisible())
            {
                vShowUpdateMsgIntrpDlg(m_unCurrInterpretedMapIndex);
            }
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
                if ( nullptr != m_pouMsgContainerIntrf )
                {
                    nMsgCount = m_pouMsgContainerIntrf->nGetAppendBufferCount();
                }
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
            OnUpdateImportLogOverwriteScroll();
            m_lstMsg.vShowHideBlankcolumn(m_bInterPretMsg);
            m_lstMsg.SetItemCountEx(nMsgCount);
            bToUpdate = TRUE;
        }
    }
    m_bUpdate = bToUpdate;
    vUpdatePtrInLstCtrl();
    LeaveCriticalSection(&m_CritSec1);

    vFitListCtrlToWindow();
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
        while( pos != nullptr )
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
        while( pos != nullptr )
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
HRESULT CMsgFrmtWnd::vGetSignalInfoArray(__int64 nMapIndex, SSignalInfoArray& SigInfoArray)
{
    static bool isHexMode;
    if (IS_NUM_HEX_SET(m_bExprnFlag_Disp))
    {
        isHexMode = true;
    }
    else
    {
        isHexMode = false;
    }

    return m_pouMsgContainerIntrf->GetSignalInfoArray(nMapIndex, SigInfoArray, isHexMode);
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
                    if (sTemp.m_opTreeWndParam != nullptr)
                    {
                        sTemp.m_opTreeWndParam->vShift (omTreeRect.Height() +
                                                        defMSG_IPET_WND_BORDER_WIDTH);
                    }
                }
                else
                {
                    ASSERT(false);
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
    COLORREF rgbTreeItem = RGB(0,0,0);
    if (nMsgKey != nInvalidKey)
    {
        rgbTreeItem = m_pouMsgContainerIntrf->getMessageColor(nMsgKey, true, &m_ouMsgAttr);
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
                if (sTemp.m_opTreeWndParam != nullptr)
                {
                    sTemp.m_opTreeWndParam->vUpdateTreeItemRect(omTreeRect);
                }
            }
            else
            {
                ASSERT(false);
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
    if ( sEntry.m_opTreeWndParam != nullptr )
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
                        if (sTemp.m_opTreeWndParam != nullptr)
                        {
                            sTemp.m_opTreeWndParam->vShift ( -(omTreeRect.Height() +
                                                               defMSG_IPET_WND_BORDER_WIDTH));
                        }
                    }
                    else
                    {
                        ASSERT(false);
                    }
                }
            }
        }

        delete sEntry.m_opTreeWndParam;
        sEntry.m_opTreeWndParam = nullptr;

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
    while( pos != nullptr )
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
            if (sTemp.m_opTreeWndParam != nullptr)
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
    static STCANDATA sCANMsg;
    static STLINDATA sLINMsg;
    for (int i = 0; i < nSize; i++)
    {
        n64Temp = m_omMgsIndexVec[i];
        if (m_omMsgDispMap.Lookup(n64Temp, sTemp))
        {
            if (sTemp.m_opTreeWndParam != nullptr)
            {
                COLORREF rgbTreeItem = RGB(0,0,0);
                if (n64Temp != nInvalidKey)
                {
                    rgbTreeItem = m_pouMsgContainerIntrf->getMessageColor(n64Temp, true, &m_ouMsgAttr);
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
                CRect omTreeRect;
                vGetTreeRect(i, SigInfoArray.GetSize(), omTreeRect);

                sTemp.m_opTreeWndParam->vUpdateTreeItemRect(omTreeRect);
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
        if (sTemp.m_opTreeWndParam != nullptr /*&& sTemp.m_opTreeWndParam->bVerifyTreeCtrl()*/)
        {
            COLORREF rgbTreeItem = RGB(0,0,0);
            if (nMapIndex != nInvalidKey)
            {
                rgbTreeItem = m_pouMsgContainerIntrf->getMessageColor(nMapIndex, true, &m_ouMsgAttr);
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
    if (pHeaderCtrl != nullptr)
    {
        int  nColumnCount = pHeaderCtrl->GetItemCount();
        LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));
        if(pnOrder != nullptr)
        {
            m_lstMsg.GetColumnOrderArray(pnOrder, nColumnCount);

            int i;
            for (i = 0 ; i < nColumnCount; i++)
            {
                pnOrder[i] = i;
            }
            m_lstMsg.SetColumnOrderArray(nColumnCount, pnOrder);
            free(pnOrder);
        }
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





std::string CMsgFrmtWnd::GetProtocolStringName()
{
    std::string strTag = "";
    switch (m_eBusType)
    {
        case CAN:
            strTag = "CAN";
            break;
        case J1939:
            strTag = "J1939";
            break;
        case LIN:
            strTag = "LIN";
            break;
        default:
            break;
    }
    return strTag;
}
HRESULT CMsgFrmtWnd::SetConfigData(xmlDocPtr pDocPtr)
{
    xmlNodeSetPtr pColMsgWnd = nullptr;
    xmlDocPtr m_xmlConfigFiledoc = pDocPtr;
    std::string strBusMsgWndName = GetProtocolStringName();
    char chMaxXpath[255];

    sprintf_s(chMaxXpath, sizeof(chMaxXpath), def_XPATH_COLUMN, strBusMsgWndName.c_str());
    xmlChar* pchPath = (xmlChar*)chMaxXpath;
    xmlXPathObjectPtr pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
    if( nullptr != pPathObject )
    {
        pColMsgWnd = pPathObject->nodesetval;
    }
    if(pColMsgWnd != nullptr)
    {
        ColumnInfoMap InfoMap;

        INT nRetVal = S_OK;
        nRetVal = xmlUtils::parseColumnInfoNode(pColMsgWnd, InfoMap);
        //Reading Version.
        BYTE byVer = 0;
        //COPY_DATA_2(&byVer, pByteSrc, sizeof(BYTE));
        UINT nSize= 0;

        if(nRetVal == S_OK)
        {
            //Reading column Header Positions.
            CHeaderCtrl* pHeaderCtrl = m_lstMsg.GetHeaderCtrl();
            if (pHeaderCtrl != nullptr)
            {
                int  nColumnCount=0;

                nColumnCount = pColMsgWnd->nodeNr;

                //Reading column count.
                //COPY_DATA_2(&nColumnCount, pByteSrc, sizeof(UINT));

                LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));
                if ( pnOrder != nullptr )
                {
                    for (int i = 0 ; i < nColumnCount; i++)
                    {
                        xmlNodePtr pNodePtr = pColMsgWnd->nodeTab[i]->xmlChildrenNode;

                        while(pNodePtr != nullptr)
                        {
                            CString strName = (char*)pNodePtr->name;

                            if(strName == "ID")
                            {
                                xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);

                                strName = ptext;

                                ColumnInfoMap::iterator itr = InfoMap.find(strName.GetBuffer(strName.GetLength()));

                                if(itr != InfoMap.end())
                                {
                                    INT nOrder = itr->second.nOrder;
                                    pnOrder[i] = itr->second.nOrder;
                                    m_lstMsg.ShowColumn(i, itr->second.isVisble);
                                    m_lstMsg.SetColumnWidth(i, itr->second.nWidth);
                                }
                                break;
                            }
                            pNodePtr = pNodePtr->next;
                        }
                    }
                    m_lstMsg.SetColumnOrderArray(nColumnCount, pnOrder);
                    free(pnOrder);
                }
            }

            bool bHexDec = false;

            sprintf_s(chMaxXpath, sizeof(chMaxXpath), def_XPATH_ISHEX, strBusMsgWndName.c_str());
            pchPath = (xmlChar*)chMaxXpath;
            //pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/IsHex";
            pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
            if( nullptr != pPathObject )
            {
                xmlNodeSetPtr pIsHexPtr = pPathObject->nodesetval;

                if(nullptr != pIsHexPtr)
                {
                    for(int i=0; i < pIsHexPtr->nodeNr; i++)
                    {
                        xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pIsHexPtr->nodeTab[i]->xmlChildrenNode, 1);
                        if ( nullptr != ptext )
                        {
                            CString strIsHex = (char*)ptext;
                            if(strIsHex == "TRUE" || strIsHex == "1")
                            {
                                bHexDec = true;
                            }
                            else
                            {
                                bHexDec = false;
                            }
                            xmlFree(ptext);
                        }
                    }
                    xmlXPathFreeObject (pPathObject);
                }
            }

            //Reading Hex/Dec Display.

            //COPY_DATA_2(&bHexDec, pByteSrc, sizeof(bool));

            // commented as IsHex is conflicting with global
            // DisplayNumericMode
            //CLEAR_EXPR_NUM_BITS(m_bExprnFlag_Disp);
            ////m_bExprnFlag_Disp |= (wArguments & BITS_NUM);
            //if(bHexDec)
            //{
            //    SET_NUM_HEX(m_bExprnFlag_Disp);
            //}
            //else
            //{
            //    SET_NUM_DEC(m_bExprnFlag_Disp);
            //}

            //Reading Overwrite/Append Mode.
            bool bOvrwAppend = false;
            sprintf_s(chMaxXpath, sizeof(chMaxXpath), def_XPATH_ISAPPENDED, strBusMsgWndName.c_str());
            pchPath = (xmlChar*)chMaxXpath;

            //pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/IsAppend";
            pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
            if( nullptr != pPathObject )
            {
                xmlNodeSetPtr pIsHexPtr = pPathObject->nodesetval;

                if(nullptr != pIsHexPtr)
                {
                    for(int i=0; i < pIsHexPtr->nodeNr; i++)
                    {
                        xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pIsHexPtr->nodeTab[i]->xmlChildrenNode, 1);
                        if ( nullptr != ptext )
                        {
                            CString strIsAppend = (char*)(ptext);
                            if(strIsAppend == "TRUE" || strIsAppend == "1")
                            {
                                bOvrwAppend = true;
                            }
                            else
                            {
                                bOvrwAppend = false;
                            }
                            xmlFree(ptext);
                        }
                    }
                    xmlXPathFreeObject (pPathObject);
                }
            }

            //COPY_DATA_2(&bOvrwAppend, pByteSrc, sizeof(bool));
            if(bOvrwAppend)
            {
                SET_MODE_APPEND(m_bExprnFlag_Disp);
            }
            else
            {
                SET_MODE_OVER(m_bExprnFlag_Disp);
            }

            //Reading Interpret Status if in overwrite Mode.
            //COPY_DATA_2(&m_bInterPretMsg, pByteSrc, sizeof(bool));
            sprintf_s(chMaxXpath, sizeof(chMaxXpath), def_XPATH_ISINTERPRET, strBusMsgWndName.c_str());
            pchPath = (xmlChar*)chMaxXpath;

            //pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/IsInterpret";
            pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
            if( nullptr != pPathObject )
            {
                xmlNodeSetPtr pIsHexPtr = pPathObject->nodesetval;

                if(nullptr != pIsHexPtr)
                {
                    for(int i=0; i < pIsHexPtr->nodeNr; i++)
                    {
                        xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pIsHexPtr->nodeTab[i]->xmlChildrenNode, 1);
                        if ( nullptr != ptext )
                        {
                            CString strIsInterpret = (char*)ptext;
                            if(strIsInterpret == "TRUE" || strIsInterpret == "1")
                            {
                                m_bInterPretMsg = TRUE;
                            }
                            else
                            {
                                m_bInterPretMsg = FALSE;
                            }
                            xmlFree(ptext);
                        }
                    }
                    xmlXPathFreeObject (pPathObject);
                }
            }

            if(m_bInterPretMsg)
            {
                SET_MODE_INTRP(m_bExprnFlag_Disp);
            }

            //Reading Time Display.
            BYTE byTimeDisplay = 0;
            sprintf_s(chMaxXpath, sizeof(chMaxXpath), def_XPATH_TIMEMODE, strBusMsgWndName.c_str());
            pchPath = (xmlChar*)chMaxXpath;

            //pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Time_Mode";
            pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);
            if( nullptr != pPathObject )
            {
                xmlNodeSetPtr pIsHexPtr = pPathObject->nodesetval;

                if(nullptr != pIsHexPtr)
                {
                    for(int i=0; i < pIsHexPtr->nodeNr; i++)
                    {
                        xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pIsHexPtr->nodeTab[i]->xmlChildrenNode, 1);
                        if ( nullptr != ptext )
                        {
                            CString strIsTimemode = (char*)ptext;
                            if(strIsTimemode == "ABSOLUTE")
                            {
                                byTimeDisplay = BIT_TM_ABS;
                            }
                            else if(strIsTimemode == "SYSTEM")
                            {
                                byTimeDisplay = BIT_TM_SYS;
                            }
                            else if(strIsTimemode == "RELATIVE")
                            {
                                byTimeDisplay = BIT_TM_REL;
                            }
                            xmlFree(ptext);
                        }
                    }
                    xmlXPathFreeObject (pPathObject);
                }
            }

            //COPY_DATA_2(&byTimeDisplay, pByteSrc, sizeof(bool));
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
            memset(&sMsgWndPlacement, -1, sizeof(sMsgWndPlacement));
            sprintf_s(chMaxXpath, sizeof(chMaxXpath), def_XPATH_WNDPOS, strBusMsgWndName.c_str());
            pchPath = (xmlChar*)chMaxXpath;
            //pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Window_Position";
            pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);

            xmlNodeSetPtr pWndPos = nullptr;

            if( nullptr != pPathObject )
            {
                pWndPos = pPathObject->nodesetval;
            }
            if(pWndPos != nullptr)
            {
                for(INT nIndex = 0; nIndex < pWndPos->nodeNr; nIndex++)
                {
                    xmlNodePtr pNodePtr = pWndPos->nodeTab[nIndex]->xmlChildrenNode;

                    while(pNodePtr != nullptr)
                    {
                        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Visibility")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString pvisibility = (char*)ptext;
                                sMsgWndPlacement.showCmd =  xmlUtils::nGetWindowVisibility(pvisibility.GetBuffer(pvisibility.GetLength()));

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"WindowPlacement")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString pWindowPlacement = (char*)ptext;
                                sMsgWndPlacement.flags =  xmlUtils::nGetWindowVisibility(pWindowPlacement.GetBuffer(pWindowPlacement.GetLength()));

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Top")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString strTop = (char*)ptext;
                                INT nTop = atoi(strTop);

                                sMsgWndPlacement.rcNormalPosition.top =  nTop;

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Left")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString strLeft = (char*)ptext;
                                INT nLeft = atoi(strLeft);

                                sMsgWndPlacement.rcNormalPosition.left =  nLeft;

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Bottom")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString strBttm = (char*)ptext;
                                INT nBttm = atoi(strBttm);

                                sMsgWndPlacement.rcNormalPosition.bottom =  nBttm;

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Right")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString strRight = (char*)ptext;
                                INT nRight = atoi(strRight);

                                sMsgWndPlacement.rcNormalPosition.right =  nRight;

                                xmlFree(ptext);
                            }
                        }

                        pNodePtr = pNodePtr->next;
                    }
                }
            }

            //COPY_DATA_2(&sMsgWndPlacement, pByteSrc, sizeof(WINDOWPLACEMENT));

            //Save the window visibility status.
            BOOL bVisible = IsWindowVisible();

            //SetWindowPlacement function call makes the window visible.
            SetWindowPlacement(&sMsgWndPlacement);

            //Regain the window's visibility status.
            if(!bVisible)
            {
                ShowWindow(SW_HIDE);
            }
            sprintf_s(chMaxXpath, sizeof(chMaxXpath), def_XPATH_INT_WND_POS, strBusMsgWndName.c_str());
            pchPath = (xmlChar*)chMaxXpath;

            //pchPath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Message_Window/Interpretation_Window_Position";
            pPathObject = xmlUtils::pGetNodes(m_xmlConfigFiledoc, pchPath);

            pWndPos = nullptr;

            if( nullptr != pPathObject )
            {
                pWndPos = pPathObject->nodesetval;
            }
            if(pWndPos != nullptr)
            {
                for(INT nIndex = 0; nIndex < pWndPos->nodeNr; nIndex++)
                {
                    xmlNodePtr pNodePtr = pWndPos->nodeTab[nIndex]->xmlChildrenNode;

                    while(pNodePtr != nullptr)
                    {
                        if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Visibility")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString stVisibilty = (char*)ptext;
                                m_sMsgIntrpWndPlacement.showCmd =  xmlUtils::nGetWindowVisibility(stVisibilty.GetBuffer(stVisibilty.GetLength()));

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"WindowPlacement")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString stVWindowPlacement = (char*)ptext;
                                m_sMsgIntrpWndPlacement.flags =  xmlUtils::nGetWindowVisibility(stVWindowPlacement.GetBuffer(stVWindowPlacement.GetLength()));

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Top")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString strTop = (char*)ptext;
                                INT nTop = atoi(strTop);

                                m_sMsgIntrpWndPlacement.rcNormalPosition.top =  nTop;

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Left")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString strLeft = (char*)ptext;
                                INT nLeft = atoi(strLeft);

                                m_sMsgIntrpWndPlacement.rcNormalPosition.left =  nLeft;

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Bottom")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString strBttm = (char*)ptext;
                                INT nBttm = atoi(strBttm);

                                m_sMsgIntrpWndPlacement.rcNormalPosition.bottom =  nBttm;

                                xmlFree(ptext);
                            }
                        }
                        else if((!xmlStrcmp(pNodePtr->name, (const xmlChar*)"Right")))
                        {
                            xmlChar* ptext = xmlNodeListGetString(m_xmlConfigFiledoc, pNodePtr->xmlChildrenNode, 1);
                            if(nullptr != ptext)
                            {
                                CString strRight = (char*)ptext;
                                INT nRight = atoi(strRight);

                                m_sMsgIntrpWndPlacement.rcNormalPosition.right =  nRight;

                                xmlFree(ptext);
                            }
                        }

                        pNodePtr = pNodePtr->next;
                    }
                }
            }
            //COPY_DATA_2(&m_sMsgIntrpWndPlacement, pByteSrc, sizeof(WINDOWPLACEMENT));
        }
        else
        {
            //load default values
            vSetDefaultConfigValues();
            vSetDefaultPlacement();
        }
    }
    else
    {
        //load default values
        vSetDefaultConfigValues();
        vSetDefaultPlacement();
    }
    vUpdatePtrInLstCtrl();
    m_lstMsg.vShowHideBlankcolumn(m_bInterPretMsg);

    //Clear all the previous messages.

    OnEditClearAll();

    return S_OK;
}
/*******************************************************************************
  Function Name  : GetConfigData
  Input(s)       : xmlNodePtr
  Output         : -
  Functionality  : Stores configuration to XML node
  Member of      : CMsgFrmtWnd
  Author(s)      : Ashwin R Uchil
  Date Created   : 3-8-2012
*******************************************************************************/
bool CMsgFrmtWnd::GetConfigData(xmlNodePtr pxmlNodePtr)
{
    CHeaderCtrl* pHeaderCtrl = m_lstMsg.GetHeaderCtrl();
    int  nColumnCount = pHeaderCtrl->GetItemCount();


    const char* omcVarChar ;

    TCHAR pcName[MAX_PATH]; // sufficient for now
    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_TEXT;
    lvColumn.pszText = (LPTSTR)pcName;
    lvColumn.cchTextMax = MAX_PATH;

    LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));
    if ( nullptr != pnOrder )
    {
        m_lstMsg.GetColumnOrderArray(pnOrder, nColumnCount);

        for (int i = 0 ; i < nColumnCount; i++)
        {
            //<COLUMN>
            xmlNodePtr pNodeColumn = xmlNewNode(nullptr, BAD_CAST DEF_COLUMN);
            xmlAddChild(pxmlNodePtr, pNodeColumn);

            //<ID>Column_Name</ID>
            m_lstMsg.GetColumn(i,&lvColumn);
            CString csColumnName;
            csColumnName.Format("%s", lvColumn.pszText);
            omcVarChar = csColumnName;
            csColumnName.TrimLeft();
            csColumnName.TrimRight();
            xmlNodePtr pColName = xmlNewChild(pNodeColumn, nullptr, BAD_CAST DEF_ID,BAD_CAST omcVarChar);
            xmlAddChild(pNodeColumn, pColName);

            //<Order>1</Order>
            CString csOrder;
            csOrder.Format("%d", (pnOrder[i] + 1));
            omcVarChar = csOrder;
            xmlNodePtr pOrder = xmlNewChild(pNodeColumn, nullptr, BAD_CAST DEF_MWND_ORDER,BAD_CAST omcVarChar);
            xmlAddChild(pNodeColumn, pOrder);

            //<Width> int </Width>
            CString csVisible;
            csVisible.Format("%d", m_lstMsg.IsColumnVisible(i));
            omcVarChar = csVisible;
            xmlNodePtr pVisisble = xmlNewChild(pNodeColumn, nullptr, BAD_CAST DEF_IS_VISIBLE,BAD_CAST omcVarChar);
            xmlAddChild(pNodeColumn, pVisisble);

            //<IsVisible>bool</IsVisible>
            CString csWidth;
            csWidth.Format("%d", m_lstMsg.GetColumnWidth(i));
            omcVarChar = csWidth;
            xmlNodePtr pWidth = xmlNewChild(pNodeColumn, nullptr, BAD_CAST DEF_MWND_COL_WIDTH,BAD_CAST omcVarChar);
            xmlAddChild(pNodeColumn, pWidth);
        }
        free(pnOrder);
    }
    CString csHex;
    csHex.Format("%d", IS_NUM_HEX_SET(m_bExprnFlag_Disp) > 0 ? true: false);
    omcVarChar = csHex;
    xmlNodePtr pHex = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_MWND_HEX,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pHex);

    CString csAppend;
    csAppend.Format("%d",  IS_MODE_APPEND(m_bExprnFlag_Disp) > 0 ? true: false);
    omcVarChar = csAppend;
    xmlNodePtr pAppend = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_MWND_APPEND,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pAppend);

    CString csInterpret;
    csInterpret.Format("%d",  IS_MODE_INTRP(m_bExprnFlag_Disp) > 0 ? true: false);
    omcVarChar = csInterpret;
    xmlNodePtr pInterpret = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_MWND_INTERPRET,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pInterpret);

    //Storing Time Display.
    CString csTimeDisplay;
    if(IS_TM_ABS_SET(m_bExprnFlag_Disp))
    {
        csTimeDisplay = "ABSOLUTE";
    }
    else if(IS_TM_REL_SET(m_bExprnFlag_Disp))
    {
        csTimeDisplay = "RELATIVE";
    }
    else if(IS_TM_SYS_SET(m_bExprnFlag_Disp))
    {
        csTimeDisplay = "SYSTEM";
    }
    omcVarChar = csTimeDisplay;
    xmlNodePtr pTimeDisplay = xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_MWND_TIME_MODE,BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pTimeDisplay);

    //window placement
    xmlNodePtr pNodeWindowsPos = xmlNewNode(nullptr, BAD_CAST DEF_WND_POS);
    xmlAddChild(pxmlNodePtr, pNodeWindowsPos);

    WINDOWPLACEMENT sMsgWndPlacement;
    GetWindowPlacement(&sMsgWndPlacement);

    xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWindowsPos, sMsgWndPlacement);

    //Interpretation_Window_Position-------------------------------------------------------
    xmlNodePtr pNodeInterpretationWndPos = xmlNewNode(nullptr, BAD_CAST DEF_INTPRET_WND_POS);
    xmlAddChild(pxmlNodePtr, pNodeInterpretationWndPos);

    m_podMsgIntprtnDlg->GetWindowPlacement(&m_sMsgIntrpWndPlacement);

    xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeInterpretationWndPos, m_sMsgIntrpWndPlacement);

    return TRUE;
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
HRESULT CMsgFrmtWnd::SetConfigData(BYTE* /*pvDataStream*/)
{
    //Maintained to load default configration.

    //load default values
    vSetDefaultConfigValues();
    vSetDefaultPlacement();

    vUpdatePtrInLstCtrl();
    m_lstMsg.vShowHideBlankcolumn(m_bInterPretMsg);

    //Clear all the previous messages.
    OnEditClearAll();

    return S_OK;
}

HRESULT CMsgFrmtWnd::SetConfigData(xmlNodePtr pNode)
{
    int         nColCnt =0;
    xmlNodePtr pCopyNodeptr = nullptr;
    xmlNodePtr pChildNode = nullptr;
    int nIndividualCol = -1;
    std::string   strVar;

    pCopyNodeptr = pNode;
    //get the column count
    while(pCopyNodeptr)
    {
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_COLUMN)))
        {
            nColCnt++;
        }
    }

    LPINT pnOrder = (LPINT) malloc(nColCnt *sizeof(int));
    while(pNode)
    {
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_COLUMN)))
        {
            nIndividualCol++;
            pChildNode = pNode->children;

            if (xmlUtils::GetDataFrmNode(pChildNode,DEF_ID,strVar))
            {
                pnOrder[nIndividualCol] = (atoi(strVar.c_str()) != 0);
            }
            if (xmlUtils::GetDataFrmNode(pChildNode,DEF_IS_VISIBLE,strVar))
            {
                if(strVar == "TRUE")
                {
                    m_lstMsg.ShowColumn(nIndividualCol, true);
                }
                else if(strVar == "FALSE")
                {
                    m_lstMsg.ShowColumn(nIndividualCol, false);
                }
            }
            if (xmlUtils::GetDataFrmNode(pChildNode,DEF_MWND_COL_WIDTH,strVar))
            {
                int nColWidth = atoi(strVar.c_str());
                m_lstMsg.SetColumnWidth(nIndividualCol, nColWidth);
            }
        }

        // commented as IsHex is conflicting with global
        // DisplayNumericMode
        /*if (xmlUtils::GetDataFrmNode(pChildNode,DEF_MWND_HEX,strVar))
        {
            if(strVar == "TRUE")
            {
                SET_NUM_HEX(m_bExprnFlag_Disp);
            }
            else if(strVar == "FALSE")
            {
                SET_NUM_DEC(m_bExprnFlag_Disp);
            }
        }*/

        if  (xmlUtils::GetDataFrmNode(pChildNode,DEF_MWND_APPEND,strVar))
        {
            if(strVar == "TRUE")
            {
                SET_MODE_APPEND(m_bExprnFlag_Disp);
            }
            else if(strVar == "FALSE")
            {
                SET_MODE_OVER(m_bExprnFlag_Disp);
            }
        }

        if  (xmlUtils::GetDataFrmNode(pChildNode,DEF_MWND_INTERPRET,strVar))
        {
            if(strVar == "TRUE")
            {
                SET_MODE_INTRP(m_bExprnFlag_Disp);
            }
        }

        if  (xmlUtils::GetDataFrmNode(pChildNode,DEF_MWND_TIME_MODE,strVar))
        {
            CLEAR_EXPR_TM_BITS(m_bExprnFlag_Disp);

            eTimerMode eTimer = xmlUtils::eGetTimerMode(strVar);
            if(eTimer == TIME_MODE_ABSOLUTE)
            {
                SET_TM_ABS(m_bExprnFlag_Disp);
            }
            else if(eTimer == TIME_MODE_RELATIVE)
            {
                SET_TM_REL(m_bExprnFlag_Disp);
            }
            else if(eTimer == TIME_MODE_SYSTEM)
            {
                SET_TM_SYS(m_bExprnFlag_Disp);
            }
        }

        WINDOWPLACEMENT sMsgWndPlacement;
        if (!xmlStrcmp(pNode->name, (const xmlChar*)DEF_WND_POS))
        {
            xmlUtils::ParseWindowsPlacement(pNode,sMsgWndPlacement);
        }
        //Save the window visibility status.
        BOOL bVisible = IsWindowVisible();
        SetWindowPlacement(&sMsgWndPlacement);
        //Regain the window's visibility status.
        if(!bVisible)
        {
            ShowWindow(SW_HIDE);
        }

        if (!xmlStrcmp(pNode->name, (const xmlChar*)DEF_INTPRET_WND_POS))
        {
            xmlUtils::ParseWindowsPlacement(pNode,m_sMsgIntrpWndPlacement);
        }
        pNode = pNode->children;

    }

    if(pnOrder != nullptr)
    {
        free(pnOrder);
        pnOrder = nullptr;
    }

    if(!pNode)
    {
        //load default values
        vSetDefaultConfigValues();
        vSetDefaultPlacement();
    }
    vUpdatePtrInLstCtrl();
    m_lstMsg.vShowHideBlankcolumn(m_bInterPretMsg);

    //Clear all the previous messages.
    OnEditClearAll();
    return true;
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
    ::GetWindowRect(m_hMainWnd, &omRect);
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
    if (pHeaderCtrl != nullptr)
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


void CMsgFrmtWnd::vSetClientID(DWORD dwClientID)
{
    m_dwClientID = dwClientID;
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

    if (strClassName=="SysListView32")
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

    CString msgName;
    if(bIsDBAssociate)//If DB is associated
    {
        //Keep the key of map
        __int64 n64Temp;
        SMSGDISPMAPENTRY sEntry;
        // Now shift the elements down Shift the map
        POSITION pos = m_omMsgDispMap.GetStartPosition();
        while (pos != nullptr)
        {
            m_omMsgDispMap.GetNextAssoc(pos, n64Temp, sEntry);
            //If entry is expandable and not already expanded then expand

            //nMsgCode = nGetCodefromMapKey(n64Temp);

            if (S_OK == m_pouMsgContainerIntrf->IsInterpretable(m_eBusType, m_pBmNetWork, n64Temp))
            {
                //Those Messages which were non intepretable and Mode None may turn intepretable
                //on associating db, which can have those msg ids.
                //Those Msgs which wer in INTERPRETING and INTEPRETABLE state remains in same state.
                if (NON_INTERPRETABLE == sEntry.m_eInterpretMode || MODE_NONE == sEntry.m_eInterpretMode)
                {
                    sEntry.m_eInterpretMode = INTERPRETABLE;
                }
            }
            m_omMsgDispMap[n64Temp] = sEntry;
        }
        m_lstMsg.vShowHideBlankcolumn( m_bInterPretMsg );
    }
    else //If DB is dissociated
    {
        //Keep the key of map
        __int64 n64Temp;
        SMSGDISPMAPENTRY sEntry;
        // Now shift the elements down Shift the map
        POSITION pos = m_omMsgDispMap.GetStartPosition();
        while (pos != nullptr)
        {
            m_omMsgDispMap.GetNextAssoc(pos, n64Temp, sEntry);

            if (S_OK == m_pouMsgContainerIntrf->IsInterpretable(m_eBusType, m_pBmNetWork, n64Temp))
            {
                continue;
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
                    vContractMsgEntry(sEntry, nMsgListIndex);
                    m_omMsgDispMap[n64Temp] = sEntry;
                }
            }
            else
            {
                sEntry.m_eInterpretMode = NON_INTERPRETABLE;
                m_omMsgDispMap[n64Temp] = sEntry;
            }
        }

        m_lstMsg.vShowHideBlankcolumn(m_bInterPretMsg);
    }
    m_lstMsg.Invalidate();
    return 0;
}
/*******************************************************************************
  Function Name  : OnImportLogPageNoChange
  Input(s)       : Page no
  Output         : LRESULT
  Functionality  : Loads the requested page
  Member of      : CMsgFrmtWnd
  Author(s)      : Robin G.K.
  Date Created   : 10-06-2015
  Modifications  :
*******************************************************************************/

void CMsgFrmtWnd::OnImportLogPageNoChange ()
{
    HRESULT hResult = S_FALSE;
    CString omStrPageNo;
    m_omSliderMsgs.SetPos(defDEFAULT_PAGE_LENGTH - 1);
    m_omEditPageNo.GetWindowText(omStrPageNo);
    unsigned long nPageNo = (unsigned long)atoi(omStrPageNo);
    nPageNo = nPageNo-1;
    if(nPageNo>=0 && m_pouMsgContainerIntrf!=nullptr)
    {
        m_bUnloadOnClear = false;
        //Clear Message Window.
        OnEditClearAll();

        // Nullcheck added as it is getting updated in OnEditClearAll()
        if(nullptr != m_pouMsgContainerIntrf)
        {
            hResult = m_pouMsgContainerIntrf->LoadPage(nPageNo);
        }
        m_bUnloadOnClear = true;
    }
}

void CMsgFrmtWnd::OnUpdateImportLogOverwriteScroll()
{
    if (m_omSliderMsgs.GetSafeHwnd() == nullptr)
    {
        return;
    }
    if (IS_MODE_APPEND(m_bExprnFlag_Disp) == FALSE)
    {
        m_omSliderMsgs.EnableWindow(TRUE);
    }
    else
    {
        m_omSliderMsgs.EnableWindow(FALSE);
    }
}
/*******************************************************************************
  Function Name  : OnImportLogOverwriteScroll
  Input(s)       : Line No
  Output         : LRESULT
  Functionality  : Scroll to required overwrite msg.
  Member of      : CMsgFrmtWnd
  Author(s)      : Robin G.K.
  Date Created   : 10-06-2015
  Modifications  :
*******************************************************************************/
LRESULT CMsgFrmtWnd::OnImportLogOverwriteScroll(WPARAM wParam, LPARAM /*lParam*/)
{
    HRESULT hResult = S_FALSE;
    unsigned long nLineNo = (unsigned long)wParam;
    if(nLineNo>=0&&m_pouMsgContainerIntrf!=nullptr)
    {
        m_bUnloadOnClear = false;
        //Clear Message Window.
        OnEditClearAll();

        // Nullcheck added as it is getting updated in OnEditClearAll()
        if(nullptr != m_pouMsgContainerIntrf)
        {
            hResult = m_pouMsgContainerIntrf->OverwritePage(nLineNo);
        }
        m_bUnloadOnClear = true;
    }
    return hResult;
}
void CMsgFrmtWnd::vSetDefaultWindowPosition(CRect omRect)
{
    m_omDefaultRect = omRect;
}

void CMsgFrmtWnd::vSetDefaultPlacement()
{
    //Update the default window co-ordinates
    SetWindowPos(&wndTop, m_omDefaultRect.left, m_omDefaultRect.top, m_omDefaultRect.Width(),
                 m_omDefaultRect.Height(), SWP_NOZORDER);
}

