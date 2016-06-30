#include "ConvertCpp.h"

void CConvertCpp::vReplaceDataBytes(std::regex rgx, std::string omData, std::string omReplaceStr, std::string& readLine)
{
    std::smatch match;

    if (std::regex_search(readLine, match, rgx))
    {
for (auto itrMatch : match)
        {
            std::string omstrMatch = itrMatch.str();

            std::string omMsgObjName = "", omByte = "";

            int nDataIndex = omstrMatch.find(omData);
            if (-1 != nDataIndex)
            {
                omMsgObjName = omstrMatch.substr(0, nDataIndex);
            }
            else
            {
                continue;
            }
            nDataIndex = omstrMatch.find("[");
            int nDataIndex1 = omstrMatch.find("]");

            if (-1 != nDataIndex && -1 != nDataIndex1)
            {
                omByte = omstrMatch.substr(nDataIndex + 1, nDataIndex1 - nDataIndex - 1);
            }

            int nReplaceINdex = readLine.length() - nDataIndex1;
            char achInput[500];
            sprintf(achInput, omReplaceStr.c_str(), omMsgObjName.c_str(), atoi(omByte.c_str()));
            int nStart = readLine.find(omstrMatch);
            readLine.replace(nStart, nDataIndex1 + 1, achInput);
        }
    }
}

void CConvertCpp::vReplaceStructVars(std::string& readLine)
{
    int nIndex = readLine.find(defST_MSG_ID.c_str());
    if (-1 != nIndex)
    {
        readLine.replace(nIndex, defST_MSG_ID.length(), defDB_MSG_ID);
    }

    nIndex = readLine.find(defST_EXT_ID);
    if (-1 != nIndex)
    {
        readLine.replace(nIndex, defST_EXT_ID.length(), defDB_EXT_ID);
    }

    nIndex = readLine.find(defST_RTR);
    if (-1 != nIndex)
    {
        readLine.replace(nIndex, defST_RTR.length(), defDB_RTR);
    }

    nIndex = readLine.find(defST_DLC);
    if (-1 != nIndex)
    {
        readLine.replace(nIndex, defST_DLC.length(), defDB_DLC);
    }

    nIndex = readLine.find(defST_CHANNEL);
    if (-1 != nIndex)
    {
        readLine.replace(nIndex, defST_CHANNEL.length(), defDB_CHANNEL);
    }

    nIndex = readLine.find(defST_CANFD);
    if (-1 != nIndex)
    {
        readLine.replace(nIndex, defST_CANFD.length(), defDB_CANFD);
    }

    nIndex = readLine.find(defST_LONGDATA);
    if (-1 != nIndex)
    {
        std::regex rgx("([a-zA-Z_]+\\.[m_sWhichBit]+\\.[m_aulData]+\\[[0-9]+\\])");
        std::string omDataVar = ".m_sWhichBit.m_aulData";

        vReplaceDataBytes(rgx, omDataVar, "*(longAt(&%s,%d))", readLine);
    }
    nIndex = readLine.find(defST_WORDDATA);
    if (-1 != nIndex)
    {
        std::regex rgx("([a-zA-Z_]+\\.[m_sWhichBit]+\\.[m_auwData]+\\[[0-9]+\\])");
        std::string omDataVar = ".m_sWhichBit.m_auwData";

        vReplaceDataBytes(rgx, omDataVar, "*(wordAt(&%s,%d))", readLine);
    }
    nIndex = readLine.find(defST_BYTEDATA);
    if (-1 != nIndex)
    {
        readLine.replace(nIndex, defST_BYTEDATA.length(), defDB_DATA);
    }
    nIndex = readLine.find(defST_WHICH_BIT);
    if (-1 != nIndex)
    {
        readLine.replace(nIndex, defST_WHICH_BIT.length(), "");
    }

    nIndex = readLine.find(defST_LONGDATA);
    if (-1 != nIndex)
    {
        readLine.replace(nIndex, defST_BYTEDATA.length(), defDB_BYTEDATA);
    }
}

bool CConvertCpp::bCheckIfOldCppFile(CString& omText)
{
    CString omTemp = C_FILE_VERSION_VALUE;

    float nCurrentVersion = atof(omTemp);
    CString omVersion;

    int nIndex = omText.Find('[');
    omVersion = omText.Mid(nIndex + 1, omText.ReverseFind('.') - nIndex + 1);
    CString omTemp1 = omVersion;

    float nVersion = atof(omTemp1);

    // If Cpp file version is less than the current version
    if (nVersion < nCurrentVersion)
    {
        omText.Replace(omVersion, omTemp);
        return true;
    }

    return false;
}

CString CConvertCpp::omStrExtractFunctionNameFromPrototype(CString omStrPrototype)
{
    CString omStrFuncName = "";

    if (!omStrPrototype.IsEmpty())
    {
        // Find open paranthesis in the defn
        int nIndex = omStrPrototype.Find("(");

        if (nIndex != -1)
        {
            // Found, Get left of open paranthesis
            omStrPrototype = omStrPrototype.Left(nIndex);

            // Remove void from "void funcname" from the string
            int nIndex = omStrPrototype.ReverseFind(SPACE);

            omStrPrototype =
                omStrPrototype.Right(omStrPrototype.GetLength() - (nIndex + 1));

            omStrPrototype.TrimRight();

            omStrFuncName = omStrPrototype;
        }
    }
    return omStrFuncName;
}

bool CConvertCpp::bCreateCppBackupFile(std::string omFilePath, ETYPE_BUS eBus, std::string& omBakFilePath)
{
    ifstream inputFile(omFilePath.c_str());

    bool bIsOldCppFile = false;
    std::list<std::string> lstWrapperFunctions;
    std::string readLine;
    CString omTextLine;
    while (std::getline(inputFile, readLine))
    {
        omTextLine = readLine.c_str();

        if (-1 == omTextLine.Find(BUSMASTER_VERSION_TAG) && -1 != omTextLine.Find(VERSION_TAG))
        {
            bIsOldCppFile = bCheckIfOldCppFile(omTextLine);

            if (true == bIsOldCppFile)
            {
                if (eBus == CAN)
                {
                    std::string omFileNew = omFilePath + ".bak";
                    ofstream backupFile(omFileNew);
                    backupFile << inputFile.rdbuf();
                    inputFile.close();
                    backupFile.close();
                }
                return true;
            }
            break;
        }
    }
    return false;
}

bool CConvertCpp::bCheckDllVersion(std::string omDllFilePath)
{
    BOOL bVersion = FALSE; //ell2kor
    int iMajorVer;
    int  iMinorVer;
    bool bFreeLibrary = false;

    HINSTANCE hModuleHandle = LoadLibrary(omDllFilePath.c_str());
    if (hModuleHandle != nullptr)
    {
        //Get the function pointer of the bGetProgramVersion()
        PFGET_PRG_VER pFGetPrgVer = (PFGET_PRG_VER)
                                    GetProcAddress(hModuleHandle,
                                            "bGetProgramVersion");
        if (pFGetPrgVer != nullptr)
        {
            bVersion = pFGetPrgVer(&iMajorVer, &iMinorVer, hModuleHandle);
            if (bVersion == TRUE)
            {
                if (iMajorVer < defMAJOR_VERSION)
                {
                    FreeLibrary(hModuleHandle);
                    return false;
                }
            }
        }
    }

    return true;
}

bool CConvertCpp::convertCppFile(std::string omFilePath, CString omBusName, ETYPE_BUS eBus)
{
    std::string omFileNew = omFilePath + ".bak";
    bool bIsOldFile = bCreateCppBackupFile(omFilePath, eBus, omFileNew);

    if (false == bIsOldFile)
    {
        return false;
    }

    ifstream cppFile(omFilePath);
    ofstream backupFile(omFileNew);

    backupFile << cppFile.rdbuf();
    cppFile.close();
    backupFile.close();

    bool bIsOldCppFile = false;
    std::string readLine;
    CString omTextLine;

    CString omBusFNPrototype = BUS_FN_PROTOTYPE_FOOTER;
    omBusFNPrototype.Replace(_("PLACE_HODLER_FOR_BUSNAME"), omBusName);

    ifstream inputFile(omFileNew.c_str());
    ofstream outputFile(omFilePath);

    std::list<std::string> lstWrapperFunctions;

    while (std::getline(inputFile, readLine))
    {
        omTextLine = readLine.c_str();

        if (-1 == omTextLine.Find(BUSMASTER_VERSION_TAG) && -1 != omTextLine.Find(VERSION_TAG))
        {
            bIsOldCppFile = bCheckIfOldCppFile(omTextLine);
            readLine = omTextLine;
        }
        else if (-1 != omTextLine.Find(defHEADER_FILE_NAME))
        {
            continue;
        }

        // On Msgname declaration
        if (true == bIsOldCppFile && omTextLine.Find(defGCC_EXTERN) != -1
                && omTextLine.Find(CGlobalObj::omGetBusSpecMsgHndlrName(eBus) + defMSG_NAME_HANDLER) != -1)
        {
            CString omTextLineTemp = omTextLine;
            CString omFuncPrototype = omStrExtractFunctionNameFromPrototype(omTextLine);
            CString omMsgName = omFuncPrototype;
            //mapWrapperToFunction[std::string(omFuncPrototype)+"_Wrapper"] = omFuncPrototype;
            lstWrapperFunctions.push_back(std::string(omFuncPrototype));
            omMsgName.Replace((CGlobalObj::omGetBusSpecMsgHndlrName(eBus) + defMSG_NAME_HANDLER), "");

            omTextLineTemp.Replace(omFuncPrototype, omFuncPrototype + "_Wrapper");
            omTextLineTemp.Replace(omMsgName + " ", CGlobalObj::omGetBusSpecMsgStructName(eBus) + " ");

            omFuncPrototype = "void " + omFuncPrototype + "(" + omMsgName + " RxMsg);" + "\n" + omTextLineTemp;

            outputFile << omFuncPrototype + "\n";
        }
        else if (omTextLine.Find(omBusFNPrototype) != -1)
        {
            outputFile << readLine + "\n";

            CString omBusFNPrototype = BUS_FN_WRAPPER_PROTOTYPE_HDR;
            omBusFNPrototype.Replace(_("PLACE_HODLER_FOR_BUSNAME"), omBusName);

            outputFile << "\n" + omBusFNPrototype + "\n";

            std::list<std::string>::iterator itrWrapper = lstWrapperFunctions.begin();
            while (itrWrapper != lstWrapperFunctions.end())
            {
                std::string omWrapper = *itrWrapper + "_Wrapper";
                char omStrWrapper[MAX_PATH] = "", omMsgHandlerFunc[MAX_PATH];
                sprintf(omStrWrapper, defDEFAULT_MSG_HANDLER_WRAPPER_CODE, omWrapper.c_str(), (CGlobalObj::omGetBusSpecMsgStructName(eBus)));
                sprintf(omMsgHandlerFunc, defMSGHANDLER_FUNCTION, (*itrWrapper).c_str());

                outputFile << std::string(omStrWrapper).c_str() << std::string(omMsgHandlerFunc).c_str() << "\n";
                itrWrapper++;
            }

            omBusFNPrototype = BUS_FN_WRAPPER_PROTOTYPE_FOOTER;
            omBusFNPrototype.Replace(_("PLACE_HODLER_FOR_BUSNAME"), omBusName);

            outputFile << omBusFNPrototype + "\n";
        }
        else
        {
            vReplaceStructVars(readLine);

            outputFile << readLine + "\n";
        }
    }
    inputFile.close();
    outputFile.close();

    return true;
}