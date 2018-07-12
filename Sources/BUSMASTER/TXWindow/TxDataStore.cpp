/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
* \file      CTxDataStore.cpp
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/

#pragma once
#include "TxDataStore.h"
CTxDataStore::CTxDataStore()
{
    InitializeCriticalSection(&m_ouCSLstMsgItem);
}
CTxDataStore::~CTxDataStore()
{
    DeleteAllMsgItems();
    DeleteCriticalSection(&m_ouCSLstMsgItem);
}
int CTxDataStore::GetMsgItem(int nIndex, ITxMsgItem*& MsgItem)
{
    EnterCriticalSection(&m_ouCSLstMsgItem);
    int hResult = S_FALSE;
    auto itr = m_lstpMsgItem.begin();
    std::advance(itr, nIndex);
    if (itr != m_lstpMsgItem.end())
    {
        hResult = S_OK;
        MsgItem = *itr;
    }
    LeaveCriticalSection(&m_ouCSLstMsgItem);
    return hResult;
}
int CTxDataStore::SetMsgItem(int nIndex, ITxMsgItem* MsgItem)
{
    EnterCriticalSection(&m_ouCSLstMsgItem);
    int hResult = S_FALSE;
    auto itr = m_lstpMsgItem.begin();
    std::advance(itr, nIndex);
    if (itr != m_lstpMsgItem.end() && nullptr != MsgItem)
    {
        hResult = S_OK;
        *itr = MsgItem;
    }
    LeaveCriticalSection(&m_ouCSLstMsgItem);
    return hResult;
}
int CTxDataStore::AddMsgItemAt(ITxMsgItem* MsgItem, int nIndex)
{
    EnterCriticalSection(&m_ouCSLstMsgItem);
    int hResult = S_FALSE;
    if (nullptr != MsgItem)
    {
        int nSize = m_lstpMsgItem.size();
        if (-1 == nIndex || nIndex == nSize)
        {
            hResult = S_OK;
            m_lstpMsgItem.push_back(MsgItem);
        }
        else if (nIndex < nSize)
        {
            auto itr = m_lstpMsgItem.begin();
            std::advance(itr, nIndex);
            if (itr != m_lstpMsgItem.end())
            {
                hResult = S_OK;
                *itr = MsgItem;
            }
        }
    }
    LeaveCriticalSection(&m_ouCSLstMsgItem);
    return hResult;
}
int CTxDataStore::DeleteMsgItemAt(int nIndex)
{
    EnterCriticalSection(&m_ouCSLstMsgItem);
    int hResult = S_FALSE;
    auto itr = m_lstpMsgItem.begin();
    std::advance(itr, nIndex);
    if (itr != m_lstpMsgItem.end())
    {
        hResult = S_OK;
        if (*itr != nullptr)
        {
            delete *itr;
            m_lstpMsgItem.erase(itr);
        }
    }
    LeaveCriticalSection(&m_ouCSLstMsgItem);
    return hResult;
}
int CTxDataStore::DeleteAllMsgItems()
{
    EnterCriticalSection(&m_ouCSLstMsgItem);
    int hResult = S_OK;
for (auto itr : m_lstpMsgItem)
    {
        if (itr != nullptr)
        {
            delete itr;
        }
    }
    m_lstpMsgItem.clear();
    LeaveCriticalSection(&m_ouCSLstMsgItem);
    return hResult;
}
int CTxDataStore::GetMsgItemCount()
{
    return m_lstpMsgItem.size();
}
int CTxDataStore::SetMsgItemsConfigData(xmlNodePtr pxmlNodePtr)
{
    EnterCriticalSection(&m_ouCSLstMsgItem);
    int hResult = S_FALSE;
    xmlChar* pchPathMsg = (xmlChar*)DEF_MESSAGE;
    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pxmlNodePtr, pchPathMsg);
    if (pObjectPath != nullptr)
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if (nullptr != pNodeSet)
        {
            hResult = S_OK;
            int nCount = 0;
for (auto itr: m_lstpMsgItem)
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[nCount];       //Take First One only
                itr->SetMsgConfig(pNode);
                ++nCount;
            }
        }
        xmlFree(pObjectPath);
    }
    LeaveCriticalSection(&m_ouCSLstMsgItem);
    return hResult;
}
int CTxDataStore::GetMsgItemsConfigData(xmlNodePtr pxmlNodePtr)
{
    EnterCriticalSection(&m_ouCSLstMsgItem);
    int hResult = S_FALSE;
    if (nullptr != pxmlNodePtr)
    {
        xmlNodePtr pxmlMsgItem = nullptr;
for (auto itr : m_lstpMsgItem)
        {
            pxmlMsgItem = xmlNewNode(nullptr, BAD_CAST DEF_MESSAGE);
            if (nullptr != pxmlMsgItem)
            {
                itr->GetMsgConfig(pxmlMsgItem);
                xmlAddChild(pxmlNodePtr, pxmlMsgItem);
            }
        }
    }
    LeaveCriticalSection(&m_ouCSLstMsgItem);
    return hResult;
}

int CTxDataStore::SetMsgItemsOldConfigData(xmlNodePtr pxmlNodePtr)
{
    EnterCriticalSection(&m_ouCSLstMsgItem);
    int hResult = S_FALSE;
    xmlChar* pchPathMsg = (xmlChar*)DEF_MSG_BLOCK;
    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pxmlNodePtr, pchPathMsg);
    if (pObjectPath != nullptr)
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if (nullptr != pNodeSet)
        {
            xmlNodePtr pNodeMsgBlock = pNodeSet->nodeTab[0];
            xmlNodePtr pCurMsgsTag = nullptr;
            auto itr = m_lstpMsgItem.begin();
            while (nullptr != pNodeMsgBlock)
            {
                pCurMsgsTag = pNodeMsgBlock->children;
                while (nullptr != pCurMsgsTag && itr != m_lstpMsgItem.end())
                {
                    if (!xmlStrcmp(pCurMsgsTag->name, (const xmlChar*)DEF_MSGS))
                    {
                        ITxMsgItem* pITxMsgItem = *itr;
                        itr++;
                        pITxMsgItem->SetOldMsgConfig(pCurMsgsTag);
                    }
                    pCurMsgsTag = pCurMsgsTag->next;
                }

                pNodeMsgBlock = pNodeMsgBlock->next;
            }
        }
        xmlFree(pObjectPath);
    }
    LeaveCriticalSection(&m_ouCSLstMsgItem);
    return hResult;
}