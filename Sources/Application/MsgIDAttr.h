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
 * \file      MsgIDAttr.h
 * \brief     This file contains the definition of CMsgIDAttr class     
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains the definition of CMsgIDAttr class     
 */

#pragma once

class CRadixEdit;   // forward declaration

class CMsgIDAttr : public CDialog
{
// Construction
public:
    bool m_bForEdit;
    BOOL m_bDBMessage;
    UINT m_nID;
    COLORREF m_sColour;
    CMsgIDAttr(ETYPE_BUS eBusType, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CMsgIDAttr)
    enum { IDD = IDD_DLG_MSGID_ATTRIB };
    CStatic m_omColourBox;
    CRadixEdit  m_odCanIDVal;
    CString m_omStrMsg;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMsgIDAttr)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CMsgIDAttr)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnPbtnColour();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    HBRUSH m_hClrBtn;
	ETYPE_BUS m_eBusType;
    // To draw a solid box with a specified colour
    //void vDrawColourBox();
};
