//***********************************************************************************
//** Module       : NetworkStats.cpp
//** Directory    : $/Frame/DeviceInterface/Vendor/RBINSTUB/NetworkStats
//** Author/Date  : Arun Kumar/RBIN/17.2006
//** Description  : Definition of Network statictics class interface
//** --------------------------------------------------------------------------------
//** Changes
//** Name/Date    : <Name>/RBIN/<mm/yyyy>
//** Description  : <Description>
//***********************************************************************************
//
#include "stdafx.h"
#include "DataTypes\Base_FlexRay_Buffer.h"
#include "FlexRaynetworkstats.h"
#include "DIL_Interface/DIL_Interface_Extern.h"
#include "TimeManager.h"
void* CFlexRayNetworkStats::sm_pouBSFlexRay;


/**
 * \param[in]
 * \return Function return Status
 *
 * Updates the busstatistics parameters using
 * vUpdateBusStatistics Function
 */
int ReadBSDataBuffer(CFlexRayNetworkStats* pBSFlexRay)
{
    ASSERT(pBSCan != nullptr);
    while (pBSFlexRay->m_ouFlexRayBufFSE.GetMsgCount() > 0)
    {
        static s_FLXMSG sFlexRayData;
        int Result = pBSFlexRay->m_ouFlexRayBufFSE.ReadFromBuffer(&sFlexRayData);
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
            pBSFlexRay->UpdateNetworkStatistics(sFlexRayData.stcDataMsg.m_lHeaderInfoFlags);
        }

    }
    return 0;
}

/**
 * \param[in]
 *
 * Thread Function that is used to read the driver data and
 * updates the Bus Statistics.
 */
DWORD WINAPI BSFlexRayDataReadThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return ((DWORD)-1);
    }

    CFlexRayNetworkStats* pBusStatistics = static_cast<CFlexRayNetworkStats*> (pThreadParam->m_pBuffer);

    if (pBusStatistics == nullptr)
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
                ReadBSDataBuffer(pBusStatistics); // Retrieve message from the driver
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

//********************************************************************************
//  Function Name       : CNetworkStats
//  Input(s)            : None
//  Output              : CNetworkStats
//  Description         : Contructor of CNetworkStats
//  Member of           : CNetworkStats
//  Friend of           : None
//  Author              : Arun Kumar
//  Creation Date       : 17th Oct, 2006
//  Modifications       :
//********************************************************************************
CFlexRayNetworkStats::CFlexRayNetworkStats(void)
{
    ResetNetworkStats();
}

//********************************************************************************
//  Function Name       : ~CNetworkStats
//  Input(s)            : None
//  Output              :
//  Description         : Destructor of CNetworkStats
//  Member of           : CNetworkStats
//  Friend of           : None
//  Author              : Arun Kumar
//  Creation Date       : 17th Oct, 2006
//  Modifications       :
//********************************************************************************
CFlexRayNetworkStats::~CFlexRayNetworkStats(void)
{
}

//********************************************************************************
//  Function Name       : UpdateNetworkStatistics
//  Input(s)            :
//  Output              :
//  Description         : Initialise CNetworkStats with n/w statictics data
//  Member of           : CNetworkStats
//  Friend of           : None
//  Author              : Arun Kumar
//  Creation Date       : 17th Oct, 2006
//  Modifications       :
//********************************************************************************
void CFlexRayNetworkStats::UpdateNetworkStatistics(DWORD dwInfoFlag)
{
    // Channel A Counter
    if ((dwInfoFlag & RBIN_FLXHDRINFO_CHANNELID) == CHANNEL_A)
    {
        if (dwInfoFlag & RBIN_FLXHDRINFO_SYNCFRAME)
        {
            m_sFlexRayBusStats.m_SyncFrameChA++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_STARTUPFRAME)
        {
            m_sFlexRayBusStats.m_StartUpFrameChA++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_VALIDCOMMUNICATIONEVENT)
        {
            m_sFlexRayBusStats.m_ValidCommEventChA++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_SYNTAXERROR)
        {
            m_sFlexRayBusStats.m_SyntaxErrorChA++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_CONTENTERROR)
        {
            m_sFlexRayBusStats.m_ContentErrorChA++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_BOUNDARYVIOLATION)
        {
            m_sFlexRayBusStats.m_BoundaryViolationChA++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_TXCONFLICT)
        {
            m_sFlexRayBusStats.m_TxConflictChA++;
        }

        if ((dwInfoFlag & RBIN_FLXHDRINFO_FRAMETYPE) == 0)
        {
            m_sFlexRayBusStats.m_StaticFramesChA++;
        }
        else
        {
            m_sFlexRayBusStats.m_DynamicFramesChA++;
        }
        UpdateTxRx(CHANNEL_A, dwInfoFlag);
    } // Channel Condition
    else  // Channel B Counter
    {
        if (dwInfoFlag & RBIN_FLXHDRINFO_SYNCFRAME)
        {
            m_sFlexRayBusStats.m_SyncFrameChB++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_STARTUPFRAME)
        {
            m_sFlexRayBusStats.m_StartUpFrameChB++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_VALIDCOMMUNICATIONEVENT)
        {
            m_sFlexRayBusStats.m_ValidCommEventChB++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_SYNTAXERROR)
        {
            m_sFlexRayBusStats.m_SyntaxErrorChB++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_CONTENTERROR)
        {
            m_sFlexRayBusStats.m_ContentErrorChB++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_BOUNDARYVIOLATION)
        {
            m_sFlexRayBusStats.m_BoundaryViolationChB++;
        }

        if (dwInfoFlag & RBIN_FLXHDRINFO_TXCONFLICT)
        {
            m_sFlexRayBusStats.m_TxConflictChB++;
        }

        if ((dwInfoFlag & RBIN_FLXHDRINFO_FRAMETYPE) == 0)
        {
            m_sFlexRayBusStats.m_StaticFramesChB++;
        }
        else
        {
            m_sFlexRayBusStats.m_DynamicFramesChB++;
        }
        UpdateTxRx(CHANNEL_B, dwInfoFlag);
    }


}

void CFlexRayNetworkStats::UpdateTxRx(ECHANNEL eChannel, DWORD dwInfoFlag)
{
    if(eChannel == CHANNEL_A)
    {
        if (dwInfoFlag & RBIN_FLXHDRINFO_SELFRECEPTION)
        {
            if (dwInfoFlag & RBIN_FLXHDRINFO_SYNCFRAME)
            {
                m_sFlexRayTxBusStats.m_SyncFrameChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_STARTUPFRAME)
            {
                m_sFlexRayTxBusStats.m_StartUpFrameChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_VALIDCOMMUNICATIONEVENT)
            {
                m_sFlexRayTxBusStats.m_ValidCommEventChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_SYNTAXERROR)
            {
                m_sFlexRayTxBusStats.m_SyntaxErrorChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_CONTENTERROR)
            {
                m_sFlexRayTxBusStats.m_ContentErrorChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_BOUNDARYVIOLATION)
            {
                m_sFlexRayTxBusStats.m_BoundaryViolationChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_TXCONFLICT)
            {
                m_sFlexRayTxBusStats.m_TxConflictChA++;
            }

            if ((dwInfoFlag & RBIN_FLXHDRINFO_FRAMETYPE) == 0)
            {
                m_sFlexRayTxBusStats.m_StaticFramesChA++;
            }
            else
            {
                m_sFlexRayTxBusStats.m_DynamicFramesChA++;
            }
        }
        else
        {
            if (dwInfoFlag & RBIN_FLXHDRINFO_SYNCFRAME)
            {
                m_sFlexRayRxBusStats.m_SyncFrameChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_STARTUPFRAME)
            {
                m_sFlexRayRxBusStats.m_StartUpFrameChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_VALIDCOMMUNICATIONEVENT)
            {
                m_sFlexRayRxBusStats.m_ValidCommEventChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_SYNTAXERROR)
            {
                m_sFlexRayRxBusStats.m_SyntaxErrorChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_CONTENTERROR)
            {
                m_sFlexRayRxBusStats.m_ContentErrorChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_BOUNDARYVIOLATION)
            {
                m_sFlexRayRxBusStats.m_BoundaryViolationChA++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_TXCONFLICT)
            {
                m_sFlexRayRxBusStats.m_TxConflictChA++;
            }

            if ((dwInfoFlag & RBIN_FLXHDRINFO_FRAMETYPE) == 0)
            {
                m_sFlexRayRxBusStats.m_StaticFramesChA++;
            }
            else
            {
                m_sFlexRayTxBusStats.m_DynamicFramesChA++;
            }
        }
    }
    else if(eChannel == CHANNEL_B)
    {
        if (dwInfoFlag & RBIN_FLXHDRINFO_SELFRECEPTION)
        {
            if (dwInfoFlag & RBIN_FLXHDRINFO_SYNCFRAME)
            {
                m_sFlexRayTxBusStats.m_SyncFrameChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_STARTUPFRAME)
            {
                m_sFlexRayTxBusStats.m_StartUpFrameChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_VALIDCOMMUNICATIONEVENT)
            {
                m_sFlexRayTxBusStats.m_ValidCommEventChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_SYNTAXERROR)
            {
                m_sFlexRayTxBusStats.m_SyntaxErrorChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_CONTENTERROR)
            {
                m_sFlexRayTxBusStats.m_ContentErrorChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_BOUNDARYVIOLATION)
            {
                m_sFlexRayTxBusStats.m_BoundaryViolationChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_TXCONFLICT)
            {
                m_sFlexRayTxBusStats.m_TxConflictChB++;
            }

            if ((dwInfoFlag & RBIN_FLXHDRINFO_FRAMETYPE) == 0)
            {
                m_sFlexRayTxBusStats.m_StaticFramesChB++;
            }
            else
            {
                m_sFlexRayTxBusStats.m_DynamicFramesChB++;
            }
        }
        else
        {
            if (dwInfoFlag & RBIN_FLXHDRINFO_SYNCFRAME)
            {
                m_sFlexRayRxBusStats.m_SyncFrameChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_STARTUPFRAME)
            {
                m_sFlexRayRxBusStats.m_StartUpFrameChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_VALIDCOMMUNICATIONEVENT)
            {
                m_sFlexRayRxBusStats.m_ValidCommEventChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_SYNTAXERROR)
            {
                m_sFlexRayRxBusStats.m_SyntaxErrorChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_CONTENTERROR)
            {
                m_sFlexRayRxBusStats.m_ContentErrorChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_BOUNDARYVIOLATION)
            {
                m_sFlexRayRxBusStats.m_BoundaryViolationChB++;
            }

            if (dwInfoFlag & RBIN_FLXHDRINFO_TXCONFLICT)
            {
                m_sFlexRayRxBusStats.m_TxConflictChB++;
            }

            if ((dwInfoFlag & RBIN_FLXHDRINFO_FRAMETYPE) == 0)
            {
                m_sFlexRayRxBusStats.m_StaticFramesChB++;
            }
            else
            {
                m_sFlexRayTxBusStats.m_DynamicFramesChB++;
            }
        }
    }
}

SSUBFLEXRAYBUSSTATISTICS* CFlexRayNetworkStats::nGetChannelStats()
{
    m_sFlexRayBusStats.m_SyncFramePerSecChA =  m_sFlexRayBusStats.m_SyncFrameChA - m_sPrevFlexRayBusStats.m_SyncFrameChA;
    m_sFlexRayBusStats.m_SyncFramePerSecChB= m_sFlexRayBusStats.m_SyncFrameChB -  m_sPrevFlexRayBusStats.m_SyncFrameChB ;

    m_sFlexRayBusStats.m_NullFramePerSecChA = m_sFlexRayBusStats.m_NullFrameChA -  m_sPrevFlexRayBusStats.m_NullFrameChA ;
    m_sFlexRayBusStats.m_NullFramePerSecChB = m_sFlexRayBusStats.m_NullFrameChB - m_sPrevFlexRayBusStats.m_NullFrameChB;

    //Storing the previous data
    m_sPrevFlexRayBusStats.m_SyncFrameChA = m_sFlexRayBusStats.m_SyncFrameChA ;
    m_sPrevFlexRayBusStats.m_SyncFrameChB = m_sFlexRayBusStats.m_SyncFrameChB ;

    m_sPrevFlexRayBusStats.m_NullFrameChA = m_sFlexRayBusStats.m_NullFrameChB;
    m_sPrevFlexRayBusStats.m_NullFrameChA = m_sFlexRayBusStats.m_NullFrameChA;

    return &m_sFlexRayBusStats;
}

SSUBFLEXRAYBUSSTATISTICS* CFlexRayNetworkStats::nGetTxStats()
{
    m_sFlexRayBusStats.m_SyncFramePerSecChA =  m_sFlexRayBusStats.m_SyncFrameChA - m_sFlexRayPrevTxBusStats.m_SyncFrameChA;
    m_sFlexRayBusStats.m_SyncFramePerSecChB= m_sFlexRayBusStats.m_SyncFrameChB -  m_sFlexRayPrevTxBusStats.m_SyncFrameChB ;

    m_sFlexRayBusStats.m_NullFramePerSecChA = m_sFlexRayBusStats.m_NullFrameChA -  m_sFlexRayPrevTxBusStats.m_NullFrameChA ;
    m_sFlexRayBusStats.m_NullFramePerSecChB = m_sFlexRayBusStats.m_NullFrameChB - m_sFlexRayPrevTxBusStats.m_NullFrameChB;

    //Storing the previous data
    m_sFlexRayPrevTxBusStats.m_SyncFrameChA = m_sFlexRayBusStats.m_SyncFrameChA ;
    m_sFlexRayPrevTxBusStats.m_SyncFrameChB = m_sFlexRayBusStats.m_SyncFrameChB ;

    m_sFlexRayPrevTxBusStats.m_NullFrameChA = m_sFlexRayBusStats.m_NullFrameChB;
    m_sFlexRayPrevTxBusStats.m_NullFrameChA = m_sFlexRayBusStats.m_NullFrameChA;
    return &m_sFlexRayTxBusStats;
}

SSUBFLEXRAYBUSSTATISTICS* CFlexRayNetworkStats::nGetRxStats()
{
    m_sFlexRayBusStats.m_SyncFramePerSecChA =  m_sFlexRayBusStats.m_SyncFrameChA - m_sFlexRayPrevRxBusStats.m_SyncFrameChA;
    m_sFlexRayBusStats.m_SyncFramePerSecChB= m_sFlexRayBusStats.m_SyncFrameChB -  m_sFlexRayPrevRxBusStats.m_SyncFrameChB ;

    m_sFlexRayBusStats.m_NullFramePerSecChA = m_sFlexRayBusStats.m_NullFrameChA -  m_sFlexRayPrevRxBusStats.m_NullFrameChA ;
    m_sFlexRayBusStats.m_NullFramePerSecChB = m_sFlexRayBusStats.m_NullFrameChB - m_sFlexRayPrevRxBusStats.m_NullFrameChB;

    //Storing the previous data
    m_sFlexRayPrevRxBusStats.m_SyncFrameChA = m_sFlexRayBusStats.m_SyncFrameChA ;
    m_sFlexRayPrevRxBusStats.m_SyncFrameChB = m_sFlexRayBusStats.m_SyncFrameChB ;

    m_sFlexRayPrevRxBusStats.m_NullFrameChA = m_sFlexRayBusStats.m_NullFrameChB;
    m_sFlexRayPrevRxBusStats.m_NullFrameChA = m_sFlexRayBusStats.m_NullFrameChA;
    return &m_sFlexRayRxBusStats;
}


/**
 * Calculates the differential time in sec
 */
void CFlexRayNetworkStats::vCalculateDiffTime(void)
{

    // check if the previous time value is not stored. take 1.0s as initial
    // value for calculation.
    if(m_unPreviousTime != -1 )
    {
        m_dDiffTime         = CTimeManager::nCalculateCurrTimeStamp(FALSE) -
                              m_unPreviousTime;
        m_unPreviousTime += static_cast<UINT>(m_dDiffTime);
        m_dDiffTime         = m_dDiffTime / defDIV_FACT_FOR_SECOND;
    }
    else
    {
        m_unPreviousTime = CTimeManager::nCalculateCurrTimeStamp(FALSE);
    }
}



/**
 * \return return the error state
 * \ Measurement period always begins (or resets) from the time of connection
 *
 * Starts the Read thread.
 */
BOOL CFlexRayNetworkStats::bStartFlexRayBSReadThread(void)
{
    m_ouReadThread.m_pBuffer = this;
    // m_ouReadThread.bStartThread(BSFlexRayDataReadThreadProc);

    return TRUE;
}

/**
 * \return contain Error information
 *
 * Does the client registration with driver and starts the Read
 * Thread.
 */
HRESULT CFlexRayNetworkStats::BSC_DoInitialization(void)
{
    if (DIL_GetInterface(FLEXRAY, (void**)&m_pouDIL_FLEXRAY) == S_OK)
    {
        DWORD dwClientId = 0;
        m_pouDIL_FLEXRAY->DILF_RegisterClient(TRUE, dwClientId, CAN_MONITOR_NODE);
        m_pouDIL_FLEXRAY->DILF_ManageMsgBuf(MSGBUF_ADD, dwClientId, &m_ouFlexRayBufFSE);
    }
    vInitialiseBSData();

    sm_pouBSFlexRay = this;

    m_unPreviousTime = -1;

    //Start the read thread

    return bStartFlexRayBSReadThread()?S_OK:S_FALSE;
    //return true;
}

/**
 * \req RS_24_08 Standard frames are considered.
 * \req RS_24_09 Extended frames are considered.
 * \req RS_24_10 RTR frames are considered.
 * \req RS_24_11 Error frames occurs under the purview of status data
 *
 * Initialises the m_sbusstatistics structor
 */
void CFlexRayNetworkStats::vInitialiseBSData(void)
{

}



//********************************************************************************
//  Function Name       : ResetNetworkStats
//  Input(s)            : None
//  Output              : DWORD
//  Description         : Reset the N/W statictic data to zero
//  Member of           : CNetworkStats
//  Friend of           : None
//  Author              : Arun Kumar
//  Creation Date       : 17th Oct, 2006
//  Modifications       :
//********************************************************************************
void CFlexRayNetworkStats::ResetNetworkStats(void)
{
    m_sFlexRayBusStats.m_NullFrameChA          = 0;
    m_sFlexRayBusStats.m_SyncFrameChA          = 0;
    m_sFlexRayBusStats.m_StartUpFrameChA       = 0;
    m_sFlexRayBusStats.m_ValidCommEventChA     = 0;
    m_sFlexRayBusStats.m_SyntaxErrorChA        = 0;
    m_sFlexRayBusStats.m_ContentErrorChA       = 0;
    m_sFlexRayBusStats.m_BoundaryViolationChA  = 0;
    m_sFlexRayBusStats.m_TxConflictChA         = 0;
    m_sFlexRayBusStats.m_StaticFramesChA       = 0;
    m_sFlexRayBusStats.m_DynamicFramesChA      = 0;

    m_sFlexRayBusStats.m_NullFrameChB          = 0;
    m_sFlexRayBusStats.m_SyncFrameChB          = 0;
    m_sFlexRayBusStats.m_StaticFramesChB       = 0;
    m_sFlexRayBusStats.m_StartUpFrameChB       = 0;
    m_sFlexRayBusStats.m_ValidCommEventChB     = 0;
    m_sFlexRayBusStats.m_SyntaxErrorChB        = 0;
    m_sFlexRayBusStats.m_ContentErrorChB       = 0;
    m_sFlexRayBusStats.m_BoundaryViolationChB  = 0;
    m_sFlexRayBusStats.m_TxConflictChB         = 0;
    m_sFlexRayBusStats.m_DynamicFramesChB      = 0;
}
