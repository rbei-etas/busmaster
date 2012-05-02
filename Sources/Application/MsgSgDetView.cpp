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
 * \file      MsgSgDetView.cpp
 * \brief     This file contain definition of all function of
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "stdafx.h"
#include "BUSMASTER.h"
#include "MsgSgDetView.h"
#include "MainFrm.h"
#include "SignalDetailsDlg.h"
#include "ValueDescriptionDlg.h"
#include "Datatype.h"

extern CCANMonitorApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMsgSgDetView

IMPLEMENT_DYNCREATE(CMsgSgDetView, CFormView)
/******************************************************************************/
/*  Function Name    :  CMsgSgDetView                                         */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

CMsgSgDetView::CMsgSgDetView()
    : CFormView(CMsgSgDetView::IDD)
{
    //{{AFX_DATA_INIT(CMsgSgDetView)
    m_omStrMessageName = "";
    m_unMessageLength = 0;
    m_omStrMessageCode = "";
    m_unNoOfSgs = 0;
    m_nFrameFormat = -1;
    m_nDataFormat = DATA_FORMAT_INTEL;
    //}}AFX_DATA_INIT
    m_sDbParams = sm_sDbParams;
}
/******************************************************************************/
/*  Function Name    :  ~CMsgSgDetView                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Destructor
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

CMsgSgDetView::~CMsgSgDetView()
{
}

void CMsgSgDetView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMsgSgDetView)
    //  DDX_Control(pDX, IDC_SPIN_MSG_LEN, m_omSpinMsgLen);
    DDX_Control(pDX, IDC_LSTC_SGIDVAL, m_omListCtrlSgIdVal);
    DDX_Control(pDX, IDC_LSTC_SIGNAL_DETAILS, m_omListCtrlSignal);
    DDX_Text(pDX, IDC_EDIT_MSG_NAME, m_omStrMessageName);
    DDX_Text(pDX, IDC_EDIT_MSG_LENGTH, m_unMessageLength);
    DDV_MinMaxUInt(pDX, m_unMessageLength, 0, m_sDbParams.m_unMaxMsgLen);
    DDX_Text(pDX, IDC_EDIT_MSGCODE, m_omStrMessageCode);
    DDV_MaxChars(pDX, m_omStrMessageCode, 8);
    DDX_Text(pDX, IDC_EDIT_NOOFSGS, m_unNoOfSgs);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgSgDetView, CFormView)
    //{{AFX_MSG_MAP(CMsgSgDetView)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_SIGNAL_DETAILS, OnClickListSignal)
    ON_NOTIFY(NM_RCLICK, IDC_LSTC_SIGNAL_DETAILS, OnRclickLstSignalDetails)
    ON_COMMAND(IDM_DELETE_SIGNAL, OnDeleteSignal)
    ON_EN_KILLFOCUS(IDC_EDIT_MSG_NAME, OnKillfocusEditMsgName)
    ON_EN_CHANGE(IDC_EDIT_MSG_NAME, OnChangeEditMsgName)
    ON_EN_KILLFOCUS(IDC_EDIT_MSGCODE, OnKillfocusEditMsgcode)
    ON_EN_KILLFOCUS(IDC_EDIT_MSG_LENGTH, OnKillfocusEditMsgLength)
    ON_EN_SETFOCUS(IDC_EDIT_MSG_NAME, OnSetfocusEditMsgName)
    ON_COMMAND(IDM_NEW_SIGNAL, OnSignalNew)
    ON_COMMAND(IDM_EDIT_SIGNAL, OnSignalEdit)
    ON_NOTIFY(NM_RCLICK, IDC_LSTC_SGIDVAL, OnRclickListSignal_Desc_Val)
    ON_COMMAND(IDR_DESCVAL_DELETE, OnSignal_Desc_Delete)
    ON_COMMAND(IDR_DESCVAL_EDIT, OnSignal_Desc_Edit)
    ON_COMMAND(IDM_DESCVAL_ADD, OnSignal_Desc_New)
    ON_EN_CHANGE(IDC_EDIT_MSGCODE, OnChangeEditMsgcode)
    ON_EN_CHANGE(IDC_EDIT_MSG_LENGTH, OnChangeEditMsgLength)
    ON_NOTIFY(NM_DBLCLK, IDC_LSTC_SIGNAL_DETAILS, OnDblclkLstcSignalDetails)
    ON_BN_CLICKED(IDC_BUTTON_DELETESIGNAL, OnButtonDeletesignal)
    ON_BN_CLICKED(IDC_BUTTON_EDITSIGNAL, OnButtonEditsignal)
    ON_BN_CLICKED(IDC_BUTTON_NEWSIGNAL, OnButtonNewsignal)
    ON_BN_CLICKED(IDC_CBTN_ADD_DESC, OnButtonAddDesc)
    ON_BN_CLICKED(IDC_CBTN_DEL_DESC, OnButtonDeleteDesc)
    ON_BN_CLICKED(IDC_CBTN_EDIT_DESC, OnButtonEditDesc)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_SGIDVAL, OnClickSignalDescVal)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_SIGNAL_DETAILS, OnItemchangedLstcSignalDetails)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgSgDetView diagnostics

#ifdef _DEBUG
void CMsgSgDetView::AssertValid() const
{
    CFormView::AssertValid();
}

void CMsgSgDetView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMsgSgDetView message handlers
/******************************************************************************/
/*  Function Name    :  OnInitialUpdate                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the frame work to update the view.
                        Inserts columns to the list control

/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

void CMsgSgDetView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    // Update this pointer to mainframe for future use
    CMainFrame* pMainFrame = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

    if (pMainFrame)
    {
        pMainFrame->podSetMsgSgDetView( this, m_sDbParams.m_eBus );
    }

    // Create a font for the main header
    CFont om_tFont;
    LOGFONT LF;
    memset(&LF, 0, sizeof(LF));
    // Setting the log font structure values to set the font
    LF.lfHeight = 10;
    LF.lfWeight = FW_HEAVY;
    LF.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
    lstrcpy(LF.lfFaceName, "Ms Sans Serif");

    if (!om_tFont.CreateFontIndirect(&LF))
    {
        TRACE("MMI: CMainFrame::OnCreate() Could not create font for Combo Box\n");
    }

    // Insert column header in the signal details list control
    RECT rRect;
    GetDlgItem(IDC_LSTC_SIGNAL_DETAILS)->GetWindowRect( &rRect );
    ScreenToClient(&rRect);
    CSize om_Coor(0,0);
    om_Coor.cx = rRect.right/10;
    // Insert column to signal list control
    m_omListCtrlSignal.InsertColumn( 0,
                                     "Name",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     0);
    m_omListCtrlSignal.InsertColumn( 1,
                                     "Byte Index",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     1);
    m_omListCtrlSignal.InsertColumn( 2,
                                     "Bit No",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     2);
    m_omListCtrlSignal.InsertColumn( 3,
                                     "Length",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     3);
    m_omListCtrlSignal.InsertColumn( 4,
                                     "Type",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     4);
    m_omListCtrlSignal.InsertColumn( 5,
                                     "Max Val",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     5);
    m_omListCtrlSignal.InsertColumn( 6,
                                     "Min Val",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     6);
    m_omListCtrlSignal.InsertColumn( 7,
                                     "Offset",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     7);
    m_omListCtrlSignal.InsertColumn( 8,
                                     "Scale Fac",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     8);
    m_omListCtrlSignal.InsertColumn( 9,
                                     "Unit",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     9);
    m_omListCtrlSignal.InsertColumn( 10,
                                     "Byte Order",
                                     LVCFMT_CENTER,
                                     om_Coor.cx,
                                     10);
    // Insert columns to the signal descriptor list control
    m_omListCtrlSgIdVal.GetWindowRect( &rRect );
    ScreenToClient(&rRect);
    m_omListCtrlSgIdVal.InsertColumn( 0,
                                      "Signal Description",
                                      LVCFMT_CENTER,
                                      rRect.right/2,
                                      0);
    m_omListCtrlSgIdVal.InsertColumn( 1,
                                      "Signal Value",
                                      LVCFMT_CENTER,
                                      rRect.right/2 - 75,
                                      1);
    // let list control select full row
    m_omListCtrlSignal.SetExtendedStyle( LVS_EX_FULLROWSELECT );
    m_omListCtrlSgIdVal.SetExtendedStyle( LVS_EX_FULLROWSELECT );
    //
    vHideControls(SW_HIDE);
}
/******************************************************************************/
/*  Function Name    :  vDisplayMsgSgInformation
/*
/*  Input(s)         :  sMESSAGE* pMsg
/*  Output           :
/*  Functionality    :  Displays the message details on the form view

/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  19.02.2002
/*  Modifications    :  Amitesh Bharti on 22.07.2004
                        Modifications to set the focus to the first entry in the
                        treeview
/******************************************************************************/

void CMsgSgDetView::vDisplayMsgSgInformation(sMESSAGE* pMsg)
{
    m_omListCtrlSignal.DeleteAllItems();
    m_omListCtrlSgIdVal.DeleteAllItems();

    if (pMsg != NULL)
    {
        // Get message name
        m_omStrMessageName = pMsg->m_omStrMessageName;
        GetDlgItem(IDC_STAT_MSG_CODE)->SetWindowText(m_sDbParams.m_omIdFieldName);
        // Get meassage code
        m_omStrMessageCode.Format( "%x", pMsg->m_unMessageCode);
        // Get message length
        m_unMessageLength = pMsg->m_unMessageLength;
        // Get number of signals
        m_unNoOfSgs = pMsg->m_unNumberOfSignals;
        // Get message frame format
        m_nFrameFormat = pMsg->m_bMessageFrameFormat;
        m_nDataFormat = pMsg->m_nMsgDataFormat;

        if (pMsg->m_bMessageFrameFormat)
        {
            GetDlgItem(IDC_STATIC_FF)->SetWindowText("Extended");
        }
        else
        {
            GetDlgItem(IDC_STATIC_FF)->SetWindowText("Standard");
        }

        if (pMsg->m_nMsgDataFormat)
        {
            GetDlgItem(IDC_STATIC_DF)->SetWindowText("Little Endian");
        }
        else
        {
            GetDlgItem(IDC_STATIC_DF)->SetWindowText("Big Endian");
        }

        UpdateData(FALSE);
        // Disable all signal manipulation buttons accept "New Signal" button
        vEnableButtons(FALSE);

        if (pMsg->m_unNumberOfSignals > 0 || pMsg->m_unMessageLength > 0)
        {
            vEnableSignalDetails( TRUE );
            // Enable "New Signal" button
            GetDlgItem(IDC_BUTTON_NEWSIGNAL)->EnableWindow(TRUE);
            sSIGNALS* pSg = pMsg->m_psSignals;
            UINT nSgCount = 0;

            while (pSg != NULL)
            {
                // Add signal details to the signal list
                vAddItemToSignalList( nSgCount, pMsg, pSg );
                nSgCount++;
                pSg = pSg->m_psNextSignalList;
            }

            // Set the selection & focus to this item
            if(nSgCount>0)
            {
                LVITEM sItem;
                sItem.mask      = LVIF_STATE;
                sItem.iItem     = 0;
                sItem.state     = LVIS_FOCUSED|LVIS_SELECTED;
                sItem.stateMask = LVIS_SELECTED |LVIS_FOCUSED;
                sItem.iSubItem  = 0;
                m_omListCtrlSignal.SetItem(&sItem );
            }
        }
        else
        {
            // if the message length is zero, the signal details shud
            //  be disabled
            vEnableSignalDetails( FALSE );
            // Disable "New Signal" button
            GetDlgItem(IDC_BUTTON_NEWSIGNAL)->EnableWindow(FALSE);
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnClickListSignal                                     */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  displays signal description and value
                        for the selected signal from signal list control

/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                                  Rework due to unit testing for CRH0005    */
/*. Modifications    :  Amitesh Bharti, 17.06.2003,                           */
/*                      Display of '-' for negative number in HEX mode        */
/*. Modifications    :  Raja N, 15.03.2004                                    */
/*                      Modified set state to Selected & Focused              */
/******************************************************************************/
void CMsgSgDetView::OnClickListSignal(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
    UINT unItemStateMask = LVNI_SELECTED|LVNI_FOCUSED;
    int nItemCount = m_omListCtrlSignal.GetItemCount();
    int nSel = m_omListCtrlSignal.GetNextItem(-1,
               LVNI_SELECTED  );

    if(nSel == -1)
    {
        m_omListCtrlSignal.SetItemState(nItemCount - 1,
                                        unItemStateMask ,
                                        unItemStateMask ) ;
    }
}

/******************************************************************************/
/*  Function Name    :  OnRclickLstSignalDetails                              */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Displays a pop-up menu to add, delete and edit signal
                        details
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/******************************************************************************/
void CMsgSgDetView::OnRclickLstSignalDetails(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    CPoint point(0,0);
    CMsgSignal* pTempMsgSg = NULL;
    // Get appropriate message ptr
    pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
    // Get cursor position wrt screen co-ord
    GetCursorPos(&point);
    CMenu* m_pomContextMenu = new CMenu;
    int nIndex = m_omListCtrlSignal.GetNextItem(-1, LVNI_SELECTED);
    //
    OnClickListSignal(NULL, 0);

    if ( m_pomContextMenu != NULL && m_unMessageLength > 0)
    {
        m_pomContextMenu->LoadMenu(IDM_SG_OPN_MENU);
        CMenu* pomSubMenu = m_pomContextMenu->GetSubMenu(0);

        if (m_omListCtrlSignal.GetItemCount() == 0  || nIndex == -1)
        {
            pomSubMenu->EnableMenuItem(IDM_DELETE_SIGNAL,
                                       MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
            pomSubMenu->EnableMenuItem(IDM_EDIT_SIGNAL,
                                       MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        }
        else
        {
            pomSubMenu->EnableMenuItem(IDM_DELETE_SIGNAL,
                                       MF_BYCOMMAND | MF_ENABLED );
            pomSubMenu->EnableMenuItem(IDM_EDIT_SIGNAL,
                                       MF_BYCOMMAND | MF_ENABLED );
        }

        // Get signal name from signal list
        CString omStrSgName =
            m_omListCtrlSignal.GetItemText( nIndex, 0 );

        // check whether new signal desc and val can be added
        if ( !pTempMsgSg->bItemCanHaveSignalDesc( m_omStrMessageName,
                omStrSgName,m_omListCtrlSgIdVal.GetItemCount()))
        {
            // not possible, so disable Add menu item
            pomSubMenu->EnableMenuItem(IDM_DESCVAL_ADD,
                                       MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        }
        else
        {
            // possible, so enable Add menu item
            pomSubMenu->EnableMenuItem(IDM_DESCVAL_ADD,
                                       MF_BYCOMMAND | MF_ENABLED );
        }

        pomSubMenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                                    point.x,
                                    point.y,
                                    this,
                                    NULL);
        // Destroyd the Menu from the resource
        m_pomContextMenu->Detach();
        m_pomContextMenu->DestroyMenu();

        if (m_pomContextMenu != NULL )
        {
            delete m_pomContextMenu;
            m_pomContextMenu = NULL;
        }
    }
    else
    {
        AfxMessageBox( defNO_SG_MINUP, MB_OK|MB_ICONINFORMATION);
    }

    *pResult = 0;
}
/******************************************************************************/
/*  Function Name    :  OnDeleteSignal                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Deletes the selected signal from the list
                        and from the data structure
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :  27.12.2002, Amarnath S                                */
/*                      The contents in the signal description list are       */
/*                      deleted                                               */
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                      Rework due to unit testing for CRH0005                */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/*  Modification     :  Anish  on 11.12.2006                                  */
/*                      Modified to get pointer to the message struct of DB   */
/*                      using Msg ID instead of name                          */
/******************************************************************************/
void CMsgSgDetView::OnDeleteSignal()
{
    CMsgSignal* pTempMsgSg = NULL;
    pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
    // Get the selected List item
    int nSelectedItem =
        m_omListCtrlSignal.GetNextItem(-1, LVNI_SELECTED);

    if ( nSelectedItem != -1 )
    {
        if ( AfxMessageBox( SG_DELETE_CONFMN,
                            MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
            // Get the signal name from the list
            CString omStrSignalName =
                m_omListCtrlSignal.GetItemText( nSelectedItem, 0 );

            if ( !omStrSignalName.IsEmpty() )
            {
                if ( !pTempMsgSg->bDeleteSignalFromMsg( nSelectedItem,
                                                        m_omStrMessageName,
                                                        omStrSignalName))
                {
                    AfxMessageBox( SG_DELETE_FAILED, MB_OK|MB_ICONINFORMATION);
                }
                else
                {
                    // Delete the entry from the signal list
                    m_omListCtrlSignal.DeleteItem( nSelectedItem );
                    // Get "no of signals" info from the message and
                    // update the form view
                    sMESSAGE* pMsg =
                        pTempMsgSg->psGetMessagePointerInactive(m_omStrMessageName);

                    if(pMsg != NULL)
                    {
                        m_unNoOfSgs = pMsg->m_unNumberOfSignals;
                    }

                    UpdateData(FALSE);
                    // Set message name in the tree view to bold
                    // to indicate changes
                    CMainFrame* pMainFrm =
                        static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

                    if(pMainFrm != NULL )
                    {
                        CMsgSgTreeView* pTreeViewPtr =
                            pMainFrm->podGetMsgSgTreeView(m_sDbParams.m_eBus);

                        if(pTreeViewPtr != NULL )
                        {
                            pTreeViewPtr->vSetTextBold();
                        }
                    }

                    // Disable signal manipulation button and
                    // deselect the selction in signal details list
                    vEnableButtons(FALSE);
                    m_omListCtrlSignal.SetSelectionMark( -1 );
                    // Delete the contents in the signal description list
                    m_omListCtrlSgIdVal.DeleteAllItems();
                    m_omListCtrlSignal.SetFocus();
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditMsgName                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates and updates the message name
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modification Date:  26.03.2002
/*  Modified By      :  Amarnath Shastry
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                      Rework due to unit testing for CRH0005                */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/*  Modification     :  Anish  on 11.12.2006                                  */
/*                      Modified to get pointer to the message struct of DB   */
/*                      using Msg ID instead of name                          */
/******************************************************************************/
void CMsgSgDetView::OnKillfocusEditMsgName()
{
    // Check if window exists
    if ( IsWindowVisible())
    {
        // get tree view ptr
        CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

        if(pMainFrm != NULL )
        {
            CMsgSgTreeView* podTreeViewPtr = pMainFrm->podGetMsgSgTreeView(m_sDbParams.m_eBus);

            if (podTreeViewPtr != NULL)
            {
                CString omStrPrevMsgName;// = STR_EMPTY;
                CString omStrPvMsgName;
                int nMsgId = -1;
                CMsgSignal* pTempMsgSg = NULL;
                pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);

                if ( TRUE == podTreeViewPtr->m_bIsNewMessage )
                {
                    omStrPrevMsgName = STR_EMPTY;
                    omStrPvMsgName   = STR_EMPTY;
                    nMsgId = -1;
                }
                else
                {
                    omStrPrevMsgName = podTreeViewPtr->m_omSelectedItemText;
                    omStrPvMsgName = m_omStrMessageName;
                    sMESSAGE* pMsg =
                        pTempMsgSg->psGetMessagePointerInactive(omStrPrevMsgName );

                    if ( pMsg != NULL)
                    {
                        nMsgId = pMsg->m_unMessageCode;
                    }
                }

                UpdateData(TRUE);

                if ( m_omStrMessageName.IsEmpty())
                {
                    GetDlgItem( IDC_EDIT_MSG_NAME )->SetFocus();
                    MessageBeep( MB_OK );
                }
                else
                {
                    if (//( omStrPrevMsgName != m_omStrMessageName )
                        omStrPvMsgName != m_omStrMessageName)
                    {
                        // check if the name is unique
                        if ( pTempMsgSg->bIsDuplicateMessageName(
                                    nMsgId, m_omStrMessageName ))
                        {
                            AfxMessageBox("Duplicate message name!",
                                          MB_OK|MB_ICONINFORMATION);
                            // Set focus back to message name edit control
                            GetDlgItem( IDC_EDIT_MSG_NAME )->SetFocus();
                            m_omStrMessageName.Empty();
                            UpdateData(FALSE);
                            vEnableControls(m_omStrMessageName);
                        }
                        else
                        {
                            sMESSAGE* pMsg =
                                new sMESSAGE;
                            pMsg->m_omStrMessageName = m_omStrMessageName;

                            if ( !pTempMsgSg->bUpdateMsg( MSG_NAME,
                                                          omStrPrevMsgName,
                                                          pMsg ))
                                AfxMessageBox("Could not update the changes..!",
                                              MB_OK|MB_ICONERROR );
                            else
                            {
                                // change the color of the message in the tree
                                // view to indicate change
                                podTreeViewPtr->vSetTextBold();
                                podTreeViewPtr->m_omSelectedItemText =
                                    m_omStrMessageName;
                            }

                            delete pMsg;
                            pMsg = NULL;
                            UpdateData(FALSE);

                            if ( TRUE == podTreeViewPtr->m_bIsNewMessage )
                            {
                                // This is no longer a new message
                                podTreeViewPtr->m_bIsNewMessage = FALSE;
                            }
                        }
                    }
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnChangeEditMsgName                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  On change of message name the tree is updated
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

void CMsgSgDetView::OnChangeEditMsgName()
{
    if ( IsWindowVisible())
    {
        CString omStr;
        GetDlgItem(IDC_EDIT_MSG_NAME)->GetWindowText(omStr);
        vEnableControls(omStr);
        CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

        if (pMainFrm != NULL )
        {
            pMainFrm->podGetMsgSgTreeView(m_sDbParams.m_eBus)->vSetMessageName(omStr);
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditMsgcode                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates and updates the message code
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modification Date:  26.03.2002
/*  Modified By      :  Amarnath Shastry                                      */
/*  Modifications    :  Amitesh Bharti, 05.06.2003,replace datatype for       */
/*                      supporting 64bits to __int64                          */
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                      Rework due to unit testing for CRH0005                */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/******************************************************************************/
void CMsgSgDetView::OnKillfocusEditMsgcode()
{
    // Check if window exists
    if ( IsWindowVisible() && !m_omStrMessageName.IsEmpty())
    {
        CString unMsgCode = m_omStrMessageCode;
        UpdateData(TRUE);
        // Remove white spaces if any
        m_omStrMessageCode.TrimLeft();
        m_omStrMessageCode.TrimRight();

        // check if empty
        if ( m_omStrMessageCode.IsEmpty())
        {
            MessageBeep((UINT)-1);
            // Set focus back to message name edit control
            GetDlgItem( IDC_EDIT_MSGCODE )->SetFocus();
            //m_omStrMessageCode = ZERO;
            UpdateData(FALSE);
        }
        else
        {
            // This is new edited value
            if ( unMsgCode != m_omStrMessageCode ||
                    unMsgCode == _T("0"))
            {
                // Get message pointer
                CMsgSignal* pTempMsgSg = NULL;
                pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
                INT nMsgCode = atoi(m_omStrMessageCode);

                // check if the name is unique
                if ( pTempMsgSg->bIsDuplicateMessageCode( m_omStrMessageName,
                        (UINT)nMsgCode ))
                {
                    AfxMessageBox("Duplicate message id!",
                                  MB_OK|MB_ICONINFORMATION);
                    // Set focus back to message name edit control
                    GetDlgItem( IDC_EDIT_MSGCODE )->SetFocus();
                    m_omStrMessageCode = _T("0");
                    UpdateData(FALSE);
                }
                else
                {
                    // check if the value is valid hex number
                    unMsgCode = m_omStrMessageCode;
                    m_omStrMessageCode.MakeUpper();

                    for ( int nCount = 0;
                            nCount < m_omStrMessageCode.GetLength();
                            nCount++ )
                    {
                        TCHAR tChar = m_omStrMessageCode.GetAt( nCount );

                        if ( !((tChar >= 'A') && ( tChar <= 'F' ) ||
                                (tChar >= '0' && tChar <= '9') ) )
                        {
                            AfxMessageBox("Invalid Hexadecimal Number!",
                                          MB_OK|MB_ICONERROR );
                            m_omStrMessageCode.Empty();
                            GetDlgItem( IDC_EDIT_MSGCODE )->SetFocus();
                            UpdateData( FALSE);
                            return;
                        }
                    }

                    // Update the value to the data structure
                    m_omStrMessageCode = unMsgCode;
                    sMESSAGE* pMsg = new sMESSAGE;
                    pMsg->m_unMessageCode = nMsgCode;

                    if ( !pTempMsgSg->bUpdateMsg( MSG_CODE,
                                                  m_omStrMessageName, pMsg ))
                    {
                        AfxMessageBox("Could not update the changes..!",
                                      MB_OK|MB_ICONERROR );
                    }
                    else
                    {
                        // Bold the message name to indicate
                        // the change
                        CMainFrame* pMainFrame =
                            static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

                        if(pMainFrame != NULL )
                        {
                            CMsgSgTreeView* pTreeViewPtr = NULL;
                            pTreeViewPtr = pMainFrame->podGetMsgSgTreeView(m_sDbParams.m_eBus);

                            if(pTreeViewPtr != NULL )
                            {
                                pTreeViewPtr->vSetTextBold();
                            }
                        }
                    }

                    delete pMsg;
                    pMsg = NULL;
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditMsgLength                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates and updates the message length
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modification Date:  26.03.2002
/*  Modified By      :  Amarnath Shastry
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/******************************************************************************/

void CMsgSgDetView::OnKillfocusEditMsgLength()
{
    // Check if window exists
    if ( IsWindowVisible())
    {
        UINT unMsgLength = m_unMessageLength;
        UpdateData(TRUE);

        // update if changed
        if ( unMsgLength != m_unMessageLength )
        {
            CMsgSignal* pTempMsgSg = NULL;
            pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
            sMESSAGE* pMsg = new sMESSAGE;
            pMsg->m_unMessageLength = m_unMessageLength;

            if ( !pTempMsgSg->bUpdateMsg( MSG_LENGTH,
                                          m_omStrMessageName,
                                          pMsg ))
                AfxMessageBox("Could not update the changes..!",
                              MB_OK|MB_ICONERROR );

            delete pMsg;
            pMsg = NULL;
        }
    }
}
/******************************************************************************/
/*  Function Name    :  vSetDefaultValues                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Sets the defaults values for the new message
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

void CMsgSgDetView::vSetDefaultValues()
{
    m_omStrMessageName = "NewMesg";
    m_omStrMessageCode = STR_EMPTY;
    m_unMessageLength = 8;
    m_omListCtrlSignal.DeleteAllItems();
    m_omListCtrlSgIdVal.DeleteAllItems();
    m_unNoOfSgs = 0;
    UpdateData(FALSE);
    GetDlgItem(IDC_EDIT_MSGCODE)->SetWindowText(_T("0"));
}
/******************************************************************************/
/*  Function Name    :  vEnableSignalDetails                                  */
/*                                                                            */
/*  Input(s)         :  BOOL bIsEnabled                                       */
/*  Output           :                                                        */
/*  Functionality    :  Enables or disables the list controls
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

void CMsgSgDetView::vEnableSignalDetails(BOOL bIsEnabled)
{
    GetDlgItem(IDC_LSTC_SIGNAL_DETAILS)->EnableWindow(bIsEnabled);
    GetDlgItem(IDC_LSTC_SGIDVAL)->EnableWindow(bIsEnabled);
}
/******************************************************************************/
/*  Function Name    :  vHideControls                                         */
/*                                                                            */
/*  Input(s)         :  UINT unEnableHide                                     */
/*  Output           :                                                        */
/*  Functionality    :  Hides or unhides the controls
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/
void CMsgSgDetView::vHideControls(UINT unEnableHide)
{
    GetDlgItem(IDC_STAT_MSG_DETAILS)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_MSGGRP)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_MSG_NAME)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_EDIT_MSG_NAME)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_MSG_CODE)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_EDIT_MSGCODE)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_MSG_CODE_HEX)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STATIC_MSGLEN)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_EDIT_MSG_LENGTH)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STATIC_FRAMEFORMAT)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STATIC_DATA_FORMAT)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STATIC_FF)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STATIC_DF)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_NOOFSGS)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_EDIT_NOOFSGS)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_LSTC_SIGNAL_DETAILS)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_LSTC_SGIDVAL)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_SGGROUP)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_DESC)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_SIGNAL_VAL)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_BUTTON_NEWSIGNAL)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_BUTTON_EDITSIGNAL)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_BUTTON_DELETESIGNAL)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_CBTN_ADD_DESC)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_CBTN_EDIT_DESC)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_CBTN_DEL_DESC)->ShowWindow(unEnableHide);
    GetDlgItem(IDC_STAT_MSGBYTES)->ShowWindow(unEnableHide);

    if ( unEnableHide == SW_HIDE )
    {
        m_bAreControlsHidden = TRUE;
    }
    else
    {
        m_bAreControlsHidden = FALSE;
    }
}
/******************************************************************************/
/*  Function Name    :  bGetControlStatus                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Returns "m_bAreControlsHidden"
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/
BOOL CMsgSgDetView::bGetControlStatus()
{
    return (m_bAreControlsHidden);
}
/******************************************************************************/
/*  Function Name    :  OnSetfocusEditMsgName                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Sets m_omStrPrevMsgName to current message name
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/
void CMsgSgDetView::OnSetfocusEditMsgName()
{
    UpdateData(TRUE);
    m_omStrPrevMsgName = m_omStrMessageName;
}
/******************************************************************************/
/*  Function Name    :  OnSignalNew                                           */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Adds new signal to selected message
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                                  Rework due to unit testing for CRH0005    */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/*  Modification     :  Anish  on 11.12.2006                                  */
/*                      Modified to get pointer to the message struct of DB   */
/*                      using Msg ID instead of name                          */
/******************************************************************************/
void CMsgSgDetView::OnSignalNew()
{
    CSignalDetailsDlg odSignalDetDlg( m_sDbParams,
                                      MD_ADD,
                                      m_nDataFormat,
                                      m_omStrMessageName,
                                      m_unMessageLength);

    if ( odSignalDetDlg.DoModal() == IDOK )
    {
        CMsgSignal* pTempMsgSg = NULL;
        pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
        // Get "no of signals" info from the message and update the form view
        sMESSAGE* pMsg =
            pTempMsgSg->psGetMessagePointerInactive(m_omStrMessageName);

        if(pMsg != NULL)
        {
            m_unNoOfSgs = pMsg->m_unNumberOfSignals;
            sSIGNALS* pSg = pMsg->m_psSignals;
            BOOL bShudILoop = TRUE;

            while ( pSg != NULL && bShudILoop)
            {
                if ( pSg->m_omStrSignalName == odSignalDetDlg.m_omStrSignalName )
                {
                    UINT nSgCount = m_omListCtrlSignal.GetItemCount();
                    // Add the new item to the signal list
                    vAddItemToSignalList( nSgCount, pMsg, pSg );
                    // set the message in the tree view to bold
                    // indicating it has changed
                    CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

                    if(pMainFrm != NULL )
                    {
                        CMsgSgTreeView* pTreeViewPtr =
                            pMainFrm->podGetMsgSgTreeView(m_sDbParams.m_eBus);

                        if(pTreeViewPtr != NULL)
                        {
                            pTreeViewPtr->vSetTextBold();
                        }
                    }

                    bShudILoop = FALSE;
                }
                else
                {
                    pSg = pSg->m_psNextSignalList;
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnSignalEdit                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Edits selected signal for changes
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                                  Rework due to unit testing for CRH0005    */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/*  Modification     :  Anish  on 11.12.2006                                  */
/*                      Modified for MDB                                      */
/******************************************************************************/

void CMsgSgDetView::OnSignalEdit()
{
    int nSelItem  = m_omListCtrlSignal.GetNextItem(-1, LVNI_SELECTED);
    int nItemCount = m_omListCtrlSignal.GetItemCount();

    if ( nSelItem != -1 )
    {
        CString omStrSelItemText = STR_EMPTY;
        // Get the byte order
        BYTE byByteOrder = DATA_FORMAT_MOTOROLA;
        omStrSelItemText =
            m_omListCtrlSignal.GetItemText( nSelItem, 10 );

        if (omStrSelItemText == _T("Intel"))
        {
            byByteOrder = DATA_FORMAT_INTEL;
        }

        // Get signal type
        omStrSelItemText =
            m_omListCtrlSignal.GetItemText( nSelItem, 4 );
        CSignalDetailsDlg odSignalDetDlg( m_sDbParams,
                                          MD_EDIT,
                                          byByteOrder,
                                          m_omStrMessageName,
                                          m_unMessageLength,
                                          omStrSelItemText,
                                          m_omListCtrlSignal.
                                          GetItemText( nSelItem, 5 ),
                                          m_omListCtrlSignal.
                                          GetItemText( nSelItem, 6 ),
                                          m_omListCtrlSignal.
                                          GetItemText( nSelItem, 7 ),
                                          m_omListCtrlSignal.
                                          GetItemText( nSelItem, 8 ));
        // Get signal name
        odSignalDetDlg.m_omStrSignalName =
            m_omListCtrlSignal.GetItemText( nSelItem, 0 );
        // Get byte Number
        omStrSelItemText = m_omListCtrlSignal.GetItemText( nSelItem, 1 );
        odSignalDetDlg.m_byByteIndex = (BYTE)atoi((const char*)omStrSelItemText);
        // Get bit Number
        omStrSelItemText =
            m_omListCtrlSignal.GetItemText( nSelItem, 2 );
        odSignalDetDlg.m_byStartBit = (BYTE)atoi(omStrSelItemText);
        omStrSelItemText =
            m_omListCtrlSignal.GetItemText( nSelItem, 3 );
        odSignalDetDlg.m_unSgLen = atoi(omStrSelItemText);
        // Get signal unit value
        odSignalDetDlg.m_omStrUnit =
            m_omListCtrlSignal.GetItemText( nSelItem, 9 );
        // Mode is edit
        // try deleteing the matrix allocated for the edited signal
        // Get message pointer for the message
        // Get appropriate msg structure ptr
        CMsgSignal* pTempMsgSg = NULL;
        BOOL bActive = FALSE;
        pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
        sSIGNALS* pSgTemp = NULL;
        sMESSAGE* pMsg =
            pTempMsgSg->psGetMessagePointerInactive( m_omStrMessageName );

        if ( pMsg != NULL)
        {
            // Get signalpointer
            CMainFrame* pFrame = static_cast<CMainFrame*>
                                 (AfxGetApp()->m_pMainWnd);

            if ( pFrame != NULL )
            {
                // Get signal pointer from the mainframe
                pSgTemp =
                    pFrame->poGetSignalPointer(
                        bActive,
                        pMsg->m_unMessageCode,
                        odSignalDetDlg.m_omStrSignalName );

                if ( pSgTemp )
                {
                    BYTE abySigMask[DATA_LENGTH_MAX] = {0};
                    CMsgSignal::bCalcBitMaskForSig( abySigMask,
                                                    DATA_LENGTH_MAX,
                                                    pSgTemp->m_unStartByte,
                                                    pSgTemp->m_byStartBit,
                                                    pSgTemp->m_unSignalLength,
                                                    pSgTemp->m_eFormat);
                    // Delete matrix info for this signal
                    pTempMsgSg->vUpdateSignalMatrix(abySigMask,
                                                    pMsg->m_bySignalMatrix,
                                                    DATA_LENGTH_MAX,
                                                    RESET);
                }
            }
        }

        if ( odSignalDetDlg.DoModal() == IDOK )
        {
            CMsgSignal* pTempMsgSg = NULL;
            pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
            // Get "no of signals" info from the message and
            // update the form view
            sMESSAGE* pMsg =
                pTempMsgSg->psGetMessagePointerInactive(m_omStrMessageName);

            if(pMsg != NULL)
            {
                sSIGNALS* pSg = pMsg->m_psSignals;
                BOOL bShudILoop = TRUE;

                while ( pSg != NULL && bShudILoop )
                {
                    if (pSg->m_omStrSignalName == odSignalDetDlg.m_omStrSignalName)
                    {
                        // Delete the current selected item from the list
                        m_omListCtrlSignal.DeleteItem( nSelItem );
                        // Add the new item to the signal list
                        vAddItemToSignalList( nSelItem, pMsg, pSg );
                        // set the message in the tree view to bold
                        // indicating it has changed
                        CMainFrame* pMainFrm =
                            static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

                        if(pMainFrm != NULL )
                        {
                            CMsgSgTreeView* pTreeViewPtr =
                                pMainFrm->podGetMsgSgTreeView(m_sDbParams.m_eBus);

                            if(pTreeViewPtr != NULL)
                            {
                                pTreeViewPtr->vSetTextBold();
                            }
                        }

                        bShudILoop = FALSE;
                    }
                    else
                    {
                        pSg = pSg->m_psNextSignalList;
                    }
                }
            }

            // Selction doesn't make sense
            m_omListCtrlSignal.SetSelectionMark( -1 );
            // Disable buttons
            vEnableButtons( FALSE );
        }
        else // Cancelled
        {
            if ( pTempMsgSg != NULL &&
                    pMsg       != NULL &&
                    pSgTemp    != NULL)
            {
                // Mode is edit
                // Restore matrix info for this signal
                BYTE abySigMask[DATA_LENGTH_MAX] = {0};
                CMsgSignal::bCalcBitMaskForSig( abySigMask,
                                                DATA_LENGTH_MAX,
                                                pSgTemp->m_unStartByte,
                                                pSgTemp->m_byStartBit,
                                                pSgTemp->m_unSignalLength,
                                                pSgTemp->m_eFormat);
                pTempMsgSg->vUpdateSignalMatrix(abySigMask,
                                                pMsg->m_bySignalMatrix,
                                                DATA_LENGTH_MAX,
                                                SET);
            }
        }
    }
    else
    {
        m_omListCtrlSignal.SetItemState(nItemCount - 1,
                                        LVIS_SELECTED ,
                                        LVIS_SELECTED ) ;
        AfxMessageBox( "Double click on the list item to select the signal \
details dialog",
                       MB_OK|MB_ICONINFORMATION);
    }
}
/******************************************************************************/
/*  Function Name    :  vAddItemToSignalList                                  */
/*                                                                            */
/*  Input(s)         :  int nRow, sSIGNALS *pSg                   */
/*  Output           :                                                        */
/*  Functionality    :  Adds the signal details into the list at a given row
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :  Rajesh : 05.03.2003: modifications w.r.t Min max types*/
/*  Modifications    :  Amitesh Bharti, 05.06.2003,replace datatype for       */
/*                      supporting 64bits to __int64                          */
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                                  Rework due to unit testing for CRH0005    */
/*. Modifications    :  Raja N, 22.07.2004,                                   */
/*                      Hex negative problem fix                              */
/******************************************************************************/
void CMsgSgDetView::vAddItemToSignalList(int nRow,  sMESSAGE* pMsg,
        sSIGNALS* pSg)
{
    if ( pSg != NULL &&
            pMsg != NULL)
    {
        // Signal name
        m_omListCtrlSignal.InsertItem( nRow,pSg->m_omStrSignalName);
        // Signal byte index
        CString omStrTemp = STR_EMPTY;
        omStrTemp.Format( "%d", pSg->m_unStartByte );
        //
        m_omListCtrlSignal.SetItemText( nRow,
                                        1,
                                        omStrTemp );
        // Signal start bit
        omStrTemp.Format( "%d", pSg->m_byStartBit );
        //
        m_omListCtrlSignal.SetItemText( nRow,
                                        2,
                                        omStrTemp );
        // Signal Length
        omStrTemp.Format( "%d", pSg->m_unSignalLength );
        //
        m_omListCtrlSignal.SetItemText( nRow,
                                        3,
                                        omStrTemp );

        // Signal type
        if ( pSg->m_bySignalType == 'B' )
        {
            omStrTemp = defBOOLEAN;
        }
        else if ( pSg->m_bySignalType == 'U' )
        {
            omStrTemp = defUNSIGNED_INT;
        }
        else
        {
            omStrTemp = defSIGNED_INT;
        }

        m_omListCtrlSignal.SetItemText( nRow,
                                        4,
                                        omStrTemp );
        __int64 n64SignalVal = 0;
        n64SignalVal = pSg->m_SignalMaxValue.n64Value;

        if(pSg->m_bySignalType == 'I')
        {
            if(n64SignalVal <0 )
            {
                unsigned __int64 un64Mask = (unsigned __int64) -1;
                // Mask unwanted portion of signal details
                n64SignalVal = n64SignalVal & un64Mask >> (defMAX_BITS -
                               pSg->m_unSignalLength);
            }
        }

        omStrTemp.Format( "%I64X", n64SignalVal);
        m_omListCtrlSignal.SetItemText( nRow,
                                        5,
                                        omStrTemp );
        n64SignalVal = pSg->m_SignalMinValue.n64Value;

        if(pSg->m_bySignalType == 'I')
        {
            if(n64SignalVal <0 )
            {
                unsigned __int64 un64Mask = (unsigned __int64) -1;
                // Mask unwanted portion of signal details
                n64SignalVal = n64SignalVal & un64Mask >> (defMAX_BITS -
                               pSg->m_unSignalLength);
            }
        }

        omStrTemp.Format( "%I64X", n64SignalVal);
        m_omListCtrlSignal.SetItemText( nRow,
                                        6,
                                        omStrTemp );
        // Get signal offset
        omStrTemp.Format( "%.2f", pSg->m_fSignalOffset );
        //
        m_omListCtrlSignal.SetItemText( nRow,
                                        7,
                                        omStrTemp );
        // Signal scale foctor
        omStrTemp.Format( "%.6f", pSg->m_fSignalFactor );
        //
        m_omListCtrlSignal.SetItemText( nRow,
                                        8,
                                        omStrTemp );
        // Signal unit
        m_omListCtrlSignal.SetItemText( nRow,
                                        9,
                                        pSg->m_omStrSignalUnit );

        // Byte order
        if (pSg->m_eFormat == DATA_FORMAT_INTEL)
        {
            omStrTemp = _T("Intel");
        }
        else
        {
            omStrTemp = _T("Motorola");
        }

        m_omListCtrlSignal.SetItemText( nRow,
                                        10,
                                        omStrTemp);
        UpdateData(FALSE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnRclickListSignal_Desc_Val                           */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResultg                      */
/*  Output           :                                                        */
/*  Functionality    :  Displays a pop-up menu on right click on signal
                        description an value list
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/******************************************************************************/
void CMsgSgDetView::OnRclickListSignal_Desc_Val(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    if ( m_omListCtrlSgIdVal.GetItemCount() )
    {
        CMsgSignal* pTempMsgSg = NULL;
        // Get appropriate message ptr
        pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
        int nIndex = m_omListCtrlSignal.GetNextItem(-1, LVNI_SELECTED);

        if ( nIndex != -1 )
        {
            CPoint point(0,0);
            // Get cursor position wrt screen co-ord
            GetCursorPos(&point);
            CMenu* m_pomContextMenu = new CMenu;

            if ( m_pomContextMenu != NULL )
            {
                m_pomContextMenu->Detach();
                m_pomContextMenu->DestroyMenu();
                // Load the Menu from the resource
                m_pomContextMenu->LoadMenu(IDM_SG_OPN_MENU);
                CMenu* pomSubMenu = m_pomContextMenu->GetSubMenu(1);
                // Get signal name from signal list
                CString omStrSgName =
                    m_omListCtrlSignal.GetItemText( nIndex, 0 );

                // check whether new signal desc and val can be added
                if ( !pTempMsgSg->bItemCanHaveSignalDesc( m_omStrMessageName,
                        omStrSgName,
                        m_omListCtrlSgIdVal.GetItemCount()))
                {
                    // not possible, so disable Add menu item
                    pomSubMenu->EnableMenuItem(IDM_DESCVAL_ADD,
                                               MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                }
                else
                {
                    // possible, so enable Add menu item
                    pomSubMenu->EnableMenuItem(IDM_DESCVAL_ADD,
                                               MF_BYCOMMAND | MF_ENABLED );
                }

                // get selected item from the signal desc-val list
                int nTemp = m_omListCtrlSgIdVal.GetNextItem(-1, LVNI_SELECTED);

                if ( nTemp == -1 )
                {
                    // not selected, so disable edit and delete options
                    pomSubMenu->EnableMenuItem(IDR_DESCVAL_DELETE,
                                               MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                    pomSubMenu->EnableMenuItem(IDR_DESCVAL_EDIT,
                                               MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                }
                else
                {
                    // selected, so enable edit and delete options
                    pomSubMenu->EnableMenuItem(IDR_DESCVAL_EDIT,
                                               MF_BYCOMMAND | MF_ENABLED );
                    pomSubMenu->EnableMenuItem(IDR_DESCVAL_DELETE,
                                               MF_BYCOMMAND | MF_ENABLED );
                }

                pomSubMenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                                            point.x,
                                            point.y ,
                                            this,
                                            NULL);

                if (m_pomContextMenu != NULL )
                {
                    delete m_pomContextMenu;
                }
            }
        }
    }

    *pResult = 0;
}
/******************************************************************************/
/*  Function Name    :  OnSignal_Desc_Delete                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Deletes a signal description and value from the list
                        for a given signal
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                                  Rework due to unit testing for CRH0005    */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/******************************************************************************/
void CMsgSgDetView::OnSignal_Desc_Delete()
{
    // Get the selected List item
    int nSelectedItem =
        m_omListCtrlSgIdVal.GetNextItem(-1, LVNI_SELECTED);

    if ( nSelectedItem != -1 )
    {
        if ( IDYES ==
                AfxMessageBox( MSG_DELETE_DESC_CONMN,MB_YESNO|MB_ICONQUESTION))
        {
            // Get index of the selected signal item
            // from the signal list
            int nIndex =
                m_omListCtrlSignal.GetNextItem(-1, LVNI_SELECTED);

            if ( nIndex != -1 )
            {
                // Get signal desc
                CString omStrDesc =
                    m_omListCtrlSgIdVal.GetItemText( nSelectedItem, 0 );
                // Get signal name from signal list
                CString omStrSgName =
                    m_omListCtrlSignal.GetItemText( nIndex, 0 );
                CMsgSignal* pTempMsgSg = NULL;
                // Get appropriate message pointer
                pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);

                // delete the selected item from the data structure
                if ( !pTempMsgSg->bDeleteSgDescVal( nSelectedItem,
                                                    this->m_omStrMessageName,
                                                    omStrSgName,
                                                    omStrDesc ))
                {
                    // Delete failed
                    AfxMessageBox(MSG_DEL_FAIL, MB_OK|MB_ICONINFORMATION);
                }
                else
                {
                    // Delete the item from signal desc list
                    m_omListCtrlSgIdVal.DeleteItem( nSelectedItem );
                    // Toggle enable-disable "new desc" button
                    vEnableNewDescButton(omStrSgName);
                    // Disable "Delete Desc" and "Edit Desc" buttons
                    GetDlgItem(IDC_CBTN_DEL_DESC)->EnableWindow(FALSE);
                    GetDlgItem(IDC_CBTN_EDIT_DESC)->EnableWindow(FALSE);
                    // set the message in the tree view to bold
                    // indicating it has changed
                    CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

                    if(pMainFrm != NULL )
                    {
                        CMsgSgTreeView* pTreeViewPtr =
                            pMainFrm->podGetMsgSgTreeView(m_sDbParams.m_eBus);

                        if(pTreeViewPtr != NULL)
                        {
                            pTreeViewPtr->vSetTextBold();
                        }
                    }

                    m_omListCtrlSgIdVal.SetFocus();
                }
            }
        }

        m_omListCtrlSgIdVal.SetSelectionMark( -1 );
    }
    else
    {
        AfxMessageBox( MSG_SELECT_SIGNAL, MB_OK|MB_ICONINFORMATION );
    }
}
/******************************************************************************/
/*  Function Name    :  OnSignal_Desc_Edit                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Edits a signal description and value from the list
                        for a given signal
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*                      Pass the index for the case where descriptor is edited*/
/*                      and otherwise zero to the constructor of              */
/*                      CValueDescriptionDlgfor validating signal descriptor  */
/*                      value and signal descriptor.                          */
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                                  Rework due to unit testing for CRH0005    */
/******************************************************************************/
void CMsgSgDetView::OnSignal_Desc_Edit()
{
    // Get index of the selected signal item
    // from the signal list
    int nIndex = m_omListCtrlSignal.GetNextItem(-1, LVNI_SELECTED);

    if ( nIndex != -1 )
    {
        // Get signal name from signal list
        CString omStrSgName =
            m_omListCtrlSignal.GetItemText( nIndex, 0 );
        int nItemIndex = m_omListCtrlSgIdVal.GetNextItem(-1, LVNI_SELECTED);

        if ( nItemIndex != -1 )
        {
            // display dialog to add new description and value
            CValueDescriptionDlg od_Dlg( m_sDbParams,
                                         MD_EDIT,
                                         m_omStrMessageCode,
                                         omStrSgName,
                                         nItemIndex);
            od_Dlg.m_omStrValueDescriptor =
                m_omListCtrlSgIdVal.GetItemText( nItemIndex, 0);
            od_Dlg.m_omStrValue =
                m_omListCtrlSgIdVal.GetItemText( nItemIndex, 1);

            if ( od_Dlg.DoModal() == IDOK )
            {
                // Delete the selected item first
                m_omListCtrlSgIdVal.DeleteItem( nItemIndex );
                // Insert desc and val into the signal desc list
                m_omListCtrlSgIdVal.InsertItem( nItemIndex,
                                                od_Dlg.m_omStrValueDescriptor );
                m_omListCtrlSgIdVal.SetItemText( nItemIndex,
                                                 1,
                                                 od_Dlg.m_omStrValue );
                // make the message name bold to indicate
                // the change
                CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

                if(pMainFrm != NULL )
                {
                    CMsgSgTreeView* pTreeViewPtr =
                        pMainFrm->podGetMsgSgTreeView(m_sDbParams.m_eBus);

                    if(pTreeViewPtr != NULL)
                    {
                        pTreeViewPtr->vSetTextBold();
                    }
                }

                // Disable "Delete Desc" and "Edit Desc" buttons
                GetDlgItem(IDC_CBTN_DEL_DESC)->EnableWindow(FALSE);
                GetDlgItem(IDC_CBTN_EDIT_DESC)->EnableWindow(FALSE);
            }
        }
        else
        {
            AfxMessageBox( "No signal description selected!",
                           MB_OK|MB_ICONINFORMATION );
        }
    }
    else
    {
        AfxMessageBox( MSG_SELECT_SIGNAL, MB_OK|MB_ICONINFORMATION );
    }

    // Deselect signal desc from the signal list
    m_omListCtrlSgIdVal.SetSelectionMark( -1 );
}
/******************************************************************************/
/*  Function Name    :  OnSignal_Desc_New                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Adds a signal description and value from the list
                        for a given signal
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :  20.05.2003, Amitesh Bharti                            */
/*                      Pass the index for the case where descriptor is edited*/
/*                      and otherwise zero to the constructor of              */
/*                      CValueDescriptionDlgfor validating signal descriptor  */
/*                      value and signal descriptor.                          */
/*. Modifications    :  Amitesh Bharti, 11.06.2003,                           */
/*                                  Rework due to unit testing for CRH0005    */
/******************************************************************************/
void CMsgSgDetView::OnSignal_Desc_New()
{
    // Get index of the selected signal item
    // from the signal list
    int nIndex = m_omListCtrlSignal.GetNextItem(-1, LVNI_SELECTED);

    if ( nIndex != -1 )
    {
        // Get signal name from signal list
        CString omStrSgName =
            m_omListCtrlSignal.GetItemText( nIndex, 0 );
        // display dialog to add new description and value
        CValueDescriptionDlg od_Dlg( m_sDbParams,
                                     MD_ADD,
                                     m_omStrMessageCode,
                                     omStrSgName,
                                     0);

        if ( od_Dlg.DoModal() == IDOK )
        {
            int nCount = m_omListCtrlSgIdVal.GetItemCount();
            // Insert desc and val into the signal desc list
            int nIndex =
                m_omListCtrlSgIdVal.InsertItem( nCount,
                                                od_Dlg.m_omStrValueDescriptor );
            m_omListCtrlSgIdVal.SetItemText( nCount,
                                             1,
                                             od_Dlg.m_omStrValue );
            // Let this be the current selection
            m_omListCtrlSgIdVal.SetSelectionMark(nIndex);
            // make the message name bold to indicate
            // the change
            CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

            if(pMainFrm != NULL )
            {
                CMsgSgTreeView* pTreeViewPtr =
                    pMainFrm->podGetMsgSgTreeView(m_sDbParams.m_eBus);

                if(pTreeViewPtr != NULL)
                {
                    pTreeViewPtr->vSetTextBold();
                }
            }

            // Toggle enable-disable "new desc" button
            vEnableNewDescButton(omStrSgName);
        }
    }
    else
    {
        AfxMessageBox( MSG_SELECT_SIGNAL, MB_OK|MB_ICONINFORMATION );
    }
}

/******************************************************************************/
/*  Function Name    :  bUpdateEditedMesageDetails                            */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Updates the message details from the form view
                        to the datastructure
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

BOOL CMsgSgDetView::bUpdateEditedMesageDetails()
{
    return 1;
}
/******************************************************************************/
/*  Function Name    :  OnChangeEditMsgcode                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  18.03.2002                                            */
/*  Modification Date:  30.03.2002                                            */
/*  Modified By      :  Amarnath Shastry                                      */
/******************************************************************************/

void CMsgSgDetView::OnChangeEditMsgcode()
{
    if ( IsWindowVisible())
    {
        CString omStr;
        GetDlgItem(IDC_EDIT_MSGCODE)->GetWindowText(omStr);
        vEnableControls(omStr);
    }
}
/******************************************************************************/
/*  Function Name    :  OnChangeEditMsgLength                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Disables the signal details if message length is set
                        to zero and highlights corresponding message
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  19.03.2002                                            */
/*  Modified Date    :  06.05.2002
/*  Modified By      :  Amarnath Shastry                                      */
/*                      Updates message length information on change to the
                        data structure
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/******************************************************************************/
void CMsgSgDetView::OnChangeEditMsgLength()
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CFormView::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO: Add your control notification handler code here
    if ( IsWindowVisible() )
    {
        UpdateData(TRUE);

        if ( m_unMessageLength == 0 )
        {
            vEnableSignalDetails(FALSE);
        }
        else
        {
            vEnableSignalDetails(TRUE);
        }

        // Highlight corresponding message in tree view
        CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

        if (pMainFrm)
        {
            CMsgSgTreeView* pTreeViewPtr = pMainFrm->podGetMsgSgTreeView(m_sDbParams.m_eBus);

            if ( pTreeViewPtr )
            {
                pTreeViewPtr->vSetTextBold();
            }
        }

        CMsgSignal* pTempMsgSg = NULL;
        pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
        sMESSAGE* pMsg = new sMESSAGE;
        pMsg->m_unMessageLength = m_unMessageLength;

        if ( !pTempMsgSg->bUpdateMsg( MSG_LENGTH, m_omStrMessageName, pMsg ))
        {
            AfxMessageBox("Could not update the changes..!", MB_OK|MB_ICONERROR );
        }

        delete pMsg;
        pMsg = NULL;
    }
}

/******************************************************************************/
/*  Function Name    :  vEnableControls                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables the signal list control and
                        message tree view if the parameter passed
                        is not empty
/*  Member of        :  CMsgSgDetView                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  18.03.2002                                            */
/*  Modifications    :
/******************************************************************************/

void CMsgSgDetView::vEnableControls(CString OmStr)
{
}
/*****************************************************************************/
/*  Function Name    :  OnDblclkLstcSignalDetails
/*
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult
/*  Output           :
/*  Functionality    :  Signal details get edited on double click on the
                        signal details
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  14.05.2002
/*  Modifications    :
/*****************************************************************************/
void CMsgSgDetView::OnDblclkLstcSignalDetails(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    OnSignalEdit();
    *pResult = 0;
}
/*****************************************************************************/
/*  Function Name    :  OnButtonDeletesignal
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls OnDeleteSignal function, Enables the user to
                        delete signal on button press
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  06-11-2002
/*****************************************************************************/
void CMsgSgDetView::OnButtonDeletesignal()
{
    OnDeleteSignal();
}
/*****************************************************************************/
/*  Function Name    :  OnButtonEditsignal
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls OnSignalEdit function, Enables the user to
                        edit signal on button press
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  06-11-2002
/******************************************************************************/
void CMsgSgDetView::OnButtonEditsignal()
{
    OnSignalEdit();
}
/******************************************************************************/
/*  Function Name    :  OnButtonNewsignal
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls OnSignalNew function, Enables the user to
                        create signal on button press
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  06-11-2002
/******************************************************************************/
void CMsgSgDetView::OnButtonNewsignal()
{
    OnSignalNew();
}
/******************************************************************************/
/*  Function Name    :  OnButtonAddDesc
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls OnSignal_Desc_New function, Enables the user to
                        create signal description on button press
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  06-11-2002
/*****************************************************************************/
void CMsgSgDetView::OnButtonAddDesc()
{
    OnSignal_Desc_New();
}
/*****************************************************************************/
/*  Function Name    :  OnButtonDeleteDesc
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls OnSignal_Desc_Delete function.Enables the user to
                        create signal description on button press
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  06-11-2002
/*****************************************************************************/
void CMsgSgDetView::OnButtonDeleteDesc()
{
    OnSignal_Desc_Delete();
}
/*****************************************************************************/
/*  Function Name    :  OnButtonEditDesc
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Calls OnSignal_Desc_Edit function.Enables the user to
                        create signal description on button press
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  06-11-2002
/*****************************************************************************/
void CMsgSgDetView::OnButtonEditDesc()
{
    OnSignal_Desc_Edit();
}
/*****************************************************************************/
/*  Function Name    :  vEnableButtons
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Enables/Disables "Edit Desc", "Delete Desc", "Edit Signal"
                        "Delete Signal" and "New Desc" buttons
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  06-11-2002
/*****************************************************************************/
void CMsgSgDetView::vEnableButtons(BOOL bEnable)
{
    GetDlgItem(IDC_BUTTON_EDITSIGNAL)->EnableWindow(bEnable);
    GetDlgItem(IDC_BUTTON_DELETESIGNAL)->EnableWindow(bEnable);
    GetDlgItem(IDC_CBTN_ADD_DESC)->EnableWindow(bEnable);
    GetDlgItem(IDC_CBTN_EDIT_DESC)->EnableWindow(bEnable);
    GetDlgItem(IDC_CBTN_DEL_DESC)->EnableWindow(bEnable);
}
/*****************************************************************************/
/*  Function Name    :  vEnableNewDescButton
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Enables/Disables "New Desc" button depending on
                        whether the user can add new description
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  06-11-2002
/*  Modification     :  Raja N on 10.03.2004
/*                      Modified to get refer inactive database structure for
/*                      editor operation
/*****************************************************************************/
void CMsgSgDetView::vEnableNewDescButton(CString omStrSgName)
{
    CMsgSignal* pTempMsgSg = NULL;
    pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);

    // Check whether new signal desc and val can be added
    // If not, disable "New Desc" button
    if ( !pTempMsgSg->bItemCanHaveSignalDesc( m_omStrMessageName,
            omStrSgName,
            m_omListCtrlSgIdVal.GetItemCount()))
    {
        // Not possible, so disable "New Desc" button
        GetDlgItem(IDC_CBTN_ADD_DESC)->EnableWindow(FALSE);
    }
    else
    {
        // Possible, so enable "New Desc" button
        GetDlgItem(IDC_CBTN_ADD_DESC)->EnableWindow(TRUE);
    }
}
/*****************************************************************************/
/*  Function Name    :  OnClickSignalDescVal
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult
/*  Output           :  -
/*  Functionality    :  Enables/Disables "Edit Desc" and "Delete Desc" button
                        depending on selection of item in signal desc list
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Amarnath Shastry
/*  Date Created     :  06-11-2002
/*  Modification by  :  Krishnaswamy B.N , 1-09-2003
/*                      If empty space in the list is selected the last item
/*                      will be selected
/*  Modification by  :  Raja N , 15-03-2004
/*                      Changed the state set to Selected & Focused
/*****************************************************************************/
void CMsgSgDetView::OnClickSignalDescVal(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    UINT unItemStateMask = LVNI_SELECTED|LVNI_FOCUSED;
    int nDescrSel = m_omListCtrlSgIdVal.GetNextItem(-1,LVIS_SELECTED);
    int nItemCount = m_omListCtrlSgIdVal.GetItemCount() ;

    if(nItemCount)
    {
        if(nDescrSel == -1)
        {
            m_omListCtrlSgIdVal.SetItemState(nItemCount -1 ,
                                             unItemStateMask, unItemStateMask );
        }
        else
        {
            m_omListCtrlSgIdVal.SetItemState(nDescrSel ,
                                             unItemStateMask, unItemStateMask );
        }

        GetDlgItem(IDC_CBTN_EDIT_DESC)->EnableWindow(TRUE);
        GetDlgItem(IDC_CBTN_DEL_DESC)->EnableWindow(TRUE);
    }

    *pResult = 0;
}

/*****************************************************************************/
/*  Function Name    :  vUpDownArrowKeySelection
/*  Input(s)         :  Selected list item
/*  Output           :  -
/*  Functionality    :  Updates the signal description with selected signal
                        details item
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Krishnaswamy B.N
/*  Date Created     :  29-08-2003
/*  Modification     :  Raja N on 10.03.2004
/*                      Modified to get refer inactive database structure for
/*                      editor operation
/*  Modification     :  Anish  on 11.12.2006                                  */
/*                      Modified for Multiple Database                        */
/*****************************************************************************/
void CMsgSgDetView::vUpDownArrowKeySelection(int nSel)
{
    CMsgSignal* pTempMsgSg = NULL;
    pTempMsgSg = *((CMsgSignal**)(m_sDbParams.m_ppvActiveDB));

    if (nSel != -1 )
    {
        // Enable/Disable signal manipulation buttons
        GetDlgItem(IDC_BUTTON_EDITSIGNAL)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_DELETESIGNAL)->EnableWindow(TRUE);
        GetDlgItem(IDC_CBTN_EDIT_DESC)->EnableWindow(FALSE);
        GetDlgItem(IDC_CBTN_DEL_DESC)->EnableWindow(FALSE);
        // Get selected text
        CString omStrSel =
            m_omListCtrlSignal.GetItemText( nSel, 0 );
        m_omListCtrlSgIdVal.DeleteAllItems();

        if ( !m_omStrMessageName.IsEmpty() && !omStrSel.IsEmpty())
        {
            // Get message pointer corresponding the message "m_omStrMessageName"
            sMESSAGE* pMsg =
                pTempMsgSg->psGetMessagePointerInactive(m_omStrMessageName);

            if ( pMsg != NULL )
            {
                // Get signal pointer
                sSIGNALS* pSg = pMsg->m_psSignals;

                //CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

                // Get the signal pointer to be deleted
                while ( pSg != NULL )
                {
                    // found
                    if ( pSg->m_omStrSignalName == omStrSel )
                    {
                        CSignalDescVal* pDescVal = pSg->m_oSignalIDVal;
                        UINT nCount = 0;

                        while ( pDescVal != NULL )
                        {
                            m_omListCtrlSgIdVal.
                            InsertItem( nCount,
                                        pDescVal->m_omStrSignalDescriptor);
                            CString omStrVal = STR_EMPTY;
                            __int64 n64SignalDesVal = pDescVal->m_n64SignalVal;

                            if(n64SignalDesVal>=0)
                            {
                                omStrVal.Format( "%I64X", n64SignalDesVal );
                            }
                            else
                            {
                                n64SignalDesVal = - n64SignalDesVal;
                                omStrVal.Format( "-%I64X", n64SignalDesVal );
                            }

                            m_omListCtrlSgIdVal.
                            SetItemText( nCount, 1, omStrVal );
                            pDescVal =
                                pDescVal->m_pouNextSignalSignalDescVal;
                            nCount++;
                        }

                        break;
                    }
                    else
                    {
                        pSg = pSg->m_psNextSignalList;
                    }
                }
            }
        }

        // Toggle enable-disable "new desc" button
        vEnableNewDescButton(omStrSel);
    }

    if( nSel == -1 )
    {
        // Disable Signal manipulation buttons
        vEnableButtons( FALSE );
        //empty the signal description window as well
        m_omListCtrlSgIdVal.DeleteAllItems();
    }
}
/*****************************************************************************/
/*  Function Name    :  OnItemchangedLstcSignalDetails
/*  Input(s)         :  Item State & Result Pointer from Message Dispatch
/*  Output           :  -
/*  Functionality    :  Updates the Signal Descriptor List as per the selection
/*  Member of        :  CMsgSgDetView
/*  Friend of        :      -
/*
/*  Author(s)        :  Raja N
/*  Date Created     :  15.03.2004
/*****************************************************************************/
void CMsgSgDetView::OnItemchangedLstcSignalDetails(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    UINT unItemStateMask = LVIS_SELECTED|LVIS_FOCUSED;

    if(pNMListView->uNewState == unItemStateMask)
    {
        vUpDownArrowKeySelection(pNMListView->iItem);
    }

    *pResult = 0;
}

/*******************************************************************************
 Function Name    :  PostNcDestroy
 Input(s)         :  -
 Output           :  -
 Functionality    :  This function will be called by framework after destroying
                     the window. This will initialise CmainFrame's view pointer
                     to null
 Member of        :  CMsgSgDetView
 Friend of        :      -

 Author(s)        :  Amitesh Bharti
 Date Created     :  22.07.2004
/******************************************************************************/
void CMsgSgDetView::PostNcDestroy()
{
    // Update this pointer to mainframe to initialise it to NULL.
    CMainFrame* pMainFrame = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

    if (pMainFrame)
    {
        pMainFrame->podSetMsgSgDetView( NULL, m_sDbParams.m_eBus );
    }

    CFormView::PostNcDestroy();
}
