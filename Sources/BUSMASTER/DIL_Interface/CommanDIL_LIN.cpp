#include "DIL_Interface_stdafx.h"
#include "CommanDIL_LIN.h"
#include "Error.h"

CCommanDIL_LIN::CCommanDIL_LIN(void)
{
    InitializeCriticalSection(&sg_CritSectForAckBuf);
    m_ouTransmitThread.m_pBuffer = this;
    m_ouTransmitThread.m_hActionEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_unClientCnt = 0;
    InitializeCriticalSection(&m_ouCriticalSection);
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

BOOL CCommanDIL_LIN::bClientExist(std::string pcClientName, INT& Index)
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
            if (m_asClientToBufMap[unClientIndex].hClientHandle != 0)
            {
                HRESULT hResult = S_OK;//(*pfCAN_RemoveClient)(sg_asClientToBufMap[unClientIndex].hClientHandle);
                if (hResult == S_OK)
                {
                    m_asClientToBufMap[unClientIndex].dwClientID = 0;
                    m_asClientToBufMap[unClientIndex].hClientHandle = 0;
                    memset (m_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                    for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                    {
                        m_asClientToBufMap[unClientIndex].pClientBuf[i] = nullptr;
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
                    m_asClientToBufMap[unClientIndex].pClientBuf[i] = nullptr;
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
HRESULT CCommanDIL_LIN::LIN_PerformClosureOperations(void)
{
    m_ouTransmitThread.bTerminateThread();      //Created or Not
    PerformClosureOperations();
    return S_OK;
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
                    /*if (!bClientExist(LIN_MONITOR_NODE, Index))
                     {
                         Index = m_unClientCnt + 1;
                     }
                     else
                     {
                         Index = m_unClientCnt;
                     }*/
                    Index = m_unClientCnt;
                    ClientID = dwGetAvailableClientSlot();
                    if ( Index < sizeof(m_asClientToBufMap)/sizeof(m_asClientToBufMap[0]) )
                    {
                        _tcscpy(m_asClientToBufMap[Index].pacClientName, pacClientName);

                        m_asClientToBufMap[Index].dwClientID = ClientID;
                        m_asClientToBufMap[Index].unBufCount = 0;
                    }
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
    if (ClientID != 0)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SLINCLIENTBUFMAP& sClientObj = m_asClientToBufMap[unClientIndex];
            if (byAction == MSGBUF_ADD)
            {
                /* Add msg buffer */
                if (pBufObj != nullptr)
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
                if (pBufObj != nullptr)
                {
                    /* Remove only buffer mentioned */
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else
                {
                    /* Remove all */
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = nullptr;
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
                LIN_ManageMsgBuf(MSGBUF_CLEAR, m_asClientToBufMap[i].dwClientID, nullptr);
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
            ( (sLinData.m_ucDataType == TX_FLAG) &&  vPeakMapEntry(sAckMap, ClientId, false ) ) ) /*Do not remove entry in map.Client will send the message only once so... */
    {
        BOOL bClientExists = bGetClientObj(ClientId, Index);
        UINT i;
        for (i = 0; i < m_unClientCnt; i++)
        {
            //Tx for sender node
            if ((i == LIN_MONITOR_NODE_INDEX)  || (bClientExists && (i == Index)))
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
                /*if ( (i == 1)&& (sAckMap.m_unMsgID == 0x12) )
                {
                int  a =0;
                }
                else*/
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
    UINT ucChecksum = sCrc.ucData[0];
    UCHAR ucMask = 0xFF;
    for(unsigned int unIndex = 1; unIndex < sCrc.unDlc; unIndex++)
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
    m_MapIdClient[RefObj.m_unChannel].insert(std::map<int, int>::value_type(RefObj.m_unMsgID, RefObj.m_unClientID));
    //LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
}

bool CCommanDIL_LIN::vPeakMapEntry(const SACK_MAP& RefObj, UINT& ClientID, bool bRemove)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    bool bResult = false;
    std::map<int, int>::iterator  iResult = m_MapIdClient[RefObj.m_unChannel].find(RefObj.m_unMsgID);

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

HRESULT CCommanDIL_LIN::LIN_SetConfigData(IBMNetWorkGetService* ouConfig)
{
    int nChannels = 0;
    m_pBMNetwork = ouConfig;
    ouConfig->GetChannelCount(LIN, nChannels);
    ICluster* pCluster;
    unsigned int unId = 0;
    unsigned int unLength = 0;
    for ( int i = 0 ; i < nChannels; i++ )
    {
        ouConfig->GetDBService(LIN, i, 0, &pCluster);
        if ( nullptr != pCluster )
        {
            std::map<UID_ELEMENT, IElement*> ouFrameList;

            pCluster->GetElementList(eFrameElement, ouFrameList);

            memset(m_ucConfiguredMasterDlc[i], 0xFF, sizeof(m_ucConfiguredMasterDlc[i]));

for ( auto ouFrame : ouFrameList )
            {
                ((IFrame*)ouFrame.second)->GetLength(unLength);
                ((IFrame*)ouFrame.second)->GetFrameId(unId);
                m_ucConfiguredMasterDlc[i][unId] = unLength;
            }
        }
    }
    return SetConfigData(ouConfig);
}



//Schedule table
HRESULT CCommanDIL_LIN::LIN_RegisterLinScheduleTable(DWORD& dwClientId, int& nChannel, CSheduleTable ouTable, int& nHandle)
{

    if (bClientIdExist(dwClientId) == false)
    {
        return ERR_NO_CLIENT_EXIST;
    }
    nChannel--;
    if ( nChannel >= 0 && nChannel < CHANNEL_ALLOWED )
    {
        EnterCriticalSection(&m_ouCriticalSection);
        nHandle = ++(m_ouScheduleController[nChannel].m_nTotalTables);
        m_ouScheduleController[nChannel].m_ouScheduleTableList[nHandle] = ouTable;


        //Register All Commands
        std::list<CScheduleCommands>::iterator itrCommands = ouTable.m_listCommands.begin();
        STLIN_MSG ouMsg;
        ouMsg.m_ucChannel = nChannel+1;
        ouMsg.m_ucMsgTyp = LIN_MASTER_RESPONSE;
for(auto itrCommands : ouTable.m_listCommands)
        {
            ouMsg.m_ucMsgID= itrCommands.m_nId;
            ouMsg.m_ucDataLen = m_ucConfiguredMasterDlc[nChannel][itrCommands.m_nId];
            memset(ouMsg.m_ucData, 0xFF, sizeof(ouMsg.m_ucData));
            if ( ouMsg.m_ucMsgID == 0x3C)
            {
                ouMsg.m_ucMsgTyp = LIN_SLAVE_RESPONSE;
                ouMsg.m_ucDataLen = 8;
                memcpy(ouMsg.m_ucData, m_ouScheduleController[nChannel].m_ouCurrentCommand.m_listIDs, 8);
                LIN_Send(ouMsg);
            }
            ouMsg.m_ucMsgTyp = LIN_MASTER_RESPONSE;
            LIN_Send(ouMsg);
        }
        LeaveCriticalSection(&m_ouCriticalSection);
    }
    else
    {
        return ERR_INVALID_CHANNEL;
    }
    return S_OK;
}

HRESULT CCommanDIL_LIN::LIN_DeRegisterLinScheduleTabel(DWORD& dwClientId, int& nChannel, int& nTableHandle)
{

    if (bClientIdExist(dwClientId) == false)
    {
        return ERR_NO_CLIENT_EXIST;
    }

    if ( nChannel >= 0 && nChannel < CHANNEL_ALLOWED )
    {
        EnterCriticalSection(&m_ouCriticalSection);
        std::map<int, CSheduleTable>::iterator itr = m_ouScheduleController[nChannel].m_ouScheduleTableList.find(nTableHandle);
        if ( itr != m_ouScheduleController[nChannel].m_ouScheduleTableList.end() )
        {
            m_ouScheduleController[nChannel].m_ouScheduleTableList.erase(itr);
        }
        LeaveCriticalSection(&m_ouCriticalSection);
    }
    else
    {
        return ERR_INVALID_CHANNEL;
    }
    return S_OK;
}



HRESULT CCommanDIL_LIN::LIN_StartLinScheduleTable(DWORD& dwClientId, int& nChannel, int& nTableHandle)
{
    nChannel--;
    if (bClientIdExist(dwClientId) == false)
    {
        return ERR_NO_CLIENT_EXIST;
    }

    HRESULT hr = S_FALSE;
    if ( nChannel >= 0 && nChannel < CHANNEL_ALLOWED )
    {
        EnterCriticalSection(&m_ouCriticalSection);
        if ( m_ouScheduleController[nChannel].m_nCurrentHandle == nTableHandle )
        {
            hr = S_OK;
        }
        else
        {
            std::map<int, CSheduleTable>::iterator itr = m_ouScheduleController[nChannel].m_ouScheduleTableList.find(nTableHandle);
            if ( itr != m_ouScheduleController[nChannel].m_ouScheduleTableList.end() )
            {
                std::list<CScheduleCommands>::iterator itrCommand = itr->second.m_listCommands.begin();
                if ( itrCommand != itr->second.m_listCommands.end() )
                {
                    m_ouScheduleController[nChannel].m_ouCurrentCommand = *itrCommand;
                    m_ouScheduleController[nChannel].m_nCurrentHandle = nTableHandle;
                    m_ouScheduleController[nChannel].m_nCurrentCommandIndex = 0;
                    m_ouScheduleController[nChannel].m_nCurrentDelay = static_cast<int>(itrCommand->m_fDelay);
                }
            }
            hr = S_OK;
        }
        LeaveCriticalSection(&m_ouCriticalSection);
    }
    else
    {
        hr = ERR_INVALID_CHANNEL;
    }
    return hr;
}

HRESULT CCommanDIL_LIN::LIN_UpdateLinScheduleTable( DWORD& dwClientId, int& nChannel, int& nTableHandle, CSheduleTable& ouTable )
{

    if (bClientIdExist(dwClientId) == false)
    {
        return ERR_NO_CLIENT_EXIST;
    }

    if ( nChannel >= 0 && nChannel < CHANNEL_ALLOWED )
    {
        EnterCriticalSection(&m_ouCriticalSection);
        std::map<int, CSheduleTable>::iterator itr = m_ouScheduleController[nChannel].m_ouScheduleTableList.find(nTableHandle);
        if ( itr != m_ouScheduleController[nChannel].m_ouScheduleTableList.end() )
        {
            itr->second = ouTable;
        }
        LeaveCriticalSection(&m_ouCriticalSection);
    }
    else
    {
        return ERR_INVALID_CHANNEL;
    }
    return S_OK;
}


HRESULT CCommanDIL_LIN::LIN_EnableLinScheuleCommand( DWORD& dwClientId, int& nChannel, int nTableHandle, int nIndex, bool bEnable )
{
    EnterCriticalSection(&m_ouCriticalSection);
    std::map<int, CSheduleTable>::iterator itrTable = m_ouScheduleController[nChannel].m_ouScheduleTableList.find(nTableHandle);
    if ( itrTable != m_ouScheduleController[nChannel].m_ouScheduleTableList.end() )
    {
        std::list<CScheduleCommands>::iterator itrCommand = itrTable->second.m_listCommands.begin();
        advance(itrCommand, nIndex);
        if ( itrCommand != itrTable->second.m_listCommands.end() )
        {
            itrCommand->m_bEnabled = bEnable;
        }
    }
    LeaveCriticalSection(&m_ouCriticalSection);
    return S_OK;
}


//Individual Header commands.
HRESULT CCommanDIL_LIN::LIN_RegisterLinHeader( DWORD& dwClientId, int& nChannel, int nId, int nCycleTimer )
{
    if (bClientIdExist(dwClientId) == false)
    {
        return ERR_NO_CLIENT_EXIST;
    }

    if ( nChannel >= 0 && nChannel < CHANNEL_ALLOWED )
    {
        sIdDelay ouidDelay;
        ouidDelay.m_nActualDelay = ouidDelay.m_nCurrentDelay = nCycleTimer;

        m_ouScheduleController[nChannel].m_ouHeaderCommands[nId] = ouidDelay;
    }
    else
    {
        return ERR_INVALID_CHANNEL;
    }
    return S_OK;
}

HRESULT CCommanDIL_LIN::LIN_DeRegisterLinHeader( DWORD& dwClientId, int& nChannel, int nId)
{
    if (bClientIdExist(dwClientId) == false)
    {
        return ERR_NO_CLIENT_EXIST;
    }

    if ( nChannel >= 0 && nChannel < CHANNEL_ALLOWED )
    {
        std::map<int, sIdDelay>::iterator itr = m_ouScheduleController[nChannel].m_ouHeaderCommands.find(nId);
        if ( itr!= m_ouScheduleController[nChannel].m_ouHeaderCommands.end() )
        {
            m_ouScheduleController[nChannel].m_ouHeaderCommands.erase(itr);
        }
    }
    else
    {
        return ERR_INVALID_CHANNEL;
    }
    return S_OK;
}


HRESULT CCommanDIL_LIN::LIN_StartHardware(void)
{
    StartHardware();
    m_ouTransmitThread.m_unActionCode = INVOKE_FUNCTION;
    m_ouTransmitThread.bStartThread(LINTxWndTransmitThread);
    SetEvent(m_ouTransmitThread.m_hActionEvent);
    return S_OK;
}

HRESULT CCommanDIL_LIN::LIN_PreStartHardware(void)
{
    PreStartHardware();
    return S_OK;
}

HRESULT CCommanDIL_LIN::LIN_StopHardware(void)
{
    m_ouTransmitThread.m_unActionCode = SUSPEND;
    for ( int nChannel = 0; nChannel < CHANNEL_ALLOWED; nChannel++ )
    {
        m_ouScheduleController[nChannel].m_nCurrentCommandIndex = -1;
        m_ouScheduleController[nChannel].m_nCurrentDelay = -1;
        m_ouScheduleController[nChannel].m_nCurrentHandle = -1;
        m_ouScheduleController[nChannel].m_nTotalTables = 0;
        m_ouScheduleController[nChannel].m_ouHeaderCommands.clear();
        m_ouScheduleController[nChannel].m_ouScheduleTableList.clear();
        m_ouScheduleController[nChannel].m_ouCurrentCommand.m_bEnabled = false;

    }

    StopHardware();
    return S_OK;
}

INT CCommanDIL_LIN::nTransmitMessages(int nChannel)
{
    STLIN_MSG ouMsg;
    ouMsg.m_ucMsgTyp = LIN_MASTER_RESPONSE;
    ouMsg.m_ucChannel = nChannel+1;

    if ( nChannel < 0 || nChannel >= CHANNEL_ALLOWED )
    {
        return S_FALSE;
    }
    EnterCriticalSection(&m_ouCriticalSection);
    //1. SCeduale Table
    m_ouScheduleController[nChannel].m_nCurrentDelay--;

    if ( 0 >= m_ouScheduleController[nChannel].m_nCurrentDelay )
    {
        //1. Transmit Messages
        ouMsg.m_ucMsgID= m_ouScheduleController[nChannel].m_ouCurrentCommand.m_nId;
        if ( m_ouScheduleController[nChannel].m_ouCurrentCommand.m_bEnabled == true)
        {
            ouMsg.m_ucDataLen = m_ucConfiguredMasterDlc[nChannel][ouMsg.m_ucMsgID];
            memset(ouMsg.m_ucData, 0xFF, sizeof(ouMsg.m_ucData));

            //Diag Message
            if ( ouMsg.m_ucMsgID == 0x3C )
            {
                ouMsg.m_ucMsgTyp = LIN_SLAVE_RESPONSE;
                ouMsg.m_ucDataLen = 8;
                memcpy(ouMsg.m_ucData, m_ouScheduleController[nChannel].m_ouCurrentCommand.m_listIDs, 8);
                LIN_Send(ouMsg);
            }
            ouMsg.m_ucMsgTyp = LIN_MASTER_RESPONSE;
            LIN_Send(ouMsg);
        }
        //2. Get Next Message
        std::map<int, CSheduleTable>::iterator itrTable = m_ouScheduleController[nChannel].m_ouScheduleTableList.find(m_ouScheduleController[nChannel].m_nCurrentHandle);
        if ( itrTable != m_ouScheduleController[nChannel].m_ouScheduleTableList.end() )
        {
            m_ouScheduleController[nChannel].m_nCurrentCommandIndex++;
            if (m_ouScheduleController[nChannel].m_nCurrentCommandIndex >= itrTable->second.m_listCommands.size() )
            {
                m_ouScheduleController[nChannel].m_nCurrentCommandIndex =  0;
            }
            std::list<CScheduleCommands>::iterator itrCommand = itrTable->second.m_listCommands.begin();
            advance(itrCommand, m_ouScheduleController[nChannel].m_nCurrentCommandIndex);
            m_ouScheduleController[nChannel].m_ouCurrentCommand = *itrCommand;
            m_ouScheduleController[nChannel].m_nCurrentDelay =  static_cast<int>(itrCommand->m_fDelay);
        }

    }

    //For Normal Headers
    for (std::map<int, sIdDelay>::iterator itr = m_ouScheduleController[nChannel].m_ouHeaderCommands.begin(); itr != m_ouScheduleController[nChannel].m_ouHeaderCommands.end(); itr++ )
    {
        itr->second.m_nCurrentDelay--;
        if ( itr->second.m_nCurrentDelay == 0 )
        {
            ouMsg.m_ucMsgID= itr->first;
            ouMsg.m_ucDataLen = m_ucConfiguredMasterDlc[nChannel][ouMsg.m_ucMsgID];
            memset(ouMsg.m_ucData, 0xFF, sizeof(ouMsg.m_ucData));
            itr->second.m_nCurrentDelay = itr->second.m_nActualDelay;
            LIN_Send(ouMsg);
        }
    }
    LeaveCriticalSection(&m_ouCriticalSection);
    return S_OK;
}


DWORD WINAPI CCommanDIL_LIN::LINTxWndTransmitThread(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == NULL)
    {
        return (DWORD)-1;
    }
    CCommanDIL_LIN* pouData = (CCommanDIL_LIN*)pThreadParam->m_pBuffer;

    if (pouData == NULL)
    {
        return (DWORD)-1;
    }

    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, 1);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                //TODO:: Required to loop by number of channels.
                pouData->nTransmitMessages(0);
            }
            break;

            case SUSPEND:
            {
                WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
            }
            break;

            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;

            default:
                break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}


