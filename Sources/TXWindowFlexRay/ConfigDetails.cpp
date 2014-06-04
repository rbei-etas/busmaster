#include "TxWindowFlexRay_stdafx.h"
#include "HashDefines.h"
#include "ConfigDetails.h"

CConfigDetails CConfigDetails::m_ouConfigDetails;

CConfigDetails::CConfigDetails(void)
{
    m_strCurrProjName = DEFAULT_PROJECT_NAME;
    m_omStrCurrFileName = "";
}

CConfigDetails::~CConfigDetails(void)
{
}

CConfigDetails& CConfigDetails::ouGetConfigDetailsObject()
{
    return m_ouConfigDetails;
}

void CConfigDetails::vReadConfigFile()
{
}

void CConfigDetails::vSaveConfigFile()
{
}

void CConfigDetails::vCloseConfigFile()
{
}

int CConfigDetails::SetConfigDatastorage(DATASTORAGEINFO * /* StorageInfo */)
{
    return true;
}

int CConfigDetails::GetConfigDatastorage(DATASTORAGEINFO * /* StorageInfo */)
{
    return true;
}

void CConfigDetails::vSetCurrProjName(std::string strCurrProjName)
{
    m_strCurrProjName = strCurrProjName;
}


std::string CConfigDetails::GetCurrProjName()
{
    return m_strCurrProjName;
}


BOOL CConfigDetails::bSetCurrProjInfo(PROJECTDATA * /* ProjData */)
{
    return TRUE;
}

BOOL CConfigDetails::bGetCurrProjInfo(PROJECTDATA * /* ProjData */)
{
    return TRUE;
}

BOOL CConfigDetails::bSetData(LPVOID /* lpVoid */, int /* nStreamLength */, std::string /* strSectionName */)
{
    return TRUE;
}

BOOL CConfigDetails::bGetData(void * & /* lpData */, int & /* nStreamLength */, std::string /* strSectionName */)
{
    return FALSE;
}

