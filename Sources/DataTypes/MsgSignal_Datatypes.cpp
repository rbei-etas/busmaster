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
 * \file      MsgSignal_Datatypes.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "Datatypes_stdafx.h"
#include "Include/BaseDefs.h"
#include "Include/Utils_macro.h"
#include "MsgSignal_Datatypes.h"

// Starts CSignalDescVal
CSignalDescVal::CSignalDescVal()
{
    m_omStrSignalDescriptor = "";
    m_n64SignalVal = 0x0;
    m_pouNextSignalSignalDescVal = NULL;
}

CSignalDescVal::~CSignalDescVal()
{
}

void CSignalDescVal::vClearNext(void)
{
    // This function clears all the succeeding entries
    if (NULL != m_pouNextSignalSignalDescVal)
    {
        m_pouNextSignalSignalDescVal->vClearNext();
        delete m_pouNextSignalSignalDescVal;
        m_pouNextSignalSignalDescVal = NULL;
    }
}

CSignalDescVal& CSignalDescVal::operator=(const CSignalDescVal& RefObj)
{
    vClearNext();
    m_omStrSignalDescriptor = RefObj.m_omStrSignalDescriptor;
    m_n64SignalVal          = RefObj.m_n64SignalVal;
    /* This is the case of copying a linked list. Recursion would have been the
    simplest approach. Howsoever, an iterative approach has been adopted with a
    view to realising something new and avoiding defining some more functions*/
    /* The technique of double pointer in order to avoid usage of multiple poi-
    nters. The double pointer contains value of the current 'next' pointer. We
    start by storing address of the 'next' variable of current node. */
    CSignalDescVal** ppouCurrTgt = &m_pouNextSignalSignalDescVal;

    // Clearly, we must go on until the end of the reference linked list.
    for (CSignalDescVal* pouCurrSrc = RefObj.m_pouNextSignalSignalDescVal;
            pouCurrSrc != NULL;
            pouCurrSrc = pouCurrSrc->m_pouNextSignalSignalDescVal)
    {
        (*ppouCurrTgt) = new CSignalDescVal; // Indirect value assigning to 'next'
        (*ppouCurrTgt)->m_omStrSignalDescriptor = pouCurrSrc->m_omStrSignalDescriptor;
        (*ppouCurrTgt)->m_n64SignalVal = pouCurrSrc->m_n64SignalVal;
        // Now it should point to the 'next' of current node.
        ppouCurrTgt = &((*ppouCurrTgt)->m_pouNextSignalSignalDescVal);
    }

    return *this;
}
// Ends CSignalDescVal

// Starts sWaveformInfo
sWaveformInfo::sWaveformInfo(): m_eSignalWaveType(eWave_NONE),
    m_fAmplitude(0.0), m_fFrequency(0.0), m_fGranularity(0.0)
{
}

sWaveformInfo& sWaveformInfo::operator=(const sWaveformInfo& RefObj)
{
    m_eSignalWaveType   = RefObj.m_eSignalWaveType;
    m_fAmplitude        = RefObj.m_fAmplitude;
    m_fFrequency        = RefObj.m_fFrequency;
    m_fGranularity      = RefObj.m_fGranularity;
    return *this;
}

CString sWaveformInfo::omGetWaveformName(eWAVEFORMTYPE eWaveform)
{
    CString Result = "";

    switch (eWaveform)
    {
        case eWave_SINE:
            Result = _T("Sine wave");
            break;

        case eWave_TRIANGLE:
            Result = _T("Triangular wave");
            break;

        case eWave_COS:
            Result = _T("Cos wave");
            break;
    }

    return Result;
}

// Ends sWaveformInfo

// Starts sSigWaveMap
sSigWaveMap::sSigWaveMap()
{
    m_omSigName = "";
}

sSigWaveMap& sSigWaveMap::operator=(const sSigWaveMap& RefObj)
{
    m_omSigName = RefObj.m_omSigName;
    sWaveInfo   = RefObj.sWaveInfo;
    return *this;
}

BOOL sSigWaveMap::operator==(const sSigWaveMap& RefObj) const
{
    return (m_omSigName == RefObj.m_omSigName);
}
// Ends sSigWaveMap


// Starts SSigGeneration
SSigGeneration::SSigGeneration()
{
    Reset();
}
SSigGeneration::~SSigGeneration()
{
    Reset();
}

void SSigGeneration::Reset(void)
{
    m_nMsgID = 0x0;
    m_fDefaultAmplitude = (float) 0.0;
    m_omSigWaveMapList.RemoveAll();
}

BOOL SSigGeneration::operator==(const SSigGeneration& RefObj) const
{
    return (m_nMsgID == RefObj.m_nMsgID);
}

SSigGeneration& SSigGeneration::operator=(const SSigGeneration& RefObj)
{
    Reset();
    m_nMsgID = RefObj.m_nMsgID;
    m_fDefaultAmplitude = RefObj.m_fDefaultAmplitude;
    POSITION Pos = RefObj.m_omSigWaveMapList.GetHeadPosition();

    while (NULL != Pos)
    {
        const sSigWaveMap& CurrObj = RefObj.m_omSigWaveMapList.GetNext(Pos);
        sSigWaveMap TmpObj = CurrObj;
        m_omSigWaveMapList.AddTail(TmpObj);
    }

    return *this;
}
// Ends SSigGeneration


// Starts static functions of sSIGNALS

UINT64 sSIGNALS::un64GetBitMask(sSIGNALS* CurrSig)
{
    UINT64 Result = 0x1;
    // First make the required number of bits (m_unSignalLength) up.
    Result <<= CurrSig->m_unSignalLength;
    --Result; // These bits are now up.
    // Then shift them to the appropriate place.
    short Shift = (DATA_FORMAT_INTEL == CurrSig->m_eFormat) ?
                  ((short)CurrSig->m_unStartByte - 1) * 8 + CurrSig->m_byStartBit
                  : 64 - ((short)CurrSig->m_unStartByte * 8 - CurrSig->m_byStartBit);
    Result <<= Shift;

    if (DATA_FORMAT_MOTOROLA == CurrSig->m_eFormat)
    {
        BYTE* pbStr = (BYTE*) &Result;
        BYTE bTmp = 0x0;
        bTmp = pbStr[0];
        pbStr[0] = pbStr[7] ;
        pbStr[7] = bTmp;
        bTmp = pbStr[1];
        pbStr[1] = pbStr[6] ;
        pbStr[6] = bTmp;
        bTmp = pbStr[2];
        pbStr[2] = pbStr[5] ;
        pbStr[5] = bTmp;
        bTmp = pbStr[3];
        pbStr[3] = pbStr[4] ;
        pbStr[4] = bTmp;
    }

    return Result;
}

void sSIGNALS::vSetSignalValue(sSIGNALS* pouCurrSignal, UCHAR aucData[8],
                               UINT64 u64SignVal)
{
    ASSERT(pouCurrSignal != NULL);
    /* Signal value data type happens to be of the same size of the entire CAN
    data byte array. Hence there is an opportunity to take advantage of this
    idiosyncratic characteristics. We will shifts the bit array in u64SignVal
    by the required number of bit positions to exactly map it as a data byte
    array and then interchange positions of bytes as per the endianness and
    finally use it as the etching mask on the target. */
    UINT64* pu64Target = (UINT64*) aucData;  // We should be able to work on
    BYTE* pbData = (BYTE*) &u64SignVal;      // these variables as an array of
    // bytes and vice versa.

    // First find out offset between the last significant bits of the signal
    // and the frame. Finding out the lsb will directly answer to this query.

    if (pouCurrSignal->m_eFormat == DATA_FORMAT_INTEL)// If Intel format
    {
        int Offset = (pouCurrSignal->m_unStartByte - 1) * 8 +
                     pouCurrSignal->m_byStartBit;
        u64SignVal <<= Offset; // Exactly map the data bits on the data bytes.
    }
    else    // If Motorola format
    {
        int Offset = pouCurrSignal->m_unStartByte * 8 -
                     pouCurrSignal->m_byStartBit;
        u64SignVal <<= (64 - Offset);
        BYTE byTmp = 0x0;
        byTmp = pbData[7];
        pbData[7] = pbData[0];
        pbData[0] = byTmp;
        byTmp = pbData[6];
        pbData[6] = pbData[1];
        pbData[1] = byTmp;
        byTmp = pbData[5];
        pbData[5] = pbData[2];
        pbData[2] = byTmp;
        byTmp = pbData[4];
        pbData[4] = pbData[3];
        pbData[3] = byTmp;
    }

    UINT64 unTmp = un64GetBitMask(pouCurrSignal);
    *pu64Target &= ~unTmp; // All bits related to the current signal will be
    // be made 0.
    *pu64Target |= u64SignVal;
}
// Ends static functions of sSIGNALS


/* STARTS static public functions of SMSGENTRY */

// Given the root, this clears the message linked list

tagSMSGENTRY::tagSMSGENTRY()
{
    m_psMsg = NULL;
    m_psNext = NULL;
}

tagSMSGENTRY::~tagSMSGENTRY()
{
    if (NULL != m_psMsg)
    {
        delete m_psMsg;
        m_psMsg = NULL;
    }

    if (NULL != m_psNext)
    {
        delete m_psNext;
        m_psNext = NULL;
    }
}

void tagSMSGENTRY::vClearMsgList(SMSGENTRY*& psMsgRoot)
{
    SMSGENTRY* psCurrEntry = psMsgRoot;

    while (NULL != psCurrEntry) // The message list
    {
        SMSGENTRY* psNext = psCurrEntry->m_psNext;
        vClearSignalList(psCurrEntry->m_psMsg->m_psSignals); // delete all
        psCurrEntry->m_psNext = NULL;
        DELETE_PTR(psCurrEntry);    // the data
        psCurrEntry = psNext; // Get on with the next one
    } // while (NULL != psCurrEntry)

    psMsgRoot = NULL;
}

// Given the root, this clears the signal linked list
void tagSMSGENTRY::vClearSignalList(sSIGNALS* psSignals)
{
    sSIGNALS* psCurrSignal = psSignals;

    while (NULL != psCurrSignal)
    {
        sSIGNALS* psSignalNext = psCurrSignal->m_psNextSignalList;
        psCurrSignal->m_psNextSignalList = NULL;

        if (NULL != psCurrSignal->m_oSignalIDVal)
        {
            psCurrSignal->m_oSignalIDVal->vClearNext();
            DELETE_PTR(psCurrSignal->m_oSignalIDVal);
        }

        DELETE_PTR(psCurrSignal);
        psCurrSignal = psSignalNext;
    } // while (NULL != psCurrSignal)
}

// This function copies the current message entry except the signal list
sMESSAGE* tagSMSGENTRY::psCopyMsgVal(sMESSAGE* psMsg)
{
    sMESSAGE* Result = new sMESSAGE;

    if (NULL != Result)
    {
        Result->m_omStrMessageName      = psMsg->m_omStrMessageName;
        Result->m_unMessageCode         = psMsg->m_unMessageCode;
        Result->m_unNumberOfSignals     = psMsg->m_unNumberOfSignals;
        Result->m_unMessageLength       = psMsg->m_unMessageLength;
        Result->m_bMessageFrameFormat   = psMsg->m_bMessageFrameFormat;
        Result->m_psSignals             = NULL;
        memcpy(Result->m_bySignalMatrix, psMsg->m_bySignalMatrix, 8);
        Result->m_nMsgDataFormat        = psMsg->m_nMsgDataFormat;
    }
    else
    {
        ASSERT(FALSE);
    }

    return Result;
}

// This function copies the current signal entry including the descriptor list.
// The copy operation excludes the next signal in the signal linked list.
sSIGNALS* tagSMSGENTRY::psCopySignalVal(sSIGNALS* psSignal)
{
    sSIGNALS* Result = new sSIGNALS;

    if (NULL != Result)
    {
        Result->m_omStrSignalName       = psSignal->m_omStrSignalName;
        Result->m_unStartByte           = psSignal->m_unStartByte;
        Result->m_unSignalLength        = psSignal->m_unSignalLength;
        Result->m_byStartBit            = psSignal->m_byStartBit;
        Result->m_bySignalType          = psSignal->m_bySignalType;
        Result->m_SignalMinValue        = psSignal->m_SignalMinValue;
        Result->m_SignalMaxValue        = psSignal->m_SignalMaxValue;
        Result->m_fSignalFactor         = psSignal->m_fSignalFactor;
        Result->m_fSignalOffset         = psSignal->m_fSignalOffset;
        Result->m_omStrSignalUnit       = psSignal->m_omStrSignalUnit;
        Result->m_eFormat               = psSignal->m_eFormat;
        Result->m_oSignalIDVal          = NULL;
        Result->m_psNextSignalList      = NULL;

        if (NULL != psSignal->m_oSignalIDVal)
        {
            Result->m_oSignalIDVal = new CSignalDescVal;
            *(Result->m_oSignalIDVal) = *(psSignal->m_oSignalIDVal);
        }
    }
    else
    {
        ASSERT(FALSE);
    }

    return Result;
}

// This function copies the signal linked list and returns root of the new list
sSIGNALS* tagSMSGENTRY::psCopySignalList(sSIGNALS* psSignal)
{
    sSIGNALS* Result = NULL;
    sSIGNALS** ppsCurrDest = &Result;
    sSIGNALS** ppsCurrSrc = &psSignal;

    while (NULL != *ppsCurrSrc)
    {
        *ppsCurrDest = psCopySignalVal(*ppsCurrSrc); // Copy signal values
        ppsCurrDest = &((*ppsCurrDest)->m_psNextSignalList); // Keep the next entry ready
        ppsCurrSrc = &((*ppsCurrSrc)->m_psNextSignalList); // Iterate to the next source entry
    }

    return Result;
}

// This function copies the message linked list and returns root of the new list
BOOL tagSMSGENTRY::bUpdateMsgList(SMSGENTRY*& Root,
                                  sMESSAGE* psMsg)
{
    BOOL Result = TRUE;
    SMSGENTRY** ppsCurrDest = &Root;
    //sMESSAGE** ppsCurrSrc = &psMsg;

    while (NULL != *ppsCurrDest) // The idea is to reach the end of message list
    {
        ppsCurrDest = &((*ppsCurrDest)->m_psNext); // Iterate to the next entry
    }

    // We're at the end of list
    *ppsCurrDest = new SMSGENTRY; // Create an initialised entry
    (*ppsCurrDest)->m_psMsg = psCopyMsgVal(psMsg); // Copy message values
    (*ppsCurrDest)->m_psMsg->m_psSignals =
        psCopySignalList(psMsg->m_psSignals); // Copy signal list
    return Result;
}
BOOL SMSGENTRY::bGetMsgPtrFromMsgId(const tagSMSGENTRY* psRoot,UINT unMsgId, sMESSAGE*& pMsg)
{
    BOOL bResult = FALSE;
    const SMSGENTRY* pTemp = psRoot;

    while (pTemp != NULL && bResult == FALSE)
    {
        if (pTemp->m_psMsg->m_unMessageCode == unMsgId)
        {
            pMsg = pTemp->m_psMsg;
            bResult = TRUE;
            break;
        }

        pTemp = pTemp->m_psNext;
    }

    return bResult;
}
/* ENDS static public functions of SMSGENTRY */