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
 * @brief This file contain definition of all function of
 * @author Amarnath Shastry
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "TxWindow_stdafx.h"             // Standard header
#include "TxWindow_resource.h"
#include "SignalDetailsDlg.h"   // for creating and editing signal details
#include "Utility/Utility.h"      // For Utility Functions
#include "Utility/Utility_Structs.h"
#include "include/Basedefs.h"
#include "Utility\MultiLanguageSupport.h"

#define CHAR_BOOL               'B'
#define CHAR_UINT               'U'
#define defEMPTY_CHAR           ' '
#define defFORMAT_DATA_FLOAT    "%f"
#define defBOOLEAN              "bool"
#define defSIGNED_INT           "int"
#define defUNSIGNED_INT         "unsigned int"
#define defSTR_CLOSE_MENU_TEXT  "&Close"
#define defSTR_MAX_VAL_INVALID              "Maximum value is out of bounds!"
#define defSTR_MIN_VAL_INVALID              "Minimum value is out of bounds!"
#define defSTR_SIGNAL_NAME_INVALID          "Signal name cannot be empty"
#define defSTR_SIGNAL_END_BIT_INVALID       "End bit of signal is out of message length"
#define defSTR_SIGNAL_DUP_START_BIT         "Duplicate signal start bit value"
#define MSG_DUPLICATE_SG_NAME               "Signal name already exists!"
#define MSG_MAX_VAL_LESS                    "Maximum value cannot be less than minimum value!"
#define MSG_MIN_VAL_MORE                    "Minimum value cannot be more than maximum value!"

enum eMODES
{
    MD_ADD = 0,
    MD_EDIT,
    MD_DELETE,
    MD_READ_ONLY,
    MD_NONE
};

CSignalDetailsDlg::CSignalDetailsDlg(eMODES eMode,
                                     int nDataFormat,
                                     CString omStrMsgName,
                                     UINT nMsgLen,
                                     CString omStrSignalType,
                                     CString omStrMaxVal,
                                     CString omStrMinVal,
                                     CString omStrOffset,
                                     CString omStrScale,
                                     CWnd* pParent /*=nullptr*/) :
    CDialog(CSignalDetailsDlg::IDD, pParent)
{
    m_shByteIndex = 0;
    m_unSgLen = 1;
    m_omStrSignalName = "";
    m_byStartBit = 0;
    m_omStrUnit = "";
    m_nMsgLength = nMsgLen;
    m_omStrMsgName = omStrMsgName;
    m_bIsCanceled = FALSE;
    m_bIsDataSaved = TRUE;
    m_omStrPrevSignalName = "";
    m_unMode = eMode;
    m_nDataFormat = nDataFormat;
    m_omStrSgType = omStrSignalType;
    m_omStrMaxVal = omStrMaxVal;
    m_omStrMinVal = omStrMinVal;
    m_omStrOffset = omStrOffset;
    m_omStrScale  = omStrScale;
    m_bDupliacateFound = FALSE;
    m_bLenChanged = FALSE;
    m_bNameChanged = FALSE;
}

CSignalDetailsDlg::CSignalDetailsDlg( eMODES eMode,
                                      sSIGNALS* psSigInfo,
                                      CWnd* pParent /*=nullptr*/)
    : CDialog(CSignalDetailsDlg::IDD, pParent)
{
    if( psSigInfo != nullptr)
    {
        if( eMode == MD_READ_ONLY )
        {
            m_shByteIndex = (SHORT)psSigInfo->m_unStartByte-1;
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

            // Format the min/max value
            if(psSigInfo->m_bySignalType  == CHAR_INT)
            {
                m_omStrMaxVal.Format(defFORMAT_INT64_DECIMAL, psSigInfo->m_SignalMaxValue.n64Value);

                m_omStrMinVal.Format(defFORMAT_INT64_DECIMAL, psSigInfo->m_SignalMinValue.n64Value);
            }
            else
            {
                m_omStrMaxVal.Format(defFORMAT_UINT64_DEC, psSigInfo->m_SignalMaxValue.un64Value);

                m_omStrMinVal.Format(defFORMAT_UINT64_DEC, psSigInfo->m_SignalMinValue.un64Value);
            }

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
    DDX_Control(pDX, IDC_EDIT_FACTOR, m_odScale);
    DDX_Control(pDX, IDC_EDIT_OFFSET, m_odOffset);
    DDX_Control(pDX, IDC_EDIT_MIN, m_odMinValue);
    DDX_Control(pDX, IDC_EDIT_MAX, m_odMaxValue);
    DDX_Control(pDX, IDC_SPIN_SGLENGTH, m_omSpinLen);
    DDX_Control(pDX, IDC_SPIN_BYINDEX, m_omSpinByIndex);
    DDX_Control(pDX, IDC_SPIN_BIT, m_omSpinStartBit);
    DDX_Control(pDX, IDC_COMB_SGTYPE, m_omComboSgType);
    DDX_Text(pDX, IDC_EDIT_BYINDEX, m_shByteIndex);
    DDV_MinMaxShort(pDX, m_shByteIndex, 0, (SHORT)m_nMsgLength-1);
    DDX_Text(pDX, IDC_EDIT_SGLEN, m_unSgLen);
    DDV_MinMaxUInt(pDX, m_unSgLen, 1, min (64, m_nMsgLength*8));
    DDX_Text(pDX, IDC_EDIT_SGNAME, m_omStrSignalName);
    DDX_Text(pDX, IDC_EDIT_STBIT, m_byStartBit);
    DDV_MinMaxByte(pDX, m_byStartBit, 0, 7);
    DDX_Text(pDX, IDC_EDIT_UNIT, m_omStrUnit);
}

BOOL CSignalDetailsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_odMaxValue.vSetBase(BASE_DECIMAL);
    m_odMinValue.vSetBase(BASE_DECIMAL);
    m_odNumericEdit.SubclassDlgItem(IDC_EDIT_SGNAME,this);

    // Add options to the combo
    m_omComboSgType.AddString( defBOOLEAN );
    m_omComboSgType.AddString( defUNSIGNED_INT );
    m_omComboSgType.AddString( defSIGNED_INT );

    // Set range for byte index, signal length and bit index
    UINT unMaxSignalLength = static_cast <UINT> (m_nMsgLength * 8) ;
    m_omSpinLen.SetRange( 1, (short)min (64, unMaxSignalLength) );
    m_omSpinByIndex.SetRange( 0,  (short)m_nMsgLength-1 );
    m_omSpinStartBit.SetRange( 0, 7 );

    // Set Byte order (m_nDataFormat)
    CButton* pRadioIntel = (CButton*)GetDlgItem(IDC_RADIO_INTEL);
    CButton* pRadioMotorola = (CButton*)GetDlgItem(IDC_RADIO_MOTOROLA);
    if ((pRadioIntel != nullptr) && (pRadioMotorola != nullptr))
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
        CEdit* pomEdit = nullptr;
        // Unit edit box
        pomEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_UNIT));
        if( pomEdit != nullptr )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Name
        pomEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SGNAME));
        if( pomEdit != nullptr )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Length
        pomEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SGLEN));
        if( pomEdit != nullptr )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Byte Index
        pomEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_BYINDEX));
        if( pomEdit != nullptr )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Start Bit
        pomEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_STBIT));
        if( pomEdit != nullptr )
        {
            pomEdit->SetReadOnly();
        }
        // Unit Signal Length
        pomEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_SGLEN));
        if( pomEdit != nullptr )
        {
            pomEdit->SetReadOnly();
        }
        // Init the pointer to nullptr to avoid unknown access
        pomEdit = nullptr;
        // These controls doesn't support read only property
        // So Disable these
        CWnd* pomWnd = nullptr;
        // Signal Type Combobox
        pomWnd = GetDlgItem(IDC_COMB_SGTYPE);
        if( pomWnd != nullptr )
        {
            pomWnd->EnableWindow(FALSE);
        }
        // Signal Length Spin control
        pomWnd = GetDlgItem(IDC_SPIN_SGLENGTH);
        if( pomWnd != nullptr )
        {
            pomWnd->EnableWindow(FALSE);
        }
        // Signal Byte index Spin control
        pomWnd = GetDlgItem(IDC_SPIN_BYINDEX);
        if( pomWnd != nullptr )
        {
            pomWnd->EnableWindow(FALSE);
        }
        // Signal Bit index Spin control
        pomWnd = GetDlgItem(IDC_SPIN_BIT);
        if( pomWnd != nullptr )
        {
            pomWnd->EnableWindow(FALSE);
        }
        //Signal data format intel
        pomWnd = GetDlgItem(IDC_RADIO_INTEL);
        if( pomWnd != nullptr )
        {
            pomWnd->EnableWindow(FALSE);
        }
        //Signal data format motorola
        pomWnd = GetDlgItem(IDC_RADIO_MOTOROLA);
        if( pomWnd != nullptr )
        {
            pomWnd->EnableWindow(FALSE);
        }
        // Hide Ok Button
        pomWnd = GetDlgItem(IDOK);
        if( pomWnd != nullptr )
        {
            pomWnd->ShowWindow(SW_HIDE);
        }

        // Now rename cancel button to "Close" and center the button
        CButton* pomCancelButton;
        pomCancelButton = (CButton*)GetDlgItem(IDCANCEL);
        if( pomCancelButton != nullptr )
        {
            pomCancelButton->SetWindowText(_(defSTR_CLOSE_MENU_TEXT));
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

    if (m_omStrSgType == defSIGNED_INT)// for int should take signed value
    {
        m_odMaxValue.vSetSigned( TRUE );
        m_odMinValue.vSetSigned( TRUE );
    }
    else // Should take only +ve value
    {
        m_odMaxValue.vSetSigned( FALSE );
        m_odMinValue.vSetSigned( FALSE );
    }

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

void CSignalDetailsDlg::OnCancel()
{
    m_bIsCanceled = TRUE;

    CDialog::OnCancel();
}

void CSignalDetailsDlg::OnOK()
{
    CDialog::OnOK();
}

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
    {
        bSkip = CDialog::PreTranslateMessage(pMsg);
    }

    return bSkip;
}
