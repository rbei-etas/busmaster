#pragma once
#include"Utils_stdafx.h"
#include"BaseImportLogFile.h"
#include "Utility.h"
#include <cctype> // for toupper()
#include <algorithm> // for transform()

#define defMASK_LOG_TIME_FORMAT " %2d:%2d:%2d:%4d %*[^\n]"

CBaseImportLogFile::CBaseImportLogFile(ETYPE_BUS eBus):m_eBus(eBus)
{
    InitializeCriticalSection(&m_ouCriticalSection);
    m_nCurrPageNo=0;
    m_nPageLength=defDEFAULT_PAGE_LENGTH;
    m_nCurrLineNo=0;
    m_nOffset=0;
    m_bIsModeHex=false;
    m_bTimeModeFound=false;
    m_bNumericModeFound=false;
    m_bVersionFound=false;
    m_bProtocolFound=false;
    m_unPrevTime = 0;
}
CBaseImportLogFile::~CBaseImportLogFile()
{
    DeleteCriticalSection(&m_ouCriticalSection);
}
HRESULT CBaseImportLogFile::AnalyseLine(std::string strLine,const unsigned long& nLineNo)
{

    //std::cout<<"analyse";
    //std::cout<<"analyse";
    HRESULT bReturn = S_FALSE;
    if(strLine == "" || nLineNo<0)
    {
        return bReturn;
    }
    EnterCriticalSection(&m_ouCriticalSection);
    //1. HEADER VALIDATION
    if(!m_bTimeModeFound || !m_bNumericModeFound || !m_bVersionFound || !m_bProtocolFound)
    {
        //Busmaster Version
        if(strLine.find(defSTR_BUSMASTER_VERSION_STRING) != std::string::npos)
        {
            m_bVersionFound = true;
        }
        if(strLine.find(defSTR_PROTOCOL_USED) != std::string::npos)
        {
            std::string busName = getBusInString(m_eBus);
            std::string protocolUsed = defSTR_PROTOCOL_USED;
            protocolUsed += busName;
            std::string strTempLine = strLine;
            std::transform(protocolUsed.begin(), protocolUsed.end(), protocolUsed.begin(), ::tolower);
            std::transform(strTempLine.begin(), strTempLine.end(), strTempLine.begin(), ::tolower);

            if (strTempLine.find(protocolUsed) != std::string::npos)
            {
                m_bProtocolFound = true;
            }

            if(!m_bProtocolFound)
            {
                //AfxMessageBox(defSTR_ERR_IMPORTLOG_PRTOCOL_MISMATCH,MB_OK|MB_ICONSTOP);
                LeaveCriticalSection(&m_ouCriticalSection);
                return S_CANCEL;

            }
        }
        //Time Mode
        if(strLine.find(HEX_MODE) != std::string::npos)
        {
            m_bIsModeHex = true;
            m_bNumericModeFound=true;
        }
        else if (strLine.find(DEC_MODE)!= std::string::npos)
        {
            m_bIsModeHex = false;
            m_bNumericModeFound=true;
        }
        if( strLine.find(SYSTEM_MODE) != std::string::npos)
        {
            m_eTimeMode= eSYSTEM_MODE;
            m_bTimeModeFound=true;
        }
        else if( strLine.find(ABSOLUTE_MODE) != std::string::npos)
        {
            m_eTimeMode= eABSOLUTE_MODE;
            m_bTimeModeFound=true;
        }
        else if( strLine.find(RELATIVE_MODE) != std::string::npos)
        {
            m_eTimeMode= eRELATIVE_MODE;
            m_bTimeModeFound=true;
        }
        //TODO: File Version Check.
    }
    //Commented line.
    if(strLine.find("***") != std::string::npos)
    {
        LeaveCriticalSection(&m_ouCriticalSection);
        return S_FALSE;

    }



    //2. TIME EXTRACTION
    if(m_bTimeModeFound && m_bNumericModeFound && m_bVersionFound && m_bProtocolFound)
    {
        int nHH,nMM,nSS,nmm;
        unsigned long nTime;

        bReturn = S_FALSE;
        if(sscanf_s(strLine.c_str(),defMASK_LOG_TIME_FORMAT,&nHH,&nMM,&nSS,&nmm) == 4)
        {
            bReturn = S_OK;

            nTime = (nHH*3600+nMM*60+nSS)*10000+nmm;

            if(m_eTimeMode == eRELATIVE_MODE)
            {

                m_vecTimeStamp.push_back(nTime+m_unPrevTime);
                m_unPrevTime += nTime;

            }
            else if(m_eTimeMode == eABSOLUTE_MODE)
            {
                m_vecTimeStamp.push_back(nTime+m_unPrevTime);
            }
            else if(m_eTimeMode == eSYSTEM_MODE)
            {
                if(nLineNo == 0)
                {
                    m_nSystemTimeOffset = nTime;
                    m_vecTimeStamp.push_back(m_unPrevTime);
                }
                else
                {
                    m_vecTimeStamp.push_back(nTime-m_nSystemTimeOffset+m_unPrevTime);
                }
            }
        }
    }
    LeaveCriticalSection(&m_ouCriticalSection);
    return bReturn;
}
HRESULT CBaseImportLogFile::LoadFile(const std::string& strFileName)
{
    HRESULT bResult = m_ouReadFile.SetFileLineAnalysisFilter(this);
    if(S_OK == bResult)
    {
        m_vecTimeStamp.clear();
        m_vecTimeStamp.shrink_to_fit();
        m_nSystemTimeOffset=0;
        m_bTimeModeFound = false;
        m_bNumericModeFound = false;
        m_bVersionFound =false;
        m_bProtocolFound = false;
        GetLocalTime(&m_ouTimeImport);
        m_unPrevTime=0;
        bResult = m_ouReadFile.LoadFile(strFileName);
        if(bResult == S_FILE_SIZE_ABOVE_LIMIT)
        {
            /*int nFileSizeGb = (double)m_ouReadFile.GetFileSizeLimit()/1073741824;
            CString omStrFileSizeError;
            omStrFileSizeError.Format(defSTR_ERR_IMPORTLOG_FILE_SIZE_EXCEEDED,nFileSizeGb);*/
            //AfxMessageBox(omStrFileSizeError,MB_OK|MB_ICONSTOP);
            return eImportLogFileSizeErr;
        }
        else if(bResult == S_INVALID_FILE)
        {
            //AfxMessageBox(defSTR_ERR_IMPORTLOG_INVALID_FILE,MB_OK|MB_ICONSTOP);
            return eImportLogInvalid;
        }
    }
    return bResult;
}

HRESULT CBaseImportLogFile::UnLoadFile()
{
    EnterCriticalSection(&m_ouCriticalSection);
    m_vecTimeStamp.clear();
    m_vecTimeStamp.shrink_to_fit();
    m_nSystemTimeOffset=0;
    m_unPrevTime=0;
    LeaveCriticalSection(&m_ouCriticalSection);
    return m_ouReadFile.UnLoadFile();
}
HRESULT CBaseImportLogFile::CancelFileLoad()
{
    EnterCriticalSection(&m_ouCriticalSection);
    m_vecTimeStamp.clear();
    m_vecTimeStamp.shrink_to_fit();
    m_nSystemTimeOffset=0;
    m_unPrevTime=0;
    LeaveCriticalSection(&m_ouCriticalSection);
    return m_ouReadFile.CancelFileLoad();
}
HRESULT CBaseImportLogFile::GetPercentageRead(int& nPercentange)
{
    HRESULT hResult = S_FALSE;
    nPercentange = m_ouReadFile.GetPercentageRead();
    if(nPercentange>0)
    {
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CBaseImportLogFile::SetPageLength(const unsigned long& nPageLength)
{
    HRESULT hResult = S_FALSE;
    if(nPageLength>0)
    {
        hResult = S_OK;
        m_nPageLength = nPageLength;
    }
    return hResult;
}
HRESULT CBaseImportLogFile::GetPageLength(unsigned long& nPageLength)
{
    nPageLength = m_nPageLength;
    return S_OK;
}
HRESULT CBaseImportLogFile::GetCurrentLine(unsigned long& nLineNo)
{
    nLineNo = m_nCurrLineNo;
    return S_OK;
}
HRESULT CBaseImportLogFile::SetCurrentLine(const unsigned long& nLineNo)
{

    m_nCurrLineNo = nLineNo;
    return S_OK;
}
HRESULT CBaseImportLogFile::GetCurrentPage(unsigned long& nPageNo)
{
    nPageNo = m_nCurrPageNo;
    return S_OK;
}
HRESULT CBaseImportLogFile::SetCurrentPage(const unsigned long& nPageNo)
{
    m_nCurrPageNo = nPageNo;
    return S_OK;
}
HRESULT CBaseImportLogFile::GetPageAndLineNo(const unsigned long& nTime,unsigned long& nPageNo,unsigned long& nLineNo)
{
    HRESULT hResult = S_FALSE;
    EnterCriticalSection(&m_ouCriticalSection);
    unsigned long nSize = m_vecTimeStamp.size();
    for(unsigned long nCount = 0; nCount<nSize; nCount++)
    {
        if(m_vecTimeStamp.at(nCount)>=nTime)
        {
            hResult = S_OK;
            if(m_vecTimeStamp.at(nCount)>nTime)
            {
                nLineNo = nCount-1;
            }
            else if(m_vecTimeStamp.at(nCount) == nTime)
            {
                nLineNo = nCount;
            }
            nPageNo = nLineNo/m_nPageLength;
            break;
        }
    }
    LeaveCriticalSection(&m_ouCriticalSection);
    return hResult;
}
HRESULT CBaseImportLogFile::GetTotalLines(unsigned long& nLinesCount)
{
    nLinesCount = m_ouReadFile.GetLinesCount();
    return S_OK;
}
HRESULT CBaseImportLogFile::GetTotalPages(unsigned long& nPagesCount)
{
    HRESULT hResult = S_FALSE;
    if(m_nPageLength!=0)
    {
        hResult=S_OK;
        nPagesCount=m_ouReadFile.GetLinesCount()/m_nPageLength;
        if(m_ouReadFile.GetLinesCount()%m_nPageLength>0)
        {
            nPagesCount++;
        }
    }
    return hResult;
}
HRESULT CBaseImportLogFile::GetMsgAt(void* /*pvMsg*/ ,const unsigned long& /*nLineNo*/)
{
    return S_OK;
}
HRESULT CBaseImportLogFile::GetCurrMsg(void* /*pvMsg*/)
{
    return S_OK;
}
HRESULT CBaseImportLogFile::GetPrevMsg(void* /*pvMsg*/)
{
    return S_OK;
}
HRESULT CBaseImportLogFile::GetNextMsg(void* /*pvMsg*/)
{
    return S_OK;
}
HRESULT CBaseImportLogFile::GetCurrListMsg(std::vector<void*>& /*vecpvMsg*/)
{
    return S_OK;
}
HRESULT CBaseImportLogFile::GetPrevListMsg(std::vector<void*>& /*vecpvMsg*/)
{
    return S_OK;
}
HRESULT CBaseImportLogFile::GetNextListMsg(std::vector<void*>& /*vecpvMsg*/)
{
    return S_OK;
}


HRESULT CBaseImportLogFile::GetTimeModeMapping(SYSTEMTIME& CurrSysTime)
{
    CurrSysTime = m_ouTimeImport;
    return S_OK;
}
