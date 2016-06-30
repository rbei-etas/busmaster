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
 * \file      SendEntity.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "TestCaseEntity.h"
#include "Send_MessageEntity.h"

class CSendEntity : public CBaseEntityTA
{
    //Member Functions
public:
    CSendEntity(void);
    CSendEntity(const CSendEntity& RefObj);
    CSendEntity& operator= (const CSendEntity& RefObj);
    HRESULT AddSubEntry(CBaseEntityTA* pouSubEntryObj);
    HRESULT DeleteSubEntry(CBaseEntityTA* pouSubEntryObj);
    HRESULT DeleteSubEntry(INT index);
    HRESULT GetData(MSXML2::IXMLDOMNodePtr& pIDomNode);
    HRESULT GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    HRESULT GetSubEntryCount(UINT& unTotal);
    HRESULT RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity);
    HRESULT SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
    HRESULT SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    const HRESULT GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity);
    HRESULT ValidateEntity(CString& omStrResult);
    eTYPE_ENTITY GetEntityType(void);
    virtual ~CSendEntity(void);

    //Attributes
private:
    CSendData m_ouData;
};
