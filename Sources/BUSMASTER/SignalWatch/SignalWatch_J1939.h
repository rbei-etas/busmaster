/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  SignalWatch_J1939.h
  Description   :
  $Log:   X:/Archive/Sources/SignalWatch/SignalWatch_J1939.h_v  $
 *
 *    Rev 1.1   15 Apr 2011 19:18:46   CANMNTTM
 * Added RBEI Copyright information.
 *
 *    Rev 1.0   01 Mar 2011 17:05:30   CANMNTTM
 *

/**
 * \file      SignalWatch_J1939.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "SignalWatch_Resource.h"
//#include "Datatypes/MsgBufAll_Datatypes.h"

/* MSG INTERPRETATION */
#include "Utility/MsgInterpretation.h"
#include "Utility/Utility_Thread.h"
/* DIL CAN INTERFACE */
//#include "DIL_Interface_extern.h"
#include "BaseDIL_CAN.h"
#include "BaseDIL_J1939.h"
#include "BaseSignalWatchImp.h"
#include "MsgBufVSE.h"

class CSignalWatch_J1939 : public CBaseSignalWatchImp
{
private:
    CMsgBufVSE m_ouMsgBufVSE_J;
public:
    virtual HRESULT DoInitialization();
    virtual HRESULT StartSigWatchReadThread();
    CSignalWatch_J1939();
    ~CSignalWatch_J1939();

private:
    void vDisplayInSigWatchWnd(STJ1939_MSG& sMsg);
    int ReadJ1939DataBuffer( CSignalWatch_J1939* pSWJ1939 );
    static DWORD WINAPI SigWatchDataReadThreadProc_J( LPVOID pVoid );
};
