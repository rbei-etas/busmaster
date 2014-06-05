#include "Stdafx.h"
#include "FIBEX_Structs.h"

CFrame::CFrame()
{
}

CFrame::~CFrame()
{
}

CFrame& CFrame::operator=(CFrame& RefObj)
{
    m_omFrameId = RefObj.m_omFrameId;
    m_omFrameType = RefObj.m_omFrameType;
    m_omShortName = RefObj.m_omShortName;
    m_unFrameLen = RefObj.m_unFrameLen;
    m_omCurrPduId = RefObj.m_omCurrPduId;
    m_omCurrSigInstId = RefObj.m_omCurrSigInstId;
    m_objPdu = RefObj.m_objPdu;

    m_mapPduInstList = RefObj.m_mapPduInstList;
    m_mapSigInstDetails = RefObj.m_mapSigInstDetails;
    return *this;
}

CFrame::CFrame(CFrame& RefObj)
{
    m_omFrameId = RefObj.m_omFrameId;
    m_omFrameType = RefObj.m_omFrameType;
    m_omShortName = RefObj.m_omShortName;
    m_unFrameLen = RefObj.m_unFrameLen;
    m_omCurrPduId = RefObj.m_omCurrPduId;
    m_omCurrSigInstId = RefObj.m_omCurrSigInstId;
    m_objPdu = RefObj.m_objPdu;

    m_mapPduInstList = RefObj.m_mapPduInstList;
    m_mapSigInstDetails = RefObj.m_mapSigInstDetails;
}

CECU::CECU()
{
    objController.clear();
    objConnector.clear();
}

CECU::~CECU()
{
}

CECU& CECU::operator=(CECU& Refobj)
{
    m_omECUId = Refobj.m_omECUId;
    m_omShortName = Refobj.m_omShortName;

    m_strCurrConnectorRef = Refobj.m_strCurrConnectorRef;
    m_strCurrControllerRef = Refobj.m_strCurrConnectorRef;

    m_mapConnector = Refobj.m_mapConnector;
    m_mapController = Refobj.m_mapController;

    objConnector = Refobj.objConnector;
    objController = Refobj.objController;
    return *this;
}

CECU::CECU(CECU& RefObj)
{
    m_omECUId = RefObj.m_omECUId;
    m_omShortName = RefObj.m_omShortName;
    m_strCurrConnectorRef = RefObj.m_strCurrConnectorRef;
    m_strCurrControllerRef = RefObj.m_strCurrConnectorRef;

    m_mapConnector = RefObj.m_mapConnector;
    m_mapController = RefObj.m_mapController;

    objConnector = RefObj.objConnector;
    objController = RefObj.objController;
}

CController::CController()
{
    m_sKeySlotUsage.m_nStartUpSync  = 0;
    m_sKeySlotUsage.m_nSync = 0;
    m_sKeySlotUsage.m_omNone = "";
    m_shMaxDynPayloadLgt  = 0;
    m_nKeySlotId = 0;
    m_shClusterDriftDamping  = 0;
    m_nDecodingCorr  = 0;
    m_nListenTimeOut  = 0;
    m_shMaxDrift  = 0;
    m_shExternOffsetCorr  = 0;
    m_shExternRateCorr  = 0;
    m_shLatestTx  = 0;
    m_nMicroPreCycle  = 0;
    m_shOffsetCorrOut  = 0;
    m_shRateCorrOut  = 0;
    m_shSamplePerMicrotick  = 0;
    m_shDelayCompensationA  = 0;
    m_shDelayCompensationB  = 0;
    m_shWakeUpPattern  = 0;
    m_bAllowHaltDewToClock  = false;
    m_shAllowPassiveToActive  = 0;
    m_shAcceptedStartUpRange  = 0;
    m_shMacroInitialOffsetA  = 0;
    m_shMacroInitialOffsetB  = 0;
    m_shMicroInitialOffsetA  = 0;
    m_shMicroInitialOffsetB  = 0;
    m_bSingleSlotEnable  = false;
    m_fMicrotick  = 0;
    m_fMicroPerMacroNom  = 0;
}

CController::~CController()
{
}

CController& CController::operator=(CController& Refobj)
{
    m_sKeySlotUsage.m_nStartUpSync = Refobj.m_sKeySlotUsage.m_nStartUpSync;
    m_sKeySlotUsage.m_nSync = Refobj.m_sKeySlotUsage.m_nSync;
    m_sKeySlotUsage.m_omNone = Refobj.m_sKeySlotUsage.m_omNone;
    m_shMaxDynPayloadLgt = Refobj.m_shMaxDynPayloadLgt;
    m_shClusterDriftDamping = Refobj.m_shClusterDriftDamping;
    m_nDecodingCorr= Refobj.m_nDecodingCorr;
    m_nListenTimeOut= Refobj.m_nListenTimeOut;
    m_shMaxDrift= Refobj.m_shMaxDrift;
    m_shExternOffsetCorr= Refobj.m_shExternOffsetCorr;
    m_shExternRateCorr= Refobj.m_shExternRateCorr;
    m_shLatestTx= Refobj.m_shLatestTx;
    m_nMicroPreCycle= Refobj.m_nMicroPreCycle;
    m_shOffsetCorrOut= Refobj.m_shOffsetCorrOut;
    m_shRateCorrOut= Refobj.m_shRateCorrOut;
    m_shSamplePerMicrotick = Refobj.m_shSamplePerMicrotick;
    m_shDelayCompensationA= Refobj.m_shDelayCompensationA;
    m_shDelayCompensationB= Refobj.m_shDelayCompensationB;
    m_shWakeUpPattern= Refobj.m_shWakeUpPattern;
    m_bAllowHaltDewToClock= Refobj.m_bAllowHaltDewToClock;
    m_shAllowPassiveToActive = Refobj.m_shAllowPassiveToActive;
    m_shAcceptedStartUpRange = Refobj.m_shAcceptedStartUpRange;
    m_shMacroInitialOffsetA = Refobj.m_shMacroInitialOffsetA;
    m_shMacroInitialOffsetB= Refobj.m_shMacroInitialOffsetB;
    m_shMicroInitialOffsetA= Refobj.m_shMicroInitialOffsetA;
    m_shMicroInitialOffsetB= Refobj.m_shMicroInitialOffsetB;
    m_bSingleSlotEnable= Refobj.m_bSingleSlotEnable;
    m_fMicrotick= Refobj.m_fMicrotick;
    m_fMicroPerMacroNom= Refobj.m_fMicroPerMacroNom;
    m_nChannels= Refobj.m_nChannels;
    m_nKeySlotId= Refobj.m_nKeySlotId;
    m_nWakeUpChannel= Refobj.m_nWakeUpChannel;
    return *this;
}

CController::CController(CController& Refobj)
{
    m_sKeySlotUsage.m_nStartUpSync = Refobj.m_sKeySlotUsage.m_nStartUpSync;
    m_sKeySlotUsage.m_nSync = Refobj.m_sKeySlotUsage.m_nSync;
    m_sKeySlotUsage.m_omNone = Refobj.m_sKeySlotUsage.m_omNone;
    m_shMaxDynPayloadLgt = Refobj.m_shMaxDynPayloadLgt;
    m_shClusterDriftDamping = Refobj.m_shClusterDriftDamping;
    m_nDecodingCorr= Refobj.m_nDecodingCorr;
    m_nListenTimeOut= Refobj.m_nListenTimeOut;
    m_shMaxDrift= Refobj.m_shMaxDrift;
    m_shExternOffsetCorr= Refobj.m_shExternOffsetCorr;
    m_shExternRateCorr= Refobj.m_shExternRateCorr;
    m_shLatestTx= Refobj.m_shLatestTx;
    m_nMicroPreCycle= Refobj.m_nMicroPreCycle;
    m_shOffsetCorrOut= Refobj.m_shOffsetCorrOut;
    m_shRateCorrOut= Refobj.m_shRateCorrOut;
    m_shSamplePerMicrotick = Refobj.m_shSamplePerMicrotick;
    m_shDelayCompensationA= Refobj.m_shDelayCompensationA;
    m_shDelayCompensationB= Refobj.m_shDelayCompensationB;
    m_shWakeUpPattern= Refobj.m_shWakeUpPattern;
    m_bAllowHaltDewToClock= Refobj.m_bAllowHaltDewToClock;
    m_shAllowPassiveToActive = Refobj.m_shAllowPassiveToActive;
    m_shAcceptedStartUpRange = Refobj.m_shAcceptedStartUpRange;
    m_shMacroInitialOffsetA = Refobj.m_shMacroInitialOffsetA;
    m_shMacroInitialOffsetB= Refobj.m_shMacroInitialOffsetB;
    m_shMicroInitialOffsetA= Refobj.m_shMicroInitialOffsetA;
    m_shMicroInitialOffsetB= Refobj.m_shMicroInitialOffsetB;
    m_bSingleSlotEnable= Refobj.m_bSingleSlotEnable;
    m_fMicrotick= Refobj.m_fMicrotick;
    m_fMicroPerMacroNom= Refobj.m_fMicroPerMacroNom;
    m_nChannels= Refobj.m_nChannels;
    m_nKeySlotId= Refobj.m_nKeySlotId;
    m_nWakeUpChannel= Refobj.m_nWakeUpChannel;
}

CConnector::CConnector()
{
    m_omChnlRef = "";
    m_omCntrlRef = "";

    m_mapRxFrameTrigs.clear();
    m_mapTxFrameTrigs.clear();

}

CConnector::~CConnector()
{
}

CConnector& CConnector::operator=(CConnector& objRef)
{
    m_omChnlRef = objRef.m_omChnlRef;
    m_omCntrlRef = objRef.m_omCntrlRef;

    m_mapRxFrameTrigs =  (objRef.m_mapRxFrameTrigs);
    m_mapTxFrameTrigs = (objRef.m_mapTxFrameTrigs);

    return *this;
}

CConnector::CConnector(CConnector& objRef)
{
    m_omChnlRef = objRef.m_omChnlRef;
    m_omCntrlRef = objRef.m_omCntrlRef;

    m_mapRxFrameTrigs =  (objRef.m_mapRxFrameTrigs);
    m_mapTxFrameTrigs = (objRef.m_mapTxFrameTrigs);
}