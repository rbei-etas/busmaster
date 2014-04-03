#pragma once
#include "SignalWatch_resource.h"
#include "DataTypes\Cluster.h"
#include "CheckBoxTree.h"
// CTree dialog

class CMsgSignalSelect : public CDialog
{
    DECLARE_DYNAMIC(CMsgSignalSelect)
    CCheckBoxTree m_omMsgSignalTree;

public:
    CMsgSignalSelect(ClusterConfig* clusterinfo,CWnd* pParent,map<int,list<string>>* m_mapMsgIDtoSignallst);   // standard constructor
    virtual ~CMsgSignalSelect();
    virtual BOOL OnInitDialog();
    void OnLButtonDown(UINT nFlags, CPoint point);
    void SetAllChildren(HTREEITEM Parent);
    map<int,list<string>>* m_mapMsgIDtoSignallst; // Map of Msg ID, list of signals
    void vStoreintoMap();
    void vRestoreData();
    afx_msg void OnBnClickedOk();
    CImageList m_omImageList;
    CImageList m_omImageList1;
    // Dialog Data
    enum { IDD = IDD_DLG_SIGNAL_WATCH_LIN };

    void vTest();
    afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    ClusterConfig m_ouCluster;
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
    void AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct);

    BOOL bCreateImageList();
    BOOL bCreateMsgImageList();
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


    CTreeCtrl* pTreeStruct;


};

