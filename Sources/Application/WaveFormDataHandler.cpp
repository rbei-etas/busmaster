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
 * \file      WaveFormDataHandler.cpp
 * \brief     Implementation file for CWaveformDataHandler class
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CWaveformDataHandler class
 */

#include "stdafx.h"
#include "WaveFormDataHandler.h"
#include "HashDefines.h"
#include "include/Utils_macro.h"
#include "Utility/XMLUtils.h"
const BYTE WAVEFORM_DATA_HANDLER_VERSION = 0x1;

CWaveFormDataHandler::CWaveFormDataHandler(void)
{
    //Initialize the Waveform with default values.
    m_sDefaultWaveInfo.m_eSignalWaveType = eWave_SINE;
    m_sDefaultWaveInfo.m_fAmplitude   = 10;
    m_sDefaultWaveInfo.m_fFrequency   = 1;
    m_shSamplingTP = 125;
    m_bSignalDefinerAutoCorrect = true;
}

CWaveFormDataHandler::~CWaveFormDataHandler(void)
{
}

SSigGeneration* CWaveFormDataHandler::psGetMsgEntryFromList(UINT MsgID,
        CSigGenerationInfoList& omCurrList)
{
    SSigGeneration* Result = nullptr;

    POSITION Pos = omCurrList.GetHeadPosition();

    while ((nullptr != Pos) && (nullptr == Result))
    {
        SSigGeneration& CurrEntry = omCurrList.GetNext(Pos);
        if (CurrEntry.m_nMsgID == MsgID)
        {
            Result = &CurrEntry;
        }
    }
    return Result;
}

sWaveformInfo* CWaveFormDataHandler::psGetSignalEntryFromMsgEntry(
    CString omSignalName, SSigGeneration& ouMsgEntry)
{
    sWaveformInfo* Result = FALSE;

    POSITION Pos = ouMsgEntry.m_omSigWaveMapList.GetHeadPosition();
    while ((nullptr != Pos) && (nullptr == Result))
    {
        sSigWaveMap& CurrWaveEntry = ouMsgEntry.m_omSigWaveMapList.GetNext(Pos);
        if (CurrWaveEntry.m_omSigName == omSignalName)
        {
            Result = &(CurrWaveEntry.sWaveInfo);
        }
    }
    return Result;
}

sWaveformInfo* CWaveFormDataHandler::psGetSignalEntryFromList(UINT MsgID,
        CString omSignalName, CSigGenerationInfoList& omCurrList)
{
    sWaveformInfo* Result = nullptr;

    SSigGeneration* psMsgEntry = psGetMsgEntryFromList(MsgID, omCurrList);

    if (nullptr != psMsgEntry)
    {
        POSITION Pos = psMsgEntry->m_omSigWaveMapList.GetHeadPosition();

        while ((nullptr != Pos) && (nullptr == Result))
        {
            sSigWaveMap& CurrEntry = psMsgEntry->m_omSigWaveMapList.GetNext(Pos);
            if (CurrEntry.m_omSigName == omSignalName)
            {
                Result = &(CurrEntry.sWaveInfo);
            }
        }
    }

    return Result;
}

//Service funtion definitions.

void CWaveFormDataHandler::vGetDefaultSignalWaveValues(sWaveformInfo& stDefaultWaveInfo) const
{
    stDefaultWaveInfo = m_sDefaultWaveInfo;
}

void CWaveFormDataHandler::vSetDefaultSignalWaveValues(const sWaveformInfo& stDefaultWaveInfo)
{
    m_sDefaultWaveInfo = stDefaultWaveInfo;
}

float CWaveFormDataHandler::fGetMsgIDDefaultValue(UINT unMsgID)
{
    float Result = (float) 0.0;

    SSigGeneration* psMsgEntry = psGetMsgEntryFromList(unMsgID,
                                 m_lstTempSignalWaveformInfo);
    if (nullptr != psMsgEntry)
    {
        Result = psMsgEntry->m_fDefaultAmplitude;
    }

    return Result;
}

void CWaveFormDataHandler::vSetMsgIDDefaultValue(UINT unMsgID, float fDefaultValue)
{
    SSigGeneration* psMsgEntry = psGetMsgEntryFromList(unMsgID,
                                 m_lstTempSignalWaveformInfo);
    if (nullptr != psMsgEntry)
    {
        psMsgEntry->m_fDefaultAmplitude = fDefaultValue;
    }
}

BOOL CWaveFormDataHandler::bGetSignalWavePatternDetails(UINT unMsgID,
        CString omSignalName, sWaveformInfo& sSigWaveInfo)
{
    BOOL Result = FALSE;

    sWaveformInfo* psWaveformEntry = psGetSignalEntryFromList(unMsgID,
                                     omSignalName, m_lstTempSignalWaveformInfo);
    if (nullptr != psWaveformEntry)
    {
        sSigWaveInfo = *psWaveformEntry;
        Result = TRUE;
    }
    return Result;
}

void CWaveFormDataHandler::vSetSignalWavePatternDetails(UINT unMsgID,
        CString omSignalName, const sWaveformInfo& sSigWaveInfo)
{
    sWaveformInfo* psWaveformEntry = psGetSignalEntryFromList(unMsgID,
                                     omSignalName, m_lstTempSignalWaveformInfo);
    if (nullptr != psWaveformEntry)
    {
        *psWaveformEntry = sSigWaveInfo;
    }
}

BOOL CWaveFormDataHandler::bIsSignalInMsgIDDefined(UINT unMsgID, CString omSignalName)
{
    return (psGetSignalEntryFromList(unMsgID, omSignalName,
                                     m_lstTempSignalWaveformInfo) != nullptr);
}

void CWaveFormDataHandler::vGetDefinedSignalsInMsgID(UINT unMsgID,
        CStringArray& omDefinedSignals)
{
    SSigGeneration* Result = psGetMsgEntryFromList(
                                 unMsgID, m_lstTempSignalWaveformInfo);

    if (nullptr != Result)
    {
        omDefinedSignals.RemoveAll();
        POSITION Pos = Result->m_omSigWaveMapList.GetHeadPosition();
        while (nullptr != Pos)
        {
            sSigWaveMap& SigEntry = Result->m_omSigWaveMapList.GetNext(Pos);
            omDefinedSignals.Add(SigEntry.m_omSigName);
        }
    }
}

sSigWaveMap* CWaveFormDataHandler::psGetSignalEntry(
    CString omSignalName, CSigWaveMapList& omSigWaveList)
{
    sSigWaveMap* Result = nullptr;

    POSITION Pos = omSigWaveList.GetHeadPosition();

    while ((nullptr != Pos) && (nullptr == Result))
    {
        sSigWaveMap& CurrEntry = omSigWaveList.GetNext(Pos);
        if (CurrEntry.m_omSigName == omSignalName)
        {
            Result = &CurrEntry;
        }
    }
    return Result;
}

// Set (add / modify) a signal wave entry, given the signal wave list
BOOL CWaveFormDataHandler::bSetSignalWaveEntry(sSigWaveMap ouSignalEntry,
        CSigWaveMapList& omSigWaveList)
{
    sSigWaveMap* psCurrEntry = psGetSignalEntry(ouSignalEntry.m_omSigName, omSigWaveList);

    if (nullptr == psCurrEntry)
    {
        omSigWaveList.AddTail(ouSignalEntry);
    }
    else
    {
        *psCurrEntry = ouSignalEntry;
    }
    return TRUE;
}

BOOL CWaveFormDataHandler::bAddSignalToDefinedWavesList(UINT unMsgID,
        CString omSignalName, sWaveformInfo& sSigWaveInfo)
{
    SSigGeneration* psMsgEntry = psGetMsgEntryFromList(
                                     unMsgID, m_lstTempSignalWaveformInfo);

    sSigWaveMap ouSignalWaveEntry;
    ouSignalWaveEntry.m_omSigName = omSignalName;
    ouSignalWaveEntry.sWaveInfo = sSigWaveInfo;
    ouSignalWaveEntry.sWaveInfo.m_fGranularity =
        fCalculateGranularity(sSigWaveInfo, m_shSamplingTP);

    if (nullptr == psMsgEntry) // Msg entry doesn't exist. So add the same
    {
        SSigGeneration ouNewMsgEntry;
        ouNewMsgEntry.m_nMsgID = unMsgID;
        ouNewMsgEntry.m_omSigWaveMapList.AddTail(ouSignalWaveEntry);
        m_lstTempSignalWaveformInfo.AddTail(ouNewMsgEntry);
    }
    else // Message entry exists.
    {
        bSetSignalWaveEntry(ouSignalWaveEntry, psMsgEntry->m_omSigWaveMapList);
    }
    return TRUE;
}

BOOL CWaveFormDataHandler::bRemoveSignalFromDefinedWavesList(
    UINT unMsgID, CString omSignalName)
{
    BOOL Result = FALSE;

    SSigGeneration ouCurrMsgEntry; // Use a message entry object
    ouCurrMsgEntry.m_nMsgID = unMsgID;

    sSigWaveMap ouCurrSigEntry; // Use a signal entry object
    ouCurrSigEntry.m_omSigName = omSignalName;

    POSITION PosMsg = m_lstTempSignalWaveformInfo.Find(ouCurrMsgEntry);
    if (nullptr != PosMsg)
    {
        SSigGeneration& ouCurrMsgEntryAlias = m_lstTempSignalWaveformInfo.GetAt(PosMsg);

        POSITION PosSig = ouCurrMsgEntryAlias.m_omSigWaveMapList.Find(ouCurrSigEntry);
        if (nullptr!= PosSig)
        {
            ouCurrMsgEntryAlias.m_omSigWaveMapList.RemoveAt(PosSig);
            if (ouCurrMsgEntryAlias.m_omSigWaveMapList.GetCount() == 0)// If the
            {
                // signal list is empty, there is no point to keep
                m_lstTempSignalWaveformInfo.RemoveAt(PosMsg);// the message entry
            }
            Result = TRUE;
        }
    }
    return Result;
}

void CWaveFormDataHandler::vCommit(void)
{
    m_lstSignalWaveformInfo.RemoveAll();
    m_lstSignalWaveformInfo.AddTail(&m_lstTempSignalWaveformInfo);
}

void CWaveFormDataHandler::vRollback(void)
{
    m_lstTempSignalWaveformInfo.RemoveAll();
    m_lstTempSignalWaveformInfo.AddTail(&m_lstSignalWaveformInfo);
}

void CWaveFormDataHandler::GetSigGenerationInfoList(CSigGenerationInfoList& Target)
{
    Target.RemoveAll();
    Target.AddTail(&m_lstSignalWaveformInfo);
}

UINT CWaveFormDataHandler::nGetNumberOfDefinedSignals(void)
{
    UINT Result = 0;

    POSITION PosMsg = m_lstTempSignalWaveformInfo.GetHeadPosition();

    while (nullptr != PosMsg)
    {
        SSigGeneration& CurrMsgEntry = m_lstTempSignalWaveformInfo.GetNext(PosMsg);
        Result += CurrMsgEntry.m_omSigWaveMapList.GetCount();
    }

    return Result;
}

void CWaveFormDataHandler::vGetAllDefinedSignalsNames(CStringArray& omSignalNames)
{
    omSignalNames.RemoveAll();  // First clear the destination buffer.

    POSITION PosMsg = m_lstTempSignalWaveformInfo.GetHeadPosition();
    while (nullptr != PosMsg)
    {
        SSigGeneration& ouCurrMsg = m_lstTempSignalWaveformInfo.GetNext(PosMsg);

        POSITION PosSig = ouCurrMsg.m_omSigWaveMapList.GetHeadPosition();
        while (nullptr != PosSig)
        {
            sSigWaveMap& ouCurrSig = ouCurrMsg.m_omSigWaveMapList.GetNext(PosSig);

            CString omWaveformDetails; // Other information accompanies the signal

            if(!bIsSignalInMsgFoundInDB(ouCurrMsg.m_nMsgID, ouCurrSig.m_omSigName))
            {
                continue;
            }

            omWaveformDetails.Format("%X ->%s (%s)", ouCurrMsg.m_nMsgID,
                                     ouCurrSig.m_omSigName,
                                     sWaveformInfo::omGetWaveformName(ouCurrSig.sWaveInfo.m_eSignalWaveType));

            omSignalNames.Add(omWaveformDetails); // [out] parameter is updated
        }
    }
}

bool CWaveFormDataHandler::bIsSignalInMsgFoundInDB(UINT& nMsgID, CString& strSignalName)
{
    if (m_podMsgEntyList != nullptr)
    {
        SMAINENTRY sMainEntry;
        sMainEntry.m_unMainEntryID = nMsgID;
        POSITION pos = m_podMsgEntyList->Find(sMainEntry);
        if ( pos != nullptr)
        {
            sMainEntry = m_podMsgEntyList->GetAt(pos);
            POSITION SubPos = sMainEntry.m_odUnSelEntryList.GetHeadPosition();
            while (SubPos != nullptr)
            {
                SSUBENTRY& sSubEntry = sMainEntry.m_odUnSelEntryList.GetNext(SubPos);

                if( strSignalName.Compare(sSubEntry.m_omSubEntryName) == 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

short CWaveFormDataHandler::shGetSamplingTimePeriod()
{
    return m_shSamplingTP;
}

float CWaveFormDataHandler::fCalculateGranularity(sWaveformInfo& sCurrWaveInfo,
        short shSamplingTP)
{
    /* Granularity means basic unit angle in radian for every iteration which
    is based on sampling time period. This can be formulated as:
    (2 * PI * Freq * Sample time period) / 1000 */
    float Result = (2 * PI * sCurrWaveInfo.m_fFrequency * shSamplingTP) / 1000;

    return Result;
}

void CWaveFormDataHandler::vSetSamplingTimePeriod(short shSamplingTimePeriod)
{
    m_shSamplingTP = shSamplingTimePeriod;

    // Recalculate granularity of each waveform entry
    POSITION Pos = m_lstTempSignalWaveformInfo.GetHeadPosition();
    while (nullptr != Pos)
    {
        SSigGeneration& CurrMsgEntry = m_lstTempSignalWaveformInfo.GetNext(Pos);
        POSITION SigPos = CurrMsgEntry.m_omSigWaveMapList.GetHeadPosition();
        while (nullptr != SigPos)
        {
            sSigWaveMap& WaveEntry = CurrMsgEntry.m_omSigWaveMapList.GetNext(SigPos);
            WaveEntry.sWaveInfo.m_fGranularity =
                fCalculateGranularity(WaveEntry.sWaveInfo, m_shSamplingTP);
        }
    }
}

HRESULT CWaveFormDataHandler::GetConfigData(BYTE** ppvDataStream, UINT& unLength)
{
    UINT nSize = 0;
    nSize += sizeof(BYTE);  // 1 Byte for Version
    nSize += sizeof(UINT);  // Bytes for Buffer Size

    nSize += nGetWaveInfoListSize();

    BYTE* pByteTrgt = new BYTE[nSize];
    *ppvDataStream = pByteTrgt;

    BYTE byVer = WAVEFORM_DATA_HANDLER_VERSION;
    COPY_DATA(pByteTrgt, &byVer , sizeof(BYTE)); //Setting Version.
    COPY_DATA(pByteTrgt, &nSize, sizeof(UINT)); //Setting Buffer Size.

    pByteTrgt = pbyGetListConfigData(pByteTrgt, nSize);
    unLength = nSize;

    return S_OK;
}

HRESULT CWaveFormDataHandler::GetConfigData(xmlNodePtr pNodePtr)
{
    pbyGetListConfigData(pNodePtr);
    return true;
}
HRESULT CWaveFormDataHandler::SetConfigData(xmlDocPtr pDoc)
{
    //Initialise
    m_lstSignalWaveformInfo.RemoveAll();
    m_lstTempSignalWaveformInfo.RemoveAll();
    INT nRetVal = S_OK;
    if( nullptr != pDoc )
    {
        xmlXPathObjectPtr pOjectPath = nullptr;
        xmlNodePtr pNodePtr = nullptr;
        xmlChar* pXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Wave_Form_Genarator/WAVE_FORM";

        pOjectPath = xmlUtils::pGetNodes(pDoc, pXpath);
        if(pOjectPath != nullptr)
        {
            xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
            UINT nMsgCount = pNodeSet->nodeNr;
            if(pNodeSet != nullptr)
            {
                for(UINT i=0; i<nMsgCount; i++)
                {
                    BOOL bMsgIdFound = FALSE;
                    SSigGeneration objSigGeneration;
                    xmlNodePtr pNode = pNodeSet->nodeTab[i]->children;
                    while (pNode != nullptr)
                    {
                        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Message_ID")))    //Reading Message ID.
                        {
                            xmlChar* key = xmlNodeListGetString(pDoc, pNode->xmlChildrenNode, 1);
                            if(nullptr != key)
                            {
                                objSigGeneration.m_nMsgID = atoi((char*)key);
                                xmlFree(key);
                                bMsgIdFound = TRUE;
                            }
                        }
                        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Default_Amplitude")))     //Reading Default Amplitude.
                        {
                            xmlChar* key = xmlNodeListGetString(pDoc, pNode->xmlChildrenNode, 1);
                            if(nullptr != key)
                            {
                                objSigGeneration.m_fDefaultAmplitude = atof((char*)key);
                                xmlFree(key);
                            }
                        }
                        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"SIGNALS")))   //Signals.
                        {
                            sSigWaveMap objSigWaveMap;
                            ParseSignalNode(pNode, objSigWaveMap);
                            objSigGeneration.m_omSigWaveMapList.AddTail(objSigWaveMap);
                        }
                        //Reading Signal Count
                        pNode = pNode->next;
                    }
                    if(bMsgIdFound == TRUE)
                    {
                        m_lstSignalWaveformInfo.AddTail(objSigGeneration);
                        m_lstTempSignalWaveformInfo.AddTail(objSigGeneration);
                    }
                }
            }
            //Storing Sampling Time period.
            //COPY_DATA_2(&m_shSamplingTP, pByteTrgt, sizeof(m_shSamplingTP));
            xmlXPathFreeObject(pOjectPath);
        }
        else
        {
            nRetVal = S_FALSE;
        }
        m_shSamplingTP = 125;
        xmlChar* pXpath2 = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Wave_Form_Genarator/Default_Sampling_Period";

        pOjectPath = xmlUtils::pGetNodes(pDoc, pXpath2);
        if(pOjectPath != nullptr)
        {
            xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
            if(pNodeSet != nullptr)
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];
                if(pNode != nullptr)
                {
                    xmlChar* key = xmlNodeListGetString(pDoc, pNode->xmlChildrenNode, 1);
                    if(nullptr != key)
                    {
                        m_shSamplingTP = atoi((char*)key);
                        xmlFree((char*)key);
                    }
                }
            }
            xmlXPathFreeObject(pOjectPath);
        }
        xmlChar* pXpath3 = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Wave_Form_Genarator/SignalDefiner_AutoCorrect";
        pOjectPath = xmlUtils::pGetNodes(pDoc, pXpath3);
        if(pOjectPath != nullptr)
        {
            xmlNodeSetPtr pNodeSet = pOjectPath->nodesetval;
            if(pNodeSet != nullptr)
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];
                if(pNode != nullptr)
                {
                    xmlChar* key = xmlNodeListGetString(pDoc, pNode->xmlChildrenNode, 1);
                    if(nullptr != key)
                    {
                        m_bSignalDefinerAutoCorrect = (atoi((char*)key) != 0);
                        xmlFree((char*)key);
                    }
                }
            }
            xmlXPathFreeObject(pOjectPath);
        }
    }
    else if( pDoc == nullptr || nRetVal == FALSE )   //Assign Default Values
    {
        m_sDefaultWaveInfo.m_eSignalWaveType = eWave_SINE;
        m_sDefaultWaveInfo.m_fAmplitude   = 10;
        m_sDefaultWaveInfo.m_fFrequency   = 1;
        m_shSamplingTP = 125;
    }
    return S_OK;
}
int CWaveFormDataHandler::ParseSignalNode(xmlNodePtr pNode, sSigWaveMap& sSignalWave)
{
    pNode = pNode->children;
    while (pNode != nullptr)
    {
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Name")))  //Reading Message ID.
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                sSignalWave.m_omSigName = (char*)key;
                xmlFree(key);
            }
        }
        else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Wave_Type")))    //Reading Message ID.
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                sSignalWave.sWaveInfo.m_eSignalWaveType = xmlUtils::eGetWaveType((char*)key);
                xmlFree(key);
            }
        }
        else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Amplitude")))    //Reading Message ID.
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                sSignalWave.sWaveInfo.m_fAmplitude = atoi((char*)key);
                xmlFree(key);
            }
        }
        else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Frequency")))    //Reading Message ID.
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                sSignalWave.sWaveInfo.m_fFrequency = atoi((char*)key);
                xmlFree(key);
            }
        }
        else if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Granularity")))  //Reading Message ID.
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                sSignalWave.sWaveInfo.m_fGranularity = atof((char*)key);
                xmlFree(key);
            }
        }
        pNode = pNode->next;
    }
    return S_OK;
}
HRESULT CWaveFormDataHandler::SetConfigData(BYTE* pvDataStream)
{
    m_lstSignalWaveformInfo.RemoveAll();
    m_lstTempSignalWaveformInfo.RemoveAll();

    BYTE* pByteTrgt = pvDataStream;

    if (pByteTrgt != nullptr)
    {
        BYTE byVer = 0;
        COPY_DATA_2(&byVer, pByteTrgt, sizeof(BYTE));
        if (byVer == WAVEFORM_DATA_HANDLER_VERSION)
        {
            UINT nSize = 0;
            COPY_DATA_2(&nSize, pByteTrgt, sizeof(UINT));

            //Reading Messages Count.
            UINT nMsgCount = 0;
            COPY_DATA_2(&nMsgCount, pByteTrgt, sizeof(UINT));

            for(UINT i=0; i<nMsgCount; i++)
            {
                SSigGeneration objSigGeneration;
                //Reading Message ID.
                COPY_DATA_2(&objSigGeneration.m_nMsgID, pByteTrgt, sizeof(UINT));
                //Reading Default Amplitude.
                COPY_DATA_2(&objSigGeneration.m_fDefaultAmplitude, pByteTrgt, sizeof(float));

                //Reading Signal Count.
                UINT nSigCount = 0;
                COPY_DATA_2(&nSigCount, pByteTrgt, sizeof(UINT));

                for(UINT j=0; j<nSigCount; j++)
                {
                    sSigWaveMap objSigWaveMap;
                    objSigWaveMap.m_omSigName = "";

                    //Reading Signal Name Size.
                    BYTE bytSignalSize = 0;
                    COPY_DATA_2(&bytSignalSize, pByteTrgt, sizeof(BYTE));

                    //Reading Signal Name
                    for(int k=0; k<bytSignalSize; k++)
                    {
                        BYTE bytTemp = 0;
                        CString strTemp;
                        COPY_DATA_2(&bytTemp, pByteTrgt, sizeof(BYTE));
                        strTemp.Format("%c", bytTemp);
                        objSigWaveMap.m_omSigName+=strTemp;
                    }
                    //Reading sWaveformInfo structure.
                    COPY_DATA_2(&objSigWaveMap.sWaveInfo.m_eSignalWaveType, pByteTrgt,
                                sizeof(objSigWaveMap.sWaveInfo.m_eSignalWaveType));

                    COPY_DATA_2(&objSigWaveMap.sWaveInfo.m_fAmplitude, pByteTrgt,
                                sizeof(objSigWaveMap.sWaveInfo.m_fAmplitude));

                    COPY_DATA_2(&objSigWaveMap.sWaveInfo.m_fFrequency, pByteTrgt,
                                sizeof(objSigWaveMap.sWaveInfo.m_fFrequency));

                    COPY_DATA_2(&objSigWaveMap.sWaveInfo.m_fGranularity, pByteTrgt,
                                sizeof(objSigWaveMap.sWaveInfo.m_fGranularity));

                    //Adding sSigWaveMap object to SSigGeneration
                    objSigGeneration.m_omSigWaveMapList.AddTail(objSigWaveMap);
                }
                m_lstSignalWaveformInfo.AddTail(objSigGeneration);
                m_lstTempSignalWaveformInfo.AddTail(objSigGeneration);
            }
            //Storing Sampling Time period.
            COPY_DATA_2(&m_shSamplingTP, pByteTrgt, sizeof(m_shSamplingTP));
        }
    }
    else    //Assign Default Values
    {
        m_sDefaultWaveInfo.m_eSignalWaveType = eWave_SINE;
        m_sDefaultWaveInfo.m_fAmplitude   = 10;
        m_sDefaultWaveInfo.m_fFrequency   = 1;
        m_shSamplingTP = 125;
    }
    return S_OK;
}

BYTE* CWaveFormDataHandler::pbyGetListConfigData(BYTE* pbyTrgtStream, const UINT /*nSize*/)
{
    BYTE* pByteTrgt = pbyTrgtStream;
    POSITION pos = m_lstSignalWaveformInfo.GetHeadPosition();

    //Storing Messages Count.
    UINT nMsgCount = m_lstSignalWaveformInfo.GetCount();
    COPY_DATA(pByteTrgt, &(nMsgCount), sizeof(UINT));

    while(pos)
    {
        SSigGeneration& objSigGen = m_lstSignalWaveformInfo.GetNext(pos);

        //Storing Message ID.
        COPY_DATA(pByteTrgt, &(objSigGen.m_nMsgID), sizeof(objSigGen.m_nMsgID));
        //Storing Default Amplitude.
        COPY_DATA(pByteTrgt, &(objSigGen.m_fDefaultAmplitude), sizeof(objSigGen.m_fDefaultAmplitude));

        //Storing Signal Count.
        UINT nSigCount = objSigGen.m_omSigWaveMapList.GetCount();
        COPY_DATA(pByteTrgt, &(nSigCount), sizeof(UINT));

        POSITION posSig = objSigGen.m_omSigWaveMapList.GetHeadPosition();
        while(posSig)
        {
            sSigWaveMap& objSigMap = objSigGen.m_omSigWaveMapList.GetNext(posSig);

            //Storing Signal Name Size.
            BYTE bytSignalSize = (BYTE)objSigMap.m_omSigName.GetLength();
            COPY_DATA(pByteTrgt, &(bytSignalSize), sizeof(bytSignalSize));

            //Storing Signal Name String.
            for(int i = 0 ; i<bytSignalSize; i++)
            {
                BYTE bytTemp = objSigMap.m_omSigName.GetAt(i);
                COPY_DATA(pByteTrgt, &(bytTemp), sizeof(BYTE));
            }

            //Storing sWaveformInfo structure.
            COPY_DATA(pByteTrgt, &(objSigMap.sWaveInfo.m_eSignalWaveType),
                      sizeof(objSigMap.sWaveInfo.m_eSignalWaveType));

            COPY_DATA(pByteTrgt, &(objSigMap.sWaveInfo.m_fAmplitude),
                      sizeof(objSigMap.sWaveInfo.m_fAmplitude));

            COPY_DATA(pByteTrgt, &(objSigMap.sWaveInfo.m_fFrequency),
                      sizeof(objSigMap.sWaveInfo.m_fFrequency));

            COPY_DATA(pByteTrgt, &(objSigMap.sWaveInfo.m_fGranularity),
                      sizeof(objSigMap.sWaveInfo.m_fGranularity));
        }
    }
    //Storing Sampling Time period.
    COPY_DATA(pByteTrgt, &m_shSamplingTP, sizeof(m_shSamplingTP));
    return pByteTrgt;
}
BOOL CWaveFormDataHandler::pbyGetListConfigData(xmlNodePtr pxmlNodePtr)
{
    const char* omcVarChar ;
    POSITION pos = m_lstSignalWaveformInfo.GetHeadPosition();
    while(pos)
    {
        //<WAVE_FORM>
        xmlNodePtr pWaveForm = xmlNewNode(nullptr, BAD_CAST DEF_WAVE_FORM);
        xmlAddChild(pxmlNodePtr, pWaveForm);

        SSigGeneration& objSigGen = m_lstSignalWaveformInfo.GetNext(pos);

        //<Message_ID />
        CString csMsdID;
        csMsdID.Format("%d", objSigGen.m_nMsgID);
        omcVarChar = csMsdID;
        xmlNodePtr pMsgID= xmlNewChild(pWaveForm, nullptr, BAD_CAST DEF_MSG_ID, BAD_CAST omcVarChar);
        xmlAddChild(pWaveForm, pMsgID);

        //<Default_Amplitude />
        CString csDefaultAmp;
        csDefaultAmp.Format("%f", objSigGen.m_fDefaultAmplitude);
        omcVarChar = csDefaultAmp;
        xmlNodePtr pDefAmp= xmlNewChild(pWaveForm, nullptr, BAD_CAST DEF_DEFAULT_AMPLITUDE, BAD_CAST omcVarChar);
        xmlAddChild(pWaveForm, pDefAmp);

        POSITION posSig = objSigGen.m_omSigWaveMapList.GetHeadPosition();
        while(posSig)
        {
            sSigWaveMap& objSigMap = objSigGen.m_omSigWaveMapList.GetNext(posSig);

            //<SIGNALS>
            xmlNodePtr pSignals = xmlNewNode(nullptr, BAD_CAST DEF_SIGNALS);
            xmlAddChild(pWaveForm, pSignals);

            //<Name>
            omcVarChar = objSigMap.m_omSigName;
            xmlNodePtr pName = xmlNewChild(pSignals, nullptr, BAD_CAST DEF_NAME, BAD_CAST omcVarChar);
            xmlAddChild(pSignals, pName);

            //<Wave_Type>
            CString csWaveType;
            if(objSigMap.sWaveInfo.m_eSignalWaveType == eWave_NONE)
            {
                csWaveType = "NONE";
            }
            else if(objSigMap.sWaveInfo.m_eSignalWaveType == eWave_SINE)
            {
                csWaveType = "SINE";
            }
            else if(objSigMap.sWaveInfo.m_eSignalWaveType == eWave_TRIANGLE)
            {
                csWaveType = "TRIANGLE";
            }
            else if(objSigMap.sWaveInfo.m_eSignalWaveType == eWave_COS)
            {
                csWaveType = "COS";
            }
            else if(objSigMap.sWaveInfo.m_eSignalWaveType == eWave_SAWTOOTH)
            {
                csWaveType = "SAWTOOTH";
            }
            omcVarChar = csWaveType;
            xmlNodePtr pWaveType= xmlNewChild(pSignals, nullptr, BAD_CAST DEF_WAVE_TYPE, BAD_CAST omcVarChar);
            xmlAddChild(pSignals, pWaveType);
            //<Amplitude>9</Amplitude>
            CString csAmp;
            csAmp.Format("%f", objSigMap.sWaveInfo.m_fAmplitude);
            omcVarChar = csAmp;
            xmlNodePtr pAmp= xmlNewChild(pSignals, nullptr, BAD_CAST DEF_AMPLITUDE, BAD_CAST omcVarChar);
            xmlAddChild(pSignals, pAmp);

            //<Frequency>100</Frequency>
            CString csFreq;
            csFreq.Format("%f", objSigMap.sWaveInfo.m_fFrequency);
            omcVarChar = csFreq;
            xmlNodePtr pFreq= xmlNewChild(pSignals, nullptr, BAD_CAST DEF_FREQUENCY, BAD_CAST omcVarChar);
            xmlAddChild(pSignals, pFreq);

            //<Granularity>1</Granularity>
            CString csGranularity;
            csGranularity.Format("%f", objSigMap.sWaveInfo.m_fGranularity);
            omcVarChar = csGranularity;
            xmlNodePtr pGranularity = xmlNewChild(pSignals, nullptr, BAD_CAST DEF_GRANULARITY, BAD_CAST omcVarChar);
            xmlAddChild(pSignals, pGranularity);
        }
    }

    //<Default_Sampling_Period>100</Default_Sampling_Period>
    CString csDefSamplinPeriod;
    csDefSamplinPeriod.Format("%d", m_shSamplingTP);
    omcVarChar = csDefSamplinPeriod;
    xmlNodePtr pDefSP= xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_DEF_SAMPLING_PERIOD, BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pDefSP);

    CString csAutoCorrect;
    csAutoCorrect.Format("%d", m_bSignalDefinerAutoCorrect);
    omcVarChar = csAutoCorrect;
    xmlNodePtr pAutoCorrect= xmlNewChild(pxmlNodePtr, nullptr, BAD_CAST DEF_SIGDEF_AUTOCORRECT, BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pAutoCorrect);
    return true;
}

UINT CWaveFormDataHandler::nGetWaveInfoListSize()
{
    UINT nSize = 0;
    nSize += sizeof(UINT); //Bytes for storing Number of SSigGeneration list items.

    POSITION pos = m_lstSignalWaveformInfo.GetHeadPosition();
    while(pos)
    {
        SSigGeneration& objSigGen = m_lstSignalWaveformInfo.GetNext(pos);

        nSize += sizeof(objSigGen.m_nMsgID); //Bytes for storing Message ID.
        nSize += sizeof(objSigGen.m_fDefaultAmplitude); //Bytes for storing Default Amplitude.

        POSITION posSig = objSigGen.m_omSigWaveMapList.GetHeadPosition();

        nSize += sizeof(UINT); //Bytes for Storing Signal Count.

        while(posSig)
        {
            sSigWaveMap& objSigMap = objSigGen.m_omSigWaveMapList.GetNext(posSig);

            nSize += sizeof(BYTE); //Byte for storing Signal Name String length.
            nSize += objSigMap.m_omSigName.GetLength(); //Bytes for storing Signal Name String.

            //Bytes for storing sWaveformInfo strucurwe.
            nSize += sizeof(objSigMap.sWaveInfo.m_eSignalWaveType);
            nSize += sizeof(objSigMap.sWaveInfo.m_fAmplitude);
            nSize += sizeof(objSigMap.sWaveInfo.m_fFrequency);
            nSize += sizeof(objSigMap.sWaveInfo.m_fGranularity);
        }
    }
    nSize += sizeof(short); //Bytes for storing sampling Time Period.
    return nSize;
}

void CWaveFormDataHandler::vGetCompleteMsgList(CStringArray& arrMsgList) const
{
    arrMsgList.RemoveAll();
    arrMsgList.Copy(m_omMsgIDList);
}

void CWaveFormDataHandler::vSetCompleteMsgList(CMainEntryList* podMsgIDList)
{
    m_omMsgIDList.RemoveAll();
    if (podMsgIDList != nullptr)
    {
        m_podMsgEntyList = podMsgIDList;
        UINT unNoOfMainEntries = podMsgIDList->GetCount();

        if ( unNoOfMainEntries > 0 )
        {
            // Add every message name into the message list
            POSITION pos = podMsgIDList->GetHeadPosition();
            while (pos != nullptr)
            {
                SMAINENTRY& sMainEntry = podMsgIDList->GetNext(pos);
                CString omMainEntryName = sMainEntry.m_omMainEntryName;
                CString omMainEntryId;
                omMainEntryId.Format(defSTR_MSG_ID_IN_HEX,sMainEntry.m_unMainEntryID);
                omMainEntryName = omMainEntryId + omMainEntryName;
                m_omMsgIDList.Add(omMainEntryName);
            }
        }
    }
}

void CWaveFormDataHandler::vGetAvailableSignalsInMsgID(UINT& nMsgID,
        CStringArray& arrAvailableSignals,bool bExcludeDefinedSignals)
{
    if (m_podMsgEntyList != nullptr)
    {
        SMAINENTRY sMainEntry;
        sMainEntry.m_unMainEntryID = nMsgID;
        POSITION pos = m_podMsgEntyList->Find(sMainEntry);
        if ( pos != nullptr)
        {
            sMainEntry = m_podMsgEntyList->GetAt(pos);
            POSITION SubPos = sMainEntry.m_odUnSelEntryList.GetHeadPosition();
            while (SubPos != nullptr)
            {
                SSUBENTRY& sSubEntry = sMainEntry.m_odUnSelEntryList.GetNext(SubPos);

                //If the Signal Name for nMsgID is not defined, then only add it.
                if(bExcludeDefinedSignals )
                {
                    if(! bIsSignalInMsgIDDefined(nMsgID, sSubEntry.m_omSubEntryName))
                    {
                        arrAvailableSignals.Add(sSubEntry.m_omSubEntryName);
                    }
                }
                else
                {
                    arrAvailableSignals.Add(sSubEntry.m_omSubEntryName);
                }
            }
        }
    }
}

void CWaveFormDataHandler::vClearSignalInfoList(void)
{
    m_lstSignalWaveformInfo.RemoveAll();
    m_lstTempSignalWaveformInfo.RemoveAll();
}
#if 0
void CWaveFormDataHandler::CopyWaveInfoStructures(CSigGenerationInfoList* pSourceList,
        CSigGenerationInfoList* pDestinationList)
{
    POSITION pos = pSourceList->GetHeadPosition();
    pDestinationList->RemoveAll();
    while(pos)
    {
        SSigGeneration& objSigGen = pSourceList->GetNext(pos);
        pDestinationList->AddTail(objSigGen);
    }
}
#endif
