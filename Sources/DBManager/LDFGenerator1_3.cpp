#include "stdafx.h"
#include "LDFGenerator1_3.h"

CLDFGenerator1_3::CLDFGenerator1_3()
{
}

CLDFGenerator1_3::~CLDFGenerator1_3()
{
}

ERRORCODE CLDFGenerator1_3::nSaveLDFDetails(LDFCluster* ouLdfCluster, std::string strLDFFilePath)
{
    m_ouCluster = ouLdfCluster;

    // Step 1 - file creation
    nCreateLDFFile(strLDFFilePath);

    // Step 2 - Generate header
    nGenerateHeader();

    // Step 3 - Generate global definitions
    nGenerateGlobalDefinitions();

    // Step 4 - Generate Node details
    nGenerateNodes();

    nGenerateDiagAddress();

    // Step 5 - Generate Signal details
    nGenerateSignals();

    nGenerateFrames();

    nGenerateScheduleTables();

    nGenerateSignalGroups();
    nGenerateSignalEncodings();

    nGenerateSignalRepresentations();

    nCloseLDFFile();

    return EC_SUCCESS;
}

ERRORCODE CLDFGenerator1_3::nGenerateDiagAddress()
{
    std::ostringstream omStringStream;
    std::map<UID_ELEMENT, IElement*> ecuMap;
    EcuProperties pEcuProps;
    ostringstream omSlaveDetails, omDiagAddress;

    // Get the list of ECU's
    if(m_ouCluster->GetElementList(eEcuElement, ecuMap) == EC_FAILURE)
    {
        return EC_FAILURE;
    }

    std::map<UID_ELEMENT, IElement*>::iterator itrNode = ecuMap.begin();

    // Check if atleast one ECU available
    if(ecuMap.size() > 0)
    {
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
                pEcu->GetProperties(pEcuProps);
            }

            if(ecuType == eLIN_Slave)
            {
                if(pEcuProps.m_ouSlavePros.m_nConfiguredNAD != INVALID_DATA)
                {
                    omSlaveDetails << defTAB << strNodeName.c_str() << defCOLON << pEcuProps.m_ouSlavePros.m_nConfiguredNAD << defSEMICOLON;
                }
            }
            itrNode++;
        }

        if( false == omSlaveDetails.str().empty() )
        {
            omDiagAddress << defDIAG_ADDRESSES << defOPEN_BRACE << endl << omSlaveDetails.str() << defCLOSE_BRACE << endl;
        }

        m_omFileStream << omDiagAddress.str();
    }

    return EC_SUCCESS;
}
