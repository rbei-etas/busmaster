#pragma once
#include "stdafx.h"
#include "CommonLDFGenerator.h"
#include "ILDFGenerator.h"

class CLDFGenerator2_1 : public CCommonLDFGenerator,  public ILDFGenerator
{
public:
    CLDFGenerator2_1(void);
    ~CLDFGenerator2_1(void);

    virtual ERRORCODE nSaveLDFDetails(LDFCluster* ouLdfCluster, std::string strLDFFilePath);

private:

    ERRORCODE nGenerateChannelName();
};

