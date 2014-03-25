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
 * \file      BusStatisticCAN.cpp
 * \author    venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "StdAfx.h"
//To Include BusStatisticCAN Definitions.

#include "BusStatisticLIN.h"
#include "Include/Lin_Error_Defs.h"
#include "DIL_Interface/DIL_Interface_Extern.h"
#include "TimeManager.h"


//LIN interface to Bus Statistics
void* CBusStatisticLIN:: sm_pouBSLin;

/**
 * \param[in] pBSLin contain the CBusStatisticLIN object
 * \return Function return Status
 *
 * Updates the busstatistics parameters using
 * vUpdateBusStatistics Function
 */
int ReadBSDataBuffer(CBusStatisticLIN* pBSLin)
{
    ASSERT(pBSLin != NULL);
    while (pBSLin->m_ouLinBufFSE.GetMsgCount() > 0)
    {
        static STLINDATA sLinData;
        
        int Result = pBSLin->m_ouLinBufFSE.ReadFromBuffer(&sLinData);
        if (Result == ERR_READ_MEMORY_SHORT)
        {
            TRACE("ERR_READ_MEMORY_SHORT");
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            TRACE("EMPTY_APP_BUFFER");
        }
        else
        {
            pBSLin->vUpdateBusStatistics(sLinData);
        }

    }
    return 0;
}

/**
 * \param[in] pVoid contains CBusStatisticLIN pointer
 *
 * Thread Function that is used to read the driver data and
 * updates the Bus Statistics.
 */
DWORD WINAPI BSDataReadThreadProc_LIN(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == NULL)
    {
        return ((DWORD)-1);
    }

    CBusStatisticLIN* pBusStatistics_LIN = static_cast<CBusStatisticLIN*> (pThreadParam->m_pBuffer);

    if (pBusStatistics_LIN == NULL)
    {
        return ((DWORD)-1);
    }
    bool bLoopON = true;
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;
    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                ReadBSDataBuffer(pBusStatistics_LIN); // Retrieve message from the driver
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                //Nothing at this moment
            }
            break;
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
            default:
            {

            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

/**
 * Constructor for CBusStatisticLIN which initialises
 * critical section and event mechanism.
 */
CBusStatisticLIN::CBusStatisticLIN(void)
{
    InitializeCriticalSection(&m_omCritSecBS);
    m_ouReadThread.m_hActionEvent = m_ouLinBufFSE.hGetNotifyingEvent();
    m_nTimerHandle = NULL;
    //Initialise Number of Bits in Standard LIN Messages
  //  UINT unBitsStdMsg[] = {44, 54, 64, 74, 84, 94, 104, 114, 124,134};
	UINT unBitsStdMsg[] = {50, 61, 71, 82, 92, 103, 113, 124, 134};
  //   UINT unBitsStdMsg[] = {52, 63, 73, 84, 94, 105, 115, 126, 136};
    memcpy(m_unBitsStdMsg, unBitsStdMsg, 9*sizeof(UINT));
    
    
    m_nFactorSTDFrame = 0;
    m_nFactorErrorFrame = 0;
    m_dDiffTime = 0;
}

/**
 * Destructor of CBusStatisticLIN and which deletes critical
 * section and terminates the thread.
 */
CBusStatisticLIN::~CBusStatisticLIN(void)
{

    m_ouReadThread.bTerminateThread();      // Terminate read thread
    m_ouLinBufFSE.vClearMessageBuffer();    // clear can buffer
    DeleteCriticalSection(&m_omCritSecBS);  // delete critical section
    if( m_nTimerHandle != NULL )
    {
        KillTimer(NULL, m_nTimerHandle);
    }
}

/**
 * \return contain Error information
 *
 * Does the client registration with driver and starts the Read
 * Thread.
 */
HRESULT CBusStatisticLIN::BSL_DoInitialization(void)
{
	if (DIL_GetInterface(LIN, (void**)&m_pouDIL_LIN) == S_OK)
    {
        DWORD dwClientId = 0;
        m_pouDIL_LIN->DILL_RegisterClient(TRUE, dwClientId, LIN_MONITOR_NODE);
        m_pouDIL_LIN->DILL_ManageMsgBuf(MSGBUF_ADD, dwClientId, &m_ouLinBufFSE);
    }
    vInitialiseBSData();

    sm_pouBSLin = this;

    //Start the read thread

    return bStartBSReadThread_LIN()?S_OK:S_FALSE;
}

/**
 * \param[in] bStart if ture starts the TIMER otherwise kill the thread
 *
 * Starts or kills the timer.
 */
BOOL CBusStatisticLIN::BSL_bStartUpdation(BOOL bStart)
{
    if(bStart == TRUE)
    {
        if(m_nTimerHandle == NULL)
        {
            m_nTimerHandle = ::SetTimer(NULL, 0, 1000, OnTimeWrapper);
        }
    }
    else
    {
        KillTimer(NULL, m_nTimerHandle);
        m_nTimerHandle = NULL;
    }
    return 0;
}

/**
 * \return contain Error information
 *
 * This function resets the bus statistics structure using
 * vInitialiseBSData() member function.
 */
HRESULT CBusStatisticLIN::BSL_ResetBusStatistic(void)
{
    EnterCriticalSection(&m_omCritSecBS);

    vInitialiseBSData();
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

HRESULT CBusStatisticLIN::BSL_GetTotalMsgCount(UINT unChannelIndex, eDirection eDir,
        BYTE byIdType, BYTE byMsgType, UINT& nMsgCount)
{
    EnterCriticalSection(&m_omCritSecBS);

    if( eDir == DIR_RX )
    {
        nMsgCount = m_sBusStatistics_LIN[unChannelIndex].m_unTotalRxMsgCount;
	} 
           
      else
            {
               nMsgCount = m_sBusStatistics_LIN[unChannelIndex].m_unTotalTxMsgCount;
            }
        
    LeaveCriticalSection(&m_omCritSecBS);
    return 0;
}

/**
 * \param[in] unChannelIndex can be CAN_CHANNEL_1, CAN_CHANNEL_2, CAN_CHANNEL_ALL
 * \param[in] eDir can be DIR_RX, DIR_TX, DIR_ALL
 * \param[out] nErrCount contain the Total error count on function return
 * \return contain Error information
 * \req RS_24_11 Error frames occurs under the purview of status data
 * \req RS_24_16 Total data shall be presented.
 *
 * Returns the total number of error messages occurred while
 * receiving or transmitting
 */
HRESULT CBusStatisticLIN::BSL_GetTotalErrCount(UINT unChannelIndex, eDirection eDir,
        UINT& nErrCount)
{
    EnterCriticalSection(&m_omCritSecBS);

    //eDir == DIR_RX
    if( DIR_RX == eDir )
    {
        nErrCount = m_sBusStatistics_LIN[ unChannelIndex ].m_unErrorRxCount;
    }
    //eDir == DIR_TX
    else if( DIR_TX == eDir )
    {
        nErrCount = m_sBusStatistics_LIN[ unChannelIndex ].m_unErrorTxCount;
    }
    //eDir == DIR_ALL
    else
    {
        nErrCount = m_sBusStatistics_LIN[ unChannelIndex ].m_unErrorTotalCount;
    }
    LeaveCriticalSection(&m_omCritSecBS);
    return 0;
}

/**
 
 * Returns average number of msgs per second (Msg/s)
 */
HRESULT CBusStatisticLIN::BSL_GetAvgMsgCountPerSec(UINT unChannelIndex,
        eDirection eDir, BYTE byIdType, double& dMsgRate)
{

    EnterCriticalSection(&m_omCritSecBS);

    if( eDir == DIR_RX )
    {
        
            dMsgRate = m_sBusStatistics_LIN[unChannelIndex].m_dTotalRxMsgRate;
        }
    else
	{
      dMsgRate = m_sBusStatistics_LIN[unChannelIndex].m_dTotalTxMsgRate;
    }
    // dMsgRate = (double)(dMsgRate / m_dDiffTime);
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

/**
 * Returns average number of errors per second (Err/s)
 */
HRESULT CBusStatisticLIN::BSL_GetAvgErrCountPerSec(UINT unChannelIndex,
        eDirection eDir, double& dErrCount)
{
    EnterCriticalSection(&m_omCritSecBS);

    //eDir == DIR_RX
    if( eDir == DIR_RX )
    {
        dErrCount = m_sBusStatistics_LIN[unChannelIndex].m_dErrorRxRate;
    }
    //eDir == DIR_TX
    else if( eDir == DIR_TX )
    {
        dErrCount = m_sBusStatistics_LIN[unChannelIndex].m_dErrorTxRate;
    }
    //eDir == DIR_ALL
    else
    {
        dErrCount = m_sBusStatistics_LIN[unChannelIndex].m_dErrorRate;
    }
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

/*
 * Returns the bus load
 */
HRESULT CBusStatisticLIN::BSL_GetBusLoad(UINT unChannelIndex, eLOAD eLoad, double& dBusLoad)
{
    EnterCriticalSection(&m_omCritSecBS);

    //eLoad = AVERAGE
    if( eLoad == AVERAGE )
    {
        dBusLoad = m_sBusStatistics_LIN[unChannelIndex].m_dAvarageBusLoad;
    }
    //eLoad = PEAK
    else if( eLoad == PEAK )
    {
        dBusLoad = m_sBusStatistics_LIN[unChannelIndex].m_dPeakBusLoad;
    }
    //eLoad = CURRENT
    else
    {
        dBusLoad = m_sBusStatistics_LIN[unChannelIndex].m_dBusLoad;
    }
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

/**
 * \param[in] unChannelIndex 
 * \param[in] eDir can be DIR_RX, DIR_TX, DIR_ALL
 * \param[in] eLoad can be CURRENT, AVERAGE, PEAK
 * \param[out] ucErrCounter on return contains the error counter
 * \return contain Error information
 * \req RS_24_11 Error frames occurs under the purview of status data
 * \req RS_24_12 Bus load status data set includes current load
 * \req RS_24_13 Bus load status data set includes peak load
 * \req RS_24_14 Bus load status data set includes average load
 *
 * Returns controller status
 */
HRESULT CBusStatisticLIN::BSL_GetErrorCounter(UINT unChannelIndex, eDirection eDir,
        eLOAD eLoad, UCHAR& ucErrCounter)
{
    EnterCriticalSection(&m_omCritSecBS);

    if( eDir == DIR_RX )
    {
        //eDir == DIR_RX; eLoad == CURRENT
        if( eLoad == CURRENT )
        {
			ucErrCounter = m_sBusStatistics_LIN[unChannelIndex].m_ucRxErrorCounter;
        }
        //eDir == DIR_RX; eLoad == PEAK
        else if( eLoad == PEAK )
        {
            ucErrCounter = m_sBusStatistics_LIN[unChannelIndex].m_ucRxPeakErrorCount;
        }
    }
    else if( eDir == DIR_TX )
    {
        //eDir == DIR_TX; eLoad == CURRENT
        if( eLoad == CURRENT )
        {
            ucErrCounter = m_sBusStatistics_LIN[unChannelIndex].m_ucTxErrorCounter;
        }
        //eDir == DIR_TX; eLoad == PEAK
        else if( eLoad == PEAK )
        {
            ucErrCounter = m_sBusStatistics_LIN[unChannelIndex].m_ucTxPeakErrorCount;
        }
    }
    else
    {
        //eDir == DIR_ALL; eLoad == CURRENT
        if( eLoad == CURRENT )
        {
            ucErrCounter = m_sBusStatistics_LIN[unChannelIndex].m_ucRxErrorCounter +
                           m_sBusStatistics_LIN[unChannelIndex].m_ucTxErrorCounter;
        }
        //eDir == DIR_ALL; eLoad == PEAK
        else if( eLoad == PEAK )
        {
            ucErrCounter = m_sBusStatistics_LIN[unChannelIndex].m_ucRxPeakErrorCount +
                           m_sBusStatistics_LIN[unChannelIndex].m_ucTxPeakErrorCount;
        }
    }
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

/**
 * \return return the error state
 *
 * Starts the Read thread.
 */
HRESULT CBusStatisticLIN::BSL_SetBaudRate(UINT unChannelIndex, double dBaudRate)
{
    EnterCriticalSection(&m_omCritSecBS);

    ASSERT(unChannelIndex < defNO_OF_LIN_CHANNELS);
    m_sBusStatistics_LIN[unChannelIndex].m_dBaudRate = dBaudRate;
    LeaveCriticalSection(&m_omCritSecBS);
    return 0L;
}

/**
 * \return Controller status
 * \req RS_24_18 Controller status data covers present controller status
 *
 * This Function renders controller status
 */
UCHAR CBusStatisticLIN::BSL_ucGetControllerStatus(UINT unChannelIndex)
{
    return m_sBusStatistics_LIN[unChannelIndex].m_ucStatus;
}

/**
 * \param[in] sBusStatistics will have the required channel's
 * \return function return status
 *
 * This Function returns the required channel's BusStatistics
 */
HRESULT CBusStatisticLIN::BSL_GetBusStatistics(UINT unChannelIndex, SBUSSTATISTICS_LIN& sBusStatistics_LIN)
{
    sBusStatistics_LIN = m_sBusStatistics_LIN[unChannelIndex];
    return S_OK;
}

/**
 * This Function will be called by the Timer event
 */
void CALLBACK CBusStatisticLIN::OnTimeWrapper(HWND, UINT, UINT_PTR, DWORD)
{
    CBusStatisticLIN* pTempClass = static_cast<CBusStatisticLIN*> (sm_pouBSLin); // cast the void pointer
    pTempClass->vCalculateBusParametres(); // call non-static function
}

/**
 * \return return the error state
 * \req RS_24_15 Measurement period always begins (or resets) from the time of connection
 *
 * Starts the Read thread.
 */

BOOL CBusStatisticLIN::bStartBSReadThread_LIN(void)
{
    m_ouReadThread.m_pBuffer = this;

    m_ouReadThread.bStartThread(BSDataReadThreadProc_LIN);

    return TRUE;
}

/**
 * \req RS_24_08 Standard frames are considered.
 * \req RS_24_09 Extended frames are considered.
 * \req RS_24_10 RTR frames are considered.
 * \req RS_24_11 Error frames occurs under the purview of status data
 *
 * Initialises the m_sbusstatistics structor
 */
void CBusStatisticLIN::vInitialiseBSData(void)
{
    EnterCriticalSection(&m_omCritSecBS);

    for( int nChannelIndex = 0; nChannelIndex < defNO_OF_LIN_CHANNELS; nChannelIndex++ )
    {
        // Reset whole statucture
        memset( &m_sBusStatistics_LIN[ nChannelIndex ] , 0, sizeof(SBUSSTATISTICS_LIN) );
        memset( &m_sPrevStatData_LIN[ nChannelIndex ] , 0, sizeof(SBUSSTATISTICS_LIN) );
        // Set Baud rate Manually
        m_sBusStatistics_LIN[ nChannelIndex ].m_dBaudRate = _tstof(defBAUDRATE);

        memset( &m_sSubBusStatistics_LIN[nChannelIndex], 0, sizeof(SSUBBUSSTATISTICS_LIN) );
    }


    memset( &m_unPrevStandardCount, 0, sizeof( m_unPrevStandardCount ) );
    memset( &m_unPrevErrorTotalCount, 0, sizeof( m_unPrevErrorTotalCount ) );
	



   // m_nFactorSTDFrame     = defBITS_STD_FRAME + defBITS_INTER_FRAME;
   // m_nFactorErrorFrame   = defBITS_ERROR_FRAME + defBITS_INTER_FRAME;
    m_dDiffTime           = defTIME_INTERVAL_LIN;
    m_unPreviousTime = -1;
    LeaveCriticalSection(&m_omCritSecBS);
}

/**
 * Calculates the differential time in sec
 */
void CBusStatisticLIN::vCalculateDiffTime(void)
{

    // check if the previous time value is not stored. take 1.0s as initial
    // value for calculation.
    if(m_unPreviousTime != -1 )
    {
        m_dDiffTime         = CTimeManager::nCalculateCurrTimeStamp(FALSE) -
                              m_unPreviousTime;
        m_unPreviousTime += static_cast<UINT>(m_dDiffTime);
        m_dDiffTime         = m_dDiffTime / defDIV_FACT_FOR_SECOND_LIN;
    }
    else
    {
        m_unPreviousTime = CTimeManager::nCalculateCurrTimeStamp(FALSE);
    }
}

/**
 * \req RS_24_08 Standard frames are considered.
 * \req RS_24_09 Extended frames are considered.
 * \req RS_24_10 RTR frames are considered.
 * \req RS_24_11 Error frames occurs under the purview of status data
 *
 * Calculate the Bus statistics in m_sBusStatistics structure
 */
void CBusStatisticLIN::vUpdateBusStatistics(STLINDATA& sLinData)
{
	EnterCriticalSection(&m_omCritSecBS);
	m_sCurrEntry = sLinData.m_uDataInfo.m_sLINMsg;
	int nCurrentChannelIndex = sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel - 1;
    INT nDLC = sLinData.m_uDataInfo.m_sLINMsg.m_ucDataLen;

    if ((nCurrentChannelIndex < 0) || (nCurrentChannelIndex > (defNO_OF_LIN_CHANNELS - 1)))
    {
        nCurrentChannelIndex = 0;   //take appropriate action
    }
    
    //is it Tx Message
    if(IS_TX_MESSAGE(sLinData.m_ucDataType))
    {
		if((sLinData.m_eLinMsgType)== LIN_EVENT)
    	{
			if(sLinData.m_uDataInfo.m_sErrInfo.m_eEventType == EVENT_LIN_WAKEUP)
	            {
               		m_sSubBusStatistics_LIN[nCurrentChannelIndex].m_unTotalWakeUpsCount++;

               	}
			else if(sLinData.m_uDataInfo.m_sErrInfo.m_eEventType == EVENT_LIN_SLEEP )
	             {

               	}
			else
			{
			m_sSubBusStatistics_LIN[nCurrentChannelIndex].m_unErrorTxCount++;
			}
		}
		else
		{
			m_sSubBusStatistics_LIN[nCurrentChannelIndex].m_unTotalTxMsgCount++;

			m_sSubBusStatistics_LIN[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsStdMsg[nDLC];
		}

    }
    //is it Rx Message
    else if(IS_RX_MESSAGE(sLinData.m_ucDataType))
    {
		if((sLinData.m_eLinMsgType == LIN_EVENT))
		{
			
			if(sLinData.m_uDataInfo.m_sErrInfo.m_eEventType == EVENT_LIN_WAKEUP)
	            {
               		m_sSubBusStatistics_LIN[nCurrentChannelIndex].m_unTotalWakeUpsCount++;

               	}
			else if(sLinData.m_uDataInfo.m_sErrInfo.m_eEventType == EVENT_LIN_SLEEP )
	             {
					 //no action taken
               	}
			else
			{

			m_sSubBusStatistics_LIN[nCurrentChannelIndex].m_unErrorRxCount++;
			}
		}
		else
		{
			m_sSubBusStatistics_LIN[nCurrentChannelIndex].m_unTotalRxMsgCount++;
			m_sSubBusStatistics_LIN[ nCurrentChannelIndex ].m_unTotalBitsperSec += m_unBitsStdMsg[nDLC];

		}

	}
	else if(sLinData.m_uDataInfo.m_sErrInfo.m_eEventType == EVENT_LIN_WAKEUP)
	{
		m_sSubBusStatistics_LIN[nCurrentChannelIndex].m_unTotalWakeUpsCount++;

	}

	    else
    {
        //Is it is Error
        m_sSubBusStatistics_LIN[nCurrentChannelIndex].m_unErrorTotalCount++;
        if (sLinData.m_uDataInfo.m_sErrInfo.m_ucErrType == ERROR_BUS)
        {
            // Update Statistics information
            m_sSubBusStatistics_LIN[ nCurrentChannelIndex ].m_unErrorTotalCount++;
            USHORT usErrorID = sLinData.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap /*& 0xE0*/;
            // Received message
            if (usErrorID & 0x20)
            {
                m_sSubBusStatistics_LIN[ nCurrentChannelIndex ].m_unErrorRxCount++;
            }
            else
            {
                m_sSubBusStatistics_LIN[ nCurrentChannelIndex ].m_unErrorTxCount++;
            }
            m_sSubBusStatistics_LIN[ nCurrentChannelIndex ].m_unTotalBitsperSec += defBITS_ERR_FRAME;
        }
    }

    LeaveCriticalSection(&m_omCritSecBS);
}

/**
 * \req RS_24_08 Standard frames are considered.
 * \req RS_24_09 Extended frames are considered.
 * \req RS_24_10 RTR frames are considered.
 * \req RS_24_11 Error frames occurs under the purview of status data
 * \req RS_24_12 Bus load status data set includes current load
 * \req RS_24_13 Bus load status data set includes peak load
 * \req RS_24_14 Bus load status data set includes average load
 * \req RS_24_18 Controller status data covers present controller status
 * \req RS_24_19 Controller status data covers present controller Tx Error Counter (peak)
 * \req RS_24_20 Controller status data covers present controller Tx Error Counter (present)
 * \req RS_24_21 Controller status data covers present controller Rx Error Counter (peak)
 * \req RS_24_22 Controller status data covers present controller Rx Error Counter (present)
 *
 * Calculates the Bus Statistics using m_sBusStatistics structure
 */
void CBusStatisticLIN::vCalculateBusParametres(void)
{
    EnterCriticalSection(&m_omCritSecBS);
    vCalculateDiffTime();
	m_dDiffTime = 1;
    for(int nChannelIndex =0; nChannelIndex <defNO_OF_LIN_CHANNELS; nChannelIndex++)
    {
        m_sBusStatistics_LIN[nChannelIndex] = m_sSubBusStatistics_LIN[nChannelIndex];
        m_sBusStatistics_LIN[nChannelIndex].m_nSamples++;
        m_sBusStatistics_LIN[ nChannelIndex ].m_unTotalMsgCount =
            m_sBusStatistics_LIN[ nChannelIndex ].m_unTotalTxMsgCount +
            m_sBusStatistics_LIN[ nChannelIndex ].m_unTotalRxMsgCount;
        //***** Total Message Rate *****//
        m_sBusStatistics_LIN[ nChannelIndex ].m_unMsgPerSecond =
            m_sBusStatistics_LIN[ nChannelIndex ].m_unTotalMsgCount -
            m_sPrevStatData_LIN[ nChannelIndex ].m_unTotalMsgCount;
		
		FLOAT msgPerSec= (m_sBusStatistics_LIN[ nChannelIndex ].m_unMsgPerSecond /(FLOAT) m_dDiffTime );
		if( msgPerSec > 0.50 && msgPerSec < 1)
		{
			m_sBusStatistics_LIN[ nChannelIndex ].m_unMsgPerSecond =1;

		}
		else
		{
			m_sBusStatistics_LIN[ nChannelIndex ].m_unMsgPerSecond =
            static_cast<UINT>
            (m_sBusStatistics_LIN[ nChannelIndex ].m_unMsgPerSecond / m_dDiffTime );

		}

		//***** Wake up Rate***//
		m_sBusStatistics_LIN[ nChannelIndex ].m_unTotalWakeUpsRate =
			m_sBusStatistics_LIN[ nChannelIndex ].m_unTotalWakeUpsCount -
            m_sPrevStatData_LIN[ nChannelIndex ].m_unTotalWakeUpsCount;




        // Calculate Error Count & Rate
        m_sBusStatistics_LIN[ nChannelIndex ].m_unErrorTotalCount =
            m_sBusStatistics_LIN[ nChannelIndex ].m_unErrorRxCount +
            m_sBusStatistics_LIN[ nChannelIndex ].m_unErrorTxCount;
        m_sBusStatistics_LIN[ nChannelIndex ].m_dErrorRate =
            m_sBusStatistics_LIN[ nChannelIndex ].m_unErrorTotalCount -
            m_sPrevStatData_LIN[ nChannelIndex ].m_unErrorTotalCount;
        // Transmitted messages
        // Calculate Total Tx Message Rate
        m_sBusStatistics_LIN[nChannelIndex].m_dTotalTxMsgRate =
            (m_sBusStatistics_LIN[nChannelIndex].m_unTotalTxMsgCount -
             m_sPrevStatData_LIN[nChannelIndex].m_unTotalTxMsgCount);
        
        // Calculate Tx Error Rate
        m_sBusStatistics_LIN[ nChannelIndex ].m_dErrorTxRate =
            (m_sBusStatistics_LIN[ nChannelIndex ].m_unErrorTxCount -
             m_sPrevStatData_LIN[ nChannelIndex ].m_unErrorTxCount);
        // Received messages
        // Calculate Total Rx Message Rate
        m_sBusStatistics_LIN[nChannelIndex].m_dTotalRxMsgRate =
            (m_sBusStatistics_LIN[nChannelIndex].m_unTotalRxMsgCount -
             m_sPrevStatData_LIN[nChannelIndex].m_unTotalRxMsgCount);
        // Calculate Rx Error Rate
        m_sBusStatistics_LIN[ nChannelIndex ].m_dErrorRxRate =
            (m_sBusStatistics_LIN[ nChannelIndex ].m_unErrorRxCount -
             m_sPrevStatData_LIN[ nChannelIndex ].m_unErrorRxCount);
        SERROR_CNT sErrorCounter;
        sErrorCounter.m_ucRxErrCount = 0;
        sErrorCounter.m_ucTxErrCount = 0;

        /*if (m_pouDIL_LIN->DILL_GetErrorCount( sErrorCounter, nChannelIndex, ERR_CNT) == S_OK)
        {
            m_sBusStatistics_LIN[ nChannelIndex ].m_ucTxErrorCounter =
                sErrorCounter.m_ucTxErrCount;
            m_sBusStatistics_LIN[ nChannelIndex ].m_ucRxErrorCounter =
                sErrorCounter.m_ucRxErrCount;
        }

        sErrorCounter.m_ucRxErrCount = 0;
        sErrorCounter.m_ucTxErrCount = 0;

        if (m_pouDIL_LIN->DILL_GetErrorCount( sErrorCounter, nChannelIndex, PEAK_ERR_CNT) == S_OK)
        {
            m_sBusStatistics_LIN[ nChannelIndex ].m_ucTxPeakErrorCount=
                sErrorCounter.m_ucTxErrCount;
            m_sBusStatistics_LIN[ nChannelIndex ].m_ucRxPeakErrorCount =
                sErrorCounter.m_ucRxErrCount;
        }*/

        // Get the controller status
        LPARAM lParam = 0;

        if (m_pouDIL_LIN->DILL_GetControllerParams( lParam, nChannelIndex,
                HW_MODE) == S_OK)
        {
            m_sBusStatistics_LIN[ nChannelIndex ].m_ucStatus = (UCHAR)lParam;
        }

        //Bus Load
        DOUBLE dBusLoad  = m_sBusStatistics_LIN[ nChannelIndex ].m_unTotalBitsperSec / m_dDiffTime;
        m_sBusStatistics_LIN[ nChannelIndex ].m_unTotalBitsperSec = 0;
        m_sSubBusStatistics_LIN[ nChannelIndex ].m_unTotalBitsperSec = 0;

        if(dBusLoad != 0)
            dBusLoad = dBusLoad /
                       ( m_sBusStatistics_LIN[ nChannelIndex ].m_dBaudRate /** defBITS_KBUAD_RATE */);

        // Get the percentage load
        dBusLoad = dBusLoad * defMAX_PERCENTAGE_BUS_LOAD_LIN;

        // check for peak load
        if( dBusLoad > m_sBusStatistics_LIN[ nChannelIndex ].m_dPeakBusLoad )
        {
            // if peak load is greater than or equal to 100% assign it 99.99%
            if( dBusLoad > defMAX_PERCENTAGE_BUS_LOAD_LIN )
            {
                dBusLoad = defMAX_PERCENTAGE_BUS_LOAD_ALLOWED_LIN;
            }

            m_sBusStatistics_LIN[ nChannelIndex ].m_dPeakBusLoad = dBusLoad ;
        }

        m_sBusStatistics_LIN[ nChannelIndex ].m_dBusLoad = dBusLoad ;
        // Calculate avarage bus load
        m_sBusStatistics_LIN[ nChannelIndex ].m_dTotalBusLoad += dBusLoad;
        // Increament samples
        // Calculate Avarage bus load
        m_sBusStatistics_LIN[ nChannelIndex ].m_dAvarageBusLoad =
            m_sBusStatistics_LIN[ nChannelIndex ].m_dTotalBusLoad /
            m_sBusStatistics_LIN[ nChannelIndex ].m_nSamples;
        // Initialise previous values with current values.
        m_unPrevErrorTotalCount[ nChannelIndex ] =
            m_sBusStatistics_LIN[ nChannelIndex ].m_unErrorTotalCount;
        m_sBusStatistics_LIN[ nChannelIndex ].m_unDLCCount = 0;
        m_sPrevStatData_LIN[ nChannelIndex ] = m_sBusStatistics_LIN[ nChannelIndex ];
    }

    LeaveCriticalSection(&m_omCritSecBS);
}

