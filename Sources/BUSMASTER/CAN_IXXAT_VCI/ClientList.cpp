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
 * @file  ClientList.cpp
 *
 * @brief
 *  This is a class to handle the registered clients.
 *  In the original code of the busmaster DIL classes there
 *  is array inside the CPP files. I add this extra class
 *  to make it easy replaceable.
 *  Here we use a MAP from the STL. Maybe this must be changed
 *  when using other C++ compilers.
 */
#include "CAN_IXXAT_VCI_stdafx.h"
#include "ClientList.h"

/**
 * @def
 *  MAX_CLIENT_ALLOWED
 *
 * @brief
 *  Gets the maximum client allowed.
 *
 * @remarks
 *  This define is in many CPP files. Maybe this should
 *  changed someday.
 */
#define MAX_CLIENT_ALLOWED 16

/**
 * @brief
 *  Default constructor. Set the class members to
 *  default values.
  */
CClientList::CClientList(void)
{
    // set it to 0, never never never decrement it!
    // make only increments for this variable
    m_dwUniqueClientID = 0;
}

/**
 * @brief
 *  Destructor.
 */
CClientList::~CClientList(void)
{
    // removing all entries from the map.
    m_ClientBufferMap.clear();
}

/**
 * @brief
 *  Deletes the entry objects and clear the map.
 */
void CClientList::DeleteAllEntries()
{
    ClientBufferMap::iterator iter;
    for (iter = m_ClientBufferMap.begin(); iter != m_ClientBufferMap.end(); ++iter)
    {
        delete iter->second;
        iter->second= nullptr;
    }
    m_ClientBufferMap.clear();
}

/**
 * @brief
 *  Registers a new client.
 *
 * @param [out]  pdwClientID
 *  Identifier for the the client.
 * @param [out]  pacClientName
 *  Name of the client.
 *
 * @return
 *  ERR_CLIENT_EXISTS - a client with this name already exits.
 *  ERR_NO_MORE_CLIENT_ALLOWED - the max. number of clients is reached.
 *  S_OK - client registered successful.
 *
 */
HRESULT CClientList::RegisterClient(DWORD& pdwClientID, std::string pacClientName)
{
    HRESULT hResult = S_FALSE;
    CClientBuffer* pClient = GetClient(pacClientName);
    if (!pClient)
    {
        // this client is not registered at this time
        if (m_ClientBufferMap.size() <= MAX_CLIENT_ALLOWED)
        {
            pdwClientID = m_dwUniqueClientID;

            // do not forget to delete this object later, we are the creator so we delete it!
            CClientBuffer* pClient = new CClientBuffer(m_dwUniqueClientID, nullptr, nullptr, pacClientName);
            m_ClientBufferMap[m_dwUniqueClientID] = pClient;
            m_dwUniqueClientID++;
            hResult = S_OK;
        }
        else
        {
            // we have already MAX_CLIENT_ALLOWED clients
            hResult = ERR_NO_MORE_CLIENT_ALLOWED;
        }

    }
    else
    {
        // a client with this name is already registered,
        // write the ID to the variable
        pdwClientID = pClient->dwClientID;
        hResult = ERR_CLIENT_EXISTS;
    }
    return hResult;
}


/**
 * @brief
 *  Removes the client described by dwClientID.
 *
 * @param dwClientID
 *  Identifier for the client.
 *
 * @return
 *  ERR_NO_CLIENT_EXIST - A client with this ID did not exists.
 *  S_OK - Client successful deleted and removed from the list.
 */
HRESULT CClientList::RemoveClient(DWORD dwClientID)
{
    HRESULT hResult = ERR_NO_CLIENT_EXIST;
    ClientBufferMap::iterator iter = m_ClientBufferMap.find(dwClientID);
    if (iter != m_ClientBufferMap.end())
    {
        delete iter->second;
        iter->second = nullptr;
        m_ClientBufferMap.erase(iter);
        hResult = S_OK;
    }
    return hResult;
}

/**
 * @brief
 *  Gets a client with the given identifier.
 *
 * @param dwClientID
 *  Identifier for the client.
 *
 * @return
 *  nullptr if it fails, else the client.
 */
CClientBuffer* CClientList::GetClientByID(DWORD dwClientID)
{
    CClientBuffer* pReturnClientObj = nullptr;
    if (m_ClientBufferMap.find(dwClientID) != m_ClientBufferMap.end())
    {
        pReturnClientObj = m_ClientBufferMap[dwClientID];
    }
    return pReturnClientObj;
}

/**
 * @brief
 *  Gets a client by the index in the list.
 *  Starting with 0 up to size() - 1,
 *
 * @param iIndexInList
 *  Index in the list.
 *
 * @return
 *  Null if it fails, else the client by index.
 */
CClientBuffer* CClientList::GetClientByIndex(int iIndexInList)
{
    CClientBuffer* pReturnClientObj = nullptr;
    ClientBufferMap::iterator iter;
    int iCurIndex = 0;
    for (iter = m_ClientBufferMap.begin(); iter != m_ClientBufferMap.end(); ++iter)
    {
        if (iCurIndex == iIndexInList)
        {
            pReturnClientObj = iter->second;
            break;
        }
        iCurIndex++;
    }
    return pReturnClientObj;

}

/**
 * @brief
 *  Get a client with the given name.
 *
 * @param [in]  pacClientName
 *  Name of the client.
 *
 * @return
 *  Null if it fails, else the client.
 */
CClientBuffer* CClientList::GetClient(std::string pacClientName)
{
    CClientBuffer* pReturnClientObj = nullptr;
    ClientBufferMap::iterator iter;
    for (iter = m_ClientBufferMap.begin(); iter != m_ClientBufferMap.end(); ++iter)
    {
        if (!iter->second->m_pacClientName.compare(pacClientName))
            // if (iter->second->m_pacClientName == pacClientName)
        {
            pReturnClientObj = iter->second;
            break;
        }
    }
    return pReturnClientObj;
}



/**
 * @brief
 *  Removes all message buffers on all clients.
 *
 * @return
 *  Always S_OK.
 *
 */
HRESULT CClientList::RemoveAllMsgBufOnAllClients()
{
    ClientBufferMap::iterator iter;
    for (iter = m_ClientBufferMap.begin(); iter != m_ClientBufferMap.end(); ++iter)
    {
        iter->second->RemoveAllMsgBuf();
    }
    return S_OK;
}