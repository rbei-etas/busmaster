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
 * \file      Verify_MessageEntity.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "BaseEntityTA.h"
#include "TSdefinitions.h"


class CSignalCondition
{
public:
    CSignalCondition();
    virtual ~CSignalCondition();
    CString m_omCondition;
    CString m_omSigName;
};
typedef CList<CSignalCondition, CSignalCondition&> CSignalConditionList;

class CVerify_MessageData
{

public:
    CVerify_MessageData(void);
    virtual ~CVerify_MessageData(void);
    HRESULT GetSignalCondition(CString& omStrSignal, CString& omSignalCondition);

    BOOL m_bResult;
    DWORD m_dwMessageID;
    CString m_omMessageName;
    eTYPE_UNIT_SIGNAL m_eSignalUnitType;
    CSignalConditionList m_odSignalConditionList;
    CVerify_MessageData& operator=(const CVerify_MessageData& RefObj);


};
typedef CList<CVerify_MessageData, CVerify_MessageData&> CVerify_MessageDataList;

class CVerify_MessageEntity : public CBaseEntityTA
{

private:
    CVerify_MessageData m_ouData;
public:
    CVerify_MessageEntity();
    virtual ~CVerify_MessageEntity();
    CVerify_MessageEntity& operator=(const CVerify_MessageEntity& RefObj)
    {
        this->m_eType = RefObj.m_eType;
        this->m_ouData = RefObj.m_ouData;
        return *this;
    }
    CVerify_MessageEntity(const CVerify_MessageEntity& RefObj)
    {
        this->m_eType = RefObj.m_eType;
        this->m_ouData = RefObj.m_ouData;
    }
    HRESULT GetData(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    eTYPE_ENTITY GetEntityType();
    HRESULT SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
    HRESULT SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    HRESULT ValidateEntity(CString& /*omStrResult*/);
    DWORD GetMsgID()
    {
        return m_ouData.m_dwMessageID;
    }
private:
    void vRetriveConditionSignalValue(IXMLDOMNode* pIDOMSChildSignal, CSignalCondition& ouSignalCondition);
    INT vUpdateSignals(CVerify_MessageData& ouData);
};

typedef CList<CVerify_MessageEntity, CVerify_MessageEntity&> CVerify_MessageEntityList;
class CVerifyData
{

public:
    CVerifyData();
    virtual ~CVerifyData();
    eERROR_ATTRIBUTE m_eAttributeError;
    CVerify_MessageEntityList m_odVerify_MessageEntityList;
    CVerifyData& operator=(const CVerifyData& RefObj);

};
