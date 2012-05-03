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
 * \file      FnsTreeView.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "NodeSimEx_stdafx.h"   // Std include header file
#include "NodeSimEx_resource.h"
#include "GlobalObj.h"
#include "FunctionView.h"
#include "KeyValue.h"           // Dialog to display user key press for key handler
#include "DefineTimerHandler.h" // Dialog class for adding new timer handler
#include "IncludeHeaderDlg.h"   // Dialog class for adding new include header
#include "ErrHandlerDlg.h"      // Dialog class for error handlers
#include "DLLHandlerDlg.h"      // Dialog class for dll handlers
#include "MsgHandlerDlg.h"      // Dialog class for message handlers
#include "EventHandlerDlg.h"    // Dialog class for event handlers
#include "UtlityDlg.h"          // Dialog class for adding utility func
#include "FnsTreeView.h"        // Class defintion file

/////////////////////////////////////////////////////////////////////////////
// CFnsTreeView

IMPLEMENT_DYNCREATE(CFnsTreeView, CTreeView)
/******************************************************************************/
/*  Function Name    :  CFnsTreeView                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
CFnsTreeView::CFnsTreeView()
{
    m_eBus = sm_eBus;
    m_bFlag = FALSE;
}
/******************************************************************************/
/*  Function Name    :  ~CFnsTreeView                                         */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Destructor
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
CFnsTreeView::~CFnsTreeView()
{
}


BEGIN_MESSAGE_MAP(CFnsTreeView, CTreeView)
    //{{AFX_MSG_MAP(CFnsTreeView)
    ON_WM_CREATE()
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnTreeItemDoubleClick)
    ON_NOTIFY_REFLECT(NM_CLICK, OnItemLeftClick)
    ON_WM_LBUTTONDOWN()
    ON_NOTIFY_REFLECT(NM_RCLICK, OnTreeViewRightclick)
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(IDM_DEL_HAND, OnDeleteHandle)
    ON_COMMAND(IDM_ADD_HAND, OnAddHandle)
    ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
    ON_COMMAND(IDM_EDIT_HAND, OnEditFunctionHeader)
    ON_COMMAND(ID_FUNCTIONEDITOR_SAVE, OnFunctionEditorSave)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFnsTreeView drawing



/////////////////////////////////////////////////////////////////////////////
// CFnsTreeView diagnostics

#ifdef _DEBUG
void CFnsTreeView::AssertValid() const
{
    CTreeView::AssertValid();
}

void CFnsTreeView::Dump(CDumpContext& dc) const
{
    CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFnsTreeView message handlers
/******************************************************************************/
/*  Function Name    :  OnInitialUpdate                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the frame work to update the view.
                        Fills up the tree with function and variable names
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
void CFnsTreeView::OnInitialUpdate()
{
    // Try populating only once
    if (m_bFlag == FALSE)
    {
        CTreeView::OnInitialUpdate();
        m_bFlag = TRUE;
    }
}
/******************************************************************************/
/*  Function Name    :  bPopulateTree                                         */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Fills the tree view with all the handler prototypes   */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  06.03.2002                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      15.02.2003, Added Tree item for DLL, ERROR handler    */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      07.04.2003, Review Comment Incorporated               */
/*  Modifications    :  Raja N                                                */
/*                      12.05.2004, Removed the code that papulates the tree  */
/*                      for Global Variables                                  */
/******************************************************************************/
BOOL CFnsTreeView::bPopulateTree()
{
    // Get reference to tree control
    CTreeCtrl& omTree = GetTreeCtrl();
    // Get document pointer
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)GetDocument();

    if (pDoc != NULL)
    {
        // Set heading text to Black, bold
        omTree.SetTextColor( BLACK_COLOR );
        // Insert root item as path of the C file opened
        HTREEITEM hItem =
            omTree.InsertItem( pDoc->GetPathName());
        omTree.SetItemImage( hItem, 2, 2 );
        // Add headers, set bold and expand the root
        HTREEITEM hHdr = omTree.InsertItem( INCLUDE_HEADERS, hItem );
        HTREEITEM hMsg = omTree.InsertItem( MESSAGE_HANDLERS, hItem );
        HTREEITEM hTmr = omTree.InsertItem( TIMER_HANDLERS, hItem );
        HTREEITEM hKey = omTree.InsertItem( KEY_HANDLERS, hItem );
        HTREEITEM hErr = NULL;

        if (m_eBus != J1939)
        {
            hErr = omTree.InsertItem( ERROR_HANDLERS, hItem );
        }

        HTREEITEM hEvenInd = NULL;

        if (m_eBus != CAN)
        {
            hEvenInd = omTree.InsertItem( EVENTIND_HANDLERS, hItem );
        }

        HTREEITEM hDLL = omTree.InsertItem( DLL_HANDLERS, hItem );
        HTREEITEM hUti = omTree.InsertItem( UTILITY_FUNCS, hItem );
        HTREEITEM hGlv = omTree.InsertItem( GLOBAL_VARIABLES, hItem );
        omTree.SetItemState( hItem, TVIS_BOLD, TVIS_BOLD );
        omTree.Expand( hItem, TVE_EXPAND );
        HTREEITEM hChild = omTree.GetChildItem(hItem);

        // Set all the child items to bold
        while (hChild != NULL)
        {
            omTree.SetItemState(hChild, TVIS_BOLD, TVIS_BOLD);
            hChild = omTree.GetNextSiblingItem(hChild);
        }

        // Get include header file array from the doc
        CStringArray* pIncArray =
            pDoc->omStrGetIncudeHeaderFiles();

        if(pIncArray != NULL )
        {
            // Add all the include headers
            for ( UINT unCount = 0;
                    unCount < (UINT)pIncArray->GetSize();
                    unCount++ )
            {
                HTREEITEM hCldIncItem =
                    omTree.InsertItem( pIncArray->GetAt(unCount), hHdr );
                omTree.SetItemImage( hCldIncItem, 5, 5 );
            }
        }

        omTree.SetItemImage( hHdr, 8, 8 );
        omTree.Expand( hHdr, TVE_EXPAND );
        omTree.SetItemImage(hErr,9,9);
        omTree.Expand( hErr, TVE_EXPAND );
        // Get message handler array from the doc
        CStringArray* pMsgArray =
            pDoc->omStrGetMessageHandlerPrototypes();

        if(pMsgArray != NULL )
        {
            // Add all the message handlers under the message handler
            // header
            for ( UINT unCount = 0;
                    unCount < (UINT)pMsgArray->GetSize();
                    unCount++ )
            {
                HTREEITEM hCldMsgItem =
                    omTree.InsertItem( pMsgArray->GetAt(unCount), hMsg );
                omTree.SetItemImage( hCldMsgItem, 5, 5 );
            }
        }

        omTree.SetItemImage( hMsg, 7, 7 );
        omTree.Expand( hMsg, TVE_EXPAND );
        // Get Timer handler array from the doc
        CStringArray* pTmrArray =
            pDoc->omStrGetTimerHandlerPrototypes();

        if(pTmrArray != NULL )
        {
            // Add all the timer handlers under the timer handler
            // header
            for ( UINT unCount = 0;
                    unCount < (UINT)pTmrArray->GetSize();
                    unCount++ )
            {
                HTREEITEM hCldTimerItem =
                    omTree.InsertItem( pTmrArray->GetAt(unCount), hTmr );
                omTree.SetItemImage( hCldTimerItem, 5, 5 );
            }
        }

        omTree.SetItemImage( hTmr, 1, 1 );
        omTree.Expand( hTmr, TVE_EXPAND );
        // Get Key handler array from the doc
        CStringArray* pKeyArray =
            pDoc->omStrGetKeyHandlerPrototypes();

        if(pKeyArray != NULL )
        {
            // Add all the key handlers under the key handler
            // header
            for ( UINT unCount = 0;
                    unCount < (UINT)pKeyArray->GetSize();
                    unCount++ )
            {
                HTREEITEM hKeyChildItem =
                    omTree.InsertItem( pKeyArray->GetAt(unCount), hKey );
                omTree.SetItemImage( hKeyChildItem, 5, 5 );
            }
        }

        omTree.SetItemImage( hKey, 4, 4 );
        omTree.Expand( hKey, TVE_EXPAND );

        // Get Error handler array from the doc
        if (hErr != NULL)
        {
            CStringArray* pomErrorArray = pDoc->pomStrGetErrorHandlerPrototypes();

            if(pomErrorArray != NULL )
            {
                // Add all the Error handlers under the key handler
                // header
                for (UINT unCount = 0; unCount < (UINT)pomErrorArray->GetSize(); unCount++ )
                {
                    HTREEITEM hErrChildItem =
                        omTree.InsertItem( pomErrorArray->GetAt(unCount), hErr );
                    omTree.SetItemImage( hErrChildItem, 5, 5 );
                }
            }

            omTree.SetItemImage(hErr,9,9);
            omTree.Expand( hErr, TVE_EXPAND );
        }

        // Get Event indication handler array from the doc
        if (hEvenInd != NULL)
        {
            CStringArray* pEventIndArray = pDoc->omStrGetEventIndPrototypes();

            if(pEventIndArray != NULL )
            {
                // Add all the Event indication handlers under
                //the Event indication handler header
                for ( UINT unCount = 0;
                        unCount < (UINT)pEventIndArray->GetSize();
                        unCount++ )
                {
                    HTREEITEM hEIChildItem =
                        omTree.InsertItem( pEventIndArray->GetAt(unCount), hEvenInd );
                    omTree.SetItemImage( hEIChildItem, 5, 5 );
                }
            }

            omTree.SetItemImage( hEvenInd, 0, 0 );
            omTree.Expand( hEvenInd, TVE_EXPAND );
        }

        // Get DLL handler array from the doc
        CStringArray* pomDLLArray =
            pDoc->pomStrGetDLLHandlerPrototypes();

        if(pomDLLArray != NULL )
        {
            // Add all the DLL handlers under the key handler
            // header
            for ( UINT unCount = 0;
                    unCount < (UINT)pomDLLArray->GetSize();
                    unCount++ )
            {
                HTREEITEM hDLLyChildItem =
                    omTree.InsertItem( pomDLLArray->GetAt(unCount), hDLL );
                omTree.SetItemImage( hDLLyChildItem, 5, 5 );
            }
        }

        omTree.SetItemImage(hDLL,10,10);
        omTree.Expand( hDLL, TVE_EXPAND );
        // Get Utility func array from the doc
        CStringArray* pUtilArray =
            pDoc->omStrGetUtilityFunctionPrototypes();

        if(pUtilArray != NULL )
        {
            // Add all the utility functions under the utility
            // header
            for ( UINT unCount = 0;
                    unCount < (UINT)pUtilArray->GetSize();
                    unCount++ )
            {
                HTREEITEM hUtilChildItem =
                    omTree.InsertItem( pUtilArray->GetAt(unCount), hUti );
                omTree.SetItemImage( hUtilChildItem, 5, 5 );
            }
        }

        omTree.SetItemImage( hUti, 6, 6 );
        omTree.Expand( hUti, TVE_EXPAND );
        omTree.SetItemImage( hGlv, 3, 3 );
        omTree.Expand( hGlv, TVE_EXPAND );
    }

    return TRUE;
}
/******************************************************************************/
/*  Function Name    :  OnCreate                                              */
/*                                                                            */
/*  Input(s)         :  LPCREATESTRUCT lpCreateStruct                         */
/*  Output           :  int                                                   */
/*  Functionality    :  Creates the image list and associates the same
                        to the tree view.
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  06.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
int CFnsTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    // Base class call
    if (CTreeView::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // Create image list
    m_omImageList.Create( IDR_BMP_FUNC_EDITOR,
                          16,
                          1,
                          WHITE_COLOR);

    // Associate the image list to the tree
    if ( m_omImageList != 0 )
    {
        GetTreeCtrl().SetImageList (&m_omImageList, TVSIL_NORMAL);
    }

    return 0;
}
/******************************************************************************/
/*  Function Name    :  PreCreateWindow                                       */
/*                                                                            */
/*  Input(s)         :  CREATESTRUCT& cs                                      */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Sets the style for the tree view
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  06.03.2002                                            */
/*  Modifications    :
/******************************************************************************/

BOOL CFnsTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
                | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES
                | TVS_DISABLEDRAGDROP |TVS_SHOWSELALWAYS |TVS_FULLROWSELECT ;
    return CTreeView::PreCreateWindow(cs);
}
/******************************************************************************/
/*  Function Name    :  OnTreeItemDoubleClick                                 */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Displays the body of the selected function in the
                        CFunctionView class
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  06.03.2002                                            */
/*  Modifications    :  Raja N on 12.05.2004                                  */
/*                   :  Added checks for Global Variable selection            */
/******************************************************************************/
void CFnsTreeView::OnTreeItemDoubleClick(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
    CTreeCtrl& omTree = GetTreeCtrl();
    HTREEITEM hSelectedItem = omTree.GetSelectedItem();

    if ( hSelectedItem != NULL )
    {
        // check if the selected item is
        // function name or global variable
        HTREEITEM hParent = omTree.GetParentItem( hSelectedItem );

        if ( hParent != NULL )
        {
            HTREEITEM hGrandParent = omTree.GetParentItem( hParent );

            if ( hGrandParent != NULL )
            {
                // Selected item is function or global variable
                // or may be the include header, Get the text
                CString omStrSelectedText =
                    omTree.GetItemText( hSelectedItem );
                // Get function name only from the prototype
                CFunctionEditorDoc* pDoc =
                    (CFunctionEditorDoc*)GetDocument();

                if (pDoc != NULL)
                {
                    // if selected item is function prototype,
                    // get the function name only. else
                    // it shud be global variable or include header
                    if ( omStrSelectedText.Find( '(' ) != -1 )
                    {
                        omStrSelectedText =
                            pDoc->omStrExtractFunctionNameFromPrototype(
                                omStrSelectedText );
                        omStrSelectedText.TrimLeft();
                        omStrSelectedText.TrimRight();
                        int nIndex = omStrSelectedText.Find( _T(" "));

                        if ( nIndex != -1 )
                        {
                            omStrSelectedText =
                                omStrSelectedText.Right(
                                    omStrSelectedText.GetLength() - (nIndex+1) );
                        }
                    }

                    // Display the function body in function view
                    CFunctionView* pView = CGlobalObj::ouGetObj(m_eBus).podGetFunctionViewPtr();

                    if(pView)
                    {
                        pView->SetActiveWindow();
                        pView->vSetFunctionToEdit(omStrSelectedText);
                    }
                }// if (pDoc != NULL)
            }//if ( hGrandParent != NULL )
            else
            {
                CString omStrSelectedText =  omTree.GetItemText(hSelectedItem);

                if( omStrSelectedText == GLOBAL_VARIABLES )
                {
                    CGlobalObj::ouGetObj(m_eBus).podGetFunctionViewPtr()->vSetFunctionToEdit( omStrSelectedText );
                }
            }
        }//if ( hParent != NULL )
    }

    //GetParentFrame()->Invalidate();
    //  *pResult = 0;
}

/******************************************************************************/
/*  Function Name    :  OnItemLeftClick                                       */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Scrolls to the selected function in the file view     */
/*                                                                            */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/
void CFnsTreeView::OnItemLeftClick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Get document
    CFunctionEditorDoc* pDoc =
        (CFunctionEditorDoc*)GetDocument();

    if ( pDoc != NULL)
    {
        CTreeCtrl& omTree = GetTreeCtrl();
        UINT* punFlag = new UINT;

        if ( punFlag != NULL )
        {
            HTREEITEM hSelItem = omTree.HitTest(m_omLeftClickPoint,punFlag);

            if(!(*punFlag & TVHT_ONITEM ))
            {
                hSelItem = NULL;
            }
            else
            {
                // Get the selected text
                CString omStrText = omTree.GetItemText( hSelItem );

                // If it is global variable then set the header as search
                // string
                if( omStrText == GLOBAL_VARIABLES)
                {
                    // Set position to the declaration
                    omStrText = BUS_VAR_HDR;
                }

                POSITION pos = pDoc->SetPosToFirstLine();
                UINT unLineCount = 0;

                while ( pos != NULL )
                {
                    CString omStrLine = pDoc->pcGetLine(pos);
                    BOOL bSkipFlag = FALSE;
                    unLineCount++;

                    // if the selected text in the tree view
                    // and the text returned by the list
                    // matches, then go to the definition
                    // of selected function
                    if ( omStrLine.Find(omStrText) != -1)
                    {
                        // If it is util fn prototype skip and
                        // goto the next line
                        if( omStrLine.Find(UTILS_PREFIX) != -1
                                && omStrLine.Find(OPEN_PARENTHESIS) != -1
                                && omStrLine.Find(SEMI_COLON) != -1)
                        {
                            //Skip execution
                            bSkipFlag = TRUE;
                        }

                        // go to the definition
                        // of selected function
                        if( bSkipFlag == FALSE)
                        {
                            // Display the function body in function view
                            CFileView* pView = CGlobalObj::ouGetObj(m_eBus).podGetFileViewPtr();

                            if(pView)
                            {
                                pView->SetActiveWindow();
                                pView->vDisplayWarningLineNumber(FNTREEVIEW, unLineCount);
                            }
                        }
                    }
                }
            }
        }

        // Clean up
        if ( punFlag != NULL )
        {
            delete punFlag;
            punFlag = NULL;
        }
    }

    *pResult = 0;
}
/******************************************************************************
  Function Name    :  OnLButtonDown
  Input(s)         :  UINT nFlags, CPoint point
  Output           :
  Functionality    :  Stores the click point
  Member of        :  CFnsTreeView
  Friend of        :      -
  Author(s)        :
  Date Created     :
  Modifications    :
******************************************************************************/
void CFnsTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_omLeftClickPoint = point;
    CTreeView::OnLButtonDown(nFlags, point);
}
/******************************************************************************/
/*  Function Name    :  OnTreeViewRightclick                                  */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Displays a pop-up on the selected item                */
/*                      enables or disables depending on the item selected    */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/

void CFnsTreeView::OnTreeViewRightclick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    ////AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // Get document
    CFunctionEditorDoc* pDoc =
        (CFunctionEditorDoc*)GetDocument();
    BOOL bHide = FALSE;

    if ( pDoc != NULL)
    {
        CTreeCtrl& omTree = GetTreeCtrl();
        UINT* punFlag = new UINT;
        HTREEITEM hSelItem = omTree.HitTest(m_omRightClickPoint,punFlag);

        if(!(*punFlag & TVHT_ONITEM ))
        {
            hSelItem = NULL;
        }
        else
        {
            omTree.SelectItem(hSelItem);
            CMenu* pomContextMenu = new CMenu;
            CMenu myMenu;

            if ( pomContextMenu != NULL )
            {
                HINSTANCE hInst = AfxGetInstanceHandle();
                // Load the Menu from the resource
                //pomContextMenu->DestroyMenu();
                BOOL bRetVal = myMenu.LoadMenu(IDM_FUNC_EDITOR);
                HMENU hmn = ::LoadMenu(hInst, _T("MyShortcutmenu"));
                BOOL bTest = pomContextMenu->LoadMenu(MAKEINTRESOURCE(IDM_FUNC_EDITOR));

                if (bTest <= 0)
                {
                    DWORD dw = GetLastError();
                    LPVOID lpMsgBuf;
                    FormatMessage(
                        FORMAT_MESSAGE_ALLOCATE_BUFFER |
                        FORMAT_MESSAGE_FROM_SYSTEM |
                        FORMAT_MESSAGE_IGNORE_INSERTS,
                        NULL,
                        dw,
                        0, // Default language
                        (LPTSTR) &lpMsgBuf,
                        0,
                        NULL
                    );
                    // Process any inserts in lpMsgBuf.
                    // ...
                    // Display the string.
                    MessageBox( (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION );
                    // Free the buffer.
                    LocalFree( lpMsgBuf );
                }

                CMenu* pomSubMenu = pomContextMenu->GetSubMenu(0);

                if (!pomSubMenu)
                {
                    AfxMessageBox(_T("sub menu failed"));
                }

                // Get the parent item
                HTREEITEM hParentItem =  omTree.GetParentItem( hSelItem );

                if ( hParentItem != NULL && pomSubMenu != NULL)
                {
                    //Disable save option
                    pomSubMenu->EnableMenuItem(ID_FUNCTIONEDITOR_SAVE,
                                               MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );
                    HTREEITEM hGrandParentItem =
                        omTree.GetParentItem( hParentItem );

                    if ( hGrandParentItem != NULL )
                    {
                        // Menu option "Edit" to be displayed
                        // only for items under global variables or
                        // include header,timer handler,utility function
                        CString omStrPar = omTree.GetItemText(hParentItem);

                        if ( omStrPar != GLOBAL_VARIABLES &&
                                omStrPar != INCLUDE_HEADERS &&
                                omStrPar != UTILITY_FUNCS &&
                                omStrPar != TIMER_HANDLERS)
                        {
                            // Disable edit menu since the item selected has no
                            // grand parent.
                            pomSubMenu->EnableMenuItem(IDM_EDIT_HAND,
                                                       MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );
                        }

                        pomSubMenu->EnableMenuItem(IDM_ADD_HAND,
                                                   MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );
                    }
                    else
                    {
                        // Disable edit menu since the item selected has
                        // no grand parent.
                        pomSubMenu->EnableMenuItem(IDM_EDIT_HAND,
                                                   MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );
                        // Selected item is last child item
                        CString omStrText =
                            omTree.GetItemText( hSelItem );

                        if ( omStrText == TIMER_HANDLERS )
                        {
                            // Number of timer hanlers can be only
                            // ten
                            // Get pointer to array
                            CStringArray* pArray =
                                pDoc->omStrGetTimerHandlerPrototypes();

                            // Check the size of the array
                            if ( pArray != NULL )
                            {
                                if ( pArray->GetSize() >= defNUMBER_OF_TIMER)
                                {
                                    pomSubMenu->EnableMenuItem(IDM_ADD_HAND,
                                                               MF_BYCOMMAND
                                                               |MF_DISABLED
                                                               |MF_GRAYED );
                                }
                            }
                        }

                        if( omStrText != ERROR_HANDLERS &&
                                omStrText != DLL_HANDLERS  )
                        {
                            pomSubMenu->EnableMenuItem(IDM_DEL_HAND,
                                                       MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );
                        }

                        // Hide menu incase of global variable selection
                        if( omStrText == GLOBAL_VARIABLES )
                        {
                            //bHide = TRUE;
                        }
                    }

                    if( bHide != TRUE)
                    {
                        ClientToScreen(&m_omRightClickPoint);
                        pomSubMenu->TrackPopupMenu(
                            TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                            m_omRightClickPoint.x,
                            m_omRightClickPoint.y,
                            this,
                            NULL);
                    }
                }
                else
                {
                    //This itself is the parent
                    //Disable all except Save
                    pomSubMenu->EnableMenuItem(IDM_DEL_HAND,
                                               MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );
                    pomSubMenu->EnableMenuItem(IDM_ADD_HAND,
                                               MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );
                    pomSubMenu->EnableMenuItem(IDM_EDIT_HAND,
                                               MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );

                    if( bHide != TRUE)
                    {
                        ClientToScreen(&m_omRightClickPoint);
                        pomSubMenu->TrackPopupMenu(
                            TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                            m_omRightClickPoint.x,
                            m_omRightClickPoint.y,
                            this,
                            NULL);
                    }
                }

                delete pomContextMenu;
                pomContextMenu = NULL;
            } // if (pomContextMenu != NULL )
        }

        delete punFlag;
        punFlag = NULL;
    }

    *pResult = 0;
}
/******************************************************************************/
/*  Function Name    :  OnRButtonDown                                         */
/*                                                                            */
/*  Input(s)         :  UINT nFlags, CPoint point                             */
/*  Output           :                                                        */
/*  Functionality    :  Stores the click point

/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :
/*  Date Created     :
/*  Modifications    :
/******************************************************************************/
void CFnsTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_omRightClickPoint = point;
    CTreeView::OnRButtonDown(nFlags, point);
}
/******************************************************************************/
/*  Function Name    :  OnDeleteHandle                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Deletes selected handler from the source

/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :
/******************************************************************************/
void CFnsTreeView::OnDeleteHandle()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bReturnVal = FALSE;
    // Get document
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)GetDocument();

    if ( pDoc != NULL)
    {
        SBUS_SPECIFIC_INFO sBusSpecInfo;
        pDoc->bGetBusSpecificInfo(sBusSpecInfo);
        CTreeCtrl& omTree = GetTreeCtrl();
        // Get slected item handle from the tree
        HTREEITEM hSelItem      = omTree.GetSelectedItem();
        HTREEITEM hSelItemChild = omTree.GetChildItem(hSelItem);
        CString omStrFnName = STR_EMPTY;
        CString omStrSelectedText = omTree.GetItemText( hSelItem );
        int nYesNo = IDYES;

        if(hSelItemChild == NULL)
        {
            // Find type of the handler, or if it is a global variable
            if ( omStrSelectedText.Find( '(' ) != -1 )
            {
                // Extract function name from the prototype
                omStrFnName =
                    pDoc->omStrExtractFunctionNameFromPrototype(
                        omStrSelectedText );
                // Timer callbacks have __stdcall prefixed to the function name
                // Trim it and get function name
                omStrFnName.TrimLeft();
                omStrFnName.TrimRight();
                int nIndex = omStrFnName.Find( _T(" "));

                if ( nIndex != -1 )
                {
                    omStrFnName =
                        omStrFnName.Right(
                            omStrFnName.GetLength() - (nIndex+1) );
                }

                // Form the message for the message box
                CString omStrMsg = defCONFIRM_DELETE_HANDLER;
                omStrMsg.Replace( _T("FUNCTION_NAME"), omStrFnName );
                nYesNo = AfxMessageBox( omStrMsg,MB_YESNO|MB_ICONINFORMATION);

                if (IDYES == nYesNo )
                {
                    // Delete the function from the source file
                    bReturnVal = bDeleteHandler(omStrFnName, pDoc);

                    // Delete Prototype declaration if it is a Util Fn
                    if( omStrSelectedText.Find(UTILS_PREFIX,0) != -1)
                    {
                        // Use the existing function to clear the line
                        CString omTemp = BUS_FN_PROTOTYPE_HDR;
                        omTemp.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                        bDeleteALineFromHeader(omTemp,
                                               omStrSelectedText);
                    }
                }
            }
            else
            {
                HTREEITEM hParItem = omTree.GetParentItem( hSelItem );

                if (hParItem != NULL )
                {
                    if (omTree.GetItemText( hParItem) == GLOBAL_VARIABLES )
                    {
                        nYesNo = AfxMessageBox( defCONFIRM_DELETE_GLOVAR,
                                                MB_YESNO|MB_ICONINFORMATION) ;

                        if (IDYES == nYesNo )
                        {
                            CString omTemp = BUS_VAR_HDR;
                            omTemp.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                            bReturnVal = bDeleteALineFromHeader(omTemp,
                                                                omStrSelectedText);
                        }
                    }
                    else if ( omTree.GetItemText( hParItem) == INCLUDE_HEADERS )
                    {
                        nYesNo = AfxMessageBox( defCONFIRM_DELETE_INCHDR,
                                                MB_YESNO|MB_ICONINFORMATION);

                        if (IDYES == nYesNo )
                        {
                            CString omTemp = BUS_INCLUDE_HDR;
                            omTemp.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                            bReturnVal = bDeleteALineFromHeader(
                                             omTemp,
                                             omStrSelectedText);
                        }
                    }
                }
            }
        }
        else if(omStrSelectedText == ERROR_HANDLERS )
        {
            // Invoke Error Handler dialog for deleting Handler
            CErrHandlerDlg od_Dlg(pDoc, NULL,TRUE);
            int nUserOption = (COMMANINT)od_Dlg.DoModal();

            // If user selects OK.
            if(nUserOption == IDOK)
            {
                int nCount = 0;
                CStringArray omStrErrorHandlersArray;
                // Get the count of error handlers to be delted.
                nCount = (COMMANINT)od_Dlg.m_omStrArrayErrorHandler.GetSize();

                // Delete error handlers
                for( int i = 0; i <nCount ; i++)
                {
                    // Construct Function name
                    CString omStrFuncName = defERROR_HANDLER_FN;
                    omStrFuncName += od_Dlg.m_omStrArrayErrorHandler.GetAt(i);
                    omStrErrorHandlersArray.Add(omStrFuncName);
                }

                bReturnVal =
                    bDeleteHandlerInArray(omStrErrorHandlersArray,pDoc);
            }
        }
        else if(omStrSelectedText == DLL_HANDLERS)
        {
            // Invoke Dialog to delete DLL handlers
            CDLLHandlerDlg od_Dlg(0, pDoc, NULL,TRUE);
            int nUserOption = (COMMANINT)od_Dlg.DoModal();

            if(nUserOption == IDOK)
            {
                int nCount = 0;
                CStringArray omStrDLLHandlersArray;
                nCount = (COMMANINT)od_Dlg.m_omStrArrayDLLHandler.GetSize();

                for( int i = 0; i <nCount ; i++)
                {
                    // Construct Function name
                    CString omStrFuncName = defDLL_HANDLER_FN ;
                    omStrFuncName += od_Dlg.m_omStrArrayDLLHandler.GetAt(i);
                    omStrDLLHandlersArray.Add(omStrFuncName) ;
                }

                bReturnVal = bDeleteHandlerInArray(omStrDLLHandlersArray,pDoc);
            }
        }

        if ( bReturnVal != FALSE)
        {
            // Update all views
            pDoc->UpdateAllViews( NULL );
            pDoc->SetModifiedFlag( TRUE );
            // Delete the selected item from the tree
            //omTree.DeleteItem( hSelItem );
            // Remove all the contents from the function view
            // if any, and set the view to read only
            // Get function view pointer
            CFunctionView* podFnView = CGlobalObj::ouGetObj(m_eBus).podGetFunctionViewPtr();

            if ( podFnView != NULL)
            {
                // Get ref to rich edit control
                CRichEditCtrl& omRichEdit =
                    podFnView->GetRichEditCtrl();
                // Set view to empty
                omRichEdit.SetWindowText( STR_EMPTY );
                // Read only view
                omRichEdit.SetReadOnly( TRUE );
                // Set the structure m_sStartPos, a private member variable
                // of CFunctionView to NULL to avoind poping up context menu
                // when view is read only.
                podFnView->vInitPositionStruct();
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnAddHandle                                           */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Adds a handler to the source code list                */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  07.03.2002                                            */
/******************************************************************************/

void CFnsTreeView::OnAddHandle()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CTreeCtrl& omTree = GetTreeCtrl();
    // Get slected item handle from the tree
    HTREEITEM hSelItem = omTree.GetSelectedItem();
    CString omStrFnName = STR_EMPTY;
    CString omStrSelectedText =
        omTree.GetItemText( hSelItem );

    // check the type of the handler selected by the user
    // and call appropriate function
    if ( omStrSelectedText == MESSAGE_HANDLERS )
    {
        vOnNewMessageHandler();
    }
    else if ( omStrSelectedText == TIMER_HANDLERS )
    {
        vOnNewTimerHandler();
    }
    else if ( omStrSelectedText == KEY_HANDLERS )
    {
        vOnNewKeyHandler();
    }
    else if ( omStrSelectedText == UTILITY_FUNCS )
    {
        vOnNewUtilityFunction();
    }
    else if ( omStrSelectedText == INCLUDE_HEADERS )
    {
        vOnNewIncludeHeader();
    }
    else if (omStrSelectedText == ERROR_HANDLERS )
    {
        vOnNewErrorHandler();
    }
    else if (omStrSelectedText == EVENTIND_HANDLERS)
    {
        vOnNewEventHandler();
    }
    else if (omStrSelectedText == DLL_HANDLERS )
    {
        vOnNewDLLHandler();
    }

    omTree.Expand( hSelItem, TVE_EXPAND );
    OnTreeItemDoubleClick(NULL, NULL);
}
/******************************************************************************/
/*  Function Name    :  vDeleteHandler                                        */
/*                                                                            */
/*  Input(s)         :  CString omStrFuncName, function name not the prototype*/
/*                      CFunctionEditorDoc* pDoc pointer to Document view     */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  Deletes the function from the source code list        */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/******************************************************************************/

BOOL CFnsTreeView::bDeleteHandler( CString omStrFuncName,
                                   CFunctionEditorDoc* pDoc)
{
    BOOL bReturn = FALSE;

    if (pDoc != NULL)
    {
        SBUS_SPECIFIC_INFO sBusSpecInfo;
        pDoc->bGetBusSpecificInfo(sBusSpecInfo);
        //Construct the Function Footer
        CString omStrFnFooter = BUS_FN_FOOTER;
        omStrFnFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
        omStrFnFooter.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"), omStrFuncName );
        //Construct the Function header
        CString omStrFnHeader = BUS_FN_HDR;
        omStrFnHeader.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
        omStrFnHeader.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"), omStrFuncName );
        CStringList omStrListTemp;
        BOOL bFlag = TRUE;
        CString omStr;
        // Find the header in the list
        POSITION pos = pDoc->m_omSourceCodeTextList.Find( omStrFnHeader );
        POSITION firstPos = pos;

        // Copy the function to be deleted to a temp list
        while ( pos != NULL && bFlag )
        {
            omStr = pDoc->m_omSourceCodeTextList.GetNext( pos );
            omStrListTemp.AddHead(omStr);

            if ( omStr == omStrFnFooter)
            {
                bFlag = FALSE;
            }
        }

        POSITION pos2 = omStrListTemp.GetHeadPosition();
        CString omTemp = STR_EMPTY;
        POSITION pos1;

        // Compare every item in the temp list
        // with the source code list, if found
        // delete the item from the source list
        while ( pos2 != NULL )
        {
            omTemp = omStrListTemp.GetNext( pos2 );
            pos1 = pDoc->m_omSourceCodeTextList.GetHeadPosition();
            // Find and delete the function
            // find from the header position
            pos1 = pDoc->m_omSourceCodeTextList.Find( omTemp, firstPos );

            if ( pos1 )
            {
                pDoc->m_omSourceCodeTextList.RemoveAt( pos1 );
            }
            else // delete the header itself
            {
                pDoc->m_omSourceCodeTextList.RemoveAt( firstPos );
            }
        }

        // Delete the function prototype from corresponding handler array
        CStringArray* pArray = NULL;

        if ( omStrFuncName.Find(CGlobalObj::omGetBusSpecMsgHndlrName(m_eBus) ) != -1 )
        {
            // Message handler
            pArray = pDoc->omStrGetMessageHandlerPrototypes();
        }
        else if (omStrFuncName.Find( defTIMER_HANDLER ) != -1)
        {
            // Timer handler
            pArray = pDoc->omStrGetTimerHandlerPrototypes();
        }
        else if (omStrFuncName.Find( defKEY_HANDLER ) != -1)
        {
            // Key Handler
            pArray = pDoc->omStrGetKeyHandlerPrototypes();
        }
        else if (omStrFuncName.Find( defERROR_HANDLER_FN ) != -1)
        {
            // Error Handler
            pArray = pDoc->pomStrGetErrorHandlerPrototypes();
        }
        else if (omStrFuncName.Find( defEVENT_IND_HANDLER ) != -1)
        {
            // Event Handler
            pArray = pDoc->omStrGetEventIndPrototypes();
        }
        else if (omStrFuncName.Find( defDLL_HANDLER_FN ) != -1)
        {
            // Dll Handler
            pArray = pDoc->pomStrGetDLLHandlerPrototypes();
        }
        else if (omStrFuncName.Find( defHASHINCLUDE ) != -1)
        {
            // include headers
            pArray = pDoc->omStrGetIncudeHeaderFiles();
        }
        else
        {
            pArray = pDoc->omStrGetUtilityFunctionPrototypes();
        }

        if ( pArray != NULL )
        {
            CString omStr = STR_EMPTY;

            for ( int nCount = 0; nCount < pArray->GetSize(); nCount++ )
            {
                omStr = pArray->GetAt( nCount );

                if ( omStr.Find( omStrFuncName ) != -1 )
                {
                    pArray->RemoveAt( nCount );
                    nCount = (COMMANINT)pArray->GetSize(); // Exit loop
                }
            }
        }

        bReturn = TRUE;
    }

    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  bDeleteALineFromHeader                                */
/*                                                                            */
/*  Input(s)         :  CString omStrHdr: Header String from which the search */
/*                      starts. omSearchStr: Search String                    */
/*  Output           :                                                        */
/*  Functionality    :  Deletes the variable from the source code list        */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/******************************************************************************/
BOOL CFnsTreeView::bDeleteALineFromHeader(CString omStrHeader,
        CString omSearchStr)
{
    CString omStrLine = STR_EMPTY;
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();
    SBUS_SPECIFIC_INFO sBusSpecInfo;
    pDoc->bGetBusSpecificInfo(sBusSpecInfo);

    if (pDoc != NULL)
    {
        // Seek to the start of global variable definition
        POSITION pos =
            pDoc->m_omSourceCodeTextList.Find( omStrHeader );

        // Find the required variable
        while ( pos != NULL )
        {
            POSITION posTobeDeleted = pos;
            omStrLine =
                pDoc->m_omSourceCodeTextList.GetNext(pos);

            if ( omStrLine.Find( omSearchStr ) != -1 )
            {
                pDoc->m_omSourceCodeTextList.RemoveAt( posTobeDeleted );
                pos = NULL;
            }
        }

        // Now remove the entry from the Global/Include array if the deleted
        // Line is a global variable or a include statement
        CStringArray* pArray = NULL;
        // Delete corresponding variable from the array
        CString omTemp = BUS_VAR_HDR;
        omTemp.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
        CString omTemp1 = BUS_INCLUDE_HDR;
        omTemp1.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);

        if ( omTemp == omStrHeader )
        {
            pArray = pDoc->omStrGetGlobalVariablePrototypes();
        }
        else if ( omTemp1 == omStrHeader )
        {
            pArray = pDoc->omStrGetIncudeHeaderFiles();
        }

        if ( pArray != NULL )
        {
            CString omStr = STR_EMPTY;

            for ( int nCount = 0; nCount < pArray->GetSize(); nCount++ )
            {
                omStr = pArray->GetAt( nCount );

                if ( omStr.Find( omSearchStr ) != -1 )
                {
                    pArray->RemoveAt( nCount );
                    nCount = (COMMANINT)pArray->GetSize(); // Exit loop
                }
            }
        }

        //
        pDoc->SetModifiedFlag( TRUE );
    }

    return TRUE;
}
/******************************************************************************/
/*  Function Name    :  OnNewMessageHandler                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays a dialog for the user to select the message
                        for the handler and adds the default code

/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/
void CFnsTreeView::vOnNewMessageHandler()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CMsgHandlerDlg od_Dlg(m_eBus);
    od_Dlg.DoModal();
}
/******************************************************************************/
/*  Function Name    :  vOnNewTimerHandler                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays a dialog for the user enter the new timer
                        handler and adds the default code

/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  07.03.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/

void CFnsTreeView::vOnNewTimerHandler()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();
    CDefineTimerHandler od_Dlg(pDoc);

    if (od_Dlg.DoModal() == IDOK )
    {
        // Construct Function definiton
        CString omStrFuncPrototype = defDEFAULT_TIMER_HANDLER_CODE;
        omStrFuncPrototype.Replace( _T("OnTimer_TIMERNAME_TIMERVALUE"),
                                    od_Dlg.m_omStrTimerFunctionName );
        // Construct the function header
        CString omStrHdr = BUS_FN_HDR;
        omStrHdr.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                          od_Dlg.m_omStrTimerFunctionName );
        // Construct the function footer
        CString omStrFooter = BUS_FN_FOOTER;
        omStrFooter.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                             od_Dlg.m_omStrTimerFunctionName );

        if ( pDoc != NULL )
        {
            SBUS_SPECIFIC_INFO sBusSpecInfo;
            pDoc->bGetBusSpecificInfo(sBusSpecInfo);
            // Replace Bus name which is missing before
            omStrHdr.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
            omStrFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
            // Add empty line
            //pDoc->m_omSourceCodeTextList.AddTail( STR_EMPTY );
            // Add header
            pDoc->m_omSourceCodeTextList.AddTail( omStrHdr );
            // Add fn definition
            pDoc->m_omSourceCodeTextList.AddTail( omStrFuncPrototype );
            // Insert the prototype into the tree under
            // key handler header
            CTreeCtrl& omTree = GetTreeCtrl();
            HTREEITEM hNew = omTree.InsertItem( omStrFuncPrototype,
                                                5, 5,
                                                omTree.GetSelectedItem());
            omTree.SelectItem( hNew );
            // Add body of the fn
            pDoc->m_omSourceCodeTextList.AddTail( _T("{") );
            pDoc->m_omSourceCodeTextList.AddTail( defTODO );
            // Add footer
            pDoc->m_omSourceCodeTextList.AddTail( omStrFooter );
            // Update key handler array
            CStringArray* pTmrArray =
                pDoc->omStrGetTimerHandlerPrototypes();

            if ( pTmrArray != NULL )
            {
                pTmrArray->Add(omStrFuncPrototype);
            }

            pDoc->UpdateAllViews( NULL );
            pDoc->SetModifiedFlag( TRUE );
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vEditTimerHandler                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays a dialog to edit the timer handler           */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  27.09.2006                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CFnsTreeView::vEditTimerHandler(HTREEITEM hItem)
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CTreeCtrl& om_tree = GetTreeCtrl();
    CString omStrVariable = om_tree.GetItemText( hItem );
    //for finding function name (middle of first and last UNDERSCORE)
    //and the timer vlaue
    int nIndex = omStrVariable.Find( defUNDERSCORE  );
    int nLastIndex = omStrVariable.ReverseFind( '_' );
    int nParanthesisIndex = omStrVariable.Find(OPEN_PARENTHESIS);
    int nSpaceIndex = omStrVariable.Find( SPACE );

    if(nIndex != -1)
    {
        //create dialog
        CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();
        CDefineTimerHandler od_Dlg(pDoc);
        //show the name of function
        CString omOldFuncName = omStrVariable.Mid(nIndex+1, nLastIndex-nIndex-1);
        od_Dlg.m_omEditStrTimerFuncName = omOldFuncName;
        CString omStrTimerVal =
            omStrVariable.Mid(nLastIndex+1, nParanthesisIndex-nLastIndex-1);
        od_Dlg.m_unEditTimerValue = StrToInt(omStrTimerVal);
        CString omOldFuncProtoType =
            omStrVariable.Mid(nSpaceIndex+1, nParanthesisIndex-nSpaceIndex-1);

        if (od_Dlg.DoModal() == IDOK)
        {
            // Construct Function definiton
            CString omStrFuncPrototype = defDEFAULT_TIMER_HANDLER_CODE;
            omStrFuncPrototype.Replace( _T("OnTimer_TIMERNAME_TIMERVALUE"),
                                        od_Dlg.m_omStrTimerFunctionName );
            //Old Function definiton
            CString omStrFuncPrototypeOld = omStrVariable;
            // Construct the function header
            CString omStrHdr = BUS_FN_HDR;
            omStrHdr.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                              od_Dlg.m_omStrTimerFunctionName );
            // Old function header
            CString omStrHdrOld = BUS_FN_HDR;
            omStrHdrOld.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                 omOldFuncProtoType );
            // Construct the function footer
            CString omStrFooter = BUS_FN_FOOTER;
            omStrFooter.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                 od_Dlg.m_omStrTimerFunctionName );
            // Old function footer
            CString omStrFooterOld = BUS_FN_FOOTER;
            omStrFooterOld.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                    omOldFuncProtoType );

            if ( pDoc != NULL )
            {
                SBUS_SPECIFIC_INFO sBusSpecInfo;
                pDoc->bGetBusSpecificInfo(sBusSpecInfo);
                // Replace Bus name which is missing before
                omStrHdr.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                omStrFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                omStrHdrOld.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                omStrFooterOld.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                //Find the previous header and replace it
                POSITION pos = pDoc->m_omSourceCodeTextList.Find(omStrHdrOld );

                if( pos != NULL )
                {
                    // Add header
                    pDoc->m_omSourceCodeTextList.SetAt( pos,omStrHdr );
                }

                pos = pDoc->m_omSourceCodeTextList.Find(omStrFuncPrototypeOld );

                if( pos != NULL )
                {
                    // Add fn definition
                    pDoc->m_omSourceCodeTextList.SetAt(pos,omStrFuncPrototype );
                }

                pos = pDoc->m_omSourceCodeTextList.Find(omStrFooterOld );

                if( pos != NULL )
                {
                    // Add footer
                    pDoc->m_omSourceCodeTextList.SetAt( pos,omStrFooter );
                }

                // Insert the prototype into the tree under
                // timer handler header
                // Add the prototype to the tree view
                CTreeCtrl& omTree = GetTreeCtrl();
                HTREEITEM hItem = omTree.GetSelectedItem();
                //HTREEITEM hNew =
                omTree.SetItemText( hItem,omStrFuncPrototype );
                omTree.SetItemImage( hItem, 5, 5 );
                // Update Timer handler array
                CStringArray* pTmrArray =
                    pDoc->omStrGetTimerHandlerPrototypes();

                if ( pTmrArray != NULL )
                {
                    BOOL bFound = FALSE;
                    int nTotalCount = (COMMANINT)pTmrArray->GetSize();
                    int nTempCount; //nTempCount declared outside

                    for(nTempCount = 0;
                            nTempCount < nTotalCount && bFound == FALSE;
                            nTempCount++)
                    {
                        if(pTmrArray->GetAt(nTempCount) == omStrFuncPrototypeOld)
                        {
                            bFound= TRUE;
                        }
                    }

                    if(bFound == TRUE)
                    {
                        pTmrArray->SetAt( nTempCount-1,omStrFuncPrototype );
                    }

                    // pTmrArray->Add(omStrFuncPrototype);
                }

                pDoc->UpdateAllViews( NULL );
                pDoc->SetModifiedFlag( TRUE );
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vOnNewKeyHandler                                      */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays a dialog for the user enter the new key      */
/*                      handler and adds the default code                     */
/*                                                                            */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/

void CFnsTreeView::vOnNewKeyHandler()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    char* pcCharacter = new char[2];
    pcCharacter[1] = '\0';

    if ( pcCharacter != NULL )
    {
        CFunctionEditorDoc* pDoc   = (CFunctionEditorDoc*)CView::GetDocument();
        CFnsTreeView* pFnsTreeView = CGlobalObj::ouGetObj(m_eBus).podGetFuncsTreeViewPtr();
        CKeyValue od_Dlg(pFnsTreeView, pDoc, NULL, pcCharacter);

        if ( od_Dlg.DoModal() == IDOK )
        {
            // Construct Function name
            CString omStrFuncName   = defKEY_HANDLER;
            CString omStrKeyPressed = "";

            if(pcCharacter[0] == '*')
            {
                omStrKeyPressed = "All";
            }
            else
            {
                omStrKeyPressed = pcCharacter[0];
            }

            omStrFuncName += omStrKeyPressed;
            // Construct Function definiton
            CString omStrFuncPrototype = defDEFAULT_KEY_HANDLER_CODE;
            omStrFuncPrototype.Replace(_T("KEYNAME"), omStrKeyPressed );
            // Form the function header
            CString omStrHdr = BUS_FN_HDR;
            omStrHdr.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                              omStrFuncName );
            // Form the function footer
            CString omStrFooter = BUS_FN_FOOTER;
            omStrFooter.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                 omStrFuncName );

            if ( pDoc != NULL )
            {
                SBUS_SPECIFIC_INFO sBusSpecInfo;
                pDoc->bGetBusSpecificInfo(sBusSpecInfo);
                // Replace Bus name which is missing before
                omStrHdr.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                omStrFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                // Add empty line
                //pDoc->m_omSourceCodeTextList.AddTail( STR_EMPTY );
                // Add header
                pDoc->m_omSourceCodeTextList.AddTail( omStrHdr );
                // Add fn definition
                pDoc->m_omSourceCodeTextList.AddTail( omStrFuncPrototype );
                // Insert the prototype into the tree under
                // key handler header
                CTreeCtrl& omTree = GetTreeCtrl();
                HTREEITEM hNew = omTree.InsertItem( omStrFuncPrototype,
                                                    5, 5,
                                                    omTree.GetSelectedItem());
                omTree.SelectItem( hNew );
                // Add body of the fn
                pDoc->m_omSourceCodeTextList.AddTail( _T("{") );
                pDoc->m_omSourceCodeTextList.AddTail( defTODO );
                // Add footer
                pDoc->m_omSourceCodeTextList.AddTail( omStrFooter );
                // Update key handler array
                CStringArray* pKeyArray =
                    pDoc->omStrGetKeyHandlerPrototypes();

                if ( pKeyArray != NULL )
                {
                    pKeyArray->Add(omStrFuncPrototype);
                }

                pDoc->UpdateAllViews( NULL );
                pDoc->SetModifiedFlag( TRUE );
            }
        }

        delete pcCharacter;
        pcCharacter = NULL;
    }
}
void CFnsTreeView::vOnNewEventHandler()
{
    CFunctionEditorDoc* pDoc   = (CFunctionEditorDoc*)CView::GetDocument();

    if (pDoc != NULL)
    {
        CTreeCtrl& omTreeCtrl = GetTreeCtrl();
        CEventHandlerDlg omDlg(omTreeCtrl, pDoc);
        omDlg.DoModal();
    }
}
/******************************************************************************/
/*  Function Name    :  vOnNewErrorHandler                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    : This function will be called to add Error handlers     */
/*                     A dialog (CErrHandlerDlg class) will be invoked for    */
/*                     the same                                               */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CFnsTreeView::vOnNewErrorHandler()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();

    if ( pDoc != NULL )
    {
        SBUS_SPECIFIC_INFO sBusSpecInfo;
        pDoc->bGetBusSpecificInfo(sBusSpecInfo);
        // Update key handler array
        CStringArray* pErrorArray =
            pDoc->pomStrGetErrorHandlerPrototypes();

        if ( pErrorArray != NULL )
        {
            if(pErrorArray->GetSize()< 5 )
            {
                CErrHandlerDlg od_Dlg(NULL, FALSE);

                if ( od_Dlg.DoModal() == IDOK )
                {
                    int nCount = (COMMANINT)od_Dlg.m_omStrArrayErrorHandler.GetSize();

                    for( int i = 0; i <nCount ; i++)
                    {
                        // Construct Function name
                        CString omStrFuncName = defERROR_HANDLER_FN;
                        omStrFuncName.Insert( omStrFuncName.GetLength(),
                                              od_Dlg.m_omStrArrayErrorHandler.GetAt(i));
                        // Construct Function definiton
                        CString omStrFuncPrototype = omStrFuncName;
                        omStrFuncPrototype.Insert( 0, SPACE );
                        omStrFuncPrototype.Insert( 0, defVOID );
                        omStrFuncPrototype.Insert(
                            omStrFuncPrototype.GetLength(), _T("(SCAN_ERR ErrorMsg)") );
                        // Form the function header
                        CString omStrHdr = BUS_FN_HDR;
                        omStrHdr.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                        omStrHdr.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                          omStrFuncName );
                        // Form the function footer
                        CString omStrFooter = BUS_FN_FOOTER;
                        omStrFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                        omStrFooter.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                             omStrFuncName );
                        pDoc->m_omSourceCodeTextList.AddTail( omStrHdr );
                        // Add fn definition
                        pDoc->m_omSourceCodeTextList.AddTail(
                            omStrFuncPrototype );
                        // Insert the prototype into the tree under
                        // error handler header
                        CTreeCtrl& omTree = GetTreeCtrl();
                        HTREEITEM hNew =
                            omTree.InsertItem( omStrFuncPrototype,
                                               5, 5,
                                               omTree.GetSelectedItem());
                        omTree.SelectItem( hNew );
                        // Add body of the fn
                        pDoc->m_omSourceCodeTextList.AddTail( _T("{") );
                        pDoc->m_omSourceCodeTextList.AddTail( defTODO );
                        // Add footer
                        pDoc->m_omSourceCodeTextList.AddTail( omStrFooter );
                        pErrorArray->Add(omStrFuncPrototype);
                    }

                    if(nCount >0)
                    {
                        pDoc->UpdateAllViews( NULL );
                        pDoc->SetModifiedFlag( TRUE );
                    }
                }
            }
        }
    }
}
void CFnsTreeView::vAddDeleteDllHandler(BOOL bDllLoad, BOOL bToAdd)
{
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*) CView::GetDocument();
    ASSERT(NULL != pDoc);
    SBUS_SPECIFIC_INFO sBusSpecInfo;
    pDoc->bGetBusSpecificInfo(sBusSpecInfo);
    CStringArray* pDLLArray = pDoc->pomStrGetDLLHandlerPrototypes();
    ASSERT(NULL != pDLLArray);
    // Construct Function name
    CString omStrFuncName = HANDLER_PRFX_DLL;
    omStrFuncName += (bDllLoad ? HANDLER_SFX_LOADDLL : HANDLER_SFX_UNLOADDLL);
    // Construct Function definiton
    CString omStrFuncPrototype = omStrFuncName;
    omStrFuncPrototype.Insert(0, SPACE);
    omStrFuncPrototype.Insert(0, defVOID);
    omStrFuncPrototype.Insert(omStrFuncPrototype.GetLength(), _T("()"));
    // Form the function header
    CString omStrHdr = BUS_FN_HDR;
    omStrHdr.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
    omStrHdr.Replace(_T("PLACE_HODLER_FOR_FUNCTIONNAME"), omStrFuncName);
    // Form the function footer
    CString omStrFooter = BUS_FN_FOOTER;
    omStrFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
    omStrFooter.Replace(_T("PLACE_HODLER_FOR_FUNCTIONNAME"), omStrFuncName);

    // Start updation of the document
    if (bToAdd)
    {
        pDoc->m_omSourceCodeTextList.AddTail(omStrHdr);        //        Header
        pDoc->m_omSourceCodeTextList.AddTail(omStrFuncPrototype);//  Definition
        pDoc->m_omSourceCodeTextList.AddTail( _T("{") );           // Function body
        pDoc->m_omSourceCodeTextList.AddTail(defTODO);         //       Comment
        pDoc->m_omSourceCodeTextList.AddTail(omStrFooter);     //        Footer
        pDLLArray->Add(omStrFuncPrototype);                    //     Prototype
    }
    else
    {
        bDeleteHandler(omStrFuncName, pDoc);
    }
}

/******************************************************************************/
/*  Function Name    :  vOnNewDLLHandler                                      */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  call this function to add new dll handlers. It will   */
/*                      call the dialog class and add handlers based on sel.  */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.03.2003                                            */
/*  Modifications    :  RaviKumar Patil                                       */
/******************************************************************************/
void CFnsTreeView::vOnNewDLLHandler()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();

    if ( pDoc != NULL )
    {
        SBUS_SPECIFIC_INFO sBusSpecInfo;
        pDoc->bGetBusSpecificInfo(sBusSpecInfo);
        // Update key handler array
        CStringArray* pDLLArray = pDoc->pomStrGetDLLHandlerPrototypes();

        if ( pDLLArray != NULL )
        {
            if(pDLLArray->GetSize()< 2 )
            {
                CDLLHandlerDlg od_Dlg(0, pDoc);

                if ( od_Dlg.DoModal() == IDOK )
                {
                    int nCount = (COMMANINT)od_Dlg.m_omStrArrayDLLHandler.GetSize();

                    for( int i = 0; i <nCount ; i++)
                    {
                        // Construct Function name
                        CString omStrFuncName = defDLL_HANDLER_FN;
                        omStrFuncName.Insert( omStrFuncName.GetLength(),
                                              od_Dlg.m_omStrArrayDLLHandler.GetAt(i));
                        // Construct Function definiton
                        CString omStrFuncPrototype = omStrFuncName;
                        omStrFuncPrototype.Insert( 0, SPACE );
                        omStrFuncPrototype.Insert( 0, defVOID );
                        omStrFuncPrototype.Insert(
                            omStrFuncPrototype.GetLength(), "()" );
                        // Form the function header
                        CString omStrHdr = BUS_FN_HDR;
                        omStrHdr.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                        omStrHdr.Replace(_T("PLACE_HODLER_FOR_FUNCTIONNAME"), omStrFuncName);
                        // Form the function footer
                        CString omStrFooter = BUS_FN_FOOTER;
                        omStrFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                        omStrFooter.Replace(_T("PLACE_HODLER_FOR_FUNCTIONNAME"), omStrFuncName);
                        pDoc->m_omSourceCodeTextList.AddTail( omStrHdr );
                        // Add fn definition
                        pDoc->m_omSourceCodeTextList.AddTail(omStrFuncPrototype );
                        // Insert the prototype into the tree under
                        // error handler header
                        CTreeCtrl& omTree = GetTreeCtrl();
                        HTREEITEM hNew =
                            omTree.InsertItem( omStrFuncPrototype,
                                               5, 5,
                                               omTree.GetSelectedItem());
                        omTree.SelectItem( hNew );
                        // Add body of the fn
                        pDoc->m_omSourceCodeTextList.AddTail( "{" );
                        pDoc->m_omSourceCodeTextList.AddTail( defTODO );
                        // Add footer
                        pDoc->m_omSourceCodeTextList.AddTail( omStrFooter );
                        pDLLArray->Add(omStrFuncPrototype);
                    }

                    if(nCount >0)
                    {
                        pDoc->UpdateAllViews( NULL );
                        pDoc->SetModifiedFlag( TRUE );
                    }
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  vOnNewUtilityFunction                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays a dialog for the user enter the new utility
                        function and adds the default code

/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/

void CFnsTreeView::vOnNewUtilityFunction()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();
    CUtlityDlg od_Dlg(pDoc);

    if (od_Dlg.DoModal() == IDOK)
    {
        // Form the function header
        CString omSelectedText = BUS_FN_HDR;

        if ( !omSelectedText.IsEmpty() )
        {
            if ( pDoc != NULL )
            {
                SBUS_SPECIFIC_INFO sBusSpecInfo;
                pDoc->bGetBusSpecificInfo(sBusSpecInfo);
                CString omFuncName =
                    pDoc->omStrExtractFunctionNameFromPrototype(
                        od_Dlg.m_omStrFunPrototype );
                //pDoc->m_omSourceCodeTextList.AddTail( STR_EMPTY );
                omSelectedText.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                omSelectedText.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                        omFuncName );
                pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
                // Add the function prototype
                pDoc->m_omSourceCodeTextList.AddTail(
                    od_Dlg.m_omStrFunPrototype );
                CString omStrPrototype = omSelectedText;
                //Add prototype to the prototype part of the doc
                CString omTemp = BUS_FN_PROTOTYPE_FOOTER;
                omTemp.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                POSITION pos = pDoc->m_omSourceCodeTextList.Find(
                                   omTemp );

                if ( pos != NULL )
                {
                    CString omStrReplace = "";
                    omStrReplace  = od_Dlg.m_omStrFunPrototype;
                    omStrReplace += SEMI_COLON;
                    pDoc->m_omSourceCodeTextList.InsertBefore(
                        pos,omStrReplace );
                }

                // Add the prototype to the tree view
                CTreeCtrl& omTree = GetTreeCtrl();
                HTREEITEM hItem = omTree.GetSelectedItem();
                HTREEITEM hNew =
                    omTree.InsertItem( od_Dlg.m_omStrFunPrototype, hItem);
                omTree.SetItemImage( hNew, 5, 5 );
                omTree.SelectItem( hNew );
                // Form the body of the function
                omSelectedText = "{";
                pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
                omSelectedText = defTODO;
                pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
                // Form the function footer
                omSelectedText = BUS_FN_FOOTER;
                omSelectedText.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                omSelectedText.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                        omFuncName );
                pDoc->m_omSourceCodeTextList.AddTail( omSelectedText );
                CStringArray* pUtilArray =
                    pDoc->omStrGetUtilityFunctionPrototypes();

                if ( pUtilArray != NULL )
                {
                    pUtilArray->Add( od_Dlg.m_omStrFunPrototype );
                }

                pDoc->UpdateAllViews( NULL );
                pDoc->SetModifiedFlag( TRUE );
            }
        }
    }

    // m_omUtiltiyFuncPrototype to be filled up
    // setmodifiedflag
}
/******************************************************************************/
/*  Function Name    :  vSetItemName                                          */
/*                                                                            */
/*  Input(s)         :  CString omStrName                                                     */
/*  Output           :                                                        */
/*  Functionality    :  Sets the item name to "omStrName"

/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  07.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
void CFnsTreeView::vSetItemName(CString omStrName)
{
    if ( !omStrName.IsEmpty())
    {
        // Eliminate ";" OR initialisation from the string
        int nIndex = omStrName.Find( _T("=") );

        if ( nIndex == -1 )
        {
            nIndex = omStrName.Find( _T(";") );
        }

        if (nIndex != -1 )
        {
            omStrName = omStrName.Left( nIndex );
        }

        omStrName.TrimRight();
        CTreeCtrl& om_tree = GetTreeCtrl();
        HTREEITEM hItem = om_tree.GetSelectedItem();

        if ( hItem != NULL )
        {
            // for new message, insert the new message name under root
            if ( hItem == om_tree.GetRootItem() )
            {
                HTREEITEM hNewItem = om_tree.InsertItem(omStrName, hItem );
                // select new message
                om_tree.SelectItem(hNewItem);
                // set its image
                om_tree.SetItemImage( hNewItem, 1, 1);
            }
            else
            {
                om_tree.SetItemText( hItem,omStrName );
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  vEditIncludeHeader                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays a dialog to edit the include header file     */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  13.02.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CFnsTreeView::vEditIncludeHeader(HTREEITEM hItem)
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // Separate the #include and the
    // filename
    CTreeCtrl& om_tree = GetTreeCtrl();
    CString omStrVariable = om_tree.GetItemText( hItem );
    int nIndex = omStrVariable.Find( SPACE );

    if ( nIndex != -1 )
    {
        CFunctionEditorDoc* pDoc =
            (CFunctionEditorDoc*)CView::GetDocument();
        // Initialise the dialog with the filename
        CIncludeHeaderDlg od_Dlg(pDoc);
        omStrVariable =
            omStrVariable.Right(omStrVariable.GetLength() - nIndex);
        omStrVariable.TrimLeft();
        od_Dlg.m_omStrIncludeFilename = omStrVariable;

        if ( od_Dlg.DoModal() == IDOK )
        {
            // Get the document
            SBUS_SPECIFIC_INFO sBusSpecInfo;
            pDoc->bGetBusSpecificInfo(sBusSpecInfo);

            if ( pDoc != NULL )
            {
                CString omTemp = BUS_INCLUDE_HDR;
                omTemp.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                POSITION pos =
                    pDoc->m_omSourceCodeTextList.Find( omTemp );
                omStrVariable = om_tree.GetItemText( hItem );
                // change corresponding array item in the array
                CStringArray* pArray =
                    pDoc->omStrGetIncudeHeaderFiles();
                int nIndex = -1;

                if ( pArray != NULL )
                {
                    // Seek to the required array element
                    for ( int nCount = 0;
                            nCount < pArray->GetSize();
                            nCount++ )
                    {
                        if (pArray->GetAt( nCount ) == omStrVariable)
                        {
                            // Found, save index
                            nIndex = nCount;
                            // exit loop
                            nCount = (COMMANINT)pArray->GetSize();
                        }
                    }
                }

                CString omStrGlobalVar = STR_EMPTY;
                POSITION prevPos = NULL;

                while ( pos != NULL )
                {
                    prevPos = pos;
                    // Get the variable
                    omStrGlobalVar =
                        pDoc->m_omSourceCodeTextList.GetNext( pos );

                    if ( omStrGlobalVar.Find(omStrVariable) !=  -1 )
                    {
                        od_Dlg.m_omStrIncludeFilename.Insert( 0,
                                                              defHASHINCLUDE );
                        // Set the value before the "pos"
                        pDoc->m_omSourceCodeTextList.SetAt( prevPos,
                                                            od_Dlg.m_omStrIncludeFilename );
                        // Get the name of the variable and insert the same into
                        // the tree view
                        om_tree.SetItemText( hItem,
                                             od_Dlg.m_omStrIncludeFilename );

                        // Update array
                        if ( nIndex != -1)
                            pArray->SetAt( nIndex,
                                           od_Dlg.m_omStrIncludeFilename );

                        pDoc->UpdateAllViews(NULL);
                        pDoc->SetModifiedFlag( TRUE );
                        pos = NULL;// break from loop
                    }// Find
                }// while
            }// if ( pDoc
        }//if ( od_Dlg.Do
    }//if ( nIndex !=
}
/******************************************************************************/
/*  Function Name    :  vEditUtilityFunc                                      */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays a dialog to edit the include util file       */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  27.09.2006                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CFnsTreeView::vEditUtilityFunc(HTREEITEM hItem)
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CTreeCtrl& om_tree = GetTreeCtrl();
    CString omStrVariable = om_tree.GetItemText( hItem );
    //for finding function name
    int nIndex = omStrVariable.Find( defUNDERSCORE );

    if(nIndex != -1)
    {
        int nLength = omStrVariable.GetLength();
        //create dialog
        CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();
        CUtlityDlg od_Dlg(pDoc);
        //show the name of function
        od_Dlg.m_omStrFunctionName = omStrVariable.Right(nLength - (nIndex+1));
        //to get the return type subtract the no. of char in UTILS_ from nIndex
        nIndex -= NO_OF_CHAR_IN_UTILS;
        //Set the data to inform that it is for editing the function
        od_Dlg.vSetUtilFuncChange(omStrVariable.Left(nIndex));

        if (od_Dlg.DoModal() == IDOK && (omStrVariable != od_Dlg.m_omStrFunPrototype))
        {
            //To remove the previous definations in fnEditor
            CString omPreviousText = BUS_FN_HDR;
            // Form the function header
            CString omSelectedText = BUS_FN_HDR;

            if ( !omSelectedText.IsEmpty() )
            {
                if ( pDoc != NULL )
                {
                    SBUS_SPECIFIC_INFO sBusSpecInfo;
                    pDoc->bGetBusSpecificInfo(sBusSpecInfo);
                    CString omStrFunctionName =
                        pDoc->omStrExtractFunctionNameFromPrototype(
                            omStrVariable );
                    CString omFuncName =
                        pDoc->omStrExtractFunctionNameFromPrototype(
                            od_Dlg.m_omStrFunPrototype );
                    //pDoc->m_omSourceCodeTextList.AddTail( STR_EMPTY );
                    omSelectedText.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                    omSelectedText.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                            omFuncName );
                    omPreviousText.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                    omPreviousText.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                            omStrFunctionName );
                    POSITION pos = pDoc->m_omSourceCodeTextList.Find(
                                       omPreviousText );

                    if(pos != NULL)
                    {
                        pDoc->m_omSourceCodeTextList.SetAt( pos,omSelectedText );
                    }

                    // Add the function prototype
                    pos = pDoc->m_omSourceCodeTextList.Find(
                              omStrVariable );

                    if(pos != NULL)
                    {
                        pDoc->m_omSourceCodeTextList.SetAt(pos,
                                                           od_Dlg.m_omStrFunPrototype );
                    }

                    CString omStrPrototype = omSelectedText;
                    //Add prototype to the prototype part of the doc
                    pos = pDoc->m_omSourceCodeTextList.Find(
                              omStrVariable + SEMI_COLON);

                    if ( pos != NULL )
                    {
                        CString omStrReplace = "";
                        omStrReplace  = od_Dlg.m_omStrFunPrototype;
                        omStrReplace += SEMI_COLON;
                        pDoc->m_omSourceCodeTextList.SetAt(pos,omStrReplace );
                    }

                    // Add the prototype to the tree view
                    CTreeCtrl& omTree = GetTreeCtrl();
                    HTREEITEM hItem = omTree.GetSelectedItem();
                    //HTREEITEM hNew =
                    omTree.SetItemText( hItem,od_Dlg.m_omStrFunPrototype );
                    omTree.SetItemImage( hItem, 5, 5 );
                    //omTree.SelectItem( hItem );
                    // Form the function footer
                    omSelectedText = BUS_FN_FOOTER;
                    omPreviousText = BUS_FN_FOOTER;
                    omSelectedText.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                    omSelectedText.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                            omFuncName );
                    omPreviousText.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                    omPreviousText.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                            omStrFunctionName );
                    pos = pDoc->m_omSourceCodeTextList.Find(
                              omPreviousText );

                    if(pos != NULL)
                    {
                        pDoc->m_omSourceCodeTextList.SetAt( pos,omSelectedText );
                    }

                    CStringArray* pUtilArray =
                        pDoc->omStrGetUtilityFunctionPrototypes();

                    if ( pUtilArray != NULL )
                    {
                        BOOL bFound = FALSE;
                        int nTotalCount = (COMMANINT)pUtilArray->GetSize();
                        int nTempCount;         //nTempCount declared outside for loop

                        for( nTempCount = 0;
                                nTempCount < nTotalCount && bFound == FALSE;
                                nTempCount++)
                        {
                            if(pUtilArray->GetAt(nTempCount) == omStrVariable)
                            {
                                bFound= TRUE;
                            }
                        }

                        if(bFound == TRUE)
                        {
                            pUtilArray->SetAt( nTempCount-1,od_Dlg.m_omStrFunPrototype );
                        }
                    }

                    pDoc->UpdateAllViews( NULL );
                    pDoc->SetModifiedFlag( TRUE );
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  vOnNewIncludeHeader                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Displays a dialog to add the include header file      */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  13.02.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CFnsTreeView::vOnNewIncludeHeader()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // Initialise the dialog with the filename
    CFunctionEditorDoc* pDoc =
        (CFunctionEditorDoc*)CView::GetDocument();
    CIncludeHeaderDlg od_Dlg(pDoc);

    if ( od_Dlg.DoModal() == IDOK )
    {
        // Get the document
        if ( pDoc != NULL )
        {
            SBUS_SPECIFIC_INFO sBusSpecInfo;
            pDoc->bGetBusSpecificInfo(sBusSpecInfo);
            CString omTemp = BUS_INCLUDE_FOOTER;
            omTemp.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
            POSITION pos =
                pDoc->m_omSourceCodeTextList.Find( omTemp );

            if ( pos != NULL )
            {
                CString omStrTemp = defHASHINCLUDE;
                omStrTemp += od_Dlg.m_omStrIncludeFilename;
                // Insert the variable before the variable footer
                pDoc->m_omSourceCodeTextList.InsertBefore( pos,
                        omStrTemp );
                CTreeCtrl& omTree = GetTreeCtrl();
                HTREEITEM hItem = omTree.GetSelectedItem();

                if ( hItem != NULL )
                {
                    omTree.InsertItem( omStrTemp, 5, 5, hItem  );
                }

                // add corresponding array item in the array
                CStringArray* pArray =
                    pDoc->omStrGetIncudeHeaderFiles();

                // Update array
                if ( pArray != NULL)
                {
                    pArray->Add( omStrTemp );
                }

                pDoc->UpdateAllViews(NULL);
                pDoc->SetModifiedFlag( TRUE );
            }// if ( pDoc
        }
    }
}

/******************************************************************************/
/*  Function Name    :  OnKeyDown                                             */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Highlights the selcted function or variable           */
/*                      in the fileview                                       */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  13.02.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CFnsTreeView::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
    TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
    // Get document
    CFunctionEditorDoc* pDoc =
        (CFunctionEditorDoc*)GetDocument();

    if ( pDoc != NULL)
    {
        CTreeCtrl& omTree = GetTreeCtrl();
        HTREEITEM hSelItem = omTree.GetSelectedItem();
        // Get the selected text
        CString omStrText = omTree.GetItemText( hSelItem );

        if (hSelItem != NULL)
        {
            // Correction required here since the tree item
            // does not return correct focused item
            if (pTVKeyDown->wVKey == VK_DOWN)
            {
                hSelItem = omTree.GetNextItem( hSelItem, TVGN_NEXTVISIBLE  );
            }
            else if (pTVKeyDown->wVKey == VK_UP)
            {
                hSelItem = omTree.GetNextItem( hSelItem, TVGN_PREVIOUSVISIBLE );
            }

            if (hSelItem != NULL)
            {
                // Get the selected text
                //CString
                omStrText = omTree.GetItemText( hSelItem );
                POSITION pos = pDoc->SetPosToFirstLine();
                UINT unLineCount = 0;

                while ( pos != NULL )
                {
                    CString omStrLine = pDoc->pcGetLine(pos);
                    unLineCount++;

                    // if the selected text in the tree view
                    // and the text returned by the list
                    // matches, then go to the definition
                    // of selected function
                    if ( omStrLine.Find(omStrText) != -1)
                    {
                        // go to the definition
                        // of selected function
                        POSITION pos = pDoc->GetFirstViewPosition();
                        CView* pView = NULL;

                        while(pos)
                        {
                            pView = pDoc->GetNextView(pos);

                            if(pView->IsKindOf(RUNTIME_CLASS(CFileView)))
                            {
                                break;
                            }
                        }

                        if(pView)
                        {
                            pView->SetActiveWindow();
                            ((CFileView*)pView)->vDisplayWarningLineNumber(
                                FNTREEVIEW, unLineCount);
                        }
                    }
                }
            }
        }
    }

    *pResult = 0;
}
/******************************************************************************/
/*  Function Name    :  OnUpdate                                              */
/*                                                                            */
/*  Input(s)         :  CView* pSender, LPARAM lHint, CObject* pHint          */
/*  Output           :                                                        */
/*  Functionality    :  Populates the tree with handlers

/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  26.04.2002                                            */
/*  Modifications    :
/******************************************************************************/
void CFnsTreeView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
    // Since update is called from other classes
    // as and when required, delete all previous
    // contents to eliminate copy of the same.
    CTreeCtrl& omTree = GetTreeCtrl();
    omTree.DeleteAllItems();
    bPopulateTree();
}
/******************************************************************************/
/*  Function Name    :  bDeleteHandlerInArray                                 */
/*                                                                            */
/*  Input(s)         :  const CStringArray &omStrArray: Handler names for del */
/*                      CFunctionEditorDoc* pDoc: Pointer to doc              */
/*  Output           :  TRUE/ FALSE                                           */
/*  Functionality    :  Delete the handlers selected in omStrArray by calling */
/*                      bDeleteHandler function is a loop                     */
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
BOOL CFnsTreeView::bDeleteHandlerInArray(const CStringArray& omStrArray,
        CFunctionEditorDoc* pDoc )
{
    BOOL bReturn = FALSE;

    if(pDoc != NULL )
    {
        INT nCount = (COMMANINT)omStrArray.GetSize();
        CString omStrFunctionName = "";

        for(int i = 0; i<nCount; i++)
        {
            bReturn =  bDeleteHandler(omStrArray.GetAt(i),pDoc);
        }
    }

    return bReturn;
}

/*******************************************************************************
 Function Name    : OnEditFunctionHeader
 Input(s)         :     -
 Output           :     -
 Functionality    : Displays a dialog to edit the Include File
 Member of        : CFnsTreeView
 Friend of        :      -
 Author(s)        : Raja N
 Date Created     : 31.05.2004
 Modifications    :
*******************************************************************************/
void CFnsTreeView::OnEditFunctionHeader()
{
    CTreeCtrl& om_tree = GetTreeCtrl();
    HTREEITEM hItem = om_tree.GetSelectedItem();

    if ( hItem != NULL )
    {
        // Get parent and decide which dialog to invoke
        HTREEITEM hParItem = om_tree.GetParentItem( hItem );

        if ( hParItem != NULL )
        {
            CString omStrTempHeader = om_tree.GetItemText( hParItem );

            // Get parent text
            if ( omStrTempHeader == INCLUDE_HEADERS )
            {
                vEditIncludeHeader(hItem);
            }
            else if ( omStrTempHeader == UTILITY_FUNCS )
            {
                vEditUtilityFunc(hItem);
            }
            else if ( omStrTempHeader == TIMER_HANDLERS )
            {
                vEditTimerHandler(hItem);
            }
        }
    }
}
void CFnsTreeView::OnFunctionEditorSave()
{
    // TODO: Add your command handler code here
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)GetDocument();

    if (pDoc != NULL)
    {
        pDoc->OnSaveDocument(pDoc->GetPathName());
    }
}
