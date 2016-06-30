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
* \file      CANTxFormView.cpp
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/


#pragma once
#include"CANTransmitter.h"
#include"HashDefines.h"
#include"CANTxFormView.h"
#include<cctype>

#include "BaseDIL_CAN.h"
//#include "..\DIL_Interface\DIL_Interface_extern.h"

#define defSTR_WARN_TXWINDOW_UPGRADE     "The \"CAN Transmission Window\" configuration is modified. \nPlease refer the Help file for further details."

CCANTxFormView* CCANTxFormView::m_pouCANTxFormView = nullptr;
int CCANTxFormView::m_nRefCounter = 0;

CCANTxFormView::CCANTxFormView()
{
    m_pouTransmitter = CCANTransmitter::GetInstance();
    if (nullptr != m_pouTxDataStore)
    {
        m_pouTransmitter->SetTxDataStore(m_pouTxDataStore);
    }
}
CCANTxFormView::~CCANTxFormView()
{
    if (nullptr != m_pouTransmitter)
    {
        ((CCANTransmitter*)m_pouTransmitter)->ReleaseInstance();
        m_pouTransmitter = nullptr;
    }
}


CCANTxFormView* CCANTxFormView::GetInstance()
{
    if (nullptr == m_pouCANTxFormView)
    {
        m_pouCANTxFormView = new CCANTxFormView();
    }
    m_nRefCounter++;
    return m_pouCANTxFormView;
}
void CCANTxFormView::ReleaseInstance()
{
    m_nRefCounter--;
    if (0 == m_nRefCounter && nullptr != m_pouCANTxFormView)
    {
        delete m_pouCANTxFormView;
        m_pouCANTxFormView = nullptr;
    }
}


int CCANTxFormView::SetClientId(unsigned long ulClientId)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTransmitter)
    {
        return hResult;
    }
    m_pouTransmitter->SetClientId(ulClientId);
    return hResult;
}
int CCANTxFormView::GetMsgRowProp(int nIndex, std::list<MSG_LST_ROW>& lstMsgRowProp)
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
        ouMsgLstRow.ListInfo.m_eType = eComboList;
        //GetMsgNameList with default Channel 0
        vGetDBMsgNameList(0, ouMsgLstRow.ListInfo.m_omEntries);
        lstMsgRowProp.push_back(ouMsgLstRow);

        //Msg Id
        vGetStringFromValue(pouMsgItem->MsgDetails.nMsgId, ouMsgLstRow.Text);
        ouMsgLstRow.ListInfo.m_eType = eNoControl;
        lstMsgRowProp.push_back(ouMsgLstRow);

        //Channel
        ouMsgLstRow.Text = std::to_string(pouMsgItem->MsgDetails.nChannel);
        ouMsgLstRow.ListInfo.m_eType = eComboItem;
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
        ouMsgLstRow.ListInfo.m_eType = eNumber;
        lstMsgRowProp.push_back(ouMsgLstRow);

        //Msg Type
        if (pouMsgItem->MsgDetails.nMsgType & CCANTxMsgItem::eCANSTD)
        {
            ouMsgLstRow.Text = def_MSG_TYPE_STD;
        }
        else if (pouMsgItem->MsgDetails.nMsgType & CCANTxMsgItem::eCANEXT)
        {
            ouMsgLstRow.Text = def_MSG_TYPE_EXT;
        }

        ouMsgLstRow.ListInfo.m_eType = eComboItem;
        ouMsgLstRow.ListInfo.m_omEntries.RemoveAll();
        GetMsgTypeList(ouMsgLstRow.ListInfo.m_omEntries);
        lstMsgRowProp.push_back(ouMsgLstRow);

        //RTR
        ouMsgLstRow.Text = "";
        ouMsgLstRow.ListInfo.m_eType = eNoControl;
        ouMsgLstRow.ListInfo.m_omEntries.RemoveAll();
        ouMsgLstRow.lvitem = { 0 };
        ouMsgLstRow.lvitem.mask = LVIF_IMAGE;
        ouMsgLstRow.lvitem.pszText = "";
        ouMsgLstRow.lvitem.iImage = (bool)(pouMsgItem->MsgDetails.nMsgType & CCANTxMsgItem::eCANRTR);
        ouMsgLstRow.lvitem.iItem = nIndex;
        ouMsgLstRow.lvitem.iSubItem = eMsgColRTR;
        ouMsgLstRow.UserProgInfo.m_bIcon = true;
        ouMsgLstRow.UserProgInfo.m_pfHandler = vCheckBoxClickHandler;
        ouMsgLstRow.UserProgInfo.m_pUserParam = this;
        lstMsgRowProp.push_back(ouMsgLstRow);

        //Repetation
        ouMsgLstRow.Text = std::to_string(pouMsgItem->TxDetails.nActualTimer);
        ouMsgLstRow.ListInfo.m_eType = eNumber;
        SNUMERICINFO NumericInfo;
        ouMsgLstRow.NumInfo = NumericInfo;
        ouMsgLstRow.UserProgInfo.m_bIcon = true;
        ouMsgLstRow.UserProgInfo.m_pfHandler = vCheckBoxClickHandler;
        ouMsgLstRow.UserProgInfo.m_pUserParam = this;

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
        ouMsgLstRow.UserProgInfo.m_pUserParam = this;

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
int CCANTxFormView::LoadAllMsgItems()
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

int CCANTxFormView::OnMsgItemSelected(int nRow)
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
void CCANTxFormView::vOnMsgTypeSelected(int nSelItem)
{
    if (-1 != nSelItem)
    {
        LVITEM lvitem;
        lvitem = { 0 };
        lvitem.mask = LVIF_IMAGE;
        lvitem.pszText = "";
        lvitem.iItem = nSelItem;
        lvitem.iSubItem = eMsgColRTR;
        m_lstMsg.GetItem(&lvitem);

        bool bIsRTR = (bool)lvitem.iImage;
        bool bActivateView = false;
        if (false == bIsRTR)
        {
            bActivateView = true;
        }
        m_omLctrSigList.EnableWindow(bActivateView);
        m_omLctrDataBytesList.EnableWindow(bActivateView);
    }
}

void CCANTxFormView::vGetColPropMap(ESTATUS_BUS eBusStatus, std::map<int, SLISTINFO>& mapColAndProp)
{
    //SLISTINFO sListInfo;
    //if (BUS_CONNECTED == eBusStatus)
    //{
    //  sListInfo.m_eType = eNoControl;
    //  mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColName, sListInfo));
    //}
    //else
    //{
    //  sListInfo.m_eType = eComboList;
    //  sListInfo.m_omEntries.RemoveAll();
    //  //Channel - 0
    //  vGetDBMsgNameList(0, sListInfo.m_omEntries);
    //  mapColAndProp.insert(std::pair<int, SLISTINFO>(eMsgColName, sListInfo));
    //}
}

void CCANTxFormView::GetMsgTypeList(CStringArray& omStrMsgTypeList)
{
    omStrMsgTypeList.Add(def_MSG_TYPE_STD);
    omStrMsgTypeList.Add(def_MSG_TYPE_EXT);
}
void CCANTxFormView::GetChannelList(CStringArray& omStrChannelList)
{
    CBaseDIL_CAN* pouBaseDIL_CAN = nullptr;
    DIL_GetInterface(CAN, (void**)&pouBaseDIL_CAN);

    if (nullptr == pouBaseDIL_CAN)
    {
        return;
    }

    LONG lParam = 0;
    CString omStrChannel = "";
    if (S_OK == pouBaseDIL_CAN->DILC_GetControllerParams(lParam, 0, NUMBER_HW))
    {
        UINT nConfigChannel = (UINT)lParam;
        for (auto nCount = 0; nCount < nConfigChannel; nCount++)
        {
            omStrChannel.Format("%d", nCount + 1);
            omStrChannelList.Add(omStrChannel);
        }
    }
}
void CCANTxFormView::vGetDBMsgNameList(int nChannel, CStringArray& omStrArrayMsgNameList)
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
int CCANTxFormView::nGetMessageIDNames(int nChannelIndex, std::map<int, std::string>& ouMsgIDNamesMap)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouIBMNetwork)
    {
        return hResult;
    }
    ICluster* ouDBService = nullptr;
    std::list<IFrame*> lstFrames;
    m_pouIBMNetwork->GetFrameList(CAN, nChannelIndex, lstFrames);
    std::string strName = "";
    unsigned int unId = 0;
    hResult = S_OK;
for (const auto& itr : lstFrames)
    {
        itr->GetName(strName);
        itr->GetFrameId(unId);
        ouMsgIDNamesMap.insert(std::pair<int, std::string>(unId, strName));
    }
    return hResult;
}

void CCANTxFormView::vCheckBoxClickHandler(CListCtrl* pList, int nItem, int nSubItem, void* UParam)
{
    CCANTxFormView* pouTxFormView = (CCANTxFormView*)UParam;
    if (nullptr == pouTxFormView)
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

    ITxMsgItem* pouMsgItem = nullptr;
    pouTxFormView->GetMsgItem(nItem, pouMsgItem);
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
        pouTxFormView->SetMsgItem(nItem, pouMsgItem);
    }
    else if (nSubItem == eMsgColRept)
    {
        pouMsgItem->TxDetails.m_bTimerEnabled = bEnable;
        pouTxFormView->SetMsgItem(nItem, pouMsgItem);
    }
    else if (nSubItem == eMsgColRTR)
    {
        if (true == bEnable)
        {
            pouMsgItem->MsgDetails.nMsgType |= CCANTxMsgItem::eCANRTR;
        }
        else
        {
            pouMsgItem->MsgDetails.nMsgType &= ~CCANTxMsgItem::eCANRTR;
        }
        pouTxFormView->SetMsgItem(nItem, pouMsgItem);
        pouTxFormView->UpdateMsgListView(nItem);
    }
    ((CListCtrl*)pList)->SetItem(&lvitem);
}
int CCANTxFormView::GetMsgItem(int nIndex, ITxMsgItem*& pMsgItem)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    hResult = m_pouTxDataStore->GetMsgItem(nIndex, pMsgItem);
    return hResult;
}
int CCANTxFormView::SetMsgItem(int nIndex, ITxMsgItem* pMsgItem)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    hResult = m_pouTxDataStore->SetMsgItem(nIndex, pMsgItem);
    return hResult;
}
int CCANTxFormView::nGetDefaultMsgItem(std::string strMsgName, ITxMsgItem*& pouMsgItem)
{
    int hResult = S_FALSE;
    int nMsgId = 0;
    IFrame* pIFrame = nullptr;
    if (nullptr == pouMsgItem)
    {
        return hResult;
    }
    int nMsgType = 0;
    //Get Valid message in Channel 0;
    if (true == bGetMsgValidNameAndId(strMsgName, 0,nMsgType , nMsgId, pIFrame))
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
            FrameProps ouFrameProp;
            pIFrame->GetProperties(ouFrameProp);
            if (eCan_Extended == ouFrameProp.m_ouCANFrameProp.m_canMsgType)
            {
                pouMsgItem->MsgDetails.nMsgType = CCANTxMsgItem::eCANEXT;
            }
            else
            {
                pouMsgItem->MsgDetails.nMsgType = CCANTxMsgItem::eCANSTD;
            }
            pouMsgItem->MsgDetails.nChannel = 1;
            pIFrame->GetSignalList(mapSignals);
            memset(pouMsgItem->MsgDetails.pchData, 0, pouMsgItem->MsgDetails.nDLC);
        }
        else
        {
            //Msg Id
            UINT unMsgID = 0;
            CHAR* pcStr = nullptr;
            BOOL bHex = TRUE;

            bHex = m_bHexMode;

            if (bHex == true)
            {
                unMsgID = strtol(strMsgName.c_str(), &pcStr, 16);
            }
            else
            {
                unMsgID = strtol(strMsgName.c_str(), &pcStr, 10);
            }
            //vGetStringFromValue(unMsgID, pouMsgItem->MsgDetails.strMsgName);
            pouMsgItem->MsgDetails.nMsgId = unMsgID;
            pouMsgItem->MsgDetails.nDLC = def_MSG_DLC_MAX;
            pouMsgItem->MsgDetails.nMsgType = nMsgType;
            pouMsgItem->MsgDetails.nChannel = 1;
            memset(pouMsgItem->MsgDetails.pchData, 0, def_MSG_DLC_MAX);
        }
    }
    return hResult;
}

bool CCANTxFormView::bGetMsgValidNameAndId(std::string& strMsgName, int nChannel,
        int& MsgType, int& nMsgId, IFrame*& pIFrame)
{
    bool bReturn = false;
    bool bValidId = false;
    nMsgId = nParseMsgId(strMsgName);
    if (nMsgId >= 0 && nMsgId < def_STD_MSG_ID_MAX)
    {
        bValidId = true;
        MsgType = CCANTxMsgItem::eCANSTD;
    }
    else if (nMsgId >= 0 && nMsgId < def_EXT_MSG_ID_MAX)
    {
        bValidId = true;
        MsgType = CCANTxMsgItem::eCANEXT;
    }
    if (bValidId)
    {
        bReturn = true;
        pIFrame = nullptr;
        if (EC_SUCCESS == m_pouIBMNetwork->GetFrame(CAN, nChannel, nMsgId, nullptr, &pIFrame))
        {
            strMsgName = std::string(strMsgName);
        }
    }

    return bReturn;
}
int& CCANTxFormView::GetValidMsgType(int& nMsgType, int nRow, int nId)
{
    CString omStrMsgType = m_lstMsg.GetItemText(nRow, eMsgColType);
    LVITEM lvitem;
    lvitem = { 0 };
    lvitem.mask = LVIF_IMAGE;
    lvitem.pszText = "";
    lvitem.iItem = nRow;
    lvitem.iSubItem = eMsgColRTR;
    m_lstMsg.GetItem(&lvitem);
    bool bIsRTR = (bool)lvitem.iImage;
    //If the id is not within the range of ext or std the type cannot be changed.
    if (omStrMsgType == def_MSG_TYPE_STD && nId >= 0 && nId < def_STD_MSG_ID_MAX)
    {
        nMsgType = CCANTxMsgItem::eCANSTD;
    }
    else if (omStrMsgType == def_MSG_TYPE_EXT && nId >= 0 && nId < def_EXT_MSG_ID_MAX)
    {
        nMsgType = CCANTxMsgItem::eCANEXT;
    }
    if (true == bIsRTR)
    {
        nMsgType |= CCANTxMsgItem::eCANRTR;
    }
    return nMsgType;
}
void CCANTxFormView::UpdateMsgListView(int nRow)
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
int CCANTxFormView::nParseMsgId(std::string strMsgNameId)
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
int CCANTxFormView::nParseIDFromName(std::string strMsgName)
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
bool CCANTxFormView::bGetKeyVal(int nItem, char& chKeyVal)
{
    bool bReturn = false;
    CString omStrKeyVal = m_lstMsg.GetItemText(nItem, eMsgColKey);
    if (omStrKeyVal.GetLength() > 0 )
    {
        chKeyVal = omStrKeyVal.GetAt(0);
        bReturn = std::isalnum(chKeyVal);
    }
    return bReturn;
}
void CCANTxFormView::vAddNewMessageCombo(int nRow)
{
    std::list<MSG_LST_ROW> lstMsgRowProp;
    if (S_OK == GetMsgRowProp(nRow, lstMsgRowProp))
    {
        SetMsgLstRow(lstMsgRowProp, nRow);
    }
}
int CCANTxFormView::OnMsgItemChanged(int nRow, int nColumn)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    CString omStrItem = m_lstMsg.GetItemText(nRow, nColumn);
    ITxMsgItem* pouMsgItem = nullptr;
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


    UCHAR ucData[defMAX_BYTE] = { 0 };
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
            break;
        case eMsgColName:
            pouMsgItem = new CCANTxMsgItem();
            if (S_OK == nGetDefaultMsgItem(std::string(omStrItem), pouMsgItem))
            {
                if (S_OK != m_pouTxDataStore->AddMsgItemAt(pouMsgItem, nRow))
                {
                    delete pouMsgItem;
                }
                if (nRow == nMsgLstCount)
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
            GetValidMsgType(pouMsgItem->MsgDetails.nMsgType,nRow,pouMsgItem->MsgDetails.nMsgId);
            m_pouTxDataStore->SetMsgItem(nRow, pouMsgItem);
            break;
        case eMsgColDLC:
            nDlc = atol(omStrItem);
            if (nDlc >= 0 && nDlc <= def_MSG_DLC_MAX)
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
            if (nChannel > 0 && nChannel <= CHANNEL_ALLOWED)
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

int CCANTxFormView::OnBusStatusChanged(ESTATUS_BUS eBusStatus)
{
    int hResult = S_FALSE;
    std::map<int, SLISTINFO> mapColAndProp;
    vGetColPropMap(eBusStatus, mapColAndProp);
    SetMsgLstCol(mapColAndProp);
    vUpdateButtonStatus();
    return hResult;
}
int CCANTxFormView::GetConfigTag(std::string& strTag)
{
    strTag = std::string(DEF_MODULE_CONFIGURATION) + "/" + std::string(DEF_CAN_TX_WINDOW);
    return S_OK;
}
int CCANTxFormView::AddNewMsgItemsToDataStore(int nCount)
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
        m_pouTxDataStore->AddMsgItemAt(new CCANTxMsgItem());
    }
    return hResult;
}
int CCANTxFormView::GetMsgListDetails(std::list<std::pair<std::string, int>>& lstMsgDetails)
{
    lstMsgDetails.push_back(std::make_pair("Message Name", 20));
    lstMsgDetails.push_back(std::make_pair("Frame Id", 6));
    lstMsgDetails.push_back(std::make_pair("Channel", 6));
    lstMsgDetails.push_back(std::make_pair("Data Length", 8));
    lstMsgDetails.push_back(std::make_pair("Message Type", 8));
    lstMsgDetails.push_back(std::make_pair("RTR", 3));
    lstMsgDetails.push_back(std::make_pair("Repetition (ms)", 8));
    lstMsgDetails.push_back(std::make_pair("Key", 5));

    return S_OK;
}
int CCANTxFormView::GetEndianess(std::pair<ISignal*, SignalInstanse>& Signal, eEndianess& endian)
{
    int hResult = S_OK;
    endian = Signal.second.m_ouSignalEndianess;
    return hResult;
}

int CCANTxFormView::SetOldFormatConfigData(xmlDocPtr pDoc)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    std::string strConfigTag = "";
    GetConfigTag(strConfigTag);
    xmlChar* pXpathChannel = (xmlChar*)strConfigTag.c_str();
    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetNodes(pDoc, pXpathChannel);
    if (pObjectPath != nullptr)
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        xmlNodePtr pNodeTxWindow = pNodeSet->nodeTab[0];
        if (nullptr != pNodeTxWindow)
        {
            xmlNodePtr pCurMsgBlocksTag = pNodeTxWindow->children;
            xmlNodePtr pCurMsgsTag = nullptr;
            int nMsgCount = 0;

            //Count the Total No of Messages in all Message Blocks.
            while (nullptr != pCurMsgBlocksTag)
            {
                if ((!xmlStrcmp(pCurMsgBlocksTag->name, (const xmlChar*)DEF_MSG_BLOCK)))
                {
                    pCurMsgsTag = pCurMsgBlocksTag->children;
                    while (nullptr != pCurMsgsTag)
                    {
                        if (!xmlStrcmp(pCurMsgsTag->name, (const xmlChar*)DEF_MSGS))
                        {
                            nMsgCount++;
                        }
                        pCurMsgsTag = pCurMsgsTag->next;
                    }
                }
                pCurMsgBlocksTag = pCurMsgBlocksTag->next;
            }

            if (nMsgCount > 0)
            {
                //Create DataStore Items for all Message Items.
                AddNewMsgItemsToDataStore(nMsgCount);
                m_pouTxDataStore->SetMsgItemsOldConfigData(pNodeTxWindow);
                AfxMessageBox(defSTR_WARN_TXWINDOW_UPGRADE, MB_ICONWARNING | MB_OK);
            }

        }

    }
    return hResult;
}