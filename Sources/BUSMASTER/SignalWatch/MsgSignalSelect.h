#pragma once
#include "SignalWatch_resource.h"
#include "ICluster.h"
#include "IBMNetWorkGetService.h"
#include "CheckBoxTree.h"
#include "Utility\Utility.h"

// CTree dialog

class CMsgSignalSelect : public CDialog
{
    DECLARE_DYNAMIC(CMsgSignalSelect)
    CCheckBoxTree m_omMsgSignalTree;
    CRITICAL_SECTION& m_omCritSecSW;
    ETYPE_BUS m_ouEbus;
public:
    CMsgSignalSelect(IBMNetWorkGetService* clusterinfo, ETYPE_BUS ouEbus, CWnd* pParent, std::map<long, std::list<std::string>>* m_mapMsgIDtoSignallst, CRITICAL_SECTION& m_omCritSecSW);   // standard constructor
    virtual ~CMsgSignalSelect();
    virtual BOOL OnInitDialog();
    void OnLButtonDown(UINT nFlags, CPoint point);
    void SetAllChildren(HTREEITEM Parent);
    std::map<long, std::list<std::string>>* m_mapMsgIDtoSignallst;  // Map of Msg ID, list of signals

    void vStoreintoMap();
    void vRestoreData();
    afx_msg void OnBnClickedOk();
    CImageList m_omImageList;
    CImageList m_omImageList1;
    // Dialog Data
    enum { IDD = IDD_DLG_SIGNAL_WATCH_CONFIGURE };

    void vTest();
    afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    IBMNetWorkGetService* m_ouCluster;
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
    void AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct);

    BOOL bCreateImageList();
    BOOL bCreateMsgImageList();

    void vGetAutoResizeValues(double fSizeX, double fSizeY,double fToXPos, double fToYPos,_AUTORESIZE& autoResize);
    void vMoveWindowPos(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct);
    HDWP vUpdateWindowPosition(int nXPos, int nYPos);
    UINT vChangeWindowPos(HDWP& handleWP, HWND wndHandle, double fSizeX, double fSizeY,CRect rectWndPos);

    VecAutoSize  m_VecAutoSize;

    CSize           m_DefaultSize;
    CSize           m_AllowedSize;


    CTreeCtrl* pTreeStruct;


};

