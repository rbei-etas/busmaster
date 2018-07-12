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
 * \file      SetResetTimer.h
 * \brief     This file contain declaration of all function and data
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain declaration of all function and data
 */

#pragma once

#include "NodeSimEx_resource.h"
#include "HashDefines.h"
#include "GlobalObj.h"

class CTransmitMsg;

typedef struct tagSCALCEXECTIMERTHREAD
{
    bool            m_bThreadStop;
    CWinThread*     m_pomThreadPtr;
    CEvent          m_omExitThreadEvent;
    CTransmitMsg*   m_pTransmitMsg;
    ETYPE_BUS       m_eBus; //Added for issue #356
    CGlobalObj*     m_pGlobalObj;
    CExecuteManager* m_pExecuteMgr;

    tagSCALCEXECTIMERTHREAD()
    {
        m_bThreadStop  = FALSE;
        m_pomThreadPtr = nullptr;
        m_pTransmitMsg = nullptr;
        m_pGlobalObj = nullptr;
        m_pExecuteMgr = nullptr;
        m_eBus = CAN;
    }

} SCALCEXECTIMERTHREAD,*PSCALCEXECTIMERTHREAD;

/////////////////////////////////////////////////////////////////////////////
// CSetResetTimer dialog

class CSetResetTimer
{
    // Construction
public:
    CSetResetTimer(ETYPE_BUS eBus, CGlobalObj* pGlobalObj, CExecuteManager* pExecuteMgr); // standard constructor
    void vSetResetAllTimers(CString om_StrNode,BOOL bEnable);
    void vStopTimer();
    void vStartTimer();
    void vAddNewNodeTimerList(CString om_NodeName,PSTIMERHANDLERLIST psTimerStrList);
    void vDeleteNodeTimerList(CString om_NodeName);
    void vRestoreTimerStatus(CString omNodeName,
                             PSTIMERHANDLERLIST psTimerListPtr);

private:
    void vDeleteTimerListOfNode(PSTIMERHANDLERLIST psListOfTimer);

    ETYPE_BUS m_eBus;
    PSNODETIMERLIST m_psFirstNodeTimerList;
    PSNODETIMERLIST m_psLastNodeTimerList;
    //For storing the ID of timer
    UINT m_unTimerID;
    CWinThread* ExecuteThread1 ;
    CStringArray m_omSysNodeName;          //store node from simulated system
    BOOL m_bSimSysNodes;                   //if dialog box from simsys or mainframe
    PSCALCEXECTIMERTHREAD m_psCalTimerThreadStruct;
public:
    static ETYPE_BUS sm_eBus;
    CGlobalObj* m_pGlobalObj;
    CExecuteManager* m_pExecuteMgr;
};
