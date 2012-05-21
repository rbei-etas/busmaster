/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  ReadCanMsg.h
  Description   :  
  $Log:   X:/Archive/Sources/DIL_J1939/ReadCanMsg.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 18:48:38   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.0   06 Dec 2010 18:47:22   rac2kor
 *  

  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#if !defined READCANMSG_H__INCLUDED_
#define READCANMSG_H__INCLUDED_

#define DEF_MAX_SIMULATED_NODE 32

typedef CMap<HANDLE,HANDLE,BYTE,BYTE> CHandleToNodeMagrMap;
class CReadCanMsg
{
protected:
    CPARAM_THREADPROC m_ouThreadUtil;

    // To reset the object
    void vReset(void);
public:
    HANDLE m_ahActionEvent[DEF_MAX_SIMULATED_NODE + 1]; // One additional
    CHandleToNodeMagrMap m_omHandleToNodeMgrMap;
    int m_nEvents;

public:
    CReadCanMsg(void);
    ~CReadCanMsg(void);

    // event handle is the action event of the thread. This'll be necessary
    // while closing the thread.
    void vRetrieveDataFromBuffer(BYTE byIndex);

    // To add event handle for a client 
    HRESULT AddEventHandle(HANDLE hHandle, BYTE byNodeMgrIndex);

    // To remove event handle of a client 
    BOOL bDeleteEventHandle(HANDLE handle);

    // Do initialisation operations
    void vDoInit(void);

    // Do closure operations
    void vDoExit(void);
};

#endif // READCANMSG_H__INCLUDED_
