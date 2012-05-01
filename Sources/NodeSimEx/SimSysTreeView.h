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
 * \file      SimSysTreeView.h
 * \brief     This header file contains the defintion of class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include "Include/Basedefs.h"
#include "SimSysConfigDetails.h"  // For CSimSysConfigDetails class decl
/////////////////////////////////////////////////////////////////////////////
// CSimSysTreeView view

class CSimSysTreeView : public CTreeView
{
public:
    CSimSysTreeView();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CSimSysTreeView)

    // Attributes
public:

    // Operations
public:

    CStringList m_omSimsysNames;
    BOOL bSetSimsysName(CString& omStrSimsysName);
    void vPopulateAddedSimSysInfo(CString omStrSimSysPath);

    void vSetAllDllloaded();
    void vSetDllLoaded();

    // to populate the tree view.
    BOOL bPopulateTree();

    // To update the edited node details (dll path)
    void vSetDllName(CString& omStrPrevDllName , CString& omStrNewDllName );

    // To update the edited node details (dll path)
    void vSetSimSysDllName(CString& omStrNodeName,
                           CString& omStrNewDllName ,
                           BOOL bIsDllLoaded);


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSimSysTreeView)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    //}}AFX_VIRTUAL

    // Implementation
protected:
    virtual ~CSimSysTreeView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:
    //{{AFX_MSG(CSimSysTreeView)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTreeViewRightclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnAddSimsys();
    afx_msg void OnAllNodeHandlers();
    afx_msg void OnDeleteAllSimsys();
    afx_msg void OnDeleteNode();
    afx_msg void OnEditNode();
    afx_msg void OnNewSimsys();
    afx_msg void OnNodeErrorhandlers();
    afx_msg void OnNodeKeyhandlers();
    afx_msg void OnNodeMessagehandlers();
    afx_msg void OnNodeTimerhandlers();
    afx_msg void OnAddnode();
    afx_msg void OnSimsysAllErrorhandlers();
    afx_msg void OnSimsysAllhandlers();
    afx_msg void OnSimsysAllKeyhandlers();
    afx_msg void OnSimsysAllMsghandlers();
    afx_msg void OnSimsysAllTimerhandlers();
    afx_msg void OnSimsysBuildall();
    afx_msg void OnDeleteAllNodes();
    afx_msg void OnDeleteSimulatedsystem();
    afx_msg void OnSimsysLoadall();
    afx_msg void OnSimsysSave();
    afx_msg void OnSimsysSaveAs();
    afx_msg void OnSimsysUnloadall();
    afx_msg void OnSimsysSaveAll();
    afx_msg void OnSimsysAllDllHandlers();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    ETYPE_BUS m_eBus;
    CMenu* pomContextMenu;
    CMenu* m_pomSubMenu;

    CMenu* m_pomContextMenu;
    BOOL bCallFrmSaveAs;
    int m_omSimsysCount;
    BOOL m_bIsNewNode;

    CString m_omSelectedItemText;
    CString omStrSimsysToBeDeleted;

    HTREEITEM m_hRootItem;

    CPoint m_omRightClickPoint;
    CPoint m_omLeftCLickPoint;
    HTREEITEM m_hTreeItem;
    void vAddEditNode(BOOL bMode);
    void vSaveSimsSysIfModified(CString omSimSysName) ;

public:
    static ETYPE_BUS CSimSysTreeView::sm_eBus;
    void vAddSimDetFromFile(CString oCfgFilename);
    void vDisplayRootMenu();
    void vDisplaySimSysMenu();
    void vDisplayNodeMenu();
    HTREEITEM hInsertAndSelectItem(CString omItemName, HTREEITEM hParent);
    PSSIMSYSINFO psGetCurrentSimSysInfo();

};
