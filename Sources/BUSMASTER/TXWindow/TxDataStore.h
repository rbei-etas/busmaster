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
* \file      TxDataStore.h
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/

#pragma once
#include "stdafx.h"
#include "..\include/XMLDefines.h"
#include "Utility/XMLUtils.h"
#include "ITxMsgItem.h"



class CTxDataStore
{
    std::list<ITxMsgItem*> m_lstpMsgItem;
    CRITICAL_SECTION m_ouCSLstMsgItem;
public:
    CTxDataStore();
    ~CTxDataStore();

    virtual int GetMsgItem(int nIndex, ITxMsgItem*& pMsgItem);
    virtual int SetMsgItem(int nIndex, ITxMsgItem* pMsgItem);
    virtual int AddMsgItemAt(ITxMsgItem* pMsgItem, int nIndex = -1);
    virtual int DeleteMsgItemAt(int nIndex);
    virtual int DeleteAllMsgItems();
    virtual int GetMsgItemCount();


    virtual int SetMsgItemsConfigData(xmlNodePtr pxmlNodePtr);
    virtual int GetMsgItemsConfigData(xmlNodePtr pxmlNodePtr);

    //Support for Old Message Block format in .cfx for CAN
    virtual int SetMsgItemsOldConfigData(xmlNodePtr pxmlNodePtr);
};