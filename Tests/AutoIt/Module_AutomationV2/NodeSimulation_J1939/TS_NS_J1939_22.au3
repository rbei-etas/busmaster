;==================================================================Test Cases/Test Data ===========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_22
; TestCases			:		Executing DLL handler
; Test Data			:		cfxNSJ1939_22.cfx
; Test Setup		:		Edit the DLL handler I.e. OnDLL_UnLoad() and save it and select menu item User Program-->Load Functions DLL

; Expected Results  : 		The DLL handlers will be executed displaying the output in notification window.
;
;=================================================================Test Procedure ====================================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_12.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxNSJ1939_22.cfx")
	sleep(1000)
	_ActivateJ1939()                                                                  ;Activate J1939 Message window
	sleep(1000)
	_ExecuteLoadDLLHandler("ForJ1939")                                                 ;Call function to execute DLL handler
	$isAppNotRes=_CloseApp()															; Close the app
    if $isAppNotRes=1 Then
		_WriteResult("Warning","TS_J1939Node_22")
    EndIf
EndIf


    ConsoleWrite("****End : TS_J1939Node_22.au3****"&@CRLF)
    ConsoleWrite(@CRLF)




