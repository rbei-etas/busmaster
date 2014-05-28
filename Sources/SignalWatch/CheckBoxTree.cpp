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
    if(hItem == nullptr)
    {
        return FALSE;
    }

    return GetItemState( hItem, TVIS_STATEIMAGEMASK )>>12 == 2;
}

void CCheckBoxTree::OnLButtonDown(UINT nFlags, CPoint point)
{
    UINT nHitTestFlags = 0;
    HTREEITEM hItem = HitTest(point, &nHitTestFlags);
    //MessageBox( "BEFORE IF", "IF1", MB_OK);
    if((hItem != nullptr) && (TVHT_ONITEMSTATEICON & nHitTestFlags))
    {
        //MessageBox( "INSIDE IF", "IF1", MB_OK);
        //BOOL bCheck= GetCheck(hItem);
        BOOL bCheck = bIsItemChecked(hItem);

        //vSetCheck(hItem,!bCheck);

        vSetCheckChildren(hItem,!bCheck);


        if(!bCheck)
        {
            HTREEITEM root,parent;

            root=GetRootItem();
            SetCheck(root);

            parent=GetParentItem(hItem);
            SetCheck(parent);
        }

        if(bCheck)
        {

            HTREEITEM root,parent,child;
            root=GetRootItem();
            SetCheck(root,0);

            /*parent=GetParentItem(hItem);

            SetCheck(parent,0);*/
            while(ItemHasChildren(root))
            {
                //MessageBox("wHILE1", "WHILE1", MB_OK);
                HTREEITEM parent=GetChildItem(root);
                while(ItemHasChildren(parent))
                {
                    if(GetCheck(parent))
                    {
                        SetCheck(parent,0);
                        child=GetChildItem(parent);
                        //MessageBox( "wHILE2", "WHILE2", MB_OK);
                        while(child)
                        {
                            if(GetCheck(child))
                            {

                                SetCheck(root);
                                SetCheck(parent);
                            }
                            child=GetNextSiblingItem(child);
                            if ( child == nullptr )
                            {
                                break;
                            }
                        }
                    }
                    parent=GetNextSiblingItem(parent);
                    if ( parent == nullptr )
                    {
                        break;
                    }

                }

                parent=GetNextSiblingItem(parent);
                if ( parent == nullptr )
                {
                    break;
                }

            }
        }
        //return;
    }

    else
    {
        //MessageBox( "INSIDE ELSE", "ELSE", MB_OK);
        CTreeCtrl::OnLButtonDown(nFlags, point);
    }
}




void CCheckBoxTree::vSetCheck(HTREEITEM hItem, BOOL bCheck)
{
    if(hItem == nullptr)
    {
        return;
    }
    if (hItem != nullptr)
    {
        int nState = (bCheck == TRUE) ? 2 : 1;
        SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
        GetItemData(hItem);
    }
    return;
}


void CCheckBoxTree::vSetCheckParent(HTREEITEM hItem)
{
    if(hItem == nullptr)
    {
        return;
    }

    HTREEITEM hParentItem = GetParentItem(hItem);
    HTREEITEM hChildItem;
    BOOL bAllChecked = TRUE;
    if( ItemHasChildren(hParentItem))
    {
        hChildItem = GetChildItem(hParentItem);
        while(hChildItem)
        {
            if(!bIsItemChecked(hChildItem))
            {
                bAllChecked = FALSE;
                break;
            }
            hChildItem = GetNextSiblingItem(hChildItem);
        }
    }
    BOOL fCheck=0;
    vSetCheck(hParentItem, bAllChecked);
    vSetCheckParent(hParentItem);
    vSetCheckChildren(hParentItem,!fCheck);
    return;
}

void CCheckBoxTree::vSetCheckChildren(HTREEITEM hItem, BOOL fCheck)
{
    if(hItem == nullptr)
    {
        return;
    }



    vSetCheck(hItem, fCheck);

    // loop to set item state for children
    if ( ItemHasChildren(hItem))
    {
        HTREEITEM htiChild = GetChildItem (hItem);
        while (htiChild != nullptr)
        {
            vSetCheck(htiChild, fCheck);
            vSetCheckChildren(htiChild, fCheck);
            htiChild = GetNextSiblingItem(htiChild);
        }
    }


    return;
}








