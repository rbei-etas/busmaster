#pragma once
#include "Defines.h"
#include "LDFTagDefines.h"
#include <string>
#include <fstream>
#include "LDFCluster.h"
#include <iostream>
#include <sstream>      // std::ostringstream
using namespace std;



class CCommonLDFGenerator
{
    char m_chValues[MAX_SUPPORTED_STRING];
public:
    CCommonLDFGenerator();
    ~CCommonLDFGenerator();

protected:
    ERRORCODE nCreateLDFFile(string strFilePath);
    ERRORCODE nGenerateHeader();
    ERRORCODE nGenerateGlobalDefinitions();
    ERRORCODE nGenerateNodes();
    ERRORCODE nGenerateSignals();
    ERRORCODE nGenerateFrames();
    ERRORCODE nGenerateSporadicFrames();
    ERRORCODE nGenerateNodeAttributes();
    ERRORCODE nGenerateScheduleTables();
    ERRORCODE nGenerateSignalGroups();
    ERRORCODE nGenerateSignalEncodings();
    ERRORCODE nGenerateSignalRepresentations();
    ERRORCODE nGenerateNodeCompositions();
    ERRORCODE nCloseLDFFile();

    LDFCluster* m_ouCluster;

public:
    std::string m_omFilePath;
    fstream m_omFileStream;

private:
    ERRORCODE nGetLDFVersion(double nVersion, string& strVersion);

    // Node Generation
    ERRORCODE nGetNodeAttributes(EcuProperties ouLINProperties, std::string strLDFVersion, ostringstream& omStringStream
                                 , ostringstream& omConfigFrames);

    ERRORCODE nGetFaultSignals(EcuProperties, std::string&);
    ERRORCODE nGetConfigurableFrames(ostringstream& omConfigFrames, std::string strLDFVersion, std::list<ConfigFrameDetails> listFrames);


    // Schedule Table Generation
    ERRORCODE nGenerateScheduleItem(CSheduleTableItem omShceduleItem, ostringstream& omSchedItemStream);
    ERRORCODE nGetFramePIDs(CSheduleTableItem, std::ostringstream& omPIDs);
};