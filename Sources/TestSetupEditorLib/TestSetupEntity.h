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
 * \file      TestSetupEntity.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include <atlbase.h>
#include "TestSetupHeader.h"
#include "BaseEntityTA.h"
#include "SendEntity.h"
#include "VerifyEntity.h"
#include "TestCaseEntity.h"
#include "VerifyResponse.h"

typedef CList<CTestCaseEntity,CTestCaseEntity&> CTestCaseEntityList;
class CTestSetupEntity : public CBaseEntityTA
{
private:
	// List of the test cases
	CTestCaseEntityList m_odTestCaseEntityList;
	CTestSetupHeader m_ouTestSetupHeader;
	
public:
    CString m_omstrCurrentTSFile;
    //Name of The TsetSetUp
    CString m_omstrTestSetupTitle;
    //Descriprion of TestSetUp
    CString m_omstrDescription;

//Member Functions
public:
	CTestSetupEntity();
    CTestSetupEntity& operator=(const CTestSetupEntity& RefObj);
	HRESULT AddSubEntry(CBaseEntityTA* pouSubEntryObj);
	HRESULT DeleteSubEntry(CBaseEntityTA* pouSubEntryObj);
	HRESULT GetData(MSXML2::IXMLDOMNodePtr& pIDomNode);
 	HRESULT GetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
	const HRESULT GetHeaderData(CTestSetupHeader& ouHeaderTS);
	const HRESULT GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity);
	HRESULT GetSubEntryCount(UINT& unTotal);
    HRESULT LoadFile(CString omFilePath);
	HRESULT RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity);
	HRESULT SaveFile();
	HRESULT SaveFileAs(CString omFilePath);
    HRESULT SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
	HRESULT SetEntityData(const CBaseEntityTA** pouEntity);
    HRESULT SetEntityData(eTYPE_ENTITY eCurrEntityType, void* pvEntityData);
    HRESULT ValidateEntity(CString& omStrResult);
    HRESULT UpdateTestSetup();
	UINT GetSubEntryCount();
	VOID vInitialise(void);
    BOOL FormatDOMDocument(MSXML2::IXMLDOMDocumentPtr pDoc, CString& omstrFilePath);
    VOID SetHeaderData(const CTestSetupHeader& ouHeaderTS);
    virtual ~CTestSetupEntity(void);
protected:
    eTYPE_ENTITY GetEntityType(void);

private:
    INT nParseTSFile(CString omstrTSFile);
    INT nLoadHeader(MSXML2::IXMLDOMNodePtr& pIDOMHeaderNode);
    INT nLoadTestCases(MSXML2::IXMLDOMNodePtr& pIDomNode);
    INT nCreateNewTestSetup(void);	
    
    INT nSaveTestCases(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode);
    INT nSaveHeader(MSXML2::IXMLDOMElementPtr& pIDOMDoc, CString& omStrFilePath);

    BOOL bAddChildToNode(MSXML2::IXMLDOMElementPtr& pNodeElement, CString omNodeName, CString omNodeval);
    void vDeleteAllEntities(void);
};
