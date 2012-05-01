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
 * \file      ReadCanMsg.h
 * \brief     Definition file for Read CAN Message
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for Read CAN Message
 */

#pragma once

#define DEF_MAX_SIMULATED_NODE 32

typedef CMap<HANDLE,HANDLE,BYTE,BYTE> CHandleToNodeMagrMap;
class CReadCanMsg
{
protected:
    CPARAM_THREADPROC m_ouThreadUtil;

    // To reset the object
    void vReset(void);
public:
    HANDLE m_ahActionEvent[DEF_MAX_SIMULATED_NODE + 1]; // One additional
    CHandleToNodeMagrMap m_omHandleToNodeMgrMap;
    int m_nEvents;

public:
    CReadCanMsg(void);
    ~CReadCanMsg(void);

    // event handle is the action event of the thread. This'll be necessary
    // while closing the thread.
    void vRetrieveDataFromBuffer(BYTE byIndex);

    // To add event handle for a client
    HRESULT AddEventHandle(HANDLE hHandle, BYTE byNodeMgrIndex);

    // To remove event handle of a client
    BOOL bDeleteEventHandle(HANDLE handle);

    // Do initialisation operations
    void vDoInit(void);

    // Do closure operations
    void vDoExit(void);
};
