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
* \file      LINTxMsgItem.h
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/

#pragma once
#include "ITxMsgItem.h"

#define def_MSG_TYPE_MASTER                             "Master"
#define def_MSG_TYPE_SLAVE                              "Slave"
#define defLIN_MSG_DLC_MAX                               8
#define defLIN_MSG_ID_MAX                                63

struct CLINTxMsgItem :public ITxMsgItem
{
    CLINTxMsgItem();
    ~CLINTxMsgItem();
    int GetMsgName(IBMNetWorkGetService* pouIBMNetwork, bool bIshex, std::string& strMsgName);
    int SetMsgConfig(xmlNodePtr pxmlNodePtr);
    int GetMsgConfig(xmlNodePtr pxmlNodePtr);
    //This function is used to support old Msg Block Format used in CAN protocol.
    int SetOldMsgConfig(xmlNodePtr pxmlNodePtr);
    int GetSignalList(IBMNetWorkGetService* pouIBMNetwork, bool bIsHex, std::list<SIG_DETAILS>& lstSigDetails);
    int GetSignal(IBMNetWorkGetService* pouIBMNetwork, bool bIsHex, std::string strSigName, SIG_DETAILS& ouSignalDetails);
private:
    std::string strGetValueAtTag(xmlNodePtr pxmlNodePtr, const std::string& strTag);
    void vGetPhysicalValueFactor(ISignal* pSignal, double& dScale);
    static bool sortByStartBit(const SIG_DETAILS& lhs, const SIG_DETAILS& rhs);
};