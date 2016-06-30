// ConfigMsgDispPropPg.cpp : implementation file
/**********************************************************************************
Project         :   Frame
FileName        :   ConfigMsgDispPropPg.cpp
Description     :   Implementation of CConfigMsgDispPropPg class
Directory       :
Version         :   V1.0
Authors         :   Ravi D
Date Created    :   17/10/2006
Date Modified   :
Modifications   :
**********************************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "HashDefines.h"
#include "ConfigMsgDispPropPg.h"

// CConfigMsgDispPropPg dialog

IMPLEMENT_DYNAMIC(CConfigMsgDispPropPg, CPropertyPage)
CConfigMsgDispPropPg::CConfigMsgDispPropPg()
    : CPropertyPage(CConfigMsgDispPropPg::IDD)
{
    m_clrNullFrame = COLOUR_NULL_FRAME;
    m_clrDynamicFrame = COLOUR_DYNC_FRAME;
    m_clrStaticFrame = COLOUR_STAT_FRAME;
    m_clrSyncFrame = COLOUR_SYNC_FRAME;
}

/**********************************************************************************
Function Name   :   ~CConfigMsgDispPropPg()
Input(s)        :   -
Output          :   -
Functionality   :   Destructor
Member of       :   CConfigMsgDispPropPg
Friend of       :   -
Authors         :   Ravi D
Date Created    :   17/10/2006
Modifications   :   -
************************************************************************************/
CConfigMsgDispPropPg::~CConfigMsgDispPropPg()
{
}

/**********************************************************************************
Function Name   :   DoDataExchange
Input(s)        :   pDX - Pointer to the exchange object
Output          :   -
Functionality   :   To exchange and validate the dialog data
Member of       :   CConfigMsgDispPropPg
Friend of       :   -
Authors         :   Ravi D
Date Created    :   17/10/2006
Modifications   :   -
************************************************************************************/
void CConfigMsgDispPropPg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigMsgDispPropPg, CPropertyPage)
    ON_WM_CTLCOLOR()
    ON_STN_CLICKED(IDC_STAT_NULLFRAME_COLOR, OnStnClickedNullframeColor)
    ON_STN_CLICKED(IDC_STAT_SYNCFRAME_COLOR, OnStnClickedSyncframeColor)
    ON_STN_CLICKED(IDC_STAT_STATICFRAME_COLOR, OnStnClickedStaticframeColor)
    ON_STN_CLICKED(IDC_STAT_DYNAMICFRAME_COLOR, OnStnClickedDynamicframeColor)
END_MESSAGE_MAP()


// CConfigMsgDispPropPg message handlers
/**********************************************************************************
Function Name   :   OnCtlColor
Input(s)        :   pDC - handle to the display context
                    pWnd - handle of the window for which the color has to be changed
                    nCtlColor - type of the window control
Output          :   handle to the brush that is used for painting
Functionality   :   To change the color scheme of the controls like list boxes etc..
Member of       :   CConfigMsgDispPropPg
Friend of       :   -
Authors         :   Ravi D
Date Created    :   17/10/2006
Modifications   :   -
************************************************************************************/
HBRUSH CConfigMsgDispPropPg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here
    if (pWnd->GetDlgCtrlID() == IDC_NULLFRAME_COLOR_STAT)
    {
        hbr = CreateSolidBrush(m_clrNullFrame);
    }

    if (pWnd->GetDlgCtrlID() == IDC_SYNCFRAME_COLOR_STAT)
    {
        hbr = CreateSolidBrush(m_clrSyncFrame);
    }

    if (pWnd->GetDlgCtrlID() == IDC_STATICFRAME_COLOR_STAT)
    {
        hbr = CreateSolidBrush(m_clrStaticFrame);
    }

    if (pWnd->GetDlgCtrlID() == IDC_DYNAMICFRAME_COLOR_STAT)
    {
        hbr = CreateSolidBrush(m_clrDynamicFrame);
    }

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

BOOL CConfigMsgDispPropPg::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigMsgDispPropPg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    CPropertyPage::OnOK();
}

void CConfigMsgDispPropPg::OnStnClickedNullframeColor()
{
    // TODO: Add your control notification handler code here
    CColorDialog dlg(m_clrNullFrame, CC_FULLOPEN | CC_ANYCOLOR, this);
    if (dlg.DoModal() == IDOK)
    {
        m_clrNullFrame = dlg.GetColor();
        GetDlgItem(IDC_STAT_NULLFRAME_COLOR)->InvalidateRect(nullptr);
    }
}

void CConfigMsgDispPropPg::OnStnClickedSyncframeColor()
{
    // TODO: Add your control notification handler code here
    CColorDialog dlg(m_clrSyncFrame, CC_FULLOPEN | CC_ANYCOLOR, this);
    if (dlg.DoModal() == IDOK)
    {
        m_clrSyncFrame = dlg.GetColor();
        GetDlgItem(IDC_STAT_SYNCFRAME_COLOR)->InvalidateRect(nullptr);
    }
}

void CConfigMsgDispPropPg::OnStnClickedStaticframeColor()
{
    // TODO: Add your control notification handler code here
    CColorDialog dlg(m_clrStaticFrame, CC_FULLOPEN | CC_ANYCOLOR, this);
    if (dlg.DoModal() == IDOK)
    {
        m_clrStaticFrame = dlg.GetColor();
        GetDlgItem(IDC_STAT_STATICFRAME_COLOR)->InvalidateRect(nullptr);
    }
}

void CConfigMsgDispPropPg::OnStnClickedDynamicframeColor()
{
    // TODO: Add your control notification handler code here
    CColorDialog dlg(m_clrDynamicFrame, CC_FULLOPEN | CC_ANYCOLOR, this);
    if (dlg.DoModal() == IDOK)
    {
        m_clrDynamicFrame = dlg.GetColor();
        GetDlgItem(IDC_STAT_DYNAMICFRAME_COLOR)->InvalidateRect(nullptr);
    }
}