///////////////////////////////////////////////////////////
//  CanNsCodeGenHelper.cpp
//  Implementation of the Class CanNsCodeGenHelper
//  Created on:      09-Oct-2015 14:37:41
//  Original author: vme1kor
///////////////////////////////////////////////////////////
#include "DataTypes_stdafx.h"
#include "CanNsCodeGenHelper.h"

#define defTab                  "\t"
#define defColon                ":"
#define defSpace                " "
#define defSemiColon            ";"
#define defCANBaseMsg           "DBCanMsg"
#define defBaseSignal           "BMSignal"
CanNsCodeGenHelper::CanNsCodeGenHelper()
{

}



CanNsCodeGenHelper::~CanNsCodeGenHelper()
{

}





std::string CanNsCodeGenHelper::GetFrameBaseClassInitialiser(IFrame* frame)
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetFrameBaseClassName()
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetFrameClassName()
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetFrameName(IFrame* frame)
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetFramePdusInitialisation(IFrame* frame, IPdu* pdu)
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetFrameSignalsInitialiser(IFrame* frame)
{

    return  NULL;
}

std::string CanNsCodeGenHelper::GetPdusDecl(IFrame* frame)
{
    return "";
}

std::string CanNsCodeGenHelper::getPduBaseClassInitialiser(IPdu* pdu)
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetPduBaseClassName()
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetPduClassName()
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetPduInitialiser(IPdu* pdu)
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetPduName(IPdu* pdu)
{

    return  NULL;
}


std::string CanNsCodeGenHelper::GetPduSignalInitialiser(IPdu* pdu, ISignal* signal)
{

    return  NULL;
}

std::string CanNsCodeGenHelper::GetProtocolIncludesHeaderName(ICluster* pCluster)
{
    return "#include \"CANIncludes.h\"";
}


std::string CanNsCodeGenHelper::GetSignalName(ISignal* signal)
{

    return  NULL;
}


void CanNsCodeGenHelper::SetCluster(ICluster* cluster)
{
    mCluster = cluster;
}

IPduCodeGenHelper* CanNsCodeGenHelper::GetPduCodeGenHelper()
{
    return this;
}
IFrameCodeGenHelper* CanNsCodeGenHelper::GetFrameCodeGenHelper()
{
    return this;
}

std::string CanNsCodeGenHelper::GetConstructorDef(IFrame* frame)
{
    std::ostringstream outStream;
    std::string frameUniqueName = GetUniqueName(frame);
    outStream << frameUniqueName << "::" << frameUniqueName << "(STCAN_MSG* objCanMsg)" << std::endl;
    return outStream.str();
}


std::string CanNsCodeGenHelper::GetBaseClassConstructorDef(IFrame* frame)
{
    std::ostringstream outStream;

    unsigned int frameId = 0;
    unsigned int dlc = 0;
    std::string frameUniqueName = GetUniqueName(frame);

    frame->GetFrameId(frameId);
    frame->GetLength(dlc);

    FrameProps eouFrameProps;
    frame->GetProperties(eouFrameProps);
    bool bIsExtended = 0;
    if (eCan_Extended == eouFrameProps.m_ouCANFrameProp.m_canMsgType)
    {
        bIsExtended = 1;
    }

    //BMLINMsg(std::string _name, unsigned char _checksumtype, int _id, int _dlc, STLIN_MSG* pMsg = NULL)

    outStream << defCANBaseMsg << "(" << "\"" << frameUniqueName << "\"" << "," << bIsExtended << ", " << frameId << ", " << dlc << ", " << "objCanMsg" << ")";


    return outStream.str();
}

std::string CanNsCodeGenHelper::GetUniqueName(IFrame* frame)
{
    std::string frameName;
    frame->GetName(frameName);
    return frameName;
}

std::string CanNsCodeGenHelper::GetBaseClassDecl(IFrame* frame)
{
    std::ostringstream outStream;
    outStream << GetUniqueName(frame) << "(STCAN_MSG* objCanMsg = NULL);";
    return outStream.str();
}

std::string CanNsCodeGenHelper::GetPdusConstructorDef(IFrame* frame)
{
    return "";
}

std::string CanNsCodeGenHelper::GetSignalsConstructorDef(IFrame* frame)
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


std::string CanNsCodeGenHelper::GenerateSignalConstuctor(int startBit, ISignal* pouSignal, eEndianess ouEndianness, std::string strContainerName)
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
        outStream << defTab << strName << "_signal" << "(" << "0 ," << "g_strClusterPath" << ",";
    }
    else
    {
        outStream << defTab << strName << "(" << "0 ," <<  "g_strClusterPath" << ",";
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
    SignalProps signalProps;
    pouSignal->GetProperties(signalProps);
    if (signalProps.m_ouCANSignalProps.m_eSignalValueType == eSignalDataType::eSigned)
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

std::string CanNsCodeGenHelper::GetDecl(IFrame* frame)
{
    std::ostringstream outStream;
    outStream << "class" << " " << GetUniqueName(frame) << ":" << "public" << " " << defCANBaseMsg;
    return outStream.str();
}

std::string CanNsCodeGenHelper::GetSignalsDecl(IFrame* frame)
{
    std::map<ISignal*, SignalInstanse> ouSignalList;
    std::ostringstream outStream;

    frame->GetSignalList(ouSignalList);

    std::list<std::string> lstSignals;
    std::string containerName = GetUniqueName(frame);
    std::string retVal;
    std::string omSignalName = "";
    for (auto itr = ouSignalList.begin(); itr != ouSignalList.end(); ++itr)
    {
        itr->first->GetName(omSignalName);

        if (std::find(lstSignals.begin(), lstSignals.end(), omSignalName) == lstSignals.end())
        {
            retVal += GenerateSignalDecl(itr->first, containerName) + "\n";
            lstSignals.push_back(omSignalName);
        }
    }
    return retVal;
}


std::string CanNsCodeGenHelper::GenerateSignalDecl(ISignal* pouSignal, std::string strContainerName)
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


void CanNsCodeGenHelper::GetSignalType(ISignal* pouSignal, std::string& strSignalType)
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

    SignalProps ouSignalProps;
    eSignalDataType eSignalType = eUnsigned;
    pouSignal->GetProperties(ouSignalProps);

    switch (ouSignalProps.eType)
    {
        case eCANProtocol:
            eSignalType = eSignalDataType::eUnsigned;
            break;
        case eLINProtocol:
            eSignalType = ouSignalProps.m_ouLINSignalProps.m_ouDataType;
            break;

        case eInvalidProtocol:
            break;
        default:
            break;
    }

    if (eSignalType == eSignalDataType::eUnsigned)
    {
        strSignalType = "unsigned " + strSignalType;
    }

}

std::string CanNsCodeGenHelper::GetUniqueName(IPdu* pdu)
{
    return "";
}

std::string CanNsCodeGenHelper::GetConstructorDef(IPdu* pdu)
{
    return "";
}


std::string CanNsCodeGenHelper::GetBaseClassConstructorDef(IPdu* pdu)
{
    return "";
}

std::string CanNsCodeGenHelper::GetSignalsConstructorDef(IPdu* pdu)
{
    return "";
}

std::string CanNsCodeGenHelper::GetDecl(IPdu* pdu)
{
    return "";
}

std::string CanNsCodeGenHelper::GetBaseClassDecl(IPdu* pdu)
{
    return "";
}

std::string CanNsCodeGenHelper::GetSignalsDecl(IPdu* pdu)
{
    return "";
}


INsCodeGenHelper* __stdcall CanNsCodeGenHelper::Create()
{

    return  new CanNsCodeGenHelper();
}

std::string CanNsCodeGenHelper::GetCheckSumType()
{

    return "0";
}