/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphExportDlg.h
  Description   :  Interface file for CGraphExportDlg class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphExportDlg.h_v  $
 * 
 *    Rev 1.2   05 Aug 2011 11:06:58   CANMNTTM
 * Updated with Graph control pointer sharing.
 * 
 *    Rev 1.1   15 Apr 2011 19:43:32   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:34   CANMNTTM
 *  
 * 
 *    Rev 1.0   16 Aug 2010 21:20:32   rac2kor
 *  

  Author        :  Raja N
  Date Created  :  10/12/2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_GRAPHEXPORTDLG_H__A97DEF46_47D4_4125_B3D3_9FD61365AB4A__INCLUDED_)
#define AFX_GRAPHEXPORTDLG_H__A97DEF46_47D4_4125_B3D3_9FD61365AB4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphExportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphExportDlg dialog
class CGraphExportDlg : public CDialog
{
// Construction
public:
    CGraphExportDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
    //{{AFX_DATA(CGraphExportDlg)
    enum { IDD = IDD_DLG_GRAPH_EXPORT };
    CString m_omStrCSVFileName;
    CString m_omStrHTMLFileName;
    CString m_omStrBMPFileName;    
	IDMGraphCtrl* m_pDMGraphCtrl;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGraphExportDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CGraphExportDlg)
    afx_msg void OnBtnBrowseCsv();
    afx_msg void OnBtnSaveCsv();
    afx_msg void OnBtnBrowseHtml();
    afx_msg void OnBtnSaveHtml();
    afx_msg void OnBtnBrowseBmp();
    afx_msg void OnBtnSaveImage();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHEXPORTDLG_H__A97DEF46_47D4_4125_B3D3_9FD61365AB4A__INCLUDED_)
