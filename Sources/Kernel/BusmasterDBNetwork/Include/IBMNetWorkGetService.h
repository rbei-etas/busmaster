#pragma once
//#include "Cluster.h"
//#include
//Settings may be inherited from some common interface
#include <list>
#include "../../ProtocolDefinitions/CommonDefines.h"
#include "../../ProtocolDefinitions/ProtocolsDefinitions.h"
#include "../../ProtocolDefinitions/FlexRayDefines.h"
#include "../../ProtocolDefinitions/ICluster.h"

class CanChannelSettings
{
public:
    int m_nBaudRate;
    std::list<std::string> m_SelectedEcuList;
    void vInitialiseConfig()
    {
        m_nBaudRate = 500; //Define it
        m_SelectedEcuList.clear();
    }

};

enum eFlexRayKeySlotMode
{
    eKeySlotManual,
    eKeySlotOff,
    eKeySlotNone
};

class FlexRayKeySlot
{
public:

    int m_nKeySlot;
    bool m_bKeySlotLeadColdStart;
    eFlexRayKeySlotMode m_eKeySlotMode;
    eFlexRayKeySlotType m_eKeySlotType;
    eFlexRayChannel m_eChannelMask;

    FlexRayKeySlot()
    {
        m_nKeySlot = 0;
        m_bKeySlotLeadColdStart = false;
        m_eKeySlotMode = eKeySlotOff;
        m_eKeySlotType = eNone;
        m_eChannelMask = eFlexRayChannelNone;
    }
};


class FlexRayChannelSettings
{
public:

    FlexRayKeySlot m_omKeySlotConfig[2]; // 2 Key Slot Configuration
    std::list<std::string> m_SelectedEcuList;
    FlexRaySettings m_ouFlexrayProtocolSettings;
    FlexRayChannelSettings()
    {
        m_omKeySlotConfig[0].m_nKeySlot = -1;
        m_omKeySlotConfig[0].m_bKeySlotLeadColdStart = false;
        m_omKeySlotConfig[1].m_nKeySlot = -1;
        m_omKeySlotConfig[1].m_bKeySlotLeadColdStart = false;

        m_omKeySlotConfig[0].m_eKeySlotMode = eKeySlotOff;
        m_omKeySlotConfig[1].m_eKeySlotMode = eKeySlotOff;

        m_omKeySlotConfig[0].m_eKeySlotType = eNone;
        m_omKeySlotConfig[1].m_eKeySlotType = eNone;

        m_omKeySlotConfig[0].m_eChannelMask = eFlexRayChannelNone;
        m_omKeySlotConfig[1].m_eChannelMask = eFlexRayChannelNone;
    }
    void vInitialiseConfig()
    {
        m_omKeySlotConfig[0].m_nKeySlot = -1;
        m_omKeySlotConfig[0].m_bKeySlotLeadColdStart = false;
        m_omKeySlotConfig[1].m_nKeySlot = -1;
        m_omKeySlotConfig[1].m_bKeySlotLeadColdStart = false;

        m_omKeySlotConfig[0].m_eKeySlotMode = eKeySlotOff;
        m_omKeySlotConfig[1].m_eKeySlotMode = eKeySlotOff;

        m_omKeySlotConfig[0].m_eKeySlotType = eNone;
        m_omKeySlotConfig[1].m_eKeySlotType = eNone;

        m_omKeySlotConfig[0].m_eChannelMask = eFlexRayChannelNone;
        m_omKeySlotConfig[1].m_eChannelMask = eFlexRayChannelNone;
        m_SelectedEcuList.clear();
    }

};

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
    CSheduleTable( const CSheduleTable& ouRefObj )
    {
        m_strTableName = ouRefObj.m_strTableName;
        m_listCommands.clear();
        m_listCommands.insert( m_listCommands.begin(), ouRefObj.m_listCommands.begin(), ouRefObj.m_listCommands.end() );
    }
    CSheduleTable operator=( const CSheduleTable& ouRefObj )
    {
        m_strTableName = ouRefObj.m_strTableName;
        m_listCommands.clear();
        m_listCommands.insert( m_listCommands.begin(), ouRefObj.m_listCommands.begin(), ouRefObj.m_listCommands.end() );
        return *this;
    }

};

class LINChannelSettings
{
public:
    int m_nBaudRate;
    bool m_bIsMasterMode;
    std::string m_strProtocolVersion;
    bool m_bOverWriteSettings;
    std::list<std::string> m_SelectedEcuList;
    //std::list<IScheduleTable*> m_ouScheduleTableList;
    std::list<CSheduleTable> ouLinParams;

    void vInitialiseConfig()
    {
        m_nBaudRate = 19200;
        m_strProtocolVersion = "LIN 2.0";
        m_bOverWriteSettings = true;
        m_bIsMasterMode = false;
        ouLinParams.clear();
    }
    LINChannelSettings()
    {
        vInitialiseConfig();
    }
    LINChannelSettings( const LINChannelSettings& ouRefObj )
    {
        m_nBaudRate = ouRefObj.m_nBaudRate;
        m_strProtocolVersion = ouRefObj.m_strProtocolVersion;
        m_bOverWriteSettings = ouRefObj.m_bOverWriteSettings;
        m_bIsMasterMode = ouRefObj.m_bIsMasterMode;
        ouLinParams.clear();
        ouLinParams.insert( ouLinParams.begin(), ouRefObj.ouLinParams.begin(), ouRefObj.ouLinParams.end() );
    }
    LINChannelSettings operator=( const LINChannelSettings& ouRefObj )
    {
        m_nBaudRate = ouRefObj.m_nBaudRate;
        m_strProtocolVersion = ouRefObj.m_strProtocolVersion;
        m_bOverWriteSettings = ouRefObj.m_bOverWriteSettings;
        m_bIsMasterMode = ouRefObj.m_bIsMasterMode;
        ouLinParams.clear();
        ouLinParams.insert( ouLinParams.begin(), ouRefObj.ouLinParams.begin(), ouRefObj.ouLinParams.end() );
        return *this;
    }
};

class ChannelSettings
{
public:
    CanChannelSettings m_ouCanSettings;
    FlexRayChannelSettings m_ouFlexRaySettings;
    LINChannelSettings m_ouLINSettings;
    void Initailise();
};


class IBMNetWorkGetService
{
public:
    virtual ERRORCODE GetChannelSettings( ETYPE_BUS eouProtocol, int nChannelIndex, ChannelSettings* ) = 0;
    virtual ERRORCODE GetDBServiceList( ETYPE_BUS eouProtocol, int nChannelIndex, std::list<ICluster*>& )=0;
    virtual ERRORCODE GetDBService( ETYPE_BUS eouProtocol, int nChannelIndex, int nDBIndex, ICluster** ) = 0;
    virtual ERRORCODE GetFrame( ETYPE_BUS eouProtocol, int nChannelIndex, unsigned int unId, void* pProtocolData, IFrame** ) = 0;     //pProtocolData extra unique info base cycle, channelab etc
    virtual ERRORCODE GetChannelCount( ETYPE_BUS eouProtocol, int& nChannelsConfigured ) = 0;
    virtual ERRORCODE GetDBServiceCount( ETYPE_BUS eouProtocol, int nChannelIndex, int& nCount ) = 0;
    virtual ERRORCODE GetFrameList( ETYPE_BUS eouProtocol, int nChannelIndex, std::list<IFrame*>& ) = 0;
    virtual ERRORCODE GetSimulatedEcuList( ETYPE_BUS, int nChannelIndex, std::list<IEcu*>& ) = 0;
    virtual ERRORCODE ReSetNetwork( ETYPE_BUS eouProtocol ) = 0;
    //virtual ERRORCODE GenerateCode( ICluster* pCluster, std::string strFileName = "", bool bRegenerate = false, bool bGenerateObj = true ) = 0;
    //virtual ERRORCODE GenerateMakeFile( ETYPE_BUS, std::string strForCppFile ) = 0;
    //virtual ERRORCODE GetUniqueFrameNames(ETYPE_BUS, int nChannelIndex, std::map<std::string, IFrame*>& strNameList) = 0;
    virtual ERRORCODE GetElementByName( ETYPE_BUS eouProtocol, int nChannelIndex, eClusterElementType oueElementType, std::string strName, IElement** pouElement ) = 0;
};