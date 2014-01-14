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

#include "..\Datatypes\FLEXRAY_Datatypes.h"
#include "..\DataTypes\Cluster.h"
#include "..\Utility\Utility_Thread.h"


typedef CList<s_FLXTXMSG, s_FLXTXMSG> CMSGLIST_FLEX;



struct LIN_CONFIG_DATA
{
    string m_strMessageName;
    unsigned char m_ucMsgTyp;       // Message Type (0 - Header / 1 - Response)
    unsigned char m_ucChksumTyp;    // Checksum Type (0 - Classical / 1 - Enhanced)
    unsigned char m_ucChannel;      // Channel Number
    unsigned char m_ucDataLen;      // Data len (0..8)
    unsigned char m_ucMsgID;        // Protected Identifier
    unsigned char m_ucData[8];      // Databytes 0..8
    unsigned char m_ucChksum;       // Checksum

    LIN_CONFIG_DATA()
    {
    }
};

struct LIN_FRAME_DATA
{
    STLIN_MSG m_ouLinMessage;
    string m_strFrameName;
    bool bSelected;
    bool bKeyEnabled;
    char m_chKeyVal;
    bool m_bTimerEnabled;
    int nCurrentTimerVal;
    int nActualTimer;
    bool    m_bDataBaseMsg;
    LIN_FRAME_DATA()
    {
        m_bTimerEnabled = false;
        bKeyEnabled = false;
        bSelected = true;
        m_chKeyVal = 'a';
        nCurrentTimerVal = 10;
        nActualTimer = 10;
        m_ouLinMessage.m_ucDataLen = 0;
        m_ouLinMessage.m_ucMsgID = -1;
        memset(&m_ouLinMessage, 0, sizeof(STLIN_MSG));
        memset(m_ouLinMessage.m_ucData, 1, 8 );
        m_bDataBaseMsg = false;
    }
};
class CBaseDIL_LIN;
class CTxLINDataStore
{
public:

    //ClusterConfig m_ouFlexConfig;
    HRESULT SetFlexRayConfig(ClusterConfig* m_ouFlexConfig);
    DWORD m_dwClientID;
    CBaseDIL_LIN* m_pouDIL_LIN_Interface;
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
    list<FRAME_STRUCT> m_ouFrameList[CHANNEL_ALLOWED];
    ClusterConfig* m_ouClusterConfig;

    //Contains Messages and their datavalues used in Tx Windows
    list<LIN_FRAME_DATA> m_ouLIN_Frame_Data;
    map<int, char> m_ouMapIndexToKey;

    //
    list<LIN_CONFIG_DATA> m_ouLIN_ConfigData[CHANNEL_ALLOWED];

    bool bIsValidMessage(int nChannelIndex, LIN_CONFIG_DATA&);
    bool bAddToChannelList(LIN_FRAME_DATA& ouData);
    CTxLINDataStore(void);
    void SetDefaultWindowPos();
    ~CTxLINDataStore(void);

    bool        m_bTransmissionON;
    bool        m_bAutoUpdate;
    WINDOWPLACEMENT m_sTxWndPlacement;


    void vSetBusStatus(ESTATUS_BUS eBusStatus);
    int UpdateMessagetoDIL(LIN_FRAME_DATA& ouFlexData,  bool bDelete);

    static CTxLINDataStore& ouGetTxLINDataStoreObj();
    static CTxLINDataStore m_sTxLINDataStoreObj;
    bool bSetConfigData(xmlDocPtr pDoc);
    bool bSetChannelConfig(xmlNodePtr pNode);
    BOOL bGetConfigData(xmlNodePtr pxmlNodePtr);
    BOOL bGetMessageListConfig( xmlNodePtr);
    BOOL bGetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData);
    BOOL bSetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid);
    BOOL bSetDILInterfacePtr(CBaseDIL_LIN* pLinDIL);
    bool parseForMessage(xmlNodePtr ptrNode, LIN_FRAME_DATA& ouData);
    int UpdateMessageList();
    bool bExistInTxList(int nChannel, LIN_FRAME_DATA& ouData);
    void vUpdateTxList(int nChannel);


    int nInsertMessage( LIN_FRAME_DATA& ouLinMsg );
    int nDeleteKey(int nIndex);
    int nDeleteAllKeyEntrs();
    int nInsertKey(int nIndex, char key);
    int nHandleSelectionState(int nIndex, bool bEnable);
    int nDeleteMessageAt( int nIndex );
    int nDeleteAllMessages();
    int nHandleKeyEvent(char chKey);
    int nTransmitMsgAt(int nIndex);
private:
    bool m_bValidTxWndSize;
    CPARAM_THREADPROC m_ouTransmitThread;

    INT nTransmitMessages();
    int ouGetLinFrame(int nIndex,  LIN_FRAME_DATA& ouLinFramData);
    int nPreMessageTransmission();
    static DWORD WINAPI LINTxWndTransmitThread(LPVOID pVoid);

    int nStopTransmissionAt(int nIndex);
    HRESULT GetMessageIDNames(int nChannelIndex, map<int, string>& ouMsgIDNamesMap);
};