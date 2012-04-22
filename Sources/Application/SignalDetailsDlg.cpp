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
 * \file      SignalDetailsDlg.cpp
 * \brief     This file contain definition of all function of 
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of 
 */
#include "stdafx.h"             // Standard header
#include "BUSMASTER.h"        // App class header
#include <math.h>               // for pow(...)
#include "DataType.h"           // For max DLC CAN
#include "SignalDetailsDlg.h"   // for creating and editing signal details
#include "MainFrm.h"            // Hex validation is defined here
#include "Utility/UtilFunctions.h"      // For Utility Functions
#include ".\signaldetailsdlg.h"

extern CCANMonitorApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CSignalDetailsDlg dialog

/*******************************************************************************
 Function Name    :  CSignalDetailsDlg
 Input(s)         :  eMODES eMode,
                     int nDataFormat,
                     CString omStrMsgName,
                     UINT nMsgLen,
                     CString omStrSignalType
 Output           :
 Functionality    :  Constructor
 Member of        :  CSignalDetailsDlg
 Friend of        :      -
 Author(s)        :  Amarnath Shastry
 Date Created     :  21.02.2002
 Modifications    :
/******************************************************************************/
CSignalDetailsDlg::CSignalDetailsDlg(const SDBPARAMS& sDbParams,
                                     eMODES eMode,
                                     int nDataFormat,
                                     CString omStrMsgName, 
                                     UINT nMsgLen,
                                     CString omStrSignalType/*""*/,
                                     CString omStrMaxVal/*""*/,
                                     CString omStrMinVal/*""*/,
                                     CString omStrOffset/*""*/,
                                     CString omStrScale/*""*/,
                                     CWnd* pParent /*=NULL*/)
    : CDialog(CSignalDetailsDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSignalDetailsDlg)
    m_byByteIndex = 1;
    m_unSgLen = 1;
    m_omStrSignalName = _T("");
    m_byStartBit = 0;
    m_omStrUnit = _T("");
    m_nMsgLength = nMsgLen;
    m_omStrMsgName = omStrMsgName;
    m_bIsCanceled = FALSE;
    m_bIsDataSaved = TRUE;
    m_omStrPrevSignalName = _T("");
    m_unMode = eMode;
    m_nDataFormat = nDataFormat;
    m_omStrSgType = omStrSignalType;
    //}}AFX_DATA_INIT

    m_omStrMaxVal = omStrMaxVal;
    m_omStrMinVal = omStrMinVal;
    m_omStrOffset = omStrOffset;
    m_omStrScale  = omStrScale;
    m_bDupliacateFound = FALSE;
    m_bLenChanged = FALSE;
    m_bNameChanged = FALSE;
    m_sDbParams = sDbParams;     
}

/*******************************************************************************
 Function Name    :  CSignalDetailsDlg
 Input(s)         :  eMode - Mode of the View
                     psSigInfo - Pointer to the Signal Information
                     pParent   - Pointer to the Parent Window
 Output           :   -
 Functionality    :  Constructor
 Member of        :  CSignalDetailsDlg
 Friend of        :      -
 Author(s)        :  Raja N
 Date Created     :  22.07.2004
 Modifications    :  Raja N on 31.07.2004, Modifications as per code review
                     comments and changed code with utility class function calls
/******************************************************************************/
CSignalDetailsDlg::CSignalDetailsDlg( eMODES eMode,
                                     sSIGNALS * psSigInfo,
                                     CWnd* pParent /*=NULL*/)
                            : CDialog(CSignalDetailsDlg::IDD, pParent)
{
    if( psSigInfo != NULL)
    {
        if( eMode == MD_READ_ONLY )
        {
            m_byByteIndex = (BYTE)psSigInfo->m_unStartByte;
            m_unSgLen = psSigInfo->m_unSignalLength;
            m_omStrSignalName = psSigInfo->m_omStrSignalName;
            m_byStartBit = psSigInfo->m_byStartBit;
            m_omStrUnit = psSigInfo->m_omStrSignalUnit;
            m_nMsgLength = psSigInfo->m_unSignalLength;
            m_bIsCanceled = FALSE;
            m_bIsDataSaved = TRUE;
            m_unMode = eMode;
            m_nDataFormat = psSigInfo->m_eFormat;
            m_omStrSgType = CString((wchar_t)(psSigInfo->m_bySignalType));
        
            // Format the min value
            __int64 unVal = psSigInfo->m_SignalMaxValue.n64Value;
            CUtilFunctions::s_vRemoveUnwantedBits( unVal,
                                                 psSigInfo->m_unSignalLength);
            m_omStrMaxVal.Format(defFORMAT_INT64_HEX, unVal);

            unVal = psSigInfo->m_SignalMinValue.n64Value;
            CUtilFunctions::s_vRemoveUnwantedBits( unVal,
                                                 psSigInfo->m_unSignalLength);
            m_omStrMinVal.Format(defFORMAT_INT64_HEX, unVal);
            // Format the factor value
            m_omStrScale.Format(defFORMAT_DATA_FLOAT,psSigInfo->m_fSignalFactor);
            // Foramt the offset value
            m_omStrOffset.Format(defFORMAT_DATA_FLOAT,
                                 psSigInfo->m_fSignalOffset);
            m_bDupliacateFound = FALSE;
            m_bLenChanged = FALSE;
            m_bNameChanged = FALSE;
        }
        else
        {
            // Implementation should be done for other options
            // Don't use before that
            ASSERT( FALSE );
        }
    }
    else
    {
        ASSERT( FALSE );
    }
}

void CSignalDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSignalDetailsDlg)
    DDX_Control(pDX, IDC_EDIT_FACTOR, m_odScale);
    DDX_Control(pDX, IDC_EDIT_OFFSET, m_odOffset);
    DDX_Control(pDX, IDC_EDIT_MIN, m_odMinValue);
    DDX_Control(pDX, IDC_EDIT_MAX, m_odMaxValue);
    DDX_Control(pDX, IDC_SPIN_SGLENGTH, m_omSpinLen);
    DDX_Control(pDX, IDC_SPIN_BYINDEX, m_omSpinByIndex);
    DDX_Control(pDX, IDC_SPIN_BIT, m_omSpinStartBit);
    DDX_Control(pDX, IDC_COMB_SGTYPE, m_omComboSgType);
    DDX_Text(pDX, IDC_EDIT_BYINDEX, m_byByteIndex);
    DDV_MinMaxByte(pDX, m_byByteIndex, 1, (BYTE)m_nMsgLength);          //VENKAT
    DDX_Text(pDX, IDC_EDIT_SGLEN, m_unSgLen);
    DDV_MinMaxUInt(pDX, m_unSgLen, 1, min (64, m_nMsgLength*8));
    DDX_Text(pDX, IDC_EDIT_SGNAME, m_omStrSignalName);
    DDX_Text(pDX, IDC_EDIT_STBIT, m_byStartBit);
    DDV_MinMaxByte(pDX, m_byStartBit, 0, 7);
    DDX_Text(pDX, IDC_EDIT_UNIT, m_omStrUnit);  
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSignalDetailsDlg, CDialog)
    //{{AFX_MSG_MAP(CSignalDetailsDlg)
    ON_EN_KILLFOCUS(IDC_EDIT_SGNAME, OnKillfocusEditSgname)
    ON_EN_KILLFOCUS(IDC_EDIT_BYINDEX, OnKillfocusEditByindex)
    ON_EN_KILLFOCUS(IDC_EDIT_FACTOR, OnKillfocusEditFactor)
    ON_EN_KILLFOCUS(IDC_EDIT_OFFSET, OnKillfocusEditOffset)
    ON_EN_KILLFOCUS(IDC_EDIT_SGLEN, OnKillfocusEditSglen)
    ON_EN_KILLFOCUS(IDC_EDIT_STBIT, OnKillfocusEditStbit)
    ON_EN_KILLFOCUS(IDC_EDIT_UNIT, OnKillfocusEditUnit)
    ON_CBN_SELCHANGE(IDC_COMB_SGTYPE, OnSelchangeCombSgtype)
    ON_EN_CHANGE(IDC_EDIT_SGLEN, OnChangeEditSglen)
    ON_EN_CHANGE(IDC_EDIT_SGNAME, OnChangeEditSgname)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_RADIO_INTEL, OnBnClickedRadioIntel)
    ON_BN_CLICKED(IDC_RADIO_MOTOROLA, OnBnClickedRadioMotorola)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSignalDetailsDlg message handlers
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Fills the combo box control,
                        sets the range for spin controls
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  21.02.2002                                            */
/*  Modifications    :  Rajesh Kumar 04.03.2003                               */
/*                      modified w.r.t changed min max types                  */
/*. Modifications    :  Amitesh Bharti, 04.07.2003,                           */
/*                        Signal length spin control will have maximum value  */
/*                        as message length * 8                               */
/*  Modifications    :  Raja N on 10.03.2004                                  */
/*                      Initialised the variable to remember the previous     */
/*                      state to avoide overwrite of signal length while      */
/*                      selecing the same type again from signal detalis      */
/*  Modifications    :  Raja N on 10.03.2004                                  */
/*                      Modified init to have a read only view of the dialog  */
/*  Modifications    :  Raja N on 22.07.2004                                  */
/*                      Fixed the negative value problem for HEX number       */
/*  Modifications    :  Raja N on 31.07.2004                                  */
/*                      Implemented code review comments to check pointers    */
/******************************************************************************/

BOOL CSignalDetailsDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    m_odNumericEdit.SubclassDlgItem(IDC_EDIT_SGNAME,this);
    SIG_VALUE minVal;
    SIG_VALUE maxVal;   

    // Add options to the combo
    m_omComboSgType.AddString( defBOOLEAN );
    m_omComboSgType.AddString( defUNSIGNED_INT );
    m_omComboSgType.AddString( defSIGNED_INT );

    // Set range for byte index, signal length and bit index
    UINT unMaxSignalLength = static_cast <UINT> (m_nMsgLength * 8) ;
    m_omSpinLen.SetRange( 1, (short)min (64, unMaxSignalLength) );
    m_omSpinByIndex.SetRange( 1,  (short)m_nMsgLength );
    m_omSpinStartBit.SetRange( 0, 7 );

    // Set Byte order (m_nDataFormat)    
    CButton* pRadioIntel = (CButton*)GetDlgItem(IDC_RADIO_INTEL);
    CButton* pRadioMotorola = (CButton*)GetDlgItem(IDC_RADIO_MOTOROLA);
    if ((pRadioIntel != NULL) && (pRadioMotorola != NULL))
    {
        if (m_nDataFormat == DATA_FORMAT_INTEL)
        {
            pRadioIntel->SetCheck(BST_CHECKED);
            pRadioMotorola->SetCheck(BST_UNCHECKED);
        }
        else
        {
            pRadioIntel->SetCheck(BST_UNCHECKED);
            pRadioMotorola->SetCheck(BST_CHECKED);
        }
    }

    if ( m_unMode == MD_EDIT )
    {
        if (m_omStrSgType == defBOOLEAN)
        {
            m_omComboSgType.SetCurSel( 0 );

            // Disable the signal length
            GetDlgItem(IDC_EDIT_MIN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_MAX)->EnableWindow(FALSE);
        }
        else if (m_omStrSgType == defUNSIGNED_INT)
        {
            m_omComboSgType.SetCurSel( 2 );
            // Disable the signal length
            GetDlgItem(IDC_EDIT_MIN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDIT_MAX)->EnableWindow(TRUE);

        }
        else if (m_omStrSgType == defSIGNED_INT)
        {
            m_omComboSgType.SetCurSel( 1 );
            // Disable the signal length
            GetDlgItem(IDC_EDIT_MIN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDIT_MAX)->EnableWindow(TRUE);

        }
        else if (m_omStrSgType == STR_EMPTY)
        {
            m_omComboSgType.SetCurSel( -1 );
        }
    }
    else
    {
        if (m_unMode == MD_ADD)
        {
            // Default be BOOLEAN
            m_omComboSgType.SetCurSel( 0 );
            // Calculate maxmin values
            vCalculateMaxMinValues(minVal, maxVal);
            
            // Set values
            if (m_omStrSgType == defSIGNED_INT)
            {
                m_odMaxValue.vSetValue( maxVal.n64Value);
                m_odMinValue.vSetValue( minVal.n64Value );  
            }
            else
            {
                m_odMaxValue.vSetValue( maxVal.un64Value);
                m_odMinValue.vSetValue( minVal.un64Value);
            }

            // Disable the signal MIN AND MAX
            GetDlgItem(IDC_EDIT_MIN)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_MAX)->EnableWindow(FALSE);            
        }
    }
    if(m_unMode == MD_READ_ONLY)
    {
        if (m_omStrSgType == CHAR_BOOL)
        {
            m_omComboSgType.SetCurSel( 0 );
        }
        else if (m_omStrSgType == CHAR_UINT)
        {
            m_omComboSgType.SetCurSel( 2 );
        }
        else if (m_omStrSgType == CHAR_INT)
        {
            m_omComboSgType.SetCurSel( 1 );
        }
        else
        {
            // Invalid type found
            ASSERT( FALSE );
            m_omComboSgType.SetCurSel( -1 );
        }
        
        // Disable all controls 
        // Use control name if it is defined
        m_odScale.SetReadOnly();
        m_odOffset.SetReadOnly();
        m_odMinValue.SetReadOnly();
        m_odMaxValue.SetReadOnly();

        // Use control ID if control is not defined
        CEdit * pomEdit = NULL;
        // Unit edit box
        pomEdit = static_cast<CEdit *>(GetDlgItem(IDC_EDIT_UNIT));
        if( pomEdit != NULL )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Name
        pomEdit = static_cast<CEdit *>(GetDlgItem(IDC_EDIT_SGNAME));
        if( pomEdit != NULL )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Length
        pomEdit = static_cast<CEdit *>(GetDlgItem(IDC_EDIT_SGLEN));
        if( pomEdit != NULL )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Byte Index
        pomEdit = static_cast<CEdit *>(GetDlgItem(IDC_EDIT_BYINDEX));
        if( pomEdit != NULL )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Start Bit
        pomEdit = static_cast<CEdit *>(GetDlgItem(IDC_EDIT_STBIT));
        if( pomEdit != NULL )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Length
        pomEdit = static_cast<CEdit *>(GetDlgItem(IDC_EDIT_SGLEN));
        if( pomEdit != NULL )
        {
            pomEdit->SetReadOnly();
        }
        // Init the pointer to NULL to avoid unknown access
        pomEdit = NULL;
        // These controls doesn't support read only property
        // So Disable these
        CWnd * pomWnd = NULL;
        // Signal Type Combobox
        pomWnd = GetDlgItem(IDC_COMB_SGTYPE);
        if( pomWnd != NULL )
        {
            pomWnd->EnableWindow(FALSE);
        }
        // Signal Length Spin control
        pomWnd = GetDlgItem(IDC_SPIN_SGLENGTH);
        if( pomWnd != NULL )
        {
            pomWnd->EnableWindow(FALSE);
        }
        // Signal Byte index Spin control
        pomWnd = GetDlgItem(IDC_SPIN_BYINDEX);
        if( pomWnd != NULL )
        {
            pomWnd->EnableWindow(FALSE);
        }
        // Signal Bit index Spin control
        pomWnd = GetDlgItem(IDC_SPIN_BIT);
        if( pomWnd != NULL )
        {
            pomWnd->EnableWindow(FALSE);
        }

        // Hide Ok Button
        pomWnd = GetDlgItem(IDOK);
        if( pomWnd != NULL )
        {
            pomWnd->ShowWindow(SW_HIDE);
        }

        // Now rename cancel button to "Close" and center the button
        CButton * pomCancelButton;
        pomCancelButton = (CButton *)GetDlgItem(IDCANCEL);
        if( pomCancelButton != NULL )
        {
            pomCancelButton->SetWindowText(defSTR_CLOSE_MENU_TEXT);
            // Get the rect before centering
            CRect omRect1, omRect2;
            pomCancelButton->GetWindowRect(&omRect1);
            ScreenToClient(&omRect1);
            // Center will place the button vertically center too
            // To avoide this call SetWindowPos with old top value
            pomCancelButton->CenterWindow();
            pomCancelButton->GetWindowRect(&omRect2);
            ScreenToClient(&omRect2);
            // Change the top position
            pomCancelButton->SetWindowPos(&wndTop,
                                        omRect2.left,
                                        omRect1.top,
                                        omRect1.Width(),
                                        omRect1.Height(),
                                        SWP_SHOWWINDOW);
        }
    }
    // Shud take only hex value
    m_odMaxValue.vSetBase(BASE_HEXADECIMAL);
    m_odMinValue.vSetBase(BASE_HEXADECIMAL);
    m_odMaxValue.vSetSigned( FALSE );
    m_odMinValue.vSetSigned( FALSE );

    // Set edit values 
    if ( !m_omStrMaxVal.IsEmpty() && !m_omStrMinVal.IsEmpty())
    {
        m_odMaxValue.SetWindowText( m_omStrMaxVal );
        m_odMinValue.SetWindowText( m_omStrMinVal );
    }

    // Set offset edit control properties
    m_odOffset.vSetBase(BASE_DECIMAL);
    m_odOffset.vAcceptFloatingNum( TRUE );
    m_odOffset.vSetSigned( TRUE );

    // Set scale edit control properties
    m_odScale.vSetBase(BASE_DECIMAL);
    m_odScale.vAcceptFloatingNum( TRUE );


    // Set edit values 
    if ( !m_omStrOffset.IsEmpty() && !m_omStrScale.IsEmpty())
    {
        m_odOffset.SetWindowText( m_omStrOffset );
        m_odScale.SetWindowText( m_omStrScale );
    }

    m_omStrFirstSignalName = m_omStrSignalName;
    // Save Current type selection
    m_omComboSgType.GetWindowText(m_omStrPrevSignalType);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditSgname                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates signal name
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  22.02.2002                                            */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Removed the dead code of database structure access    */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Added check for READ_ONLY mode to avoide manipulations*/
/******************************************************************************/
void CSignalDetailsDlg::OnKillfocusEditSgname() 
{
    // check if the user has pressed cancel button
    // if yes, skip validation
    // if the user presses escape button,
    // "m_bIsCanceled" will be set to true
    if ( !m_bIsCanceled && m_unMode != MD_READ_ONLY )
    {
        CButton* pCancelButton1 = 
                (CButton*)GetDlgItem(IDCANCEL)->GetFocus();

        CButton* pCancelButton2 = 
                (CButton*)GetDlgItem(IDCANCEL);

        if ( pCancelButton1 != pCancelButton2 )
        {
            m_omStrPrevSignalName = m_omStrSignalName;

            UpdateData(TRUE);

            if ( !m_omStrSignalName.IsEmpty() )
            {
                // if name is changed, update the changes
                if ( m_omStrPrevSignalName != m_omStrSignalName )
                {
                    // Get appropriate msg structure ptr
                    m_bIsDataSaved = FALSE;
                }
            }
            else
            {
                /*AfxMessageBox( "Signal name cannot be empty", 
                    MB_OK|MB_ICONINFORMATION);
                GetDlgItem(IDC_EDIT_SGNAME)->SetFocus();*/
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditByindex                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates signal byte index
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  22.02.2002                                            */
/*  Modifications    :  
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Added check for READ_ONLY mode to avoide manipulations*/
/******************************************************************************/
void CSignalDetailsDlg::OnKillfocusEditByindex() 
{
    // check if the user has pressed cancel button
    // if yes, skip validation
    // if the user presses escape button,
    // "m_bIsCanceled" will be set to true
    if ( !m_bIsCanceled && m_unMode != MD_READ_ONLY)
    {
        CButton* pCancelButton1 = 
                (CButton*)GetDlgItem(IDCANCEL)->GetFocus();

        CButton* pCancelButton2 = 
                (CButton*)GetDlgItem(IDCANCEL);

        if ( pCancelButton1 != pCancelButton2 )
        {
            BYTE byPrevByteIndex = m_byByteIndex;

            UpdateData(TRUE);

            if (byPrevByteIndex != m_byByteIndex)
            {
                m_bIsDataSaved = FALSE;
            }
        }
    }
    
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditFactor                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates signal factor
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  22.02.2002                                            */
/*  Modifications    :  
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Added check for READ_ONLY mode to avoide manipulations*/
/******************************************************************************/
void CSignalDetailsDlg::OnKillfocusEditFactor() 
{
    // check if the user has pressed cancel button
    // if yes, skip validation
    // if the user presses escape button,
    // "m_bIsCanceled" will be set to true
    if ( !m_bIsCanceled && m_unMode != MD_READ_ONLY)
    {
        CButton* pCancelButton1 = 
                (CButton*)GetDlgItem(IDCANCEL)->GetFocus();

        CButton* pCancelButton2 = 
                (CButton*)GetDlgItem(IDCANCEL);

        if ( pCancelButton1 != pCancelButton2 )
        {
            m_bIsDataSaved = FALSE;
        }
    }
}
/******************************************************************************/
/*  Function Name    :  bIsEditMinMaxValueValid                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Validates signal maximum and minimum value            */
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Krishnaswamy B.N                                      */
/*  Date Created     :  28.08.2003                                            */
/******************************************************************************/
BOOL CSignalDetailsDlg::bIsEditMinMaxValueValid() 
{
    // check if the user has pressed cancel button
    // if yes, skip validation
    // if the user presses escape button,
    // "m_bIsCanceled" will be set to true

    BOOL bRetVal = TRUE;

    if ( !m_bIsCanceled )
    {
        CButton* pCancelButton1 = 
                (CButton*)GetDlgItem(IDCANCEL)->GetFocus();

        CButton* pCancelButton2 = 
                (CButton*)GetDlgItem(IDCANCEL);

        if ( pCancelButton1 != pCancelButton2 )
        {
            // Get max and min values
            m_odMaxValue.GetWindowText( m_omStrMaxVal );
            m_odMinValue.GetWindowText( m_omStrMinVal );
            UINT unMaxLength = m_omStrMaxVal.GetLength();
            UINT unMinLength = m_omStrMinVal.GetLength();
            //check for the empty string
            if(unMinLength == 0)
            {
                AfxMessageBox( "Minimum value field can't be empty!", 
                                    MB_OK|MB_ICONINFORMATION);
                UpdateData(FALSE);
                GetDlgItem(IDC_EDIT_MIN)->SetFocus();
                bRetVal = FALSE;
            }
            else if(unMaxLength == 0)
            {
                AfxMessageBox( "Maximum value field can't be empty!", 
                                    MB_OK|MB_ICONINFORMATION);
                UpdateData(FALSE);
                GetDlgItem(IDC_EDIT_MAX)->SetFocus();
                bRetVal = FALSE;
            }//check for the boundary condition
            else if( bIsMaximumValueValid() == FALSE) 
            {
                UpdateData(FALSE);
                GetDlgItem(IDC_EDIT_MAX)->SetFocus();
                bRetVal = FALSE;
            }
            else
            {
                if(bIsMinimumValueValid() == FALSE) 
                {
                    UpdateData(FALSE);
                    GetDlgItem(IDC_EDIT_MIN)->SetFocus();
                    bRetVal = FALSE;
                }
            }
        }
    }
    return bRetVal;
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditOffset                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates signal Offset value
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  22.02.2002                                            */
/*  Modifications    :  
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Added check for READ_ONLY mode to avoide manipulations*/
/******************************************************************************/
void CSignalDetailsDlg::OnKillfocusEditOffset() 
{
    // check if the user has pressed cancel button
    // if yes, skip validation
    // if the user presses escape button,
    // "m_bIsCanceled" will be set to true
    if ( !m_bIsCanceled && m_unMode != MD_READ_ONLY)
    {
        CButton* pCancelButton1 = 
                (CButton*)GetDlgItem(IDCANCEL)->GetFocus();

        CButton* pCancelButton2 = 
                (CButton*)GetDlgItem(IDCANCEL);

        if ( pCancelButton1 != pCancelButton2 )
        {
            m_bIsDataSaved = FALSE;
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditSgLen                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates signal length
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  22.02.2002                                            */
/*  Modifications    :  rajesh: 05.03.2003 : Min-max values changes
/*. Modifications    :  Amitesh Bharti, 04.07.2003,                           */
/*                        Signal length going beyond message length is        */
/*                         validated                                          */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Added check for READ_ONLY mode to avoide manipulations*/
/******************************************************************************/
void CSignalDetailsDlg::OnKillfocusEditSglen() 
{
    // check if the user has pressed cancel button
    // if yes, skip validation
    // if the user presses escape button,
    // "m_bIsCanceled" will be set to true
    
    SIG_VALUE minVal, maxVal;
    BOOL bError = FALSE;
    if ( !m_bIsCanceled && m_unMode != MD_READ_ONLY)
    {
        CButton* pCancelButton1 = 
                (CButton*)GetDlgItem(IDCANCEL)->GetFocus();

        CButton* pCancelButton2 = 
                (CButton*)GetDlgItem(IDCANCEL);

        if ( pCancelButton1 != pCancelButton2 )
        {

            UpdateData(TRUE);

            // Validate signal length 
            // depending on the type
            CString omStrPrevSgName = STR_EMPTY;

            m_omComboSgType.GetWindowText(omStrPrevSgName);

            if ( !omStrPrevSgName.IsEmpty() )
            {
                if ( omStrPrevSgName == defBOOLEAN )
                {
                    if (m_unSgLen > 1)
                    {
                        m_unSgLen = 1;
                        bError = TRUE;
                        GetDlgItem(IDC_EDIT_SGLEN)->SetFocus();
                    }
                }
                else if ( omStrPrevSgName == defSIGNED_INT )
                {
                    if (m_unSgLen < 2)
                    {
                        m_unSgLen = 2;
                        bError = TRUE;
                        GetDlgItem(IDC_EDIT_SGLEN)->SetFocus();
                    }
                }
            }

            if ( bError != TRUE)
            {
                if ( m_unSgLen > 0 )
                {
                    // check for duplicate start bit value
                    // Auto-Update the max and min value 
                    // depending on signal byte index, bit index and type
                    if ( m_bLenChanged == TRUE)
                    {
                        UINT unMaxSignalLength = 
                                static_cast<UINT> (m_nMsgLength * 8);
                        if ( m_unSgLen <= unMaxSignalLength )
                        {
                            vCalculateMaxMinValues(minVal, maxVal);
                            if(omStrPrevSgName == defSIGNED_INT)
                            {
                                m_odMaxValue.vSetValue( maxVal.n64Value);
                                m_odMinValue.vSetValue( minVal.n64Value);
                            }
                            else
                            {
                                m_odMaxValue.vSetValue( maxVal.un64Value);
                                m_odMinValue.vSetValue( minVal.un64Value);
                            }
                        }
                    }
                    else
                    {
                        // Set values
                        m_odMaxValue.vSetValue( m_odMaxValue.lGetValue() );
                        m_odMinValue.vSetValue( m_odMinValue.lGetValue() );
                    }

                    m_bDupliacateFound = FALSE;
                    m_bIsDataSaved = FALSE;
                }
                else
                {
                    m_unSgLen = 1;
                    bError = TRUE;
                }
            }
            if ( bError == TRUE )
            {
                UpdateData(FALSE);
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditStBit                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates signal start bit
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  25.02.2002                                            */
/*  Modifications    :  Raja N on 10.03.2004
/*                      Removed the dead code of accessing the database       */
/*                      structure                                             */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Added check for READ_ONLY mode to avoide manipulations*/
/******************************************************************************/
void CSignalDetailsDlg::OnKillfocusEditStbit() 
{
    // check if the user has pressed cancel button
    // if yes, skip validation
    // if the user presses escape button,
    // "m_bIsCanceled" will be set to true
    if ( !m_bIsCanceled && m_unMode != MD_READ_ONLY)
    {
        CButton* pCancelButton1 = 
                (CButton*)GetDlgItem(IDCANCEL)->GetFocus();

        CButton* pCancelButton2 = 
                (CButton*)GetDlgItem(IDCANCEL);

        if ( pCancelButton1 != pCancelButton2 )
        {
            BYTE unPrevSgStBit = m_byStartBit;

            UpdateData(TRUE);

            if (unPrevSgStBit != m_byStartBit)
            {
                m_bIsDataSaved = FALSE;
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnKillfocusEditUnit                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Validates signal unit
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  25.02.2002                                            */
/*  Modifications    :  
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Added check for READ_ONLY mode to avoide manipulations*/
/******************************************************************************/
void CSignalDetailsDlg::OnKillfocusEditUnit() 
{
    // check if the user has pressed cancel button
    // if yes, skip validation
    // if the user presses escape button,
    // "m_bIsCanceled" will be set to true
    if ( !m_bIsCanceled && m_unMode != MD_READ_ONLY)
    {
        CButton* pCancelButton1 = 
                (CButton*)GetDlgItem(IDCANCEL)->GetFocus();

        CButton* pCancelButton2 = 
                (CButton*)GetDlgItem(IDCANCEL);

        if ( pCancelButton1 != pCancelButton2 )
        {
            CString omStrPrevSgUnit = m_omStrUnit;

            UpdateData(TRUE);

            // if name is changed, update the changes
            if ( omStrPrevSgUnit != m_omStrUnit )
            {
                m_bIsDataSaved = FALSE;
            }
        }
    }
    
}
/******************************************************************************/
/*  Function Name    :  OnSelchangeCombSgtype                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  On change of signal type, corresponding value
                        is set and max and min values are calculated
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  25.02.2002                                            */
/*  Modifications    :  Rajesh Kumar : 04.03.2003
/*                      min max type changes            
/*  Modifications    :  Raja N on 10.03.2004                                  */
/*                      Added code to check the previous selection before     */
/*                      changing the signal len to default                    */
/*  Modifications    :  Raja N on 22.07.2004                                  */
/*                      Removed code to set negative value for the Max/Min    */
/*                      values as these are displayed in hex format           */
/*  Modifications    :  Raja N on 31.07.2004, Added checks of returned pointer*/
/******************************************************************************/
void CSignalDetailsDlg::OnSelchangeCombSgtype() 
{
    if ( !m_bIsCanceled )
    {
        SIG_VALUE minVal, maxVal;
        CWnd * pomWnd = NULL;
        CButton* pCancelButton1 = NULL;
        CButton* pCancelButton2 = NULL;
        // Get 
        pomWnd = GetDlgItem(IDCANCEL);
        if( pomWnd != NULL )
        {
            pCancelButton1 = static_cast<CButton*>(pomWnd->GetFocus());
        }
        pCancelButton2 = static_cast<CButton*>(GetDlgItem(IDCANCEL));
        if ( pCancelButton1 != pCancelButton2 )
        {
            CString omStrPrevSgName = STR_EMPTY;

            m_omComboSgType.GetWindowText(omStrPrevSgName);

            if ( !omStrPrevSgName.IsEmpty() 
                    && m_omStrPrevSignalType != omStrPrevSgName )
            {
                // Auto-update the signal length
                // for a given signal type
                if ( (omStrPrevSgName == defBOOLEAN)  || 
                     (omStrPrevSgName == defUNSIGNED_INT) )
                {
                    m_unSgLen = 1;
                }
                else if ( omStrPrevSgName == defSIGNED_INT )
                {
                    m_unSgLen = 2;
                }

                // Auto-Update the max and min value 
                // depending on signal byte index, bit index and type
                vCalculateMaxMinValues(minVal, maxVal);
                // Set values
                if(omStrPrevSgName == defSIGNED_INT)
                {
                    m_odMaxValue.vSetValue( maxVal.n64Value);
                    m_odMinValue.vSetValue( minVal.n64Value);
                }
                else
                {
                    m_odMaxValue.vSetValue( maxVal.un64Value);
                    m_odMinValue.vSetValue( minVal.un64Value);
                }
                // rajesh : 04.03.2003: END

                m_bIsDataSaved = FALSE;

                if ( omStrPrevSgName == defBOOLEAN)
                {
                    GetDlgItem(IDC_EDIT_MIN)->EnableWindow(FALSE);
                    GetDlgItem(IDC_EDIT_MAX)->EnableWindow(FALSE);
                }
                else
                {
                    GetDlgItem(IDC_EDIT_MIN)->EnableWindow(TRUE);
                    GetDlgItem(IDC_EDIT_MAX)->EnableWindow(TRUE);
                }
                
                //Store Current Selection
                m_omStrPrevSignalType = omStrPrevSgName;

                UpdateData(FALSE);
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  OnCancel                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Sets a flag and calls base class function
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  25.02.2002                                            */
/*  Modifications    :  
/******************************************************************************/
void CSignalDetailsDlg::OnCancel() 
{
    m_bIsCanceled = TRUE;

    CDialog::OnCancel();
}
/******************************************************************************/
/*  Function Name    :  OnOk                                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Updates the signal details to the data structure      */
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  25.02.2002                                            */
/*. Modifications    :  Amitesh Bharti, 05.06.2003,                           */
/*                                  replace datatype for supporting 64bits to */
/*                                   __int64                                  */
/*. Modifications    :  Amitesh Bharti, 04.07.2003,                           */
/*                        Signal length going beyond message length is        */
/*                         validated                                          */
/*. Modifications    :  Krishnaswamy B.N 28.08.2003,                          */
/*                      Checking for the validity of maximum and minimum value*/
/*  Modifications    :  Raja N.                                               */
/*                      14.01.2004, The memory allocation failure message was */
/*                      thrown in a wrong else condtion. Corrected            */
/*  Modifications    :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation                                      */
/*  Modifications    :  Raja N on 22.07.2004                                  */
/*                      Added check to prevent processing data in read only   */
/*                      mode and modified the value to extend sign bit        */
/*  Modifications    :  Raja N on 31.07.2004, Removed hardcoded strings       */
/******************************************************************************/
void CSignalDetailsDlg::OnOK() 
{
    BOOL bReturnFlag = TRUE;
    if(m_unMode != MD_READ_ONLY)
    {
        
        UpdateData(TRUE);
        if ( m_omStrSignalName.IsEmpty() )
        {
            AfxMessageBox( defSTR_SIGNAL_NAME_INVALID,
                           MB_OK | MB_ICONINFORMATION );
            GetDlgItem(IDC_EDIT_SGNAME)->SetFocus();
            bReturnFlag = FALSE;
        }
        else if(bIsEditMinMaxValueValid() == FALSE)
        {
            bReturnFlag = FALSE;
        }
        else if(bReturnFlag == TRUE)
        {
            // Get appropriate msg structure ptr
            CMsgSignal* pTempMsgSg = NULL;

            pTempMsgSg = (*(CMsgSignal**)(m_sDbParams.m_ppvActiveDB));

            if(pTempMsgSg != NULL)
            {
                if ( m_bNameChanged && 
                    (pTempMsgSg->bIsDuplicateSignalName( m_omStrMsgName,
                     m_omStrSignalName)) && bReturnFlag == TRUE)
                {
                    AfxMessageBox( MSG_DUPLICATE_SG_NAME, MB_OK|MB_ICONINFORMATION);
                    m_omStrSignalName.Empty();
                    UpdateData(FALSE);
                    GetDlgItem( IDC_EDIT_SGNAME )->SetFocus();
                    m_bNameChanged = FALSE;
                    bReturnFlag = FALSE;
                }
            }
            if(!(CMsgSignal::bValidateSignal(m_nMsgLength, m_byByteIndex, 
                                                m_byStartBit, m_unSgLen, 
                                                (EFORMAT_DATA) m_nDataFormat)) && 
                                                (bReturnFlag == TRUE))
            {
                AfxMessageBox( defSTR_SIGNAL_END_BIT_INVALID,
                                            MB_OK | MB_ICONINFORMATION );
                GetDlgItem( IDC_EDIT_SGLEN )->SetFocus();
                bReturnFlag = FALSE;
            }
            // check for duplicate start bit value
            if ( pTempMsgSg->bIsDuplicateSignalStartBitValue( m_omStrMsgName,
                        m_byByteIndex, m_unSgLen, m_byStartBit, m_nDataFormat )
                        && bReturnFlag == TRUE)
            {
                AfxMessageBox( defSTR_SIGNAL_DUP_START_BIT,
                                            MB_OK | MB_ICONINFORMATION );
                GetDlgItem( IDC_EDIT_STBIT )->SetFocus();
                bReturnFlag = FALSE;
            }

            if ( m_unMode == MD_ADD && bReturnFlag == TRUE )
            {
                // Allocate memory to the new signal
                pTempMsgSg->bAddSignalToMsg( m_omStrMsgName );
            }
            if ( bReturnFlag == TRUE)
            {

                sSIGNALS* pSg = new sSIGNALS;

                if ( pSg != NULL )
                {
                    pSg->m_eFormat = (EFORMAT_DATA) m_nDataFormat;
                    CString omStrSgType = STR_EMPTY;
                    m_omComboSgType.GetLBText( 
                        m_omComboSgType.GetCurSel(), omStrSgType );
                    if ( omStrSgType == defBOOLEAN )
                    {
                        pSg->m_bySignalType = CHAR_BOOL;
                    }
                    else if ( omStrSgType == defUNSIGNED_INT )
                    {
                        pSg->m_bySignalType = CHAR_UINT;
                    }
                    else if ( omStrSgType == defSIGNED_INT )
                    {
                        pSg->m_bySignalType = CHAR_INT;
                    }
                    pSg->m_fSignalFactor            = m_odScale.fGetValue();

                    if(pSg->m_bySignalType == CHAR_INT)
                    {
                        pSg->m_SignalMaxValue.n64Value   =
                            (__int64)m_odMaxValue.lGetValue();
                        pSg->m_SignalMinValue.n64Value   =
                            (__int64)m_odMinValue.lGetValue();
                        // Change the values to get app. decimal value
                        if( m_unSgLen < defMAX_BITS )
                        {
                            // Extend the sign bit to actual value
                            CUtilFunctions::s_vExtendSignBit(
                                                pSg->m_SignalMaxValue.n64Value,
                                                m_unSgLen );
                            CUtilFunctions::s_vExtendSignBit(
                                                pSg->m_SignalMinValue.n64Value,
                                                m_unSgLen );
                        }
                    }
                    else
                    {
                        pSg->m_SignalMaxValue.un64Value   =
                            (__int64)m_odMaxValue.lGetValue();
                        pSg->m_SignalMinValue.un64Value   =
                            (__int64)m_odMinValue.lGetValue();
                    }

                    pSg->m_fSignalOffset            = m_odOffset.fGetValue();
                    pSg->m_omStrSignalName          = m_omStrSignalName;
                    pSg->m_omStrSignalUnit          = m_omStrUnit;
                    pSg->m_unSignalLength           = m_unSgLen;
                    pSg->m_unStartByte              = m_byByteIndex;
                    pSg->m_byStartBit               = m_byStartBit;

                    if ( m_unMode == MD_ADD )
                        m_omStrPrevSignalName = STR_EMPTY;

                    // Fill the matrix for edited signal
                    pTempMsgSg->vUpdateSignalDetails( m_omStrMsgName,
                        m_omStrPrevSignalName,  pSg );                      

                    // Now saved
                    m_bIsDataSaved = TRUE;

                    delete pSg;

                    pSg = NULL;
                }
                else
                {
                    AfxMessageBox(MSG_MEMORY_CONSTRAINT, MB_OK|MB_ICONINFORMATION);
                }
            }
        }
    }
    if( bReturnFlag == TRUE )
    {
        CDialog::OnOK();
    }
}
/******************************************************************************/
/*  Function Name    :  vCalculateMaxMinValues                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Calculates max and mininmum value 
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  25.02.2002                                            */
/*  Modifications    :  Rajesh Kumar 04.03.2003 - arguments changed           */
/*  Modifications    :  Amitesh Bharti, 05.06.2003,                           */
/*                      replace datatype for supporting 64bits to __int64     */
/*  Modifications    :  Raja N, 22.07.2004                                    */
/*                      Modified the Min and Max calculation for -ve value fix*/
/*  Modifications    :  Raja N on 31.07.2004, Removed hardcoded strings       */
/******************************************************************************/
void CSignalDetailsDlg::vCalculateMaxMinValues(SIG_VALUE &rMinVal, 
                                               SIG_VALUE &rMaxVal)
{
    // Get signal type
    CString omStrSgType = STR_EMPTY;

    m_omComboSgType.GetWindowText(omStrSgType );
    
    // if signal type is bool and signal length is 1
    // then set max and min values to 1 and 0
    if ( omStrSgType == defBOOLEAN && m_unSgLen == 1 )
    {
        rMinVal.un64Value = FALSE;
        rMaxVal.un64Value = TRUE;
    }
    // if signal type is UINT the max value is {(2**sigLen)-1}
    else if ( omStrSgType == defUNSIGNED_INT )
    {
        rMinVal.un64Value = 0;
        rMaxVal.un64Value = 1;
        UINT nPower = m_unSgLen;
        if(m_unSgLen == defMAX_BITS )
        {
            nPower = m_unSgLen - 1;
        }
        // Get the 2 power Signal Len
        // 2 ^ n == 2 << n
        rMaxVal.un64Value <<= nPower;
        // Subtract one to get max val
        rMaxVal.un64Value = rMaxVal.un64Value - 1;
    }
    // if signal type is INT the max and min values are
    // {(2**(sigLen-1))-1} and -{(2**(sigLen-1))-1}
    else if ( omStrSgType == defSIGNED_INT )
    {
        rMaxVal.n64Value = 1;        
        // The Range is from - (2 ^ Signal Len) to + ( min - 1 )
        // say for 8 bits it is - (128) to + (127)
        // Find the min first
        rMinVal.n64Value = rMaxVal.n64Value << (m_unSgLen - 1);
        rMaxVal.n64Value = rMinVal.n64Value - 1;
    }

    m_bLenChanged = FALSE;
    UpdateData( FALSE );
}

/******************************************************************************/
/*  Function Name    :  OnChangeEditSglen                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Sets a flag to oindicate a change
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  25.04.2002                                            */
/*  Modifications    :  
/******************************************************************************/
void CSignalDetailsDlg::OnChangeEditSglen() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    m_bLenChanged = TRUE;
    
}
/******************************************************************************/
/*  Function Name    :  OnChangeEditSgname                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Sets a flag to oindicate a change
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  20.05.2002                                            */
/*  Modifications    :  
/******************************************************************************/

void CSignalDetailsDlg::OnChangeEditSgname() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    
    // TODO: Add your control notification handler code here
    m_bNameChanged = TRUE;
}
/******************************************************************************/
/*  Function Name    :  PreTranslateMessage                                   */
/*                                                                            */
/*  Input(s)         :  MSG* pMsg                                             */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Doesn't process a space
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  23.10.2002                                            */
/*  Modifications    :  Raja N on 31.07.2004, Removed hardcoded strings       */
/******************************************************************************/
BOOL CSignalDetailsDlg::PreTranslateMessage(MSG* pMsg) 
{
    // Capture the space character and 
    // do not process the same
    BOOL bSkip = FALSE;
    if ( pMsg->message == WM_CHAR )
    {
        if ( pMsg->wParam == defEMPTY_CHAR)
        {
            bSkip = TRUE;
        }
    }
    if ( bSkip == FALSE)
        bSkip = CDialog::PreTranslateMessage(pMsg);

    return bSkip;
}
/******************************************************************************/
/*  Function Name    :  bIsMaximumValueValid                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Checks for the validity of maximum value              */
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Krishnaswamy B.N                                      */
/*  Date Created     :  28.08.2003                                            */
/*  Modifications    :  Raja N on 22.07.2004, Added function call to extend   */
/*                      the sign bit to get actual value for signed number    */
/*  Modifications    :  Raja N on 31.07.2004, Removed hardcoded strings       */
/******************************************************************************/
BOOL CSignalDetailsDlg::bIsMaximumValueValid()
{
    BOOL bRetVal = TRUE;
    SIG_VALUE  minVal,maxVal;
    CString strSignalType;
    m_omComboSgType.GetLBText(m_omComboSgType.GetCurSel(), strSignalType);
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    if(strSignalType == defSIGNED_INT)
    {
        __int64 n64;         
        if(pMainFrame != NULL)
        {
            n64 = pMainFrame->nConvertStringToInt(m_omStrMaxVal);
        }
        __int64 n64MinVal   = m_odMinValue.lGetValue();
        // Extend the Sign bit
        CUtilFunctions::s_vExtendSignBit( n64MinVal, m_unSgLen );
        CUtilFunctions::s_vExtendSignBit( n64, m_unSgLen );
        if ( n64 < n64MinVal )
        {
            AfxMessageBox(MSG_MAX_VAL_LESS, MB_OK|MB_ICONINFORMATION);
            bRetVal = FALSE;
        }
        else
        {
            if ( m_unSgLen <= defMAX_BITS )
            {
                // Get possible max and min value
                vCalculateMaxMinValues( minVal, maxVal);
                if ( n64 > maxVal.n64Value )
                {
                    AfxMessageBox( defSTR_MAX_VAL_INVALID,
                                   MB_OK | MB_ICONINFORMATION );
                    m_odMaxValue.vSetValue( maxVal.n64Value );
                    bRetVal = FALSE; 
                }
                else
                {
                    // Accept the changes
                    m_bIsDataSaved = FALSE;
                }
            }
            else
            {
                // Accept the changes
                m_bIsDataSaved = FALSE;
            }
        }
    }
    else // unsigned int / boolean
    {
        __int64 un64 = pMainFrame->nConvertStringToInt(m_omStrMaxVal);
        __int64 un64MinVal    = m_odMinValue.lGetValue();

        int nLength = m_omStrMaxVal.GetLength();
        if( nLength > 16 )
        {   
             AfxMessageBox( defSTR_MAX_VAL_INVALID, MB_OK | MB_ICONINFORMATION);
             bRetVal = FALSE;
        }
        if ( un64 < un64MinVal && bRetVal == TRUE)
        {
            AfxMessageBox(MSG_MAX_VAL_LESS, MB_OK|MB_ICONINFORMATION);
            bRetVal = FALSE;
        }
        else if(bRetVal == TRUE)
        {
            if ( m_unSgLen <= defMAX_BITS )
            {
                // Get possible max and min value
                vCalculateMaxMinValues( minVal, maxVal);
                if ( un64 > maxVal.un64Value )
                {
                    AfxMessageBox( defSTR_MAX_VAL_INVALID,
                                   MB_OK | MB_ICONINFORMATION );
                    m_odMaxValue.vSetValue(maxVal.un64Value);
                    bRetVal = FALSE; 
                }
                else
                {
                    // Accept the changes
                    m_bIsDataSaved = FALSE;
                }
            }
            else
            {
                // Accept the changes
                m_bIsDataSaved = FALSE;
            }
        }
    }
    return bRetVal;
}
/******************************************************************************/
/*  Function Name    :  bIsMinimumValueValid                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Checks for the validity of minimum value              */
/*  Member of        :  CSignalDetailsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Krishnaswamy B.N                                      */
/*  Date Created     :  28.08.2003                                            */
/*  Modifications    :  Raja N on 22.07.2004, Added function call to extend   */
/*                      the sign bit to get actual value for signed number    */
/*  Modifications    :  Raja N on 31.07.2004, Removed hardcoded strings       */
/******************************************************************************/
BOOL CSignalDetailsDlg::bIsMinimumValueValid()
{
    BOOL bRetVal = TRUE;
    SIG_VALUE  minVal,maxVal;
    CString strSignalType;
    m_omComboSgType.GetLBText(m_omComboSgType.GetCurSel(), strSignalType);
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    if(strSignalType == defSIGNED_INT)
    {
        __int64 n64 = 0;
        if(pMainFrame != NULL)
        {
            n64 = pMainFrame->nConvertStringToInt(m_omStrMinVal);
            // Extend the Sign Bit
            CUtilFunctions::s_vExtendSignBit( n64, m_unSgLen );
        }
        __int64 n64MaxVal   = m_odMaxValue.lGetValue();
        CUtilFunctions::s_vExtendSignBit( n64MaxVal, m_unSgLen );
        if ( n64 > n64MaxVal )
        {
            AfxMessageBox(MSG_MIN_VAL_MORE, MB_OK|MB_ICONINFORMATION);
            bRetVal = FALSE;
        }
        else
        {
            if ( m_unSgLen <= defMAX_BITS )
            {
                // Get possible max and min value
                vCalculateMaxMinValues( minVal,maxVal);
                // Extend the Sign Bit to get the actual value
                CUtilFunctions::s_vExtendSignBit( minVal.n64Value, m_unSgLen);
                if ( n64 < minVal.n64Value)
                {
                    AfxMessageBox( defSTR_MIN_VAL_INVALID,
                                   MB_OK | MB_ICONINFORMATION );
                    m_odMinValue.vSetValue(minVal.n64Value);
                    bRetVal = FALSE;
                }
                else
                {
                    // Accept the changes
                    m_bIsDataSaved = FALSE;
                }
            }
            else
            {
                UpdateData(TRUE);
            }
        }
    }
    else // unsigned int
    {
        __int64 un64 = 0;
        if(pMainFrame != NULL)
        {
            un64 = pMainFrame->nConvertStringToInt(m_omStrMinVal);
        }
        __int64 un64MaxVal   = m_odMaxValue.lGetValue();
         if ( un64 > un64MaxVal )
        {
            AfxMessageBox(MSG_MIN_VAL_MORE, MB_OK|MB_ICONINFORMATION);
            bRetVal = FALSE;
        }
        else
        {
    
            if ( m_unSgLen <= defMAX_BITS )
            {
                // Get possible max and min value
                vCalculateMaxMinValues( minVal,maxVal);
                if ( un64 < minVal.un64Value)
                {
                    AfxMessageBox( defSTR_MIN_VAL_INVALID,
                                   MB_OK | MB_ICONINFORMATION );
                    m_odMinValue.vSetValue(minVal.un64Value);
                    bRetVal = FALSE;
                }
                else
                {
                    // Accept the changes
                    m_bIsDataSaved = FALSE;
                }
            }
            else
            {
                UpdateData(TRUE);
            }
        }
    }
    return bRetVal;
}

void CSignalDetailsDlg::OnBnClickedRadioIntel()
{
    m_nDataFormat = DATA_FORMAT_INTEL;
}

void CSignalDetailsDlg::OnBnClickedRadioMotorola()
{
    m_nDataFormat = DATA_FORMAT_MOTOROLA;
    // TODO: Add your control notification handler code here
}
