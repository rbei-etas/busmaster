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
                               std::string& omAttribute, _bstr_t temp)
{
    std::string strTemp;
    bool bResult = false;
    MSXML2::IXMLDOMNamedNodeMapPtr pCodeAttrib;
    MSXML2::IXMLDOMNodePtr iTempNode = nullptr;

    pCodeAttrib = iNode->Getattributes();
    if (pCodeAttrib->Getlength() >= 1)
    {
        iTempNode = pCodeAttrib->getNamedItem(temp);
        if (iTempNode != nullptr)
        {
            omAttribute = _com_util::ConvertBSTRToString(iTempNode->text.GetBSTR());
            bResult = true;
        }
    }
    return bResult;
}

bool CNodeUtil::bRemoveTag(const std::string omSrcString, std::string& omDestString)
{
    bool bResult = TRUE;
    omDestString = "";
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
