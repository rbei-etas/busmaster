#pragma once

#include <memory>

#ifdef DBMANAGER_EXPORTS
#define DB_EXPORTS __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define DB_EXPORTS
#define EXPIMP_TEMPLATE extern
#endif

#include "IClusterProps.h"

//Forword Decleration;
class ICluster;
class IElement;
class IEcu;
class IFrame;
class IPdu;
class ISignal;
class ICoding;
class IScheduleTable;
class IFlexRayDBManager;
class CFibexCluster;



class INotifyClusterChange
{
public:
    virtual ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData) = 0;
};

class ICluster
{
public:
    ICluster(void) {};
    virtual ~ICluster(void) {};

    virtual ERRORCODE GetNextUniqueId(UID_ELEMENT&) = 0;
    //Editor
    virtual ERRORCODE LoadFromFile(const std::string& strFileName, std::list<ParsingResults>& ouErrors, std::list<ParsingResults>& ouWarnings) = 0;
    virtual ERRORCODE SaveToFile(const std::string& strFileName) = 0;
    virtual ERRORCODE ValidateCluster(std::list<std::string>&) = 0;
    virtual ERRORCODE GetDBFilePath(std::string& oustrDbFileList) = 0;
    virtual ERRORCODE GetDBFileChecksum(std::string& strDBFileChecksum) = 0;
    virtual ERRORCODE Clear() = 0;

    virtual ERRORCODE CreateElement(eClusterElementType eType, IElement** pElement) = 0;
    virtual ERRORCODE DeleteElement(eClusterElementType eType, UID_ELEMENT& nUniqueId) = 0;

    virtual ERRORCODE GetProperties(ePropertyType ouePropertyType, void* pProperties) = 0;
    virtual ERRORCODE SetProperties(ePropertyType ouePropertyType, void* pProperties) = 0;

    virtual ERRORCODE GetElement(eClusterElementType eType, UID_ELEMENT nId, IElement** pElement ) = 0;
    virtual ERRORCODE GetElementList(eClusterElementType eType, std::map<UID_ELEMENT, IElement*>& pElement ) = 0;
    virtual ERRORCODE GetElementByName(eClusterElementType eType, std::string ouElementName, IElement** pElement) = 0;
    virtual ERRORCODE GetElementListByName(eClusterElementType eType, std::string& strEcuName,   std::list<IElement*>& pElementList) = 0;

    virtual ERRORCODE GetClusterType( ETYPE_BUS& ) = 0;

    virtual ERRORCODE EnableEventNotofications(bool bEnable = true) = 0;
    virtual bool isNotoficationsEnabled() = 0;
    //Interpretation

    virtual ERRORCODE GetEcu(const std::string& strEcuName, IEcu** pEcu) = 0;
    virtual ERRORCODE GetName(std::string& strClusterName) = 0;

    virtual ERRORCODE GetFrame(unsigned int& unId, void* vProtocolFrameParams, IFrame** pFrame) = 0;
    virtual ERRORCODE GetEcuList( std::list< IEcu*>& pEcuList ) = 0;
    virtual ERRORCODE GetFrameList( std::list< IFrame*>& pFrameList ) = 0;
    virtual ERRORCODE GetSignalList( std::list< ISignal*>& pSignalList ) = 0;
    virtual ERRORCODE GetPduList( std::list< IPdu*>& pPduList ) = 0;

    virtual eClusterElementType GetElementType(UID_ELEMENT& ouElementUid) = 0;

    virtual ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification, UID_ELEMENT uidOnElement = INVALID_UID_ELEMENT) = 0;
    virtual ERRORCODE NotifyClusterChange(eAction oueAction, UID_ELEMENT& m_nUnId, eClusterElementType& oueElementType, void* pAcionData) = 0;

};

class IElement
{
protected:
    eClusterElementType m_oueElementType;
    std::string m_strName;
    ICluster* m_pBaseCluster;
    UID_ELEMENT m_nUnId;



    IElement(ICluster* pCluster)
    {
        m_pBaseCluster = pCluster;
        m_pBaseCluster->GetNextUniqueId(m_nUnId);
        m_oueElementType = eInvalidElement;
    }

public:
    virtual ERRORCODE GetName(std::string& strName)
    {
        strName = m_strName;
        return EC_SUCCESS;
    }

    virtual ERRORCODE SetName(const std::string& strName)
    {
        //if ( m_strName != strName )
        {
            eNameChangeActionData ouData;
            ouData.m_strNewName = strName;
            ouData.m_strOldName = m_strName;

            m_strName = strName;
            if( nullptr != m_pBaseCluster )
            {
                m_pBaseCluster->NotifyClusterChange(eNameChange, this->m_nUnId, m_oueElementType, &ouData);
            }
            return EC_SUCCESS;
        }
        return EC_SUCCESS;
    }

    virtual ERRORCODE GetUniqueId(UID_ELEMENT& unId)
    {
        unId = m_nUnId;
        return EC_SUCCESS;
    }

    virtual UID_ELEMENT GetUniqueId()
    {
        return m_nUnId;
    }

    virtual eClusterElementType GetElementType()
    {
        return m_oueElementType;
    }

    virtual ERRORCODE SetElementType(eClusterElementType& ouElementType)
    {
        m_oueElementType = ouElementType;
        return EC_SUCCESS;
    }

    virtual ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification) = 0;

};

class IEcu : public IElement
{
protected:
    IEcu(ICluster* pBaseCluster):IElement(pBaseCluster) {};
    virtual ~IEcu() {};
public:
    //Editor
    virtual ERRORCODE GetEcuType(eEcuType&) = 0;
    virtual ERRORCODE MapFrame(eDIR eDir, UID_ELEMENT& nId) = 0;
    virtual ERRORCODE UnMapFrame(eDIR eDir, UID_ELEMENT& nId) = 0;

    virtual ERRORCODE MapSignal(eDIR eDir, UID_ELEMENT& nId) = 0;
    virtual ERRORCODE UnMapSignal(eDIR eDir, UID_ELEMENT& nId) = 0;


    //Interpretation
    virtual ERRORCODE GetFrameList(eDIR eDir, std::list<IFrame*>& ouFrames) = 0;
    virtual ERRORCODE GetFrame(UID_ELEMENT& nId, void* pProtocolSpecParam, IFrame** ouFrame) = 0;

    virtual ERRORCODE GetSignalList(eDIR eDir, std::map<ISignal*, ISignal*>& ouSignals) = 0;

    virtual ERRORCODE GetProperties(EcuProperties&) = 0;
    virtual ERRORCODE SetProperties(EcuProperties&) = 0;
};


class IFrame:  public IElement
{
protected:
    IFrame(ICluster* pBaseCluster):IElement(pBaseCluster) {};
    virtual ~IFrame() {};

public:
    virtual ERRORCODE GetFrameType(eFrameType&) = 0;
    virtual ERRORCODE GetFrameId(unsigned int& unFrameId) = 0;
    virtual ERRORCODE GetLength(unsigned int& unFrameLength) = 0;

    virtual ERRORCODE GetEcus( eDIR eDirection, std::list<IEcu*>& ) = 0;

    virtual ERRORCODE MapSignal( UID_ELEMENT& nId, SignalInstanse& ouProps) = 0;
    virtual ERRORCODE UnMapSignal(UID_ELEMENT&) = 0;

    virtual ERRORCODE MapPdu( UID_ELEMENT& nId, PduInstanse& ouProps) = 0;
    virtual ERRORCODE UnMapPdu(UID_ELEMENT&) = 0;
    virtual ERRORCODE MapNode( eDIR eDirection, UID_ELEMENT&) = 0;
    virtual ERRORCODE UnMapNode( eDIR eDirection, UID_ELEMENT&) = 0;

    virtual ERRORCODE SetProperties(FrameProps&) = 0;
    virtual ERRORCODE GetProperties(FrameProps&) = 0;

    virtual ERRORCODE GetSignalList(std::map<ISignal*, SignalInstanse>& mapSignals) = 0;
    virtual unsigned int GetSignalCount() = 0;
    virtual ERRORCODE GetPduList(std::map<IPdu*, PduInstanse>& mapPdus) = 0;
    virtual ERRORCODE GetUpdatedPdus(unsigned char*, std::map<IPdu*, PduInstanse>&) = 0;

    virtual ERRORCODE InterpretSignals( unsigned char*, int nSize, std::list<InterpreteSignals>& ouSignalInfoList, bool bIsHex, bool formatHex = false) = 0;
};

class IPdu : public IElement
{
protected:
    IPdu(ICluster* pBaseCluster):IElement(pBaseCluster) {};
    virtual ~IPdu() {};
public:
    virtual ERRORCODE GetSignalList(std::map<ISignal*, SignalInstanse>& mapSignals) = 0;
    virtual unsigned int GetSignalCount() = 0;
    virtual ERRORCODE GetProperties(PduProps&) = 0;
    virtual ERRORCODE SetProperties(PduProps&) = 0;
    virtual ERRORCODE MapSignal( UID_ELEMENT& nId, SignalInstanse& ouProps) = 0;
    virtual ERRORCODE UnMapSignal(UID_ELEMENT&) = 0;

    virtual ERRORCODE InterpretSignals(int nPduStartBit, unsigned char*, int nSize, std::list<InterpreteSignals>& ouSignalInfoList, bool bIsHex, bool formatHex = false) = 0;

    virtual ERRORCODE MapFrame( UID_ELEMENT& nId, PduInstanse& ) = 0;
    virtual ERRORCODE UnMapFrame(UID_ELEMENT&) = 0;

    virtual ERRORCODE GetMappedFrames(std::map<IFrame*, PduInstanse>& lstFrames) = 0;

};

class ISignal : public IElement
{
protected:
    ISignal(ICluster* pBaseCluster):IElement(pBaseCluster) {};
    virtual ~ISignal() {};
public:

    virtual ERRORCODE GetEcus(eDIR eDirection, std::list<IEcu*>& ouNodes) = 0;
    virtual ERRORCODE GetLength(unsigned int& unSignalLength) = 0;
    virtual ERRORCODE GetMinMaxValue(uint64_t& unMinValue, uint64_t& unMaxValue) = 0;
    virtual ERRORCODE MapNode( eDIR eDirection, UID_ELEMENT&) = 0;
    virtual ERRORCODE UnMapNode( eDIR eDirection, UID_ELEMENT&) = 0;

    virtual ERRORCODE GetEncoding(ICoding**) = 0;
    virtual ERRORCODE SetEncoding(UID_ELEMENT&) = 0;

    virtual ERRORCODE GetProperties(SignalProps&) = 0;
    virtual ERRORCODE SetProperties(SignalProps&) = 0;

    virtual ERRORCODE GetDataType(eSignalDataType&) = 0;
    virtual ERRORCODE GetUnit(std::string&) = 0;

    virtual ERRORCODE GetRawValue(int nStartBit, int nSignalLength, int nByteLength, bool bIntel, unsigned char* pchData, uint64_t& unRawValue) = 0;
    virtual ERRORCODE GetEnggValueFromRaw(uint64_t  dwRawValue, double& dEnggValue) = 0;
    virtual ERRORCODE GetRawValueFromEng(double dEnggValue, uint64_t&  dwRawValue) = 0;
};


class ICoding : public IElement
{
protected:
    ICoding(ICluster* pBaseCluster):IElement(pBaseCluster) {};
    virtual ~ICoding() {};

public:


    virtual ERRORCODE GetProperties(CompuMethodProps&) = 0;
    virtual ERRORCODE SetProperties(CompuMethodProps&) = 0;


    virtual ERRORCODE GetValueDescriptions(std::map<int, std::string>&) = 0;
    virtual ERRORCODE SetValueDescriptions(std::map<int, std::string>&) = 0;

    virtual ERRORCODE GetValueDescription(int, std::string&) = 0;
    virtual ERRORCODE SetValueDescription(int, std::string&) = 0;

    virtual ERRORCODE GetSignalList(std::map<UID_ELEMENT, ISignal*>& lstSignals) = 0;
    virtual ERRORCODE MapSignal(UID_ELEMENT) = 0;
    virtual ERRORCODE UnMapSignal(UID_ELEMENT) = 0;

    virtual ERRORCODE GetEngValue(eSignalDataType eDataType, int  nSiglength, uint64_t unRawValue, double& dEngValue) = 0;
    virtual ERRORCODE GetRawValue(double dEngValue, uint64_t& unRawValue) = 0;
};

class ISignalGroup : public IElement
{
protected:
    ISignalGroup(ICluster* pBaseCluster):IElement(pBaseCluster) {};
    virtual ~ISignalGroup() {};

public:
    virtual ERRORCODE MapSignal( UID_ELEMENT& nId, SignalInstanse& ouProps) = 0;
    virtual ERRORCODE UnMapSignal(UID_ELEMENT&) = 0;

    virtual ERRORCODE GetProperties(SignalGroupProps&) = 0;
    virtual ERRORCODE SetProperties(SignalGroupProps&) = 0;

    virtual ERRORCODE GetSignalList(std::map<ISignal*, SignalInstanse>& mapSignals) = 0;
};





class IScheduleTable: public IElement
{
protected:
    IScheduleTable(ICluster* pBaseCluster):IElement(pBaseCluster) {};
    virtual ~IScheduleTable() {};
public:
    virtual ERRORCODE GetProperties(ScheduleTableProps&) = 0;
    virtual ERRORCODE SetProperties(ScheduleTableProps&) = 0;
};

class CLINSheduleTable : public IScheduleTable, public INotifyClusterChange
{
    ScheduleTableProps m_ouSheduleTableProps;
    friend class CLINSheduleTableContainer;
protected:
    CLINSheduleTable(ICluster*);
public:


    ERRORCODE GetProperties(ScheduleTableProps&);
    ERRORCODE SetProperties(ScheduleTableProps&);
    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);

    ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification);
private:
    ERRORCODE HandleFrameChanges(UID_ELEMENT nId, eAction eActionType);
    ERRORCODE HandleEcuChanges(UID_ELEMENT nId, eAction eActionType);
};




class INodeConfiguration : public IElement
{
protected:
    INodeConfiguration(ICluster* pBaseCluster):IElement(pBaseCluster) {};
    virtual ~INodeConfiguration() {};
public:
    virtual ERRORCODE GetCompositeNodes(std::list<CompositeNode>& ouCompositeNodeList) = 0;
    virtual ERRORCODE SetCompositeNodes(std::list<CompositeNode>& ouCompositeNodeList) = 0;
};




class ClusterResult
{
public:
    ICluster* m_pCluster;
    std::list<ParsingResults> m_pErrors;
    std::list<ParsingResults> m_pWarnings;
    bool operator==(const ClusterResult& obj)
    {
        return ( m_pCluster == obj.m_pCluster );
    }

    bool operator > (const ClusterResult& obj)
    {
        return ( m_pCluster > obj.m_pCluster );
    }

    bool operator < (const ClusterResult& obj)
    {
        return ( m_pCluster < obj.m_pCluster );
    }
};

#ifdef _WIN32
extern "C" __declspec(dllexport) ERRORCODE ParseDBFile( std::string& strFileName, ETYPE_BUS clusterType, std::list<ClusterResult>& ouClusterResultList );
extern "C" __declspec(dllexport) ERRORCODE FreeCluster( ICluster* pCluster );
extern "C" __declspec(dllexport) ERRORCODE CreateLDFCluster(ICluster** pouLdfCluster);
// TODO figure out whether it is needed on Unixes!
#endif
