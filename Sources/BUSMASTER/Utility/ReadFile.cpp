#include<fstream>
#include<regex>
#include"ReadFile.h"
#define defDEFAULT_FILE_SIZE_LIMIT 1073741824*2
CReadFile::CReadFile()
{
    m_pFile=nullptr;
    m_nFileSizeLimitBytes = defDEFAULT_FILE_SIZE_LIMIT;
    m_bIsLoaded=false;
}
CReadFile::~CReadFile()
{
    if(nullptr!=m_pFile)
    {
        fclose (m_pFile);
        m_pFile = nullptr;
    }
}
HRESULT CReadFile::SetFileLineAnalysisFilter(IFileLineAnalysisFilter* pouFileLineAnalysisFilter)
{
    HRESULT hResult = S_FALSE;
    if(pouFileLineAnalysisFilter!=nullptr)
    {
        m_pouFileLineAnalysisFilter = pouFileLineAnalysisFilter;
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CReadFile::LoadFile(const std::string& strFileName)
{
    HRESULT hResult = S_FALSE;
    m_pFile=nullptr;
    m_nLineCount=0;
    m_nTotalRead=0;
    m_bIsLoaded=false;
    unsigned long nReadSize=0;
    m_nFileSize=0;
    m_bCancelLoad=false;
    m_vecByteOffset.clear();
    m_vecByteOffset.shrink_to_fit();
    hResult = UnLoadFile();
    if(hResult == S_FALSE)
    {
        return hResult;
    }
    hResult = S_FALSE;
    fopen_s (&m_pFile, strFileName.c_str() , "rb" );
    //TODO: File Size and File path Check.
    if (nullptr!=m_pFile)
    {
        m_bIsLoaded=true;
        fseek(m_pFile,0,SEEK_END);
        m_nFileSize = ftell(m_pFile);
        if(m_nFileSize>m_nFileSizeLimitBytes)
        {
            UnLoadFile();
            return S_FILE_SIZE_ABOVE_LIMIT;
        }
        else
        {
            fseek(m_pFile,0, SEEK_SET);
        }
        hResult = S_OK;
        char  Buffer[MAX_LENGTH+1];
        size_t nPrevIdx = 0;
        size_t nCurIdx=0;
        std::string strBuf="";
        std::string strBufSubString="";
        HRESULT hAllowLine = S_FALSE;
        do
        {
            memset(Buffer,'\0',sizeof(Buffer));
            nReadSize = fread(Buffer,1,MAX_LENGTH,m_pFile);
            Buffer[nReadSize] = '\0';
            strBuf = std::string(Buffer,sizeof(Buffer));
            nPrevIdx=0;
            nCurIdx=0;
            do
            {
                nCurIdx=strBuf.find('\n',nPrevIdx);
                strBufSubString = strBuf.substr(nPrevIdx,nCurIdx-nPrevIdx);
                if(nCurIdx!=std::string::npos)
                {
                    hAllowLine = m_pouFileLineAnalysisFilter->AnalyseLine(strBufSubString,m_nLineCount);
                }
                else
                {
                    hAllowLine = S_FALSE;
                }
                if(nCurIdx!=std::string::npos && hAllowLine == S_OK)
                {
                    m_vecByteOffset.push_back( m_nTotalRead+nPrevIdx);
                    m_nLineCount++;
                }
                if(nCurIdx == std::string::npos&&!ferror(m_pFile))
                {
                    if(!feof(m_pFile))
                    {
                        fseek ( m_pFile , m_nTotalRead+nPrevIdx , SEEK_SET );
                        nReadSize=nPrevIdx;
                    }
                    else if(feof(m_pFile) && hAllowLine == S_OK)
                    {
                        //last line doesn't have '\n'
                        m_vecByteOffset.push_back( m_nTotalRead+nPrevIdx);
                        m_nLineCount++;
                        //m_vecByteOffset.insert(std::pair<unsigned long,unsigned long>(m_nLineCount++,m_nFileSize+nPrevIdx));
                    }
                }
                nPrevIdx=nCurIdx+1;
            }
            while(nCurIdx!=std::string::npos && hAllowLine!=S_CANCEL && m_bCancelLoad == false);
            m_nTotalRead+=nReadSize;
            if(m_bCancelLoad == true)
            {
                break;
            }
        }
        while(!feof(m_pFile)&&!ferror(m_pFile) && hAllowLine!=S_CANCEL);
        if(hAllowLine==S_CANCEL||m_bCancelLoad==true)
        {
            UnLoadFile();
            hResult = S_FALSE;
            m_bCancelLoad=false;
        }
        else if(m_vecByteOffset.size()==0)
        {
            UnLoadFile();
            hResult = S_INVALID_FILE;
        }
    }
    return hResult;
}
HRESULT CReadFile::UnLoadFile()
{
    HRESULT hResult = S_FALSE;
    m_vecByteOffset.clear();
    m_vecByteOffset.shrink_to_fit();
    if(nullptr!=m_pFile)
    {
        if(fclose (m_pFile)==0)
        {
            m_bIsLoaded=false;
            hResult = S_OK;
        }
        m_pFile = nullptr;
    }
    else if(m_pFile == nullptr)
    {
        hResult = S_OK;
    }
    return hResult;
}
void CReadFile::GetLine(unsigned long nLineNo,std::string& strLine)
{
    if ( nLineNo >= m_vecByteOffset.size())
    {
        strLine = "";
    }
    else
    {
        unsigned long nStartByte = m_vecByteOffset.at(nLineNo);
        if (nullptr!=m_pFile)
        {
            char  Buffer[MAX_LENGTH];
            size_t nSize = -1;
            std::string strBuf="";
            memset(Buffer,'\0',sizeof(char)*MAX_LENGTH);
            fseek ( m_pFile , nStartByte , SEEK_SET );
            int numRead = fread(Buffer,1,MAX_LENGTH,m_pFile);
            if (numRead < 0 || numRead >= MAX_LENGTH)
            {
                numRead = MAX_LENGTH;
            }
            Buffer[numRead-1] = '\0';
            strBuf = std::string(Buffer);
            nSize = -1;
            nSize=strBuf.find('\n');
            if(nSize!=std::string::npos)
            {
                strLine = strBuf.substr(0,nSize);
            }
            else if(feof(m_pFile))
            {
                strLine = strBuf;
            }
        }
    }

}
int CReadFile::GetFileSize()
{
    return m_nFileSize;
}
int CReadFile::GetLinesCount()
{
    return m_nLineCount;
}
HRESULT CReadFile::SetFileSizeLimit(const unsigned long& nBytes)
{
    HRESULT hResult =S_FALSE;
    if(nBytes>0)
    {
        m_nFileSizeLimitBytes = nBytes;
        hResult = S_OK;
    }
    return hResult;
}
unsigned long CReadFile::GetFileSizeLimit()
{
    return m_nFileSizeLimitBytes;
}
int CReadFile::GetPercentageRead()
{
    int nPerct = 0;
    if(m_nFileSize>0)
    {
        nPerct=((double)m_nTotalRead/m_nFileSize)*100;
    }
    return nPerct;
}
HRESULT CReadFile::CancelFileLoad()
{
    m_bCancelLoad = true;
    return S_OK;
}