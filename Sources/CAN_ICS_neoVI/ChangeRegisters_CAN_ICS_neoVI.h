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
 * \file      ChangeRegisters_CAN_ICS_neoVI.h
 * \brief     This header file contains the defination of class       
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defination of class       
 */

#pragma once

// CChangeRegisters_CAN_ICS_neoVI dialog
#include "Utility/RadixEdit.h"
#include "Math.h"
#include "Include/Struct_Can.h"
#include "Include/CanUsbDefs.h"
#include "ChangeRegDefines.h"

#define defICSneoVIFrequency (32 * 1000000)
#define defPropDelayICSneoVI 1
#define defMAXPropDelay 8

enum {NO_DEF = 0, TS1_TS2 = 1, PD_TS1_TS2, SJW_TS1_TS2};

    struct sCNF1
    {
        UCHAR   ucBRPbit   : 6;    // 0..5   (6 bits)
        UCHAR   ucSJWbit   : 2;    // 6..7   (2 bits)
    };

    // Packing of Struct BTR0 in one Byte
    union uCNF1
    {
        UCHAR   ucCNF1;
        sCNF1   sCNF1Bit;
    };

    struct sCNF2
    {
        UCHAR  ucPropDelaybit : 3;// 0..2 (3 bits)
        UCHAR  ucTSEG1bit : 3;    // 3...5  (3 bits)
        UCHAR  ucSAMbit   : 1;    // 6   (1 bit)
        UCHAR  ucFLAGbit  : 1;    // 7 (1 bit)
    };

    // Packing of Struct BTR0 in one Byte
    union uCNF2
    {
        UCHAR   ucCNF2;
        sCNF2   sCNF2Bit;
    };

    struct sCNF3
    {
        UCHAR   ucTSEG2bit : 3;    // 3...5  (3 bits)
    };
    // Packing of Struct BTR0 in one Byte
    union uCNF3
    {
        UCHAR   ucCNF3;
        sCNF3   sCNF3Bit;
    };

    typedef struct tagSBRP_NBT_SAMP_n_SJW
    {
        USHORT  usBRP;
        USHORT  usNBT;
        USHORT  usSampling;
        USHORT  usPropDelay;
        USHORT  usSJW;
        USHORT  usSample;
    } sBRP_NBT_SAMP_n_SJW;

    typedef struct tagSCOLUMNS
    {
        uCNF1   uCNFReg1;
        uCNF2   uCNFReg2;
        uCNF3   uCNFReg3;
        sBRP_NBT_SAMP_n_SJW  sBRPNBTSampNSJW;
    } sCOLUMNS;

class CChangeRegisters_CAN_ICS_neoVI : public CDialog
{
// Construction
public:
    // To Fill controller information taken from configuration module
    BOOL   bFillControllerConfig();
    // standard constructor
    CChangeRegisters_CAN_ICS_neoVI(CWnd* pParent = NULL, PSCONTROLLER_DETAILS psControllerDetails = NULL, UINT nHardwareCount = 0);
    virtual ~CChangeRegisters_CAN_ICS_neoVI();
    BOOL bSetBaudRateFromCom(int nChannel,BYTE bBTR0,BYTE bBTR1);
    BOOL bGetBaudRateFromCom(int nChannel,BYTE &bBTR0,BYTE &bBTR1);
    BOOL bSetFilterFromCom(BOOL  bExtended, DWORD  dBeginMsgId, 
                                   DWORD dEndMsgId);
    BOOL bGetFilterFromCom(BOOL& bExtended, double& dBeginMsgId, double& dEndMsgId);
    INT nGetInitStatus();

protected:
// Dialog Data
    //{{AFX_DATA(CChangeRegisters_CAN_ICS_neoVI)
    enum { IDD = IDD_DLG_CHANGE_REGISTERS_CAN_ICS_NEOVI };
    CListCtrl   m_omChannelList;
    CRadixEdit  m_omEditWarningLimit;
    CComboBox   m_omCombSampling;
    CListCtrl   m_omListCtrlBitTime;
    CRadixEdit   m_omEditBaudRate;
    CString m_omStrEditBTR0;
    CString m_omStrEditBTR1;
    CString m_omStrEditCNF1;
    CString m_omStrEditCNF2;
    CString m_omStrEditCNF3;
    CString m_omStrComboSampling;
    CString m_omStrEditBaudRate;
    CString m_omStrComboClock;
    CString m_omStrEditWarningLimit;
    CString m_omStrPropDelay;
    CString m_omStrSJW;
    //}}AFX_DATA
    DOUBLE  m_dEditBaudRate;
    UINT    m_unCombClock;
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChangeRegisters_CAN_ICS_neoVI)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
private:
    sCOLUMNS m_asColListCtrl[defREG_VALUE_LIST_COUNT_MAX];

    // Generated message map functions
    //{{AFX_MSG(CChangeRegisters_CAN_ICS_neoVI)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnKillfocusEditBaudRate();
    afx_msg void OnSelchangeCombSampling();
    afx_msg void OnSetfocusEditBaudRate();
    afx_msg void OnClickedOK();
    afx_msg void OnSetfocusCombSampling();
    afx_msg void OnItemchangedLstcBtrList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // Pointer to hold controller information
    PSCONTROLLER_DETAILS  m_pControllerDetails;
    int m_nLastSelection;
    CImageList m_omChannelImageList;
    SACC_FILTER_INFO m_sAccFilterInfo;
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
    void    vDisplayListBox(INT nIndex,INT nItemFocus);
    void    vChangeListBoxValues(INT nflag);
    void    vSelSetFocusItemList(INT nItemCount,INT nItem);
    void    vValidateBaudRate();
    DOUBLE  dCalculateBaudRateFromBTRs(CString omStrCNF1, CString omStrCNF2,
                                       CString omStrCNF3);
    BOOL nListBoxValues(sCOLUMNS psColListCtrl[], DOUBLE dBuadRate,
                        WORD usClockFreq, UINT *puwIndex, INT nSample);
    // To save user given values in to the backend data
    void vUpdateControllerDetails();
    // To set the backend data to UI
    void vFillControllerConfigDetails();
    bool bCalculateICSneoVIRegValues(WORD wNbt, WORD wBrp, UINT *puwIndex, INT nSample);
    bool bCalculateICSneoVIParams(sBRP_NBT_SAMP_n_SJW& CurEntry, 
                               UINT& unCurrIndex, BYTE bOption);
    BYTE bGetNBT(double fBaudRate);
    int nGetValueFromComboBox(CComboBox& omComboBox);
    CString omGetFormattedRegVal(UCHAR ucRegVal);
    void bDecideCalculatingOption(void);
    BOOL GetRegisterValues(BYTE& bCNF1, BYTE& bCNF2, BYTE& bCNF3);
    int GetSelectedEntryIndex(void);
    BOOL bUpdateControllerDataMembers(void);

public:
    afx_msg void OnHdnItemclickLstcBtrList(NMHDR *pNMHDR, LRESULT *pResult);
    //int m_nCurrSJW;
    afx_msg void OnCbnSelchangeCombSjw();
    afx_msg void OnCbnSelchangeCombPropdelay();
    CComboBox m_omCtrlPropDelay;
    CComboBox m_omCtrlSJW;
    CComboBox m_omCtrlClock;
};
