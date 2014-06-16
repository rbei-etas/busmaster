/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  HardwareListing.h
  Description   :  Interface file for CHardwareListing class
  $Log:   X:/Archive/Sources/ConfigDialogsDIL/HardwareListing.h_v  $
 *
 *    Rev 1.3   15 Apr 2011 18:47:18   CANMNTTM
 * Added RBEI Copyright information.
 *
 *    Rev 1.2   01 Feb 2011 19:52:38   CANMNTTM
 *
 *
 *    Rev 1.1   14 Dec 2010 19:28:44   CANMNTTM
 * Improvement: Hardware details are shown even when clicking selected list.
 *
 *    Rev 1.0   16 Aug 2010 18:51:24   rac2kor
 *

  Author        :  Raja N
  Date Created  :  07.09.2004
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *********************************************************************/
#if !defined(AFX_HARDWARELISTING_H__87D35685_F751_4B55_A2F6_347FFFA3B77A__INCLUDED_)
#define AFX_HARDWARELISTING_H__87D35685_F751_4B55_A2F6_347FFFA3B77A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HardwareListing.h : header file

// For resource symbols
#include "LIN_PEAK_USB_resource.h"
// For DIL datatypes
#include "DataTypes/DIL_Datatypes.h"
#include "Include/CanUsbDefs.h"
#include <map>
/////////////////////////////////////////////////////////////////////////////
// CHardwareListing dialog
typedef void (*fnCallBackBlink)(INTERFACE_HW);
class CHardwareListing : public CDialog
{
    fnCallBackBlink m_pfnBlinkFunction;
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
    CHardwareListing(INTERFACE_HW*, int , int*,CWnd* pParent = NULL, fnCallBackBlink m_pfnBlinkFunction= NULL);
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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HARDWARELISTING_H__87D35685_F751_4B55_A2F6_347FFFA3B77A__INCLUDED_)
