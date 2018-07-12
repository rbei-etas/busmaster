

/******************************************************************************
  Project       :  Frame_McNet
  FileName      :  MonitorNode.h
  Description   :  MAnage Monitor Node
  $Log:

  Author(s)     :  Anish Kumar
  Date Created  :  05/01/2010
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#pragma once

#include "NodeConManager.h"
#include "ConnectionDet.h"

#define MAX_NODE_TO_MONITOR         56

class CMonitorNode : public CNodeConManager
{
private:
    CConnectionDet* m_pMonNodeConDetArr[MAX_NODE_TO_MONITOR];
private:
    CConnectionDet* pAddConDet(UINT unID);
    BOOL bProcessConLevelMsgByMon(const STCANDATA& CurrMsgCAN);
    CConnectionDet* pGetConDet(BYTE bySrc, BYTE byDest);
    void vProcessBroadCastDataByMonNode( const STCANDATA& CurrMsgCAN, CConnectionDet* pConDet );
    void vProcessLongDataByMonNode( const STCANDATA& CurrMsgCAN, CConnectionDet* pConDet );
    BOOL bProcessNodeLevelMsgByMonNode(const STCANDATA& sCanData);
    void vTransmitMessage(STJ1939_MSG* psMsg);
public:
    CMonitorNode(int nNodeNo, char* pacNodeName, UINT64 un64NAME, BYTE byPrefAdres);
    ~CMonitorNode(void);
    void vRemoveAllConnections();
    BOOL bAddConDetObj(CConnectionDet* pConDet);
    void vReadCANdataBuffer();
    void vInitializeConnectionVar(UINT unCANID, CConnectionDet* const pConDet);
    void vProcessCANMsgByMonNode( const STCANDATA CurrMsgCAN );
    CConnectionDet* pouCreateAndAddConnnection(UINT unId);
};
