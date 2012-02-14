#pragma once

#include "SignalDefiner_Resource.h"
#include "afxwin.h"
// CSignalDefinerDlg dialog

class CSignalDefinerDlg : public CDialog
{
	DECLARE_DYNAMIC(CSignalDefinerDlg)

public:
	CSignalDefinerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSignalDefinerDlg();

// Dialog Data
	enum { IDD = IDD_SIGNAL_DEFINER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_fAmplitude;
	float m_fFrequency;
	int m_nSelCycle;
	double m_dblSamplingTimePeriod;
	BSTR m_strSignalName;
	
	afx_msg void OnCbnSelchangeComboSignalType();
	afx_msg void OnCbnSelchangeComboCycles();
	afx_msg void OnEnChangeEditSignalAmplitude();
	afx_msg void OnEnChangeEditSignalFrequency();
	afx_msg void OnEnChangeEditSignalSamplingTime();
	afx_msg void OnBnClickedOk();

	virtual BOOL OnInitDialog();

	CComboBox m_ctrSignalType;
	CComboBox m_ctrNoOfCycles;	

	CWnd* m_poDMGraphCtrl;	
	int m_nSignalType;

private:

	void vGenerateWave();
	void SetGraphData(VARIANT* pvarrX, VARIANT* pvarrY);		
};
