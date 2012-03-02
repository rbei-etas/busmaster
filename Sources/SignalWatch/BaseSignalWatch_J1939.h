/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  BaseSignalWatch_J1939.h
  Description   :  
  $Log:   X:/Archive/Sources/SignalWatch/BaseSignalWatch_J1939.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:18:42   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.0   01 Mar 2011 17:05:30   CANMNTTM
 *  

  Author(s)     :  Pradeep Kadoor
  Date Created  :  16/02/2011
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#if !defined BASESIGNALWATCH_J1939_H__INCLUDED_
#define BASESIGNALWATCH_J1939_H__INCLUDED_

class CBaseSignalWatch_J1939
{
public:
    virtual HRESULT SW_DoInitialization(void) = 0;
    virtual HRESULT SW_ShowAddDelSignalsDlg(CWnd* pParent, CMainEntryList* podMainSubList) = 0;
    virtual HRESULT SW_ShowSigWatchWnd(CWnd* pParent, INT nCmd) = 0;
    virtual HRESULT SW_GetConfigSize(void) = 0;
    virtual HRESULT SW_GetConfigData(void* pbyConfigData) = 0;
    virtual HRESULT SW_SetConfigData(const void* pbyConfigData) = 0;
    virtual HRESULT SW_ClearSigWatchWnd(void) = 0;
    virtual HRESULT SW_UpdateMsgInterpretObj(void* RefObj) = 0;
    virtual BOOL    SW_IsWindowVisible(void) = 0;
    virtual HRESULT SW_SetDisplayMode(BOOL bHex) = 0;
};

#endif //BASESIGNALWATCH_J1939_H__INCLUDED_