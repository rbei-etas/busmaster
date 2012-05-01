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
 * \file      TestSetupEntity.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TestSetupEditorLib_stdafx.h"
#include "TestSetupEntity.h"

/******************************************************************************
Function Name  :  CTestSetupEntity
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestSetupEntity::CTestSetupEntity()
{
    ::CoInitialize(NULL);
    m_eType = TEST_SETUP;
    m_omstrCurrentTSFile = def_EMPTYFILENAME;
}


/******************************************************************************
Function Name  :  ~CTestSetupEntity
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestSetupEntity::~CTestSetupEntity()
{
    m_odTestCaseEntityList.RemoveAll();
}

/******************************************************************************
Function Name  :  operator=
Input(s)       :  CTestSetupEntity& RefObj
Output         :  CTestSetupEntity&
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CTestSetupEntity& CTestSetupEntity::operator=(const CTestSetupEntity& RefObj)
{
    m_odTestCaseEntityList.RemoveAll();
    INT Count = (INT)RefObj.m_odTestCaseEntityList.GetCount();

    for(int i=0; i<Count; i++)
    {
        POSITION pos = RefObj.m_odTestCaseEntityList.FindIndex(i);
        //m_odTestCaseEntityList.AddTail(RefObj.m_odTestCaseEntityList.GetAt(pos));
        CTestCaseEntity msg = RefObj.m_odTestCaseEntityList.GetAt(pos);
        m_odTestCaseEntityList.AddTail(msg);
    }

    m_ouTestSetupHeader = RefObj.m_ouTestSetupHeader;
    m_omstrCurrentTSFile =  RefObj.m_omstrCurrentTSFile;
    m_omstrTestSetupTitle = RefObj.m_omstrTestSetupTitle;
    m_omstrDescription = RefObj.m_omstrDescription;
    return *this;
}


/******************************************************************************
Function Name  :  AddSubEntry
Input(s)       :  CBaseEntityTA* pouSubEntryObj
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS008
******************************************************************************/
HRESULT CTestSetupEntity::AddSubEntry(CBaseEntityTA* pouSubEntryObj)
{
    CTestCaseEntity& odTestCaseEntity = *((CTestCaseEntity*)(pouSubEntryObj));
    m_odTestCaseEntityList.AddTail(odTestCaseEntity);
    return  S_OK;
}


/******************************************************************************
Function Name  :  DeleteSubEntry
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
CodeTag        :  CS011
******************************************************************************/
HRESULT CTestSetupEntity::DeleteSubEntry(CBaseEntityTA* pouSubEntryObj)
{
    HRESULT hResult = ERR_WRONG_ID;;
    INT nCount = (INT)m_odTestCaseEntityList.GetCount();

    for(int i = 0; i < nCount; i++)
    {
        POSITION pos = m_odTestCaseEntityList.FindIndex(i);

        if(pos != NULL)
        {
            CTestCaseEntity& pEntity = m_odTestCaseEntityList.GetAt(pos);

            if(pEntity.GetID() == pouSubEntryObj->GetID())
            {
                m_odTestCaseEntityList.RemoveAt(pos);
                hResult = S_OK;
                break;
            }
        }
    }

    return hResult;
}


/******************************************************************************
Function Name  :  GetHeaderData
Input(s)       :  CTestSetupHeader& ouHeaderTS
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
const HRESULT CTestSetupEntity::GetHeaderData(CTestSetupHeader& ouHeaderTS)
{
    ouHeaderTS = m_ouTestSetupHeader;
    return S_OK;
}


/******************************************************************************
Function Name  :  GetSubEntityObj
Input(s)       :  UINT unIndex
                  CBaseEntityTA**  pouSubEntity
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
const HRESULT CTestSetupEntity::GetSubEntityObj(UINT unIndex, CBaseEntityTA**  pouSubEntity)
{
    POSITION pos =  m_odTestCaseEntityList.FindIndex(unIndex);

    if(pos != NULL)
    {
        CTestCaseEntity& ob = m_odTestCaseEntityList.GetAt(pos);
        *pouSubEntity = &ob;
        return  S_OK;
    }

    return ERR_WRONG_INDEX;
}


/******************************************************************************
Function Name  :  GetSubEntryCount
Input(s)       :  UINT& unTotal
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestSetupEntity::GetSubEntryCount(UINT& unTotal)
{
    unTotal = (UINT)m_odTestCaseEntityList.GetCount();
    return  S_OK;
}

/******************************************************************************
Function Name  :  GetSubEntryCount
Input(s)       :  -
Output         :  UINT
Functionality  :  Returns the subentry count
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
UINT CTestSetupEntity::GetSubEntryCount(void)
{
    return  (UINT)m_odTestCaseEntityList.GetCount();
}

/******************************************************************************
Function Name  :  LoadFile
Input(s)       :  CString omFilePath
Output         :  HRESULT
Functionality  :  Load the Test Setup File
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS001
******************************************************************************/
HRESULT CTestSetupEntity::LoadFile(CString omFilePath)
{
    m_omstrCurrentTSFile = omFilePath;
    return nParseTSFile(m_omstrCurrentTSFile);
}

/******************************************************************************
Function Name  :  nCreateNewTestSetup
Input(s)       :
Output         :  INT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CTestSetupEntity::nCreateNewTestSetup(void)
{
    vDeleteAllEntities();
    m_ouTestSetupHeader.InitHeaderToDefault();
    return S_OK;
}

/******************************************************************************
Function Name  :  vDeleteAllEntities
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
void CTestSetupEntity::vDeleteAllEntities(void)
{
    INT lCount = (INT)m_odTestCaseEntityList.GetCount();

    for(INT i = 0; i<lCount; i++)
    {
        POSITION pos = m_odTestCaseEntityList.FindIndex(i);
        CTestCaseEntity& pTCEntity = m_odTestCaseEntityList.GetAt(pos);
        pTCEntity.vDeleteAllEntities();
    }

    m_odTestCaseEntityList.RemoveAll();
    m_ouDataBaseManager.bFreeMessageMemory();
}


/******************************************************************************
Function Name  :  RepositionSubEntity
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS022
******************************************************************************/
HRESULT CTestSetupEntity::RepositionSubEntity(CBaseEntityTA* pouRefSubEntity, CBaseEntityTA* pouCurrSubEntity)
{
    UINT unCount = (UINT)m_odTestCaseEntityList.GetCount();
    CTestCaseEntity ouTCEntity = *((CTestCaseEntity*)pouRefSubEntity);

    for(UINT i=0; i<unCount; i++)
    {
        POSITION pos = m_odTestCaseEntityList.FindIndex(i);
        CTestCaseEntity& ouTestcaseEntity = m_odTestCaseEntityList.GetAt(pos);

        if(ouTestcaseEntity.GetID() == pouRefSubEntity->GetID())
        {
            m_odTestCaseEntityList.RemoveAt(pos);
            break;
        }
    }

    POSITION posNew = NULL;

    if(pouCurrSubEntity == NULL)        //Insert At First Index;
    {
        posNew =  m_odTestCaseEntityList.AddHead(ouTCEntity);
    }
    else
    {
        for(UINT i=0; i<unCount; i++)
        {
            POSITION pos = m_odTestCaseEntityList.FindIndex(i);
            CTestCaseEntity& ouTestcaseEntity = m_odTestCaseEntityList.GetAt(pos);

            if(ouTestcaseEntity.GetID() == pouCurrSubEntity->GetID())
            {
                posNew =  m_odTestCaseEntityList.InsertAfter(pos, ouTCEntity);
                break;
            }
        }
    }

    CTestCaseEntity& odTCData = m_odTestCaseEntityList.GetAt(posNew);
    return odTCData.GetID();
}


/******************************************************************************
Function Name  :  SaveFile
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS005
******************************************************************************/
HRESULT CTestSetupEntity::SaveFile(void)
{
    if(m_omstrCurrentTSFile != def_EMPTYFILENAME)      //We dont have a file path
    {
        SaveFileAs(m_omstrCurrentTSFile);
    }
    else
    {
        return S_FALSE;
    }

    return  S_FALSE;
}

/******************************************************************************
Function Name  :  vInitialise
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS045
******************************************************************************/
void CTestSetupEntity::vInitialise(void)
{
    m_omstrCurrentTSFile = def_EMPTYFILENAME;
    m_omstrDescription = "";
    m_omstrTestSetupTitle = "<New Test SetUp>";
    vDeleteAllEntities();
    nCreateNewTestSetup();
}

/******************************************************************************
Function Name  :  SaveFileAs
Input(s)       :  CString omFilePath
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS006
******************************************************************************/
HRESULT CTestSetupEntity::SaveFileAs(CString omFilePath)
{
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    HRESULT hr = pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");

    if ( FAILED(hr) )
    {
        return S_FALSE;
    }

    MSXML2::IXMLDOMProcessingInstructionPtr pIDomPi;
    pIDomPi = pIDOMDoc->createProcessingInstruction(def_DOM_INSTRUCTION);
    VARIANT_BOOL b = TRUE;
    pIDOMDoc->put_preserveWhiteSpace(b);

    if (pIDomPi != NULL)
    {
        pIDOMDoc->appendChild(pIDomPi);
    }

    //testSETUP Node <testsetup>
    MSXML2::IXMLDOMElementPtr pIDomTSNode = pIDOMDoc->createElement(_bstr_t(def_STR_TSNODE_NAME));
    pIDOMDoc->appendChild(pIDomTSNode);
    //Title Attribute <testsetup title = "">
    MSXML2::IXMLDOMAttributePtr pIDomTSAtrrib = pIDOMDoc->createAttribute(def_STR_TSATTRIB_TITLE);

    if(pIDomTSAtrrib!= NULL)
    {
        pIDomTSAtrrib->value = _bstr_t(m_omstrTestSetupTitle);
        pIDomTSNode->setAttributeNode(pIDomTSAtrrib);
    }

    //Descriprion Node <testsetup title = ""><description>
    MSXML2::IXMLDOMElementPtr pIDomDescriptionNode = pIDOMDoc->createElement(_bstr_t(def_STR_DESC_NODE));
    pIDomDescriptionNode->Puttext(_bstr_t(m_omstrDescription));
    pIDomTSNode->appendChild(pIDomDescriptionNode);
    //Heade Node
    MSXML2::IXMLDOMElementPtr pIDomHeaderNode = pIDOMDoc->createElement(_bstr_t(def_STR_HEADER_NAME));
    nSaveHeader(pIDomHeaderNode, omFilePath);
    pIDomTSNode->appendChild(pIDomHeaderNode);
    //TestCase List
    MSXML2::IXMLDOMElementPtr pIDomTestCaseNode = pIDOMDoc->createElement(_bstr_t(def_STR_TCLIST_NODE));
    SetData(pIDomTestCaseNode);
    pIDomTSNode->appendChild(pIDomTestCaseNode);
    FormatDOMDocument(pIDOMDoc, omFilePath);
    return S_OK;
}

/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestSetupEntity::SetEntityData(const CBaseEntityTA** /*pouEntity*/)
{
    return  ERR_NOT_IMPLEMENTED;
}
/******************************************************************************
Function Name  :  UpdateTestSetup
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  28/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTestSetupEntity::UpdateTestSetup()
{
    CString omStrFile = m_omstrCurrentTSFile;
    vInitialise();
    return LoadFile(omStrFile);
}
/******************************************************************************
Function Name  :  ValidateEntity
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  13/04/2011
Modifications  :
Code Tag       :  CS026
******************************************************************************/
HRESULT CTestSetupEntity::ValidateEntity(CString& omStrResult)
{
    UINT unCount;
    GetSubEntryCount(unCount);
    CString omStrHeaderResult;
    HRESULT hResult = ERR_VALID_SUCCESS;

    if(m_ouTestSetupHeader.ValidateEntity(omStrHeaderResult) == ERR_VALID_ERROR)
    {
        omStrResult += omStrHeaderResult;
        hResult = ERR_VALID_ERROR;
    }

    if(unCount <=0 )
    {
        omStrResult += _T("No TestCases Are Defined\r\n");
        hResult = ERR_VALID_ERROR;
    }
    else
    {
        for(UINT i=0; i<unCount; i++)
        {
            CString omStrTcResult;
            CBaseEntityTA* pTcEntity;
            GetSubEntityObj(i, &pTcEntity);

            if(pTcEntity->ValidateEntity(omStrTcResult) == ERR_VALID_ERROR)
            {
                hResult = ERR_VALID_ERROR;
            }

            omStrResult += omStrTcResult;
        }
    }

    return hResult;
}


/******************************************************************************
Function Name  :  SetHeaderData
Input(s)       :  CTestSetupHeader& ouHeaderTS
Output         :  void
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
void CTestSetupEntity::SetHeaderData(const CTestSetupHeader& ouHeaderTS)
{
    m_ouTestSetupHeader = ouHeaderTS;
}

/******************************************************************************
Function Name  :  GetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestSetupEntity::GetEntityData(eTYPE_ENTITY /*eCurrEntityType*/, void* /*pvEntityData*/)
{
    return ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  SetEntityData
Input(s)       :  eTYPE_ENTITY eCurrEntityType
                  void* pvEntityData
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestSetupEntity::SetEntityData(eTYPE_ENTITY /*eCurrEntityType*/, void* /*pvEntityData*/)
{
    return ERR_NOT_IMPLEMENTED;
}

/******************************************************************************
Function Name  :  GetData
Input(s)       :  MSXML2::IXMLDOMNodePtr& pIDomNode
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        : CS003
******************************************************************************/
HRESULT CTestSetupEntity::GetData(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    nLoadTestCases(pIDomNode);
    return S_OK;
}

/******************************************************************************
Function Name  :  SetData
Input(s)       :  MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode
Output         :  HRESULT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
HRESULT CTestSetupEntity::SetData(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode)
{
    nSaveTestCases(pIDomTestCaseNode);
    return S_OK;
}

/******************************************************************************
Function Name  :  FormatDOMDocument
Input(s)       :  MSXML2::IXMLDOMDocumentPtr pDoc
                  CString& omstrFilePath
Output         :  bool
Functionality  :  Formats the pDoc Pointer into File omStrFilePath
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
BOOL CTestSetupEntity::FormatDOMDocument(MSXML2::IXMLDOMDocumentPtr pDoc, CString& omstrFilePath)
{
    //Referance Taken From Msdn
    MSXML2::ISAXXMLReaderPtr pSaxXmlReader = NULL;
    pSaxXmlReader.CreateInstance(L"Msxml2.SAXXMLReader");
    CComPtr<IStream> pStream;
    DWORD grfMode = STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;

    //W4 Removal
    if(SHCreateStreamOnFile ((LPCTSTR)omstrFilePath, grfMode, &pStream) == S_OK)
    {
        MSXML2::IMXWriterPtr pImxWriter;
        pImxWriter.CreateInstance (L"Msxml2.MXXMLWriter");
        pImxWriter->put_output (CComVariant(pStream));
        pSaxXmlReader->putContentHandler((MSXML2::ISAXContentHandlerPtr)pImxWriter);
        pSaxXmlReader->putErrorHandler((MSXML2::ISAXErrorHandlerPtr)pImxWriter);
        pSaxXmlReader->putDTDHandler((MSXML2::ISAXDTDHandlerPtr)pImxWriter);
        pImxWriter->put_byteOrderMark(VARIANT_TRUE);
        pImxWriter->put_indent(VARIANT_TRUE);
        //W4 Removal
        pSaxXmlReader->parse((_variant_t)(pDoc.GetInterfacePtr()));
        pImxWriter->flush ();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************
Function Name  :  GetEntityType
Input(s)       :
Output         :  eTYPE_ENTITY
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
eTYPE_ENTITY CTestSetupEntity::GetEntityType()
{
    return m_eType;
}


/******************************************************************************
Function Name  :  nParseTSFile
Input(s)       :  CString omstrTSFile
Output         :  INT
Functionality  :  Load the omStrTSFille
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        : CS002
******************************************************************************/
INT CTestSetupEntity::nParseTSFile(CString omstrTSFile)
{
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    HRESULT hr = pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");

    if ( FAILED(hr) )
    {
        throw "Unable to create XML parser object";
    }

    if(PathFileExists(omstrTSFile) == FALSE)
    {
        return ERR_PATH_INCORRECT;
    }

    //Create A TestSetup DOM document
    CComVariant varFileName((LPCSTR)omstrTSFile);

    try
    {
        pIDOMDoc->load(varFileName);
        //GetTest Setup Title
        MSXML2::IXMLDOMNodePtr pIDOMTiltleNode;
        _bstr_t bstrNodeName = def_STR_TSNODE_NAME;
        CComVariant NodeValue;
        pIDOMTiltleNode = pIDOMDoc->selectSingleNode(bstrNodeName);
        MSXML2::IXMLDOMNamedNodeMapPtr pDOMTCAtrributes;
        pDOMTCAtrributes = pIDOMTiltleNode->Getattributes();
        MSXML2::IXMLDOMNodePtr pIDOMChildNode;
        //bstrNodeName = def_STR_TSATTRIB_TITLE;
        bstrNodeName.Assign(SysAllocString(CT2W(def_STR_TSATTRIB_TITLE)));
        pIDOMChildNode = pDOMTCAtrributes->getNamedItem(bstrNodeName);
        pIDOMChildNode->get_nodeTypedValue(&NodeValue);
        m_omstrTestSetupTitle = strCopyBSTRToCString(NodeValue);
        pIDOMChildNode.Release();
        //Get Description
        //bstrNodeName = def_STR_TSDESCRIPTION;
        bstrNodeName.Assign(SysAllocString(CT2W(def_STR_TSDESCRIPTION)));
        MSXML2::IXMLDOMNodePtr pIDOMDescriptionNode = pIDOMDoc->selectSingleNode(bstrNodeName);
        pIDOMDescriptionNode->get_nodeTypedValue(&NodeValue);
        m_omstrDescription = strCopyBSTRToCString(NodeValue);
        //Get Header Node
        //bstrNodeName = def_STR_TSHEADER;
        bstrNodeName.Assign(SysAllocString(CT2W(def_STR_TSHEADER)));
        MSXML2::IXMLDOMNodePtr pIDOMHeaderNode = pIDOMDoc->selectSingleNode(bstrNodeName);
        nLoadHeader(pIDOMHeaderNode);
        pIDOMHeaderNode.Release();
        //Get TestCases List
        //bstrNodeName = def_STR_TSTCLIST;
        bstrNodeName.Assign(SysAllocString(CT2W(def_STR_TSTCLIST)));
        MSXML2::IXMLDOMNodePtr pIDOMTSNode = pIDOMDoc->selectSingleNode(bstrNodeName);
        GetData(pIDOMTSNode);
    }
    catch(...)
    {
        return S_FALSE;
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  nLoadHeader
Input(s)       :  MSXML2::IXMLDOMNodePtr& pHeaderDOMNode
Output         :  INT
Functionality  :  Retrives the Header info
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :
******************************************************************************/
INT CTestSetupEntity::nLoadHeader(MSXML2::IXMLDOMNodePtr& pHeaderDOMNode)
{
    // CComPtr<IXMLDOMNode> pHeaderDOMNode;
    MSXML2::IXMLDOMNodeListPtr pXMLDOMInfoList;
    MSXML2::IXMLDOMNodePtr pInfoNode;
    MSXML2::IXMLDOMNodePtr pInfoCategoryNode;
    MSXML2::IXMLDOMNodePtr pInfoValueNode;
    _bstr_t bstrNodeName = "info";
    CComVariant NodeValue;
    pXMLDOMInfoList = pHeaderDOMNode->selectNodes(bstrNodeName);
    LONG lCount = 0;
    pXMLDOMInfoList->get_length(&lCount);
    SInfo ouTempSInfo;

    for(int i = 0; i<lCount; i++)
    {
        pInfoNode = pXMLDOMInfoList->Getitem(i);
        //bstrNodeName = def_STR_CATEGORY_NODE;
        bstrNodeName.Assign(SysAllocString(CT2W(def_STR_CATEGORY_NODE)));
        pInfoCategoryNode = pInfoNode->selectSingleNode(bstrNodeName);
        pInfoCategoryNode->get_nodeTypedValue(&NodeValue);
        ouTempSInfo.m_omCategory = strCopyBSTRToCString(NodeValue);
        pInfoCategoryNode.Release();
        //bstrNodeName = def_STR_VALUE_NODE;
        bstrNodeName.Assign(SysAllocString(CT2W(def_STR_VALUE_NODE)));
        pInfoCategoryNode = pInfoNode->selectSingleNode(bstrNodeName);
        pInfoCategoryNode->get_nodeTypedValue(&NodeValue);
        ouTempSInfo.m_omValue = strCopyBSTRToCString(NodeValue);
        pInfoCategoryNode.Release();
        pInfoNode.Release();

        if(ouTempSInfo.m_omCategory == def_STR_MODULENAME)
        {
            m_ouTestSetupHeader.m_sModuleName.m_omCategory = ouTempSInfo.m_omCategory;
            m_ouTestSetupHeader.m_sModuleName.m_omValue = ouTempSInfo.m_omValue;
        }

        if(ouTempSInfo.m_omCategory == def_STR_ENGINEERNAME)
        {
            m_ouTestSetupHeader.m_sEngineerInfo1.m_omCategory = ouTempSInfo.m_omCategory;
            m_ouTestSetupHeader.m_sEngineerInfo1.m_omValue = ouTempSInfo.m_omValue;
        }

        if(ouTempSInfo.m_omCategory == def_STR_ENGINERROLE)
        {
            m_ouTestSetupHeader.m_sEngineerInfo2.m_omCategory = ouTempSInfo.m_omCategory;
            m_ouTestSetupHeader.m_sEngineerInfo2.m_omValue = ouTempSInfo.m_omValue;
        }

        if(ouTempSInfo.m_omCategory == def_STR_VERSION)
        {
            m_ouTestSetupHeader.m_sVersion.m_omCategory = ouTempSInfo.m_omCategory;
            m_ouTestSetupHeader.m_sVersion.m_omValue = ouTempSInfo.m_omValue;
        }
    }

    //Taking Database
    //bstrNodeName = def_STR_LISTOFDB;
    bstrNodeName.Assign(SysAllocString(CT2W(def_STR_LISTOFDB)));
    pInfoNode = pHeaderDOMNode->selectSingleNode(bstrNodeName);
    pInfoNode->get_nodeTypedValue(&NodeValue);
    m_ouTestSetupHeader.m_omDatabasePath = strCopyBSTRToCString(NodeValue);
    //Set The Database Path
    SetDatabaseFile(m_ouTestSetupHeader.m_omDatabasePath);
    pInfoNode.Release();
    //bstrNodeName = def_STR_REPORT_FORMAT;
    bstrNodeName.Assign(SysAllocString(CT2W(def_STR_REPORT_FORMAT)));
    pInfoNode = pHeaderDOMNode->selectSingleNode(bstrNodeName);
    pInfoNode->get_nodeTypedValue(&NodeValue);
    CString omStrExt;
    omStrExt = strCopyBSTRToCString(NodeValue);
    pInfoNode.Release();

    if(omStrExt == "HTM")
    {
        m_ouTestSetupHeader.m_sReportFile.m_eType = HTM;
    }
    else
    {
        m_ouTestSetupHeader.m_sReportFile.m_eType = TXT;
    }

    //bstrNodeName = def_STR_REPORT_PATH;
    bstrNodeName.Assign(SysAllocString(CT2W(def_STR_REPORT_PATH)));
    pInfoNode = pHeaderDOMNode->selectSingleNode(bstrNodeName);
    pInfoNode->get_nodeTypedValue(&NodeValue);
    m_ouTestSetupHeader.m_sReportFile.m_omPath = strCopyBSTRToCString(NodeValue);
    pInfoNode.Release();
    //bstrNodeName = def_STR_REPORT_TIMEMODE;
    bstrNodeName.Assign(SysAllocString(CT2W(def_STR_REPORT_TIMEMODE)));
    pInfoNode = pHeaderDOMNode->selectSingleNode(bstrNodeName);
    pInfoNode->get_nodeTypedValue(&NodeValue);
    CString strTemp;
    strTemp = strCopyBSTRToCString(NodeValue);

    if(strTemp == "ABS")
    {
        m_ouTestSetupHeader.m_sReportFile.m_eTimeMode = ABS;
    }
    else                    //Default
    {
        m_ouTestSetupHeader.m_sReportFile.m_eTimeMode = REL;
    }

    pInfoNode.Release();
    //bstrNodeName = def_STR_BUS_TYPE;
    bstrNodeName.Assign(SysAllocString(CT2W(def_STR_BUS_TYPE)));
    pInfoNode= pHeaderDOMNode->selectSingleNode(bstrNodeName);
    pInfoNode->get_nodeTypedValue(&NodeValue);
    strTemp = strCopyBSTRToCString(NodeValue);

    if(strTemp == "CAN")
    {
        m_ouTestSetupHeader.m_eBus = CAN;
    }

    pInfoNode.Release();
    return 0;
}

/******************************************************************************
Function Name  :  nSaveHeader
Input(s)       :
Output         :  INT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        :  CS025
******************************************************************************/
INT CTestSetupEntity::nSaveHeader(MSXML2::IXMLDOMElementPtr& pIDomHeaderNode, CString& omStrFilePath)
{
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");
    MSXML2::IXMLDOMElementPtr pChildElement;
    MSXML2::IXMLDOMElementPtr pInfoElement;
    CString omstrTemp;
    //Module Info
    pInfoElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_INFO_NODE));

    if (pInfoElement != NULL)
    {
        bAddChildToNode(pInfoElement, def_STR_CATEGORY_NODE, m_ouTestSetupHeader.m_sModuleName.m_omCategory);
        bAddChildToNode(pInfoElement, def_STR_VALUE_NODE, m_ouTestSetupHeader.m_sModuleName.m_omValue);
    }

    pIDomHeaderNode->appendChild(pInfoElement);
    //Version
    pInfoElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_INFO_NODE));

    if (pInfoElement != NULL)
    {
        bAddChildToNode(pInfoElement, def_STR_CATEGORY_NODE, m_ouTestSetupHeader.m_sVersion.m_omCategory);
        bAddChildToNode(pInfoElement, def_STR_VALUE_NODE, m_ouTestSetupHeader.m_sVersion.m_omValue);
    }

    pIDomHeaderNode->appendChild(pInfoElement);
    //Engineer Name
    pInfoElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_INFO_NODE));

    if (pInfoElement != NULL)
    {
        bAddChildToNode(pInfoElement, def_STR_CATEGORY_NODE, m_ouTestSetupHeader.m_sEngineerInfo1.m_omCategory);
        bAddChildToNode(pInfoElement, def_STR_VALUE_NODE, m_ouTestSetupHeader.m_sEngineerInfo1.m_omValue);
    }

    pIDomHeaderNode->appendChild(pInfoElement);
    //Engineer Role
    pInfoElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_INFO_NODE));

    if (pInfoElement != NULL)
    {
        bAddChildToNode(pInfoElement, def_STR_CATEGORY_NODE, m_ouTestSetupHeader.m_sEngineerInfo2.m_omCategory);
        bAddChildToNode(pInfoElement, def_STR_VALUE_NODE, m_ouTestSetupHeader.m_sEngineerInfo2.m_omValue);
    }

    pIDomHeaderNode->appendChild(pInfoElement);
    //Bus Type
    pInfoElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_DBPATH_NODE));
    bAddChildToNode(pInfoElement, def_STR_DATABASE, m_ouTestSetupHeader.m_omDatabasePath);
    pIDomHeaderNode->appendChild(pInfoElement);
    //Database Path
    pInfoElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_BUS_TYPE));

    if(m_ouTestSetupHeader.m_eBus == CAN)
    {
        omstrTemp = "CAN";
    }

    pInfoElement->Puttext(_bstr_t(omstrTemp));
    pIDomHeaderNode->appendChild(pInfoElement);
    //Report File
    pInfoElement   =  pIDOMDoc->createElement(_bstr_t(def_STR_REPORT_NODE));

    if (pInfoElement != NULL)
    {
        switch(m_ouTestSetupHeader.m_sReportFile.m_eType)
        {
            case HTM:
                omstrTemp = "HTM";
                break;

            case TXT:
            default:
                omstrTemp = "TXT";
                break;
        };

        if(m_ouTestSetupHeader.m_sReportFile.m_omPath == _T(""))
        {
            CString omReportPath(omStrFilePath);
            int nIndex = omReportPath.Find(".xml");
            omReportPath.Delete(nIndex, 4);
            CFile omFile(omStrFilePath,  CFile::modeRead);
            CString omReportName = omFile.GetFileTitle() + _T(".") + omstrTemp;
            omReportPath += omReportName;
            m_ouTestSetupHeader.m_sReportFile.m_omPath = omReportPath;
        }

        //Report Path
        bAddChildToNode(pInfoElement, def_STR_PATH_NODE, m_ouTestSetupHeader.m_sReportFile.m_omPath);
        //Extension
        bAddChildToNode(pInfoElement, def_STR_FORMAT_NODE, omstrTemp);

        switch(m_ouTestSetupHeader.m_sReportFile.m_eType)
        {
            case ABS:
                omstrTemp = "ABS";
                break;

            case REL:
            default:
                omstrTemp = "REL";
                break;
        }

        //Time
        bAddChildToNode(pInfoElement, def_STR_TIMEMODE_NODE, omstrTemp);
    }

    pIDomHeaderNode->appendChild(pInfoElement);
    return 0;
}

/******************************************************************************
Function Name  :  bAddChildToNode
Input(s)       :  MSXML2::IXMLDOMElementPtr& pNodeElement,
                  CString omNodeName
                  CString omNodeval
Output         :  BOOL
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
BOOL CTestSetupEntity::bAddChildToNode(MSXML2::IXMLDOMElementPtr& pNodeElement,
                                       CString omNodeName, CString omNodeval)
{
    BOOL bReturn = FALSE;
    MSXML2::IXMLDOMElementPtr pChildElement;
    MSXML2::IXMLDOMDocumentPtr m_pXMLDom;
    m_pXMLDom.CreateInstance(L"Msxml2.DOMDocument");

    if (pNodeElement != NULL)
    {
        pChildElement   =  m_pXMLDom->createElement(_bstr_t(omNodeName));

        if (pChildElement != NULL)
        {
            pChildElement->Puttext((_bstr_t)omNodeval);
            pNodeElement->appendChild(pChildElement);
            bReturn = TRUE;
        }
    }

    return bReturn;
}

/******************************************************************************
Function Name  :  nLoadTestCases
Input(s)       :  MSXML2::IXMLDOMNodePtr& pTSDOM
Output         :  INT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
Codetag        : CS004
******************************************************************************/
INT CTestSetupEntity::nLoadTestCases(MSXML2::IXMLDOMNodePtr& pTSDOM)
{
    LONG lCount;
    _bstr_t bstrNodeName(def_STR_TESTCASE_NODE);
    MSXML2::IXMLDOMNodeListPtr pDOMTCNodeList;
    MSXML2::IXMLDOMNodePtr pIXMLDOMTestCase;
    pDOMTCNodeList = pTSDOM->selectNodes(bstrNodeName);
    pDOMTCNodeList->get_length(&lCount);

    for(int i=0; i<lCount; i++)
    {
        CTestCaseEntity odTestCaseEntity;
        pIXMLDOMTestCase = pDOMTCNodeList->Getitem(i);
        odTestCaseEntity.GetData(pIXMLDOMTestCase);
        m_odTestCaseEntityList.AddTail(odTestCaseEntity);
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  nSaveTestCases
Input(s)       :  MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode
Output         :  INT
Functionality  :
Member of      :  CTestSetupEntity
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CTestSetupEntity::nSaveTestCases(MSXML2::IXMLDOMElementPtr& pIDomTestCaseNode)
{
    MSXML2::IXMLDOMDocumentPtr pIDOMDoc;
    pIDOMDoc.CreateInstance(L"Msxml2.DOMDocument");
    INT lCount = (INT)m_odTestCaseEntityList.GetCount();

    for(int i=0; i<lCount; i++)
    {
        MSXML2::IXMLDOMElementPtr pChildElement = pIDOMDoc->createElement(_bstr_t(def_STR_TESTCASE_NODE));
        POSITION pos = m_odTestCaseEntityList.FindIndex(i);
        CTestCaseEntity& odTestCaseEntity = m_odTestCaseEntityList.GetAt(pos);
        odTestCaseEntity.SetData(pChildElement);
        pIDomTestCaseNode->appendChild(pChildElement);
    }

    return S_OK;
}

