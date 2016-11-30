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
* \file      LINTxFormView.cpp
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/


#pragma once
#include"LINTransmitter.h"
#include"HashDefines.h"
#include"LINTxFormView.h"
#include "LINDefines.h"
#include<cctype>




CLINTxFormView* CLINTxFormView::m_pouLINTxFormView = nullptr;
int CLINTxFormView::m_nRefCounter = 0;

CLINTxFormView::CLINTxFormView()
{
    m_pouTransmitter = CLINTransmitter::GetInstance();
    if (nullptr != m_pouTxDataStore)
    {
        m_pouTransmitter->SetTxDataStore(m_pouTxDataStore);
    }
}
CLINTxFormView::~CLINTxFormView()
{
    if (nullptr != m_pouTransmitter)
    {
        ((CLINTransmitter*)m_pouTransmitter)->ReleaseInstance();
        m_pouTransmitter = nullptr;
    }
}


CLINTxFormView* CLINTxFormView::GetInstance()
{
    if (nullptr == m_pouLINTxFormView)
    {
        m_pouLINTxFormView = new CLINTxFormView();
    }
    m_nRefCounter++;
    return m_pouLINTxFormView;
}
void CLINTxFormView::ReleaseInstance()
{
    m_nRefCounter--;
    if (0 == m_nRefCounter && nullptr != m_pouLINTxFormView)
    {
        delete m_pouLINTxFormView;
        m_pouLINTxFormView = nullptr;
    }
}


int CLINTxFormView::SetClientId(unsigned long ulClientId)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTransmitter)
    {
        return hResult;
    }
    m_pouTransmitter->SetClientId(ulClientId);
    return hResult;
}
int CLINTxFormView::GetMsgRowProp(int nIndex, std::list<MSG_LST_ROW>& lstMsgRowProp)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    int nCount = m_pouTxDataStore->GetMsgItemCount();
    MSG_LST_ROW ouMsgLstRow;
    if (nIndex < nCount)
    {
        ITxMsgItem* pouMsgItem = nullptr;
        m_pouTxDataStore->GetMsgItem(nIndex, pouMsgItem);

        if (nullptr == pouMsgItem)
        {
            return hResult;
        }

        hResult = S_OK;
        //Msg Name
        pouMsgItem->GetMsgName(m_pouIBMNetwork, m_bHexMode, ouMsgLstRow.Text);
        ouMsgLstRow.ListInfo.m_eType = ((m_eBusStatus == BUS_CONNECTED) ? eNoControl : eComboList);
        //GetMsgNameList with default Channel 0
        vGetDBMsgNameList(0, ouMsgLstRow.ListInfo.m_omEntries);
        lstMsgRowProp.push_back(ouMsgLstRow);

        //Msg Id
        vGetStringFromValue(pouMsgItem->MsgDetails.nMsgId, ouMsgLstRow.Text);
        ouMsgLstRow.ListInfo.m_eType = eNoControl;
        lstMsgRowProp.push_back(ouMsgLstRow);

        //Channel
        ouMsgLstRow.Text = std::to_string(pouMsgItem->MsgDetails.nChannel);
        ouMsgLstRow.ListInfo.m_eType = ((m_eBusStatus == BUS_CONNECTED) ? eNoControl : eComboItem);
        ouMsgLstRow.ListInfo.m_omEntries.RemoveAll();
        GetChannelList(ouMsgLstRow.ListInfo.m_omEntries);
        lstMsgRowProp.push_back(ouMsgLstRow);

        //DLC
        ouMsgLstRow.Text = std::to_string(pouMsgItem->MsgDetails.nDLC);
        ouMsgLstRow.NumInfo.m_uMinVal.m_un64Value = 1;
        ouMsgLstRow.NumInfo.m_uMaxVal.m_un64Value = 8;
        ouMsgLstRow.NumInfo.m_uDelta.m_un64Value = 1;
        ouMsgLstRow.NumInfo.m_nTextLimit = 1;
        ouMsgLstRow.NumInfo.m_byFlag = BUDDY_CTRL;
        ouMsgLstRow.NumInfo.m_nSigLength = 8;
        ouMsgLstRow.NumInfo.m_byBase = defBASE_DEC;
        ouMsgLstRow.ListInfo.m_eType = ((m_eBusStatus == BUS_CONNECTED) ? eNoControl : eNumber);
        lstMsgRowProp.push_back(ouMsgLstRow);

        //Msg Type
        if (pouMsgItem->MsgDetails.nMsgType == LIN_SLAVE_RESPONSE)
        {
            ouMsgLstRow.Text = def_MSG_TYPE_SLAVE;
        }
        else
        {
            ouMsgLstRow.Text = def_MSG_TYPE_MASTER;
        }

        ouMsgLstRow.ListInfo.m_eType = ((m_eBusStatus == BUS_CONNECTED) ? eNoControl : eComboItem);
        ouMsgLstRow.ListInfo.m_omEntries.RemoveAll();
        GetMsgTypeList(ouMsgLstRow.ListInfo.m_omEntries);
        lstMsgRowProp.push_back(ouMsgLstRow);

        //Repetation
        ouMsgLstRow.Text = std::to_string(pouMsgItem->TxDetails.nActualTimer);
        ouMsgLstRow.ListInfo.m_eType = eNumber;
        SNUMERICINFO NumericInfo;
        ouMsgLstRow.NumInfo = NumericInfo;
        ouMsgLstRow.UserProgInfo.m_bIcon = true;
        ouMsgLstRow.UserProgInfo.m_pfHandler = vCheckBoxClickHandler;
        ouMsgLstRow.UserProgInfo.m_pUserParam = m_pouTxDataStore;

        ouMsgLstRow.lvitem = { 0 };
        ouMsgLstRow.lvitem.mask = LVIF_IMAGE;
        ouMsgLstRow.lvitem.pszText = "";
        ouMsgLstRow.lvitem.iImage = pouMsgItem->TxDetails.m_bTimerEnabled;
        ouMsgLstRow.lvitem.iItem = nIndex;
        ouMsgLstRow.lvitem.iSubItem = eMsgColRept;
        lstMsgRowProp.push_back(ouMsgLstRow);


        //Key
        ouMsgLstRow.Text = pouMsgItem->TxDetails.m_chKeyVal;
        ouMsgLstRow.ListInfo.m_eType = eAlphaNumericType;
        ouMsgLstRow.UserProgInfo.m_bIcon = true;
        ouMsgLstRow.UserProgInfo.m_pfHandler = vCheckBoxClickHandler;
        ouMsgLstRow.UserProgInfo.m_pUserParam = m_pouTxDataStore;

        ouMsgLstRow.lvitem = { 0 };
        ouMsgLstRow.lvitem.mask = LVIF_IMAGE;
        ouMsgLstRow.lvitem.pszText = "a";
        ouMsgLstRow.lvitem.iImage = pouMsgItem->TxDetails.bKeyEnabled;
        ouMsgLstRow.lvitem.iItem = nIndex;
        ouMsgLstRow.lvitem.iSubItem = eMsgColKey;
        lstMsgRowProp.push_back(ouMsgLstRow);
    }
    else if (nIndex == nCount)
    {
        hResult = S_OK;
        ouMsgLstRow.Text = def_ADD_MESSAGE;
        ouMsgLstRow.ListInfo.m_eType = eComboList;
        //GetMsgNameList with default Channel 0
        vGetDBMsgNameList(0, ouMsgLstRow.ListInfo.m_omEntries);
        lstMsgRowProp.push_back(ouMsgLstRow);

        ouMsgLstRow.Text = "";
        ouMsgLstRow.ListInfo.m_eType = eNoControl;

        ouMsgLstRow.lvitem = { 0 };

        for (auto nIndex = 1; nIndex < eMsgColAll; nIndex++)
        {
            lstMsgRowProp.push_back(ouMsgLstRow);
        }

    }
    return hResult;
}
int CLINTxFormView::LoadAllMsgItems()
{
    int hResult = S_FALSE;
    vClearAllViews();
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    hResult = S_OK;
    int nSize = m_pouTxDataStore->GetMsgItemCount();
    std::list<MSG_LST_ROW> lstMsgRowProp;
    for (int nCount = 0; nCount <= nSize; nCount++)
    {
        lstMsgRowProp.clear();
        if (S_OK == GetMsgRowProp(nCount, lstMsgRowProp))
        {
            SetMsgLstRow(lstMsgRowProp, nCount);
        }
    }
    vUpdateButtonStatus();
    return hResult;
}

int CLINTxFormView::OnMsgItemSelected(int nRow)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    double dFactor = 0;
    std::list<SIG_DETAILS> lstSigDetails;
    ITxMsgItem* pouMsgItem = nullptr;
    m_pouTxDataStore->GetMsgItem(nRow, pouMsgItem);
    if (nullptr == pouMsgItem)
    {
        return hResult;
    }
    if (nullptr != m_pouIBMNetwork)
    {
        pouMsgItem->GetSignalList(m_pouIBMNetwork, m_bHexMode, lstSigDetails);
    }

    hResult = S_OK;

    UpdateDataAndSignalView(pouMsgItem->MsgDetails.pchData, pouMsgItem->MsgDetails.nDLC, lstSigDetails);

    vOnMsgTypeSelected(nRow);

    return hResult;
}
void CLINTxFormView::vOnMsgTypeSelected(int nSelItem)
{
    if (-1 != nSelItem)
    {
        CString omStrMsgType = m_lstMsg.GetItemText(nSelItem, eMsgColType);
        eLIN_MSG_TYPE eLinMsgType = LIN_SLAVE_RESPONSE;
        if (omStrMsgType == def_MSG_TYPE_MASTER)
        {
            eLinMsgType = LIN_MASTER_RESPONSE;
        }
        else if (omStrMsgType == def_MSG_TYPE_SLAVE)
        {
            eLinMsgType = LIN_SLAVE_RESPONSE;
        }

        bool bActivateView = false;
        if (eLinMsgType == LIN_MASTER_RESPONSE)
        {
            bActivateView = false;
        }
        else if (eLinMsgType == LIN_SLAVE_RESPONSE)
        {
            bActivateView = true;
        }
        m_omLctrSigList.EnableWindow(bActivateView);
        m_omLctrDataBytesList.EnableWindow(bActivateView);
    }
}

void CLINTxFormView::vGetColPropMap(ESTATUS_BUS eBusStatus, std::map<int, SLISTINFO>& mapColAndProp)
{
    SLISTINFO sListInfo;
    if (BUS_CONNECTED == eBusStatus)
    {
        sListInfo.m_eType = eNoControl;
        mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColName, sListInfo));
        mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColId, sListInfo));
        mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColChannel, sListInfo));
        mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColDLC, sListInfo));
        mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColType, sListInfo));
    }
    else
    {
        sListInfo.m_eType = eComboList;
        sListInfo.m_omEntries.RemoveAll();
        //Channel - 0
        vGetDBMsgNameList(0, sListInfo.m_omEntries);
        mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColName, sListInfo));

        sListInfo.m_eType = eComboItem;
        sListInfo.m_omEntries.RemoveAll();
        GetMsgTypeList(sListInfo.m_omEntries);
        mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColType, sListInfo));

		sListInfo.m_eType = eComboItem;
		sListInfo.m_omEntries.RemoveAll();
		GetChannelList(sListInfo.m_omEntries);
		mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColChannel, sListInfo));

		sListInfo.m_eType = eNumber;
		sListInfo.m_omEntries.RemoveAll();
		mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColDLC, sListInfo));
    }
}

void CLINTxFormView::GetMsgTypeList(CStringArray& omStrMsgTypeList)
{
    omStrMsgTypeList.Add(def_MSG_TYPE_MASTER);
    omStrMsgTypeList.Add(def_MSG_TYPE_SLAVE);
}
void CLINTxFormView::GetChannelList(CStringArray& omStrChannelList)
{
    if (nullptr == m_pouIBMNetwork)
    {
        return;
    }
    int nConfigChannel = 0;
    m_pouIBMNetwork->GetChannelCount(LIN, nConfigChannel);

    CString strChannel;

    for (int i = 0; i < nConfigChannel; i++)
    {
        strChannel.Format("%d", i + 1);
        omStrChannelList.Add(strChannel);
    }

}
void CLINTxFormView::vGetDBMsgNameList(int nChannel, CStringArray& omStrArrayMsgNameList)
{
    char chString[MAX_PATH] = "";
    std::map<int, std::string> ouMsgIDNamesMap;
    nGetMessageIDNames(nChannel, ouMsgIDNamesMap);

for (auto itrMsgId : ouMsgIDNamesMap)
    {
        if (false == m_bHexMode)
        {
            sprintf_s(chString, MAX_PATH, "%s [%0d]", itrMsgId.second.c_str(), itrMsgId.first);
        }
        else
        {
            sprintf_s(chString, MAX_PATH, "%s [0x%02X]", itrMsgId.second.c_str(), itrMsgId.first);
        }

        omStrArrayMsgNameList.Add(chString);
    }
}
int CLINTxFormView::nGetMessageIDNames(int nChannelIndex, std::map<int, std::string>& ouMsgIDNamesMap)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouIBMNetwork)
    {
        return hResult;
    }
    ICluster* ouDBService = nullptr;
    m_pouIBMNetwork->GetDBService(LIN, nChannelIndex, 0, &ouDBService);
    if (ouDBService != nullptr)
    {
        hResult = S_OK;
        std::map<UID_ELEMENT, IElement*> ouFrameList;
        std::string strName;
        unsigned int unId = 0;
        ouDBService->GetElementList(eFrameElement, ouFrameList);


for (auto itrFrame : ouFrameList)
        {
            ((IFrame*)((&itrFrame)->second))->GetFrameId(unId);
            ((IFrame*)((&itrFrame)->second))->GetName(strName);

            LinFrameProps ouFrameProps;
            ((IFrame*)((&itrFrame)->second))->GetProperties(ouFrameProps);
			if (ouFrameProps.m_eLinFrameType == eLinUnconditionalFrame || ouFrameProps.m_eLinFrameType == eLinDiagnosticFrame)
            {
                ouMsgIDNamesMap.insert(std::map<int, std::string>::value_type(unId, strName));
            }
        }
    }
    return hResult;
}

void CLINTxFormView::vCheckBoxClickHandler(CListCtrl* pList, int nItem, int nSubItem, void* UParam)
{
    CTxDataStore* pouTxDataStore = (CTxDataStore*)UParam;
    if (nullptr == pouTxDataStore)
    {
        return;
    }

    LVITEM lvitem = { 0 };
    lvitem.mask = LVIF_IMAGE;
    lvitem.iItem = nItem;
    lvitem.iSubItem = nSubItem;

    ((CListCtrl*)pList)->GetItem(&lvitem);


    lvitem.mask = LVIF_IMAGE;
    lvitem.pszText = "";

    bool bEnable = false;

    ITxMsgItem* pouMsgItem=nullptr;
    pouTxDataStore->GetMsgItem(nItem, pouMsgItem);
    if (nullptr == pouMsgItem)
    {
        return;
    }
    if (lvitem.iImage == 0)
    {
        lvitem.iImage = 1;
        bEnable = true;
    }
    else
    {
        lvitem.iImage = 0;
        bEnable = false;
    }

    if (nSubItem == eMsgColKey)
    {
        pouMsgItem->TxDetails.bKeyEnabled = bEnable;
        pouTxDataStore->SetMsgItem(nItem, pouMsgItem);
    }
    else if (nSubItem == eMsgColRept)
    {
        pouMsgItem->TxDetails.m_bTimerEnabled = bEnable;
        pouTxDataStore->SetMsgItem(nItem, pouMsgItem);
    }
    ((CListCtrl*)pList)->SetItem(&lvitem);
}
int CLINTxFormView::nGetDefaultMsgItem(std::string strMsgName, ITxMsgItem*& pouMsgItem)
{
    int hResult = S_FALSE;
    int nMsgId = 0;
    IFrame* pIFrame = nullptr;
    if (nullptr == pouMsgItem)
    {
        return hResult;
    }
    if (true == bGetMsgValidNameAndId(strMsgName, nMsgId, pIFrame))
    {
        hResult = S_OK;
        if (nullptr != pIFrame)
        {
            unsigned int nTemp;
            CString omStrTemp;
            std::map<ISignal*, SignalInstanse> mapSignals;

            //pIFrame->GetName(pouMsgItem->MsgDetails.strMsgName);

            pIFrame->GetFrameId(nTemp);
            pouMsgItem->MsgDetails.nMsgId = nTemp;

            pIFrame->GetLength(nTemp);
            pouMsgItem->MsgDetails.nDLC = nTemp;

            pouMsgItem->MsgDetails.nMsgType = LIN_SLAVE_RESPONSE;
            pouMsgItem->MsgDetails.nChannel = 1;
            pIFrame->GetSignalList(mapSignals);
            nGetDataFromSignals(mapSignals, pouMsgItem->MsgDetails.nDLC, pouMsgItem->MsgDetails.pchData);
        }
        else
        {
            //Msg Id
            UINT unMsgID = 0;
            CHAR* pcStr = nullptr;
            bool bHex = true;

            bHex = m_bHexMode;

            if (bHex == true)
            {
                unMsgID = strtol(strMsgName.c_str(), &pcStr, 16);
            }
            else
            {
                unMsgID = strtol(strMsgName.c_str(), &pcStr, 10);
            }
            /*vGetStringFromValue(unMsgID, pouMsgItem->MsgDetails.strMsgName);*/
            pouMsgItem->MsgDetails.nMsgId = unMsgID;
            pouMsgItem->MsgDetails.nDLC = 8;
            pouMsgItem->MsgDetails.nMsgType = LIN_SLAVE_RESPONSE;
            pouMsgItem->MsgDetails.nChannel = 1;
            memset(pouMsgItem->MsgDetails.pchData, 0xFF, sizeof(unsigned char)*defLIN_MSG_DLC_MAX);
        }
    }
    return hResult;
}
int CLINTxFormView::nGetDataFromSignals(std::map<ISignal*, SignalInstanse>& mapSignal, int nDLC, unsigned char* pchData)
{
    __int64 nI64SignVal=0;
    LINSignalProps ouSignalProps;
	for (auto itr : mapSignal)
    {
        itr.first->GetProperties(ouSignalProps);

        //update the data
        vGetDataBytesFromSignal(ouSignalProps.m_nIntialValue, std::make_pair(itr.first, itr.second), nDLC, pchData);


    }
    return S_OK;
}
bool CLINTxFormView::bGetMsgValidNameAndId(std::string& strMsgName, int& nMsgId, IFrame*& pIFrame)
{
    bool bReturn = false;
    nMsgId = nParseMsgId(strMsgName);
    if (nMsgId >= 0 && nMsgId <= defLIN_MSG_ID_MAX)
    {
        //Default Channel - 0
        bReturn = true;
        pIFrame = nullptr;
        if (EC_SUCCESS == m_pouIBMNetwork->GetFrame(LIN, 0, nMsgId, nullptr, &pIFrame))
        {
            strMsgName = std::string(strMsgName);
        }
    }
    return bReturn;
}
eLIN_MSG_TYPE CLINTxFormView::GetMsgType(int nRow)
{
    CString omStrMsgType = m_lstMsg.GetItemText(nRow, eMsgColType);
    eLIN_MSG_TYPE eLinMsgType;
    if (omStrMsgType == def_MSG_TYPE_MASTER)
    {
        eLinMsgType = LIN_MASTER_RESPONSE;
    }
    else if (omStrMsgType == def_MSG_TYPE_SLAVE)
    {
        eLinMsgType = LIN_SLAVE_RESPONSE;
    }
    return eLinMsgType;
}
void CLINTxFormView::UpdateMsgListView(int nRow)
{
    if (nullptr == m_pouTxDataStore)
    {
        return;
    }
    std::list<MSG_LST_ROW> lstMsgRowProp;
    ITxMsgItem* pouMsgItem = nullptr;
    m_pouTxDataStore->GetMsgItem(nRow, pouMsgItem);

    if (S_OK == GetMsgRowProp(nRow, lstMsgRowProp))
    {
        SetMsgLstRow(lstMsgRowProp, nRow);
    }
    std::list<SIG_DETAILS> lstSigDetails;
    if (nullptr != pouMsgItem)
    {
        pouMsgItem->GetSignalList(m_pouIBMNetwork, m_bHexMode, lstSigDetails);
        UpdateDataAndSignalView(pouMsgItem->MsgDetails.pchData, pouMsgItem->MsgDetails.nDLC, lstSigDetails);
    }

    vOnMsgTypeSelected(nRow);
    vUpdateButtonStatus();
    m_lstMsg.SetItemState(nRow, LVIS_SELECTED, LVIS_SELECTED);
    m_lstMsg.SetSelectionMark(nRow);
}
int CLINTxFormView::nParseMsgId(std::string strMsgNameId)
{
    CString omStrMsgName = strMsgNameId.c_str();
    int nMsgID = -1;
    // Get message Id from database in case user has selected a message name.
    //if it is name [] is present
    int nIndex = omStrMsgName.Find(defMSGID_NAME_START_CHAR);
    if (nIndex != -1)
    {
        nMsgID = nParseIDFromName(std::string(omStrMsgName));
    }
    // If the message name is not selected then get it from edit box and convert
    // it to the integer value.
    else
    {
        UINT unMsgID = 0;
        CHAR* pcStr = nullptr;
        BOOL bHex = TRUE;

        if (m_bHexMode == true)
        {
            unMsgID = strtol(omStrMsgName, &pcStr, 16);
        }
        else
        {
            unMsgID = strtol(omStrMsgName, &pcStr, 10);
        }
        // check if the field is empty or user have entered "0".
        // so in case user has entered "0" or if non zero value is
        // entered and strtol function has writtened correct value.
        if (omStrMsgName.CompareNoCase("0") != 0 && unMsgID != 0 || omStrMsgName.CompareNoCase("0") == 0)
        {
            nMsgID = static_cast<INT> (unMsgID);
        }
    }
    return nMsgID;
}
int CLINTxFormView::nParseIDFromName(std::string strMsgName)
{
    CString omMsgName = strMsgName.c_str();
    CString omStrMsgID;
    UINT unMsgID = (UINT)-1;
    CHAR* pcStopStr = nullptr;
    std::string strStartKey;
    if (m_bHexMode == false)
    {
        strStartKey = "[";
    }
    else
    {
        strStartKey = "[0x";
    }

    int nIndex1 = omMsgName.Find(strStartKey.c_str());
    if (nIndex1 != -1)
    {
        int nIndex2 = omMsgName.ReverseFind(']');
        if (nIndex1 != -1 && nIndex2 > nIndex1)
        {
            int nLength = omMsgName.GetLength();
            omStrMsgID = omMsgName.Mid(nIndex1 + 1, nIndex2 - nIndex1);
            if (m_bHexMode == false)
            {
                unMsgID = strtol((LPCTSTR)omStrMsgID, &pcStopStr, defBASE_DEC);
            }
            else
            {
                unMsgID = strtol((LPCTSTR)omStrMsgID, &pcStopStr, defBASE_HEX);
            }
        }
    }
    return unMsgID;
}
bool CLINTxFormView::bGetKeyVal(int nItem, char& chKeyVal)
{
    bool bReturn = false;
    CString omStrKeyVal = m_lstMsg.GetItemText(nItem, eMsgColKey);
    if (omStrKeyVal.GetLength() > 0)
    {
        chKeyVal = omStrKeyVal.GetAt(0);
        bReturn = std::isalnum(chKeyVal) > 0 ? true : false;
    }
    return bReturn;
}
void CLINTxFormView::vAddNewMessageCombo(int nRow)
{
    std::list<MSG_LST_ROW> lstMsgRowProp;
    if (S_OK == GetMsgRowProp(nRow, lstMsgRowProp))
    {
        SetMsgLstRow(lstMsgRowProp, nRow);
    }
}
int CLINTxFormView::OnMsgItemChanged(int nRow, int nColumn)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    CString omStrItem = m_lstMsg.GetItemText(nRow, nColumn);
    ITxMsgItem* pouMsgItem=nullptr;
    bool bAddNewMessageCombo = false;
    int nMsgLstCount = m_pouTxDataStore->GetMsgItemCount();
    if (nRow < nMsgLstCount)
    {
        m_pouTxDataStore->GetMsgItem(nRow, pouMsgItem);
        if (nullptr == pouMsgItem)
        {
            return hResult;
        }
    }

    if (nRow == nMsgLstCount && nColumn != eMsgColName)
    {
        return hResult;
    }


    unsigned char ucData[defLIN_MSG_DLC_MAX] = { 0 };
    memset(ucData, 0xFF, sizeof(unsigned char)*defLIN_MSG_DLC_MAX);

    int nDlc = 0;
    int nChannel = 0;
    char chKey = '\0';
    switch (nColumn)
    {
        case eMsgColRept:
            pouMsgItem->TxDetails.nActualTimer = nGetValidTimerValue(atol(omStrItem));
            pouMsgItem->TxDetails.nCurrentTimerVal = pouMsgItem->TxDetails.nActualTimer;
            m_pouTxDataStore->SetMsgItem(nRow, pouMsgItem);
            break;
        case eMsgColKey:
            if (true == bGetKeyVal(nRow, chKey))
            {
                pouMsgItem->TxDetails.m_chKeyVal = chKey;
                m_pouTxDataStore->SetMsgItem(nRow, pouMsgItem);
            }
            m_pouTxDataStore->SetMsgItem(nRow, pouMsgItem);
            break;
        case eMsgColName:
            pouMsgItem = new CLINTxMsgItem();
            if (S_OK == nGetDefaultMsgItem(std::string(omStrItem), pouMsgItem))
            {
                if (S_OK != m_pouTxDataStore->AddMsgItemAt(pouMsgItem, nRow))
                {
                    delete pouMsgItem;
                }
                if(nRow == nMsgLstCount)
                {
                    bAddNewMessageCombo = true;
                }
            }
            else
            {
                delete pouMsgItem;
            }
            break;
        case eMsgColType:
            pouMsgItem->MsgDetails.nMsgType = GetMsgType(nRow);
            m_pouTxDataStore->SetMsgItem(nRow, pouMsgItem);
            break;
        case eMsgColDLC:
            nDlc = atol(omStrItem);
            if (nDlc >= 0 && nDlc <= defLIN_MSG_DLC_MAX)
            {
                memcpy(ucData, pouMsgItem->MsgDetails.pchData, pouMsgItem->MsgDetails.nDLC);
                memcpy(pouMsgItem->MsgDetails.pchData, ucData, nDlc);
                pouMsgItem->MsgDetails.nDLC = nDlc;
                m_pouTxDataStore->SetMsgItem(nRow, pouMsgItem);
            }
            break;
        case eMsgColChannel:
            nChannel = atol(omStrItem);
            //TODO: To check the below condition.
            if (nChannel >= 0 && nChannel < CHANNEL_ALLOWED)
            {
                pouMsgItem->MsgDetails.nChannel = nChannel;
                m_pouTxDataStore->SetMsgItem(nRow, pouMsgItem);
            }
            break;
    }
    UpdateMsgListView(nRow);
    if (true == bAddNewMessageCombo)
    {
        vAddNewMessageCombo(nRow + 1);
        EnsureLastItemVisible();
    }
    return S_OK;
}

int CLINTxFormView::OnBusStatusChanged(ESTATUS_BUS eBusStatus)
{
    int hResult = S_FALSE;
    std::map<int, SLISTINFO> mapColAndProp;
    vGetColPropMap(eBusStatus, mapColAndProp);
    SetMsgLstCol(mapColAndProp);
    vUpdateButtonStatus();
    return hResult;
}
int CLINTxFormView::GetConfigTag(std::string& strTag)
{
    strTag = std::string(DEF_MODULE_CONFIGURATION_XPATH) + "/" + std::string(DEF_LIN_TX_WINDOW);
    return S_OK;
}
int CLINTxFormView::AddNewMsgItemsToDataStore(int nCount)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    hResult = S_OK;
    m_pouTxDataStore->DeleteAllMsgItems();
    for (auto nIndex = 0; nIndex < nCount; ++nIndex)
    {
        m_pouTxDataStore->AddMsgItemAt(new CLINTxMsgItem());
    }
    return hResult;
}
int CLINTxFormView::GetMsgListDetails(std::list<std::pair<std::string,int>>& lstMsgDetails)
{
    lstMsgDetails.push_back(std::pair<std::string, int>("Message Name", 20));
    lstMsgDetails.push_back(std::pair<std::string, int>("Frame Id", 6));
    lstMsgDetails.push_back(std::pair<std::string, int>("Channel", 6));
    lstMsgDetails.push_back(std::pair<std::string, int>("Data Length", 8));
    lstMsgDetails.push_back(std::pair<std::string, int>("Message Type", 8));
    lstMsgDetails.push_back(std::pair<std::string, int>("Repetition (ms)", 8));
    lstMsgDetails.push_back(std::pair<std::string, int>("Key", 5));
    return S_OK;
}
int CLINTxFormView::GetEndianess(std::pair<ISignal*, SignalInstanse>& Signal, eEndianess& endian)
{
    int hResult = S_FALSE;
    if (nullptr == Signal.first)
    {
        return hResult;
    }
    hResult = S_OK;
    LINSignalProps ouSignalProps;
    Signal.first->GetProperties(ouSignalProps);
    endian = ouSignalProps.m_ouEndianess;

    return hResult;
}

int CLINTxFormView::SetOldFormatConfigData(xmlDocPtr pDoc)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    std::string strConfigTag = "";
    GetConfigTag(strConfigTag);
    strConfigTag += "/" + std::string(DEF_CHANNEL_MSG_NAME);
    xmlChar* pXpathChannel = (xmlChar*)strConfigTag.c_str();
    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetNodes(pDoc, pXpathChannel);
    if (pObjectPath != nullptr)
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;

        if (nullptr != pNodeSet)
        {
            xmlChar* pchPathMsg = (xmlChar*)DEF_MESSAGE;
            xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pNodeSet->nodeTab[0], pchPathMsg);
            if (nullptr != pObjectPath)
            {
                hResult = S_OK;
                xmlNodeSetPtr pNodeMsg = pObjectPath->nodesetval;
                if (nullptr != pNodeMsg)
                {
                    if (S_OK == AddNewMsgItemsToDataStore(pNodeMsg->nodeNr) && nullptr != m_pouTxDataStore)
                    {
                        m_pouTxDataStore->SetMsgItemsConfigData(pNodeSet->nodeTab[0]);
                    }
                }
            }
        }
    }

    return hResult;
}