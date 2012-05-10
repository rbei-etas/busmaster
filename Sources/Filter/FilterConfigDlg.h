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
 * \file      FilterConfigDlg.h
 * \brief     Interface file for CFilterConfigDlg class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CFilterConfigDlg class
 */

#pragma once

#include "Utility/ComboItem.h"              // For Custom Combobox Implementation
#include "Utility/EditItem.h"               // For Custom Editbox Implementation
#include "Utility/RadixEdit.h"              // For the RAdix Edit control definition
#include "Utility/numSpinCtrl.h"            // For the custom spin control
#include "Utility/NumEdit.h"                // For Custom Numeric Edit control Impl
#include "Utility/FFListCtrl.h"             // For Flicker Free List Control
#include "Utility/Utility_Structs.h"        // SLISTINFO definitions
#include "FlexListCtrl.h"           // For Flexible List Control
#include "Datatypes/MsgSignal_Datatypes.h"

class CFilterConfigDlg : public CDialog
{
    // Construction
public:
    // Constructor with Filter List
    CFilterConfigDlg( SFILTERAPPLIED_CAN* psSrcList,
                      const SMSGENTRY* pMsgDBDetails, UINT nHardware,
                      CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CFilterConfigDlg)
    enum { IDD = IDD_DLG_CREATE_FILTER };
    CButton m_omAddFilter;
    CStatic     m_omStatusText;
    CRadixEdit  m_omMsgIDRangeTo;
    CRadixEdit  m_omMsgIDRangeFrom;
    CButton     m_omRadioID;
    CComboBox   m_omMsgType;
    CComboBox   m_omMsgIDType;
    CComboBox   m_omMsgDirection;
    CComboBox   m_omMsgChannel;
    CComboBox   m_omMsgIDFrom;
    CListCtrl   m_omLstcFilterDetails;
    CFlexListCtrl   m_omLstcFilterList;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFilterConfigDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CFilterConfigDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnItemchangedLstcFilterNames(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedLstcFilterDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangeCombMsgIdFrom();
    afx_msg void OnEditChangeMsgIDCombo();
    afx_msg void OnRadioMessageId();
    afx_msg void OnRadioRange();
    afx_msg void OnUpdateEditRange();
    afx_msg void OnSelchangeFilterComponentCombo();
    afx_msg void OnBtnAddFilterToList();
    afx_msg void OnBtnDeleteFilter();
    afx_msg void OnBtnDeleteAllFilter();
    afx_msg void OnBeginlabeleditLstcFilterNames(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBtnDelete();
    afx_msg void OnBtnAdd();
    afx_msg void OnOkPress();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    /*BYTE m_byJunk[100];               // Facing a bug in FlexListCntrl.cpp.
                                      // This memory will be corrupted  */
    BOOL m_bUpdating;                 // To indicate Programmed Update of lists
    BOOL m_bDisableFilterCompUpdate;  // To avoid validation of filter data
    CImageList m_omImagelist;         // Image List for Filters
    int m_nSelecetedNamedFilterIndex; // selected index of Filter Named
    CString m_omStrSelectedFilterNameBeforeEdit; // Backup text to restore back
    int m_nSelecetedFilterIndex;      // Selected index of filter
    const SMSGENTRY* m_psMsgSignal;         // Database information
    UINT m_nHardware;
public:
    SFILTERAPPLIED_CAN* m_psFilterApplied; // Reference to Filter List

private:
    // To create named filter UI List
    BOOL bCreateNamedFilterList();
    // To Create Filtet Details UI List
    BOOL bCreateFilterDetailsList();
    // To Create Filter UI Components
    BOOL bCreateFilterComps();
    // To initialise Filter UI Comps
    void vInitFilterComps();
    // To populate Message ID Combo with DB Names
    void vPopulateDBMessages();
    // To populate Filter Names UI from the filter list
    BOOL bPopulateNamedFilterList();
    // To update Filter details list control for the selected named filter
    void vUpdateFilterDetails( int nSelectedItemIndex );
    // To Update Filter details list control for the given filter
    void vUpdateFilterDetails( const PSFILTERSET psFilterSet );
    // To handle Filter Name update event
    void vUpdateFromFilterName(int, int);
    // To handle Filter Type update event
    void vUpdateFromFilterType(int, int);
    // To create Image List used in Filter and Filter Details List
    BOOL bCreateImageList();
    // To update Filter Componnents with filter details of selected
    void vUpdateFilterComponents(int nSelectedItem);
    // To update Filter Componnents with supplied filter details
    void vUpdateFilterComponents(const SFILTER_CAN& sFilter);
    // To Format Filter Details string from the filter details
    void vFormatDisplayString( const SFILTER_CAN& sFilter,
                               SFILTERDISPLAYINFO& sFilterDisplyInfo );
    // To Update Filter Details entry with supplied display information
    void vUpdateFilterListDetails( int nIndex,
                                   const SFILTERDISPLAYINFO& sFilterDisplyInfo );
    // To get Message ID From Message ID Combobox
    int nGetMsgIDFromCombo(const CComboBox& omCombo);
    // To Set text to the statusbar
    void vSetStatusText(CString omStrText);
    // To get Filter Details from the UI
    BOOL bGetFilterData(SFILTER_CAN& sFilter);
    // To Update Filter Details in Filter list (Data) with given filter
    BOOL bUpdateSelectedItem(SFILTER_CAN& sFilter);
    // To Add new Filter in to the filter list (Data)
    BOOL bAddNewItem(SFILTER_CAN& sFilter);
    // To update UI buttons with respect to list content and selection
    void vEnableDisableButtons();
    // To update filter components with given Enable/Disable Value
    void vEnableDisableFilterComps( BOOL bEnable );
    void vAdjustWidthMessageComboBox();

};
