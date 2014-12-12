#include "stdafx.h"
#include "LdfManager.h"
#include "LDFCluster.h"
#include "LDFTagDefines.h"
#include "LDFGenerator1_3.h"
#include "LDFGenerator2_0.h"
#include "LDFGenerator2_1.h"



#include "ICluster.h"

#define VALIDATE_LIN_ELEMENTTYPE(x) if ( x < eFrameElement || x >= eAllElement) return EC_FAILURE;



ERRORCODE GetCluster(eClusterType oueClusterType, ICluster** pLDFCluster)
{
    ERRORCODE ecError  = EC_FAILURE;
    switch(oueClusterType)
    {
        case LIN:
        {
            LDFCluster* pouLdfCluster = new LDFCluster;
            *pLDFCluster = pouLdfCluster;
            ecError = EC_SUCCESS;
            break;
        }
        default:
            break;
    }
    return ecError;
}



ERRORCODE LDFCluster::LoadFromFile(std::string& strFileName, std::list<ParsingResults>& ouErrors, std::list<ParsingResults>& ouWarnings )
{
    EnableEventNotofications(false);
    Clear();
    ILdfManager ouLdfManager;
    return ouLdfManager.nLoadLDFFromFile(this, strFileName, ouErrors, ouWarnings);
}

ERRORCODE LDFCluster::SaveToFile(std::string& strFileName)
{
    ILdfManager ouLdfManager;
    return ouLdfManager.nSaveLDFToFile(this, strFileName);
}
ERRORCODE LDFCluster::ValidateCluster(std::list<std::string>&)
{

    return EC_NA;
}

ERRORCODE LDFCluster::Clear()
{
    for(int nElementIndex = eFrameElement; nElementIndex < eAllElement; nElementIndex++)
    {
        if(nullptr != m_pEntiyContainers[nElementIndex])
        {
            m_pEntiyContainers[nElementIndex]->Clear();
        }
    }
    m_ouNotifications.clear();
    m_ouElermentTypeMap.clear();
    m_ouDyanamicFramelist.clear();
    m_nID = INVALID_UID_ELEMENT;
    m_ouNodeCompositionConfigurationList.m_ouNodeConfiguration.clear();
    m_ouLinSettings.clear();

    return EC_SUCCESS;
}

ERRORCODE LDFCluster::CreateElement(eClusterElementType eType, IElement** pElement)
{
    VALIDATE_LIN_ELEMENTTYPE(eType);
    ERRORCODE ec = m_pEntiyContainers[eType]->CreateElement(pElement);
    if ( EC_SUCCESS == ec )
    {
        m_ouElermentTypeMap[(*pElement)->GetUniqueId()] = eType;
    }

}

ERRORCODE LDFCluster::DeleteElement(eClusterElementType eType, UID_ELEMENT& nUniqueId)
{
    VALIDATE_LIN_ELEMENTTYPE(eType);
    return m_pEntiyContainers[eType]->DeleteElement(nUniqueId);
}

ERRORCODE GetProperties(ePropertyType ouePropertyType, void* pProperties);
ERRORCODE SetProperties(ePropertyType ouePropertyType, void* pProperties);


ERRORCODE LDFCluster::GetProperties(ePropertyType ouePropertyType, void* pProperties)
{
    if( nullptr == pProperties )
    {
        return EC_FAILURE;
    }
    switch ( ouePropertyType )
    {
        case eLINClusterProperties:
            *((LIN_Settings*)pProperties) = m_ouLinSettings;
            break;

        case eLdfNodeComposition:
            *((NodeCompositionConfigurationList*)pProperties) = m_ouNodeCompositionConfigurationList;
            break;

        case eLdfDyanmicFrameList:
            *((std::list<unsigned int>*)pProperties) = m_ouDyanamicFramelist;
            break;

        default:
            return EC_NA;
    }
    return EC_SUCCESS;
}
ERRORCODE LDFCluster::SetProperties(ePropertyType ouePropertyType, void* pProperties)
{
    if( nullptr == pProperties )
    {
        return EC_FAILURE;
    }
    switch ( ouePropertyType )
    {
        case eLINClusterProperties:
            m_ouLinSettings = *((LIN_Settings*)pProperties);
            break;

        case eLdfNodeComposition:
            m_ouNodeCompositionConfigurationList = *((NodeCompositionConfigurationList*)pProperties);
            break;

        case eLdfDyanmicFrameList:
            m_ouDyanamicFramelist = *((std::list<unsigned int>*)pProperties);
            break;

        default:
            return EC_NA;
    }

    return EC_SUCCESS;
}


ERRORCODE LDFCluster::GetElement(eClusterElementType eType, UID_ELEMENT& nUniqueId, IElement** pElement )
{
    VALIDATE_LIN_ELEMENTTYPE(eType);
    return m_pEntiyContainers[eType]->GetElement(nUniqueId, pElement);
}

ERRORCODE LDFCluster::GetElementList(eClusterElementType eType, std::map<UID_ELEMENT, IElement*>& pElementMap )
{
    VALIDATE_LIN_ELEMENTTYPE(eType);
    return m_pEntiyContainers[eType]->GetElementList(pElementMap);
}


ERRORCODE LDFCluster::GetClusterType(eClusterType&)
{
    return LIN;
}


//Interpretation
ERRORCODE LDFCluster::GetEcu(std::string& strEcuName, IEcu** pEcu)
{
    return m_pEntiyContainers[eEcuElement]->GetElementByName(strEcuName, (IElement**)pEcu);
}

ERRORCODE LDFCluster::GetFrame(unsigned int& unId, IFrame** pFrame)
{
    return EC_NA;
}



//Editor
CLINEcu::CLINEcu(ICluster* pCluster):IEcu(pCluster)
{
    m_oueElementType = eEcuElement;
    nInitialiseEcuProperties();
}

CLINEcu::~CLINEcu()
{

}

ERRORCODE CLINEcu::GetEcuType(eEcuType& eType)
{
    eType = m_eEcuProps.m_eEcuType;
    return EC_SUCCESS;
}

ERRORCODE CLINEcu::SetEcuType(eEcuType& eType)
{
    if( eType == eLIN_Master || eType == eLIN_Slave )
    {
        m_eEcuProps.m_eEcuType = eType;
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}

ERRORCODE CLINEcu::MapFrame(eDIR eDir, UID_ELEMENT& nId)
{
    IFrame* pElement;
    if ( m_pBaseCluster->GetElement(eFrameElement, nId, (IElement**)&pElement)  == EC_SUCCESS )
    {
        eFrameType ouEFrameType;
        pElement->GetFrameType(ouEFrameType);
        if ( m_eEcuProps.m_eEcuType == eLIN_Slave && (ouEFrameType == eLIN_Unconditional ))
        {
            ConfigFrameDetails ouConfigFrame;
            ouConfigFrame.m_uidFrame = nId;
            auto itr = std::find(m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.begin(),m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.end(), ouConfigFrame);
            if ( itr == m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.end() )
            {

                m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.push_back(ouConfigFrame);
            }
        }

        m_nFrames[eDir].push_back(nId);
        m_nFrames[eDir].sort();
        m_nFrames[eDir].unique();
        pElement->RegisterForChangeNotification(this);
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}

ERRORCODE CLINEcu::UnMapFrame(eDIR eDir, UID_ELEMENT& nId)
{
    if ( m_eEcuProps.m_eEcuType == eLIN_Slave )
    {
        ConfigFrameDetails ouConfigFrame;
        ouConfigFrame.m_uidFrame= nId;
        auto itr = std::find(m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.begin(),m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.end(), ouConfigFrame);
        if ( itr != m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.end() )
        {
            m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.erase(itr);
        }
    }
    m_nFrames[eDir].remove(nId);
    return EC_SUCCESS;
}

ERRORCODE CLINEcu::MapSignal(eDIR eDir, UID_ELEMENT& nId)
{
    IElement* pElement;
    if ( m_pBaseCluster->GetElement(eSignalElement, nId, &pElement)  == EC_SUCCESS )
    {
        m_nSignals[eDir][nId] = nId;
        pElement->RegisterForChangeNotification(this);
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}

ERRORCODE CLINEcu::UnMapSignal(eDIR eDir, UID_ELEMENT& nId)
{
    auto itr = m_nSignals[eDir].find(nId);
    if ( itr != m_nSignals[eDir].end() )
    {
        m_nSignals[eDir].erase(itr);
    }
    return EC_SUCCESS;
}

ERRORCODE CLINEcu::GetSignalList(eDIR eDir, std::map<ISignal*, ISignal*>& ouSignals)
{
    IElement* pElement;
for ( auto itr : m_nSignals[eDir])
    {
        if ( m_pBaseCluster->GetElement(eSignalElement, (UID_ELEMENT)itr.first, &pElement)  == EC_SUCCESS )
        {
            ouSignals[(ISignal*)pElement] = (ISignal*)pElement;
        }
    }
    return EC_SUCCESS;
}

ERRORCODE CLINEcu::GetFrameList(eDIR eDir, std::list<IFrame*>& ouFrames)
{
    IElement* pElement;
for ( auto itr : m_nFrames[eDir])
    {
        if ( m_pBaseCluster->GetElement(eFrameElement, itr, &pElement)  == EC_SUCCESS )
        {
            ouFrames.push_back((IFrame*)pElement);
            ouFrames.unique();
        }
    }
    return EC_SUCCESS;
}

ERRORCODE CLINEcu::GetFrame(UID_ELEMENT& nId, IFrame** ouFrame)
{
    IElement* pElement;
    if ( m_pBaseCluster->GetElement(eFrameElement, nId, &pElement)  == EC_SUCCESS )
    {
        *ouFrame = (IFrame*)pElement;
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}

ERRORCODE CLINEcu::GetProperties(EcuProperties& pProperties)
{
    pProperties = m_eEcuProps;
    return EC_SUCCESS;
}

ERRORCODE CLINEcu::SetProperties(EcuProperties& pProperties)
{
    m_eEcuProps = pProperties;
    m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.clear();

for ( auto itr : pProperties.m_ouSlavePros.m_nConfiurableFrameIdList )
    {
        auto itrItem  = find ( m_nFrames[eTx].begin(), m_nFrames[eTx].end(), itr.m_uidFrame);
        if ( itrItem == m_nFrames[eTx].end() )      //Not there in Tx
        {
            itrItem  = find ( m_nFrames[eRx].begin(), m_nFrames[eRx].end(), itr.m_uidFrame);
            if ( itrItem != m_nFrames[eRx].end() )      //Not there in Rx
            {
                m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.push_back(itr);
            }
        }
        else
        {
            m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.push_back(itr);
        }
    }

    eClusterElementType eType = eEcuElement;
    if ( m_pBaseCluster != nullptr )
    {
        m_pBaseCluster->NotifyClusterChange(eModify, m_nUnId, eType, nullptr);
    }
    return EC_SUCCESS;
}

ERRORCODE CLINEcu::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{

    switch(eActionType)
    {
        case eDeleted:
            return OnElementDeleted(nId, ouElementType);
            break;
        default:
            EC_SUCCESS;
            break;
    }

    return EC_NA;
}

ERRORCODE CLINEcu::OnElementDeleted(UID_ELEMENT nId, eClusterElementType ouElementType)
{
    if ( ouElementType == eSignalElement )
    {
        auto itr = m_nSignals[eTx].find(nId);
        if ( itr != m_nSignals[eTx].end() )
        {
            m_nSignals[eTx].erase(itr);
        }

        itr = m_nSignals[eRx].find(nId);
        if ( itr != m_nSignals[eRx].end() )
        {
            m_nSignals[eRx].erase(itr);
        }
    }
    else if ( ouElementType == eFrameElement )
    {
        m_nFrames[eTx].remove(nId);
        m_nFrames[eRx].remove(nId);
        if ( m_eEcuProps.m_eEcuType == eLIN_Slave )
        {
            ConfigFrameDetails ouConfigFrame;
            ouConfigFrame.m_uidFrame = nId;
            auto itr = std::find(m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.begin(),m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.end(), ouConfigFrame);
            if ( itr != m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.end() )
            {
                m_eEcuProps.m_ouSlavePros.m_nConfiurableFrameIdList.erase(itr);
            }
        }
    }
    return EC_SUCCESS;
}




//////////////////////////////////////LINFrame Implimenattion////////////////////////////////////////////////

CLINFrame::CLINFrame(ICluster* pCluster):IFrame(pCluster)
{
    m_oueElementType = eFrameElement;
    nInitialiseFrameProperties();
}

CLINFrame::~CLINFrame()
{

}


ERRORCODE CLINFrame::nInitialiseFrameProperties()
{
    return EC_NA;
}


ERRORCODE CLINFrame::GetFrameType(eFrameType& eType)
{
    eType = m_ouProps.m_eFrameType;
    return EC_SUCCESS;
}

ERRORCODE CLINFrame::GetFrameId(unsigned int& unFrameId)
{
    unFrameId = 0xFF;
    switch (m_ouProps.m_eFrameType)
    {
        case eLIN_Unconditional:
            unFrameId = m_ouProps.m_ouLINUnConditionFrameProps.m_unId;
            break;

        case eLIN_EventTriggered:
            unFrameId = m_ouProps.m_ouLINEventTrigFrameProps.m_unId;
            break;
        case eLIN_Diagnostic:
            unFrameId = m_ouProps.m_ouLINDiagnosticFrameProps.m_unId;
            break;
        default:
            break;
    }
    return EC_SUCCESS;
}

ERRORCODE CLINFrame::GetEcus(eDIR eDirection, std::list<IEcu*>& ouNodes)
{
    IElement* pElement;
    if ( eTx == eDirection )
    {
for(auto itr : m_ouTxNodes)
        {
            if ( m_pBaseCluster->GetElement(eEcuElement, itr, &pElement)  == EC_SUCCESS )
            {
                ouNodes.push_back((IEcu*)pElement);
            }
        }
    }

    else if ( eRx == eDirection )
    {
        UID_ELEMENT unId = INVALID_UID_ELEMENT;
for ( auto itr : m_SignalMap)
        {
            unId = itr.first;
            if ( m_pBaseCluster->GetElement(eSignalElement, unId, &pElement)  == EC_SUCCESS )
            {
                ((ISignal*)pElement)->GetEcus(eRx, ouNodes);
            }
        }
    }

    ouNodes.unique();

    return EC_SUCCESS;
}

ERRORCODE CLINFrame::GetSignalList(std::map<ISignal*, SignalInstanse>& mapSignals)
{
    IElement* pElement;
    UID_ELEMENT unId = INVALID_UID_ELEMENT;
for( auto itr : m_SignalMap)
    {
        unId = itr.first;
        if ( m_pBaseCluster->GetElement(eSignalElement, unId, &pElement)  == EC_SUCCESS )
        {
            mapSignals[(ISignal*)pElement] = itr.second;
        }
    }

    return EC_SUCCESS;
}
ERRORCODE CLINFrame::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    switch(ouElementType)
    {
        case eSignalElement:
            HandleSignalChanges(eActionType, nId, pAcionData);
            break;
        case eFrameElement:
            HandleFrameChanges(eActionType, nId, pAcionData);
            break;
        default:
            EC_SUCCESS;
            break;
    }

    return EC_NA;
}

ERRORCODE CLINFrame::HandleRxEcuUnmap(UID_ELEMENT ouRxEcu)
{
    //Remove the node if no associated signals are haveing the node as publisher
    bool bEcufound = false;
    std::list<IEcu*> ouNodes;
    IElement* pElement;
    IEcu* pEcuUnmapped;
    m_pBaseCluster->GetElement(eEcuElement, ouRxEcu, (IElement**)&pEcuUnmapped);
    if ( nullptr != pEcuUnmapped )
    {
for ( auto itr : m_SignalMap)
        {
            UID_ELEMENT unId = itr.first;
            if ( m_pBaseCluster->GetElement(eSignalElement, unId, &pElement)  == EC_SUCCESS )
            {
                ((ISignal*)pElement)->GetEcus(eRx, ouNodes);
                if ( std::find(ouNodes.begin(), ouNodes.end(), pEcuUnmapped ) != ouNodes.end() )
                {
                    bEcufound = true;
                    break;
                }
            }
        }
        if ( false == bEcufound )
        {
            pEcuUnmapped->UnMapFrame(eRx, m_nUnId);
        }
    }
    return 0;
}


ERRORCODE CLINFrame::HandleSignalChanges(eAction eActionType, UID_ELEMENT nId, void* pAcionData)
{
    switch (eActionType)
    {
        case eTxNodeAdded:
            if ( nullptr != pAcionData )
            {
                UID_ELEMENT uidNewEcu = *((UID_ELEMENT*)pAcionData);
                //Just Remove the Signal Id;
                auto itr = m_SignalMap.find(nId);
                if ( itr != m_SignalMap.end() )
                {
                    m_SignalMap.erase(itr);
                }
            }
            break;
        case eRxNodeRemoved:
        {
            if ( nullptr == pAcionData)
            {
                break;
            }
            UID_ELEMENT uidElement = *((UID_ELEMENT*)pAcionData);
            HandleRxEcuUnmap(uidElement);
        }
        break;
        case eRxNodeAdded:
        {
            if ( nullptr == pAcionData )
            {
                break;
            }
            UID_ELEMENT uidElement = *((UID_ELEMENT*)pAcionData);
            IEcu* pEcuMapped;
            m_pBaseCluster->GetElement(eEcuElement, uidElement, (IElement**)&pEcuMapped);
            if ( nullptr != pEcuMapped )
            {
                pEcuMapped->MapFrame(eRx, m_nUnId);
                m_ouRxNodes.push_back(uidElement);
            }
        }
        break;
        case eBeforeDeletion:
        {
            auto itr = m_SignalMap.find(nId);
            if ( itr != m_SignalMap.end() )
            {
                m_SignalMap.erase(itr);
            }

            ISignal* pSignal = nullptr;
            std::list<IEcu*> ouRxNodes;
            m_pBaseCluster->GetElement(eSignalElement, nId, (IElement**)&pSignal);
            if ( nullptr != pSignal )
            {
                pSignal->GetEcus(eRx, ouRxNodes);
for ( auto itr : ouRxNodes )
                {
                    HandleRxEcuUnmap(itr->GetUniqueId());
                }
            }
        }
        break;
        case eDeleted:
            auto itr = m_SignalMap.find(nId);
            if ( itr != m_SignalMap.end() )
            {
                m_SignalMap.erase(itr);
            }
    }
    return EC_SUCCESS;
}

ERRORCODE CLINFrame::HandleFrameChanges(eAction eActionType, UID_ELEMENT nId, void* pAcionData)
{
    switch (eActionType)
    {
        case eDeleted:
            if ( eLIN_EventTriggered == m_ouProps.m_eFrameType )
            {
                auto itr =  m_ouProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame.find(nId);
                if ( itr != m_ouProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame.end() )
                {
                    m_ouProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame.erase(itr);
                }

            }
            else if ( eLIN_Sporadic == m_ouProps.m_eFrameType )
            {
                auto itr =  m_ouProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame.find(nId);
                if ( itr != m_ouProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame.end() )
                {
                    m_ouProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame.erase(itr);
                }
            }
        default:
            break;
    }
    return EC_SUCCESS;
}


//TODO: Help - Tx Node Map - Signals will be deleted
ERRORCODE CLINFrame::MapNode( eDIR eDirection, UID_ELEMENT& unId)
{
    if ( eTx == eDirection )
    {
        IElement* pElement;
        if ( m_pBaseCluster->GetElement(eEcuElement, unId, &pElement)  == EC_SUCCESS )
        {
            if(m_ouProps.m_eFrameType == eLIN_Diagnostic)
            {
                m_ouTxNodes.push_back(unId);
            }
            else
            {
                if(m_ouTxNodes.size() <= 0)
                {
                    m_ouTxNodes.push_back(unId);
                }
                m_SignalMap.clear();
            }

            return EC_SUCCESS;
        }
        return EC_FAILURE;
    }
    else if ( eRx == eDirection )
    {
        m_ouRxNodes.push_back(unId);
        m_ouRxNodes.sort();
        m_ouRxNodes.unique();
    }

    return EC_NA;
}

ERRORCODE CLINFrame::UnMapNode( eDIR eDirection, UID_ELEMENT& unId)
{
    if ( eTx == eDirection )
    {
        IElement* pElement;
        if ( m_pBaseCluster->GetElement(eEcuElement, unId, &pElement)  == EC_SUCCESS )
        {
            m_ouTxNodes.remove(unId);
            //m_ouTxNode = INVALID_UID_ELEMENT;
            m_SignalMap.clear();
            return EC_SUCCESS;
        }
        return EC_FAILURE;
    }
    else if ( eRx == eDirection )
    {

        IElement* pElement;
        if ( m_pBaseCluster->GetElement(eEcuElement, unId, &pElement)  == EC_SUCCESS )
        {
            m_ouRxNodes.remove(unId);
            return EC_SUCCESS;
        }
        return EC_FAILURE;
    }
    return EC_NA;
}

ERRORCODE CLINFrame::MapSignal( UID_ELEMENT& nId, SignalInstanse& ouProps)
{

    //TODO: Validation Required for signal position,overlapping, length, Tx Nodes
    IElement* pElement;
    if ( m_pBaseCluster->GetElement(eSignalElement, nId, &pElement)  == EC_SUCCESS )
    {
        m_pBaseCluster->RegisterForChangeNotification(this, nId);
        m_SignalMap[nId] = ouProps;
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}

ERRORCODE CLINFrame::UnMapSignal(UID_ELEMENT& nId)
{
    m_SignalMap.erase(nId);
    return EC_FAILURE;
}

ERRORCODE CLINFrame::SetProperties(FrameProps& pFrameProps)
{
    if ( pFrameProps.m_eFrameType < eLIN_Frame_start && pFrameProps.m_eFrameType >= eLIN_Frame_end )
    {
        return EC_FAILURE;
    }

    //TODO: Validation on Signals is required if Frame Length is Changed
    m_ouProps = pFrameProps;


    if ( eLIN_EventTriggered == m_ouProps.m_eFrameType )
    {
for ( auto itr : m_ouProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame )
        {
            m_pBaseCluster->RegisterForChangeNotification(this, itr.first);
        }
    }
    else if ( eLIN_Sporadic == m_ouProps.m_eFrameType )
    {
for ( auto itr : m_ouProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame )
        {
            m_pBaseCluster->RegisterForChangeNotification(this, itr.first);
        }
    }


    return EC_SUCCESS;
}

ERRORCODE CLINFrame::GetProperties(FrameProps& pFrameProps)
{
    pFrameProps = m_ouProps;
    return EC_SUCCESS;
}



//////////////////////////////////////LINSignal Implimenattion////////////////////////////////////////////////


CLINSignal::CLINSignal(ICluster* pBaseCluster):ISignal(pBaseCluster)
{
    m_oueElementType = eSignalElement;
}

CLINSignal::~CLINSignal()
{
}

ERRORCODE CLINSignal::GetEcus(eDIR eDirection, std::list<IEcu*>& ouNodes)
{
    IElement* pElement;
    if ( eTx == eDirection )
    {
for(auto itr : m_ouTxNodes)
        {
            if ( m_pBaseCluster->GetElement(eEcuElement, itr, &pElement)  == EC_SUCCESS )
            {
                ouNodes.push_back((IEcu*)pElement);
            }
        }
    }

    else if ( eRx == eDirection )
    {
for ( auto itr : m_ouRxNodes)
        {
            if ( m_pBaseCluster->GetElement(eEcuElement, itr, &pElement)  == EC_SUCCESS )
            {
                ouNodes.push_back((IEcu*)pElement);
            }
        }
    }

    return EC_SUCCESS;
}


ERRORCODE CLINSignal::MapNode( eDIR eDirection, UID_ELEMENT& unId)
{
    IElement* pElement;
    if ( m_pBaseCluster->GetElement(eEcuElement, unId, &pElement)  != EC_SUCCESS )
    {
        return EC_FAILURE;
    }

    m_pBaseCluster->RegisterForChangeNotification(this, unId);
    if ( eTx == eDirection )
    {
        if(m_ouSignalProps.m_ouLINSignalProps.m_ouSignalType == eSignalDiag)
        {
            m_ouTxNodes.push_back(unId);
        }
        else
        {
            if(m_ouTxNodes.size() <= 0)
            {
                m_ouTxNodes.push_back(unId);
            }
            else
            {
                auto itr = m_ouTxNodes.begin();
                if  ( *itr != unId )
                {

                    IEcu* pEcu;
                    eClusterElementType eType = eSignalElement;
                    if ( m_pBaseCluster != nullptr )
                    {
                        m_pBaseCluster->GetElement(eEcuElement, *itr, (IElement**)&pEcu);
                        if ( nullptr != pEcu )
                        {
                            pEcu->UnMapSignal(eTx, m_nUnId);
                        }
                        m_pBaseCluster->NotifyClusterChange(eTxNodeAdded, m_nUnId, eType, &unId);
                    }
                    *itr = unId;
                }
            }
        }
        return EC_SUCCESS;
    }
    else if ( eRx == eDirection )
    {
        m_ouRxNodes.push_back(unId);
        m_ouRxNodes.sort();
        m_ouRxNodes.unique();
        eClusterElementType oueClusterElementType = eSignalElement;
        m_pBaseCluster->NotifyClusterChange(eRxNodeAdded, m_nUnId, oueClusterElementType, &unId);
        return EC_SUCCESS;
    }
    return EC_NA;
}

ERRORCODE CLINSignal::UnMapNode( eDIR eDirection, UID_ELEMENT& unId)
{
    IElement* pElement;
    if ( m_pBaseCluster->GetElement(eEcuElement, unId, &pElement)  != EC_SUCCESS )
    {
        return EC_FAILURE;
    }

    if ( eTx == eDirection )
    {
        m_ouTxNodes.remove(unId);
        return EC_SUCCESS;
    }
    else if ( eRx == eDirection )
    {
        m_ouRxNodes.remove(unId);
        eClusterElementType oueClusterElementType = eSignalElement;
        m_pBaseCluster->NotifyClusterChange(eRxNodeRemoved, m_nUnId, oueClusterElementType, &unId);
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}


ERRORCODE CLINSignal::GetEncoding(ICoding** pCoding)
{
    return m_pBaseCluster->GetElement(eCodingElement, m_nCodingId, (IElement**)pCoding);
}
ERRORCODE CLINSignal::SetEncoding(UID_ELEMENT& nCodingId)
{
    IElement* pElement;
    if ( ( INVALID_UID_ELEMENT == nCodingId) ||  m_pBaseCluster->GetElement(eCodingElement, nCodingId, &pElement)  == EC_SUCCESS )
    {
        m_nCodingId = nCodingId;
        eClusterElementType eType = eSignalElement;
        if ( m_pBaseCluster != nullptr )
        {
            m_pBaseCluster->NotifyClusterChange(eModify, m_nUnId, eType, nullptr);
        }
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}


ERRORCODE CLINSignal::GetProperties(SignalProps& ouSignalProps)
{
    ouSignalProps = m_ouSignalProps;
    return EC_SUCCESS;
}

ERRORCODE CLINSignal::SetProperties(SignalProps& ouSignalProps)
{
    //TODO: Notification
    m_ouSignalProps = ouSignalProps;
    eClusterElementType eType = eSignalElement;
    if ( m_pBaseCluster != nullptr )
    {
        m_pBaseCluster->NotifyClusterChange(eModify, m_nUnId, eType, nullptr);
    }
    return EC_SUCCESS;
}
ERRORCODE CLINSignal::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    switch(ouElementType)
    {
        case eCodingElement:
            return HandleCodingChanges(nId, eActionType);
            break;
        case eEcuElement:
            return HandleEcuChanges(nId, eActionType);
            break;
        default:
            EC_SUCCESS;
            break;
    }

    return EC_NA;

}
ERRORCODE CLINSignal::HandleEcuChanges(UID_ELEMENT nId, eAction eActionType)
{
    switch (eActionType)
    {
        case eDeleted:
            m_ouTxNodes.remove(nId);
            m_ouRxNodes.remove(nId);
            break;
        default:
            break;
    }
    return EC_SUCCESS;
}


ERRORCODE CLINSignal::HandleCodingChanges(UID_ELEMENT nId, eAction eActionType)
{
    switch (eActionType)
    {
        case eDeleted:
            if ( m_nCodingId == nId )
            {
                m_nCodingId = INVALID_UID_ELEMENT;
            }
            break;
        default:
            break;
    }
    return EC_SUCCESS;
}


//////////////////////////////////////Signal Group Implimenattion////////////////////////////////////////////////

CLINSignalGroup::CLINSignalGroup(ICluster* pBaseCluster):ISignalGroup(pBaseCluster)
{
    m_pBaseCluster = pBaseCluster;
    m_oueElementType = eSignalGroupElement;
}

CLINSignalGroup::~CLINSignalGroup()
{

}

ERRORCODE CLINSignalGroup::MapSignal( UID_ELEMENT& nId, SignalInstanse& ouProps)
{
    IElement* pElement;
    if ( EC_SUCCESS == m_pBaseCluster->GetElement(eSignalElement, nId, &pElement) )
    {
        m_ouSignalList[nId] = ouProps;
        return EC_SUCCESS;
    }
    return EC_NA;
}

eClusterElementType CLINSignalGroup::GetElementType()
{
    return eSignalGroupElement;
}


ERRORCODE CLINSignalGroup::UnMapSignal(UID_ELEMENT& nId)
{
    auto itrUID = m_ouSignalList.find(nId);
    if ( m_ouSignalList.end() != itrUID )
    {
        m_ouSignalList.erase(itrUID);
        return EC_SUCCESS;
    }
    return EC_NA;
}

ERRORCODE CLINSignalGroup::GetProperties(SignalGroupProps& ouProps)
{
    ouProps = m_ouSignalGroupProps;
    return EC_SUCCESS;
}

ERRORCODE CLINSignalGroup::SetProperties(SignalGroupProps& ouProps)
{
    m_ouSignalGroupProps = ouProps;
    return EC_SUCCESS;
}


ERRORCODE CLINSignalGroup::GetSignalList(std::map<ISignal*, SignalInstanse>& mapSignals)
{
    ISignal* pSignal;
    UID_ELEMENT uidTemp = INVALID_UID_ELEMENT;
for ( auto itrSignal : m_ouSignalList )
    {
        uidTemp = itrSignal.first;
        if ( EC_SUCCESS == m_pBaseCluster->GetElement(eSignalElement, uidTemp, (IElement**)&pSignal) )
        {
            mapSignals[pSignal] = itrSignal.second;
        }
    }
    return EC_SUCCESS;
}

ERRORCODE CLINSignalGroup::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    return EC_SUCCESS;
}


//////////////////////////////////////LIN Coding Implimenattion////////////////////////////////////////////////


CLINCoding::CLINCoding(ICluster* pCluster):ICoding(pCluster)
{
    //TODO: Initialise CompuMethod Props
    m_oueElementType = eCodingElement;
    m_pBaseCluster = pCluster;
}

ERRORCODE CLINCoding::GetProperties(CompuMethodProps& ouCompuMethodProp)
{
    ouCompuMethodProp = m_ouCompuMethodProps;
    return EC_SUCCESS;
}

ERRORCODE CLINCoding::SetProperties(CompuMethodProps& ouCompuMethodProp)
{
    m_ouCompuMethodProps = ouCompuMethodProp;
    return EC_SUCCESS;
}

ERRORCODE CLINCoding::GetValueDescriptions(std::map<int, std::string>&)
{
    return EC_NA;
}

ERRORCODE CLINCoding::SetValueDescriptions(std::map<int, std::string>&)
{
    return EC_NA;
}

ERRORCODE CLINCoding::GetValueDescription(int, std::string&)
{
    return EC_NA;
}

ERRORCODE CLINCoding::SetValueDescription(int, std::string&)
{
    return EC_NA;
}

ERRORCODE CLINCoding::GetSignalList(std::map<UID_ELEMENT, ISignal*>& lstSignals)
{
    ISignal* pSignal;
    UID_ELEMENT unId = INVALID_UID_ELEMENT;
for ( auto itr : m_ouMapSignal)
    {
        unId = itr.first;
        if ( EC_SUCCESS == m_pBaseCluster->GetElement(eSignalElement, unId, (IElement**)&pSignal) )
        {
            lstSignals[unId] = pSignal;
        }
    }
    return EC_SUCCESS;
}

ERRORCODE CLINCoding::MapSignal(UID_ELEMENT unId)
{
    ISignal* pSignal;

    if ( EC_SUCCESS == m_pBaseCluster->GetElement(eSignalElement, unId, (IElement**)&pSignal) )
    {
        m_ouMapSignal[unId] = unId;
    }
    else
    {
        return EC_FAILURE;
    }
    return EC_SUCCESS;
}
ERRORCODE CLINCoding::UnMapSignal(UID_ELEMENT unId)
{
    auto itr = m_ouMapSignal.find(unId);
    if ( itr != m_ouMapSignal.end())
    {
        m_ouMapSignal.erase(itr);
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}

ERRORCODE CLINCoding::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    switch(ouElementType)
    {
        case eFrameElement:
            break;
        default:
            EC_SUCCESS;
            break;
    }

    return EC_NA;

}

//////////////////////////////////////LIN Coding Implimenattion////////////////////////////////////////////////



CSheduleTable::CSheduleTable(ICluster* pBaseCluster):IScheduleTable(pBaseCluster)
{
    m_oueElementType = eScheduleTableElement;
}

ERRORCODE CSheduleTable::GetProperties(ScheduleTableProps& ouProperties)
{
    ouProperties = m_ouSheduleTableProps;
    return EC_SUCCESS;
}

ERRORCODE CSheduleTable::SetProperties(ScheduleTableProps& ouProperties)
{
    m_ouSheduleTableProps = ouProperties;

for(auto itr : m_ouSheduleTableProps.m_ouCSheduleTableItem)
    {
        m_pBaseCluster->RegisterForChangeNotification(this);
    }

    return EC_SUCCESS;
}
ERRORCODE CSheduleTable::RegisterForChangeNotification( INotifyClusterChange* ouNotification)
{
    if ( nullptr != m_pBaseCluster )
    {
        return m_pBaseCluster->RegisterForChangeNotification(ouNotification, m_nUnId);
    }
    return EC_FAILURE;
}
ERRORCODE CSheduleTable::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    switch(ouElementType)
    {
        case eFrameElement:
            return HandleFrameChanges(nId, eActionType);
            break;
        case eEcuElement:
            return HandleEcuChanges(nId, eActionType);
            break;
        default:
            EC_SUCCESS;
            break;
    }
    return EC_NA;
}
ERRORCODE CSheduleTable::HandleEcuChanges(UID_ELEMENT nId, eAction eActionType)
{
    std::list<CSheduleTableItem>::iterator itr = m_ouSheduleTableProps.m_ouCSheduleTableItem.begin();
    switch (eActionType)
    {
        case eDeleted:

            while(itr != m_ouSheduleTableProps.m_ouCSheduleTableItem.end())
            {
                //CSheduleTable ouSchedTable = itr;
                if(itr->m_nNode == nId)
                {
                    itr = m_ouSheduleTableProps.m_ouCSheduleTableItem.erase(itr);
                    continue;
                }
                itr++;
            }
            return EC_SUCCESS;
            break;
        default:
            break;
    }
    return EC_NA;
}



ERRORCODE CSheduleTable::HandleFrameChanges(UID_ELEMENT nId, eAction eActionType)
{
    std::list<CSheduleTableItem>::iterator itr = m_ouSheduleTableProps.m_ouCSheduleTableItem.begin();
    switch (eActionType)
    {
        case eDeleted:

            while(itr != m_ouSheduleTableProps.m_ouCSheduleTableItem.end())
            {
                //CSheduleTable ouSchedTable = itr;
                if(itr->m_nFrameId == nId)
                {
                    itr = m_ouSheduleTableProps.m_ouCSheduleTableItem.erase(itr);
                    continue;
                }
                itr++;
            }
            return EC_SUCCESS;
            break;
        default:
            break;
    }
    return EC_NA;
}

//////////////////////////////ElementContainer Implimentation///////////////////////////////////////////////

ElementContainer::ElementContainer(ICluster* pBaseContainer)
{
    m_pBaseContainer = pBaseContainer;
}

ERRORCODE ElementContainer::GetElementByName(std::string& strEcuName, IElement** pEcu)
{
    *pEcu = nullptr;
    return EC_NA;
}

ERRORCODE ElementContainer::GetElementByID(UID_ELEMENT& unId, IElement** pEcu)
{
    return EC_NA;
}

//////////////////////////////EcuContainer Implimentation///////////////////////////////////////////////
EcuContainer::EcuContainer(ICluster* pBaseContainer):ElementContainer(pBaseContainer)
{
    m_oupBaseContainer = pBaseContainer;
}

ERRORCODE EcuContainer::CreateElement(IElement** pElement)
{
    CLINEcu* pLinEcu = new CLINEcu(m_pBaseContainer);
    m_ouEcuMap[pLinEcu->m_nUnId] = pLinEcu;
    *pElement = (IElement*)pLinEcu;
    pLinEcu->RegisterForChangeNotification(this);
    return EC_SUCCESS;
}

ERRORCODE EcuContainer::DeleteElement(UID_ELEMENT& nUniqueId)
{
    //TODO: Notification
    if ( m_ouEcuMap.find(nUniqueId) != m_ouEcuMap.end() )
    {
        m_ouEcuMap.erase(nUniqueId);

        eClusterElementType ouClusterElement = eEcuElement;
        m_pBaseContainer->NotifyClusterChange(eDeleted, nUniqueId, ouClusterElement, nullptr);
        return EC_SUCCESS;
    }
    else
    {
        return EC_FAIL_ELEMENT_NOT_FOUND;
    }
}

ERRORCODE EcuContainer::GetElement(UID_ELEMENT& nId, IElement** pElement)
{
    std::map<UID_ELEMENT, IEcu*>::iterator itr = m_ouEcuMap.find(nId);
    if ( m_ouEcuMap.end() != itr )
    {
        *pElement = itr->second;
        return EC_SUCCESS;
    }
    else
    {
        *pElement = nullptr;
        return EC_FAILURE;
    }
}

ERRORCODE EcuContainer::GetElementList( std::map<UID_ELEMENT, IElement*>& pElement )
{
for ( auto itr : m_ouEcuMap )
    {
        pElement[itr.first] = (IElement*)itr.second;
    }
    return EC_SUCCESS;
}

ERRORCODE EcuContainer::GetElementByName(std::string& strEcuName, IElement** pEcu)
{
    *pEcu = nullptr;
    auto itr = m_ouNameToId.find(strEcuName);
    if ( itr != m_ouNameToId.end() )
    {
        return m_oupBaseContainer->GetElement(eEcuElement, itr->second, pEcu);
    }

    return EC_NA;
}

ERRORCODE EcuContainer::GetElementByID(UID_ELEMENT& unId, IElement** pEcu)
{
    return EC_NA;
}
ERRORCODE EcuContainer::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    switch(eActionType)
    {
        case eNameChange:
        {
            eNameChangeActionData* ouActionData = (eNameChangeActionData*)pAcionData;
            if ( ouActionData->m_strNewName != ouActionData->m_strOldName )
            {

                m_ouNameToId[ouActionData->m_strNewName] = nId;
                m_ouNameToId.erase(ouActionData->m_strOldName);
            }
        }
        break;
    }
    return EC_NA;
}

ERRORCODE EcuContainer::Clear()
{
for(auto itrEcu : m_ouEcuMap)
    {
        delete (CLINEcu*)itrEcu.second;
    }
    m_ouEcuMap.clear();
    m_ouNameToId.clear();
    return EC_SUCCESS;
}

ERRORCODE CLINEcu::nInitialiseEcuProperties()
{
    return EC_NA;
}



//////////////////////////////FrameContainer Implimentation///////////////////////////////////////////////
FrameContainer::FrameContainer(ICluster* pBaseContainer):ElementContainer(pBaseContainer)
{

}

ERRORCODE OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    return EC_NA;
}
ERRORCODE FrameContainer::CreateElement(IElement** pElement)
{
    CLINFrame* pLinEcu = new CLINFrame(m_pBaseContainer);
    m_ouFrameMap[pLinEcu->m_nUnId] = pLinEcu;
    *pElement = (IElement*)pLinEcu;
    pLinEcu->RegisterForChangeNotification(this);
    return EC_SUCCESS;
}

ERRORCODE FrameContainer::DeleteElement(UID_ELEMENT& nUniqueId)
{
    //TODO: Notification
    if ( m_ouFrameMap.find(nUniqueId) != m_ouFrameMap.end() )
    {
        m_ouFrameMap.erase(nUniqueId);

        eClusterElementType ouClusterElement = eFrameElement;
        m_pBaseContainer->NotifyClusterChange(eDeleted, nUniqueId, ouClusterElement, nullptr);
        return EC_SUCCESS;
    }
    else
    {
        return EC_FAIL_ELEMENT_NOT_FOUND;
    }
}

ERRORCODE FrameContainer::Clear()
{
for(auto itrEcu : m_ouFrameMap)
    {
        delete (CLINFrame*)itrEcu.second;
    }
    m_ouFrameMap.clear();
    return EC_SUCCESS;
}

ERRORCODE FrameContainer::GetElement(UID_ELEMENT& nId, IElement** pElement)
{
    std::map<UID_ELEMENT, IFrame*>::iterator itr = m_ouFrameMap.find(nId);
    if ( m_ouFrameMap.end() != itr )
    {
        *pElement = itr->second;
        return EC_SUCCESS;
    }
    else
    {
        *pElement = nullptr;
        return EC_FAILURE;
    }
}

ERRORCODE FrameContainer::GetElementList( std::map<UID_ELEMENT, IElement*>& pElement )
{
for ( auto itr : m_ouFrameMap )
    {
        pElement[itr.first] = (IElement*)itr.second;
    }
    return EC_SUCCESS;
}

ERRORCODE FrameContainer::GetElementByName(std::string& strEcuName, IElement** pEcu)
{
    *pEcu = nullptr;
    return EC_NA;
}

ERRORCODE FrameContainer::GetElementByID(UID_ELEMENT& unId, IElement** pEcu)
{
    return EC_NA;
}
ERRORCODE FrameContainer::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    return EC_NA;
}


//////////////////////////////SignalContainer Implimentation///////////////////////////////////////////////
SignalContainer::SignalContainer(ICluster* pBaseContainer):ElementContainer(pBaseContainer)
{

}

ERRORCODE SignalContainer::CreateElement(IElement** pElement)
{
    CLINSignal* pLinEcu = new CLINSignal(m_pBaseContainer);
    m_ouSignalMap[pLinEcu->m_nUnId] = pLinEcu;
    *pElement = (IElement*)pLinEcu;
    return EC_SUCCESS;
}

ERRORCODE SignalContainer::DeleteElement(UID_ELEMENT& nUniqueId)
{
    //TODO: Notification
    if ( m_ouSignalMap.find(nUniqueId) != m_ouSignalMap.end() )
    {
        eClusterElementType ouClusterElement = eSignalElement;
        m_pBaseContainer->NotifyClusterChange(eBeforeDeletion, nUniqueId, ouClusterElement, nullptr);
        m_ouSignalMap.erase(nUniqueId);
        m_pBaseContainer->NotifyClusterChange(eDeleted, nUniqueId, ouClusterElement, nullptr);
        return EC_SUCCESS;
    }
    else
    {
        return EC_FAIL_ELEMENT_NOT_FOUND;
    }
}


ERRORCODE SignalContainer::Clear()
{
for(auto itrEcu : m_ouSignalMap)
    {
        delete (CLINSignal*)itrEcu.second;
    }
    m_ouSignalMap.clear();
    return EC_SUCCESS;
}


ERRORCODE SignalContainer::GetElement(UID_ELEMENT& nId, IElement** pElement)
{
    std::map<UID_ELEMENT, ISignal*>::iterator itr = m_ouSignalMap.find(nId);
    if ( m_ouSignalMap.end() != itr )
    {
        *pElement = itr->second;
        return EC_SUCCESS;
    }
    else
    {
        *pElement = nullptr;
        return EC_FAILURE;
    }
}

ERRORCODE SignalContainer::GetElementList( std::map<UID_ELEMENT, IElement*>& pElement )
{
for ( auto itr : m_ouSignalMap )
    {
        pElement[itr.first] = (IElement*)itr.second;
    }
    return EC_SUCCESS;
}

ERRORCODE SignalContainer::GetElementByName(std::string& strEcuName, IElement** pEcu)
{
    *pEcu = nullptr;
    return EC_NA;
}

ERRORCODE SignalContainer::GetElementByID(UID_ELEMENT& unId, IElement** pEcu)
{
    return EC_NA;
}


//////////////////////////////SignalGroupContainer Implimentation//////////////////////////////////////////
/*class SignalGroupContainer : public ElementContainer
{
    std::map<UID_ELEMENT, ISignalGroup*> m_ouSignalGroupMap;
public:
    SignalGroupContainer(ICluster*);
    ERRORCODE CreateElement(IElement** pElement);
    ERRORCODE DeleteElement(UID_ELEMENT& nUniqueId);
    ERRORCODE GetElement(UID_ELEMENT& nId, IElement** pElement);
    ERRORCODE GetElementList( std::map<UID_ELEMENT, IElement*>& pElement );
    ERRORCODE GetElementByName(std::string& strEcuName, IElement** pEcu) ;
    ERRORCODE GetElementByID(UID_ELEMENT& unId, IElement** pEcu) ;
};*/

CLINSignalGroupContainer::CLINSignalGroupContainer(ICluster* pBaseContainer):ElementContainer(pBaseContainer)
{

}

ERRORCODE CLINSignalGroupContainer::CreateElement(IElement** pElement)
{
    CLINSignalGroup* pLinSignalGroup = new CLINSignalGroup(m_pBaseContainer);
    m_ouSignalGroupMap[pLinSignalGroup->m_nUnId] = pLinSignalGroup;
    *pElement = (IElement*)pLinSignalGroup;
    return EC_SUCCESS;
}

ERRORCODE CLINSignalGroupContainer::DeleteElement(UID_ELEMENT& nUniqueId)
{
    auto itrSignalGroup = m_ouSignalGroupMap.find(nUniqueId);
    if ( m_ouSignalGroupMap.end() != itrSignalGroup )
    {
        m_ouSignalGroupMap.erase(itrSignalGroup);
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}
ERRORCODE CLINSignalGroupContainer::GetElement(UID_ELEMENT& nId, IElement** pElement)
{
    auto itr = m_ouSignalGroupMap.find(nId);
    *pElement = nullptr;
    if ( m_ouSignalGroupMap.end() != itr )
    {
        *pElement = itr->second;
        return EC_SUCCESS;
    }
    return EC_FAILURE;
}


ERRORCODE CLINSignalGroupContainer::Clear()
{
for(auto itrEcu : m_ouSignalGroupMap)
    {
        delete (CLINSignalGroup*)itrEcu.second;
    }
    m_ouSignalGroupMap.clear();
    return EC_SUCCESS;
}

ERRORCODE CLINSignalGroupContainer::GetElementList(std::map<UID_ELEMENT, IElement*>& pElement )
{
for ( auto itr : m_ouSignalGroupMap )
    {
        pElement[itr.first] = (IElement*)itr.second;
    }
    return EC_SUCCESS;
}

//////////////////////////////CodingContainer Implimentation///////////////////////////////////////////////
CodingContainer::CodingContainer(ICluster* pBaseContainer):ElementContainer(pBaseContainer)
{

}

ERRORCODE CodingContainer::CreateElement(IElement** pElement)
{
    CLINCoding* pLinEcu = new CLINCoding(m_pBaseContainer);
    m_ouCodingMap[pLinEcu->m_nUnId] = pLinEcu;
    *pElement = (IElement*)pLinEcu;
    return EC_SUCCESS;
}

ERRORCODE CodingContainer::DeleteElement(UID_ELEMENT& nUniqueId)
{
    //TODO: Notification
    if ( m_ouCodingMap.find(nUniqueId) != m_ouCodingMap.end() )
    {
        m_ouCodingMap.erase(nUniqueId);

        eClusterElementType ouClusterElement = eCodingElement;
        m_pBaseContainer->NotifyClusterChange(eDeleted, nUniqueId, ouClusterElement, nullptr);
        return EC_SUCCESS;
    }
    else
    {
        return EC_FAIL_ELEMENT_NOT_FOUND;
    }

}

ERRORCODE CodingContainer::Clear()
{
for(auto itrEcu : m_ouCodingMap)
    {
        delete (CLINCoding*)itrEcu.second;
    }
    m_ouCodingMap.clear();
    return EC_SUCCESS;
}

ERRORCODE CodingContainer::GetElement(UID_ELEMENT& nId, IElement** pElement)
{
    std::map<UID_ELEMENT, ICoding*>::iterator itr = m_ouCodingMap.find(nId);
    if ( m_ouCodingMap.end() != itr )
    {
        *pElement = itr->second;
        return EC_SUCCESS;
    }
    else
    {
        *pElement = nullptr;
        return EC_FAILURE;
    }
}

ERRORCODE CodingContainer::GetElementList( std::map<UID_ELEMENT, IElement*>& pElement )
{
for ( auto itr : m_ouCodingMap )
    {
        pElement[itr.first] = (IElement*)itr.second;
    }
    return EC_SUCCESS;
}

ERRORCODE CodingContainer::GetElementByName(std::string& strEcuName, IElement** pEcu)
{
    *pEcu = nullptr;
    return EC_NA;
}

ERRORCODE CodingContainer::GetElementByID(UID_ELEMENT& unId, IElement** pEcu)
{
    return EC_NA;
}

//////////////////////////////ScheduleTableContainer Implimentation///////////////////////////////////////////////

CLINSheduleTableContainer::CLINSheduleTableContainer(ICluster* pBaseContainer):ElementContainer(pBaseContainer)
{

}

ERRORCODE CLINSheduleTableContainer::CreateElement(IElement** pElement)
{
    CSheduleTable* pLinEcu = new CSheduleTable(m_pBaseContainer);
    m_ouScheduleTableMap[pLinEcu->m_nUnId] = pLinEcu;
    *pElement = (IElement*)pLinEcu;
    return EC_SUCCESS;
}

ERRORCODE CLINSheduleTableContainer::DeleteElement(UID_ELEMENT& nUniqueId)
{
    //TODO: Notification
    if ( m_ouScheduleTableMap.find(nUniqueId) != m_ouScheduleTableMap.end() )
    {
        m_ouScheduleTableMap.erase(nUniqueId);

        eClusterElementType ouClusterElement = eScheduleTableElement;
        m_pBaseContainer->NotifyClusterChange(eDeleted, nUniqueId, ouClusterElement, nullptr);
        return EC_SUCCESS;
    }
    else
    {
        return EC_FAIL_ELEMENT_NOT_FOUND;
    }
}

ERRORCODE CLINSheduleTableContainer::Clear()
{
for(auto itrEcu : m_ouScheduleTableMap)
    {
        delete (CSheduleTable*)itrEcu.second;
    }
    m_ouScheduleTableMap.clear();

    return EC_SUCCESS;
}

ERRORCODE CLINSheduleTableContainer::GetElement(UID_ELEMENT& nId, IElement** pElement)
{
    std::map<UID_ELEMENT, IScheduleTable*>::iterator itr = m_ouScheduleTableMap.find(nId);
    if ( m_ouScheduleTableMap.end() != itr )
    {
        *pElement = itr->second;
        return EC_SUCCESS;
    }
    else
    {
        *pElement = nullptr;
        return EC_FAILURE;
    }
}

ERRORCODE CLINSheduleTableContainer::GetElementList( std::map<UID_ELEMENT, IElement*>& pElement )
{
for ( auto itr : m_ouScheduleTableMap )
    {
        pElement[itr.first] = (IElement*)itr.second;
    }
    return EC_SUCCESS;
}

ERRORCODE CLINSheduleTableContainer::GetElementByName(std::string& strEcuName, IElement** pEcu)
{
    *pEcu = nullptr;
    return EC_NA;
}

ERRORCODE CLINSheduleTableContainer::GetElementByID(UID_ELEMENT& unId, IElement** pEcu)
{
    return EC_NA;
}


CLDFNodeConfiguration::CLDFNodeConfiguration(ICluster* pBaseCluster):INodeConfiguration(pBaseCluster)
{
    m_pBaseCluster = pBaseCluster;
    m_oueElementType = eLinNodeComposition;
}



CLDFNodeConfigContainer::CLDFNodeConfigContainer(ICluster* pBaseContainer):ElementContainer(pBaseContainer)
{
    m_pBaseContainer = pBaseContainer;
}



ERRORCODE CLDFNodeConfigContainer::CreateElement(IElement** pElement)
{
    CLDFNodeConfiguration* pouLdfNodeConfig = new CLDFNodeConfiguration(m_pBaseContainer);
    m_ouNodeConfigMap[pouLdfNodeConfig->m_nUnId] = pouLdfNodeConfig;
    *pElement = (IElement*)pouLdfNodeConfig;
    return EC_SUCCESS;
}

ERRORCODE CLDFNodeConfigContainer::DeleteElement(UID_ELEMENT& nUniqueId)
{
    //TODO: Notification
    if ( m_ouNodeConfigMap.find(nUniqueId) != m_ouNodeConfigMap.end() )
    {
        m_ouNodeConfigMap.erase(nUniqueId);

        eClusterElementType ouClusterElement = eScheduleTableElement;
        m_pBaseContainer->NotifyClusterChange(eDeleted, nUniqueId, ouClusterElement, nullptr);
        return EC_SUCCESS;
    }
    else
    {
        return EC_FAIL_ELEMENT_NOT_FOUND;
    }
}

ERRORCODE CLDFNodeConfigContainer::GetElement(UID_ELEMENT& nId, IElement** pElement)
{
    std::map<UID_ELEMENT, INodeConfiguration*>::iterator itr = m_ouNodeConfigMap.find(nId);
    if ( m_ouNodeConfigMap.end() != itr )
    {
        *pElement = itr->second;
        return EC_SUCCESS;
    }
    else
    {
        *pElement = nullptr;
        return EC_FAILURE;
    }
}

ERRORCODE CLDFNodeConfigContainer::GetElementList( std::map<UID_ELEMENT, IElement*>& pElement )
{
for ( auto itr : m_ouNodeConfigMap )
    {
        pElement[itr.first] = (IElement*)itr.second;
    }
    return EC_SUCCESS;
}

ERRORCODE CLDFNodeConfigContainer::GetElementByName(std::string& strEcuName, IElement** pEcu)
{
    *pEcu = nullptr;
    return EC_NA;
}

ERRORCODE CLDFNodeConfigContainer::GetElementByID(UID_ELEMENT& unId, IElement** pEcu)
{
    return EC_NA;
}

ERRORCODE CLDFNodeConfigContainer::OnClusterChange(eAction eActionType, UID_ELEMENT nId, eClusterElementType& ouElementType, void* pAcionData, void* pUserData)
{
    return EC_NA;
}

ERRORCODE CLDFNodeConfigContainer::Clear()
{
for(auto itrEcu : m_ouNodeConfigMap)
    {
        delete (CLDFNodeConfiguration*)itrEcu.second;
    }
    m_ouNodeConfigMap.clear();

    return EC_SUCCESS;
}



