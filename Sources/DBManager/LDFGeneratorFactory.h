#pragma once
#include "LDFGenerator1_3.h"
#include "LDFGenerator2_0.h"
#include "LDFGenerator2_1.h"
#include "CommonLDFGenerator.h"

class LDFGeneratorFactory
{
public:
    LDFGeneratorFactory(void);
    ~LDFGeneratorFactory(void);

    int nSaveLDFDetails(LDFCluster ouLdfCluster, std::string strLDFFilePath);
};

