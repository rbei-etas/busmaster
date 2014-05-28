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
            ASSERT(FALSE);
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

        if(nullptr == *m_ppouDBPtr)
        {
            break;
        }

        // Get message and signal details from database.
        sMESSAGE* psCurrMsg = (*m_ppouDBPtr)->psGetMessagePointer(ouCurrEntry.m_nMsgID);
        if(nullptr == psCurrMsg)
        {
            break;
        }

        // Assign values to the CAN frame to transmit
        STCAN_MSG sCurrFrame = {'\0'}; // m_ucEXTENDED & m_ucRTR are zero.
        sCurrFrame.m_unMsgID = ouCurrEntry.m_nMsgID;
        sCurrFrame.m_ucDataLen = (UCHAR) (psCurrMsg->m_unMessageLength);
        sCurrFrame.m_ucChannel = 0x1;

        // Iterate through the master signal list and query for each signal in
        // the waveform list.
        sSIGNALS* psCurrSignal = nullptr;
        if(nullptr == psCurrMsg->m_psSignals)
        {
            break;
        }
        else
        {
            psCurrSignal = psCurrMsg->m_psSignals;  // Master list of the signals which is expected to exist
        }

        bool bSawtoothExists = false;

        while (nullptr != psCurrSignal)
        {
            // Search if it occurs in the waveform list.
            sSigWaveMap ouCurrSig;
            if (bGetSignalEntry(psCurrSignal->m_omStrSignalName,
                                &(ouCurrEntry.m_omSigWaveMapList), ouCurrSig))
            {
                UINT unStep = ( 1 / (ouCurrSig.sWaveInfo.m_fGranularity/(2 * PI )) );

                if ( ouCurrSig.sWaveInfo.m_eSignalWaveType == eWave_SAWTOOTH && ( (CurrItr == 0x00) || (CurrItr % unStep) == 0) )
                {
                    sSIGNALS::vSetSignalValue(psCurrSignal, sCurrFrame.m_ucData, (UINT64) ouCurrSig.sWaveInfo.m_fAmplitude * 2 );
                    bSawtoothExists = true;
                }
                else
                {
                    UINT64 Amplitude = u64GetCurrAmplitude(CurrItr, ouCurrSig.sWaveInfo);
                    sSIGNALS::vSetSignalValue(psCurrSignal, sCurrFrame.m_ucData,
                                              Amplitude);
                }
                iStep = unStep;
                eWFType =  ouCurrSig.sWaveInfo.m_eSignalWaveType;
            }
            else
            {
                sSIGNALS::vSetSignalValue(psCurrSignal, sCurrFrame.m_ucData,
                                          (UINT64) ouCurrEntry.m_fDefaultAmplitude);
            }
            psCurrSignal = psCurrSignal->m_psNextSignalList; // Iterate
        }
        // End of processing. Now transmit the CAN frame.
        m_pouDIL_CAN_Interface->DILC_SendMsg(m_dwClientID, sCurrFrame);

        /* If sawtooth signals are present */
        if ( bSawtoothExists == true)
        {
            /* Reloop for sending Sawtooth signals with value 0 */
            psCurrSignal = psCurrMsg->m_psSignals;
            while (nullptr != psCurrSignal)
            {
                // Search if it occurs in the waveform list.
                sSigWaveMap ouCurrSig;
                if (bGetSignalEntry(psCurrSignal->m_omStrSignalName,
                                    &(ouCurrEntry.m_omSigWaveMapList), ouCurrSig))
                {
                    UINT unStep = ( 1 / (ouCurrSig.sWaveInfo.m_fGranularity/(2 * PI )) );
                    if ( ouCurrSig.sWaveInfo.m_eSignalWaveType == eWave_SAWTOOTH && ( (CurrItr == 0x00) || (CurrItr % unStep) == 0) )
                    {
                        sSIGNALS::vSetSignalValue(psCurrSignal, sCurrFrame.m_ucData, 0 );
                    }
                }
                else
                {
                    sSIGNALS::vSetSignalValue(psCurrSignal, sCurrFrame.m_ucData,
                                              (UINT64) ouCurrEntry.m_fDefaultAmplitude);
                }
                psCurrSignal = psCurrSignal->m_psNextSignalList; // Iterate
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
            ASSERT(FALSE); // Unexpected situation.
        }
    }

    // As thread parameter we need an auto-reset event.
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;
    pThreadParam->m_hActionEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    MMRESULT Result = timeSetEvent(SamplingRate, SamplingRate /*time.wPeriodMin*/,
                                   (LPTIMECALLBACK) pThreadParam->m_hActionEvent, 0,
                                   TIME_CALLBACK_EVENT_SET | TIME_PERIODIC);
    ASSERT(nullptr != Result);

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
        CBaseDIL_CAN* pouDIL_CAN_Interface, CMsgSignal** ppouDBPtr)
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
