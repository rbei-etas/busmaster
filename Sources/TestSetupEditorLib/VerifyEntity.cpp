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
 * \file      VerifyEntity.cpp
 * \brief     This File Contains CVerfiyEntity Class Definitions
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This File Contains CVerfiyEntity Class Definitions
 */


#include "TestSetupEditorLib_stdafx.h"
#include "VerifyEntity.h"

/******************************************************************************
Function Name  :  CVerifyEntity
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyEntity::CVerifyEntity(void)
{
    m_eType = VERIFY;
}


/******************************************************************************
Function Name  :  ~CVerifyEntity
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyEntity::~CVerifyEntity(void)
{
    m_ouData.m_odVerify_MessageEntityList.RemoveAll();
}

/******************************************************************************
Function Name  :  CVerifyEntity
Input(s)       :  CVerifyEntity& RefObj
Output         :  -
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyEntity::CVerifyEntity(const CVerifyEntity& RefObj)
{
    m_ouData = RefObj.m_ouData;
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CVerifyEntity& RefObj
Output         :  -
Functionality  :
Member of      :  CVerifyEntity&
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyEntity& CVerifyEntity::operator= (CVerifyEntity& RefObj)
{
    m_ouData = RefObj.m_ouData;
    return *this;
}


/******************************************************************************
Function Name  :  AddSubEntry
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerifyEntity::AddSubEntry(CBaseEntityTA* pouSubEntryObj)
{
    CVerify_MessageEntity& odVerify_MessageEntity = *((CVerify_MessageEntity*)(pouSubEntryObj));
    m_ouData.m_odVerify_MessageEntityList.AddTail(odVerify_MessageEntity);
    return  S_OK;
}


/******************************************************************************
Function Name  :  DeleteSubEntry
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS017
******************************************************************************/
HRESULT CVerifyEntity::DeleteSubEntry(CBaseEntityTA* pouSubEntryObj)
{
    if(pouSubEntryObj!= NULL)
    {
        INT nCount = (INT)m_ouData.m_odVerify_MessageEntityList.GetCount();

        for(int i = 0; i < nCount; i++)
        {
            POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(i);
            CVerify_MessageEntity& pEntity = m_ouData.m_odVerify_MessageEntityList.GetAt(pos);

            if(pEntity.GetID() == pouSubEntryObj->GetID())
            {
                m_ouData.m_odVerify_MessageEntityList.RemoveAt(pos);
                return S_OK;
            }
        }
    }

    return S_FALSE;
}

/******************************************************************************
Function Name  :  DeleteSubEntry
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerifyEntity::DeleteSubEntry(INT index)
{
    POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(index);
    m_ouData.m_odVerify_MessageEntityList.RemoveAt(pos);
    return S_OK;
}

/******************************************************************************
Function Name  :  GetSubEntityObj
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
const HRESULT CVerifyEntity::GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity)
{
    POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(unIndex);

    if(pos != NULL)
    {
        CVerify_MessageEntity& VerifyMsgentity = (m_ouData.m_odVerify_MessageEntityList.GetAt(pos));
        *pouSubEntity = &VerifyMsgentity;
        return  S_OK;
    }

    return ERR_WRONG_INDEX;
}

/******************************************************************************
Function Name  :  GetSubEntryCount
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerifyEntity::GetSubEntryCount(UINT& unTotal)
{
    unTotal = (UINT)m_ouData.m_odVerify_MessageEntityList.GetCount();
    return  S_OK;
}


/******************************************************************************
Function Name  :  RepositionSubEntity
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS020
******************************************************************************/
HRESULT CVerifyEntity::RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity)
{
    UINT unCount = (INT)m_ouData.m_odVerify_MessageEntityList.GetCount();
    CVerify_MessageEntity ouSendEntity = *((CVerify_MessageEntity*)pouRefSubEntity);

    for(UINT i=0; i<unCount; i++)
    {
        POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(i);
        CVerify_MessageEntity& ouSendMsgEntity = m_ouData.m_odVerify_MessageEntityList.GetAt(pos);

        if(ouSendMsgEntity.GetID() == pouRefSubEntity->GetID())
        {
            m_ouData.m_odVerify_MessageEntityList.RemoveAt(pos);
            break;
        }
    }

    POSITION posNew = NULL;

    if(pouCurrSubEntity == NULL)        //Insert At First Index;
    {
        posNew =  m_ouData.m_odVerify_MessageEntityList.AddHead(ouSendEntity);
    }
    else
    {
        for(UINT i=0; i<unCount; i++)
        {
            POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(i);
            CVerify_MessageEntity& ouSendMsgEntity = m_ouData.m_odVerify_MessageEntityList.GetAt(pos);

            if(ouSendMsgEntity.GetID() == pouCurrSubEntity->GetID())
            {
                posNew =  m_ouData.m_odVerify_MessageEntityList.InsertAfter(pos, ouSendEntity);
                break;
            }
        }
    }

    if(posNew != NULL)
    {
        CVerify_MessageEntity& odSendMsgData = m_ouData.m_odVerify_MessageEntityList.GetAt(posNew);
        return odSendMsgData.GetID();
    }

    return S_FALSE;
}

/******************************************************************************
Function Name  :  GetData
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerifyEntity::GetData(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    _bstr_t bstrNodeName = def_STR_VERIFYMSG_NODE;
    CComVariant NodeValue;
    MSXML2::IXMLDOMNamedNodeMapPtr pDOMVerifyAtrributes;
    MSXML2::IXMLDOMNodePtr pIDOMChildNode;
    pDOMVerifyAtrributes = pIDomNode->Getattributes();
    //bstrNodeName = L"failure";
    bstrNodeName.Assign(SysAllocString(CT2W("failure")));
    pIDOMChildNode = pDOMVerifyAtrributes->getNamedItem(bstrNodeName);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    CString strTemp;
    strTemp = strCopyBSTRToCString(NodeValue);

    if(strTemp == "SUCCESS")
    {
        m_ouData.m_eAttributeError = SUCCESS;
    }
    else if(strTemp == "WARNING")
    {
        m_ouData.m_eAttributeError = WARNING;
    }
    else if(strTemp == "ERROR")
    {
        m_ouData.m_eAttributeError = ERRORS;
    }
    else
    {
        m_ouData.m_eAttributeError = FATAL;
    }

    MSXML2::IXMLDOMNodeListPtr pIDOMSendList;
    LONG lCount;
    //bstrNodeName = def_STR_VERIFYMSG_NODE;
    bstrNodeName.Assign(SysAllocString(CT2W(def_STR_VERIFYMSG_NODE)));
    pIDOMSendList = pIDomNode->selectNodes(bstrNodeName);
    pIDOMSendList->get_length(&lCount);

    for(int i = 0; i < lCount; i++)
    {
        CVerify_MessageEntity odVerify_MessageEntity;
        MSXML2::IXMLDOMNodePtr pIXMLDOMVerifyMsgEntity;
        pIXMLDOMVerifyMsgEntity = pIDOMSendList->Getitem(i);

        if(odVerify_MessageEntity.GetData(pIXMLDOMVerifyMsgEntity) == S_OK)
        {
            m_ouData.m_odVerify_MessageEntityList.AddTail(odVerify_MessageEntity);
        }
    }

    return S_OK;
}
/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerifyEntity::GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == VERIFY && pvEntityData != NULL)
    {
        *((CVerifyData*)pvEntityData) = m_ouData;
    }

    return  S_OK;
}
/******************************************************************************
Function Name  :  GetEntityType
Input(s)       :
Output         :  eTYPE_ENTITY
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
eTYPE_ENTITY CVerifyEntity::GetEntityType(void)
{
    return m_eType;
}

/******************************************************************************
Function Name  :  SetData
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerifyEntity::SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode)
{
    CString omstrTemp;
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");
    INT lCount = (INT)m_ouData.m_odVerify_MessageEntityList.GetCount();
    MSXML2::IXMLDOMElementPtr pIDomSendNode =  pIDOMDoc->createElement(_bstr_t(def_STR_VERIFY_NODE));
    MSXML2::IXMLDOMAttributePtr pIDomTSAtrrib = pIDOMDoc->createAttribute(def_STR_ATTRIB_FAIL);

    if(pIDomTSAtrrib!= NULL)
    {
        switch(m_ouData.m_eAttributeError)
        {
            case SUCCESS:
                omstrTemp = "SUCCESS";
                break;

            case WARNING:
                omstrTemp = "WARNING";
                break;

            case FATAL:
                omstrTemp = "FATAL";
                break;

            case ERRORS:
            default:
                omstrTemp = "ERRORS";
                break;
        }

        pIDomTSAtrrib->value = _bstr_t(omstrTemp);
        pIDomSendNode->setAttributeNode(pIDomTSAtrrib);
    }

    for(INT i=0; i<lCount; i++)
    {
        POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(i);
        CVerify_MessageEntity& ouVerifyMsgEntity = m_ouData.m_odVerify_MessageEntityList.GetAt(pos);
        ouVerifyMsgEntity.SetData(pIDomSendNode);
    }

    pIDomTestCaseNode->appendChild(pIDomSendNode);
    return S_OK;
}

/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS015
******************************************************************************/
HRESULT CVerifyEntity::SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == VERIFY && pvEntityData != NULL)
    {
        m_ouData = *((CVerifyData*)pvEntityData);
    }

    return  S_OK;
}

/******************************************************************************
Function Name  :  ValidateEntity
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  13/04/2011
Modifications  :
Code Tag       :  CS030
******************************************************************************/
HRESULT CVerifyEntity::ValidateEntity(CString& omStrResult)
{
    UINT unVerifyMessageCount;
    GetSubEntryCount(unVerifyMessageCount);

    if(unVerifyMessageCount <= 0)
    {
        omStrResult += (_T("Error: Has No Verify Messages\r\n"));       //CS030
        return ERR_VALID_ERROR;
    }

    return ERR_VALID_SUCCESS;
}