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
 * \file      BaseEntityTA.h
 * \brief     Contains The CBaseEntityTA Class Declarations.
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains The CBaseEntityTA Class Declarations.
 */

#pragma once

#include <afxtempl.h>
#include "DataBaseMsgList.h"
#include "TSDefinitions.h"
#include "Include/Error.h"

class CBaseEntityTA
{
//Member function
public:
	CBaseEntityTA(void);
	virtual ~CBaseEntityTA(void);
    CBaseEntityTA(const CBaseEntityTA& RefObj);
    CBaseEntityTA& operator=(const CBaseEntityTA& RefObj);

	virtual HRESULT AddSubEntry(CBaseEntityTA* pouSubEntryObj);
	virtual HRESULT DeleteSubEntry(CBaseEntityTA* pouSubEntryObj);
    virtual HRESULT DeleteSubEntry(INT index);
    virtual HRESULT GetData(MSXML2::IXMLDOMNodePtr& /*pIDomNode*/);
	virtual HRESULT GetEntityData(eTYPE_ENTITY /*eCurrEntityType*/, void* /*pvEntityData*/);
    virtual const HRESULT GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity);
    virtual HRESULT GetSubEntryCount(UINT& unTotal);
    virtual HRESULT RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity);
    virtual HRESULT SetData(MSXML2::IXMLDOMElementPtr& /*pIDomTestCaseNode*/);
	virtual HRESULT SetEntityData(eTYPE_ENTITY /*eCurrEntityType*/, void* /*pvEntityData*/);
    virtual HRESULT RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, UINT unNewPosIndex);
    virtual HRESULT ValidateEntity(CString& omStrResult);
	virtual eTYPE_ENTITY GetEntityType(void);
    

    HRESULT SearchEntityObject(DWORD dwId, CBaseEntityTA** pEnityObject);
    HRESULT SetDatabaseFile(CString& omstrDataBasePath);
    HRESULT GetDataBaseManager(CDataBaseMsgList& ouDataBaseManager);
    static DWORD GetUniqueID(void);
    void vEnableEntity(BOOL bEnable);
    BOOL bGetEnableStatus(void);
    DWORD GetID(void);
   
//Member variables
public:
    static CDataBaseMsgList m_ouDataBaseManager;
private:
	static DWORD m_dwNextID;
protected:
    BOOL m_bEnable;
	DWORD m_dwID;
	eTYPE_ENTITY m_eType;
};
