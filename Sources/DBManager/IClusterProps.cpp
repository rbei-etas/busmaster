#include "stdafx.h"
#include <map>
#include <string>
#include <list>
#include "IClusterProps.h"


#define DELETE_AND_CREATE(x, y) if ( x!= nullptr ) delete x; x = new y;

SignalInstanse::SignalInstanse()
{
    m_nStartBit = INVALID_DATA;
}



CSheduleTableItem::CSheduleTableItem()
{
    vInitialiseData();
}

void CSheduleTableItem::vInitialiseData()
{
    //m_strTableName = "";
    m_nFrameId = INVALID_DATA;
    m_dDelay = INVALID_DATA;
    m_eDiagType = eLIN_SID_NONE;
    m_nNode = INVALID_UID_ELEMENT;
    memset(m_chDataBytes, 0, sizeof(m_chDataBytes));
}


LINMasterProps::LINMasterProps()
{
    vInitialiseData();
}

void LINMasterProps::vInitialiseData()
{
    m_fJitter = 0.0;
    m_fTimeBase = 1;
}

LINSlaveProps::LINSlaveProps()
{
    vInitialiseData();
}

void LINSlaveProps::vInitialiseData()
{
    m_fProtocolVersion=INVALID_DATA;
    m_dNASTimeout=1000;
    m_dNCRTimeout=1000;
    m_dP2Min=50;
    m_dSTMin=0;
    m_nConfiguredNAD=INVALID_DATA;
    m_nInitialNAD=INVALID_DATA;
    m_nFunctionId=INVALID_DATA;
    m_nSupplierId=INVALID_DATA;
    m_nVariant=INVALID_DATA;
    m_nFaultStateSignals.clear();
    m_nRespErrSignal=INVALID_UID_ELEMENT;
}

EcuProperties::EcuProperties()
{
    m_eEcuType = eNone;
}

FrameProps::FrameProps()
{
    m_eFrameType = eFrame_Invalid;
}

//ERRORCODE LINEventTrigFrameProps::GetUnconditionalFrameList(std::list<IFrame*>& lstFrame)
//{
//  for(auto itr : m_pouUnconditionalFrame)
//  {
//      lstFrame.push_back(itr.second);
//  }
//
//  return EC_SUCCESS;
//}
//
//
//ERRORCODE LINSporadicFrameProps::GetUnconditionalFrameList(std::list<IFrame*>& lstFrame)
//{
//  for(auto itr : m_pouUnconditionalFrame)
//  {
//      lstFrame.push_back(itr.second);
//  }
//
//  return EC_SUCCESS;
//}

LINSignalProps::LINSignalProps()
{
    m_nLength = INVALID_DATA;
    m_ouSignalType = eSignalNormal;
    m_ouDataType = eUnsigned;
    m_ouEndianess = eIntel;
    m_ouValueType = eScalar;
    m_nIntialValue = 0;
}



SignalProps::SignalProps()
{
    eType = eInvalidProtocol;
}

PhysicalValue::PhysicalValue()
{
    m_dOffset = static_cast<double>(INVALID_DATA);
    m_dFactor = static_cast<double>(INVALID_DATA);
    m_unMin = INVALID_DATA;
    m_unMax = INVALID_DATA;
    m_strUnit = "";
}

LINCompuMethods::LINCompuMethods()
{
    //m_ouLogicalValueList.clear();         //Not Required
    //m_ouPhysicalValueList.clear();
    m_bBCD = false;
    m_bASCII = false;
}

LINCompuMethods::LINCompuMethods(const LINCompuMethods& ouObject)
{
    m_ouLogicalValueList  = ouObject.m_ouLogicalValueList;
    m_ouPhysicalValueList = ouObject.m_ouPhysicalValueList;
    m_bBCD = ouObject.m_bBCD;
    m_bASCII = ouObject.m_bASCII;
}



CompuMethodProps::CompuMethodProps()
{
    m_eType = eInvalidProtocol;
}

void LIN_Settings::clear()
{
    m_dProtocolVers = INVALID_DATA;
    m_dLanguageVers = INVALID_DATA;
    m_dBaudRate = INVALID_DATA;
    m_strChannelName = EMPTY_STRING;
    m_strHeaderComment = EMPTY_STRING;
}

LIN_Settings::LIN_Settings()
{
    clear();
}
