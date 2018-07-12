#include "stdafx.h"
#include "ScheduleTableCfgDlg.h"
#include "resource.h"
#include "HashDefines.h"
#include "ICluster.h"
#include "Include\XMLDefines.h"
#include "Utility\XMLUtils.h"
#include "LINScheduleDataStore.h"


// CTree dialog
IMPLEMENT_DYNAMIC(CScheduleTableCfgDlg, CDialog)

CScheduleTableCfgDlg::CScheduleTableCfgDlg(IBMNetWorkGetService* pClusterConfig, CWnd* pParent /*=NULL*/)
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
    //ON_MESSAGE(IDD_DLG_LIN_SCHEDULE_CONFIG,OnInitDialog)
    ON_BN_CLICKED(IDCLOSE, OnClose)
    ON_WM_LBUTTONDOWN()
    ON_WM_CLOSE()
END_MESSAGE_MAP()


BOOL CScheduleTableCfgDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // use the initial dialog size as the default minimum
    if ((m_AllowedSize.cx == 0) && (m_AllowedSize.cy == 0))
    {
        CRect rcWindow;
        GetWindowRect(rcWindow);
        m_AllowedSize = rcWindow.Size();
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
    m_DefaultSize = rcClient.Size();
    vMoveWindowPos(IDC_TREE_LIN_SCHEDULE, 0, 0, 100.0, 100.0);
    vMoveWindowPos(IDCLOSE, 100.0, 100.0, 0.0, 0.0);
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

	auto columnWidth = rect.Width() - 5;
    //command Name
    hditem.cxy = columnWidth * 0.4;
    hditem.pszText = defSTR_FRAME_NAME;
    header.SetItem(0, &hditem);

    //Id
    hditem.cxy = columnWidth * 0.2;
    hditem.pszText = defSTR_FRAME_ID;
    header.SetItem(1, &hditem);

    //Column Width
    hditem.cxy = columnWidth * 0.2;
    hditem.pszText = defSTR_FRAME_TYPE;
    header.SetItem(2, &hditem);

    //Delay
    hditem.cxy = columnWidth * 0.2;
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

	auto columnWidth = static_cast<int>( (rect.Width() - 10 ) / 4);
    //command Name
	hditem.cxy = columnWidth;
    hditem.pszText = defSTR_FRAME_NAME;
    header.InsertItem(0, &hditem);

    //Id
	hditem.cxy = columnWidth;
    hditem.pszText = defSTR_FRAME_ID;
    header.InsertItem(1, &hditem);

    //Column Width
	hditem.cxy = columnWidth;
    hditem.pszText = defSTR_FRAME_TYPE;
    header.InsertItem(2, &hditem);

    //Delay
	hditem.cxy = columnWidth;
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

void CScheduleTableCfgDlg::vGetAutoResizeValues(double fSizeX, double fSizeY,double fToXPos, double fToYPos,_AUTORESIZE& autoResize)
{
    autoResize.fToXPos = fToXPos / 100.0;
    autoResize.fToYPos = fToYPos / 100.0;
    autoResize.fSizeX = fSizeX / 100.0;
    autoResize.fSizeY = fSizeY / 100.0;
}

void CScheduleTableCfgDlg::vMoveWindowPos(int nResourceId, double fToXPos, double fToYPos, double fSizeX, double fSizeY)
{
    double dStandard = 100.0;
    _AUTORESIZE autoResize;
    GetDlgItem(nResourceId, &autoResize.wndHandle);
    if(autoResize.wndHandle != nullptr)
    {
        vGetAutoResizeValues(fSizeX, fSizeY, fToXPos, fToYPos, autoResize);
        ::GetWindowRect(autoResize.wndHandle, &autoResize.wndRect);
        ScreenToClient(autoResize.wndRect);
        m_VecAutoSize.push_back(autoResize);
    }
}

void CScheduleTableCfgDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CDialog::OnGetMinMaxInfo(lpMMI);

    if (lpMMI->ptMinTrackSize.x < m_AllowedSize.cx)
    {
        lpMMI->ptMinTrackSize.x = m_AllowedSize.cx;
    }
    if (lpMMI->ptMinTrackSize.y < m_AllowedSize.cy)
    {
        lpMMI->ptMinTrackSize.y = m_AllowedSize.cy;
    }
}

UINT CScheduleTableCfgDlg::vChangeWindowPos(HDWP& handleWP, HWND /*wndHandle*/, double fSizeX, double fSizeY,CRect /*rectWndPos*/)
{
    if (handleWP == NULL)
    {
        handleWP = BeginDeferWindowPos(m_VecAutoSize.size());
    }
    UINT uFlags = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    if ((fSizeX != 0.0) || (fSizeY != 0.0))
    {
        uFlags |= SWP_NOCOPYBITS;
    }

    return uFlags;
}

HDWP CScheduleTableCfgDlg::vUpdateWindowPosition(int nXPos, int nYPos)
{
    HDWP handleWP = NULL;

    for (VecAutoSize::iterator itr = m_VecAutoSize.begin();  itr != m_VecAutoSize.end();  ++itr)
    {
        if (itr->wndHandle != NULL)
        {
            CRect rectWndPos(itr->wndRect);
            int nCalXPos = int(nXPos * itr->fToXPos);
            int nCalYPos = int(nYPos * itr->fToYPos);

            int nCalRightPos = int(nXPos * itr->fSizeX);
            int nCalBottomPos = int(nYPos * itr->fSizeY);

            rectWndPos.OffsetRect(nCalXPos, nCalYPos);
            rectWndPos.right = rectWndPos.right + nCalRightPos;
            rectWndPos.bottom = rectWndPos.bottom + nCalBottomPos;
            UINT uFlags = vChangeWindowPos(handleWP, itr->wndHandle, itr->fSizeX, itr->fSizeY, rectWndPos);

            DeferWindowPos(handleWP, itr->wndHandle, NULL, rectWndPos.left, rectWndPos.top, rectWndPos.Width(), rectWndPos.Height(), uFlags);
        }
    }

    return handleWP;
}

void CScheduleTableCfgDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    int nXPos = cx - m_DefaultSize.cx;
    int nYPos = cy - m_DefaultSize.cy;
    HDWP hDefer = NULL;

    hDefer = vUpdateWindowPosition(nXPos, nYPos);

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

