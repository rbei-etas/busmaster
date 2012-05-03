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
 * \file      RxMsgList.cpp
 * \brief     Implementation of CRxMsgList class, To customize the receive
 * \authors   Ravi. D, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CRxMsgList class, To customize the receive
 */

#include "StdAfx.h"
#include "BUSMASTER.h"
#include "RxMsgList.h"
#include "Resource.h"
#include "Common.h"
#include "hashdefines.h"
#include ".\rxmsglist.h"

//const int BUFFER_LENGTH_OVERWRITE       = 2000;
//#define def_MSG_BUFFER_LENGTH             2000
const int nMAX_PSZTEXT_SIZE               = 260;

extern CCANMonitorApp theApp;

/******************************************************************************
Function Name   :   CRxMsgList
Input(s)        :   -
Output          :   -
Functionality   :   constructor
Member of       :   CRxMsgList
Friend of       :   -
Authors         :   Ravi D
Date Created    :   19/09/2006
Modifications   :   -
******************************************************************************/
CRxMsgList::CRxMsgList()
{
    m_bConnected = FALSE;
    m_nSortedColumn = 1;
    m_pbSortableColumn = NULL;
    m_pbAscendingOrder = NULL;
    m_lClkPoint = CPoint(0,0);
    m_omNewFont.CreateFont(10, 0,
                           DEFAULT_FONT_ESCAPEMENT,
                           DEFAULT_FONT_ORIENTATION,
                           FW_NORMAL,
                           NOT_ITALIC,
                           NO_UNDERLINE,
                           NO_STRIKEOUT,
                           DEFAULT_CHARSET,
                           OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS,
                           DEFAULT_QUALITY,
                           FIXED_PITCH | FF_MODERN,
                           _T("Courier"));
}

/**********************************************************************************
Function Name   :   ~CRxMsgList
Input(s)        :   -
Output          :   -
Functionality   :   Destructor
Member of       :   CRxMsgList
Friend of       :   -
Authors         :   Ravi D
Date Created    :   19/09/2006
Modifications   :   -
************************************************************************************/
CRxMsgList::~CRxMsgList(void)
{
    free(m_pbSortableColumn);
    free(m_pbAscendingOrder);
}

BEGIN_MESSAGE_MAP(CRxMsgList, CFlickerFreeListCtrl)
    ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
    ON_WM_DESTROY()
    ON_NOTIFY(HDN_ITEMCHANGEDA, 0, OnHdnItemchanged)
    ON_NOTIFY(HDN_ITEMCHANGEDW, 0, OnHdnItemchanged)
    ON_WM_VSCROLL()
    ON_WM_LBUTTONDBLCLK()
    ON_MESSAGE(LVM_DELETECOLUMN, OnDeleteColumn)
    ON_MESSAGE(LVM_INSERTCOLUMN, OnInsertColumn)
    ON_MESSAGE(LVM_SETCOLUMNWIDTH, OnSetColumnWidth)
    ON_NOTIFY_EX(HDN_BEGINTRACKA, 0, OnHeaderBeginResize)
    ON_NOTIFY_EX(HDN_BEGINTRACKW, 0, OnHeaderBeginResize)
    ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
    ON_NOTIFY_EX(HDN_DIVIDERDBLCLICKA, 0, OnHeaderDividerDblClick)
    ON_NOTIFY_EX(HDN_DIVIDERDBLCLICKW, 0, OnHeaderDividerDblClick)
    ON_WM_CONTEXTMENU() // OnContextMenu
    ON_WM_KEYDOWN()     // OnKeyDown
    ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHdnItemclick)
    ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHdnItemclick)
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/**********************************************************************************
Function Name   :   OnLvnGetdispinfo
Input(s)        :   pNMHDR - pointer to the structure
                    pResult -
Output          :   -
Functionality   :   to retrieve item data stored by the application
Member of       :   CRxMsgList
Friend of       :   -
Authors         :   Ravi D
Date Created    :   19/09/2006
Modifications   :   Anish Kumar,02.11.09, Modified to make it Msg type independent
************************************************************************************/
void CRxMsgList::OnLvnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    LV_ITEM* pItem= &(pDispInfo->item);
    static SWMUPDATEPTRPARA sParam;

    if ( (0 == pItem->iSubItem) && (pItem->mask & LVIF_TEXT))
    {
        sParam.m_nListIndex  = pItem->iItem;
        sParam.m_eInPretMode = MODE_NONE;
        sParam.m_bResult     = FALSE;
        //For any row the update should happen only once when this fn is called
        //for first sub item
        ::SendMessage( m_hParent, WM_PARENT_UPDATE_MSGPTR, (WPARAM)&sParam, 0);
    }

    if (sParam.m_bResult != FALSE)
    {
        //SetItemData(iItemIndx,lst.m_ColourCode);
        if ((pItem->mask & LVIF_TEXT) && (pItem->iSubItem!= 0 && (NULL != m_pomDataPtrArr[pItem->iSubItem-1] ))) //valid text buffer?
        {
            //pItem->pszText can not display more than 260 chars
            //when copied more than 260 chars the application was getting closed abruptly
            CString str(m_pomDataPtrArr[pItem->iSubItem-1]);

            if (nMAX_PSZTEXT_SIZE >= str.GetLength())
            {
                strcpy(pItem->pszText, m_pomDataPtrArr[pItem->iSubItem-1]);
            }
            else
            {
                _tcsncpy_s(pItem->pszText, sizeof(pItem->pszText), m_pomDataPtrArr[pItem->iSubItem-1], nMAX_PSZTEXT_SIZE);
            }
        }

        if ((pItem->mask & LVIF_IMAGE))
        {
            BOOL bInterpret = FALSE;
            ::SendMessage( m_hParent, WM_GET_INTERPRET_STATE, (WPARAM)&bInterpret, 0);

            if(!bInterpret)
            {
                return;
            }

            if (sParam.m_eInPretMode  == INTERPRETABLE)
            {
                pItem->iImage = m_nIconPlus;
            }
            else if (sParam.m_eInPretMode  == INTERPRETING)
            {
                pItem->iImage = m_nIconMinus;
            }
            else
            {
                pItem->iImage = m_nIconBlank;
            }
        }
    }

    *pResult = 0;
}

/*******************************************************************************
  Function Name  : vShowHideBlankcolumn
  Input(s)       : bInterpretON
  Output         : -
  Functionality  : Shows/Hides the Blank Column based on the input 'bInterpretON'.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::vShowHideBlankcolumn(BOOL bInterpretON)
{
    if(bInterpretON)
    {
        ShowColumn(0,TRUE);
    }
    else if(IsColumnVisible(0) )
    {
        ShowColumn(0,FALSE);
    }
}

/**********************************************************************************
Function Name   :   OnNMCustomdraw
Input(s)        :   pNMHDR - structure containing information about the drawing operation
                    pResult -
Output          :   -
Functionality   :   to custom draw
Member of       :   CRxMsgList
Authors         :   Ravi D
Date Created    :   19/09/2006
Modifications   :   -
************************************************************************************/
void CRxMsgList::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

    if (pNMCD->nmcd.dwDrawStage == CDDS_PREPAINT)
    {
        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
    {
        ::SendMessage( m_hParent, WM_PARENT_UPDATE_MSG_CLR, (WPARAM)pNMCD->nmcd.dwItemSpec, 0);

        if(m_clrMsg)
        {
            pNMCD->clrText = m_clrMsg;
        }

        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
    {
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == (CDDS_ITEMPOSTPAINT | CDDS_SUBITEM))
    {
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT)
    {
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPREERASE)
    {
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPOSTERASE)
    {
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_SUBITEM )
    {
        *pResult = CDRF_DODEFAULT;
    }
    else
    {
        *pResult = CDRF_DODEFAULT;
    }
}

/*******************************************************************************
  Function Name  : vSetMsgColor
  Input(s)       : COLORREF
  Output         : -
  Functionality  : Sets the member variable m_clrMsg to input COLORREF.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::vSetMsgColor(COLORREF color)
{
    m_clrMsg = color;
}

/**********************************************************************************
Function Name   :   vSetIsAppend
Input(s)        :
Output          :   -
Functionality   :   to change m_bIsAppend as user's choice
Member of       :   CRxMsgList
Authors         :   Anish kr
Date Created    :   19/07/2007
Modifications   :   -
************************************************************************************/
void CRxMsgList::OnNMDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Send corresponding message to parent window with current selection
    INT nItem = GetNextItem(-1, LVNI_SELECTED);

    if (nItem != -1)
    {
        GetParent()->SendMessage(WM_LSTC_DBLCLK, nItem,(LPARAM)&m_lClkPoint);
    }

    *pResult = 0;
}

/*******************************************************************************
  Function Name  : vDoInitialization
  Input(s)       : -
  Output         : -
  Functionality  : Initialize the Image List with Plus, Minus an Blank icons.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::vDoInitialization()
{
    m_ImageList.Create(16, 16, TRUE, 3, 1);
    m_nIconMinus  = m_ImageList.Add((AfxGetApp()->LoadIcon(IDI_ICON_MINUS)));
    m_nIconPlus   = m_ImageList.Add((AfxGetApp()->LoadIcon(IDI_ICON_PLUS)));
    m_nIconBlank  = m_ImageList.Add((AfxGetApp()->LoadIcon(IDI_ICON_BLANK)));
    SetImageList(&m_ImageList, LVSIL_SMALL);
    CWnd* pWnd = GetParent();

    if (NULL != pWnd)
    {
        m_hParent = pWnd->m_hWnd;
    }

    CRxMsgList::SetFont(&m_omNewFont, TRUE);
}

/*******************************************************************************
  Function Name  : OnDestroy
  Input(s)       : -
  Output         : -
  Functionality  : Call the OnDestroy() of Base class.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::OnDestroy()
{
    CFlickerFreeListCtrl::OnDestroy();
}

/*******************************************************************************
  Function Name  : OnHdnItemchanged
  Input(s)       : NMHDR*, LRESULT*
  Output         : -
  Functionality  : Handles Notification event HDN_ITEMCHANGEDA.
                   Redraws Window on Item Change.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::OnHdnItemchanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    //LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    RedrawWindow();
    *pResult = 0;
}

UINT UpDateThread(LPVOID lParams)
{
    CRxMsgList* pList = static_cast<CRxMsgList*> (lParams);

    for(int i =0 ; i<3; i++)
    {
        Sleep(50);
        ::SendMessage( pList->m_hParent, WM_UPDATE_TREE_ITEMS_POS, 0, 0);
        pList->RedrawWindow();
    }

    return 0;
}

/*******************************************************************************
  Function Name  : OnVScroll
  Input(s)       : nSBCode, nPos, pScrollBar
  Output         : -
  Functionality  : Handles the Vertical scrolling event.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    BOOL bInterpret = FALSE;
    ::SendMessage( m_hParent, WM_GET_INTERPRET_STATE, (WPARAM)&bInterpret, 0);

    if ( bInterpret && theApp.pouGetFlagsPtr()->nGetFlagStatus( SENDMESG ) )
    {
        AfxBeginThread(UpDateThread, (LPVOID)this);
        CFlickerFreeListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
        return;
    }

    ::SendMessage( m_hParent, WM_UPDATE_TREE_ITEMS_POS, 0, 0);
    RedrawWindow();
    CFlickerFreeListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

/*******************************************************************************
  Function Name  : OnMouseWheel
  Input(s)       : nFlags, zDelta, pt
  Output         : -
  Functionality  : Handles the Mouse wheel scrolling event.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 09-09-2010
  Modifications  :
*******************************************************************************/
BOOL CRxMsgList::OnMouseWheel(UINT /*nFlags*/, short /*zDelta*/, CPoint /*pt*/)
{
    BOOL bInterpret = FALSE;
    ::SendMessage( m_hParent, WM_GET_INTERPRET_STATE, (WPARAM)&bInterpret, 0);

    if ( bInterpret )
    {
        AfxBeginThread(UpDateThread, (LPVOID)this);
    }

    return FALSE;
    //return CFlickerFreeListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

/*******************************************************************************
  Function Name  : OnLButtonDblClk
  Input(s)       : nFlags, point
  Output         : -
  Functionality  : Handles the LButtonDblClk event.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // Send corresponding message to parent window with current selection
    INT nItem = GetNextItem(-1, LVNI_SELECTED);

    if (nItem != -1)
    {
        GetParent()->SendMessage(WM_LSTC_DBLCLK, nItem,(LPARAM)&point);
    }

    m_lClkPoint = point;
    CListCtrl::OnLButtonDblClk(nFlags, point);
    CFlickerFreeListCtrl::OnLButtonDblClk(nFlags, point);
}

/*******************************************************************************
  Function Name  : vSetDsipItemDataPtrArr
  Input(s)       : char* pomDataPtrArr[]
  Output         : -
  Functionality  : Sets the member variable m_pomDataPtrArr to PSDI_CAN DLL
                   message data buffer pointer location.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::vSetDsipItemDataPtrArr(char* pomDataPtrArr[MAX_MSG_WND_COL_CNT])
{
    memcpy(m_pomDataPtrArr, pomDataPtrArr, sizeof (m_pomDataPtrArr));
}

/*******************************************************************************
  Function Name  : GetFirstVisibleColumn
  Input(s)       : -
  Output         : int
  Functionality  : Returns an integer valuie of the index of first visible column.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
int CRxMsgList::GetFirstVisibleColumn()
{
    int nColCount = GetHeaderCtrl()->GetItemCount();

    for(int i = 0; i < nColCount; ++i)
    {
        int nCol = GetHeaderCtrl()->OrderToIndex(i);

        if (IsColumnVisible(nCol))
        {
            return nCol;
        }
    }

    return -1;
}

/*******************************************************************************
  Function Name  : ShowColumn
  Input(s)       : nCol, bShow
  Output         : BOOL
  Functionality  : Shows/Hides a Column nCol based on boolean bShow.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
BOOL CRxMsgList::ShowColumn(int nCol, bool bShow)
{
    SetRedraw(FALSE);
    ColumnState& columnState = GetColumnState(nCol);
    int nColCount = GetHeaderCtrl()->GetItemCount();
    int* pOrderArray = new int[nColCount];
    VERIFY( GetColumnOrderArray(pOrderArray, nColCount) );

    if (bShow)
    {
        // Restore the position of the column
        int nCurIndex = -1;

        for(int i = 0; i < nColCount ; ++i)
        {
            if (pOrderArray[i]==nCol)
            {
                nCurIndex = i;
            }
            else if (nCurIndex!=-1)
            {
                // We want to move it to the original position,
                // and after the last hidden column
                if ( (i <= columnState.m_OrgPosition)
                        || !IsColumnVisible(pOrderArray[i])
                   )
                {
                    pOrderArray[nCurIndex] = pOrderArray[i];
                    pOrderArray[i] = nCol;
                    nCurIndex = i;
                }
            }
        }
    }
    else
    {
        // Move the column to the front of the display order list
        int nCurIndex(-1);

        for(int i = nColCount-1; i >=0 ; --i)
        {
            if (pOrderArray[i]==nCol)
            {
                // Backup the current position of the column
                columnState.m_OrgPosition = i;
                nCurIndex = i;
            }
            else if (nCurIndex!=-1)
            {
                pOrderArray[nCurIndex] = pOrderArray[i];
                pOrderArray[i] = nCol;
                nCurIndex = i;
            }
        }
    }

    VERIFY( SetColumnOrderArray(nColCount, pOrderArray) );
    delete [] pOrderArray;

    if (bShow)
    {
        // Restore the column width
        columnState.m_Visible = true;
        VERIFY( SetColumnWidth(nCol, columnState.m_OrgWidth) );
    }
    else
    {
        // Backup the column width
        if(GetColumnWidth(nCol) == 0)
        {
            columnState.m_Visible = false;
        }
        else
        {
            int orgWidth = GetColumnWidth(nCol);
            VERIFY( SetColumnWidth(nCol, 0) );
            columnState.m_Visible = false;
            columnState.m_OrgWidth = orgWidth;
        }
    }

    SetRedraw(TRUE);
    Invalidate(FALSE);
    return TRUE;
}

/*******************************************************************************
  Function Name  : SetColumnWidthAuto
  Input(s)       : nCol, includeHeader
  Output         : BOOL
  Functionality  : Sets the column width of nCol to automatic.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
BOOL CRxMsgList::SetColumnWidthAuto(int nCol, bool includeHeader)
{
    if (nCol == -1)
    {
        for(int i = 0; i < GetHeaderCtrl()->GetItemCount() ; ++i)
        {
            SetColumnWidthAuto(i, includeHeader);
        }

        return TRUE;
    }
    else
    {
        if (includeHeader)
        {
            return SetColumnWidth(nCol, LVSCW_AUTOSIZE_USEHEADER);
        }
        else
        {
            return SetColumnWidth(nCol, LVSCW_AUTOSIZE);
        }
    }
}

/*******************************************************************************
  Function Name  : GetColumnState
  Input(s)       : nCol
  Output         : ColumnState&
  Functionality  : Returns the Column State of nCol as ColumnState&
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
CRxMsgList::ColumnState& CRxMsgList::GetColumnState(int nCol)
{
    VERIFY( nCol >=0 && nCol < m_ColumnStates.GetSize() );
    return m_ColumnStates[nCol];
}

/*******************************************************************************
  Function Name  : IsColumnVisible
  Input(s)       : nCol
  Output         : bool
  Functionality  : Returns a bool value indicating ncol's visibility.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
bool CRxMsgList::IsColumnVisible(int nCol)
{
    return GetColumnState(nCol).m_Visible;
}

/*******************************************************************************
  Function Name  : GetColumnStateCount
  Input(s)       : -
  Output         : int
  Functionality  : Returns count of items in m_ColumnStates.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
int CRxMsgList::GetColumnStateCount()
{
    return m_ColumnStates.GetSize();
}

/*******************************************************************************
  Function Name  : InsertColumnState
  Input(s)       : nCol, bVisible, nOrgWidth
  Output         : -
  Functionality  : Adds new column state to m_ColumnStates array.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::InsertColumnState(int nCol, bool bVisible, int nOrgWidth)
{
    VERIFY( nCol >=0 && nCol <= m_ColumnStates.GetSize() );
    ColumnState columnState;
    columnState.m_OrgWidth = nOrgWidth;
    columnState.m_Visible = bVisible;

    if (nCol == m_ColumnStates.GetSize())
    {
        // Append column picker to the end of the array
        m_ColumnStates.Add(columnState);
    }
    else
    {
        m_ColumnStates.InsertAt(nCol, columnState);
        // Insert column in the middle of the array
        //CArray<ColumnState, ColumnState> newArray;
        //for(int i=0 ; i < m_ColumnStates.GetSize(); ++i)
        //{
        //  if (i == nCol)
        //      newArray.Add(columnState);
        //  newArray.Add(m_ColumnStates[i]);
        //}
        //m_ColumnStates = newArray;
    }
}

/*******************************************************************************
  Function Name  : DeleteColumnState
  Input(s)       : nCol
  Output         : -
  Functionality  : Removes column state of nCol from m_ColumnStates array.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::DeleteColumnState(int nCol)
{
    VERIFY( nCol >=0 && nCol < m_ColumnStates.GetSize() );
    m_ColumnStates.RemoveAt(nCol);
}

/*******************************************************************************
  Function Name  : OnContextMenu
  Input(s)       : pWnd, point
  Output         : -
  Functionality  : Show the context menu with a list of column names as menu items.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
    if (point.x==-1 && point.y==-1)
    {
        // OBS! point is initialized to (-1,-1) if using SHIFT+F10 or VK_APPS
    }
    else
    {
        CPoint pt = point;
        ScreenToClient(&pt);
        CRect headerRect;
        GetHeaderCtrl()->GetClientRect(&headerRect);

        if (headerRect.PtInRect(pt))
        {
            // Show context-menu with the option to show hide columns
            CMenu menu;

            if (menu.CreatePopupMenu())
            {
                for( int i = GetColumnStateCount()-1 ; i >= 0; --i)
                {
                    UINT uFlags = MF_BYPOSITION | MF_STRING;

                    // Put check-box on context-menu
                    if (IsColumnVisible(i))
                    {
                        uFlags |= MF_CHECKED;
                    }
                    else
                    {
                        uFlags |= MF_UNCHECKED;
                    }

                    if(i == 0)      //Disable 'Time' column
                    {
                        break;
                    }

                    // Retrieve column-title
                    LVCOLUMN lvc = {0};
                    lvc.mask = LVCF_TEXT;
                    char sColText[256];
                    lvc.pszText = sColText;
                    lvc.cchTextMax = 15;    //Set the Width of Menu Items here
                    //lvc.cchTextMax = sizeof(sColText)-1;
                    VERIFY( GetColumn(i, &lvc) );
                    menu.InsertMenu(0, uFlags, i, lvc.pszText);
                }

                menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, 0);
            }
        }
    }
}

/*******************************************************************************
  Function Name  : OnCommand
  Input(s)       : wParam, lParam
  Output         : BOOL
  Functionality  : Toggle the visible state of Column.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
// Handle context-menu event for showing / hiding columns
BOOL CRxMsgList::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
    if (HIWORD(wParam) == 0)
    {
        int nCol = LOWORD(wParam);
        ShowColumn(nCol, !IsColumnVisible(nCol));
    }

    return TRUE;
}

/*******************************************************************************
  Function Name  : OnKeyDown
  Input(s)       : nChar, nRepCnt, nFlags
  Output         : -
  Functionality  : Handle the case of VK_ADD CTRL + NumPlus (Auto size all columns).
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  : 14-09-2010, ArunKumar K,
                   Added new Handlers for VK_PRIOR, VK_NEXT, VK_END, VK_HOME.
                   15-07-2010, ArunKumar K,
                   Added new Handlers for VK_UP, VK_DOWN.
                   13-07-2010, ArunKumar K,
                   Added new Handlers for VK_LEFT, VK_RIGHT, VK_RETURN.
*******************************************************************************/
void CRxMsgList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch(nChar)
    {
        case VK_ADD:    // CTRL + NumPlus (Auto size all columns)
        {
            if (GetKeyState(VK_CONTROL) < 0)
            {
                // Special handling to avoid showing "hidden" columns
                SetColumnWidthAuto(-1);
                return;
            }
        }
        break;

        case VK_UP:
        {
            BOOL bInterpret = FALSE;
            ::SendMessage( m_hParent, WM_GET_INTERPRET_STATE, (WPARAM)&bInterpret, 0);

            if(bInterpret)
            {
                INT nItem = GetNextItem(-1, LVNI_SELECTED);

                if (nItem != -1)
                {
                    GetParent()->SendMessage(WM_GET_NEXT_PREV_MSG_INDEX, (WPARAM)false, (LPARAM)&nItem);
                    SetItemState(nItem-1,LVIS_SELECTED , LVIS_SELECTED );
                    return;
                }
            }

            break;
        }

        case VK_DOWN:
        {
            BOOL bInterpret = FALSE;
            ::SendMessage( m_hParent, WM_GET_INTERPRET_STATE, (WPARAM)&bInterpret, 0);

            if(bInterpret)
            {
                INT nItem = GetNextItem(-1, LVNI_SELECTED);

                if (nItem != -1)
                {
                    GetParent()->SendMessage(WM_GET_NEXT_PREV_MSG_INDEX, (WPARAM)true, (LPARAM)&nItem);
                    SetItemState(nItem+1,LVIS_SELECTED , LVIS_SELECTED );
                    return;
                }
            }

            break;
        }

        case VK_LEFT:
        {
            INT nItem = GetNextItem(-1, LVNI_SELECTED);

            if (nItem != -1)
            {
                GetParent()->SendMessage(WM_EXPAND_COLLAPSE_MSG, nItem, TREESTATE_COLLAPSE);
                SetItemState(nItem,LVIS_SELECTED , LVIS_SELECTED );
                return;
            }

            break;
        }

        case VK_RIGHT:
        {
            INT nItem = GetNextItem(-1, LVNI_SELECTED);

            if (nItem != -1)
            {
                GetParent()->SendMessage(WM_EXPAND_COLLAPSE_MSG, nItem, TREESTATE_EXPAND);
                SetItemState(nItem,LVIS_SELECTED , LVIS_SELECTED );
                return;
            }

            break;
        }

        case VK_RETURN:
        {
            INT nItem = GetNextItem(-1, LVNI_SELECTED);

            if (nItem != -1)
            {
                GetParent()->SendMessage(WM_EXPAND_COLLAPSE_MSG, nItem, TREESTATE_TOGGLE);
            }

            break;
        }

        case VK_PRIOR:
        case VK_NEXT:
        case VK_END:
        case VK_HOME:
        {
            BOOL bInterpret = FALSE;
            ::SendMessage( m_hParent, WM_GET_INTERPRET_STATE, (WPARAM)&bInterpret, 0);

            if ( bInterpret )
            {
                AfxBeginThread(UpDateThread, (LPVOID)this);
            }
        }
    }

    CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

/*******************************************************************************
  Function Name  : OnHeaderBeginResize
  Input(s)       : pNMHDR, pResult
  Output         : BOOL
  Functionality  : Handle the event for blocking resize of hidden columns.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
BOOL CRxMsgList::OnHeaderBeginResize(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
    // Check that column is allowed to be resized
    NMHEADER* pNMH = (NMHEADER*)pNMHDR;
    int nCol = (int)pNMH->iItem;

    if (!IsColumnVisible(nCol) || nCol == 0) //If Column is not visible or if it is Zeroth Column
    {
        *pResult = TRUE;    // Block resize
        return TRUE;        // Block event
    }

    return FALSE;
}

/*******************************************************************************
  Function Name  : OnSetColumnWidth
  Input(s)       : wParam, lParam
  Output         : LRESULT
  Functionality  : Handle the event for checking whether column is allowed to resize.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CRxMsgList::OnSetColumnWidth(WPARAM wParam, LPARAM lParam)
{
    // Check that column is allowed to be resized
    int nCol = (int)wParam;

    if (!IsColumnVisible(nCol))
    {
        return FALSE;
    }

    // Let the CListCtrl handle the event
    return DefWindowProc(LVM_SETCOLUMNWIDTH, wParam, lParam);
}

/*******************************************************************************
  Function Name  : OnHeaderEndDrag
  Input(s)       : pNMHDR, pResult
  Output         : BOOL
  Functionality  : Event is fired after column Drag is ended.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
BOOL CRxMsgList::OnHeaderEndDrag(UINT, NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
    NMHEADER* pNMH = (NMHEADER*)pNMHDR;

    if (pNMH->pitem->mask & HDI_ORDER)
    {
        // Correct iOrder so it is just after the last hidden column
        int nColCount = GetHeaderCtrl()->GetItemCount();
        int* pOrderArray = new int[nColCount];
        VERIFY( GetColumnOrderArray(pOrderArray, nColCount) );

        if(pNMH->iItem == 0 )
        {
            pNMH->pitem->iOrder = 0;
            return FALSE;
        }

        for(int i = 0; i < nColCount ; ++i)
        {
            if (IsColumnVisible(pOrderArray[i]))
            {
                pNMH->pitem->iOrder = max(pNMH->pitem->iOrder,i);

                if(pNMH->pitem->iOrder == 0)
                {
                    pNMH->pitem->iOrder++;
                }

                break;
            }
        }

        delete [] pOrderArray;
    }

    return FALSE;
}

/*******************************************************************************
  Function Name  : OnHeaderDividerDblClick
  Input(s)       : pNMHDR, pResult
  Output         : BOOL
  Functionality  : Event is fired when user double clicks on Divider.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
BOOL CRxMsgList::OnHeaderDividerDblClick(UINT, NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
    NMHEADER* pNMH = (NMHEADER*)pNMHDR;
    SetColumnWidthAuto(pNMH->iItem);
    return TRUE;    // Don't let parent handle the event
}

/*******************************************************************************
  Function Name  : OnDeleteColumn
  Input(s)       : wParam, lParam
  Output         : LRESULT
  Functionality  : Handles the DeleteColumn functionality to Default Window Procedure.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CRxMsgList::OnDeleteColumn(WPARAM wParam, LPARAM lParam)
{
    // Let the CListCtrl handle the event
    LRESULT lRet = DefWindowProc(LVM_DELETECOLUMN, wParam, lParam);

    if (lRet == FALSE)
    {
        return FALSE;
    }

    // Book keeping of columns
    DeleteColumnState((int)wParam);
    return lRet;
}

/*******************************************************************************
  Function Name  : OnInsertColumn
  Input(s)       : wParam, lParam
  Output         : LRESULT
  Functionality  : Handles the InsertColumn functionality to Default Window Procedure.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CRxMsgList::OnInsertColumn(WPARAM wParam, LPARAM lParam)
{
    // Let the CListCtrl handle the event
    LRESULT lRet = DefWindowProc(LVM_INSERTCOLUMN, wParam, lParam);

    if (lRet == -1)
    {
        return -1;
    }

    int nCol = (int)lRet;

    // Book keeping of columns
    if (GetColumnStateCount() < GetHeaderCtrl()->GetItemCount())
    {
        InsertColumnState((int)nCol, true);    // Insert as visible
    }

    return lRet;
}

/*******************************************************************************
  Function Name  : PreSubclassWindow
  Input(s)       : -
  Output         : -
  Functionality  : Calls the PreSubclassWindow() of base class.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::PreSubclassWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    CFlickerFreeListCtrl::PreSubclassWindow();
}

/*******************************************************************************
  Function Name  : vSetSortableMsgColumns
  Input(s)       : sHdrColStruct, eBusType
  Output         : -
  Functionality  : Sets the Sortable message window columns for current bus eBusType.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::vSetSortableMsgColumns(SMSGWNDHDRCOL& sHdrColStruct, ETYPE_BUS eBusType)
{
    int nColCount = GetHeaderCtrl()->GetItemCount();
    nColCount--;    //Reduce the size by 1 for the Blank column.
    m_pbSortableColumn = (bool*)malloc(sizeof(bool) * nColCount);
    m_pbAscendingOrder = (bool*)malloc(sizeof(bool) * nColCount);

    if( m_pbSortableColumn == NULL || m_pbAscendingOrder == NULL)
    {
        AfxMessageBox(_T("Failed to create Memory for Sortable Column."));
        return;
    }

    for(int i=0; i < nColCount; i++)
    {
        m_pbSortableColumn[i] = false;
        m_pbAscendingOrder[i] = true;
    }

    //Set the Sortable Columns as true.

    if(eBusType == CAN) //If the BUS is CAN ,Set the Time field, Channel and ID fields as Sortable
    {
        m_pbSortableColumn[sHdrColStruct.m_byTimePos] = true;
        m_pbSortableColumn[sHdrColStruct.m_byChannel] = true;
        m_pbSortableColumn[sHdrColStruct.m_byIDPos]   = true;
    }
    else if(eBusType == J1939)
        //If the BUS is J1939,Set the Time field, Channel, CANID, PGN, Type, Src and Dest fields as Sortable
    {
        m_pbSortableColumn[sHdrColStruct.m_byTimePos]   = true;
        m_pbSortableColumn[sHdrColStruct.m_byChannel]   = true;
        m_pbSortableColumn[sHdrColStruct.m_byIDPos]     = true;
        m_pbSortableColumn[sHdrColStruct.m_byPGNPos]    = true;
        //m_pbSortableColumn[sHdrColStruct.m_byMsgTypePos]= true;
        m_pbSortableColumn[sHdrColStruct.m_bySrcPos]    = true;
        m_pbSortableColumn[sHdrColStruct.m_byDestPos]   = true;
    }
}

/*******************************************************************************
  Function Name  : OnHdnItemclick
  Input(s)       : pNMHDR, pResult
  Output         : -
  Functionality  : Posts the WM_PARENT_SORT_COLUMN message to Parent class
                   and updates the variables m_pbAscendingOrder, m_nSortedColumn.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  : 13-07-2010, ArunKumar K,
                   Added condition 'pLV->iItem>0' in If Statement to ignore
                   Sorting of Blank Column at Zeroth index in Interpret mode.
*******************************************************************************/
void CRxMsgList::OnHdnItemclick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMHEADER pLV = reinterpret_cast<LPNMHEADER>(pNMHDR);

    if(!m_bConnected && pLV->iItem>0 && (this->GetItemCount()>0) && m_pbSortableColumn[pLV->iItem-1])
    {
        m_wndHeader.SelectSortedColumn(pLV->iItem);
        m_wndHeader.SelectSortAscDesc(m_pbAscendingOrder[pLV->iItem-1]);
        // show the appropriate arrow in the header control.
        m_wndHeader.UpdateArrowDirection();
        ::SendMessage( m_hParent, WM_PARENT_SORT_COLUMN, (WPARAM)pLV->iItem, (LPARAM)m_pbAscendingOrder[pLV->iItem-1]);
        m_pbAscendingOrder[pLV->iItem-1]=!m_pbAscendingOrder[pLV->iItem-1];

        if(m_nSortedColumn != pLV->iItem)
        {
            m_pbAscendingOrder[m_nSortedColumn-1] = true;
            m_nSortedColumn = pLV->iItem;
        }
    }

    *pResult = 0;
}

/*******************************************************************************
  Function Name  : OnHdnItemclick
  Input(s)       : bConnected
  Output         : -
  Functionality  : Sets the variable m_bConnected to bConnected input.
  Member of      : CRxMsgList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::SetConnectionStatus(bool bConnected)
{
    m_bConnected = bConnected;
}


// CListHdrCtrl definitions

/*******************************************************************************
  Function Name  : CListHdrCtrl
  Input(s)       : -
  Output         : -
  Functionality  : Constructor
  Member of      : CListHdrCtrl
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
CRxMsgList::CListHdrCtrl::CListHdrCtrl()
{
    m_nSortCol = -1;
    m_bIsAscendingSort = TRUE;
}

/*******************************************************************************
  Function Name  : RetreiveSortColID
  Input(s)       : -
  Output         : int
  Functionality  : Returns the current Sort column (m_nSortCol).
  Member of      : CListHdrCtrl
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
int CRxMsgList::CListHdrCtrl::RetreiveSortColID() const
{
    return m_nSortCol;
}

/*******************************************************************************
  Function Name  : IsAscendingSort
  Input(s)       : -
  Output         : BOOL
  Functionality  : Returns the current Sort order (m_bIsAscendingSort).
  Member of      : CListHdrCtrl
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
BOOL CRxMsgList::CListHdrCtrl::IsAscendingSort() const
{
    return m_bIsAscendingSort;
}

/*******************************************************************************
  Function Name  : SelectSortAscDesc
  Input(s)       : bAscending
  Output         : -
  Functionality  : Sets the variable m_bIsAscendingSort to input bAscending.
  Member of      : CListHdrCtrl
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::CListHdrCtrl::SelectSortAscDesc(BOOL bAscending)
{
    m_bIsAscendingSort = bAscending;
}

/*******************************************************************************
  Function Name  : SelectSortedColumn
  Input(s)       : nCol
  Output         : -
  Functionality  : Select the current sort column as nColumn.
  Member of      : CListHdrCtrl
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::CListHdrCtrl::SelectSortedColumn(int nColumn)
{
    m_nSortCol = nColumn;
}

/*******************************************************************************
  Function Name  : UpdateArrowDirection
  Input(s)       : -
  Output         : -
  Functionality  : Updates Sort Arrow with m_nSortCol.
  Member of      : CListHdrCtrl
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::CListHdrCtrl::UpdateArrowDirection()
{
    // change item to owner draw style.
    HD_ITEM hditem;
    hditem.mask = HDI_FORMAT;
    VERIFY(GetItem(m_nSortCol, &hditem));
    hditem.fmt |= HDF_OWNERDRAW;
    VERIFY(SetItem(m_nSortCol, &hditem));
    // Redraw header control
    Invalidate();
}

/*******************************************************************************
  Function Name  : ClearArrowDirection
  Input(s)       : -
  Output         : -
  Functionality  : Clears Sort Arrow.
  Member of      : CListHdrCtrl
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::CListHdrCtrl::ClearArrowDirection()
{
    m_nSortCol = -1;
    m_bIsAscendingSort = TRUE;
    // Redraw header control
    Invalidate();
}

/*******************************************************************************
  Function Name  : DrawItem
  Input(s)       : LPDRAWITEMSTRUCT
  Output         : -
  Functionality  : overrided for drawing the Arrow direction for sort in
                   Column header field.
  Member of      : CListHdrCtrl
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CRxMsgList::CListHdrCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    //Attach the column header HDC.
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);
    // save device context.
    const int nSavedDCState = dc.SaveDC();
    // get the column rect.
    CRect rc(lpDrawItemStruct->rcItem);
    // set the clipping region to limit drawing within the column.
    CRgn rgn;
    rgn.CreateRectRgnIndirect(&rc);
    dc.SelectObject(&rgn);
    rgn.DeleteObject();
    // draw the background,
    CBrush brush(GetSysColor(COLOR_3DFACE));
    dc.FillRect(rc, &brush);
    // get the column text and format.
    char szText[ 256 ];
    HD_ITEM hditem;
    hditem.mask = HDI_TEXT | HDI_FORMAT;
    hditem.pszText = szText;
    hditem.cchTextMax = 255;
    GetItem(lpDrawItemStruct->itemID, &hditem);
    // determine the format for drawing the column label.
    UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS ;

    if(hditem.fmt & HDF_CENTER)
    {
        uFormat |= DT_CENTER;
    }
    else if(hditem.fmt & HDF_RIGHT)
    {
        uFormat |= DT_RIGHT;
    }
    else
    {
        uFormat |= DT_LEFT;
    }

    // adjust the rect if the mouse button is pressed on it.
    if(lpDrawItemStruct->itemState == ODS_SELECTED)
    {
        rc.left++;
        rc.top += 2;
        rc.right++;
    }

    CRect rcIcon(lpDrawItemStruct->rcItem);
    const int nOffset = (rcIcon.bottom - rcIcon.top) / 4;

    // adjust the rect further if the sort arrow is to be displayed.
    if(lpDrawItemStruct->itemID == (UINT)m_nSortCol)
    {
        rc.right -= 3 * nOffset;
    }

    rc.left += nOffset;
    rc.right -= nOffset;

    // draw the column label.
    if(rc.left < rc.right)
    {
        dc.DrawText(szText, -1, rc, uFormat);
    }

    // draw the sort arrow.
    if(lpDrawItemStruct->itemID == (UINT)m_nSortCol)
    {
        // set up the pens to use for drawing the arrow.
        CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
        CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
        CPen* pOldPen = dc.SelectObject(&penLight);
        CDC dcMem;
        BITMAP bm;
        CBitmap bmpArrowDir;
        COLORMAP cm = {RGB(0, 0, 0), GetSysColor(COLOR_BTNTEXT)};
        dcMem.CreateCompatibleDC(&dc);

        if(m_bIsAscendingSort)
        {
            bmpArrowDir.LoadMappedBitmap(IDB_HDRUP, 0, &cm, 1);
            bmpArrowDir.GetBitmap(&bm);
            dcMem.SelectObject(&bmpArrowDir);
            dc.SetStretchBltMode(HALFTONE);
            dc.SetBkMode(TRANSPARENT);
            dc.StretchBlt(rcIcon.right-15, rcIcon.top+5, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND);
        }
        else
        {
            bmpArrowDir.LoadMappedBitmap(IDB_HDRDOWN, 0, &cm, 1);
            bmpArrowDir.GetBitmap(&bm);
            dcMem.SelectObject(&bmpArrowDir);
            dc.SetStretchBltMode(HALFTONE);
            dc.SetBkMode(TRANSPARENT);
            dc.StretchBlt(rcIcon.right-15, rcIcon.top+5, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND);
        }

        bmpArrowDir.DeleteObject();
        // restore the pen.
        dc.SelectObject(pOldPen);
    }

    // restore the stored device context state.
    dc.RestoreDC(nSavedDCState);
    // detach the HDC before exit.
    dc.Detach();
}
