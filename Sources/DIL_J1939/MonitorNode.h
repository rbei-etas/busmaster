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
 * \file      MonitorNode.h
 * \brief     Definition file for Monitor Node
 * \author    Anish Kumar, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for Monitor Node
 */

#pragma once

/* Project includes */
#include "NodeConManager.h"
#include "ConnectionDet.h"

#define MAX_NODE_TO_MONITOR         56

class CMonitorNode : public CNodeConManager
{
private:
    CConnectionDet* m_pMonNodeConDetArr[MAX_NODE_TO_MONITOR];
private:
    CConnectionDet* pAddConDet(UINT unID);
    BOOL bProcessConLevelMsgByMon(const sTCANDATA& CurrMsgCAN);
    CConnectionDet* pGetConDet(BYTE bySrc, BYTE byDest);
    void vProcessBroadCastDataByMonNode(const sTCANDATA& CurrMsgCAN, CConnectionDet* pConDet);
    void vProcessLongDataByMonNode(const sTCANDATA& CurrMsgCAN, CConnectionDet* pConDet);
    BOOL bProcessNodeLevelMsgByMonNode(const STCANDATA& sCanData);
    void vTransmitMessage(STJ1939_MSG* psMsg);
public:
    CMonitorNode(int nNodeNo, char* pacNodeName, UINT64 un64NAME, BYTE byPrefAdres);
    ~CMonitorNode(void);
    void vRemoveAllConnections();
    BOOL bAddConDetObj(CConnectionDet* pConDet);
    void vReadCANdataBuffer();
    void vInitializeConnectionVar(UINT unCANID, CConnectionDet* const pConDet);
    void vProcessCANMsgByMonNode(const sTCANDATA CurrMsgCAN);
    CConnectionDet* pouCreateAndAddConnnection(UINT unId);
};
