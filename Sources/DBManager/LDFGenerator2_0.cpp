#include "stdafx.h"
#include "LDFGenerator2_0.h"


CLDFGenerator2_0::CLDFGenerator2_0(void)
{
}


CLDFGenerator2_0::~CLDFGenerator2_0(void)
{
}

ERRORCODE CLDFGenerator2_0::nSaveLDFDetails(LDFCluster* ouLdfCluster, std::string strLDFFilePath)
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

    nGenerateNodeCompositions();
    // Step 5 - Generate Signal details
    nGenerateSignals();

    nGenerateDynamicFrames();

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

ERRORCODE CLDFGenerator2_0::nGenerateDynamicFrames()
{
    std::ostringstream omFrames, omDynamicFrames;
    if(m_ouCluster->m_ouDyanamicFramelist.size() > 0)
    {
        auto itrDynFrames = m_ouCluster->m_ouDyanamicFramelist.begin();

        omFrames << *itrDynFrames;
        itrDynFrames++;

        while(itrDynFrames != m_ouCluster->m_ouDyanamicFramelist.end())
        {
            omFrames << defCOMMA << *itrDynFrames;
            itrDynFrames++;
        }

        if(false == omFrames.str().empty())
        {
            omDynamicFrames << defDYNAMIC_FRAMES << defOPEN_BRACE << endl << defTAB << omFrames.str() << endl << defCLOSE_BRACE << endl << endl;
            m_omFileStream << omDynamicFrames.str();
        }
    }

    return EC_SUCCESS;
}