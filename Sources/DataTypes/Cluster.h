#pragma once
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include "FEALData.h"
#include "Include\BaseDefs.h"
#include "DataTypes\MsgSignal_Datatypes.h"

typedef UINT SLOT;
typedef LONG SLOT_BASECYCLE;
typedef std::string ECU_ID;


#define     LIN_SID_ASSIGN_FRAME_ID     0xB1
#define     LIN_SID_UNASSIGN_FRAME_ID   0xB1
#define     LIN_SID_ASSIGN_NAD_ID       0xB0
#define     LIN_SID_COND_CHANGE_NAD     0xB3
#define     LIN_SID_DATA_DUMP           0xB4
#define     LIN_SID_SAVE_CONFIG         0xB6
#define     LIN_SID_ASSIGN_FRAME_RANGE  0xB7



#define     LIN_NAD_INDEX       0
#define     LIN_PCI_INDEX       1
#define     LIN_SID_INDEX       2
#define     LIN_RSID_INDEX      3

enum ENDIANNESS
{
    INTEL,
    MOTOROLA
};

enum ESLOT_TYPE
{
    STATIC,
    DYNAMIC
};

enum ESYNC
{
    SYNC,
    STARTUP,
    NONE_TYPE
};

class SIGNAL_STRUCT
{
public:
    //string m_omSigId;
    unsigned short     m_unStartbit;  // The prime identifier of a signal
    std::string m_strSignalName;
    unsigned short m_nLength;
    char m_bDataType;
    //string m_omDataType;
    std::string  m_omUnit;
    unsigned int m_unDefaultVal;
    //string  m_omPhylVal;

    //CSigConstraintsArray      m_ouSigConstrnt;
    CCompuMethodEx            m_ouCompuMethod; // Engg. value computing methods

    //TODO
    /*COMPU_EXPRESSION m_ouCompuMetod;*/
    ENDIANNESS m_ouEndianness;

    // Signal Id to desc
    //map<string, string> m_ouDescriptorMap;

    SIGNAL_STRUCT();
    ~SIGNAL_STRUCT();
    SIGNAL_STRUCT(const SIGNAL_STRUCT& Refobj);
    SIGNAL_STRUCT& operator=(const SIGNAL_STRUCT& Refobj);

};

class CMuxPdu
{
public:
    std::string omPduId;
    std::list<SIGNAL_STRUCT> lstSignalStruct;
};
class PDU_STRUCT
{
public:
    std::string m_strPduId;
    std::string m_strPDUName;
    int m_nStartBit;
    int m_nLength;
    ENDIANNESS m_ouEndianness;

    // Switch code to signal list
    std::map<std::string, CMuxPdu> m_ouSwitchToSignals;
    std::list<SIGNAL_STRUCT> m_ouSignalList;
public:
    HRESULT GetSignalList ( std::list<SIGNAL_STRUCT>& ouSignalList );
    ~PDU_STRUCT()
    {
    }

};

class FRAME_STRUCT
{
public:
    std::string m_strFrameId;
    std::string m_strFrameName;
    unsigned short m_nSlotId;
    unsigned short m_nBaseCycle;
    unsigned short m_nReptition;
    unsigned short m_nLength;
    //bool m_bConsiderPdu;
    std::list<PDU_STRUCT>    m_ouPduList;
    //list<SIGNAL_STRUCT>   m_ouSignalList;
    ECHANNEL    m_ouChannel;
    ESLOT_TYPE m_eSlotType;
    ESYNC m_eSync;

    bool operator == (FRAME_STRUCT& ob1)
    {
        return
            (m_strFrameId == ob1.m_strFrameId) &&
            (m_nSlotId == ob1.m_nSlotId) &&
            (m_nBaseCycle == ob1.m_nBaseCycle);
    }
    bool operator < (FRAME_STRUCT& ob1)
    {
        return m_strFrameId < ob1.m_strFrameId;
    }

    bool operator > (FRAME_STRUCT& ob1)
    {
        return (m_strFrameId > ob1.m_strFrameId);
    }

    ~FRAME_STRUCT()
    {
    }

    FRAME_STRUCT();
    HRESULT GetPDUList(std::list<SIGNAL_STRUCT>& ouSignalList);
    HRESULT GetSignalList(std::string omStrPduName, std::list<SIGNAL_STRUCT>& ouSignalList);
    HRESULT GetSignalList(std::list<SIGNAL_STRUCT>& ouSignalList);
    HRESULT GetSignalNames(CStringList& ouSignalList);
    HRESULT GetSignalCount(int& nCount);
};

bool Compare_Frame_Structs(FRAME_STRUCT& ob1, FRAME_STRUCT& ob2);

class Ecu_FlexRay_Params
{
public:
    //One Controller per ECU
    ABS_FLEXRAY_SPEC_CNTLR m_ouControllerParams;
    int m_nKeySlot;

};
class Ecu_Lin_Params
{
public:
    std::string m_strProtocolVer;
    int m_nConfiguredNAD;
    int m_nInitialNAD;
    unsigned char m_unProductId[3];
    float m_p2Min;
    float m_stMin;
    float m_nAsTimeout;
    float m_nCrTimeout;
    std::string m_strResponseError;
};
class Ecu_CAN_Params
{
};
class ECU_Struct
{
public:
    std::string m_strEcuId;
    std::string m_strECUName;

    Ecu_FlexRay_Params m_ouFlexRayParams;
    Ecu_Lin_Params  m_ouLinParams;
    Ecu_CAN_Params m_ouCANParams;


    // Slot Id to Tx FrameList
    std::map<UINT, std::list<FRAME_STRUCT>> m_ouTxFrames;

    // Slot Id to Rx FrameList
    std::map<UINT, std::list<FRAME_STRUCT>> m_ouRxFrames;

    ECHANNEL    m_ouChannel;

public:
    ECU_Struct();
    HRESULT GetControllerParams(ABS_FLEXRAY_SPEC_CNTLR& ouControllerParams);
    HRESULT GetFrameList( std::list<FRAME_STRUCT>& ouFrameList, EDIRECTION ouEDIRECTION );
    HRESULT GetFrameList( std::list<FRAME_STRUCT>& ouFrameList);
    HRESULT GetFrameNames( std::list<std::string>& ouFrameList);
    HRESULT GetPDUList ( SLOT_BASECYCLE ouSlotBaseKey, std::list<PDU_STRUCT>& ouPduList );
    HRESULT GetSignalList ( SLOT_BASECYCLE ouSlotBaseKey, std::list<SIGNAL_STRUCT>& ouSignalList );
    HRESULT GetFrame(UINT unSlotId, UINT nBaseCycle, ECHANNEL& oeChannel, FRAME_STRUCT& ouFrame);
    HRESULT GetFrame(UINT unSlotId, FRAME_STRUCT& ouFrame);

};

/////////////Schedule Table Definitions - LIN//////////////
enum eCommandType
{
    COMMAND_INVALID,
    COMMAND_UNCONDITIONAL,
    COMMAND_SPORADIC,
    COMMAND_EVENT,
    COMMAND_MASTERREQ,
    COMMAND_SLAVE,
    COMMAND_ASSIGN_FRAME_ID,
    COMMAND_ASSIGN_FRAME_ID_RANGE,
    COMMAND_ASSIGN_NAD,
    COMMAND_CONDITIONAL_CHANGE_NAD,
    COMMAND_DATA_DUMP,
    COMMAND_FREE_FORMAT,
    COMMAND_READ_BY_IDENTIFIER,
    COMMAND_SAVE_CONFIGURATION,
    COMMAND_UNASSIGN_FRAME_ID,
    COMMAND_TOTAL
};

const std::string m_omCommandType[COMMAND_TOTAL] =
{
    "Invalid",
    "UnConditional",
    "Sporadic Frame",
    "Event Triggered",
    "Master Request",
    "Slave Response",
    "AssignFrameId",
    "AssignFrameIdRange",
    "AssignNAD",
    "ConditionalChangeNAD",
    "DataDump",
    "FreeFormat",
    "ReadyByIdentifier",
    "SaveConfiguration",
    "UnassignFrameId"
};

class CScheduleCommands
{
public:
    eCommandType m_eCommandType;
    std::string m_strCommandName;                   //
    std::string m_strNodeName;
    double m_fDelay;
    unsigned char m_listIDs[8];
    int m_nId;
    bool m_bEnabled;
    CScheduleCommands()
    {
        m_bEnabled = false;
    }

};



class CSheduleTable
{
public:
    std::string m_strTableName;
    std::list<CScheduleCommands> m_listCommands;
    CSheduleTable()
    {
        m_strTableName.clear();
        m_listCommands.clear();
    }
    CSheduleTable(const CSheduleTable& ouRefObj)
    {
        m_strTableName = ouRefObj.m_strTableName;
        m_listCommands.clear();
        m_listCommands.insert(m_listCommands.begin(), ouRefObj.m_listCommands.begin(), ouRefObj.m_listCommands.end());
    }
    CSheduleTable operator=(const CSheduleTable& ouRefObj)
    {
        m_strTableName = ouRefObj.m_strTableName;
        m_listCommands.clear();
        m_listCommands.insert(m_listCommands.begin(), ouRefObj.m_listCommands.begin(), ouRefObj.m_listCommands.end());
        return *this;
    }

};
//////////////////////////////////////////////
typedef std::map<ECU_ID, ECU_Struct> ECUMAP;

class Cluster
{
public:
    std::string  m_strName;
    std::string m_omClusterID;
    //string m_omChannelRef;

    std::map<std::string, std::string> m_mapChnls;

    ABS_FLEXRAY_CLUSTER m_ouClusterInfo;

    //ECU ID to ECU object
    ECUMAP  m_ouEcuList;

    //To Identify which slot to ECU instead of Looping every time
    std::map<SLOT, std::list<ECU_ID> > m_mapSlotEcu;

    BOOL m_bAutoServerMode;

public:

    void Clear();
    Cluster();
    ~Cluster();
    HRESULT GetClusterInfo( ABS_FLEXRAY_CLUSTER& ouClusterInfo );
    HRESULT GetECUList( std::list<ECU_Struct>& ouEcuList );
    HRESULT GetECU( std::string omECUStrId, ECU_Struct& ouEcu);
    HRESULT unListGetMessageIDs(UINT*&, UINT& nMsgCount);
    HRESULT GetFrameList( std::string omStrEcuName, std::list<FRAME_STRUCT>& ouFrameList );
    HRESULT GetTxFrameList( std::string omStrEcuName, std::list<FRAME_STRUCT>& ouFrameList );
    HRESULT GetFrameNames(std::string omStrEcuName, std::list<std::string>& lstFrames);
    HRESULT GetRxFrameList( std::string omStrEcuName, std::list<FRAME_STRUCT>& ouFrameList );
    HRESULT GetPDUList ( SLOT_BASECYCLE ouSlotBaseKey, std::list<PDU_STRUCT>& ouPduList );
    HRESULT GetSignalList ( SLOT_BASECYCLE ouSlotBaseKey, std::list<SIGNAL_STRUCT>& ouSignalList );
    HRESULT GetEcuChannel( ECHANNEL& ouChannelType );

    CString bWriteDBHeader(CString omStrActiveDataBase, ETYPE_BUS eBus);
    BOOL bSortSignalStartBitAscend(UINT* punSigStartBit, UINT unCount);
    BOOL bFormSigNameAndLength(UINT* punLength,
                               UINT* punStartBit,
                               CStringArray& omStrArraySigName,
                               std::list<FRAME_STRUCT>::iterator itrFrame);
    SIGNAL_STRUCT psGetSigPtr(UINT unStartBitSrc, std::list<SIGNAL_STRUCT> sigList);

    BOOL bInsertBusSpecStructures(CStdioFile& omHeaderFile,
                                  CString& omStrcommandLine,
                                  CStringArray& omStrArraySigName,
                                  std::list<FRAME_STRUCT>::iterator itrFrame, ETYPE_BUS eBUS);
    HRESULT GetFrames(std::list<FRAME_STRUCT>& ouFrameList);
};

class FlexRayChannelParam
{
public:
    int m_nKetSlot;
    int m_nSecondKeySlot;
    void vInitialiseConfig()
    {
        m_nKetSlot = -1;
        m_nSecondKeySlot = -1;
    }
};

class LinChannelParam
{
public:
    int m_nBaudRate;
    bool m_bIsMasterMode;
    std::string m_strProtocolVersion;
    bool m_bOverWriteSettings;
    std::list<CSheduleTable> ouLinParams;
    void vInitialiseConfig()
    {
        m_nBaudRate = 19200;
        m_strProtocolVersion = "LIN 2.0";
        m_bOverWriteSettings = true;
        m_bIsMasterMode = false;
        ouLinParams.clear();
    }
    LinChannelParam()
    {
        vInitialiseConfig();
    }
    LinChannelParam(const LinChannelParam& ouRefObj)
    {
        m_nBaudRate = ouRefObj.m_nBaudRate;
        m_strProtocolVersion = ouRefObj.m_strProtocolVersion;
        m_bOverWriteSettings = ouRefObj.m_bOverWriteSettings;
        m_bIsMasterMode = ouRefObj.m_bIsMasterMode;
        ouLinParams.clear();
        ouLinParams.insert(ouLinParams.begin(), ouRefObj.ouLinParams.begin(), ouRefObj.ouLinParams.end());
    }
    LinChannelParam operator=(const LinChannelParam& ouRefObj)
    {
        m_nBaudRate = ouRefObj.m_nBaudRate;
        m_strProtocolVersion = ouRefObj.m_strProtocolVersion;
        m_bOverWriteSettings = ouRefObj.m_bOverWriteSettings;
        m_bIsMasterMode = ouRefObj.m_bIsMasterMode;
        ouLinParams.clear();
        ouLinParams.insert(ouLinParams.begin(), ouRefObj.ouLinParams.begin(), ouRefObj.ouLinParams.end());
        return *this;
    }


};



typedef struct tag_Channel_Config
{
    Cluster m_ouClusterInfo;
    ETYPE_BUS m_eBusChannelType;
    std::list<std::string> m_strSlectedEculist;
    std::string m_strDataBasePath;

    FlexRayChannelParam m_ouFlexRayParams;
    LinChannelParam     m_ouLinParams;
    std::list<CSheduleTable> GetScheduleTablesList()
    {
        return m_ouLinParams.ouLinParams;
    }


    tag_Channel_Config(ETYPE_BUS eBusChannelType = BUS_INVALID)
    {
        m_eBusChannelType = eBusChannelType;
        m_strSlectedEculist.clear();
        m_strDataBasePath = "";
        m_ouFlexRayParams.m_nKetSlot = -1;
        m_ouFlexRayParams.m_nSecondKeySlot = -1;
    }
    tag_Channel_Config(const tag_Channel_Config& ouRefObj)
    {
        m_eBusChannelType = ouRefObj.m_eBusChannelType;
        m_ouClusterInfo = ouRefObj.m_ouClusterInfo;
        m_strSlectedEculist = ouRefObj.m_strSlectedEculist;
        m_strDataBasePath = ouRefObj.m_strDataBasePath;
        m_ouFlexRayParams.m_nKetSlot = ouRefObj.m_ouFlexRayParams.m_nKetSlot;
        m_ouFlexRayParams.m_nSecondKeySlot = ouRefObj.m_ouFlexRayParams.m_nSecondKeySlot;
    }

    tag_Channel_Config& operator=(tag_Channel_Config& ouRefObj)
    {
        m_eBusChannelType = ouRefObj.m_eBusChannelType;
        m_ouClusterInfo = ouRefObj.m_ouClusterInfo;
        m_strSlectedEculist = ouRefObj.m_strSlectedEculist;
        m_strDataBasePath = ouRefObj.m_strDataBasePath;
        m_ouFlexRayParams.m_nKetSlot = ouRefObj.m_ouFlexRayParams.m_nKetSlot;
        m_ouFlexRayParams.m_nSecondKeySlot = ouRefObj.m_ouFlexRayParams.m_nSecondKeySlot;
        m_ouLinParams.m_bOverWriteSettings = ouRefObj.m_ouLinParams.m_bOverWriteSettings;
        m_ouLinParams.m_nBaudRate = ouRefObj.m_ouLinParams.m_nBaudRate;
        m_ouLinParams.m_strProtocolVersion = ouRefObj.m_ouLinParams.m_strProtocolVersion;
        m_ouLinParams.m_bIsMasterMode = ouRefObj.m_ouLinParams.m_bIsMasterMode;
        m_ouLinParams.ouLinParams = ouRefObj.m_ouLinParams.ouLinParams;
        return *this;
    }


    HRESULT GetSelectedECUTxFrames(std::list<FRAME_STRUCT>& ouFrameList)
    {
        std::list<std::string>::iterator itrEcuList;
        std::list<FRAME_STRUCT> ouFrameTempList;
        for ( itrEcuList =m_strSlectedEculist.begin(); itrEcuList != m_strSlectedEculist.end(); itrEcuList++ )
        {
            m_ouClusterInfo.GetTxFrameList(*itrEcuList, ouFrameTempList);
        }
        ouFrameTempList.sort();
        ouFrameTempList.unique();
        ouFrameTempList.erase (unique (ouFrameTempList.begin(), ouFrameTempList.end(), Compare_Frame_Structs ), ouFrameTempList.end());  //  2.72,  3.14, 12.15


        //TODO::
        int nCount = 0;
        std::list<FRAME_STRUCT>::iterator itrFrame = ouFrameTempList.begin();
        while( itrFrame != ouFrameTempList.end())
        {
            ouFrameList.push_back(*itrFrame);
            nCount++;

            itrFrame++;
        }

        return S_OK;
    }



    HRESULT GetSelectedECUFrames(std::list<FRAME_STRUCT>& ouFrameList)
    {
        std::list<std::string>::iterator itrEcuList;
        std::list<FRAME_STRUCT> ouFrameTempList;
        for ( itrEcuList =m_strSlectedEculist.begin(); itrEcuList != m_strSlectedEculist.end(); itrEcuList++ )
        {
            m_ouClusterInfo.GetFrameList(*itrEcuList, ouFrameTempList);
        }
        ouFrameTempList.sort();
        ouFrameTempList.unique();
        ouFrameTempList.erase (unique (ouFrameTempList.begin(), ouFrameTempList.end(), Compare_Frame_Structs ), ouFrameTempList.end());  //  2.72,  3.14, 12.15

        //TODO::
        int nCount = 0;
        std::list<FRAME_STRUCT>::iterator itrFrame = ouFrameTempList.begin();
        while( itrFrame != ouFrameTempList.end())
        {
            ouFrameList.push_back(*itrFrame);
            nCount++;

            itrFrame++;
        }

        return S_OK;
    }

    HRESULT GetSelectedECUTxFrameNames(std::list<std::string>& ouFrameTempList)
    {
        if(m_strDataBasePath != "" && m_strSlectedEculist.size() > 0)
        {
            std::list<std::string>::iterator itrEcuList = m_strSlectedEculist.begin();

            if(itrEcuList != m_strSlectedEculist.end())
            {
                while ( itrEcuList != m_strSlectedEculist.end() )
                {
                    m_ouClusterInfo.GetFrameNames(*itrEcuList, ouFrameTempList);
                    itrEcuList++;
                }
            }

            ouFrameTempList.sort();
            ouFrameTempList.unique();
            // ouFrameTempList.erase (unique (ouFrameTempList.begin(), ouFrameTempList.end(), Compare_Frame_Structs ), ouFrameTempList.end());  //  2.72,  3.14, 12.15
        }

        return S_OK;
    }
    HRESULT GetFrame(UINT unSlotId, UINT unCycle, ECHANNEL oeChannel, FRAME_STRUCT& ouFrame)
    {
        std::map<SLOT, std::list<ECU_ID>>:: iterator itrSlotEcu = m_ouClusterInfo.m_mapSlotEcu.find(unSlotId);
        if ( itrSlotEcu !=  m_ouClusterInfo.m_mapSlotEcu.end() )
        {
            std::list<ECU_ID>::iterator itrEcu = itrSlotEcu->second.begin();
            while ( itrEcu !=  itrSlotEcu->second.end() )
            {
                ECUMAP::iterator itrEcuStruct = m_ouClusterInfo.m_ouEcuList.find(*itrEcu);
                if ( itrEcuStruct != m_ouClusterInfo.m_ouEcuList.end() )
                {
                    if  ( S_OK == itrEcuStruct->second.GetFrame(unSlotId, unCycle, oeChannel, ouFrame) )
                    {
                        return S_OK;
                    }
                }
                itrEcu++;
            }
        }
        return S_FALSE;
    }

    HRESULT GetFrame(UINT unSlotId, FRAME_STRUCT& ouFrame)
    {
        if ( m_ouClusterInfo.m_mapSlotEcu.size() > 0 )
        {
            std::map<SLOT, std::list<ECU_ID>>:: iterator itrSlotEcu = m_ouClusterInfo.m_mapSlotEcu.find(unSlotId);
            if ( itrSlotEcu !=  m_ouClusterInfo.m_mapSlotEcu.end() )
            {
                std::list<ECU_ID>::iterator itrEcu = itrSlotEcu->second.begin();
                while ( itrEcu !=  itrSlotEcu->second.end() )
                {
                    ECUMAP::iterator itrEcuStruct = m_ouClusterInfo.m_ouEcuList.find(*itrEcu);
                    if ( itrEcuStruct != m_ouClusterInfo.m_ouEcuList.end() )
                    {
                        if  ( S_OK == itrEcuStruct->second.GetFrame(unSlotId, ouFrame) )
                        {
                            return S_OK;
                        }
                    }
                    itrEcu++;
                }
            }
            return S_FALSE;
        }
        return S_FALSE;
    }


} CHANNEL_CONFIG;

typedef struct _FlexConfig
{
    CHANNEL_CONFIG m_ouFlexChannelConfig[CHANNEL_ALLOWED];
    INT m_nChannelsConfigured;
    void InitialiseConfig()
    {
        m_nChannelsConfigured = 1;
        for (int i = 0; i < CHANNEL_ALLOWED; i++ )
        {
            m_ouFlexChannelConfig[i].m_ouClusterInfo.Clear();
            m_ouFlexChannelConfig[i].m_strDataBasePath = "";
            m_ouFlexChannelConfig[i].m_strSlectedEculist.clear();
            m_ouFlexChannelConfig[i].m_ouLinParams.vInitialiseConfig();
            m_ouFlexChannelConfig[i].m_ouFlexRayParams.vInitialiseConfig();
        }
    }

} ClusterConfig;
