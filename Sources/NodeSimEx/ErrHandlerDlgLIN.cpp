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
 * \file      ErrHandlerDlg.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
#include "NodeSimEx_stdafx.h"

#include "FunctionEditorDoc.h"
#include "ErrHandlerDlgLIN.h"
#include "GlobalObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/******************************************************************************/
/*  Function Name    :  CErrHandlerDlg                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class.Initialisation of  m_bIsDelete is done here*/
/*  Member of        :  CErrHandlerDlgLIN                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
CErrHandlerDlgLIN::CErrHandlerDlgLIN(CFunctionEditorDoc* pDoc, CWnd* pParent /*=nullptr*/,BOOL bIsDelete /*FALSE*/)
    : CDialog(CErrHandlerDlgLIN::IDD, pParent)
{
    //{{AFX_DATA_INIT(CErrHandlerDlg)
    //}}AFX_DATA_INIT
    m_bIsDelete = bIsDelete;
    m_pDoc      = pDoc;
}

/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to exchange and validate      */
/*                         dialog data                                        */
/*  Member of        :  CErrHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CErrHandlerDlgLIN::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CErrHandlerDlg)
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrHandlerDlgLIN, CDialog)
    //{{AFX_MSG_MAP(CErrHandlerDlg)
    ON_BN_CLICKED(IDCANCEL_LIN, OnCbtnErrorCancel)
    ON_BN_CLICKED(IDOK_LIN, OnCbtnErrorOk)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the WM_INITDIALOG message. This message is sent to */
/*                      the dialog box during DoModal calls,which occur       */
/*                      immediately before the dialog box is displayed.       */
/*                      All controls of dialog are initialised in this        */
/*                      function. Depeding upon the flag m_bDelete, the check */
/*                      will be checked and/or disable                        */
/*  Member of        :  CErrHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2003                                            */
/*  Modifications    :  Raja N                                                */
/*                      On 07.09.2004, Added code to disable Active, Passive &*/
/*                      Bus Off handlers in case of USB build as it is not    */
/*                      supported currently                                   */
/*  Modifications    :  Raja N                                                */
/*                      On 15.09.2004, Removed code to disable handlers as    */
/*                      they are supported now                                */
/******************************************************************************/
BOOL CErrHandlerDlgLIN::OnInitDialog()
{
    CStringArray* pomStrArrayHandlerName = nullptr;

    CDialog::OnInitDialog();
    // remove all the element previously added, if any.
    m_omStrArrayErrorHandler.RemoveAll();
    // Get the pointer to document

    if( m_pDoc != nullptr )
    {
        CButton* pomButton = nullptr;
        CString omStrHandlerName = STR_EMPTY ;
        CString omStrControl     = STR_EMPTY ;
        INT nCheck               = 0;

        // get the pointer to CStringArray of error handlers prototypes
        pomStrArrayHandlerName = m_pDoc->pomStrGetErrorHandlerPrototypes();

        // Check for the handlers already added.
        int nIndex;     //nIndex declared outside
        for( nIndex = 0; nIndex < defERROR_HANDLER_NUMBER; nIndex++)
        {
            for(int j = 0; j < pomStrArrayHandlerName->GetSize(); j++)
            {
                pomButton = (CButton*)
                            GetDlgItem(IDC_CHK_ERR_CHECKSUM_LIN + nIndex);
                if(pomButton != nullptr )
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

        // Disable all other check box corresponding to which the handlers
        // are not added and dialog box in invoked to delete the handlers
        if(m_bIsDelete == TRUE )
        {
            for( nIndex = 0; nIndex < defERROR_HANDLER_NUMBER;
                    nIndex++)
            {
                pomButton = (CButton*)
                            GetDlgItem(IDC_CHK_ERR_CHECKSUM_LIN + nIndex);
                nCheck = pomButton->GetCheck();
                if(nCheck == 0 )
                {
                    pomButton->EnableWindow(FALSE);
                }
            }
            // Set the dialog caption text to indicate user is deleting
            // the handlers
            SetWindowText(defERROR_HANDLER_TEXT_DEL);
        }
        else
        {
            // Set the dialog caption text to indicate user is adding
            // the handlers
            SetWindowText(defERROR_HANDLER_TEXT_ADD);
        }
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/******************************************************************************/
/*  Function Name    :  OnCbtnErrorCancel                                     */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    : message handlers on CANCEL request. Calls the  parent  */
/*                     class OnCancel() member function.                      */
/*  Member of        :  CErrHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CErrHandlerDlgLIN::OnCbtnErrorCancel()
{
    CDialog::OnCancel();
}

/******************************************************************************/
/*  Function Name    :  OnCbtnErrorOk                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user selects OK button.  */
/*                      This function will get the user selections and add    */
/*                      handlers to be added/deleted in the member variable   */
/*                      m_omStrArrayErrorHandler.                             */
/*  Member of        :  CErrHandlerDlg                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  19.02.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CErrHandlerDlgLIN::OnCbtnErrorOk()
{
    CButton* pomButton       = nullptr;
    CString omStrHandlerName = STR_EMPTY ;
    CString omStrControl     = STR_EMPTY ;
    INT nCheck               = 0;
    BOOL bIsEnable           = TRUE;
    // Get the selected handlers either for deleteting or adding and
    // add the name in the array
    for(int j=0; j<defERROR_HANDLER_NUMBER_LIN; j++)
    {
        pomButton =
            (CButton*)GetDlgItem(IDC_CHK_ERR_CHECKSUM_LIN + j);
        if(pomButton != nullptr )
        {
            nCheck      = pomButton->GetCheck();
            bIsEnable   = pomButton->IsWindowEnabled();
            pomButton->GetWindowText(omStrControl);
            omStrControl.Replace("Error","");
            omStrControl.TrimLeft();
            omStrControl.TrimRight();
            omStrControl.Replace(' ','_');
            omStrHandlerName = omStrControl;
            if(nCheck == 1 && bIsEnable != 0 )
            {
                m_omStrArrayErrorHandler.Add(omStrHandlerName);
            }
        }
    }
    CDialog::OnOK();
}
