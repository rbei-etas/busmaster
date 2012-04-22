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
 * \file      VerifyResponse.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "VerifyEntity.h"

typedef CMap<UINT, UINT, CVerify_MessageData, CVerify_MessageData&> CVerifyMsgMap;

class CVerifyResponseData
{
public:
    CVerifyData m_VerifyData;
    USHORT m_ushDuration;
};

class CVerifyResponse : public CVerifyEntity
{
public:
    CVerifyMsgMap m_ouVerifyMsgMap;
    USHORT m_ushDuration;
public:
	CVerifyResponse(void);
    CVerifyResponse& operator= (CVerifyResponse& RefObj);
    CVerifyResponse(const CVerifyResponse& RefObj);
	//HRESULT AddSubEntry(CBaseEntityTA* pouSubEntryObj);
	//HRESULT DeleteSubEntry(CBaseEntityTA* pouSubEntryObj);
    //HRESULT DeleteSubEntry(INT index);
	HRESULT GetData(MSXML2::IXMLDOMNodePtr& pIDomNode);
	HRESULT GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
	eTYPE_ENTITY GetEntityType(void);
	//const HRESULT GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity);
	//HRESULT GetSubEntryCount(UINT& unTotal);
	//HRESULT RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity);
	HRESULT SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
	HRESULT SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    HRESULT GetMessageFromId(UINT& unId, CVerify_MessageData* ouData);
    HRESULT ValidateEntity(CString& omStrResult);
    virtual ~CVerifyResponse(void);
};
