/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  ExportLogFileDlg.h
  Description   :  
  $Log:   X:/Archive/Sources/Application/ExportLogFileDlg.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 20:00:04   CANMNTTM
 * Added RBEI Copyright information.
  
  Author(s)     :  Pradeep Kadoor
  Date Created  :  15/04/2011
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 ******************************************************************************/
// CExportLogFileDlg dialog
#include "LogToExcel.h"
#include "Defines.h"
#include "resource.h"
#include "afxwin.h"
class CExportLogFileDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CExportLogFileDlg)

public:
	CExportLogFileDlg(ETYPE_BUS eBus = CAN, CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportLogFileDlg();

// Dialog Data
	enum { IDD = IDD_DLG_EXPORT_LOG_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogBrowse();
	CString m_omStrLogFileName;
	afx_msg void OnBnClickedBtnCsvBrowse();
	CString m_omStrCSVFileName;
	afx_msg void OnBnClickedButtonSelectall();
	virtual BOOL OnInitDialog();
private: 
    ETYPE_BUS m_eBus;
    CHAR_ARRAY_20* m_pacFields;
    UINT m_unNoOfFileds;
	void vPopulateAvailableList();
	void vEnableDisableControls();

public:
	CListBox m_omAvailableList;
	CListBox m_omSelectedList;
	CButton m_omSelectAllFields;
	CButton m_omSelectOneField;
	CButton m_omRemoveOneField;
	CButton m_omRemoveAllFields;
	afx_msg void OnBnClickedConvert();
	afx_msg void OnBnClickedCbtnCancelexport();
	afx_msg void OnLbnSelchangeLstAvailable();
	afx_msg void OnLbnSelchangeLstSelected();
	afx_msg void OnBnClickedButtonSelectone();
	afx_msg void OnBnClickedButtonRemoveone();
	afx_msg void OnBnClickedButtonRemoveall();
    afx_msg void OnCbnSelchangeComboBustype();
    CComboBox m_omBusType;
};
