// CANFrameParam.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "CANFrameParam.h"


// CCANFrameParam dialog

IMPLEMENT_DYNAMIC(CCANFrameParam, CDialog)
CCANFrameParam::CCANFrameParam(CWnd* pParent /*=NULL*/)
	: CDialog(CCANFrameParam::IDD, pParent)
    , m_bExtended(FALSE)
    , m_bRTR(FALSE)
{
    memset(&m_sCFrame, 0, sizeof(m_sCFrame));
}

CCANFrameParam::~CCANFrameParam()
{
}

void CCANFrameParam::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHK_EXTENDED, m_bExtended);
    DDX_Check(pDX, IDC_CHK_RTR, m_bRTR);
    DDX_Text (pDX, IDC_EDIT_CHANNEL, m_sCFrame.m_ucChannel);
    DDX_Text (pDX, IDC_EDIT_CANID,   m_sCFrame.m_unMsgID);
    DDX_Text (pDX, IDC_EDIT_DLC,     m_sCFrame.m_ucDataLen);
    DDX_Text (pDX, IDC_EDIT_DATA1,   m_sCFrame.m_ucData[0]);
    DDX_Text (pDX, IDC_EDIT_DATA2,   m_sCFrame.m_ucData[1]);
    DDX_Text (pDX, IDC_EDIT_DATA3,   m_sCFrame.m_ucData[2]);
    DDX_Text (pDX, IDC_EDIT_DATA4,   m_sCFrame.m_ucData[3]);
    DDX_Text (pDX, IDC_EDIT_DATA5,   m_sCFrame.m_ucData[4]);
    DDX_Text (pDX, IDC_EDIT_DATA6,   m_sCFrame.m_ucData[5]);
    DDX_Text (pDX, IDC_EDIT_DATA7,   m_sCFrame.m_ucData[6]);
    DDX_Text (pDX, IDC_EDIT_DATA8,   m_sCFrame.m_ucData[7]);
}


BEGIN_MESSAGE_MAP(CCANFrameParam, CDialog)
END_MESSAGE_MAP()


// CCANFrameParam message handlers

void CCANFrameParam::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData(TRUE);

    m_sCFrame.m_bEXTENDED = (unsigned char) m_bExtended;
    m_sCFrame.m_bRTR = (unsigned char) m_bRTR;

    CDialog::OnOK();
}

BOOL CCANFrameParam::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_bExtended = (BOOL) m_sCFrame.m_bEXTENDED;
    m_bRTR = (BOOL) m_sCFrame.m_bRTR;

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
