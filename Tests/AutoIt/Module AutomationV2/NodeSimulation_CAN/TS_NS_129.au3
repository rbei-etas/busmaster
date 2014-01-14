;==================================================================Test Cases/Test Data ===========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_129
; TestCases			:		Executing Load DLL Handler
; Test Data			:		cfxNS129.cfx
; Test Setup		:		Edit the DLL handler I.e. OnDLL_UnLoad() and save it and select menu item User Program-->Load Functions DLL

; Expected Results  : 		The DLL handlers will be executed displaying the output in notification window
;
;=================================================================Test Procedure ====================================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_129.au3****"&@CRLF)
_launchApp()
WinActivate($WIN_BUSMASTER)
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxNSCAN_129.cfx")
	sleep(1000)
	_ExecuteLoadDLLHandler("ForCAN")
	$isAppNotRes=_CloseApp()															; Close the app
	if $isAppNotRes=1 Then
		_WriteResult("Warning","TS_NS_129")
    EndIf
EndIf



