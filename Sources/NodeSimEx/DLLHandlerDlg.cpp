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
 * \file      DLLHandlerDlg.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "NodeSimEx_stdafx.h"
#include "NodeSimEx_resource.h"          //For resource IDs
#include "HashDefines.h"
#include "FunctionEditorDoc.h" //Accessing the CFunctionEditorDoc class pointer
#include "DLLHandlerDlg.h"  //Included to provide definition for CDLLHandlerDlg class
#include "GlobalObj.h"

/////////////////////////////////////////////////////////////////////////////
// CDLLHandlerDlg dialog

/******************************************************************************
Function Name    :  CDLLHandlerDlg
Input(s)         :  byType - Dialog type, (0)Load_Unload dialog or (1)EventIndication Dialog
                    pDoc - Document pointer
                    pParent - Parent window object pointer
                    bIsDelete - Indicates the working mode. FALSE means handler
                    to be added. Else handler to be deleted.
Output           :  -
Functionality    :  Constructor
Member of        :  CDLLHandlerDlg
Friend of        :  -
Author(s)        :  Ravikumar Patil
Date Created     :  06.03.2003
Modifications    :  Ratnadip Choudhury, 27/05/2009. Updated function header.
******************************************************************************/
CDLLHandlerDlg::CDLLHandlerDlg(BYTE byType, CFunctionEditorDoc* pDoc, CWnd* pParent /*=NULL*/, BOOL bIsDelete /*FALSE*/)
    : CDialog(CDLLHandlerDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDLLHandlerDlg)
    m_bCheckDLLLoad = FALSE;
    m_bCheckDLLUnLoad = FALSE;
    //}}AFX_DATA_INIT
    m_bIsDelete = bIsDelete;
    m_pDoc      = pDoc;
    m_byType    = byType;
}

/******************************************************************************
Function Name    :  DoDataExchange
Input(s)         :  CDataExchange* pDX
Output           :  void
Functionality    :  Framework calls this to exchange and validate dialog data
Member of        :  CDLLHandlerDlg
Friend of        :  -
Author(s)        :  Ravikumar Patil
Date Created     :  06.03.2003
Modifications    :  Ratnadip Choudhury, 27/05/2009. Removed control variable
                    associated to IDC_CBTN__DLL_OK which wasn't used.
******************************************************************************/
void CDLLHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDLLHandlerDlg)
    DDX_Control(pDX, IDC_CHKB_DLL_UNLOAD_HANDLER, m_omCheckDLLUnLoad);
    DDX_Control(pDX, IDC_CHKB_DLL_LOAD_HANDLER, m_omCheckDLLLoad);
    DDX_Check(pDX, IDC_CHKB_DLL_LOAD_HANDLER, m_bCheckDLLLoad);
    DDX_Check(pDX, IDC_CHKB_DLL_UNLOAD_HANDLER, m_bCheckDLLUnLoad);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLLHandlerDlg, CDialog)
    //{{AFX_MSG_MAP(CDLLHandlerDlg)
    ON_BN_CLICKED(IDC_CBTN__DLL_OK, OnBnClickedOk)
    ON_BN_CLICKED(IDC_CBTN__DLL_CANCEL, OnCbtnDllCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLLHandlerDlg message handlers

/******************************************************************************
Function Name    :  OnBnClickedOk
Input(s)         :  -
Output           :  void
Functionality    :  Handler for IDOK button.
Member of        :  CDLLHandlerDlg
Friend of        :  -
Author(s)        :  Ratnadip Choudhury
Date Created     :  06.03.2003
Modifications    :
******************************************************************************/
void CDLLHandlerDlg::OnBnClickedOk()
{
    CButton* pomButton       = NULL;
    CString omStrHandlerName = STR_EMPTY ;
    CString omStrControl     = STR_EMPTY ;
    INT nCheck               = 0;
    BOOL bIsEnable           = TRUE;

    // Get the selected handlers either for deleteting or adding and
    // add the name in the array
    for(int j=0; j<defDLL_HANDLER_NUMBER; j++)
    {
        pomButton = (CButton*)GetDlgItem(IDC_CHKB_DLL_LOAD_HANDLER + j);

        if(pomButton != NULL )
        {
            nCheck      = pomButton->GetCheck();
            bIsEnable   = pomButton->IsWindowEnabled();
            pomButton->GetWindowText(omStrControl);
            omStrControl.TrimLeft();
            omStrControl.TrimRight();
            omStrControl.Replace(' ','_');
            omStrHandlerName = omStrControl;

            if(nCheck == 1 && bIsEnable != 0 )
            {
                m_omStrArrayDLLHandler.Add(omStrHandlerName);
            }
        }
    }

    OnOK();
}

/******************************************************************************
Function Name    :  OnInitDialog
Input(s)         :  void
Output           :  BOOL. Returns TRUE as a green signal to continue further.
Functionality    :  Framework calls this function in response to the message
                    WM_INITDIALOG. This message is sent to the dialog box
                    during DoModal call.
Member of        :  CDLLHandlerDlg
Friend of        :  -
Author(s)        :  Ratnadip Choudhury
Date Created     :  27.05.2009
Modifications    :
******************************************************************************/
BOOL CDLLHandlerDlg::OnInitDialog()
{
    CStringArray* pomStrArrayHandlerName = NULL;
    CDialog::OnInitDialog();
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHKB_DLL_LOAD_HANDLER);

    if (pButton != NULL)
    {
        pButton->SetWindowText(m_byType == 0? _T("Load"): _T("Data Confirm Event"));
    }

    pButton = (CButton*)GetDlgItem(IDC_CHKB_DLL_LOAD_HANDLER + 1);

    if (pButton != NULL)
    {
        pButton->SetWindowText(m_byType == 0? _T("Unload"): _T("Address Change Event"));
    }

    if( m_pDoc != NULL )
    {
        CButton* pomButton       = NULL;
        CString omStrHandlerName = STR_EMPTY ;
        CString omStrControl     = STR_EMPTY ;
        // get the pointer to CStringArray of error handlers prototypes
        pomStrArrayHandlerName = m_byType == 0? m_pDoc->pomStrGetDLLHandlerPrototypes(): m_pDoc->omStrGetEventIndPrototypes();

        if(pomStrArrayHandlerName != NULL )
        {
            // Check for the handlers already added.
            for(INT  i = 0; i<defDLL_HANDLER_NUMBER; i++)
            {
                for(INT j =0; j<pomStrArrayHandlerName->GetSize(); j++)
                {
                    pomButton = (CButton*)GetDlgItem(IDC_CHKB_DLL_LOAD_HANDLER+i);

                    if(pomButton != NULL )
                    {
                        omStrHandlerName = pomStrArrayHandlerName->GetAt(j);
                        pomButton->GetWindowText(omStrControl);
                        omStrControl.Replace(' ','_');

                        // The find the control text in added handlers text
                        if( omStrHandlerName.Find(omStrControl) != -1 )
                        {
                            // If the dialog box is invoked for delete, check
                            // the box of already added handlers else check and
                            // disable
                            if(m_bIsDelete == TRUE )
                            {
                                pomButton->SetCheck(TRUE);
                            }
                            else
                            {
                                pomButton->SetCheck(TRUE);
                                pomButton->EnableWindow(FALSE);
                            }
                        }
                    }
                }
            }
        }

        // Disable all other check box corresponding to which the handlers
        // are not added and dialog box in invoked to delete the handlers
        if(m_bIsDelete == TRUE )
        {
            for(INT i = 0; i<defDLL_HANDLER_NUMBER; i++)
            {
                pomButton = (CButton*)GetDlgItem(IDC_CHKB_DLL_LOAD_HANDLER+i);

                if(pomButton != NULL )
                {
                    INT nCheck = pomButton->GetCheck();

                    if(nCheck == 0 )
                    {
                        pomButton->EnableWindow(FALSE);
                    }
                }
            }

            // Set the dialog caption text to indicate user is deleting
            // the handlers
            SetWindowText(defDLL_HANDLER_TEXT_DEL);
        }
        else
        {
            // Set the dialog caption text to indicate user is adding
            // the handlers
            SetWindowText(defDLL_HANDLER_TEXT_ADD);
        }
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CDLLHandlerDlg message handlers
/******************************************************************************/
/*  Function Name    : OnCbtnDllCancel                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    : message handlers on CANCEL request. Calls the  parent  */
/*                     class OnCancel() member function.                      */
/*  Member of        : CDLLHandlerDlg                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        : Ravikumar Patil                                        */
/*  Date Created     : 06.03.2003                                             */
/*  Modifications    :                                                        */
/******************************************************************************/
void CDLLHandlerDlg::OnCbtnDllCancel()
{
    CDialog::OnCancel();
    // TODO: Add your control notification handler code here
}