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

  Author(s)     :  Pradeep Kadoor
  Date Created  :  16/02/2011
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#if !defined SIGNALWATCH_J1939_H__INCLUDED_
#define SIGNALWATCH_J1939_H__INCLUDED_

#include "SignalWatch_Resource.h"
#include "Include/Basedefs.h"
#include "Datatypes/MsgBufAll_Datatypes.h"
#include "Datatypes/MsgBufVSE.h"
#include "Datatypes/SigWatch_Datatypes.h"
#include "Datatypes/J1939_Datatypes.h"
#include "SignalWatchDefs.h"
#include "DataTypes/MainSubEntry.h"
#include "SigWatchAddDelDlg.h"
#include "SigWatchDlg.h"
/* MSG INTERPRETATION */
#include "Utility/MsgInterpretation.h"
#include "Utility/Utility_Thread.h"
/* DIL CAN INTERFACE */
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/BaseDIL_J1939.h"

#include "BaseSignalWatch_J1939.h"

class CSignalWatch_J1939 : CBaseSignalWatch_J1939
{
public:
    CMsgBufVSE m_ouMsgBufVSE_J;

private:
    BOOL m_bHex;
    CSigWatchDlg* m_pouSigWnd;
    CMsgInterpretationJ1939* m_pMsgInterPretObj_J;
    CPARAM_THREADPROC m_ouReadThread;
    CRITICAL_SECTION m_omCritSecSW;

public:
    BOOL InitInstance(void);
    int ExitInstance(void);

    HRESULT SW_DoInitialization(void);
    HRESULT SW_ShowAddDelSignalsDlg(CWnd* pParent, CMainEntryList* podMainSubList);
    HRESULT SW_ShowSigWatchWnd(CWnd* pParent, INT nCmd);
    HRESULT SW_SetDisplayMode(BOOL bHex);
    HRESULT SW_GetConfigSize(void);
    HRESULT SW_GetConfigData(void* pbyConfigData);    
    HRESULT SW_SetConfigData(const void* pbyConfigData);
    HRESULT SW_ClearSigWatchWnd(void);
    HRESULT SW_UpdateMsgInterpretObj(void* RefObj);
    BOOL    SW_IsWindowVisible(void);
    void vDisplayInSigWatchWnd(STJ1939_MSG& sMsg);

private:
    BOOL bStartSigWatchReadThread(void);

};

#endif //SIGNALWATCH_J1939_H__INCLUDED_