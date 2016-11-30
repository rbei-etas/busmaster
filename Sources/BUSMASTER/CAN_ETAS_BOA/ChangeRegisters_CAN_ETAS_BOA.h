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
 * @brief This header file contains the defination of class
 * @author    Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defination of class
 */

#pragma once

#include "Utility/RadixEdit.h"
#include "Math.h"
#include "BaseDIL_CAN_Controller.h"
#include "ChangeRegDefines.h"
#include "CAN_ETAS_BOA_Resource.h"
#include "DIL_Interface\IChangeRegisters.h"
#include "afxwin.h"
#define defMAXPropDelay 8

class CChangeRegisters_CAN_ETAS_BOA : public CDialog, public IChangeRegisters //CPropertyPage
{
    // Construction
public:
	int InvokeAdavancedSettings(PSCONTROLLER_DETAILS pControllerDetails, UINT nCount,UINT );
    DOUBLE vValidateBaudRate(DOUBLE dbaudrate,int,UINT );
public:
    // To Fill controller information taken from configuration module
    BOOL   bFillControllerConfig();
    // standard constructor
    CChangeRegisters_CAN_ETAS_BOA(CWnd* pParent = nullptr, PSCONTROLLER_DETAILS psControllerDetails = nullptr, UINT nHardwareCount = 0);
    virtual ~CChangeRegisters_CAN_ETAS_BOA();
    BOOL bSetBaudRateFromCom(int nChannel,BYTE bBTR0,BYTE bBTR1);
    BOOL bGetBaudRateFromCom(int nChannel,BYTE& bBTR0,BYTE& bBTR1);
    BOOL bSetFilterFromCom(BOOL  bExtended, DWORD  dBeginMsgId,
                           DWORD dEndMsgId);
    BOOL bGetFilterFromCom(BOOL& bExtended, double& dBeginMsgId, double& dEndMsgId);
    INT nGetInitStatus();

protected:
    // Dialog Data
    //{{AFX_DATA(CChangeRegisters_CAN_ETAS_BOA)
    enum { IDD = IDD_DLG_CHANGE_REGISTERS_CAN_ETAS_BOA };
    CListCtrl   m_omChannelList;
    CRadixEdit  m_omEditWarningLimit;
    CComboBox   m_omCombSampling;
    CListCtrl   m_omListCtrlBitTime;
    CRadixEdit   m_omEditSamplePoint;
    CRadixEdit   m_omEditBaudRate;
    CString m_omStrEditBTR0;
    CString m_omStrEditBTR1;
    CString m_omStrEditCNF1;
    CString m_omStrEditCNF2;
    CString m_omStrEditCNF3;
    CString m_omStrComboSampling;
    CString m_omStrEditBaudRate;
    CString m_omStrEditWarningLimit;
    // CString m_omStrSamplePoint;
    CString m_omStrSJW;
    //}}AFX_DATA
    CString m_omstrBTL_Cycles;
    DOUBLE  m_dEditBaudRate;
    UINT    m_unCombClock;
    /*CAN FD Parameters */
    CString m_omstrDataBitRate;
    //CString m_omstrDataSamplePoint;
    CString m_omstrDataBTL_Cycles;
    CString m_omstrDataSJW;
    CString m_omstrTxDelayCompensationON;
    CString m_omstrTxDelayCompensationQuanta;
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChangeRegisters_CAN_ETAS_BOA)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
private:
    // Generated message map functions
    //{{AFX_MSG(CChangeRegisters_CAN_ETAS_BOA)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnKillfocusEditBaudRate();
    afx_msg void OnSelchangeCombSampling();
    afx_msg void OnSetfocusEditBaudRate();
    afx_msg void OnClickedOK();
    afx_msg void OnSetfocusCombSampling();
    afx_msg void OnClickListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // Pointer to hold controller information
    SCONTROLLER_DETAILS  m_pControllerDetails[defNO_OF_CHANNELS];
	SCONTROLLER_DETAILS m_asDummyControllerDetails[defNO_OF_CHANNELS];
    PSCONTROLLER_DETAILS psMainContrDets;
    int m_nLastSelection;
    CImageList m_omChannelImageList;
    USHORT  m_usBTR0BTR1;
    UCHAR   m_ucWarningLimit;
    UCHAR   m_ucControllerMode;

    BOOL    m_bDialogCancel;
    int     m_nPropDelay;
    int     m_nSJWCurr;
    BYTE    m_bOption;
    UINT    m_nNoHardware;
    INT     m_nDataConfirmStatus;

    // To populate list box with possible vlaues for the given baudrate
    // and to set the focus to the selected item

    // To save user given values in to the backend data
    void vUpdateControllerDetails();
    // To set the backend data to UI
    void vFillControllerConfigDetails();
    BYTE bGetNBT(double fBaudRate);
    int nGetValueFromComboBox(CComboBox& omComboBox);
    CString omGetFormattedRegVal(UCHAR ucRegVal);
    int GetSelectedEntryIndex(void);
    BOOL bUpdateControllerDataMembers(void);

    void vEnableFDParameters(BOOL bEnable);
public:
    afx_msg void OnCbnSelchangeCombSjw();
    afx_msg void OnCbnSelchangeCombDelayCompensation();
    afx_msg void OnCbnSelchangeCombPropdelay();
    CComboBox m_omCtrlSamplePoint;
    CComboBox m_omCtrlSJW;
    BOOL m_bSelfReception;
};
