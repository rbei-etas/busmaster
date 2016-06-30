//#include "DataTypes_stdafx.h"
#include "BusMasterNetWork.h"
#include <fstream>
#include <string>
#include <sstream>      // std::ostringstream
#include <sys/stat.h>
#include <map>
//#include <unistd.h>
//#include <time.h>
//#include "NodeSimCodeGenerator.h"
#define TAG_CLASS                   "class"
#define TAG_COLON                   ":"
#define TAG_SPACE                   " "
#define TAG_TAB                     "\t"
#define TAG_INHERITBASEFXCLASS      ": public BMFlexRayMsg"
#define TAG_INHERITBASEPDUCLASS     ": public BMPdu"
#define TAG_OPENFBRACE              "{"
#define TAG_CLOSEFBRACE             "}"
#define TAG_PUBLICSTART             "public:"
#define TAG_DATABYTEDECL            "unsigned char* m_DataBytes;"
#define TSG_BASESIGNAL              "BMSignal"
#define TAG_BMBASEMSG               "BMFlexRayMsg"
#define TAG_DATABYTES               "m_DataBytes"
#define TAG_SEMICOLON               ";"

ERRORCODE BMNetwork::GetChannelSettings(ETYPE_BUS eouProtocol, int nChannelIndex, ChannelSettings* ouChannelSettings)
{
    return m_ouProtocolConfig[eouProtocol].GetChannelSettings(nChannelIndex, ouChannelSettings);
    /*if ( nullptr == ouChannelSettings )
    {
        return EC_FAILURE;
    }

    int nChCount;
    m_ouProtocolConfig[eouProtocol].GetChannelCount(nChCount);

    if ( nChannelIndex < nChCount )
    {
        *ouChannelSettings =
    }*/
}

ERRORCODE BMNetwork::SetChannelSettings(ETYPE_BUS eouProtocol, int nChannelIndex, ChannelSettings* ouChannelSettings)
{
    return m_ouProtocolConfig[eouProtocol].SetChannelSettings(nChannelIndex, ouChannelSettings);
}

ERRORCODE BMNetwork::GetChannelCount(ETYPE_BUS eouProtocol, int& nChannelsConfigured)
{
    return m_ouProtocolConfig[eouProtocol].GetChannelCount(nChannelsConfigured);
}


ERRORCODE BMNetwork::GetDBServiceCount(ETYPE_BUS eouProtocol, int nChannelIndex, int& nCount)
{
    return m_ouProtocolConfig[eouProtocol].GetDBCount(nChannelIndex, nCount);
}

ERRORCODE BMNetwork::GetDBService(ETYPE_BUS eouProtocol, int nChannelIndex, int nDBIndex, ICluster** ouCluster)
{
    *ouCluster = nullptr;
    return m_ouProtocolConfig[eouProtocol].GetDBService(nChannelIndex, nDBIndex, ouCluster);
}
ERRORCODE BMNetwork::GetFrameList( ETYPE_BUS eouProtocol, int nChannelIndex, std::list<IFrame*>& frames )
{
    std::list<ICluster*> ouClusterList;
    m_ouProtocolConfig[eouProtocol].GetDBServiceList( nChannelIndex, ouClusterList );
    std::map<UID_ELEMENT, IElement*> frameList;
for ( auto itr : ouClusterList )
    {
        frameList.clear();
        itr->GetElementList( eFrameElement, frameList );
for ( auto itrFrame : frameList )
        {
            frames.push_back( (IFrame*)itrFrame.second );
        }
    }
    return EC_SUCCESS;
}
ERRORCODE BMNetwork::GetDBServiceList(ETYPE_BUS eouProtocol, int nChannelIndex, std::list<ICluster*>& ouClusterList)
{
    return m_ouProtocolConfig[eouProtocol].GetDBServiceList (nChannelIndex, ouClusterList);
}

ERRORCODE BMNetwork::SetDBService(ETYPE_BUS eouProtocol, int nChannelIndex, int /*nDBIndex*/, ICluster* ouCluster)
{
    return m_ouProtocolConfig[eouProtocol].SetDBService(nChannelIndex, ouCluster);
}

ERRORCODE BMNetwork::AddDBService(ETYPE_BUS eouProtocol, int nChannelIndex, ICluster* ouCluster)
{
    return m_ouProtocolConfig[eouProtocol].AddDBService(nChannelIndex, ouCluster);
}

ERRORCODE BMNetwork::LoadDb( ETYPE_BUS forCluster, int channelIndex, std::string dbFilePath )
{
    //Delete current cluster If Exists
    DeleteDBService( forCluster , channelIndex, dbFilePath);

    std::list<ICluster*> clusterList;
    ParseDbFile( dbFilePath, forCluster, clusterList );

    //Load First Cluster Only
    if ( clusterList.size() > 0 )
    {
        return AddDBService( forCluster, channelIndex, *( clusterList.begin() ) );
    }
    return EC_FAILURE;
}
ERRORCODE BMNetwork::DeleteDBService(ETYPE_BUS eouProtocol, int nChannelIndex, std::string dbPath)
{
    std::list<ICluster*> clusterList;
    m_ouProtocolConfig[eouProtocol].GetDBServiceList(nChannelIndex, clusterList);
    std::string path;
    int index = 0;
for (auto cluster : clusterList)
    {
        cluster->GetDBFilePath(path);
        if (path == dbPath)
        {
            if (EC_SUCCESS == m_ouProtocolConfig[eouProtocol].ReleaseDbService(nChannelIndex, index))
            {
                mDbManagerAcessor.mFreeCluster(cluster);
                return EC_SUCCESS;
            }
            else
            {
                return EC_FAILURE;
            }
        }
        index++;
    }
    return EC_FAILURE;
}


ERRORCODE BMNetwork::GetSimulatedEcuList(ETYPE_BUS eouProtocol, int nChannelIndex, std::list<IEcu*>& ouEcuList)
{
    return m_ouProtocolConfig[eouProtocol].GetSimulatedEcuList(nChannelIndex, ouEcuList);
}

ERRORCODE BMNetwork::GetFrame(ETYPE_BUS eouProtocol, int nChannelIndex, unsigned int unId, void* pProtocolData, IFrame** pouFrame)
{
    return m_ouProtocolConfig[eouProtocol].GetFrame(nChannelIndex, unId, pProtocolData, pouFrame);
}

ERRORCODE BMNetwork::GetElementByName(ETYPE_BUS eouProtocol, int nChannelIndex, eClusterElementType oueElementType, std::string strName, IElement** pouElement)
{
    return m_ouProtocolConfig[eouProtocol].GetElementByName(nChannelIndex, oueElementType, strName, pouElement);
}
ERRORCODE BMNetwork::SetChannelCount(ETYPE_BUS eouProtocol, int /*nCount*/)
{
    return m_ouProtocolConfig[eouProtocol].SetChannelCount(1);
    return EC_NA;
}

ERRORCODE BMNetwork::SetSimulatedEcuList(ETYPE_BUS eouProtocol, int nChannelIndex, std::list<IEcu*>& ouEcuList)
{
    return m_ouProtocolConfig[eouProtocol].SetSimulatedEcuList(nChannelIndex, ouEcuList);
}

ERRORCODE BMNetwork::ReSetNetwork(ETYPE_BUS eouProtocol)
{
    int channels;
    m_ouProtocolConfig[eouProtocol].GetChannelCount(channels);
    for (int i = 0; i < channels; i++)
    {
        std::list<ICluster*> clusterList;
        m_ouProtocolConfig[eouProtocol].GetDBServiceList(i, clusterList);
        std::list<std::string> pathList;
        std::string path;
for (auto itr : clusterList)
        {
            itr->GetDBFilePath(path);
            pathList.push_back(path);
        }
for (auto itr : pathList)
        {
            DeleteDBService(eouProtocol, i, itr);
        }
    }
    return m_ouProtocolConfig[eouProtocol].ResetConfiguration();
}




BMNetwork::BMNetwork()
{

}




ERRORCODE BMNetwork::ParseDbFile( std::string strFileName, ETYPE_BUS clusterType, std::list<ICluster*>& ouClusterList )
{
    std::list< std::string> strFile;
    std::list<ClusterResult> ouClusterResult;

    ouClusterList.clear();

    ERRORCODE ecCode = mDbManagerAcessor.mParseDbFile( strFileName, clusterType, ouClusterResult);

    if ( EC_FAILURE != ecCode )
    {
        for ( std::list<ClusterResult>::iterator itr = ouClusterResult.begin(); itr != ouClusterResult.end(); itr++ )
        {
            ouClusterList.push_back(itr->m_pCluster);
        }
    }
    return ecCode;
}

BMProtocolConfig::BMProtocolConfig()
{
    m_nChannelConfigured  = 0;
}

ERRORCODE BMProtocolConfig::ResetConfiguration()
{
    for ( int i = 0; i < m_nChannelConfigured; i++ )
    {
        m_ouChannelConfig[i].ResetChannelSettings();
    }
    m_nChannelConfigured  = 0;
    return EC_SUCCESS;
}


ERRORCODE BMProtocolConfig::SetChannelCount(int nCount)
{
    m_nChannelConfigured = nCount;
    return EC_SUCCESS;
}

ERRORCODE BMProtocolConfig::GetChannelSettings( int nChannelIndex, ChannelSettings* ouSettings)
{
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].GetChannelSettings(ouSettings);
    }
    return EC_FAILURE;
}

ERRORCODE BMProtocolConfig::GetDBService( int nChannelIndex, int nDbIndex, ICluster** ouCluster)
{
    *ouCluster = nullptr;
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].GetDB(nDbIndex, ouCluster);
    }

    return EC_FAILURE;
}

ERRORCODE BMProtocolConfig::GetDBServiceList( int nChannelIndex, std::list<ICluster*>& ouClusterList)
{
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].GetDBList(ouClusterList);
    }
    return EC_FAILURE;
}

ERRORCODE BMProtocolConfig::GetDBCount(int nChannelIndex, int& nCount)
{
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].GetDBCount(nCount);
    }
    return EC_FAILURE;
}

ERRORCODE BMProtocolConfig::GetFrame( int nChannelIndex, unsigned int unId, void* ouFrameProps, IFrame** pouFrame)
{
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].GetFrame(unId, ouFrameProps, pouFrame);
    }
    return EC_FAILURE;
}

ERRORCODE BMProtocolConfig::GetElementByName(int nChannelIndex, eClusterElementType oueElementType, std::string strName, IElement** pElement)
{
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].GetElementByName(oueElementType, strName, pElement);
    }
    return EC_FAILURE;
}



ERRORCODE BMProtocolConfig::SetChannelSettings( int nChannelIndex, ChannelSettings* ouChannelSettings)
{
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].SetChannelSettings(ouChannelSettings);
    }
    return EC_FAILURE;
}

ERRORCODE BMProtocolConfig::SetDBService( int nChannelIndex, ICluster* ouCluster)
{
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].SetDB(ouCluster);
    }
    return EC_FAILURE;
}

ERRORCODE BMProtocolConfig::AddDBService( int nChannelIndex, ICluster* ouCluster)
{
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].AddDB(ouCluster);
    }
    return EC_FAILURE;
}

ERRORCODE BMProtocolConfig::LoadDb( int channelIndex, std::string dbFilePath )
{
    channelIndex = 0;       //Comment this line once channelwise nework is added for CAN
    return EC_FAILURE;
}
ERRORCODE BMProtocolConfig::ReleaseDbService(int nChannelIndex, int dbIndex)
{
    if ( nChannelIndex < m_nChannelConfigured )
    {
        return m_ouChannelConfig[nChannelIndex].ReleaseDb( dbIndex );
    }
    return EC_FAILURE;
}

ERRORCODE BMProtocolConfig::GetChannelCount(int& nChannelsConfigured)
{
    nChannelsConfigured = m_nChannelConfigured;
    return EC_SUCCESS;
}

ERRORCODE BMProtocolConfig::GetProtocolType(ETYPE_BUS& ouePrtocol)
{
    ouePrtocol = m_oueBusType;
    return EC_SUCCESS;
}

ERRORCODE BMProtocolConfig::SetProtocolType(ETYPE_BUS ouePrtocol)
{
    m_oueBusType = ouePrtocol;
    return EC_SUCCESS;
}

ERRORCODE BMProtocolConfig::GetSimulatedEcuList(int nChannelIndex, std::list<IEcu*>& ouEcuList)
{
    return m_ouChannelConfig[nChannelIndex].GetSimulatedEcuList(ouEcuList);;
}

ERRORCODE BMProtocolConfig::SetSimulatedEcuList(int nChannelIndex, std::list<IEcu*>& ouEcuList)
{
    return m_ouChannelConfig[nChannelIndex].SetSimulatedEcuList(ouEcuList);
}

ERRORCODE BMChannelConfig::GetChannelSettings( ChannelSettings* ouChannelSettings)
{
    if ( nullptr != ouChannelSettings )
    {
        *ouChannelSettings = m_ouChannelSettings;
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}
ERRORCODE BMChannelConfig::ResetChannelSettings()
{
    m_pSimulatedEcuList.clear();
    m_ouDbList.clear();
    m_ouChannelSettings.Initailise();
    return EC_SUCCESS;
}


ERRORCODE BMChannelConfig::GetFrame( unsigned int unId, void* ouFrameProps, IFrame** ouFrame)
{
    *ouFrame = nullptr;
for ( auto itr : m_ouDbList)
    {
        itr->GetFrame(unId, ouFrameProps, ouFrame);
        if ( nullptr != *ouFrame )
        {
            return EC_SUCCESS;
        }
    }
    return EC_FAILURE;
}
ERRORCODE BMChannelConfig::GetElementByName(eClusterElementType oueElementType, std::string strName, IElement** pElement)
{
    *pElement = nullptr;
for(auto itr : m_ouDbList)
    {
        itr->GetElementByName(oueElementType, strName, pElement);
        if ( nullptr != *pElement )
        {
            return EC_SUCCESS;
        }
    }
    return EC_FAILURE;
}

ERRORCODE BMChannelConfig::SetChannelSettings( ChannelSettings* ouChannelSettings)
{
    if ( nullptr !=  ouChannelSettings)
    {
        m_ouChannelSettings = *ouChannelSettings;
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}

ERRORCODE BMChannelConfig::AddDB(ICluster* pouCluster)
{
    m_ouDbList.push_back(pouCluster);
    return EC_SUCCESS;
}

ERRORCODE BMChannelConfig::SetDB(ICluster* pouCluster)
{
    m_ouDbList.clear();     //TODO
    m_ouDbList.push_back(pouCluster);
    return EC_SUCCESS;
}

ERRORCODE BMChannelConfig::ReleaseDb(int dbIndex)
{
    if (dbIndex < m_ouDbList.size())
    {
        auto itrDb = m_ouDbList.begin();
        std::advance(itrDb, dbIndex);
        if (m_ouDbList.end() != itrDb)
        {
            m_ouDbList.remove(*itrDb);
            return EC_SUCCESS;
        }
        return EC_FAILURE;
    }
    return EC_FAILURE;
}

ERRORCODE BMChannelConfig::GetDBCount(int& nCount)
{
    nCount = m_ouDbList.size();
    return EC_SUCCESS;
}

ERRORCODE BMChannelConfig::GetDB(int nIndex, ICluster** pCluster)
{
    if ( nullptr == pCluster )
    {
        return EC_FAILURE;
    }
    *pCluster = nullptr;
    if ( m_ouDbList.size() > nIndex )
    {
        auto itr = m_ouDbList.cbegin();
        std::advance(itr, nIndex);
        if ( m_ouDbList.end() != itr )
        {
            *pCluster = *itr;
            return EC_SUCCESS;
        }
    }

    return EC_NA;
}

ERRORCODE BMChannelConfig::GetDBList(std::list<ICluster*>& ouDbList)
{
    ouDbList = m_ouDbList;
    return EC_SUCCESS;
}

ERRORCODE BMChannelConfig::ClearDBServices()
{
    m_ouDbList.clear(); //TODO
    return EC_NA;
}

ERRORCODE BMChannelConfig::GetSimulatedEcuList(std::list<IEcu*>& ouEcuList)
{
    ouEcuList.clear();
    ouEcuList = m_pSimulatedEcuList;
    return EC_NA;
}


ERRORCODE BMChannelConfig::SetSimulatedEcuList(std::list<IEcu*>& ouEcuList)
{
    m_pSimulatedEcuList.clear();
    m_pSimulatedEcuList = ouEcuList;
    return EC_SUCCESS;
}


void ChannelSettings::Initailise()
{
    m_ouCanSettings.vInitialiseConfig();
    m_ouLINSettings.vInitialiseConfig();
    m_ouFlexRaySettings.vInitialiseConfig();
}






//ERRORCODE BMNetwork::GenerateCode(ICluster* pCluster, std::string strFileName , bool bRegenerate , bool bGenerateObj )
//{
//    return S_OK;
//    //NodeSimCodeGenerator ouCodeGenrator;
//    //return ouCodeGenrator.CompileCode(pCluster,  bRegenerate, bGenerateObj);
//}
//
//
//ERRORCODE BMNetwork::GenerateMakeFile(ETYPE_BUS oueProtocolType, std::string strForCppFile)
//{
//    return S_OK;
//    //NodeSimCodeGenerator ouCodeGenrator;
//    //return ouCodeGenrator.GenerateMakeFile(this, oueProtocolType,  strForCppFile);
//}

//std::string BMNetwork::GetUniqueFrameName(IFrame* pouFrame)
//{
//    if ( pouFrame == nullptr )
//    {
//        return "";
//    }
//    return "";
//    /*std::ostringstream omstrTempFrameName;
//    std::string strFrameName;
//    NodeSimCodeGenerator ouNodeSimcodeGen;
//    FrameProps ouProps;
//
//    pouFrame->GetProperties(ouProps);
//    pouFrame->GetName(strFrameName);
//
//    if ( ouProps.m_eFrameType > eFlexRay_Frame_Start && ouProps.m_eFrameType < eFlexRay_Frame_End )
//    {
//        pouFrame->GetProperties(ouProps);
//        omstrTempFrameName << strFrameName;
//
//        omstrTempFrameName << "_" << ouProps.m_ouFlexRayFrameProps.m_unSlotId << "_" << ouProps.m_ouFlexRayFrameProps.m_unBaseCycle <<"_ch_" \
//                           << ouNodeSimcodeGen.GetChannelName(ouProps.m_ouFlexRayFrameProps.m_oueChannel);
//        return omstrTempFrameName.str();
//    }
//
//    else
//    {
//        return strFrameName;
//    }*/
//}
//ERRORCODE BMNetwork::GetUniqueFrameNames(ETYPE_BUS eouProtocol, int nChannel, std::map<std::string, IFrame*>& strNameList)
//{
//    std::list<IFrame*> lstMsgNames;
//    //CGlobalObj::ouGetObj(m_eBus).m_ouClusterConfig->GetDBService(m_eBus, atoi(strChnlNum)-1, 0, &pCluster);
//
//  GetFrameList(eouProtocol, 0, lstMsgNames);
//for ( auto itr : lstMsgNames )
//        {
//            strNameList[GetUniqueFrameName(itr)] = itr;
//        }
//
//
//    return EC_SUCCESS;
//}

bool BMNetwork::isDbManagerAvailable()
{
    return mDbManagerAcessor.isDbManagerAvailable();
}
