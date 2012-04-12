/*******************************************************************************
  Project       :  CAN-Monitor                                      
  FileName      :  wrapper.c                                          
  Description   :  
                   
  $Log:   Y:/EMC2/Projects/CAN_Monitor_Maintenance/Archives/Source/Misc/GCC/Wrapper.h_v  $
 * 
 *    Rev 1.7   27 Dec 2006 16:12:14   amb2kor
 * Changes for Global variable support: Multiple database support.
 * 
 *    Rev 1.6   02 Feb 2006 19:55:38   amb2kor
 * Changes due to CRH0031 ( Multinode Support )
 * 
 *    Rev 1.5   21 Jul 2005 19:05:24   amb2kor
 * changes due to Multi Channel concept. Trace is modified and
 * New API to get version information is added.
 * 
 *    Rev 1.4   Aug 07 2004 07:48:08   amb2kor
 * Changes due to CRH0026: After UT.
 * 
 *    Rev 1.3   30 Oct 2003 11:01:32   amb2kor
 *  
 * 
 *    Rev 1.2   02 May 2003 11:55:44   amb2kor
 * Removed parameter name sTxMsg from SendMsg function.
 * 
 *    Rev 1.1   24 Apr 2003 09:50:22   amb2kor
 * For system testing
 * 
 *    Rev 1.0.1.1   03 Apr 2003 12:20:48   amb2kor
 * prototype of SendMsg function is changed to accept single structure.
 * 
 *    Rev 1.0   14 Feb 2003 14:34:36   amb2kor
 * Base Version : Latest version From CAN-Monitor2.x project
 * Version Label in CAN-Monitor2.x : CAN-Monitor2.0 EVAL/CMRL
 * 
 * 
  
  Author(s)     :  Amarnath Shastry
  Date Created  :  20/03/2002                                             
  Modified By   :  Ravikumar Patil  on 04.03.03
*******************************************************************************/
#ifndef _WRAPPER_H_
#define _WRAPPER_H_


typedef UINT (*APPFUNCPROC) (STCAN_MSG,HMODULE);
typedef UINT (*LOGENABLE) (BOOL);
//typedef int (*LOGDISABLE) (BOOL);
typedef UINT (*WRITETOLOGFILE) (char*);
typedef UINT (*WRITETOTRACE) (char*);
typedef void (*RESETCONTROLLER) (BOOL);
typedef UINT (*CONTROLLERMODE) (BOOL);
typedef UINT (*GOONLINEOFFLINE) (BOOL,HMODULE) ;
typedef UINT (*DISCONNECT) (BOOL) ;
typedef UINT (*STARTSTOPTIMER) (char*,UINT,BOOL,HMODULE) ;
typedef UINT (*SETTIMERVAL) (char*,UINT,HMODULE);
typedef UINT (*ENABLEDISABLEMSGHANDLERS)(BOOL,HMODULE);
typedef UINT (*ENABLEDISABLEKEYHANDLERS)(BOOL,HMODULE);
typedef UINT (*ENABLEDISABLEERRORHANDLERS)(BOOL,HMODULE);
typedef UINT (*DLLMSGTXONOFF)(BOOL,HMODULE);
typedef HMODULE (*DLLGETNODEHANDLER)(char*);

const UINT MAX_SEND_TIME = 3; // Millisecond
const INT ERR_INVALID_ADDRESS   = -100;


// Please insert any new wrapper prototype here
// Prototypes Start
UINT SendMsg(STCAN_MSG);
UINT EnableLogging() ;
UINT DisableLogging() ;
UINT WriteToLogFile(char*) ;
UINT Trace(char*,...) ;
void ResetController(BOOL) ;
UINT SetPassiveMode() ;
UINT SetActiveMode() ;
UINT GoOnline() ;
UINT GoOffline() ;
UINT Disconnect() ;
UINT StartTimer(char*,UINT) ;
UINT StopTimer(char*) ;
BOOL SetTimerVal(char*,UINT);
BOOL EnableMsgHandlers(BOOL);
BOOL EnableErrorHandlers(BOOL);
BOOL EnableKeyHandlers(BOOL);
BOOL bGetProgramVersion(int* pnMajorVersion, int* pnMinorVersion,HANDLE hModuleHandle );
BOOL EnableDisableMsgTx(BOOL);
HMODULE hGetDllHandle(char*);
// Prototypes End


#endif