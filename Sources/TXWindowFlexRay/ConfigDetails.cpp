#pragma once
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
    /*UCHAR pCheckSum,fChecksum;
    if (bGetCheckSum(m_omStrCurrFileName ,& pCheckSum, &fChecksum))
    {
        if (pCheckSum == fChecksum)
        {
            if (DoDatastorageOperation(OPEN) == 0)
            {
                PROJECTDATA sTempData;
                GetProjectData(m_strCurrProjName, sTempData);
                if (sTempData.m_dwAppUniqueId != FRAME_UNIQUE_ID)
                {
                    AfxMessageBox ("The Configuration file is not Compatible",
                                                             MB_OK | MB_ICONERROR);
                    DoDatastorageOperation(CLOSE);
                }
            }
            else
            {
                AfxMessageBox ("The Configuration file is not Valid",
                                                           MB_OK | MB_ICONERROR);
                DoDatastorageOperation(CLOSE);
            }
        }
        else
        {
            AfxMessageBox ("Configuration file changed outside the Application",
                MB_OK | MB_ICONERROR);
        }
    }*/
}

void CConfigDetails::vSaveConfigFile()
{
    /*UCHAR pCheckSum;
    DoDatastorageOperation(SAVE);
    bSetCheckSum(m_omStrCurrFileName , &pCheckSum);*/
}

void CConfigDetails::vCloseConfigFile()
{
    /*DoDatastorageOperation(CLOSE);
    m_omStrCurrFileName.Empty();*/
}

int CConfigDetails::SetConfigDatastorage(DATASTORAGEINFO* StorageInfo)
{
    //m_omStrCurrFileName = StorageInfo->FSInfo->m_FilePath;
    //return SetDatastorageConfig(StorageInfo);
    return true;
}

int CConfigDetails::GetConfigDatastorage(DATASTORAGEINFO* StorageInfo)
{
    //return GetDatastorageConfig(StorageInfo);
    return true;
}

void CConfigDetails::vSetCurrProjName(string strCurrProjName)
{
    m_strCurrProjName = strCurrProjName;
}


string CConfigDetails::GetCurrProjName()
{
    return m_strCurrProjName;
}


BOOL CConfigDetails::bSetCurrProjInfo(PROJECTDATA* ProjData)
{
    //ProjData->m_ProjectName = m_strCurrProjName;
    //return AddModifyProjectTable(m_strCurrProjName , ProjData);
    return TRUE;
}

BOOL CConfigDetails::bGetCurrProjInfo(PROJECTDATA* ProjData)
{
    //return GetProjectData(m_strCurrProjName, *ProjData);
    return TRUE;
}

BOOL CConfigDetails::bSetData(LPVOID lpVoid, int nStreamLength, string strSectionName)
{
    BOOL bRetVal = TRUE;

    // check input param
    //if(lpVoid != NULL)
    //{
    //  //if(m_bConfigInfoLoaded == TRUE)
    //  //{

    //  SECTIONDATA tempSecData;
    //  tempSecData.m_omSectionName = strSectionName;
    //  tempSecData.m_nBLOBLen = nStreamLength;
    //  tempSecData.m_bBLOB = new BYTE[nStreamLength];
    //  memcpy(tempSecData.m_bBLOB,lpVoid, nStreamLength);
    //  AddModifySectionData(m_strCurrProjName,
    //      tempSecData.m_omSectionName, &tempSecData);
    //  //delete []tempSecData.m_bBLOB;

    //  //}
    //}
    return bRetVal;
}

BOOL CConfigDetails::bGetData(void*& lpData, int& nStreamLength, string strSectionName)
{
    BOOL bRetVal = FALSE;


    // is the configuration loaded?
    // if((m_bConfigInfoLoaded == TRUE) && (bRetVal == TRUE))
    /*{
        SECTIONDATA tempSecData;
        tempSecData.m_omSectionName = strSectionName;
        bRetVal = GetSectionData(m_strCurrProjName,
            tempSecData.m_omSectionName, tempSecData);
        if (bRetVal != FALSE)
        {
            bRetVal = FALSE;
            BYTE* pbNewDat = new BYTE[tempSecData.m_nBLOBLen];
            if (pbNewDat != NULL)
            {
                memcpy(pbNewDat, tempSecData.m_bBLOB, tempSecData.m_nBLOBLen);
                nStreamLength = tempSecData.m_nBLOBLen;
                lpData = (void *) pbNewDat;
                bRetVal = TRUE;
            }
            else
            {
            }
        }

    }*/
    return bRetVal;
}

