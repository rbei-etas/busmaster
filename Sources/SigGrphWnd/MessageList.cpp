/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  MessageList.cpp
  Description   :  Implementation file for CMessageList class
  $Log:   X:/Archive/Sources/SigGrphWnd/MessageList.cpv  $
   
  Author(s)     :  Ratnadip Choudhury
  Date Created  :  31.01.2004
  Modified By   :  Amitesh Bharti
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
*********************************************************************/

#include "SigGrphWnd_stdafx.h"
#include "MessageList.h"
#include "SigGrphWnd_Defines.h"

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
  Modifications  : Raja N
                   Modified the code to support any number of columns. Original
                   code was to support only two columns.
                   Raja N on 14.12.2004
                   Review comments implemented. Removed hardcoded value.
*******************************************************************************/
void CMessageList::DrawItem(LPDRAWITEMSTRUCT lpDis) 
{
    // Get the column Count
    CHeaderCtrl * pHeader = GetHeaderCtrl();
    int nColCount = pHeader->GetItemCount();
    const int nOffset = 2;    
    // Initialise to default values
    COLORREF TxtColour = (COLORREF)GetItemData(lpDis->itemID);
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
        // Correct right magin
        if( nIndex != nColCount - 1 )
        {
            CRect omNextSubItemRect;
            GetSubItemRect( lpDis->itemID,
                            nIndex + 1, LVIR_BOUNDS,
                            omNextSubItemRect);
            omSubItemRect.right = omNextSubItemRect.left;
        }
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