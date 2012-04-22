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
 * \file      TxMsgDetailsView.h
 * \brief     Interface file for CTxMsgDetailsView class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgDetailsView class
 */

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "TxWindow_resource.h"
#include "Utility/RadixEdit.h"          // For the RAdix Edit control definition
#include "FlexListCtrl.h"
#include "CMsgSignalTemp.h" 
#include "SignalMatrix.h"
#include "Utility/MsgInterpretation.h"  // For message interpretation
class CTxMsgDetailsView : public CFormView
{
// Form Data
public:
    //{{AFX_DATA(CTxMsgDetailsView)
    enum { IDD = IDD_DLG_TX_MSG_DETAILS };
    CComboBox   m_omComboChannelID;
    CSignalMatrix    m_odSignalMatrix;
    CFlexListCtrl   m_omLctrSigList;
    CComboBox   m_omComboMsgIDorName;
    CButton m_omButtonUpdateMsg;
    CRadixEdit  m_odDLC;
    CRadixEdit  m_odDB8;
    CRadixEdit  m_odDB7;
    CRadixEdit  m_odDB6;
    CRadixEdit  m_odDB5;
    CRadixEdit  m_odDB4;
    CRadixEdit  m_odDB3;
    CRadixEdit  m_odDB2;
    CRadixEdit  m_odDB1;
    BOOL    m_bIsRTR;
    CString m_omStrMsgIDorName;
    int     m_nRBTNFrameFormat;
    int     m_nChannelID;
    //}}AFX_DATA

// Operations
public:
    CMsgSignal *m_pouDBPtr;
	void vSetMsgDBPtr(void* pMsgDB);
    // To set given CAN values in to GUI
    void vSetValues(STXCANMSGDETAILS* psTxMsg);
    // To Disable signal values list control
    void vDisableSignalComponents();
    // To Enable/ Disable Add button after validating data
    void vEnableAddButton(BOOL bEnable);
    // To Update Signal list control with signals of the give CAN message
    BOOL bUpdateSignalList(STCAN_MSG sMsg);
    // To Set Status bar text
    BOOL bSetStatusText(const char *pStrText);
    // To populate Message Name combobox with database messages
    void vPopulateMessageComboBox();
    // To set properties of controls used
    void vSetControlProperties();	
	void vUpdateChannelIDInfo();


protected:
    // protected constructor used by dynamic creation
    CTxMsgDetailsView();
    DECLARE_DYNCREATE(CTxMsgDetailsView)

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTxMsgDetailsView)
    public:
    virtual void OnInitialUpdate();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CTxMsgDetailsView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CTxMsgDetailsView)
    afx_msg void OnEditchangeCombMsgIdName();
    afx_msg void OnSelchangeCombMsgIdName();
    afx_msg void OnUpdateEditDLC();
    afx_msg void OnUpdateEditDataBytes();
    afx_msg void OnRbtnMsgtypeStd();
    afx_msg void OnChkbMsgtypeRtr();
    afx_msg void OnButtonAddMsg();
    afx_msg void OnItemchangedLstcSigDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangeCombChannelId();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    // Image List for Signal List
    CImageList m_omSigImageList;
    // Pointer to store currently selected message detail
    sMESSAGE * m_psSelectedMsgDetails;
    // Temp data Array
    UNION_BYTE m_unData[ defMAX_BYTE ];
    // Bit Array
    UINT m_bData [ defMAX_BYTE * defBITS_IN_BYTE ];
    // Local variable to denote text update by prog not by user
    BOOL m_bUpdating;
    // Dirty Flag
    BOOL m_bIsMsgDirty;


    // Methods
    // To initialise signal list control
    void vInitSignalListCtrl();
    // To set up signal name double click handler procedure
    static void vSignalNameHandler( CListCtrl *pList,
                                    int nItem, int nSubItem,
                                    void * uParam );
    //To get signal details of selected signal in the signal list
    sSIGNALS * psGetSelectedSignalStruct(int nIndex);
    // To update message details from the user entered values
    BOOL bUpdateMessageDetail(STCAN_MSG* psMsgDetails);
    // To get Message ID from Message ID/ Name combobox
    INT nGetMessageID();
    // TO update selected message details with user entered values
    BOOL vUpdateSelectedMessageDetails();
    // To show signal values got from interpretation module
    void vShowSignalValues(const CSignalInfoArray &romSigInfo);
    // To validate user entered message ID
    int nGetValidMessageID();
    // Get get parent window pointer
    CWnd * pomGetParentWindow() const;
    // To get Tx Blocks View Pointer
    CWnd * pomGetBlocksViewPointer() const;
    // To get List View Pointer
    CWnd * pomGetListViewPointer() const;
    // To get Functions View Poniter
    CWnd * pomGetFunctionsViewPointer() const;
    // To validate user entered CAN message data
    BOOL bValidateData();
    // To check data bytes boundary condition
    BOOL bCheckIfValueIsMoreThan255(USHORT usNoOfEditCtrlsToCheck);
    // To set the dirty bit of the selected message
    void SetDirty(BOOL bDirty = TRUE);
    // To add a message in to the selected message block
    BOOL bAddMsgInBlock();
    // To highlight selected signal postion in signal matrix
    void vUpdateSignalMatrix(int nSelectedIndex);
    // To validate physical value change
    void vUpdateFromPhysicalValue(int nItem, int nSubItem);
    // To validate raw value change
    void vUpdateFromRawValue(int nItem, int nSubItem);
    // To update CAN message data bytes
    void vUpdateDataBytes();
    // To pack signal value in to the CAN message data bytes 
    void vUpdateSignalData(sSIGNALS * psSignal,
                           __int64 nI64SignVal );
    // To set default values for Message ID, DLC, databytes and Channel ID
    void vSetDefaultValues();
    void vUpdateSignalMatrix(void);
};
