#include "DIL_Interface_stdafx.h"
#include "CommanDIL_Flexray.h"


CCommanDIL_Flexray::CCommanDIL_Flexray(void)
{
    InitializeCriticalSection(&sg_CritSectForAckBuf);
}


CCommanDIL_Flexray::~CCommanDIL_Flexray(void)
{
    DeleteCriticalSection(&sg_CritSectForAckBuf);
}
HRESULT CCommanDIL_Flexray::FLEXRAY_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    HRESULT hResult;
    if (bRegister)
    {
        hResult = m_ClientList.RegisterClient(ClientID, pacClientName);
    }
    else
    {
        hResult = m_ClientList.RemoveClient(ClientID);
    }
    return hResult;
}

HRESULT CCommanDIL_Flexray::FLEXRAY_SendFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg)
{
    HRESULT hr = FLEXRAY_SendMsg(dwClientID, pouFlxTxMsg);
    /*if ( S_OK == hr )
    {
        map<SLOT, list<int>>::iterator itrMap = m_mapSlotClient.find(pouFlxTxMsg->m_sFlxMsg.wIdentifier);
        if ( itrMap != m_mapSlotClient.end() )
        {
            itrMap->second.push_back(dwClientID);
        }
        else
        {
            list<int> nListClient;
            nListClient.push_back(dwClientID);
            m_mapSlotClient[pouFlxTxMsg->m_sFlxMsg.wIdentifier] = nListClient;
        }
    }*/
    return hr;
}

HRESULT CCommanDIL_Flexray::FLEXRAY_DeleteFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg)
{
    HRESULT hr = FLEXRAY_DeleteMsg(dwClientID, pouFlxTxMsg);
    /*if ( S_OK == hr )
    {
        map<SLOT, list<int>>::iterator itrMap = m_mapSlotClient.find(pouFlxTxMsg->m_sFlxMsg.wIdentifier);
        if ( itrMap != m_mapSlotClient.end() )
        {
            itrMap->second.remove(dwClientID);
        }
    }*/
    return hr;
}

HRESULT CCommanDIL_Flexray::FLEXRAY_ManageMsgBuf(BYTE byAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (pBufObj)
    {
        CClientBuffer* pClientBuf = m_ClientList.GetClientByID(ClientID);
        if (pClientBuf)
        {
            if (byAction == MSGBUF_ADD)
            {
                /* Add msg buffer */
                hResult = pClientBuf->AddMsgBuf(pBufObj);
            }
            else if (byAction == MSGBUF_CLEAR)
            {
                /* clear msg buffer */
                if (pBufObj != nullptr)
                {
                    hResult = pClientBuf->RemoveMsgBuf(pBufObj);
                }
                else
                {
                    // remove all message buffers
                    hResult = pClientBuf->RemoveAllMsgBuf();
                }
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
            hResult = m_ClientList.RemoveAllMsgBufOnAllClients();
        }
    }
    return hResult;
}


void CCommanDIL_Flexray::vWriteIntoClientsBuffer(s_FLXMSG& sFlexRayData)
{
    //Write into the client's buffer and Increment message Count
    for (int iClientIndex=0; iClientIndex < m_ClientList.Size(); iClientIndex++)
    {
        CClientBuffer* pClient = m_ClientList.GetClientByIndex(iClientIndex);
        if (pClient)
        {
            for (int iBufIndex = 0; iBufIndex < pClient->NumOfSEBuffers(); iBufIndex++)
            {
                CBaseFLEXBufFSE* pFLEXBufSE = pClient->GetSEBufferByIndex(iBufIndex);
                if (pFLEXBufSE)
                {
                    pFLEXBufSE->WriteIntoBuffer(&sFlexRayData);
                }
            }
        }
    }
}


void CCommanDIL_Flexray::vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    //EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    sg_asAckMapBuf.push_back(RefObj);
    //LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
}

bool CCommanDIL_Flexray::bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    bool bResult = false;
    CACK_MAP_LIST::iterator  iResult =
        find( sg_asAckMapBuf.begin(), sg_asAckMapBuf.end(), RefObj );

    //if ((*iResult).m_ClientID > 0)
    if (iResult != sg_asAckMapBuf.end())
    {
        bResult = true;
        ClientID = (*iResult).m_unClientID;
        sg_asAckMapBuf.erase(iResult);
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer

    return bResult;
}