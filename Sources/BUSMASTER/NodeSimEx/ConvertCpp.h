#pragma once
#include "NodeSimEx_stdafx.h"
#include <regex>
#include <istream>
#include <string>
#include <fstream>
#include "HashDefines.h"
#include "GlobalObj.h"
using namespace std;

class CConvertCpp
{
public:
    bool convertCppFile(std::string strFileName, CString omBusName, ETYPE_BUS eBus);
    bool bCheckIfOldCppFile(CString& omText);
    void vReplaceStructVars(std::string& readLine);
    void vReplaceDataBytes(std::regex rgx, std::string omData, std::string omReplaceStr, std::string& readLine);
    CString omStrExtractFunctionNameFromPrototype(CString omStrPrototype);
    bool CConvertCpp::bCreateCppBackupFile(std::string omFilePath, ETYPE_BUS eBus, std::string&);
    bool bCheckDllVersion(std::string omDllFilePath);
};