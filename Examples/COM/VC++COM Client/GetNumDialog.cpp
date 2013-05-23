// GetNumDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "GetNumDialog.h"


// GetNumDialog dialog

IMPLEMENT_DYNAMIC(GetNumDialog, CDialog)
GetNumDialog::GetNumDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GetNumDialog::IDD, pParent)
    , m_dwNumber(0)
{
}

GetNumDialog::~GetNumDialog()
{
}

void GetNumDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_GETNUM, m_dwNumber);
}


BEGIN_MESSAGE_MAP(GetNumDialog, CDialog)
END_MESSAGE_MAP()


// GetNumDialog message handlers

void GetNumDialog::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateData(TRUE);

    CDialog::OnOK();
}
