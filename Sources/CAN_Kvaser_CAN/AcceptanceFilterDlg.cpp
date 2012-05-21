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
 * \file      AcceptanceFilterDlg.cpp
 * \brief     This file contain definition of CAcceptanceFilterDlg class
 * \authors   Amitesh Bharti, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of CAcceptanceFilterDlg class
 */
// For all standard header file include
#include "CAN_Kvaser_CAN_stdafx.h"
// Definition of App class
// Definition of CRadicEdit class.
#include "Utility/RadixEdit.h"
// Contains structures that are common across the following
// SW modules - Dongle Driver, DIL & Application

// Definition of CAcceptanceFilterDlg class.
#include "include/struct_can.h"
#include "AcceptanceFilterDlg.h"
// Micros definitions
#include "ContrConfigPeakUsbDefs.h"

#define defMAX_TEXT_ACCEPTANCE  2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/******************************************************************************/
/*  Function Name    :  CAcceptanceFilterDlg                                  */ 
/*  Input(s)         :  CWnd* pParent: Pointer to parent class                */
/*                      PSACC_FILTER_INFO psAccFilterInfo: Acceptance Filter  */    
/*                                                         information        */
/*  Output           :                                                        */    
/*  Functionality    :  Constructor is called when user create an object of   */
/*                      this class. Initialisation of all data members        */
/*  Member of        :  CAcceptanceFilterDlg                                  */    
/*  Friend of        :      -                                                 */    
/*  Author(s)        :  Amitesh Bharti                                        */    
/*  Date Created     :  25.03.2003                                            */    
/*  Modifications    :  Raja N on 11.03.2005                                  */    
/*                      Modified the parameter type to get complete controller*/
/*                      details. Now this dialog will modifiy this data only  */
/*                      and will not directly modify the configuration module */
/******************************************************************************/
//IMPLEMENT_DYNAMIC(CAcceptanceFilterDlg, CPropertyPage)
CAcceptanceFilterDlg::CAcceptanceFilterDlg(CWnd* pParent /*=NULL*/,
                                           PSCONTROLER_DETAILS psControllerInfo)
                        : CDialog(CAcceptanceFilterDlg::IDD, pParent)
    //: CPropertyPage(CAcceptanceFilterDlg::IDD, IDS_PPAGE_ACCEPTANCE_FILTER)
{
    //{{AFX_DATA_INIT(CAcceptanceFilterDlg)
    m_omStrAccCodeByte1 = _T("");
    m_omStrAccCodeByte2 = _T("");
    m_omStrAccCodeByte3 = _T("");
    m_omStrAccCodeByte4 = _T("");
    m_omStrAccMaskByte1 = _T("");
    m_omStrAccMaskByte2 = _T("");
    m_omStrAccMaskByte3 = _T("");
    m_omStrAccMaskByte4 = _T("");

	for (int i = 0; i < CAN_MSG_IDS ; i++)
	{
		m_omStrLocalAccCodeByte1[i] = _T("");
		m_omStrLocalAccCodeByte2[i] = _T("");
		m_omStrLocalAccCodeByte3[i] = _T("");
		m_omStrLocalAccCodeByte4[i] = _T("");
		m_omStrLocalAccMaskByte1[i] = _T("");
		m_omStrLocalAccMaskByte2[i] = _T("");
		m_omStrLocalAccMaskByte3[i] = _T("");
		m_omStrLocalAccMaskByte4[i] = _T("");
	}
//}}AFX_DATA_INIT
    // Set the Filter mode to Single
    m_bAccFilterMode = 0;
    // Copy controller information pointer
    m_psControllerInfo = psControllerInfo;	

}
/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to exchange and validate      */
/*                         dialog data                                        */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CAcceptanceFilterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAcceptanceFilterDlg)
    DDX_Control(pDX, IDC_EDIT_ACCEPTANCE_MASK4, m_omEditAccMaskByte4);
    DDX_Control(pDX, IDC_EDIT_ACCEPTANCE_MASK3, m_omEditAccMaskByte3);
    DDX_Control(pDX, IDC_EDIT_ACCEPTANCE_MASK2, m_omEditAccMaskByte2);
    DDX_Control(pDX, IDC_EDIT_ACCEPTANCE_MASK1, m_omEditAccMaskByte1);
    DDX_Control(pDX, IDC_EDIT_ACCEPTANCE_CODE4, m_omEditAccCodeByte4);
    DDX_Control(pDX, IDC_EDIT_ACCEPTANCE_CODE3, m_omEditAccCodeByte3);
    DDX_Control(pDX, IDC_EDIT_ACCEPTANCE_CODE2, m_omEditAccCodeByte2);
    DDX_Control(pDX, IDC_EDIT_ACCEPTANCE_CODE1, m_omEditAccCodeByte1);
    DDX_Text(pDX, IDC_EDIT_ACCEPTANCE_CODE1, m_omStrAccCodeByte1);
    DDX_Text(pDX, IDC_EDIT_ACCEPTANCE_CODE2, m_omStrAccCodeByte2);
    DDX_Text(pDX, IDC_EDIT_ACCEPTANCE_CODE3, m_omStrAccCodeByte3);
    DDX_Text(pDX, IDC_EDIT_ACCEPTANCE_CODE4, m_omStrAccCodeByte4);
    DDX_Text(pDX, IDC_EDIT_ACCEPTANCE_MASK1, m_omStrAccMaskByte1);
    DDX_Text(pDX, IDC_EDIT_ACCEPTANCE_MASK2, m_omStrAccMaskByte2);
    DDX_Text(pDX, IDC_EDIT_ACCEPTANCE_MASK3, m_omStrAccMaskByte3);
    DDX_Text(pDX, IDC_EDIT_ACCEPTANCE_MASK4, m_omStrAccMaskByte4);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcceptanceFilterDlg, CDialog)
    //{{AFX_MSG_MAP(CAcceptanceFilterDlg)
    ON_BN_CLICKED(IDC_CBTN_ACCETANCE_OK, OnCbtnAccetanceOk)
    ON_EN_MAXTEXT(IDC_EDIT_ACCEPTANCE_CODE1, OnMaxtextEditAcceptanceCode1)
    ON_EN_MAXTEXT(IDC_EDIT_ACCEPTANCE_CODE2, OnMaxtextEditAcceptanceCode2)
    ON_EN_MAXTEXT(IDC_EDIT_ACCEPTANCE_CODE3, OnMaxtextEditAcceptanceCode3)
    ON_EN_MAXTEXT(IDC_EDIT_ACCEPTANCE_CODE4, OnMaxtextEditAcceptanceCode4)
    ON_EN_MAXTEXT(IDC_EDIT_ACCEPTANCE_MASK1, OnMaxtextEditAcceptanceMask1)
    ON_EN_MAXTEXT(IDC_EDIT_ACCEPTANCE_MASK2, OnMaxtextEditAcceptanceMask2)
    ON_EN_MAXTEXT(IDC_EDIT_ACCEPTANCE_MASK3, OnMaxtextEditAcceptanceMask3)
    //}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RBTN_DUAL_FILTER_MODE,   OnBnClickedRbtnDualFilterMode)
	ON_BN_CLICKED(IDC_RBTN_SINGLE_FILTER_MODE, OnBnClickedRbtnSingleFilterMode)
	ON_BN_CLICKED(IDC_RBTN_FILTER_ACCEPT_ALL,  OnBnClickedRbtnFilterAcceptAll)
	ON_BN_CLICKED(IDC_RBTN_FILTER_REJECT_ALL,  OnBnClickedRbtnFilterRejectAll)
	ON_BN_CLICKED(IDC_RBTN_FILTER_MANUAL_SET,  OnBnClickedRbtnFilterManualSet)
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  OnCbtnAccetanceOk                                     */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework when user selects OK button   */
/*                      It will intialise the configuration class data member */
/*                      Also the acceptance filter information will be stored */
/*                      m_psAccFilterInfo for CChangeRegisters class to access*/
/*                      it.                                                   */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :  Raja N on 11.03.2005                                  */
/*                      Removed the code that updates the configuration module*/
/*                      and added code to update controller information object*/
/******************************************************************************/
void CAcceptanceFilterDlg::OnCbtnAccetanceOk() 
{
    BOOL bButtonChecked = TRUE;
    
    UpdateData(TRUE);

	vSaveFilterDetails(!m_bAccFilterMode);
    // Set only acceptance filter information for both standard and extended envelopes.
	for ( int i = 0; i < CAN_MSG_IDS; i++ )
	{
		m_psControllerInfo->m_enmHWFilterType[i] = m_enmHWFilterType[i];

		_tcscpy(m_psControllerInfo->m_omStrAccCodeByte1[i], m_omStrLocalAccCodeByte1[i].GetBuffer(MAX_PATH));
		_tcscpy(m_psControllerInfo->m_omStrAccCodeByte2[i], m_omStrLocalAccCodeByte2[i].GetBuffer(MAX_PATH));
		_tcscpy(m_psControllerInfo->m_omStrAccCodeByte3[i], m_omStrLocalAccCodeByte3[i].GetBuffer(MAX_PATH));
		_tcscpy(m_psControllerInfo->m_omStrAccCodeByte4[i], m_omStrLocalAccCodeByte4[i].GetBuffer(MAX_PATH));

		_tcscpy(m_psControllerInfo->m_omStrAccMaskByte1[i], m_omStrLocalAccMaskByte1[i].GetBuffer(MAX_PATH));
		_tcscpy(m_psControllerInfo->m_omStrAccMaskByte2[i], m_omStrLocalAccMaskByte2[i].GetBuffer(MAX_PATH));
		_tcscpy(m_psControllerInfo->m_omStrAccMaskByte3[i], m_omStrLocalAccMaskByte3[i].GetBuffer(MAX_PATH));
		_tcscpy(m_psControllerInfo->m_omStrAccMaskByte4[i], m_omStrLocalAccMaskByte4[i].GetBuffer(MAX_PATH));				
	}

	m_psControllerInfo->m_bAccFilterMode    = m_bAccFilterMode;

	CDialog::OnOK();
}
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to when the dialog control is */
/*                      initialised. The control will be initialised with last*/
/*                      used value or default value.                          */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :  Raja N on 07.09.2004                                  */
/*                      Added code to disable Dual filtering option in case of*/
/*                      USB Build                                             */
/*  Modifications    :  Raja N on 11.03.2005                                  */
/*                      Removed the code that gets data from configuration    */
/*                      module and removed dual filterign deletion code as it */
/*                      is supported now in USB                               */
/*  Modifications    :  Raja N on 20.07.2005                                  */
/*                      Added code rename single and dual filter text         */
/******************************************************************************/
BOOL CAcceptanceFilterDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    PSCONTROLER_DETAILS  psControllerDetails = m_psControllerInfo;
    
    if(psControllerDetails != NULL)
    {
		m_bAccFilterMode    = psControllerDetails->m_bAccFilterMode;		
        if(m_bAccFilterMode == 0)
        {
            CheckDlgButton(IDC_RBTN_SINGLE_FILTER_MODE,BST_CHECKED);
        }
        else
        {
            CheckDlgButton(IDC_RBTN_DUAL_FILTER_MODE,BST_CHECKED);
        }
        // Get the acceptance mask and code value with type of filter
		for (int i = 0 ; i < CAN_MSG_IDS ; i++)
		{
			m_omStrLocalAccCodeByte1[i] = psControllerDetails->m_omStrAccCodeByte1[i];
			m_omStrLocalAccCodeByte2[i] = psControllerDetails->m_omStrAccCodeByte2[i];
			m_omStrLocalAccCodeByte3[i] = psControllerDetails->m_omStrAccCodeByte3[i];
			m_omStrLocalAccCodeByte4[i] = psControllerDetails->m_omStrAccCodeByte4[i];
			m_omStrLocalAccMaskByte1[i] = psControllerDetails->m_omStrAccMaskByte1[i];
			m_omStrLocalAccMaskByte2[i] = psControllerDetails->m_omStrAccMaskByte2[i];
			m_omStrLocalAccMaskByte3[i] = psControllerDetails->m_omStrAccMaskByte3[i];
			m_omStrLocalAccMaskByte4[i] = psControllerDetails->m_omStrAccMaskByte4[i];     
			m_enmHWFilterType[i]        = psControllerDetails->m_enmHWFilterType[i];
		}		
		vSetFilterDetails(false);		
    }
    // Set the base of all edit control
    m_omEditAccCodeByte1.vSetBase(BASE_HEXADECIMAL);
    m_omEditAccCodeByte2.vSetBase(BASE_HEXADECIMAL);
    m_omEditAccCodeByte3.vSetBase(BASE_HEXADECIMAL);
    m_omEditAccCodeByte4.vSetBase(BASE_HEXADECIMAL);
    m_omEditAccMaskByte1.vSetBase(BASE_HEXADECIMAL);
    m_omEditAccMaskByte2.vSetBase(BASE_HEXADECIMAL);
    m_omEditAccMaskByte3.vSetBase(BASE_HEXADECIMAL);
    m_omEditAccMaskByte4.vSetBase(BASE_HEXADECIMAL);
    // Set the maximum text limit as two.
    m_omEditAccCodeByte1.LimitText(defMAX_TEXT_ACCEPTANCE);
    m_omEditAccCodeByte2.LimitText(defMAX_TEXT_ACCEPTANCE);
    m_omEditAccCodeByte3.LimitText(defMAX_TEXT_ACCEPTANCE);
    m_omEditAccCodeByte4.LimitText(defMAX_TEXT_ACCEPTANCE);
    m_omEditAccMaskByte1.LimitText(defMAX_TEXT_ACCEPTANCE);
    m_omEditAccMaskByte2.LimitText(defMAX_TEXT_ACCEPTANCE);
    m_omEditAccMaskByte3.LimitText(defMAX_TEXT_ACCEPTANCE);
    m_omEditAccMaskByte4.LimitText(defMAX_TEXT_ACCEPTANCE);

    // Change the text in case of USB Mode
    CWnd * pWnd = GetDlgItem(IDC_RBTN_SINGLE_FILTER_MODE);
    if( pWnd != NULL )
    {
        // Set the text to Standard
        pWnd->SetWindowText( defSTR_MSG_ID_STANDARD );
    }
    pWnd = GetDlgItem(IDC_RBTN_DUAL_FILTER_MODE);
    if( pWnd != NULL )
    {
        // Set the text to Extended
        pWnd->SetWindowText( defSTR_MSG_ID_EXTENDED );
    }	

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

/******************************************************************************/
/*  Function Name    :  OnMaxtextEditAcceptanceCode1                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to when the edit box reached  */
/*                      maximum text limit. The focus will be shifted to next */
/*                      next control in tab order.                            */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CAcceptanceFilterDlg::OnMaxtextEditAcceptanceCode1() 
{
    NextDlgCtrl();
}

/******************************************************************************/
/*  Function Name    :  OnMaxtextEditAcceptanceCode2                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to when the edit box reached  */
/*                      maximum text limit. The focus will be shifted to next */
/*                      next control in tab order.                            */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CAcceptanceFilterDlg::OnMaxtextEditAcceptanceCode2() 
{
    NextDlgCtrl();
}

/******************************************************************************/
/*  Function Name    :  OnMaxtextEditAcceptanceCode3                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to when the edit box reached  */
/*                      maximum text limit. The focus will be shifted to next */
/*                      next control in tab order.                            */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CAcceptanceFilterDlg::OnMaxtextEditAcceptanceCode3() 
{
    NextDlgCtrl( );
}

/******************************************************************************/
/*  Function Name    :  OnMaxtextEditAcceptanceCode4                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to when the edit box reached  */
/*                      maximum text limit. The focus will be shifted to next */
/*                      next control in tab order.                            */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CAcceptanceFilterDlg::OnMaxtextEditAcceptanceCode4() 
{
    NextDlgCtrl();
}

/******************************************************************************/
/*  Function Name    :  OnMaxtextEditAcceptanceMask1                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to when the edit box reached  */
/*                      maximum text limit. The focus will be shifted to next */
/*                      next control in tab order.                            */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CAcceptanceFilterDlg::OnMaxtextEditAcceptanceMask1() 
{
    NextDlgCtrl();
}

/******************************************************************************/
/*  Function Name    :  OnMaxtextEditAcceptanceMask2                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to when the edit box reached  */
/*                      maximum text limit. The focus will be shifted to next */
/*                      next control in tab order.                            */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CAcceptanceFilterDlg::OnMaxtextEditAcceptanceMask2() 
{
    NextDlgCtrl();
}

/******************************************************************************/
/*  Function Name    :  OnMaxtextEditAcceptanceMask3                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to when the edit box reached  */
/*                      maximum text limit. The focus will be shifted to next */
/*                      next control in tab order.                            */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  27.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CAcceptanceFilterDlg::OnMaxtextEditAcceptanceMask3() 
{
    NextDlgCtrl();
}

/******************************************************************************/
/*  Function Name    :  ~CAcceptanceFilterDlg                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This is destructor of the class. It is called when    */
/*                      object of this class is being destroyed. All memory   */
/*                      allocation is deleted here.                           */
/*  Member of        :  CAcceptanceFilterDlg                                  */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.04.2003                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
CAcceptanceFilterDlg::~CAcceptanceFilterDlg()
{
}

void CAcceptanceFilterDlg::OnOK()
{
    OnCbtnAccetanceOk();
}

/**
 * \brief     List hardware interfaces
 *
 * This function gets called when switched to extended filter mode
 */
void CAcceptanceFilterDlg::OnBnClickedRbtnDualFilterMode()
{
	UpdateData(TRUE);
	m_bAccFilterMode = 1;
	vSetFilterDetails(true);	
}

/**
 * \brief     List hardware interfaces
 *
 * This function gets called when switched to standard filter mode
 */
void CAcceptanceFilterDlg::OnBnClickedRbtnSingleFilterMode()
{
	UpdateData(TRUE);
	m_bAccFilterMode = 0;
	vSetFilterDetails(true);	
}

/**
 * \brief     List hardware interfaces
 *
 * This function gets called when switched to 'Accept all' filter type
 */
void CAcceptanceFilterDlg::OnBnClickedRbtnFilterAcceptAll()
{
	m_enmHWFilterType[m_bAccFilterMode] = HW_FILTER_ACCEPT_ALL;
	vEnableDisableCodeMaskFields(false);
}

/**
 * \brief     List hardware interfaces
 *
 * This function gets called when switched to 'Reject all' filter type
 */
void CAcceptanceFilterDlg::OnBnClickedRbtnFilterRejectAll()
{
	m_enmHWFilterType[m_bAccFilterMode] = HW_FILTER_REJECT_ALL;
	vEnableDisableCodeMaskFields(false);
}

/**
 * \brief     List hardware interfaces
 *
 * This function gets called when switched to 'Use mask and code' filter type
 */
void CAcceptanceFilterDlg::OnBnClickedRbtnFilterManualSet()
{
	m_enmHWFilterType[m_bAccFilterMode] = HW_FILTER_MANUAL_SET;
	vEnableDisableCodeMaskFields(true);
}

/**
 * \brief     List hardware interfaces
 *
 * Call this function to set the currently selected filter mode code, mask fields.
 */
void CAcceptanceFilterDlg::vSetFilterDetails(bool bSave)
{		
	if ( bSave )
		vSaveFilterDetails(m_bAccFilterMode);
	m_omStrAccCodeByte1 = m_omStrLocalAccCodeByte1[m_bAccFilterMode];
	m_omStrAccCodeByte2 = m_omStrLocalAccCodeByte2[m_bAccFilterMode];
	m_omStrAccCodeByte3 = m_omStrLocalAccCodeByte3[m_bAccFilterMode];
	m_omStrAccCodeByte4 = m_omStrLocalAccCodeByte4[m_bAccFilterMode];
	m_omStrAccMaskByte1 = m_omStrLocalAccMaskByte1[m_bAccFilterMode];
	m_omStrAccMaskByte2 = m_omStrLocalAccMaskByte2[m_bAccFilterMode];
	m_omStrAccMaskByte3 = m_omStrLocalAccMaskByte3[m_bAccFilterMode];
	m_omStrAccMaskByte4 = m_omStrLocalAccMaskByte4[m_bAccFilterMode];	

	vSetHWFilterTypeDetails();

	UpdateData(FALSE);
}

/**
 * \brief     List hardware interfaces
 *
 * Call this function to save code, mask filter details of previous filter mode.
 */
void CAcceptanceFilterDlg::vSaveFilterDetails(BOOL bAccFilterMode)
{
	m_omStrLocalAccCodeByte1[!bAccFilterMode] = m_omStrAccCodeByte1;
	m_omStrLocalAccCodeByte2[!bAccFilterMode] = m_omStrAccCodeByte2;
	m_omStrLocalAccCodeByte3[!bAccFilterMode] = m_omStrAccCodeByte3;
	m_omStrLocalAccCodeByte4[!bAccFilterMode] = m_omStrAccCodeByte4;
	m_omStrLocalAccMaskByte1[!bAccFilterMode] = m_omStrAccMaskByte1;
	m_omStrLocalAccMaskByte2[!bAccFilterMode] = m_omStrAccMaskByte2;
	m_omStrLocalAccMaskByte3[!bAccFilterMode] = m_omStrAccMaskByte3;
	m_omStrLocalAccMaskByte4[!bAccFilterMode] = m_omStrAccMaskByte4;
}

/**
 * \brief     List hardware interfaces
 *
 * Call this function to enable or disable the code, mask options.
 */
void CAcceptanceFilterDlg::vEnableDisableCodeMaskFields(bool bEnable)
{
	GetDlgItem(IDC_EDIT_ACCEPTANCE_CODE1)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_ACCEPTANCE_CODE2)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_ACCEPTANCE_CODE3)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_ACCEPTANCE_CODE4)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_ACCEPTANCE_MASK1)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_ACCEPTANCE_MASK2)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_ACCEPTANCE_MASK3)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_ACCEPTANCE_MASK4)->EnableWindow(bEnable);
}

/**
 * \brief     List hardware interfaces
 *
 * Call this function to set hardware filter detail related settings
 */
void CAcceptanceFilterDlg::vSetHWFilterTypeDetails()
{
	CheckDlgButton(IDC_RBTN_FILTER_ACCEPT_ALL,BST_UNCHECKED);
	CheckDlgButton(IDC_RBTN_FILTER_REJECT_ALL,BST_UNCHECKED);
	CheckDlgButton(IDC_RBTN_FILTER_MANUAL_SET,BST_UNCHECKED);
	if ( m_enmHWFilterType[m_bAccFilterMode] == HW_FILTER_ACCEPT_ALL )
	{
		CheckDlgButton(IDC_RBTN_FILTER_ACCEPT_ALL,BST_CHECKED);		
		vEnableDisableCodeMaskFields(false);
	}
	else if ( m_enmHWFilterType[m_bAccFilterMode] == HW_FILTER_REJECT_ALL )
	{
		CheckDlgButton(IDC_RBTN_FILTER_REJECT_ALL,BST_CHECKED);
		vEnableDisableCodeMaskFields(false);
	}
	else
	{
		CheckDlgButton(IDC_RBTN_FILTER_MANUAL_SET,BST_CHECKED);			
		vEnableDisableCodeMaskFields(true);
	}
}

