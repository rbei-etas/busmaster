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
 * @file
 *  ClientBuffer.h
 *
 * @brief
 *  Declares the client buffer class.
 */

#pragma once

#include "include/basedefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"

#include <vector>
using namespace std;

#ifdef _DEBUG
static DWORD g_dwIxxatBufObjectCounter = 0;
#endif

#define MAX_BUFF_ALLOWED 16



/**
 * @class CClientBuffer
 *
 * @brief
 *      Implements the client buffer class. This class includes a STL::vector
 *      which holds the pointers to the CBaseCANBufFSE* objects.
 *      Do not delete this CBaseCANBufFSE* objects because they were created
 *      by another function and should deleted by the creator.
 *
 */
class CClientBuffer
{
public:
    DWORD dwClientID; ///< Identifier for the client
    HANDLE hClientHandle; ///< Handle of the client
    HANDLE hPipeFileHandle; ///< Handle of the pipe file
    string m_pacClientName; ///< Name of the client

    HRESULT AddMsgBuf(CBaseCANBufFSE* pBufObj);
    HRESULT RemoveMsgBuf(CBaseCANBufFSE* pBufObj);
    HRESULT RemoveAllMsgBuf();

    CBaseCANBufFSE* GetSEBufferByIndex(int iBufIndex);

    int NumOfSEBuffers()
    {
        return m_BaseCANBufFSEVector.size();
    }

    CClientBuffer();
    CClientBuffer(DWORD dwClntID, HANDLE hClntHandle, HANDLE hPipeHandle, string szClientName);
    ~CClientBuffer();

protected:
    typedef std::vector<CBaseCANBufFSE*> BaseCANBufFSEVector;
    BaseCANBufFSEVector m_BaseCANBufFSEVector;

    //DWORD m_dwUniqueBufferID; ///< Key value for the clients, never decement this variable

    BOOL MsgBufExist(CBaseCANBufFSE* pBufObj);
};