#pragma once
#include <Windows.h>
#include <string>
#include <list>
//#include "../Include/BaseDefs.h"
#include <string>
#include "../ProtocolDefinitions/ICluster.h"
class AccessDBManager
{
    typedef ERRORCODE( *PARSEDBFILE )( std::string& strFileName, ETYPE_BUS clusterType, std::list<ClusterResult>& ouClusterResultList );
    typedef ERRORCODE( *FREECLUSTER )( ICluster* );
    HMODULE mDllHandle;
    bool mDbManagerAvailable;
public:
    AccessDBManager();
    PARSEDBFILE mParseDbFile;
    FREECLUSTER mFreeCluster;
    bool isDbManagerAvailable();
    HRESULT LoadDbManager();
    void ReleaseDbmanager();
    ~AccessDBManager();
};

