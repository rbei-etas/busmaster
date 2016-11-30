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
* \file      ITxMsgItem.h
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/

#pragma once
#include "stdafx.h"
#include "IBMNetWorkGetService.h"
#include "..\include/XMLDefines.h"
#include "Utility/XMLUtils.h"

struct TX_DETAILS
{
    bool bSelected;
    bool bKeyEnabled;
    char m_chKeyVal;
    bool m_bTimerEnabled;
    int nCurrentTimerVal;
    int nActualTimer;
    bool    m_bDataBaseMsg;
    TX_DETAILS()
    {
        m_bTimerEnabled = false;
        bKeyEnabled = false;
        bSelected = true;
        m_chKeyVal = 'a';
        nCurrentTimerVal = 10;
        nActualTimer = 10;
        m_bDataBaseMsg = false;
    }
};
struct MSG_DETAILS
{
    //std::string strMsgName;
    int nMsgId;
    int nChannel;
    int nDLC;
    int nMsgType;
    unsigned char* pchData;
    MSG_DETAILS()
    {
        //strMsgName = "";
        nMsgId = 0;
        nChannel = 0;
        nDLC = 0;
        nMsgType = 0;
        pchData = nullptr;
    }
};
struct SIG_DETAILS
{
    std::pair<ISignal*, SignalInstanse> Signal;
    InterpreteSignals SignalInterprete;
    double dPhyFactor;
};
struct ITxMsgItem
{
    TX_DETAILS TxDetails;
    MSG_DETAILS MsgDetails;
    virtual int GetMsgName(IBMNetWorkGetService* pouIBMNetwork, bool bIshex, std::string& strMsgName) = 0;
    virtual int SetMsgConfig(xmlNodePtr pxmlNodePtr) = 0;
    virtual int GetMsgConfig(xmlNodePtr pxmlNodePtr) = 0;
    virtual int SetOldMsgConfig(xmlNodePtr pxmlNodePtr) = 0;
    virtual int GetSignalList(IBMNetWorkGetService* pouIBMNetwork, bool bIsHex, std::list<SIG_DETAILS>& lstSigDetails) = 0;
    virtual int GetSignal(IBMNetWorkGetService* pouIBMNetwork, bool bIsHex, std::string strSigName, SIG_DETAILS& ouSignalDetails) = 0;
    virtual ~ITxMsgItem () {}
};
