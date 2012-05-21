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
 * \file      FunctionView.cpp
 * \brief     Implementation of CSignalValues Dialog class:
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CSignalValues Dialog class:
 */

#include "NodeSimEx_stdafx.h"   // Standard MFC includes
#include "NodeSimEx_resource.h"
#include "FunctionEditorDoc.h"  // Function Editor Document interfaces
#include "HashDefines.h"        // All #defines are here
#include "ExploreMsgSg.h"       // Dialog to select message or signal
#include "SelectFunctions.h"    // Dialog to select wrapper function
#include "EditFrameWnd.h"
#include "GlobalObj.h"
#include "functionview.h"

IMPLEMENT_DYNCREATE(CFunctionView, CRichEditView)

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : CFunctionView                                          */
/*                                                                           */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : Constructor                                            */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 25th February 2002                                     */
/*  Modifications   : Raja N on 12.05.2004                                   */
/*                    Added initialisation of the variables m_nCurrentLine   */
/*                    and m_nStartingLine                                    */
/*****************************************************************************/
CFunctionView::CFunctionView()
{
    m_omStrFnName      = "";
    m_bIsValidFunction = FALSE;
    m_sStartPos        = NULL;
    m_nStartingLine    = -1;
    m_nCurrentLine     = -1;
    m_eBus             = sm_eBus;
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : CFunctionView                                          */
/*                                                                           */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : Destructor                                             */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 25th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
CFunctionView::~CFunctionView()
{
}


BEGIN_MESSAGE_MAP(CFunctionView, CRichEditView)
    //{{AFX_MSG_MAP(CFunctionView)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdatePaste)
    ON_COMMAND(ID_EDIT_PASTE, OnPaste)
    ON_WM_CHAR()
    ON_WM_DROPFILES()
    ON_WM_KEYDOWN()
    ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
    ON_COMMAND(IDM_CONTEXT_INSERTMESSAGE, OnInsertMessage)
    ON_COMMAND(IDM_CONTEXT_INSERTSIGNAL, OnInsertSignal)
    ON_COMMAND(IDM_CONTEXT_INSERTFUNCTION, OnInsertFunction)
    ON_COMMAND(ID_EDIT_CUT, OnCut)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFunctionView message handlers


/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnInitialUpdate                                        */
/*                                                                           */
/*  Input(s)        : None                                                   */
/*  Output          : None                                                   */
/*  Functionality   : Called when the window is displayed for he first time  */
/*                    and performs the following actions:                    */
/*                    o    Sets the Word-wrap property to OFF,               */
/*                    o    Creates and sets the window font,                 */
/*                    o    Sets the View's attribute to read only,           */
/*                    o    Calls RevokeDragDrop() function to prevent from   */
/*                    o    dropping OLE objects to the Richeditview.         */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 25th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnInitialUpdate()
{
    CRichEditView::OnInitialUpdate();
    //Set Word Wrap OFF
    m_nWordWrap = WrapNone;
    //To turn on the horizontal scroll bar
    WrapChanged();
    //Try to create a Font
    BOOL bSuccess = m_omFont.CreateFont(10, 0, 0, 0, 400, FALSE, FALSE, 0,
                                        ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                                        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                        DEFAULT_PITCH | FF_MODERN, DEFAULT_FONT);

    //If succeeded, Set the Font
    if ( bSuccess )
    {
        SetFont(&m_omFont);
    }

    //Set the Richedit control to be Read-only
    GetRichEditCtrl().SetReadOnly();
    RevokeDragDrop(m_hWnd);
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : vSetFunctionToEdit                                     */
/*                                                                           */
/*  Input(s)        : omStrFunction : Name of the function to be edited      */
/*  Output          : None                                                   */
/*  Functionality   : This function is called by the function tree view when */
/*                    the user doubleclicks on a function name or a global   */
/*                    variable in the tree.                                  */
/*                    This function does the following actions:              */
/*                    o    Looks for the function header of the function to  */
/*                         be edited,                                        */
/*                    o    Searches for the opening braces and diplays the   */
/*                         lines in the view till the footer is found.       */
/*                    o    If footer is not found it displays the line as a  */
/*                         global variable.                                  */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 25th February 2002                                     */
/*  Modifications   : Raja N on 18.05.2004                                   */
/*                    Added check for global varaible block and set the      */
/*                    global function name accordingly                       */
/*****************************************************************************/
void CFunctionView::vSetFunctionToEdit(const CString& omStrFunction)
{
    m_omStrFnName = omStrFunction;
    CString omStrFnBody("");
    BOOL bGlobalVar = FALSE;
    m_bIsValidFunction = FALSE;
    m_sStartPos = NULL;
    CFunctionEditorDoc* pDoc = NULL;
    pDoc = (CFunctionEditorDoc*)CView::GetDocument();

    if ( pDoc != NULL )
    {
        SBUS_SPECIFIC_INFO sBusSpecInfo;
        pDoc->bGetBusSpecificInfo(sBusSpecInfo);
        CString omStrFnHeader, omStrFnFooter;

        // If it is a global variable block then set the block
        // with global variable boundary
        if( omStrFunction == GLOBAL_VARIABLES )
        {
            omStrFnHeader = BUS_VAR_HDR;
            omStrFnHeader.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
            omStrFnFooter = BUS_VAR_FOOTER;
            omStrFnFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
            bGlobalVar = TRUE;
        }
        else
        {
            //Construct the Function Header
            omStrFnHeader = BUS_FN_HDR;
            omStrFnHeader.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
            omStrFnHeader.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                   omStrFunction );
            //Construct the Function Footer
            omStrFnFooter = BUS_FN_FOOTER;
            omStrFnFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
            omStrFnFooter.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                   omStrFunction );
        }

        POSITION sPos = pDoc->m_omSourceCodeTextList.GetHeadPosition();
        int nLineNumber = 0;

        while ( sPos != NULL )
        {
            //Iterate through the Source Code String-List
            CString omStrLine = pDoc->m_omSourceCodeTextList.GetNext(sPos);
            // Increment the line count
            nLineNumber++;

            //If the current line matches the Function Header...
            //(means the starting of the function we are looking for)
            if ( omStrLine == omStrFnHeader )
            {
                if( bGlobalVar == FALSE)
                {
                    m_nStartingLine = nLineNumber;
                    //Skip Function name and parameters line
                    omStrLine = pDoc->m_omSourceCodeTextList.GetNext(sPos);
                    //Get Next line
                    omStrLine = pDoc->m_omSourceCodeTextList.GetNext(sPos);

                    //Opening brace indicates start of function body
                    if ( omStrLine.Find('{') != -1 )
                    {
                        //Store the start for later use
                        m_sStartPos = sPos;

                        //Loop through the function body till we encounter
                        //the function footer
                        while ( (sPos != NULL) && ( omStrLine != omStrFnFooter) )
                        {
                            omStrLine =
                                pDoc->m_omSourceCodeTextList.GetNext(sPos);

                            if ( omStrLine == omStrFnFooter )
                            {
                                m_bIsValidFunction = TRUE;
                            }
                            else
                            {
                                omStrFnBody += omStrLine;
                                omStrFnBody += '\n';
                            }
                        }
                    }
                }
                else
                {
                    //Store the start for later use
                    m_sStartPos = sPos;
                    m_nStartingLine = nLineNumber;

                    while ( (sPos != NULL) && ( omStrLine != omStrFnFooter) )
                    {
                        omStrLine = pDoc->m_omSourceCodeTextList.GetNext(sPos);

                        if ( omStrLine == omStrFnFooter )
                        {
                            m_bIsValidFunction = TRUE;
                        }
                        else
                        {
                            omStrFnBody += omStrLine;
                            omStrFnBody += '\n';
                        }
                    }
                }
            }
        }

        if ( m_bIsValidFunction )           //If a function name is set...
        {
            if ( !omStrFnBody.IsEmpty() )
            {
                //We got to remove the last '\n' that got added
                //when we constructed omStrFnBody from the source lines
                omStrFnBody.TrimRight();
                SetWindowText(omStrFnBody);
                GetRichEditCtrl().SetReadOnly(FALSE);
            }
        }
        else // Display Global Variable
        {
            SetWindowText(omStrFnBody);
            GetRichEditCtrl().SetReadOnly(TRUE);
        }
    }
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnContextMenu                                          */
/*                                                                           */
/*  Input(s)        : pWnd : Pointer to the window                           */
/*                    point: Co-ordinates to display the context menu.       */
/*  Output          : None                                                   */
/*  Functionality   : This function displays a context menu when the user    */
/*                    presses the conext key from the keyboard.              */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 26th February 2002                                     */
/*****************************************************************************/
void CFunctionView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if ( m_sStartPos != NULL ) // Display only if user has selected
    {
        // a function or variable to edit
        CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)CWnd::FromHandle(CGlobalObj::sm_hWndMDIParentFrame);
        CEditFrameWnd* podParentFrame   = NULL;
        CMDIChildWnd* podActiveChildWnd = NULL;

        if(pMainFrame != NULL )
        {
            // Get the current active window.
            podActiveChildWnd =
                (CMDIChildWnd*)pMainFrame->MDIGetActive();
            // Get the parent frame window of view
            podParentFrame = (CEditFrameWnd*)GetParentFrame();

            // Check the acitve child window. If it is CEditFrameWnd
            // then pop up the context menu.
            if(podParentFrame == podActiveChildWnd)
            {
                if ( point == CPoint(-1, -1) )
                {
                    if ( pWnd != NULL )
                    {
                        point = pWnd->GetCaretPos();
                    }
                }

                ClientToScreen(&point);
                CMenu omCtxMenu;

                if (omCtxMenu.LoadMenu(IDM_FNVIEW_CTXMENU))
                {
                    CMenu* pomCtxMenu = omCtxMenu.GetSubMenu(0);
                    int nDisable = MF_BYCOMMAND | MF_DISABLED | MF_GRAYED ;

                    // Disable insert Message and Signal if there is no active
                    //database
                    if( CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList.GetCount() == 0)
                    {
                        pomCtxMenu->EnableMenuItem(
                            IDM_CONTEXT_INSERTMESSAGE, nDisable );
                        pomCtxMenu->EnableMenuItem(
                            IDM_CONTEXT_INSERTSIGNAL, nDisable );
                    }

                    // Disable Insert Signal & Function in case of global
                    // variable section selected
                    if( m_omStrFnName == GLOBAL_VARIABLES )
                    {
                        pomCtxMenu->EnableMenuItem( IDM_CONTEXT_INSERTSIGNAL,
                                                    nDisable );
                        pomCtxMenu->EnableMenuItem( IDM_CONTEXT_INSERTFUNCTION,
                                                    nDisable );
                    }

                    if (pomCtxMenu != NULL)
                    {
                        pomCtxMenu->TrackPopupMenu(
                            TPM_RIGHTBUTTON | TPM_LEFTALIGN,
                            point.x, point.y,
                            AfxGetMainWnd());
                    }
                }
            }
        }
    }
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnRButtonUp                                            */
/*                                                                           */
/*  Input(s)        : nFlags : Virtual key (Shift, Ctrl...) status           */
/*                    point: Co-ordinates to display the context menu.       */
/*  Output          : None                                                   */
/*  Functionality   : This function displays a context menu when the user    */
/*                    clicks the right mouse button. This function in turn   */
/*                    sends a WM_CONTEXTMENU message.                        */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 26th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnRButtonUp(UINT nFlags, CPoint point)
{
    SendMessage(WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM(point.x, point.y));
    CRichEditView::OnRButtonUp(nFlags, point);
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnUpdatePaste                                          */
/*                                                                           */
/*  Input(s)        : pCmdUI : Indicates the menu item (IDC_EDIT).           */
/*  Output          : None                                                   */
/*  Functionality   : This function enables the paste command from the       */
/*                    context menu if the clipboard format is CF_TEXT.       */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 26th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnUpdatePaste(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( (::IsClipboardFormatAvailable(CF_TEXT))
                    && (m_bIsValidFunction) );
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnUpdatePaste                                          */
/*                                                                           */
/*  Input(s)        : None                                                   */
/*  Output          : None                                                   */
/*  Functionality   : This function pastes the contents from the clipboard   */
/*                    into the richedit control, updates the function body   */
/*                    in the document's source code list and notifies all    */
/*                    views that the document has changed.                   */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 26th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnPaste()
{
    (GetRichEditCtrl()).PasteSpecial(CF_TEXT);
    UpdateFileViewAndSetModified();
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnChar                                                 */
/*                                                                           */
/*  Input(s)        : nChar   : Contains the character code value of the key */
/*                    nRepCnt : Contains the repeat count                    */
/*                    nFlags  : Contains the scan code, key-transition code, */
/*                              previous key state, and context code.        */
/*  Output          : None                                                   */
/*  Functionality   : This function is invoked when the user presses a key   */
/*                    and calls the function UpdateFileViewAndSetModified(). */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 25th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CRichEditView::OnChar(nChar, nRepCnt, nFlags);
    // Update the File view and set modified flag to true
    UpdateFileViewAndSetModified();
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnDropFiles                                            */
/*                                                                           */
/*  Input(s)        : hDropInfo : Information of the dropped file.           */
/*  Output          : None                                                   */
/*  Functionality   : This function is needed to prevent the user from       */
/*                    dropping any files or OLE objects into the functionview*/
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 27th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnDropFiles(HDROP /*hDropInfo*/)
{
    return;
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnKeyDown                                              */
/*                                                                           */
/*  Input(s)        : nChar   : Contains the character code value of the key */
/*                    nRepCnt : Contains the repeat count                    */
/*                    nFlags  : Contains the scan code, key-transition code, */
/*                              previous key state, and context code.        */
/*  Output          : None                                                   */
/*  Functionality   : This function is needed to handle 'Delete key' and just*/
/*                    calls OnChar() with the VK_DELETE virtual key code.    */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 27th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if ( nChar == VK_DELETE )
    {
        OnChar(nChar, 0, 0);
    }
    else
    {
        CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : UpdateFunctionInDocument                               */
/*                                                                           */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : This function is takes the contents of the functionview*/
/*                    and updates the function in the document's source code */
/*                    list. This function does the following actions:        */
/*                    o    Deletes the function body from the sourcecode list*/
/*                    o    Add the required number of lines in sourcecodelist*/
/*                    o    Insert the lines from the Richedit control in the */
/*                         source code list.                                 */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 27th February 2002                                     */
/*  Modifications   : Raja N on 12.05.2004                                   */
/*                    Modified to include Global variable header in case of  */
/*                    Global Variables Update. Also added code to find the   */
/*                    current line number to set the focus in File View      */
/*****************************************************************************/
BOOL CFunctionView::UpdateFunctionInDocument()
{
    BOOL bRetVal    = FALSE;
    POSITION sStart = m_sStartPos;

    if ( sStart != NULL )
    {
        CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();

        if ( pDoc != NULL )
        {
            SBUS_SPECIFIC_INFO sBusSpecInfo;
            pDoc->bGetBusSpecificInfo(sBusSpecInfo);
            //Construct the Function Footer
            CString omStrFnFooter;

            // If it is global variable then select Global variable footer
            if( m_omStrFnName == GLOBAL_VARIABLES )
            {
                omStrFnFooter = BUS_VAR_FOOTER;
                omStrFnFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
            }
            else
            {
                // Select function common footer
                omStrFnFooter = BUS_FN_FOOTER;
                // Form function specific footer
                omStrFnFooter.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
                omStrFnFooter.Replace( _T("PLACE_HODLER_FOR_FUNCTIONNAME"),
                                       m_omStrFnName );
            }

            CString omStrLine("");
            // Get the Edit control ref.
            CRichEditCtrl& romEditCtrl = GetRichEditCtrl();
            // get the total lines of code in the rich edit control
            int nLineCount      = romEditCtrl.GetLineCount();
            long lStart, lEnd;
            // Get the cursor position
            romEditCtrl.GetSel(lStart, lEnd);
            // Get the cursor line and save
            m_nCurrentLine = (int) romEditCtrl.LineFromChar(lStart);
            BOOL bDone = FALSE;
            pDoc->m_omSourceCodeTextList.GetNext(sStart);
            POSITION sPos1  = NULL;
            POSITION sPos2  = NULL;

            for( sPos1 = sStart; ( ((sPos2 = sPos1) != NULL) && (!bDone) ); )
            {
                CString omStrDel =
                    pDoc->m_omSourceCodeTextList.GetNext( sPos1 );

                if( omStrDel == omStrFnFooter )
                {
                    bDone = TRUE;
                }
                else
                {
                    pDoc->m_omSourceCodeTextList.RemoveAt( sPos2 );
                }
            }

            BOOL bFirst = TRUE;
            POSITION sPos = m_sStartPos;

            for (int nLineIndex = 0; nLineIndex < nLineCount; nLineIndex++)
            {
                CString omStrNewItem("");
                int nCharIndex  = GetRichEditCtrl().LineIndex(nLineIndex);
                int nLineLength = GetRichEditCtrl().LineLength(nCharIndex);
                nLineLength = ( nLineLength < 4 ) ? 4 : nLineLength;
                GetRichEditCtrl().GetLine(nLineIndex,
                                          omStrNewItem.GetBuffer(nLineLength),
                                          nLineLength);
                omStrNewItem.ReleaseBuffer(nLineLength);
                omStrNewItem.TrimRight();

                if ( bFirst )
                {
                    pDoc->m_omSourceCodeTextList.SetAt(sPos, omStrNewItem);
                    bFirst = FALSE;
                }
                else
                {
                    pDoc->m_omSourceCodeTextList.InsertAfter(
                        sPos, omStrNewItem);
                    pDoc->m_omSourceCodeTextList.GetNext(sPos);
                }
            }

            bRetVal = TRUE;
        }
    }

    return bRetVal;
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnEditUndo                                             */
/*                                                                           */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : This function is takes the contents of the functionview*/
/*                    after undoing and updates the function in the          */
/*                    document's source code list                            */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 27th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnEditUndo()
{
    GetRichEditCtrl().Undo();
    UpdateFileViewAndSetModified();
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnInsertSignal                                         */
/*                                                                           */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : This function is the command handler for               */
/*                    IDM_CONTEXT_INSERTSIGNAL. This function displays a     */
/*                    CExploreMsgSg dialog to the user to select the signal  */
/*                    name to be inserted into the fucntion body.            */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 27th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnInsertSignal()
{
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();
    CExploreMsgSg od_Dlg(pDoc, CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList,
                         TRUE, FNVIEW, SEL_SIGNAL );

    if ( od_Dlg.DoModal() == IDOK )
    {
        // get reference to the RichEdit
        CRichEditCtrl& om_RichEdit = GetRichEditCtrl( );

        // Add selecetd message to the edit control
        if ( !od_Dlg.m_omStrSelectedItemText.IsEmpty())
        {
            om_RichEdit.ReplaceSel( ( LPCTSTR )od_Dlg.m_omStrSelectedItemText,
                                    TRUE );
            // Update all views and set modified flag to true
            UpdateFileViewAndSetModified();
        }
    }
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnInsertMessage                                        */
/*                                                                           */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : This function is the command handler for               */
/*                    IDM_CONTEXT_INSERTMESSAGE. This function displays a    */
/*                    CExploreMsgSg dialog to the user to select the message */
/*                    name to be inserted into the function body.            */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 27th February 2002                                     */
/*  Modifications   : Raja N on 12.05.2004                                   */
/*                    Remove data init code of global variable section       */
/*****************************************************************************/
void CFunctionView::OnInsertMessage()
{
    eSELTYPE eType =
        m_omStrFnName == GLOBAL_VARIABLES ? SEL_GLOBAL_MESSAGE : SEL_MESSAGE;
    CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();
    CExploreMsgSg od_Dlg(pDoc, CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList,
                         TRUE, FNVIEW, eType);

    if ( od_Dlg.DoModal() == IDOK )
    {
        // get reference to the RichEdit
        CRichEditCtrl& om_RichEdit = GetRichEditCtrl( );

        // Add selecetd message to the edit control
        if ( !od_Dlg.m_omStrSelectedItemText.IsEmpty())
        {
            // In case of global varaible section init of data members is not
            // required.
            if( m_omStrFnName == GLOBAL_VARIABLES)
            {
                // Remove Data Init
                CString omString = od_Dlg.m_omStrSelectedItemText;
                // Find the second {
                int pos1 = omString.ReverseFind(defCHAR_OPEN_PARENTHESIS);
                // Find first }
                int pos2 = omString.Find(defCHAR_CLOSE_PARENTHESIS);

                // If found
                if( pos1 != -1 && pos2 != -1 )
                {
                    // Remove data init
                    omString = omString.Left(pos1 - 2);
                    // Append close braces
                    omString += defCLOSE_PARENTHESIS;
                    omString += SEMI_COLON_STR;
                }

                // Set the text to the view
                om_RichEdit.ReplaceSel( omString, TRUE );
            }
            else
            {
                // Set the text with data init
                om_RichEdit.ReplaceSel(
                    ( LPCTSTR )od_Dlg.m_omStrSelectedItemText,
                    TRUE );
            }

            // Update all views and set modified flag to true
            UpdateFileViewAndSetModified();
        }
    }
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : OnInsertFunction                                       */
/*                                                                           */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : This function is the command handler for               */
/*                    IDM_CONTEXT_INSERTFUNCTION. This function displays a   */
/*                    CSelectFunctions dialog to the user to select the      */
/*                    function name to be inserted into the function body.   */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 27th February 2002                                     */
/*  Modifications   :                                                        */
/*                                                                           */
/*****************************************************************************/
void CFunctionView::OnInsertFunction()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CSelectFunctions od_Dlg(m_eBus);

    if ( od_Dlg.DoModal() == IDOK )
    {
        // get reference to the RichEdit
        CRichEditCtrl& om_RichEdit = GetRichEditCtrl( );

        // Add selecetd message to the edit control
        if ( !od_Dlg.m_omSelectedFunctionPrototype.IsEmpty())
        {
            om_RichEdit.ReplaceSel(
                ( LPCTSTR )od_Dlg.m_omSelectedFunctionPrototype,
                TRUE );
            // Update all views and set modified flag to true
            UpdateFileViewAndSetModified();
        }
    }
}

/*****************************************************************************/
/*                                                                           */
/*  Function Name   : UpdateFileViewAndSetModified                           */
/*                                                                           */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : This function calls  UpdateFunctionInDocument() to     */
/*                    update the function body in the document's source code */
/*                    list, informs all other views that the document has    */
/*                    been modified and sets the dirty flag of the document. */
/*                                                                           */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*                                                                           */
/*  Author(s)       : Soj Thomas                                             */
/*  Date Created    : 25th February 2002                                     */
/*  Modifications   : Raja N on 12.05.2004                                   */
/*                    Added code to select the editing line in the File View */
/*****************************************************************************/
void CFunctionView::UpdateFileViewAndSetModified()
{
    // If document is updated successfully then,
    // update the view
    if ( UpdateFunctionInDocument())
    {
        CFunctionEditorDoc* pDoc = (CFunctionEditorDoc*)CView::GetDocument();

        if ( pDoc != NULL )
        {
            //pomMainFrm->CGlobalObj::podGetFunctionEditorDoc()->Invalidate();
            CFileView* pFileView = CGlobalObj::ouGetObj(m_eBus).podGetFileViewPtr();

            if (pFileView != NULL)
            {
                pFileView->OnUpdate( NULL, 0, NULL );
                pFileView->vGoToLine( m_nStartingLine + m_nCurrentLine );
            }

            pDoc->SetModifiedFlag( TRUE );
        }
    }
}
/*****************************************************************************/
/*  Function Name   : OnCut                                                  */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : Calls rich edit control's cut function and updates the */
/*                    same in the file view.                                 */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*  Author(s)       : Amarnath S                                             */
/*  Date Created    : 27th December 2002                                     */
/*  Modifications   :                                                        */
/*****************************************************************************/
void CFunctionView::OnCut()
{
    GetRichEditCtrl().Cut();
    UpdateFileViewAndSetModified();
}
/*****************************************************************************/
/*  Function Name   : vInitPositionStruct                                    */
/*  Input(s)        : None.                                                  */
/*  Output          : None                                                   */
/*  Functionality   : Initialise the member variable m_sStartPos to NULL.    */
/*                    This function should be called after making the view as*/
/*                    read only.                                             */
/*  Member of       : CFunctionView                                          */
/*  Friend of       : None                                                   */
/*  Author(s)       : Amitesh Bharti                                         */
/*  Date Created    : 12.05.2003                                             */
/*  Modifications   :                                                        */
/*****************************************************************************/
void CFunctionView::vInitPositionStruct()
{
    m_sStartPos = NULL;
}

