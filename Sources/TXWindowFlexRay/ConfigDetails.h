#pragma once

#include "HashDefines.h"
#include "Utility\ComputeCheckSum.h"
#include "ProjectConfiguration\ProjectConfiguration_extern.h"

class CConfigDetails : public CComputeCheckSum
{
private:
    CString m_omStrCurrFileName;
    std::string m_strCurrProjName;
    static CConfigDetails m_ouConfigDetails;
    CConfigDetails(void);
public:
    static CConfigDetails& ouGetConfigDetailsObject();
    ~CConfigDetails(void);

    int SetConfigDatastorage(DATASTORAGEINFO* StorageInfo);
    int GetConfigDatastorage(DATASTORAGEINFO* StorageInfo);

    void vReadConfigFile();
    void vSaveConfigFile();
    void vCloseConfigFile();

    void vSetCurrProjName(std::string strCurrProjName);
    std::string GetCurrProjName();
    BOOL bSetCurrProjInfo(PROJECTDATA* ProjData);
    BOOL bGetCurrProjInfo(PROJECTDATA* ProjData);

    BOOL bSetData(LPVOID lpVoid, int nStreamLength, std::string strSectionName);
    BOOL bGetData(void*& lpData,int& nStreamLength, std::string strSectionName);
};
