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
 * \file      ExploreMsgSg.cpp
 * \brief     This file contain definition of all function of
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "NodeSimEx_stdafx.h"             // Standard include header file
#include "NodeSimEx_resource.h"
#include "FunctionEditorDoc.h"  // Document defintion file
#include "ExploreMsgSg.h"       // Class defintion file

/******************************************************************************/
/*  Function Name    :  CExploreMsgSg                                         */
/*                                                                            */
/*  Input(s)         :  enumerator to indicate selection of message or signal */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :
/******************************************************************************/

CExploreMsgSg::CExploreMsgSg( CFunctionEditorDoc* pDoc,
                              CMsgNameMsgCodeList& odMsgNameCodelist,
                              BOOL bCheckRequired,
                              eMESSAGEFROM eWindow,
                              eSELTYPE eSelType,
                              CWnd* pParent /*=NULL*/)
    : CDialog(CExploreMsgSg::IDD, pParent),
      m_odMsgNameCodeList(odMsgNameCodelist)
{
    //{{AFX_DATA_INIT(CExploreMsgSg)
    m_bWantStructure = FALSE;
    //}}AFX_DATA_INIT
    m_eSelectType       = eSelType;
    m_bWantStructure    = FALSE;
    m_bCheckRequired    = bCheckRequired;
    m_eWindow           = eWindow;
    m_omStrMessageName  = STR_EMPTY;
    m_pDoc              = pDoc;
}


void CExploreMsgSg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExploreMsgSg)
    DDX_Control(pDX, IDC_LSTB_SIGNAL, m_omSignalListBox);
    DDX_Control(pDX, IDC_LSTC_MSGS, m_omMsgList);
    DDX_Check(pDX, IDC_CHKB_WANT_STRUCTURE, m_bWantStructure);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExploreMsgSg, CDialog)
    //{{AFX_MSG_MAP(CExploreMsgSg)
    ON_NOTIFY(NM_DBLCLK, IDC_LSTC_MSGS, OnDblclkListMsgs)
    ON_LBN_DBLCLK(IDC_LSTB_SIGNAL, OnDblclkListSignals)
    ON_BN_CLICKED(IDC_CBTN_OK, OnSelect)
    ON_BN_CLICKED(IDC_CBTN_CANCEL, OnCbtnCancel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_MSGS, OnItemchangedLstcMsgs)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_MSGS, OnClickLstcMsgs)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExploreMsgSg message handlers

/******************************************************************************/
/*  Function Name    :  OnDblclkListMsgs                                      */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user double clicks on a message,      */
/*                      and calls OnSelect() function                         */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Amitesh Bharti,                                       */
/*                      20.05.2003, call OnSelect to close the dialog only if */
/*                      user wants to select message.                         */
/*  Modifications    :  Raja N                                                */
/*                      11.03.2004, Set the focus to the last selected item   */
/*                      if the user double clicks outside                     */
/*  Modifications    :  Raja N on 31.05.2004                                  */
/*                      Added check for message list view dbl click on msg for*/
/*                      Global variable addition                              */
/******************************************************************************/
void CExploreMsgSg::OnDblclkListMsgs(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    if(m_omMsgList.GetNextItem(-1, LVNI_SELECTED) == -1)
    {
        m_omMsgList.SetItemState(m_nMsgIndex, LVNI_SELECTED | LVNI_FOCUSED,
                                 LVNI_SELECTED | LVNI_FOCUSED);
    }

    if ( m_eSelectType == SEL_MESSAGE || m_eSelectType == SEL_GLOBAL_MESSAGE )
    {
        OnSelect();
    }

    *pResult = 0;
}
/******************************************************************************/
/*  Function Name    :  OnDblclkListSignals                                   */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user double clicks on a signal,       */
/*                      and calls OnSelect() function                         */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Amitesh Bharti,                                       */
/*                      20.05.2003, call OnSelect to close the dialog only if */
/*                      user wants to select singal.                          */
/******************************************************************************/

void CExploreMsgSg::OnDblclkListSignals()
{
    if ( m_eSelectType == SEL_SIGNAL )
    {
        OnSelect();
    }
}
/******************************************************************************/
/*  Function Name    :  OnSelect                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user clicks on Select button. This
                        function gets the selected message or signal.
                        Associates signal with the message if the selection
                        mode is SEL_SIGNAL. Also forms the structure variable
                        if that option is checked for that message
                                                                              */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Amitesh Bharti
                        13.06.2002 While making message structure string,
                        the message name is replace with MSG_STRUCT_VAR for
                        both message selection and signal selection if user
                        choose to create message structure.Space and Semi Colon
                        is removed from #define MSG_STRUCT_VAR and put
                        seperatly.
/*  Modifications    :  Raja N                                                */
/*                      10/03/2004 Modified the message and signal structure  */
/*                      creation to include init of database messages.        */
/*  Modifications    :  Raja N on 31.05.2004                                  */
/*                      Added code to insert message variable with structure  */
/*                      for Global variable addition                          */
/******************************************************************************/

void CExploreMsgSg::OnSelect()
{
    UpdateData(TRUE);
    UCHAR ucChannelId = '1';
    // Get Main Frame Window Pointer
    CFunctionEditorDoc* pDoc = NULL;

    // For global variable addition structure is required
    if( m_eSelectType == SEL_GLOBAL_MESSAGE)
    {
        // Same as select message
        m_eSelectType = SEL_MESSAGE;
        // Enable structure definition
        m_bWantStructure = TRUE;
    }

    if(IsDlgButtonChecked(IDC_RBTN_CHANNEL2))
    {
        ucChannelId='2';
    }

    // Get the Document Object
    if (m_bWantStructure == TRUE)
    {
        pDoc = m_pDoc;
    }

    if ( m_eSelectType == SEL_MESSAGE )
    {
        // Get selected message text
        m_omStrMessageName = m_omStrSelectedItemText =
                                 m_omMsgList.GetItemText( m_nMsgIndex, 0);
        UINT unMsgID = (COMMANUINT)m_omMsgList.GetItemData(m_nMsgIndex);

        if (m_bWantStructure == TRUE)
        {
            // Get the Initialised string from document
            if (pDoc != NULL)
            {
                //To pass the actual name of message
                int nIndex = m_omStrMessageName.ReverseFind(defMSGID_NAME_START_CHAR);

                if(nIndex != -1)
                {
                    m_omStrSelectedItemText = m_omStrMessageName.Left(nIndex);
                    m_omStrMessageName = m_omStrSelectedItemText;
                }

                CString omStrMsgStructure =
                    pDoc->omStrGetInitialisedMessage(unMsgID,
                                                     m_omStrSelectedItemText,
                                                     MSG_STRUCT_VAR,TRUE,ucChannelId);
                m_omStrSelectedItemText = omStrMsgStructure;
            }
        }
    }
    else
    {
        // User wants signal to be selected
        CString omStrMsg = m_omMsgList.GetItemText(m_nMsgIndex, 0);
        UINT unMsgID = (COMMANUINT)m_omMsgList.GetItemData(m_nMsgIndex);
        int nSgIndex = (COMMANUINT)m_omSignalListBox.GetCurSel();

        if ( nSgIndex != -1 )
        {
            // Get selected signal text
            CString omStrSgName = STR_EMPTY;
            m_omSignalListBox.GetText( nSgIndex, omStrSgName );

            if ( m_bWantStructure )
            {
                omStrSgName.Insert( 0, (TCHAR)PERIOD );
                ////To pass the actual name of message
                int nIndex = omStrMsg.ReverseFind(defMSGID_NAME_START_CHAR);

                if( nIndex!=NULL )
                {
                    omStrMsg = omStrMsg.Left(nIndex);
                }

                // Get the Initialised string from document
                CString omStrMsgStructure =
                    pDoc->omStrGetInitialisedMessage(unMsgID,
                                                     omStrMsg,
                                                     MSG_STRUCT_VAR,TRUE,ucChannelId);
                // Form the declaration and signal access statements
                m_omStrSelectedItemText.Format(defFNS_INIT_SIG_FORMAT,
                                               omStrMsgStructure,
                                               MSG_STRUCT_VAR,
                                               defSIGNALMEMBER);
            }

            m_omStrSelectedItemText += omStrSgName;
        }
    }

    CDialog::OnOK();
}

/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Initialises the dialog with all the messages of active*/
/*                      database. Disables signal ist if the user asks for the*/
/*                      message name.                                         */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  01.03.2002                                            */
/*  Modifications    :  Raja N on 11.03.2004                                  */
/*                      Included init of m_nMsgIndex to the selected item     */
/*  Modifications    :  Raja N on 31.05.2004                                  */
/*                      Added new type to have message list view alone for    */
/*                      Global variable addition                              */
/*  Modifications    :  Raja N on 04.06.2004                                  */
/*                      Corrected the positioning problem with control in     */
/*                      case of font change.                                  */
/******************************************************************************/
BOOL CExploreMsgSg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Disable the chck if not required
    if ( m_bCheckRequired )
    {
        GetDlgItem(IDC_CHKB_WANT_STRUCTURE)->EnableWindow( TRUE );
    }
    else
    {
        GetDlgItem(IDC_CHKB_WANT_STRUCTURE)->EnableWindow( FALSE );
    }

    // Disable the signal list box
    // if the user wants message list only
    if ( m_eSelectType == SEL_MESSAGE )
    {
        GetDlgItem(IDC_LSTB_SIGNAL)->EnableWindow( FALSE );
    }
    else if( m_eSelectType == SEL_GLOBAL_MESSAGE)
    {
        CRect omDRect, omLRect, omBRect;
        CWnd* pWnd = GetDlgItem(IDC_CHKB_WANT_STRUCTURE);

        if( pWnd != NULL )
        {
            // Hide want structure item
            pWnd->ShowWindow(SW_HIDE);
            // Store window rect to resize the list control
            pWnd->GetWindowRect(&omBRect);
            ScreenToClient(&omBRect);
            // Get Dialog size
            GetWindowRect(&omDRect);
            //Get Channel selection radio buttons
            pWnd = GetDlgItem(IDC_STAT_SELCHANNEL);

            if (pWnd != NULL)
            {
                pWnd->ShowWindow(FALSE);
            }

            pWnd = GetDlgItem(IDC_RBTN_CHANNEL1);

            if (pWnd != NULL)
            {
                pWnd->ShowWindow(FALSE);
            }

            pWnd = GetDlgItem(IDC_RBTN_CHANNEL2);

            if (pWnd != NULL)
            {
                pWnd->ShowWindow(FALSE);
            }

            // Get list control size
            pWnd = GetDlgItem(IDC_LSTC_MSGS);

            if( pWnd != NULL )
            {
                // Convert it in to client co ordiantes
                pWnd->GetWindowRect(&omLRect);
                ScreenToClient(&omLRect);
                // The new bottom equal to the check box item bottom
                omLRect.bottom = omBRect.bottom;
                pWnd->MoveWindow(omLRect);
                // Change the postion of select button
                pWnd = GetDlgItem(IDC_CBTN_OK);

                if( pWnd != NULL )
                {
                    pWnd->GetWindowRect(&omBRect);
                    ScreenToClient(&omBRect);
                    // Reduce the size by half and add right margin
                    int nDialogWidth = (omDRect.Width() / 2) + omLRect.left - 1;
                    MoveWindow( omDRect.left, omDRect.top,
                                nDialogWidth, omDRect.Height());
                    // Move Select button
                    nDialogWidth = nDialogWidth / 2 - omBRect.Width() / 2;
                    pWnd->MoveWindow( nDialogWidth, omBRect.top,
                                      omBRect.Width(), omBRect.Height());
                    // Remove the text "Signal" from the caption
                    SetWindowText(defSTR_INSERT_GLOBAL_DLG_CAPTION);
                }
            }
        }
    }
    else
    {
        GetDlgItem(IDC_LSTB_SIGNAL)->EnableWindow( TRUE );
    }

    //  Get the msg list rectangle
    RECT rRect;
    GetDlgItem(IDC_LSTC_MSGS)->GetWindowRect(&rRect);
    ScreenToClient(&rRect);
    // Insert a column to the list control
    m_omMsgList.InsertColumn( 0, STR_EMPTY, LVCFMT_LEFT, rRect.bottom );
    m_omMsgList.DeleteAllItems();
    // Init last selection to -1
    m_nMsgIndex = -1;

    if (m_odMsgNameCodeList.GetCount() > 0)
    {
        POSITION pos = m_odMsgNameCodeList.GetHeadPosition();
        int nIndex = 0;

        while (pos != NULL)
        {
            SMSG_NAME_CODE& sMsgNameCode = m_odMsgNameCodeList.GetNext(pos);
            CString omMsgWithId;
            omMsgWithId.Format("%s[0x%x]", sMsgNameCode.m_omMsgName, sMsgNameCode.m_dwMsgCode);
            int nInsertedIndex = m_omMsgList.InsertItem( nIndex++, omMsgWithId);
            m_omMsgList.SetItemData(nInsertedIndex, sMsgNameCode.m_dwMsgCode);
        }

        m_nMsgIndex = 0;
        m_omMsgList.SetItemState(m_nMsgIndex, LVIS_SELECTED|LVIS_FOCUSED,
                                 LVIS_SELECTED|LVIS_FOCUSED);
    }

    //Set Default channel as Channel 1
    CButton* pButtn=(CButton*)GetDlgItem(IDC_RBTN_CHANNEL1);
    pButtn->SetCheck(BST_CHECKED);
    return FALSE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Calls base class OnCanCel function                    */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.04.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CExploreMsgSg::OnCbtnCancel()
{
    CDialog::OnCancel();
}
/******************************************************************************/
/*  Function Name    :  OnItemchangedLstcMsgs                                 */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user changes selection on a message,  */
/*                      and displays corresponding signals in signal list     */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  20.01.2004                                            */
/*  Modifications    :  Raja N on 11.03.2004                                  */
/*                      Included assignment of selected item variable while   */
/*                      the item selection got changed                        */
/******************************************************************************/
void CExploreMsgSg::OnItemchangedLstcMsgs(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // User wants the signal details
    if ( m_eSelectType == SEL_SIGNAL )
    {
        if(pNMListView->uNewState == (LVIS_FOCUSED |LVIS_SELECTED) )
        {
            // Delete every other item from the list box.
            m_omSignalListBox.ResetContent();
            // Get handle to selected message item
            int hSelItem = pNMListView->iItem;

            if ( (hSelItem != -1) )
            {
                CStringList omSignalNames;
                omSignalNames.RemoveAll();
                // Store last selection
                m_nMsgIndex = hSelItem;
                // Get selected message name
                m_omStrMessageName = m_omMsgList.GetItemText( hSelItem, 0 );
                DWORD dwMsgCode = (COMMANDWORD)m_omMsgList.GetItemData(hSelItem);
                vGetSigNamesFromMsgCode(dwMsgCode, omSignalNames);
                // Add signal names into the list box
                POSITION pos = omSignalNames.GetHeadPosition();

                while ( pos != NULL )
                {
                    m_omSignalListBox.AddString( omSignalNames.GetNext(pos));
                }

                m_omSignalListBox.SetCurSel( 0 );
                // Set horizontal extent of the list
                // box to max string availaable
                // so that user can scroll
                CSize   sz;
                CString omStrText;
                CDC*  pDC = m_omSignalListBox.GetDC();

                if ( pDC != NULL)
                {
                    int nDx = 0;

                    for (int nCount = 0;
                            nCount < m_omSignalListBox.GetCount();
                            nCount++)
                    {
                        m_omSignalListBox.GetText( nCount, omStrText );
                        // remove space
                        omStrText.TrimRight();
                        sz = pDC->GetTextExtent(omStrText);

                        if (sz.cx > nDx)
                        {
                            nDx = sz.cx;
                        }
                    }

                    m_omSignalListBox.ReleaseDC(pDC);
                    // Set the horizontal extent so every character of all
                    // strings can be scrolled to.
                    m_omSignalListBox.SetHorizontalExtent(nDx);
                }
            }
        }
    }
    else
    {
        // Store last selection
        m_nMsgIndex = pNMListView->iItem ;
    }

    *pResult = 0;
}

/******************************************************************************/
/*  Function Name    :  OnClickLstcMsgs                                       */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user changes selection on a message,  */
/*                      and this will store the selection. If the user clicks */
/*                      clicks outside this will set the focus to the last    */
/*                      selected item                                         */
/*  Member of        :  CExploreMsgSg                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Raja N                                                */
/*  Date Created     :  11.03.2004                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CExploreMsgSg::OnClickLstcMsgs(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    int nIndex = m_omMsgList.GetNextItem(-1, LVNI_SELECTED);

    if( nIndex == -1)
    {
        m_omMsgList.SetItemState(m_nMsgIndex, LVNI_SELECTED | LVNI_FOCUSED,
                                 LVNI_SELECTED | LVNI_FOCUSED);
    }
    else
    {
        m_nMsgIndex = nIndex;
    }

    *pResult = 0;
}

void CExploreMsgSg::vGetSigNamesFromMsgCode(DWORD dwMsgCode, CStringList& omSignalNames)
{
    SMSG_NAME_CODE sMsgCodeName;
    omSignalNames.RemoveAll();
    sMsgCodeName.m_dwMsgCode = dwMsgCode;
    POSITION pos = m_odMsgNameCodeList.Find(sMsgCodeName);

    if (pos != NULL)
    {
        sMsgCodeName = m_odMsgNameCodeList.GetAt(pos);
        omSignalNames.AddTail(&(sMsgCodeName.m_omSignalNames));
    }
}
