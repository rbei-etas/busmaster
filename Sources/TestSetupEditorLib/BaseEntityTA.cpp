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
 * \file      BaseEntityTA.cpp
 * \brief     Contains The CBaseEntityTA Class Definitions.
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains The CBaseEntityTA Class Definitions.
 */
#include "TestSetupEditorLib_stdafx.h"
#include "BaseEntityTA.h"

DWORD CBaseEntityTA::m_dwNextID = 0;
CDataBaseMsgList CBaseEntityTA::m_ouDataBaseManager;

/******************************************************************************
Function Name  :  CBaseEntityTA
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CBaseEntityTA::CBaseEntityTA(void)
{
    m_dwID = GetUniqueID();
    m_bEnable = FALSE;
}

/******************************************************************************
Function Name  :  ~CBaseEntityTA
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CBaseEntityTA::~CBaseEntityTA(void)
{
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CBaseEntityTA& RefObj
Output         :  CBaseEntityTA&
Functionality  :  = operator overloading
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CBaseEntityTA& CBaseEntityTA::operator=(const CBaseEntityTA& RefObj)
{
    m_dwID = RefObj.m_dwID;
    m_dwNextID = RefObj.m_dwNextID;
    m_eType = RefObj.m_eType;
    m_bEnable = RefObj.m_bEnable;
    return *this;
}

/******************************************************************************
Function Name  :  CBaseEntityTA
Input(s)       :  CBaseEntityTA& RefObj
Output         :  -
Functionality  :  Copy Constructor
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CBaseEntityTA::CBaseEntityTA(const CBaseEntityTA& RefObj)
{
    m_dwNextID = RefObj.m_dwNextID;
    m_dwID = GetUniqueID();
    m_eType = RefObj.m_eType;
}

/******************************************************************************
Function Name  :  vEnableEntity
Input(s)       :  BOOL bEnable - TRUE to Enable
                               - FALSE to Disable
Output         :  void
Functionality  :  Enable or Disable the execution of the entity
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
void CBaseEntityTA::vEnableEntity(BOOL bEnable)
{
    m_bEnable = bEnable;
}

/******************************************************************************
Function Name  :  bGetEnableStatus
Input(s)       :  -
Output         :  BOOL - Returns the Enable status of the Entity
Functionality  :
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
BOOL CBaseEntityTA::bGetEnableStatus(void)
{
    return m_bEnable;
}


/******************************************************************************
Function Name  :  AddSubEntry
Input(s)       :  CBaseEntityTA* pouSubEntryObj
Output         :  HRESULT - ERR_NOT_IMPLEMENTED
Functionality  :  Not Implemented
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CBaseEntityTA::AddSubEntry(CBaseEntityTA* /*pouSubEntryObj*/)
{
    return  ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  DeleteSubEntry
Input(s)       :  CBaseEntityTA* pouSubEntryObj
Output         :  HRESULT - ERR_NOT_IMPLEMENTED
Functionality  :  Not Implemented
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CBaseEntityTA::DeleteSubEntry(CBaseEntityTA* /*pouSubEntryObj*/)
{
    return ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  DeleteSubEntry
Input(s)       :  INT index - Index to delete
Output         :  HRESULT - ERR_NOT_IMPLEMENTED
Functionality  :  Not Implemented
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CBaseEntityTA::DeleteSubEntry(INT /*index*/)
{
    return ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  GetID
Input(s)       :  -
Output         :  DWORD - Unique ID
Functionality  :  Return Unique ID of the Entity
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
DWORD CBaseEntityTA::GetID(void)
{
    return  m_dwID;
}

/******************************************************************************
Function Name  :  GetSubEntityObj
Input(s)       :  INT index - Index to delete
                  CBaseEntityTA**  pouSubEntity
Output         :  HRESULT - ERR_NOT_IMPLEMENTED
Functionality  :  Not Implemented
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
const HRESULT CBaseEntityTA::GetSubEntityObj(UINT /*unIndex*/, CBaseEntityTA**  /*pouSubEntity*/)
{
    return  ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  GetEntityType
Input(s)       :  void
Output         :  eTYPE_ENTITY
Functionality  :  Return the Entity Type
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
eTYPE_ENTITY CBaseEntityTA::GetEntityType(void)
{
    return BASE_ENTITY;
}

/******************************************************************************
Function Name  :  GetSubEntryCount
Input(s)       :  UINT& unTotal - Return the Subentry Count
Output         :  HRESULT
Functionality  :  Not Implemented
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CBaseEntityTA::GetSubEntryCount(UINT& /*unTotal*/)
{
    return  ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  GetUniqueID
Input(s)       :  -
Output         :  DWORD
Functionality  :  Return the Unique ID
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
DWORD CBaseEntityTA::GetUniqueID(void)
{
    return ++m_dwNextID;
}

/******************************************************************************
Function Name  :  RepositionSubEntity
Input(s)       :  CBaseEntityTA* pouRefSubEntity
                  CBaseEntityTA* pouCurrSubEntity
Output         :  HRESULT
Functionality  :  Not iumplemented
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CBaseEntityTA::RepositionSubEntity(CBaseEntityTA* /*pouRefSubEntity*/, CBaseEntityTA* /*pouCurrSubEntity*/)
{
    return  ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  RepositionSubEntity
Input(s)       :  CBaseEntityTA* pouRefSubEntity
                  CBaseEntityTA* pouCurrSubEntity
Output         :  HRESULT
Functionality  :  Not iumplemented
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CBaseEntityTA::RepositionSubEntity(CBaseEntityTA* /*pouRefSubEntity*/, UINT /*unNewPosIndex*/)
{
    return  ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  SearchEntityObject
Input(s)       :  DWORD dwId - ID of the Entity
                  CBaseEntityTA** pEnityObject - entity object
Output         :  HRESULT - S_OK if the ID is Found
Functionality  :  Retrives the entity of the Given ID.
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CBaseEntityTA::SearchEntityObject(DWORD dwId, CBaseEntityTA** pEnityObject)
{
    *pEnityObject = NULL;

    if( GetID() == dwId)
    {
        *pEnityObject = this;
    }
    else
    {
        UINT unSubEntities = 0;
        GetSubEntryCount(unSubEntities);

        for( UINT i = 0; (i < unSubEntities)&&(*pEnityObject == NULL); i++)
        {
            CBaseEntityTA* pouSubEntityObj = NULL;
            GetSubEntityObj(i, &pouSubEntityObj);
            pouSubEntityObj->SearchEntityObject(dwId, pEnityObject);
        }
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  SetDatabaseFile
Input(s)       :  CString& omstrDataBasePath - Data Base Path
Output         :  HRESULT - S_OK if Success
                  ERR_INVALID_DATABASE - If Failed
Functionality  :  Set the current database
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CBaseEntityTA::SetDatabaseFile(CString& omstrDataBasePath)
{
    if( TRUE == m_ouDataBaseManager.bFillDataStructureFromDatabaseFile(omstrDataBasePath))
    {
        return S_OK;
    }
    else
    {
        return ERR_INVALID_DATABASE;
    }
}

/******************************************************************************
Function Name  :  GetDataBaseManager
Input(s)       :  CDataBaseMsgList& ouDataBaseManager
Output         :  HRESULT
Functionality  :
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CBaseEntityTA::GetDataBaseManager(CDataBaseMsgList& ouDataBaseManager)
{
    ouDataBaseManager = m_ouDataBaseManager;
    return S_OK;
}
/******************************************************************************
Function Name  :  ValidateEntity
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  13/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CBaseEntityTA::ValidateEntity(CString& /*omStrResult*/)
{
    return ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  GetData
Input(s)       :  MSXML2::IXMLDOMNodePtr&
Output         :  HRESULT
Functionality  :
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  02/06/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CBaseEntityTA::GetData(MSXML2::IXMLDOMNodePtr& /*pIDomNode*/)
{
    return ERR_NOT_IMPLEMENTED;
}
/******************************************************************************
Function Name  :  SetData
Input(s)       :  MSXML2::IXMLDOMElementPtr&
Output         :  HRESULT
Functionality  :
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  02/06/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CBaseEntityTA::SetData(MSXML2::IXMLDOMElementPtr& /*pIDomTestCaseNode*/)
{
    return ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :  eTYPE_ENTITY
                  void*
Output         :  HRESULT
Functionality  :
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  02/06/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CBaseEntityTA::GetEntityData(eTYPE_ENTITY /*eCurrEntityType*/, void* /*pvEntityData*/)
{
    return ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :  eTYPE_ENTITY
                  void*
Output         :  HRESULT
Functionality  :
Member of      :  CBaseEntityTA
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  02/06/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CBaseEntityTA::SetEntityData(eTYPE_ENTITY /*eCurrEntityType*/, void* /*pvEntityData*/)
{
    return ERR_NOT_IMPLEMENTED;
}
