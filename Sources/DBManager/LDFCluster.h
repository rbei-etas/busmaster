#pragma once
#include <iostream>
#include <list>
#include <map>
#include "ICluster.h"

class CLINEcu : public IEcu, public INotifyClusterChange
{
    std::list<UID_ELEMENT> m_nFrames[DIR_ALL];      //Int Id
    std::map<UID_ELEMENT, UID_ELEMENT> m_nSignals[DIR_ALL];     //Int Id

    EcuProperties m_eEcuProps;
    friend class EcuContainer;
private:
    ERRORCODE nInitialiseEcuProperties();
protected:
    CLINEcu(ICluster*);
    ~CLINEcu();

public:
    //Editor

    ERRORCODE GetEcuType(eEcuType&);
    ERRORCODE SetEcuType(eEcuType&);

    ERRORCODE MapFrame(eDIR eDir, UID_ELEMENT& nId) ;
    ERRORCODE UnMapFrame(eDIR eDir, UID_ELEMENT& nId) ;

    ERRORCODE MapSignal(eDIR eDir, UID_ELEMENT& nId);
    ERRORCODE UnMapSignal(eDIR eDir, UID_ELEMENT& nId);

    //Interpretation
    ERRORCODE GetFrameList(eDIR eDir, std::list<IFrame*>& ouFrames);
    ERRORCODE GetFrame(UID_ELEMENT& nId, IFrame** ouFrame);

    ERRORCODE GetSignalList(eDIR eDir, std::map<ISignal*, ISignal*>& ouSignals);

    ERRORCODE GetProperties( EcuProperties&);
    ERRORCODE SetProperties( EcuProperties&);

    //Notification
    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);
    ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification)
    {
        if ( nullptr != m_pBaseCluster )
        {
            return m_pBaseCluster->RegisterForChangeNotification(ouNotification, m_nUnId);
        }
        return EC_FAILURE;
    }
private:
    ERRORCODE OnElementDeleted(UID_ELEMENT nId, eClusterElementType ouElementType);
};

class CLINFrame: public IFrame, public INotifyClusterChange
{
    friend class FrameContainer;
    FrameProps m_ouProps;
    std::list<UID_ELEMENT> m_ouTxNodes;
    std::list<UID_ELEMENT> m_ouRxNodes;
    std::map<UID_ELEMENT, SignalInstanse> m_SignalMap;
private:
    ERRORCODE nInitialiseFrameProperties();
protected:
    CLINFrame(ICluster* pCluster);
    ~CLINFrame();

public:


    ERRORCODE GetFrameType(eFrameType&);
    ERRORCODE GetFrameId(unsigned int& unFrameId);

    ERRORCODE GetEcus( eDIR eDirection, std::list<IEcu*>& );

    ERRORCODE MapSignal( UID_ELEMENT& nId, SignalInstanse& ouProps);
    ERRORCODE UnMapSignal(UID_ELEMENT&);

    ERRORCODE MapNode( eDIR eDirection, UID_ELEMENT&);
    ERRORCODE UnMapNode( eDIR eDirection, UID_ELEMENT&);

    ERRORCODE SetProperties(FrameProps&);
    ERRORCODE GetProperties(FrameProps&);

    ERRORCODE GetSignalList(std::map<ISignal*, SignalInstanse>& mapSignals);
    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);

    ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification)
    {
        if ( nullptr != m_pBaseCluster )
        {
            return m_pBaseCluster->RegisterForChangeNotification(ouNotification, m_nUnId);
        }
        return EC_FAILURE;
    }
private:
    ERRORCODE HandleSignalChanges(eAction eActionType, UID_ELEMENT nId, void* pAcionData);
    ERRORCODE HandleFrameChanges(eAction eActionType, UID_ELEMENT nId, void* pAcionData);
    ERRORCODE HandleRxEcuUnmap(UID_ELEMENT ouRxEcu);

};

class CLINSignal:public ISignal, public INotifyClusterChange
{
    std::list<UID_ELEMENT> m_ouTxNodes;
    std::list<UID_ELEMENT> m_ouRxNodes;
    SignalProps m_ouSignalProps;
    UID_ELEMENT m_nCodingId;
    friend class SignalContainer;
protected:
    CLINSignal(ICluster*);
    ~CLINSignal();

public:

    ERRORCODE GetEcus(eDIR eDirection, std::list<IEcu*>& ouNodes);

    ERRORCODE MapNode( eDIR eDirection, UID_ELEMENT&);
    ERRORCODE UnMapNode( eDIR eDirection, UID_ELEMENT&);

    ERRORCODE GetEncoding(ICoding**);
    ERRORCODE SetEncoding(UID_ELEMENT&);

    ERRORCODE GetProperties(SignalProps&);
    ERRORCODE SetProperties(SignalProps&);

    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);

    ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification)
    {
        if ( nullptr != m_pBaseCluster )
        {
            return m_pBaseCluster->RegisterForChangeNotification(ouNotification, m_nUnId);
        }
        return EC_FAILURE;
    }

private:
    ERRORCODE HandleCodingChanges(UID_ELEMENT nId, eAction eActionType);
    ERRORCODE HandleEcuChanges(UID_ELEMENT nId, eAction eActionType);

};

class CLINSignalGroup : public ISignalGroup, public INotifyClusterChange
{
    std::map<UID_ELEMENT, SignalInstanse> m_ouSignalList;
    friend class CLINSignalGroupContainer;
    SignalGroupProps m_ouSignalGroupProps;

protected:
    CLINSignalGroup(ICluster* pBaseCluster);
    virtual ~CLINSignalGroup();
public:

    ERRORCODE MapSignal( UID_ELEMENT& nId, SignalInstanse& ouProps);
    ERRORCODE UnMapSignal(UID_ELEMENT&);

    ERRORCODE GetProperties(SignalGroupProps&);
    ERRORCODE SetProperties(SignalGroupProps&);

    ERRORCODE GetSignalList(std::map<ISignal*, SignalInstanse>& mapSignals);

    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);

    eClusterElementType GetElementType();

    ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification)
    {
        if ( nullptr != m_pBaseCluster )
        {
            return m_pBaseCluster->RegisterForChangeNotification(ouNotification, m_nUnId);
        }
        return EC_FAILURE;
    }


};

class CLINCoding : public ICoding, public INotifyClusterChange
{
    CompuMethodProps m_ouCompuMethodProps;
    std::map<UID_ELEMENT, UID_ELEMENT> m_ouMapSignal;
    friend class CodingContainer;
protected:
    CLINCoding(ICluster* pCluster);
public:


    ERRORCODE GetProperties(CompuMethodProps& );
    ERRORCODE SetProperties(CompuMethodProps& );


    ERRORCODE GetValueDescriptions(std::map<int, std::string>&);
    ERRORCODE SetValueDescriptions(std::map<int, std::string>&);

    ERRORCODE GetValueDescription(int, std::string&);
    ERRORCODE SetValueDescription(int, std::string&);

    ERRORCODE GetSignalList(std::map<UID_ELEMENT, ISignal*>& lstSignals);
    ERRORCODE MapSignal(UID_ELEMENT);
    ERRORCODE UnMapSignal(UID_ELEMENT);

    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);

    ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification)
    {
        if ( nullptr != m_pBaseCluster )
        {
            return m_pBaseCluster->RegisterForChangeNotification(ouNotification, m_nUnId);
        }
        return EC_FAILURE;
    }



};



class CLDFNodeConfiguration : public INodeConfiguration, public INotifyClusterChange
{
    std::list<CompositeNode> m_ouCompositeNodeList;
protected:
    CLDFNodeConfiguration(ICluster* pBaseCluster);

    friend class CLDFNodeConfigContainer;
public:
    ERRORCODE GetCompositeNodes(std::list<CompositeNode>& ouCompositeNodeList)
    {
        ouCompositeNodeList = m_ouCompositeNodeList;
        return EC_SUCCESS;
    }

    ERRORCODE SetCompositeNodes(std::list<CompositeNode>& ouCompositeNodeList)
    {
        m_ouCompositeNodeList = ouCompositeNodeList;
        if ( nullptr != m_pBaseCluster )
        {
for ( auto itr : m_ouCompositeNodeList )
            {
                m_pBaseCluster->RegisterForChangeNotification(this, itr.m_uidNode);
            }
        }

        return EC_SUCCESS;
    }

    ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification)
    {
        if ( nullptr != m_pBaseCluster )
        {
            return m_pBaseCluster->RegisterForChangeNotification(ouNotification, m_nUnId);
        }
        return EC_FAILURE;
    }
    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
    {
        switch (eActionType)
        {
            case eDeleted:
                if ( ouElementType == eEcuElement )
                {
                    std::list<CompositeNode>::iterator itr = m_ouCompositeNodeList.begin();
                    while ( itr != m_ouCompositeNodeList.end() )
                    {
                        if ( itr->m_uidNode == m_nUnId )
                        {
                            itr = m_ouCompositeNodeList.erase(itr);
                            continue;
                        }
                        itr++;
                    }

                }
                break;
            default:
                break;
        }
        return 0;
    }
};


class ElementContainer : public INotifyClusterChange
{
protected:
    ICluster* m_pBaseContainer;
public:
    ElementContainer(ICluster* pBaseContainer);


    virtual ERRORCODE CreateElement(IElement** pElement)
    {
        return EC_NA;
    }
    virtual ERRORCODE DeleteElement(UID_ELEMENT& nUniqueId)
    {
        return EC_NA;
    }
    virtual ERRORCODE Clear()
    {
        return EC_NA;
    }
    virtual ERRORCODE GetElement(UID_ELEMENT& nId, IElement** pElement)
    {
        return EC_NA;
    }
    virtual ERRORCODE GetElementList( std::map<UID_ELEMENT, IElement*>& pElement )
    {
        return EC_NA;
    }
    virtual ERRORCODE GetElementByName(std::string& strEcuName, IElement** pEcu);
    virtual ERRORCODE GetElementByID(UID_ELEMENT& unId, IElement** pEcu);
    virtual ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
    {
        return EC_NA;
    };
};

class CLDFNodeConfigContainer: public ElementContainer
{
    std::map<UID_ELEMENT, INodeConfiguration*> m_ouNodeConfigMap;
    ICluster* m_oupBaseContainer;
public:
    CLDFNodeConfigContainer(ICluster* pBaseContainer);
    ERRORCODE CreateElement(IElement** pElement);
    ERRORCODE DeleteElement(UID_ELEMENT& nUniqueId);
    ERRORCODE GetElement(UID_ELEMENT& nId, IElement** pElement);
    ERRORCODE GetElementList( std::map<UID_ELEMENT, IElement*>& pElement );
    ERRORCODE GetElementByName(std::string& strEcuName, IElement** pEcu) ;
    ERRORCODE GetElementByID(UID_ELEMENT& unId, IElement** pEcu) ;
    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);
    ERRORCODE Clear();
};


class EcuContainer: public ElementContainer
{
    std::map<UID_ELEMENT, IEcu*> m_ouEcuMap;
    std::map<std::string, UID_ELEMENT> m_ouNameToId;
    ICluster* m_oupBaseContainer;
public:
    EcuContainer(ICluster* pBaseContainer);
    ERRORCODE CreateElement(IElement** pElement);
    ERRORCODE DeleteElement(UID_ELEMENT& nUniqueId);
    ERRORCODE GetElement(UID_ELEMENT& nId, IElement** pElement);
    ERRORCODE GetElementList( std::map<UID_ELEMENT, IElement*>& pElement );
    ERRORCODE GetElementByName(std::string& strEcuName, IElement** pEcu) ;
    ERRORCODE GetElementByID(UID_ELEMENT& unId, IElement** pEcu) ;
    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);
    ERRORCODE Clear();
};

class FrameContainer: public ElementContainer
{
    std::map<UID_ELEMENT, IFrame*> m_ouFrameMap;
public:
    FrameContainer(ICluster*);
    ERRORCODE CreateElement(IElement** pElement);
    ERRORCODE DeleteElement(UID_ELEMENT& nUniqueId);
    ERRORCODE GetElement(UID_ELEMENT& nId, IElement** pElement);
    ERRORCODE GetElementList( std::map<UID_ELEMENT, IElement*>& pElement );
    ERRORCODE GetElementByName(std::string& strEcuName, IElement** pEcu) ;
    ERRORCODE GetElementByID(UID_ELEMENT& unId, IElement** pEcu) ;
    ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData);
    ERRORCODE Clear();
};

class SignalContainer: public ElementContainer
{
    std::map<UID_ELEMENT, ISignal*> m_ouSignalMap;
public:
    SignalContainer(ICluster*);
    ERRORCODE CreateElement(IElement** pElement);
    ERRORCODE DeleteElement(UID_ELEMENT& nUniqueId);
    ERRORCODE GetElement(UID_ELEMENT& nId, IElement** pElement);
    ERRORCODE GetElementList( std::map<UID_ELEMENT, IElement*>& pElement );
    ERRORCODE GetElementByName(std::string& strEcuName, IElement** pEcu) ;
    ERRORCODE GetElementByID(UID_ELEMENT& unId, IElement** pEcu) ;
    ERRORCODE Clear();
};

class CLINSignalGroupContainer : public ElementContainer
{
    std::map<UID_ELEMENT, ISignalGroup*> m_ouSignalGroupMap;
public:
    CLINSignalGroupContainer(ICluster*);
    ERRORCODE CreateElement(IElement** pElement);
    ERRORCODE DeleteElement(UID_ELEMENT& nUniqueId);
    ERRORCODE GetElement(UID_ELEMENT& nId, IElement** pElement);
    ERRORCODE GetElementList( std::map<UID_ELEMENT, IElement*>& pElement );
    ERRORCODE Clear();
    //ERRORCODE GetElementByName(std::string& strEcuName, IElement** pEcu) ;
    //ERRORCODE GetElementByID(UID_ELEMENT& unId, IElement** pEcu) ;
};

class CodingContainer: public ElementContainer
{
    std::map<UID_ELEMENT, ICoding*> m_ouCodingMap;
public:
    CodingContainer(ICluster*);
    ERRORCODE CreateElement(IElement** pElement);
    ERRORCODE DeleteElement(UID_ELEMENT& nUniqueId);
    ERRORCODE GetElement(UID_ELEMENT& nId, IElement** pElement);
    ERRORCODE GetElementList( std::map<UID_ELEMENT, IElement*>& pElement );
    ERRORCODE GetElementByName(std::string& strEcuName, IElement** pEcu) ;
    ERRORCODE GetElementByID(UID_ELEMENT& unId, IElement** pEcu) ;
    ERRORCODE Clear();
};

class CLINSheduleTableContainer : public ElementContainer
{
    std::map<UID_ELEMENT, IScheduleTable*> m_ouScheduleTableMap;
public:
    CLINSheduleTableContainer(ICluster*);
    ERRORCODE CreateElement(IElement** pElement) ;
    ERRORCODE DeleteElement(UID_ELEMENT& nUniqueId);
    ERRORCODE GetElement(UID_ELEMENT& nId, IElement** pElement);
    ERRORCODE GetElementList( std::map<UID_ELEMENT, IElement*>& pElement );
    ERRORCODE GetElementByName(std::string& strEcuName, IElement** pEcu) ;
    ERRORCODE GetElementByID(UID_ELEMENT& unId, IElement** pEcu);
    ERRORCODE Clear();
};

class LDFCluster: public ICluster
{
private:
    std::map<UID_ELEMENT, std::list<INotifyClusterChange*>> m_ouNotifications;
    UID_ELEMENT m_nID;
    bool m_bEnableNotifications;
    std::map<UID_ELEMENT, eClusterElementType> m_ouElermentTypeMap;
public:
    NodeCompositionConfigurationList m_ouNodeCompositionConfigurationList;
    std::list<unsigned int> m_ouDyanamicFramelist;
public:
    virtual ERRORCODE GetNextUniqueId(UID_ELEMENT& unId)
    {
        unId = ++m_nID;
        return EC_SUCCESS;
    }


    LDFCluster()
    {
        m_nID = INVALID_UID_ELEMENT;
        memset(m_pEntiyContainers, 0, sizeof(m_pEntiyContainers));

        m_pEntiyContainers[eFrameElement] = new FrameContainer(this);
        m_pEntiyContainers[eSignalElement] = new SignalContainer(this);
        m_pEntiyContainers[eEcuElement] = new EcuContainer(this);
        m_pEntiyContainers[eCodingElement] = new CodingContainer(this);
        m_pEntiyContainers[eScheduleTableElement] = new CLINSheduleTableContainer(this);
        m_pEntiyContainers[eSignalGroupElement] = new CLINSignalGroupContainer(this);
        m_pEntiyContainers[eLinNodeComposition] = new CLDFNodeConfigContainer(this);
        m_bEnableNotifications = false;
    }

private:


    ElementContainer* m_pEntiyContainers[eAllElement];
    LIN_Settings m_ouLinSettings;
private:


public:
    UID_ELEMENT GetNextUniqueId()
    {
        return m_nID++;
    }
    //Editor
    ERRORCODE LoadFromFile(std::string& strFileName, std::list<ParsingResults>& ouErrors, std::list<ParsingResults>& ouWarnings);
    ERRORCODE SaveToFile(std::string& strFileName);
    ERRORCODE ValidateCluster(std::list<std::string>&);
    ERRORCODE Clear();

    ERRORCODE CreateElement(eClusterElementType eType, IElement** pElement);
    ERRORCODE DeleteElement(eClusterElementType eType, UID_ELEMENT& nUniqueId);

    ERRORCODE GetProperties(ePropertyType ouePropertyType, void* pProperties);
    ERRORCODE SetProperties(ePropertyType ouePropertyType, void* pProperties);

    ERRORCODE GetElement(eClusterElementType eType, UID_ELEMENT& nId, IElement** pElement );
    ERRORCODE GetElementList(eClusterElementType eType, std::map<UID_ELEMENT, IElement*>& pElement );

    ERRORCODE GetClusterType(eClusterType&);
    ERRORCODE EnableEventNotofications(bool bEnable = true)
    {
        m_bEnableNotifications = bEnable;
        return EC_SUCCESS;
    };
    bool isNotoficationsEnabled()
    {
        return m_bEnableNotifications;
    };

    //Interpretation
    ERRORCODE GetEcu(std::string& strEcuName, IEcu** pEcu);
    ERRORCODE GetFrame(unsigned int& unId, IFrame** pFrame);

    virtual eClusterElementType GetElementType(UID_ELEMENT& ouElementUid)
    {
        std::map<UID_ELEMENT, eClusterElementType>::iterator itr = m_ouElermentTypeMap.find(ouElementUid);
        if ( m_ouElermentTypeMap.end() != itr )
        {
            return itr->second;
        }

        //TODO: iMPLIMENTATION
        return eInvalidElement;
    };
    ERRORCODE RegisterForChangeNotification( INotifyClusterChange* ouNotification, UID_ELEMENT uidOnElement = INVALID_UID_ELEMENT)
    {
        m_ouNotifications[uidOnElement].push_back(ouNotification);
        return EC_SUCCESS;
    }

    ERRORCODE NotifyClusterChange(eAction oueAction, UID_ELEMENT& m_nUnId, eClusterElementType& oueElementType, void* pAcionData)
    {
        if (true == isNotoficationsEnabled())
        {
            auto itr = m_ouNotifications.find(m_nUnId);
            if ( itr != m_ouNotifications.end() )
            {
for ( auto itrClientList : itr->second)
                {
                    itrClientList->OnClusterChange(oueAction, m_nUnId, oueElementType, pAcionData, nullptr);
                }
            }
            auto itr2 = m_ouNotifications.find(INVALID_UID_ELEMENT);
            if ( itr2 != m_ouNotifications.end() )
            {
for ( auto itrClientList : itr2->second)
                {
                    itrClientList->OnClusterChange(oueAction, m_nUnId, oueElementType, pAcionData, nullptr);
                }
            }
        }
        return EC_NA;
    }


};
