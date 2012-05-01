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
 * \file      VerifyEntity.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "TestCaseEntity.h"
#include "Verify_MessageEntity.h"
class CVerifyEntity : public CBaseEntityTA
{
public:
    CVerifyData m_ouData;
public:
    CVerifyEntity(void);
    CVerifyEntity& operator= (CVerifyEntity& RefObj);
    CVerifyEntity(const CVerifyEntity& RefObj);
    virtual HRESULT AddSubEntry(CBaseEntityTA* pouSubEntryObj);
    virtual HRESULT DeleteSubEntry(CBaseEntityTA* pouSubEntryObj);
    virtual HRESULT DeleteSubEntry(INT index);
    HRESULT GetData(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    eTYPE_ENTITY GetEntityType(void);
    virtual const HRESULT GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity);
    virtual HRESULT GetSubEntryCount(UINT& unTotal);
    virtual HRESULT RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity);
    HRESULT SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
    HRESULT SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    HRESULT ValidateEntity(CString& omStrResult);
    virtual ~CVerifyEntity(void);
};
