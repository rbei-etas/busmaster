/**
 * \file      HardwareListing.h
 * \brief     Interface file for CHardwareListing class
 * \author    a1@uincore.net
 * \copyright Copyright (c) 2015, Robert Bosch Engineering and Business Solutions. All rights reserved.

 * Interface file for CHardwareListing class
 */

#pragma once

// For resource symbols
#include "LIN_Kvaser_Resource.h"
// For DIL datatypes
#include "DataTypes/DIL_Datatypes.h"
#include "Include/CanUsbDefs.h"
#include <map>
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
    afx_msg void OnBlinkHw();
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

};
