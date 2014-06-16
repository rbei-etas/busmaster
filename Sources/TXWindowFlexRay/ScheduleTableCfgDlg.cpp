#include "TxWindowFlexRay_stdafx.h"
#include "ScheduleTableCfgDlg.h"
#include "TxWindowFlexRay_resource.h"
#include "HashDefines.h"
#include "DataTypes\Cluster.h"
#include "Include\XMLDefines.h"
#include "Utility\XMLUtils.h"
#include "LINScheduleDataStore.h"

#define TREE_VIEW_CHECK_STATE_CHANGE (WM_USER + 100)
#define IDC_FILESYS_TREE 99999
#define DEF_COL_SPACE 60
#define DEF_FRAME_COL_WIDTH 210
#define DEF_TYPE_COL_WIDTH 100
#define DEF_DELAY_COL_WIDTH 100

// CTree dialog
IMPLEMENT_DYNAMIC(CScheduleTableCfgDlg, CDialog)

CScheduleTableCfgDlg::CScheduleTableCfgDlg(ClusterConfig& pClusterConfig, CWnd* pParent /*=NULL*/)
    : CDialog(CScheduleTableCfgDlg::IDD,pParent)
{
    m_ouClusterConfig=pClusterConfig;

}

CScheduleTableCfgDlg::~CScheduleTableCfgDlg()
{
}

void CScheduleTableCfgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_TREE_LIN_SCHEDULE, m_pChkBxTree);
}

BEGIN_MESSAGE_MAP(CScheduleTableCfgDlg, CDialog)
    ON_WM_SIZE()
    ON_WM_SHOWWINDOW()
    ON_MESSAGE(IDD_DLG_LIN_SCHEDULE_CONFIG,OnInitDialog)
    ON_BN_CLICKED(IDCLOSE, OnClose)
    ON_WM_LBUTTONDOWN()
    ON_WM_CLOSE()
END_MESSAGE_MAP()


BOOL CScheduleTableCfgDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // use the initial dialog size as the default minimum
    if ((m_szMinimum.cx == 0) && (m_szMinimum.cy == 0))
    {
        CRect rcWindow;
        GetWindowRect(rcWindow);
        m_szMinimum = rcWindow.Size();
    }

    //AutoMove(IDCANCEL, 100.0, 100.0, 0.0, 0.0);
    // keep the initial size of the client area as a baseline for moving/sizing controls

    CRect rcTreeWnd;
    CWnd* pPlaceholderWnd = GetDlgItem(IDC_TREE_LIN_SCHEDULE);
    pPlaceholderWnd->GetWindowRect(&rcTreeWnd);
    ScreenToClient(&rcTreeWnd);

    pPlaceholderWnd->DestroyWindow();
    // create the multi-column tree window
    m_TreeWnd.CreateEx(WS_EX_CLIENTEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE,
                       rcTreeWnd, this, IDC_TREE_LIN_SCHEDULE);
    CRect rect;
    m_TreeWnd.GetTreeCtrl().GetWindowRect(&rect);
    m_ouCheckTreeCtrl.Create(WS_CHILD | WS_VISIBLE | TVS_NOHSCROLL | TVS_NOTOOLTIPS,  rect, this, 1);
    m_ouCheckTreeCtrl.SubclassWindow(m_TreeWnd.GetTreeCtrl().GetSafeHwnd());

    DWORD dwStyle = GetWindowLong(m_ouCheckTreeCtrl, GWL_STYLE);
    dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_FULLROWSELECT | TVS_DISABLEDRAGDROP;
    SetWindowLong(m_ouCheckTreeCtrl, GWL_STYLE, dwStyle);
    /* Remove the style */
    m_ouCheckTreeCtrl.ModifyStyle( TVS_CHECKBOXES, 0 );
    /* Now explicitly set it */
    m_ouCheckTreeCtrl.ModifyStyle( 0, TVS_CHECKBOXES );

    if( NULL == m_ouCheckTreeCtrl )
    {
        return FALSE;
    }

    // 1. Set tree column headers
    vSetColumnHeaders();

    m_ouCheckTreeCtrl.nUpdateScheduleView();
    m_ouCheckTreeCtrl.Expand(m_ouCheckTreeCtrl.GetRootItem(), TVM_EXPAND);

    // keep the initial size of the client area as a baseline for moving/sizing controls
    CRect rcClient;
    GetClientRect(rcClient);
    m_szInitial = rcClient.Size();
    AutoMove(IDC_TREE_LIN_SCHEDULE, 0, 0, 100.0, 100.0);
    AutoMove(IDCLOSE, 100.0, 100.0, 0.0, 0.0);
    return TRUE;
}

void CScheduleTableCfgDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonDown(nFlags, point);
}

void CScheduleTableCfgDlg::vAdjustColumns()
{

    HDITEM hditem;
    CHeaderCtrl& header = m_TreeWnd.GetHeaderCtrl();
    hditem.mask =  HDI_WIDTH ;
    hditem.fmt = HDF_LEFT | HDF_STRING;

    CRect rect;
    m_TreeWnd.GetTreeCtrl().GetWindowRect(&rect);

    //command Name
    hditem.cxy = rect.Width() * 0.4;
    hditem.pszText = defSTR_FRAME_NAME;
    header.SetItem(0, &hditem);

    //Id
    hditem.cxy = rect.Width() * 0.2;
    hditem.pszText = defSTR_FRAME_ID;
    header.SetItem(1, &hditem);

    //Column Width
    hditem.cxy = rect.Width() * 0.3;
    hditem.pszText = defSTR_FRAME_TYPE;
    header.SetItem(2, &hditem);

    //Delay
    hditem.cxy = rect.Width() * 0.2;
    hditem.pszText = defSTR_DELAY;
    header.SetItem(3, &hditem);
    header.RedrawWindow();
}

void CScheduleTableCfgDlg::vSetColumnHeaders()
{
    CHeaderCtrl& header = m_TreeWnd.GetHeaderCtrl();

    HDITEM hditem;
    hditem.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
    hditem.fmt = HDF_LEFT | HDF_STRING;

    CRect rect;
    m_TreeWnd.GetWindowRect(&rect);

    //command Name
    hditem.cxy = rect.Width()/4;
    hditem.pszText = defSTR_FRAME_NAME;
    header.InsertItem(0, &hditem);

    //Id
    hditem.cxy = rect.Width()/4;
    hditem.pszText = defSTR_FRAME_ID;
    header.InsertItem(1, &hditem);

    //Column Width
    hditem.cxy = rect.Width()/4;
    hditem.pszText = defSTR_FRAME_TYPE;
    header.InsertItem(2, &hditem);

    //Delay
    hditem.cxy = rect.Width()/4;
    hditem.pszText = defSTR_DELAY;
    header.InsertItem(3, &hditem);



    m_TreeWnd.UpdateColumns();
    //m_TreeWnd.RedrawWindow();
}

void CScheduleTableCfgDlg::vSaveWindowPos()
{
    WINDOWPLACEMENT schedDlgPlacement;
    GetWindowPlacement(&schedDlgPlacement);

    CLINScheduleDataStore::pGetLINSchedDataStore().vSetWindowPlacement(schedDlgPlacement);
}



void CScheduleTableCfgDlg::OnClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);

    *pResult = 0;
}

void CScheduleTableCfgDlg::AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct)
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

void CScheduleTableCfgDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CDialog::OnGetMinMaxInfo(lpMMI);

    if (lpMMI->ptMinTrackSize.x < m_szMinimum.cx)
    {
        lpMMI->ptMinTrackSize.x = m_szMinimum.cx;
    }
    if (lpMMI->ptMinTrackSize.y < m_szMinimum.cy)
    {
        lpMMI->ptMinTrackSize.y = m_szMinimum.cy;
    }
}

void CScheduleTableCfgDlg::OnSize(UINT nType, int cx, int cy)
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
            {
                hDefer = BeginDeferWindowPos(m_MovingChildren.size());
            }
            UINT uFlags = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER;
            if ((p->m_dXSizeFrac != 0.0) || (p->m_dYSizeFrac != 0.0))
            {
                uFlags |= SWP_NOCOPYBITS;
            }
            DeferWindowPos(hDefer, p->m_hWnd, NULL, rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height(), uFlags);
        }
    }
    if (hDefer != NULL)
    {
        EndDeferWindowPos(hDefer);
    }
    vAdjustColumns();
}

void CScheduleTableCfgDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);

    if (bShow == TRUE )
    {
        SendMessage(WM_NCPAINT, 1, 0);
    }
}

void CScheduleTableCfgDlg::OnClose()
{
    // Call Show Window to save co ordinates
    this->ShowWindow(SW_HIDE);
}


void CScheduleTableCfgDlg::nUpdateScheduleView()
{
    m_ouCheckTreeCtrl.nUpdateScheduleView();
}

