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
 * @brief This header file contains the defintion of class
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include <string>
#include "FunctionEditorDoc.h"
class CGlobalObj;

class CFnsTreeView : public CTreeView
{
protected:
    //CFnsTreeView(){}
    // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CFnsTreeView)

    // Operations
public:
    CFnsTreeView(CGlobalObj* pGlobalObj = nullptr);
    static ETYPE_BUS sm_eBus;
    void vSetItemName( CString );

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFnsTreeView)
public:
    virtual void OnInitialUpdate();
    virtual void vActivateFileView(UINT);
    virtual ~CFnsTreeView();

protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    //}}AFX_VIRTUAL

    // Implementation
protected:

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:
    //{{AFX_MSG(CFnsTreeView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //afx_msg void OnTreeItemDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTreeItemSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemLeftClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTreeViewRightclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnDeleteHandle();
    afx_msg void OnAddHandle();
    afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnEditFunctionHeader();

    virtual void vAddBusSpecificHandle(CString );
    virtual bool bEnablePreconnectHandler();
    virtual bool bEnableBusSpecificHandler(CString omstrTreeItemName);

    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    //  Add new message handler
    virtual void vOnNewMessageHandler();
    void vOnNewHandler(CString omStrSelectedText);
private:
    ETYPE_BUS m_eBus;
    // Add new header
    void vOnNewIncludeHeader();
    // Edit selceted header
    void vEditIncludeHeader(HTREEITEM hItem);
    // Edit selceted utility function
    void vEditUtilityFunc(HTREEITEM hItem);
    // Edit selceted utility function
    void vEditTimerHandler(HTREEITEM hItem);
    // Edit the selected function
    void vEditSelectedFunction();
    // Add new global variable
    void vOnNewGlobalVariable();
    // New utility function
    void vOnNewUtilityFunction();
    // Add new key handler
    void vOnNewKeyHandler();
    // Add new POC handler
    //void vOnNewPOCHandler();
    // Add new Cycle handler
    //void vOnNewStartCycleHandler();
    // Add new PDU handler
    //void vOnNewPDUHandler();
    // Add new timer handler
    void vOnNewTimerHandler();

    //  Add new error handler
    void vOnNewErrorHandler();
    //  Add new event handler
    void vOnNewEventHandler();
    //  Add new DLL handler
    void vOnNewDLLHandler();
    void vOnNewBusEventHandler();
    // Delete a line from the code with the search string & header string
    BOOL bDeleteALineFromHeader(CString omStrHeader,CString);
    // Dlete selected handler
    virtual BOOL bDeleteHandler(CString omStrFuncName,CFunctionEditorDoc* pDoc);
    virtual CString omGetMsgHandlerName();
    BOOL bHandleMsgHandlerDelete(CString omStrHandler);
    BOOL bHandlePduHandlerDelete(CString omStrHandler);
    BOOL bDeleteHandlerInArray(const CStringArray& omStrArray,CFunctionEditorDoc* pDoc);
    virtual void vAddDeleteDllHandler(BOOL bDllLoad, BOOL bToAdd);
    virtual CFunctionEditorDoc* pGetFunctionEditorDoc();
    virtual BOOL vDeleteBusSpecificHandlers(CStringArray** pArray, CFunctionEditorDoc* pDoc, CString omStrFuncName);

    // Right and left mouse click points
    CPoint m_omRightClickPoint;
    CPoint m_omLeftClickPoint;

    BOOL m_bFlag;
    // Image List object
    CImageList m_omImageList;

    INT nGetLinEventTypeFromName(CString strName, std::string& strEventName);


public:

    // Populate tree with function prototypes
    // and global variables
    virtual BOOL bPopulateTree();
    afx_msg void OnFunctionEditorSave();
    static CGlobalObj* m_pGlobalObj;
};
