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
 * \file      EventHandlerDlg.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

// CEventHandlerDlg dialog

class CEventHandlerDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventHandlerDlg)

public:
    CEventHandlerDlg(CTreeCtrl& omTreeCtrl, CFunctionEditorDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEventHandlerDlg();

// Dialog Data
	enum { IDD = IDD_EVENT_HANDLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
private:
    CFunctionEditorDoc* m_pDoc;
    CTreeCtrl& m_omTreeCtrl;

    void vAddAdresClaimHandler(HTREEITEM hParent);
    BOOL bIsEventHandlerExist(const CString& omStrFuncType);
    void vAddDataConfHandler(HTREEITEM hParent);
    void vAddEventHandlerFunction(const CString& omStrFuncName, const CString& omStrFuncPrototype);
};
