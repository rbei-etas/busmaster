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
 * \file      WaveformTransmitter.cpp
 * \brief     Waveform data Transmitter implementation file
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Waveform data Transmitter implementation file
 */

#include "stdafx.h"
#include <math.h>
#include "WaveFormTransmitter.h"

const float SINE_COEFF = (8 / (PI* PI));
CPARAM_THREADPROC* pThreadParam = nullptr; //declare global for termination in destructor

// Start of helper functions
BOOL CWaveformTransmitter::bIsWaveformSignalPresent(void)
{
    ASSERT(nullptr != m_pWaveDataHandler);
    CStringArray arrSignalNames;
    m_pWaveDataHandler->vGetAllDefinedSignalsNames(arrSignalNames);
    return (m_pWaveDataHandler->nGetNumberOfDefinedSignals() > 0
            && arrSignalNames.GetSize()>0 );
}

short CWaveformTransmitter::shGetSamplingTimePeriod(void)
{
    return m_pWaveDataHandler->shGetSamplingTimePeriod();
}

int CWaveformTransmitter::nGetIterationLimit(void)
{
    return m_nIterLimit;
}

float CWaveformTransmitter::fRound(float val, unsigned int decimals)
{
    ASSERT(val!=0);//val must be different from zero to avoid overflow!

    float sign = fabs(val)/val;//we obtain the sign to calculate positive always
    float tempval = fabs(val*pow((float)10, (float)decimals));//shift decimal places
    unsigned int tempint = (unsigned int)tempval;
    float decimalpart = tempval-tempint;//obtain just the decimal part

    if(decimalpart>=0.5)//next integer number if greater or equal to 0.5
    {
        tempval = ceil(tempval);
    }
    else
    {
        tempval = floor(tempval);    //otherwise stay in the current interger part
    }

    return (tempval*pow((float)10,-(float)decimals))*sign;//shift again to the normal decimal places
}

UINT64 CWaveformTransmitter::u64GetCurrAmplitude(int CurrItr,
        sWaveformInfo& ouCurrSig)
{
    float Result = 0.0;

    switch (ouCurrSig.m_eSignalWaveType)
    {
        case eWave_SINE:
        {
            Result = ouCurrSig.m_fAmplitude * sin(CurrItr * ouCurrSig.m_fGranularity);
        }
        break;
        case eWave_COS:
        {
            Result = ouCurrSig.m_fAmplitude * cos(CurrItr * ouCurrSig.m_fGranularity);
        }
        break;
        case eWave_TRIANGLE:
        {
            float Val = CurrItr * ouCurrSig.m_fGranularity;
            Result = (sin(Val) - sin(3 * Val) / 9 + sin(5 * Val) / 25
                      - sin(7 * Val) / 49 + sin(9 * Val) / 81) * SINE_COEFF;
            Result *= ouCurrSig.m_fAmplitude;
        }
        break;
        case eWave_SAWTOOTH:
        {
            UINT unStep = ( 1 / (ouCurrSig.m_fGranularity/(2 * PI )) );
            if ( unStep == 1 )
            {
                CurrItr = 1;
            }
            else
            {
                CurrItr= (CurrItr) % unStep;
            }
            Result = ( ( 2 * ouCurrSig.m_fAmplitude * CurrItr * (ouCurrSig.m_fGranularity/(2 * PI))) -  ouCurrSig.m_fAmplitude);/* Sawtooth :((2A t /T) - A) */
        }
        break;
        default:
            ASSERT(false);
    }
    //ArunKumar K: Currently using the Peak to Peak Amplitude as 0 to 2*Amplitude
    //instead of -Amplitude to +Amplitude
    Result += ouCurrSig.m_fAmplitude;

    if(Result!=0)
    {
        Result = fRound(Result, 0);
    }

    return (UINT64) Result;
}

BOOL CWaveformTransmitter::bGetSignalEntry(CString omSignalName,
        CSigWaveMapList* pomSigWaveList, sSigWaveMap& Result)
{
    BOOL bIsPresent = FALSE;

    ASSERT(nullptr != pomSigWaveList);
    POSITION CurrSigPos = pomSigWaveList->GetHeadPosition();
    while ((nullptr != CurrSigPos) && (FALSE == bIsPresent))
    {
        // Retrieve the present signal entry and query for the next one
        sSigWaveMap& ouCurrSig = pomSigWaveList->GetNext(CurrSigPos);
        if (ouCurrSig.m_omSigName == omSignalName)
        {
            Result = ouCurrSig;
            bIsPresent = TRUE;
        }
    }

    return bIsPresent;
}

UINT64 CWaveformTransmitter::un64GetBitMask(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned char* /*aucData*/, unsigned long long /*u64SignVal*/)
{
    UINT64 Result = 0x1;

    // First make the required number of bits (m_unSignalLength) up.
    Result <<= length;
    --Result; // These bits are now up.

    // Then shift them to the appropriate place.
    short Shift = (true == bIntel) ?
                  ((short)byte - 1) * 8 + startBitIndexInByte
                  : 64 - ((short)byte * 8 - startBitIndexInByte);
    Result <<= Shift;

    if (false == bIntel)
    {
        BYTE* pbStr = (BYTE*)&Result;

        BYTE bTmp = 0x0;
        bTmp = pbStr[0];
        pbStr[0] = pbStr[7];
        pbStr[7] = bTmp;
        bTmp = pbStr[1];
        pbStr[1] = pbStr[6];
        pbStr[6] = bTmp;
        bTmp = pbStr[2];
        pbStr[2] = pbStr[5];
        pbStr[5] = bTmp;
        bTmp = pbStr[3];
        pbStr[3] = pbStr[4];
        pbStr[4] = bTmp;
    }
    return Result;
}
void CWaveformTransmitter::vGetDataBytesFromSignal(unsigned long long u64SignVal, std::pair<ISignal*, SignalInstanse>& Signal, int nDLC, unsigned char* pucData)
{
    unsigned char ucData[MAX_PATH] = { 0 };
    memcpy(ucData, pucData, nDLC);
    int byteOffsetBy8 = Signal.second.m_nStartBit / 64;
    int signalStartBitInByte = Signal.second.m_nStartBit % 8;
    int byteStart = Signal.second.m_nStartBit / 8 - (byteOffsetBy8 * 8) + 1;
    unsigned int nLength = 0;
    Signal.first->GetLength(nLength);
    bool bIntel = false;
    eEndianess endian = Signal.second.m_ouSignalEndianess;

    if (eIntel == endian)
    {
        bIntel = true;
    }
    vSetSignalValue(byteStart, signalStartBitInByte, nLength, bIntel, u64SignVal, &ucData[0], nDLC);
    memcpy(pucData, ucData, nDLC);
}
void CWaveformTransmitter::vSetSignalValue(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned long long u64SignVal, unsigned char* aucData, int dataLenght)
{
    /* Signal valuedata type happens to be of the same size of the entire CAN
    data byte array. Hence there is an opportunity to take advantage of this
    idiosyncratic characteristics. We will shifts the bit array in u64SignVal
    by the required number of bit positions to exactly map it as a data byte
    array and then interchange positions of bytes as per the endianness and
    finally use it as the etching mask on the target. */
    UINT64* pu64Target = (UINT64*)aucData; // We should be able to work on
    BYTE* pbData = (BYTE*)&u64SignVal; // these variables as an arrayof
    // bytes andvice versa.
    // First findout offset between the last significant bits of the signal
    // and theframe. Finding out the lsb will directly answer to thisquery.
    UINT64 unMaxVal = pow((double)2, (double)length);
    unMaxVal -= 1;
    u64SignVal = u64SignVal&unMaxVal;
    if (bIntel == true)// If Intel format
    {
        int Offset = (byte - 1) * 8 + startBitIndexInByte;
        u64SignVal <<= Offset;// Exactly map the data bits on the databytes.
    }
    else// If Motorola format
    {
        int Offset = byte * 8 - startBitIndexInByte;
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
    UINT64 unTmp = un64GetBitMask(byte, startBitIndexInByte, length, bIntel, aucData, u64SignVal);
    *pu64Target &= ~unTmp;// All bits related to the current signal willbe
    // be made0.
    *pu64Target |= u64SignVal;
}

void CWaveformTransmitter::vProcessWaveForm(int CurrItr,int& iStep,eWAVEFORMTYPE& eWFType)
{
    /*  Steps involved are:
        1. Get list of waveform messages and start with the first message
        2. Calculate value of each of the selected signals based on current iteration
        3. Update CAN frame and do it for all the selected signals.
        4. Assume default values for other signals
        5. Transmit the frame.
        6. Repeat steps 2 to 5 for the entire set of messages. */

    POSITION CurrMsgPos = m_omSigGenList.GetHeadPosition();
    while (nullptr != CurrMsgPos)
    {
        // Retrieve the present entry and query for the next one
        SSigGeneration& ouCurrEntry = m_omSigGenList.GetNext(CurrMsgPos);

        if(nullptr == m_ppouDBPtr)
        {
            break;
        }

        // Get message and signal details from database.
        IFrame* frame = nullptr;
        m_ppouDBPtr->GetFrame(CAN, 0, ouCurrEntry.m_nMsgID, nullptr, &frame);

        if (nullptr == frame)
        {
            break;
        }

        // Assign values to the CAN frame to transmit
        unsigned int nLength = 0;
        STCAN_MSG sCurrFrame = {'\0'}; // m_ucEXTENDED & m_ucRTR are zero.
        sCurrFrame.m_unMsgID = ouCurrEntry.m_nMsgID;
        frame->GetLength(nLength);
        sCurrFrame.m_ucDataLen = (UCHAR)(nLength);
        sCurrFrame.m_ucChannel = 0x1;

        // Iterate through the master signal list and query for each signal in
        // the waveform list.
        std::map<ISignal*, SignalInstanse> signalList;
        frame->GetSignalList(signalList);


        bool bSawtoothExists = false;
        std::string name = "";
for (auto signal : signalList)
        {
            // Search if it occurs in the waveform list.
            sSigWaveMap ouCurrSig;
            signal.first->GetName(name);
            if (bGetSignalEntry(name.c_str(),
                                &(ouCurrEntry.m_omSigWaveMapList), ouCurrSig))
            {
                UINT unStep = ( 1 / (ouCurrSig.sWaveInfo.m_fGranularity/(2 * PI )) );

                if ( ouCurrSig.sWaveInfo.m_eSignalWaveType == eWave_SAWTOOTH && ( (CurrItr == 0x00) || (CurrItr % unStep) == 0) )
                {
                    std::pair<ISignal*, SignalInstanse> signalPair(signal.first, signal.second);
                    vGetDataBytesFromSignal((UINT64)ouCurrSig.sWaveInfo.m_fAmplitude * 2, signalPair, sCurrFrame.m_ucDataLen, sCurrFrame.m_ucData);
                    bSawtoothExists = true;
                }
                else
                {
                    UINT64 Amplitude = u64GetCurrAmplitude(CurrItr, ouCurrSig.sWaveInfo);
                    std::pair<ISignal*, SignalInstanse> signalPair(signal.first, signal.second);
                    vGetDataBytesFromSignal((UINT64)Amplitude, signalPair, sCurrFrame.m_ucDataLen, sCurrFrame.m_ucData);
                }
                iStep = unStep;
                eWFType =  ouCurrSig.sWaveInfo.m_eSignalWaveType;
            }
            else
            {
                std::pair<ISignal*, SignalInstanse> signalPair(signal.first, signal.second);
                vGetDataBytesFromSignal((UINT64)ouCurrEntry.m_fDefaultAmplitude, signalPair, sCurrFrame.m_ucDataLen, sCurrFrame.m_ucData);
            }
        }
        // End of processing. Now transmit the CAN frame.
        m_pouDIL_CAN_Interface->DILC_SendMsg(m_dwClientID, sCurrFrame);

        /* If sawtooth signals are present */
        if ( bSawtoothExists == true)
        {
            /* Reloop for sending Sawtooth signals with value 0 */
for (auto signal : signalList)
            {
                // Search if it occurs in the waveform list.
                sSigWaveMap ouCurrSig;
                std::string name;
                signal.first->GetName(name);
                if (bGetSignalEntry(name.c_str(),
                                    &(ouCurrEntry.m_omSigWaveMapList), ouCurrSig))
                {
                    UINT unStep = ( 1 / (ouCurrSig.sWaveInfo.m_fGranularity/(2 * PI )) );
                    if ( ouCurrSig.sWaveInfo.m_eSignalWaveType == eWave_SAWTOOTH && ( (CurrItr == 0x00) || (CurrItr % unStep) == 0) )
                    {
                        std::pair<ISignal*, SignalInstanse> signalPair(signal.first, signal.second);
                        vGetDataBytesFromSignal(0, signalPair, sCurrFrame.m_ucDataLen, sCurrFrame.m_ucData);

                    }
                }
                else
                {
                    std::pair<ISignal*, SignalInstanse> signalPair(signal.first, signal.second);
                    vGetDataBytesFromSignal((UINT64)ouCurrEntry.m_fDefaultAmplitude, signalPair, sCurrFrame.m_ucDataLen, sCurrFrame.m_ucData);
                }
            }
            // End of processing. Now transmit the CAN frame.
            m_pouDIL_CAN_Interface->DILC_SendMsg(m_dwClientID, sCurrFrame);
        }
    }
}

/******************************************************************************
Function Name   : TransmissionThreadProc
Input(s)        : pVoid -
Output          :
Functionality   : Transmit thread for the sending waveform messages.
Member of       : Global
Friend of       : -
Author(s)       : Arunkumar K.
Date Created    : 26.08.2010
Modification    :
******************************************************************************/
DWORD WINAPI TransmissionThreadProc(LPVOID pVoid)
{
    pThreadParam = (CPARAM_THREADPROC*) pVoid;
    ASSERT(nullptr != pThreadParam);

    CWaveformTransmitter* pCurrObj = static_cast<CWaveformTransmitter*> (pThreadParam->m_pBuffer);
    ASSERT(nullptr != pCurrObj);

    UINT SamplingRate = pCurrObj->shGetSamplingTimePeriod();
    int nIterLimit = pCurrObj->nGetIterationLimit();

    TIMECAPS time;
    if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
    {
        if (time.wPeriodMin <= SamplingRate)
        {
            MMRESULT mmResult = timeBeginPeriod(time.wPeriodMin);
            if (TIMERR_NOCANDO == mmResult)
            {
                // TBD
            }
        }
        else
        {
            ASSERT(false); // Unexpected situation.
        }
    }

    // As thread parameter we need an auto-reset event.
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;
    pThreadParam->m_hActionEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    MMRESULT Result = timeSetEvent(SamplingRate, SamplingRate /*time.wPeriodMin*/,
                                   (LPTIMECALLBACK) pThreadParam->m_hActionEvent, 0,
                                   TIME_CALLBACK_EVENT_SET | TIME_PERIODIC);
    //ASSERT(nullptr != Result);

    bool bLoopON = true;
    int i = 0;
    int iStep =0;
    eWAVEFORMTYPE eWaveType;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);

        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                // Calculate signal values at the curent iteration and transmit
                // the message(s).
                pCurrObj->vProcessWaveForm(i,iStep,eWaveType);
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
        if(eWaveType == eWave_SAWTOOTH)
        {
            int nExcess= 0;
            if( ((nIterLimit % iStep) != 0)&& (nIterLimit > iStep))
            {
                nExcess = (nIterLimit % iStep);
                nIterLimit = nIterLimit - ( nExcess);
            }
            i = (i < nIterLimit-1) ? ++i : 0;
        }
        else
        {
            i = (i < nIterLimit) ? ++i : 0;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());

    if(Result != 0)
    {
        Result = timeKillEvent(Result);
    }

    pThreadParam = nullptr; //thread terminated

    return 0;
}
// End of helper functions

CWaveformTransmitter::CWaveformTransmitter()
{
    m_bEnabled = FALSE;
    m_bTxON = FALSE;
    m_nIterLimit = 0;
    m_pWaveDataHandler      = nullptr;
    m_pouDIL_CAN_Interface  = nullptr;
    m_ppouDBPtr             = nullptr;
}

CWaveformTransmitter::~CWaveformTransmitter()
{
    //terminate the thread if it is still running
    while(pThreadParam != nullptr)
    {
        ::Sleep(10);
        if(pThreadParam != nullptr) //check for thread is terminated or not
        {
            pThreadParam->m_unActionCode = EXIT_THREAD;
            SetEvent(pThreadParam->hGetExitNotifyEvent());
            continue;
        }
        else
        {
            break;
        }
    }
}

void CWaveformTransmitter::vDoInitialisation(CWaveFormDataHandler* pWaveDataHandler,
        CBaseDIL_CAN* pouDIL_CAN_Interface, IBMNetWorkGetService* ppouDBPtr)
{
    m_pWaveDataHandler      = pWaveDataHandler;
    m_pouDIL_CAN_Interface  = pouDIL_CAN_Interface;
    m_ppouDBPtr             = ppouDBPtr;
}

BOOL CWaveformTransmitter::bIsBlockEnabled(void)
{
    return m_bEnabled;
}

BOOL CWaveformTransmitter::bIsWaveformTxON(void)
{
    return m_bTxON;
}

BOOL CWaveformTransmitter::bUpdateBlock(BOOL bNodeConnected)
{
    /* Conditions:
    _________________________________________________________
    Node connected | Waveform signals present | Block enabled
    ---------------------------------------------------------
        TRUE       |        TRUE              |    TRUE
          X        |        FALSE             |    FALSE
        FALSE      |          X               |    FALSE
    --------------------------------------------------------- */

    if (bNodeConnected)
    {
        if (bIsWaveformSignalPresent())
        {
            m_bEnabled = TRUE;
        }
        else
        {
            if (bIsWaveformTxON())
            {
                vStopSignalTransmission();
            }
            m_bEnabled = FALSE;
        }
    }
    else
    {
        // Stop transmission if its ON
        if (bIsWaveformTxON())
        {
            vStopSignalTransmission();
        }
        m_bEnabled = FALSE;
    }
    return m_bEnabled;
}

void CWaveformTransmitter::vStartSignalTransmission(DWORD dwClientID)
{
    m_dwClientID = dwClientID;
    m_pWaveDataHandler->GetSigGenerationInfoList(m_omSigGenList);
    short shWvfrm = 0;
    shWvfrm = m_pWaveDataHandler->shGetSamplingTimePeriod();
    if(shWvfrm > 0)
    {
        m_nIterLimit = 1000 / shWvfrm - 1;
    }
    else
    {
        shWvfrm = 125;
        m_nIterLimit = 1000 / shWvfrm - 1;
    }

    if (FALSE == m_bTxON)
    {
        m_ouTransmitThread.m_pBuffer = this;
        m_bTxON = m_ouTransmitThread.bStartThread(TransmissionThreadProc);
    }
}

void CWaveformTransmitter::vStopSignalTransmission(void)
{
    m_bTxON = !m_ouTransmitThread.bTerminateThread();
}
