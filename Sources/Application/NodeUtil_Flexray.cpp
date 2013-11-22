// NodeUtil.cpp: implementation of the CNodeUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "NodeUtil_Flexray.h"
#include "Fibex_Defs.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace MSXML2;
extern MSXML2::IXMLDOMDocument2Ptr gpXMLDom;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MSXML2::IXMLDOMDocument2Ptr gpXMLDom;
HRESULT hr;
CNodeUtil::CNodeUtil()
{
}

CNodeUtil::~CNodeUtil()
{
}


bool CNodeUtil::bReadAttribute(MSXML2::IXMLDOMNode* iNode,
                               string& omAttribute, _bstr_t temp)
{
    string strTemp;
    bool bResult = false;
    MSXML2::IXMLDOMNamedNodeMapPtr pCodeAttrib;
    MSXML2::IXMLDOMNodePtr iTempNode = NULL;

    pCodeAttrib = iNode->Getattributes();
    if (pCodeAttrib->Getlength() >= 1)
    {
        iTempNode = pCodeAttrib->getNamedItem(temp);
        if (iTempNode != NULL)
        {
            omAttribute = _com_util::ConvertBSTRToString(iTempNode->text.GetBSTR());
            bResult = true;
        }
    }
    return bResult;
}

bool CNodeUtil::bRemoveTag(const string omSrcString, string& omDestString)
{
    bool bResult = TRUE;
    omDestString = _T("");
    int nLen = omSrcString.length();
    int nPos = omSrcString.find(_T(':'));
    if (nPos < 0)
    {
        omDestString = omSrcString;
        bResult = FALSE;
    }
    else
    {
        // omDestString = omSrcString.Right(nLen - nPos - 1);

        omDestString = omSrcString.substr(nPos + 1, nLen - nPos - 1);
    }
    return bResult;
}
//
//string CNodeUtil::omAddNameSpaceExt(string omNameSpace, string omNodeName)
//{
//    //If name space separator is already there then don't add the name space
//    string FinalName = omNodeName;
//    if (omNodeName.Find(NAME_SPACE_SEPARATOR) == -1)
//    {
//        FinalName = omNameSpace + NAME_SPACE_SEPARATOR + omNodeName;
//    }
//    return FinalName;
//}
//
//BOOL CNodeUtil::bCreateNode(MSXML2::IXMLDOMElementPtr& pChildNode,
//                            string omNodeName, string omNameSpace,
//                            string omNodeval )
//{
//    BOOL bReturn = FALSE;
//    if ( !(omNodeName == INVALID_STRING))
//    {
//        omNodeName = omAddNameSpaceExt( omNameSpace, omNodeName);
//        pChildNode =  gpXMLDom->createElement(_bstr_t(omNodeName));
//        if (pChildNode != NULL)
//        {
//            if ( !(omNodeval == INVALID_STRING))
//            {
//                pChildNode->Puttext((_bstr_t)omNodeval);
//            }
//            bReturn = TRUE;
//        }
//    }
//    return bReturn;
//}
//
//BOOL CNodeUtil::bAddAttribToNode(MSXML2::IXMLDOMElementPtr& pNode,
//                                 string omAttribName, string omAttribVal)
//{
//    BOOL bReturn = FALSE;
//    if ( pNode != NULL )
//    {
//        MSXML2::IXMLDOMAttributePtr pAttribute;
//        if ( !(omAttribName == INVALID_STRING) && !(omAttribVal == INVALID_STRING))
//        {
//            pAttribute = gpXMLDom->createAttribute(_bstr_t(omAttribName));
//            if ( pAttribute != NULL)
//            {
//                //Add attrib val
//                pAttribute->value = _bstr_t(omAttribVal);
//                pNode->setAttributeNode(pAttribute);
//                bReturn = TRUE;
//            }
//        }
//    }
//    return bReturn;;
//}
//
//
//
//bool CNodeUtil::bAddNodeWithOneAttrib(MSXML2::IXMLDOMElementPtr& pNodeElement,
//                                      string omNodeName, string omNameSpace,
//                                      string omNodeVal, string omAttribName,
//                                      string omAttribVal)
//{
//    BOOL bReturn = FALSE;
//    MSXML2::IXMLDOMElementPtr pChildElement;
//
//    if (bCreateNode(pChildElement, omNodeName, omNameSpace, omNodeVal))
//    {
//        bAddAttribToNode(pChildElement, omAttribName, omAttribVal);
//        bReturn = TRUE;
//        pNodeElement->appendChild(pChildElement);
//    }
//    return bReturn;
//}
//
//bool CNodeUtil::bAddChildToNode(MSXML2::IXMLDOMElementPtr& pNodeElement,
//                                string omNodeName, string omNameSpace,
//                                string omNodeval)
//{
//    BOOL bReturn = FALSE;
//    MSXML2::IXMLDOMElementPtr pChildElement;
//    if ((pNodeElement != NULL) && (!(omNodeName == INVALID_STRING)))
//    {
//        omNodeName = omAddNameSpaceExt( omNameSpace, omNodeName);
//        pChildElement   =  gpXMLDom->createElement(_bstr_t(omNodeName));
//        if (pChildElement != NULL)
//        {
//            if ( !(omNodeval == INVALID_STRING))
//            {
//                pChildElement->Puttext((_bstr_t)omNodeval);
//            }
//            pNodeElement->appendChild(pChildElement);
//            bReturn = TRUE;
//        }
//    }
//    return bReturn;
//}
//bool CNodeUtil::bAddRefrences(MSXML2::IXMLDOMElementPtr& pMainParent,string omMainNodeName ,string omSubNodeName,
//                              CStringArray& omRefArray)
//{
//    bool bReturn = FALSE;
//    int nRefCnt = omRefArray.GetSize();
//    if (pMainParent != NULL && nRefCnt > 0)
//    {
//        MSXML2::IXMLDOMElementPtr pRefMain;
//        BOOL bRet= bCreateNode( pRefMain,
//                                omMainNodeName, NAME_SPACE_FIBEX, "");
//        if (bRet)
//        {
//            MSXML2::IXMLDOMElementPtr pRefNode;
//            for (int i = 0; i < nRefCnt; i++)
//            {
//                string omIDRef = omRefArray.GetAt(i);
//                //Add child node with attrib ID-REF
//                bRet= bAddNodeWithOneAttrib( pRefMain,
//                                             omSubNodeName, NAME_SPACE_FIBEX, "", NAME_ID_REF, omIDRef);
//                if ( !bRet )
//                {
//                    ASSERT(false);
//                }
//            }
//            pMainParent->appendChild(pRefMain);
//            bReturn = TRUE;
//        }
//    }
//    return bReturn;
//}
//bool CNodeUtil::bAddID_OID_Attrib(string strID, string strOID,
//                                  MSXML2::IXMLDOMElementPtr& pNodeElement)
//{
//    BOOL bReturn = FALSE;
//    if (pNodeElement != NULL)
//    {
//        MSXML2::IXMLDOMAttributePtr pAttribute;
//        pAttribute = gpXMLDom->createAttribute(_bstr_t(NAME_ID));
//        if (pAttribute != NULL && (!(strID == INVALID_STRING)))
//        {
//            pAttribute->value = _bstr_t(strID);
//            pNodeElement->setAttributeNode(pAttribute);
//            bReturn = TRUE;
//            //pAttribute->Release();
//        }
//        pAttribute = gpXMLDom->createAttribute(_bstr_t(NAME_OPTIONAL_ID));
//        //If optional ID is present
//        if ( pAttribute != NULL && (!(strOID == INVALID_STRING)) && bReturn )
//        {
//            pAttribute->value = _bstr_t(strOID);
//            pNodeElement->setAttributeNode(pAttribute);
//            //pAttribute->Release();
//        }
//    }
//    return bReturn;
//}