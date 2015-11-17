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

CSimSysManager::CSimSysManager(ETYPE_BUS eBus) : m_ouSimSysNodeInfo(eBus)
{
    m_pomSimSysTreeView = nullptr;
    m_CopyJ1939SimNode = nullptr;
    m_eBus = eBus;
}

CSimSysManager::~CSimSysManager(void)
{
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



CSimSysTreeView* CSimSysManager::podGetSimSysTreeView()
{
    return(m_pomSimSysTreeView);
}

void CSimSysManager::podSetSimSysTreeView(CSimSysTreeView* pTreeView)
{
    m_pomSimSysTreeView = pTreeView;
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

    PSNODELIST pTempNode = m_ouSimSysNodeInfo.m_psNodesList;


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

    while(pTempNode != nullptr)
    {
        //Node
        xmlNodePtr pSimSysNode = xmlNewNode(nullptr, BAD_CAST DEF_NODE);
        xmlAddChild(pNodePtr, pSimSysNode);

        //Name
        omcVarChar = pTempNode->m_sNodeInfo.m_omStrNodeName;
        xmlNodePtr pSimSysNodeName = xmlNewChild(pSimSysNode, nullptr, BAD_CAST DEF_NODE_NAME,BAD_CAST omcVarChar);
        xmlAddChild(pSimSysNode, pSimSysNodeName);

        if(m_eBus == J1939)
        {
            //Preferred_Address
            CString omStrPrefAddr;
            omStrPrefAddr.Format("%u", pTempNode->m_sNodeInfo.m_byPrefAddress);
            omcVarChar = omStrPrefAddr;
            xmlNodePtr pSimSysPrefAddr = xmlNewChild(pSimSysNode, nullptr, BAD_CAST DEF_NODE_PREFERRED_ADDRESS,BAD_CAST omcVarChar);
            xmlAddChild(pSimSysNode, pSimSysPrefAddr);

            //ECU_Name
            CString omStrECUName;
            omStrECUName.Format("%llu",pTempNode->m_sNodeInfo.m_unEcuName);
            omcVarChar = omStrECUName;
            xmlNodePtr pSimSysECUName = xmlNewChild(pSimSysNode, nullptr, BAD_CAST DEF_NODE_ECU_NAME,BAD_CAST omcVarChar);
            xmlAddChild(pSimSysNode, pSimSysECUName);
        }

        //Is_Enabled
        if(pTempNode->m_sNodeInfo.m_bIsNodeEnabled)
        {
            omcVarChar = "TRUE";
        }
        else
        {
            omcVarChar = "FALSE";
        }
        xmlNodePtr pSimSysIsNodeEnabled = xmlNewChild(pSimSysNode, nullptr, BAD_CAST DEF_IS_NODE_ENABLED,BAD_CAST omcVarChar);
        xmlAddChild(pSimSysNode, pSimSysIsNodeEnabled);

        //File_Type
        if(pTempNode->m_sNodeInfo.m_eNodeFileType == NODE_FILE_C_CPP)
        {
            omcVarChar = "C_CPP_FILE";
        }
        else if(pTempNode->m_sNodeInfo.m_eNodeFileType == NODE_FILE_DLL)
        {
            omcVarChar = "DLL_FILE";
        }
        xmlNodePtr pSimSysIsDLL = xmlNewChild(pSimSysNode, nullptr, BAD_CAST DEF_NODE_FILE_TYPE,BAD_CAST omcVarChar);
        xmlAddChild(pSimSysNode, pSimSysIsDLL);

        //File_Path
        std::string omPath, omStrConfigFolder;
        char configPath[MAX_PATH]= {0};
        AfxGetMainWnd()->SendMessage(MSG_GET_CONFIGPATH, (WPARAM)configPath, 0);
        CUtilFunctions::nGetBaseFolder(configPath, omStrConfigFolder );
        if(pTempNode->m_sNodeInfo.m_eNodeFileType == NODE_FILE_DLL)
        {
            CUtilFunctions::MakeRelativePath(omStrConfigFolder.c_str(), (char*)pTempNode->m_sNodeInfo.m_omStrDllName.GetBuffer(MAX_PATH), omPath);
        }
        else if(pTempNode->m_sNodeInfo.m_eNodeFileType == NODE_FILE_C_CPP)
        {
            CUtilFunctions::MakeRelativePath(omStrConfigFolder.c_str(), (char*)pTempNode->m_sNodeInfo.m_omStrCFileName.GetBuffer(MAX_PATH), omPath);
        }
        omcVarChar = omPath.c_str();
        xmlNodePtr pColName = xmlNewChild(pSimSysNode, nullptr, BAD_CAST DEF_NODE_FILE_PATH,BAD_CAST omcVarChar);
        xmlAddChild(pSimSysNode, pColName);

        pTempNode = pTempNode->m_psNextNode;
    }

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

    UINT nCount = m_ouSimSysNodeInfo.m_unNumberOfNodesAdded;
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

    UINT unCount = m_ouSimSysNodeInfo.m_unNumberOfNodesAdded;
    COPY_DATA(tempBuffAddress, &unCount, sizeof (UINT));

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
    if( nullptr != pNode )
    {
        pNode = pNode->xmlChildrenNode;
        m_ouSimSysNodeInfo.vReleaseNodeList();
        while (pNode != nullptr)
        {
            if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Window_Position")))
            {
                nWndPos =  xmlUtils::ParseWindowsPlacement(pNode, CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement);
            }
            if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Node")))
            {
                vLoadNodeInfoFromConfiguration(pNode);
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
    }
}
//
//HRESULT CSimSysManager::GenerateMakeFile(std::string strCppFile)
//{
//    if ( nullptr != CGlobalObj::ouGetObj(m_eBus).m_ouClusterConfig )
//    {
//        return CGlobalObj::ouGetObj(m_eBus).m_ouClusterConfig->GenerateMakeFile(m_eBus, strCppFile);
//    }
//    return S_OK;
//}
//
//HRESULT CSimSysManager::GenerateMakeFile()
//{
//    if ( nullptr != CGlobalObj::ouGetObj(m_eBus).m_ouClusterConfig )
//    {
//        PSNODELIST pNodeList =  m_ouSimSysNodeInfo.m_psNodesList;
//
//        while ( nullptr != pNodeList )
//        {
//            CGlobalObj::ouGetObj(m_eBus).m_ouClusterConfig->GenerateMakeFile(m_eBus, pNodeList->m_sNodeInfo.m_omStrCFileName.GetBuffer(0));
//            pNodeList = pNodeList->m_psNextNode;
//        }
//    }
//    return S_OK;
//}


/******************************************************************************
    Function Name    :  vLoadNodeInfoFromConfiguration
    Input(s)         :  -
    Output           :  -
    Functionality    :  Copies Node info from Configuration
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Robin G.K.
    Date Created     :  21.10.14
    Modification     :
/*****************************************************************************/
void CSimSysManager :: vLoadNodeInfoFromConfiguration(xmlNodePtr pNode)
{
    PSNODELIST pTempNode = new SNODELIST(m_eBus);
    pNode = pNode->children;
    while (pNode != nullptr && pTempNode != nullptr)
    {

        if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_NODE_NAME)))
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                CString omStrKey = (char*)key;
                pTempNode->m_sNodeInfo.m_omStrNodeName = omStrKey;
                //Set Node File is modified.
                pTempNode->m_sNodeInfo.m_eNodeState = NODE_REQ_CLEAN_BUILT;
                pTempNode->m_sNodeInfo.m_ouLastDBChecksum = 0;
                xmlFree(key);
            }
        }
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_NODE_PREFERRED_ADDRESS)) && m_eBus == J1939)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                CString omStrKey = (char*)key;
                pTempNode->m_sNodeInfo.m_byPrefAddress = atoi(omStrKey);
                xmlFree(key);
            }
        }
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_NODE_ECU_NAME)) && m_eBus == J1939)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                std::string strKey = (char*)key;
                std::stringstream sstr(strKey);
                sstr >> pTempNode->m_sNodeInfo.m_unEcuName;
                xmlFree(key);
            }
        }
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_IS_NODE_ENABLED)))
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                CString omStrKey = (char*)key;
                if(omStrKey == "TRUE")
                {
                    pTempNode->m_sNodeInfo.m_bIsNodeEnabled = TRUE;
                }
                else
                {
                    pTempNode->m_sNodeInfo.m_bIsNodeEnabled = FALSE;
                }
                xmlFree(key);
            }
        }
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_NODE_FILE_TYPE)))
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                CString omStrKey = (char*)key;
                if(omStrKey == "C_CPP_FILE")
                {
                    pTempNode->m_sNodeInfo.m_eNodeFileType = NODE_FILE_C_CPP;
                }
                else if(omStrKey == "DLL_FILE")
                {
                    pTempNode->m_sNodeInfo.m_eNodeFileType = NODE_FILE_DLL;
                }
                xmlFree(key);
            }
        }
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_NODE_FILE_PATH)))
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                CString omStrFileName = (char*)key;
                if(PathIsRelative((char*)key) == TRUE && !omStrFileName.IsEmpty())
                {
                    std::string omStrConfigFolder;
                    std::string omPath;
                    char configPath[MAX_PATH]= {0};
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
                if(pTempNode->m_sNodeInfo.m_eNodeFileType == NODE_FILE_DLL)
                {
                    pTempNode->m_sNodeInfo.m_omStrDllName = omStrFileName;
                }
                else if(pTempNode->m_sNodeInfo.m_eNodeFileType == NODE_FILE_C_CPP)
                {
                    pTempNode->m_sNodeInfo.m_omStrCFileName = omStrFileName;
                }

                xmlFree(key);

            }
        }
        pNode = pNode->next;
    }
    if(pTempNode != nullptr)
    {
        m_ouSimSysNodeInfo.vAddNodeToList(pTempNode);
        CGlobalObj::ouGetObj(m_eBus).RegisterNodeToDIL(TRUE, &pTempNode->m_sNodeInfo);
        bUpdateNodeInfoFile(pTempNode);

    }
    pTempNode = nullptr;
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
        bReturn = (unCount == m_ouSimSysNodeInfo.m_unNumberOfNodesAdded);
        tempBuffAddress += sizeof(UINT);
        if (bReturn)
        {
            for (UINT i = 0; (i < unCount) ; i++)
            {
                UINT nSize = 0;
                memcpy(&nSize, tempBuffAddress, sizeof(UINT));
                tempBuffAddress += sizeof(UINT);
                CString omTmp = "";
                char* pFileLen = new TCHAR[nSize];
                memcpy(pFileLen, tempBuffAddress, sizeof(char) * nSize);
                tempBuffAddress += sizeof(char) * nSize;
                omTmp = pFileLen;
                delete [] pFileLen;
            }
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

/******************************************************************************
    Function Name    :  bUpdateNodeInfoFile
    Input(s)         :  -
    Output           :  -
    Functionality    :  Associates .cpp(or .c) file and .dll file to the node.
    Member of        :  CSimSysManager
    Friend of        :
    Author(s)        :  Robin G.K.
    Date Created     :  21.10.14
    Modification     :
/*****************************************************************************/
void CSimSysManager::bUpdateNodeInfoFile(PSNODELIST psNodeList)
{
    CSimSysTreeView* pSimSysTreeView =
        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
    CSimSysNodeInfo* pSimSysNodeInf =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CString omStrFileName;
    if(psNodeList->m_sNodeInfo.m_omStrDllName.IsEmpty() || psNodeList->m_sNodeInfo.m_omStrCFileName.IsEmpty())
    {
        if(psNodeList->m_sNodeInfo.m_eNodeFileType == NODE_FILE_DLL)
        {
            omStrFileName = psNodeList->m_sNodeInfo.m_omStrDllName;
        }
        else if(psNodeList->m_sNodeInfo.m_eNodeFileType == NODE_FILE_C_CPP)
        {
            omStrFileName = psNodeList->m_sNodeInfo.m_omStrCFileName;
        }
    }
    int nIndex = omStrFileName.ReverseFind('.');
    if( nIndex >= 0 )
    {
        omStrFileName = omStrFileName.Left(nIndex);

    }
    CString omStrDllFile = omStrFileName + defDOT_DLL;
    // file-attribute information
    _tfinddata_t fileinfo;
    if (_tfindfirst( omStrDllFile.GetBuffer(MAX_PATH), &fileinfo) != -1L)
    {
        psNodeList->m_sNodeInfo.m_omStrDllName = omStrDllFile;
    }

    CString omStrCPPFile = omStrFileName + defDOT_CPP;
    if(_tfindfirst( omStrCPPFile.GetBuffer(MAX_PATH), &fileinfo) != -1L)
    {
        //Unregister the previously monitoring file
        psNodeList->m_sNodeInfo.RegisterFileToMonitor(std::string(psNodeList->m_sNodeInfo.m_omStrCFileName),false);
        psNodeList->m_sNodeInfo.m_omStrCFileName = omStrCPPFile;
        //Register the new monitoring file.
        psNodeList->m_sNodeInfo.RegisterFileToMonitor(std::string(psNodeList->m_sNodeInfo.m_omStrCFileName),true);
    }

    CString omStrCFile = omStrFileName + defDOT_C;
    if(_tfindfirst( omStrCFile.GetBuffer(MAX_PATH), &fileinfo) != -1L)
    {
        psNodeList->m_sNodeInfo.m_omStrCFileName = omStrCFile;
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
    Modification     :  Robin G.K. changes due to removed .sim file
/*****************************************************************************/
void CSimSysManager ::vInitailizeSimSysInfo()
{

    //Delete Node.
    m_ouSimSysNodeInfo.bDeleteNodeFromSimSys("");
}