// BusStatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "BusStatDlg.h"


// CBusStatDlg dialog

IMPLEMENT_DYNAMIC(CBusStatDlg, CDialog)

CBusStatDlg::CBusStatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBusStatDlg::IDD, pParent)
    , m_omDetails(_T(""))
{
}

CBusStatDlg::CBusStatDlg(sBUSSTATISTICS_USR* psBusStat, CWnd* pParent /*=NULL*/)
	: CDialog(CBusStatDlg::IDD, pParent)
    , m_omDetails(_T(""))
{
    m_psBusStat = psBusStat;
}

CBusStatDlg::~CBusStatDlg()
{
}

void CBusStatDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_omDetails);
}


BEGIN_MESSAGE_MAP(CBusStatDlg, CDialog)
END_MESSAGE_MAP()


// CBusStatDlg message handlers

BOOL CBusStatDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_omDetails.Format(_T("BusLoad: %f\r\nPeakBusLoad: %f\r\nTotalMessage: %d\r\nTotal Tx: %d"),
        m_psBusStat->m_dBusLoad, 
        m_psBusStat->m_dPeakBusLoad,
        m_psBusStat->m_unTotalMsgCount,
        m_psBusStat->m_unTotalTxMsgCount);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
