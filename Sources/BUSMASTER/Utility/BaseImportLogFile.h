#pragma once
#include<vector>
#include"..\Include\BaseDefs.h"
#include"IFileLineAnalysisFilter.h"
#include"..\Application\HashDefines.h"
#include "IImportLogFile.h"
#include"ReadFile.h"



class CBaseImportLogFile : public IImportLogFile,private IFileLineAnalysisFilter
{

private:
    unsigned long m_nSystemTimeOffset;
    CRITICAL_SECTION m_ouCriticalSection;
protected:
    ETYPE_BUS m_eBus;
    CReadFile m_ouReadFile;
    std::vector<unsigned long> m_vecTimeStamp;
    unsigned long m_unPrevTime;
    SYSTEMTIME m_ouTimeImport;
    unsigned long m_nCurrPageNo,m_nPageLength,m_nCurrLineNo;
    bool m_bIsModeHex;
    eTIMEMODE   m_eTimeMode;
    int m_nOffset;
    bool m_bTimeModeFound,m_bNumericModeFound,m_bVersionFound,m_bProtocolFound;

private:
    HRESULT AnalyseLine(std::string strLine,const unsigned long& nLineNo);
protected:
    CBaseImportLogFile();
public:
    CBaseImportLogFile(ETYPE_BUS eBus);
    virtual ~CBaseImportLogFile();
    virtual HRESULT LoadFile(const std::string& strFileName);
    virtual HRESULT UnLoadFile();
    virtual HRESULT CancelFileLoad();
    virtual HRESULT GetPercentageRead(int& nPercentange);
    virtual HRESULT SetPageLength(const unsigned long& nPageLength);
    virtual HRESULT GetPageLength(unsigned long& nPageLength);
    virtual HRESULT GetCurrentLine(unsigned long& nLineNo);
    virtual HRESULT SetCurrentLine(const unsigned long& nLineNo);
    virtual HRESULT GetCurrentPage(unsigned long& nPageNo);
    virtual HRESULT SetCurrentPage(const unsigned long& nPageNo);
    virtual HRESULT GetPageAndLineNo(const unsigned long& nTime,unsigned long& nPageNo,unsigned long& nLineNo);
    virtual HRESULT GetTotalLines(unsigned long& nLinesCount);
    virtual HRESULT GetTotalPages(unsigned long& nPagesCount);
    virtual HRESULT GetMsgAt(void* pvMsg,const unsigned long& nLineNo);
    virtual HRESULT GetCurrMsg(void* pvMsg);
    virtual HRESULT GetPrevMsg(void* pvMsg);
    virtual HRESULT GetNextMsg(void* pvMsg);
    virtual HRESULT GetCurrListMsg(std::vector<void*>& vecpvMsg);
    virtual HRESULT GetPrevListMsg(std::vector<void*>& vecpvMsg);
    virtual HRESULT GetNextListMsg(std::vector<void*>& vecpvMsg);
    virtual HRESULT GetTimeModeMapping(SYSTEMTIME& CurrSysTime);
};