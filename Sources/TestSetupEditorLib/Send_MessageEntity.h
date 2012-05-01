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
 * \file      Send_MessageEntity.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "BaseEntityTA.h"
#include "TSdefinitions.h"

/////////////////////////////////////////////////////////
//Class SignalData Declaration
/////////////////////////////////////////////////////////
class CSignalData
{
public:
    CSignalData(void);
    CSignalData& operator=(const CSignalData& RefObj);
    virtual ~CSignalData(void);

    //Atributes
public:
    CString m_omSigName;
    tagUSIGNALVALUE m_uValue;
};
typedef CList<CSignalData, CSignalData&> CSignalDataList;

/////////////////////////////////////////////////////////
//Class CSend_MessageData Declaration
/////////////////////////////////////////////////////////
class CSend_MessageData
{
public:
    CSend_MessageData(void);
    CSend_MessageData& operator=(const CSend_MessageData& RefObj);
    virtual ~CSend_MessageData(void);
public:
    //Message ID
    DWORD m_dwMessageID;
    //Message Name
    CString m_omMessageName;
    //Signal Type
    eTYPE_UNIT_SIGNAL m_eSignalUnitType;
    //Signal Data List
    CSignalDataList m_odSignalDataList;
    //Signal Vlaue
    tagUSIGNALVALUE m_uDefaultSignalValue;
};
typedef CList<CSend_MessageData, CSend_MessageData&> CSend_MessageDataList;

/////////////////////////////////////////////////////////
//Class CSend_MessageEntity Declaration
/////////////////////////////////////////////////////////
class CSend_MessageEntity : public CBaseEntityTA
{
    //Member Function
public:
    CSend_MessageEntity(void);
    CSend_MessageEntity& operator=(const CSend_MessageEntity& RefObj);
    CSend_MessageEntity(const CSend_MessageEntity& RefObj);
    HRESULT GetData(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    HRESULT SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
    HRESULT SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    HRESULT ValidateEntity(CString& /*omStrResult*/);
    eTYPE_ENTITY GetEntityType(void);
    virtual ~CSend_MessageEntity(void);
private:
    VOID vRetriveSignalValue(IXMLDOMNode* pIDOMSChildSignal, CSignalData& m_ouSignal);
    INT nUpdateSignals(CSend_MessageData& ouData);

    //Attributes
private:
    //Send Message Data
    CSend_MessageData m_ouData;
};
typedef CList<CSend_MessageEntity, CSend_MessageEntity&> CSend_MessageEntityList;

/////////////////////////////////////////////////////////
//Class SendData Declaration
/////////////////////////////////////////////////////////
class CSendData
{

public:
    CSendData(void);
    CSend_MessageEntityList m_odSend_MessageDataList;
    CSendData& operator=(const CSendData& RefObj);
    virtual ~CSendData(void);
};
