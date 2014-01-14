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

typedef CList<s_FLXTXMSG, s_FLXTXMSG> CMSGLIST_FLEX;

//typedef enum eTXWNDDETAILS
//{
//    TX_MSG_BLOCK_COUNT,    // Message Block count
//    TX_WND_SPLITTER_DATA,
//    TX_WINDOW_PLACEMENT,
//};

struct FLEXRAY_FRAME_DATA
{
    FRAME_STRUCT m_ouFlexFrame;
    unsigned char m_ouData[254];
    bool bSelected;
    FLEXRAY_FRAME_DATA()
    {
        bSelected = false;
        memset( m_ouData, 0, 254 );
    }
};

struct FLEXRAY_CONFIG_DATA
{
    string m_strMessageName;
    int m_nSlotId;
    int m_nBaseCycle;
    int m_nDLC;
    int m_nRepetition;
    unsigned char m_ucData[254];
    bool m_bEnable;
};

class CTxFlexRayDataStore
{
public:

    //ClusterConfig m_ouFlexConfig;
    HRESULT SetFlexRayConfig(ClusterConfig& m_ouFlexConfig);

    bool m_bHexMode;
    bool m_bIsConfigUpdated;
    bool m_bDatabaseUpdated;
    ESTATUS_BUS m_eBusStatus;
    //Venkat
    int m_nChannelsConfigured;

    //Selected ECU Frame List - used to add messages in Tx List (Add NewFrame)
    list<FRAME_STRUCT> m_ouFrameList[CHANNEL_ALLOWED];

    //Contains Messages and their datavalues used in Tx Windows
    list<FLEXRAY_FRAME_DATA> m_ouFlexray_Frame_Data[CHANNEL_ALLOWED];

    //
    list<FLEXRAY_CONFIG_DATA> m_ouFlexray_ConfigData[CHANNEL_ALLOWED];

    bool bIsValidMessage(int nChannelIndex, FLEXRAY_CONFIG_DATA&);
    bool bAddToChannelList(int nChannel, FLEXRAY_CONFIG_DATA ouData);
    CTxFlexRayDataStore(void);
    void SetDefaultWindowPos();
    ~CTxFlexRayDataStore(void);

    bool        m_bTransmissionON;
    bool        m_bAutoUpdate;
    UINT m_MMTimerPeriod;
    UINT m_MMTimerId;
    SCALCEXECTIMETHREAD m_sCalTimerThreadStruct;
    WINDOWPLACEMENT m_sTxWndPlacement;
    CRITICAL_SECTION        m_MsgBlockCriticalSection;
    // void vManageTimerExecution();
    void vStartTxTimer();
    void vStopTxTimer();

    void vSetBusStatus(ESTATUS_BUS eBusStatus);
    int UpdateMessagetoDIL(FLEXRAY_FRAME_DATA& ouFlexData,  bool bDelete);

    static CTxFlexRayDataStore& ouGetTxFlexRayDataStoreObj();
    static CTxFlexRayDataStore m_sTxFlexRayDataStoreObj;
    bool bSetConfigData(xmlDocPtr pDoc);
    bool bSetChannelConfig(xmlNodePtr pNode);
    BOOL bGetConfigData(xmlNodePtr pxmlNodePtr);
    BOOL bGetMessageListConfig(int nChannel, xmlNodePtr);
    BOOL bGetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData);
    BOOL bSetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid);
    BOOL bSetDILInterfacePtr();
    bool parseForMessage(xmlNodePtr ptrNode, FLEXRAY_CONFIG_DATA& ouData);
    void vResetMonoshotBlocks();
    int UpdateMessageList();
    bool bExistInTxList(int nChannel, FLEXRAY_FRAME_DATA& ouData);
    void vUpdateTxList(int nChannel);
private:
    bool m_bValidTxWndSize;
};