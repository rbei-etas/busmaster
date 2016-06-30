
#include "stdafx.h"
#include "CCheckColumnTreeCtrl.h"
#include "resource.h"
#include "Application\HashDefines.h"
#include <string>
#include "iosfwd"
#include "LINScheduleDataStore.h"

using namespace std;

IMPLEMENT_DYNAMIC(CCheckColumnTreeCtrl, CColumnTreeCtrl)

BEGIN_MESSAGE_MAP(CCheckColumnTreeCtrl, CColumnTreeCtrl)
    ON_WM_KEYDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_SIZE()
END_MESSAGE_MAP()

#define ITEM_TYPE_CHANNEL           0
#define ITEM_TYPE_SCHEDULETABLE     1
#define ITEM_TYPE_MASTER_COMMAND    2


union sTreeData
{
    DWORD m_dwTreeData;
    struct
    {
        unsigned char m_uchItemType;
        unsigned char m_uchItemIndex;
        unsigned char m_uchItemSubType;
        unsigned char m_uchUnused;
    } m_unionData;
};

CCheckColumnTreeCtrl::CCheckColumnTreeCtrl()
{
    bCreateImageList();

}

CCheckColumnTreeCtrl::~CCheckColumnTreeCtrl()
{
}

void CCheckColumnTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
    CTreeCtrl::OnSize(nType, cx, cy);
}

BOOL CCheckColumnTreeCtrl::bCreateImageList()
{
    // Flag to indicate result
    BOOL bReturn = FALSE;
    // Create Image List
    if( m_omImageList.Create( IDR_BMP_MSG, defICON_SIZE,
                              defICON_GROW, defCOLOR_WHITE ) == TRUE )
    {
        // If successful set the result
        bReturn = TRUE;

    }
    // Return the result
    return bReturn;
}
int CCheckColumnTreeCtrl::nUpdateScheduleView()
{
    DeleteAllItems();
    SetImageList(&m_omImageList, TVSIL_NORMAL);

    SCheduleTableData* ouTableData = CLINScheduleDataStore::pGetLINSchedDataStore().m_ouTableData;
    IBMNetWorkGetService* ouClusterConfig = CLINScheduleDataStore::pGetLINSchedDataStore().pGetClusterConfig();
    list<CSheduleTable>::iterator itrSchdTable;
    HTREEITEM hItemChannel = NULL,  hItemSchedule = NULL, hItemFrame = NULL;
    char chChannelName[255];

    // Add Channel entry in to tree
    sTreeData ouTreeItemData;
    int nChannels = 0;

    ouClusterConfig->GetChannelCount(LIN, nChannels);
    for(INT nIndex = 0; nIndex < nChannels; nIndex++)
    {

        ouTreeItemData.m_unionData.m_uchItemType = ITEM_TYPE_CHANNEL;
        ouTreeItemData.m_unionData.m_uchItemIndex = nIndex;

        sprintf_s(chChannelName, "Channel %d", nIndex+1);

        // Insert channel
        hItemChannel = InsertItem(chChannelName, NULL, NULL);
        SetItemData(hItemChannel,  ouTreeItemData.m_dwTreeData);
        SetItemImage(hItemChannel, 1, 1);
        SetCheck(hItemChannel, true );
        itrSchdTable = ouTableData[nIndex].m_ouScheduleTables.begin();

        // Add Schedule tables in to tree
        int nTableIndex = 0;
        while(itrSchdTable != ouTableData[nIndex].m_ouScheduleTables.end())
        {
            hItemSchedule = InsertItem(itrSchdTable->m_strTableName.c_str(),hItemChannel, NULL);

            ouTreeItemData.m_unionData.m_uchItemType = ITEM_TYPE_SCHEDULETABLE;
            ouTreeItemData.m_unionData.m_uchItemIndex = nTableIndex++;

            SetItemData(hItemSchedule, ouTreeItemData.m_dwTreeData);
            SetItemImage(hItemSchedule, 2, 2);

            if ( itrSchdTable->m_strTableName == ouTableData[nIndex].m_strCurrentTable )
            {
                SetCheck(hItemSchedule, true);
            }

            list<CScheduleCommands>::iterator itrFramelist = itrSchdTable->m_listCommands.begin();

            // Add Frames
            char chFrameName[512] = {0};

            string strCmdType = "";
            string strName = "";
            double fDelay= itrFramelist->m_fDelay ;
            eCommandType eCmdType = itrFramelist->m_eCommandType;

            int nCommandIndex = 0;
            while(itrFramelist != itrSchdTable->m_listCommands.end())
            {
                ouTreeItemData.m_unionData.m_uchItemType = ITEM_TYPE_MASTER_COMMAND;
                ouTreeItemData.m_unionData.m_uchItemIndex = nCommandIndex++;

                strName = itrFramelist->m_strCommandName.c_str();
                if ( itrFramelist->m_nId == 0x3C )
                {
                    strCmdType = m_omCommandType[COMMAND_MASTERREQ];
                    eCmdType = COMMAND_MASTERREQ;

                }
                else if ( itrFramelist->m_nId == 0x3D )
                {
                    strCmdType = m_omCommandType[COMMAND_SLAVE];
                    eCmdType = COMMAND_SLAVE;
                }
                else
                {
                    strCmdType = m_omCommandType[itrFramelist->m_eCommandType];
                    eCmdType = itrFramelist->m_eCommandType;
                }

                sprintf_s(chFrameName, "%s\t0x%X\t%s\t%0.f", strName.c_str(), itrFramelist->m_nId,
                          strCmdType.c_str() , itrFramelist->m_fDelay);

                // Insert frames
                hItemFrame = InsertItem(chFrameName, hItemSchedule, NULL);
                ouTreeItemData.m_unionData.m_uchItemSubType = CLINScheduleDataStore::pGetLINSchedDataStore().bIsFrameSupported(eCmdType);
                SetItemData(hItemFrame, ouTreeItemData.m_dwTreeData);
                SetItemImage(hItemFrame, 0, 0);

                if ( itrFramelist->m_bEnabled == true &&  ( ouTreeItemData.m_unionData.m_uchItemSubType == true ) )
                {
                    SetCheck(hItemFrame, true);
                }


                itrFramelist++;
            }


            itrSchdTable++;
        }
        Expand(hItemChannel, TVM_EXPAND);
    }
    return 0;
}


BOOL CCheckColumnTreeCtrl::bIsItemChecked(HTREEITEM hItem)
{
    if(hItem == NULL)
    {
        return FALSE;
    }

    return GetItemState( hItem, TVIS_STATEIMAGEMASK )>>12 == 2;
}




INT CCheckColumnTreeCtrl::nEnableScheduleCommand(HTREEITEM hItem, bool bEnable)
{
    SetCheck(hItem, bEnable);
    return S_FALSE;
}


void CCheckColumnTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    UINT nHitTestFlags = 0;
    HTREEITEM hItem = HitTest(point, &nHitTestFlags);

    if((hItem != NULL) && (TVHT_ONITEMSTATEICON & nHitTestFlags))
    {
        vOnItemClick(hItem);
    }
    else
    {
        CTreeCtrl::OnLButtonDown(nFlags, point);
    }
}

void CCheckColumnTreeCtrl::EnableChildItems(HTREEITEM hItem, bool bEnable)
{
    HTREEITEM hChild = GetChildItem(hItem);
    sTreeData ouItemData;
    while ( hChild )
    {
        ouItemData.m_dwTreeData = GetItemData(hChild);
        if(ouItemData.m_unionData.m_uchItemType == ITEM_TYPE_MASTER_COMMAND &&
                ouItemData.m_unionData.m_uchItemSubType == false)
        {
            nEnableScheduleCommand(hChild, false);
        }
        else
        {
            nEnableScheduleCommand(hChild, bEnable);
        }
        hChild = GetNextItem(hChild, TVGN_NEXT);
    }
}



/*
1. Click on Schedule table
    ->Enable
        1. Enable All Items
        2. Disable all other tables
    ->Disable
        1. Disable all items

2. Click on Command
    ->Enable
        1. Enable It
        2. Enable Parent
        3. Disable Other Parents(Tables)
    ->Disable
        1. Disable It
        2. Disable parent if all child are disabled
*/

void CCheckColumnTreeCtrl::vOnItemClick(HTREEITEM hItem)
{
    sTreeData ouItemData;
    ouItemData.m_dwTreeData = GetItemData(hItem);

    if ( ouItemData.m_unionData.m_uchItemType == ITEM_TYPE_SCHEDULETABLE )
    {
        vHandleScheduleTableClick(hItem);
    }
    else if(ouItemData.m_unionData.m_uchItemType == ITEM_TYPE_MASTER_COMMAND )
    {
        if( ouItemData.m_unionData.m_uchItemSubType == true)
        {
            vHandleCommandItemClick(hItem);
        }
        else
        {
            MessageBox("Currently Only Conditional and Diagnostic messages are supported", "Unsupported Frames", MB_OK | MB_ICONINFORMATION);
        }
    }
}

int CCheckColumnTreeCtrl::nGetTableDetails(HTREEITEM hTableItem, int& nChannelIndex, int& nTableIndex)
{
    sTreeData sData;
    nTableIndex = -1, nChannelIndex = -1;

    sData.m_dwTreeData = GetItemData(hTableItem);
    nTableIndex = sData.m_unionData.m_uchItemIndex;

    hTableItem = GetParentItem(hTableItem);
    if ( hTableItem != NULL )
    {
        sData.m_dwTreeData = GetItemData(hTableItem);
        nChannelIndex = sData.m_unionData.m_uchItemIndex;
    }

    if ( nTableIndex == -1 || nChannelIndex == -1 )
    {
        return S_FALSE;
    }
    return S_OK;
}

int CCheckColumnTreeCtrl::nGetItemDetails(HTREEITEM hCommandItem, int& nChannelIndex, int& nTableIndex, int& nItemIndex)
{
    sTreeData sData;
    nItemIndex = -1, nTableIndex = -1, nChannelIndex = -1;

    sData.m_dwTreeData = GetItemData(hCommandItem);
    nItemIndex = sData.m_unionData.m_uchItemIndex;

    hCommandItem = GetParentItem(hCommandItem);
    if ( hCommandItem != NULL )
    {
        sData.m_dwTreeData = GetItemData(hCommandItem);
        nTableIndex = sData.m_unionData.m_uchItemIndex;

        hCommandItem = GetParentItem(hCommandItem);
        if ( hCommandItem != NULL )
        {
            sData.m_dwTreeData = GetItemData(hCommandItem);
            nChannelIndex = sData.m_unionData.m_uchItemIndex;
        }
    }
    if ( nItemIndex == -1 || nTableIndex == -1 || nChannelIndex == -1 )
    {
        return S_FALSE;
    }
    return S_OK;
}



void CCheckColumnTreeCtrl::vHandleScheduleTableClick(HTREEITEM hItem)
{
    BOOL bCheckStatus = bIsItemChecked(hItem);
    int nChannelIndex = -1, nTableIndex = -1;
    if ( S_OK != nGetTableDetails(hItem, nChannelIndex, nTableIndex) )
    {
        return;

    }

    if(bCheckStatus == false)
    {
        SetCheck(hItem);
        HTREEITEM hChannelItem = GetParentItem(hItem);
        HTREEITEM hTableItem = GetChildItem(hChannelItem);

        while(hTableItem)
        {
            if ( hTableItem == hItem )
            {
                EnableChildItems(hTableItem, true);
            }
            else
            {
                SetCheck(hTableItem, false);
                EnableChildItems(hTableItem, false);
            }
            hTableItem = GetNextItem(hTableItem, TVGN_NEXT);
        }
    }
    else
    {
        SetCheck(hItem, false);
        EnableChildItems(hItem, false);
    }
    CLINScheduleDataStore::pGetLINSchedDataStore().nEnableScheduleTable(nChannelIndex, nTableIndex, !bCheckStatus);
}





void CCheckColumnTreeCtrl::vHandleCommandItemClick(HTREEITEM hItem)
{
    HTREEITEM hItemSave = hItem;
    BOOL bCheckStatus = bIsItemChecked(hItem);
    HTREEITEM hTableItem = GetParentItem(hItem);
    HTREEITEM hTableItemSave = hTableItem;

    HTREEITEM hChannelItem = GetParentItem(hTableItem);

    BOOL bTableDisabled = false;

    if ( bCheckStatus == false )        //Now Enable
    {
        nEnableScheduleCommand(hItem, true);
        SetCheck(hTableItem, true);
        hTableItem = GetChildItem(hChannelItem);
        while(hTableItem)
        {
            if ( hTableItem != hTableItemSave )
            {
                SetCheck(hTableItem, false);
                EnableChildItems(hTableItem, false);
            }
            hTableItem = GetNextItem(hTableItem, TVGN_NEXT);
        }
    }
    else                                //Now Disable
    {
        nEnableScheduleCommand(hItem, false);
        hItem = GetChildItem(hTableItem);

        int nTableIndex = 0;
        while( hItem && bTableDisabled == false )
        {
            bTableDisabled = bIsItemChecked(hItem);
            hItem = GetNextItem(hItem, TVGN_NEXT);
            nTableIndex++;
        }
        SetCheck(hTableItem, bTableDisabled);

    }

    int nChannelIndex = -1, nTableIndex = -1, nItemIndex = -1;

    int nRet = nGetItemDetails(hItemSave, nChannelIndex, nTableIndex, nItemIndex);
    if ( nRet == S_OK )
    {
        CLINScheduleDataStore::pGetLINSchedDataStore().nEnableScheduleCommand(nChannelIndex, nTableIndex, nItemIndex, !bCheckStatus);
    }


}

void CCheckColumnTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    HTREEITEM hItem = GetSelectedItem();
    UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK );
    if ( !(nState>>VK_MENU) && nChar == VK_SPACE )
    {
        return vOnItemClick(hItem);
    }
    CColumnTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}