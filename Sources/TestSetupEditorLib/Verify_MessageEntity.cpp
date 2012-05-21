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
 * \file      Verify_MessageEntity.cpp
 * \brief     This File Contains Verify_MessageEntity, CVerify_MessageData,
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This File Contains Verify_MessageEntity, CVerify_MessageData,
 */
#include "TestSetupEditorLib_stdafx.h"
#include "Verify_MessageEntity.h"

/******************************************************************************
Function Name  :  CVerify_MessageEntity
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerify_MessageEntity::CVerify_MessageEntity(void)
{
    m_eType = VERIFY_MESSAGE;
}

/******************************************************************************
Function Name  :  ~CVerify_MessageEntity
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerify_MessageEntity::~CVerify_MessageEntity(void)
{
}

/******************************************************************************
Function Name  :  GetData
Input(s)       :  MSXML2::IXMLDOMNodePtr& pIDomNode
Output         :  HRESULT
Functionality  :  Fill the datastructure from xml node pIDomNode
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerify_MessageEntity::GetData(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    IXMLDOMNode* pIDOMChildNode;
    LONG lCount;
    CComBSTR bstrNodeName;
    CComVariant NodeValue;
    CString omstrTemp;
    CSignalCondition ouSignalCondition;
    IXMLDOMNamedNodeMapPtr pIDOMAttributes;
    pIDOMAttributes = pIDomNode->Getattributes();// get_attributes((IXMLDOMNamedNodeMap**)&pIDOMAttributes);
    //Retriving Message ID
    bstrNodeName = def_STR_TCATTRIB_ID;
    pIDOMAttributes->getNamedItem(bstrNodeName, &pIDOMChildNode);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    omstrTemp = strCopyBSTRToCString(NodeValue);
    m_ouData.m_dwMessageID = atoi((LPCSTR)omstrTemp);
    m_ouData.m_omMessageName = omstrTemp;

    if(m_ouDataBaseManager.bIsValidMessageID(m_ouData.m_dwMessageID)== FALSE)
    {
        //TODO::INVALID MSG POSSIBLE ONLY WHEN THE FILE IS EDITED WITH NOTEPAD.
        return -1;
    }

    pIDOMChildNode->Release();
    //Retriving Message UNIT
    bstrNodeName = def_STR_TCATTRIB_UNIT;
    pIDOMAttributes->getNamedItem(bstrNodeName, &pIDOMChildNode);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    omstrTemp = strCopyBSTRToCString(NodeValue);

    if(omstrTemp == "RAW")
    {
        m_ouData.m_eSignalUnitType = RAW;
    }
    else                                    // else if(omstrTemp == "ENG")
    {
        m_ouData.m_eSignalUnitType = ENG;
    }

    pIDOMChildNode->Release();
    //Retriving Signals and there Data
    sMESSAGE sMsg;
    IXMLDOMNodeListPtr pIDOMSignalList;
    IXMLDOMNode* pIDOMSChildSignal;
    bstrNodeName = def_STR_SIGNAL_NODE;
    pIDOMSignalList = pIDomNode->selectNodes((_bstr_t)bstrNodeName);
    pIDOMSignalList->get_length(&lCount);
    INT nRetVal;
    nRetVal = m_ouDataBaseManager.nGetMessageName(m_ouData.m_dwMessageID, m_ouData.m_omMessageName);

    if(nRetVal != S_OK)
    {
        return nRetVal;
    }

    nRetVal =  m_ouDataBaseManager.nGetMessageInfo(m_ouData.m_omMessageName, sMsg);

    if(nRetVal != S_OK)
    {
        return nRetVal;
    }

    UINT unSignalCount = sMsg.m_unNumberOfSignals;
    //W4 Removal - Local variable not required
    //UINT unTSSignalCount = lCount;
    sSIGNALS* pSig = sMsg.m_psSignals;

    for(UINT i = 0; i < unSignalCount; i++)
    {
        CSignalCondition ouSignalData, ouTSSignalData;
        ouSignalData.m_omSigName = pSig->m_omStrSignalName;
        ouSignalData.m_omCondition = "";

        for(int i = 0; i < lCount; i++)
        {
            pIDOMSignalList->get_item(i, &pIDOMSChildSignal);
            vRetriveConditionSignalValue(pIDOMSChildSignal, ouTSSignalData);

            if(ouSignalData.m_omSigName == ouTSSignalData.m_omSigName)
            {
                ouSignalData.m_omCondition = ouTSSignalData.m_omCondition;
                break;
            }
        }

        m_ouData.m_odSignalConditionList.AddTail(ouSignalData);
        pSig = pSig->m_psNextSignalList;
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  vRetriveConditionSignalValue
Input(s)       :  IXMLDOMNode* pIDOMSChildSignal
                  CSignalCondition& ouSignalCondition
Output         :  void
Functionality  :  Retrives the Signal Info from the pIDOMSChildSignal
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
void CVerify_MessageEntity::vRetriveConditionSignalValue(IXMLDOMNode* pIDOMSChildSignal, CSignalCondition& ouSignalCondition)
{
    CComBSTR bstrNodeName = L"name";
    CComVariant NodeValue;
    CString omstrTemp;
    IXMLDOMNamedNodeMap* pIDOMAttributes;
    IXMLDOMNode* pIDOMChildNode;
    pIDOMSChildSignal->get_attributes(&pIDOMAttributes);
    pIDOMAttributes->getNamedItem(bstrNodeName, &pIDOMChildNode);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    ouSignalCondition.m_omSigName = strCopyBSTRToCString(NodeValue);
    pIDOMChildNode->Release();
    pIDOMAttributes->Release();
    pIDOMSChildSignal->get_nodeTypedValue(&NodeValue);
    ouSignalCondition.m_omCondition = strCopyBSTRToCString(NodeValue);
    ouSignalCondition.m_omCondition.Remove('\"');
}

/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :  Returns rhe entity Data
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerify_MessageEntity::GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == VERIFY_MESSAGE && pvEntityData != NULL)
    {
        if (pvEntityData != NULL)
        {
            *((CVerify_MessageData*)pvEntityData) = m_ouData;
        }
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  GetEntityType
Input(s)       :  -
Output         :  eTYPE_ENTITY
Functionality  :  Returns the entity Type
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
eTYPE_ENTITY CVerify_MessageEntity::GetEntityType(void)
{
    return m_eType;
}

/******************************************************************************
Function Name  :  SetData
Input(s)       :  MSXML2::IXMLDOMElementPtr& pIDomVerifyNode
Output         :  HRESULT
Functionality  :  Creates the XML file with pIDomVerifyNode
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerify_MessageEntity::SetData(MSXML2::IXMLDOMElementPtr& pIDomVerifyNode)
{
    CString omstrTemp;
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");
    MSXML2::IXMLDOMElementPtr pChildElement, pSubElement;

    if (pIDOMDoc != NULL)
    {
        pChildElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_VERIFYMSG_NODE));
        MSXML2::IXMLDOMAttributePtr pIDomTSAtrrib = pIDOMDoc->createAttribute(def_STR_TCATTRIB_ID);

        if(pIDomTSAtrrib!= NULL)
        {
            pIDomTSAtrrib->value = _bstr_t(m_ouData.m_dwMessageID);
            pChildElement->setAttributeNode(pIDomTSAtrrib);
        }

        pIDomTSAtrrib = pIDOMDoc->createAttribute(def_STR_TCATTRIB_UNIT);

        if(pIDomTSAtrrib!= NULL)
        {
            switch(m_ouData.m_eSignalUnitType)
            {
                case ENG:
                    omstrTemp = "ENG";
                    break;

                default:
                case RAW:
                    omstrTemp = "RAW";
                    break;
            }

            pIDomTSAtrrib->value = _bstr_t(omstrTemp);
            pChildElement->setAttributeNode(pIDomTSAtrrib);
        }

        INT lCount = (INT)m_ouData.m_odSignalConditionList.GetCount();

        for(INT i =0; i<lCount; i++)
        {
            POSITION pos = m_ouData.m_odSignalConditionList.FindIndex(i);
            CSignalCondition& ouSignalData = m_ouData.m_odSignalConditionList.GetAt(pos);
            pSubElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_SIGNAL_NODE));
            pIDomTSAtrrib = pIDOMDoc->createAttribute(def_NAME_NODE);
            pIDomTSAtrrib->value = _bstr_t(ouSignalData.m_omSigName);
            pSubElement->setAttributeNode(pIDomTSAtrrib);
            pSubElement->Puttext(_bstr_t(ouSignalData.m_omCondition));
            pChildElement->appendChild(pSubElement);
        }

        pIDomVerifyNode->appendChild(pChildElement);
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :  Sets entity Data with pvEntityData
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS014
******************************************************************************/
HRESULT CVerify_MessageEntity::SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(m_eType == eCurrEntityType)
    {
        m_ouData = *((CVerify_MessageData*)(pvEntityData));
        vUpdateSignals(m_ouData);
        return S_OK;
    }

    return S_FALSE;
}

/******************************************************************************
Function Name  :  vUpdateSignals
Input(s)       :  CVerify_MessageData& ouData
Output         :  INT
Functionality  :
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CVerify_MessageEntity::vUpdateSignals(CVerify_MessageData& ouData)
{
    sMESSAGE sMsg;
    INT nRetVal = m_ouDataBaseManager.nGetMessageInfo(ouData.m_omMessageName, sMsg);

    if(nRetVal != S_OK)
    {
        return nRetVal;
    }

    UINT unSignalCount = sMsg.m_unNumberOfSignals;
    UINT unTSSignalCount = (UINT)ouData.m_odSignalConditionList.GetCount();
    sSIGNALS* pSig = sMsg.m_psSignals;
    CVerify_MessageData ouTempSignalData;
    ouTempSignalData.m_dwMessageID = ouData.m_dwMessageID;
    ouTempSignalData.m_eSignalUnitType = ouData.m_eSignalUnitType;
    ouTempSignalData.m_omMessageName = ouData.m_omMessageName;

    for(UINT i = 0; i < unSignalCount; i++)
    {
        CSignalCondition ouSignalData;
        ouSignalData.m_omSigName = pSig->m_omStrSignalName;
        ouSignalData.m_omCondition = "";
        ouTempSignalData.m_odSignalConditionList.AddTail(ouSignalData);
        pSig = pSig->m_psNextSignalList;
    }

    for(UINT i = 0; i < unTSSignalCount; i++)
    {
        POSITION pos = ouData.m_odSignalConditionList.FindIndex(i);
        CSignalCondition& ouTSSignalData =  ouData.m_odSignalConditionList.GetAt(pos);

        for(UINT j = 0; j <unSignalCount; j++)
        {
            POSITION pos = ouTempSignalData.m_odSignalConditionList.FindIndex(j);
            CSignalCondition& ouSignalData =  ouTempSignalData.m_odSignalConditionList.GetAt(pos);

            if(ouSignalData.m_omSigName == ouTSSignalData.m_omSigName)
            {
                ouSignalData.m_omCondition = ouTSSignalData.m_omCondition;
                break;
            }
        }
    }

    ouData = ouTempSignalData;
    return 0;
}
/******************************************************************************
Function Name  :  ValidateEntity
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CVerify_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CVerify_MessageEntity::ValidateEntity(CString& /*omStrResult*/)
{
    return ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  CVerifyData
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CVerifyData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  CS029
******************************************************************************/
CVerifyData::CVerifyData(void)
{
    m_odVerify_MessageEntityList.RemoveAll();
    m_eAttributeError = ERRORS;
}


/******************************************************************************
Function Name  :  ~CVerifyData
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CVerifyData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyData::~CVerifyData(void)
{
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CVerifyData& RefObj
Output         :  CVerifyData&
Functionality  :  = operator overloading
Member of      :  CVerifyData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerifyData& CVerifyData::operator=(const CVerifyData& RefObj)
{
    m_odVerify_MessageEntityList.RemoveAll();
    m_eAttributeError = RefObj.m_eAttributeError;
    INT Count = (INT)RefObj.m_odVerify_MessageEntityList .GetCount();

    for(int i=0; i<Count; i++)
    {
        POSITION pos = RefObj.m_odVerify_MessageEntityList.FindIndex(i);
        //m_odVerify_MessageEntityList.AddTail(RefObj.m_odVerify_MessageEntityList.GetAt(pos));
        CVerify_MessageEntity msg = RefObj.m_odVerify_MessageEntityList.GetAt(pos);
        m_odVerify_MessageEntityList.AddTail(msg);
    }

    return *this;
}


/******************************************************************************
Function Name  :  CVerify_MessageData
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CVerify_MessageData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerify_MessageData::CVerify_MessageData(void)
{
    m_bResult = FALSE;
    //W4 Removal
    m_dwMessageID = (DWORD)-1;
    m_eSignalUnitType = RAW;
    m_omMessageName = "";
    m_odSignalConditionList.RemoveAll();
}


/******************************************************************************
Function Name  :  ~CVerify_MessageData
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CVerify_MessageData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerify_MessageData::~CVerify_MessageData(void)
{
}

/******************************************************************************
Function Name  :  GetSignalCondition
Input(s)       :  CString& omStrSignal
                  CString& omSignalCondition
Output         :  HRESULT
Functionality  :  Retrives the signal Condition from signal name
Member of      :  CVerify_MessageData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CVerify_MessageData::GetSignalCondition(CString& omStrSignal, CString& omSignalCondition)
{
    BOOL bResult = S_FALSE;
    INT nCount = (INT)m_odSignalConditionList.GetCount();

    for(int i = 0; i<nCount; i++)
    {
        POSITION pos = m_odSignalConditionList.FindIndex(i);
        CSignalCondition& ouCondition = m_odSignalConditionList.GetAt(pos);

        if(ouCondition.m_omSigName == omStrSignal)
        {
            omSignalCondition = ouCondition.m_omCondition;
            bResult = S_OK;
        }
    }

    return bResult;
}



/******************************************************************************
Function Name  :  operator=
Input(s)       :  CVerify_MessageData& RefObj
Output         :  CVerify_MessageData&
Functionality  :  = operator overloading
Member of      :  CVerify_MessageData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CVerify_MessageData& CVerify_MessageData::operator=(const CVerify_MessageData& RefObj)
{
    m_omMessageName = RefObj.m_omMessageName;
    m_dwMessageID = RefObj.m_dwMessageID;
    m_eSignalUnitType = RefObj.m_eSignalUnitType;
    m_odSignalConditionList.RemoveAll();
    INT Count = (INT)RefObj.m_odSignalConditionList.GetCount();

    for(int i=0; i<Count; i++)
    {
        POSITION pos = RefObj.m_odSignalConditionList.FindIndex(i);
        //m_odSignalConditionList.AddTail(RefObj.m_odSignalConditionList.GetAt(pos));
        CSignalCondition msg = RefObj.m_odSignalConditionList.GetAt(pos);
        m_odSignalConditionList.AddTail(msg);
    }

    return  *this;
}



/******************************************************************************
Function Name  :  CSignalCondition
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CSignalCondition
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSignalCondition::CSignalCondition(void)
{
    m_omCondition = "";
    m_omSigName = "";
}



/******************************************************************************
Function Name  :  ~CSignalCondition
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CSignalCondition
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSignalCondition::~CSignalCondition()
{
}