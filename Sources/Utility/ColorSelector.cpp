// ColorSelector.cpp : implementation file
//
#include "Utils_StdAfx.h"
#include "ColorSelector.h"


// CColorSelector

//IMPLEMENT_DYNAMIC(CColorSelector, CButton)


CColorSelector::CColorSelector()
{

}

CColorSelector::~CColorSelector()
{
}


BEGIN_MESSAGE_MAP(CColorSelector, CButton)
    ON_CONTROL_REFLECT(BN_CLICKED, OnBnClicked)
END_MESSAGE_MAP()



// CColorSelector message handlers



void CColorSelector::PreSubclassWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    this->ModifyStyle(0, BS_OWNERDRAW);
    CButton::PreSubclassWindow();
}

void CColorSelector::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your message handler code here and/or call default
    CDC omDc;
    omDc.Attach(lpDrawItemStruct->hDC);
	UINT unButtonState  = lpDrawItemStruct->itemState;
	int nPushState = ((unButtonState & ODS_SELECTED) ? DFCS_PUSHED : 0) |
					 ((unButtonState & ODS_DISABLED) ? DFCS_INACTIVE : 0);

    CRect omItemRect    = lpDrawItemStruct->rcItem;
    
	
    
	CRect omArrowRect;
    omArrowRect.left = max(0, omItemRect.Width()- GetSystemMetrics(SM_CXHTHUMB) - 3 );
    omArrowRect.right = max(0, omItemRect.Width()-3);
    omArrowRect.top = 3;
    omArrowRect.bottom = max(omItemRect.Height()-3, GetSystemMetrics(SM_CYVTHUMB)-3);
    
	
    omDc.DrawFrameControl(&omArrowRect, DFC_SCROLL, DFCS_SCROLLDOWN  | nPushState);

    // Create backgroung brush
    CBrush brush( m_omColorBkg);
    omDc.SelectStockObject(BLACK_PEN);
    omDc.DrawEdge(&lpDrawItemStruct->rcItem, EDGE_RAISED, BF_RECT);
    CRect omButtonRect;
    
    omButtonRect.left = lpDrawItemStruct->rcItem.left+5;
    omButtonRect.top = lpDrawItemStruct->rcItem.top+5;
    omButtonRect.right = lpDrawItemStruct->rcItem.right - omArrowRect.Width()-5;// - omArrowRect.Width() ;
    omButtonRect.bottom = lpDrawItemStruct->rcItem.bottom - 5;
    
    //omButtonRect.DeflateRect(3, 3);
    omDc.FillRect(omButtonRect, &brush);
    // Select Old Brush
    omDc.SelectObject(brush);
    
    omButtonRect.DeflateRect(-1, -1);
    
    omDc.Rectangle(omButtonRect);

    if (unButtonState & ODS_FOCUS) 
    {
        omButtonRect.DeflateRect(1,1);
        omDc.DrawFocusRect(omButtonRect);
    }
}

void CColorSelector::OnBnClicked()
{
    // TODO: Add your control notification handler code here
    CColorDialog omColorDialog;
    omColorDialog.m_cc.Flags |= CC_SOLIDCOLOR| CC_RGBINIT;
    if (IDOK == omColorDialog.DoModal())
    {
        m_omColorBkg = omColorDialog.GetColor();
        RedrawWindow();
    }

}
void CColorSelector::SetColour(COLORREF omColor)
{
    m_omColorBkg = omColor;
    if (TRUE == IsWindow(m_hWnd))
    {
        CWnd* pParent = GetParent();
        if( NULL != pParent )
        {
            pParent->SendMessage(WM_CPN_COLORSELECTED, (WPARAM)m_omColorBkg, GetDlgCtrlID());
        }
        RedrawWindow();
    }
};