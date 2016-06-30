#include "DataTypes_stdafx.h"
#include "NodeSimCodeGenerator.h"
#include <algorithm>
#include <string>
#include <sstream>      // std::ostringstream
#include <sys/stat.h>
#include <time.h>
#include "Utility\Utility.h"
#include "IBMNetWorkGetService.h"


#define TAG_COLON                   ":"
#define TAG_SPACE                   " "
#define TAG_TAB                     "\t"
#define TAG_OPENFBRACE              "{"
#define TAG_CLOSEFBRACE             "}"
#define TAG_PUBLICSTART             "public:"
#define TAG_DATABYTEDECL            "unsigned char m_ucData[%d];"
#define TAG_DATABYTES               "m_ucData"
#define TAG_SEMICOLON               ";"

#define defNameSpace               "namespace"

NodeSimCodeGenerator::NodeSimCodeGenerator()
{

}


NodeSimCodeGenerator::~NodeSimCodeGenerator(void)
{
}


std::string NodeSimCodeGenerator::AddDoubleQuotes(std::string strTarget)
{
    std::string strQuotes = "\"";
    return ( strQuotes + strTarget + strQuotes);
}

ERRORCODE NodeSimCodeGenerator::GenerateCode(std::string pchHeaderFilePath, std::string pchCppFilePath, ICluster* pCluster, INsCodeGenHelper* codeGenHelper)
{
    ETYPE_BUS clusterType;
    pCluster->GetClusterType(clusterType);


    if (nullptr == codeGenHelper)
    {
        return E_NOTIMPL;
    }
    codeGenHelper->SetCluster(pCluster);
    std::ostringstream omStringStream;
    std::fstream m_omFileHeader;
    std::fstream m_omFileCode;
    std::string strName;

    std::string strPath;




    m_omFileHeader.open(pchHeaderFilePath, std::fstream::out|std::fstream::binary);
    m_omFileCode.open(pchCppFilePath, std::fstream::out|std::fstream::binary);


    if(m_omFileHeader.is_open() == false || m_omFileCode.is_open() == false)
    {
        return EC_FAILURE;
    }


    GenerateHeaderFile(m_omFileHeader, codeGenHelper, pCluster);
    GenerateCppFile(m_omFileCode, codeGenHelper, pCluster);

    return S_OK;

}

ERRORCODE NodeSimCodeGenerator::GenerateHeaderFile(std::fstream& fileHeader, INsCodeGenHelper* codeGenHelper, ICluster* cluster)
{
    //1. includes
    std::string headers;
    fileHeader << codeGenHelper->GetProtocolIncludesHeaderName(cluster) << std::endl;

    //2. cluster NameSpace;
    std::string clusterName = GetClusterName(cluster);

    fileHeader << defNameSpace << TAG_SPACE << clusterName << TAG_OPENFBRACE << std::endl;

    //3. Pdu namespace
    fileHeader << defNameSpace  <<" " <<"PDU" << TAG_OPENFBRACE <<std::endl;

    //4. Pdu Decl

    char chTemp[256];
    std::string pduName;
    std::map<UID_ELEMENT, IElement*> pduList;
    cluster->GetElementList(ePduElement, pduList);
    IPduCodeGenHelper* pduCodeGenHelper = codeGenHelper->GetPduCodeGenHelper();
    PduProps pduProps;
for ( auto itrPdu : pduList )
    {
        IPdu* pdu = (IPdu*)(itrPdu.second);
        pdu->GetProperties(pduProps);
        fileHeader << pduCodeGenHelper->GetDecl(pdu) << std::endl;
        fileHeader << TAG_OPENFBRACE << std::endl;
        fileHeader << TAG_PUBLICSTART << std::endl;

        sprintf_s(chTemp, sizeof(chTemp), TAG_DATABYTEDECL, pduProps.m_ouFlexRayPduProps.m_unByteLength);
        fileHeader << chTemp;

        fileHeader << pduCodeGenHelper->GetBaseClassDecl(pdu) << std::endl;

        fileHeader << pduCodeGenHelper->GetSignalsDecl(pdu)<< std::endl;

        fileHeader << "};" << std::endl;

        pduName = pduCodeGenHelper->GetUniqueName(pdu);
        fileHeader << "typedef" << " " << "_" << pduName << " " << pduName <<TAG_SEMICOLON << std::endl;
    }

    //5. Endl Pdu namespace
    fileHeader << TAG_CLOSEFBRACE << std::endl;

    //6. Frame Namespace start
    fileHeader << defNameSpace <<" " <<"Frame" << TAG_OPENFBRACE <<std::endl;

    //7. Using namespace
    fileHeader <<"using namespace PDU;" << std::endl;

    //8. Class Decleration

    std::string frameName;
    std::map<UID_ELEMENT, IElement*> frameList;
    cluster->GetElementList(eFrameElement, frameList);
    IFrameCodeGenHelper* frameCodeGenHelper = codeGenHelper->GetFrameCodeGenHelper();
    std::map<IPdu*, PduInstanse> pduMap;
for ( auto itrPdu : frameList )
    {
        IFrame* frame = (IFrame*)(itrPdu.second);

        fileHeader << frameCodeGenHelper->GetDecl(frame) << std::endl;
        fileHeader << TAG_OPENFBRACE << std::endl;
        fileHeader << TAG_PUBLICSTART << std::endl;

        fileHeader << frameCodeGenHelper->GetBaseClassDecl(frame) << std::endl;

        fileHeader << frameCodeGenHelper->GetPdusDecl(frame)<< std::endl;

        pduMap.clear();
        frame->GetPduList(pduMap);
        if ( pduMap.size() == 0 )
        {
            fileHeader << frameCodeGenHelper->GetSignalsDecl(frame)<< std::endl;
        }

        fileHeader << "};" << std::endl;
    }

    //9. Endl Frame namespace
    fileHeader << TAG_CLOSEFBRACE << std::endl;

    //10. Endl cluster namespace
    fileHeader << TAG_CLOSEFBRACE << std::endl;

    //11. using namespaces
    fileHeader << "using namespace" << " " <<clusterName << "::" << "Frame" <<TAG_SEMICOLON << std::endl;
    fileHeader << "using namespace" << " " <<clusterName << "::" << "PDU" <<TAG_SEMICOLON << std::endl;

    return EC_SUCCESS;

}

ERRORCODE NodeSimCodeGenerator::GenerateCppFile(std::fstream& fileHeader, INsCodeGenHelper* codeGenHelper, ICluster* cluster)
{
    //1. Header path
    std::string dbAppPath;
    GetBusMasterDBCachePath(dbAppPath);
    std::string strChkSum;
    std::string headerfilePath;
    cluster->GetDBFileChecksum(strChkSum);

    char headerFilePath[MAX_PATH*2];
    headerfilePath = strChkSum + ".h";
    PathCombine(headerFilePath, dbAppPath.c_str(), headerfilePath.c_str());
    fileHeader <<"#include " << AddDoubleQuotes(headerFilePath) <<std::endl ;

    //2. namespace cluster
    std::string clusterName = GetClusterName(cluster);

    fileHeader <<defNameSpace <<" " << clusterName <<"{" <<std::endl ;

    //3. Cluster Id
    fileHeader << "unsigned long long g_strClusterPath = " <<"0x" << strChkSum  <<TAG_SEMICOLON << std::endl ;

    //4. Pdu namespace
    fileHeader << defNameSpace  <<" " <<"PDU" << TAG_OPENFBRACE <<std::endl;

    //5. pdu definition
    char chTemp[256];
    std::string pduName;
    std::map<UID_ELEMENT, IElement*> pduList;
    cluster->GetElementList(ePduElement, pduList);
    IPduCodeGenHelper* pduCodeGenHelper = codeGenHelper->GetPduCodeGenHelper();
    PduProps pduProps;
for ( auto itrPdu : pduList )
    {
        IPdu* pdu = (IPdu*)(itrPdu.second);
        fileHeader << pduCodeGenHelper->GetConstructorDef(pdu) <<":"  <<std::endl;
        fileHeader << pduCodeGenHelper->GetBaseClassConstructorDef(pdu) <<std::endl;
        fileHeader << pduCodeGenHelper->GetSignalsConstructorDef(pdu) << std::endl;
        fileHeader << TAG_OPENFBRACE << std::endl << TAG_CLOSEFBRACE <<std::endl;
    }

    //6. Endl Pdu namespace
    fileHeader << TAG_CLOSEFBRACE << std::endl;

    //7. Frame Namespace start
    fileHeader << defNameSpace <<" " <<"Frame" << TAG_OPENFBRACE <<std::endl;

    //8. Using namespace
    fileHeader <<"using namespace PDU;" << std::endl;

    //9. Frame definition

    std::string frameName;
    std::map<UID_ELEMENT, IElement*> frameList;
    std::map<IPdu*, PduInstanse> pduMap;
    cluster->GetElementList(eFrameElement, frameList);
    IFrameCodeGenHelper* frameCodeGenHelper = codeGenHelper->GetFrameCodeGenHelper();

for ( auto itrFrame : frameList )
    {
        IFrame* frame = (IFrame*)(itrFrame.second);
        fileHeader << frameCodeGenHelper->GetConstructorDef(frame) <<":"  <<std::endl;
        fileHeader << frameCodeGenHelper->GetBaseClassConstructorDef(frame) <<std::endl;
        fileHeader << frameCodeGenHelper->GetPdusConstructorDef(frame) << std::endl;

        pduMap.clear();
        frame->GetPduList(pduMap);
        if ( pduMap.size() == 0 )
        {
            fileHeader << frameCodeGenHelper->GetSignalsConstructorDef(frame) << std::endl;
        }

        fileHeader << TAG_OPENFBRACE << std::endl << TAG_CLOSEFBRACE <<std::endl;
    }

    //10. Endl Frame namespace
    fileHeader << TAG_CLOSEFBRACE << std::endl;

    //11. Cluster namespace
    fileHeader << TAG_CLOSEFBRACE << std::endl;


    return EC_SUCCESS;
}


bool cNamingConventionFunc(char c)
{
    if ( isalnum(c) )
    {
        return false;
    }
    return true;
}
std::string NodeSimCodeGenerator::GetClusterName(ICluster* pCluster)
{
    if ( nullptr == pCluster )
    {
        return "";
    }
    std::string clusterName;
    pCluster->GetName(clusterName);
    if ( "" == clusterName )
    {
        std::string dbFilePath;
        pCluster->GetDBFilePath(dbFilePath);
        std::string fileName = PathFindFileName(dbFilePath.c_str());

        fileName.erase(std::remove_if(fileName.begin(), fileName.end(), cNamingConventionFunc), fileName.end());


        return fileName;
    }
    return clusterName;
}
bool NodeSimCodeGenerator::bIsDependentFileModified(ICluster* pCluster)
{
    struct stat dbFileAttributes;           // create a file attribute structure
    struct stat codeFileAttributes;         // create a file attribute structure
    std::string strDbFileName;
    std::string strCodeFileName;

    pCluster->GetDBFilePath(strDbFileName);


    int nRetVal = stat(strDbFileName.c_str(), &dbFileAttributes);
    if ( nRetVal < 0 )      // get the attributes of afile.txt
    {
        return true;
    }

    std::string strCheckSum;
    std::string strDbAppPath;
    std::string strGccVer;
    GetBusMasterDBCachePath(strDbAppPath);
    pCluster->GetDBFileChecksum(strCheckSum);
    GetCurrentGccVersion(strGccVer);

    strCodeFileName = strDbAppPath + "\\";
    strCodeFileName += strCheckSum + "_gcc" + strGccVer +".obj";


    nRetVal = stat(strCodeFileName.c_str(), &codeFileAttributes);
    if ( nRetVal < 0 )      // get the attributes of afile.txt
    {
        return true;
    }

    double dDiff = difftime(codeFileAttributes.st_ctime, dbFileAttributes.st_mtime);
    /*if ( fabs(dDiff - 0.0f) < std::numeric_limits<double>::epsilon() )
    {

    }*/
    if ( dDiff > 0.0f )
    {
        return false;
    }
    return true;
}

ERRORCODE NodeSimCodeGenerator::GenerateObjectFile(std::string strCppFilePath, ICluster* pCluster)
{
    PROCESS_INFORMATION sProcessInfo;
    STARTUPINFO         sStartInfo;
    ZeroMemory(&sProcessInfo, sizeof(sProcessInfo) );
    ZeroMemory(&sStartInfo,sizeof(STARTUPINFO));
    sStartInfo.cb           = sizeof(STARTUPINFO);
    //to use the specified handles
    sStartInfo.dwFlags      = STARTF_USESTDHANDLES;
    sStartInfo.hStdOutput   = nullptr;
    sStartInfo.hStdInput    = nullptr;
    sStartInfo.hStdError    = nullptr;
    // Use this if you want to hide the child:
    sStartInfo.wShowWindow  = SW_HIDE;

    CString omStrCommandParam;

    std::string strIncludeDir;
    GetBusmasterNodeSimualtionFolder(NS_INCLUDE_FOLDER, strIncludeDir);

    std::string strCheckSum;
    std::string strDbAppPath;
    std::string strGccVer;
    GetBusMasterDBCachePath(strDbAppPath);
    GetCurrentGccVersion(strGccVer);
    pCluster->GetDBFileChecksum(strCheckSum);



    std::string strObjFile = strDbAppPath + "\\";
    strObjFile += strCheckSum + "_gcc" + strGccVer + ".obj";


    omStrCommandParam = "g++ -DNO_INCLUDE_TEMPSTRUCT -c -I";
    omStrCommandParam += AddDoubleQuotes(strIncludeDir).c_str();
    omStrCommandParam += "\ -xc++ ";
    omStrCommandParam += AddDoubleQuotes(strCppFilePath).c_str();
    omStrCommandParam += " -o";
    omStrCommandParam += AddDoubleQuotes(strObjFile).c_str();


    CreateProcess( nullptr, omStrCommandParam.GetBuffer(omStrCommandParam.GetLength()),
                   nullptr, nullptr,
                   true, CREATE_NO_WINDOW,
                   nullptr, nullptr,
                   &sStartInfo, &sProcessInfo);

    return WaitForSingleObject(sProcessInfo.hProcess, INFINITE);

}

ERRORCODE NodeSimCodeGenerator::GenerateMakeFile(IBMNetWorkGetService* pBmNetWork, ETYPE_BUS eBus, std::string strForCppFile)
{
    std::string strMakeFilePath;
    std::string strLibFolder;
    std::string strSimIncludeFolder;
    GetBusmasterNodeSimualtionFolder(NS_MAKEFILE_FOLDER, strMakeFilePath);
    GetBusmasterNodeSimualtionFolder(NS_LIB_FOLDER, strLibFolder);
    GetBusmasterNodeSimualtionFolder(NS_INCLUDE_FOLDER, strSimIncludeFolder);
    CStdioFile omStdiofile;
    CStdioFile omStdioMakefile;
    CFileException omException ;
    DWORD dwSize                 = 0;
    DWORD dwRead                 = 0;
    BOOL bFileOpen               = FALSE;
    CString omStrFile            = "";


    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath( strForCppFile.c_str(), drive, dir, fname, ext );
    char chAbsPath[MAX_PATH] = {'\0'};
    PathCombine(chAbsPath, strMakeFilePath.c_str(), "GCCDLLMakeTemplate");
    strMakeFilePath = std::string(chAbsPath);

    bFileOpen = omStdiofile.Open(strMakeFilePath.c_str(), CFile::modeRead | CFile::typeText, &omException);
    if(bFileOpen!=FALSE)
    {
        CHAR* pcBuff   = nullptr;
        CString omTemp = "";
        // Get the size of file
        dwSize = (DWORD)omStdiofile.GetLength();
        //pcBuff = (char*) new char[dwSize];
        pcBuff = (char*) new char[dwSize+1];

        if(pcBuff!=nullptr)
        {
            memset(pcBuff, 0, dwSize);
            // Read the whole file and put the content to pcBuff;
            dwRead = omStdiofile.Read(pcBuff,dwSize);
            // Add end of string;
            *(pcBuff+dwRead) = '\0';
            // copy the content to CString object;
            omStrFile = pcBuff;
            // Delete the buffer and initialise it to nullptr
            delete []pcBuff;
            pcBuff = nullptr;
        }

        //1. NS_LIB Folder
        omStrFile.Replace("<BUSMASTER_NS_LIB_FOLDER>", strLibFolder.c_str());

        //2. Protocol Lib
        omStrFile.Replace("<PROTOCOL_HELPER_LIB>", GetProtocolLib(eBus).c_str());

        //3. DB Object
        std::string strCheckSum;
        std::string strDbCachePath;
        std::string strGccVer;
        std::string strObjPath ="";
        std::string strIncPath ="";
        GetCurrentGccVersion(strGccVer);
        GetBusMasterDBCachePath(strDbCachePath);
        int nChannels = 0;
        pBmNetWork->GetChannelCount(eBus, nChannels);
        ICluster* pCluster = nullptr;
        for ( int i = 0; i < nChannels; i++ )
        {
            int nDBCount = 0;
            pBmNetWork->GetDBServiceCount(eBus, i, nDBCount);

            for (int nDbIndex = 0; nDbIndex < nDBCount; nDbIndex++)
            {
                pBmNetWork->GetDBService(eBus, i, nDbIndex, &pCluster);
                strObjPath += "\"";
                strObjPath += strDbCachePath + "\\";

                strIncPath += "-include\"";
                strIncPath += strDbCachePath + "\\";

                if (pCluster != nullptr)
                {
                    pCluster->GetDBFileChecksum(strCheckSum);
                    strObjPath += strCheckSum + "_gcc";
                    strObjPath += strGccVer + ".obj";

                    strIncPath += strCheckSum + ".h";
                }
                strObjPath += "\" ";
                strIncPath += "\" ";
            }
        }
        omStrFile.Replace("<DB_OBJFILES>", strObjPath.c_str());

        //4. DB Include
        omStrFile.Replace("<DB_INCLUDES>", strIncPath.c_str());
        //5. FileName
        omStrFile.Replace("<FILENAME>", fname);

        //6. BM Includes
        omStrFile.Replace("<BM_INCLUDES>", strSimIncludeFolder.c_str());

        //7. file path
        char chShortPath[1024];
        GetShortPathName(strForCppFile.c_str(), chShortPath, 1024);
        CString strTemp = chShortPath;
        strTemp.Replace("\\", "/");
        omStrFile.Replace("<FILEPATH>", strTemp);

        std::strcat(fname, "make");
        PathCombine(chAbsPath, drive, dir);
        char chFullPath[BM_MAX_PATH] = { '\0' };
        PathCombine(chFullPath, chAbsPath, fname);
        std::string strMakeFileName = std::string(chFullPath);
        bFileOpen = omStdioMakefile.Open(strMakeFileName.c_str(), CFile::modeWrite | CFile::typeText | CFile::modeCreate, &omException);

        if(bFileOpen!=FALSE)
        {
            omStdioMakefile.WriteString(omStrFile);
        }
    }
    return S_OK;
}

std::string NodeSimCodeGenerator::GetProtocolLib(ETYPE_BUS eBus)
{
    std::string strLib = "Wrapper_" + getBusInString(eBus);
    return strLib;
}

ERRORCODE NodeSimCodeGenerator::CompileCode(ICluster* pCluster, bool bRegenerate, bool bReCompile, INsCodeGenHelper* codeGenHelper)
{
    if (nullptr == codeGenHelper)
    {
        return EC_FAILURE;
    }

    ERRORCODE ecRetVal = EC_FAILURE;
    std::string strFileName;

    pCluster->GetDBFilePath(strFileName);

    if ( bRegenerate == false )
    {
        bRegenerate = bIsDependentFileModified(pCluster);
    }
    if ( bRegenerate == true )              //Not else
    {
        if ( "" != strFileName )
        {
            std::string strCheckSum;
            pCluster->GetDBFileChecksum(strCheckSum);

            char chTempFolder[MAX_PATH];
            char chTempPath[MAX_PATH];
            GetTempPath( MAX_PATH, chTempFolder );
            GetTempFileName(chTempFolder, // directory for tmp files
                            TEXT("BM"),     // temp file name prefix
                            0,                // create unique name
                            chTempPath);  // buffer for name

            std::string strCppFilePath =  chTempPath; //+ ".cpp";






            std::string strDbAppPath;
            GetBusMasterDBCachePath(strDbAppPath);




            std::string strHeaderFilePath = strDbAppPath + "\\";
            strHeaderFilePath += strCheckSum + ".h";

            GenerateCode(strHeaderFilePath, strCppFilePath, pCluster, codeGenHelper);
            ecRetVal = GenerateObjectFile(strCppFilePath, pCluster);

            //DeleteFile(chTempPath);
        }
        else
        {
            ecRetVal = EC_FAILURE;
        }
    }

    return ecRetVal;
}



