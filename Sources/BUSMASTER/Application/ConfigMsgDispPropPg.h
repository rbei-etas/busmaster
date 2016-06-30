/**********************************************************************************
    Project         :   Frame
    FileName        :   ConfigMsgDispPropPg.h
    Description     :   Definition of CConfigMsgDispPropPg class
    Directory       :
    Version         :   V1.0
    Authors         :   Ravi D
    Date Created    :   17/10/2006
    Date Modified   :
    Modifications   :
**********************************************************************************/

#pragma once

// CConfigMsgDispPropPg dialog

class CConfigMsgDispPropPg : public CPropertyPage
{
    DECLARE_DYNAMIC(CConfigMsgDispPropPg)

public:
    CConfigMsgDispPropPg();
    virtual ~CConfigMsgDispPropPg();

    // Dialog Data
    enum { IDD = IDD_PROPPAGE_CONFIG_MSG_DISPLAY };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    COLORREF m_clrNullFrame;
    COLORREF m_clrDynamicFrame;
    COLORREF m_clrStaticFrame;
    COLORREF m_clrSyncFrame;
    afx_msg void OnStnClickedNullframeColor();
    afx_msg void OnStnClickedSyncframeColor();
    afx_msg void OnStnClickedStaticframeColor();
    afx_msg void OnStnClickedDynamicframeColor();
};
