//***********************************************************************************
//** Module       : NetworkStats.h
//** Directory    : $/Frame/DeviceInterface/Vendor/RBINSTUB/NetworkStats
//** Author/Date  : Arun Kumar/RBIN/17.2006
//** Description  : Declaration Network statictics class
//** --------------------------------------------------------------------------------
//** Changes
//** Name/Date    : <Name>/RBIN/<mm/yyyy>
//** Description  : <Description>
//***********************************************************************************

#pragma once

//#include "include/BaseDefs.h"
#include "Utility/Utility_Thread.h"
#include "Datatypes/MsgBufAll_Datatypes.h"
#include "DIL_Interface/BaseDIL_FLEXRAY.h"

struct sSUBFLEXRAYBUSSTATISTICS
{
    DWORD m_NullFrameChA; // Null Frame Counter for Channel A
    DWORD m_NullFrameChB; // Null Frame Counter for Channel B
    DWORD m_NullFramePerSecChA; // Null Frame Counter for Channel A
    DWORD m_NullFramePerSecChB; // Null Frame Counter for Channel B
    DWORD m_SyncFrameChA;
    DWORD m_SyncFrameChB;
    DWORD m_SyncFramePerSecChA;
    DWORD m_SyncFramePerSecChB;
    DWORD m_StartUpFrameChA;
    DWORD m_StartUpFrameChB;
    DWORD m_ValidCommEventChA;
    DWORD m_ValidCommEventChB;
    DWORD m_SyntaxErrorChA;
    DWORD m_SyntaxErrorChB;
    DWORD m_ContentErrorChA;
    DWORD m_ContentErrorChB;
    DWORD m_BoundaryViolationChA;
    DWORD m_BoundaryViolationChB;
    DWORD m_TxConflictChA;
    DWORD m_TxConflictChB;
    DWORD m_StaticFramesChA;
    DWORD m_StaticFramesChB;
    DWORD m_DynamicFramesChA;
    DWORD m_DynamicFramesChB;
};
typedef sSUBFLEXRAYBUSSTATISTICS SSUBFLEXRAYBUSSTATISTICS;

class CFlexRayNetworkStats
{
public:
    CFlexRayNetworkStats(void);
    ~CFlexRayNetworkStats(void);

    void UpdateNetworkStatistics(DWORD dwInfoFlag);

    /*   DWORD GetNullFrameCount(ECHANNEL eChannel);
    DWORD GetNullFrameCountperSec(ECHANNEL eChannel);
       DWORD GetSyncFrameCount(ECHANNEL eChannel);
    DWORD GetSyncFrameCountperSec(ECHANNEL eChannel);
       DWORD GetStartUpFrameCount(ECHANNEL eChannel);
       DWORD GetValidCommEventCount(ECHANNEL eChannel);
       DWORD GetSyntaxErrorCount(ECHANNEL eChannel);
    DWORD GetSyntaxErrorCountperSec(ECHANNEL eChannel);
       DWORD GetContentErrorCount(ECHANNEL eChannel);
       DWORD GetBoundaryViolationCount(ECHANNEL eChannel);
       DWORD GetTxConflictCount(ECHANNEL eChannel);
       DWORD GetStaticFrameCount(ECHANNEL eChannel);
       DWORD GetDynamicFrameCount(ECHANNEL eChannel);*/
    HRESULT BSC_DoInitialization(void);
    void vInitialiseBSData(void);
    void vCalculateDiffTime(void);
    DWORD dGetTransmittedStats(ECHANNEL eChannel);
    DWORD dGetReceivedStats(ECHANNEL eChannel);
    CFLEXBufFSE m_ouFlexRayBufFSE;
    BOOL bStartFlexRayBSReadThread(void);
    void UpdateTxRx(ECHANNEL eChannel, DWORD dwInfoFlag);
    SSUBFLEXRAYBUSSTATISTICS* nGetTxStats();
    SSUBFLEXRAYBUSSTATISTICS* nGetRxStats();
    SSUBFLEXRAYBUSSTATISTICS* nGetChannelStats();

private:

    static void*  sm_pouBSFlexRay;
    //used to Creating threads
    CPARAM_THREADPROC m_ouReadThread;

    double m_dDiffTime;
    INT  m_unPreviousTime;

    SSUBFLEXRAYBUSSTATISTICS      m_sFlexRayBusStats;
    SSUBFLEXRAYBUSSTATISTICS      m_sPrevFlexRayBusStats;

    SSUBFLEXRAYBUSSTATISTICS      m_sFlexRayTxBusStats;
    SSUBFLEXRAYBUSSTATISTICS      m_sFlexRayRxBusStats;

    SSUBFLEXRAYBUSSTATISTICS      m_sFlexRayPrevTxBusStats;
    SSUBFLEXRAYBUSSTATISTICS      m_sFlexRayPrevRxBusStats;

    CBaseDIL_FLEXRAY* m_pouDIL_FLEXRAY;

    void ResetNetworkStats(void);
};
