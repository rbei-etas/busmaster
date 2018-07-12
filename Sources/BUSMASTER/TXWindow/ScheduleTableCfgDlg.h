#pragma once
#include "resource.h"
#include "ICluster.h"
#include "IBMNetWorkGetService.h"
#include "Utility\ColumnTreeWnd.h"
#include "CCheckColumnTreeCtrl.h"
#include "Utility\Utility.h"
class CScheduleTableCfgDlg : public CDialog
{
    DECLARE_DYNAMIC(CScheduleTableCfgDlg)

public:
    CCheckColumnTreeCtrl m_ouCheckTreeCtrl;


    CScheduleTableCfgDlg()
    {
    }
    CScheduleTableCfgDlg(IBMNetWorkGetService* pClusterConfig,CWnd* pParent);   // standard constructor
    virtual ~CScheduleTableCfgDlg();
    void nUpdateScheduleView();
    // Dialog Data
    enum { IDD = IDD_DLG_LIN_SCHEDULE_CONFIG };

protected:
    CColumnTreeWnd m_TreeWnd;
    IBMNetWorkGetService* m_ouClusterConfig;

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnClose();
    afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    void OnLButtonDown(UINT nFlags, CPoint point);
    void vSaveWindowPos();


    DECLARE_MESSAGE_MAP()

private:

    void vGetAutoResizeValues(double fSizeX, double fSizeY,double fToXPos, double fToYPos,_AUTORESIZE& autoResize);

    VecAutoSize  m_VecAutoSize;

    CSize           m_DefaultSize;// m_szInitial;
    CSize           m_AllowedSize;// m_szMinimum;
    CRect  m_rCrt;

    int    m_nMincX;
    int    m_nMincY;

    CImageList m_omImageList;

    void vSetColumnHeaders();
    void vAdjustColumns();
    void vMoveWindowPos(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct);
    HDWP vUpdateWindowPosition(int nXPos, int nYPos);
    UINT vChangeWindowPos(HDWP& handleWP, HWND wndHandle, double fSizeX, double fSizeY,CRect rectWndPos);
};

