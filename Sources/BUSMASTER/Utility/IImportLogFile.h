#pragma once
#include<Windows.h>
#include<string>
#include<vector>
#include"ProtocolsDefinitions.h"
class IImportLogFile
{
public:
    virtual HRESULT LoadFile(const std::string& strFileName)=0;
    virtual HRESULT UnLoadFile()=0;
    virtual HRESULT CancelFileLoad()=0;
    virtual HRESULT GetPercentageRead(int& nPercentange)=0;
    virtual HRESULT SetPageLength(const unsigned long& nLineNo) = 0;
    virtual HRESULT GetPageLength(unsigned long& nLineNo) = 0;
    virtual HRESULT GetCurrentLine(unsigned long& nLineNo) = 0;
    virtual HRESULT SetCurrentLine(const unsigned long& nLineNo) = 0;
    virtual HRESULT GetCurrentPage(unsigned long& nPageNo) = 0;
    virtual HRESULT SetCurrentPage(const unsigned long& nPageNo) = 0;
    virtual HRESULT GetPageAndLineNo(const unsigned long& nTime,unsigned long& nPageNo,unsigned long& nLineNo)=0;
    virtual HRESULT GetTotalLines(unsigned long& nLinesCount) = 0;
    virtual HRESULT GetTotalPages(unsigned long& nPagesCount) = 0;
    virtual HRESULT GetMsgAt(void* pvMsg,const unsigned long& nLineNo) = 0;
    virtual HRESULT GetCurrMsg(void* pvMsg) = 0;
    virtual HRESULT GetPrevMsg(void* pvMsg) = 0;
    virtual HRESULT GetNextMsg(void* pvMsg) = 0;
    virtual HRESULT GetCurrListMsg(std::vector<void*>& vecpvMsg) = 0;
    virtual HRESULT GetPrevListMsg(std::vector<void*>& vecpvMsg) = 0;
    virtual HRESULT GetNextListMsg(std::vector<void*>& vecpvMsg) = 0;
    virtual HRESULT GetTimeModeMapping(SYSTEMTIME& CurrSysTime) = 0;
};
