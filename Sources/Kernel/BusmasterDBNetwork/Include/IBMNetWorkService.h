#pragma once
#include "../../ProtocolDefinitions/ICluster.h"
#include "IBMNetWorkGetService.h"
class IBMNetWorkService : public IBMNetWorkGetService
{
public:
    virtual ~IBMNetWorkService() {};
    virtual ERRORCODE SetChannelSettings( ETYPE_BUS eouProtocol, int nChannelIndex, ChannelSettings* ) = 0;
    virtual ERRORCODE SetDBService( ETYPE_BUS eouProtocol, int nChannelIndex, int nDBIndex, ICluster* ) = 0;
    virtual ERRORCODE DeleteDBService( ETYPE_BUS eouProtocol, int nChannelIndex, std::string dbPath ) = 0;
    virtual ERRORCODE AddDBService( ETYPE_BUS eouProtocol, int nChannelIndex, ICluster* ) = 0;
    virtual ERRORCODE LoadDb( ETYPE_BUS eouProtocol, int nChannelIndex, std::string ) = 0;
    virtual ERRORCODE SetChannelCount( ETYPE_BUS eouProtocol, int nCount ) = 0;
    virtual ERRORCODE ParseDbFile( std::string strFileName, ETYPE_BUS clusterType, std::list<ICluster*>& ) = 0;
    virtual ERRORCODE SetSimulatedEcuList( ETYPE_BUS, int nChannelIndex, std::list<IEcu*>& ) = 0;
	virtual ERRORCODE ReSetNetwork(ETYPE_BUS eouProtocol) = 0;
    virtual bool isDbManagerAvailable() = 0;
};