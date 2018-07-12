#pragma once
#include "ICluster.h"
#include "INsCodeGenHelper.h"
#include <fstream>
class IBMNetWorkGetService;
class NodeSimCodeGenerator
{
public:
    NodeSimCodeGenerator();
    ~NodeSimCodeGenerator(void);

    ERRORCODE GenerateCode(std::string pchHeaderFilePath, std::string pchCppFilePath, ICluster* pCluster, INsCodeGenHelper* codeGenHelper);
    ERRORCODE CompileCode(ICluster*, bool bRegenerateCode, bool bReCompile, INsCodeGenHelper* codeGenHelper);
    ERRORCODE GenerateMakeFile( IBMNetWorkGetService* pBmNetWork, ETYPE_BUS eBus, std::string strForCppFile);
private:
    ERRORCODE GenerateHeaderFile(std::fstream&, class INsCodeGenHelper*, ICluster*);
    ERRORCODE GenerateCppFile(std::fstream&, class INsCodeGenHelper*, ICluster*);

    std::string GetClusterName(ICluster* pCluster);
    std::string GetProtocolLib(ETYPE_BUS);
    std::string AddDoubleQuotes(std::string strTarget);
    std::string GetUniqueFrameName(IFrame* pouName);
    void GenerateFrameHeader(std::fstream& omHeaderFileStream, std::fstream& omCppFileStream, IFrame* pouFrame, bool bDecName );
    void GenerateSignalConstuctor( std::ostringstream& omStringCppStream, std::string strStartBit, ISignal* pouSignal, eEndianess ouEndianness, std::string strContainerName);
    void GenerateSignal(std::ostringstream& omHeaderStringStream, std::ostringstream& omCodeStream, ISignal* pouSignal, std::string strContainerName);
    void GeneratePduHeader(std::fstream& omHeaderFileStream, std::fstream& omCppFileStream, IPdu* pouPdu, int nStartBit = 0 );
    void GetSignalType(ISignal* pouSignal, std::string& strSignalType);


    bool bIsDependentFileModified(ICluster*);
    ERRORCODE GenerateObjectFile(std::string strCppFilePath, ICluster* pCluster);
};

