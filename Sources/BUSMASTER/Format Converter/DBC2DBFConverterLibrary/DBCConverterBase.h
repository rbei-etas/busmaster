#pragma once


#include "DBFMessage.h"
#include "../DBC2DBFConverterLibrary/DBFTagDefinitions.h"
//#include "../FormatConverterApp/GettextBusmaster.h"
#include "../../Utility/MultiLanguageSupport.h"
#include "afx.h"
#include "../../../Kernel/ProtocolDefinitions/ProtocolsDefinitions.h"
class CDBCConverterBase
{
public:
    virtual HRESULT LoadDBCFile(CString strDBCFile) = 0;
    virtual HRESULT GenerateImportList(/* sMESSAGE*& */) = 0;
    virtual HRESULT ConvertFile(CString strDBFFile) = 0;
    virtual HRESULT FindMessage(CString strMsgName, CMessage* ) = 0;
    virtual HRESULT FindMessage(UINT unMsgId, CMessage* ) = 0;
    virtual HRESULT GetResultString(char* pchResult) = 0;
    virtual HRESULT FindSignalAlias(CString& strMsgName, CString& strSignalName, CString& strSignalAlias) = 0 ;
    virtual HRESULT ClearMsgList() = 0;
    virtual HRESULT GetMessageNameList(CStringArray& meassageList) = 0;
    virtual ~CDBCConverterBase() {};
};

typedef HRESULT(*GETCONVERTER)(CDBCConverterBase*& ouDBCConverter, ETYPE_BUS);

