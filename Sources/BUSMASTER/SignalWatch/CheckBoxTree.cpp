// CheckBoxTree.cpp : implementation file
//

#include "SignalWatch_stdafx.h"
#include "CheckBoxTree.h"

// CCheckBoxTree

IMPLEMENT_DYNAMIC(CCheckBoxTree, CTreeCtrl)

CCheckBoxTree::CCheckBoxTree()
{
}

CCheckBoxTree::~CCheckBoxTree()
{
}

BEGIN_MESSAGE_MAP(CCheckBoxTree, CTreeCtrl)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CCheckBoxTree::bIsItemChecked(HTREEITEM hItem)
{
    if (hItem == nullptr)
    {
        return FALSE;
    }

    return GetItemState(hItem, TVIS_STATEIMAGEMASK) >> 12 == 2;
}

void CCheckBoxTree::OnLButtonDown(UINT nFlags, CPoint point)
{
    UINT nHitTestFlags = 0;
    HTREEITEM hItem = HitTest(point, &nHitTestFlags);

    if ((hItem != nullptr) && (TVHT_ONITEMSTATEICON & nHitTestFlags))
    {
        BOOL bCheck = bIsItemChecked(hItem);
        HTREEITEM hRoot = GetRootItem();

        //Toggle the clicked item.
        SetCheck(hItem, !bCheck);

        //Toggle the all the children item to the state of the current item
        vSetCheckChildren(hItem, !bCheck);

        //If Unchecked -> Checked State.
        if (!bCheck)
        {
            //Set all the Unchecked Ancestors to Checked State
            HTREEITEM hParent = GetParentItem(hItem);
            while (hParent != nullptr && !GetCheck(hParent))
            {
                vSetCheck(hParent, !bCheck);
                hParent = GetParentItem(hParent);
            }
        }

        //If Checked -> Unchecked State.
        else if (bCheck)
        {
            /*
            Logic
            1. Iterate through the Siblings of the current Node -> if None is checked then, Uncheck the Parent Node.
            2. Iterate through the Siblings of the Parent Node -> if None is checked then, Uncheck the Grandparent Node.
            3. Repeat the Same till the last ancestor.
            */

            HTREEITEM hParent = GetParentItem(hItem);
            while (hParent != nullptr && GetCheck(hParent))
            {
                HTREEITEM hChild = GetChildItem(hParent);
                bool bIsAnySiblingChecked = false;

                do
                {
                    if (GetCheck(hChild))
                    {
                        bIsAnySiblingChecked = true;
                        break;
                    }

                    hChild = GetNextSiblingItem(hChild);
                }
                while (hChild != nullptr);

                //If None of the Siblings of the selected Node is Checked then Uncheck  its Parent Node.
                if (!bIsAnySiblingChecked)
                {
                    vSetCheck(hParent, !bCheck);
                }

                //Find the Parent of the Parent Node. (This helps to tranverse all the ancestors of the Node)
                hParent = GetParentItem(hParent);
            }
        }
    }

    else
    {
        CTreeCtrl::OnLButtonDown(nFlags, point);
    }
}

void CCheckBoxTree::vSetCheck(HTREEITEM hItem, BOOL bCheck)
{
    if (hItem == nullptr)
    {
        return;
    }

    int nState = (bCheck == TRUE) ? 2 : 1;
    SetItemState(hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
    GetItemData(hItem);
}

void CCheckBoxTree::vSetCheckParent(HTREEITEM hItem)
{
    if (hItem == nullptr)
    {
        return;
    }

    HTREEITEM hParentItem = GetParentItem(hItem);
    HTREEITEM hChildItem;
    BOOL bAllChecked = TRUE;
    if (ItemHasChildren(hParentItem))
    {
        hChildItem = GetChildItem(hParentItem);
        while (hChildItem)
        {
            if (!bIsItemChecked(hChildItem))
            {
                bAllChecked = FALSE;
                break;
            }
            hChildItem = GetNextSiblingItem(hChildItem);
        }
    }
    BOOL fCheck = 0;
    vSetCheck(hParentItem, bAllChecked);
    vSetCheckParent(hParentItem);
    vSetCheckChildren(hParentItem, !fCheck);
}

void CCheckBoxTree::vSetCheckChildren(HTREEITEM hItem, BOOL fCheck)
{
    if (hItem == nullptr)
    {
        return;
    }

    vSetCheck(hItem, fCheck);

    // loop to set item state for children
    if (ItemHasChildren(hItem))
    {
        HTREEITEM htiChild = GetChildItem(hItem);
        while (htiChild != nullptr)
        {
            vSetCheck(htiChild, fCheck);
            vSetCheckChildren(htiChild, fCheck);
            htiChild = GetNextSiblingItem(htiChild);
        }
    }
}