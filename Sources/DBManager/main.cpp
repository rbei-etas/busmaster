// LDF_Generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LDFGeneratorFactory.h"
#include "ICluster.h"

void SimulateLDFFile(LDFCluster& ouCluster);

#define PARSING_REQUIRED

/*int _tmain(int argc, _TCHAR* argv[])
{
    LDFCluster ouCluster;
    std::string strLoadFile = argv[1];

#ifdef PARSING_REQUIRED
    ouCluster.LoadFromFile(strLoadFile);

#else
    SimulateLDFFile(ouCluster);

#endif // PARSING_REQUIRED


    std::string strSaveFilePath = argv[2];

    ouCluster.SaveToFile(strSaveFilePath);
}*/

void SimulateLDFFile(LDFCluster& ouCluster)
{
    //std::string strName = "";
    //EcuProperties ouEcuProperties;
    //UID_ELEMENT nSeatEcu, nMotor1, nMotor2;
    //IEcu* pElement;

    //// Cluster settings
    //Protocolsettings ouSettings;
    //ouSettings.m_oueProtocolType = eLINProtocol;

    //ouCluster.GetClusterProperties(ouSettings);
    //
    //ouSettings.m_ouLinSettings.m_dProtocolVers = defVersion_2_1;
    //ouSettings.m_ouLinSettings.m_dLanguageVers = defVersion_2_1;
    //ouSettings.m_ouLinSettings.m_dBaudRate = 19.2;
    //ouSettings.m_ouLinSettings.m_strChannelName= "Lin Cluster";

    //ouCluster.SetClusterProperties(ouSettings);

    //ISignal* pSignalElement , *pMotor1ErrroCodeSig, *pMotor1ErrorVal, *pMotor1Selectionsig, *pMotor1LinErr;
    //ouCluster.CreateElement(eSignalElement, (IElement**)&pMotor1ErrroCodeSig);
    //strName = "Motor1ErrorCode";
    //pMotor1ErrroCodeSig->SetName(strName);
    //UID_ELEMENT nMotor1ErrorCode;
    //pMotor1ErrroCodeSig->GetUniqueId(nMotor1ErrorCode);

    //ouCluster.CreateElement(eSignalElement, (IElement**)&pMotor1ErrorVal);
    //strName = "Motor1ErrorValue";
    //pMotor1ErrorVal->SetName(strName);
    //UID_ELEMENT nMotor1ErrorValue;
    //pMotor1ErrorVal->GetUniqueId(nMotor1ErrorValue);

    ////Master- SeatECU
    //ouCluster.CreateElement(eEcuElement, (IElement**)&pElement);
    //pElement->GetProperties(ouEcuProperties);
    //ouEcuProperties.m_eEcuType = eLIN_Master;
    //
    //ouEcuProperties.m_ouMasterProps.m_fJitter = (double)10.0;
    //ouEcuProperties.m_ouMasterProps.m_fTimeBase = (double)5;
    //strName = "SeatECU";
    //pElement->SetName(strName);
    //pElement->SetProperties(ouEcuProperties);
    //pElement->GetUniqueId(nSeatEcu);

    //ouCluster.CreateElement(eSignalElement, (IElement**)&pMotor1Selectionsig);
    //strName = "Motor1Selection";
    //pMotor1Selectionsig->SetName(strName);
    //UID_ELEMENT nMotor1Selection;
    //pMotor1Selectionsig->GetUniqueId(nMotor1Selection);

    //ouCluster.CreateElement(eSignalElement, (IElement**)&pMotor1LinErr);
    //strName = "Motor1LinError";
    //pMotor1LinErr->SetName(strName);
    //UID_ELEMENT nMotor1LinError;
    //pMotor1LinErr->GetUniqueId(nMotor1LinError);

    //IFrame* pMotor1Controlframe, *pMotor1State_CyclFrame;
    //ouCluster.CreateElement(eFrameElement, (IElement**)&pMotor1Controlframe);
    //strName = "Motor1Control";
    //pMotor1Controlframe->SetName(strName);
    //UID_ELEMENT nMotor1Control;
    //pMotor1Controlframe->GetUniqueId(nMotor1Control);

    //ouCluster.CreateElement(eFrameElement, (IElement**)&pMotor1State_CyclFrame);
    //strName = "Motor1State_Cycl";
    //pMotor1State_CyclFrame->SetName(strName);
    //UID_ELEMENT nMotor1State_Cycl;
    //pMotor1State_CyclFrame->GetUniqueId(nMotor1State_Cycl);

    //IFrame* pMotorsControl;
    //ouCluster.CreateElement(eFrameElement, (IElement**)&pMotorsControl);
    //strName = "MotorsControl";
    //pMotorsControl->SetName(strName);
    //UID_ELEMENT nMotorsControl;
    //pMotorsControl->GetUniqueId(nMotorsControl);

    //IFrame* pETF_MotorStates;
    //ouCluster.CreateElement(eFrameElement, (IElement**)&pETF_MotorStates);
    //strName = "ETF_MotorStates";
    //pETF_MotorStates->SetName(strName);
    //UID_ELEMENT nETF_MotorStates;
    //pMotor1Controlframe->GetUniqueId(nETF_MotorStates);
    //
    ////Slave - Motor1
    //ouCluster.CreateElement(eEcuElement, (IElement**)&pElement);
    //pElement->GetProperties(ouEcuProperties);
    //ouEcuProperties.m_eEcuType = eLIN_Slave;
    //
    //ouEcuProperties.m_ouSlavePros.m_fProtocolVersion = (double)2.1;
    //strName = "Motor1";
    //pElement->SetName(strName);
    //ouEcuProperties.m_ouSlavePros.m_fProtocolVersion = defVersion_2_1;
    //ouEcuProperties.m_ouSlavePros.m_nConfiguredNAD = 0x2;
    //ouEcuProperties.m_ouSlavePros.m_nInitialNAD = 0xA;
    //ouEcuProperties.m_ouSlavePros.m_nSupplierId = 0x1E;
    //ouEcuProperties.m_ouSlavePros.m_nVariant = 0x0;
    //ouEcuProperties.m_ouSlavePros.m_nFunctionId = 0x1;
    //ouEcuProperties.m_ouSlavePros.m_nRespErrSignal = (int)nMotor1LinError;
    //
    //ouEcuProperties.m_ouSlavePros.m_nFaultStateSignals.push_back(nMotor1ErrorValue);
    //ouEcuProperties.m_ouSlavePros.m_nFaultStateSignals.push_back(nMotor1ErrorCode);
    //ouEcuProperties.m_ouSlavePros.m_dP2Min = 100;
    //ouEcuProperties.m_ouSlavePros.m_dSTMin = 20;
    //ouEcuProperties.m_ouSlavePros.m_dNASTimeout = 1000;
    //ouEcuProperties.m_ouSlavePros.m_dNCRTimeout = 1000;
    //pElement->MapFrame(eTx, nMotor1Control);
    //pElement->MapFrame(eTx, nMotor1State_Cycl);

    //pElement->SetProperties(ouEcuProperties);
    //pElement->GetUniqueId(nMotor1);

    ////Slave - Motor2
    //ouCluster.CreateElement(eEcuElement, (IElement**)&pElement);
    //pElement->GetProperties(ouEcuProperties);
    //ouEcuProperties.m_eEcuType = eLIN_Slave;
    //
    //
    //ouEcuProperties.m_ouSlavePros.m_fProtocolVersion = (double)2.1;
    //strName = "Motor2";
    //pElement->SetName(strName);
    //ouEcuProperties.m_ouSlavePros.m_fProtocolVersion = defVersion_2_1;
    //ouEcuProperties.m_ouSlavePros.m_nConfiguredNAD = 0x3;
    //ouEcuProperties.m_ouSlavePros.m_nInitialNAD = 0xC;
    //ouEcuProperties.m_ouSlavePros.m_nSupplierId = 0x2E;
    //ouEcuProperties.m_ouSlavePros.m_nVariant = 0x1;
    //ouEcuProperties.m_ouSlavePros.m_nFunctionId = 0xB;
    //ouEcuProperties.m_ouSlavePros.m_nRespErrSignal = nMotor1LinError;
    //
    //ouEcuProperties.m_ouSlavePros.m_nFaultStateSignals.push_back(nMotor1ErrorValue);
    //ouEcuProperties.m_ouSlavePros.m_nFaultStateSignals.push_back(nMotor1ErrorCode);
    //ouEcuProperties.m_ouSlavePros.m_dP2Min = 50;
    //ouEcuProperties.m_ouSlavePros.m_dSTMin = 0;
    //ouEcuProperties.m_ouSlavePros.m_dNASTimeout = 1000;
    //ouEcuProperties.m_ouSlavePros.m_dNCRTimeout = 1000;
    //pElement->MapFrame(eTx, nMotor1Control);
    //pElement->MapFrame(eTx, nMotor1State_Cycl);
    //pElement->SetProperties(ouEcuProperties);
    //pElement->GetUniqueId(nMotor2);

    //// Signals

    //// Signal1 = Motor1ErrorCode
    //
    //SignalProps ouProps;
    //ouProps.eType = eLINProtocol;
    //
    //ouProps.m_ouLINSignalProps.m_nLength = 8;
    //ouProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    //ouProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    //ouProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    //ouProps.m_ouLINSignalProps.m_nIntialValue = 5;
    //ouProps.m_ouLINSignalProps.m_ouSignalType = eSignalNormal;
    //pMotor1ErrroCodeSig->SetProperties(ouProps);
    //pMotor1ErrroCodeSig->MapNode(eTx, nMotor1);
    //pMotor1ErrroCodeSig->MapNode(eRx, nSeatEcu);
    //

    //// Signal2 = Motor1ErrorVal
    //ouProps.eType = eLINProtocol;
    //ouProps.m_ouLINSignalProps.m_nLength = 8;
    //ouProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    //ouProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    //ouProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    //ouProps.m_ouLINSignalProps.m_nIntialValue= 1;
    //ouProps.m_ouLINSignalProps.m_ouSignalType = eSignalNormal;
    //pMotor1ErrorVal->SetProperties(ouProps);
    //pMotor1ErrorVal->MapNode(eTx, nMotor1);
    //pMotor1ErrorVal->MapNode(eRx, nSeatEcu);
    //
    //// Signal3 = Motor1LinErr
    //

    //ouProps.eType = eLINProtocol;
    //ouProps.m_ouLINSignalProps.m_nLength = 1;
    //ouProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    //ouProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    //ouProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    //ouProps.m_ouLINSignalProps.m_nIntialValue = 0;
    //ouProps.m_ouLINSignalProps.m_ouSignalType = eSignalNormal;
    //pMotor1LinErr->SetProperties(ouProps);
    //pMotor1LinErr->MapNode(eTx, nMotor1);
    //pMotor1LinErr->MapNode(eRx, nSeatEcu);
    //

    //// Signal3 = Motor1Position
    //ouCluster.CreateElement(eSignalElement, (IElement**)&pSignalElement);
    //strName = "Motor1Position";
    //pSignalElement->SetName(strName);
    //
    //ouProps.eType = eLINProtocol;
    //ouProps.m_ouLINSignalProps.m_nLength = 32;
    //ouProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    //ouProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    //ouProps.m_ouLINSignalProps.m_ouValueType = eByteArray;
    //ouProps.m_ouLINSignalProps.m_nIntialValue = 0x1020304;
    //ouProps.m_ouLINSignalProps.m_ouSignalType = eSignalNormal;
    //pSignalElement->SetProperties(ouProps);
    //pSignalElement->MapNode(eTx, nMotor1);
    //pSignalElement->MapNode(eRx, nSeatEcu);


    //// Signal3 = Motor1Selection
    //
    //ouProps.eType = eLINProtocol;
    //ouProps.m_ouLINSignalProps.m_nLength = 2;
    //ouProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    //ouProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    //ouProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    //ouProps.m_ouLINSignalProps.m_nIntialValue = 0;
    //ouProps.m_ouLINSignalProps.m_ouSignalType = eSignalNormal;
    //pMotor1Selectionsig->SetProperties(ouProps);
    //pMotor1Selectionsig->MapNode(eTx, nMotor1);
    //pMotor1Selectionsig->MapNode(eRx, nSeatEcu);
    //

    //// Signal3 = Motor1Temp
    //ouCluster.CreateElement(eSignalElement, (IElement**)&pSignalElement);
    //strName = "Motor1Temp";
    //pSignalElement->SetName(strName);
    //
    //ouProps.eType = eLINProtocol;
    //ouProps.m_ouLINSignalProps.m_nLength = 8;
    //ouProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    //ouProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    //ouProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    //ouProps.m_ouLINSignalProps.m_nIntialValue = 5;
    //ouProps.m_ouLINSignalProps.m_ouSignalType = eSignalNormal;
    //pSignalElement->SetProperties(ouProps);
    //pSignalElement->MapNode(eTx, nMotor1);
    //pSignalElement->MapNode(eRx, nSeatEcu);

    //// Signal3 = Motor1Control_NewSignal
    //ouCluster.CreateElement(eSignalElement, (IElement**)&pSignalElement);
    //strName = "Motor1Control_NewSignal";
    //pSignalElement->SetName(strName);
    //
    //ouProps.eType = eLINProtocol;
    //ouProps.m_ouLINSignalProps.m_nLength = 8;
    //ouProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    //ouProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    //ouProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    //ouProps.m_ouLINSignalProps.m_nIntialValue = 5;
    //ouProps.m_ouLINSignalProps.m_ouSignalType = eSignalNormal;
    //pSignalElement->SetProperties(ouProps);
    //pSignalElement->MapNode(eTx, nMotor1);
    //pSignalElement->MapNode(eRx, nSeatEcu);

    //UID_ELEMENT nMotor1Control_NewSignal;
    //pSignalElement->GetUniqueId(nMotor1Control_NewSignal);

    //// Signal4
    //ouCluster.CreateElement(eSignalElement, (IElement**)&pSignalElement);
    //strName = "Motor2Temp";
    //pSignalElement->SetName(strName);
    //
    //ouProps.eType = eLINProtocol;
    //ouProps.m_ouLINSignalProps.m_nLength = 2;
    //ouProps.m_ouLINSignalProps.m_ouDataType = eUnsigned;
    //ouProps.m_ouLINSignalProps.m_ouEndianess = eIntel;
    //ouProps.m_ouLINSignalProps.m_ouValueType = eScalar;
    //ouProps.m_ouLINSignalProps.m_nIntialValue = 0;
    //ouProps.m_ouLINSignalProps.m_ouSignalType = eSignalNormal;
    //pSignalElement->SetProperties(ouProps);
    //pSignalElement->MapNode(eTx, nMotor1);
    //pSignalElement->MapNode(eRx, nSeatEcu);
    //UID_ELEMENT nMotor1Temp;
    //pSignalElement->GetUniqueId(nMotor1Temp);

    ////Frame
    //
    //FrameProps ouFrameProps;
    //ouFrameProps.m_eFrameType = eLIN_Unconditional;
    //
    //ouFrameProps.m_ouLINUnConditionFrameProps.m_nLength = 1;
    //ouFrameProps.m_ouLINUnConditionFrameProps.m_unId = 51;
    //pMotor1Controlframe->SetProperties(ouFrameProps);
    //pMotor1Controlframe->MapNode(eTx, nMotor1);
    //pMotor1Controlframe->MapNode(eRx, nMotor1);       //Invalid
    //SignalInstanse ouSignalInstance;

    //ouSignalInstance.m_nStartBit = 0;
    //pMotor1Controlframe->MapSignal(nMotor1Selection, ouSignalInstance);
    //
    //ouSignalInstance.m_nStartBit = 3;
    //pMotor1Controlframe->MapSignal(nMotor1Control_NewSignal, ouSignalInstance);
    //

    ////Frame2
    //
    //ouFrameProps.m_eFrameType = eLIN_Unconditional;
    //ouFrameProps.m_ouLINUnConditionFrameProps.m_nLength = 1;
    //ouFrameProps.m_ouLINUnConditionFrameProps.m_unId = 48;
    //pMotor1State_CyclFrame->SetProperties(ouFrameProps);
    //pMotor1State_CyclFrame->MapNode(eTx, nSeatEcu);
    //pMotor1State_CyclFrame->MapNode(eRx, nSeatEcu);       //Invalid

    //ouSignalInstance.m_nStartBit = 0;
    //pMotor1State_CyclFrame->MapSignal(nMotor1Temp, ouSignalInstance);
    //
    //ouSignalInstance.m_nStartBit = 8;
    //pMotor1State_CyclFrame->MapSignal(nMotor1Selection, ouSignalInstance);
    //
    //IFrame *pSporadicFrame;
    //ouCluster.CreateElement(eFrameElement, (IElement**)&pSporadicFrame);
    //strName = "SporadicControlFrame";
    //pSporadicFrame->SetName(strName);
    //
    //ouFrameProps.m_eFrameType = eLIN_Sporadic;
    //
    //
    //ouFrameProps.m_ouLINSporadicFrameProps.m_ouUnconditionalFrame.push_back(nMotor1Control);
    //
    //ouFrameProps.m_ouLINSporadicFrameProps.m_ouUnconditionalFrame.push_back(nMotor1State_Cycl);
    //UID_ELEMENT nSporadicControlFrame;
    //pSporadicFrame->GetUniqueId(nSporadicControlFrame);

    //// Schedule Tables
    //IScheduleTable *pSchedTable;
    //ScheduleTableProps ouSchedProps;
    //CSheduleTableItem ouSchedItem;

    //ouCluster.CreateElement(eScheduleTableElement, (IElement**)&pSchedTable);
    //strName = "NormalTable";
    //pSchedTable->SetName(strName);

    //ouSchedProps.m_strTableName =  "NormalTable";
    //ouSchedItem.m_dDelay = 50;
    //ouSchedItem.m_nFrameId = nMotorsControl;
    //ouSchedProps.m_ouCSheduleTableItem.push_back(ouSchedItem);

    //ouSchedItem.m_dDelay = 50;
    //ouSchedItem.m_nFrameId = nETF_MotorStates;
    //ouSchedProps.m_ouCSheduleTableItem.push_back(ouSchedItem);
    //pSchedTable->SetProperties(ouSchedProps);

    //ouCluster.CreateElement(eScheduleTableElement, (IElement**)&pSchedTable);
    //strName = "ETFCollisionResolving";
    //pSchedTable->SetName(strName);

    //ouSchedProps.m_strTableName =  "ETFCollisionResolving";
    //ouSchedItem.m_dDelay = 50;
    //ouSchedItem.m_nFrameId = nMotor1State_Cycl;
    //ouSchedProps.m_ouCSheduleTableItem.push_back(ouSchedItem);
    //ouSchedItem.m_dDelay = 50;
    //ouSchedItem.m_nFrameId = nMotor1Control;
    //
    //ouSchedProps.m_ouCSheduleTableItem.clear();
    //ouSchedProps.m_ouCSheduleTableItem.push_back(ouSchedItem);
    //pSchedTable->SetProperties(ouSchedProps);

    //ouCluster.CreateElement(eScheduleTableElement, (IElement**)&pSchedTable);
    //strName = "InitTable";
    //pSchedTable->SetName(strName);

    //ouSchedProps.m_strTableName =  "InitTable";
    //ouSchedItem.m_dDelay = 10;
    //ouSchedItem.m_nFrameId = nMotor1Control;
    //ouSchedProps.m_ouCSheduleTableItem.push_back(ouSchedItem);
    //ouSchedItem.m_dDelay = 10;
    //ouSchedItem.m_nFrameId = nMotor1State_Cycl;
    //
    //ouSchedProps.m_ouCSheduleTableItem.clear();
    //ouSchedProps.m_ouCSheduleTableItem.push_back(ouSchedItem);
    //pSchedTable->SetProperties(ouSchedProps);
}