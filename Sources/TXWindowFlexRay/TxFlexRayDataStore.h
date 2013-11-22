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

typedef enum eTXWNDDETAILS
{
    TX_MSG_BLOCK_COUNT,    // Message Block count
    TX_WND_SPLITTER_DATA,
    TX_WINDOW_PLACEMENT,
};
typedef struct tagSMSGBLOCK
{
    BOOL            m_bCyclic;
    BOOL            m_bActive;
    BOOL            m_bMonoshotSent;
    UINT            m_unTimeInterval;
    UINT            m_unCurrTime;
    CString         m_omBlockName;
    CMSGLIST_FLEX   m_omFlexMsgList;

    tagSMSGBLOCK()
    {
        m_bCyclic            = FALSE;
        m_bActive            = FALSE;
        m_bMonoshotSent      = FALSE;
        m_unTimeInterval     = 0;
        m_unCurrTime         = 0;
        m_omFlexMsgList.RemoveAll();
    };

    BOOL CopyValidTXData(const tagSMSGBLOCK& Source)
    {
        BOOL bNonEmptyList = TRUE;

        m_bCyclic = Source.m_bCyclic;
        m_bActive = Source.m_bActive;
        m_unTimeInterval = Source.m_unTimeInterval;
        m_omBlockName = Source.m_omBlockName;
        m_bMonoshotSent = Source.m_bMonoshotSent;

        const CMSGLIST_FLEX& CurrList = Source.m_omFlexMsgList;
        POSITION CurrPos = CurrList.GetHeadPosition();
        for (int j = 0; j < CurrList.GetCount(); j++)
        {
            const s_FLXTXMSG& CurrTxMsg = CurrList.GetNext(CurrPos);
            // if (CurrTxMsg.m_bToTransmit)
            {
                // bNonEmptyList = TRUE;
                m_omFlexMsgList.AddTail(CurrTxMsg);
            }
        }
        return bNonEmptyList;
    };

    UINT unCalculateSize()
    {
        return (2 * sizeof(BOOL) + 2 * sizeof(UINT) + strlen(m_omBlockName) + 1);
    };

    // Retrieve data from the location(DestAddress)
    //and return the location for reading next data
    //BYTE* RetrieveStructdata(BYTE *DestAddress)
    //{
    //  memcpy(&m_bCyclic, DestAddress, sizeof(BOOL));
    //  DestAddress += sizeof(BOOL);
    //  memcpy(&m_bActive, DestAddress, sizeof(BOOL));
    //  DestAddress += sizeof(BOOL);
    //  memcpy(&m_unTimeInterval, DestAddress, sizeof(UINT));
    //  DestAddress += sizeof(UINT);
    //  memcpy(&m_unCurrTime, DestAddress, sizeof(UINT));
    //  DestAddress += sizeof(UINT);
    //  m_omBlockName.Empty();
    //  m_omBlockName = (char*)DestAddress;
    //  while (*DestAddress != '\0')
    //  {
    //      DestAddress++;
    //  }
    //  return ++DestAddress;
    //};

    // Save data at the location(SrcAddress)
    //and return the location for saving next data
    /*BYTE* SaveStructData(BYTE *SrcAddress)
    {
        memcpy(SrcAddress,&m_bCyclic, sizeof(BOOL));
        SrcAddress += sizeof(BOOL);
        memcpy(SrcAddress,&m_bActive, sizeof(BOOL));
        SrcAddress += sizeof(BOOL);
        memcpy(SrcAddress,&m_unTimeInterval, sizeof(UINT));
        SrcAddress += sizeof(UINT);
        memcpy(SrcAddress,&m_unCurrTime, sizeof(UINT));
        SrcAddress += sizeof(UINT);
        memcpy(SrcAddress,m_omBlockName.GetBuffer(MAX_PATH), strlen(m_omBlockName)+1);
        SrcAddress += strlen(m_omBlockName)+1;
        return SrcAddress;
    };*/

} SMSGBLOCK;


typedef CArray<SMSGBLOCK*, SMSGBLOCK*> CMSGBLOCKMANAGER;


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

    //FlexConfig m_ouFlexConfig;
    HRESULT SetFlexRayConfig(FlexConfig& m_ouFlexConfig);

    bool m_bHexMode;
    bool m_bIsConfigUpdated;
    bool m_bDatabaseUpdated;
    bool m_bBusConnected;
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
    void vManageTimerExecution();
    void vStartTxTimer();
    void vStopTxTimer();
    int nGetBlockCount();


    void vSetBusStatus(bool bConnected);
    int UpdateMessagetoDIL(FLEXRAY_FRAME_DATA& ouFlexData,  bool bDelete);

    //Array of msg block after pressing update
    CMSGBLOCKMANAGER m_omTxMsgBlockMan;

    int         m_nBlockCount;

    static CTxFlexRayDataStore& ouGetTxFlexRayDataStoreObj();
    static CTxFlexRayDataStore m_sTxFlexRayDataStoreObj;
    void vRemoveAllBlock(CMSGBLOCKMANAGER& mouCurrMsgBlockMan);
    void vCopyMsgBlockManager(CMSGBLOCKMANAGER& mouTarget, const CMSGBLOCKMANAGER& mouSource);
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