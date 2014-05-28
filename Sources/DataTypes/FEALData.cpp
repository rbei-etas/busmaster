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
 * \file      FEALData.cpp
 * \brief     Implementation file for FIBEX data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for FIBEX data types.
 */
#include "DataTypes_StdAfx.h"
#include "FEALData.h"


void tagAbsSWAKEUP::DoCleanup()
{
}

void tagAbsSFlexrayCluster::DoCleanup()
{
    memset(this, 0, sizeof(tagAbsSFlexrayCluster));
}

tagAbsSCluster::tagAbsSCluster()
{
    m_ouBusInfo.m_pouFlexRayCluster = new ABS_FLEXRAY_CLUSTER;
}

tagAbsSCluster::~tagAbsSCluster()
{
    if (m_ouBusInfo.m_pouFlexRayCluster != nullptr)
    {
        delete m_ouBusInfo.m_pouFlexRayCluster;
        m_ouBusInfo.m_pouFlexRayCluster = nullptr;
    }
}

tagAbsSCluster& tagAbsSCluster::operator=(tagAbsSCluster& RefObj)
{
    m_omClusterID = RefObj.m_omClusterID;
    m_omClusterName = RefObj.m_omClusterName;

    m_omChannelRefs.RemoveAll();
    m_omChannelRefs.Copy(RefObj.m_omChannelRefs);
    if (m_ouBusInfo.m_pouFlexRayCluster == nullptr)
    {
        m_ouBusInfo.m_pouFlexRayCluster = new ABS_FLEXRAY_CLUSTER;
    }
    if ((RefObj.m_ouBusInfo.m_pouFlexRayCluster != nullptr) && (m_ouBusInfo.m_pouFlexRayCluster != nullptr))
    {
        *(m_ouBusInfo.m_pouFlexRayCluster) = *(RefObj.m_ouBusInfo.m_pouFlexRayCluster);
    }
    return *this;
}

void tagAbsSCluster::DoCleanup()
{
    m_omClusterID  = EMPTY_STRING;
    m_omClusterName  = EMPTY_STRING;
    m_ulSpeed  = EMPTY_VALUE;
    m_bIS_HIGH_LOW_BIT_ORDER  = false;
    m_omBIT_COUNTING_POLICY  = EMPTY_STRING;
    m_omProtocol  = EMPTY_STRING;
    m_omChannelRefs.RemoveAll();
}

tagAbsSFRAME_TRIGGERING& tagAbsSFRAME_TRIGGERING::operator=(tagAbsSFRAME_TRIGGERING& RefObj)
{
    m_omID = RefObj.m_omID;
    m_odTimingList.RemoveAll();
    m_odTimingList.AddTail(&(RefObj.m_odTimingList));
    m_omFrameRef = RefObj.m_omFrameRef;
    memcpy (m_uIdentifier.m_acMFR_ID_EXTN, RefObj.m_uIdentifier.m_acMFR_ID_EXTN, 256);
    m_uIdentifier.m_lIDENTIFIER_VALUE = RefObj.m_uIdentifier.m_lIDENTIFIER_VALUE;

    return *this;
}

tagAbsSPDU_TRIGGERING& tagAbsSPDU_TRIGGERING::operator=(tagAbsSPDU_TRIGGERING& RefObj)
{
    m_omID = RefObj.m_omID;
    m_odTimingList.RemoveAll();
    m_odTimingList.AddTail(&(RefObj.m_odTimingList));
    m_omPDU_REF = RefObj.m_omPDU_REF;

    return *this;
}

tagAbsSChannel::tagAbsSChannel()
{
    //m_pPDUTriggeringList = nullptr;
}
tagAbsSChannel::~tagAbsSChannel()
{
    /*if (m_pPDUTriggeringList != nullptr)
    {
        m_pPDUTriggeringList->RemoveAll();
        m_pPDUTriggeringList = nullptr;
    }*/
}

tagAbsSChannel& tagAbsSChannel::operator=(tagAbsSChannel& RefObj)
{
    m_omChannelID = RefObj.m_omChannelID;
    m_omShortName = RefObj.m_omShortName;
    m_omChannelName = RefObj.m_omChannelName;

    m_odFrameTriggeringList.RemoveAll();
    m_odFrameTriggeringList.AddTail(&(RefObj.m_odFrameTriggeringList));
    // if (m_pPDUTriggeringList != nullptr)
    {
        m_pPDUTriggeringList.RemoveAll();
        //if (RefObj.m_pPDUTriggeringList != nullptr)
        if(RefObj.m_pPDUTriggeringList.IsEmpty() == FALSE)
        {
            m_pPDUTriggeringList.AddTail(&(RefObj.m_pPDUTriggeringList));
        }
        /* else
         {
             delete m_pPDUTriggeringList;
             m_pPDUTriggeringList = nullptr;
         }*/

    }
    /* else
     {
         if (RefObj.m_pPDUTriggeringList != nullptr)
         {
             m_pPDUTriggeringList = new AbsCPduTriggeringList;
             m_pPDUTriggeringList->AddTail(RefObj.m_pPDUTriggeringList);
         }
     }*/
    return *this;
}
void tagAbsSChannel::DoCleanup()
{
    m_omChannelID   = EMPTY_STRING;
    m_omShortName   = EMPTY_STRING;
    m_omChannelName = EMPTY_STRING;
    m_odFrameTriggeringList.RemoveAll();
    m_pPDUTriggeringList.RemoveAll();
    /* if (m_pPDUTriggeringList != nullptr)
     {
         m_pPDUTriggeringList->RemoveAll();
     }*/
}
void tagAbsSFLEXRAY_SPECIFIC_CTRL::DoCleanup()
{
    m_sKeySlotUsage.m_nStartUpSync  = EMPTY_VALUE;
    m_sKeySlotUsage.m_nSync = EMPTY_VALUE;
    m_sKeySlotUsage.m_omNone = EMPTY_STRING;
    m_shMaxDynPayloadLgt  = EMPTY_VALUE;


    m_shClusterDriftDamping  = EMPTY_VALUE;


    m_nDecodingCorr  = EMPTY_VALUE;



    m_nListenTimeOut  = EMPTY_VALUE;


    m_shMaxDrift  = EMPTY_VALUE;


    m_shExternOffsetCorr  = EMPTY_VALUE;


    m_shExternRateCorr  = EMPTY_VALUE;


    m_shLatestTx  = EMPTY_VALUE;


    m_nMicroPreCycle  = EMPTY_VALUE;


    m_shOffsetCorrOut  = EMPTY_VALUE;


    m_shRateCorrOut  = EMPTY_VALUE;


    m_shSamplePerMicrotick  = EMPTY_VALUE;


    m_shDelayCompensationA  = EMPTY_VALUE;


    m_shDelayCompensationB  = EMPTY_VALUE;


    m_shWakeUpPattern  = EMPTY_VALUE;


    m_bAllowHaltDewToClock  = false;


    m_shAllowPassiveToActive  = EMPTY_VALUE;


    m_shAcceptedStartUpRange  = EMPTY_VALUE;


    m_shMacroInitialOffsetA  = EMPTY_VALUE;


    m_shMacroInitialOffsetB  = EMPTY_VALUE;


    m_shMicroInitialOffsetA  = EMPTY_VALUE;


    m_shMicroInitialOffsetB  = EMPTY_VALUE;


    m_bSingleSlotEnable  = false;
    m_fMicrotick  = EMPTY_VALUE;
    m_fMicroPerMacroNom  = EMPTY_VALUE;

}

void tagAbsSCONTROLLER::DoCleanup()
{
    m_omID  = EMPTY_STRING;
    m_omType  = EMPTY_STRING;
    m_omContrName  = EMPTY_STRING;
    m_omVENDOR_NAME  = EMPTY_STRING;
    m_omCHIP_NAME  = EMPTY_STRING;
    m_ulTERMINATION_IMPEDANCE  = EMPTY_VALUE;
    m_omTRANSCEIVER  = EMPTY_STRING;
    m_sFlexraySpecControllerData.DoCleanup();

}

tagAbsSECU::tagAbsSECU()
{
    m_odConnectorList = nullptr;
}
tagAbsSECU::~tagAbsSECU()
{
    if (m_odConnectorList != nullptr)
    {
        delete m_odConnectorList;
        m_odConnectorList = nullptr;
    }
}
tagAbsSECU& tagAbsSECU::operator=(tagAbsSECU& RefObj)
{
    m_omECUID = RefObj.m_omECUID;
    m_omECUName = RefObj.m_omECUName;

    m_odControllerList.RemoveAll();
    m_odControllerList.AddTail(&(RefObj.m_odControllerList));

    m_odConnectorInfo.clear();
    for (AbsConnectorInfoList::iterator it = RefObj.m_odConnectorInfo.begin();
            it != RefObj.m_odConnectorInfo.end(); ++it)
    {
        SBASE_CONNECTOR sConnectorInfo;
        sConnectorInfo.m_omCHANNEL_REF      = it->m_omCHANNEL_REF;
        sConnectorInfo.m_omCONTROLLER_REF   = it->m_omCONTROLLER_REF;
        sConnectorInfo.m_omID               = it->m_omID;
        sConnectorInfo.m_omType             = it->m_omType;

        m_odConnectorInfo.push_back(sConnectorInfo);
    }

    return *this;
}
void tagAbsSECU::DoCleanUp()
{
    m_omECUID  = EMPTY_STRING;
    m_omECUName  = EMPTY_STRING;
    m_odControllerList.RemoveAll();
}

tagAbsSFrame::tagAbsSFrame()
{
}

tagAbsSFrame::~tagAbsSFrame()
{
}

tagAbsSFrame& tagAbsSFrame::operator=(tagAbsSFrame& RefObj)
{
    m_omFrameID     = RefObj.m_omFrameID;
    m_omFrameName   = RefObj.m_omFrameName;
    m_unByteLength  = RefObj.m_unByteLength ;
    m_omFrameType   = RefObj.m_omFrameType;
    m_odSigInstList.RemoveAll();
    m_odSigInstList.AddTail(&(RefObj.m_odSigInstList));

    m_podPduInstList.RemoveAll();
    m_podPduInstList.AddTail(&(RefObj.m_podPduInstList));

    return *this;
}

void tagAbsSFrame::DoCleanup()
{
    m_omFrameID  = EMPTY_STRING;
    m_omFrameName  = EMPTY_STRING;
    m_unByteLength  = EMPTY_VALUE;
    m_omFrameType  = EMPTY_STRING;
    m_odSigInstList.RemoveAll();
    m_podPduInstList.RemoveAll();
}

void tagAbsSSignal::DoCleanup()
{
    m_omSignalID  = EMPTY_STRING;
    m_omSignalName  = EMPTY_STRING;
    m_dDEFAULT_VALUE  = EMPTY_VALUE;
    m_omCoding  = EMPTY_STRING;
    m_unPriority  = EMPTY_VALUE;

}

tagAbsSCODED_TYPE& tagAbsSCODED_TYPE::operator=(tagAbsSCODED_TYPE& RefObj)
{
    m_omDataType = RefObj.m_omDataType;
    m_omCategory = RefObj.m_omCategory;
    m_omEncoding = RefObj.m_omEncoding;
    m_omTermination = RefObj.m_omTermination;
    m_bChoice = RefObj.m_bChoice;
    m_sLength.m_odMinMaxList.RemoveAll();
    m_sLength.m_odMinMaxList.AddTail(&(RefObj.m_sLength.m_odMinMaxList));
    m_sLength.m_unLength = RefObj.m_sLength.m_unLength;
    return *this;
}

void tagAbsSCODED_TYPE::DoCleanup()
{
    m_omDataType = EMPTY_STRING;
    m_omCategory = EMPTY_STRING;
    m_omEncoding = EMPTY_STRING;
    m_omTermination = EMPTY_STRING;
    m_sLength.m_odMinMaxList.RemoveAll();
    m_sLength.m_unLength = EMPTY_VALUE ;
}

tagAbsSCOMPU_SCALE& tagAbsSCOMPU_SCALE::operator=(tagAbsSCOMPU_SCALE& RefObj)
{
    m_sLower = RefObj.m_sLower;
    m_sUpper = RefObj.m_sUpper;
    m_sCompuConst = RefObj.m_sCompuConst;
    m_sCompuRationalCoeffs.m_faCompuDeno.RemoveAll();
    m_sCompuRationalCoeffs.m_faCompuDeno.AddTail(&(RefObj.m_sCompuRationalCoeffs.m_faCompuDeno));
    m_sCompuRationalCoeffs.m_faCompuNuma.RemoveAll();
    m_sCompuRationalCoeffs.m_faCompuNuma.AddTail(&(RefObj.m_sCompuRationalCoeffs.m_faCompuNuma));
    m_omCompuGenMath = RefObj.m_omCompuGenMath;
    return *this;
}

tagAbsSCOMPU_INTERNAL_TO_PHYS& tagAbsSCOMPU_INTERNAL_TO_PHYS::operator=(tagAbsSCOMPU_INTERNAL_TO_PHYS& RefObj)
{
    m_odCompuScaleList.RemoveAll();
    m_odCompuScaleList.AddTail(&(RefObj.m_odCompuScaleList));
    m_sCompuDefValue = RefObj.m_sCompuDefValue;
    return *this;
}

tagAbsSCompuMethod& tagAbsSCompuMethod::operator=(tagAbsSCompuMethod& RefObj)
{
    m_omID                      = RefObj.m_omID;
    m_omCompuName               = RefObj.m_omCompuName;
    m_omCategory                = RefObj.m_omCategory;
    m_omUnitRef                 = RefObj.m_omUnitRef;
    m_odPhysConstrs.RemoveAll();
    m_odPhysConstrs.AddTail(&(RefObj.m_odPhysConstrs));
    m_odInternalConstrs.RemoveAll();
    m_odInternalConstrs.AddTail(&(RefObj.m_odInternalConstrs));
    m_sCompu_Internal_To_Phys   = RefObj.m_sCompu_Internal_To_Phys;
    return *this;
}

tagAbsSCoding& tagAbsSCoding::operator=(tagAbsSCoding& RefObj)
{
    m_omCodingID = RefObj.m_omCodingID;
    m_omCodingName = RefObj.m_omCodingName;
    m_sPhysicalType = RefObj.m_sPhysicalType;
    m_sCodedType = RefObj.m_sCodedType;
    m_odCompuMethodList.RemoveAll();
    m_odCompuMethodList.AddTail(&(RefObj.m_odCompuMethodList));

    return *this;
}

void tagAbsSCoding::DoCleanup()
{
    m_omCodingID = EMPTY_STRING;
    m_omCodingName = EMPTY_STRING;
    m_sPhysicalType.m_fPrecision = EMPTY_VALUE;
    m_sPhysicalType.m_omBaseType = EMPTY_STRING;
    m_sCodedType.DoCleanup();
    m_odCompuMethodList.RemoveAll();

}

tagAbsSFunction& tagAbsSFunction::operator=(tagAbsSFunction& RefObj)
{
    m_omFunctionID      = RefObj.m_omFunctionID;
    m_omFunctionName    = RefObj.m_omFunctionName;

    m_odInputPorts.RemoveAll();
    m_odInputPorts.AddTail(&(RefObj.m_odInputPorts));
    m_odOutputPorts.RemoveAll();
    m_odOutputPorts.AddTail(&(RefObj.m_odOutputPorts));

    return *this;
}

void tagAbsSFunction::DoCleanup()
{
    m_omFunctionID  = EMPTY_STRING;
    m_omFunctionName  = EMPTY_STRING;
    m_odInputPorts.RemoveAll();
    m_odOutputPorts.RemoveAll();
}

tagAbsSComposite& tagAbsSComposite::operator=(tagAbsSComposite& RefObj)
{
    m_omCompositeID = RefObj.m_omCompositeID;
    m_omCompositeName = RefObj.m_omCompositeName;

    m_omCompositionLevel = RefObj.m_omCompositionLevel;
    m_omRefValues.RemoveAll();
    m_omRefValues.Copy(RefObj.m_omRefValues);

    return *this;
}

void tagAbsSComposite::Docleanup()
{
    m_omCompositeID  = EMPTY_STRING;
    m_omCompositeName  = EMPTY_STRING;

    m_omCompositionLevel  = EMPTY_STRING;
    m_omRefValues.RemoveAll();

}

tagAbsSPdu& tagAbsSPdu::operator=(tagAbsSPdu& RefObj)
{
    m_omPduID = RefObj.m_omPduID;
    m_omPduName = RefObj.m_omPduName;
    m_unByteLen = RefObj.m_unByteLen;
    m_omPduType = RefObj.m_omPduType;
    m_odSigInstList.RemoveAll();
    m_odSigInstList.AddTail(&(RefObj.m_odSigInstList));
    return *this;
}

void tagAbsSPdu::DoCleanup()
{
    m_omPduID  = EMPTY_STRING;
    m_omPduName  = EMPTY_STRING;
    m_unByteLen  = EMPTY_VALUE;
    m_omPduType  = EMPTY_STRING;
    m_odSigInstList.RemoveAll();
}

void tagAbsSElement::DoCleanup()
{
    m_omClusterList.RemoveAll();
    m_omChannelList.RemoveAll();
    m_omECUList.RemoveAll();
    m_omFrameList.RemoveAll();
    m_omSignalList.RemoveAll();
    m_omFunctionList.RemoveAll();
    m_omCompositeList.RemoveAll();
    m_omPDUList.RemoveAll();
}

BOOL tagAbsSElement::RemoveSignal(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP /* Category */)
{
    BOOL bResult = FALSE;
    switch (eEntity)
    {
        case TYPE_CSIGNAL:
        {
            POSITION pos = GetSignalPos(omID);
            if (pos != nullptr)
            {
                m_omSignalList.RemoveAt(pos);
            }
            bResult = TRUE;
        }
        break;
        case TYPE_CSIGNALLIST:
        {
            m_omSignalList.RemoveAll();
            bResult = TRUE;
        }
        break;
    }
    return bResult;
}

BOOL tagAbsSElement::RemoveCluster(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP /* Category */)
{
    BOOL bResult = FALSE;
    switch (eEntity)
    {
        case TYPE_CCLUSTER:
        {
            POSITION pos = GetClusterPos(omID);
            if (pos != nullptr)
            {
                m_omClusterList.RemoveAt(pos);
            }
            bResult = TRUE;
        }
        break;
        case TYPE_CCLUSTERLIST:
        {
            m_omClusterList.RemoveAll();
            bResult = TRUE;
        }
        break;
    }
    return bResult;
}

BOOL tagAbsSElement::RemoveChannel(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP /* Category */)
{
    BOOL bResult = FALSE;
    switch (eEntity)
    {
        case TYPE_CCHANNEL:
        {
            POSITION pos = GetChannelPos(omID);
            if (pos != nullptr)
            {
                m_omChannelList.RemoveAt(pos);
            }
            bResult = TRUE;
        }
        break;
        case TYPE_CCHANNELLIST:
        {
            m_omChannelList.RemoveAll();
            bResult = TRUE;
        }
        break;
    }
    return bResult;
}

BOOL tagAbsSElement::RemoveECU(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP /* Category */)
{
    BOOL bResult = FALSE;
    switch (eEntity)
    {
        case TYPE_CECU:
        {
            POSITION pos = GetECUPos(omID);
            if (pos != nullptr)
            {
                m_omECUList.RemoveAt(pos);
            }
            bResult = TRUE;
        }
        break;
        case TYPE_CECULIST:
        {
            m_omECUList.RemoveAll();
            bResult = TRUE;
        }
        break;
    }
    return bResult;
}

BOOL tagAbsSElement::RemoveFrame(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP /* Category */)
{
    BOOL bResult = FALSE;
    switch (eEntity)
    {
        case TYPE_CFRAME:
        {
            POSITION pos = GetFramePos(omID);
            if (pos != nullptr)
            {
                m_omFrameList.RemoveAt(pos);
            }
            bResult = TRUE;
        }
        break;
        case TYPE_CFRAMELIST:
        {
            m_omFrameList.RemoveAll();
            bResult = TRUE;
        }
        break;
    }
    return bResult;
}

BOOL tagAbsSElement::RemoveFunction(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP /* Category */)
{
    BOOL bResult = FALSE;
    switch (eEntity)
    {
        case TYPE_CFUNCTION:
        {
            POSITION pos = GetFunctionPos(omID);
            if (pos != nullptr)
            {
                m_omFunctionList.RemoveAt(pos);
            }
            bResult = TRUE;
        }
        break;
        case TYPE_CFUNCTIONLIST:
        {
            m_omFunctionList.RemoveAll();
            bResult = TRUE;
        }
        break;
    }
    return bResult;
}

BOOL tagAbsSElement::RemoveComposite(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP /* Category */)
{
    BOOL bResult = FALSE;
    switch (eEntity)
    {
        case TYPE_CCOMPOSITE:
        {
            POSITION pos = GetCompositePos(omID);
            if (pos != nullptr)
            {
                m_omCompositeList.RemoveAt(pos);
            }
            bResult = TRUE;
        }
        break;
        case TYPE_CCOMPOSITELIST:
        {
            m_omCompositeList.RemoveAll();
            bResult = TRUE;
        }
        break;
    }
    return bResult;
}

BOOL tagAbsSElement::RemovePDU(eENTITY_FIBEX eEntity, CString omID, CATEGORY_GROUP /* Category */)
{
    BOOL bResult = FALSE;
    switch (eEntity)
    {
        case TYPE_CPDU:
        {
            POSITION pos = GetPDUPos(omID);
            if (pos != nullptr)
            {
                m_omPDUList.RemoveAt(pos);
            }
            bResult = TRUE;
        }
        break;
        case TYPE_CPDULIST:
        {
            m_omPDUList.RemoveAll();
            bResult = TRUE;
        }
        break;
    }
    return bResult;
}

POSITION tagAbsSElement::GetFramePos(CString omID)
{
    POSITION pos = nullptr;
    int nListCount = m_omFrameList.GetCount();
    for (int i = 0; i < nListCount; i++)
    {
        pos = m_omFrameList.FindIndex(i);
        ABS_FRAME& ouFrame = m_omFrameList.GetAt(pos);
        CString omKey = ouFrame.m_omFrameID;
        if (omKey == omID)
        {
            i = nListCount;
        }
    }
    return pos;
}

POSITION tagAbsSElement::GetCompositePos(CString omID)
{
    POSITION pos = nullptr;
    int nListCount = m_omCompositeList.GetCount();
    for (int i = 0; i < nListCount; i++)
    {
        pos = m_omCompositeList.FindIndex(i);
        ABS_COMPOSITE& ouComp = m_omCompositeList.GetAt(pos);
        CString omKey = ouComp.m_omCompositeID;
        if (omKey == omID)
        {
            i = nListCount;
        }
    }
    return pos;
}

POSITION tagAbsSElement::GetECUPos(CString omID)
{
    POSITION pos = nullptr;
    int nListCount = m_omECUList.GetCount();
    for (int i = 0; i < nListCount; i++)
    {
        pos = m_omECUList.FindIndex(i);
        ABS_ECU& ouEcu = m_omECUList.GetAt(pos);
        CString omKey = ouEcu.m_omECUID;
        if (omKey == omID)
        {
            i = nListCount;
        }
    }
    return pos;
}

POSITION tagAbsSElement::GetChannelPos(CString omID)
{
    POSITION pos = nullptr;
    int nListCount = m_omChannelList.GetCount();
    for (int i = 0; i < nListCount; i++)
    {
        pos = m_omChannelList.FindIndex(i);
        ABS_CHANNEL& ouChannel = m_omChannelList.GetAt(pos);
        CString omKey = ouChannel.m_omChannelID;
        if (omKey == omID)
        {
            i = nListCount;
        }
    }
    return pos;
}
POSITION tagAbsSElement::GetClusterPos(CString omID)
{
    POSITION pos = nullptr;
    int nListCount = m_omClusterList.GetCount();
    for (int i = 0; i < nListCount; i++)
    {
        pos = m_omClusterList.FindIndex(i);
        ABS_CLUSTER& ouCluster = m_omClusterList.GetAt(pos);
        CString omKey = ouCluster.m_omClusterID;
        if (omKey == omID)
        {
            i = nListCount;
        }
    }
    return pos;
}
POSITION tagAbsSElement::GetSignalPos(CString omID)
{
    POSITION pos = nullptr;
    int nListCount = m_omSignalList.GetCount();
    for (int i = 0; i < nListCount; i++)
    {
        pos = m_omSignalList.FindIndex(i);
        ABS_SIGNAL& ouSig = m_omSignalList.GetAt(pos);
        CString omKey = ouSig.m_omSignalID;
        if (omKey == omID)
        {
            i = nListCount;
        }
    }
    return pos;
}
POSITION tagAbsSElement::GetPDUPos(CString omID)
{
    POSITION pos = nullptr;
    int nListCount = m_omPDUList.GetCount();
    for (int i = 0; i < nListCount; i++)
    {
        pos = m_omPDUList.FindIndex(i);
        ABS_PDU& ouPdu = m_omPDUList.GetAt(pos);
        CString omKey = ouPdu.m_omPduID;
        if (omKey == omID)
        {
            i = nListCount;
        }
    }
    return pos;
}
POSITION tagAbsSElement::GetFunctionPos(CString omID)
{
    POSITION pos = nullptr;
    int nListCount = m_omFunctionList.GetCount();
    for (int i = 0; i < nListCount; i++)
    {
        pos = m_omFunctionList.FindIndex(i);
        ABS_FUNCTION& ouFunc = m_omFunctionList.GetAt(pos);
        CString omKey = ouFunc.m_omFunctionID;
        if (omKey == omID)
        {
            i = nListCount;
        }
    }
    return pos;
}

void tagAbsSUnit_Spec::DoCleanup()
{
    m_odUnitList.RemoveAll();
}

void tagAbsSProcInfo::DoCleanup()
{
    m_ouUnitSpec.DoCleanup();
    m_omCodingList.RemoveAll();
}

void tagAbsSProject::DoCleanup()
{
    m_omID = EMPTY_STRING;
    m_omProjectName = EMPTY_STRING;
}

void tagAbsSFibexContainer::DoCleanup()
{
    m_omProject.DoCleanup();
    m_omProtocol.DoCleanup();
    m_omElement.DoCleanup();
    m_omProcInfo.DoCleanup();
    m_omRequirements.DoCleanup();

}
//tagAbsSPDUTRIGGTIMING::tagAbsSPDUTRIGGTIMING()
//{
//    m_byTimeMode = 0x0;
//   // m_psCyclicTiming = nullptr;
//    /*m_psEventControlledTiming = nullptr;
//    m_psRequestControlledTiming = nullptr;*/
//
//}
tagAbsSPDUTRIGGTIMING& tagAbsSPDUTRIGGTIMING::operator=(tagAbsSPDUTRIGGTIMING& RefObj)
{
    m_byTimeMode = RefObj.m_byTimeMode;

    m_psCyclicTiming = RefObj.m_psCyclicTiming;
    m_psEventControlledTiming = (RefObj.m_psEventControlledTiming);
    m_psRequestControlledTiming = (RefObj.m_psRequestControlledTiming);


    /*  if (RefObj.m_psCyclicTiming != nullptr)
      {
          if (m_psCyclicTiming == nullptr)
          {
              m_psCyclicTiming = new ABS_CYCLIC_TIMING();
          }
          *m_psCyclicTiming = *(RefObj.m_psCyclicTiming);
      }*/
    /* if (RefObj.m_psEventControlledTiming != nullptr)
     {
         if (m_psEventControlledTiming == nullptr)
         {
             m_psEventControlledTiming = new ABS_EVENT_CONTROLLED_TIMING();
         }
         *m_psEventControlledTiming = *(RefObj.m_psEventControlledTiming);
     }*/
    /* if (RefObj.m_psRequestControlledTiming != nullptr)
     {
         if (m_psRequestControlledTiming == nullptr)
         {
             m_psRequestControlledTiming = new ABS_REQUEST_CONTROLLED_TIMING();
         }
         *m_psRequestControlledTiming = *(RefObj.m_psRequestControlledTiming);
     }*/
    return *this;
}
//tagAbsSPDUTRIGGTIMING::~tagAbsSPDUTRIGGTIMING()
//{
//    /*if (m_psCyclicTiming != nullptr)
//    {
//        delete m_psCyclicTiming;
//        m_psCyclicTiming = nullptr;
//    }*/
//  /*  if (m_psEventControlledTiming != nullptr)
//    {
//        delete m_psEventControlledTiming;
//        m_psEventControlledTiming = nullptr;
//    }
//    if (m_psRequestControlledTiming != nullptr)
//    {
//        delete m_psRequestControlledTiming;
//        m_psRequestControlledTiming = nullptr;
//    }*/
//}

//requirements
void tagAbsSRequirements::DoCleanup()
{
}

//Protocols
void tagAbsSProtocol::DoCleanup()
{
}