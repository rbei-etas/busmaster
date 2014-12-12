#include "stdafx.h"
#include "LDFGeneratorFactory.h"


LDFGeneratorFactory::LDFGeneratorFactory(void)
{
}


LDFGeneratorFactory::~LDFGeneratorFactory(void)
{
}

int LDFGeneratorFactory::nSaveLDFDetails(LDFCluster ouLdfCluster, std::string strLDFFilePath)
{
    /*ILDFGenerator *pLDFGenerator = NULL;
    LIN_Settings ouSettings;
    ouLdfCluster.GetClusterProperties(&ouSettings);

    if(ouSettings.m_dProtocolVers == 13)
    {
        pLDFGenerator = (ILDFGenerator *)new CLDFGenerator1_3();
    }
    else if(ouSettings.m_nProtocolVersion == 20)
    {
        pLDFGenerator = (ILDFGenerator *)new CLDFGenerator2_0();
    }
    else if(ouSettings.m_nProtocolVersion == 21)
    {
        pLDFGenerator = (ILDFGenerator *)new CLDFGenerator2_1();
    }

    pLDFGenerator->nSaveLDFDetails(ouLdfCluster, strLDFFilePath);*/

    return eSUCCESS;
}