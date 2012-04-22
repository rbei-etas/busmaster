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
 * \file      ValueDescriptionDlg.cpp
 * \brief     This file contain definition of all function of 
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of 
 */
#include "stdafx.h"                 // Standard header
#include "BUSMASTER.h"            // App class header
#include "ValueDescriptionDlg.h"    // Class definition is here 
#include "MainFrm.h"                // Hex validation definition is here
#include "HashDefines.h"            // All hash defines sre here

extern CCANMonitorApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CValueDescriptionDlg dialog
/******************************************************************************/
/*  Function Name    :  CValueDescriptionDlg                                  */
/*  Input(s)         :  eMODES eMode,
                        CString omStrMsgName,
                        CString omStrSgName,                    
                        CWnd* pParent                                         */
/*  Output           :                                                        */
/*  Functionality    :  Constructor                                           */
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  27.02.2002                                            */
/*  Modifications    :  11-12-2002, Amarnath S                                */ 
/*                      Initialised members                                   */  
/******************************************************************************/
CValueDescriptionDlg::CValueDescriptionDlg(const SDBPARAMS& sDbParams,
                                           eMODES eMode,
                                           CString omStrMessageCode,
                                           CString omStrSgName,                 
                                           INT nIndex,
                                           CWnd* pParent /*=NULL*/)
    : CDialog(CValueDescriptionDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CValueDescriptionDlg)
    m_omStrValueDescriptor = _T("");
    m_omStrValue = _T("");
    m_bIsCanceled = FALSE;
    m_omStrMessageCode = omStrMessageCode;
    m_omStrSgName = omStrSgName;
    m_bMode = eMode;
    m_nIndex = nIndex;
    //}}AFX_DATA_INIT

    m_omStrPrevDesc         = STR_EMPTY;
    m_omStrPrevSignalVal    = STR_EMPTY;
    m_sDbParams             = sDbParams;
}


void CValueDescriptionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CValueDescriptionDlg)
    DDX_Text(pDX, IDC_EDIT_VALUE_DESC, m_omStrValueDescriptor);
    DDX_Text(pDX, IDC_EDIT_VAL, m_omStrValue);
	DDV_MaxChars(pDX, m_omStrValue, 16);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueDescriptionDlg, CDialog)
    //{{AFX_MSG_MAP(CValueDescriptionDlg)
    ON_BN_CLICKED(IDC_CBTN_OK, OnClickedOk)
    ON_BN_CLICKED(IDC_CBTN_CANCEL, OnCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/******************************************************************************/
/*  Function Name    :  OnClickedOk                                           */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Updates the dialog values to the data structure
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  27.02.2002                                            */
/*  Modifications    :  11-12-2002, Amarnath S                                */
/*                      Changes done to validate member variables             */
/*  Modifications    :  20-05-2003, Amitesh Bharti                            */
/*                      Pass the index for the case where descriptor is edited*/
/*                      to function for validating signal descriptor value and*/
/*                      signal descriptor.                                    */
/*  Modifications    :  10.03.2004, Raja N                                    */
/*                      Modified to refer inactive data structure for editor  */
/*                      purpose.                                              */
/******************************************************************************/
void CValueDescriptionDlg::OnClickedOk() 
{
    UpdateData(TRUE);
    BOOL bRetValue = TRUE;
    // Validate data
    if ( m_omStrValueDescriptor.IsEmpty() == TRUE )
    {
        AfxMessageBox("Signal Description cannot be emtpy!",MB_OK);
        GetDlgItem(IDC_EDIT_VALUE_DESC)->SetFocus();
        bRetValue = FALSE;
    }

    if ( bRetValue == TRUE)
    {
        if ( m_omStrValue.IsEmpty() == TRUE )
        {
            AfxMessageBox("Signal value cannot be emtpy!",MB_OK);
            GetDlgItem(IDC_EDIT_VAL)->SetFocus();
            bRetValue = FALSE;
        }

        if ( bRetValue == TRUE )
        {
            // Get appropriate msg structure ptr
            CMsgSignal* pTempMsgSg = NULL;
            pTempMsgSg = *((CMsgSignal**)(m_sDbParams.m_ppvActiveDB));

            if ( pTempMsgSg != NULL )
            {
                // Check if description is duplicate
                if ( m_omStrPrevDesc != m_omStrValueDescriptor )
                {
                    if(pTempMsgSg->bIsDuplicateValueDescription(m_omStrMessageCode,
                        m_omStrSgName, 
                        m_omStrValue, 
                        m_omStrValueDescriptor,
                        m_nIndex,
                        m_bMode))
                    {
                       AfxMessageBox("Signal Descriptor already exists!",MB_OK);
                        GetDlgItem(IDC_EDIT_VALUE_DESC)->SetFocus();
                        bRetValue = FALSE;
                    }
                }

                // Check if signal value is duplicate
                if ( bRetValue == TRUE )
                {
                    if ( m_omStrPrevSignalVal  != m_omStrValue )
                    {
                        if ( pTempMsgSg->bIsDuplicateSignalValue(m_omStrMessageCode,
                            m_omStrSgName, 
                            m_omStrValue, 
                            m_omStrValueDescriptor,
                            m_nIndex,
                            m_bMode))
                        {
                            AfxMessageBox("Signal Value already exists!", MB_OK);
                            GetDlgItem(IDC_EDIT_VAL )->SetFocus();
                            bRetValue = FALSE;
                        }
                    }
                }

                // Check if the signal value is within the range
                if ( bRetValue == TRUE )
                {
                    if (pTempMsgSg->bIsSignalValueOutofRange(m_omStrMessageCode,
                        m_omStrSgName, 
                        m_omStrValue ))
                    {
                        AfxMessageBox( MSG_SG_VAL_OUT_OF_RANGE, MB_OK);
                        GetDlgItem(IDC_EDIT_VAL )->SetFocus();
                        bRetValue = FALSE;
                    }

                    if ( bRetValue == TRUE )
                    {
                        // Update the desc into the data structure
                        CSignalDescVal* pNew = new CSignalDescVal;
                        CMainFrame* pMainFrame = 
                            (CMainFrame*)AfxGetApp()->m_pMainWnd; 
                        if ( pNew       != NULL &&
                             pMainFrame != NULL)
                        {
                            CString omStrPrevDesc = STR_EMPTY;
                            if ( MD_ADD == m_bMode )
                            {
                                // new desc and val hence allocate memory
                                pTempMsgSg->bAddSgDescVal( m_omStrMessageCode,
                                                           m_omStrSgName );
                            }
                            // convert string to int
                            pNew->m_n64SignalVal = 
                                pMainFrame->nConvertStringToInt( m_omStrValue );
                            pNew->m_omStrSignalDescriptor = 
                                m_omStrValueDescriptor;
                            if (MD_EDIT == m_bMode)
                            {
                                //m_omStrPrevDesc = m_omStrValueDescriptor;
                            }
                            // update the edited values to the data structure
                            if ( !pTempMsgSg->bUpdateSgDescVal( m_omStrMessageCode,
                                                                m_omStrSgName,
                                                                m_omStrPrevDesc,
                                                                pNew ))
                            {
                                AfxMessageBox("Could not update...",
                                               MB_OK|MB_ICONINFORMATION);
                            }
                            // clean up
                            delete pNew;
                            pNew = NULL;
                            CDialog::OnOK();
                        }
                    }
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnCancel                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Sets a flag and calls the base class function
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  27.02.2002                                            */
/*  Modifications    :                                                        */ 
/******************************************************************************/
void CValueDescriptionDlg::OnCancel() 
{
    m_bIsCanceled = TRUE;
    
    CDialog::OnCancel();
}
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Initialises the dialog members                        */
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  27.02.2002                                            */
/*  Modifications    :  11-12-2002, Amarnath S                                */ 
/*                      Initialised members in edit mode                      */  
/******************************************************************************/
BOOL CValueDescriptionDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

    if ( m_bMode == MD_EDIT )
    {
        m_omStrPrevDesc         = m_omStrValueDescriptor;
        m_omStrPrevSignalVal    = m_omStrValue;
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
/******************************************************************************/
/*  Function Name    :  PreTranslateMessage                                   */
/*  Input(s)         :  MSG* pMsg                                             */
/*  Output           :                                                        */
/*  Functionality    :  Process hexadecimal characters                        */
/*                      in signal value edit control                          */
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  11.12.2002                                            */
/*  Modifications    :  23-10-2003, Amitesh Bharti                            */ 
/*                      Remove condition for not processing space.            */  
/******************************************************************************/
BOOL CValueDescriptionDlg::PreTranslateMessage(MSG* pMsg) 
{
    // Do not process non-hexadecimal characters
    // in signal value edit control
    BOOL bSkip = FALSE;
    CEdit* omEditCtrlName   = (CEdit*) GetDlgItem(IDC_EDIT_VAL);
    CEdit* omEditFocusName  = (CEdit*) GetFocus();
    if ( pMsg->message == WM_CHAR )
    {
        if ( omEditCtrlName == omEditFocusName )
        {
		    int nStart, nEnd;
            omEditCtrlName->GetSel(nStart, nEnd);
            if (nStart == 0 && 
                pMsg->wParam == MINUS_SIGN) 
            {
                bSkip = FALSE;
            }
            else
            if ( ( pMsg->wParam >= 0x61 && pMsg->wParam<=0x66 )||// A-F
                 ( pMsg->wParam >= 0x41 && pMsg->wParam<=0x46 )||// a-f
                 ( pMsg->wParam >= '0' && pMsg->wParam <='9' ) ||// 0-9
                   pMsg->wParam == 0x08 )// BackSpace
            {
                bSkip = FALSE;
            }
            else
            {
                bSkip = TRUE;
            }
        }
    }
    if ( bSkip == FALSE )
        bSkip = CDialog::PreTranslateMessage(pMsg);
    return bSkip;
}
