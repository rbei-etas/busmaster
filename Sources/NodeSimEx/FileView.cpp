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
 * \file      FileView.cpp
 * \brief     This file contain definition of all function of 
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of 
 */

#include "NodeSimEx_stdafx.h"             // Std header include file
#include "Utility/OffScreenDC.h"
#include "FileView.h"           // Clas defintion file
#include "EditFrameWnd.h"       // Child window definition file
#include "GlobalObj.h"

/////////////////////////////////////////////////////////////////////////////
// CFileView

IMPLEMENT_DYNCREATE(CFileView, CScrollView)

BEGIN_MESSAGE_MAP(CFileView, CScrollView)
    //{{AFX_MSG_MAP(CFileView)
    ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileView construction/destruction
/******************************************************************************
  Function Name    :  CFileView                                       
                                                                      
  Input(s)         :  -
  Output           :  -                                           
  Functionality    :  Constructor, Class members intialisation
  Member of        :  CFileView                                       
  Friend of        :      -                                           
                                                                      
  Author(s)        :  Amarnath Shastry                                
  Date Created     :  05.03.2002                                      
  Modifications    :  
******************************************************************************/
CFileView::CFileView()
{
    m_eBus = sm_eBus;
    m_nCharHeight = 0;
    m_nCharWidth = 0;
}
/******************************************************************************
  Function Name    :  ~CFileView                                          
                                                                      
  Input(s)         :  -
  Output           :  -                                           
  Functionality    :  Destructor
  Member of        :  CFileView                                       
  Friend of        :      -                                           
                                                                      
  Author(s)        :  Amarnath Shastry                                
  Date Created     :  05.03.2002                                      
  Modifications    :  
******************************************************************************/
CFileView::~CFileView()
{
}
/******************************************************************************
  Function Name    :  OnDraw
                                                                      
  Input(s)         :  CDC* pomDC
  Output           :  -                                           
  Functionality    :  Called by the frame work to update the view.
                      This function gets source code from the document
                      and displays on the view.
                      If warning is specified, it highlight that line 
                      with different color.
                      If single line of comment is found, it displays the line
                      with differet color.
  Member of        :  CFileView                                       
  Friend of        :      -                                           
                                                                      
  Author(s)        :                                
  Date Created     :  
******************************************************************************/
void CFileView::OnDraw(CDC* pDC)
{
    // Get document
    CFunctionEditorDoc* pomDoc = omGetDocument();
    ASSERT_VALID(pomDoc);
    // Initialise backend Buffer
    // Get Client rectangle
    // This will give the starting point and size
    CRect omRect;
    GetClientRect(&omRect);
    // Get current scroll position
    CPoint omPoint = GetScrollPosition();
    // Add this walue with Client Rect
    // to get rect from the starting point to the end of scrolling point
    omRect.right += omPoint.x;
    omRect.bottom += omPoint.y;
    // Create Backend Buffer
    /*************************************************************/
    // If backend buffer creation failed it will use Screen DC
    // Directly to avoid showing blank screen
    // The Flag m_bCreateSuccess gives an indication of which DC
    // it is using. If it is TRUE then that is Buffer. If it is 
    // FALSE then it is directly drawing on the pDC (screen pr printer DC).
    // No extra check is required to handle create failure
    /************************************************************/
    COffScreenDC  omMemDC(pDC, &omRect);
    CDC * pomDC = NULL;
    pomDC = &omMemDC;
    if(pomDoc != NULL)
    {
        TCHAR acSourceLineNo[10]  = STR_EMPTY;
        long lLineCount          = LONG_INIT;
        long lCurrentWarnLineNum = LONG_INIT;
        int  nTabStopPositions   = INT_INIT;
        BOOL bWarningLine        = FALSE;
        COLORREF    CurrentTextColor   = DWORD_INIT,
                    CurrentBkColor     = DWORD_INIT;

        // Change Font
        CFont  omNewFont;
        CFont* pomOldFont=NULL;
        BOOL bCommentFound = FALSE;
        BOOL bWithInComment = FALSE;

        // Create font
        BOOL bSuccess = omNewFont.CreateFont(m_nCharHeight,
                                             m_nCharWidth,
                                             DEFAULT_FONT_ESCAPEMENT,
                                             DEFAULT_FONT_ORIENTATION,
                                             FW_NORMAL,
                                             NOT_ITALIC,
                                             NO_UNDERLINE,
                                             NO_STRIKEOUT,
                                             DEFAULT_CHARSET,
                                             OUT_CHARACTER_PRECIS, 
                                             CLIP_CHARACTER_PRECIS,
                                             DEFAULT_QUALITY,
                                             DEFAULT_PITCH | FF_MODERN,
                                             DEFAULT_FONT);
        if(bSuccess == TRUE)
        {
			// Select the new font object
            pomOldFont = pomDC -> SelectObject(&omNewFont);

            // Get line count
            lLineCount = pomDoc->dwGetLineCount ();

            // Get warning line number
            lCurrentWarnLineNum = pomDoc->m_lCurrentWarningLineNum;
            nTabStopPositions   = defNO_OF_CHARS_IN_TAB * m_nCharWidth;

            if(lLineCount  > defCOUNT_INIT)
            {
                POSITION Position = pomDoc -> SetPosToFirstLine();
                if( Position!= NULL)
                {

                    for(long lInt = defCOUNT_INIT; lInt < lLineCount ; lInt++)
                    {  
                        int nMargin = MARGIN_FOR_FILE_VIEW;
                        bCommentFound = FALSE;
                        // Set the background mix mode to 
                        // tranparent
                        pomDC -> SetBkMode(TRANSPARENT);
                        // Display line number												
                        wsprintf(acSourceLineNo,_T("%lu:"),lInt+NEXT_POSITION);

                        CString omStr = (CString)
                            pomDoc -> pcGetLine(Position);

                        int nIndex = omStr.Find( _T("/*") );
                        // Starting of comment or already in comment block
                        if (nIndex != -1 || bWithInComment)
                        {
                            if( nIndex == -1 )
                                nIndex = 0;

                            pomDC -> SetTextColor(DIALOG_COLOR);

                            // if comment is in betn the line,
                            // get uncommented chars
                            CString omStrTemp = 
                                omStr.Left( nIndex );
                            
                            // set uncommented char to blue
                            pomDC -> SetTextColor(BLUE_COLOR);
                            
                            pomDC -> TextOut(DEFAULT_X_POS,
                                             (m_nCharHeight * (lInt+INCR_LEN)),
                                             acSourceLineNo);
                            
                            pomDC -> TabbedTextOut(
                                            ( nMargin) * m_nCharWidth,
                                            (m_nCharHeight * (lInt+INCR_LEN)),
                                            omStrTemp,
                                            TAB_POSITION,
                                            &nTabStopPositions,
                                            TAB_ORIGIN);

                            nMargin += nIndex;
                            // Get commented text and set different color
                            pomDC -> SetTextColor(DIALOG_COLOR);
                            omStrTemp = 
                                    omStr.Right( omStr.GetLength() - (nIndex));
                            nIndex = omStrTemp.Find( _T("*/") );
                            omStr = STR_EMPTY;
                            
                            if ( nIndex != -1 )
                            {
                                // Set the comment flag to true
                                bWithInComment = FALSE;
                                omStr = omStrTemp.Right(
                                    omStrTemp.GetLength() - (nIndex+2));
                                omStrTemp = omStrTemp.Left( nIndex+2 );
                            }
                            else
                            {
                                // Reset the comment flag
                                bWithInComment = TRUE;
                            }
                            
                            pomDC -> TextOut(DEFAULT_X_POS,
                                             (m_nCharHeight * (lInt+INCR_LEN)),
                                             acSourceLineNo);
                            pomDC -> TabbedTextOut(
                                            ( nMargin) * m_nCharWidth,
                                            (m_nCharHeight * (lInt+INCR_LEN)),
                                            omStrTemp,
                                            TAB_POSITION,
                                            &nTabStopPositions,
                                            TAB_ORIGIN);
                            
                            nMargin += nIndex+2;
                            pomDC -> SetTextColor(BLUE_COLOR);
                        }
                        //else
                        {
                            nIndex = omStr.Find( _T("//") );
                            if (nIndex != -1 )
                            {
                                pomDC -> SetTextColor(DIALOG_COLOR);
                                // if comment is in betn the line,
                                // get uncommented chars
                                CString omStrTemp = omStr.Left( nIndex );
                                // set uncommented char to blue
                                pomDC -> SetTextColor(BLUE_COLOR);
                                pomDC -> TextOut(DEFAULT_X_POS,
                                             (m_nCharHeight * (lInt+INCR_LEN)),
                                             acSourceLineNo);
                                pomDC -> TabbedTextOut(
                                            ( nMargin) * m_nCharWidth,
                                            (m_nCharHeight * (lInt+INCR_LEN)),
                                            omStrTemp,
                                            TAB_POSITION,
                                            &nTabStopPositions,
                                            TAB_ORIGIN);
                                nMargin += nIndex;                
                                // Get commented text and set different color
                                pomDC -> SetTextColor(DIALOG_COLOR);
                                omStrTemp = 
                                    omStr.Right( omStr.GetLength() - (nIndex));
                               
                                omStr = STR_EMPTY;
                                pomDC -> TextOut(DEFAULT_X_POS,
                                             (m_nCharHeight * (lInt+INCR_LEN)),
                                             acSourceLineNo);
                                pomDC -> TabbedTextOut(
                                            ( nMargin) * m_nCharWidth,
                                            (m_nCharHeight * (lInt+INCR_LEN)),
                                            omStrTemp,
                                            TAB_POSITION,
                                            &nTabStopPositions,
                                            TAB_ORIGIN);
                            
                                nMargin += nIndex + 2;
                                pomDC -> SetTextColor(BLUE_COLOR);
                            }
                            else
                            {
                                pomDC -> SetTextColor(BLUE_COLOR);
                            }
                        }
                        
                        if(lCurrentWarnLineNum == lInt+NEXT_POSITION)
                        {
                            bWarningLine = TRUE;

                            // Get & save current color settings
                            CurrentTextColor = pomDC -> GetTextColor();
                            CurrentBkColor = pomDC -> GetBkColor();
                            // Set the background mix mode to 
                            // opaque
                            pomDC -> SetBkMode(OPAQUE);

                            // Display the warning line in WHITE with RED
                            // Background
                            pomDC -> SetBkColor(RED_COLOR);
                            pomDC -> SetTextColor(WHITE_COLOR);
                        }

                        pomDC -> TextOut(DEFAULT_X_POS,
                                         (m_nCharHeight * (lInt+INCR_LEN)),
                                         acSourceLineNo);
                        pomDC -> TabbedTextOut(
                                        nMargin * m_nCharWidth,
                                        (m_nCharHeight * (lInt+INCR_LEN)),
                                        omStr,
                                        TAB_POSITION,
                                        &nTabStopPositions,
                                        TAB_ORIGIN);

                        // Restore normal display colors and background
                        // mix mode 
                        if(bWarningLine == TRUE)
                        {
                            pomDC -> SetTextColor(CurrentTextColor);
                            pomDC -> SetBkColor(CurrentBkColor);
                            pomDC -> SetBkMode(TRANSPARENT);
                            bWarningLine = FALSE;
                        }

                    }// End of  for(long lInt = defCOUNT_INIT; lInt < lLin....

                }// End of if( Position!= NULL)
            }// End of if(lLineCount  > defCOUNT_INIT)

            pomDC -> SelectObject(pomOldFont);
            omNewFont.DeleteObject();
        }
    } // end of if(pomDoc != NULL)
}
/******************************************************************************
  Function Name    :  OnInitialUpdate 
                                                                      
  Input(s)         :  -
  Output           :  -                                           
  Functionality    :  Initialises the CFileView class pointer 
                      defined in CMainFrame class for future use.
  Member of        :  CFileView                                       
  Friend of        :      -                                           
                                                                      
  Author(s)        :                                
  Date Created     :                                     
  Modifications    :  
******************************************************************************/
void CFileView::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();
}

#ifdef _DEBUG
void CFileView::AssertValid() const
{
    CScrollView::AssertValid();
}

void CFileView::Dump(CDumpContext& dc) const
{
    CScrollView::Dump(dc);
}

#endif //_DEBUG

/******************************************************************************
  Function Name    :  omGetDocument 
                                                                      
  Input(s)         :  -
  Output           :  -                                           
  Functionality    :  Returs document pointer
  Member of        :  CFileView                                       
  Friend of        :      -                                           
                                                                      
  Author(s)        :                                 
  Date Created     :                                      
  Modifications    :  
******************************************************************************/
CFunctionEditorDoc* CFileView::omGetDocument()
{
    return ( (CFunctionEditorDoc*)GetDocument());
}
/******************************************************************************
  Function Name    :  vDisplayWarningLineNumber 
                                                                      
  Input(s)         :  eMESSAGEFROM eMsgFrom, UINT unLineNo
  Output           :  -                                           
  Functionality    :  Scrolls to the line number spaecified and 
                      highlights the same.
  Member of        :  CFileView                                       
  Friend of        :      -                                           
                                                                      
  Author(s)        :                                 
  Date Created     :  
******************************************************************************/
void CFileView::vDisplayWarningLineNumber( eMESSAGEFROM eMsgFrom, UINT unLineNo)
{
    // Get document
    CFunctionEditorDoc*  pomSrcFileDoc = omGetDocument();

    // Doc is valid
    if(pomSrcFileDoc != NULL)
    {

        // Main frame is valid and 
        // function called from COutWnd class
        if (eMsgFrom == OUTWND)
        {
            // Get child window, active may be 
            // "COutWnd"
            CFrameWnd* pMainFrame = (CFrameWnd*)CWnd::FromHandle(CGlobalObj::sm_hWndMDIParentFrame);
            CMDIChildWnd *pChild = ( CMDIChildWnd *) pMainFrame->GetActiveFrame();

            CString omStrText = STR_EMPTY;
            BOOL bInLoop = TRUE;

            do 
            {
                // Get next window
                pChild = (CEditFrameWnd *) pChild->GetWindow(GW_HWNDNEXT);
                if(pChild != NULL )
                {
                    // Get Window caption
                    pChild->GetWindowText(omStrText);
                    
                    // check the title of child window,
                    // if matches, activate my window it
                    if (omStrText == omGetDocument()->GetTitle())
                    {
                        bInLoop = FALSE;
                    }
                }
                else
                {
                    bInLoop = FALSE;
                }
            }
            while (bInLoop);

            if (pChild != NULL)
            {
                pChild->MDIActivate();
            }
        }
        
        // Update document with the line no
        pomSrcFileDoc->m_lCurrentWarningLineNum = unLineNo;
            
        // Scroll to the line specified
        vGoToLine( unLineNo );
        
        // refresh the view
        Invalidate(TRUE);
    } // end of if(pomSrcFileDoc)
}
/******************************************************************************
  Function Name    :  OnUpdate 
                                                                      
  Input(s)         :  CView* pSender, LPARAM lHint, CObject* pHint
  Output           :  -                                           
  Functionality    :  Called by the frame work to update the view.
                      Scrolls to the position, for any changes in the 
                      view.
  Member of        :  CFileView                                       
  Friend of        :      -                                           
                                                                      
  Author(s)        :  Amarnath Shastry                                
  Date Created     :  05.03.2002
******************************************************************************/
void CFileView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) 
{
    CSize omDocSize;

    // Set SCROLL Sizes
    CFunctionEditorDoc* pomDoc = omGetDocument();

    // Get Character Height & Width
    CClientDC omDeviceContext(this);

    m_nCharHeight = GET_FILE_VIEW_CHAR_HEIGHT();
    m_nCharWidth  = GET_FILE_VIEW_CHAR_WIDTH();


    if(pomDoc != NULL)
    {
        omDocSize.cx  =  ((pomDoc -> nGetMaxLineLength() + 1) * m_nCharWidth);
        omDocSize.cy  =  m_nCharHeight * ((pomDoc -> dwGetLineCount() + 1) + 
                                                SPACE_BET_LINE_IN_FILE_VIEW);
        if( GetTotalSize() != omDocSize)
            SetScrollSizes(MM_TEXT, omDocSize);
    }
    
    // refresh the view
    Invalidate(TRUE);
}
/******************************************************************************
  Function Name    :  vGoToLine 
                                                                      
  Input(s)         :  UINT unLineNo
  Output           :  -                                           
  Functionality    :  Scrolls to the line number specified 
  Member of        :  CFileView                                       
  Friend of        :      -                                           
                                                                      
  Author(s)        :  Amarnath Shastry                                
  Date Created     :  05.03.2002
******************************************************************************/
void CFileView::vGoToLine(UINT unLineNo)
{
    // calculate the x and y points relative to the line number

    POINT Point = {INT_INIT,INT_INIT};

    // Get client rectangle
    CRect Rectangle(INT_INIT,INT_INIT,INT_INIT,INT_INIT);
    GetClientRect(&Rectangle);

    // Update point specs
    Point.x = INT_INIT;
    Point.y = unLineNo * m_nCharHeight - Rectangle.Height() / 2;

    // Scroll to the postion
    ScrollToPosition(Point);
}

/******************************************************************************
  Function Name    :  OnEraseBkgnd 

                                                                      
  Input(s)         :  -
  Output           :  -                                           
  Functionality    :  Called by the Framework to Erase the Screen Background
  Member of        :  CFileView                                       
  Friend of        :      -                                           
                                                                      
  Author(s)        :  Raja N                                
  Date Created     :  22.07.2004
******************************************************************************/
BOOL CFileView::OnEraseBkgnd(CDC* pDC) 
{
    // Return TRUE here as erasing is taken care in Draw Handler
    UNUSED_ALWAYS( pDC);
    return TRUE;
}
