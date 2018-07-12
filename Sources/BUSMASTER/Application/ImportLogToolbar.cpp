// ImportLogToolbar.cpp : implementation file
//
#include "StdAfx.h"
#include "ImportLogToolbar.h"
#include "HashDefines.h"

// CImportLogToolbar

IMPLEMENT_DYNAMIC(CImportLogToolbar, CToolBar)

CImportLogToolbar::CImportLogToolbar()
{
    m_hWndScroll=nullptr;
}

CImportLogToolbar::~CImportLogToolbar()
{
}


BEGIN_MESSAGE_MAP(CImportLogToolbar, CToolBar)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CImportLogToolbar::OnHScroll(UINT /*nSBCode*/,UINT /*nPos*/,CScrollBar* pScrollBar)
{
    CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
    if(m_hWndScroll!=nullptr && pSlider!=nullptr)
    {
        int npos =  pSlider->GetPos();
        ::SendMessage(m_hWndScroll,WM_IMPORTLOG_OVERWRITE_SCROLL, npos, (LPARAM)0);
    }
}
void CImportLogToolbar::SetSliderHandleWnd(const HWND& hWnd)
{
    m_hWndScroll = hWnd;
}
// CImportLogToolbar message handlers


