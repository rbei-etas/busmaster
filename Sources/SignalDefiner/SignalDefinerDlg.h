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
    bool    m_bAutoCorrect;
    float m_fAmplitude;
    float m_fFrequency;
    int m_nSelCycle;
    double m_dblSamplingTimePeriod;
    BSTR m_strSignalName;
    CComPtr<IDMGraphCollection> spElements;

    afx_msg void OnCbnSelchangeComboSignalType();
    afx_msg void OnCbnSelchangeComboCycles();
    afx_msg void OnEnChangeEditSignalAmplitude();
    afx_msg void OnEnChangeEditSignalFrequency();
    afx_msg void OnEnChangeEditSignalSamplingTime();
    afx_msg void OnBnClickedOk();
    afx_msg void OnEnUpdateEditSignalSamplingTime();
    afx_msg void OnBnClickedAutoCorrect();
    afx_msg void OnClose();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnKillFocus(UINT nID);

    virtual BOOL OnInitDialog();

    CComboBox m_ctrSignalType;
    CComboBox m_ctrNoOfCycles;
    CButton   m_btnAutoCorrect;

    CWnd* m_poDMGraphCtrl;
    int m_nSignalType;

private:

    void vGenerateWave();
    void SetGraphData(VARIANT* pvarrX, VARIANT* pvarrY);
};
