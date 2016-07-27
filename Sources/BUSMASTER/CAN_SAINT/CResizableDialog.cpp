#include "CAN_SAINT_stdafx.h"
#include "CResizableDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CResizableDialog::CResizableDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
    : CDialog(nIDTemplate, pParent),
      m_bShowGripper(true),
      m_szMinimum(0, 0),
      m_hGripper(NULL)
{
}


BEGIN_MESSAGE_MAP(CResizableDialog, CDialog)
    //{{AFX_MSG_MAP(CResizableDialog)
    ON_WM_GETMINMAXINFO()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CResizableDialog::AutoMove( int iID, 
						    double dXMovePct, 
							double dYMovePct, 
							double dXSizePct, 
							double dYSizePct)
{
    ASSERT((dXMovePct + dXSizePct) <= 100.0);   // can't use more than 100% of the resize for the child
    ASSERT((dYMovePct + dYSizePct) <= 100.0);   // can't use more than 100% of the resize for the child
    SMovingChild s;
    GetDlgItem(iID, &s.m_hWnd);
    ASSERT(s.m_hWnd != NULL);
    s.m_dXMoveFrac = dXMovePct / 100.0;
    s.m_dYMoveFrac = dYMovePct / 100.0;
    s.m_dXSizeFrac = dXSizePct / 100.0;
    s.m_dYSizeFrac = dYSizePct / 100.0;
    ::GetWindowRect(s.m_hWnd, &s.m_rcInitial);
    ScreenToClient(s.m_rcInitial);
    m_MovingChildren.push_back(s);
}

BOOL CResizableDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // use the initial dialog size as the default minimum
    if ((m_szMinimum.cx == 0) && (m_szMinimum.cy == 0))
    {
        CRect rcWindow;
        GetWindowRect(rcWindow);
        m_szMinimum = rcWindow.Size();
    }

    // keep the initial size of the client area as a baseline for moving/sizing controls
    CRect rcClient;
    GetClientRect(rcClient);
    m_szInitial = rcClient.Size();

    // create a gripper in the bottom-right corner
    if (m_bShowGripper && ((GetStyle() & WS_THICKFRAME) != 0))
    {
        SMovingChild s;
        s.m_rcInitial.SetRect(-GetSystemMetrics(SM_CXVSCROLL), -GetSystemMetrics(SM_CYHSCROLL), 0, 0);
        s.m_rcInitial.OffsetRect(rcClient.BottomRight());
        m_hGripper = CreateWindow(_T("Scrollbar"), _T("size"), WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP,
                                  s.m_rcInitial.left, s.m_rcInitial.top, s.m_rcInitial.Width(), s.m_rcInitial.Height(),
                                  m_hWnd, NULL, AfxGetInstanceHandle(), NULL);
        ASSERT(m_hGripper != NULL);
        if (m_hGripper != NULL)
        {
            s.m_hWnd = m_hGripper;
            s.m_dXMoveFrac = 1.0;
            s.m_dYMoveFrac = 1.0;
            s.m_dXSizeFrac = 0.0;
            s.m_dYSizeFrac = 0.0;
            m_MovingChildren.push_back(s);

            // put the gripper first in the z-order so it paints first and doesn't obscure other controls
            ::SetWindowPos(m_hGripper, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
        }
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CResizableDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
    CDialog::OnGetMinMaxInfo(lpMMI);

    if (lpMMI->ptMinTrackSize.x < m_szMinimum.cx)
        lpMMI->ptMinTrackSize.x = m_szMinimum.cx;
    if (lpMMI->ptMinTrackSize.y < m_szMinimum.cy)
        lpMMI->ptMinTrackSize.y = m_szMinimum.cy;
}

void CResizableDialog::OnSize(UINT nType, int cx, int cy) 
{
    CDialog::OnSize(nType, cx, cy);

    int iXDelta = cx - m_szInitial.cx;
    int iYDelta = cy - m_szInitial.cy;
    HDWP hDefer = NULL;
    for (MovingChildren::iterator p = m_MovingChildren.begin();  p != m_MovingChildren.end();  ++p)
    {
        if (p->m_hWnd != NULL)
        {
            CRect rcNew(p->m_rcInitial);
            rcNew.OffsetRect(int(iXDelta * p->m_dXMoveFrac), int(iYDelta * p->m_dYMoveFrac));
            rcNew.right += int(iXDelta * p->m_dXSizeFrac);
            rcNew.bottom += int(iYDelta * p->m_dYSizeFrac);
            if (hDefer == NULL)
                hDefer = BeginDeferWindowPos(m_MovingChildren.size());
            UINT uFlags = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER;
            if ((p->m_dXSizeFrac != 0.0) || (p->m_dYSizeFrac != 0.0))
                uFlags |= SWP_NOCOPYBITS;
            DeferWindowPos(hDefer, p->m_hWnd, NULL, rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height(), uFlags);
        }
    }
    if (hDefer != NULL)
        EndDeferWindowPos(hDefer);

    if (m_hGripper != NULL)
        ::ShowWindow(m_hGripper, (nType == SIZE_MAXIMIZED) ? SW_HIDE : SW_SHOW);
}
