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
* \file      FibexClass.cpp
* \brief     Implementation of the CMainFrame class
* \authors   RBIN/EMC2 - Amarnath Shastry, Amitesh Bharti, Anish Kumar, Arunkumar Karri
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Implementation of the CMainFrame class
*/
//  : Defines the initialization routines for the DLL.
//

#include "stdafx.h"         // Standard header include file
#include "FibexClass.h"
#include "Utility/XMLUtils.h"
#include "HashDefines.h"
#define USAGE_EXPORT
#include "FibexClass_extern.h"

#include "Fibex_Defs.h"
#include "NodeUtil_Flexray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern MSXML2::IXMLDOMDocument2Ptr gpXMLDom;

static std::string sg_omActiveVersion = VERSION_NONE;
static HMODULE sg_HMODULE = nullptr;


typedef int (WINAPIV* PF_LOADFIBEXFILE)(std::string);
typedef BOOL (WINAPIV* PF_WRITRFIBEXFILE)(std::string);
typedef int (WINAPIV* PF_TRANSLATE)(eENTITY_FIBEX, std::string, BYTE*, BOOL);
typedef int (WINAPIV* PF_RESETFIBEXENTITY)(eENTITY_FIBEX, PVOID);
typedef int (WINAPIV* PF_RETREIVECONNECTORINFO)(PVOID odConnectorList, AbsConnectorInfoList& omConnectorsInECUInfo);


static PF_LOADFIBEXFILE         sg_FunLoadFibexFile = nullptr;
static PF_WRITRFIBEXFILE        sg_FunWriteFibexFile = nullptr;
static PF_TRANSLATE             sg_FunTranslate ;
static PF_RESETFIBEXENTITY      sg_FunResetFibexEntity = nullptr;
static PF_RETREIVECONNECTORINFO sg_FunRetreiveConnectorInfo = nullptr;

CPARSER_FIBEX* g_pouPARSER_FIBEX = nullptr;

/******************************* CPARSER_FIBEX ******************************************/

// To retrieve version information of the Fibex file in consideration
std::string CPARSER_FIBEX::GetVersionInfo(const std::string& omFibexFilePath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    std::string strVersion = "";
    std::string strTempXMLPath = omFibexFilePath;

    xmlDocPtr objXMLConfigFiledoc = nullptr;

    /* Open XML File */
    objXMLConfigFiledoc = xmlReadFile(strTempXMLPath.c_str(), nullptr, 0);

    xmlNodePtr pRootNode = xmlDocGetRootElement(objXMLConfigFiledoc);

    // Get the Fibex version
    if(pRootNode != nullptr)
    {
        _xmlAttr* pRootNodeAttr = pRootNode->properties;

        while (pRootNodeAttr != nullptr)
        {
            //VERSION
            if ((!xmlStrcmp(pRootNodeAttr->name, (const xmlChar*)"VERSION")))
            {
                char chVersion[1024];
                sprintf_s(chVersion, sizeof(chVersion), "%s", pRootNodeAttr->children->content);;
                strVersion = chVersion;
                break;
            }
            pRootNodeAttr = pRootNodeAttr->next;
        }

    }

    /* Free the Doc Pointer to XML */
    xmlFreeDoc(objXMLConfigFiledoc);
    objXMLConfigFiledoc = nullptr;

    return strVersion;
}

std::string CPARSER_FIBEX::GetActiveVersion()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return sg_omActiveVersion;
}

int CPARSER_FIBEX::SetActiveVersion(std::string& omVersion)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    int nResult = FCLASS_FAILURE;

    if (GetActiveVersion() == omVersion)
    {
        return FCLASS_SUCCESS;  // The intended version is already loaded
    }
    else
    {
        std::string omVersionNone = VERSION_NONE;
        nResult = SetActiveVersion(omVersionNone); // First unload
    }

    std::string omDllNameBase = "FibexDll_";
    std::string omExtn = "";
    std::string omReqVersion;
    omReqVersion= omVersion[0];

    if (omReqVersion == VERSION_1)
    {
        omExtn = _T("1_2");
    }
    else if (omReqVersion == VERSION_2)
    {
        omExtn = _T("2_0_1");
    }
    else if ((omVersion == VERSION_3_0) || (omVersion == VERSION_3_0_0))
    {
        omExtn = _T("3_0");
    }
    else if (omVersion == VERSION_NONE)
    {
        return UnloadLibrary();
    }
    else
    {
        UnloadLibrary();//First unload then return the reason
        return VERSION_NOT_SUPPORTED;
    }

    return nResult;
}

// To load the Fibex file
int CPARSER_FIBEX::LoadFibexFile(std::string omFibexFilePath,
                                 PABS_FIBEX_CONTAINER pAbsFibexContainer)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    int nResult = FCLASS_FAILURE;
    std::string omVersion = GetVersionInfo(omFibexFilePath);

    m_strFIBEXVersion = omVersion;

    if (omVersion != VERSION_NONE)
    {
        if (omVersion != GetActiveVersion())
        {
            nResult = SetActiveVersion(omVersion);
        }
        else
        {
            nResult = FCLASS_SUCCESS;
        }
    }

    if (sg_FunLoadFibexFile != nullptr)
    {
        if ((nResult = (*sg_FunLoadFibexFile)(omFibexFilePath)) == FCLASS_SUCCESS)
        {
            if (bTranslate(TYPE_CFIBEXCONTAINER, "", (BYTE*) pAbsFibexContainer,
                           1) == FALSE)
            {
                nResult = FCLASS_FAILURE;
            }
        }
    }
    // Unload loaded specific library after filling up the data structure.
    return nResult;
}

void CPARSER_FIBEX::Initialize()
{
    m_mapCoding.clear();
    m_mapUnitRefToUnit.clear();
    lstCoding.RemoveAll();
    mapCodingPosition.clear();
    m_mapSignalDetails.clear();
    m_mapFrameDetails.clear();
    m_mapPduDetails.clear();
    m_mapECUDetails.clear();
    m_mapChnlToECUDetails.clear();
    m_mapClusterDetails.clear();
    m_mapFramTrigIdToFrame.clear();
    m_mapChannelRefToChnlName.clear();
    m_ECUIdToChnlRefs.clear();
    m_strFIBEXVersion = "";
    m_mapFrameIdToSignalDetails.clear();
}

// To load the Fibex file
int CPARSER_FIBEX::LoadFibexFile_Generic(std::string omFibexFilePath, std::map<std::string, Cluster>& lstCluster)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    Initialize();

    int nResult = S_OK;
    //string omVersion = GetVersionInfo(omFibexFilePath);


    nResult = LoadFibexFile_v2_0(omFibexFilePath);
    if(nResult != FCLASS_SUCCESS)
    {
        return nResult;
    }

    FillFibexStructurev2(lstCluster);

    // Unload loaded specific library after filling up the data structure.
    return nResult;
}

void CPARSER_FIBEX::FillFibexStructurev2(std::map<std::string, Cluster>& lstCluster)
{
    Cluster objCluster;
    CopyClusterDetails(objCluster);

    lstCluster[objCluster.m_omClusterID] = objCluster;
}

void CPARSER_FIBEX::CopyClusterDetails(Cluster& objCluster)
{
    std::map<std::string, CClusterv2>::iterator itr = m_mapClusterDetails.begin();

    while(itr != m_mapClusterDetails.end())
    {
        CClusterv2 objFlxCluster = itr->second;

        memcpy(&objCluster.m_ouClusterInfo, &objFlxCluster.m_absFlxCluster, sizeof(ABS_FLEXRAY_CLUSTER));
        objCluster.m_omClusterID = objFlxCluster.m_omClusterId;
        objCluster.m_strName = objFlxCluster.m_omFlxClusterName;
        //objCluster.m_omChannelRef = objFlxCluster.m_omChannelRef;

        objCluster.m_mapChnls = objFlxCluster.m_mapChnls;

        std::map<std::string, std::string> mapECUs;

        std::map<std::string, CECU>::iterator itrECU = m_mapECUDetails.begin();

        while(itrECU != m_mapECUDetails.end())
        {
            CECU objEcu = itrECU->second;

            CopyECUDetails(objEcu, objCluster);

            itrECU++;
        }

        itr++;
    }
}

void CPARSER_FIBEX::CopyECUDetails(CECU objBaseECU, Cluster& objCluster)
{
    ECU_Struct objECU;

    objECU.m_strEcuId = objBaseECU.m_omECUId;
    objECU.m_strECUName = objBaseECU.m_omShortName;

    CConnector objBaseConn;
    CController objBaseCntrl;

    std::map<std::string, CConnector>::iterator itrECUConn = objBaseECU.m_mapConnector.begin();

    BOOL bContToChannel = FALSE;

    //map<string, map<UINT, FRAME_STRUCT> > mapTxFrames;
    std::map<UINT, std::list<FRAME_STRUCT> > mapTxFrames;
    std::map<UINT, std::list<FRAME_STRUCT> > mapRxFrames;

    BOOL bIsECUConn = FALSE;
    while(itrECUConn != objBaseECU.m_mapConnector.end())
    {
        objBaseConn = itrECUConn->second;

        //if(objBaseConn.m_omChnlRef == objCluster.m_omChannelRef)
        // If the channel reference of the connector matches with
        // the current cluster channel
        if(objCluster.m_mapChnls.find( objBaseConn.m_omChnlRef ) != objCluster.m_mapChnls.end())
        {
            if(objBaseConn.m_omCntrlRef != "")
            {
                objBaseCntrl = objBaseECU.m_mapController[objBaseConn.m_omCntrlRef];
            }

            bIsECUConn = TRUE;

            std::map<std::string, CFrameTriggering>::iterator itr = objBaseConn.m_mapTxFrameTrigs.begin();

            //map<UINT, list<FRAME_STRUCT> > mapTxFrames;

            memcpy(&objECU.m_ouFlexRayParams.m_ouControllerParams, &objBaseCntrl, sizeof(ABS_FLEXRAY_SPEC_CNTLR));

            while(itr != objBaseConn.m_mapTxFrameTrigs.end())
            {
                std::string strFrameTrigId = itr->first;
                CFrameTriggering objBaseFrameTrig = itr->second;

                CFrameTrig objFrameTrig = m_mapFramTrigIdToFrame[strFrameTrigId];

                std::map<std::string, CFrame>::iterator itrSubFrame = m_mapFrameDetails.find(objFrameTrig.m_omFrameRef);

                std::string strFrameRefs, strFrameRefToSearch;
                strFrameRefToSearch = objFrameTrig.m_omFrameRef;

                if(itrSubFrame == m_mapFrameDetails.end())
                {
                    strFrameRefs = m_SubFrameToFrame[objFrameTrig.m_omFrameRef];
                    strFrameRefToSearch =  strFrameRefs;
                }

                CFrame objBaseFrame = m_mapFrameDetails[strFrameRefToSearch];
                FRAME_STRUCT objFrame;

                // If no Frame ref
                if(strFrameRefToSearch == "")
                {
                    itr++;
                    continue;
                }

                //objFrame.m_bConsiderPdu = false;
                /*objFrame.m_nBaseCycle = objFrameTrig.m_nBaseCycle;
                objFrame.m_nReptition = objFrameTrig.m_nCycleRepetition;
                objFrame.m_nSlotId = objFrameTrig.m_nSlotId;*/
                objFrame.m_strFrameName = objBaseFrame.m_omShortName;
                objFrame.m_strFrameId = objBaseFrame.m_omFrameId;
                objFrame.m_nLength = objBaseFrame.m_unFrameLen;
                std::string strChnlName = m_mapChannelRefToChnlName[objBaseConn.m_omChnlRef];

                /*if(objECU.m_strECUName == "ZGW")
                {
                    if(objFrame.m_nSlotId == 145)
                    {
                        objFrame.m_nSlotId = objFrame.m_nSlotId;
                    }
                }*/

                ECHANNEL eChannel = GetChnlEnumFromChnlName(strChnlName);
                objFrame.m_ouChannel = eChannel;

                /*if(m_mapFrameIdToPduList.find(objFrame.m_strFrameId) != m_mapFrameIdToPduList.end())
                {
                objFrame.m_ouPduList = m_mapFrameIdToPduList.find(objFrame.m_strFrameId)->second;
                }
                else */
                {
                    std::map<std::string, CPdu_Instance>::iterator itrPduInst = objBaseFrame.m_mapPduInstList.begin();

                    while(itrPduInst != objBaseFrame.m_mapPduInstList.end())
                    {
                        CPdu_Instance objPduInst;

                        objPduInst = itrPduInst->second;

                        CPdu objPdu = m_mapPduDetails[objPduInst.m_omPduRef];

                        PDU_STRUCT objPduStrct;

                        if(objPduInst.m_strPduId == PDU_DUMMY_2_0)
                        {
                            objPduStrct.m_strPduId = PDU_DUMMY_2_0;
                            objPduStrct.m_nStartBit = 0;

                            // If multiplexer pdu
                            if(objBaseFrame.m_objPdu.m_ouMuxSwitch.m_nSwitchBitLen > 0)
                            {
                                objBaseFrame.m_objPdu.m_strPduId = PDU_DUMMY_2_0;
                                objBaseFrame.m_objPdu.m_nStartBit = 0;
                                CopyPduDetails(objBaseFrame.m_objPdu, objPduStrct);
                            }
                        }
                        else
                        {
                            CopyPduDetails(objPdu, objPduStrct);
                            objPduInst.m_mapSigInstDetails = objPdu.m_mapSigInstDetails;
                        }

                        // Copying signal details
                        std::map<std::string, CSigInstance>::iterator itrSig = objPduInst.m_mapSigInstDetails.begin();

                        //PDU_STRUCT objPdu;
                        //objPdu.m_strPduId = PDU_DUMMY_2_0;
                        std::list<SIGNAL_STRUCT> lstExistSigStrct;
                        if(m_mapFrameIdToSignalDetails.find(objFrame.m_strFrameId + objPduStrct.m_strPduId) != m_mapFrameIdToSignalDetails.end())
                        {
                            lstExistSigStrct = m_mapFrameIdToSignalDetails[objFrame.m_strFrameId + objPduStrct.m_strPduId];
                            objPduStrct.m_ouSignalList = lstExistSigStrct;
                        }
                        else
                        {
                            while (itrSig != objPduInst.m_mapSigInstDetails.end())
                            {
                                CSigInstance objSigInst;
                                objSigInst = itrSig->second;
                                SIGNAL_STRUCT objSignal;

                                objSignal.m_unStartbit = objSigInst.m_unBitPos + objPduInst.m_nStartBit;
                                objSignal.m_ouEndianness = objSigInst.m_bByteOrder ? MOTOROLA : INTEL;
                                std::string strSigRef = objSigInst.m_omSigRef;

                                CSignal objBaseSig = m_mapSignalDetails[strSigRef];

                                //objSignal.m_omSigId = objBaseSig.m_omSigId.c_str();
                                objSignal.m_strSignalName = objBaseSig.m_omShortName;

                                POSITION posCoding = mapCodingPosition[objBaseSig.m_omCodingRef];
                                if(posCoding != nullptr)
                                {
                                    CCODING_v2& objCoding = lstCoding.GetAt(posCoding);

                                    SCODED_TYPE_v2& objCodedType = objCoding.m_sCodedType;
                                    SPHYSICAL_TYPE_v2& objPhysType =  objCoding.m_sPhysicalType;

                                    objSignal.m_nLength = objCodedType.m_sLength.m_unLength;

                                    std::string strDataType;
                                    strDataType = objCodedType.m_ouDataType;

                                    if ( (strDataType =="A_INT16") || (strDataType == "A_INT8") || (strDataType == "A_INT64") || (strDataType == "A_INT32") )
                                    {
                                        objSignal.m_bDataType = CHAR_INT;
                                    }
                                    else
                                    {
                                        objSignal.m_bDataType = CHAR_UINT;
                                    }
                                    if(objCoding.m_odCompuMethodList.GetCount() > 0)
                                    {
                                        SCOMPU_METHOD_v2& objCompuMthd = objCoding.m_odCompuMethodList.GetHead();

                                        objSignal.m_ouCompuMethod.m_eCompuType = objCompuMthd.m_eCategory;
                                        ////objSignal.m_ouCompuMethod.m_uMethod =
                                        std::string strUnitReference = objCompuMthd.m_omUnitRef;

                                        if(objCompuMthd.m_omUnitRef != INVALID_STRING)
                                        {
                                            UNIT_EX objUnitEx = m_mapUnitRefToUnit[strUnitReference];

                                            objSignal.m_omUnit = objUnitEx.m_omDisplayName;
                                        }

                                        CopyCodingDetails(objSignal, objCompuMthd );

                                    }
                                }

                                //string strSigIdForDesc = objSignal.m_omSigId;
                                //objSignal.m_ouDescriptorMap[strSigIdForDesc] = objBaseSig.m_omdeDesc;

                                objPduStrct.m_ouSignalList.push_back(objSignal);

                                //objFrame.m_ouSignalList.push_back(objSignal);
                                itrSig++;
                            }
                        }
                        itrPduInst++;

                        objFrame.m_ouPduList.push_back(objPduStrct);

                        if(m_mapFrameIdToSignalDetails.find(objFrame.m_strFrameId+objPduStrct.m_strPduId) == m_mapFrameIdToSignalDetails.end())
                        {
                            m_mapFrameIdToSignalDetails[objFrame.m_strFrameId + objPduStrct.m_strPduId] = objPduStrct.m_ouSignalList;
                        }
                    }
                }

                if(objFrame.m_nSlotId == 214)
                {
                    int i = 0;
                }

                std::map<int, CAbsSchdTiming>::iterator itrAbsTiming = objFrameTrig.mapAbsSchdTiming.begin();

                while(itrAbsTiming != objFrameTrig.mapAbsSchdTiming.end())
                {
                    objFrame.m_nBaseCycle = itrAbsTiming->second.m_nBaseCycle;
                    objFrame.m_nReptition = itrAbsTiming->second.m_nCycleRepetition;
                    objFrame.m_nSlotId = itrAbsTiming->second.m_nSlotId;

                    if(objFrame.m_nSlotId <= objCluster.m_ouClusterInfo.m_shNUMBER_OF_STATIC_SLOTS)
                    {
                        objFrame.m_eSlotType = STATIC;
                    }
                    else
                    {
                        objFrame.m_eSlotType = DYNAMIC;
                    }

                    std::map<UINT, std::list<FRAME_STRUCT> >::iterator itrtxFrame = mapTxFrames.find(objFrame.m_nSlotId);

                    BOOL bExisting = FALSE;
                    // Check if the list contains frames
                    //map<UINT, list<FRAME_STRUCT>> lstTxFrames;
                    std::list<FRAME_STRUCT> lstFrames;
                    if(itrtxFrame != mapTxFrames.end())
                    {
                        // Getting already existing list of frame from frame id
                        lstFrames = itrtxFrame->second;

                        std::list<FRAME_STRUCT>::iterator itrFrameList = lstFrames.begin();

                        while(itrFrameList != lstFrames.end())
                        {
                            FRAME_STRUCT& objExstFrame = *itrFrameList;

                            // If slot id and base cycle of the frame matches with the
                            // existing frame update the channel details of the frame
                            if((objExstFrame.m_nBaseCycle == objFrame.m_nBaseCycle) /*&& (objExstFrame.m_nReptition == objFrame.m_nReptition)*/)
                            {
                                if(objExstFrame.m_strFrameId == objFrame.m_strFrameId)
                                {
                                    bExisting = TRUE;
                                    if(objFrame.m_ouChannel != objExstFrame.m_ouChannel)
                                    {
                                        // Collecting ECU Channel type
                                        if(objFrame.m_ouChannel == CHANNEL_A && objExstFrame.m_ouChannel == CHANNEL_B)
                                        {
                                            objExstFrame.m_ouChannel = CHANNEL_AB;
                                        }
                                        else if( objFrame.m_ouChannel == CHANNEL_B && objExstFrame.m_ouChannel == CHANNEL_A)
                                        {
                                            objExstFrame.m_ouChannel = CHANNEL_AB;
                                        }
                                    }
                                }
                            }
                            if(bExisting == TRUE)
                            {
                                break;
                            }

                            itrFrameList++;
                        }
                    }

                    if(bExisting == FALSE)
                    {
                        // If the frame length is an odd number
                        if((objFrame.m_nLength % 2) > 0)
                        {
                            // If static assigning the max static payload length
                            if(objFrame.m_eSlotType == STATIC)
                            {
                                objFrame.m_nLength = (objCluster.m_ouClusterInfo.m_shPAYLOAD_LENGTH_STATIC * 2);
                            }
                            // If dynamic assigning the immediate next even length
                            // If next even length is invalid, assiging the max dynamic payload length
                            else if(objFrame.m_eSlotType == DYNAMIC)
                            {
                                objFrame.m_nLength++;

                                if(objFrame.m_nLength > objECU.m_ouFlexRayParams.m_ouControllerParams.m_shMaxDynPayloadLgt)
                                {
                                    objFrame.m_nLength = objECU.m_ouFlexRayParams.m_ouControllerParams.m_shMaxDynPayloadLgt;
                                }
                            }
                        }
                        // Adding the frame list to Slot id
                        lstFrames.push_back(objFrame);
                        //lstTxFrames[objFrame.m_nSlotId] = objFrame;
                    }

                    mapTxFrames[objFrame.m_nSlotId] = lstFrames;

                    std::list<ECU_ID> lstECUIds;

                    if(objCluster.m_mapSlotEcu.find(objFrame.m_nSlotId) != objCluster.m_mapSlotEcu.end())
                    {
                        lstECUIds = objCluster.m_mapSlotEcu.find(objFrame.m_nSlotId)->second;
                    }

                    lstECUIds.push_back(objECU.m_strEcuId);
                    lstECUIds.unique();
                    objCluster.m_mapSlotEcu[objFrame.m_nSlotId] = lstECUIds;
                    itrAbsTiming++;
                }
                itr++;
            }

            std::map<std::string, CFrameTriggering>::iterator itrRx = objBaseConn.m_mapRxFrameTrigs.begin();

            //map<string, FRAME_STRUCT> lstRxFrames;
            //map<UINT, list<FRAME_STRUCT> > mapRxFrames;
            while(itrRx != objBaseConn.m_mapRxFrameTrigs.end())
            {
                std::string strFrameTrigId = itrRx->first;
                CFrameTriggering objBaseFrameTrig = itrRx->second;

                CFrameTrig objFrameTrig = m_mapFramTrigIdToFrame[strFrameTrigId];

                std::map<std::string, CFrame>::iterator itrSubFrame = m_mapFrameDetails.find(objFrameTrig.m_omFrameRef);

                std::string strFrameRefs, strFrameRefToSearch;
                strFrameRefToSearch = objFrameTrig.m_omFrameRef;

                if(itrSubFrame == m_mapFrameDetails.end())
                {
                    strFrameRefs = m_SubFrameToFrame[objFrameTrig.m_omFrameRef];
                    strFrameRefToSearch =  strFrameRefs;
                }

                CFrame objBaseFrame = m_mapFrameDetails[strFrameRefToSearch];

                FRAME_STRUCT objFrame;

                // If no Frame ref
                if(strFrameRefToSearch == "")
                {
                    itrRx++;
                    continue;
                }

                objFrame.m_strFrameName = objBaseFrame.m_omShortName;
                objFrame.m_strFrameId = objBaseFrame.m_omFrameId;
                objFrame.m_nLength = objBaseFrame.m_unFrameLen;

                if(objFrame.m_nSlotId == 214)
                {
                    int i = 0;
                }

                std::string strChnlName = m_mapChannelRefToChnlName[objBaseConn.m_omChnlRef];

                ECHANNEL eChannel = GetChnlEnumFromChnlName(strChnlName);
                objFrame.m_ouChannel = eChannel;

                std::map<std::string, CPdu_Instance>::iterator itrPduInst = objBaseFrame.m_mapPduInstList.begin();

                while(itrPduInst != objBaseFrame.m_mapPduInstList.end())
                {
                    CPdu_Instance objPduInst;

                    objPduInst = itrPduInst->second;

                    CPdu objPdu = m_mapPduDetails[objPduInst.m_omPduRef];

                    PDU_STRUCT objPduStrct;

                    if(objPduInst.m_strPduId == PDU_DUMMY_2_0)
                    {
                        objPduStrct.m_strPduId = PDU_DUMMY_2_0;
                        objPduStrct.m_nStartBit = 0;

                        // If multiplexer pdu
                        if(objBaseFrame.m_objPdu.m_ouMuxSwitch.m_nSwitchBitLen > 0)
                        {
                            objBaseFrame.m_objPdu.m_strPduId = PDU_DUMMY_2_0;
                            objBaseFrame.m_objPdu.m_nStartBit = 0;
                            CopyPduDetails(objBaseFrame.m_objPdu, objPduStrct);
                        }
                    }
                    else
                    {
                        CopyPduDetails(objPdu, objPduStrct);
                        objPduInst.m_mapSigInstDetails = objPdu.m_mapSigInstDetails;
                    }

                    std::list<SIGNAL_STRUCT> lstExistSigStrct;
                    if(m_mapFrameIdToSignalDetails.find(objFrame.m_strFrameId + objPduStrct.m_strPduId) != m_mapFrameIdToSignalDetails.end())
                    {
                        lstExistSigStrct = m_mapFrameIdToSignalDetails[objFrame.m_strFrameId + objPduStrct.m_strPduId];
                        objPduStrct.m_ouSignalList = lstExistSigStrct;
                    }
                    else
                    {

                        // Copying signal details
                        std::map<std::string, CSigInstance>::iterator itrSig = objPduInst.m_mapSigInstDetails.begin();

                        while (itrSig != objPduInst.m_mapSigInstDetails.end())
                        {
                            CSigInstance objSigInst;
                            objSigInst = itrSig->second;
                            SIGNAL_STRUCT objSignal;

                            objSignal.m_unStartbit = objSigInst.m_unBitPos + objPduInst.m_nStartBit;
                            objSignal.m_ouEndianness = objSigInst.m_bByteOrder ? MOTOROLA : INTEL;
                            std::string strSigRef = objSigInst.m_omSigRef;

                            CSignal objBaseSig = m_mapSignalDetails[strSigRef];

                            //objSignal.m_omSigId = objBaseSig.m_omSigId.c_str();
                            objSignal.m_strSignalName = objBaseSig.m_omShortName;

                            POSITION posCoding = mapCodingPosition[objBaseSig.m_omCodingRef];
                            if(posCoding != nullptr)
                            {
                                CCODING_v2& objCoding = lstCoding.GetAt(posCoding);

                                SCODED_TYPE_v2& objCodedType = objCoding.m_sCodedType;
                                SPHYSICAL_TYPE_v2& objPhysType =  objCoding.m_sPhysicalType;

                                objSignal.m_nLength = objCodedType.m_sLength.m_unLength;

                                std::string strDataType;
                                strDataType = objCodedType.m_ouDataType;



                                if ( (strDataType =="A_INT16") || (strDataType == "A_INT8") || (strDataType == "A_INT64") || (strDataType == "A_INT32") )
                                {
                                    objSignal.m_bDataType = CHAR_INT;
                                }
                                else
                                {
                                    objSignal.m_bDataType = CHAR_UINT;
                                }

                                if(objCoding.m_odCompuMethodList.GetCount() > 0)
                                {
                                    SCOMPU_METHOD_v2& objCompuMthd = objCoding.m_odCompuMethodList.GetHead();

                                    objSignal.m_ouCompuMethod.m_eCompuType = objCompuMthd.m_eCategory;
                                    ////objSignal.m_ouCompuMethod.m_uMethod =

                                    std::string strUnitReference = objCompuMthd.m_omUnitRef;

                                    if(objCompuMthd.m_omUnitRef != INVALID_STRING)
                                    {
                                        UNIT_EX objUnitEx = m_mapUnitRefToUnit[strUnitReference];

                                        objSignal.m_omUnit = objUnitEx.m_omDisplayName;
                                    }

                                    CopyCodingDetails(objSignal, objCompuMthd );

                                }
                            }

                            //string strSigIdForDesc = objSignal.m_omSigId;
                            //objSignal.m_ouDescriptorMap[strSigIdForDesc] = objBaseSig.m_omdeDesc;

                            objPduStrct.m_ouSignalList.push_back(objSignal);

                            //objFrame.m_ouSignalList.push_back(objSignal);
                            itrSig++;
                        }
                    }

                    objFrame.m_ouPduList.push_back(objPduStrct);
                    itrPduInst++;

                    if(m_mapFrameIdToSignalDetails.find(objFrame.m_strFrameId+objPduStrct.m_strPduId) == m_mapFrameIdToSignalDetails.end())
                    {
                        m_mapFrameIdToSignalDetails[objFrame.m_strFrameId + objPduStrct.m_strPduId] = objPduStrct.m_ouSignalList;
                    }
                }

                std::map<int, CAbsSchdTiming>::iterator itrAbsTimingRx = objFrameTrig.mapAbsSchdTiming.begin();

                while(itrAbsTimingRx != objFrameTrig.mapAbsSchdTiming.end())
                {
                    objFrame.m_nBaseCycle = itrAbsTimingRx->second.m_nBaseCycle;
                    objFrame.m_nReptition = itrAbsTimingRx->second.m_nCycleRepetition;
                    objFrame.m_nSlotId = itrAbsTimingRx->second.m_nSlotId;

                    if(objFrame.m_nSlotId <= objCluster.m_ouClusterInfo.m_shNUMBER_OF_STATIC_SLOTS)
                    {
                        objFrame.m_eSlotType = STATIC;
                    }
                    else
                    {
                        objFrame.m_eSlotType = DYNAMIC;
                    }

                    std::map<UINT, std::list<FRAME_STRUCT>>::iterator itrRxFrame = mapRxFrames.find(objFrame.m_nSlotId);

                    // Check if the slot contains already a frame
                    std::list<FRAME_STRUCT> lstRxFrames;

                    BOOL bExisting = FALSE;
                    if(itrRxFrame != mapRxFrames.end())
                    {
                        // Getting already existing list of frames for the slot
                        lstRxFrames = itrRxFrame->second;

                        std::list<FRAME_STRUCT>::iterator itrRxLst = lstRxFrames.begin();

                        while(itrRxLst != lstRxFrames.end())
                        {

                            FRAME_STRUCT& objExstFrame = *itrRxLst;

                            // If slot id and base cycle of the frame matches with the
                            // existing frame update the channel details of the frame
                            if(objExstFrame.m_nBaseCycle == objFrame.m_nBaseCycle /*&& objExstFrame.m_nReptition == objExstFrame.m_nReptition*/)
                            {
                                if(objExstFrame.m_strFrameId == objFrame.m_strFrameId)
                                {
                                    bExisting = TRUE;
                                    if(objFrame.m_ouChannel != objExstFrame.m_ouChannel)
                                    {
                                        // Collecting ECU Channel type
                                        if(objFrame.m_ouChannel == CHANNEL_A && objExstFrame.m_ouChannel == CHANNEL_B)
                                        {
                                            objExstFrame.m_ouChannel = CHANNEL_AB;
                                        }
                                        else if( objECU.m_ouChannel == CHANNEL_B && objExstFrame.m_ouChannel == CHANNEL_A)
                                        {
                                            objExstFrame.m_ouChannel = CHANNEL_AB;
                                        }
                                    }
                                }
                            }

                            if(bExisting == TRUE)
                            {
                                break;
                            }

                            itrRxLst++;
                        }
                    }

                    if(bExisting == FALSE)
                    {
                        // If the frame length is an odd number
                        if((objFrame.m_nLength % 2) > 0)
                        {
                            // If static assigning the max static payload length
                            if(objFrame.m_eSlotType == STATIC)
                            {
                                objFrame.m_nLength = (objCluster.m_ouClusterInfo.m_shPAYLOAD_LENGTH_STATIC * 2);
                            }
                            // If dynamic assigning the immediate next even length
                            // If next even length is invalid, assiging the max dynamic payload length
                            else if(objFrame.m_eSlotType == DYNAMIC)
                            {
                                objFrame.m_nLength++;

                                if(objFrame.m_nLength > objECU.m_ouFlexRayParams.m_ouControllerParams.m_shMaxDynPayloadLgt)
                                {
                                    objFrame.m_nLength = objECU.m_ouFlexRayParams.m_ouControllerParams.m_shMaxDynPayloadLgt;
                                }
                            }
                        }

                        // Adding the frame list to Slot id
                        lstRxFrames.push_back(objFrame);
                    }


                    mapRxFrames[objFrame.m_nSlotId] = lstRxFrames;

                    std::list<ECU_ID> lstECUIds;

                    if(objCluster.m_mapSlotEcu.find(objFrame.m_nSlotId) != objCluster.m_mapSlotEcu.end())
                    {
                        lstECUIds = objCluster.m_mapSlotEcu.find(objFrame.m_nSlotId)->second;
                    }

                    lstECUIds.push_back(objECU.m_strEcuId);
                    lstECUIds.unique();

                    objCluster.m_mapSlotEcu[objFrame.m_nSlotId] = lstECUIds;

                    itrAbsTimingRx++;
                }
                itrRx++;
            }

            objECU.m_ouRxFrames = mapRxFrames;
            objECU.m_ouTxFrames = mapTxFrames;

            if(objECU.m_ouFlexRayParams.m_ouControllerParams.m_sKeySlotUsage.m_omNone == "")
            {
                if(objECU.m_ouFlexRayParams.m_ouControllerParams.m_sKeySlotUsage.m_nStartUpSync > 0)
                {
                    objECU.m_ouFlexRayParams.m_nKeySlot = objECU.m_ouFlexRayParams.m_ouControllerParams.m_sKeySlotUsage.m_nStartUpSync;
                }
                else if(objECU.m_ouFlexRayParams.m_ouControllerParams.m_sKeySlotUsage.m_nSync > 0)
                {
                    objECU.m_ouFlexRayParams.m_nKeySlot = objECU.m_ouFlexRayParams.m_ouControllerParams.m_sKeySlotUsage.m_nSync;
                }
            }

            std::map<std::string, std::string> lstChnlRefs;
            lstChnlRefs = m_ECUIdToChnlRefs[objECU.m_strEcuId];

            std::map<std::string, std::string>::iterator itrchnlRef = lstChnlRefs.begin();
            while(itrchnlRef != lstChnlRefs.end())
            {
                std::string strChnlRef = lstChnlRefs[itrchnlRef->first];

                std::map<std::string, std::string>::iterator itrChnl = m_mapChannelRefToChnlName.find(strChnlRef);

                if(itrChnl != m_mapChannelRefToChnlName.end())
                {
                    std::string strChannelName = m_mapChannelRefToChnlName[strChnlRef];

                    // Collecting ECU Channel type
                    if(objECU.m_ouChannel == CHANNEL_A && strChannelName == "B")
                    {
                        objECU.m_ouChannel = CHANNEL_AB;
                    }
                    else if( objECU.m_ouChannel == CHANNEL_B && strChannelName == "A")
                    {
                        objECU.m_ouChannel = CHANNEL_AB;
                    }
                    else if(strChannelName == "A")
                    {
                        objECU.m_ouChannel = CHANNEL_A;
                    }
                    else if(strChannelName == "B")
                    {
                        objECU.m_ouChannel = CHANNEL_B;
                    }
                }

                itrchnlRef++;
            }
        }
        itrECUConn++;
    }

    if(bIsECUConn == TRUE)
    {
        objCluster.m_ouEcuList[objECU.m_strEcuId] = objECU;
    }
}

void CPARSER_FIBEX::CopyPduDetails(CPdu objPdu, PDU_STRUCT& objActPdu)
{
    objActPdu.m_strPduId = objPdu.m_strPduId;
    objActPdu.m_strPDUName = objPdu.m_strPDUName;
    objActPdu.m_nStartBit = objPdu.m_nStartBit;
    objActPdu.m_nLength = objPdu.m_nLength;
    objActPdu.m_ouEndianness = (ENDIANNESS)objPdu.m_ouEndianness;

    // Check if Pdu is a multiplexor
    if(objPdu.m_ouMuxSwitch.m_nSwitchBitLen > 0)
    {
        int nStartBit = objPdu.m_ouStaticPart.m_StaticPartBitPos;
        int nBitLen = objPdu.m_ouStaticPart.m_nStPartBitLen;
        int nEndBit = nStartBit + nBitLen;

        CMuxPdu objSwitchMuxPdu;
        //Adding switch signal
        SIGNAL_STRUCT objSwitchSig;

        objSwitchSig.m_strSignalName = objPdu.m_ouMuxSwitch.m_omSwitchName;
        objSwitchSig.m_unStartbit = objPdu.m_ouMuxSwitch.m_nSwitchBitPos;
        objSwitchSig.m_ouEndianness = (ENDIANNESS)objPdu.m_ouMuxSwitch.m_ouSwitchByteOrder;
        objSwitchSig.m_nLength = objPdu.m_ouMuxSwitch.m_nSwitchBitLen;


        //string omSwCodeMux = MUX_SWITCH;
        //lstStaticSigs.push_back(objSwitchSig);
        objSwitchMuxPdu.omPduId = MUX_SWITCH;
        objSwitchMuxPdu.lstSignalStruct.push_back(objSwitchSig);
        // Adding switch signal to Pdu
        objActPdu.m_ouSwitchToSignals[objSwitchMuxPdu.omPduId] = objSwitchMuxPdu;

        std::list<SIGNAL_STRUCT> lstStaticSigs;
        lstStaticSigs.clear();
        std::map<std::string, std::string>::iterator itrPdu = objPdu.m_ouStaticPart.m_mapPduIdToPdu.begin();

        CMuxPdu objstaticMuxPdu;

        while(itrPdu != objPdu.m_ouStaticPart.m_mapPduIdToPdu.end())
        {
            std::string strPdu = itrPdu->second;;
            CPdu objStPdu = m_mapPduDetails[strPdu];
            objstaticMuxPdu.omPduId = strPdu;
            std::list<SIGNAL_STRUCT> lstSigStrct;
            GetSignalStruct(objStPdu.m_mapSigInstDetails, lstSigStrct);

            std::list<SIGNAL_STRUCT>::iterator itrSigStrct = lstSigStrct.begin();

            while(itrSigStrct != lstSigStrct.end())
            {
                SIGNAL_STRUCT objSigStruct = *itrSigStrct;
                int nSigSB = objSigStruct.m_unStartbit;
                int nSigL = objSigStruct.m_nLength;
                int nSigEB = nSigSB + nSigL + nStartBit;

                // Check if the signal is with in the segment position's
                //if((nStartBit >= nSigSB) && (nEndBit <= nSigEB))
                if(((nSigSB + nStartBit) >= nStartBit) && ((nSigEB) <= nEndBit))
                {
                    lstStaticSigs.push_back(objSigStruct);
                }

                itrSigStrct++;
            }

            itrPdu++;
        }

        if(lstStaticSigs.size() > 0)
        {
            std::string omSwCode = MUX_STATIC;
            objstaticMuxPdu.lstSignalStruct = lstStaticSigs;
            objActPdu.m_ouSwitchToSignals[omSwCode] = objstaticMuxPdu;
        }

        // Dynpart signals
        int nDPStartBit = objPdu.m_ouDynPart.m_nDynPartBitPos;
        int nDPBitLen = objPdu.m_ouDynPart.m_nDynPartBitLen;
        int nDPEndBit = nDPStartBit + nDPBitLen;

        std::map<std::string, std::string>::iterator itrDPPdu = objPdu.m_ouDynPart.m_mapSwitchToPdu.begin();

        CMuxPdu objDynMuxPdu;
        std::list<SIGNAL_STRUCT> lstDynStaticSigs;
        while(itrDPPdu != objPdu.m_ouDynPart.m_mapSwitchToPdu.end())
        {
            lstDynStaticSigs.clear();
            std::string strDPPdu = itrDPPdu->second;;
            objDynMuxPdu.omPduId = strDPPdu;

            std::string strSwFrameRef = "";
            CPdu objDPPdu;
            CFrame objFrame;
            if(m_strFIBEXVersion == VERSION_2)
            {
                strSwFrameRef = m_SubFrameToFrame[strDPPdu];
                objFrame = m_mapFrameDetails[strSwFrameRef];
            }
            else
            {
                objDPPdu = m_mapPduDetails[strDPPdu];
            }

            //map<string, CSigInstance>::iterator itrDPPdu = objDPPdu.m_mapSigInstDetails.begin();

            std::list<SIGNAL_STRUCT> lstSigDStrct;

            if(m_strFIBEXVersion == VERSION_2)
            {
                std::map<std::string, CSigInstance> mapsigInst = m_mapSubFrameToSigInst[strDPPdu];
                GetSignalStruct(mapsigInst, lstSigDStrct);
            }
            else
            {
                GetSignalStruct(objDPPdu.m_mapSigInstDetails, lstSigDStrct);
            }

            std::list<SIGNAL_STRUCT>::iterator itrSigStruct = lstSigDStrct.begin();

            while(itrSigStruct != lstSigDStrct.end())
            {
                SIGNAL_STRUCT objSigDStrct = *itrSigStruct;
                int nDSigSB = objSigDStrct.m_unStartbit;
                int nDSigL = objSigDStrct.m_nLength;
                int nDSigEB = nDSigSB + nDSigL + nDPStartBit;

                // Check if the signal is with in the segment position's
                //if((nDPStartBit >= nDSigSB) && (nDPEndBit <= nDSigEB))
                if((( nDSigSB + nDPStartBit) >= nDPStartBit) && (nDSigEB <= nDPEndBit))
                {
                    objSigDStrct.m_unStartbit = nDSigSB + nDPStartBit;
                    objSigDStrct.m_nLength = nDPBitLen;
                    lstDynStaticSigs.push_back(objSigDStrct);
                }

                itrSigStruct++;
            }

            int nSwitchCode = atoi(itrDPPdu->first.c_str());

            if(lstDynStaticSigs.size() > 0)
            {
                std::ostringstream stringStream;
                stringStream << nSwitchCode;
                objDynMuxPdu.lstSignalStruct = lstDynStaticSigs;
                objActPdu.m_ouSwitchToSignals[stringStream.str()] = objDynMuxPdu;
            }

            itrDPPdu++;
        }
    }
}

void CPARSER_FIBEX::GetSignalStruct(std::map<std::string, CSigInstance> mapSigInstance, std::list<SIGNAL_STRUCT>& listSignal)
{
    std::map<std::string, CSigInstance>::iterator itrSigInst = mapSigInstance.begin();
    while (itrSigInst != mapSigInstance.end())
    {
        CSigInstance objSigInst;
        objSigInst = itrSigInst->second;
        SIGNAL_STRUCT objSignal;

        objSignal.m_unStartbit = objSigInst.m_unBitPos;
        objSignal.m_ouEndianness = objSigInst.m_bByteOrder ? MOTOROLA : INTEL;
        std::string strSigRef = objSigInst.m_omSigRef;

        CSignal objBaseSig = m_mapSignalDetails[strSigRef];

        //objSignal.m_omSigId = objBaseSig.m_omSigId.c_str();
        objSignal.m_strSignalName = objBaseSig.m_omShortName;

        POSITION posCoding = mapCodingPosition[objBaseSig.m_omCodingRef];
        if(posCoding != nullptr)
        {
            CCODING_v2& objCoding = lstCoding.GetAt(posCoding);

            SCODED_TYPE_v2& objCodedType = objCoding.m_sCodedType;
            SPHYSICAL_TYPE_v2& objPhysType =  objCoding.m_sPhysicalType;

            objSignal.m_nLength = objCodedType.m_sLength.m_unLength;

            std::string strDataType;
            strDataType = objCodedType.m_ouDataType;

            //objSignal.m_omDataType = strDataType;
            if ( (strDataType =="A_INT16") || (strDataType == "A_INT8") || (strDataType == "A_INT64") || (strDataType == "A_INT32") )
            {
                objSignal.m_bDataType = CHAR_INT;
            }
            else
            {
                objSignal.m_bDataType = CHAR_UINT;
            }
            if(objCoding.m_odCompuMethodList.GetCount() > 0)
            {
                SCOMPU_METHOD_v2& objCompuMthd = objCoding.m_odCompuMethodList.GetHead();

                objSignal.m_ouCompuMethod.m_eCompuType = objCompuMthd.m_eCategory;
                std::string strUnitReference = objCompuMthd.m_omUnitRef;

                if(objCompuMthd.m_omUnitRef != INVALID_STRING)
                {
                    UNIT_EX objUnitEx = m_mapUnitRefToUnit[strUnitReference];

                    objSignal.m_omUnit = objUnitEx.m_omDisplayName;
                }

                CopyCodingDetails(objSignal, objCompuMthd );

            }
        }

        listSignal.push_back(objSignal);
        itrSigInst++;
    }
}

ECHANNEL CPARSER_FIBEX::GetChnlEnumFromChnlName(std::string strChannelName)
{
    if(strChannelName == "A")
    {
        return CHANNEL_A;
    }
    if(strChannelName == "B")
    {
        return CHANNEL_B;
    }

    return UNSPECIFIED;
}

void CPARSER_FIBEX::CopyCodingDetails(SIGNAL_STRUCT& objSignal, SCOMPU_METHOD_v2 objCompuMthd)
{
    CCompuMethodEx tempCompuMethod;
    tempCompuMethod.m_eCompuType = sCopyCompuType(objCompuMthd.m_ouCategory);
    sCopyCompuMethod(tempCompuMethod, objCompuMthd.m_sCompu_Internal_To_Phys);
    //ouTempSignalDef.m_ouCompuMethods.SetSize(ouTempSignalDef.m_ouCompuMethods.GetSize() + 1);
    objSignal.m_ouCompuMethod = (tempCompuMethod);
}


void CPARSER_FIBEX::sCopyCompuMethod(CCompuMethodEx& ouCompuMethodEx, SCOMPU_INTERNAL_TO_PHYS_v2& ouCompuInToPhys)
{
    POSITION pos = ouCompuInToPhys.m_odCompuScaleList.GetHeadPosition();
    COMPU_EXPRESSION_MSGSIG eCategory = ouCompuMethodEx.m_eCompuType;
    //COMPU_METHOD_EX &ouReturnCompuMethod = (COMPU_METHOD_EX)ouCompuMethodEx.m_uMethod;
    switch (eCategory)
    {
        case IDENTICAL_ENUM:
        {
            while (pos != nullptr)
            {
                SCOMPU_SCALE_v2& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);
                ouCompuMethodEx.m_uMethod.m_IdenticalCode.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
                ouCompuMethodEx.m_uMethod.m_IdenticalCode.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
            }
            break;
        }
        case LINEAR_ENUM:
        {
            while (pos != nullptr)
            {
                SCOMPU_SCALE_v2& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);
                ouCompuMethodEx.m_uMethod.m_LinearCode.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
                ouCompuMethodEx.m_uMethod.m_LinearCode.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
                POSITION pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetHeadPosition();
                if (pos1 != nullptr)
                {
                    ouCompuMethodEx.m_uMethod.m_LinearCode.m_dD0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetNext(pos1);
                }
                else
                {
                    ouCompuMethodEx.m_uMethod.m_LinearCode.m_dD0 = 1;
                }
                pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetHeadPosition();
                if (pos1 != nullptr)
                {
                    ouCompuMethodEx.m_uMethod.m_LinearCode.m_dN0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);
                    float fCheck = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetAt(pos1);
                    if (fCheck == 0.062525)
                    {
                        int z = 0;
                    }
                    ouCompuMethodEx.m_uMethod.m_LinearCode.m_dN1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);
                }
            }
            break;
        }
        case SCALE_LINEAR_ENUM:
        {
            while (pos != nullptr)
            {
                SCOMPU_SCALE_v2& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);

                LINEAR_CODE_EX objLinearCode;

                objLinearCode.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
                objLinearCode.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;


                POSITION pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetHeadPosition();
                if (pos1 != nullptr)
                {
                    objLinearCode.m_dD0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetNext(pos1);
                }

                pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetHeadPosition();
                if (pos1 != nullptr)
                {
                    objLinearCode.m_dN0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);

                    objLinearCode.m_dN1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);
                }

                ouCompuMethodEx.m_uMethod.m_objScaleLinear.push_back(objLinearCode);
            }
            break;
        }
        case TEXTTABLE_ENUM:
        {
            while (pos != nullptr)
            {
                SCOMPU_SCALE_v2& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);

                TEXT_CODE_VAR_EX objTextCdeVar;

                objTextCdeVar.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
                objTextCdeVar.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
                objTextCdeVar.m_aTextName = tempAbsCompuScale.m_sCompuConst.m_omVT.c_str();

                ouCompuMethodEx.m_uMethod.m_objTextCode.push_back(objTextCdeVar);
            }
            break;
        }
        case TAB_NOINTP_ENUM:
        {
            while (pos != nullptr)
            {
                SCOMPU_SCALE_v2& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);

                TAB_CODE_VAR_EX objTab;

                objTab.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
                objTab.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
                objTab.m_dPhysVal = tempAbsCompuScale.m_sCompuConst.m_fV;

                ouCompuMethodEx.m_uMethod.m_objTabCode.push_back(objTab);
            }
            break;
        }
        case FORMULA_ENUM:
        {
            while (pos != nullptr)
            {
                SCOMPU_SCALE_v2& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);
                PFORMULA_CODE_VAR_EX tempFormula = new FORMULA_CODE_VAR_EX;
                tempFormula->m_pFirstOperand = nullptr;

                tempFormula->m_omFormula = tempAbsCompuScale.m_sCompuConst.m_omVT.c_str();
            }
            break;
        }
    }
}

COMPU_EXPRESSION_MSGSIG CPARSER_FIBEX::sCopyCompuType(std::string ouCompuType)
{
    COMPU_EXPRESSION_MSGSIG sReturnCompuEx = IDENTICAL_ENUM;
    if (ouCompuType == "IDENTICAL")
    {
        sReturnCompuEx = IDENTICAL_ENUM;
    }
    else if (ouCompuType == "LINEAR")
    {
        sReturnCompuEx = LINEAR_ENUM;
    }
    else if (ouCompuType == "SCALE-LINEAR")
    {
        sReturnCompuEx = SCALE_LINEAR_ENUM;
    }
    else if (ouCompuType == "TEXTTABLE")
    {
        sReturnCompuEx = TEXTTABLE_ENUM;
    }
    else if (ouCompuType == "SCALE-LINEAR-TEXTTABLE")
    {
        sReturnCompuEx = SCALE_LINEAR_TEXTTABLE_ENUM;
    }
    else if (ouCompuType == "TAB-NOINTP")
    {
        sReturnCompuEx = TAB_NOINTP_ENUM;
    }
    else if (ouCompuType == "FORMULA")
    {
        sReturnCompuEx = FORMULA_ENUM;
    }

    return sReturnCompuEx;
}

RANGE_VALID CPARSER_FIBEX::eGetRangeValid(std::string omValidity)
{
    RANGE_VALID eResult = VALID;

    if (omValidity == "NOT-VALID")
    {
        eResult = INVALID;
    }
    else if (omValidity == "NOT-AVAILABLE")
    {
        eResult = NOT_AVAILABLE;
    }
    else if (omValidity ==  "VALID")
    {
        eResult = VALID;
    }

    return eResult;
}

HRESULT CPARSER_FIBEX::LoadFibexFile_v2_0(std::string omFibexFilePath)
{

    int nError = 0;
    std::string ErrorString = "";
    HRESULT hr = S_FALSE;
    MSXML2::IXMLDOMDocument2Ptr pXMLDoc;
    MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMNamedNodeMapPtr = nullptr;
    LPVOID pfg = nullptr;
    CoInitialize(nullptr);
    //pXMLDoc = (IXMLDOMDocument2Ptr)(pfg);

    //hr = pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
    hr = (hr == S_OK) ? hr : pXMLDoc.CreateInstance( __uuidof(MSXML2::DOMDocument60) );
    hr = (hr == S_OK) ? hr : pXMLDoc.CreateInstance( __uuidof(MSXML2::DOMDocument30) );
    /*hr = (hr == S_OK) ? hr : pXMLDoc.CreateInstance( __uuidof(MSXML2::DOMDocument50) );*/
    hr = (hr == S_OK) ? hr : pXMLDoc.CreateInstance( __uuidof(MSXML2::DOMDocument40) );
    hr = (hr == S_OK) ? hr : pXMLDoc.CreateInstance( __uuidof(MSXML2::DOMDocument26) );
    hr = (hr == S_OK) ? hr : pXMLDoc.CreateInstance( __uuidof(MSXML2::DOMDocument) );


    if (SUCCEEDED(hr))
    {

        pXMLDoc->async = VARIANT_FALSE;
        pXMLDoc->validateOnParse = VARIANT_FALSE;


        if (pXMLDoc->load(variant_t (omFibexFilePath.c_str()))!=VARIANT_TRUE)
        {
            return FCLASS_FAILURE;
        }
        else
        {

            //Get the root element
            MSXML2::IXMLDOMElementPtr m_pRooterElm = nullptr;


            hr = pXMLDoc->get_documentElement(&m_pRooterElm);

            std::string strVersion = "";

            _variant_t varValue;
            // Get the Fibex version
            if(pXMLDoc != nullptr)
            {
                varValue = m_pRooterElm->getAttribute("VERSION");
            }

            if(varValue.vt == VT_NULL)
            {
                return FCLASS_FAILURE;
            }

            m_strFIBEXVersion = _com_util::ConvertBSTRToString(varValue.bstrVal);

            if(m_strFIBEXVersion == "")
            {
                return FCLASS_FAILURE;
            }

            if(m_strFIBEXVersion.find(VERSION_2) == 0)
            {
                m_strFIBEXVersion = VERSION_2;
            }
            if(m_strFIBEXVersion.find(VERSION_3_0) == 0)
            {
                m_strFIBEXVersion = VERSION_3_0;
            }
            if(m_strFIBEXVersion.find(VERSION_3_1) == 0)
            {
                m_strFIBEXVersion = VERSION_3_1;
            }

            if(m_strFIBEXVersion != VERSION_2 && m_strFIBEXVersion != VERSION_3_0 && m_strFIBEXVersion != VERSION_3_1)
            {
                return VERSION_NOT_SUPPORTED;
            }

            if (FAILED(hr) || (m_pRooterElm == nullptr))
            {
                ErrorString = "Empty document";
                nError = 1;
            }
            else
            {
                //CWaitCursor omWaitCursor;
                std::string strTemp, TempNodeName;
                LONG lIdCount = 0, lChild = 0;
                CComPtr<MSXML2::IXMLDOMNodeList> NodeListPtr;
                MSXML2::IXMLDOMNodePtr iNode = nullptr;

                pIXMLDOMNamedNodeMapPtr = m_pRooterElm->Getattributes();
                if (pIXMLDOMNamedNodeMapPtr->Getlength() >= 1)
                {
                    iNode = pIXMLDOMNamedNodeMapPtr->getNamedItem(_bstr_t("VERSION"));
                }
                /* READ THE CHILDS OF THE ROOT ELEMENT */
                MSXML2::IXMLDOMNodeListPtr NodesListPtr = nullptr;
                m_pRooterElm->get_childNodes(&NodesListPtr);
                long lCount = 0;
                NodesListPtr->get_length(&lCount);
                for(int i=0; i < lCount; i++)
                {
                    iNode = NodesListPtr->Getitem(i);
                    if (iNode != nullptr)
                    {
                        BSTR bstr = iNode->GetnodeName().GetBSTR();
                        strTemp = _com_util::ConvertBSTRToString(bstr);

                        //strTemp = string(iNode->GetnodeName().GetBSTR());

                        //bRemoveTag(strTemp,TempNodeName);

                        if (strTemp == ELEMENTSDATA_NODE)
                        {
                            LoadElementsv2(iNode);
                            //m_ouElement.Load(FRAME, iNode);
                        }
                        else if(strTemp == PROC_INFO_DATA_NODE)
                        {
                            LoadCodings(iNode);
                        }
                        iNode.Release();
                    }
                }

            }
            m_pRooterElm.Release();
        }
        pXMLDoc.Release();
    }

    pIXMLDOMNamedNodeMapPtr.Release();

    return FCLASS_SUCCESS;
}

void CPARSER_FIBEX::LoadCodings(MSXML2::IXMLDOMNodePtr pIDomNode)
{
    HRESULT hResult = S_FALSE;

    //MSXML2::IXMLDOMNodeList* pChildList = nullptr;
    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pIDomNode->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrDOMNode = pChildList->Getitem(i);
        std::string strTemp;
        BSTR bstr = pCurrDOMNode->GetnodeName().GetBSTR();
        std::string omNodeName = _com_util::ConvertBSTRToString(bstr);
        std::string omModNodeName = "";
        CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

        if(omModNodeName == CODINGS_NODE)
        {
            CollectCodings(pCurrDOMNode);
        }
        else if(omModNodeName == UNIT_SPEC_NODE)
        {
            //LoadCodings(pCurrDOMNode);
            LoadCodingsUnit(pCurrDOMNode);
        }
        else if(omModNodeName == UNITS_NODE)
        {
            CollectUnits(pCurrDOMNode);
        }

        pCurrDOMNode.Release();
    }

    pChildList.Release();
}

void CPARSER_FIBEX::LoadCodingsUnit(MSXML2::IXMLDOMNodePtr pIDomNode)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pIDomNode->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrDOMNode = pChildList->Getitem(i);
        std::string strTemp;
        BSTR bstr = pCurrDOMNode->GetnodeName().GetBSTR();
        std::string omNodeName = _com_util::ConvertBSTRToString(bstr);
        std::string omModNodeName = "";
        CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

        if(omModNodeName == UNITS_NODE)
        {
            CollectUnits(pCurrDOMNode);
        }

        pCurrDOMNode.Release();
    }
    pChildList.Release();
}

void CPARSER_FIBEX::CollectUnits(MSXML2::IXMLDOMNodePtr pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
    }
    pChildList.Release();
    //   return hResult;
}

void CPARSER_FIBEX::LoadElementsv2(MSXML2::IXMLDOMNodePtr pIDomNode)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pIDomNode->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrDOMNode = pChildList->Getitem(i);
        std::string strTemp;
        BSTR bstr = pCurrDOMNode->GetnodeName().GetBSTR();
        std::string omNodeName = _com_util::ConvertBSTRToString(bstr);
        std::string omModNodeName = "";
        CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

        if(omModNodeName == SIGNALS_NODE)
        {
            CollectSignalInfov2(pCurrDOMNode);
        }
        else if (omModNodeName == PDUS_NODE)
        {
            CollectFrameInfov2(pCurrDOMNode);
        }
        else if (omModNodeName == FRAMES_NODE)
        {
            CollectFrameInfov2(pCurrDOMNode);
        }
        else if(omModNodeName == ECUS_NODE)
        {
            CollectECUInfov2(pCurrDOMNode);
        }
        else if (omModNodeName == CLUSTERS_NODE)
        {
            CollectClusterinfov2(pCurrDOMNode);
        }
        else if(omModNodeName == CHANNELS_NODE)
        {
            CollectFrameTrigInfov2(pCurrDOMNode);
        }

        pCurrDOMNode.Release();
    }
    pChildList.Release();
}

void CPARSER_FIBEX::CollectCodings(MSXML2::IXMLDOMNodePtr pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);

        pCurrChild.Release();
    }

    pChildList.Release();

    //   return hResult;
}

HRESULT CPARSER_FIBEX::Process_Load(MSXML2::IXMLDOMNodePtr pCurrDOMNode)
{
    HRESULT hResult = S_FALSE;

    BSTR bstr = pCurrDOMNode->GetnodeName().GetBSTR();
    std::string omNodeName = _com_util::ConvertBSTRToString(bstr);
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    //MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();
    if (omModNodeName == CODING_NODE)
    {
        CCODING_v2 ouCoding;
        hResult = ouCoding.Load(pCurrDOMNode);
        if (hResult == S_OK)
        {
            std::string strCodingId = ouCoding.m_omID;
            POSITION pos = lstCoding.AddTail(ouCoding);

            mapCodingPosition[strCodingId] = pos;
            /*m_mapCoding[strCodingId] = ouCoding;*/
            //m_odCodingList.AddTail(ouCoding);
        }
    }
    else if(omModNodeName == UNIT)
    {
        std::string strId;
        CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID"));
        UNIT_EX objUnitEx;
        objUnitEx.m_omID=strId;
        DoIterate_ForLoad(pCurrDOMNode, objUnitEx);

        std::string strUnitRef = objUnitEx.m_omID;
        m_mapUnitRefToUnit[strUnitRef] = objUnitEx;
    }

    return hResult;
}


HRESULT CPARSER_FIBEX::Process_Load(MSXML2::IXMLDOMNodePtr pCurrDOMNode, UNIT_EX& objUnit)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();

    BSTR bstr = pCurrDOMNode->GetnodeName().GetBSTR();
    std::string omNodeName = _com_util::ConvertBSTRToString(bstr);
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);
    if(omModNodeName == UNIT)
    {
        Process_Load(pCurrDOMNode, objUnit);
    }
    else if(omModNodeName == NAME_ELEMENT_SHORT)
    {
        if(pNodeParent != nullptr)
        {
            BSTR bstr = pCurrDOMNode->GetnodeName().GetBSTR();
            std::string strNode =_com_util::ConvertBSTRToString(bstr);
            CNodeUtil::bRemoveTag(strNode, strNode);
            if(strNode == UNIT)
            {
                std::string strUnitName  = (_com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()));

                objUnit.m_omUnitName = strUnitName;
            }
        }
    }
    else if(omModNodeName == U_DISP_NAME)
    {
        std::string strDispName  = (_com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()));

        objUnit.m_omDisplayName = strDispName;
    }
    else if(omModNodeName == U_PHYS_DIM_REF)
    {
        std::string strId = "";
        CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID-REF"));
        objUnit.m_omPHYSICAL_DIMENSION_REF  = ( strId );
    }

    pNodeParent.Release();
    return hResult;
}


HRESULT CPARSER_FIBEX::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, UNIT_EX& objUnit)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild, objUnit);

        pCurrChild.Release();
    }
    pChildList.Release();
    return hResult;
}

void CPARSER_FIBEX::CollectClusterinfov2(MSXML2::IXMLDOMNodePtr pIDomNode)
{
    std::string strTemp;
    long lTotal = 0;
    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;

    pIDomNode->get_childNodes(&pChildList);
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        BSTR bstr = pCurrChild->GetnodeName().GetBSTR();
        std::string omNodeName = _com_util::ConvertBSTRToString(bstr);

        //MSXML2::IXMLDOMNodeListPtr pShortName = pCurrChild->selectNodes(_bstr_t("//fx:ELEMENT-REVISIONS"));
        std::string omModNodeName = "";
        CNodeUtil::bRemoveTag(omNodeName, omModNodeName);
        if (omModNodeName == CLUSTER_NODE)
        {
            CClusterv2 objCluster;
            std::string strId = "";
            bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrChild, strId , _bstr_t("ID"));

            if(bValidFrameId == true)
            {
                objCluster.m_omClusterId = strId;
            }

            DoIterate_ForLoad(pCurrChild, objCluster);

            if(objCluster.m_bFlexray == TRUE)
            {
                m_mapClusterDetails[objCluster.m_omClusterId] = objCluster;
            }
        }

        pCurrChild.Release();
    }

    pChildList.Release();
}

void CPARSER_FIBEX::CollectECUInfov2(MSXML2::IXMLDOMNodePtr pIDomNode)
{
    std::string strTemp;
    long lTotal = 0;
    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;

    pIDomNode->get_childNodes(&pChildList);
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());

        std::string omModNodeName = "";
        CNodeUtil::bRemoveTag(omNodeName, omModNodeName);
        if (omModNodeName == ECU_NODE)
        {
            CECU objECU;
            std::string strId = "";
            bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrChild, strId , _bstr_t("ID"));

            CString strTemp = strId.c_str();

            if(bValidFrameId == true)
            {
                objECU.m_omECUId = strId;
            }

            DoIterate_ForLoad(pCurrChild, objECU);

            m_mapECUDetails[objECU.m_omECUId] = objECU;

            std::map<std::string, std::string> mapECUs;

            std::map<std::string, std::map<std::string, std::string>>::iterator itrChnlRef = m_mapChnlToECUDetails.begin();

            if(itrChnlRef != m_mapChnlToECUDetails.end())
            {
                mapECUs = itrChnlRef->second;
            }

            mapECUs[objECU.m_omECUId] = objECU.m_omECUId;
            m_mapChnlToECUDetails[objECU.objConnector.m_omChnlRef] = mapECUs;
        }

        pCurrChild.Release();
    }
    pChildList.Release();
}


void CPARSER_FIBEX::CollectFrameTrigInfov2(MSXML2::IXMLDOMNodePtr pIDomNode)
{
    std::string strTemp;
    long lTotal = 0;
    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;

    pIDomNode->get_childNodes(&pChildList);
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());

        std::string omModNodeName = "";
        CNodeUtil::bRemoveTag(omNodeName, omModNodeName);
        if (omModNodeName == CHANNEL_NODE)
        {
            //CFrame objFrame;
            std::string strId = "";
            bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrChild, strId , _bstr_t("ID"));

            /*if(bValidFrameId == true)
            {
            objFrame.m_omFrameId = strId;
            }*/

            std::string strChnlRef = strId;
            /*map<string, map<string,string>>::iterator itrChannel = m_mapChnlToECUDetails.find(strChnlRef);

            if(itrChannel != m_mapChnlToECUDetails.end())*/
            {
                CFrameTrig objFrameTrig;

                DoIterate_ForLoad(pCurrChild, objFrameTrig, strChnlRef);
            }

            //m_mapFrameDetails[objFrame.m_omFrameId] = objFrame;
        }

        pCurrChild.Release();
    }

    pChildList.Release();
}


HRESULT CPARSER_FIBEX::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CFrameTrig& objFrameTrig, std::string strChannelRef)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    MSXML2::IXMLDOMNodePtr pCurrChild;
    for (int i = 0; i < lTotal; i++)
    {
        pCurrChild = pChildList->Getitem(i);
        std::string omModNodeName = "";
        omModNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
        hResult = Process_Load(pCurrChild, objFrameTrig, strChannelRef);

        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT CPARSER_FIBEX::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CFrameTrig& objFrameTrig, std::string strChannelRef)
{
    HRESULT hResult = S_OK;
    std::string strTemp;
    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrDOMNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if(omModNodeName == FRAME_TRIGGERINGS_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objFrameTrig, strChannelRef);
    }
    else if(omModNodeName == FLEXRAY_CHANNEL_NAME_NODE)
    {
        std::string strChannelName = ( _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()) );
        m_mapChannelRefToChnlName[strChannelRef] = strChannelName;
    }
    else if(omModNodeName == FRAME_TRIGGERING_NODE)
    {
        std::string strId = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID"));

        std::string strFrameTrigId = strId;
        objFrameTrig.m_omFramTrigId = strId;
        DoIterate_ForLoad(pCurrDOMNode, objFrameTrig, strChannelRef);
    }
    else if(omModNodeName == ABSOLUTELY_SCHLD_TIMING_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objFrameTrig, strChannelRef);
    }
    else if(omModNodeName == TIMINGS_NODE)
    {
        objFrameTrig.mapAbsSchdTiming.clear();
        DoIterate_ForLoad(pCurrDOMNode, objFrameTrig, strChannelRef);
    }
    else if (omModNodeName == FRAME_REF_NODE)
    {
        std::string strId = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID-REF"));
        objFrameTrig.m_omFrameRef  = ( strId );

        m_mapFramTrigIdToFrame[objFrameTrig.m_omFramTrigId] = objFrameTrig;
    }
    else if( omModNodeName == SUB_FRAME_REF_NODE)
    {
        std::string strId = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID-REF"));
        objFrameTrig.m_omFrameRef  = ( strId );

        m_mapFramTrigIdToFrame[objFrameTrig.m_omFramTrigId] = objFrameTrig;
    }
    else if (omModNodeName == SLOT_ID_NODE)
    {
        int nSlotId = atoi( _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()) );
        objFrameTrig.m_nCurrentSlot = nSlotId;
        objFrameTrig.mapAbsSchdTiming[nSlotId].m_nSlotId = nSlotId;
    }
    else if (omModNodeName == BASE_CYCLE_NODE)
    {
        objFrameTrig.mapAbsSchdTiming[objFrameTrig.m_nCurrentSlot].m_nBaseCycle = atoi( _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()) );
    }
    else if (omModNodeName == CYCLE_REPETITION_NODE)
    {
        objFrameTrig.mapAbsSchdTiming[objFrameTrig.m_nCurrentSlot].m_nCycleRepetition = atoi( _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()) );
    }

    return hResult;
}

void CPARSER_FIBEX::CollectFrameInfov2(MSXML2::IXMLDOMNodePtr pIDomNode)
{
    std::string strTemp;
    long lTotal = 0;
    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;

    pIDomNode->get_childNodes(&pChildList);
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());

        std::string omModNodeName = "";
        CNodeUtil::bRemoveTag(omNodeName, omModNodeName);
        if (omModNodeName == FRAME_NODE)
        {
            CFrame objFrame;
            std::string strId = "";
            bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrChild, strId , _bstr_t("ID"));

            if(bValidFrameId == true)
            {
                objFrame.m_omFrameId = strId;
            }

            if(m_strFIBEXVersion == VERSION_2)
            {
                CPdu_Instance objPdu;
                objPdu.m_strPduId = PDU_DUMMY_2_0;
                objPdu.m_nStartBit = 0;
                objFrame.m_mapPduInstList[objPdu.m_strPduId] = objPdu;
            }

            DoIterate_ForLoad(pCurrChild, objFrame);

            if(m_strFIBEXVersion == VERSION_2)
            {
                CPdu_Instance& objPdu = objFrame.m_mapPduInstList[PDU_DUMMY_2_0];

                objPdu.m_mapSigInstDetails = objFrame.m_mapSigInstDetails;
            }

            m_mapFrameDetails[objFrame.m_omFrameId] = objFrame;
        }
        if(omModNodeName == PDU_NODE)
        {
            CPdu objPdu;
            std::string strId = "";
            bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrChild, strId , _bstr_t("ID"));

            if(bValidFrameId == true)
            {
                objPdu.m_strPduId = strId;
            }

            DoIterate_ForLoad(pCurrChild, objPdu);

            m_mapPduDetails[objPdu.m_strPduId] = objPdu;
        }

        pCurrChild.Release();
    }

    pChildList.Release();
}


HRESULT CPARSER_FIBEX::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CClusterv2& objCluster)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    MSXML2::IXMLDOMNodePtr pCurrChild;
    for (int i = 0; i < lTotal; i++)
    {
        pCurrChild = pChildList->Getitem(i);
        std::string omModNodeName = "";
        omModNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
        hResult = Process_Load(pCurrChild, objCluster);

        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}



HRESULT CPARSER_FIBEX::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CClusterv2& objCluster)
{
    HRESULT hResult = S_FALSE;
    std::string strTemp;
    char* stopstring;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrDOMNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if(omModNodeName == SHORT_NAME_NODE)
    {
        std::string strShortName = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();
        if(pNodeParent != nullptr)
        {
            std::string strNode = _com_util::ConvertBSTRToString(pNodeParent->GetnodeName().GetBSTR());
            CNodeUtil::bRemoveTag(strNode, strNode);
            if(strNode == CLUSTER_NODE)
            {
                objCluster.m_omFlxClusterName = strShortName;
            }
        }
        pNodeParent.Release();
    }
    if (omModNodeName == PROTOCOL_NODE)
    {
        std::string strProtocol = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        CNodeUtil::bReadAttribute(pCurrDOMNode, strTemp , _bstr_t("xsi:type"));

        objCluster.m_omProjectType = (strTemp.c_str());
        std::string strTemp = strProtocol.c_str();

        if(strTemp.compare("FlexRay") == 0)
        {
            objCluster.m_bFlexray = TRUE;
        }
    }
    else if(omModNodeName == CHANNEL_REFS_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objCluster);
    }
    else if(omModNodeName == CHANNEL_REF_NODE)
    {
        CNodeUtil::bReadAttribute(pCurrDOMNode, strTemp , _bstr_t("ID-REF"));
        //objCluster.m_omChannelRef = strTemp;

        std::string strChnlRef = strTemp;
        objCluster.m_mapChnls[strChnlRef] = strChnlRef;
    }
    else if (omModNodeName == COLD_START_ATTEMPTS_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shCOLD_START_ATTEMPTS = atoi(strTemp.c_str());
    }
    else if (omModNodeName == ACTION_POINT_OFFSET_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shACTION_POINT_OFFSET = atoi(strTemp.c_str());
    }
    else if (omModNodeName == DYNAMIC_SLOT_IDLE_PHASE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shDYNAMIC_SLOT_IDLE_PHASE = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MINISLOT_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shMINISLOT = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MINISLOT_ACTION_PT_OFFSET_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shMINISLOT_ACTION_POINT_OFFSET = atoi(strTemp.c_str());
    }
    else if (omModNodeName == N_I_T_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shNIT = atoi(strTemp.c_str());
    }
    else if (omModNodeName == SAMPLE_CLOCK_PERIOD_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_fSAMPLE_CLOCK_PERIOD = atol(strTemp.c_str());
    }
    else if (omModNodeName == STATIC_SLOT_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shSTATIC_SLOT = atoi(strTemp.c_str());
    }
    else if (omModNodeName == SYMBOL_WINDOW_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shSYMBOL_WINDOW = atoi(strTemp.c_str());
    }
    else if (omModNodeName == T_S_S_TRANSMITTER_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shTSS_TRANSMITTER = atoi(strTemp.c_str());
    }
    else if (omModNodeName == WAKE_UP_NODE)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode, objCluster);
    }
    else if( omModNodeName == WAS_RX_IDLE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_RX_IDLE = atoi(strTemp.c_str());
    }
    else if( omModNodeName == WAS_RX_LOW_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_RX_LOW = atoi(strTemp.c_str());
    }
    else if( omModNodeName == WAS_RX_WINDOW_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_RX_WINDOW = atoi(strTemp.c_str());
    }
    else if( omModNodeName == WAS_TX_IDLE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_TX_IDLE = atoi(strTemp.c_str());
    }
    else if( omModNodeName == WAS_TX_LOW_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_ouWAKEUP.m_shWAKE_UP_SYMBOL_TX_LOW = atoi(strTemp.c_str());
    }
    else if (omModNodeName == LISTEN_NOISE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shLISTEN_NOISE = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MACRO_PER_CYCLE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shMACRO_PER_CYCLE = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MACROTICK_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_fMACROTICK = strtol(strTemp.c_str(), &stopstring, 10);
    }
    else if (omModNodeName == MAX_INITZN_ERROR_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_fMAX_INITIALIZATION_ERROR = strtol(strTemp.c_str(), &stopstring, 10);
    }
    else if (omModNodeName == MAX_WITOUT_CLK_CORR_FATAL_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shMAX_WITHOUT_CLOCK_CORRECTION_FATAL = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MAX_WITOUT_CLK_CORR_PASSIVE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shMAX_WITHOUT_CLOCK_CORRECTION_PASSIVE = atoi(strTemp.c_str());
    }
    else if (omModNodeName == NET_MGNT_VECT_LEGTH_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shNETWORK_MANAGEMENT_VECTOR_LENGTH = atoi(strTemp.c_str());
    }
    else if (omModNodeName == NUMBER_OF_MINISLOTS_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shNUMBER_OF_MINISLOTS = atoi(strTemp.c_str());
    }
    else if (omModNodeName == NUMBER_OF_STATIC_SLOTS_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shNUMBER_OF_STATIC_SLOTS = atoi(strTemp.c_str());
    }
    else if (omModNodeName == OFFSET_CORRECTION_START_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shOFFSET_CORRECTION_START = atoi(strTemp.c_str());
    }
    else if (omModNodeName == PAYLOAD_LENGTH_STATIC_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shPAYLOAD_LENGTH_STATIC = atoi(strTemp.c_str());
    }
    else if (omModNodeName == SYNC_NODE_MAX_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shSYNC_NODE_MAX = atoi(strTemp.c_str());
    }
    else if (omModNodeName == CAS_RX_LOW_MAX_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shCAS_RX_LOW_MAX = atoi(strTemp.c_str());
    }
    else if (omModNodeName == BIT_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_fBIT = strtol(strTemp.c_str(), &stopstring, 10);
    }
    else if (omModNodeName == CYCLE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shCYCLE = atoi(strTemp.c_str());
    }
    else if (omModNodeName == CLUSTER_DRIFT_DAMPING_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objCluster.m_absFlxCluster.m_shCLUSTER_DRIFT_DAMPING = atoi(strTemp.c_str());
    }

    return hResult;
}

HRESULT CPARSER_FIBEX::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CECU& objECU)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    MSXML2::IXMLDOMNodePtr pCurrChild;
    for (int i = 0; i < lTotal; i++)
    {
        pCurrChild = pChildList->Getitem(i);
        std::string omModNodeName = "";
        omModNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
        hResult = Process_Load(pCurrChild, objECU);
        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT CPARSER_FIBEX::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CECU& objECU)
{
    HRESULT hResult = S_OK;
    std::string strTemp;
    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrDOMNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();

    if (omModNodeName == SHORT_NAME_NODE  )
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        if(pNodeParent != nullptr)
        {
            std::string strNode = _com_util::ConvertBSTRToString(pNodeParent->GetnodeName().GetBSTR());
            CNodeUtil::bRemoveTag(strNode, strNode);
            if(strNode == ECU_NODE)
            {
                objECU.m_omShortName = strTemp;

                if(objECU.m_omShortName == "ZGW")
                {
                    objECU.m_omShortName = objECU.m_omShortName;
                }
                objECU.m_strCurrConnectorRef = "";
                objECU.m_strCurrControllerRef = "";
            }
        }
    }
    if(omModNodeName == CONTROLLERS_NODE)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
    }
    if(omModNodeName == CONTROLLER_NODE)
    {
        std::string strId;
        CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID"));
        objECU.m_strCurrControllerRef = strId;
        objECU.objController.clear();
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);

        std::string strCtrlId = strId;
        objECU.m_mapController[objECU.m_strCurrControllerRef] = objECU.objController;
    }
    else if(omModNodeName == CONTROLLERS_NODE)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
    }
    /*else if(omModNodeName == CONTROLLER_NODE)
    {
    string strId;
    CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID"));
    objECU.m_strCurrControllerRef = strId;
    objECU.objController.clear();
    hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);

    string strCtrlId = strId;
    objECU.m_mapController[strCtrlId] = objECU.objController;
    }*/
    else if(omModNodeName == CONNECTORS_NODE)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
    }
    else if(omModNodeName == CONNECTOR_NODE)
    {
        std::string strId;
        CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID"));
        objECU.m_strCurrConnectorRef = strId;
        objECU.objConnector.clear();
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
        std::string strConnId = strId;
        objECU.m_mapConnector[objECU.m_strCurrConnectorRef] = objECU.objConnector;
    }
    else if(omModNodeName == INPUTS_NODE)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
    }
    else if(omModNodeName == INPUT_PORT)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
    }
    else if(omModNodeName == OUTPUTS_NODE)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
    }
    else if(omModNodeName == OUTPUT_PORT)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
    }
    else if(omModNodeName == FRAME_TRIGGERING_REF_NODE)
    {
        MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();
        std::string strRef = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strRef , _bstr_t("ID-REF"));
        CFrameTriggering objFrameTrig;
        objECU.objConnector.m_omCurrFrameTrigId = strRef;
        objFrameTrig.m_omFrameRef = strRef;

        std::string strReference = strRef;

        if(pNodeParent != nullptr)
        {
            std::string strNode = _com_util::ConvertBSTRToString(pNodeParent->GetnodeName().GetBSTR());
            CNodeUtil::bRemoveTag(strNode, strNode);
            if(strNode == INPUT_PORT)
            {
                if(objFrameTrig.m_omFrameRef != "")
                {
                    if(objFrameTrig.m_omFrameRef == ("idExpansion4bus1460flexraycluster620frame7799component13182") )
                    {
                        objFrameTrig.m_omFrameRef = objFrameTrig.m_omFrameRef;
                    }
                    objECU.objConnector.m_mapRxFrameTrigs[strReference] = objFrameTrig;
                }
            }
            else if(strNode == OUTPUT_PORT)
            {
                if(objFrameTrig.m_omFrameRef != "")
                {
                    objECU.objConnector.m_mapTxFrameTrigs[strReference] = objFrameTrig;
                }
            }
        }

        pNodeParent.Release();
    }
    else if(omModNodeName == SIGNAL_INSTANCE_REFS)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
    }
    else if(omModNodeName == SIGNAL_INSTANCE_REF_NODE)
    {
        std::string strRef = "";
        CNodeUtil::bReadAttribute(pCurrDOMNode, strRef , _bstr_t("ID-REF"));

        if(objECU.objConnector.m_mapRxFrameTrigs.find(objECU.objConnector.m_omCurrFrameTrigId) != objECU.objConnector.m_mapRxFrameTrigs.end())
        {
            CFrameTriggering& objFrameTrig = objECU.objConnector.m_mapRxFrameTrigs[objECU.objConnector.m_omCurrFrameTrigId];
            std::string strReference = strRef;
            objFrameTrig.m_mapSigInst[strReference] = strRef;
        }
        else if(objECU.objConnector.m_mapTxFrameTrigs.find(objECU.objConnector.m_omCurrFrameTrigId) != objECU.objConnector.m_mapTxFrameTrigs.end())
        {
            CFrameTriggering& objFrameTrig = objECU.objConnector.m_mapTxFrameTrigs[objECU.objConnector.m_omCurrFrameTrigId];
            std::string strReference = strRef;
            objFrameTrig.m_mapSigInst[strReference] = strRef;
        }
    }
    else if(omModNodeName == CHANNEL_REF_NODE)
    {
        std::string strRef = "";
        CNodeUtil::bReadAttribute(pCurrDOMNode, strRef , _bstr_t("ID-REF"));
        //string strChnlRef = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objConnector.m_omChnlRef = strRef;
        std::map<std::string, std::string> lstChnlRefs;

        if(m_ECUIdToChnlRefs.find(objECU.m_omECUId) != m_ECUIdToChnlRefs.end())
        {
            lstChnlRefs = (m_ECUIdToChnlRefs[objECU.m_omECUId]);
        }
        lstChnlRefs[objECU.objConnector.m_omChnlRef] = objECU.objConnector.m_omChnlRef;
        m_ECUIdToChnlRefs[objECU.m_omECUId] = lstChnlRefs;
    }
    else if(omModNodeName == CONTROLLER_REF_NODE)
    {
        //string strCntrlRef = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        std::string strRef = "";
        CNodeUtil::bReadAttribute(pCurrDOMNode, strRef , _bstr_t("ID-REF"));
        objECU.objConnector.m_omCntrlRef = strRef;
    }
    else if (omModNodeName == KEY_SLOT_USAGE_NODE)
    {
        objECU.objController.m_sKeySlotUsage.m_nStartUpSync = 0;
        objECU.objController.m_sKeySlotUsage.m_nSync = 0;
        objECU.objController.m_sKeySlotUsage.m_omNone = "";
        hResult = DoIterate_ForLoad(pCurrDOMNode, objECU);
    }
    else if (omModNodeName == STARTUP_SYNC_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_sKeySlotUsage.m_nStartUpSync = atoi(strTemp.c_str());
    }
    else if (omModNodeName == SYNC_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_sKeySlotUsage.m_nSync = atoi(strTemp.c_str());
    }
    else if (omModNodeName == NONE_NODE)
    {
        objECU.objController.m_sKeySlotUsage.m_omNone = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
    }
    else if (omModNodeName == MAX_DYNAMIC_PAYLOAD_LENGTH_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shMaxDynPayloadLgt = atoi(strTemp.c_str());
    }
    else if (omModNodeName == CLUSTER_DRIFT_DAMPING_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shClusterDriftDamping = atoi(strTemp.c_str());
    }
    else if (omModNodeName == DECODING_CORRECTION_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_nDecodingCorr = atoi(strTemp.c_str());
    }
    else if (omModNodeName == LISTENT_TIMEOUT_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_nListenTimeOut = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MAX_DRIFT_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shMaxDrift = atoi(strTemp.c_str());
    }
    else if (omModNodeName == EXTERN_OFFSET_CORRECTION_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shExternOffsetCorr = atoi(strTemp.c_str());
    }
    else if (omModNodeName == EXTERN_RATE_CORRECTION_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shExternRateCorr = atoi(strTemp.c_str());
    }
    else if (omModNodeName == LATEST_TX_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shLatestTx = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MICRO_PER_CYCLE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_nMicroPreCycle = atoi(strTemp.c_str());
    }
    else if (omModNodeName == OFFSET_CORRECTION_OUT_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shOffsetCorrOut = atoi(strTemp.c_str());
    }
    else if (omModNodeName == RATE_CORRECTION_OUT_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shRateCorrOut = atoi(strTemp.c_str());
    }
    else if (omModNodeName == SAMPLES_PER_MICROTICK_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shSamplePerMicrotick = atoi(strTemp.c_str());
    }
    else if (omModNodeName == DELAY_COMPENSATION_A_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shDelayCompensationA = atoi(strTemp.c_str());
    }
    else if (omModNodeName == DELAY_COMPENSATION_B_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shDelayCompensationB = atoi(strTemp.c_str());
    }
    else if (omModNodeName == WAKE_UP_PATTERN_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shWakeUpPattern = atoi(strTemp.c_str());
    }
    else if (omModNodeName == ALLOW_HALT_DUE_TO_CLOCK_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        std::string strTempTemp = strTemp.c_str();
        if ( !strTempTemp.compare("true"))
        {
            objECU.objController.m_bAllowHaltDewToClock = true;
        }
        else
        {
            objECU.objController.m_bAllowHaltDewToClock = false;
        }
    }
    else if (omModNodeName == ALLOW_PASSIVE_TO_ACTIVE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shAllowPassiveToActive = atoi(strTemp.c_str());
    }
    else if (omModNodeName == ACCEPTED_STARTUP_RANGE_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shAcceptedStartUpRange = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MACRO_INITIAL_OFFSET_A_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shMacroInitialOffsetA = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MACRO_INITIAL_OFFSET_B_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shMacroInitialOffsetB = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MICRO_INITIAL_OFFSET_A_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shMicroInitialOffsetA = atoi(strTemp.c_str());
    }
    else if (omModNodeName == MICRO_INITIAL_OFFSET_B_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_shMicroInitialOffsetB = atoi(strTemp.c_str());
    }
    else if (omModNodeName == SINGLE_SLOT_ENABLED_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        std::string strTempTemp = strTemp.c_str();
        if ( !strTempTemp.compare("true"))
        {
            objECU.objController.m_bSingleSlotEnable = true;
        }
        else
        {
            objECU.objController.m_bSingleSlotEnable = false;
        }
    }
    else if (omModNodeName == MICROTICK_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController.m_fMicrotick = atof(strTemp.c_str());
    }
    else if (omModNodeName == MICRO_PER_MACRO_NOM_NODE)
    {
        strTemp = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objECU.objController. m_fMicroPerMacroNom = atof(strTemp.c_str());
    }

    pNodeParent.Release();
    return hResult;
}

void CPARSER_FIBEX::CollectSignalInfov2(MSXML2::IXMLDOMNodePtr pIDomNode)
{
    std::string strTemp;
    long lTotal = 0;
    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;

    pIDomNode->get_childNodes(&pChildList);
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());

        std::string omModNodeName = "";
        CNodeUtil::bRemoveTag(omNodeName, omModNodeName);
        if (omModNodeName == SIGNAL_NODE)
        {
            CSignal objSignal;
            std::string strId = "";
            bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrChild, strId , _bstr_t("ID"));

            if(bValidFrameId == true)
            {
                objSignal.m_omSigId = strId;
            }

            DoIterate_ForLoad(pCurrChild, objSignal);

            m_mapSignalDetails[objSignal.m_omSigId] = objSignal;

            //CSignal ouSignal;
            //hResult = ouSignal.Load(pCurrChild);
            //if (hResult == S_OK)
            //{
            //    //m_odSignalList.AddTail(ouSignal);
            //}
        }
        pCurrChild.Release();
    }
    pChildList.Release();
}

HRESULT CPARSER_FIBEX::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CFrame& objFrame)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    MSXML2::IXMLDOMNodePtr pCurrChild;
    for (int i = 0; i < lTotal; i++)
    {
        pCurrChild = pChildList->Getitem(i);
        std::string omModNodeName = "";
        omModNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
        hResult = Process_Load(pCurrChild, objFrame);

        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT CPARSER_FIBEX::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CPdu& objPdu)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    MSXML2::IXMLDOMNodePtr pCurrChild;
    for (int i = 0; i < lTotal; i++)
    {
        pCurrChild = pChildList->Getitem(i);
        std::string omModNodeName = "";
        omModNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
        hResult = Process_Load(pCurrChild, objPdu);

        pCurrChild.Release();
    }

    pChildList.Release();
    return hResult;
}

HRESULT CPARSER_FIBEX::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent, CSignal& objSignal)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    MSXML2::IXMLDOMNodePtr pCurrChild;
    for (int i = 0; i < lTotal; i++)
    {
        pCurrChild = pChildList->Getitem(i);
        std::string omModNodeName = "";
        omModNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
        hResult = Process_Load(pCurrChild, objSignal);

        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}


HRESULT CPARSER_FIBEX::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CPdu& objPdu)
{
    HRESULT hResult = S_OK;
    std::string strTemp;
    //char* stopstring;
    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrDOMNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    std::string strNode = "";
    MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();
    if(pNodeParent != nullptr)
    {
        strNode = _com_util::ConvertBSTRToString(pNodeParent->GetnodeName().GetBSTR());
        CNodeUtil::bRemoveTag(strNode, strNode);
    }

    if(omModNodeName == SHORT_NAME_NODE)
    {
        std::string strShortName  = (_com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()));
        if(strNode == PDU_NODE)
        {
            objPdu.m_ouMuxSwitch.m_nSwitchBitLen = 0;
            objPdu.m_strPDUName = strShortName;
        }
        else if(strNode == SWITCH_NODE)
        {
            objPdu.m_ouMuxSwitch.m_omSwitchName = strShortName;
        }
        else if(strNode == DYNAMIC_PART_NODE)
        {
            objPdu.m_ouDynPart.m_ouDynPartName = strShortName;
        }
        else if(strNode == STATIC_PART_NODE)
        {
            objPdu.m_ouStaticPart.m_ouStaticPartName = strShortName;
        }
    }
    else if(omModNodeName == BYTE_LENGTH_NODE)
    {
        std::string strByteLen  = (_com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()));

        objPdu.m_nLength =  atoi(strByteLen.c_str());
    }
    else if(omModNodeName == BIT_LENGTH_NODE)
    {
        std::string strBitLen  = (_com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()));

        if(strNode == SEGMENT_POSITION_NODE)
        {
            if(objPdu.m_bIsStaticPart == FALSE)
            {
                objPdu.m_ouDynPart.m_nDynPartBitLen =  atoi(strBitLen.c_str());
            }
            else if(objPdu.m_bIsStaticPart == TRUE)
            {
                objPdu.m_ouStaticPart.m_nStPartBitLen = atoi(strBitLen.c_str());
            }
        }
        else if(strNode == SWITCH_NODE)
        {
            objPdu.m_ouMuxSwitch.m_nSwitchBitLen = atoi(strBitLen.c_str());
        }
    }
    else if(omModNodeName == PDU_TYPE_NODE)
    {
        std::string strPduType  = (_com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()));

        objPdu.m_strPduType =  strPduType;
    }
    else if(omModNodeName == SIGNAL_INSTANCES_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == SIGNAL_INSTANCE_NODE)
    {
        std::string strId = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID"));
        CSigInstance objSignalInst;
        if(bValidFrameId == true)
        {
            objSignalInst.m_omSigInstId = strId;

            objPdu.m_omCurrSigInstId = strId;

            objPdu.m_mapSigInstDetails[objSignalInst.m_omSigInstId] = objSignalInst;

            DoIterate_ForLoad(pCurrDOMNode, objPdu);
        }
    }
    else if(omModNodeName == BIT_POSITION_NODE)
    {
        std::string strBitPos = "";
        strBitPos  = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());

        if(strNode == SIGNAL_INSTANCE_NODE)
        {
            CSigInstance& objSigInst = objPdu.m_mapSigInstDetails[objPdu.m_omCurrSigInstId];
            objSigInst.m_unBitPos = atoi(strBitPos.c_str());
        }
        else if(strNode == SWITCH_NODE)
        {
            objPdu.m_ouMuxSwitch.m_nSwitchBitPos = atoi( strBitPos.c_str() );
        }
        else if(strNode == SEGMENT_POSITION_NODE)
        {
            if(objPdu.m_bIsStaticPart == FALSE)
            {
                objPdu.m_ouDynPart.m_nDynPartBitPos = atoi( strBitPos.c_str() );
            }
            else if(objPdu.m_bIsStaticPart == TRUE)
            {
                objPdu.m_ouStaticPart.m_StaticPartBitPos = atoi( strBitPos.c_str() );
            }
        }
        else if(strNode == DATA_NODE)
        {
            if(objPdu.m_bIsStaticPart == FALSE)
            {
                objPdu.m_ouDynPart.m_nDynPartBitPos = atoi( strBitPos.c_str() );
            }
        }
    }
    else if(omModNodeName == IS_HIGH_LOW_BYTE_ORDER_NODE)
    {
        std::string strBitOrder = "";
        strBitOrder  = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());

        if(strNode == SIGNAL_INSTANCE_NODE)
        {
            CSigInstance& objSigInst = objPdu.m_mapSigInstDetails[objPdu.m_omCurrSigInstId];
            if((strBitOrder) == "true")
            {
                objSigInst.m_bByteOrder = 1;
            }
            else if(strBitOrder == "false")
            {
                objSigInst.m_bByteOrder = 0;
            }
        }
        else if(strNode == SWITCH_NODE)
        {
            if(strBitOrder == "false")
            {
                objPdu.m_ouMuxSwitch.m_ouSwitchByteOrder = INTEL_ENUM;
            }
            else if(strBitOrder == "true")
            {
                objPdu.m_ouMuxSwitch.m_ouSwitchByteOrder = MOTOROLA_ENUM;
            }
        }
        else if(strNode == SEGMENT_POSITION_NODE)
        {
            if(objPdu.m_bIsStaticPart == FALSE)
            {
                if(strBitOrder == "false")
                {
                    objPdu.m_ouDynPart.m_ouDynPartByteOrder = INTEL_ENUM;
                }
                else if(strBitOrder == "true")
                {
                    objPdu.m_ouDynPart.m_ouDynPartByteOrder = MOTOROLA_ENUM;
                }
            }
            else if(objPdu.m_bIsStaticPart == TRUE)
            {
                if(strBitOrder == "false")
                {
                    objPdu.m_ouStaticPart.m_ouStPartByteOrder = INTEL_ENUM;
                }
                else if(strBitOrder == "true")
                {
                    objPdu.m_ouStaticPart.m_ouStPartByteOrder = MOTOROLA_ENUM;
                }
            }
        }
        else if(strNode == DATA_NODE)
        {
            if(objPdu.m_bIsStaticPart == FALSE)
            {
                if(strBitOrder == "false")
                {
                    objPdu.m_ouDynPart.m_ouDynPartByteOrder = INTEL_ENUM;
                }
                else if(strBitOrder == "true")
                {
                    objPdu.m_ouDynPart.m_ouDynPartByteOrder = MOTOROLA_ENUM;
                }
            }
        }
    }
    else if(omModNodeName == SIGNAL_REF_NODE)
    {
        std::string strRef = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strRef , _bstr_t("ID-REF"));

        CSigInstance& objSigInst = objPdu.m_mapSigInstDetails[objPdu.m_omCurrSigInstId];
        objSigInst.m_omSigRef = strRef;
    }
    else if(omModNodeName == MULTIPLEXERS_NODE)
    {
        objPdu.m_bIsStaticPart = FALSE;
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == MULTIPLEXER_NODE)
    {
        objPdu.m_bIsStaticPart = FALSE;
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == SWITCH_NODE)
    {
        objPdu.m_ouMuxSwitch.m_nSwitchBitLen = 0;
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == SUB_FRAMES_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == DATA_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == DYNAMIC_PART_NODE || omModNodeName == SUB_FRAME_NODE)
    {
        objPdu.m_bIsStaticPart = FALSE;
        std::string strRef = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strRef , _bstr_t("ID"));

        objPdu.m_ouDynPart.m_ouDynPartId = strRef;

        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == SEGMENT_POSITIONS_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == SEGMENT_POSITION_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == SWITCHED_PDU_INSTANCES_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == SWITCHED_PDU_INSTANCE_NODE)
    {
        objPdu.m_ouDynPart.m_ouCurrSwitchCode = "";
        objPdu.m_ouDynPart.m_ouCurrPduRef = "";
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == SWITCH_CODE_NODE)
    {
        std::string swCode = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objPdu.m_ouDynPart.m_ouCurrSwitchCode = swCode;
        if(strNode == SUB_FRAME_NODE)
        {
            objPdu.m_ouDynPart.m_mapSwitchToPdu[swCode] = objPdu.m_ouDynPart.m_ouDynPartId;
        }
        else
        {
            objPdu.m_ouDynPart.m_mapSwitchToPdu[swCode] = objPdu.m_ouDynPart.m_ouCurrPduRef;
        }
    }
    else if(omModNodeName == PDU_REF_NODE)
    {
        std::string strId = "";
        CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID-REF"));
        if(strNode == SWITCHED_PDU_INSTANCE_NODE)
        {
            objPdu.m_ouDynPart.m_ouCurrPduRef = strId;
        }
        else if(strNode == STATIC_PDU_INSTANCE_NODE)
        {
            objPdu.m_ouStaticPart.m_mapPduIdToPdu[strId] = strId;
        }
    }
    else if(omModNodeName == STATIC_PART_NODE)
    {
        std::string strId = "";
        CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID"));
        objPdu.m_bIsStaticPart = TRUE;
        objPdu.m_ouStaticPart.m_ouStaticPartId = strId;
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }
    else if(omModNodeName == STATIC_PDU_INSTANCE_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objPdu);
    }

    pNodeParent.Release();

    return hResult;
}

HRESULT CPARSER_FIBEX::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CFrame& objFrame)
{
    HRESULT hResult = S_OK;
    std::string strTemp;
    //char* stopstring;
    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrDOMNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    std::string strNode = "";
    MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();
    if(pNodeParent != nullptr)
    {
        strNode = _com_util::ConvertBSTRToString(pNodeParent->GetnodeName().GetBSTR());
        CNodeUtil::bRemoveTag(strNode, strNode);
    }

    if(omModNodeName == BYTE_LENGTH_NODE)
    {
        std::string strByteLen  = (_com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()));

        objFrame.m_unFrameLen =  atoi(strByteLen.c_str());
    }
    else if(omModNodeName == BIT_LENGTH_NODE)
    {
        std::string strBitLen  = (_com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR()));

        if(strNode == SWITCH_NODE)
        {
            objFrame.m_objPdu.m_ouMuxSwitch.m_nSwitchBitLen = atoi(strBitLen.c_str());
        }
        else if(strNode == DATA_NODE)
        {
            objFrame.m_objPdu.m_ouDynPart.m_nDynPartBitLen = atoi( strBitLen.c_str() );
        }
    }
    else if(omModNodeName == DATA_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objFrame);
    }
    else if (omModNodeName == SHORT_NAME_NODE)
    {
        std::string strShortName  = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());

        MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();
        if(pNodeParent != nullptr)
        {
            std::string strNode = _com_util::ConvertBSTRToString(pNodeParent->GetnodeName().GetBSTR());
            CNodeUtil::bRemoveTag(strNode, strNode);
            if(strNode == FRAME_NODE)
            {
                objFrame.m_omShortName = strShortName;
            }
            else if(strNode == SWITCH_NODE)
            {
                objFrame.m_objPdu.m_ouMuxSwitch.m_omSwitchName = strShortName;
            }
        }

        pNodeParent.Release();

    }
    else if (omModNodeName == FRAME_TYPE_NODE)
    {
        objFrame.m_omFrameType  = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
    }
    else if(omModNodeName == PDU_INSTANCES_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objFrame);
    }
    else if(omModNodeName == PDU_INSTANCE_NODE)
    {
        std::string strId = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID"));
        CPdu_Instance objPduInstnc;
        if(bValidFrameId == true)
        {
            objPduInstnc.m_strPduId = strId;
            objFrame.m_omCurrPduId = strId;

            objFrame.m_mapPduInstList[objFrame.m_omCurrPduId] = objPduInstnc;

            DoIterate_ForLoad(pCurrDOMNode, objFrame);
        }
    }
    else if(omModNodeName == PDU_REF_NODE)
    {
        std::string strId = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID-REF"));

        CPdu_Instance& objPduInst = objFrame.m_mapPduInstList[objFrame.m_omCurrPduId];

        objPduInst.m_omPduRef = strId;
    }
    else if(omModNodeName == SIGNAL_INSTANCES_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objFrame);
    }
    else if(omModNodeName == SIGNAL_INSTANCE_NODE)
    {
        std::string strId = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strId , _bstr_t("ID"));
        CSigInstance objSignalInst;
        if(bValidFrameId == true)
        {
            objSignalInst.m_omSigInstId = strId;

            objFrame.m_omCurrSigInstId = strId;

            objFrame.m_mapSigInstDetails[objSignalInst.m_omSigInstId] = objSignalInst;

            //objFrame.m_CurrSigInstPos = objFrame.m_lstSigInstance.AddTail(objSignalInst);

            DoIterate_ForLoad(pCurrDOMNode, objFrame);
        }
    }
    else if(omModNodeName == BIT_POSITION_NODE)
    {
        std::string strBitPos = "";
        strBitPos  = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());

        MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();

        if(pNodeParent != nullptr)
        {
            std::string strNode = _com_util::ConvertBSTRToString(pNodeParent->GetnodeName().GetBSTR());
            CNodeUtil::bRemoveTag(strNode, strNode);

            if(strNode == SIGNAL_INSTANCE_NODE)
            {
                CSigInstance& objSigInst = objFrame.m_mapSigInstDetails[objFrame.m_omCurrSigInstId];
                objSigInst.m_unBitPos = atoi(strBitPos.c_str());
            }
            else if(strNode == PDU_INSTANCE_NODE)
            {
                CPdu_Instance& objPduInstnce = objFrame.m_mapPduInstList[objFrame.m_omCurrPduId];

                objPduInstnce.m_nStartBit = atoi(strBitPos.c_str());
            }
            else if(strNode == SWITCH_NODE)
            {
                objFrame.m_objPdu.m_ouMuxSwitch.m_nSwitchBitPos = atoi( strBitPos.c_str() );
            }
            else if(strNode == DATA_NODE)
            {
                objFrame.m_objPdu.m_ouDynPart.m_nDynPartBitPos = atoi( strBitPos.c_str() );
            }
        }
        pNodeParent.Release();

    }
    else if(omModNodeName == IS_HIGH_LOW_BYTE_ORDER_NODE)
    {
        std::string strBitOrder = "";
        strBitOrder  = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());

        MSXML2::IXMLDOMNodePtr pNodeParent = pCurrDOMNode->GetparentNode();

        if(pNodeParent != nullptr)
        {
            std::string strNode = _com_util::ConvertBSTRToString(pNodeParent->GetnodeName().GetBSTR());
            CNodeUtil::bRemoveTag(strNode, strNode);

            if(strNode == SIGNAL_INSTANCE_NODE)
            {
                CSigInstance& objSigInst = objFrame.m_mapSigInstDetails[objFrame.m_omCurrSigInstId];
                if((strBitOrder) == "true")
                {
                    objSigInst.m_bByteOrder = 1;
                }
                else if(strBitOrder == "false")
                {
                    objSigInst.m_bByteOrder = 0;
                }
            }
            else if(strNode == PDU_INSTANCE_NODE)
            {
                CPdu_Instance& objPduInstnce = objFrame.m_mapPduInstList[objFrame.m_omCurrPduId];

                if(strBitOrder == "false")
                {
                    objPduInstnce.m_ouEndianess = INTEL_ENUM;
                }
                else if(strBitOrder == "true")
                {
                    objPduInstnce.m_ouEndianess = MOTOROLA_ENUM;
                }
            }
            else if(strNode == SWITCH_NODE)
            {
                if(strBitOrder == "false")
                {
                    objFrame.m_objPdu.m_ouMuxSwitch.m_ouSwitchByteOrder = INTEL_ENUM;
                }
                else if(strBitOrder == "true")
                {
                    objFrame.m_objPdu.m_ouMuxSwitch.m_ouSwitchByteOrder = MOTOROLA_ENUM;
                }
            }
            else if(strNode == DATA_NODE)
            {
                if(strBitOrder == "false")
                {
                    objFrame.m_objPdu.m_ouDynPart.m_ouDynPartByteOrder = INTEL_ENUM;
                }
                else if(strBitOrder == "true")
                {
                    objFrame.m_objPdu.m_ouDynPart.m_ouDynPartByteOrder = MOTOROLA_ENUM;
                }
            }
        }

        pNodeParent.Release();
    }
    else if(omModNodeName == SUB_FRAMES_NODE)
    {
        DoIterate_ForLoad(pCurrDOMNode, objFrame);
    }
    else if(omModNodeName == SUB_FRAME_NODE)
    {
        std::string strRef = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strRef , _bstr_t("ID"));

        objFrame.m_objPdu.m_ouDynPart.m_ouDynPartId = strRef;
        DoIterate_ForLoad(pCurrDOMNode, objFrame);
    }
    else if(omModNodeName == SWITCH_CODE_NODE)
    {
        std::string swCode = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        objFrame.m_objPdu.m_ouDynPart.m_ouCurrSwitchCode = swCode;

        objFrame.m_objPdu.m_ouDynPart.m_mapSwitchToPdu[swCode] = objFrame.m_objPdu.m_ouDynPart.m_ouDynPartId;
    }
    else if(omModNodeName == SIGNAL_REF_NODE)
    {
        std::string strRef = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strRef , _bstr_t("ID-REF"));

        CSigInstance& objSigInst = objFrame.m_mapSigInstDetails[objFrame.m_omCurrSigInstId];
        objSigInst.m_omSigRef = strRef;
    }
    else if(omModNodeName == SUB_FRAME_REF_NODE)
    {
        std::string strRef = "";
        bool bValidFrameId = CNodeUtil::bReadAttribute(pCurrDOMNode, strRef , _bstr_t("ID-REF"));

        m_SubFrameToFrame[strRef] = objFrame.m_omFrameId;

        if(strNode == SIGNAL_INSTANCE_NODE)
        {
            CSigInstance objSigInst = objFrame.m_mapSigInstDetails[objFrame.m_omCurrSigInstId];
            std::map<std::string, std::map<std::string, CSigInstance>>::iterator itrSubFramlst = m_mapSubFrameToSigInst.find(strRef);

            std::map<std::string, CSigInstance> mapSigInst;

            if(itrSubFramlst != m_mapSubFrameToSigInst.end())
            {
                mapSigInst = itrSubFramlst->second;
            }
            mapSigInst[objFrame.m_omCurrSigInstId] = objSigInst;
            m_mapSubFrameToSigInst[strRef] = mapSigInst;
        }
    }
    else if(omModNodeName == MULTIPLEXERS_NODE)
    {
        if(m_strFIBEXVersion == VERSION_2)
        {
            DoIterate_ForLoad(pCurrDOMNode, objFrame);
        }
    }
    else if(omModNodeName == MULTIPLEXER_NODE)
    {
        if(m_strFIBEXVersion == VERSION_2)
        {
            DoIterate_ForLoad(pCurrDOMNode, objFrame);
        }
    }
    else if(omModNodeName == SWITCH_NODE)
    {
        if(m_strFIBEXVersion == VERSION_2)
        {
            objFrame.m_objPdu.m_ouMuxSwitch.m_nSwitchBitLen = 0;
            DoIterate_ForLoad(pCurrDOMNode, objFrame);
        }
    }

    pNodeParent.Release();
    return hResult;
}


HRESULT CPARSER_FIBEX::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode, CSignal& objSignal)
{
    HRESULT hResult = S_OK;
    std::string strTemp;
    //char* stopstring;
    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrDOMNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if(omModNodeName == CODING_REF_NODE)
    {
        std::string strCoding = "";
        CNodeUtil::bReadAttribute(pCurrDOMNode, strCoding, _bstr_t("ID-REF"));
        objSignal.m_omCodingRef = strCoding;
    }
    else if (omModNodeName == SHORT_NAME_NODE)
    {
        objSignal.m_omShortName  = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
    }
    else if (omModNodeName == DESC_NODE)
    {
        std::string strLang = "";
        CNodeUtil::bReadAttribute(pCurrDOMNode, strLang, _bstr_t("xml:lang"));

        if(strLang == "de")
        {
            objSignal.m_omdeDesc  = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        }
        else if(strLang == "en")
        {
            objSignal.m_omenDesc  = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
        }
    }

    return hResult;
}

// To create the Fibex Entity
int CPARSER_FIBEX::ResetFibexEntity(eENTITY_FIBEX eEntity, PVOID pFibexEntity)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (sg_FunResetFibexEntity != nullptr)
    {
        (*sg_FunResetFibexEntity)(eEntity, pFibexEntity);
    }
    else
    {
        ASSERT(false);
    }
    return TRUE;
}

// To delete the Fibex Entity
int CPARSER_FIBEX::Remove(eENTITY_FIBEX /* eEntity */, std::string /* omID */)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return TRUE;
}

// Save contents into a Fibex file.
BOOL CPARSER_FIBEX::WriteFibexFile(std::string omFibexFilePath,
                                   PABS_FIBEX_CONTAINER pAbsFibexContainer)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (sg_FunWriteFibexFile != nullptr)
    {
        bTranslate(TYPE_CFIBEXCONTAINER, "",
                   (BYTE*) pAbsFibexContainer, FALSE);
        (*sg_FunWriteFibexFile)(omFibexFilePath);
    }
    else
    {
        ASSERT(false);
    }

    return TRUE;
}

// To convert Fibex specific data into the abstraction layer data and vice
// versa. 'bToAbstract' denotes the direction; TRUE means from Fibex specific
// data into the abstract data. Subject to the value of 'bToAbstract', Buffer
// will be interpreted.
int CPARSER_FIBEX::bTranslate(eENTITY_FIBEX eEntity, std::string omID, BYTE* Buffer, BOOL bToAbstract)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (sg_FunTranslate != nullptr)
    {
        int nCheck = (*sg_FunTranslate)(eEntity, omID, (BYTE*)Buffer, bToAbstract);
    }
    else
    {
        ASSERT(false);
    }

    return TRUE;
}

// Call this API to get the cluster information like, number of Flexray clusters, controllers are associated.
HRESULT CPARSER_FIBEX::hGetFLEXRAYClustersInfo(PABS_FIBEX_CONTAINER pAbsFibexContainer, AbsClusterInfoList& omAvailableClustersInfo)
{
    /* Validate the incoming FIBEX structure */
    if ( pAbsFibexContainer )
    {
        /* Retreive the cluster information */
        POSITION pos = pAbsFibexContainer->m_omElement.m_omClusterList.GetHeadPosition();
        int nIndex = 0;
        while (pos != nullptr)
        {
            ABS_CLUSTER& sClusterDetails = pAbsFibexContainer->m_omElement.m_omClusterList.GetNext(pos);
            /* Add the cluster information */
            ABS_CLUSTER_INFO sClusterInfo;

            /* Add Cluster ID */
            sClusterInfo.m_omClusterID =  sClusterDetails.m_omClusterID;

            /* retreive the associated controller IDs */
            UINT unChannelsCnt =  sClusterDetails.m_omChannelRefs.GetSize();
            for (UINT i = 0; i < unChannelsCnt ; i++)
            {
                std::string strChannelID = sClusterDetails.m_omChannelRefs.GetAt(i);

                /* get the controller with which this channel is associated, from ECUs, Connectors */

                POSITION posECU = pAbsFibexContainer->m_omElement.m_omECUList.GetHeadPosition();

                while (posECU != nullptr)
                {
                    ABS_ECU& sECUInfo = pAbsFibexContainer->m_omElement.m_omECUList.GetNext(posECU);

                    /* if channel Ref is found, add it to cluster info list and break */

                    AbsConnectorInfoList::iterator itr = sECUInfo.m_odConnectorInfo.begin();
                    while (itr != sECUInfo.m_odConnectorInfo.end())
                    {
                        if ( itr->m_omCHANNEL_REF == strChannelID.c_str() )
                        {
                            //sClusterInfo.m_omControllerIDs
                            sClusterInfo.m_omControllerIDs.push_back(itr->m_omCONTROLLER_REF);
                        }
                        itr++;
                    }
                }
            }
            /* Add the cluster information to list */
            omAvailableClustersInfo.push_back(sClusterInfo);
        }
    }
    return S_OK;
}

int CPARSER_FIBEX::UnloadLibrary(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    int Result = FCLASS_FAILURE;

    if (nullptr != sg_HMODULE)
    {
        FreeLibrary(sg_HMODULE);
        sg_HMODULE = nullptr;

        sg_FunLoadFibexFile     = nullptr;
        sg_FunWriteFibexFile    = nullptr;
        sg_FunTranslate         = nullptr;
        sg_FunResetFibexEntity  = nullptr;

        Result = FCLASS_SUCCESS;
    }
    return Result;
}

///////////////////////////////////////////////////////////
//  CCODING.cpp
//  Implementation of the Class CCODING
//  Created on:      23-Sep-2008 11:31:19 AM
///////////////////////////////////////////////////////////

//#include "FibexClass/FibexLib_stdafx.h"
//#include "CCODING.h"
//#include "NodeUtil.h"
#include "CFibexEntityData.h"
#include "..\DataTypes\FEALData.h"

namespace BASE_DATA_TYPE
{
CStringTokenList m_odUnitRef;
};
namespace CATEGORY1
{
CStringTokenList m_odUnitRef;
};
namespace CATEGORY2
{
CStringTokenList m_odUnitRef;
};
namespace CATEGORY3
{
CStringTokenList m_odUnitRef;
};
namespace ENCODING
{
CStringTokenList m_odUnitRef;
};
namespace TERMINATION
{
CStringTokenList m_odUnitRef;
};
namespace VALIDITY
{
CStringTokenList m_odUnitRef;
};
namespace INTERVAL_TYPE
{
CStringTokenList m_odUnitRef;
};

/* Starts implementation of SCOMPU_INTERNAL_TO_PHYS */
SCOMPU_INTERNAL_TO_PHYS_v2::SCOMPU_INTERNAL_TO_PHYS_v2()
{
}

SCOMPU_INTERNAL_TO_PHYS_v2::~SCOMPU_INTERNAL_TO_PHYS_v2()
{
    DoCleanup();
}
void SCOMPU_INTERNAL_TO_PHYS_v2::DoCleanup()
{
    m_odCompuScaleList.RemoveAll();
    m_sCompuDefValue.DoCleanup();
}

SCOMPU_INTERNAL_TO_PHYS_v2& SCOMPU_INTERNAL_TO_PHYS_v2::operator=(SCOMPU_INTERNAL_TO_PHYS_v2& RefObj)
{
    m_odCompuScaleList.RemoveAll();
    m_odCompuScaleList.AddTail(&RefObj.m_odCompuScaleList);
    m_sCompuDefValue   = RefObj.m_sCompuDefValue;

    return *this;
}


SCOMPU_INTERNAL_TO_PHYS_v2::SCOMPU_INTERNAL_TO_PHYS_v2(SCOMPU_INTERNAL_TO_PHYS_v2& RefObj)
{
    m_odCompuScaleList.RemoveAll();
    m_odCompuScaleList.AddTail(&RefObj.m_odCompuScaleList);
    m_sCompuDefValue   = RefObj.m_sCompuDefValue;
}

HRESULT SCOMPU_INTERNAL_TO_PHYS_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return DoIterate_ForLoad(pIDomNode);
}

HRESULT SCOMPU_INTERNAL_TO_PHYS_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild)
{
    HRESULT hResult = S_FALSE;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == COMPU_SCALES_NODE)
    {
        DoIterate_ForLoad(pCurrChild);
    }
    else if (omModNodeName == COMPU_SCALE_NODE)
    {
        SCOMPU_SCALE_v2 sCompuScale;
        hResult = sCompuScale.Load(pCurrChild);
        if (hResult == S_OK)
        {
            m_odCompuScaleList.AddTail(sCompuScale);
        }
    }
    else if (omModNodeName == COMPU_DEFAULT_VALUE_NODE)
    {
        hResult = m_sCompuDefValue.Load(pCurrChild);
    }

    return S_OK;
}
/* Ends implementation of SCOMPU_INTERNAL_TO_PHYS */

/* Starts implementation of SCOMPU_METHOD */


SCOMPU_METHOD_v2 :: SCOMPU_METHOD_v2()
{
    DoCleanup();
}
SCOMPU_METHOD_v2::~SCOMPU_METHOD_v2()
{
    DoCleanup();
}
void SCOMPU_METHOD_v2::DoCleanup()
{
    m_ouCategory = INVALID_STRING;
    //  m_odPhysConstrs.RemoveAll();
    //  m_odInternalConstrs.RemoveAll();
    m_sCompu_Internal_To_Phys.DoCleanup();
    m_sNamedElemType.DoCleanup();

    m_omUnitRef = INVALID_STRING;
}

SCOMPU_METHOD_v2::SCOMPU_METHOD_v2(SCOMPU_METHOD_v2& RefObj)
{
    m_sNamedElemType = RefObj.m_sNamedElemType;

    m_ouCategory =  RefObj.m_ouCategory;
    m_eCategory = RefObj.m_eCategory;
    m_omUnitRef = RefObj.m_omUnitRef;
    //  m_odPhysConstrs.RemoveAll();
    //  m_odPhysConstrs.AddTail(&(RefObj.m_odPhysConstrs));
    //m_odInternalConstrs.RemoveAll();
    //m_odInternalConstrs.AddTail(&(RefObj.m_odInternalConstrs));
    m_sCompu_Internal_To_Phys = RefObj.m_sCompu_Internal_To_Phys;
}

HRESULT SCOMPU_METHOD_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return DoIterate_ForLoad(pIDomNode);
}

HRESULT SCOMPU_METHOD_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild)
{
    HRESULT hResult = S_FALSE;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);
    static BYTE ConstrType = 0x0;
    if (omModNodeName == CATEGORY_NODE)
    {
        std::string omTmp = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
        if (CATEGORY3::m_odUnitRef.Find(omTmp))
        {
            m_ouCategory = omTmp;
        }
    }
    else if (omModNodeName == UNIT_REF_NODE)
    {
        CNodeUtil::bReadAttribute(pCurrChild, m_omUnitRef, "ID-REF");
    }
    else if (omModNodeName == PHYS_CONSTRS_NODE)
    {
        ConstrType = 0x0;
        DoIterate_ForLoad(pCurrChild);
    }
    else if (omModNodeName == INTERNAL_CONSTRS_NODE)
    {
        ConstrType = 0x1;

        DoIterate_ForLoad(pCurrChild);
    }

    else if (omModNodeName == COMPU_INTERNAL_TO_PHYS_NODE)
    {
        m_sCompu_Internal_To_Phys.Load(pCurrChild);
    }
    else
    {
        m_sNamedElemType.Load(pCurrChild);
    }

    return S_OK;
}
/* Ends implementation of SCOMPU_METHOD */

/* Starts implementation of SPHYSICAL_TYPE */
SPHYSICAL_TYPE_v2::SPHYSICAL_TYPE_v2()
{
    DoCleanup();
}

SPHYSICAL_TYPE_v2::~SPHYSICAL_TYPE_v2()
{
    DoCleanup();
}
void SPHYSICAL_TYPE_v2::DoCleanup()
{
    m_ouToken = INVALID_STRING;

}

HRESULT SPHYSICAL_TYPE_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    std::string omTmp = "";
    CNodeUtil::bReadAttribute(pIDomNode, omTmp, "BASE-DATA-TYPE");
    if (BASE_DATA_TYPE::m_odUnitRef.Find(omTmp))
    {
        m_ouToken = omTmp;
    }
    return DoIterate_ForLoad(pIDomNode);
}

HRESULT SPHYSICAL_TYPE_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild)
{
    HRESULT hResult = S_FALSE;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == "PRECISION")
    {
        std::string omTmp = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
        m_fPrecision = atof(omTmp.c_str());
    }
    return S_OK;
}
/* Ends implementation of SPHYSICAL_TYPE */

/* Starts implementation of CCODING */
SCODED_TYPE_v2::SCODED_TYPE_v2()
{
    DoCleanup();
}

SCODED_TYPE_v2::~SCODED_TYPE_v2()
{
    DoCleanup();
}
void SCODED_TYPE_v2::DoCleanup()
{
    m_ouDataType = INVALID_STRING;
    m_ouCategory = INVALID_STRING;
    m_ouEncoding = INVALID_STRING;
    m_ouTermination = INVALID_STRING;
    m_sLength.DoCleanup();

}
SCODED_TYPE_v2& SCODED_TYPE_v2::operator=(SCODED_TYPE_v2& sCodedType)
{
    m_ouDataType         = sCodedType.m_ouDataType;
    m_ouCategory         = sCodedType.m_ouCategory;
    m_ouEncoding         = sCodedType.m_ouEncoding;
    m_ouTermination         = sCodedType.m_ouTermination;

    m_bChoice = sCodedType.m_bChoice;
    m_sLength = sCodedType.m_sLength;
    return *this;

}
HRESULT SCODED_TYPE_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    std::string omTmp = "";
    CNodeUtil::bReadAttribute(pIDomNode, omTmp, "ho:BASE-DATA-TYPE");
    if (BASE_DATA_TYPE::m_odUnitRef.Find(omTmp))
    {
        m_ouDataType = omTmp;
    }
    CNodeUtil::bReadAttribute(pIDomNode, omTmp, "CATEGORY");
    if (CATEGORY2::m_odUnitRef.Find(omTmp))
    {
        m_ouCategory = omTmp;
    }
    CNodeUtil::bReadAttribute(pIDomNode, omTmp, "ENCODING");
    if (ENCODING::m_odUnitRef.Find(omTmp))
    {
        m_ouEncoding = omTmp;
    }
    CNodeUtil::bReadAttribute(pIDomNode, omTmp, "TERMINATION");
    if (TERMINATION::m_odUnitRef.Find(omTmp))
    {
        m_ouTermination = omTmp;
    }
    return DoIterate_ForLoad(pIDomNode);
}

HRESULT SCODED_TYPE_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild)
{
    HRESULT hResult = S_FALSE;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == BIT_LENGTH_NODE)
    {
        m_bChoice = BIT_LEN;
        std::string omTmp = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
        m_sLength.m_unLength = atoi(omTmp.c_str());
    }
    else if (omModNodeName == MIN_LENGTH_NODE)
    {
        m_bChoice = MIN_MAX_LEN;
        SMIN_MAX_LEN_v2 sMinMaxLen;
        std::string omTmp = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
        sMinMaxLen.m_unMinLen = atoi(omTmp.c_str());
        MSXML2::IXMLDOMNodePtr pNextSibling = nullptr;
        if ((pCurrChild->get_nextSibling(&pNextSibling)) == S_OK)
        {
            std::string omTmp = _com_util::ConvertBSTRToString(pNextSibling->GetnodeName().GetBSTR());
            std::string omTmp1 = "";
            CNodeUtil::bRemoveTag(omTmp, omTmp1);
            if (omTmp1 == MAX_LENGTH_NODE)
            {
                std::string omTemp = _com_util::ConvertBSTRToString(pNextSibling->text.GetBSTR());
                sMinMaxLen.m_unMaxLen = atoi(omTemp.c_str());
            }

        }

        pNextSibling.Release();
        m_sLength.m_odMinMaxList.AddTail(sMinMaxLen);
    }
    return S_OK;
}
/* Ends implementation of CCODING */

/* Starts implementation of CCODING */

CCODING_v2::CCODING_v2()
{
    DoCleanup();

    /* INITIALISE BASE_DATA_TYPE TOKEN */
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_UINT8"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_INT8"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_UINT16"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_INT16"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_UINT32"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_INT32"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_UINT64"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_INT64"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_FLOAT32"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_FLOAT64"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_ASCIISTRING"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_UNICODE2STRING"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_BYTEFIELD"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("A_BITFIELD"));
    BASE_DATA_TYPE::m_odUnitRef.AddTail((std::string)_T("OTHER"));
    /* INITIALISE CATEGORY1 TOKEN */
    CATEGORY1::m_odUnitRef.AddTail((std::string)_T("COUNTRY"));
    CATEGORY1::m_odUnitRef.AddTail((std::string)_T("EQUIV-UNITS"));
    /* INITIALISE CATEGORY3 TOKEN */
    CATEGORY3::m_odUnitRef.AddTail((std::string)_T("IDENTICAL"));
    CATEGORY3::m_odUnitRef.AddTail((std::string)_T("LINEAR"));
    CATEGORY3::m_odUnitRef.AddTail((std::string)_T("SCALE-LINEAR"));
    CATEGORY3::m_odUnitRef.AddTail((std::string)_T("TEXTTABLE"));
    CATEGORY3::m_odUnitRef.AddTail((std::string)_T("TAB-NOINTP"));
    CATEGORY3::m_odUnitRef.AddTail((std::string)_T("FORMULA"));
    /* INITIALISE CATEGORY2 TOKEN */
    CATEGORY2::m_odUnitRef.AddTail((std::string)_T("LEADING-LENGTH-INFO-TYPE"));
    CATEGORY2::m_odUnitRef.AddTail((std::string)_T("END-OF-PDU"));
    CATEGORY2::m_odUnitRef.AddTail((std::string)_T("MIN-MAX-LENGTH-TYPE"));
    CATEGORY2::m_odUnitRef.AddTail((std::string)_T("STANDARD-LENGTH-TYPE"));
    /* INITIALISE ENCODING TOKEN */
    ENCODING::m_odUnitRef.AddTail((std::string)_T("SIGNED"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("UNSIGNED"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("BIT"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("IEEE-FLOATING-TYPE"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("BCD"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("DSP-FRACTIONAL"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("SM"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("BCD-P"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("BCD-UP"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("1C"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("2C"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("UTF-8"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("UCS-2"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("ISO-8859-1"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("ISO-8859-2"));
    ENCODING::m_odUnitRef.AddTail((std::string)_T("WINDOWS-1252"));
    /* INITIALISE TERMINATION TOKEN */
    TERMINATION::m_odUnitRef.AddTail((std::string)_T("NONE"));
    TERMINATION::m_odUnitRef.AddTail((std::string)_T("ZERO"));
    TERMINATION::m_odUnitRef.AddTail((std::string)_T("HEX-FF"));
    TERMINATION::m_odUnitRef.AddTail((std::string)_T("LENGTH"));
    /* INITIALISE VALIDITY TOKEN */
    VALIDITY::m_odUnitRef.AddTail((std::string)_T("VALID"));
    VALIDITY::m_odUnitRef.AddTail((std::string)_T("NOT-VALID"));
    VALIDITY::m_odUnitRef.AddTail((std::string)_T("NOT-AVAILABLE"));
    VALIDITY::m_odUnitRef.AddTail((std::string)_T("NOT-DEFINED"));
    VALIDITY::m_odUnitRef.AddTail((std::string)_T("ERROR"));
    VALIDITY::m_odUnitRef.AddTail((std::string)_T("OTHER"));
    /* INITIALISE INTERVAL-TYPE TOKEN */
    INTERVAL_TYPE::m_odUnitRef.AddTail((std::string)_T("OPEN"));
    INTERVAL_TYPE::m_odUnitRef.AddTail((std::string)_T("CLOSED"));
    INTERVAL_TYPE::m_odUnitRef.AddTail((std::string)_T("INFINITE"));
}

CCODING_v2::~CCODING_v2()
{
    DoCleanup();
}
void CCODING_v2::DoCleanup()
{
    BASE_DATA_TYPE::m_odUnitRef.RemoveAll();
    CATEGORY1::m_odUnitRef.RemoveAll();
    CATEGORY3::m_odUnitRef.RemoveAll();
    CATEGORY2::m_odUnitRef.RemoveAll();
    ENCODING::m_odUnitRef.RemoveAll();
    TERMINATION::m_odUnitRef.RemoveAll();
    VALIDITY::m_odUnitRef.RemoveAll();
    INTERVAL_TYPE::m_odUnitRef.RemoveAll();

    m_sPhysicalType.DoCleanup();
    m_sCodedType.DoCleanup();
    m_omMFR_CODING_EXTN = INVALID_STRING;
    m_odCompuMethodList.RemoveAll();
}

CCODING_v2& CCODING_v2::operator=(CCODING_v2& RefObj)
{
    m_omID = RefObj.m_omID;
    m_sNamedElemType = RefObj.m_sNamedElemType;

    m_odElementRevList.RemoveAll();
    m_odElementRevList.AddTail(&(RefObj.m_odElementRevList));

    m_sPhysicalType     = RefObj.m_sPhysicalType;
    m_sCodedType        = RefObj.m_sCodedType;
    m_omMFR_CODING_EXTN = RefObj.m_omMFR_CODING_EXTN;
    m_odCompuMethodList.RemoveAll();
    m_odCompuMethodList.AddTail(&(RefObj.m_odCompuMethodList));

    return *this;
}

CCODING_v2::CCODING_v2(CCODING_v2& RefObj)
{
    m_omID = RefObj.m_omID;
    m_sNamedElemType = RefObj.m_sNamedElemType;

    m_odElementRevList.RemoveAll();
    m_odElementRevList.AddTail(&(RefObj.m_odElementRevList));

    m_sPhysicalType     = RefObj.m_sPhysicalType;
    m_sCodedType        = RefObj.m_sCodedType;
    m_omMFR_CODING_EXTN = RefObj.m_omMFR_CODING_EXTN;
    m_odCompuMethodList.RemoveAll();
    m_odCompuMethodList.AddTail(&(RefObj.m_odCompuMethodList));
}

HRESULT CCODING_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    CNodeUtil::bReadAttribute(pIDomNode, m_omID, "ID");
    //CNodeUtil::bReadAttribute(pIDomNode, m_omOID, "ID");
    return DoIterate_ForLoad(pIDomNode);
}

HRESULT CCODING_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode)
{
    HRESULT hResult = S_FALSE;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrDOMNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == ELEMENT_REVISIONS_NODE)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode);
    }
    else if (omModNodeName == ELEMENT_REVISIONS_NODE)
    {
        SELEMENT_REVISION_v2 sElementRev;
        hResult = sElementRev.Load(pCurrDOMNode);
        if (hResult == S_OK)
        {
            m_odElementRevList.AddTail(sElementRev);
        }
    }
    else if (omModNodeName == PHYSICAL_TYPE_NODE)
    {
        hResult = m_sPhysicalType.Load(pCurrDOMNode);
    }
    else if (omModNodeName == CODED_TYPE_NODE)
    {
        hResult = m_sCodedType.Load(pCurrDOMNode);
    }
    else if (omModNodeName == COMPU_METHODS_NODE)
    {
        hResult = DoIterate_ForLoad(pCurrDOMNode);
    }
    else if (omModNodeName == COMPU_METHOD_NODE)
    {
        SCOMPU_METHOD_v2 sCompuMethod;
        hResult = sCompuMethod.Load(pCurrDOMNode);
        if (hResult == S_OK)
        {
            m_odCompuMethodList.AddTail(sCompuMethod);
        }
    }
    else if (omModNodeName == MANUFACTURE_CODING_EXTENSION_NODE)
    {
        m_omMFR_CODING_EXTN = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
    }
    else
    {
        hResult = m_sNamedElemType.Load(pCurrDOMNode);
    }
    return S_OK;

}
/* Ends implementation of CCODING */
/* SLIMIT ---STARTS */
SLIMIT_v2::SLIMIT_v2()
{
    DoCleanup();
}
SLIMIT_v2::~SLIMIT_v2()
{
    DoCleanup();
}
void SLIMIT_v2::DoCleanup()
{
    m_ouIntervalType = INVALID_STRING;
    m_fValue = -1;
}
/* SLIMIT ---ENDS */

/* SCONSTRS ---STARTS */
SCONSTRS_v2::SCONSTRS_v2()
{
    DoCleanup();
}

SCONSTRS_v2& SCONSTRS_v2::operator=(SCONSTRS_v2& RefObj)
{
    m_ouValidity = RefObj.m_ouValidity;
    m_sLowerLimit = RefObj.m_sLowerLimit;
    m_sUpperLimit = RefObj.m_sUpperLimit;

    return *this;
}

SCONSTRS_v2::SCONSTRS_v2(SCONSTRS_v2& RefObj)
{
    m_ouValidity = RefObj.m_ouValidity;
    m_sLowerLimit = RefObj.m_sLowerLimit;
    m_sUpperLimit = RefObj.m_sUpperLimit;
}

SCONSTRS_v2::~SCONSTRS_v2()
{
    DoCleanup();
}
void SCONSTRS_v2::DoCleanup()
{
    m_ouValidity = "VALID";
    m_sLowerLimit.DoCleanup();
    m_sUpperLimit.DoCleanup();

}

HRESULT SCONSTRS_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return DoIterate_ForLoad(pIDomNode);
}
HRESULT SCONSTRS_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild)
{
    HRESULT hResult = S_FALSE;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == LOWER_LIMIT_NODE)
    {
        std::string omTmp = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName());
        m_sLowerLimit.m_fValue = atof(omTmp.c_str());

        CNodeUtil::bReadAttribute(pCurrChild, omTmp, "INTERVAL-TYPE");
        if (INTERVAL_TYPE::m_odUnitRef.Find(omTmp))
        {
            m_sLowerLimit.m_ouIntervalType = omTmp;
        }
    }
    else if (omModNodeName == UPPER_LIMIT_NODE)
    {
        std::string omTmp= _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
        m_sUpperLimit.m_fValue = atof(omTmp.c_str());
        CNodeUtil::bReadAttribute(pCurrChild, omTmp, "INTERVAL-TYPE");
        if (INTERVAL_TYPE::m_odUnitRef.Find(omTmp))
        {
            m_sUpperLimit.m_ouIntervalType = omTmp;
        }
    }
    return S_OK;
}
/* SCONSTRS ---ENDS */
/* SCOMPU_SCALE ---STARTS */
SCOMPU_SCALE_v2::SCOMPU_SCALE_v2()
{
    DoCleanup();
}

SCOMPU_SCALE_v2::~SCOMPU_SCALE_v2()
{
    DoCleanup();
}

void SCOMPU_SCALE_v2::DoCleanup()
{
    m_omCompuGenMath = INVALID_STRING;
}
HRESULT SCOMPU_SCALE_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return DoIterate_ForLoad(pIDomNode);
}
HRESULT SCOMPU_SCALE_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild)
{
    HRESULT hResult = S_FALSE;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == DESC_NODE)
    {

    }
    else if (omModNodeName == LOWER_LIMIT_NODE)
    {
        std::string omTmp = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
        m_sLower.m_fValue = atof(omTmp.c_str());
        CNodeUtil::bReadAttribute(pCurrChild, omTmp, "INTERVAL-TYPE");
        if (INTERVAL_TYPE::m_odUnitRef.Find(omTmp))
        {
            m_sLower.m_ouIntervalType = omTmp;
        }
    }
    else if (omModNodeName == UPPER_LIMIT_NODE)
    {
        std::string omTmp = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
        m_sUpper.m_fValue = atof(omTmp.c_str());
        CNodeUtil::bReadAttribute(pCurrChild, omTmp, "INTERVAL-TYPE");
        if (INTERVAL_TYPE::m_odUnitRef.Find(omTmp))
        {
            m_sUpper.m_ouIntervalType = omTmp;
        }
    }
    else if (omModNodeName == COMPU_CONST_NODE)
    {
        hResult = m_sCompuConst.Load(pCurrChild);
    }
    else if (omModNodeName == COMPU_RATIONAL_COEFFS_NODE)
    {
        m_sCompuRationalCoeffs.Load(pCurrChild);
    }
    else if (omModNodeName == COMPU_GENERIC_MATH_NODE)
    {
        m_omCompuGenMath = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
    }
    return S_OK;
}
/* SCOMPU_SCALE ---ENDS */

/* SCOMPU_CONST ---STARTS*/
SCOMPU_CONST_v2::SCOMPU_CONST_v2()
{
    DoCleanup();
}
SCOMPU_CONST_v2::~SCOMPU_CONST_v2()
{
    DoCleanup();
}
void SCOMPU_CONST_v2::DoCleanup()
{
    m_fV        = -1;
    m_omVT      = INVALID_STRING;
}
HRESULT SCOMPU_CONST_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return DoIterate_ForLoad(pIDomNode);
}
HRESULT SCOMPU_CONST_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild)
{
    HRESULT hResult = S_FALSE;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == V_NODE)
    {
        m_bIsDouble = true;
        std::string omTmp = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
        m_fV = atof(omTmp.c_str());
    }
    else if (omModNodeName == VT_NODE)
    {
        m_bIsDouble = false;
        m_omVT = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
    }
    return S_OK;
}
/* SCOMPU_CONST ---ENDS*/

/* SCOMPU_RATIONAL_COEFFS ---STARTS */
SCOMPU_RATIONAL_COEFFS_v2::SCOMPU_RATIONAL_COEFFS_v2()
{
    DoCleanup();
}
void SCOMPU_RATIONAL_COEFFS_v2::DoCleanup()
{
    m_faCompuNuma.RemoveAll();
    m_faCompuDeno.RemoveAll();
}
HRESULT SCOMPU_RATIONAL_COEFFS_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return DoIterate_ForLoad(pIDomNode);
}

HRESULT SCOMPU_RATIONAL_COEFFS_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrChild)
{
    HRESULT hResult = S_FALSE;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrChild->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);
    static BYTE bChoice = 0x0;
    if (omModNodeName == COMPU_NUMERATOR_NODE)
    {
        bChoice = 0x0;
        DoIterate_ForLoad(pCurrChild);
    }
    else if (omModNodeName == COMPU_DENOMINATOR_NODE)
    {
        bChoice = 0x1;
        DoIterate_ForLoad(pCurrChild);
    }
    else if (omModNodeName == V_NODE)
    {
        std::string omTmp = "";
        float fTemp = INVALID_VALUE;
        if (bChoice == 0x0)
        {
            omTmp = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
            fTemp = atof(omTmp.c_str());
            if (fTemp == 0.062525)
            {
                int z = 0;
            }
            m_faCompuNuma.AddTail(fTemp);
        }
        else if (bChoice == 0x1)
        {
            omTmp = _com_util::ConvertBSTRToString(pCurrChild->text.GetBSTR());
            fTemp = atof(omTmp.c_str());
            m_faCompuDeno.AddTail(fTemp);
        }
    }
    return S_OK;
}
/* SCOMPU_RATIONAL_COEFFS ---ENDS */

/* SLENGTH ---STARTS */
SLENGTH_v2::SLENGTH_v2()
{
}
SLENGTH_v2::~SLENGTH_v2()
{
    DoCleanup();
}
void SLENGTH_v2::DoCleanup()
{
    m_odMinMaxList.RemoveAll();
}
SLENGTH_v2& SLENGTH_v2::operator=(SLENGTH_v2& sLength)
{
    m_odMinMaxList.RemoveAll();
    m_odMinMaxList.AddTail(&(sLength.m_odMinMaxList));
    m_unLength = sLength.m_unLength;
    return *this;
}
/* SLENGTH ---ENDS */

SCOMPU_METHOD_v2& SCOMPU_METHOD_v2::operator=(SCOMPU_METHOD_v2& RefObj)
{
    m_sNamedElemType = RefObj.m_sNamedElemType;

    m_ouCategory =  RefObj.m_ouCategory;
    m_eCategory = RefObj.m_eCategory;
    m_omUnitRef = RefObj.m_omUnitRef;
    m_sCompu_Internal_To_Phys = RefObj.m_sCompu_Internal_To_Phys;
    return *this;
}


/* SELEMENT_REVISION --- STARTS */
SELEMENT_REVISION_v2::SELEMENT_REVISION_v2()
{
    DoCleanup();
}
SELEMENT_REVISION_v2::~SELEMENT_REVISION_v2()
{
    DoCleanup();
}
void SELEMENT_REVISION_v2::DoCleanup()
{
    m_odCompanyRevInfoList.RemoveAll();
    m_odModificationList.RemoveAll();

    m_omDate = INVALID_STRING;
    m_omREVISION_LABEL = INVALID_STRING;
    m_omSTATE = INVALID_STRING;
    m_omTEAM_MEMBER_REF = INVALID_STRING;
}
HRESULT SELEMENT_REVISION_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return DoIterate_ForLoad(pIDomNode);
}

HRESULT SELEMENT_REVISION_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    HRESULT hResult = S_OK;

    std::string omNodeName = _com_util::ConvertBSTRToString(pIDomNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == TEAM_MEMBER_REF_NODE)
    {
        CNodeUtil::bReadAttribute(pIDomNode, m_omTEAM_MEMBER_REF, _bstr_t("ID-REF"));
    }
    else if (omModNodeName == REVISION_LABEL_NODE)
    {
        m_omREVISION_LABEL = _com_util::ConvertBSTRToString(pIDomNode->text.GetBSTR());
    }
    else if (omModNodeName == STATE_NODE)
    {
        m_omSTATE = _com_util::ConvertBSTRToString(pIDomNode->text.GetBSTR());
    }
    else if (omModNodeName == DATE_NODE)
    {
        m_omDate = _com_util::ConvertBSTRToString(pIDomNode->text.GetBSTR());
    }
    else if (omModNodeName == COMPANY_REVISION_INFOS_NODE)
    {
        hResult = DoIterate_ForLoad(pIDomNode);
    }
    else if (omModNodeName == COMPANY_REVISION_INFO_NODE)
    {
        SCOMPANY_REV_INFO_v2 sComRevInfo;
        hResult = sComRevInfo.Load(pIDomNode);
        if (hResult == S_OK)
        {
            m_odCompanyRevInfoList.AddTail(sComRevInfo);
        }
    }
    else if (omModNodeName == MODIFICATIONS_NODE)
    {
        hResult = DoIterate_ForLoad(pIDomNode);
    }
    else if (omModNodeName == MODIFICATION_NODE)
    {
        SMODIFICATION_v2 sModification;
        hResult = sModification.Load(pIDomNode);
        if (hResult == S_OK)
        {
            m_odModificationList.AddTail(sModification);
        }
    }

    return S_OK;
}

SELEMENT_REVISION_v2& SELEMENT_REVISION_v2::operator=(SELEMENT_REVISION_v2& RefObj)
{
    m_omDate            = RefObj.m_omDate;
    m_omREVISION_LABEL  = RefObj.m_omREVISION_LABEL;
    m_omSTATE           = RefObj.m_omSTATE;
    m_omTEAM_MEMBER_REF = RefObj.m_omTEAM_MEMBER_REF;


    m_odCompanyRevInfoList.RemoveAll();
    m_odCompanyRevInfoList.AddTail(&(RefObj.m_odCompanyRevInfoList));

    m_odModificationList.RemoveAll();
    m_odModificationList.AddTail(&(RefObj.m_odModificationList));

    return *this;
}
/* SELEMENT_REVISION --- ENDS */


/* SCOMPANY_REV_INFO --- STARTS */
SCOMPANY_REV_INFO_v2::SCOMPANY_REV_INFO_v2()
{
    DoCleanup();
}
void SCOMPANY_REV_INFO_v2::DoCleanup()
{
    m_omRev_Level = INVALID_STRING;
    m_omState = INVALID_STRING;
    m_omCompany_Data_Ref = INVALID_STRING;
}
HRESULT SCOMPANY_REV_INFO_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return DoIterate_ForLoad(pIDomNode);
}

HRESULT SCOMPANY_REV_INFO_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pCurrDOMNode)
{
    HRESULT hResult = S_OK;

    std::string omNodeName = _com_util::ConvertBSTRToString(pCurrDOMNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == COMPANY_DATA_REF_NODE)
    {
        CNodeUtil::bReadAttribute(pCurrDOMNode,m_omCompany_Data_Ref, "ID-REF");
    }
    else if (omModNodeName == REVISION_LABEL_NODE)
    {
        m_omRev_Level = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
    }
    else if (omModNodeName == STATE_NODE)
    {
        m_omState = _com_util::ConvertBSTRToString(pCurrDOMNode->text.GetBSTR());
    }
    return hResult;
}

/* SCOMPANY_REV_INFO --- ENDS */

/* SMODIFICATION --- STARTS */
SMODIFICATION_v2::SMODIFICATION_v2()
{
    DoCleanup();
}
void SMODIFICATION_v2::DoCleanup()
{
    m_omChange = INVALID_STRING;
    m_omReason = INVALID_STRING;
}
HRESULT SMODIFICATION_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return DoIterate_ForLoad(pIDomNode);
}

HRESULT SMODIFICATION_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    HRESULT hResult = S_OK;

    std::string omNodeName = _com_util::ConvertBSTRToString(pIDomNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == CHANGE_NODE)
    {
        m_omChange = _com_util::ConvertBSTRToString(pIDomNode->text.GetBSTR());
    }
    if (omModNodeName == REASON_NODE)
    {
        m_omReason = _com_util::ConvertBSTRToString(pIDomNode->text.GetBSTR());
    }
    return hResult;
}
/* SMODIFICATION --- ENDS */

/* SNAMED_ELEMENT_TYPE --- STARTS */
SNAMED_ELEMENT_TYPE_v2::SNAMED_ELEMENT_TYPE_v2()
{
    DoCleanup();
}
void SNAMED_ELEMENT_TYPE_v2::DoCleanup()
{
    m_omLongName = INVALID_STRING;
    m_omLNameLang = INVALID_STRING;
    m_omShortName = INVALID_STRING;
    m_omProductRef = INVALID_STRING;
}
HRESULT SNAMED_ELEMENT_TYPE_v2::Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    return Process_Load(pIDomNode);
}

HRESULT SNAMED_ELEMENT_TYPE_v2::Process_Load(MSXML2::IXMLDOMNodePtr& pIDomNode)
{
    HRESULT hResult = S_OK;

    std::string omNodeName = _com_util::ConvertBSTRToString(pIDomNode->GetnodeName().GetBSTR());
    std::string omModNodeName = "";
    CNodeUtil::bRemoveTag(omNodeName, omModNodeName);

    if (omModNodeName == SHORT_NAME_NODE)
    {
        m_omShortName = _com_util::ConvertBSTRToString(pIDomNode->text.GetBSTR());
    }
    else if (omModNodeName == LONG_NAME_NODE)
    {
        m_omLongName = _com_util::ConvertBSTRToString(pIDomNode->text.GetBSTR());
        CNodeUtil::bReadAttribute(pIDomNode, m_omLNameLang, _bstr_t("lang"));
    }
    else if (omModNodeName == DESC_NODE)
    {
        m_omDESC.m_omDescription = _com_util::ConvertBSTRToString(pIDomNode->text.GetBSTR());
        CNodeUtil::bReadAttribute(pIDomNode, m_omDESC.m_omDescriptType, _bstr_t("TYPE"));
        CNodeUtil::bReadAttribute(pIDomNode, m_omDESC.m_omDescriptionLang, _bstr_t("lang"));
    }
    else if (omModNodeName == PRODUCT_REF_NODE)
    {
        CNodeUtil::bReadAttribute(pIDomNode, m_omProductRef, _bstr_t("ID-REF"));
    }

    return hResult;
}
/* SNAMED_ELEMENT_TYPE --- ENDS */

/* SDECRIPTION --- STARTS */
SDECRIPTION_v2::SDECRIPTION_v2()
{
    m_omDescription     = INVALID_STRING;
    m_omDescriptType    = INVALID_STRING;
    m_omDescriptionLang = INVALID_STRING;
}
/* SDECRIPTION --- ENDS */


HRESULT CCODING_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT SCODED_TYPE_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT SCOMPANY_REV_INFO_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);

        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT SCOMPU_CONST_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);

        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT SCOMPU_INTERNAL_TO_PHYS_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }
    pChildList.Release();

    return hResult;
}

HRESULT SCOMPU_METHOD_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }
    pChildList.Release();

    return hResult;
}

HRESULT SCOMPU_RATIONAL_COEFFS_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT SCOMPU_SCALE_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT SCONSTRS_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }
    pChildList.Release();

    return hResult;
}

HRESULT SELEMENT_REVISION_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }
    pChildList.Release();
    return hResult;
}

HRESULT SMODIFICATION_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}

HRESULT SPHYSICAL_TYPE_v2::DoIterate_ForLoad(MSXML2::IXMLDOMNodePtr& pParent)
{
    HRESULT hResult = S_FALSE;

    MSXML2::IXMLDOMNodeListPtr pChildList = nullptr;
    pParent->get_childNodes(&pChildList);
    long lTotal = 0;
    pChildList->get_length(&lTotal);
    for (int i = 0; i < lTotal; i++)
    {
        MSXML2::IXMLDOMNodePtr pCurrChild = pChildList->Getitem(i);
        hResult = Process_Load(pCurrChild);
        pCurrChild.Release();
    }

    pChildList.Release();

    return hResult;
}
