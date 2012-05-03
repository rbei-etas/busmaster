/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      SimSysManager.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "NodeSimEx_stdafx.h"
#include "HashDefines.h"
#include "Include/Utils_macro.h"
#include "simsysmanager.h"
#include "GlobalObj.h"

#define SIM_WND_CONFIG_SECTION_NAME "SimWndConfigSection"
#define SIMSYS_CONFIG_SECTION_NAME      "SimSysConfig"

//INstantiation of singleton class
CSimSysManager* CSimSysManager::sm_pouSimSysManager[BUS_TOTAL] = {NULL};

CSimSysManager::CSimSysManager(ETYPE_BUS eBus) : m_ouSimSysNodeInfo(eBus),
    m_omSimSysConfig(eBus)
{
    m_pomSimSysDetView = NULL;
    m_pomSimSysTreeView = NULL;
    m_eBus = eBus;
}

CSimSysManager::~CSimSysManager(void)
{
    m_ouSimSysNodeInfo.bDeleteSimsysFromInfo(STR_EMPTY);
}

void CSimSysManager::vApplicationClosing()
{
    if (m_ouSimSysNodeInfo.bIsAnySimSysModified())
    {
        int nReturn = AfxMessageBox(ASK_SIMSYS_SAVE_PROMPT,
                                    MB_YESNO|MB_ICONQUESTION);

        if ((nReturn == IDYES) && (m_pomSimSysTreeView != NULL))
        {
            vSaveAllSimSys();
        }
    }
}
void CSimSysManager::vSaveAllSimSys()
{
    PSSIMSYSINFO psSimSys = m_ouSimSysNodeInfo.psReturnSimsysInfoListPtr();

    while (psSimSys != NULL)
    {
        m_omSimSysConfig.nSaveConfiguration(psSimSys->m_omStrSimSysName,
                                            psSimSys );
        psSimSys = psSimSys->m_psSimsysNext;
    }
}

CSimSysManager& CSimSysManager::ouGetSimSysManager(ETYPE_BUS eBus)
{
    if (sm_pouSimSysManager[eBus] == NULL)
    {
        sm_pouSimSysManager[eBus] = new CSimSysManager(eBus);
    }

    return *(sm_pouSimSysManager[eBus]);
}

void CSimSysManager::vClearObj(ETYPE_BUS eBus)
{
    if (sm_pouSimSysManager[eBus] != NULL)
    {
        delete sm_pouSimSysManager[eBus];
        sm_pouSimSysManager[eBus] = NULL;
    }
}

CSimSysNodeInfo* CSimSysManager::pomGetSimSysNodeInfo()
{
    return (&m_ouSimSysNodeInfo );
}


PSSIMSYSINFO CSimSysManager::psReturnSimsysInfoPtr()
{
    return m_ouSimSysNodeInfo.psReturnSimsysInfoListPtr();
}
/*
CString CSimSysManager::omStrGetUnionFilePath(CString omStrTemp)
{
    CString omStrHeaderFileName = omStrTemp.Left(omStrTemp.ReverseFind('.'));
    omStrHeaderFileName += defHEADER_FILE_NAME;
    return omStrHeaderFileName;
}
*/

CSimSysConfigDetails* CSimSysManager::pomGetSimSysConfig()
{
    return &m_omSimSysConfig;
}

void CSimSysManager::podSetSimSysDetView(CSimSysDetView* pDetView)
{
    m_pomSimSysDetView = pDetView;
}

CSimSysDetView* CSimSysManager::podGetSimSysDetView()
{
    return(m_pomSimSysDetView);
}

CSimSysTreeView* CSimSysManager::podGetSimSysTreeView()
{
    return(m_pomSimSysTreeView);
}

void CSimSysManager::podSetSimSysTreeView(CSimSysTreeView* pTreeView)
{
    m_pomSimSysTreeView = pTreeView;
}

CFlags& CSimSysManager::ouGetFlags()
{
    return m_ouFlags;
}


/******************************************************************************
    Function Name    :  vSaveSimSysWndConfig
    Input(s)         :  -
    Output           :  -
    Functionality    :  Saves simulation window configuration
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  12.02.2009
    Modification     :
/*****************************************************************************/
void CSimSysManager::vSaveSimSysWndConfig()
{
    if (CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd != NULL)
    {
        CGlobalObj::ouGetObj(m_eBus).m_nSimSysDataSize = unGetStoreSIMFBufferSize(); //Get the required size

        if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != NULL)
        {
            delete[] CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr;
        }

        CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr = new BYTE[CGlobalObj::ouGetObj(m_eBus).m_nSimSysDataSize];

        if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != NULL)
        {
            SaveSIMDataIntoBuffer(CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr);// save the config into buffer
            /* Add into a different section SIM_WND_CONFIG_SECTION_NAME*/
            /*CConfigDetails::ouGetConfigDetailsObject().bSetData((LPVOID)SectionBuffer,
                                            unBufferSize, SIM_WND_CONFIG_SECTION_NAME);
            delete [] SectionBuffer;*/
        }
    }
    else
    {
        if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr == NULL)
        {
            //New configuration. So get default values
            int nCount = 0;//Total number of simsys
            int nTotalSize = sizeof(BYTE);
            nTotalSize += sizeof(WINDOWPLACEMENT) + sizeof(UINT);
            CGlobalObj::ouGetObj(m_eBus).m_nSimSysDataSize = nTotalSize;
            CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr = new BYTE[nTotalSize];
            BYTE* byTempData = CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr;
            BYTE byVersion = 0x1;
            COPY_DATA(byTempData, &byVersion, sizeof(BYTE));
            CGlobalObj::ouGetObj(m_eBus).bGetDefaultValue(SIMSYS_WND_PLACEMENT, CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement);
            COPY_DATA(byTempData, &(CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement), sizeof(WINDOWPLACEMENT));
            COPY_DATA(byTempData, &nCount, sizeof(UINT));
        }
    }
}

/******************************************************************************
    Function Name    :  unGetStoreSIMBufferSize
    Input(s)         :  -
    Output           :  UINT (size)
    Functionality    :  size of simulation window configuration buffer
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19.02.2009
    Modification     :
/*****************************************************************************/
UINT CSimSysManager::unGetStoreSIMFBufferSize()
{
    //Size for SIM Filenames
    UINT nCount = m_ouSimSysNodeInfo.unGetNumberOfSimSys();
    UINT unTotalSize = 0;
    unTotalSize += sizeof(BYTE);//Configuration version
    unTotalSize += sizeof(WINDOWPLACEMENT);
    unTotalSize += sizeof(UINT); // Count
    unTotalSize += (nCount * (sizeof (char) * MAX_PATH));
    return unTotalSize;
}

/******************************************************************************
    Function Name    :  SaveSIMDataIntoBuffer
    Input(s)         :  Pointer to the buffer of BYTES
    Output           :  -
    Functionality    :  Saves simulation window params into buffer
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19.02.2009
    Modification     :
/*****************************************************************************/
void CSimSysManager::SaveSIMDataIntoBuffer(BYTE* DesBuffer)
{
    BYTE* tempBuffAddress = DesBuffer;
    BYTE byVersion = 0x1;
    COPY_DATA(tempBuffAddress, &byVersion, sizeof(BYTE));
    WINDOWPLACEMENT WndPlacement;
    CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->GetWindowPlacement(&WndPlacement);
    COPY_DATA(tempBuffAddress, &WndPlacement, sizeof(WINDOWPLACEMENT));
    UINT unCount = m_ouSimSysNodeInfo.unGetNumberOfSimSys();
    COPY_DATA(tempBuffAddress, &unCount, sizeof (UINT));
    PSSIMSYSINFO pSimSysInfo = m_ouSimSysNodeInfo.psReturnSimsysInfoListPtr();

    for (UINT i = 0; (i < unCount) && (pSimSysInfo != NULL); i++)
    {
        UINT unSize = 0;
        CString omTmp = pSimSysInfo->m_omStrSimSysName;
        char acFilename[MAX_PATH];
        strcpy_s(acFilename, omTmp.GetBuffer(MAX_PATH));
        COPY_DATA(tempBuffAddress, acFilename, sizeof(char) * MAX_PATH);
        pSimSysInfo = pSimSysInfo->m_psSimsysNext;
    }
}

/******************************************************************************
    Function Name    :  vLoadSimSysWndConfig
    Input(s)         :  -
    Output           :  -
    Functionality    :  Loads simulation window configuration
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19.02.2009
    Modification     :
/*****************************************************************************/
void CSimSysManager :: vLoadSimSysWndConfig()
{
    //First initialize all data
    vInitailizeSimSysInfo();

    //Now load the data
    //int nBufferCount = 0;
    //BYTE *SrcBuffer = NULL;
    /* Retrieve the section SIMSYS_WND_CONFIG_SECTION_NAME*/
    /*CConfigDetails::ouGetConfigDetailsObject().bGetData((void *&)(SrcBuffer),
                                            nBufferCount,SIM_WND_CONFIG_SECTION_NAME);*/
    if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != NULL)
    {
        /* Copy the retrieved config data into member variables and apply*/
        CopySIMDataFromBuffer(CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr);
        //delete SrcBuffer;
    }

    //Now Populate the tree view if it is present
    if (m_pomSimSysTreeView != NULL)
    {
        if (m_pomSimSysTreeView->IsWindowVisible())
        {
            m_pomSimSysTreeView->bPopulateTree();
        }
    }
}


/******************************************************************************
    Function Name    :  vInitailizeSimSysInfo
    Input(s)         :  -
    Output           :  -
    Functionality    :  It initialize the simulated sys info
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Anish Kr
    Date Created     :  04.03.2009
    Modification     :
/*****************************************************************************/
void CSimSysManager ::vInitailizeSimSysInfo()
{
    if (m_ouSimSysNodeInfo.bIsAnySimSysModified())
    {
        int nReturn = AfxMessageBox(ASK_SIMSYS_SAVE_PROMPT,
                                    MB_YESNO|MB_ICONQUESTION);

        if (nReturn == IDYES)
        {
            vSaveAllSimSys();
        }
    }

    m_ouSimSysNodeInfo.bDeleteSimsysFromInfo(STR_EMPTY);

    //Now Populate the tree view if it is present
    if (m_pomSimSysTreeView != NULL)
    {
        if (m_pomSimSysTreeView->IsWindowVisible())
        {
            m_pomSimSysTreeView->bPopulateTree();
        }
    }
}

/******************************************************************************
    Function Name    :  CopyNFDataFromBuffer
    Input(s)         :  -
    Output           :  -
    Functionality    :  Copies simulation window params into buffer
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19.02.2009
    Modification     :
/*****************************************************************************/
void CSimSysManager :: CopySIMDataFromBuffer(BYTE* SrcBuffer)
{
    BYTE* tempBuffAddress = SrcBuffer;
    BYTE byVersion = 0;
    COPY_DATA_2(&byVersion, tempBuffAddress, sizeof(BYTE));
    COPY_DATA_2(&CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement, tempBuffAddress, sizeof(WINDOWPLACEMENT));
    UINT UnCount = 0;
    COPY_DATA_2(&UnCount, tempBuffAddress, sizeof(UINT));
    CString omTmp = "";

    for (UINT i = 0; i < UnCount; i++)
    {
        char acFilename[MAX_PATH] = {_T('\0')};
        COPY_DATA_2(acFilename, tempBuffAddress, sizeof (char) * MAX_PATH);
        omTmp.Format("%s", acFilename);
        //Add the simsys file details
        vLoadSimInfoFromConfiguration(omTmp);
    }
}

/******************************************************************************
    Function Name    :  vLoadSimInfoFromConfiguration
    Input(s)         :  -
    Output           :  -
    Functionality    :  Copies simulation file details to simsysnode info
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Anish
    Date Created     :  27.02.2009
    Modification     :
/*****************************************************************************/
void CSimSysManager ::vLoadSimInfoFromConfiguration(CString omFileName)
{
    PSSIMSYSINFO pSimsys = NULL;
    int nReturn = m_omSimSysConfig.nLoadConfiguration(omFileName, pSimsys);

    if((nReturn == defCONFIG_FILE_SUCCESS) && (pSimsys != NULL))
    {
        m_ouSimSysNodeInfo.vAddSimSys(pSimsys);
    }
}

/******************************************************************************
    Function Name    :  bIsConfigChanged
    Input(s)         :  -
    Output           :  boolean variable to indicate change in configuration
    Functionality    :  Saves simulation window configuration
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19.02.2009
    Modification     :
/*****************************************************************************/
BOOL CSimSysManager :: bIsConfigChanged()
{
    //int nBufferCount = 0;
    BOOL bReturn = FALSE;

    //BYTE *SrcBuffer = NULL;
    /*CConfigDetails::ouGetConfigDetailsObject().bGetData((void *&)(SrcBuffer),
                                            nBufferCount,SIM_WND_CONFIG_SECTION_NAME);*/
    if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != NULL)
    {
        BYTE* tempBuffAddress = CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr;
        //
        UINT unCount = 0;
        memcpy(&unCount, tempBuffAddress, sizeof(UINT));
        bReturn = (unCount == m_ouSimSysNodeInfo.unGetNumberOfSimSys());
        tempBuffAddress += sizeof(UINT);

        if (bReturn)
        {
            PSSIMSYSINFO pSimSysInfo = m_ouSimSysNodeInfo.psReturnSimsysInfoListPtr();

            for (UINT i = 0; (i < unCount) && (pSimSysInfo != NULL); i++)
            {
                UINT nSize = 0;
                memcpy(&nSize, tempBuffAddress, sizeof(UINT));
                tempBuffAddress += sizeof(UINT);
                CString omTmp = "";
                char* pFileLen = new char[nSize];
                memcpy(pFileLen, tempBuffAddress, sizeof(char) * nSize);
                tempBuffAddress += sizeof(char) * nSize;
                omTmp = pFileLen;

                if (pSimSysInfo->m_omStrSimSysName != omTmp)
                {
                    bReturn = false;
                    i = unCount;
                }

                pSimSysInfo = pSimSysInfo->m_psSimsysNext;
                delete [] pFileLen;
            }
        }

        //
        //delete SrcBuffer;
    }

    return !bReturn;
}
