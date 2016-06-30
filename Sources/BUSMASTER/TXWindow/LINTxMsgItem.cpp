/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
* \file      LINTxMsgItem.cpp
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/


#pragma once
#include "LINTxMsgItem.h"

CLINTxMsgItem::CLINTxMsgItem()
{
    MsgDetails.pchData = new unsigned char[defLIN_MSG_DLC_MAX];
    memset(MsgDetails.pchData, 0xFF, sizeof(unsigned char)*defLIN_MSG_DLC_MAX);
}
CLINTxMsgItem::~CLINTxMsgItem()
{
    if (nullptr != MsgDetails.pchData)
    {
        delete MsgDetails.pchData;
        MsgDetails.pchData = nullptr;
    }
}
std::string CLINTxMsgItem::strGetValueAtTag(xmlNodePtr pxmlNodePtr, const std::string& strTag)
{
    std::string strValue = "";
    xmlChar* pchPathMsg = (xmlChar*)strTag.c_str();
    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pxmlNodePtr, pchPathMsg);
    if (pObjectPath != nullptr)
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if (nullptr != pNodeSet)
        {
            xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            strValue = ((char*)key);
        }
    }
    return strValue;
}
void CLINTxMsgItem::vGetPhysicalValueFactor(ISignal* pSignal, double& dScale)
{
    if (nullptr != pSignal)
    {
        ICoding* pCoding = nullptr;
        if (EC_SUCCESS == pSignal->GetEncoding(&pCoding))
        {
            if (nullptr != pCoding)
            {
                CompuMethodProps ouCompuProps;
                pCoding->GetProperties(ouCompuProps);

                if (ouCompuProps.m_ouLinCompuMethods.m_ouPhysicalValueList.size() > 0)
                {
                    std::list<PhysicalValue>::iterator itrPhyValue = ouCompuProps.m_ouLinCompuMethods.m_ouPhysicalValueList.begin();

                    if (itrPhyValue != ouCompuProps.m_ouLinCompuMethods.m_ouPhysicalValueList.end())
                    {
                        dScale = itrPhyValue->m_dFactor;
                    }
                }
            }
        }
    }
}
bool CLINTxMsgItem::sortByStartBit(const SIG_DETAILS& lhs, const SIG_DETAILS& rhs)
{
    return lhs.Signal.second.m_nStartBit < rhs.Signal.second.m_nStartBit;
}
int CLINTxMsgItem::GetMsgName(IBMNetWorkGetService* pouIBMNetwork, bool bIshex, std::string& strMsgName)
{
    int hResult = S_FALSE;
    IFrame* pIFrame = nullptr;
    if (nullptr == pouIBMNetwork)
    {
        return hResult;
    }
    pouIBMNetwork->GetFrame(LIN, MsgDetails.nChannel - 1, MsgDetails.nMsgId, nullptr, &pIFrame);

    if (nullptr != pIFrame)
    {
        hResult = S_OK;
        pIFrame->GetName(strMsgName);
    }
    else
    {
        hResult = S_OK;
        char chValue[1024];
        if (bIshex == false)
        {
            sprintf_s(chValue, 1024, "%d", MsgDetails.nMsgId);
        }
        else
        {
            sprintf_s(chValue, 1024, "0x%X", MsgDetails.nMsgId);
        }
        strMsgName = chValue;
    }
    return hResult;
}
int CLINTxMsgItem::SetMsgConfig(xmlNodePtr pxmlNodePtr)
{
    int hResult = S_FALSE;

    if (nullptr == pxmlNodePtr)
    {
        return hResult;
    }

    //Name
    //MsgDetails.strMsgName = strGetValueAtTag(pxmlNodePtr, DEF_NAME);
    //Channel
    MsgDetails.nChannel = atoi(strGetValueAtTag(pxmlNodePtr, DEF_CHANNEL).c_str());
    //MsgId
    MsgDetails.nMsgId = atoi(strGetValueAtTag(pxmlNodePtr, DEF_MESSAGE_ID).c_str());
    //Msg Type
    MsgDetails.nMsgType = atoi(strGetValueAtTag(pxmlNodePtr, DEF_MESSAGE_TYPE).c_str());
    //Msg DLC
    MsgDetails.nDLC = atoi(strGetValueAtTag(pxmlNodePtr, DEF_DLC).c_str());
    //Msg Databytes
    std::string strDataBytes = strGetValueAtTag(pxmlNodePtr, DEF_DATABYTES).c_str();
    char* pch;
    pch = strtok((char*)strDataBytes.c_str(), " ,.-");
    int i = 0;
    while (pch != nullptr)
    {
        MsgDetails.pchData[i] = atoi(pch);
        pch = strtok(nullptr, " ,.-");
        i++;
    }
    //Repetition
    TxDetails.nActualTimer = atoi(strGetValueAtTag(pxmlNodePtr, DEF_REPETION).c_str());
    TxDetails.nCurrentTimerVal = TxDetails.nActualTimer;

    //TimerEnabled
    TxDetails.m_bTimerEnabled = true;
    if (0 == std::strcmp(strGetValueAtTag(pxmlNodePtr, DEF_REPETITION_ENABLED).c_str(), "FALSE"))
    {
        TxDetails.m_bTimerEnabled = false;
    }

    //Key Value
    TxDetails.m_chKeyVal = strGetValueAtTag(pxmlNodePtr, DEF_KEY_VAL)[0];

    //Key Value Enabled
    TxDetails.bKeyEnabled = true;
    if (0 == std::strcmp(strGetValueAtTag(pxmlNodePtr, DEF_KEY_ENABLED).c_str(), "FALSE"))
    {
        TxDetails.bKeyEnabled = false;
    }

    return hResult;
}
int CLINTxMsgItem::GetMsgConfig(xmlNodePtr pxmlNodePtr)
{
    int hResult = S_FALSE;
    if (nullptr != pxmlNodePtr)
    {
        hResult = S_OK;
        char pchData[1024];
        //Name
        //xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_NAME, BAD_CAST MsgDetails.strMsgName.c_str());

        //Channel
        sprintf(pchData, "%d", MsgDetails.nChannel);
        xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_CHANNEL, BAD_CAST pchData);

        //Message Id
        sprintf(pchData, "%d", MsgDetails.nMsgId);
        xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_MSG_ID, BAD_CAST pchData);

        //Checksum Type -- Currently not used but reserved for future.
        sprintf(pchData, "%d", 0);
        xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_LIN_MESSAGE_CHECKSUM, BAD_CAST pchData);

        //Message Type
        sprintf(pchData, "%d", MsgDetails.nMsgType);
        xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_MESSAGE_TYPE, BAD_CAST pchData);

        //DLC
        sprintf(pchData, "%d", MsgDetails.nDLC);
        xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_DLC, BAD_CAST pchData);

        //Data Bytes
        std::string strDatabytes;
        sprintf(pchData, "%d", MsgDetails.pchData[0]);
        strDatabytes = pchData;
        for (int i = 1; i < MsgDetails.nDLC; i++)
        {
            sprintf(pchData, ",%d", MsgDetails.pchData[i]);
            strDatabytes.append(pchData);
        }

        xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_DATABYTES, BAD_CAST strDatabytes.c_str());

        //Repetition
        sprintf(pchData, "%d", TxDetails.nActualTimer);
        xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_REPETION, BAD_CAST pchData);

        //Repetition Enabled
        if (true == TxDetails.m_bTimerEnabled)
        {
            xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_REPETITION_ENABLED, BAD_CAST "TRUE");
        }
        else
        {
            xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_REPETITION_ENABLED, BAD_CAST "FALSE");
        }

        //Key
        sprintf(pchData, "%c", TxDetails.m_chKeyVal);
        xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_KEY_VAL, BAD_CAST pchData);

        //Key Enabled
        if (true == TxDetails.bKeyEnabled)
        {
            xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_KEY_ENABLED, BAD_CAST "TRUE");
        }
        else
        {
            xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_KEY_ENABLED, BAD_CAST "FALSE");
        }


    }

    return hResult;
}
int CLINTxMsgItem::SetOldMsgConfig(xmlNodePtr pxmlNodePtr)
{
    //This function is Not Applicable for LIN.
    SetMsgConfig(pxmlNodePtr);
    return S_OK;
}
int CLINTxMsgItem::GetSignalList(IBMNetWorkGetService* pouIBMNetwork, bool bIsHex, std::list<SIG_DETAILS>& lstSigDetails)
{
    int hResult = S_FALSE;
    std::map<ISignal*, SignalInstanse> mapSignal;
    std::list<InterpreteSignals> lstSignalInterprete;
    if (MsgDetails.nChannel < 0 || MsgDetails.nChannel > CHANNEL_ALLOWED)
    {
        return hResult;
    }

    if (nullptr == pouIBMNetwork)
    {
        return hResult;
    }

    IFrame* ouFrameStruct = nullptr;
    CString omStrMsgName = "";
    pouIBMNetwork->GetFrame(LIN, MsgDetails.nChannel - 1, MsgDetails.nMsgId, nullptr, &ouFrameStruct);
    if (nullptr != ouFrameStruct)
    {
        hResult = S_OK;
        ouFrameStruct->GetSignalList(mapSignal);
        ouFrameStruct->InterpretSignals(MsgDetails.pchData, MsgDetails.nDLC, lstSignalInterprete, bIsHex);
    }

    auto itrSigItp = lstSignalInterprete.begin();
    auto itrSignal = mapSignal.begin();
    SIG_DETAILS ouSigDetails;
    double dScale = 0;
    for (; itrSigItp != lstSignalInterprete.end() && itrSignal != mapSignal.end(); ++itrSigItp, ++itrSignal)
    {
        ouSigDetails.SignalInterprete = *itrSigItp;
        ouSigDetails.Signal = *itrSignal;
        vGetPhysicalValueFactor(itrSignal->first, dScale);
        if (dScale > 0)
        {
            ouSigDetails.dPhyFactor = dScale;
        }
        else
        {
            ouSigDetails.dPhyFactor = 1;
        }
        lstSigDetails.push_back(ouSigDetails);
    }
    //Sort by start bit.
    lstSigDetails.sort(CLINTxMsgItem::sortByStartBit);
}

int CLINTxMsgItem::GetSignal(IBMNetWorkGetService* pouIBMNetwork, bool bIsHex, std::string strSigName, SIG_DETAILS& ouSignalDetails)
{
    int hResult = S_FALSE;
    std::list<SIG_DETAILS> lstSigDetails;
    GetSignalList(pouIBMNetwork, bIsHex, lstSigDetails);
    std::string strName = "";
for (auto itr : lstSigDetails)
    {
        itr.Signal.first->GetName(strName);
        if (strName == strSigName)
        {
            hResult = S_OK;
            ouSignalDetails = itr;
        }
    }
    return hResult;
}