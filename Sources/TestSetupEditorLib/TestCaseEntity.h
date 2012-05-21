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
 * \file      TestCaseEntity.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "BaseEntityTA.h"

//////////////////////////////////////////////////
//class CTestCaseData Declaration
//////////////////////////////////////////////////
typedef CList<CBaseEntityTA*, CBaseEntityTA*&> CTAEntityList;
class CTestCaseData
{
public:
    CTestCaseData(void);
    void vRemoveAll(void);
    CTestCaseData& operator=(const CTestCaseData& RefObj);
    virtual ~CTestCaseData(void);

    //Attributes
public:
    //Exception Handle
    eACTION_EXCP m_eExcpAction;
    //TestCase List
    CTAEntityList m_odTAEntityList;
    //Test Case ID
    CString m_omID;
    //Test Case Title
    CString m_omTitle;
};

//////////////////////////////////////////////////
//class CTestCaseEntity Declaration
//////////////////////////////////////////////////
class CTestCaseEntity : public CBaseEntityTA
{

public:
    CTestCaseEntity(void);
    CTestCaseEntity& operator=(const CTestCaseEntity& RefObj);
    CTestCaseEntity(const CTestCaseEntity& RefObj);
    HRESULT AddSubEntry(CBaseEntityTA* pouSubEntryObj);
    HRESULT DeleteSubEntry(CBaseEntityTA* pouSubEntryObj);
    HRESULT DeleteSubEntry(INT /*index*/);
    HRESULT GetData(MSXML2::IXMLDOMNodePtr& pIDomNode) ;
    HRESULT GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    eTYPE_ENTITY GetEntityType(void);
    const HRESULT GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity);
    HRESULT GetSubEntryCount(UINT& unTotal);
    HRESULT RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity);
    HRESULT SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
    HRESULT SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    HRESULT SetTestCaseDetails(CString& omTitle, CString& omID, eACTION_EXCP& eExcpAction);
    HRESULT GetTestCaseDetails(CString& omTitle, CString& omID, eACTION_EXCP& eExcpAction);
    HRESULT ValidateEntity(CString& omStrResult);
    void vDeleteAllEntities(void);
    virtual ~CTestCaseEntity(void);


    //Attributes
private:
    CTestCaseData m_ouData;
};

//////////////////////////////////////////////////
//class CReplayEntity Declaration
//////////////////////////////////////////////////
class CReplayEntity : public CBaseEntityTA
{
public:
    CReplayEntity(void);
    CReplayEntity(const CReplayEntity& RefObj);
    CReplayEntity& operator = (CReplayEntity& RefObj);
    virtual HRESULT GetData(MSXML2::IXMLDOMNodePtr& pIDomNode);
    virtual HRESULT GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    virtual eTYPE_ENTITY GetEntityType(void);
    virtual HRESULT SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
    virtual HRESULT SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    virtual ~CReplayEntity(void);

public:
    //Path of the replay file
    CString m_omFilePath;
};

//////////////////////////////////////////////////
//class CWaitEntityData Declaration
//////////////////////////////////////////////////
class CWaitEntityData
{
public:
    CWaitEntityData(void);
    CWaitEntityData& operator=(const CWaitEntityData& RefObj);
    virtual ~CWaitEntityData(void);

public:
    //Purpose of Waiting
    CString m_omPurpose;
    //Duration of the waiting period in millisecond
    USHORT m_ushDuration;
};

//////////////////////////////////////////////////
//class CWaitEntity Declaration
//////////////////////////////////////////////////
class CWaitEntity : public CBaseEntityTA
{

public:
    CWaitEntity(void);
    CWaitEntity(const CWaitEntity& RefObj);
    CWaitEntity& operator = (CWaitEntity& RefObj);
    virtual HRESULT GetData(MSXML2::IXMLDOMNodePtr& pIDomNode);
    virtual HRESULT GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    virtual eTYPE_ENTITY GetEntityType(void);
    virtual HRESULT SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
    virtual HRESULT SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    virtual ~CWaitEntity(void);
    //Attributes
private:
    CWaitEntityData m_ouData;
};
