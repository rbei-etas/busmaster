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
 * @file  ClientBuffer.cpp
 *
 * @brief
 *      Implements the client buffer class. This class includes a STL::vector
 *      which holds the pointers to the CBaseCANBufFSE* objects.
 *      Do not delete this CBaseCANBufFSE* objects because they were created
 *      by another function and should deleted by the creator.
 */

#include "CAN_IXXAT_VCI_stdafx.h"
#include "ClientBuffer.h"


/**
 * @brief
 *  Default constructor.
 *
 */
CClientBuffer::CClientBuffer()
{
#ifdef _DEBUG
    g_dwIxxatBufObjectCounter++;
#endif
    dwClientID = 0;
    hClientHandle = NULL;
    hPipeFileHandle = NULL;
    m_pacClientName = "";
}

/**
 * @brief
 *  Constructor with parameters.
 *
 * @param dwClntID
 *  Identifier for the client.
 * @param hClntHandle
 *  Handle of the client.
 * @param hPipeHandle
 *  Handle of the pipe.
 * @param szClientName
 *  Name of the client e.g. 'CAN_MONITOR'.
 */
CClientBuffer::CClientBuffer(DWORD dwClntID, HANDLE hClntHandle, HANDLE hPipeHandle, std::string szClientName)
{
#ifdef _DEBUG
    g_dwIxxatBufObjectCounter++;
#endif
    dwClientID = dwClntID;
    hClientHandle = hClntHandle;
    hPipeFileHandle = hPipeHandle;
    m_pacClientName = szClientName;
}


/**
 *
 * @brief
 *  Destructor. Clean the interal used sfl::vector.
 *
 */
CClientBuffer::~CClientBuffer()
{
    m_BaseCANBufFSEVector.clear();

#ifdef _DEBUG
    g_dwIxxatBufObjectCounter--;
#endif
}


/**
 * @brief
 *  Adds a new message buffer to the internal stl::vector.
 *
 * @param [in]  pBufObj
 *  If non-null, the buffer object.
 *
 * @return
 *  ERR_BUFFER_EXISTS - this buffer object is already in the list.
 *  S_OK - Successful added.
 *
 */
HRESULT CClientBuffer::AddMsgBuf(CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = ERR_BUFFER_EXISTS;
    if (!MsgBufExist(pBufObj))
    {
        m_BaseCANBufFSEVector.push_back(pBufObj);
        hResult = S_OK;
    }
    return hResult;
}

/**
 * @brief
 *  Removes a message buffer object from the internal std::vector.
 *
 * @param [in,out]  pBufObj
 *  If non-null, the buffer object.
 *
 * @return
 *  ERROR_NOT_FOUND - This object exitst no in internal list.
 *  S_OK - Successful removed.
 *
 */
HRESULT CClientBuffer::RemoveMsgBuf(CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = ERROR_NOT_FOUND;
    BaseCANBufFSEVector::iterator iter;
    for (iter = m_BaseCANBufFSEVector.begin(); iter != m_BaseCANBufFSEVector.end(); ++iter)
    {
        if (*iter == pBufObj)
        {
            m_BaseCANBufFSEVector.erase(iter);
            hResult = S_OK;
            break;
        }
    }
    return hResult;
}


/**
 * @brief
 *  Removes all message buffer. Clear the internal
 *  list complete.
 *
 * @return
 *  Always S_OK.
 *
 */
HRESULT CClientBuffer::RemoveAllMsgBuf()
{
    m_BaseCANBufFSEVector.clear();
    return S_OK;
}

/**
 * @brief
 *  Gets a buffer object by it's index in
 *  the internal list.
 *
 * @param iBufIndex
 *  Zero-based index of the buffer index.
 *
 * @return
 *  null if it fails, else the buffer object at this index.
 *
 */
CBaseCANBufFSE* CClientBuffer::GetSEBufferByIndex(int iBufIndex)
{
    CBaseCANBufFSE* pResultBufSE = NULL;
    if (iBufIndex < m_BaseCANBufFSEVector.size())
    {
        pResultBufSE = m_BaseCANBufFSEVector[iBufIndex];
    }

    return pResultBufSE;
}


/**
 * @brief
 *  Check if this message buffer exists.
 *
 * @param [in]  pBufObj
 *  If non-null, the buffer object.
 *
 * @return
 *  true - this object already exits
 *  false - object not found in internal list
 *
 */
BOOL CClientBuffer::MsgBufExist(CBaseCANBufFSE* pBufObj)
{
    BOOL bResult = FALSE;
    for (int i=0; i < (int)m_BaseCANBufFSEVector.size(); i++)
    {
        if (m_BaseCANBufFSEVector[i] == pBufObj)
        {
            bResult = TRUE;
            break;
        }
    }
    return bResult;
}