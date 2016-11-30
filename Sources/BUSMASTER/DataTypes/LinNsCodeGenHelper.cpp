
#include "DataTypes_stdafx.h"
#include "LinNsCodeGenHelper.h"
#include "LINDefines.h"
#define defTab                  "\t"
#define defColon                ":"
#define defSpace                " "
#define defSemiColon            ";"

#define defLinBaseMsg           "DBLinMsg"
#define defBaseSignal           "BMSignal"


LinNsCodeGenHelper::LinNsCodeGenHelper()
{

}



LinNsCodeGenHelper::~LinNsCodeGenHelper()
{

}



void LinNsCodeGenHelper::SetCluster(ICluster* cluster)
{
    mCluster = cluster;
}


std::string LinNsCodeGenHelper::GetProtocolIncludesHeaderName(ICluster* /*pCluster*/)
{
    return "#include \"LINIncludes.h\"";
}

IPduCodeGenHelper* LinNsCodeGenHelper::GetPduCodeGenHelper()
{
    return this;
}
IFrameCodeGenHelper* LinNsCodeGenHelper::GetFrameCodeGenHelper()
{
    return this;
}


std::string LinNsCodeGenHelper::GetConstructorDef(IPdu* /*pdu*/)
{
    return "";
}


std::string LinNsCodeGenHelper::GetBaseClassConstructorDef(IPdu* /*pdu*/)
{
    return "";
}


std::string LinNsCodeGenHelper::GetSignalsConstructorDef(IPdu* /*pdu*/)
{
    return "";
}

std::string LinNsCodeGenHelper::GetUniqueName(IPdu* /*pdu*/)
{
    return "";
}

std::string LinNsCodeGenHelper::GetDecl(IPdu* /*pdu*/)
{
    return "";
}

std::string LinNsCodeGenHelper::GetBaseClassDecl(IPdu* /*pdu*/)
{
    return "";
}

std::string LinNsCodeGenHelper::GetSignalsDecl(IPdu* /*pdu*/)
{
    return "";
}



std::string LinNsCodeGenHelper::GetConstructorDef(IFrame* frame)
{
    std::ostringstream outStream;
    std::string frameUniqueName = GetUniqueName(frame);
    outStream << frameUniqueName <<"::" << frameUniqueName << "(STLIN_MSG* objLinMsg)" <<std::endl;
    return outStream.str();
}

std::string LinNsCodeGenHelper::GetBaseClassConstructorDef(IFrame* frame)
{
    std::ostringstream outStream;

    unsigned int frameId = 0;
    unsigned int dlc = 0;
    std::string frameUniqueName = GetUniqueName(frame);

    frame->GetFrameId(frameId);
    frame->GetLength(dlc);

    //BMLINMsg(std::string _name, unsigned char _checksumtype, int _id, int _dlc, STLIN_MSG* pMsg = NULL)

    outStream << defLinBaseMsg << "(" <<"\"" << frameUniqueName << "\"" << "," << GetCheckSumType() << "," << frameId << "," <<dlc
              << "," <<"objLinMsg" << ")";


    return outStream.str();
}

std::string LinNsCodeGenHelper::GetPdusConstructorDef(IFrame* /*frame*/)
{
    return "";
}

std::string LinNsCodeGenHelper::GetSignalsConstructorDef(IFrame* frame)
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

    for( auto itr = ouSignalList.begin(); itr != ouSignalList.end(); ++itr )
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

std::string LinNsCodeGenHelper::GetUniqueName(IFrame* frame)
{
    std::string frameName;
    frame->GetName(frameName);
    return frameName;
}

std::string LinNsCodeGenHelper::GetDecl(IFrame* frame)
{
    std::ostringstream outStream;
    outStream << "class" <<" " << GetUniqueName(frame) << ":" << "public" << " " << defLinBaseMsg;
    return outStream.str();
}

std::string LinNsCodeGenHelper::GetBaseClassDecl(IFrame* frame)
{
    std::ostringstream outStream;
    outStream << GetUniqueName(frame) << "(STLIN_MSG* objLinMsg = NULL);";
    return outStream.str();
}

std::string LinNsCodeGenHelper::GetPdusDecl(IFrame* /*frame*/)
{
    return "";
}

std::string LinNsCodeGenHelper::GetSignalsDecl(IFrame* frame)
{
    std::map<ISignal*, SignalInstanse> ouSignalList;
    std::ostringstream outStream;

    frame->GetSignalList(ouSignalList);


    std::string containerName = GetUniqueName(frame);
    std::string retVal;
    for( auto itr = ouSignalList.begin(); itr != ouSignalList.end(); ++itr )
    {
        retVal += GenerateSignalDecl( itr->first, containerName) + "\n";
    }
    return retVal;
}

INsCodeGenHelper* LinNsCodeGenHelper::Create()
{

    return  new LinNsCodeGenHelper();
}

std::string LinNsCodeGenHelper::GetCheckSumType()
{
    LIN_Settings linSettings;
    mCluster->GetProperties(eLINClusterProperties, &linSettings);
    if ( linSettings.m_dProtocolVers > 1.3f )
    {
        return "1";
    }
    return "0";
}


std::string LinNsCodeGenHelper::GenerateSignalConstuctor(  int startBit, ISignal* pouSignal, eEndianess ouEndianness, std::string strContainerName)
{
    std::ostringstream outStream;

    // BMSignal(std::string name, bool bIntel, int startBit, int length, int Type, unsigned char* pchData):
    std::string strName;
    pouSignal->GetName(strName);
    unsigned int unLength;
    pouSignal->GetLength(unLength);


    //1. Name
    if ( strContainerName == strName )
    {
        outStream << defTab << strName << "_signal" << "(" << "4 ," << "g_strClusterPath" << ",";
    }
    else
    {
        outStream << defTab << strName << "(" << "4 ," << "g_strClusterPath" << ",";
    }
    //2. Big or Littelendian
    if ( ouEndianness == eIntel )
    {
        outStream << " true,";
    }
    else
    {
        outStream << " false,";
    }

    //3. START BIT
    outStream << startBit <<", ";

    //4.
    outStream << unLength <<", ";

    //5.
    //5.
    LINSignalProps signalProps;
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
    outStream << "data" <<", ";

    //7.
    ICoding* pouCoding;
    pouSignal->GetEncoding(&pouCoding);
    strName = "";
    if ( pouCoding != nullptr )
    {
        pouCoding->GetName(strName);
    }
    outStream <<"\""<< strName  <<"\")";

    return outStream.str();
}


std::string LinNsCodeGenHelper::GenerateSignalDecl(ISignal* pouSignal, std::string strContainerName)
{
    std::string strName;
    std::string strSignalType;
    std::ostringstream outStream;
    pouSignal->GetName(strName);

    GetSignalType(pouSignal, strSignalType);

    if ( strContainerName == strName )
    {
        outStream << defTab << defBaseSignal << "<" << strSignalType << ">"
                  << defSpace << strName << "_signal" << defSemiColon;
    }
    else
    {
        outStream << defTab << defBaseSignal << "<" << strSignalType << ">"
                  <<defSpace << strName << defSemiColon;
    }
    return outStream.str();
}


void LinNsCodeGenHelper::GetSignalType(ISignal* pouSignal, std::string& strSignalType)
{
    unsigned int nLength = 0;
    pouSignal->GetLength(nLength);

    if ( nLength <= 8 )
    {
        strSignalType = "char";
    }
    else if ( nLength > 8 && nLength <= 16)
    {
        strSignalType = "short";
    }

    else if ( nLength > 16 && nLength <= 32)
    {
        strSignalType = "int";
    }

    else if ( nLength > 32 && nLength <= 64)
    {
        strSignalType = "__int64";
    }

    LINSignalProps ouSignalProps;
    eSignalDataType eSignalType = eUnsigned;
    pouSignal->GetProperties(ouSignalProps);

	eSignalType = ouSignalProps.m_ouDataType;


    if ( eSignalType == eSignalDataType::eUnsigned )
    {
        strSignalType = "unsigned " + strSignalType;
    }

}