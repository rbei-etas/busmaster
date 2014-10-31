#include "stdafx.h"
#include "LDFGenerator2_1.h"


CLDFGenerator2_1::CLDFGenerator2_1(void)
{
}


CLDFGenerator2_1::~CLDFGenerator2_1(void)
{
}

ERRORCODE CLDFGenerator2_1::nSaveLDFDetails(LDFCluster* ouLdfCluster, std::string strLDFFilePath)
{
    m_ouCluster = ouLdfCluster;

    // Step 1 - file creation
    nCreateLDFFile(strLDFFilePath);

    // Step 2 - Generate header
    nGenerateHeader();

    // Step 3 - Generate global definitions
    nGenerateGlobalDefinitions();
    nGenerateChannelName();

    // Step 4 - Generate Node details
    nGenerateNodes();

    nGenerateNodeCompositions();
    // Step 5 - Generate Signal details
    nGenerateSignals();

    // Step 6 - Generate Frames
    nGenerateFrames();

    // Step 7 - Generate Node Attributes
    nGenerateNodeAttributes();

    nGenerateScheduleTables();
    nGenerateSignalGroups();

    nGenerateSignalEncodings();

    nGenerateSignalRepresentations();

    nCloseLDFFile();
    return EC_SUCCESS;
}

ERRORCODE CLDFGenerator2_1::nGenerateChannelName()
{
    std::ostringstream omStringStream;
    LIN_Settings ouSettings;
    m_ouCluster->GetProperties(eLINClusterProperties, &ouSettings);

    if( false == ouSettings.m_strChannelName.empty())
    {
        omStringStream << defLIN_GN_CHANNEL_NAME << defDOUBLEQUOTE << ouSettings.m_strChannelName << defDOUBLEQUOTE << defSEMICOLON;
        m_omFileStream << omStringStream.str();
    }

    return EC_SUCCESS;
}