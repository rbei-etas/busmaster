#pragma once
#include "stdafx.h"
#include "CommonLDFGenerator.h"
#include "ILDFGenerator.h"
class CLDFGenerator2_0 : public CCommonLDFGenerator,  public ILDFGenerator
{
public:
    CLDFGenerator2_0(void);
    ~CLDFGenerator2_0(void);

    virtual ERRORCODE nSaveLDFDetails(LDFCluster* ouLdfCluster, std::string strLDFFilePath);

private:
    ERRORCODE nGenerateDynamicFrames();
};

