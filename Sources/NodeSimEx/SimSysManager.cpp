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
#include "Utility/XMLUtils.h"
#include "HashDefines.h"
#include "Include/Utils_macro.h"
#include "simsysmanager.h"
#include "GlobalObj.h"
#include "include/XMLDefines.h"
#include <Shlwapi.h>
#include "Utility\UtilFunctions.h"

#define MSG_GET_CONFIGPATH  10000

#define SIM_WND_CONFIG_SECTION_NAME "SimWndConfigSection"
#define SIMSYS_CONFIG_SECTION_NAME      "SimSysConfig"

//INstantiation of singleton class
CSimSysManager* CSimSysManager::sm_pouSimSysManager[BUS_TOTAL] = {nullptr};

CSimSysManager::CSimSysManager(ETYPE_BUS eBus) : m_ouSimSysNodeInfo(eBus),
    m_omSimSysConfig(eBus)
{
    m_pomSimSysDetView = nullptr;
    m_pomSimSysTreeView = nullptr;
    m_CopyJ1939SimNode = nullptr;
    m_eBus = eBus;
}

CSimSysManager::~CSimSysManager(void)
{
    m_ouSimSysNodeInfo.bDeleteSimsysFromInfo("");
}

void CSimSysManager::vApplicationClosing(bool bSave)
{
    if (bSave && m_ouSimSysNodeInfo.bIsAnySimSysModified())
    {
        vSaveAllSimSys();
    }

    m_ouSimSysNodeInfo.bDeleteSimsysFromInfo("");
}
void CSimSysManager::vSaveAllSimSys()
{
    PSSIMSYSINFO psSimSys = m_ouSimSysNodeInfo.psReturnSimsysInfoListPtr();
    while (psSimSys != nullptr)
    {
        m_omSimSysConfig.nSaveConfiguration(psSimSys->m_omStrSimSysName,
                                            psSimSys );
        psSimSys = psSimSys->m_psSimsysNext;
    }
}

void CSimSysManager::vSaveSimSys(CString omStrSimSysName)
{
    PSSIMSYSINFO psSimSys = m_ouSimSysNodeInfo.psReturnSimsysInfoListPtr();
    while (psSimSys != nullptr)
    {
        if ( omStrSimSysName.Compare(psSimSys->m_omStrSimSysName) == 0 )
        {
            m_omSimSysConfig.nSaveConfiguration(psSimSys->m_omStrSimSysName,
                                                psSimSys );
        }
        psSimSys = psSimSys->m_psSimsysNext;
    }
}

CSimSysManager& CSimSysManager::ouGetSimSysManager(ETYPE_BUS eBus)
{
    if (sm_pouSimSysManager[eBus] == nullptr)
    {
        sm_pouSimSysManager[eBus] = new CSimSysManager(eBus);
    }
    return *(sm_pouSimSysManager[eBus]);
}

void CSimSysManager::vClearObj(ETYPE_BUS eBus)
{
    if (sm_pouSimSysManager[eBus] != nullptr)
    {
        delete sm_pouSimSysManager[eBus];
        sm_pouSimSysManager[eBus] = nullptr;
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
    if (CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd != nullptr)
    {
        CGlobalObj::ouGetObj(m_eBus).m_nSimSysDataSize = unGetStoreSIMFBufferSize(); //Get the required size

        if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != nullptr)
        {
            delete[] CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr;
        }
        CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr = new BYTE[CGlobalObj::ouGetObj(m_eBus).m_nSimSysDataSize];
        if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != nullptr)
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
        if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr == nullptr)
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

bool CSimSysManager::bGetConfigData(xmlNodePtr pNodePtr)
{

    const char* omcVarChar ;
    UINT unCount = m_ouSimSysNodeInfo.unGetNumberOfSimSys();
    PSSIMSYSINFO pSimSysInfo = m_ouSimSysNodeInfo.psReturnSimsysInfoListPtr();
    for (UINT i = 0; (i < unCount) && (pSimSysInfo != nullptr); i++)
    {
        CString omTmp = pSimSysInfo->m_omStrSimSysName;

        std::string omPath, omStrConfigFolder;
        char configPath[MAX_PATH];
        AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
        CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
        CUtilFunctions::MakeRelativePath(omStrConfigFolder.c_str(), (char*)omTmp.GetBuffer(MAX_PATH), omPath);


        omcVarChar = omPath.c_str();
        xmlNodePtr pColName = xmlNewChild(pNodePtr, nullptr, BAD_CAST DEF_SYS_PATH,BAD_CAST omcVarChar);
        xmlAddChild(pNodePtr, pColName);

        pSimSysInfo = pSimSysInfo->m_psSimsysNext;
    }


    //window placement
    xmlNodePtr pNodeWindowsPos = xmlNewNode(nullptr, BAD_CAST DEF_WND_POS);
    xmlAddChild(pNodePtr, pNodeWindowsPos);


    WINDOWPLACEMENT sWndPlacement;
    if(CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd != nullptr)
    {
        CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->GetWindowPlacement(&sWndPlacement);
        xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWindowsPos,sWndPlacement);
    }
    else if(CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd == nullptr)
    {
        xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWindowsPos,CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement);
    }

    ////visibility
    //CString csVisibility;
    //csVisibility.Format("%d",  sWndPlacement.flags);
    //omcVarChar = csVisibility;
    //xmlNodePtr pVisibility = xmlNewChild(pNodeWindowsPos, nullptr, BAD_CAST DEF_VISIBILITY,BAD_CAST omcVarChar);
    //xmlAddChild(pNodeWindowsPos, pVisibility);

    ////WindowPlacement
    //CString csWindowPlacement;
    //csWindowPlacement.Format("%d",  sWndPlacement.flags);
    //omcVarChar = csWindowPlacement;
    //xmlNodePtr pWindowPlacement = xmlNewChild(pNodeWindowsPos, nullptr, BAD_CAST DEF_MWND_PLACEMENT,BAD_CAST omcVarChar);
    //xmlAddChild(pNodeWindowsPos, pWindowPlacement);

    ////Top
    //CString csTop;
    //csTop.Format("%d",  sWndPlacement.rcNormalPosition.top);
    //omcVarChar = csTop;
    //xmlNodePtr pTop= xmlNewChild(pNodeWindowsPos, nullptr, BAD_CAST DEF_MWND_TOP,BAD_CAST omcVarChar);
    //xmlAddChild(pNodeWindowsPos, pTop);

    ////Left
    //CString csLeft;
    //csLeft.Format("%d",  sWndPlacement.rcNormalPosition.left);
    //omcVarChar = csLeft;
    //xmlNodePtr pLeft = xmlNewChild(pNodeWindowsPos, nullptr, BAD_CAST DEF_MWND_LEFT,BAD_CAST omcVarChar);
    //xmlAddChild(pNodeWindowsPos, pLeft);

    ////Bottom
    //CString csBottom;
    //csBottom.Format("%d",  sWndPlacement.rcNormalPosition.bottom);
    //omcVarChar = csBottom;
    //xmlNodePtr pBottom= xmlNewChild(pNodeWindowsPos, nullptr, BAD_CAST DEF_BOTTOM,BAD_CAST omcVarChar);
    //xmlAddChild(pNodeWindowsPos, pBottom);

    ////Right
    //CString csRight;
    //csRight.Format("%d",  sWndPlacement.rcNormalPosition.right);
    //omcVarChar = csRight;
    //xmlNodePtr pRight = xmlNewChild(pNodeWindowsPos, nullptr, BAD_CAST DEF_RIGHT,BAD_CAST omcVarChar);
    //xmlAddChild(pNodeWindowsPos, pRight);

    return true;
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
    for (UINT i = 0; (i < unCount) && (pSimSysInfo != nullptr); i++)
    {
        UINT unSize = 0;
        CString omTmp = pSimSysInfo->m_omStrSimSysName;
        char acFilename[MAX_PATH];
        _tcscpy(acFilename, omTmp.GetBuffer(MAX_PATH));
        COPY_DATA(tempBuffAddress, acFilename, sizeof(char) * MAX_PATH);
        pSimSysInfo = pSimSysInfo->m_psSimsysNext;
    }

}

void CSimSysManager::vLoadSimSysWndConfig(xmlDocPtr pDoc, ETYPE_BUS eBus)
{
    //First initialize all data
    vInitailizeSimSysInfo();

    if ( nullptr != pDoc )
    {
        /* Copy the retrieved config data into member variables and apply*/
        CopySIMDataFromBuffer(pDoc, eBus);
        //delete SrcBuffer;
    }

    //Now Populate the tree view if it is present
    if (m_pomSimSysTreeView != nullptr)
    {
        if (m_pomSimSysTreeView->IsWindowVisible())
        {
            m_pomSimSysTreeView->bPopulateTree();
        }
    }
}
void CSimSysManager::CopySIMDataFromBuffer(xmlNodePtr pDoc, ETYPE_BUS /* eBus */)
{
    xmlNodePtr pNode = pDoc;
    //if( nullptr != pObjectPath )
    {
        //xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        //if( nullptr != pNodeSet )
        //{
        //  pNode = pNodeSet->nodeTab[0];       //Take First One only
        //}
        if( nullptr != pNode )
        {
            pNode = pNode->xmlChildrenNode;
            while (pNode != nullptr)
            {
                if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Window_Position")))
                {
                    xmlUtils::ParseWindowsPlacement(pNode, CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement);
                }
                if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Sym_Path")))
                {
                    xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                    if(nullptr != key)
                    {
                        CString omStrFileName;
                        if(PathIsRelative((char*)key) == TRUE)
                        {
                            std::string omStrConfigFolder;
                            std::string omPath;
                            char configPath[MAX_PATH];
                            AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
                            CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
                            char chAbsPath[MAX_PATH];
                            PathCombine(chAbsPath, omStrConfigFolder.c_str(), (char*)key);
                            omStrFileName = chAbsPath;
                        }
                        else
                        {
                            omStrFileName = (char*)key;
                        }
                        vLoadSimInfoFromConfiguration(omStrFileName.GetBuffer(MAX_PATH));
                        xmlFree(key);
                    }
                }
                pNode = pNode->next;
            }
        }
        //xmlXPathFreeObject(pObjectPath);
    }
    /*
    COPY_DATA_2(&CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement, tempBuffAddress, sizeof(WINDOWPLACEMENT));
    UINT UnCount = 0;
    COPY_DATA_2(&UnCount, tempBuffAddress, sizeof(UINT));

    CString omTmp = "";
    for (UINT i = 0; i < UnCount; i++)
    {
        char acFilename[MAX_PATH] = {'\0'};
        COPY_DATA_2(acFilename, tempBuffAddress, sizeof (char) * MAX_PATH);
        omTmp.Format("%s", acFilename);
        //Add the simsys file details
        vLoadSimInfoFromConfiguration(omTmp);
    }*/
}

void CSimSysManager::CopySIMDataFromBuffer(xmlDocPtr pDoc, ETYPE_BUS eBus)
{
    xmlChar* pXpath = nullptr;
    xmlXPathObjectPtr pObjectPath = nullptr;

    if(eBus == CAN)
    {
        pXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Simulated_Systems";
        pObjectPath = xmlUtils::pGetNodes(pDoc, pXpath);
    }
    else if(eBus == J1939)
    {
        pXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/J1939_Simulated_Systems";
        pObjectPath = xmlUtils::pGetNodes(pDoc, pXpath);
    }
    else if(eBus == LIN)
    {
        pXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Simulated_Systems";
        pObjectPath = xmlUtils::pGetNodes(pDoc, pXpath);
    }

    if(m_CopyJ1939SimNode != nullptr && (eBus == J1939))
    {
        xmlFreeNode(m_CopyJ1939SimNode);
        m_CopyJ1939SimNode = nullptr;
    }

    else if(pObjectPath != nullptr && (eBus == J1939))
    {
        xmlFreeNode(m_CopyJ1939SimNode);
        m_CopyJ1939SimNode = nullptr;
        //m_CopyJ1939SimNode = xmlCopyNode(pObjectPath->nodesetval->nodeTab[0], 1);
    }
    else
    {
        xmlFreeNode(m_CopyJ1939SimNode);
        m_CopyJ1939SimNode = nullptr;
    }

    xmlNodePtr pNode = nullptr;



    if( nullptr != pObjectPath )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( nullptr != pNodeSet )
        {
            pNode = pNodeSet->nodeTab[0];       //Take First One only
        }
        vSetConfigData(pNode);
        xmlXPathFreeObject(pObjectPath);
    }
}

void CSimSysManager::vSetConfigData(xmlNodePtr pNode)
{
    INT nWndPos = S_FALSE;
    vInitailizeSimSysInfo();
    if( nullptr != pNode )
    {
        pNode = pNode->xmlChildrenNode;

        while (pNode != nullptr)
        {
            if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Window_Position")))
            {
                nWndPos =  xmlUtils::ParseWindowsPlacement(pNode, CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement);
            }
            if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Sym_Path")))
            {
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
                if(nullptr != key)
                {
                    CString omStrFileName;
                    if(PathIsRelative((char*)key) == TRUE)
                    {
                        std::string omStrConfigFolder;
                        std::string omPath;
                        char configPath[MAX_PATH];
                        AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
                        CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
                        char chAbsPath[MAX_PATH];
                        PathCombine(chAbsPath, omStrConfigFolder.c_str(), (char*)key);
                        omStrFileName = chAbsPath;
                    }
                    else
                    {
                        omStrFileName = (char*)key;
                    }
                    vLoadSimInfoFromConfiguration((char*)omStrFileName.GetBuffer(MAX_PATH));
                    xmlFree(key);
                }
            }
            pNode = pNode->next;
        }
    }
    if(nWndPos == S_FALSE)
    {
        /*CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition.top  = 160;
        CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition.left = 520;
        CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition.right = 1458;
        CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition.bottom = 686;
        CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.showCmd = SW_NORMAL;*/
        CGlobalObj::ouGetObj(m_eBus).bGetDefaultValue(SIMSYS_WND_PLACEMENT, CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement);
    }
    if (m_pomSimSysTreeView != nullptr)
    {
        if (m_pomSimSysTreeView->IsWindowVisible())
        {
            m_pomSimSysTreeView->bPopulateTree();
        }
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
    //BYTE *SrcBuffer = nullptr;
    /* Retrieve the section SIMSYS_WND_CONFIG_SECTION_NAME*/
    /*CConfigDetails::ouGetConfigDetailsObject().bGetData((void *&)(SrcBuffer),
                                            nBufferCount,SIM_WND_CONFIG_SECTION_NAME);*/
    if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != nullptr)
    {
        /* Copy the retrieved config data into member variables and apply*/
        CopySIMDataFromBuffer(CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr);
        //delete SrcBuffer;
    }
    //Now Populate the tree view if it is present
    if (m_pomSimSysTreeView != nullptr)
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
    //if(m_CopyJ1939SimNode == nullptr)
    {
        m_ouSimSysNodeInfo.bDeleteSimsysFromInfo("");
    }
    //Now Populate the tree view if it is present
    //SSH + commneted to resolve issue #392 'Duplicate simulated systems'
    /*if (m_pomSimSysTreeView != nullptr)
    {
        if (m_pomSimSysTreeView->IsWindowVisible())
        {
            m_pomSimSysTreeView->bPopulateTree();
        }
    }*/
    //SSH -
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
        char acFilename[MAX_PATH] = {'\0'};
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
    PSSIMSYSINFO pSimsys = nullptr;
    int nReturn = m_omSimSysConfig.nLoadConfiguration(omFileName, pSimsys);
    if((nReturn == defCONFIG_FILE_SUCCESS) && (pSimsys != nullptr))
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
    //BYTE *SrcBuffer = nullptr;
    /*CConfigDetails::ouGetConfigDetailsObject().bGetData((void *&)(SrcBuffer),
                                            nBufferCount,SIM_WND_CONFIG_SECTION_NAME);*/
    if (CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != nullptr)
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
            for (UINT i = 0; (i < unCount) && (pSimSysInfo != nullptr); i++)
            {
                UINT nSize = 0;
                memcpy(&nSize, tempBuffAddress, sizeof(UINT));
                tempBuffAddress += sizeof(UINT);
                CString omTmp = "";
                char* pFileLen = new TCHAR[nSize];
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
    else
    {
        CSimSysNodeInfo* pSimSysNodeInfo = CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
        if (nullptr != pSimSysNodeInfo)
        {
            return pSimSysNodeInfo->bIsAnySimSysModified();
        }
    }

    return !bReturn;
}

void CSimSysManager::vSetDatabaseConfiguration(ClusterConfig* ouLINConfig)
{
    CGlobalObj::ouGetObj(m_eBus).m_ouClusterConfig = ouLINConfig;
    /*string strDbName;
    for(INT nIndex = 0; nIndex < ouLINConfig.m_nChannelsConfigured;nIndex++)
    {
        strDbName = ouLINConfig.m_ouFlexChannelConfig[nIndex].m_strDataBasePath;
    }*/
}