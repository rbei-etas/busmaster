#pragma once
#include<string.h>

class IFileLineAnalysisFilter
{
public:
    virtual HRESULT AnalyseLine(std::string strLine,const unsigned long& nLineNo)=0;
};

