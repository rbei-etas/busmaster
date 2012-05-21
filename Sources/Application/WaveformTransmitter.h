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
 * \file      WaveformTransmitter.h
 * \brief     Waveform data Transmitter Header file
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Waveform data Transmitter Header file
 */

#pragma once
#include "Include/Basedefs.h"
#include "struct.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "WaveFormDataHandler.h"
#include "MsgSignal.h"
#include "Utility/Utility_Thread.h"



class CWaveformTransmitter
{
private:
    CPARAM_THREADPROC m_ouTransmitThread;
    // Holds the Base DIL pointer.
    CBaseDIL_CAN* m_pouDIL_CAN_Interface;
    // Holds the wave data handler pointer.
    CWaveFormDataHandler* m_pWaveDataHandler;
    // Holds the Database Message Signal pointer.
    CMsgSignal** m_ppouDBPtr;
    // Waveform signal list
    CSigGenerationInfoList m_omSigGenList;
    //
    BOOL m_bEnabled;    // Enable / disable state
    BOOL m_bTxON;       // Current Tx state
    int m_nIterLimit;   // Iteration limit
    DWORD m_dwClientID; // Client ID to use while transmitting wave patterns

    // Given signal name and CSigWaveMapList, this retrieves the sSigWaveMap entry
    BOOL bGetSignalEntry(CString omSignalName,
                         CSigWaveMapList* pomSigWaveList, sSigWaveMap& Result);

    // Given the present iteration, this returns the current signal amplitude
    UINT64 u64GetCurrAmplitude(int CurrIteration, sWaveformInfo& ouCurrSig);
    // To know if at least one waveform signal has been defined
    BOOL bIsWaveformSignalPresent(void);

    //To0 calculate Rounding values for floats.
    float fRound(float val, unsigned int decimals);

    //Services:
public:

    CWaveformTransmitter();     // Standard / default constructor
    ~CWaveformTransmitter();    // Destructor

    // Helper functions; to be invoked from thread
    // To calculate signal values at the curent iteration and transmit
    void vProcessWaveForm(int CurrIteration);
    // Get sampling Time period.
    short shGetSamplingTimePeriod(void);
    // To get the iteration limit
    int nGetIterationLimit(void);

    // Service methods for the owner entity
    // 1. Do the initialisation
    void vDoInitialisation(CWaveFormDataHandler* pWaveDataHandler,
                           CBaseDIL_CAN* pouDIL_CAN_Interface, CMsgSignal** ppouDBPtr);
    // 2. Is the transmission block enable
    BOOL bIsBlockEnabled(void);
    // 3. Is waveform transmission ON
    BOOL bIsWaveformTxON(void);
    // 4. Enable / disable block
    BOOL bUpdateBlock(BOOL bNodeConnected);
    // 5. Starts wave pattern transmisstion using a client ID
    void vStartSignalTransmission(DWORD dwClientID);
    // 6. Stops the Signal Waves Message Sending.
    void vStopSignalTransmission(void);
};
