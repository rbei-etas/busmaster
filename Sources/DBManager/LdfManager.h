#pragma once
#include "LDFCluster.h"
#include "IClusterProps.h"

class ILdfManager
{
public:
    virtual ERRORCODE nSaveLDFToFile(LDFCluster* pouLdfCluster, std::string strLDFFilePath) ;
    virtual ERRORCODE nLoadLDFFromFile(LDFCluster* pouLdfCluster, std::string strLDFFilePath, std::list<ParsingResults>& ouErrors, std::list<ParsingResults>& ouWarnings);
};

