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

/* include afxwin.h to avoid windows.h inclusion error */
#include <afxwin.h>

#include <afxcmn.h>

// For resource symbols
//#include "FLEXRAY_ETAS_BOA_Resource.h"
// For DIL datatypes
#include "../DataTypes/DIL_Datatypes.h"
#include "../Include/CanUsbDefs.h"
#include <map>

////////////////////// from "FLEXRAY_ETAS_BOA_Resource.h"
#define IDC_BUT_REMOVE                  3
#define IDC_BUT_SELECT                  5
#define IDD_DIALOG1                     101
#define IDD_DLG_HW_LISTING              130
#define IDD_DLG_ACCEPTANCE              197
#define IDR_BMP_NET                     204
#define IDD_DLG_CHANGE_REGISTERS        228
#define IDC_EDIT_BAUD_RATE              1000
#define IDC_EDIT_BTR0                   1001
#define IDC_EDIT_BTR1                   1002
#define IDC_STAT_ACCEPTANCE             1003
#define IDC_EDIT_NET_NAME               1007
#define IDC_EDIT_FIRMWARE               1008
#define IDC_EDIT_NET_NAME2              1009
#define IDC_EDIT_NET_ADDITIONALINFO     1009
#define IDC_LSTC_BTR_LIST               1012
#define IDC_STAT_BAUD_RATE              1013
#define IDC_STAT_KBPS                   1014
#define IDC_STAT_BUAD_RATE              1015
#define IDC_STAT_BTR0                   1016
#define IDC_STAT_BTR1                   1017
#define IDC_LSTC_HW_LIST                1018
#define IDC_EDIT_BRP                    1018
#define IDC_EDIT_DRIVER_ID              1019
#define IDC_COMB_SAMPLING               1019
#define IDC_BUT_BLINK                   1020
#define IDC_COMB_CLOCK                  1020
#define IDC_STAT_CLOCK                  1022
#define IDC_ButtonOK                    1060
#define IDC_STAT_LINE3                  1119
#define IDC_EDIT_WARNING_LIMIT          1175
#define IDC_EDIT_ACCEPTANCE_CODE2       1181
#define IDC_CBTN_ACCEPTANCE             1199
#define IDC_CBTN_BLINK                  1200
#define IDC_CBTN_ACCETANCE_OK           1200
#define IDC_EDIT_ACCEPTANCE_MASK2       1201
#define IDC_EDIT_ACCEPTANCE_CODE3       1202
#define IDC_EDIT_ACCEPTANCE_MASK3       1203
#define IDC_EDIT_ACCEPTANCE_MASK1       1204
#define IDC_EDIT_ACCEPTANCE_MASK4       1205
#define IDC_EDIT_ACCEPTANCE_CODE1       1206
#define IDC_EDIT_ACCEPTANCE_CODE4       1207
#define IDC_RBTN_SINGLE_FILTER_MODE     1208
#define IDC_RBTN_DUAL_FILTER_MODE       1209
#define IDC_STAT_FILTER_MODE            1210
#define IDC_STAT_ACCEPTANCE_FILTER_CODE 1211
#define IDC_STAT_ACCEPTANCE_FILTER_MASK 1212
#define IDC_RBTN_FILTER_ACCEPT_ALL      1213
#define IDC_RBTN_FILTER_REJECT_ALL      1214
#define IDC_RBTN_FILTER_MANUAL_SET      1215
#define IDC_STAT_FILTER_TYPES           1216
#define IDC_EDIT_ACC_CODE               1217
#define IDC_EDIT_ACC_MASK               1219
#define IDC_LSTC_SELECTED_HW_LIST       1226
#define IDC_LIST_CHANNELS               1246
#define IDD_LIST_HW_INTRS               3002
#define IDC_LIST1                       3004
#define IDC_NAME                        3005
#define IDC_VENDOR                      3006
#define IDC_DESC                        3007
#define IDR_PGM_EDTYPE                  3008
#define IDC_EDIT_CHANNEL_DESC           3012
#define IDC_CHKB_SELF_RECEPTION         3013




/////////////////////////////////////////////////////////////////////////////
// CHardwareListing dialog
class CHardwareListing : public CDialog
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
    CHardwareListing(INTERFACE_HW*, int , int*, CWnd* pParent = nullptr);
    // standard constructor
    CHardwareListing();
    //Get selection list
    INT nGetSelectedList(int* pnList);
    //Set selection list
    void vSetSelectedList();

    // Dialog Data
    //{{AFX_DATA(CHardwareListing)
    enum { IDD = IDD_DLG_HW_LISTING };
    CListCtrl   m_omSelectedHwList;
    CEdit   m_omNetName;
    CEdit   m_omFirmware;
    CEdit   m_omDriverID;
    CListCtrl   m_omHardwareList;
    CString m_omAdditionalInfo;
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
    afx_msg void OnButtonRemove();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnItemchangedLstcSelectedHwList(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void vEnableDisableButtons();
    // Pointer to Hardware List
    INTERFACE_HW* m_psHwInterface;
    // Size of the array
    int m_nSize;
    //Number of selected items
    int m_nNoOfHwSelected;
    // Image for CListCtrl
    CImageList m_omImageList;
    // Selected Item index
    int m_nSelectedItem;
    // Pointer to Selected List
    int* m_pnSelList;
public:
    afx_msg void OnNMClickLstcHwList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMClickLstcSelectedHwList(NMHDR* pNMHDR, LRESULT* pResult);

public:
    // Hardware CONTAINER
    typedef struct tagHardwareContainer
    {
        int     m_omDriverId;
        CString     m_omHardwareName;
        CString     m_omFirmware;
        CString     m_omHardwareDesc;

    } HARDWARE_CONTAINER, *PHARDWARE_CONTAINER;

    typedef std::pair <int, PHARDWARE_CONTAINER> Int_Pair;
    std::map <int, PHARDWARE_CONTAINER>::iterator m_pIter;
    std::map <int, PHARDWARE_CONTAINER> mHardwareListMap;

    //Hardware container object
    PHARDWARE_CONTAINER m_pouHardwareContainer;
    void vSortHardwareItems();

};
