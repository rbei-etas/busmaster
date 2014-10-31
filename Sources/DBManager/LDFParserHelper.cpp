#include "stdafx.h"
#include "LDFParserHelper.h"
#include "Defines.h"
#include "ScheduleTableParser.h"
#include "LDFTagDefines.h"


#define RETURN_IF_ERROR if( true == m_bParseError ) return EC_FAILURE;

CLDFParserHelper::CLDFParserHelper(LDFCluster* pouLDFCluster)
{
    m_pouLDFCluster = pouLDFCluster;
    m_pouLDFCluster->EnableEventNotofications();
    m_bParseError = false;
    m_bMasterAdded = false;
}

CLDFParserHelper::CLDFParserHelper()
{
    m_pouLDFCluster = nullptr;
    m_bLinTagFound = false;
    m_bParseError = false;
    m_bMasterAdded = false;
}

CLDFParserHelper::~CLDFParserHelper(void)
{
}

ERRORCODE CLDFParserHelper::LinDescrptionTagFound()
{
    m_bLinTagFound = true;
    return EC_SUCCESS;
}



ERRORCODE CLDFParserHelper::AddComment(std::string& strComment)
{
    RETURN_IF_ERROR;

    if ( m_bLinTagFound == false )
    {
        LIN_Settings ouProtocolSettings;
        m_pouLDFCluster->GetProperties(eLINClusterProperties, &ouProtocolSettings);
        ouProtocolSettings.m_strHeaderComment += strComment;
        m_pouLDFCluster->SetProperties(eLINClusterProperties, &ouProtocolSettings);
    }

    return EC_SUCCESS;
}



void CLDFParserHelper::SetCluster(LDFCluster* pCluster, char** pTokens)
{
    m_pchTokens = pTokens;
    m_pouLDFCluster = pCluster;
    m_pouLDFCluster->EnableEventNotofications();
}

bool CLDFParserHelper::bIsValidProtocol(LIN_Settings& ouSettings)
{
    return false;
}



ERRORCODE CLDFParserHelper::AddProtocolInfo(double dProtocolVer, double dLanguagever, double dSpeed)
{
    RETURN_IF_ERROR;

    m_pouLDFCluster->GetProperties(eLINClusterProperties, &m_ouProtocolSettings);

    if ( !(bAreEqual(dProtocolVer, defVersion_1_3) ||
            bAreEqual(dProtocolVer, defVersion_2_0) ||
            bAreEqual(dProtocolVer, defVersion_2_1)) )
    {
        AddError(eError, "Invalid Protocol Version (Supported Versions 1.3, 2.0, 2.1)", "File Can not be loaded");
        return EC_FAILURE;
    }


    if ( !(bAreEqual(dLanguagever, defVersion_1_3) ||
            bAreEqual(dLanguagever, defVersion_2_0) ||
            bAreEqual(dLanguagever, defVersion_2_1)) )
    {
        AddError(eError, "Invalid Langauge Version (Supported Versions 1.3, 2.0, 2.1)", "File Can not be loaded");
        return EC_FAILURE;
    }

    m_ouProtocolSettings.m_dProtocolVers = dProtocolVer;
    m_ouProtocolSettings.m_dLanguageVers = dLanguagever;
    m_ouProtocolSettings.m_dBaudRate = dSpeed;

    m_pouLDFCluster->SetProperties(eLINClusterProperties, &m_ouProtocolSettings);
    return EC_SUCCESS;
}

ERRORCODE CLDFParserHelper::AddChannelName(std::string& strChannelName)
{
    RETURN_IF_ERROR;
    LIN_Settings ouProtocolSettings;
    m_pouLDFCluster->GetProperties(eLINClusterProperties, &ouProtocolSettings);

    if ( bAreEqual ( ouProtocolSettings.m_dProtocolVers, defVersion_2_1 ) == true )
    {
        Trim(strChannelName, '"');
        ouProtocolSettings.m_strChannelName = strChannelName;
        m_pouLDFCluster->SetProperties(eLINClusterProperties, &ouProtocolSettings);
        return EC_SUCCESS;
    }
    else
    {
        AddError(eWarning, "Tag \"Channel_name\" is Invalid in current version", "Tag will be removed once the file is saved");
    }
}


void CLDFParserHelper:: Trim(std::string& str , char chChar)
{
    /*std::string::size_type pos1 = str.find_first_not_of(chChar);
    std::string::size_type pos2 = str.find_last_not_of(chChar);

    if(pos1 != -1 &&  pos2 != -1)
    {
    str = str.substr(pos1 == std::string::npos ? 0 : pos1, pos2 == std::string::npos ? str.length() - 1 : pos2 - pos1 + 1);
    }
    else
    {
    str = "";
    }*/

    size_t p = str.find_first_not_of(chChar);
    str.erase(0, p);

    p = str.find_last_not_of(chChar);
    if (std::string::npos != p)
    {
        str.erase(p+1);
    }
}


ERRORCODE CLDFParserHelper::AddMaster(std::string& strName, double dTimeBase, double dJitterBase)
{
    RETURN_IF_ERROR;
    if ( false == m_bMasterAdded )
    {
        if ( m_mapEcuUID.find(strName) != m_mapEcuUID.end() )
        {
            char chError[256];
            sprintf_s(chError, sizeof(chError),"Duplicate Ecu Name \" %s \" Found", strName.c_str());
            AddError(eError, chError, "File Can not be loaded");
            return EC_FAILURE;
        }

        IEcu* pElement;
        EcuProperties ouEcuProperties;
        UID_ELEMENT uidEcu = INVALID_UID_ELEMENT;
        m_pouLDFCluster->CreateElement(eEcuElement, (IElement**)&pElement);

        if(nullptr != pElement)
        {
            pElement->GetProperties(ouEcuProperties);
        }
        ouEcuProperties.m_eEcuType = eLIN_Master;
        ouEcuProperties.m_ouMasterProps.m_fJitter = (double)dJitterBase;
        ouEcuProperties.m_ouMasterProps.m_fTimeBase = (double)dTimeBase;

        if(nullptr != pElement)
        {
            pElement->SetName(strName);
            pElement->SetProperties(ouEcuProperties);
            pElement->GetUniqueId(uidEcu);
        }
        m_mapEcuUID[strName] = uidEcu;
        m_bMasterAdded = true;
    }
    else
    {
        AddError(eError, "Master is Already added", "File can not be Loaded");
        return EC_FAILURE;
    }
    return EC_SUCCESS;
}

ERRORCODE CLDFParserHelper::AddSlaves(std::list<std::string>& ouSlaveList)
{
    RETURN_IF_ERROR;
    IEcu* pElement;
    EcuProperties ouEcuProperties;
for (auto strNode : ouSlaveList )
    {
        if ( m_mapEcuUID.find(strNode) != m_mapEcuUID.end() )
        {
            char chError[256];
            sprintf_s(chError, sizeof(chError),"Duplicate Ecu Name \" %s \" Found", strNode.c_str());
            AddError(eError, chError, "File can not be Loaded");
            return EC_FAILURE;
        }

        UID_ELEMENT uidEcu = INVALID_UID_ELEMENT;
        m_pouLDFCluster->CreateElement(eEcuElement, (IElement**)&pElement);

        if(nullptr != pElement)
        {
            pElement->GetProperties(ouEcuProperties);
            ouEcuProperties.m_eEcuType = eLIN_Slave;
            pElement->SetName(strNode);
            ouEcuProperties.m_ouSlavePros.m_nConfiguredNAD = 0XFF;

            ouEcuProperties.m_ouSlavePros.m_fProtocolVersion = m_ouProtocolSettings.m_dProtocolVers;
            pElement->SetProperties(ouEcuProperties);
            pElement->GetUniqueId(uidEcu);
        }

        m_mapEcuUID[strNode] = uidEcu;

    }
    return EC_SUCCESS;
}

ERRORCODE CLDFParserHelper::AddSignal(ldfsignalDefinition& ouldfsignalDefinition)
{
    RETURN_IF_ERROR;
    char chError[256];
    if ( m_mapSignalUID.find(ouldfsignalDefinition.m_strName) != m_mapSignalUID.end() )
    {
        sprintf_s(chError, sizeof(chError),"Duplicate Signal Name  \" %s \" Found", ouldfsignalDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }


    ISignal* pSignalElement;
    UID_ELEMENT unID = INVALID_UID_ELEMENT;

    m_pouLDFCluster->CreateElement(eSignalElement, (IElement**)&pSignalElement);

    if(nullptr != pSignalElement)
    {
        pSignalElement->SetName(ouldfsignalDefinition.m_strName);
        pSignalElement->GetUniqueId(unID);
    }

    m_mapSignalUID[ouldfsignalDefinition.m_strName] = unID;

    SignalProps ouProps;
    ouProps.eType = eLINProtocol;

    ouProps.m_ouLINSignalProps.m_nLength = ouldfsignalDefinition.m_nSize;
    ouProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    ouProps.m_ouLINSignalProps.m_ouEndianess = eIntel;

    if ( eByteArray == ouldfsignalDefinition.m_ouValueType && bAreEqual(defVersion_1_3 , m_ouProtocolSettings.m_dProtocolVers) == true )
    {
        sprintf_s(chError, sizeof(chError),"Byte Array in Version %f for Signal \" %s \" is Invalid.", defVersion_1_3,
                  ouldfsignalDefinition.m_strName.c_str());
        AddError(eWarning, chError, "Signal value will be saved as Scalar value");
    }

    ouProps.m_ouLINSignalProps.m_ouValueType = ouldfsignalDefinition.m_ouValueType;
    ouProps.m_ouLINSignalProps.m_nIntialValue = ouldfsignalDefinition.m_unInitValue;
    ouProps.m_ouLINSignalProps.m_ouSignalType = ouldfsignalDefinition.m_ouSignalType;

    if(nullptr != pSignalElement)
    {
        pSignalElement->SetProperties(ouProps);
    }
    if ( eSignalNormal == ouldfsignalDefinition.m_ouSignalType )
    {
        IEcu* pouEcu = nullptr;
        m_pouLDFCluster->GetEcu(ouldfsignalDefinition.m_strPublisher, &pouEcu);
        if ( pouEcu != nullptr )
        {
            pouEcu->GetUniqueId(unID);
            pSignalElement->MapNode(eTx, unID);

            unID = pSignalElement->GetUniqueId();
            pouEcu->MapSignal(eTx, unID);
        }
        else
        {
            sprintf_s(chError, sizeof(chError),"Publisher Ecu %s is Invalid for Signal \" %s \" Found",
                      ouldfsignalDefinition.m_strPublisher.c_str(), ouldfsignalDefinition.m_strName.c_str());
            AddError(eError, chError, "File can not be Loaded");
            return EC_FAILURE;
        }
        std::map<std::string, std::string> strEcuNames;
        std::string strDuplicateNames;
for ( auto itr : ouldfsignalDefinition.m_strSuscriberList )
        {
            m_pouLDFCluster->GetEcu(itr, &pouEcu);
            if ( pouEcu != nullptr )
            {
                if ( strEcuNames.find(itr) != strEcuNames.end() )
                {
                    strDuplicateNames += "\"";
                    strDuplicateNames +=itr + "\" ";
                }
                else
                {
                    pouEcu->GetUniqueId(unID);
                    pSignalElement->MapNode(eRx, unID);

                    unID = pSignalElement->GetUniqueId();
                    pouEcu->MapSignal(eRx, unID);
                }
            }
            else
            {
                sprintf_s(chError, sizeof(chError),"Subscriber Ecu %s is Invalid for Signal \" %s \" Found",
                          ouldfsignalDefinition.m_strPublisher.c_str(), ouldfsignalDefinition.m_strName.c_str());
                AddError(eWarning, chError, "Invalid Ecu names are ignored.");
            }
        }
        if ( strDuplicateNames.size() > 0 )
        {
            sprintf_s(chError, sizeof(chError),"Duplicate Subscriber Ecu(s) %s Found",
                      strDuplicateNames.c_str());
            AddError(eWarning, chError, "Duplicate Ecu names are Ignored");
        }
    }

    return EC_SUCCESS;
}

ERRORCODE CLDFParserHelper::AddMessage(ldfFrameDefinition& ouldfFrameDefinition)
{
    RETURN_IF_ERROR;
    IFrame* pFrameElement = nullptr;
    UID_ELEMENT uidFrame = INVALID_UID_ELEMENT;
    char chError[254] = "";
    if ( m_mapFrameUID.find(ouldfFrameDefinition.m_strName) != m_mapFrameUID.end() )
    {
        sprintf_s(chError, sizeof(chError),"Duplicate Unconditional frame Name  \" %s \" Found", ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }

    if ( ouldfFrameDefinition.m_unId >= 0x3C && ouldfFrameDefinition.m_unId < 0x40 )
    {
        sprintf_s(chError, sizeof(chError),"ID \" %d \" used for Frame %s is reserved and should not be used for User Defined Frames",
                  ouldfFrameDefinition.m_unId, ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }

    if ( ouldfFrameDefinition.m_unId > 0x40 )
    {
        sprintf_s(chError, sizeof(chError),"ID  \" %d \" is Invalid for Unconditional Frame %s ", ouldfFrameDefinition.m_unId, ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }

    m_pouLDFCluster->GetFrame(ouldfFrameDefinition.m_unId, &pFrameElement);
    if ( nullptr != pFrameElement )
    {
        //Already ID is defined.
        if ( bAreEqual( m_ouProtocolSettings.m_dProtocolVers, defVersion_2_0 ) == true )
        {
            std::list<unsigned int> listDynamicFrames;
            m_pouLDFCluster->GetProperties(eLdfDyanmicFrameList, &listDynamicFrames);
            listDynamicFrames.unique();
            listDynamicFrames.sort();
            if ( std::find(listDynamicFrames.begin(), listDynamicFrames.end(), ouldfFrameDefinition.m_unId) == listDynamicFrames.end() )
            {
                sprintf_s(chError, sizeof(chError),"Duplicate Unconditional frame ID  \" %d \" Found", ouldfFrameDefinition.m_unId);
                AddError(eError, chError, "File can not be Loaded");
                return EC_FAILURE;
            }
        }
        else
        {
            sprintf_s(chError, sizeof(chError),"Duplicate Unconditional frame ID  \" %d \" Found", ouldfFrameDefinition.m_unId);
            AddError(eError, chError, "File can not be Loaded");
            return EC_FAILURE;
        }
    }



    m_pouLDFCluster->CreateElement(eFrameElement, (IElement**)&pFrameElement);

    if(nullptr != pFrameElement)
    {
        pFrameElement->GetUniqueId(uidFrame);

        m_mapFrameUID[ouldfFrameDefinition.m_strName] = uidFrame;

        //1. Set Frame Name
        pFrameElement->SetName(ouldfFrameDefinition.m_strName);
    }

    //2. Set Frame Props
    FrameProps ouFrameProps;
    ouFrameProps.m_eFrameType = eLIN_Unconditional;

    ouFrameProps.m_ouLINUnConditionFrameProps.m_nLength = ouldfFrameDefinition.m_unSize;
    if ( ouldfFrameDefinition.m_unSize > 8 || ouldfFrameDefinition.m_unSize <= 0 )
    {
        sprintf_s(chError, sizeof(chError),"Invalid Unconditional frame Length \" %d \" Found for Frame %s",
                  ouldfFrameDefinition.m_unSize, ouldfFrameDefinition.m_strName.c_str());
        AddError(eWarning, chError, "Length is Resized to 8 Bytes");
        ouFrameProps.m_ouLINUnConditionFrameProps.m_nLength = 8;
    }


    if ( ouldfFrameDefinition.m_unId < defLIN_VALID_UNCOND_START_ID  || ouldfFrameDefinition.m_unId > defLIN_VALID_UNCOND_END_ID )
    {
        sprintf_s(chError, sizeof(chError),"Invalid Unconditional frame ID \" %d \" Found for Frame %s",
                  ouldfFrameDefinition.m_unId, ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }

    ouFrameProps.m_ouLINUnConditionFrameProps.m_unId = ouldfFrameDefinition.m_unId;

    pFrameElement->SetProperties(ouFrameProps);

    //3. Map Tx ECU and Map Tx Frame to Ecu
    IEcu* pouEcu = nullptr;
    UID_ELEMENT unEcuID = INVALID_UID_ELEMENT;
    m_pouLDFCluster->GetEcu(ouldfFrameDefinition.m_strPublisher, &pouEcu);
    if ( pouEcu != nullptr )
    {
        pouEcu->GetUniqueId(unEcuID);
        pFrameElement->MapNode(eTx, unEcuID);

        pouEcu->MapFrame(eTx, uidFrame);
    }
    else
    {
        sprintf_s(chError, sizeof(chError),"Publisher Ecu %s Not Defined for Unconditional Frame \" %s \" ",
                  ouldfFrameDefinition.m_strPublisher.c_str(), ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }

    //4. Map Signals and Map RX frames TO ecu
    SignalInstanse ouSignalInstance;
    ISignal* pSignal;
    std::list<IEcu*> ecuList;
for ( auto itr : ouldfFrameDefinition.m_mapSignalPos )
    {

        UID_ELEMENT unSigID = m_mapSignalUID[itr.first];
        m_pouLDFCluster->GetElement(eSignalElement, unSigID,  (IElement**)&pSignal);
        ecuList.clear();

        if(nullptr != pSignal)
        {
            pSignal->GetEcus(eTx, ecuList);
        }
        else
        {
            sprintf_s(chError, sizeof(chError),"Signal %s Associated to Frame %s is not defined in LDF File.",
                      itr.first.c_str(), ouldfFrameDefinition.m_strName.c_str());
            AddError(eWarning, chError, "Signal Will be ignored");
            continue;
        }
        //Ldf contains one Ecu so take First Ecu only;
        auto itrTxEcu =  ecuList.begin();

        if ( itrTxEcu != ecuList.end() )
        {

            UID_ELEMENT unTempEcuId = INVALID_UID_ELEMENT;
            (*itrTxEcu)->GetUniqueId(unTempEcuId);
            if ( unTempEcuId == unEcuID )
            {
                ouSignalInstance.m_nStartBit = itr.second;
                pFrameElement->MapSignal(unSigID, ouSignalInstance);
                ecuList.clear();
                if(nullptr != pSignal)
                {
                    pSignal->GetEcus(eRx, ecuList);
                }
for ( auto itrEcu : ecuList )
                {
                    UID_ELEMENT uidEcu = itrEcu->GetUniqueId();
                    itrEcu->MapFrame(eRx, uidFrame);
                    pFrameElement->MapNode(eRx, uidEcu);
                }
            }
        }
    }
    return EC_SUCCESS;
}

ERRORCODE CLDFParserHelper::AddMessage(ldfSporadicFrameDef& ouldfFrameDefinition)
{
    RETURN_IF_ERROR;
    IFrame* pFrameElement;
    UID_ELEMENT uidFrame = INVALID_UID_ELEMENT;

    char chError[254] = "";

    if ( m_mapFrameUID.find(ouldfFrameDefinition.m_strName) != m_mapFrameUID.end() )
    {
        sprintf_s(chError, sizeof(chError),"Duplicate Sporadic frame Name  \" %s \" Found", ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }

    m_pouLDFCluster->CreateElement(eFrameElement, (IElement**)&pFrameElement);

    if(nullptr != pFrameElement)
    {
        pFrameElement->GetUniqueId(uidFrame);
    }
    m_mapFrameUID[ouldfFrameDefinition.m_strName] = uidFrame;

    if(nullptr != pFrameElement)
    {
        //1. Set Frame Name
        pFrameElement->SetName(ouldfFrameDefinition.m_strName);
    }

    //2. Set Frame Props
    FrameProps ouFrameProps;
    ouFrameProps.m_eFrameType = eLIN_Sporadic;

    //3. Map Associated Frames
    IElement* pUnCondFrame = nullptr;
    UID_ELEMENT unID = INVALID_UID_ELEMENT;
for ( auto itr : ouldfFrameDefinition.m_FrameList )
    {
        std::map<std::string, UID_ELEMENT>::iterator itrFrame = m_mapFrameUID.find(itr);
        if ( m_mapFrameUID.end() != itrFrame )
        {
            m_pouLDFCluster->GetElement(eFrameElement, itrFrame->second, &pUnCondFrame);

            if(nullptr != pUnCondFrame)
            {
                ouFrameProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame[itrFrame->second] = itrFrame->second;
            }
        }
        else
        {
            sprintf_s(chError, sizeof(chError),"Unconditional Frame %s Associated to Sporadic frame \" %s \" is Not Found", itr.c_str(), ouldfFrameDefinition.m_strName.c_str());
            AddError(eWarning, chError, "Frame was ignored and will be deleted once the file is saved.");
        }

    }

    pFrameElement->SetProperties(ouFrameProps);

    return EC_SUCCESS;
}

ERRORCODE CLDFParserHelper::AddMessage(ldfEventTriggerdFrame& ouldfFrameDefinition)
{
    RETURN_IF_ERROR;
    IFrame* pFrameElement = nullptr;
    UID_ELEMENT uidEventFrameId = INVALID_UID_ELEMENT;
    char chError[254] = "";

    if ( ouldfFrameDefinition.m_unId >= 0x3C && ouldfFrameDefinition.m_unId < 0x40 )
    {
        sprintf_s(chError, sizeof(chError),"ID \" %d \" used for Frame %s is reserved and should not be used for User Defined Frames",
                  ouldfFrameDefinition.m_unId, ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }

    if ( ouldfFrameDefinition.m_unId > 0x40 )
    {
        sprintf_s(chError, sizeof(chError),"ID  \" %d \" is Invalid for Event Triggered Frame %s ", ouldfFrameDefinition.m_unId, ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }


    m_pouLDFCluster->GetFrame(ouldfFrameDefinition.m_unId, &pFrameElement);
    if ( nullptr != pFrameElement )
    {
        //Already ID is defined.
        if ( bAreEqual( m_ouProtocolSettings.m_dProtocolVers, defVersion_2_0 ) == true )
        {
            std::list<unsigned int> listDynamicFrames;
            m_pouLDFCluster->GetProperties(eLdfDyanmicFrameList, &listDynamicFrames);
            listDynamicFrames.unique();
            listDynamicFrames.sort();
            if ( std::find(listDynamicFrames.begin(), listDynamicFrames.end(), ouldfFrameDefinition.m_unId) == listDynamicFrames.end() )
            {
                sprintf_s(chError, sizeof(chError),"Duplicate Unconditional frame ID  \" %d \" Found", ouldfFrameDefinition.m_unId);
                AddError(eError, chError, "File can not be Loaded");
                return EC_FAILURE;
            }
        }
        else
        {
            sprintf_s(chError, sizeof(chError),"Duplicate Unconditional frame ID  \" %d \" Found", ouldfFrameDefinition.m_unId);
            AddError(eError, chError, "File can not be Loaded");
            return EC_FAILURE;
        }
    }

    if ( ouldfFrameDefinition.m_unId < defLIN_VALID_UNCOND_START_ID  || ouldfFrameDefinition.m_unId > defLIN_VALID_UNCOND_END_ID )
    {
        sprintf_s(chError, sizeof(chError),"Invalid Eventriggered frame ID \" %d \" Found for Frame %s",
                  ouldfFrameDefinition.m_unId, ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }

    if ( m_mapFrameUID.find(ouldfFrameDefinition.m_strName) != m_mapFrameUID.end() )
    {
        sprintf_s(chError, sizeof(chError),"Duplicate Event Triggered frame Name  \" %s \" Found", ouldfFrameDefinition.m_strName.c_str());
        AddError(eError, chError, "File can not be Loaded");
        return EC_FAILURE;
    }
    m_pouLDFCluster->CreateElement(eFrameElement, (IElement**)&pFrameElement);

    if(nullptr != pFrameElement)
    {
        pFrameElement->GetUniqueId(uidEventFrameId);
    }
    m_mapFrameUID[ouldfFrameDefinition.m_strName] = uidEventFrameId;

    if(nullptr != pFrameElement)
    {
        //1. Set Frame Name
        pFrameElement->SetName(ouldfFrameDefinition.m_strName);
    }

    //2. Set Frame Props
    FrameProps ouFrameProps;
    ouFrameProps.m_eFrameType = eLIN_EventTriggered;

    if ( ouldfFrameDefinition.m_strScheduleTable.size() <= 0 && bAreEqual(m_ouProtocolSettings.m_dProtocolVers, defVersion_2_1) == true )
    {
        sprintf_s(chError, sizeof(chError),"No schedule table is Associated to Event Triggered frame \" %s \"", ouldfFrameDefinition.m_strName.c_str());
        AddError(eWarning, chError, "");
    }
    else
    {
        m_ScheduleEventFrameMap[ouldfFrameDefinition.m_strScheduleTable].push_back(uidEventFrameId);
    }



    //3. Map Associated Frames

    //Warning if no frames
    if ( ouldfFrameDefinition.m_strAssociatedFrames.size() <= 0 )
    {
        sprintf_s(chError, sizeof(chError),"No Frames are Associated to Event Triggered frame \" %s \"", ouldfFrameDefinition.m_strName.c_str());
        AddError(eWarning, chError, "");
    }
    else
    {
        IElement* pUnCondFrame = nullptr;
        UID_ELEMENT unID = INVALID_UID_ELEMENT;
for ( auto itr : ouldfFrameDefinition.m_strAssociatedFrames )
        {
            std::map<std::string, UID_ELEMENT>::iterator itrFrame = m_mapFrameUID.find(itr);
            if ( m_mapFrameUID.end() != itrFrame )
            {
                m_pouLDFCluster->GetElement(eFrameElement, itrFrame->second, &pUnCondFrame);
                ouFrameProps.m_ouLINEventTrigFrameProps.m_pouUnconditionalFrame[itrFrame->second] = itrFrame->second;
            }
            else
            {
                sprintf_s(chError, sizeof(chError),"Unconditional Frame %s Associated to Event Triggered \" %s \" is Not Found", itr.c_str(), ouldfFrameDefinition.m_strName.c_str());
                AddError(eWarning, chError, "Frame was ignored and will be deleted once the file is saved.");
            }
        }
    }

    //4. ID
    ouFrameProps.m_ouLINEventTrigFrameProps.m_unId = ouldfFrameDefinition.m_unId;
    pFrameElement->SetProperties(ouFrameProps);

    return EC_NA;
}




ERRORCODE CLDFParserHelper::AddMessage(ldfDiagFrame& ouldfDiagFrame)
{
    RETURN_IF_ERROR;
    IFrame* pFrameElement;
    UID_ELEMENT uidEventFrameId = INVALID_UID_ELEMENT;

    m_pouLDFCluster->CreateElement(eFrameElement, (IElement**)&pFrameElement);

    if(nullptr != pFrameElement)
    {
        pFrameElement->GetUniqueId(uidEventFrameId);
    }
    m_mapFrameUID[ouldfDiagFrame.m_strName] = uidEventFrameId;

    if(nullptr != pFrameElement)
    {
        //1. Set Frame Name
        pFrameElement->SetName(ouldfDiagFrame.m_strName);
    }

    //2. Set Frame Props
    FrameProps ouFrameProps;
    ouFrameProps.m_eFrameType = eLIN_Diagnostic;

    ouFrameProps.m_ouLINDiagnosticFrameProps.m_nLength = 8; //Const
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_unId = ouldfDiagFrame.m_unId;


    //3. Map Tx ECU
    std::map<UID_ELEMENT, IElement*> ouEcuMap;
    EcuProperties ouEcuProps;
    UID_ELEMENT uId = INVALID_UID_ELEMENT;
    eDIR oueDir = eTx;
    m_pouLDFCluster->GetElementList(eEcuElement, ouEcuMap);
    UID_ELEMENT unMasterNode;
    std::list<UID_ELEMENT> lstSlaveNodes;
for ( auto itrEcu : ouEcuMap )
    {
        ((IEcu*)itrEcu.second)->GetProperties(ouEcuProps);
        if ( ouEcuProps.m_eEcuType == eLIN_Master )
        {
            oueDir = ouldfDiagFrame.m_unId == 0x3c ? eTx : eRx;
            unMasterNode = ((IEcu*)itrEcu.second)->GetUniqueId();
        }
        else if ( ouEcuProps.m_eEcuType == eLIN_Slave )
        {
            oueDir = ouldfDiagFrame.m_unId == 0x3d ? eRx : eTx;
            lstSlaveNodes.push_back( ((IEcu*)itrEcu.second)->GetUniqueId());
        }
        uId = pFrameElement->GetUniqueId();
        //pFrameElement->MapNode(oueDir, uId);
        ((IEcu*)itrEcu.second)->MapFrame(oueDir, uId);
    }
    ouFrameProps.m_ouLINDiagnosticFrameProps.m_eDiagType = eLIN_SLAVE_FRAME_ID;
    if ( ouldfDiagFrame.m_unId == 0x3c )
    {
        ouFrameProps.m_ouLINDiagnosticFrameProps.m_eDiagType = eLIN_MASTER_FRAME_ID;
    }


    ouFrameProps.m_eFrameType = eLIN_Diagnostic;

    if(nullptr != pFrameElement)
    {
        pFrameElement->SetProperties(ouFrameProps);

        if(ouldfDiagFrame.m_unId == 0x3c)
        {
            pFrameElement->MapNode(eTx, unMasterNode);
        }
        else
        {
for(auto itrSlave : lstSlaveNodes)
            {
                pFrameElement->MapNode(eTx, itrSlave);
            }
        }
    }
    //4. Map Signals
    IElement* pSignal = nullptr;
    SignalInstanse ouSignalInstance;
for ( auto itr : ouldfDiagFrame.m_strAssociatedSignals )
    {
        //TODO: Validations
        uId = m_mapSignalUID[itr.first];
        m_pouLDFCluster->GetElement(eSignalElement,uId, &pSignal);
        ouSignalInstance.m_nStartBit = itr.second;
        pFrameElement->MapSignal(uId, ouSignalInstance);

        if(nullptr != pSignal)
        {
            // Map Subscribers to signals
            if(ouldfDiagFrame.m_unId == 0x3c)
            {
                // If MasterReq, Publisher = Master Node, Subcriber = Each Slave node
for(auto itrSlave : lstSlaveNodes)
                {
                    ((ISignal*)pSignal)->MapNode(eRx, itrSlave);
                }
                ((ISignal*)pSignal)->MapNode(eTx, unMasterNode);
            }
            else
            {
                // If MasterReq, Publisher = Each Slave node, Subcriber = Master node
for(auto itrSlave : lstSlaveNodes)
                {
                    ((ISignal*)pSignal)->MapNode(eTx, itrSlave);
                }
                ((ISignal*)pSignal)->MapNode(eRx, unMasterNode);
            }
        }
    }
    return EC_SUCCESS;
}

ERRORCODE CLDFParserHelper::AddScheduleTable(ldfScheduleTable& ouScheduleTable)
{
    RETURN_IF_ERROR;
    IScheduleTable* pScheduleTable;
    UID_ELEMENT ouUid = INVALID_UID_ELEMENT;
    m_pouLDFCluster->CreateElement(eScheduleTableElement, (IElement**)&pScheduleTable);

    m_pouLDFCluster->GetNextUniqueId(ouUid);

    CScheduleTableParser ouScheduleTableParser(m_pouLDFCluster, m_mapFrameUID);


    if ( ouScheduleTable.m_ouCommandList.size() <= 0 )
    {
        char chError[1024] = "";
        sprintf_s(chError, sizeof(chError),"Empty Schedule table \"%s\" Found", ouScheduleTable.m_strTableName.c_str());
        AddError(eWarning, chError, "");
    }

    ouScheduleTableParser.GetScheduleTable(pScheduleTable, ouScheduleTable);


    auto itrEventList =  m_ScheduleEventFrameMap.find(ouScheduleTable.m_strTableName);
    IFrame* pEventFrame;

    if ( itrEventList != m_ScheduleEventFrameMap.end() )
    {
        FrameProps ouFrameProps;
for( auto itrEventFrame : itrEventList->second )
        {
            m_pouLDFCluster->GetElement(eFrameElement, itrEventFrame, (IElement**)&pEventFrame);
            if ( nullptr != pEventFrame )
            {
                pEventFrame->GetProperties(ouFrameProps);
                pScheduleTable->GetUniqueId(ouUid);
                ouFrameProps.m_ouLINEventTrigFrameProps.m_nCollisionResolveTable = ouUid;
                pEventFrame->SetProperties(ouFrameProps);
            }
        }
    }

    return EC_NA;
}

ERRORCODE CLDFParserHelper::AddNodeAttributes(ldfNodeProperties& ouLdfNodeProps)
{
    RETURN_IF_ERROR;

    char chError[1024] = "";
    if ( bAreEqual(m_ouProtocolSettings.m_dProtocolVers, defVersion_1_3 ) == true )
    {
        sprintf_s(chError, sizeof(chError),"Node Attribute Section is Invalid For Node \"%s\" ", ouLdfNodeProps.m_strName.c_str());
        AddError(eWarning, chError, "Node Attribute Section will be ignored once the file is saved.");
    }
    else
    {
        IEcu* pEcu;
        EcuProperties ouNodeProps;
        m_pouLDFCluster->GetEcu(ouLdfNodeProps.m_strName, &pEcu);

        if ( nullptr != pEcu )
        {
            pEcu->GetProperties(ouNodeProps);

            if ( ouLdfNodeProps.m_dProtocolVer > m_ouProtocolSettings.m_dProtocolVers )
            {
                sprintf_s(chError, sizeof(chError),"ECU \"%s\" Protocol version is Greater than Actual Protocol section", ouLdfNodeProps.m_strName.c_str());
                AddError(eWarning, chError, "ECU Protocol version is changed to Actual Protocol section");

                ouLdfNodeProps.m_dProtocolVer = m_ouProtocolSettings.m_dProtocolVers;
            }
            else if ( ouLdfNodeProps.m_dProtocolVer != 1.3 && ouLdfNodeProps.m_dProtocolVer != 2.0 &&
                      ouLdfNodeProps.m_dProtocolVer != 2.1 )
            {
                sprintf_s(chError, sizeof(chError),"ECU \"%s\" Protocol version is Invalid", ouLdfNodeProps.m_strName.c_str());
                AddError(eWarning, chError, "ECU Protocol version is changed to Actual Protocol section");
                ouLdfNodeProps.m_dProtocolVer = m_ouProtocolSettings.m_dProtocolVers;
            }


            ouNodeProps.m_ouSlavePros.m_fProtocolVersion = ouLdfNodeProps.m_dProtocolVer;
            ouNodeProps.m_ouSlavePros.m_dNASTimeout = ouLdfNodeProps.m_dN_As_timeout;
            ouNodeProps.m_ouSlavePros.m_dNCRTimeout = ouLdfNodeProps.m_dN_Cr_timeout;
            ouNodeProps.m_ouSlavePros.m_dP2Min = ouLdfNodeProps.m_dP2Min;
            ouNodeProps.m_ouSlavePros.m_dSTMin = ouLdfNodeProps.m_dSTMin;
            ouNodeProps.m_ouSlavePros.m_nConfiguredNAD = ouLdfNodeProps.m_unConfigNAD;
            ouNodeProps.m_ouSlavePros.m_nInitialNAD = ouLdfNodeProps.m_unInitialNAD;
            ouNodeProps.m_ouSlavePros.m_nFunctionId = ouLdfNodeProps.m_unFunctionId;
            ouNodeProps.m_ouSlavePros.m_nSupplierId = ouLdfNodeProps.m_unSupplierId;
            ouNodeProps.m_ouSlavePros.m_nVariant = ouLdfNodeProps.m_unVariant;

            if ( ouLdfNodeProps.m_strRespErrSigName.size() > 0 )
            {
                auto itrSignalUID = m_mapSignalUID.find(ouLdfNodeProps.m_strRespErrSigName);

                if ( m_mapSignalUID.end() != itrSignalUID )
                {
                    ouNodeProps.m_ouSlavePros.m_nRespErrSignal = itrSignalUID->second;
                }
                else
                {
                    sprintf_s(chError, sizeof(chError),"Response Signal %s is Invalid For ECU \"%s\" ", ouLdfNodeProps.m_strRespErrSigName.c_str(), ouLdfNodeProps.m_strName.c_str());
                    AddError(eWarning, chError, "Signal was ignored");
                }
            }



            std::string strInvalid = "";
for ( auto itrFaultSigList : ouLdfNodeProps.m_strFaultSignalList )
            {
                auto itrSignalUID = m_mapSignalUID.find(itrFaultSigList);
                if ( m_mapSignalUID.end() != itrSignalUID )
                {
                    ouNodeProps.m_ouSlavePros.m_nFaultStateSignals.push_back( itrSignalUID->second);
                }
                else
                {
                    strInvalid += itrFaultSigList + " ";
                }
            }
            if ( strInvalid.size() > 0 )
            {
                sprintf_s(chError, sizeof(chError),"Falut state Signal(s) %s is Invalid For ECU \"%s\" ", strInvalid.c_str(), ouLdfNodeProps.m_strName.c_str());
                AddError(eWarning, chError, "Signals were ignored");
            }


            //Configurable Frames
            std::list<IFrame*> ouFrameList;
            std::map<UID_ELEMENT, UID_ELEMENT> uniqueFrameList;
            pEcu->GetFrameList(eTx, ouFrameList);
            pEcu->GetFrameList(eRx, ouFrameList);
            eFrameType oueFrameType;
for ( auto itr : ouFrameList )
            {
                itr->GetFrameType(oueFrameType);
                if ( oueFrameType == eLIN_Unconditional )
                {
                    uniqueFrameList[itr->GetUniqueId()] = itr->GetUniqueId();
                }
            }


            ouFrameList.unique();
            ouFrameList.sort();

            ouNodeProps.m_ouSlavePros.m_nConfiurableFrameIdList.clear();
            strInvalid = "";
for ( auto itrFramList : ouLdfNodeProps.FrameToIdMap )
            {
                auto itrFrameUID = m_mapFrameUID.find(itrFramList.m_strFrameName);

                if ( m_mapFrameUID.end() != itrFrameUID )
                {
                    auto itrValidConfigFrame = uniqueFrameList.find(itrFrameUID->second);
                    if ( itrValidConfigFrame != uniqueFrameList.end())
                    {
                        ConfigFrameDetails ouConfigFrame;
                        ouConfigFrame.m_uidFrame = itrFrameUID->second;
                        ouConfigFrame.m_unConfigMsgId = itrFramList.unId;
                        ouNodeProps.m_ouSlavePros.m_nConfiurableFrameIdList.push_back(ouConfigFrame);
                        uniqueFrameList.erase(itrValidConfigFrame);
                    }
                }
                else
                {
                    strInvalid += itrFramList.m_strFrameName.c_str() ;
                    strInvalid += " ";
                }
            }

for ( auto itr : uniqueFrameList )
            {
                ConfigFrameDetails ouConfigFrame;
                ouConfigFrame.m_uidFrame = itr.first;
                ouConfigFrame.m_unConfigMsgId = 0;
                ouNodeProps.m_ouSlavePros.m_nConfiurableFrameIdList.push_back(ouConfigFrame);
            }


            if ( strInvalid.size() > 0 )
            {
                sprintf_s(chError, sizeof(chError),"Configurable Frame(s) %s is Invalid For ECU \"%s\" ", strInvalid.c_str(), ouLdfNodeProps.m_strName.c_str());
                AddError(eWarning, chError, "Frames ignored");
            }

            pEcu->SetProperties(ouNodeProps);
        }
        else
        {
            sprintf_s(chError, sizeof(chError),"ECU \"%s\" In Attribute Section is Invalid ", ouLdfNodeProps.m_strName.c_str());
            AddError(eWarning, chError, "Node Attribute Section For the ECU will be ignored once the file is saved.");
        }
    }
    return EC_NA;
}
ERRORCODE CLDFParserHelper::AddNodeDiagAddress(std::string& strNode, unsigned int unDiagId) //unDiagId is ConfigNAD;
{
    RETURN_IF_ERROR;
    char chError[1024];
    if ( bAreEqual(m_ouProtocolSettings.m_dProtocolVers, defVersion_1_3 ) == false )
    {
        sprintf_s(chError, sizeof(chError),"Node Diag Address Section is Invalid For Node \"%s\" ", strNode.c_str());
        AddError(eWarning, chError, "Node Diag Address Section will be ignored once the file is saved.");
    }
    IEcu* pEcu = nullptr;
    EcuProperties ouNodeProps;
    m_pouLDFCluster->GetEcu(strNode, &pEcu);

    if ( nullptr != pEcu )
    {
        pEcu->GetProperties(ouNodeProps);
        ouNodeProps.m_ouSlavePros.m_nConfiguredNAD = unDiagId;
        pEcu->SetProperties(ouNodeProps);
    }
    else
    {
        sprintf_s(chError, sizeof(chError),"ECU %s in Diag Address Section is Invalid ", strNode.c_str());
        AddError(eWarning, chError, "Node Diag Address Section For the ECU Will be ignored.");
    }
    return EC_NA;
}



ERRORCODE CLDFParserHelper::AddSignalCompuMethod(std::string& strName, LINCompuMethods& ouCompuMethod)
{
    RETURN_IF_ERROR;

    char chError[1024] = "";
    if (m_mapCompuMethodToUID.find(strName) != m_mapCompuMethodToUID.end() )
    {
        sprintf_s(chError, sizeof(chError),"Duplicate Coding %s was found", strName.c_str());
        AddError(eError, chError, "File can not be loaded");
        return EC_FAILURE;
    }



    ICoding* pCoding;

    UID_ELEMENT uidCompuMethod = INVALID_UID_ELEMENT;
    m_pouLDFCluster->CreateElement(eCodingElement, (IElement**)&pCoding);

    if(nullptr != pCoding)
    {
        pCoding->GetUniqueId(uidCompuMethod);

        m_mapCompuMethodToUID[strName] = uidCompuMethod;
        pCoding->SetName(strName);
    }

    CompuMethodProps ouCompuMethodProps;
    ouCompuMethodProps.m_eType = eLINProtocol;
    ouCompuMethodProps.m_ouLinCompuMethods = ouCompuMethod;
    pCoding->SetProperties(ouCompuMethodProps);

    return EC_NA;
}

ERRORCODE CLDFParserHelper::MapCompuMethodToSignals(std::string& strCompuMethodName, std::list<std::string>& signalsList)
{
    RETURN_IF_ERROR;
    UID_ELEMENT uidCompuMethod = INVALID_UID_ELEMENT;
    auto itrCompuMethod = m_mapCompuMethodToUID.find(strCompuMethodName);
    ISignal* pSignal;
    ICoding* pCoding;
    UID_ELEMENT uidtem = INVALID_UID_ELEMENT;
    char chError[1024] = "";
    if ( m_mapCompuMethodToUID.end() == itrCompuMethod )
    {
        sprintf_s(chError, sizeof(chError),"Invalid Coding %s was found in \"Signal_representation\" section", strCompuMethodName.c_str());
        AddError(eWarning, chError, "File can not be loaded");
        return EC_FAILURE;
    }

    pCoding = nullptr;
for ( auto itrSignal : signalsList )
    {
        auto itrUIDSignal = m_mapSignalUID.find(itrSignal);
        if ( m_mapSignalUID.end() != itrUIDSignal )
        {
            m_pouLDFCluster->GetElement(eSignalElement, itrUIDSignal->second, (IElement**)&pSignal);
            if( nullptr != pSignal )
            {
                pCoding = nullptr;
                pSignal->GetEncoding(&pCoding);
                if ( nullptr == pCoding )
                {
                    pSignal->SetEncoding(itrCompuMethod->second);
                    uidtem = itrCompuMethod->second;
                    m_pouLDFCluster->GetElement(eCodingElement, uidtem, (IElement**)&pCoding);

                    if(nullptr != pCoding)
                    {
                        pCoding->MapSignal(itrUIDSignal->second);
                    }
                }
                else
                {
                    std::string strName;
                    char chTemp[1024] = "";

                    pSignal->GetName(strName);
                    sprintf_s(chError, sizeof(chError),"Coding %s was already added for signal %s", strCompuMethodName.c_str(), strName.c_str());

                    pCoding->GetName(strName);
                    sprintf_s(chTemp, sizeof(chTemp),"Coding %s is considerd for signal", strName.c_str());

                    AddError(eWarning, chError, chTemp);
                }
            }
        }
    }

    return EC_SUCCESS;
}

ERRORCODE CLDFParserHelper::AddNodeConfiguration(std::string& strConfigName, std::list<ldfNodeComposition> ouNodeConfigurationList)
{
    RETURN_IF_ERROR;
    //Gel All Logical Nodes.
    std::map<std::string, std::string> strLogicalNodeName;
    std::map<UID_ELEMENT, IElement*> pouCompositeList;
    std::list<CompositeNode> ouCompositeNodeList;

    m_pouLDFCluster->GetElementList(eLinNodeComposition, pouCompositeList);
for ( auto itr : pouCompositeList )
    {
        ((INodeConfiguration*)(itr.second))->GetCompositeNodes(ouCompositeNodeList);
for ( auto itrNode : ouCompositeNodeList )
        {
for ( auto itrLogicNode : itrNode.m_strLogicalNodes )
            {
                strLogicalNodeName[itrLogicNode] = itrLogicNode;
            }
        }
    }


    INodeConfiguration* pNodeConfig;
    m_pouLDFCluster->CreateElement(eLinNodeComposition, (IElement**)&pNodeConfig);


    CompositeNode ouNodeComposition;
    std::list<CompositeNode> ouNodeCompositionConfiguration;
    pNodeConfig->SetName(strConfigName);


for ( auto itr : ouNodeConfigurationList )
    {
        auto itrEcuId =  m_mapEcuUID.find(itr.m_strEcuName);
        if ( m_mapEcuUID.end() != itrEcuId )
        {
            ouNodeComposition.m_strLogicalNodes.clear();
            ouNodeComposition.m_uidNode = itrEcuId->second;
for ( auto itrLogicalNodes : itr.m_strLogicalNodeList )
            {
                if ( strLogicalNodeName.find(itrLogicalNodes) == strLogicalNodeName.end() )
                {
                    ouNodeComposition.m_strLogicalNodes.push_back(itrLogicalNodes);
                    strLogicalNodeName[itrLogicalNodes] = itrLogicalNodes;
                }
                else
                {
                    //TODO::Warning
                }
            }
            ouNodeCompositionConfiguration.push_back(ouNodeComposition);
        }
        else
        {
            return EC_FAILURE;
            //TODO: Validation
        }
    }
    pNodeConfig->SetCompositeNodes(ouNodeCompositionConfiguration);

    return EC_NA;
}

ERRORCODE CLDFParserHelper::AddSignalGroup(std::string& omSigGrpName, unsigned int nGroupSize, std::map<std::string, unsigned int>& strSignalOffset)
{
    RETURN_IF_ERROR;

    char chError[1024] = "";
    if ( m_mapSignalUID.find(omSigGrpName) != m_mapSignalUID.end())
    {
        sprintf_s(chError, sizeof(chError),"Duplicate Signal Group %s was found in \"Signal groups\" section", omSigGrpName.c_str());
        AddError(eError, chError, "File can not be loaded");
        return EC_FAILURE;
    }


    ISignalGroup* pSignalGroup = nullptr;
    UID_ELEMENT ouUid = INVALID_UID_ELEMENT;
    SignalInstanse ouInstance;
    m_pouLDFCluster->CreateElement(eSignalGroupElement, (IElement**)&pSignalGroup);
    if ( nullptr != pSignalGroup  )
    {
for ( auto itrSignalOffset : strSignalOffset )
        {
            auto itrSignalUid = m_mapSignalUID.find( itrSignalOffset.first );
            if ( m_mapSignalUID.end() != itrSignalUid)
            {
                ouUid = (*itrSignalUid).second;
                ouInstance.m_nStartBit = itrSignalOffset.second;

                if(nullptr != pSignalGroup)
                {
                    pSignalGroup->MapSignal( ouUid, ouInstance);
                }
                else
                {
                    sprintf_s(chError, sizeof(chError),"Signal Name %s was Not found in \"%s\" ", omSigGrpName.c_str());
                    AddError(eWarning, chError, "Signal will be ignored and will not be saved once the file saved.");
                }

            }
        }
    }
    SignalGroupProps ouSignalGroupProps;
    ouSignalGroupProps.eType = eLINProtocol;
    ouSignalGroupProps.m_ouLinSignalGroupProps.m_nGroupSize = nGroupSize;
    pSignalGroup->SetName(omSigGrpName);
    pSignalGroup->SetProperties(ouSignalGroupProps);
    return EC_SUCCESS;
}

ERRORCODE CLDFParserHelper::AddDynamicFrames(std::list<unsigned int>& listDynamicFrames)
{
    RETURN_IF_ERROR;

    char chError[1024] = "";
    if ( bAreEqual( m_ouProtocolSettings.m_dProtocolVers, defVersion_2_0 ) == false )
    {
        sprintf_s(chError, sizeof(chError),"Dynamic Frames are invalid in this version");
        AddError(eWarning, chError, "Section will be ignore on saving the file");
    }


    m_pouLDFCluster->SetProperties(eLdfDyanmicFrameList, &listDynamicFrames);
    return EC_NA;
}



ERRORCODE CLDFParserHelper::AddParsingResult(ParsingResults& ouParsingResult)
{
    RETURN_IF_ERROR;
    if ( ouParsingResult.m_ouErrorType == eError )
    {
        m_ouErrors.push_back(ouParsingResult);
        m_bParseError = true;
    }
    else if ( ouParsingResult.m_ouErrorType == eWarning )
    {
        m_ouWarnings.push_back(ouParsingResult);
    }
    return 0;
}

void CLDFParserHelper::AddError(eErrorSeverity ouSeverity, std::string strDesc, std::string strActionTaken)
{
    ParsingResults ouParsingResults;
    ouParsingResults.m_ouErrorType = ouSeverity;
    ouParsingResults.m_strErrorDesc = strDesc;
    ouParsingResults.m_unErrorCode = 0;
    ouParsingResults.m_unLineNum = 0;
    ouParsingResults.m_strActionTaken = strActionTaken;
    AddParsingResult(ouParsingResults);
}




bool CLDFParserHelper::bAreEqual(double a, double b)
{
    return fabs(a - b) < std::numeric_limits<double>::epsilon();
}