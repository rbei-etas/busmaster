#include "stdafx.h"
#include "CommonLDFGenerator.h"
#include "FrameGenerator.h"
#include "SignalGenerator.h"
#include <algorithm>
#include <string>
using namespace std;
// Common LDFGenerator
CCommonLDFGenerator::CCommonLDFGenerator(void)
{
    m_ouCluster = nullptr;
}

CCommonLDFGenerator::~CCommonLDFGenerator(void)
{
}

ERRORCODE CCommonLDFGenerator::nCreateLDFFile(string strFilePath)
{
    m_omFilePath = strFilePath;
    m_omFileStream.open(strFilePath, fstream::out|fstream::binary);

    if(m_omFileStream.is_open() == false)
    {
        return EC_FAILURE;
    }

    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nCloseLDFFile()
{
    m_omFileStream.close();

    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGenerateHeader()
{
    LIN_Settings ouSettings;
    m_ouCluster->GetProperties(eLINClusterProperties, &ouSettings);

    //Delete \r in comment since fstream << will add extra line character while printing.
    //ouSettings.m_strHeaderComment.erase(std::remove(ouSettings.m_strHeaderComment.begin(), ouSettings.m_strHeaderComment.end(), '\r'), ouSettings.m_strHeaderComment.end());
    //m_omFileStream.write(ouSettings.m_strHeaderComment.c_str(), ouSettings.m_strHeaderComment.size());
    m_omFileStream<<ouSettings.m_strHeaderComment<<endl;
    return EC_SUCCESS;
}
ERRORCODE CCommonLDFGenerator::nGenerateGlobalDefinitions()
{
    std::ostringstream omStringStream;
    omStringStream << defLIN_GN_DESC_FILE << endl;

    LIN_Settings ouSettings;
    m_ouCluster->GetProperties(eLINClusterProperties, &ouSettings);

    string strVersion;
    if(nGetLDFVersion(ouSettings.m_dProtocolVers, strVersion) != EC_SUCCESS)
    {
        return EC_FAILURE;
    }

    omStringStream << defLIN_GN_PROTOCOL_VERSION << defDOUBLE_QUOTE << strVersion.c_str() << defDOUBLE_QUOTE << defSEMICOLON;
    omStringStream << defLIN_GN_LANGUAGE_VERSION << defDOUBLE_QUOTE << strVersion.c_str() << defDOUBLE_QUOTE << defSEMICOLON;
    omStringStream << defLIN_GN_SPEED << ouSettings.m_dBaudRate << defUNIT_KBPS << defSEMICOLON;
    m_omFileStream << omStringStream.str();

    return 0;
}

ERRORCODE CCommonLDFGenerator::nGenerateNodes()
{
    std::ostringstream omStringStream;
    std::map<UID_ELEMENT, IElement*> ecuMap;
    std::list<std::string> lstSlaves;
    std::string strSlaves;

    // Get the list of ECU's
    if(m_ouCluster->GetElementList(eEcuElement, ecuMap) == EC_FAILURE)
    {
        return EC_FAILURE;
    }

    std::map<UID_ELEMENT, IElement*>::iterator itrNode = ecuMap.begin();

    // Check if atleast one ECU available
    if(ecuMap.size() > 0)
    {
        omStringStream << defNEWLINE << defLIN_GN_NODES << defOPEN_BRACE ;

        while(itrNode != ecuMap.end())
        {
            // Get the ECU
            CLINEcu* pEcu = (CLINEcu*)itrNode->second;

            std::string strNodeName;
            eEcuType ecuType = eNone;

            if(nullptr != pEcu)
            {
                // Get ECU Properties
                pEcu->GetName(strNodeName);
                pEcu->GetEcuType(ecuType);
            }

            if(ecuType == eLIN_Master)
            {
                EcuProperties ouLINProperties;
                if(nullptr != pEcu)
                {
                    pEcu->GetProperties((EcuProperties&)ouLINProperties);
                }

                double fJitter = ouLINProperties.m_ouMasterProps.m_fJitter;
                double fTimeBase = ouLINProperties.m_ouMasterProps.m_fTimeBase;

                omStringStream << defMASTER << strNodeName.c_str() << defCOMMA
                               << fTimeBase << defUNIT_MS << defCOMMA << fJitter << defUNIT_MS << defSEMICOLON;
            }
            else if(ecuType == eLIN_Slave)
            {
                lstSlaves.push_back(strNodeName.c_str());
            }
            itrNode++;
        }

        std::list<string>::iterator itrSlave = lstSlaves.begin();
        while(itrSlave != lstSlaves.end())
        {
            strSlaves += *itrSlave ;
            itrSlave++;
            if(itrSlave != lstSlaves.end())
            {
                strSlaves += defCOMMA;
            }
        }

        omStringStream << defSLAVES << strSlaves.c_str() << defSEMICOLON;
        omStringStream << defCLOSE_BRACE << endl << endl;
        m_omFileStream << omStringStream.str();
    }
    else
    {
        omStringStream << defNEWLINE << defLIN_GN_NODES << defOPEN_BRACE << endl << defCLOSE_BRACE;
    }

    return 0;
}

ERRORCODE CCommonLDFGenerator::nGenerateSignals()
{
    ostringstream omFrameStream;
    SignalGenerator omSignalGenerator;
    omSignalGenerator.nGenerateSignals(m_ouCluster, omFrameStream);

    if(omFrameStream.str().empty() == false)
    {
        m_omFileStream << omFrameStream.str();
    }

    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGenerateSignalEncodings()
{
    ostringstream omEncodingStream;
    SignalGenerator omSignalGenerator;
    omSignalGenerator.nGenerateSignalEncodingTypes(m_ouCluster, omEncodingStream);

    if(omEncodingStream.str().empty() == false)
    {
        m_omFileStream << omEncodingStream.str();
    }
    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGenerateSignalRepresentations()
{
    ostringstream omEncodingStream;
    SignalGenerator omSignalGenerator;
    omSignalGenerator.nGenerateSignalRepresentation(m_ouCluster, omEncodingStream);

    if(omEncodingStream.str().empty() == false)
    {
        m_omFileStream << omEncodingStream.str();
    }
    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGenerateFrames()
{
    ostringstream omFrameStream;
    CFrameGenerator omFrameGenerator;
    omFrameGenerator.nGenerateFrames(m_ouCluster, omFrameStream);

    if(omFrameStream.str().empty() == false)
    {
        m_omFileStream << omFrameStream.str();
    }

    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGenerateNodeAttributes()
{
    ostringstream omStringStream;
    std::map<UID_ELEMENT, IElement*> ecuMap;
    std::string strLDFVersion, strVersion, strFaultStSignals = "";

    LIN_Settings ouSettings;
    m_ouCluster->GetProperties(eLINClusterProperties, &ouSettings);
    if(nGetLDFVersion(ouSettings.m_dProtocolVers, strLDFVersion) != EC_SUCCESS)
    {
        return EC_FAILURE;
    }

    // Get the list of ECU's
    if(m_ouCluster->GetElementList(eEcuElement, ecuMap) == EC_FAILURE)
    {
        return EC_FAILURE;
    }

    std::map<UID_ELEMENT, IElement*>::iterator itrNode = ecuMap.begin();

    // Check if atleast one ECU available
    if(ecuMap.size() > 0)
    {
        omStringStream << defNODE_ATTRIBUTES << defOPEN_BRACE << endl;

        while(itrNode != ecuMap.end())
        {
            // Get the ECU
            CLINEcu* pEcu = (CLINEcu*)itrNode->second;

            std::string strNodeName;
            eEcuType ecuType = eNone;
            EcuProperties ouLINProperties;

            if(nullptr != pEcu)
            {
                // Get ECU Properties
                pEcu->GetName(strNodeName);
                pEcu->GetEcuType(ecuType);

                // Get ECU Properties
                pEcu->GetProperties((EcuProperties&)ouLINProperties);
            }

            if(ecuType == eLIN_Slave)
            {
                omStringStream << defTAB << strNodeName << defOPEN_BRACE << endl;
                ostringstream omConfigFrames;

                // Get Configurable Frames list
                nGetLDFVersion( ouLINProperties.m_ouSlavePros.m_fProtocolVersion, strLDFVersion);

                nGetConfigurableFrames(omConfigFrames,strLDFVersion, ouLINProperties.m_ouSlavePros.m_nConfiurableFrameIdList);

                nGetNodeAttributes(ouLINProperties, strLDFVersion, omStringStream, omConfigFrames);

                omStringStream << defTAB << defCLOSE_BRACE << endl;
            }

            itrNode++;
        }
        omStringStream << defCLOSE_BRACE << endl << endl;
        m_omFileStream << omStringStream.str();
    }
    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGenerateScheduleTables()
{
    ostringstream omStringStream;
    std::map<UID_ELEMENT, IElement*> mapSchedTable;
    std::string strSchedTable, strFrameName;

    m_ouCluster->GetElementList(eScheduleTableElement, mapSchedTable);

    if(mapSchedTable.size() > 0)
    {
        omStringStream << defSCHEDULE_TABLES << defOPEN_BRACE << endl;
        IScheduleTable* pSchedTable;
        ScheduleTableProps pSchedProps;
for(auto itrSched : mapSchedTable)
        {
            pSchedTable = (IScheduleTable*)itrSched.second;

            if(nullptr != pSchedTable)
            {
                pSchedTable->GetName(strSchedTable);
                pSchedTable->GetProperties(pSchedProps);
            }

            omStringStream << defSPACE << strSchedTable.c_str() << defOPEN_BRACE << endl;

for(auto itrSchedItem : pSchedProps.m_ouCSheduleTableItem)
            {
                nGenerateScheduleItem(itrSchedItem, omStringStream);
            }

            omStringStream << defSPACE << defCLOSE_BRACE << endl;
        }

        omStringStream << defCLOSE_BRACE << endl;

        m_omFileStream << omStringStream.str();
    }
    else
    {
        omStringStream << defSCHEDULE_TABLES << defOPEN_BRACE << endl << defCLOSE_BRACE << endl;
        m_omFileStream << omStringStream.str();
    }

    return EC_NA;
}

ERRORCODE CCommonLDFGenerator::nGenerateScheduleItem(CSheduleTableItem omShceduleItem, ostringstream& omSchedItemStream)
{
    IFrame* pFrame = nullptr;

    m_ouCluster->GetElement(eFrameElement, omShceduleItem.m_nFrameId, (IElement**)&pFrame);

    std::string strFrameName, strEcuName, strLDFVersion;
    std::ostringstream omStream, omPIDs;

    if(nullptr != pFrame)
    {
        pFrame->GetName(strFrameName);
    }
    IEcu* pECU = nullptr;

    if(omShceduleItem.m_nNode != INVALID_UID_ELEMENT)
    {
        m_ouCluster->GetElement(eEcuElement, omShceduleItem.m_nNode, (IElement**)&pECU);

        if(nullptr != pECU)
        {
            pECU->GetName(strEcuName);
        }
    }

    UID_ELEMENT idFrame;
    LIN_Settings ouSettings;
    m_ouCluster->GetProperties(eLINClusterProperties, &ouSettings);
    nGetLDFVersion(ouSettings.m_dProtocolVers, strLDFVersion);

    switch (omShceduleItem.m_eDiagType)
    {
        case eLIN_NORMAL_FRAME_ID:
            omSchedItemStream << defTAB << strFrameName.c_str() << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_MASTER_FRAME_ID:
            omSchedItemStream << defTAB << defMASTER_REQUEST << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_SLAVE_FRAME_ID:
            omSchedItemStream << defTAB << defSLAVE_RESP << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_SID_ASSIGN_NAD_ID:
            if(strLDFVersion == defLIN_VERSION_2_0)
            {
                omSchedItemStream << defTAB << defASSIGN_NAD << defOPEN_BRACE
                                  << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[0] << defCOMMA
                                  << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[7] << defCOMMA
                                  << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[3] << defCOMMA
                                  << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[5]
                                  <<  defCLOSE_BRACE
                                  << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            }
            else if(strLDFVersion == defLIN_VERSION_2_1)
            {
                omSchedItemStream << defTAB << defASSIGN_NAD << defOPEN_BRACE <<  strEcuName.c_str() << defCLOSE_BRACE
                                  << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            }
            break;
        case eLIN_SID_COND_CHANGE_NAD:
            omSchedItemStream << defTAB << defCONDITIONALCHANGENAD << defOPEN_BRACE
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[0] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[3] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[4] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[5] << defCOMMA
                              << defHEX << std::hex <<std::uppercase <<  (int)omShceduleItem.m_chDataBytes[6] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[7]
                              << defCLOSE_BRACE << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_SID_DATA_DUMP:
            omSchedItemStream << defTAB << defDATADUMP << defOPEN_BRACE << strEcuName.c_str() << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[3] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[4] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[5] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[6] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[7] << defCLOSE_BRACE
                              << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_SID_SAVE_CONFIG:
            omSchedItemStream << defTAB << defSAVECONFIG << defOPEN_BRACE <<  strEcuName.c_str() << defCLOSE_BRACE
                              << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_SID_ASSIGN_FRAME_RANGE:

            nGetFramePIDs(omShceduleItem, omPIDs);
            omSchedItemStream << defTAB << defASSIGN_FRAMEID_RANGE << defOPEN_BRACE << strEcuName.c_str() << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)( omShceduleItem.m_chDataBytes[3] )<<  omPIDs.str() << defCLOSE_BRACE
                              << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_SID_FREEFORMAT:
            omSchedItemStream << defTAB << defFREEFORMAT << defOPEN_BRACE
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[0] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[1] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[2] << defCOMMA
                              << defHEX << std::hex <<std::uppercase  << (int)omShceduleItem.m_chDataBytes[3]<< defCOMMA
                              << defHEX << std::hex <<std::uppercase <<  (int)omShceduleItem.m_chDataBytes[4] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[5]  << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[6] << defCOMMA
                              << defHEX << std::hex <<std::uppercase <<  (int)omShceduleItem.m_chDataBytes[7] <<  defCLOSE_BRACE
                              << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_SID_READ_BY_IDENTIFIER:
            omSchedItemStream << defTAB << defFREEFORMAT << defOPEN_BRACE << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[0]
                              << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[1] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[2] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[3] << defCOMMA
                              << defHEX << std::hex <<std::uppercase <<  (int)omShceduleItem.m_chDataBytes[4] << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[5]  << defCOMMA
                              << defHEX << std::hex <<std::uppercase << (int)omShceduleItem.m_chDataBytes[6]  << defCOMMA
                              << defHEX << std::hex <<std::uppercase <<  (int)omShceduleItem.m_chDataBytes[7] <<  defCLOSE_BRACE
                              << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_SID_ASSIGN_FRAME_ID:
            idFrame = omShceduleItem.m_chDataBytes[7];
            m_ouCluster->GetElement(eFrameElement, idFrame, (IElement**)&pFrame);

            if(nullptr != pFrame)
            {
                pFrame->GetName(strFrameName);
            }
            omSchedItemStream << defTAB << defASSIGNFRAMEID << defOPEN_BRACE << strEcuName.c_str() << defCOMMA << strFrameName.c_str() << defCLOSE_BRACE
                              << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        case eLIN_SID_UNASSIGN_FRAME_ID:
            idFrame = omShceduleItem.m_chDataBytes[7];
            m_ouCluster->GetElement(eFrameElement, idFrame, (IElement**)&pFrame);

            if(nullptr != pFrame)
            {
                pFrame->GetName(strFrameName);
            }
            omSchedItemStream << defTAB << defUNASSIGNFRAMEID << defOPEN_BRACE << strEcuName.c_str() << defCOMMA << strFrameName.c_str() << defCLOSE_BRACE
                              << defDELAY << omShceduleItem.m_dDelay << defUNIT_MS << defSEMICOLON;
            break;
        default:
            break;
    }
    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGetFramePIDs(CSheduleTableItem omSchedItem, std::ostringstream& omPIDs)
{
    unsigned int unIndex =0;

    for(unIndex = 4; unIndex < MAX_LIN_FRAME_LENGTH; unIndex++)
    {
        if(omSchedItem.m_chDataBytes[4] == 0xFF)
        {
            break;
        }
        omPIDs << defCOMMA << defHEX << std::hex <<std::uppercase<< (int)( omSchedItem.m_chDataBytes[unIndex]);
    }
    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGetLDFVersion(double fVersion, string& strVersion)
{
    if(fVersion == defVersion_1_3)
    {
        strVersion = defLIN_VERSION_1_3;
    }
    else if(fVersion == defVersion_2_0)
    {
        strVersion = defLIN_VERSION_2_0;
    }
    else if(fVersion == defVersion_2_1)
    {
        strVersion = defLIN_VERSION_2_1;
    }
    else
    {
        return EC_FAILURE;
    }

    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGetFaultSignals(EcuProperties ouLINProperties, std::string& strFaultStSignals)
{
    UID_ELEMENT unSigId;
    IElement* pSigElement = nullptr;
    std::string strSigName;
    unsigned int nCount = 0;
    strFaultStSignals.empty();
    if(ouLINProperties.m_ouSlavePros.m_nFaultStateSignals.size() > 0)
    {
for(auto itrSig : ouLINProperties.m_ouSlavePros.m_nFaultStateSignals)
        {
            unSigId = itrSig;
            m_ouCluster->GetElement(eSignalElement, unSigId, &pSigElement);

            if(nullptr != pSigElement)
            {
                pSigElement->GetName(strSigName);
            }

            strFaultStSignals += strSigName;

            nCount++;
            if(nCount < ouLINProperties.m_ouSlavePros.m_nFaultStateSignals.size())
            {
                strFaultStSignals += defCOMMA;
            }
        }
    }

    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGetNodeAttributes(EcuProperties ouLINProperties, std::string strLDFVersion, ostringstream& omStringStream
        , ostringstream& omConfigFrames)
{
    IElement* pSig = nullptr;
    std::string strVersion;
    UID_ELEMENT unRespSigId = ouLINProperties.m_ouSlavePros.m_nRespErrSignal;
    m_ouCluster->GetElement(eSignalElement, unRespSigId, &pSig);
    std::string strRespSignal, strFaultStSignals;
    ostringstream omfaultsignals, omRespError;

    nGetLDFVersion(ouLINProperties.m_ouSlavePros.m_fProtocolVersion, strVersion);

    if(nullptr != pSig)
    {
        pSig->GetName(strRespSignal);
    }

    // Get FaultSignals
    nGetFaultSignals(ouLINProperties, strFaultStSignals);
    if(strFaultStSignals.empty() == false)
    {
        omfaultsignals << defTWOTAB << defLIN_FAULT_SIGNALS << strFaultStSignals.c_str() << defSEMICOLON;
    }

    if(false == strRespSignal.empty())
    {
        omRespError << defTWOTAB << defLIN_RESPONSE_ERROR << strRespSignal.c_str() << defSEMICOLON;
    }

    if(strLDFVersion == defLIN_VERSION_1_3)
    {
        omStringStream << defTWOTAB << defLIN_PROTOCOL << defDOUBLEQUOTE << strVersion.c_str() << defDOUBLEQUOTE << defSEMICOLON
                       << defTWOTAB << defLIN_CONFIGURED_NAD << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nConfiguredNAD) << defSEMICOLON;
    }
    else if(strLDFVersion == defLIN_VERSION_2_0)
    {
        omStringStream << defTWOTAB << defLIN_PROTOCOL << defDOUBLEQUOTE << strVersion.c_str() << defDOUBLEQUOTE << defSEMICOLON
                       << defTWOTAB << defLIN_CONFIGURED_NAD << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nConfiguredNAD) << defSEMICOLON
                       << defTWOTAB << defLIN_PRODUCT_ID << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nSupplierId) << defCOMMA
                       << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nFunctionId) << defCOMMA
                       << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nVariant) << defSEMICOLON
                       << omRespError.str()
                       << defTWOTAB << defLIN_P2_MIN << ouLINProperties.m_ouSlavePros.m_dP2Min << defUNIT_MS << defSEMICOLON
                       << defTWOTAB << defLIN_ST_MIN << ouLINProperties.m_ouSlavePros.m_dSTMin << defUNIT_MS << defSEMICOLON;
    }
    else if(strLDFVersion == defLIN_VERSION_2_1)
    {
        omStringStream << defTWOTAB << defLIN_PROTOCOL << defDOUBLEQUOTE << strVersion.c_str() << defDOUBLEQUOTE << defSEMICOLON
                       << defTWOTAB << defLIN_CONFIGURED_NAD << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nConfiguredNAD) << defSEMICOLON
                       << defTWOTAB << defLIN_INITIAL_NAD << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nInitialNAD) << defSEMICOLON
                       << defTWOTAB << defLIN_PRODUCT_ID << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nSupplierId) << defCOMMA
                       << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nFunctionId) << defCOMMA
                       << defHEX << std::uppercase << std::hex << (ouLINProperties.m_ouSlavePros.m_nVariant) << defSEMICOLON
                       << omRespError.str()
                       << omfaultsignals.str() << defTWOTAB << defLIN_P2_MIN << ouLINProperties.m_ouSlavePros.m_dP2Min << defUNIT_MS << defSEMICOLON
                       << defTWOTAB << defLIN_ST_MIN << ouLINProperties.m_ouSlavePros.m_dSTMin << defUNIT_MS << defSEMICOLON
                       << defTWOTAB << defLIN_N_AS_TIMEOUT << std::dec << ouLINProperties.m_ouSlavePros.m_dNASTimeout << defUNIT_MS << defSEMICOLON
                       << defTWOTAB << defLIN_N_Cr_TIMEOUT << std::dec << ouLINProperties.m_ouSlavePros.m_dNCRTimeout << defUNIT_MS << defSEMICOLON;

    }

    if(omConfigFrames.str().empty() == false)
    {
        omStringStream << defTWOTAB << omConfigFrames.str() << endl;
    }

    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGetConfigurableFrames(ostringstream& omConfigFrames, std::string strLDFVersion ,std::list<ConfigFrameDetails> listFrames)
{
    if(listFrames.size() <= 0 && strLDFVersion == defLIN_VERSION_1_3)
    {
        return EC_FAILURE;
    }

    std::string strFrameName;
    FrameProps omFrameProps;
    IFrame* pFrame = nullptr;
    UID_ELEMENT unElmntId = INVALID_UID_ELEMENT;

    omConfigFrames << defLIN_CONFIG_FRAMES << defOPEN_BRACE << endl;
    int ninde = 0;
for(auto itrFrame : listFrames)
    {
        unElmntId = itrFrame.m_uidFrame;
        m_ouCluster->GetElement(eFrameElement, unElmntId, (IElement**)&pFrame);

        if(nullptr != pFrame)
        {
            unsigned int unFrameId = 0;
            pFrame->GetName(strFrameName);
            pFrame->GetProperties(omFrameProps);
            pFrame->GetFrameId(unFrameId);

            if(strLDFVersion == defLIN_VERSION_2_0)// slave version
            {
                omConfigFrames << defTAB << defTWOTAB << strFrameName.c_str() << defEQUAL
                               << defHEX << std::hex << itrFrame.m_unConfigMsgId << defSEMICOLON;
            }
            else if(strLDFVersion == defLIN_VERSION_2_1)
            {
                omConfigFrames << defTAB << defTWOTAB << strFrameName.c_str() << defSEMICOLON;
            }
        }
    }
    omConfigFrames << defTWOTAB << defCLOSE_BRACE;

    return EC_SUCCESS;
}

ERRORCODE CCommonLDFGenerator::nGenerateSignalGroups()
{
    ostringstream omFrameStream;
    SignalGenerator omSignalGenerator;
    omSignalGenerator.nGenerateSignalGroups(m_ouCluster, omFrameStream);
    m_omFileStream << omFrameStream.str();
    return EC_SUCCESS;
}
ERRORCODE CCommonLDFGenerator::nGenerateNodeCompositions()
{
    IEcu* pEcu = nullptr;
    std::string omEcuName;
    std::ostringstream omNodeComposition, omNodeConfiguration;
    std::map<UID_ELEMENT, IElement*> nodeCompMap;

    m_ouCluster->GetElementList(eLinNodeComposition, nodeCompMap);
    std::list<CompositeNode> lstCompNodes;
for(auto itrNodeComposition : nodeCompMap )
    {
        CLDFNodeConfiguration* ouNodeConfig = (CLDFNodeConfiguration*)itrNodeComposition.second;

        ouNodeConfig->GetCompositeNodes(lstCompNodes);
        std::ostringstream omCompositeNodes;
for(auto itrCompositeNode : lstCompNodes )
        {
            if(EC_SUCCESS == m_ouCluster->GetElement(eEcuElement, itrCompositeNode.m_uidNode, (IElement**)&pEcu) )
            {
                pEcu->GetName(omEcuName);
            }
            std::string omLogicalNodes = EMPTY_STRING;
            if(itrCompositeNode.m_strLogicalNodes.size() > 0)
            {
                auto itrLogicalNode = itrCompositeNode.m_strLogicalNodes.begin();
                omLogicalNodes = *itrLogicalNode;
                itrLogicalNode++;
                while(itrLogicalNode != itrCompositeNode.m_strLogicalNodes.end())
                {
                    omLogicalNodes += defCOMMA + *itrLogicalNode;
                    itrLogicalNode++;
                }
                omCompositeNodes << endl << defTWOTAB << omEcuName.c_str() << defOPEN_BRACE << endl << defTWOTAB << defTAB
                                 << omLogicalNodes.c_str() << endl << defTWOTAB << defCLOSE_BRACE;
            }
        }
        std::string omNodeConfigName;
        itrNodeComposition.second->GetName(omNodeConfigName);
        omNodeConfiguration << defTAB << defNODE_CONFIGURATION << omNodeConfigName.c_str()
                            << defOPEN_BRACE << omCompositeNodes.str() << endl << defTAB << defCLOSE_BRACE << endl ;
    }
    if(omNodeConfiguration.str().empty() == false)
    {
        m_omFileStream << defNODE_COMPOSITION << defOPEN_BRACE << endl << omNodeConfiguration.str() << defCLOSE_BRACE << endl;
    }
    return EC_SUCCESS;
}