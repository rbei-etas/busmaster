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
#if !defined(AFX_HARDWARELISTING_H__87D35685_F751_4B55_A2F6_347FFFA3B77A__INCLUDED_)
#define AFX_HARDWARELISTING_H__87D35685_F751_4B55_A2F6_347FFFA3B77A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HardwareListing.h : header file

// For resource symbols
#include "CAN_Vector_XL_Resource.h"
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
    void vSetHardwareList(INTERFACE_HW *, int );
    // Constructor
    CHardwareListing(INTERFACE_HW *, int , int*, CWnd* pParent = NULL);
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
    INTERFACE_HW * m_psHwInterface;
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
    afx_msg void OnNMClickLstcHwList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickLstcSelectedHwList(NMHDR *pNMHDR, LRESULT *pResult);

public:
	// Hardware CONTAINER
	typedef struct tagHardwareContainer
	{
		int		m_omDriverId;
		CString		m_omHardwareName;
		CString		m_omFirmware;
		CString		m_omHardwareDesc;
	
	}HARDWARE_CONTAINER, *PHARDWARE_CONTAINER;

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
