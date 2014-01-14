/**********************************************************************************
    Project         :   Frame
    FileName        :   TxMsgList.h
    Description     :   Definition of CTxMsgList class
    Directory       :
    Version         :   V1.0
    Authors         :   Ravi D
    Date Created    :   19/09/2006
    Date Modified   :
    Modifications   :
**********************************************************************************/

#pragma once
#include "Utility/flickerfreelistctrl.h"
#include <map>
using namespace std;
//#include ".\Customization\CHexEdit\HexEdit.h"
//#include ".\Customization\CCustomLabel\CustomLabel.h"

//#include ".\Application\DisplayMatrix\DisplayMatrixDlg.h"

//#include "FRAME_BaseUIDoc.h"
//#include "Mainfrm.h"

// Sent to parent window when an item text editing was committed
// wParam: The item index
// lParam: The column index
#define WM_EDIT_COMMITTED   (WM_APP + 10002)

//class CHexEdit;
//class CCustomLabel;

struct ColumnProperty
{
    bool bMovable;
};

class CTxMsgList :
    public CFlickerFreeListCtrl
{
public:
    CTxMsgList(void);
    virtual ~CTxMsgList(void);
    virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
    DECLARE_MESSAGE_MAP()
    afx_msg void OnHdnBegintrack(NMHDR* pNMHDR, LRESULT* pResult);
public:
    BOOL StartEdit(int nItem, int nSubItem);
    BOOL EndEdit(BOOL bCommit = TRUE);
    //CHexEdit* GetEditControl(void);
    //CHexEdit* m_pWndEdit; // Edit control, for subitem edit
    CEdit* GetEditControl(void);
    CEdit* m_pWndEdit;  // Edit control, for subitem edit
    // Is subitem edit allowed?
    BOOL m_bAllowEdit;
    // Format of previously edited subitem
    DWORD m_dwPrevEditFmt;
    // Position of the subitem that is currently being edited
    POINT m_ptEditting;
    DWORD _GetHeaderTextFormat(int nColumn) const;
    BOOL _IsEditVisible(void)   const;
    BOOL _IsValidIndex(int nIndex) const;
    void _UnsetSortedColumn(void);
    // The sorted column, -1 if none
    int m_nSortCol;
    // Is sort ascending?
    BOOL m_bSortAscending;
    // FALSE if the list control has LVS_NOCOLUMNHEADER flag
    BOOL HasColumnHeader(void) const;
    int GetItemImage(int nItem, int nSubItem) const;
    int GetColumnCount(void) const;
    CHeaderCtrl* GetHeaderCtrl() const;
    CImageList* GetImageList() const;
    BOOL IsEditable() const; // Is Item text editable?
    void SetEditable(BOOL bSet = TRUE); // Allow item text editting
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
    CComboBox* ShowCombo(int nItem, int nSubItem, const CStringArray& omList);
    int CalcHorzExtent(CWnd* pWnd, CStringList* pList);
    afx_msg void OnLvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
    BOOL        m_bSelected;

    CPoint  m_ptSubItemPoint;
    CFont m_fntText;
public:
    int InsertColumn(int nCol, const LVCOLUMN* pColumn, ColumnProperty ouColumnProperty );
    bool GetTextFont( LOGFONT* plgfnt);
    bool SetFontBold(bool bBold);
    bool SetTextFont( const LOGFONT& lgfnt );
    bool SetFontFaceName(const CString& sFaceName);
    //IProjectCfg_MsgDispPtr m_pProjCfgMsgDisp;
    afx_msg void MeasureItem (LPMEASUREITEMSTRUCT lpMeasureItemStruct);

private:
    bool bIsChangeable(int nItem, int nSubItem);
    void vDataFormat(CString& omDataBytes,int* nDataLength,bool bFlag);
    bool bValidateDynamicLength(long lValue);

    map<int, ColumnProperty> m_ouColumnPropMap;
};
