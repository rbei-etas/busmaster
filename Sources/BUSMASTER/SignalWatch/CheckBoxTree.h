#pragma once


// CCheckBoxTree

class CCheckBoxTree : public CTreeCtrl
{
    DECLARE_DYNAMIC(CCheckBoxTree)

public:
    CCheckBoxTree();
    virtual ~CCheckBoxTree();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void vSetCheck(HTREEITEM hItem, BOOL bCheck);
    afx_msg void vSetCheckChildren(HTREEITEM hItem, BOOL fCheck);
    afx_msg BOOL bIsItemChecked(HTREEITEM hItem);
    afx_msg void vSetCheckParent(HTREEITEM hItem);



protected:
    DECLARE_MESSAGE_MAP()
};


