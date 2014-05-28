// FlexrayMsgHandlerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <map>

#include "TxWindowFlexRay_resource.h"
#include "..\DataTypes\FLEXRAY_Datatypes.h"

class CFlxTxMsgBuffer;

class CFlexrayMsgHandlerDlg : public CDialog
{
private:

    CFlxTxMsgBuffer* m_pouTxMsgList;
    void vRefreshMsgList(void);
    INT m_nCurrentChannel;
    CImageList m_omHeaderImageList;
    void vSetMessageCheckValue( BOOL bToBeChecked );
    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    std::map<long, int> m_pSelectedItem;
    CListCtrl m_lstcMsgList;
    // Dialog Data
    enum { IDD = IDD_FLEXRAYMSGHANDLER_DIALOG };

    CFlexrayMsgHandlerDlg(INT nChannel = 1, CWnd* pParent = nullptr);
    afx_msg void OnBnClickedCbtnSelectmsg();
    afx_msg void OnBnClickedCbtnCancelmsgselection();
    afx_msg void OnColumnclickLstcMsgDetails(NMHDR* pNMHDR, LRESULT* pResult);
};
