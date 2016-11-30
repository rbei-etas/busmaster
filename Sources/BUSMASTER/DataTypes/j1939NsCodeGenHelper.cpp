///////////////////////////////////////////////////////////
//  J1939NsCodeGenHelper.cpp
//  Implementation of the Class J1939NsCodeGenHelper
//  Created on:      09-Oct-2015 14:37:41
//  Original author: vme1kor
///////////////////////////////////////////////////////////
#include "DataTypes_stdafx.h"
#include "J1939NsCodeGenHelper.h"
#include "CANDefines.h"
#define defTab                  "\t"
#define defColon                ":"
#define defSpace                " "
#define defSemiColon            ";"
#define defCANBaseMsg           "BMJ1939Msg"
#define defBaseSignal           "BMSignal"

J1939NsCodeGenHelper::J1939NsCodeGenHelper()
{

}

J1939NsCodeGenHelper::~J1939NsCodeGenHelper()
{

}

std::string J1939NsCodeGenHelper::GetFrameBaseClassInitialiser(IFrame* /*frame*/)
{

    return  NULL;
}


std::string J1939NsCodeGenHelper::GetFrameBaseClassName()
{

    return  NULL;
}


std::string J1939NsCodeGenHelper::GetFrameClassName()
{

    return  NULL;
}


std::string J1939NsCodeGenHelper::GetFrameName(IFrame* /*frame*/)
{

    return  NULL;
}

std::string J1939NsCodeGenHelper::GetFramePdusInitialisation(IFrame* /*frame*/, IPdu* /*pdu*/)
{

    return  NULL;
}

std::string J1939NsCodeGenHelper::GetFrameSignalsInitialiser(IFrame* /*frame*/)
{

    return  NULL;
}

std::string J1939NsCodeGenHelper::GetPdusDecl(IFrame* /*frame*/)
{
    return "";
}

std::string J1939NsCodeGenHelper::getPduBaseClassInitialiser(IPdu* /*pdu*/)
{

    return  NULL;
}


std::string J1939NsCodeGenHelper::GetPduBaseClassName()
{

    return  NULL;
}


std::string J1939NsCodeGenHelper::GetPduClassName()
{

    return  NULL;
}


std::string J1939NsCodeGenHelper::GetPduInitialiser(IPdu* /*pdu*/)
{

    return  NULL;
}


std::string J1939NsCodeGenHelper::GetPduName(IPdu* /*pdu*/)
{

    return  NULL;
}


std::string J1939NsCodeGenHelper::GetPduSignalInitialiser(IPdu* /*pdu*/, ISignal* /*signal*/)
{
	return  NULL;
}

std::string J1939NsCodeGenHelper::GetProtocolIncludesHeaderName(ICluster* /*pCluster*/)
{
    return "#include \"J1939Includes.h\"";
}

std::string J1939NsCodeGenHelper::GetSignalName(ISignal* /*signal*/)
{
	return  NULL;
}

void J1939NsCodeGenHelper::SetCluster(ICluster* cluster)
{
    mCluster = cluster;
}

IPduCodeGenHelper* J1939NsCodeGenHelper::GetPduCodeGenHelper()
{
    return this;
}
IFrameCodeGenHelper* J1939NsCodeGenHelper::GetFrameCodeGenHelper()
{
    return this;
}

std::string J1939NsCodeGenHelper::GetConstructorDef(IFrame* frame)
{
    std::ostringstream outStream;
    std::string frameUniqueName = GetUniqueName(frame);
    outStream << frameUniqueName << "::" << frameUniqueName << "(J1939_MSG* objJ1939Msg)" << std::endl;
    return outStream.str();
}

std::string J1939NsCodeGenHelper::GetBaseClassConstructorDef(IFrame* frame)
{
    std::ostringstream outStream;

    unsigned int frameId = 0;
    unsigned int dlc = 0;
    std::string frameUniqueName = GetUniqueName(frame);

    frame->GetFrameId(frameId);
    frame->GetLength(dlc);

    CANFrameProps eouFrameProps;
    frame->GetProperties(eouFrameProps);
    bool bIsExtended = 0;
    if (eCan_Extended == eouFrameProps.m_canMsgType)
    {
        bIsExtended = 1;
    }

    //BMLINMsg(std::string _name, unsigned char _checksumtype, int _id, int _dlc, STLIN_MSG* pMsg = NULL)

    outStream << defCANBaseMsg << "(" << "\"" << frameUniqueName << "\"" << "," << frameId << ", " << dlc << ", " << "objJ1939Msg" << ")";


    return outStream.str();
}

std::string J1939NsCodeGenHelper::GetUniqueName(IFrame* frame)
{
    std::string frameName;
    frame->GetName(frameName);
    return frameName;
}

std::string J1939NsCodeGenHelper::GetBaseClassDecl(IFrame* frame)
{
    std::ostringstream outStream;
    outStream << GetUniqueName(frame) << "(J1939_MSG* objJ1939Msg = NULL);";
    return outStream.str();
}

std::string J1939NsCodeGenHelper::GetPdusConstructorDef(IFrame* /*frame*/)
{
    return "";
}

std::string J1939NsCodeGenHelper::GetSignalsConstructorDef(IFrame* frame)
{
    std::map<ISignal*, SignalInstanse> ouSignalList;
    std::string frameName;
    std::ostringstream outStream;
    frame->GetSignalList(ouSignalList);
    frameName = GetUniqueName(frame);

    int nSignalCount = ouSignalList.size();
    int iSignalIndex = 0;

    std::map<std::string, std::string> signalMap;
    std::string signalName;

    for (auto itr = ouSignalList.begin(); itr != ouSignalList.end(); ++itr)
    {
        itr->first->GetName(signalName);
        if (signalMap.end() == signalMap.find(signalName))
        {
            signalMap[signalName] = signalName;
            if (iSignalIndex < nSignalCount)
            {
                outStream << "," << std::endl;
                iSignalIndex++;
            }
            outStream << GenerateSignalConstuctor(itr->second.m_nStartBit, itr->first, itr->second.m_ouSignalEndianess, frameName);
        }
    }
    return  outStream.str();
}


std::string J1939NsCodeGenHelper::GenerateSignalConstuctor(int startBit, ISignal* pouSignal, eEndianess ouEndianness, std::string strContainerName)
{
    std::ostringstream outStream;

    // BMSignal(std::string name, bool bIntel, int startBit, int length, int Type, unsigned char* pchData):
    std::string strName;
    pouSignal->GetName(strName);
    unsigned int unLength;
    pouSignal->GetLength(unLength);

    //1. Name
    if (strContainerName == strName)
    {
        outStream << defTab << strName << "_signal" << "(" << "2 ," << "g_strClusterPath" << ",";
    }
    else
    {
        outStream << defTab << strName << "(" << "2 ," <<  "g_strClusterPath" << ",";
    }
    //2. Big or Littelendian
    if (ouEndianness == eIntel)
    {
        outStream << " true,";
    }
    else
    {
        outStream << " false,";
    }

    //3. START BIT
    outStream << startBit << ", ";

    //4.
    outStream << unLength << ", ";

    //5.
    CANSignalProps signalProps;
    pouSignal->GetProperties(signalProps);
    if (signalProps.m_ouDataType == eSignalDataType::eSigned)
    {
        outStream << 1 << ", ";
    }
    else
    {
        outStream << 0 << ", ";
    }
    
    //6.
    outStream << "data" << ", ";

    //7.
    ICoding* pouCoding;
    pouSignal->GetEncoding(&pouCoding);
    strName = "";
    if (pouCoding != nullptr)
    {
        pouCoding->GetName(strName);
    }
    outStream << "\"" << strName << "\")";

    return outStream.str();
}

std::string J1939NsCodeGenHelper::GetDecl(IFrame* frame)
{
    std::ostringstream outStream;
    outStream << "class" << " " << GetUniqueName(frame) << ":" << "public" << " " << defCANBaseMsg;
    return outStream.str();
}

std::string J1939NsCodeGenHelper::GetSignalsDecl(IFrame* frame)
{
    std::map<ISignal*, SignalInstanse> ouSignalList;
    std::ostringstream outStream;

    frame->GetSignalList(ouSignalList);


    std::string containerName = GetUniqueName(frame);
    std::string retVal;
    for (auto itr = ouSignalList.begin(); itr != ouSignalList.end(); ++itr)
    {
        retVal += GenerateSignalDecl(itr->first, containerName) + "\n";
    }
    return retVal;
}


std::string J1939NsCodeGenHelper::GenerateSignalDecl(ISignal* pouSignal, std::string strContainerName)
{
    std::string strName;
    std::string strSignalType;
    std::ostringstream outStream;
    pouSignal->GetName(strName);

    GetSignalType(pouSignal, strSignalType);

    if (strContainerName == strName)
    {
        outStream << defTab << defBaseSignal << "<" << strSignalType << ">"
                  << defSpace << strName << "_signal" << defSemiColon;
    }
    else
    {
        outStream << defTab << defBaseSignal << "<" << strSignalType << ">"
                  << defSpace << strName << defSemiColon;
    }
    return outStream.str();
}


void J1939NsCodeGenHelper::GetSignalType(ISignal* pouSignal, std::string& strSignalType)
{
    unsigned int nLength = 0;
    pouSignal->GetLength(nLength);

    if (nLength <= 8)
    {
        strSignalType = "char";
    }
    else if (nLength > 8 && nLength <= 16)
    {
        strSignalType = "short";
    }

    else if (nLength > 16 && nLength <= 32)
    {
        strSignalType = "int";
    }

    else if (nLength > 32 && nLength <= 64)
    {
        strSignalType = "__int64";
    }

    CANSignalProps ouSignalProps;
    eSignalDataType eSignalType = eUnsigned;
    pouSignal->GetProperties(ouSignalProps);

	eSignalType = ouSignalProps.m_ouDataType;


    if (eSignalType == eSignalDataType::eUnsigned)
    {
        strSignalType = "unsigned " + strSignalType;
    }

}

std::string J1939NsCodeGenHelper::GetUniqueName(IPdu* /*pdu*/)
{
    return "";
}

std::string J1939NsCodeGenHelper::GetConstructorDef(IPdu* /*pdu*/)
{
    return "";
}


std::string J1939NsCodeGenHelper::GetBaseClassConstructorDef(IPdu* /*pdu*/)
{
    return "";
}

std::string J1939NsCodeGenHelper::GetSignalsConstructorDef(IPdu* /*pdu*/)
{
    return "";
}

std::string J1939NsCodeGenHelper::GetDecl(IPdu* /*pdu*/)
{
    return "";
}

std::string J1939NsCodeGenHelper::GetBaseClassDecl(IPdu* /*pdu*/)
{
    return "";
}

std::string J1939NsCodeGenHelper::GetSignalsDecl(IPdu* /*pdu*/)
{
    return "";
}


INsCodeGenHelper* __stdcall J1939NsCodeGenHelper::Create()
{

    return  new J1939NsCodeGenHelper();
}

std::string J1939NsCodeGenHelper::GetCheckSumType()
{

    return "0";
}