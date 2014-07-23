;==================================================================Test Cases/Test Data ===========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_24
; TestCases			:		Executing UnLoad DLL Handler
; Test Data			:		cfxNSJ1939_24.cfx
; Test Setup		:		Edit the DLL handler I.e. OnDLL_UnLoad() and save it and select menu item User Program-->Unload

; Expected Results  : 		The DLL Unload handlers will be executed displaying the output in notification window
;=================================================================Test Procedure ====================================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_12.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxNSJ1939_24.cfx")
	 sleep(1000)
    _ActivateJ1939()                                                                  ;Activate J1939 Message window
	sleep(1000)
	_ExecuteUnloadDLLHandler("ForJ1939")
	$isAppNotRes=_CloseApp()															; Close the app
    if $isAppNotRes=1 Then
		_WriteResult("Warning","TS_J1939Node_24")
    EndIf
EndIf

    ConsoleWrite("****End : TS_J1939Node_24.au3****"&@CRLF)
    ConsoleWrite(@CRLF)





