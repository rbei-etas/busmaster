/*****************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  SignalDetailsDlg.h
  Description   :  This header file contains the defintion of class
                   CSignalDetailsDlg. All function prototype and data
                   members of this class is defined here
  $Log:   X:/Archive/Sources/SigGrphWnd/SignalDetailsDlg.h_v  $
 *
 *    Rev 1.0   13 Dec 2010 22:00:52   CANMNTTM
 *
 *
 *    Rev 1.0   16 Aug 2010 21:20:42   rac2kor
 *

  Author(s)       :  Amarnath Shastry
  Date Created    :  21.02.2002
  Modified By     :
*****************************************************************************/
#if !defined(AFX_SIGNALDETAILSDLG_H__70C2D904_2743_11D6_A61C_00D0B76BEBF5__INCLUDED_)
#define AFX_SIGNALDETAILSDLG_H__70C2D904_2743_11D6_A61C_00D0B76BEBF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SignalDetailsDlg.h : header file
//
#include "Utility/RadixEdit.h"
#include "MsgSignal.h"
#include "Utility/AlphanumiricEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CSignalDetailsDlg dialog

class CSignalDetailsDlg : public CDialog
{
    // Construction
public:
    CSignalDetailsDlg( eMODES eMode,
                       int nDataFormat,
                       CString omStrMsgName,
                       UINT nMsgLen ,
                       CString omStrSignalType = STR_EMPTY,
                       CString omStrMaxVal = STR_EMPTY,
                       CString omStrMinVal = STR_EMPTY,
                       CString omStrOffset = _T("0"),
                       CString omStrScale = _T("1"),
                       CWnd* pParent = NULL);   // standard constructor
    CSignalDetailsDlg( eMODES eMode, sSIGNALS* psSigInfo,
                       CWnd* pParent /*=NULL*/);
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
    BYTE    m_byByteIndex;
    UINT    m_unSgLen;
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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGNALDETAILSDLG_H__70C2D904_2743_11D6_A61C_00D0B76BEBF5__INCLUDED_)
