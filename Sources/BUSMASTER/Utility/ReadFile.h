#pragma once
#include<Windows.h>
#include<string>
#include<fstream>
#include"IFileLineAnalysisFilter.h"


#define MAX_LENGTH 4096
#define S_CANCEL 3
#define S_FILE_SIZE_ABOVE_LIMIT 4
#define S_INVALID_FILE 5

class CReadFile
{
    FILE* m_pFile;
    std::string m_strFileName;
    unsigned long m_nLineCount;
    unsigned long m_nFileSize;
    std::vector<unsigned long> m_vecByteOffset;
    IFileLineAnalysisFilter* m_pouFileLineAnalysisFilter;
    unsigned long m_nFileSizeLimitBytes;
    unsigned long m_nTotalRead;
    bool m_bCancelLoad;
    bool m_bIsLoaded;
public:
    CReadFile();
    ~CReadFile();
    HRESULT SetFileLineAnalysisFilter(IFileLineAnalysisFilter* pouFileLineAnalysisFilter);
    void GetLine(unsigned long nLineNo,std::string& strLine);
    HRESULT LoadFile(const std::string& strFileName);
    HRESULT UnLoadFile();
    HRESULT SetFileSizeLimit(const unsigned long& nBytes);
    unsigned long GetFileSizeLimit();
    int GetFileSize();
    int GetLinesCount();
    int GetPercentageRead();
    HRESULT CancelFileLoad();
};