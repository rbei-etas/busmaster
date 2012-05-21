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
 * \file      Send_MessageEntity.cpp
 * \brief     his File Contains Send_MessageEntity, CSend_MessageData,
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 * \req       RS_27_36 - Transmit a message
 *
 * his File Contains Send_MessageEntity, CSend_MessageData,
 */

#include "TestSetupEditorLib_stdafx.h"
#include "Send_MessageEntity.h"

/******************************************************************************
Function Name  :  CSend_MessageEntity
Input(s)       :  -
Output         :  -
Functionality  :  constructor
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSend_MessageEntity::CSend_MessageEntity(void)
{
    m_eType = SEND_MESSAGE;
}

/******************************************************************************
Function Name  :  ~CSend_MessageEntity
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSend_MessageEntity::~CSend_MessageEntity(void)
{
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CSend_MessageEntity& RefObj
Output         :  CSend_MessageEntity&
Functionality  :  = Operator Overloading
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSend_MessageEntity& CSend_MessageEntity::operator=(const CSend_MessageEntity& RefObj)
{
    m_eType = RefObj.m_eType;
    m_ouData = RefObj.m_ouData;
    return *this;
}

/******************************************************************************
Function Name  :  CSend_MessageEntity
Input(s)       :  CSend_MessageEntity& RefObj
Output         :  -
Functionality  :  CopyConstructor
Member of      :  -
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSend_MessageEntity::CSend_MessageEntity(const CSend_MessageEntity& RefObj)
{
    m_eType = RefObj.m_eType;
    m_ouData = RefObj.m_ouData;
}

/******************************************************************************
Function Name  :  GetData
Input(s)       :  MSXML2::IXMLDOMNodePtr& pIDomNode
Output         :  HRESULT
Functionality  :  Parse The Xml File
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CSend_MessageEntity::GetData(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    IXMLDOMNode* pIDOMChildNode;
    LONG lCount;
    CComBSTR bstrNodeName;
    CComVariant NodeValue;
    CString omstrTemp;
    CSignalData m_ouSignal;
    IXMLDOMNamedNodeMapPtr pIDOMAttributes;
    pIDOMAttributes = pIDomNode->Getattributes();// get_attributes((IXMLDOMNamedNodeMap**)&pIDOMAttributes);
    //Retriving Message ID
    bstrNodeName = def_STR_TCATTRIB_ID;
    pIDOMAttributes->getNamedItem(bstrNodeName, &pIDOMChildNode);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    omstrTemp = strCopyBSTRToCString(NodeValue);
    m_ouData.m_dwMessageID = atoi((LPCSTR)omstrTemp);
    pIDOMChildNode->Release();

    if(m_ouDataBaseManager.bIsValidMessageID(m_ouData.m_dwMessageID)== FALSE)
    {
        //TODO::INVALID MSG POSSIBLE ONLY WHEN THE FILE IS EDITED WITH NOTEPAD.
        return -1;
    }

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
    //Retriving Default Value of a signal
    bstrNodeName = def_STR_ATTRIIB_DEFAULT;
    pIDOMAttributes->getNamedItem(bstrNodeName, &pIDOMChildNode);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    omstrTemp = strCopyBSTRToCString(NodeValue);

    switch(m_ouData.m_eSignalUnitType)
    {
        case ENG:
            m_ouData.m_uDefaultSignalValue.m_fValue = (float)atof(omstrTemp);
            break;

        case RAW:
        default:
            m_ouData.m_uDefaultSignalValue.m_u64Value = _atoi64(omstrTemp);
            break;
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

    nRetVal = m_ouDataBaseManager.nGetMessageInfo(m_ouData.m_omMessageName, sMsg);

    if(nRetVal != S_OK)
    {
        return nRetVal;
    }

    UINT unSignalCount = sMsg.m_unNumberOfSignals;
    //UINT unTSSignalCount = lCount;
    sSIGNALS* pSig = sMsg.m_psSignals;

    for(UINT i = 0; i < unSignalCount; i++)
    {
        CSignalData ouSignalData, ouTSSignalData;
        ouSignalData.m_omSigName = pSig->m_omStrSignalName;
        ouSignalData.m_uValue = m_ouData.m_uDefaultSignalValue;

        for(int i = 0; i < lCount; i++)
        {
            pIDOMSignalList->get_item(i, &pIDOMSChildSignal);
            vRetriveSignalValue(pIDOMSChildSignal, ouTSSignalData);

            if(ouSignalData.m_omSigName == ouTSSignalData.m_omSigName)
            {
                ouSignalData.m_uValue = ouTSSignalData.m_uValue;
                break;
            }
        }

        m_ouData.m_odSignalDataList.AddTail(ouSignalData);
        pSig = pSig->m_psNextSignalList;
    }

    //////////////////////////////////////////////////////////////////////////////////////
    return S_OK;
}

/******************************************************************************
Function Name  :  vRetriveSignalValue
Input(s)       :  IXMLDOMNode* pIDOMSChildSignal
                  CSignalData& m_ouSignal
Output         :  void
Functionality  :  Retives the signal value
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
void CSend_MessageEntity::vRetriveSignalValue(IXMLDOMNode* pIDOMSChildSignal, CSignalData& m_ouSignal)
{
    CComBSTR bstrNodeName = def_NAME_NODE;
    CComVariant NodeValue;
    CString strTemp;
    IXMLDOMNamedNodeMap* pIDOMAttributes;
    IXMLDOMNode* pIDOMChildNode;
    pIDOMSChildSignal->get_attributes(&pIDOMAttributes);
    pIDOMAttributes->getNamedItem(bstrNodeName, &pIDOMChildNode);
    pIDOMChildNode->get_nodeTypedValue(&NodeValue);
    m_ouSignal.m_omSigName = strCopyBSTRToCString(NodeValue);
    pIDOMChildNode->Release();
    pIDOMAttributes->Release();
    pIDOMSChildSignal->get_nodeTypedValue(&NodeValue);
    strTemp = strCopyBSTRToCString(NodeValue);

    if(m_ouData.m_eSignalUnitType == RAW)
    {
        m_ouSignal.m_uValue.m_u64Value = _atoi64((LPCSTR)strTemp);
    }
    else
    {
        m_ouSignal.m_uValue.m_fValue = (float)atof((LPCSTR)strTemp);
    }
}

/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :  Return the Entity Data
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CSend_MessageEntity::GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == SEND_MESSAGE && pvEntityData != NULL)
    {
        if (pvEntityData != NULL)
        {
            *((CSend_MessageData*)pvEntityData) = m_ouData;
        }
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  GetEntityType
Input(s)       :  -
Output         :  eTYPE_ENTITY
Functionality  :  returns the Entity type
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
eTYPE_ENTITY CSend_MessageEntity::GetEntityType(void)
{
    return m_eType;
}

/******************************************************************************
Function Name  :  SetData
Input(s)       :  MSXML2::IXMLDOMElementPtr& pIDomSendNode
Output         :  HRESULT
Functionality  :  Create the XML document from the datastructure
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CSend_MessageEntity::SetData(MSXML2::IXMLDOMElementPtr& pIDomSendNode)
{
    CString omstrTemp;
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");
    MSXML2::IXMLDOMElementPtr pChildElement, pSubElement;

    if (pIDOMDoc != NULL)
    {
        pChildElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_SENDMSG_NODE));
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
                case RAW:
                    omstrTemp = "RAW";
                    break;

                default:
                case ENG:
                    omstrTemp = "ENG";
                    break;
            }

            pIDomTSAtrrib->value = _bstr_t(omstrTemp);
            pChildElement->setAttributeNode(pIDomTSAtrrib);
        }

        pIDomTSAtrrib = pIDOMDoc->createAttribute(def_STR_ATTRIIB_DEFAULT);

        if(pIDomTSAtrrib!= NULL)
        {
            switch(m_ouData.m_eSignalUnitType)
            {
                case ENG:
                    omstrTemp.Format("%f", m_ouData.m_uDefaultSignalValue.m_fValue);
                    break;

                default:
                case RAW:
                    omstrTemp.Format("%d", m_ouData.m_uDefaultSignalValue.m_u64Value);
                    break;
            }

            pIDomTSAtrrib->value = _bstr_t(omstrTemp);
            pChildElement->setAttributeNode(pIDomTSAtrrib);
        }

        INT lCount = (INT)m_ouData.m_odSignalDataList.GetCount();

        for(int i =0; i<lCount; i++)
        {
            POSITION pos = m_ouData.m_odSignalDataList.FindIndex(i);
            CSignalData& ouSignalData = m_ouData.m_odSignalDataList.GetAt(pos);
            pSubElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_SIGNAL_NODE));
            pIDomTSAtrrib = pIDOMDoc->createAttribute(def_NAME_NODE);
            pIDomTSAtrrib->value = _bstr_t(ouSignalData.m_omSigName);
            pSubElement->setAttributeNode(pIDomTSAtrrib);

            switch(m_ouData.m_eSignalUnitType)
            {
                case ENG:
                    omstrTemp.Format("%f", ouSignalData.m_uValue.m_fValue);
                    break;

                default:
                case RAW:
                    omstrTemp.Format("%d", ouSignalData.m_uValue.m_u64Value);
                    break;
            }

            pSubElement->Puttext((_bstr_t)omstrTemp);
            pChildElement->appendChild(pSubElement);
        }

        pIDomSendNode->appendChild(pChildElement);
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :  Sets the current entity data
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS012
******************************************************************************/
HRESULT CSend_MessageEntity::SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(m_eType == eCurrEntityType)
    {
        m_ouData = *((CSend_MessageData*)(pvEntityData));
        nUpdateSignals(m_ouData);
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

/******************************************************************************
Function Name  :  nUpdateSignals
Input(s)       :  CSend_MessageData& ouData
Output         :  INT
Functionality  :
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CSend_MessageEntity::nUpdateSignals(CSend_MessageData& ouData)
{
    sMESSAGE sMsg;

    if(m_ouDataBaseManager.nGetMessageInfo(m_ouData.m_omMessageName, sMsg) == ERR_INVALID_DATABASE)
    {
        return ERR_INVALID_DATABASE;
    }

    UINT unSignalCount = sMsg.m_unNumberOfSignals;
    UINT unTSSignalCount = (UINT)ouData.m_odSignalDataList.GetCount();
    sSIGNALS* pSig = sMsg.m_psSignals;
    CSend_MessageData ouTempSignalData;
    ouTempSignalData.m_dwMessageID = ouData.m_dwMessageID;
    ouTempSignalData.m_eSignalUnitType = ouData.m_eSignalUnitType;
    ouTempSignalData.m_omMessageName = ouData.m_omMessageName;
    ouTempSignalData.m_uDefaultSignalValue = ouData.m_uDefaultSignalValue;

    for(UINT i = 0; i < unSignalCount; i++)
    {
        CSignalData ouSignalData;
        ouSignalData.m_omSigName = pSig->m_omStrSignalName;
        ouSignalData.m_uValue = ouData.m_uDefaultSignalValue;
        ouTempSignalData.m_odSignalDataList.AddTail(ouSignalData);
        pSig = pSig->m_psNextSignalList;
    }

    for(UINT i = 0; i < unTSSignalCount; i++)
    {
        POSITION pos = ouData.m_odSignalDataList.FindIndex(i);
        CSignalData& ouTSSignalData =  ouData.m_odSignalDataList.GetAt(pos);

        for(UINT j = 0; j <unSignalCount; j++)
        {
            POSITION pos = ouTempSignalData.m_odSignalDataList.FindIndex(j);
            CSignalData& ouSignalData =  ouTempSignalData.m_odSignalDataList.GetAt(pos);

            if(ouSignalData.m_omSigName == ouTSSignalData.m_omSigName)
            {
                ouSignalData.m_uValue = ouTSSignalData.m_uValue;
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
Member of      :  CSend_MessageEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CSend_MessageEntity::ValidateEntity(CString& /*omStrResult*/)
{
    return ERR_NOT_IMPLEMENTED;
}
//Send Data Starts Here
/******************************************************************************
Function Name  :  CSendData
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CSendData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSendData::CSendData(void)
{
    m_odSend_MessageDataList.RemoveAll();
}



/******************************************************************************
Function Name  :  ~CSendData
Input(s)       :  Destructor
Output         :  -
Functionality  :
Member of      :  CSendData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSendData::~CSendData(void)
{
    m_odSend_MessageDataList.RemoveAll();
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :
Output         :  CSendData&
Functionality  :
Member of      :  CSendData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSendData& CSendData::operator=(const CSendData& RefObj)
{
    m_odSend_MessageDataList.RemoveAll();
    INT Count = (INT)RefObj.m_odSend_MessageDataList.GetCount();

    for(INT i=0; i<Count; i++)
    {
        POSITION pos = RefObj.m_odSend_MessageDataList.FindIndex(i);
        //m_odSend_MessageDataList.AddTail(RefObj.m_odSend_MessageDataList.GetAt(pos));
        CSend_MessageEntity msg = RefObj.m_odSend_MessageDataList.GetAt(pos);
        m_odSend_MessageDataList.AddTail(msg);
    }

    return  *this;
}

//Signal Data Starts Here
/******************************************************************************
Function Name  :  CSignalData
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CSignalData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSignalData::CSignalData(void)
{
    m_omSigName = "";
    m_uValue.m_u64Value = 0;
}

/******************************************************************************
Function Name  :  ~CSignalData
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CSignalData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSignalData::~CSignalData(void)
{
}


/******************************************************************************
Function Name  :  operator=
Input(s)       :  CSignalData& RefObj
Output         :  CSignalData&
Functionality  :  = Operator Overloading
Member of      :  CSignalData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSignalData& CSignalData::operator=(const CSignalData& RefObj)
{
    m_omSigName = RefObj.m_omSigName;
    m_uValue = RefObj.m_uValue;
    return *this;
}


//CSend_MessageData starts Here

/******************************************************************************
Function Name  :  CSend_MessageData
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CSend_MessageData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  CS027
******************************************************************************/
CSend_MessageData::CSend_MessageData(void)
{
    //For W4 Removal
    m_dwMessageID = (DWORD)-1;
    m_eSignalUnitType = ENG;        //CS027
    m_omMessageName = "";
    m_odSignalDataList.RemoveAll();
    m_uDefaultSignalValue.m_fValue = 0.0;
}

/******************************************************************************
Function Name  :  ~CSend_MessageData
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CSend_MessageData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSend_MessageData::~CSend_MessageData(void)
{
    m_odSignalDataList.RemoveAll();
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CSend_MessageData& RefObj
Output         :  CSend_MessageData&
Functionality  :  = operator overloading
Member of      :  CSend_MessageData
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CSend_MessageData& CSend_MessageData::operator=(const CSend_MessageData& RefObj)
{
    m_dwMessageID = RefObj.m_dwMessageID;
    m_eSignalUnitType = RefObj.m_eSignalUnitType;
    m_odSignalDataList.RemoveAll();
    //m_odSignalDataList.AddHead(&(const_cast<CSignalDataList>(RefObj.m_odSignalDataList)));
    INT Count = (INT)RefObj.m_odSignalDataList.GetCount();

    for(int i=0; i<Count; i++)
    {
        POSITION pos = RefObj.m_odSignalDataList.FindIndex(i);
        //m_odSignalDataList.AddTail(RefObj.m_odSignalDataList.GetAt(pos));
        CSignalData msg = RefObj.m_odSignalDataList.GetAt(pos);
        m_odSignalDataList.AddTail(msg);
    }

    m_omMessageName = RefObj.m_omMessageName;
    m_uDefaultSignalValue = RefObj.m_uDefaultSignalValue;
    return  *this;
}