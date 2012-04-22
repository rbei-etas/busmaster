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
 * \file      ChangeRegisters.h
 * \brief     This header file contains the defination of class
 * \authors   Amitesh Bharti, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defination of class
 */

#pragma once

// CChangeRegisters dialog
#include "CAN_Kvaser_CAN_resource.h"
#include "Utility/RadixEdit.h"
#include "ChangeRegDefines.h"
#include "Include/CanUsbDefs.h"


class CChangeRegisters : public CDialog//CPropertyPage
{
    //DECLARE_DYNCREATE(CChangeRegisters)
// Construction
public:
    // To Fill controller information taken from configuration module
    BOOL   bFillControllerConfig();
   // standard constructor
    CChangeRegisters(CWnd* pParent = NULL, PSCONTROLER_DETAILS psControllerDetails = NULL, UINT nHardwareCount = 0);
    virtual ~CChangeRegisters();
	BOOL bSetBaudRateFromCom(int nChannel,BYTE bBTR0,BYTE bBTR1);
	BOOL bGetBaudRateFromCom(int nChannel,BYTE &bBTR0,BYTE &bBTR1);
	BOOL bSetFilterFromCom(BOOL  bExtended, DWORD  dBeginMsgId, 
								   DWORD dEndMsgId);
	BOOL bGetFilterFromCom(BOOL  &bExtended, double  &dBeginMsgId, 
								   double &dEndMsgId);
    INT nGetInitStatus();

protected:
// Dialog Data
    //{{AFX_DATA(CChangeRegisters)
    enum { IDD = IDD_DLG_CHANGE_REGISTERS };
    CListCtrl   m_omChannelList;
    CRadixEdit  m_omEditWarningLimit;
    CComboBox   m_omCombSampling;
    CComboBox   m_omCombClock;
    CRadixEdit   m_omEditBRP;
    CListCtrl   m_omListCtrlBitTime;
    CRadixEdit   m_omEditBaudRate;
    CRadixEdit   m_omEditBTR1;
    CRadixEdit   m_omEditBTR0;
    BYTE    m_byEditBRP;
    CString m_omStrEditBTR0;
    CString m_omStrEditBTR1;
    CString m_omStrComboSampling;
    CString m_omStrEditBaudRate;
    CString m_omStrComboClock;
    CString m_omStrEditWarningLimit;
    //}}AFX_DATA
    DOUBLE  m_dEditBaudRate;
    UINT    m_unCombClock;
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChangeRegisters)    
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
private:

    // Defination of BTR0 Register
    struct sBTR0
    {
     UCHAR  ucBRPbit   : 6;    // 0..5   (6 bits)
     UCHAR  ucSJWbit   : 2;    // 6..7   (2 bits)
    };

    // Packing of Struct BTR0 in one Byte
    union uBTR0
    {
     UCHAR ucBTR0;
     sBTR0 sBTR0Bit;
    };

    // Defination of BTR1 Register
    struct sBTR1
    {
     UCHAR ucTSEG1bit  : 4;    // 0..3   (4 bits)
     UCHAR ucTSEG2bit  : 3;    // 4..7   (3 bits)
     UCHAR ucSAMbit    : 1;    // 8      (1 bit )
    };

    // Packing of Struct BTR1 in one Byte
    union  uBTR1
    {
     UCHAR ucBTR1;
     sBTR1 sBTR1Bit;
    } ;

    // For Values of BRP, NBT , Sampling Percentage and SJW
    struct sBRP_NBT_SAMP_n_SJW
    {
     USHORT usBRP;
     USHORT usNBT;
     USHORT usSampling;
     USHORT usSJW;
    } ;
    // structure defination of all five columns in the list control    
    struct sCOLUMNS
    {
     uBTR1                uBTRReg1;
     uBTR0                uBTRReg0;
     sBRP_NBT_SAMP_n_SJW  sBRPNBTSampNSJW ;
    } m_asColListCtrl[defREG_VALUE_LIST_COUNT_MAX];

    // Generated message map functions
    //{{AFX_MSG(CChangeRegisters)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnKillfocusEditBaudRate();
    afx_msg void OnKillfocusEditBTR0();
    afx_msg void OnKillfocusEditBTR1();
    afx_msg void OnSelchangeCombSampling();
    afx_msg void OnSetfocusEditBaudRate();
    afx_msg void OnSetfocusEditBTR0();
    afx_msg void OnSetfocusEditBTR1();
    afx_msg void OnClickedOK();
    afx_msg void OnSelchangeCombClock();
    afx_msg void OnSetfocusCombClock();
    afx_msg void OnSetfocusCombSampling();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void OnCbtnAcceptance();
    afx_msg void OnItemchangedLstcBtrList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkListChannels(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnCbtnBlink();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // Pointer to hold controller information
    PSCONTROLER_DETAILS  m_pControllerDetails;
    int m_nLastSelection;
    CImageList m_omChannelImageList;    
    USHORT   m_usBTR0BTR1;
    UCHAR    m_ucWarningLimit;
    UCHAR    m_ucControllerMode;
    UINT     m_unHardwareCount;
    INT      m_nDataConfirmStatus;
    BOOL    m_bDialogCancel;
    // To populate list box with possible vlaues for the given baudrate
    // and to set the focus to the selected item
    void    vDisplayListBox(INT nIndex,INT nItemFocus  );
    void    vChangeListBoxValues(INT nflag) ;
    void    vUpdateBTRsBRPEditWindow(INT nColumnCount, INT nItem);
    void    vCalculateBaudRateNBTR0(CString omStrBtr1);
    void    vCalculateBaudRateNBTR1(CString omStrBtr0);
    void    vSelSetFocusItemList(INT nItemCount,INT nItem);
    void    vValidateBaudRate();
    DOUBLE  dCalculateBaudRateFromBTRs(CString omStrBTR0, CString omStrBTR1);
    BOOL nListBoxValues(struct sCOLUMNS *psColListCtrl,
                                  DOUBLE dBuadRate,WORD usClockFreq,
                                  UINT *puwIndex,INT nSample);
    BOOL nCalculateRegValues (struct sCOLUMNS *psColListCtrl,
                                            WORD wNbt,WORD wBrp,UINT *puwIndex,
                                            INT nSample);
    // To save user given values in to the backend data
    void vUpdateControllerDetails();
    // To set the backend data to UI
    void vFillControllerConfigDetails();
};
