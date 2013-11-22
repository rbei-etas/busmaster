#pragma once
//#pragma pack(8)
//#pragma pack(show)

#include "HashDefines.h"
#include "Utility\ComputeCheckSum.h"
#include "ProjectConfiguration\ProjectConfiguration_extern.h"


class CConfigDetails : public CComputeCheckSum
{
private:
    CString m_omStrCurrFileName;
    string m_strCurrProjName;
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

    void vSetCurrProjName(string strCurrProjName);
    string GetCurrProjName();
    BOOL bSetCurrProjInfo(PROJECTDATA* ProjData);
    BOOL bGetCurrProjInfo(PROJECTDATA* ProjData);

    BOOL bSetData(LPVOID lpVoid, int nStreamLength, string strSectionName);
    BOOL bGetData(void*& lpData,int& nStreamLength, string strSectionName);
};
