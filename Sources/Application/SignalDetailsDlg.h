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
 * \file      SignalDetailsDlg.h
 * \brief     This header file contains the defintion of class
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include "HashDefines.h"
#include "Utility/RadixEdit.h"
#include "MsgSignal.h"
#include "Utility/AlphanumiricEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CSignalDetailsDlg dialog

class CSignalDetailsDlg : public CDialog
{
    // Construction
public:
    CSignalDetailsDlg( const SDBPARAMS& sParams,
                       eMODES eMode,
                       int nDataFormat,
                       CString omStrMsgName,
                       UINT nMsgLen ,
                       CString omStrSignalType = "",
                       CString omStrMaxVal = "",
                       CString omStrMinVal = "",
                       CString omStrOffset = "0",
                       CString omStrScale = "1",
                       CWnd* pParent = nullptr);   // standard constructor
    CSignalDetailsDlg( eMODES eMode, sSIGNALS* psSigInfo,
                       CWnd* pParent /*=nullptr*/);
    // Dialog Data
    //{{AFX_DATA(CSignalDetailsDlg)
    enum { IDD = IDD_DLG_SIGNAL };
    CRadixEdit  m_odScale;
    CRadixEdit  m_odOffset;
    CRadixEdit  m_odMinValue;
    CRadixEdit  m_odMaxValue;
    CSpinButtonCtrl m_omSpinLen;
    CSpinButtonCtrl m_omSpinByIndex;
    CSpinButtonCtrl m_omSpinStartBit;
    CComboBox   m_omComboSgType;
    SHORT    m_shByteIndex;
    UINT    m_unSgLen;
    UINT m_unMinVal;
    CString m_omStrSignalName;
    BYTE    m_byStartBit;
    CString m_omStrUnit;
    //}}AFX_DATA

    CAlphanumiricEdit m_odNumericEdit;
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSignalDetailsDlg)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

public:
    bool IsRangeChanged();

    BOOL ValidateSignalShortName(CString omStrSignalShortName);
    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSignalDetailsDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnKillfocusEditSgname();
    afx_msg void OnKillfocusEditByindex();
    afx_msg void OnKillfocusEditFactor();
    afx_msg void OnKillfocusEditOffset();
    afx_msg void OnKillfocusEditSglen();
    afx_msg void OnKillfocusEditStbit();
    afx_msg void OnKillfocusEditUnit();
    afx_msg void OnSelchangeCombSgtype();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnChangeEditSglen();
    afx_msg void OnChangeEditSgname();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    SDBPARAMS m_sDbParams;
    BOOL m_bLenChanged;
    CString m_omStrFirstSignalName;
    BOOL m_bDupliacateFound;
    // rajesh : 04.03.2003 : changed arguments
    void vCalculateMaxMinValues(SIG_VALUE& rMinVal, SIG_VALUE& rMaxVal);
    // void vCalculateMaxMinValues(long &minVal, long &maxVal);
    //  krishnaswamy B.N 28.08.2003
    //  Validation of minimum and maximum values in OnOk function
    BOOL bIsEditMinMaxValueValid();
    BOOL bIsMaximumValueValid();
    BOOL bIsMinimumValueValid();
    BOOL bIsDataModified(); //KSS
    void vSetInitialData(); //KSS
    BOOL SaveSigLength();   //added to save length and calculate range of data

    CString m_omStrSgType;
    int m_nDataFormat;
    UINT m_unMode;
    CString m_omStrPrevSignalName;
    CString m_omStrPrevSignalType;
    BOOL m_bIsDataSaved;
    BOOL m_bIsCanceled;
    CString m_omStrMsgName;
    UINT m_nMsgLength;
    CString m_omStrMaxVal;
    CString m_omStrMinVal;
    CString m_omStrOffset;
    CString m_omStrScale;
    BOOL m_bNameChanged;
    afx_msg void OnBnClickedRadioIntel();
    afx_msg void OnBnClickedRadioMotorola();
    //KSS
    // Member variables to store the intial values for omparison later
    int     m_nDataFormatInitial;
    UINT    m_unSgLenInitial;
    SHORT   m_shByteIndexInitial;
    BYTE    m_byStartBitInitial;
    CString m_omStrSignalNameInitial;
    CString m_omStrUnitInitial;
    CString m_omStrMaxValInitial;
    CString m_omStrMinValInitial;
    CString m_omStrOffsetInitial;
    CString m_omStrScaleInitial;
    CString m_omStrSgTypeInitial;
    //KSS
};
