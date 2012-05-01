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
 * \file      VerifyResponse.cpp
 * \brief     This File Contains CVerfiyEntity Class Definitions
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This File Contains CVerfiyEntity Class Definitions
 */


#include "TestSetupEditorLib_stdafx.h"
#include "VerifyResponse.h"

/******************************************************************************
Function Name  :  CVerifyResponse
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyResponse::CVerifyResponse(void)
{
    m_eType = VERIFYRESPONSE;
}


/******************************************************************************
Function Name  :  ~CVerifyResponse
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyResponse::~CVerifyResponse(void)
{
}

/******************************************************************************
Function Name  :  CVerifyResponse
Input(s)       :  CVerifyResponse& RefObj
Output         :  -
Functionality  :
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyResponse::CVerifyResponse(const CVerifyResponse& RefObj)
{
    m_ouData = RefObj.m_ouData;
    m_ushDuration = RefObj.m_ushDuration;
    m_ouVerifyMsgMap.RemoveAll();
    INT nCount = (INT)m_ouData.m_odVerify_MessageEntityList.GetCount();

    for(int i = 0; i < nCount; i++)
    {
        CVerify_MessageData ouVerifyData;
        POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(i);
        CVerify_MessageEntity& ouVerifyEntity = m_ouData.m_odVerify_MessageEntityList.GetAt(pos);
        ouVerifyEntity.GetEntityData(VERIFY_MESSAGE, &ouVerifyData);
        m_ouVerifyMsgMap[ouVerifyData.m_dwMessageID] = ouVerifyData;
    }
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CVerifyResponse& RefObj
Output         :  -
Functionality  :
Member of      :  CVerifyResponse&
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyResponse& CVerifyResponse::operator= (CVerifyResponse& RefObj)
{
    m_ouData = RefObj.m_ouData;
    m_ushDuration = RefObj.m_ushDuration;
    m_ouVerifyMsgMap.RemoveAll();
    INT nCount = (INT)m_ouData.m_odVerify_MessageEntityList.GetCount();

    for(int i = 0; i < nCount; i++)
    {
        CVerify_MessageData ouVerifyData;
        POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(i);
        CVerify_MessageEntity& ouVerifyEntity = m_ouData.m_odVerify_MessageEntityList.GetAt(pos);
        ouVerifyEntity.GetEntityData(VERIFY_MESSAGE, &ouVerifyData);
        m_ouVerifyMsgMap[ouVerifyData.m_dwMessageID] = ouVerifyData;
    }

    return *this;
}


/******************************************************************************
Function Name  :  GetData
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerifyResponse::GetData(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    CVerifyEntity::GetData(pIDomNode);
    _bstr_t bstrNodeName = def_STR_VERIFYMSG_NODE;
    CComVariant NodeValue;
    MSXML2::IXMLDOMNamedNodeMapPtr pDOMVerifyAtrributes;
    MSXML2::IXMLDOMNodePtr pIDOMChildNode;
    pDOMVerifyAtrributes = pIDomNode->Getattributes();
    //bstrNodeName = def_STR_ATTRIB_WAITFOR;
    bstrNodeName.Assign(SysAllocString(CT2W(def_STR_ATTRIB_WAITFOR)));
    pIDOMChildNode = pDOMVerifyAtrributes->getNamedItem(bstrNodeName);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    CString strTemp;
    strTemp = strCopyBSTRToCString(NodeValue);
    //W4 Removal
    m_ushDuration = (USHORT)atoi((LPCSTR)strTemp);
    INT nCount = (INT)m_ouData.m_odVerify_MessageEntityList.GetCount();

    for(int i = 0; i < nCount; i++)
    {
        CVerify_MessageData ouVerifyData;
        POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(i);
        CVerify_MessageEntity& ouVerifyEntity = m_ouData.m_odVerify_MessageEntityList.GetAt(pos);
        ouVerifyEntity.GetEntityData(VERIFY_MESSAGE, &ouVerifyData);
        m_ouVerifyMsgMap[ouVerifyData.m_dwMessageID] = ouVerifyData;
    }

    return S_OK;
}
/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerifyResponse::GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == VERIFYRESPONSE && pvEntityData != NULL)
    {
        ((CVerifyResponseData*)pvEntityData)->m_VerifyData = m_ouData;
        ((CVerifyResponseData*)pvEntityData)->m_ushDuration = m_ushDuration;
    }

    return  S_OK;
}
/******************************************************************************
Function Name  :  GetEntityType
Input(s)       :
Output         :  eTYPE_ENTITY
Functionality  :
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
eTYPE_ENTITY CVerifyResponse::GetEntityType(void)
{
    return m_eType;
}

/******************************************************************************
Function Name  :  SetData
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerifyResponse::SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode)
{
    CString omstrTemp;
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");
    INT lCount = (INT)m_ouData.m_odVerify_MessageEntityList.GetCount();
    MSXML2::IXMLDOMElementPtr pIDomSendNode =  pIDOMDoc->createElement(_bstr_t(def_STR_VERIFYRES_NODE));
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

    MSXML2::IXMLDOMAttributePtr pIDomTSAtrribWait = pIDOMDoc->createAttribute(def_STR_ATTRIB_WAITFOR);

    if(pIDomTSAtrribWait!= NULL)
    {
        CString omstrWaitFor;
        omstrWaitFor.Format("%d", m_ushDuration);
        pIDomTSAtrribWait->value = _bstr_t(omstrWaitFor);
        pIDomSendNode->setAttributeNode(pIDomTSAtrribWait);
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
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :
******************************************************************************/
HRESULT CVerifyResponse::SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == VERIFYRESPONSE && pvEntityData != NULL)
    {
        m_ouData = ((CVerifyResponseData*)pvEntityData)->m_VerifyData;
        m_ushDuration = ((CVerifyResponseData*)pvEntityData)->m_ushDuration;
    }

    return  S_OK;
}
/******************************************************************************
Function Name  :  GetMessageFromId
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  25/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CVerifyResponse::GetMessageFromId(UINT& unId, CVerify_MessageData* ouData)
{
    //W4 Removal
    UINT nCount = (UINT)m_ouData.m_odVerify_MessageEntityList.GetCount();

    for(UINT i=0; i<nCount; i++)
    {
        POSITION pos = m_ouData.m_odVerify_MessageEntityList.FindIndex(i);

        if(pos != NULL)
        {
            CVerify_MessageEntity& VerifyMsgentity = (m_ouData.m_odVerify_MessageEntityList.GetAt(pos));

            if(unId == VerifyMsgentity.GetMsgID())
            {
                VerifyMsgentity.GetEntityData(VERIFY_MESSAGE, ouData);
                return  S_OK;
            }
        }
    }

    return S_FALSE;
}
/******************************************************************************
Function Name  :  ValidateEntity
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerifyResponse
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  13/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CVerifyResponse::ValidateEntity(CString& omStrResult)
{
    UINT unVerifyMessageCount;
    GetSubEntryCount(unVerifyMessageCount);

    if(unVerifyMessageCount <= 0)
    {
        omStrResult += _T("Error: Has No Verify Messages\r\n");
        return ERR_VALID_ERROR;
    }

    return ERR_VALID_SUCCESS;
}