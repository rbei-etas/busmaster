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
 * \file      TestCaseEntity.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "TestSetupEditorLib_stdafx.h"
#include "TestCaseEntity.h"
#include "SendEntity.h"
#include "VerifyEntity.h"
#include "verifyResponse.h"

/******************************************************************************
Function Name  :  CTestCaseEntity
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestCaseEntity::CTestCaseEntity(void)
{
    m_eType = TEST_CASE;
}

/******************************************************************************
Function Name  :  ~CTestCaseEntity
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestCaseEntity::~CTestCaseEntity()
{
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CTestCaseEntity& RefObj
Output         :  CTestCaseEntity&
Functionality  :  = operator overloading
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestCaseEntity& CTestCaseEntity::operator=(const CTestCaseEntity& RefObj)
{
    m_ouData = RefObj.m_ouData;
    return *this;
}

/******************************************************************************
Function Name  :  CTestCaseEntity
Input(s)       :  -
Output         :  -
Functionality  :  Copy Constructor
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestCaseEntity::CTestCaseEntity(const CTestCaseEntity& RefObj)
{
    m_ouData = RefObj.m_ouData;
}


/******************************************************************************
Function Name  :  AddSubEntry
Input(s)       :  CBaseEntityTA* pouSubEntryObj
Output         :  HRESULT
Functionality  :
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestCaseEntity::AddSubEntry(CBaseEntityTA* pouSubEntryObj)
{
    m_ouData.m_odTAEntityList.AddTail(pouSubEntryObj);
    return pouSubEntryObj->GetID();
}

/******************************************************************************
Function Name  :  vDeleteAllEntities
Input(s)       :  -
Output         :  void
Functionality  :  Delets all the list items
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
void CTestCaseEntity::vDeleteAllEntities(void)
{
    /*INT nCount = (INT)m_ouData.m_odTAEntityList.GetCount();
    for(INT i = 0; i<nCount; i++);
    {
        POSITION pos = m_ouData.m_odTAEntityList.FindIndex(i);
        m_ouData.vRemoveAll();
    }*/
    m_ouData.vRemoveAll();
}

/******************************************************************************
Function Name  :  DeleteSubEntry
Input(s)       :  CBaseEntityTA* pouSubEntryObj
Output         :  HRESULT
Functionality  :  Deletes a Subentry
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS018
******************************************************************************/
HRESULT CTestCaseEntity::DeleteSubEntry(CBaseEntityTA* pouSubEntryObj)
{
    HRESULT hResult  = S_FALSE;
    INT nCount = (INT)m_ouData.m_odTAEntityList.GetCount();

    for(int i = 0; i < nCount; i++)
    {
        POSITION pos = m_ouData.m_odTAEntityList.FindIndex(i);
        CBaseEntityTA* pEntity = m_ouData.m_odTAEntityList.GetAt(pos);

        if(pEntity->GetID() == pouSubEntryObj->GetID())
        {
            m_ouData.m_odTAEntityList.RemoveAt(pos);
            hResult = S_OK;
            break;
        }
    }

    return hResult;
}

/******************************************************************************
Function Name  :  DeleteSubEntry
Input(s)       :  INT index
Output         :  HRESULT
Functionality  :
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestCaseEntity::DeleteSubEntry(INT /*index*/)
{
    return ERR_NOT_IMPLEMENTED;
}


/******************************************************************************
Function Name  :  GetSubEntityObj
Input(s)       :  UINT unIndex
                  CBaseEntityTA**  pouSubEntity
Output         :  HRESULT
Functionality  :
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
const HRESULT CTestCaseEntity::GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity)
{
    POSITION pos = m_ouData.m_odTAEntityList.FindIndex(unIndex);

    if(pos != NULL)
    {
        *pouSubEntity =  m_ouData.m_odTAEntityList.GetAt(pos);
        return  S_OK;
    }

    return S_FALSE;
}


/******************************************************************************
Function Name  :  GetSubEntryCount
Input(s)       :  UINT& unTotal
Output         :  HRESULT
Functionality  :  return the subenty count
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestCaseEntity::GetSubEntryCount(UINT& unTotal)
{
    unTotal = (UINT)m_ouData.m_odTAEntityList.GetCount();
    return  NULL;
}

/******************************************************************************
Function Name  :  GetData
Input(s)       :  MSXML2::IXMLDOMNodePtr& pIDomNode
Output         :  HRESULT
Functionality  :  Fills the datastructure from the XML node
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestCaseEntity::GetData(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    //Getting Attributes
    _bstr_t bstrNodeName;
    CComVariant NodeValue;
    MSXML2::IXMLDOMNamedNodeMapPtr pDOMTCAtrributes;
    pDOMTCAtrributes = pIDomNode->Getattributes();
    MSXML2::IXMLDOMNodePtr pIDOMChildNode;
    //bstrNodeName = L"identifier";
    bstrNodeName.Assign(SysAllocString(CT2W("identifier")));
    pIDOMChildNode = pDOMTCAtrributes->getNamedItem(bstrNodeName);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    m_ouData.m_omID = strCopyBSTRToCString(NodeValue);
    pIDOMChildNode.Release();
    //bstrNodeName = L"title";
    bstrNodeName.Assign(SysAllocString(CT2W("title")));
    pIDOMChildNode = pDOMTCAtrributes->getNamedItem(bstrNodeName);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    m_ouData.m_omTitle = strCopyBSTRToCString(NodeValue);
    pIDOMChildNode.Release();
    //bstrNodeName = L"exp_handler";
    bstrNodeName.Assign(SysAllocString(CT2W("exp_handler")));
    pIDOMChildNode = pDOMTCAtrributes->getNamedItem(bstrNodeName);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    CString strTemp;
    strTemp = strCopyBSTRToCString(NodeValue);
    pIDOMChildNode.Release();

    if(strTemp == "CONTINUE")
    {
        m_ouData.m_eExcpAction = CONTINUE;
    }
    else
    {
        m_ouData.m_eExcpAction = EXIT;
    }

    //Getting Testcases
    MSXML2::IXMLDOMNodeListPtr pDOMChildList = pIDomNode->GetchildNodes();
    LONG lCount;
    pDOMChildList->get_length(&lCount);
    CComBSTR NodeName;

    for(int i = 0; i < lCount; i++)
    {
        pIDOMChildNode = pDOMChildList->Getitem(i);
        pIDOMChildNode->get_nodeName(&NodeName);
        CBaseEntityTA* odpBaseEntity = NULL;

        if(NodeName == def_STR_SEND_NODE)
        {
            odpBaseEntity = new CSendEntity();
        }
        else if(NodeName == def_STR_VERIFY_NODE)
        {
            odpBaseEntity = new CVerifyEntity();
        }
        else if(NodeName == def_STR_WAIT_NODE)
        {
            odpBaseEntity = new CWaitEntity();
        }
        else if(NodeName == def_STR_REPLAY_NODE)
        {
            odpBaseEntity = new CReplayEntity();
        }
        else if(NodeName == def_STR_VERIFYRES_NODE)
        {
            odpBaseEntity = new CVerifyResponse();
        }

        if(odpBaseEntity == NULL)
        {
            //TODO::return a correct value
            return -1;
        }

        odpBaseEntity->GetData(pIDOMChildNode);
        m_ouData.m_odTAEntityList.AddTail(odpBaseEntity);
        //pIDOMChildNode->Release();
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  SetData
Input(s)       :  MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode
Output         :  HRESULT
Functionality  :
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestCaseEntity::SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode)
{
    CString omstrTemp;
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");
    MSXML2::IXMLDOMAttributePtr pIDomTSAtrrib = pIDOMDoc->createAttribute(def_STR_TCATTRIB_ID);

    if(pIDomTSAtrrib!= NULL)
    {
        pIDomTSAtrrib->value = _bstr_t(m_ouData.m_omID);
        pIDomTestCaseNode->setAttributeNode(pIDomTSAtrrib);
    }

    pIDomTSAtrrib = pIDOMDoc->createAttribute(def_STR_TCATTRIB_TITLE);

    if(pIDomTSAtrrib!= NULL)
    {
        pIDomTSAtrrib->value = _bstr_t(m_ouData.m_omTitle);
        pIDomTestCaseNode->setAttributeNode(pIDomTSAtrrib);
    }

    pIDomTSAtrrib = pIDOMDoc->createAttribute(def_STR_TCATTRIB_H_EXP);

    if(pIDomTSAtrrib!= NULL)
    {
        switch(m_ouData.m_eExcpAction)
        {
            case EXIT:
                omstrTemp = "EXIT";
                break;

            case CONTINUE:
            default:
                omstrTemp = "CONTINUE";
                break;
        }

        pIDomTSAtrrib->value = _bstr_t(omstrTemp);
        pIDomTestCaseNode->setAttributeNode(pIDomTSAtrrib);
    }

    INT lCount = (INT)m_ouData.m_odTAEntityList.GetCount();

    for(int i=0; i<lCount; i++)
    {
        POSITION pos = m_ouData.m_odTAEntityList.FindIndex(i);
        CBaseEntityTA* pEntity = m_ouData.m_odTAEntityList.GetAt(pos);
        pEntity->SetData(pIDomTestCaseNode);
    }

    return 0;
}

/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestCaseEntity::GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == TEST_CASE && pvEntityData != NULL)
    {
        *((CTestCaseData*)pvEntityData) = m_ouData;
        return S_OK;
    }

    return S_FALSE;
}

/******************************************************************************
Function Name  :  SetTestCaseDetails
Input(s)       :  CString& omTitle - TestCase Title
                  CString& omID - TestCase ID
                  eACTION_EXCP& eExcpAction - Exception Handler
Output         :  HRESULT
Functionality  :  Sets the TestCase Details
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestCaseEntity::SetTestCaseDetails(CString& omTitle, CString& omID, eACTION_EXCP& eExcpAction)
{
    m_ouData.m_omTitle = omTitle;
    m_ouData.m_omID = omID;
    m_ouData.m_eExcpAction = eExcpAction;
    return S_OK;
}

/******************************************************************************
Function Name  :  GetTestCaseDetails
Input(s)       :  CString& omTitle - TestCase Title
                  CString& omID - TestCase ID
                  eACTION_EXCP& eExcpAction - Exception Handler
Output         :  HRESULT
Functionality  :  Returns the TestCase Details
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestCaseEntity::GetTestCaseDetails(CString& omTitle, CString& omID, eACTION_EXCP& eExcpAction)
{
    omTitle = m_ouData.m_omTitle;
    omID = m_ouData.m_omID;
    eExcpAction = m_ouData.m_eExcpAction;
    return S_OK;
}

/******************************************************************************
Function Name  :  ValidateEntity
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  13/04/2011
Modifications  :
Code Tag       :  CS028
******************************************************************************/
HRESULT CTestCaseEntity::ValidateEntity(CString& omStrResult)
{
    UINT unTestCaseCount;
    CString omStrTemp;
    GetSubEntryCount(unTestCaseCount);
    HRESULT hResult = ERR_VALID_SUCCESS;

    if(unTestCaseCount == 0)
    {
        omStrTemp.Format("\tError: Atleast One Verify Node Should be Defined\r\n");
        hResult = ERR_VALID_ERROR;      //CS028
    }
    else
    {
        BOOL bVerifyEntityFound = FALSE;

        //omStrTemp = "\t";
        for(UINT i = 0; i < unTestCaseCount; i++)
        {
            CString omStrTCResult;
            CBaseEntityTA* pTCSubEntity;

            if(GetSubEntityObj(i, &pTCSubEntity) == S_OK)
            {
                omStrTCResult.Format(_T("Node %d:"), i+1);
                HRESULT hTempResult = pTCSubEntity->ValidateEntity(omStrTCResult);

                if(hTempResult != ERR_VALID_SUCCESS)
                {
                    omStrTemp += "\t" + omStrTCResult;

                    if(hResult != ERR_VALID_ERROR)
                    {
                        hResult = hTempResult;
                    }
                }

                if(pTCSubEntity->GetEntityType() == VERIFY)
                {
                    bVerifyEntityFound = TRUE;
                }
            }
        }

        if(bVerifyEntityFound == FALSE)
        {
            omStrTemp += "\tError: No Verify Entity Found\r\n";
            hResult = ERR_VALID_ERROR;
        }
    }

    if(hResult != ERR_VALID_SUCCESS)
    {
        omStrResult.Format(_T("TestCase %s Result \r\n"), m_ouData.m_omTitle);
        omStrResult += omStrTemp + "\n";
    }

    return hResult;
}
/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestCaseEntity::SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == TEST_CASE && pvEntityData != NULL)
    {
        m_ouData = *((CTestCaseData*)pvEntityData);
        return S_OK;
    }

    return S_FALSE;
}
eTYPE_ENTITY CTestCaseEntity::GetEntityType()
{
    return m_eType;
}

/******************************************************************************
Function Name  :  RepositionSubEntity
Input(s)       :  CBaseEntityTA* pouRefSubEntity
                  CBaseEntityTA* pouCurrSubEntity
Output         :  HRESULT - returns the New Entity ID
Functionality  :  Reposition pouRefSubEntity after pouCurrSubEntity
Member of      :  CTestCaseEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS021
******************************************************************************/
HRESULT CTestCaseEntity::RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity)
{
    //DESIGN:: First Take copy of Data and delete it from current position and add at required place
    UINT unCount = (UINT)m_ouData.m_odTAEntityList.GetCount();

    for(UINT i=0; i<unCount; i++)
    {
        POSITION pos = m_ouData.m_odTAEntityList.FindIndex(i);
        CBaseEntityTA* pEntity = m_ouData.m_odTAEntityList.GetAt(pos);

        if(pEntity->GetID() == pouRefSubEntity->GetID())
        {
            m_ouData.m_odTAEntityList.RemoveAt(pos);
            break;
        }
    }

    if(pouCurrSubEntity == NULL)        //Insert At First Index;
    {
        m_ouData.m_odTAEntityList.AddHead(pouRefSubEntity);
    }
    else
    {
        for(UINT i=0; i<unCount; i++)
        {
            POSITION pos = m_ouData.m_odTAEntityList.FindIndex(i);
            CBaseEntityTA* pEntity = m_ouData.m_odTAEntityList.GetAt(pos);

            if(pEntity->GetID() == pouCurrSubEntity->GetID())
            {
                m_ouData.m_odTAEntityList.InsertAfter(pos, pouRefSubEntity);
                break;
            }
        }
    }

    return pouRefSubEntity->GetID();
}


/******************************************************************************
Function Name  :  CWaitEntity
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CWaitEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CWaitEntity::CWaitEntity(void)
{
    m_eType = WAIT;
}

/******************************************************************************
Function Name  :  ~CWaitEntity
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CWaitEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CWaitEntity::~CWaitEntity(void)
{
}

/******************************************************************************
Function Name  :  CWaitEntity
Input(s)       :  CWaitEntity& RefObj
Output         :  -
Functionality  :  Copy Constructor
Member of      :  CWaitEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CWaitEntity::CWaitEntity(const CWaitEntity& RefObj)
{
    m_ouData = RefObj.m_ouData;
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CWaitEntity& RefObj
Output         :  CWaitEntity&
Functionality  :  = operator overloading
Member of      :  CWaitEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CWaitEntity& CWaitEntity::operator=(CWaitEntity& RefObj)
{
    m_ouData = RefObj.m_ouData;
    return *this;
}

/******************************************************************************
Function Name  :  GetData
Input(s)       :  MSXML2::IXMLDOMNodePtr& pIDomNode
Output         :  HRESULT
Functionality  :
Member of      :  CWaitEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CWaitEntity::GetData(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    _bstr_t bstrNodeName;
    CComVariant NodeValue;
    MSXML2::IXMLDOMNamedNodeMapPtr pDOMWaitAtrributes;
    pDOMWaitAtrributes = pIDomNode->Getattributes();
    MSXML2::IXMLDOMNodePtr pIDOMChildNode;
    //bstrNodeName = L"purpose";
    bstrNodeName.Assign(SysAllocString(CT2W("purpose")));
    pIDOMChildNode = pDOMWaitAtrributes->getNamedItem(bstrNodeName);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    m_ouData.m_omPurpose = strCopyBSTRToCString(NodeValue);
    pIDomNode->get_nodeTypedValue(&NodeValue);
    CString strTemp;
    strTemp = strCopyBSTRToCString(NodeValue);
    //W4 Removal - Type Conversion No problem.
    m_ouData.m_ushDuration = (USHORT)atoi((LPCSTR)strTemp);
    pIDOMChildNode.Release();
    pDOMWaitAtrributes.Release();
    return S_OK;
}

/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType, void* pvEntityData
Output         :  HRESULT
Functionality  :
Member of      :  CWaitEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CWaitEntity::GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == WAIT && pvEntityData != NULL)
    {
        *((CWaitEntityData*)pvEntityData) = m_ouData;
        return S_OK;
    }

    return S_FALSE;
}

/******************************************************************************
Function Name  :  GetEntityType
Input(s)       :  -
Output         :  eTYPE_ENTITY
Functionality  :  returns the entity type
Member of      :  CWaitEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
eTYPE_ENTITY CWaitEntity::GetEntityType(void)
{
    return m_eType;
}

/******************************************************************************
Function Name  :  SetData
Input(s)       :  MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode
Output         :  HRESULT
Functionality  :
Member of      :  CWaitEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CWaitEntity::SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode)
{
    CString omstrTemp;
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");

    if (pIDOMDoc != NULL)
    {
        MSXML2::IXMLDOMElementPtr pIDomWaitNode =  pIDOMDoc->createElement(_bstr_t(def_STR_WAIT_NODE));
        MSXML2::IXMLDOMAttributePtr pIDomTSAtrrib = pIDOMDoc->createAttribute(def_STR_STTRIB_PURPOSE);

        if(pIDomTSAtrrib!= NULL)
        {
            pIDomTSAtrrib->value = _bstr_t(m_ouData.m_omPurpose);
            pIDomWaitNode->setAttributeNode(pIDomTSAtrrib);
        }

        omstrTemp.Format("%d", m_ouData.m_ushDuration);
        pIDomWaitNode->Puttext(_bstr_t(omstrTemp));
        pIDomTestCaseNode->appendChild(pIDomWaitNode);
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :
Member of      :  CWaitEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CWaitEntity::SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == WAIT && pvEntityData != NULL)
    {
        m_ouData = *((CWaitEntityData*)pvEntityData);
        return S_OK;
    }

    return S_FALSE;
}


/******************************************************************************
Function Name  :  CWaitEntityData
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CWaitEntityData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CWaitEntityData::CWaitEntityData(VOID)
{
    m_omPurpose = "Waiting for";
    m_ushDuration = 100;
}


/******************************************************************************
Function Name  :  ~CWaitEntityData
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CWaitEntityData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CWaitEntityData::~CWaitEntityData()
{
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  const CWaitEntityData& RefObj
Output         :  CWaitEntityData&
Functionality  :  = operator overloading
Member of      :  CWaitEntityData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CWaitEntityData& CWaitEntityData::operator=(const CWaitEntityData& RefObj)
{
    m_omPurpose = RefObj.m_omPurpose;
    m_ushDuration = RefObj.m_ushDuration;
    return  *this;
}

/******************************************************************************
Function Name  :  CReplayEntity
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CReplayEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CReplayEntity::CReplayEntity(void)
{
    m_eType = REPLAY;
}

/******************************************************************************
Function Name  :  ~CReplayEntity
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CReplayEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CReplayEntity::~CReplayEntity()
{
}

/******************************************************************************
Function Name  :  CReplayEntity
Input(s)       :  CReplayEntity& RefObj
Output         :  -
Functionality  :  copy constructor
Member of      :  CReplayEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CReplayEntity::CReplayEntity(const CReplayEntity& RefObj)
{
    m_omFilePath = RefObj.m_omFilePath;
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CReplayEntity& RefObj
Output         :  CReplayEntity&
Functionality  :  = operator overloading
Member of      :  CReplayEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CReplayEntity& CReplayEntity::operator=(CReplayEntity& RefObj)
{
    m_omFilePath = RefObj.m_omFilePath;
    return *this;
}

/******************************************************************************
Function Name  :  GetData
Input(s)       :  MSXML2::IXMLDOMNodePtr& pIDomNode
Output         :  HRESULT
Functionality  :
Member of      :  CReplayEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CReplayEntity::GetData(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    CComVariant NodeValue;
    pIDomNode->get_nodeTypedValue(&NodeValue);
    m_omFilePath = strCopyBSTRToCString(NodeValue);
    m_omFilePath.Remove('\"');
    return S_OK;
}

/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType - Entity Type
                  void* pvEntityData
Output         :  HRESULT
Functionality  :  Return the entity data
Member of      :  CReplayEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CReplayEntity::GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == REPLAY && pvEntityData != NULL)
    {
        *((CString*)pvEntityData) =  m_omFilePath;
        return S_OK;
    }

    return S_FALSE;
}

/******************************************************************************
Function Name  :  GetEntityType
Input(s)       :  -
Output         :  eTYPE_ENTITY
Functionality  :  returns the entity type
Member of      :  CReplayEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
eTYPE_ENTITY CReplayEntity::GetEntityType(void)
{
    return m_eType;
}

/******************************************************************************
Function Name  :  SetData
Input(s)       :  MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode
Output         :  HRESULT
Functionality  :
Member of      :  CReplayEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CReplayEntity::SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode)
{
    CString omstrTemp;
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");

    if (pIDOMDoc != NULL)
    {
        MSXML2::IXMLDOMElementPtr pIDomWaitNode =  pIDOMDoc->createElement(_bstr_t(def_STR_REPLAY_NODE));
        pIDomWaitNode->Puttext(_bstr_t(m_omFilePath));
        pIDomTestCaseNode->appendChild(pIDomWaitNode);
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType - entity type
                  void* pvEntityData
Output         :  HRESULT
Functionality  :  Sets the entity data from the pvEntityData
Member of      :  CReplayEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS009
******************************************************************************/
HRESULT CReplayEntity::SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == REPLAY && pvEntityData != NULL)
    {
        m_omFilePath = *((CString*)pvEntityData);
        return S_OK;
    }

    return S_FALSE;
}


/******************************************************************************
Function Name  :  CTestCaseData
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CTestCaseData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  CS025
******************************************************************************/
CTestCaseData::CTestCaseData(void)
{
    m_omTitle = "Untitled TestCase";
    m_omID = "000";
    m_eExcpAction = CONTINUE;
    m_odTAEntityList.RemoveAll();
}


/******************************************************************************
Function Name  :  ~CTestCaseData
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CTestCaseData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestCaseData::~CTestCaseData()
{
}

/******************************************************************************
Function Name  :  vRemoveAll
Input(s)       :  -
Output         :  void
Functionality  :  Removes all entries
Member of      :  CTestCaseData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
void CTestCaseData::vRemoveAll(void)
{
    INT nCount = (INT)m_odTAEntityList.GetCount();

    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_odTAEntityList.FindIndex(i);
        CBaseEntityTA* pEntity = m_odTAEntityList.GetAt(pos);
        delete pEntity;
    }

    m_odTAEntityList.RemoveAll();
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CTestCaseData& RefObj
Output         :  CTestCaseData&
Functionality  :  = operator overloading
Member of      :  CTestCaseData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestCaseData& CTestCaseData::operator=(const CTestCaseData& RefObj)
{
    m_eExcpAction = RefObj.m_eExcpAction;
    m_omID = RefObj.m_omID;
    m_omTitle = RefObj.m_omTitle;
    m_odTAEntityList.RemoveAll();
    INT nCount = (INT)RefObj.m_odTAEntityList.GetCount();

    for(int i=0; i<nCount; i++)
    {
        POSITION pos = RefObj.m_odTAEntityList.FindIndex(i);
        CBaseEntityTA* pEntity = RefObj.m_odTAEntityList.GetAt(pos);
        m_odTAEntityList.AddTail(pEntity);
    }

    return  *this;
}
