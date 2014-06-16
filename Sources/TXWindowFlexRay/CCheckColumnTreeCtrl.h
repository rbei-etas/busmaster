#include "Utility\ColumnTreeCtrl.h"
#include "DataTypes\Cluster.h"
class CCheckColumnTreeCtrl : public CColumnTreeCtrl
{
    DECLARE_DYNAMIC(CCheckColumnTreeCtrl)
    CImageList m_omImageList;
public:
    CCheckColumnTreeCtrl();
    virtual ~CCheckColumnTreeCtrl();

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL bIsItemChecked(HTREEITEM hItem);
    void OnLButtonDown(UINT nFlags, CPoint point);

public:
    int nUpdateScheduleView();
    void OnSize(UINT nType, int cx, int cy);

private:
    void vOnItemClick(HTREEITEM hItem);
    void vHandleScheduleTableClick(HTREEITEM hItem);
    void vHandleCommandItemClick(HTREEITEM hItem);

    //void vEnableCommandItem( int nChannelIndex, int nTableIndex, int nItemIndex, bool bEnable );

    BOOL bCreateImageList();
    INT nGetFrameType(eCommandType eCmdType);

    INT nEnableScheduleCommand(HTREEITEM hItem, bool bEnable);

    void EnableChildItems(HTREEITEM hItem, bool bEnable);
    int nGetItemDetails(HTREEITEM hCommandItem, int& nChannelIndex, int& nTableIndex, int& nItemIndex);
    int nGetTableDetails(HTREEITEM hTableItem, int& nChannelIndex, int& nTableIndex);
};