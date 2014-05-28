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
        int i = 0;
    }

};

class FRAME_STRUCT
{
public:
    std::string m_strFrameId;
    std::string m_strFrameName;
    short   m_nSlotId;
    unsigned char m_nBaseCycle;
    unsigned char m_nReptition;
    unsigned char m_nLength;
    //bool m_bConsiderPdu;
    std::list<PDU_STRUCT>    m_ouPduList;
    //list<SIGNAL_STRUCT>   m_ouSignalList;
    ECHANNEL    m_ouChannel;
    ESLOT_TYPE m_eSlotType;
    ESYNC m_eSync;

    bool operator == (FRAME_STRUCT& ob1)
    {
        bool bResult = false;

        if((m_strFrameId == ob1.m_strFrameId) && (m_nSlotId == ob1.m_nSlotId) && (m_nBaseCycle == ob1.m_nBaseCycle))
        {
            bResult = true;
        }

        return bResult;
    }
    bool operator < (FRAME_STRUCT& ob1)
    {
        return (m_strFrameId < ob1.m_strFrameId);
    }

    bool operator > (FRAME_STRUCT& ob1)
    {
        return (m_strFrameId > ob1.m_strFrameId);
    }

    ~FRAME_STRUCT()
    {

    }

    FRAME_STRUCT();
    HRESULT GetPDUList ( std::list<SIGNAL_STRUCT>& ouSignalList );
    HRESULT GetSignalList ( std::string omStrPduName, std::list<SIGNAL_STRUCT>& ouSignalList );
    HRESULT GetSignalList ( std::list<SIGNAL_STRUCT>& ouSignalList );
    HRESULT GetSignalNames (CStringList& ouSignalList );
    HRESULT GetSignalCount ( int& nCount);
};

bool Compare_Frame_Structs(FRAME_STRUCT& ob1, FRAME_STRUCT& ob2 );

class ECU_Struct
{
public:
    std::string m_strEcuId;
    std::string m_strECUName;

    //One Controller per ECU
    ABS_FLEXRAY_SPEC_CNTLR m_ouControllerParams;
    int m_nKeySlot;
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
    std::string m_strProtocolVersion;
    bool m_bOverWriteSettings;
    void vInitialiseConfig()
    {
        m_nBaudRate = 19200;
        m_strProtocolVersion = "LIN 2.0";
        m_bOverWriteSettings = true;
    }
    LinChannelParam()
    {
        vInitialiseConfig();
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
