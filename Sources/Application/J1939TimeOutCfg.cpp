/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  J1939TimeOutCfg.cpp
  Description   :
  $Log:   X:/Archive/Sources/Application/J1939TimeOutCfg.cpv  $
  Author(s)     :  Pradeep Kadoor
  Date Created  :  22/12/2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#include "stdafx.h"
#include "Resource.h"
#include "Datatypes/J1939_Datatypes.h"
#include "InterfaceGetter.h"
#include "DIL_Interface/BaseDIL_J1939.h"
#include "J1939TimeOutCfg.h"


#define TEXT_BROADCAST_TO   _T("Tb: Time interval between two packets in broadcast transmission.")
#define TEXT_RESPONSE_TO    _T("Tr: Maximum wait period for a sender without receiving response from the receiver.")
#define TEXT_HOLDING_TO     _T("Th: Time interval between two 'Clear to Send' message when delaying the data.")
#define TEXT_T1_TO          _T("T1: Maximum wait period for a receiver when more packets were expected.")
#define TEXT_T2_TO          _T("T2: Maximum wait period for a receiver to receive data packets after sending 'Clear to Send' message.")
#define TEXT_T3_TO          _T("T3: Maximum wait period for a sender to receive 'End of Message Acknowledgement' after sending last data packet.")
#define TEXT_T4_TO          _T("T4: Maximum wait period for a sender to receive another 'Clear to Send' message after receiving one to delay the data.")
// CJ1939TimeOutCfg dialog

IMPLEMENT_DYNAMIC(CJ1939TimeOutCfg, CDialog)
CJ1939TimeOutCfg::CJ1939TimeOutCfg(CWnd* pParent /*=NULL*/)
    : CDialog(CJ1939TimeOutCfg::IDD, pParent)
{
}

CJ1939TimeOutCfg::~CJ1939TimeOutCfg()
{
}

void CJ1939TimeOutCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_BROADCAST, m_omBroadcast);
    DDX_Control(pDX, IDC_EDIT_RESPONSE, m_omResponse);
    DDX_Control(pDX, IDC_EDIT_HOLDING, m_omHolding);
    DDX_Control(pDX, IDC_EDIT_T1, m_omT1);
    DDX_Control(pDX, IDC_EDIT_T2, m_omT2);
    DDX_Control(pDX, IDC_EDIT_T3, m_omT3);
    DDX_Control(pDX, IDC_EDIT_T4, m_omT4);
}


BEGIN_MESSAGE_MAP(CJ1939TimeOutCfg, CDialog)
    ON_BN_CLICKED(IDC_CHECK_BROADCAST, OnBnClickedCheckBroadcast)
    ON_BN_CLICKED(IDC_CHECK_RESPONSE, OnBnClickedCheckResponse)
    ON_BN_CLICKED(IDC_CHECK_HOLDING, OnBnClickedCheckHolding)
    ON_BN_CLICKED(IDC_CHECK_T1, OnBnClickedCheckT1)
    ON_BN_CLICKED(IDC_CHECK_T2, OnBnClickedCheckT2)
    ON_BN_CLICKED(IDC_CHECK_T3, OnBnClickedCheckT3)
    ON_BN_CLICKED(IDC_CHECK_T4, OnBnClickedCheckT4)
    ON_EN_CHANGE(IDC_EDIT_BROADCAST, OnEnChangeEditBroadcast)
    ON_EN_CHANGE(IDC_EDIT_RESPONSE, OnEnChangeEditResponse)
    ON_EN_CHANGE(IDC_EDIT_HOLDING, OnEnChangeEditHolding)
    ON_EN_CHANGE(IDC_EDIT_T1, OnEnChangeEditT1)
    ON_EN_CHANGE(IDC_EDIT_T2, OnEnChangeEditT2)
    ON_EN_CHANGE(IDC_EDIT_T3, OnEnChangeEditT3)
    ON_EN_CHANGE(IDC_EDIT_T4, OnEnChangeEditT4)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

//Init dialog
BOOL CJ1939TimeOutCfg::OnInitDialog()
{
    CDialog::OnInitDialog();
    CButton* pButton = NULL;
    UINT unTimeOutVal = 0;
    pButton = (CButton*)GetDlgItem(IDC_CHECK_BROADCAST);
    pButton->SetCheck(BST_UNCHECKED);
    GetIJ1939DIL()->DILIJ_GetTimeOut(TYPE_TO_BROADCAST, unTimeOutVal);
    m_omBroadcast.vSetBase(BASE_DECIMAL);
    m_omBroadcast.vSetSigned(FALSE);
    m_omBroadcast.LimitText(4);
    m_omBroadcast.vSetValue(unTimeOutVal);
    OnBnClickedCheckBroadcast();
    pButton = (CButton*)GetDlgItem(IDC_CHECK_RESPONSE);
    pButton->SetCheck(BST_UNCHECKED);
    GetIJ1939DIL()->DILIJ_GetTimeOut(TYPE_TO_RESPONSE, unTimeOutVal);
    m_omResponse.vSetBase(BASE_DECIMAL);
    m_omResponse.vSetSigned(FALSE);
    m_omResponse.LimitText(4);
    m_omResponse.vSetValue(unTimeOutVal);
    OnBnClickedCheckResponse();
    pButton = (CButton*)GetDlgItem(IDC_CHECK_HOLDING);
    pButton->SetCheck(BST_UNCHECKED);
    GetIJ1939DIL()->DILIJ_GetTimeOut(TYPE_TO_HOLDING, unTimeOutVal);
    m_omHolding.vSetBase(BASE_DECIMAL);
    m_omHolding.vSetSigned(FALSE);
    m_omHolding.LimitText(4);
    m_omHolding.vSetValue(unTimeOutVal);
    OnBnClickedCheckHolding();
    pButton = (CButton*)GetDlgItem(IDC_CHECK_T1);
    pButton->SetCheck(BST_UNCHECKED);
    GetIJ1939DIL()->DILIJ_GetTimeOut(TYPE_TO_T1, unTimeOutVal);
    m_omT1.vSetBase(BASE_DECIMAL);
    m_omT1.vSetSigned(FALSE);
    m_omT1.LimitText(4);
    m_omT1.vSetValue(unTimeOutVal);
    OnBnClickedCheckT1();
    pButton = (CButton*)GetDlgItem(IDC_CHECK_T2);
    pButton->SetCheck(BST_UNCHECKED);
    GetIJ1939DIL()->DILIJ_GetTimeOut(TYPE_TO_T2, unTimeOutVal);
    m_omT2.vSetBase(BASE_DECIMAL);
    m_omT2.vSetSigned(FALSE);
    m_omT2.LimitText(4);
    m_omT2.vSetValue(unTimeOutVal);
    OnBnClickedCheckT2();
    pButton = (CButton*)GetDlgItem(IDC_CHECK_T3);
    pButton->SetCheck(BST_UNCHECKED);
    GetIJ1939DIL()->DILIJ_GetTimeOut(TYPE_TO_T3, unTimeOutVal);
    m_omT3.vSetBase(BASE_DECIMAL);
    m_omT3.vSetSigned(FALSE);
    m_omT3.LimitText(4);
    m_omT3.vSetValue(unTimeOutVal);
    OnBnClickedCheckT3();
    pButton = (CButton*)GetDlgItem(IDC_CHECK_T4);
    pButton->SetCheck(BST_UNCHECKED);
    GetIJ1939DIL()->DILIJ_GetTimeOut(TYPE_TO_T4, unTimeOutVal);
    m_omT4.vSetBase(BASE_DECIMAL);
    m_omT4.vSetSigned(FALSE);
    m_omT4.LimitText(4);
    m_omT4.vSetValue(unTimeOutVal);
    OnBnClickedCheckT4();
    pButton = (CButton*)GetDlgItem(IDC_STATIC_DESCRIPTION);
    pButton->SetWindowText(_T(""));
    return TRUE;
}
// CJ1939TimeOutCfg message handlers

void CJ1939TimeOutCfg::OnBnClickedCheckBroadcast()
{
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_BROADCAST);
    BOOL bEnable = pButton->GetCheck() == BST_CHECKED;
    m_omBroadcast.EnableWindow(bEnable);
    OnEnChangeEditBroadcast();
    GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CJ1939TimeOutCfg::OnBnClickedCheckResponse()
{
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_RESPONSE);
    BOOL bEnable = pButton->GetCheck() == BST_CHECKED;
    m_omResponse.EnableWindow(bEnable);
    OnEnChangeEditResponse();
    GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CJ1939TimeOutCfg::OnBnClickedCheckHolding()
{
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_HOLDING);
    BOOL bEnable = pButton->GetCheck() == BST_CHECKED;
    m_omHolding.EnableWindow(bEnable);
    OnEnChangeEditHolding();
    GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CJ1939TimeOutCfg::OnBnClickedCheckT1()
{
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_T1);
    BOOL bEnable = pButton->GetCheck() == BST_CHECKED;
    m_omT1.EnableWindow(bEnable);
    OnEnChangeEditT1();
    GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CJ1939TimeOutCfg::OnBnClickedCheckT2()
{
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_T2);
    BOOL bEnable = pButton->GetCheck() == BST_CHECKED;
    m_omT2.EnableWindow(bEnable);
    OnEnChangeEditT2();
    GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CJ1939TimeOutCfg::OnBnClickedCheckT3()
{
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_T3);
    BOOL bEnable = pButton->GetCheck() == BST_CHECKED;
    m_omT3.EnableWindow(bEnable);
    OnEnChangeEditT3();
    GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CJ1939TimeOutCfg::OnBnClickedCheckT4()
{
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_T4);
    BOOL bEnable = pButton->GetCheck() == BST_CHECKED;
    m_omT4.EnableWindow(bEnable);
    OnEnChangeEditT4();
    GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CJ1939TimeOutCfg::OnEnChangeEditBroadcast()
{
    GetDlgItem(IDC_STATIC_DESCRIPTION)->SetWindowText(TEXT_BROADCAST_TO);
}

void CJ1939TimeOutCfg::OnEnChangeEditResponse()
{
    GetDlgItem(IDC_STATIC_DESCRIPTION)->SetWindowText(TEXT_RESPONSE_TO);
}

void CJ1939TimeOutCfg::OnEnChangeEditHolding()
{
    GetDlgItem(IDC_STATIC_DESCRIPTION)->SetWindowText(TEXT_HOLDING_TO);
}

void CJ1939TimeOutCfg::OnEnChangeEditT1()
{
    GetDlgItem(IDC_STATIC_DESCRIPTION)->SetWindowText(TEXT_T1_TO);
}

void CJ1939TimeOutCfg::OnEnChangeEditT2()
{
    GetDlgItem(IDC_STATIC_DESCRIPTION)->SetWindowText(TEXT_T2_TO);
}

void CJ1939TimeOutCfg::OnEnChangeEditT3()
{
    GetDlgItem(IDC_STATIC_DESCRIPTION)->SetWindowText(TEXT_T3_TO);
}

void CJ1939TimeOutCfg::OnEnChangeEditT4()
{
    GetDlgItem(IDC_STATIC_DESCRIPTION)->SetWindowText(TEXT_T4_TO);
}

void CJ1939TimeOutCfg::OnBnClickedOk()
{
    CButton* pButton = NULL;
    pButton = (CButton*)GetDlgItem(IDC_CHECK_BROADCAST);

    if (pButton->GetCheck() == BST_CHECKED)
    {
        GetIJ1939DIL()->DILIJ_ConfigureTimeOut(TYPE_TO_BROADCAST, (UINT)m_omBroadcast.lGetValue());
    }

    pButton = (CButton*)GetDlgItem(IDC_CHECK_RESPONSE);

    if (pButton->GetCheck() == BST_CHECKED)
    {
        GetIJ1939DIL()->DILIJ_ConfigureTimeOut(TYPE_TO_RESPONSE, (UINT)m_omResponse.lGetValue());
    }

    pButton = (CButton*)GetDlgItem(IDC_CHECK_HOLDING);

    if (pButton->GetCheck() == BST_CHECKED)
    {
        GetIJ1939DIL()->DILIJ_ConfigureTimeOut(TYPE_TO_HOLDING, (UINT)m_omHolding.lGetValue());
    }

    pButton = (CButton*)GetDlgItem(IDC_CHECK_T1);

    if (pButton->GetCheck() == BST_CHECKED)
    {
        GetIJ1939DIL()->DILIJ_ConfigureTimeOut(TYPE_TO_T1, (UINT)m_omT1.lGetValue());
    }

    pButton = (CButton*)GetDlgItem(IDC_CHECK_T2);

    if (pButton->GetCheck() == BST_CHECKED)
    {
        GetIJ1939DIL()->DILIJ_ConfigureTimeOut(TYPE_TO_T2, (UINT)m_omT2.lGetValue());
    }

    pButton = (CButton*)GetDlgItem(IDC_CHECK_T3);

    if (pButton->GetCheck() == BST_CHECKED)
    {
        GetIJ1939DIL()->DILIJ_ConfigureTimeOut(TYPE_TO_T3, (UINT)m_omT3.lGetValue());
    }

    pButton = (CButton*)GetDlgItem(IDC_CHECK_T4);

    if (pButton->GetCheck() == BST_CHECKED)
    {
        GetIJ1939DIL()->DILIJ_ConfigureTimeOut(TYPE_TO_T4, (UINT)m_omT4.lGetValue());
    }

    GetDlgItem(IDOK)->EnableWindow(FALSE);
    //OnOK();
}
