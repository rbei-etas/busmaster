// MFCRibbonBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "BUSMASTER.h"
#include "MFCRibbonBarEx.h"


// CMFCRibbonBarEx

IMPLEMENT_DYNAMIC(CMFCRibbonBarEx, CMFCRibbonBar)

CMFCRibbonBarEx::CMFCRibbonBarEx()
{

}

CMFCRibbonBarEx::~CMFCRibbonBarEx()
{
}


BEGIN_MESSAGE_MAP(CMFCRibbonBarEx, CMFCRibbonBar)
END_MESSAGE_MAP()



// CMFCRibbonBarEx message handlers
bool CMFCRibbonBarEx::isWindowHidden()
{
    return (m_dwHideFlags == AFX_RIBBONBAR_HIDE_ELEMENTS);
}

void CMFCRibbonBarEx::ToggleWindowDisplay()
{
    ToggleMimimizeState();
    UpdateRibbonBarMinimisedIcon();
}

void CMFCRibbonBarEx::UpdateRibbonBarMinimisedIcon()
{
    CMFCRibbonButtonIcon* minimiseBtn = (CMFCRibbonButtonIcon*)FindByID(ID_BUTTON_TOGGLERIBBON);
    if (nullptr != minimiseBtn)
    {
        if (isWindowHidden())
        {
            minimiseBtn->SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SHOW_RIBBON)));
        }
        else
        {
            minimiseBtn->SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_HIDE_RIBBON)));
        }
        minimiseBtn->Redraw();
    }
}
BOOL CMFCRibbonBarEx::OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit)
{
    auto result = CMFCRibbonBar::OnShowRibbonContextMenu(pWnd, x, y, pHit);
    UpdateRibbonBarMinimisedIcon();
    return result;
}