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
 * \file      HardwareListing.h
 * \brief     Interface file for CHardwareListing class
 * \author    RaTnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CHardwareListing class
 */

#pragma once

// For resource symbols
// For DIL datatypes
#include <afxwin.h>         // MFC core and standard components
#include <afxcmn.h>
//#include "DataTypes/DIL_Datatypes.h"
//#include "Include/CanUsbDefs.h"
#include "CANDriverDefines.h"
#include "ProtocolsDefinitions.h"
#include <map>
#include "..\DIL_Interface\ChannelSelectionResource.h"
#include "resource.h"
#include "Utility/RadixEdit.h"
#include "DIL_Interface\IChangeRegisters.h"

class CHardwareListingCAN : public CDialog
{

public:
    // Array of channels
    int m_anSelectedChannels[ CHANNEL_ALLOWED ];
    // Array to hold driver handle
    //SHWNETLIST m_sHwNetList[ MAX_HCANNET ];
    // To update selected hardware detaisl
    void vUpdateHwDetails( int nIndex );
    // To set List of hardware handles
    void vSetHardwareList(INTERFACE_HW*, int );
    // Constructor
	CHardwareListingCAN(INTERFACE_HW*, int, int*, ETYPE_BUS eBusType, int nMaxChannels, CWnd* pParent = nullptr, PSCONTROLLER_DETAILS InitData = nullptr, IChangeRegisters* pAdvancedSettings = nullptr);
    // standard constructor
	CHardwareListingCAN();
    //Get selection list
    INT nGetSelectedList(int* pnList);
    //Set selection list
    void vSetSelectedList();

    // Dialog Data
    //{{AFX_DATA(CHardwareListing)
    enum { IDD = IDD_DLG_HW_LISTING_CAN };
    CListCtrl   m_omSelectedHwList;
    CEdit   m_omNetName;
    CEdit   m_omFirmware;
    CEdit   m_omDriverID;
    CListCtrl   m_omHardwareList;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHardwareListing)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CHardwareListing)
    virtual BOOL OnInitDialog();
    afx_msg void OnItemchangedHWList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonSelect();
	void vMoveItemToSelectedList();
    afx_msg void OnButtonRemove();
	void vMoveItemFromSelectedList();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnItemchangedLstcSelectedHwList(NMHDR* pNMHDR, LRESULT* pResult);
	void InitializeControllerDetails(int nItem);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void vEnableDisableButtons();
	void vUpdateControllerDetails();
	void vFillControllerConfigDetails();
	void    vValidateBaudRate();
	void vCalculateConfigParams();
    // Pointer to Hardware List
    INTERFACE_HW* m_psHwInterface;

	bool m_bOnEnterBaudRate;
    // Size of the array
    int m_nSize;
	CWnd* m_pParent;
	int m_nLastSelection;
    //Number of selected items
    int m_nNoOfHwSelected;
	 PSCONTROLLER_DETAILS  m_pControllerDetails;
	 SCONTROLLER_DETAILS  m_asDummyControllerDetails[CHANNEL_ALLOWED];
    // Image for CListCtrl
    CImageList m_omImageList;
    // Selected Item index
    int m_nSelectedItem;
    // Pointer to Selected List
    int* m_pnSelList;

	std::map<std::string, bool> m_mapChannelToCANFDStatus;
	std::string m_omChannelDesc[defNO_OF_CHANNELS];
	bool m_pnCANDFDValue[defNO_OF_CHANNELS];
    ETYPE_BUS m_eBus;
    int m_nMaxChannelAllowed;
public:
    afx_msg void OnNMClickLstcHwList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMClickLstcSelectedHwList(NMHDR* pNMHDR, LRESULT* pResult);
	DOUBLE  m_dEditBaudRate;
	CString m_omStrBTR0Val;
	CString m_omStrBTR1Val;

public:
    // Hardware CONTAINER
    typedef struct tagHardwareContainer
    {
        int                 m_omDriverId;
        CString             m_omHardwareName;
        CString             m_omFirmware;
        CString             m_omHardwareDesc;

    } HARDWARE_CONTAINER, *PHARDWARE_CONTAINER;

    typedef std::pair <int, PHARDWARE_CONTAINER> Int_Pair;
    std::map <int, PHARDWARE_CONTAINER>::iterator m_pIter;
    std::map <int, PHARDWARE_CONTAINER> mHardwareListMap;

    //Hardware container object
    PHARDWARE_CONTAINER m_pouHardwareContainer;
    void vSortHardwareItems();
private:
    std::string getProtocolChannelNameFormat( int );	
	void vEnableBTRParams(BOOL bEnable);
	
public:
	CRadixEdit  m_omEditBaudRate;
	CString m_omStrEditBaudRate;
	CRadixEdit   m_omEditBTR1;
	CRadixEdit   m_omEditBTR0;
	CString m_omStrEditBTR0;
	CString m_omStrEditBTR1;
	CEdit m_omCANFDEditBaudRate;
	CString m_omCANFDStrEditBaudRate;
	BOOL m_bDialogCancel;
	CButton m_chkCANFD;
	BOOL m_bCANFD;
	IChangeRegisters* m_pAdvChnlConfig;
	afx_msg void OnEnKillfocusCanEditBaudRate();
	void OnKillFocusOfEditBox(CEdit* , CString, bool );
	void ValidationOfKillFocus( CEdit* omEditBaudRate, CString omStrEditBaudRate, bool bchkCANFD, INT nMaxBitRateAllowed );
	afx_msg void OnBnClickedButtonAdvance();
	afx_msg int nFillChannelDetails();
	afx_msg void OnHdnItemdblclickLstcHwList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLstcHwList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLstcSelectedHwList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckCanfd();
	afx_msg void OnEnKillfocusCanfdEditBaudRate();
};
