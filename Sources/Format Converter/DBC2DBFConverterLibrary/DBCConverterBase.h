#pragma once

#include <windows.h>
#include "DBFMessage.h"
#include "../DBC2DBFConverterLibrary/DBFTagDefinitions.h"
//#include "../FormatConverterApp/GettextBusmaster.h"
#include "../../Utility/MultiLanguageSupport.h"

class CDBCConverterBase
{
public:
    virtual HRESULT LoadDBCFile(string strDBCFile) = 0;
    virtual HRESULT GenerateImportList(/* sMESSAGE*& */) = 0;
    virtual HRESULT ConvertFile(string strDBFFile) = 0;
    virtual HRESULT FindMessage(string strMsgName, CMessage& ) = 0;
    virtual HRESULT FindMessage(UINT unMsgId, CMessage& ) = 0;
    virtual HRESULT GetResultString(char* pchResult) = 0;
    virtual HRESULT FindSignalAlias(string& strMsgName, string& strSignalName, string& strSignalAlias) = 0 ;
    virtual HRESULT ClearMsgList() = 0;
    virtual HRESULT GetMessageNameList(std::list<string>& meassageList) = 0;
    virtual ~CDBCConverterBase() {};
};

typedef HRESULT (*GETCONVERTER)(CDBCConverterBase*& ouDBCConverter, eBUS_TYPE);

