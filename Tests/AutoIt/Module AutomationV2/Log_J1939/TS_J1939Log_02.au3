;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939Log_02
; TestCases			:		Functionality test - Stopping logging
; Test Data			:
; Test Setup		:		1.Repreat the previous test case but instead of closing the application change the configuration file.

; Expected Results  : 		1. As mentioned in the previous test case.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939Log_02.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)
$EndBlock=""
if winexists($WIN_BUSMASTER) then

;~ 	_loadConfig("cfxJ1939Log_01")																		; Load Configuration

	_ActivateJ1939()																					; Activate J1939

	_ConfigureJ1939Log("J1939Logtest_02","System","ALL","Hex","Overwrite","","","False")				; Configure Logging details

	_ConnectDisconnect()																				; Connect
	sleep(1500)

	_J1939tMsgWin("Request Address","","","")																	; Transmit "Request Address" msg

	_ConnectDisconnect()																				; Disconnect
	sleep(1500)

	_DeactJ1939()																						; Deactivate J1939

	_loadConfig("cfxJ1939Log_02")																		; Load Configuration

	$GetLogFile_Path=_OutputDataPath()																	; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	$EndBlock=FileReadLine ($GetLogFile_Path & "\J1939Logtest_02.log",16)								; Read the 6th line from the Log file
	ConsoleWrite("$EndBlock:"&$EndBlock&@CRLF)

EndIf


if $EndBlock="***[STOP LOGGING SESSION]***" Then
	_WriteResult("Pass","TS_J1939Log_02")
Else
	_WriteResult("Fail","TS_J1939Log_02")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Log_02")
EndIf



ConsoleWrite("****End : TS_J1939Log_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
