#pragma once




#include "IClusterProps.h"



//Forword Decleration;
class ICluster;
class IElement;
class IEcu;
class IFrame;
class IPdu;             //When Flexray Comes
class ISignal;
class ICoding;
class IScheduleTable;

extern "C" EXPORTS ERRORCODE GetCluster(eClusterType oueClusterType, ICluster** pLDFCluster);

class INotifyClusterChange
{
public:
    virtual ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData) = 0;
};


class EXPORTS IElement
{
protected:
    eClusterElementType m_oueElementType;
    std::string m_strName;
    ICluster* m_pBaseCluster;
    UID_ELEMENT m_nUnId;



    IElement(ICluster* pCluster);

public:
    virtual ERRORCODE GetName(std::string& strName);

    virtual ERRORCODE SetName(std::string& strName);

    virtual ERRORCODE GetUniqueId(UID_ELEMENT& unId);

    virtual UID_ELEMENT GetUniqueId();

    virtual eClusterElementType GetElementType();

    virtual ERRORCODE SetElementType(eClusterElementType& ouElementType);

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
    virtual ERRORCODE GetFrame(UID_ELEMENT& nId, IFrame** ouFrame) = 0;

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

    virtual ERRORCODE GetEcus( eDIR eDirection, std::list<IEcu*>& ) = 0;

    virtual ERRORCODE MapSignal( UID_ELEMENT& nId, SignalInstanse& ouProps) = 0;
    virtual ERRORCODE UnMapSignal(UID_ELEMENT&) = 0;

    virtual ERRORCODE MapNode( eDIR eDirection, UID_ELEMENT&) = 0;
    virtual ERRORCODE UnMapNode( eDIR eDirection, UID_ELEMENT&) = 0;

    virtual ERRORCODE SetProperties(FrameProps&) = 0;
    virtual ERRORCODE GetProperties(FrameProps&) = 0;

    virtual ERRORCODE GetSignalList(std::map<ISignal*, SignalInstanse>& mapSignals) = 0;
};

class IPdu
{

};

class ISignal : public IElement
{
protected:
    ISignal(ICluster* pBaseCluster):IElement(pBaseCluster) {};
    virtual ~ISignal() {};
public:

    virtual ERRORCODE GetEcus(eDIR eDirection, std::list<IEcu*>& ouNodes) = 0;

    virtual ERRORCODE MapNode( eDIR eDirection, UID_ELEMENT&) = 0;
    virtual ERRORCODE UnMapNode( eDIR eDirection, UID_ELEMENT&) = 0;

    virtual ERRORCODE GetEncoding(ICoding**) = 0;
    virtual ERRORCODE SetEncoding(UID_ELEMENT&) = 0;

    virtual ERRORCODE GetProperties(SignalProps&) = 0;
    virtual ERRORCODE SetProperties(SignalProps&) = 0;
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

class ScheduleTableProps
{
public:
    //Name Only for Now
    std::string m_strTableName;
    std::list<CSheduleTableItem> m_ouCSheduleTableItem;
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

class CSheduleTable : public IScheduleTable, public INotifyClusterChange
{
    ScheduleTableProps m_ouSheduleTableProps;
    friend class CLINSheduleTableContainer;
protected:
    CSheduleTable(ICluster*);
public:


    ERRORCODE GetProperties(ScheduleTableProps&);
    ERRORCODE SetProperties(ScheduleTableProps&);
    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);

    ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification);
private:
    ERRORCODE HandleFrameChanges(UID_ELEMENT nId, eAction eActionType);
    ERRORCODE HandleEcuChanges(UID_ELEMENT nId, eAction eActionType);
};


class CompositeNode
{
public:
    UID_ELEMENT m_uidNode;
    std::list<std::string> m_strLogicalNodes;
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


class ICluster
{
public:
    ICluster(void) {};
    virtual ~ICluster(void) {};

    virtual ERRORCODE GetNextUniqueId(UID_ELEMENT&) = 0;
    //Editor
    virtual ERRORCODE LoadFromFile(std::string& strFileName, std::list<ParsingResults>& ouErrors, std::list<ParsingResults>& ouWarnings) = 0;
    virtual ERRORCODE SaveToFile(std::string& strFileName) = 0;
    virtual ERRORCODE ValidateCluster(std::list<std::string>&) = 0;
    virtual ERRORCODE Clear() = 0;

    virtual ERRORCODE CreateElement(eClusterElementType eType, IElement** pElement) = 0;
    virtual ERRORCODE DeleteElement(eClusterElementType eType, UID_ELEMENT& nUniqueId) = 0;

    virtual ERRORCODE GetProperties(ePropertyType ouePropertyType, void* pProperties) = 0;
    virtual ERRORCODE SetProperties(ePropertyType ouePropertyType, void* pProperties) = 0;

    virtual ERRORCODE GetElement(eClusterElementType eType, UID_ELEMENT& nId, IElement** pElement ) = 0;
    virtual ERRORCODE GetElementList(eClusterElementType eType, std::map<UID_ELEMENT, IElement*>& pElement ) = 0;

    virtual ERRORCODE GetClusterType(eClusterType&) = 0;

    virtual ERRORCODE EnableEventNotofications(bool bEnable = true) = 0;
    virtual bool isNotoficationsEnabled() = 0;
    //Interpretation

    virtual ERRORCODE GetEcu(std::string& strEcuName, IEcu** pEcu) = 0;
    virtual ERRORCODE GetFrame(unsigned int& unId, IFrame** pFrame) = 0;

    virtual eClusterElementType GetElementType(UID_ELEMENT& ouElementUid) = 0;

    //uidOnElement INVALID_ Means notify for all elements
    virtual ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification, UID_ELEMENT uidOnElement = INVALID_UID_ELEMENT) = 0;
    virtual ERRORCODE NotifyClusterChange(eAction oueAction, UID_ELEMENT& m_nUnId, eClusterElementType& oueElementType, void* pAcionData) = 0;

};
