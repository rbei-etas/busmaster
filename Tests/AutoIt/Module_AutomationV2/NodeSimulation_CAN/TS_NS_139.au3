;==================================================================Test Cases/Test Data ===========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_139
; TestCases			:		Executing UnLoad DLL Handler
; Test Data			:		cfxNS139.cfx
; Test Setup		:		Edit the DLL handler I.e. OnDLL_UnLoad() and save it and select menu item User Program-->Unload

; Expected Results  : 		The DLL Unload handlers will be executed displaying the output in notification window.
;
;=================================================================Test Procedure ====================================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_129.au3****"&@CRLF)
_launchApp()
WinActivate($WIN_BUSMASTER)
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxNSCAN_139.cfx")
	sleep(1000)
	_ExecuteUnloadDLLHandler("ForCAN")
 	$isAppNotRes=_CloseApp()															; Close the app
	if $isAppNotRes=1 Then
		_WriteResult("Warning","TS_NS_139")
    EndIf
EndIf


