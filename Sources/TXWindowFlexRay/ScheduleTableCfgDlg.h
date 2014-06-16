#pragma once
#include "TxWindowFlexRay_resource.h"
#include "DataTypes\Cluster.h"
#include "Utility\ColumnTreeWnd.h"
#include "CCheckColumnTreeCtrl.h"
class CScheduleTableCfgDlg : public CDialog
{
    DECLARE_DYNAMIC(CScheduleTableCfgDlg)

public:
    CCheckColumnTreeCtrl m_ouCheckTreeCtrl;


    CScheduleTableCfgDlg()
    {
    }
    CScheduleTableCfgDlg(ClusterConfig& pClusterConfig,CWnd* pParent);   // standard constructor
    virtual ~CScheduleTableCfgDlg();
    void nUpdateScheduleView();
    // Dialog Data
    enum { IDD = IDD_DLG_LIN_SCHEDULE_CONFIG };

protected:
    CColumnTreeWnd m_TreeWnd;
    ClusterConfig m_ouClusterConfig;

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

    struct SMovingChild
    {
        HWND        m_hWnd;
        double      m_dXMoveFrac;
        double      m_dYMoveFrac;
        double      m_dXSizeFrac;
        double      m_dYSizeFrac;
        CRect       m_rcInitial;
    };
    typedef std::vector<SMovingChild>   MovingChildren;

    MovingChildren  m_MovingChildren;

    CSize           m_szInitial;
    CSize           m_szMinimum;
    CRect  m_rCrt;

    int    m_nMincX;
    int    m_nMincY;



    CImageList m_omImageList;

    void vSetColumnHeaders();
    void vAdjustColumns();
    void AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct);

};

