

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

#if !defined MONITORNODE_H__INCLUDED_
#define MONITORNODE_H__INCLUDED_
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
    BOOL bProcessConLevelMsgByMon(const sTCANDATA& CurrMsgCAN);
    CConnectionDet* pGetConDet(BYTE bySrc, BYTE byDest);
    void vProcessBroadCastDataByMonNode(const sTCANDATA& CurrMsgCAN, CConnectionDet* pConDet);
    void vProcessLongDataByMonNode(const sTCANDATA& CurrMsgCAN, CConnectionDet* pConDet);
    BOOL bProcessNodeLevelMsgByMonNode(const STCANDATA& sCanData);
    void vTransmitMessage(STJ1939_MSG* psMsg);
public:
    CMonitorNode(int nNodeNo, TCHAR* pacNodeName, UINT64 un64NAME, BYTE byPrefAdres);
    ~CMonitorNode(void);
    void vRemoveAllConnections();
    BOOL bAddConDetObj(CConnectionDet* pConDet);
    void vReadCANdataBuffer();
    void vInitializeConnectionVar(UINT unCANID, CConnectionDet* const pConDet);
    void vProcessCANMsgByMonNode(const sTCANDATA CurrMsgCAN);
    CConnectionDet* pouCreateAndAddConnnection(UINT unId);
};

#endif //MONITORNODE_H__INCLUDED_