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
* \file      CANTxFormView.h
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/


#pragma once
#include "ITxFormView.h"
#include "CANTxMsgItem.h"

class CCANTxFormView :public ITxFormView
{
private:
    static CCANTxFormView* m_pouCANTxFormView;
    static int m_nRefCounter;
    enum eMsgColumns
    {
        eMsgColName,
        eMsgColId,
        eMsgColChannel,
        eMsgColDLC,
        eMsgColType,
        eMsgColRTR,
        eMsgColRept,
        eMsgColKey,
        eMsgColAll
    };
    CCANTxFormView();
    ~CCANTxFormView();
    CCANTxFormView(const CCANTxFormView&);
    CCANTxFormView operator = (const CCANTxFormView&);

    int GetMsgRowProp(int nIndex, std::list<MSG_LST_ROW>& lstMsgRowProp);
    void vOnMsgTypeSelected(int nSelItem);
    void vGetColPropMap(ESTATUS_BUS eBusStatus, std::map<int, SLISTINFO>& mapColAndProp);
    void GetMsgTypeList(CStringArray& omStrMsgTypeList);
    void GetChannelList(CStringArray& omStrChannelList);
    void vGetDBMsgNameList(int nChannel, CStringArray& omStrArrayMsgNameList);
    int nGetMessageIDNames(int nChannelIndex, std::map<int, std::string>& ouMsgIDNamesMap);
    static void vCheckBoxClickHandler(CListCtrl* pList, int nItem, int nSubItem, void* UParam);
    int GetMsgItem(int nIndex, ITxMsgItem*& pMsgItem);
    int SetMsgItem(int nIndex, ITxMsgItem* pMsgItem);
    int nGetDefaultMsgItem(std::string strMsgName, ITxMsgItem*& pouLinMsgItem);

    bool bGetMsgValidNameAndId(std::string& strMsgName,int nChannel,int& MsgType,int& nMsgId, IFrame*& pIFrame);
    int& GetValidMsgType(int& nMsgType, int nRow, int nId);
    void UpdateMsgListView(int nRow);
    int nParseMsgId(std::string strMsgNameId);
    int nParseIDFromName(std::string strMsgName);
    bool bGetKeyVal(int nItem, char& chKeyVal);
    void vAddNewMessageCombo(int nRow);
public:
    static CCANTxFormView* GetInstance();
    static void ReleaseInstance();
    int SetClientId(unsigned long ulClientId);

    //Overidden functions from ITxFormView
    int LoadAllMsgItems();
    int OnMsgItemSelected(int nRow);
    int OnMsgItemChanged(int nRow, int nColumn);
    int OnBusStatusChanged(ESTATUS_BUS eBusStatus);
    int GetConfigTag(std::string& strTag);
    int AddNewMsgItemsToDataStore(int nCount);
    int GetMsgListDetails(std::list<std::pair<std::string, int>>& lstMsgDetails);
    int GetEndianess(std::pair<ISignal*, SignalInstanse>& Signal, eEndianess& endian);
    int SetOldFormatConfigData(xmlDocPtr pDoc);

};