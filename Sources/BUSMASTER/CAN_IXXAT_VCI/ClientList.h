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
 * @file ClientList.h
 *
 * This is a class to handle the registered clients.
 * In the original code of the busmaster DIL classes there
 * is array inside the CPP files. I add this extra class
 * to make it easy replaceable.
 */

#pragma once

#include "ClientBuffer.h"

/**
 * @brief
 *  List to managed the clients of the busmaster. E.g. CAN monitor.
 *
 */
class CClientList
{
public:
    CClientList(void);
    ~CClientList(void);
    void DeleteAllEntries();

    HRESULT RegisterClient(DWORD& pdwClientID, std::string pacClientName);
    HRESULT RemoveClient(DWORD dwClientID);
    CClientBuffer* GetClientByID(DWORD dwClientID);
    CClientBuffer* GetClientByIndex(int iIndexInList);

    HRESULT RemoveAllMsgBufOnAllClients();

    int Size()
    {
        return m_ClientBufferMap.size();

    }

protected:
    CClientBuffer* GetClient(std::string pacClientName);


protected:
    typedef std::map<DWORD, CClientBuffer*> ClientBufferMap;
    ClientBufferMap m_ClientBufferMap;

    DWORD m_dwUniqueClientID; ///< Key value for the clients, never decement this variable

};
