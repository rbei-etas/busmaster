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
* \file      CANTxMsgItem.h
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/

#pragma once
#include "ITxMsgItem.h"

#define def_MSG_TYPE_STD                                "Std"
#define def_MSG_TYPE_EXT                                "Ext"
#define def_MSG_DLC_MAX                                  8
#define def_STD_MSG_ID_MAX                               0x800      //Std has 11- bit Identifier, So 2^11 in hex
#define def_EXT_MSG_ID_MAX                               0x20000000 //Ext has 29- bit Identifier, So 2^29 in hex

struct CCANTxMsgItem :public ITxMsgItem
{
    CCANTxMsgItem();
    ~CCANTxMsgItem();
    enum eCANMsgType
    {
        eCANSTD = 0x1,
        eCANEXT = 0x2,
        eCANRTR = 0x4
    };
    int GetMsgName(IBMNetWorkGetService* pouIBMNetwork, bool bIshex, std::string& strMsgName);
    int SetMsgConfig(xmlNodePtr pxmlNodePtr);
    int GetMsgConfig(xmlNodePtr pxmlNodePtr);
    int SetOldMsgConfig(xmlNodePtr pxmlNodePtr);
    int GetSignalList(IBMNetWorkGetService* pouIBMNetwork, bool bIsHex, std::list<SIG_DETAILS>& lstSigDetails);
    int GetSignal(IBMNetWorkGetService* pouIBMNetwork, bool bIsHex, std::string strSigName, SIG_DETAILS& ouSignalDetails);
private:
    std::string strGetValueAtTag(xmlNodePtr pxmlNodePtr, const std::string& strTag, int nodeTabIndex);
    void vGetPhysicalValueFactor(ISignal* pSignal, double& dScale);
    static bool sortByStartBit(const SIG_DETAILS& lhs, const SIG_DETAILS& rhs);
};