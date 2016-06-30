#pragma once

#include <windows.h>
#include <stdio.h>
#include "Common.h"



typedef UINT (*LOGENABLE) (BOOL);
typedef UINT (*WRITETOLOGFILE) (char*);
typedef UINT (*WRITETOTRACE) (char*);
typedef UINT (*DISCONNECT) (BOOL) ;
typedef UINT (*STARTSTOPTIMER) (char*,UINT,BOOL,HMODULE) ;
typedef UINT (*SETTIMERVAL) (char*,UINT,HMODULE);
typedef HMODULE (*DLLGETNODEHANDLER)(char*);

typedef int (*KEYPRESSED)();
typedef void (*SETKEYPRESSED)(KEYPRESSED);
typedef unsigned int (*TIMENOW)();
typedef void (*DLLTIMENOW)(TIMENOW);


GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetEnableLoggingProcAddress(LOGENABLE pEnableLoggingFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetDisableLoggingProcAddress(LOGENABLE pDisableLoggingFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetWriteToLogFileProcAddress(WRITETOLOGFILE pLogFileFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetTraceProcAddress(WRITETOTRACE pTraceFunc);

GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetDisconnectProcAddress(DISCONNECT pDisconnect);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetConnectProcAddress(DISCONNECT pConnect);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetStopTimerProcAddress(STARTSTOPTIMER pStopTimer);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetStartTimerProcAddress(STARTSTOPTIMER pStartTimer);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSetTimerValProcAddress(SETTIMERVAL pSetTimerVal);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGetDllHandleProcAddress(DLLGETNODEHANDLER pGetNodeHandler);

GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetKeyPressed(KEYPRESSED pKeyPressed);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetTimeNow(TIMENOW pTimeNow);

GCC_EXTERN int keypressed_CAPL();
GCC_EXTERN unsigned int timenow_CAPL();


GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT EnableLogging() ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT DisableLogging() ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT WriteToLogFile(char*) ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT Trace(char*,...);

GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT Connect() ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT Disconnect() ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT StartTimer(char*,UINT) ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT StopTimer(char*) ;
GCC_EXTERN /*extern "C"*/ BOOL GCC_EXPORT SetTimerVal(char*,UINT);
GCC_EXTERN /*extern "C"*/ BOOL GCC_EXPORT bGetProgramVersion(int* pnMajorVersion, int* pnMinorVersion,HMODULE hModuleHandle );
GCC_EXTERN /*extern "C"*/ HMODULE GCC_EXPORT GetDllHandle(char*);



