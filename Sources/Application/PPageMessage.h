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
 * \file      PPageMessage.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once
#include"DataType.h"
#include"MsgSignal.h"
#include "resource.h"
class CPPageMessage : public CPropertyPage
{
    DECLARE_DYNCREATE(CPPageMessage)

protected:
    CPPageMessage();

    // Construction
public:
    CPPageMessage(BOOL bForDBMsg, ETYPE_BUS eBusType, void* pouMsgSigBUS);
    ~CPPageMessage();
    /*void vSetMsgIDList(); */

    // Dialog Data
    //{{AFX_DATA(CPPageMessage)
    enum { IDD = IDD_PPAGE_MESSAGE };
    CMessageList    m_odMsgList;
    CButton m_ctrlRemove;
    CButton m_ctrlAdd;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPPageMessage)
public:
    virtual void OnCancel();
    virtual void OnOK();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPPageMessage)
    virtual BOOL OnInitDialog();
    afx_msg void OnButtonAdd();
    afx_msg void OnButtonEdit();
    afx_msg void OnButtonRemove();
    afx_msg void OnDblclkListMessage(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedListMessage(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMCustomdrawListMessage(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    BOOL m_bForDBMsg;
    SCanIDList m_sNewItem;
    char m_acMsgEntry[128];
    COLORREF* m_pRGBColors;
    ETYPE_BUS m_eBusType;
	void *m_pouDb;
    CMsgSignal* m_pouMsgSigBus;
	ClusterConfig *m_ouClusterConfig;	//For FLEXRAY & LIN for CAN also in Future
    int nInitialiseMsgLCtrl(UINT unTotalDBMsgs, UINT* punDBMsgIDs);
    BOOL bIsMsgIDPresent(UINT*, UINT, UINT) const;
    int nEnterMessageAttrib(const SCanIDList& sMsgAttrib, int nItem);
};
