#pragma once

//#include "Cluster.h"
#include "Include/IBMNetWorkService.h"
#include <algorithm>
#include "AccessDBManager.h"
#include "../BusmasterDriverInterface/Include/DeviceListInfo.h"





class BMChannelConfig
{
private:
    std::list<IEcu*> m_pSimulatedEcuList;
    std::list<ICluster*> m_ouDbList; //DataBase Servie more than one db can be allowed
    ChannelSettings m_ouChannelSettings;     //ChannelSettings - BaudRate....
public:
    ERRORCODE GetChannelSettings( ChannelSettings* );
    ERRORCODE SetChannelSettings( ChannelSettings* );
    ERRORCODE AddDB(ICluster*);
    ERRORCODE SetDB(ICluster*);
    ERRORCODE ReleaseDb(int nIndex);
    ERRORCODE GetDBCount(int& nCount);
    ERRORCODE GetDB(int nIndex, ICluster**);
    ERRORCODE GetDBList(std::list<ICluster*>&);
    ERRORCODE ClearDBServices();
    ERRORCODE GetSimulatedEcuList(std::list<IEcu*>&);
    ERRORCODE SetSimulatedEcuList(std::list<IEcu*>&);
    ERRORCODE GetFrame( unsigned int unId, void* ouFrameProps, IFrame**);
    ERRORCODE GetElementByName(eClusterElementType oueElementType, std::string strName, IElement** pElement);
    ERRORCODE ResetChannelSettings();
};

class BMProtocolConfig
{
private:
    BMChannelConfig m_ouChannelConfig[CHANNEL_ALLOWED];     //Max Channels
    int m_nChannelConfigured;                               //Number of Channels Configured
    ETYPE_BUS m_oueBusType;
public:
    BMProtocolConfig();
    ERRORCODE GetChannelSettings( int nChannelIndex, ChannelSettings* );
    ERRORCODE GetDBService( int nChannelIndex, int nDbIndex, ICluster**);
    ERRORCODE GetDBServiceList( int nChannelIndex, std::list<ICluster*>&);
    ERRORCODE GetDBCount(int nChannelIndex, int& nCount);
    ERRORCODE GetFrame( int nChannelIndex, unsigned int unId, void* ouFrameProps, IFrame**);
    ERRORCODE GetElementByName(int nChannelIndex, eClusterElementType oueElementType, std::string strName, IElement**);
    ERRORCODE SetChannelSettings( int nChannelIndex, ChannelSettings* );
    ERRORCODE SetDBService( int nChannelIndex, ICluster*);
    ERRORCODE AddDBService( int nChannelIndex, ICluster*);
    ERRORCODE LoadDb( int channelIndex, std::string dbFilePath );
    ERRORCODE ReleaseDbService(int nChannelIndex, int dbIndex);
    ERRORCODE GetChannelCount(int& nChannelsConfigured);
    ERRORCODE GetProtocolType(ETYPE_BUS& ouePrtocol);
    ERRORCODE SetProtocolType(ETYPE_BUS ouePrtocol);
    ERRORCODE GetSimulatedEcuList(int nChannelIndex, std::list<IEcu*>&);
    ERRORCODE SetSimulatedEcuList(int nChannelIndex, std::list<IEcu*>&);
    ERRORCODE SetChannelCount(int nCount);
    ERRORCODE ResetConfiguration();
};





class BMNetwork : public IBMNetWorkService
{

private:
    BMProtocolConfig m_ouProtocolConfig[ETYPE_BUS::BUS_TOTAL];
    //std::string GetUniqueFrameName(IFrame* pouFrame);
    AccessDBManager mDbManagerAcessor;

public:

    BMNetwork();
    ERRORCODE GetChannelSettings(ETYPE_BUS eouProtocol, int nChannelIndex, ChannelSettings* );
    ERRORCODE SetChannelSettings(ETYPE_BUS eouProtocol, int nChannelIndex, ChannelSettings* );
    ERRORCODE GetChannelCount(ETYPE_BUS eouProtocol, int& nChannelsConfigured);
    ERRORCODE SetSimulatedEcuList(ETYPE_BUS, int nChannelIndex, std::list<IEcu*>&);
    //ERRORCODE GenerateCode(ICluster* pCluster, std::string strFileName = "", bool bRegenerate = false, bool bGenerateObj = true);
    //ERRORCODE GenerateMakeFile(ETYPE_BUS, std::string strForCppFile);
    //ERRORCODE GetUniqueFrameNames(ETYPE_BUS, int nChannelIndex, std::map<std::string, IFrame*>& strNameList);
    bool isDbManagerAvailable();

    ERRORCODE SetChannelCount(ETYPE_BUS eouProtocol, int nCount);

    ERRORCODE GetDBServiceCount(ETYPE_BUS eouProtocol, int nChannelIndex, int& nCount);
    ERRORCODE GetDBService(ETYPE_BUS eouProtocol, int nChannelIndex, int nDBIndex, ICluster**);
    ERRORCODE GetDBServiceList(ETYPE_BUS eouProtocol, int nChannelIndex, std::list<ICluster*>&);
    ERRORCODE GetFrameList( ETYPE_BUS eouProtocol, int nChannelIndex, std::list<IFrame*>& );
    ERRORCODE SetDBService(ETYPE_BUS eouProtocol, int nChannelIndex, int nDBIndex, ICluster*);

    ERRORCODE AddDBService(ETYPE_BUS eouProtocol, int nChannelIndex, ICluster*);
    ERRORCODE DeleteDBService(ETYPE_BUS eouProtocol, int nChannelIndex, std::string dbPath);




    ERRORCODE GetSimulatedEcuList(ETYPE_BUS, int nChannelIndex, std::list<IEcu*>&);
    ERRORCODE GetFrame(ETYPE_BUS eouProtocol, int nChannelIndex, unsigned int unId, void* pProtocolData, IFrame**);
    ERRORCODE GetElementByName(ETYPE_BUS eouProtocol, int nChannelIndex, eClusterElementType oueElementType, std::string strName, IElement**);

    ERRORCODE ParseDbFile( std::string strFileName, ETYPE_BUS forCluster, std::list<ICluster*>& );
    ERRORCODE LoadDb( ETYPE_BUS eouProtocol, int nChannelIndex, std::string );

    ERRORCODE ReSetNetwork(ETYPE_BUS eouProtocol);

private:
    ERRORCODE ParseDBFile(std::string& strFileName, std::list<ClusterResult>& ouClusterResultList);
};
