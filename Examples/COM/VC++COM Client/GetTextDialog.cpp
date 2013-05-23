// GetTextDialog.cpp : implementation file
//

#include "stdafx.h"
//#include "Client_CANvas.h"
#include "Resource.h"
#include "GetTextDialog.h"


// CGetTextDialog dialog

IMPLEMENT_DYNAMIC(CGetTextDialog, CDialog)
CGetTextDialog::CGetTextDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGetTextDialog::IDD, pParent)
    , m_omText(_T(""))
{
}

CGetTextDialog::~CGetTextDialog()
{
}

void CGetTextDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_omText);
}


BEGIN_MESSAGE_MAP(CGetTextDialog, CDialog)
END_MESSAGE_MAP()


// CGetTextDialog message handlers

void CGetTextDialog::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData(TRUE);

    CDialog::OnOK();
}
