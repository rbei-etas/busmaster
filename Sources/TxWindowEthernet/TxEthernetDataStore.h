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
 * \file      TxWndDataStore.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#pragma once
#include "TxWindowEthernet_Stdafx.h"
#include "..\Datatypes\DIL_Datatypes.h"
//#include "..\DataTypes\Cluster.h"
#include "..\Include\Struct_ETHERNET.h"
#include "..\Utility\Utility_Thread.h"


typedef CList<STETHERNETDATA, STETHERNETDATA> CMSGLIST_ETHERNET;

typedef enum eTXWNDDETAILS
{
    TX_MSG_BLOCK_COUNT,    // Message Block count
    TX_WND_SPLITTER_DATA,
    TX_WINDOW_PLACEMENT,
};



struct ETHERNET_FRAME_DATA
{
	STETHERNET_MSG m_ouEthernetMessage;
    string m_strFrameName;
    bool bSelected;
    bool bKeyEnabled;
    char m_chKeyVal;
    bool m_bTimerEnabled;
    int nCurrentTimerVal;
    int nActualTimer;
    bool    m_bDataBaseMsg;
    ETHERNET_FRAME_DATA()
    {
        m_bTimerEnabled = false;
        bKeyEnabled = false;
        bSelected = true;
        m_chKeyVal = 'a';
        nCurrentTimerVal = 10;
        nActualTimer = 10;
        m_ouEthernetMessage.m_unDataLen = 0;
        m_ouEthernetMessage.m_unMsgID = -1;
        memset(&m_ouEthernetMessage, 0, sizeof(STETHERNET_MSG));
        memset(m_ouEthernetMessage.m_ucData, 1, 1500 );
        m_bDataBaseMsg = false;
    }
};
class CBaseDIL_ETHERNET;
class CTxEthernetDataStore
{
public:

    //ClusterConfig m_ouFlexConfig;
    //HRESULT SetFlexRayConfig(ClusterConfig* m_ouFlexConfig);
    DWORD m_dwClientID;
    CBaseDIL_ETHERNET* m_pouDIL_Ethernet_Interface;
    map<int, string> m_strMessageNames[CHANNEL_ALLOWED];
    CRITICAL_SECTION m_ouCSMsgList;
    CRITICAL_SECTION m_ouCSKeyMap;

    bool m_bHexMode;
    bool m_bIsConfigUpdated;
    bool m_bDatabaseUpdated;
    ESTATUS_BUS m_eBusStatus;
    //Venkat
    int m_nChannelsConfigured;

    //Selected ECU Frame List - used to add messages in Tx List (Add NewFrame)
    //list<FRAME_STRUCT> m_ouFrameList[CHANNEL_ALLOWED];
    //ClusterConfig* m_ouClusterConfig;

    //Contains Messages and their datavalues used in Tx Windows
	//changed to pointer. Had issues in retreiving values. Values stored were lost
    list<ETHERNET_FRAME_DATA> m_ouEthernet_Frame_Data;
    map<int, char> m_ouMapIndexToKey;

 
    bool bAddToChannelList(ETHERNET_FRAME_DATA& ouData);
    CTxEthernetDataStore(void);
    void SetDefaultWindowPos();
    ~CTxEthernetDataStore(void);

    bool        m_bTransmissionON;
    bool        m_bAutoUpdate;
    WINDOWPLACEMENT m_sTxWndPlacement;


    void vSetBusStatus(ESTATUS_BUS eBusStatus);
    int UpdateMessagetoDIL(ETHERNET_FRAME_DATA& ouFlexData,  bool bDelete);

    static CTxEthernetDataStore& ouGetTxEthernetDataStoreObj();
    static CTxEthernetDataStore m_sTxEthernetDataStoreObj;
    bool bSetConfigData(xmlDocPtr pDoc);
    bool bSetChannelConfig(xmlNodePtr pNode);
    BOOL bGetConfigData(xmlNodePtr pxmlNodePtr);
    BOOL bGetMessageListConfig( xmlNodePtr);
    BOOL bGetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData);
    BOOL bSetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid);
    BOOL bSetDILInterfacePtr(CBaseDIL_ETHERNET* pLinDIL);
    bool parseForMessage(xmlNodePtr ptrNode, ETHERNET_FRAME_DATA& ouData);
    int UpdateMessageList();
    bool bExistInTxList(int nChannel, ETHERNET_FRAME_DATA& ouData);
    void vUpdateTxList(int nChannel);


    int nInsertMessage( ETHERNET_FRAME_DATA& ouLinMsg );
    int nDeleteKey(int nIndex);
    int nDeleteAllKeyEntrs();
    int nInsertKey(int nIndex, char key);
    int nHandleSelectionState(int nIndex, bool bEnable);
    int nDeleteMessageAt( int nIndex );
    int nDeleteAllMessages();
    int nHandleKeyEvent(char chKey);
    int nTransmitMsgAt(int nIndex);
	HMODULE hGetCustomProtocolHandle();

private:
    bool m_bValidTxWndSize;
    CPARAM_THREADPROC m_ouTransmitThread;

    INT nTransmitMessages();
    int ouGetLinFrame(int nIndex,  ETHERNET_FRAME_DATA& ouLinFramData);
    int nPreMessageTransmission();
    static DWORD WINAPI EthernetTxWndTransmitThread(LPVOID pVoid);

    int nStopTransmissionAt(int nIndex);
    HRESULT GetMessageIDNames(int nChannelIndex, map<int, string>& ouMsgIDNamesMap);
};