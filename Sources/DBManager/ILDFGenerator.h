#pragma once
#include "LDFCluster.h"

class ILDFGenerator
{
public:
    ERRORCODE virtual nSaveLDFDetails(LDFCluster* ouLdfCluster, std::string strLDFFilePath) = 0;
    ILDFGenerator(void);
    virtual ~ILDFGenerator(void);
};

