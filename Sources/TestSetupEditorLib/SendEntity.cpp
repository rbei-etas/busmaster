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
 * \file      SendEntity.cpp
 * \brief     This File Contains CSendEntity Class Definitions
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 * \req       RS_27_36 - Transmit a message
 *
 * This File Contains CSendEntity Class Definitions
 */
#include "TestSetupEditorLib_stdafx.h"
#include "SendEntity.h"



/******************************************************************************
Function Name  :  CSendEntity
Input(s)       :  -
Output         :  -
Functionality  :  Constructor 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
CSendEntity::CSendEntity(void)
{
    m_eType = SEND;
}

/******************************************************************************
Function Name  :  ~CSendEntity
Input(s)       :  -
Output         :  -
Functionality  :  Destructor 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
CSendEntity::~CSendEntity(void)
{
}

/******************************************************************************
Function Name  :  CSendEntity
Input(s)       :  CSendEntity& RefObj
Output         :  -
Functionality  :  Copy Constructor 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
CSendEntity::CSendEntity(const CSendEntity& RefObj)
{
    m_ouData = RefObj.m_ouData;
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  const CSendEntity& RefObj
Output         :  CSendEntity&
Functionality  :  = Operator overloading 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
CSendEntity& CSendEntity::operator= (const CSendEntity& RefObj)
{
    
    m_ouData = RefObj.m_ouData;
    return *this;
}

/******************************************************************************
Function Name  :  AddSubEntry
Input(s)       :  CBaseEntityTA* pouSubEntryObj
Output         :  HRESULT - S_OK If Success.
Functionality  :  Adds Send_Message Entity 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
HRESULT CSendEntity::AddSubEntry(CBaseEntityTA* pouSubEntryObj)
{
    if(pouSubEntryObj != NULL)
    {
        CSend_MessageEntity &odSend_MessageEntity = *((CSend_MessageEntity*)(pouSubEntryObj));
        m_ouData.m_odSend_MessageDataList.AddTail(odSend_MessageEntity);
        return  S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

/******************************************************************************
Function Name  :  DeleteSubEntry
Input(s)       :  CBaseEntityTA* pouSubEntryObj
Output         :  HRESULT S_OK if pouSubEntryObj is Found and deleted
                  otherwise S_FALSE
Functionality  :  Deletes a sub-entry 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
Codetag        :  CS016
******************************************************************************/
HRESULT CSendEntity::DeleteSubEntry(CBaseEntityTA* pouSubEntryObj)
{
    HRESULT hResult = S_FALSE;
    if(pouSubEntryObj != NULL)
    {
        INT nCount = (INT)m_ouData.m_odSend_MessageDataList.GetCount();
        for(INT i = 0; i < nCount; i++)
        {
            POSITION pos = m_ouData.m_odSend_MessageDataList.FindIndex(i);
            CSend_MessageEntity &pEntity = m_ouData.m_odSend_MessageDataList.GetAt(pos);
            if(pEntity.GetID() == pouSubEntryObj->GetID())
            {
                m_ouData.m_odSend_MessageDataList.RemoveAt(pos);
                hResult = S_OK;
                break;
            }
        }
    }
    return hResult;
}

/******************************************************************************
Function Name  :  DeleteSubEntry
Input(s)       :  INT index
Output         :  HRESULT
Functionality  :  Delets the subenttry item according to the Index 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
HRESULT CSendEntity::DeleteSubEntry(INT nIndex)
{
    POSITION pos = m_ouData.m_odSend_MessageDataList.FindIndex(nIndex);
    if( pos != NULL)
    {
        m_ouData.m_odSend_MessageDataList.RemoveAt(pos);
        return S_OK;
    }
    return S_FALSE;
}

/******************************************************************************
Function Name  :  GetSubEntityObj
Input(s)       :  UINT unIndex
                  CBaseEntityTA**  pouSubEntity
Output         :  HRESULT
Functionality  :  Returns a sub-entity object based on unIndex
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
const HRESULT CSendEntity::GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity)
{
    POSITION pos = m_ouData.m_odSend_MessageDataList.FindIndex(unIndex);
    if(pos != NULL)
    {
        CSend_MessageEntity &SendMsgentity = m_ouData.m_odSend_MessageDataList.GetAt(pos);
        *pouSubEntity = &SendMsgentity;
	    return  S_OK;
    }
    return  S_FALSE;
}

/******************************************************************************
Function Name  :  ValidateEntity
Input(s)       :  
Output         :  HRESULT
Functionality  :   
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  13/04/2011
Modifications  :  
Code Tag       : CS026
******************************************************************************/
HRESULT CSendEntity::ValidateEntity(CString& omStrResult)
{
    UINT unSendMessageCount;
    GetSubEntryCount(unSendMessageCount);
    if(unSendMessageCount <= 0)
    {
        omStrResult += (_T("Warning: Has No Send Messages\r\n"));
        return ERR_VALID_WARNING;               //CS026
    }
    return ERR_VALID_SUCCESS;
}


/******************************************************************************
Function Name  :  GetSubEntryCount
Input(s)       :  UINT& unTotal
Output         :  HRESULT
Functionality  :  Return the total number of sub-entries.
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
HRESULT CSendEntity::GetSubEntryCount(UINT& unTotal)
{
    unTotal = (UINT)m_ouData.m_odSend_MessageDataList.GetCount();
    return  S_OK;
}


/******************************************************************************
Function Name  :  RepositionSubEntity
Input(s)       :  CBaseEntityTA* pouRefSubEntity
                  CBaseEntityTA* pouCurrSubEntity
Output         :  HRESULT - returns the New Entity ID
Functionality  :  Reposition pouRefSubEntity after pouCurrSubEntity
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
Codetag        :  CS019
******************************************************************************/
HRESULT CSendEntity::RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity)
{
//DESIGN:: First Take copy of Data and delete it from current position and add at required place 

    UINT unCount = (UINT)m_ouData.m_odSend_MessageDataList.GetCount();
    CSend_MessageEntity ouSendEntity = *((CSend_MessageEntity*)pouRefSubEntity);
    for(UINT i=0; i<unCount; i++)
    {
        POSITION pos = m_ouData.m_odSend_MessageDataList.FindIndex(i);
        CSend_MessageEntity& ouSendMsgEntity = m_ouData.m_odSend_MessageDataList.GetAt(pos);
        if(ouSendMsgEntity.GetID() == pouRefSubEntity->GetID())
        {
            m_ouData.m_odSend_MessageDataList.RemoveAt(pos);
            break;
        }
    }
    POSITION posNew = NULL;
    if(pouCurrSubEntity == NULL)        //Insert At First Index;
    {
        posNew =  m_ouData.m_odSend_MessageDataList.AddHead(ouSendEntity);
    }
    else
    {
        for(UINT i=0; i<unCount; i++)
        {
            POSITION pos = m_ouData.m_odSend_MessageDataList.FindIndex(i);
            CSend_MessageEntity& ouSendMsgEntity = m_ouData.m_odSend_MessageDataList.GetAt(pos);
            if(ouSendMsgEntity.GetID() == pouCurrSubEntity->GetID())
            {
                posNew =  m_ouData.m_odSend_MessageDataList.InsertAfter(pos, ouSendEntity);
                break;
            }
        }
    }
    if(posNew != NULL)
    {
        CSend_MessageEntity &odSendMsgData = m_ouData.m_odSend_MessageDataList.GetAt(posNew);
        return odSendMsgData.GetID();
    }
    return S_FALSE;
}


/******************************************************************************
Function Name  :  GetData
Input(s)       :  MSXML2::IXMLDOMNodePtr& pIDomNode
Output         :  HRESULT
Functionality  :  Reads the XML node and constructs the data structure 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
HRESULT CSendEntity::GetData(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    MSXML2::IXMLDOMNodeListPtr pIDOMSendList;
    _bstr_t bstrNodeName = def_STR_SENDMSG_NODE;
    long lCount;
    
    pIDOMSendList = pIDomNode->selectNodes(bstrNodeName);
    pIDOMSendList->get_length(&lCount);
    
    for(int i = 0; i < lCount; i++)
    {
        CSend_MessageEntity odSend_MessageEntity;
        MSXML2::IXMLDOMNodePtr pIXMLDOMSendMsgEntity;
        pIXMLDOMSendMsgEntity = pIDOMSendList->Getitem(i);
        if(odSend_MessageEntity.GetData(pIXMLDOMSendMsgEntity)==S_OK)
        {
            m_ouData.m_odSend_MessageDataList.AddTail(odSend_MessageEntity);
        }
    }
    
    return  S_OK;
}

/******************************************************************************
Function Name  :  SetData
Input(s)       :  MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode
Output         :  HRESULT
Functionality  :  Creates the Xml Document 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
HRESULT CSendEntity::SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode)
{
    CString omstrTemp;
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");
    
    INT nCount = (INT)m_ouData.m_odSend_MessageDataList.GetCount();
    MSXML2::IXMLDOMElementPtr pIDomSendNode =  pIDOMDoc->createElement(_bstr_t(def_STR_SEND_NODE));
    for(INT i=0; i<nCount; i++)
    {
        POSITION pos = m_ouData.m_odSend_MessageDataList.FindIndex(i);
        CSend_MessageEntity& ouSendMsgEntity = m_ouData.m_odSend_MessageDataList.GetAt(pos);
        ouSendMsgEntity.SetData(pIDomSendNode);
    }
    pIDomTestCaseNode->appendChild(pIDomSendNode);
    return S_OK;
}

/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :  Returns The Entity Data 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
HRESULT CSendEntity::GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == SEND && pvEntityData != NULL)
    {
        *((CSendData*)pvEntityData) = m_ouData;
    }
    return  S_OK;
}

/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :  Copies the entity Data 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
Codetag        :  CS013
******************************************************************************/
HRESULT CSendEntity::SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData)
{
    if(eCurrEntityType == SEND && pvEntityData != NULL)
    {
        m_ouData = *((CSendData*)pvEntityData);
    }
    return  S_OK;
}

/******************************************************************************
Function Name  :  GetEntityType
Input(s)       :  -
Output         :  eTYPE_ENTITY
Functionality  :  Returns the Entity Type 
Member of      :  CSendEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :  
******************************************************************************/
eTYPE_ENTITY CSendEntity::GetEntityType(void)
{
    return  m_eType;
}
