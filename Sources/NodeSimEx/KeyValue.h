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
 * \file      KeyValue.h
 * \brief     This file contain the definition all of CKeyValue class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all of CKeyValue class
 */

#pragma once

#include "NodeSimEx_resource.h"

class CKeyValue : public CDialog
{
private:
    CFnsTreeView* m_pFnsTreeView;
    CFunctionEditorDoc* m_pDoc;
// Construction
public:
    // standard constructor
    CKeyValue(CFnsTreeView* pFnsTreeView, CFunctionEditorDoc* pDoc,
              CWnd* pParent = NULL,TCHAR* pcKeyVal = (TCHAR*)NULL);

// Dialog Data
    //{{AFX_DATA(CKeyValue)
    enum { IDD = IDD_DLG_KEY_VALUE };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CKeyValue)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
// Implementation
protected:
   
    // Generated message map functions
    //{{AFX_MSG(CKeyValue)
    virtual void OnCancel();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    virtual void OnOK();
    virtual BOOL OnInitDialog();
	afx_msg void OnCbtnKeyApply();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    TCHAR* m_pcKeyVal;
    BOOL bValidateDuplicateKeyHandler(CFunctionEditorDoc* pDoc);

};
