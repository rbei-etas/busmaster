#include "DIL_Interface_stdafx.h"
#include "CommanDIL_LIN.h"


CCommanDIL_LIN::CCommanDIL_LIN(void)
{
    InitializeCriticalSection(&sg_CritSectForAckBuf);
    m_unClientCnt = 0;
}


CCommanDIL_LIN::~CCommanDIL_LIN(void)
{
    DeleteCriticalSection(&sg_CritSectForAckBuf);
}

DWORD CCommanDIL_LIN::dwGetAvailableClientSlot()
{
    DWORD nClientId = 2;
    for (int i = 0; i < MAX_CLIENT_ALLOWED; i++)
    {
        if (bClientIdExist(nClientId))
        {
            nClientId += 1;
        }
        else
        {
            i = MAX_CLIENT_ALLOWED; // break the loop
        }
    }

    return nClientId;
}
BOOL CCommanDIL_LIN::bClientIdExist(const DWORD& dwClientId)
{
    BOOL bReturn = FALSE;
    for (UINT i = 0; i < m_unClientCnt; i++)
    {
        if (m_asClientToBufMap[i].dwClientID == dwClientId)
        {
            bReturn = TRUE;
            i = m_unClientCnt; // break the loop
        }
    }
    return bReturn;
}

BOOL CCommanDIL_LIN::bClientExist(string pcClientName, INT& Index)
{
    for (UINT i = 0; i < m_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName.c_str(), m_asClientToBufMap[i].pacClientName))
        {
            Index = i;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CCommanDIL_LIN::bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;
    for (UINT i = 0; i < m_unClientCnt; i++)
    {
        if (m_asClientToBufMap[i].dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = m_unClientCnt; // break the loop
            bResult = TRUE;
        }
    }
    return bResult;
}
BOOL CCommanDIL_LIN::bIsBufferExists(const SLINCLIENTBUFMAP& sClientObj, const CBaseLINBufFSE* pBuf)
{
    BOOL bExist = FALSE;
    for (UINT i = 0; i < sClientObj.unBufCount; i++)
    {
        if (pBuf == sClientObj.pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.unBufCount; // break the loop
        }
    }

    return bExist;
}

BOOL CCommanDIL_LIN::bRemoveClientBuffer(CBaseLINBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseLINBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;
    for (UINT i = 0; i < unCount; i++)
    {
        if (RootBufferArray[i] == BufferToRemove)
        {
            if (i < (unCount - 1)) /* If not the last bufffer */
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }
            unCount--;
        }
    }
    return bReturn;
}

BOOL CCommanDIL_LIN::bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (m_unClientCnt > 0)
    {
        UINT unClientIndex = (UINT)-1;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            /* clear the client first */
            if (m_asClientToBufMap[unClientIndex].hClientHandle != NULL)
            {
                HRESULT hResult = S_OK;//(*pfCAN_RemoveClient)(sg_asClientToBufMap[unClientIndex].hClientHandle);
                if (hResult == S_OK)
                {
                    m_asClientToBufMap[unClientIndex].dwClientID = 0;
                    m_asClientToBufMap[unClientIndex].hClientHandle = NULL;
                    memset (m_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                    for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                    {
                        m_asClientToBufMap[unClientIndex].pClientBuf[i] = NULL;
                    }
                    m_asClientToBufMap[unClientIndex].unBufCount = 0;
                    bResult = TRUE;
                }
                else
                {
                    //vRetrieveAndLog(hResult, __FILE__, __LINE__);
                }
            }
            else
            {
                m_asClientToBufMap[unClientIndex].dwClientID = 0;
                memset (m_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                {
                    m_asClientToBufMap[unClientIndex].pClientBuf[i] = NULL;
                }
                m_asClientToBufMap[unClientIndex].unBufCount = 0;
                bResult = TRUE;

            }
            if (bResult == TRUE)
            {
                if ((unClientIndex + 1) < m_unClientCnt)
                {
                    m_asClientToBufMap[unClientIndex] = m_asClientToBufMap[m_unClientCnt - 1];
                }
                m_unClientCnt--;
            }
        }
    }
    return bResult;
}

HRESULT CCommanDIL_LIN::LIN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    if (bRegister)
    {
        if (m_unClientCnt < MAX_CLIENT_ALLOWED)
        {
            INT Index = 0;
            if (!bClientExist(pacClientName, Index))
            {
                /* Currently store the client information */
                if (_tcscmp(pacClientName, LIN_MONITOR_NODE) == 0)
                {
                    /* First slot is reserved to monitor node */
                    ClientID = 1;
                    _tcscpy(m_asClientToBufMap[0].pacClientName, pacClientName);
                    m_asClientToBufMap[0].dwClientID = ClientID;
                    m_asClientToBufMap[0].unBufCount = 0;
                }
                else
                {
                    if (!bClientExist(LIN_MONITOR_NODE, Index))
                    {
                        Index = m_unClientCnt + 1;
                    }
                    else
                    {
                        Index = m_unClientCnt;
                    }
                    ClientID = dwGetAvailableClientSlot();
                    _tcscpy(m_asClientToBufMap[Index].pacClientName, pacClientName);

                    m_asClientToBufMap[Index].dwClientID = ClientID;
                    m_asClientToBufMap[Index].unBufCount = 0;
                }
                m_unClientCnt++;
                hResult = S_OK;
            }
            else
            {
                ClientID = m_asClientToBufMap[Index].dwClientID;
                hResult = ERR_CLIENT_EXISTS;
            }
        }
        else
        {
            hResult = ERR_NO_MORE_CLIENT_ALLOWED;
        }
    }
    else
    {
        if (bRemoveClient(ClientID))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }

    return hResult;
}

HRESULT CCommanDIL_LIN::LIN_SendMsg(DWORD dwClientID, STLIN_MSG& pouFlxTxMsg)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    SACK_MAP sAckMap;
    HRESULT hResult = S_FALSE;
    if (bClientIdExist(dwClientID))
    {
        hResult = LIN_Send(pouFlxTxMsg);
        if ( hResult == S_OK )
        {
            sAckMap.m_unClientID = dwClientID;
            sAckMap.m_unMsgID    = pouFlxTxMsg.m_ucMsgID;
            sAckMap.m_unChannel  = pouFlxTxMsg.m_ucChannel;

            /* Mark an entry in Map. This is helpful to identify
               which client has been sent this message in later stage */
            vMarkEntryIntoMap(sAckMap);
        }
        else
        {
            LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
            return hResult;
        }
    }
    else
    {
        hResult = ERR_NO_CLIENT_EXIST;
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer

    return hResult;
}

HRESULT CCommanDIL_LIN::LIN_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG& pouFlxTxMsg)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    SACK_MAP sAckMap;
    HRESULT hResult = S_FALSE;
    if (bClientIdExist(dwClientID))
    {
        hResult = LIN_DisableSlave(pouFlxTxMsg);
        LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
        return hResult;
    }
    else
    {
        hResult = ERR_NO_CLIENT_EXIST;
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer

    return hResult;

}

HRESULT CCommanDIL_LIN::LIN_ManageMsgBuf(BYTE byAction, DWORD ClientID, CBaseLINBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (ClientID != NULL)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SLINCLIENTBUFMAP& sClientObj = m_asClientToBufMap[unClientIndex];
            if (byAction == MSGBUF_ADD)
            {
                /* Add msg buffer */
                if (pBufObj != NULL)
                {
                    if (sClientObj.unBufCount < MAX_BUFF_ALLOWED)
                    {
                        if (bIsBufferExists(sClientObj, pBufObj) == FALSE)
                        {
                            sClientObj.pClientBuf[sClientObj.unBufCount++] = pBufObj;
                            hResult = S_OK;
                        }
                        else
                        {
                            hResult = ERR_BUFFER_EXISTS;
                        }
                    }
                }
            }
            else if (byAction == MSGBUF_CLEAR)
            {
                /* clear msg buffer */
                if (pBufObj != NULL)
                {
                    /* Remove only buffer mentioned */
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else
                {
                    /* Remove all */
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = NULL;
                    }
                    sClientObj.unBufCount = 0;
                }
                hResult = S_OK;
            }
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    else
    {
        if (byAction == MSGBUF_CLEAR)
        {
            /* clear msg buffer */
            for (UINT i = 0; i < m_unClientCnt; i++)
            {
                LIN_ManageMsgBuf(MSGBUF_CLEAR, m_asClientToBufMap[i].dwClientID, NULL);
            }
            hResult = S_OK;
        }
    }

    return hResult;
}


void CCommanDIL_LIN::vWriteIntoClientsBuffer(STLINDATA& sLinData)
{
    //Write into the client's buffer and Increment message Count
    SACK_MAP sAckMap;
    UINT ClientId = 0;
    UINT Index = (UINT)-1;
    sAckMap.m_unChannel = sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel;;
    sAckMap.m_unMsgID = sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID;

    //Check if it is an acknowledgement message
    if ( (sLinData.m_eLinMsgType == LIN_MSG) &&
            ( (sLinData.m_ucDataType == TX_FLAG) &&  vPeakMapEntry(sAckMap, ClientId, false ) ) )
    {
        BOOL bClientExists = bGetClientObj(ClientId, Index);
        UINT i;
        for (i = 0; i < m_unClientCnt; i++)
        {
            //Tx for sender node
            if ((i == CAN_MONITOR_NODE_INDEX)  || (bClientExists && (i == Index)))
            {
                sLinData.m_ucDataType = TX_FLAG;
                sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgTyp = TX_FLAG;
                for (UINT j = 0; j < m_asClientToBufMap[i].unBufCount; j++)
                {
                    m_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sLinData);
                }
            }
            else
            {
                //Send the other nodes as Rx.
                if ( (i == 1)&& (sAckMap.m_unMsgID == 0x12) )
                {
                    int  a =0;
                }
                else
                {
                    for (UINT j = 0; j < m_asClientToBufMap[i].unBufCount; j++)
                    {
                        //static STLINDATA sTempCanData;
                        sLinData.m_ucDataType = RX_FLAG;
                        sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgTyp = RX_FLAG;
                        m_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sLinData);
                    }
                }
            }
        }
    }
    else // provide it to everybody
    {
        for (UINT i = 0; i < m_unClientCnt; i++)
        {
            for (UINT j = 0; j < m_asClientToBufMap[i].unBufCount; j++)
            {
                m_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sLinData);
            }
        }
    }
}

UCHAR CCommanDIL_LIN::ucCalculateClassicChecksum(SLIN_CRC sCrc)
{
    /*sCrc.ucData[0] = 0x4A;
    sCrc.ucData[1] = 0x55;
    sCrc.ucData[2] = 0x93;
    sCrc.ucData[3] = 0xE5;*/

    UINT ucChecksum = sCrc.ucData[0];
    UCHAR ucMask = 0xFF;
    for(INT unIndex = 1; unIndex < sCrc.unDlc; unIndex++)
    {
        UCHAR ucDatabyte = sCrc.ucData[unIndex];

        ucChecksum += ucDatabyte;
        UCHAR ucCarry = ucChecksum / ucMask;

        if(ucCarry != 0 )
        {
            ucChecksum = ucChecksum - ucMask;
        }
    }

    ucChecksum = ~ucChecksum;
    return ucChecksum;
}

UCHAR CCommanDIL_LIN::ucCalculatePID(UCHAR ucId)
{
    char chChecksum = ucId;
    char chMask[8], byte[8];
    for(INT nIndex = 0; nIndex < 8; nIndex++)
    {
        chMask[nIndex] = nIndex+1;
        byte[nIndex] = ((chChecksum) >> nIndex) & 1;
    }

    char chP0, chP1;
    // chP0 = ID0+ID1+ID2+ID4
    chP0 = byte[0] ^ byte[1] ^ byte[2] ^ byte[4];


    // chP1 = ID1+ID3+ID4+ID5
    chP1 = byte[1] + byte[3] + byte[4] + byte[5];

    chP1 = ~chP1;

    //00010110 -> p00010110
    chP0 = chP0 << 6;
    chP1 = chP1 << 7;
    chChecksum = chChecksum + chP0 + chP1;
    ucId = chChecksum;
    return ucId;
}

UCHAR CCommanDIL_LIN::ucCalculateEnhancedChecksum(SLIN_CRC sCrc)
{
    UINT ucChecksum = ucCalculatePID(sCrc.unID);
    UCHAR ucMask = 0xFF;
    for(INT unIndex = 0; unIndex < sCrc.unDlc; unIndex++)
    {
        UCHAR ucDatabyte = sCrc.ucData[unIndex];

        ucChecksum += ucDatabyte;
        UCHAR ucCarry = ucChecksum / ucMask;

        if(ucCarry != 0 )
        {
            ucChecksum = ucChecksum - ucMask;
        }
    }

    ucChecksum = ~ucChecksum;
    return ucChecksum;
}

UCHAR CCommanDIL_LIN::ucChecksumCalculation(SLIN_CRC sCrc)
{
    UCHAR ucChecksum = 0;
    switch(sCrc.unCrcType)
    {
        case LIN_CHECKSUM_CLASSIC:
            ucChecksum = ucCalculateClassicChecksum(sCrc);
            break;
        case LIN_CHECKSUM_ENHANCED:

            ucChecksum = ucCalculateEnhancedChecksum(sCrc);
            break;
    }
    return ucChecksum;
}

void CCommanDIL_LIN::vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    //EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    m_MapIdClient[RefObj.m_unChannel].insert(map<int, int>::value_type(RefObj.m_unMsgID, RefObj.m_unClientID));
    //LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
}
bool CCommanDIL_LIN::vPeakMapEntry(const SACK_MAP& RefObj, UINT& ClientID, bool bRemove)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    BOOL bResult = FALSE;
    map<int, int>::iterator  iResult = m_MapIdClient[RefObj.m_unChannel].find(RefObj.m_unMsgID);

    //if ((*iResult).m_ClientID > 0)
    if (iResult != m_MapIdClient[RefObj.m_unChannel].end())
    {
        bResult = TRUE;
        ClientID = iResult->second;
        if ( bRemove == true )
        {
            m_MapIdClient[RefObj.m_unChannel].erase(iResult);
        }
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
    return bResult;
}