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
 * \file      MessageList.cpp
 * \brief     Implementation file for CMessageList class
 * \authors   Ratnadip Choudhury, Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CMessageList class
 */
#include "stdafx.h"
#include "BUSMASTER.h"
#include "MessageList.h"

/*******************************************************************************
  Function Name  : CMessageList
  Description    : Standard default constructor
  Member of      : CMessageList
  Functionality  : This will initialise local variables
  Author(s)      : Ratnadip Choudhury
  Date Created   : 02/12/2004
  Modifications  :
*******************************************************************************/
CMessageList::CMessageList()
{
    // Create GDI Objects
    // Create White color brush
    m_hWhiteBrush = CreateSolidBrush( WHITE_COLOR );
    m_hBlueBrush = CreateSolidBrush( BLUE_COLOR );
}

/*******************************************************************************
  Function Name  : ~CMessageList
  Description    : Standard Destructor
  Member of      : CMessageList
  Functionality  : -
  Author(s)      : Ratnadip Choudhury
  Date Created   : 02/12/2004
  Modifications  :
*******************************************************************************/
CMessageList::~CMessageList()
{
}


BEGIN_MESSAGE_MAP(CMessageList, CListCtrl)
    //{{AFX_MSG_MAP(CMessageList)
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
    ON_WM_CONTEXTMENU()    // OnContextMenu
    ON_MESSAGE(LVM_INSERTCOLUMN, OnInsertColumn)
    ON_NOTIFY_EX(HDN_ENDDRAG, 0, OnHeaderEndDrag)
    ON_NOTIFY_EX(HDN_BEGINTRACKA, 0, OnHeaderBeginResize)
    ON_NOTIFY_EX(HDN_BEGINTRACKW, 0, OnHeaderBeginResize)
END_MESSAGE_MAP()


/*******************************************************************************
  Function Name  : DrawItem
  Input(s)       : lpDis - Pointer to Display Structure
  Output         :
  Functionality  : This will be callee by the framework to draw list control
                   items
  Member of      : CMessageList
  Author(s)      : Ratnadip Choudhury
  Date Created   : 02/12/2004
  Modifications  : ArunKumar K on 17.03.2011,
                   Commented "Correct right margin" code.
                   Raja N
                   Modified the code to support any number of columns. Original
                   code was to support only two columns.
                   Raja N on 14.12.2004
                   Review comments implemented. Removed hardcoded value.
*******************************************************************************/
void CMessageList::DrawItem(LPDRAWITEMSTRUCT lpDis)
{
    // Get the column Count
    CHeaderCtrl* pHeader = GetHeaderCtrl();
    int nColCount = pHeader->GetItemCount();
    const int nOffset = 2;
    // Initialise to default values
    COLORREF TxtColour = GetItemData(lpDis->itemID);
    HBRUSH hBkBrush = m_hWhiteBrush;

    // Select Blue brush and white pen if this item is selected
    if ((lpDis->itemState & ODS_SELECTED) &&
            (lpDis->itemState & ODS_FOCUS))
    {
        TxtColour = WHITE_COLOR;
        hBkBrush = m_hBlueBrush;
    }

    // Set Text color and fill background
    ::SetTextColor(lpDis->hDC, TxtColour);
    ::FillRect(lpDis->hDC, &(lpDis->rcItem), hBkBrush);
    // Draw Item and subitems
    CRect omSubItemRect;

    for( int nIndex = 0; nIndex < nColCount; nIndex++ )
    {
        // Get Item boundary
        GetSubItemRect( lpDis->itemID,
                        nIndex, LVIR_BOUNDS,
                        omSubItemRect);
        // Correct right margin
        //Commented by Arun on 17-03-2011 to protect text visibility when next column is hidden
        /* if( nIndex != nColCount - 1 )
         {
             CRect omNextSubItemRect;
             GetSubItemRect( lpDis->itemID,
                             nIndex + 1, LVIR_BOUNDS,
                             omNextSubItemRect);
             omSubItemRect.right = omNextSubItemRect.left;
         }*/
        // Give some offset from the border
        omSubItemRect.left += nOffset;
        // Draw Message Name
        m_omStrInARow = GetItemText(lpDis->itemID, nIndex);
        ::DrawText( lpDis->hDC,
                    m_omStrInARow,
                    m_omStrInARow.GetLength(),
                    &omSubItemRect, DT_LEFT);
    }
}

/*******************************************************************************
  Function Name  : OnDestroy
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CMessageList
  Author(s)      : Ratnadip Choudhury
  Date Created   : 02/12/2004
  Modifications  :
*******************************************************************************/
void CMessageList::OnDestroy()
{
    // Call Parent function to do normal destroy
    CFFListCtrl::OnDestroy();
    // Delete GDI objects
    ::DeleteObject(m_hWhiteBrush);
    ::DeleteObject(m_hBlueBrush);
}

/*******************************************************************************
  Function Name  : GetColumnTitleState
  Input(s)       : nColumn
  Output         : ColumnTitleState&
  Functionality  : Returns the Column Title State of nCol as ColumnTitleState&
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
CMessageList::ColumnTitleState& CMessageList::GetColumnTitleState(int nColumn)
{
    VERIFY(nColumn >=0 &&  nColumn < m_ColumnTitleStates.GetSize());
    return m_ColumnTitleStates[nColumn];
}

/*******************************************************************************
  Function Name  : IsColumnShown
  Input(s)       : nColumn
  Output         : bool
  Functionality  : Returns a bool value indicating ncol's visibility.
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
bool CMessageList::IsColumnShown(int nColumn)
{
    return GetColumnTitleState(nColumn).m_bVisible;
}

/*******************************************************************************
  Function Name  : GetColumnTitleStateCount
  Input(s)       : -
  Output         : int
  Functionality  : Returns count of items in m_ColumnTitleStates.
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
int CMessageList::GetColumnTitleStateCount()
{
    //Return the size of the array
    return m_ColumnTitleStates.GetSize();
}

/*******************************************************************************
  Function Name  : InsertColumnTitleState
  Input(s)       : nCol, bVisible, nOrgWidth
  Output         : -
  Functionality  : Adds new column state to m_ColumnTitleStates array.
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CMessageList::InsertColumnTitleState(int nColumn, bool bVisible, int nOrigWidth)
{
    VERIFY( nColumn >=0 && nColumn <= m_ColumnTitleStates.GetSize() );
    ColumnTitleState columnTitleState;
    columnTitleState.m_nOrigWidth = nOrigWidth;
    columnTitleState.m_bVisible = bVisible;

    if (nColumn == m_ColumnTitleStates.GetSize())
    {
        // Append column state at the end of array
        m_ColumnTitleStates.Add(columnTitleState);
    }
    else
    {
        // Insert column title state in the middle of the array
        m_ColumnTitleStates.InsertAt(nColumn, columnTitleState);
        /*CArray<ColumnTitleState, ColumnTitleState> newArrayState;

        for(int i=0 ; i < m_ColumnTitleStates.GetSize(); ++i)
        {
            if (i == nColumn)
                newArrayState.Add(columnTitleState);
            newArrayState.Add(m_ColumnTitleStates[i]);
        }
        m_ColumnTitleStates = newArrayState;*/
    }
}

/*******************************************************************************
  Function Name  : DeleteColumnTitleState
  Input(s)       : nColumn
  Output         : -
  Functionality  : Removes column state of nCol from m_ColumnTitleStates array.
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CMessageList::DeleteColumnTitleState(int nColumn)
{
    VERIFY( nColumn >=0 && nColumn < m_ColumnTitleStates.GetSize() );
    m_ColumnTitleStates.RemoveAt(nColumn);
}

/*******************************************************************************
  Function Name  : OnContextMenu
  Input(s)       : pWnd, point
  Output         : -
  Functionality  : Show the context menu with a list of column names as menu items.
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
void CMessageList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
    if (! (point.x==-1 && point.y==-1) )
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
                for( int i = GetColumnTitleStateCount()-1 ; i >= 0; --i)
                {
                    UINT uFlags = MF_BYPOSITION | MF_STRING;

                    // Put check-box on context-menu
                    if (IsColumnShown(i))
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

                    //venkat
                    if(GetColumn(i, &lvc) != 0)
                    {
                        menu.InsertMenu(0, uFlags, i, lvc.pszText);
                    }
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
  Functionality  : Show / hide columns event handler of Handle context menu clicks
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/

BOOL CMessageList::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
    if (HIWORD(wParam) == 0)
    {
        int nCol = LOWORD(wParam);
        MakeColumnVisible(nCol, !IsColumnShown(nCol));
    }

    return TRUE;
}

/*******************************************************************************
  Function Name  : MakeColumnVisible
  Input(s)       : nCol, bShow
  Output         : BOOL
  Functionality  : Shows/Hides a Column nCol based on boolean bShow.
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
BOOL CMessageList::MakeColumnVisible(int nCol, bool bShow)
{
    SetRedraw(FALSE);
    ColumnTitleState& columnTitleState = GetColumnTitleState(nCol);
    int nColumnCount = GetHeaderCtrl()->GetItemCount();
    int* pnOrderArray = new int[nColumnCount];
    GetColumnOrderArray(pnOrderArray, nColumnCount);

    if (bShow)
    {
        // Restore the position of the column
        int nCurIndex = -1;

        for(int i = 0; i < nColumnCount ; ++i)
        {
            if (pnOrderArray[i]==nCol)
            {
                nCurIndex = i;
            }
            else if (nCurIndex!=-1)
            {
                // We want to move it to the original position,
                // and after the last hidden column
                if ( (i <= columnTitleState.m_nOrigPosition)
                        || !IsColumnShown(pnOrderArray[i])
                        || (IsColumnShown(pnOrderArray[i]) && pnOrderArray[i] < columnTitleState.m_nOrigPosition)
                   )
                {
                    pnOrderArray[nCurIndex] = pnOrderArray[i];
                    pnOrderArray[i] = nCol;
                    nCurIndex = i;
                }
            }
        }
    }
    else
    {
        // Move the column to the front of the display order list
        int nCurIndex(-1);

        for(int i = nColumnCount-1; i >=0 ; --i)
        {
            if (pnOrderArray[i]==nCol)
            {
                // Take a backup of the current column position
                columnTitleState.m_nOrigPosition = i;
                nCurIndex = i;
            }
            else if (nCurIndex!=-1)
            {
                pnOrderArray[nCurIndex] = pnOrderArray[i];
                pnOrderArray[i] = nCol;
                nCurIndex = i;
            }
        }
    }

    SetColumnOrderArray(nColumnCount, pnOrderArray);
    delete [] pnOrderArray;

    if (bShow)
    {
        // Restore the saved column width
        columnTitleState.m_bVisible = true;
        SetColumnWidth(nCol, columnTitleState.m_nOrigWidth);
    }
    else
    {
        // Backup the current column width
        if(GetColumnWidth(nCol) != 0)
        {
            int orgWidth = GetColumnWidth(nCol);
            SetColumnWidth(nCol, 0);
            columnTitleState.m_nOrigWidth = orgWidth;
        }

        columnTitleState.m_bVisible = false;
    }

    SetRedraw(TRUE);
    Invalidate(FALSE);
    return TRUE;
}

/*******************************************************************************
  Function Name  : OnInsertColumn
  Input(s)       : wParam, lParam
  Output         : LRESULT
  Functionality  : Handles the InsertColumn functionality to Default Window Procedure.
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
LRESULT CMessageList::OnInsertColumn(WPARAM wParam, LPARAM lParam)
{
    // Let the base class CListCtrl handle this event
    LRESULT lReturn = DefWindowProc(LVM_INSERTCOLUMN, wParam, lParam);

    if (lReturn == -1)
    {
        return -1;
    }

    int nColumn = (int)lReturn;

    // Save details of columns
    if (GetHeaderCtrl()->GetItemCount() > GetColumnTitleStateCount())
    {
        InsertColumnTitleState((int)nColumn, true);    // Set default visibility
    }

    return lReturn;
}

/*******************************************************************************
  Function Name  : OnHeaderEndDrag
  Input(s)       : pNMHDR, pResult
  Output         : BOOL
  Functionality  : Event is fired after column Drag is ended.
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
BOOL CMessageList::OnHeaderEndDrag(UINT, NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
    NMHEADER* pNMHeader = (NMHEADER*)pNMHDR;

    if (pNMHeader->pitem->mask & HDI_ORDER)
    {
        // Correct iOrder so it is just after the last hidden column
        int nColCount = GetHeaderCtrl()->GetItemCount();
        int* pOrderArray = new int[nColCount];
        VERIFY( GetColumnOrderArray(pOrderArray, nColCount) );

        if(pNMHeader->iItem == 0 )
        {
            pNMHeader->pitem->iOrder = 0;
            return FALSE;
        }

        for(int i = 0; i < nColCount ; ++i)
        {
            if (IsColumnShown(pOrderArray[i]))
            {
                pNMHeader->pitem->iOrder = max(pNMHeader->pitem->iOrder,i);

                if(pNMHeader->pitem->iOrder == 0)
                {
                    pNMHeader->pitem->iOrder++;
                }

                break;
            }
        }

        delete [] pOrderArray;
    }

    return FALSE;
}

/*******************************************************************************
  Function Name  : OnHeaderBeginResize
  Input(s)       : pNMHDR, pResult
  Output         : BOOL
  Functionality  : Handle the event for blocking resize of hidden columns.
  Member of      : CMessageList
  Author(s)      : Arunkumar K
  Date Created   : 20-05-2010
  Modifications  :
*******************************************************************************/
BOOL CMessageList::OnHeaderBeginResize(UINT, NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    //Block all column resizes
    *pResult = TRUE;    // Block resize
    return TRUE;        // Block event
}



