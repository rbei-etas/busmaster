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
 * \file      FunctionView.h
 * \brief     Interface for CFunctionView class. Has functions to Set the
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface for CFunctionView class. Has functions to Set the
 */

#pragma once

class CFunctionView : public CRichEditView
{
    // Construction
public:
    CFunctionView();

public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFunctionView)
public:
    virtual void OnInitialUpdate();
    //}}AFX_VIRTUAL

public:
    static ETYPE_BUS sm_eBus;
    // Implementation
    //Sets the function in edit mode in the window
    //Function to be edited is passes in as omStrFunction
    void vSetFunctionToEdit(const CString& omStrFunction);
    // Initialise the member variable m_sStartPos to NULL;
    void vInitPositionStruct();
    //Destructor
    virtual ~CFunctionView();

protected:
    DECLARE_DYNCREATE(CFunctionView)

    // Generated message map functions
protected:
    //{{AFX_MSG(CFunctionView)
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnUpdatePaste(CCmdUI* pCmdUI);
    afx_msg void OnPaste();
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnEditUndo();
    afx_msg void OnInsertSignal();
    afx_msg void OnInsertMessage();
    afx_msg void OnInsertFunction();
    afx_msg void OnCut();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

private:
    //Updates the file view and sets the dirty flag
    void     UpdateFileViewAndSetModified();

    //Updates the edited function body in the document's source code list
    BOOL     UpdateFunctionInDocument();

    CString  m_omStrFnName;         //Function being edited

    CFont    m_omFont;              //Window font

    BOOL     m_bIsValidFunction;    //Flag indicating validity of function

    POSITION m_sStartPos;           //Start position (in the document's source
    //code list) of the function to edit.
    // Variables to remember starting line of procedure and current line in the
    // rich edit view. These will be used to select correct line in the FileView
    int m_nStartingLine, m_nCurrentLine;
    ETYPE_BUS m_eBus;
};
