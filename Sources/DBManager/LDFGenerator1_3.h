#pragma once
#include "stdafx.h"
#include "CommonLDFGenerator.h"
#include "ILDFGenerator.h"

class CLDFGenerator1_3 : public CCommonLDFGenerator, public ILDFGenerator
{
public:
    CLDFGenerator1_3(void);
    ~CLDFGenerator1_3(void);

public:
    ERRORCODE nSaveLDFDetails(LDFCluster* ouLdfCluster, std::string strLDFFilePath);
private:
    ERRORCODE nGenerateDiagAddress();
};

