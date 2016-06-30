;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_32
; TestCases			:		Functionality test - Stopping logging
; Test Data			:
; Test Setup		:		1. Make sure log configuration has at least one file.
;~ 							2. Connect the tool and enable logging.
;~ 							3. Start message transmission.
;~ 							4. Close the application.
;~ 							5. Open the log file and check for the session end.

;~ 							Repeat the previous test case steps till 3.  But instead of closing the application change the configuration file.

; Expected Results  : 		1. After step 5, log file will have the session end block to denote the end of session.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_32.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)
Local $EndBlockAftExit="",$EndBlockAftLoad=""

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLog_32.cfx")																; Load Configuration

	_ConnectDisconnect()																		; Connect
	sleep(1000)
	;_TransmitMsgsMenu()																			; Transmit CAN messages
	Send("b")
	sleep(1000)

	_ConnectDisconnect()																		; Disconnect

	$isAppNotRes=_CloseApp()																	; Close the app

	$GetLogFile_Path=_TestDataPath()															; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	$Count=_FileCountLines($GetLogFile_Path & "\Logtest32.log")									; Fetch the no. of lines in the log file

	if $Count>=20 Then
		$EndBlockAftExit=FileReadLine ($GetLogFile_Path & "\Logtest32.log",20)					; Read the 20th line from the Log file
	EndIf

	_launchApp()																				; Launch the app

	_ConnectDisconnect()																		; Connect

	;_TransmitMsgsMenu()																			; Transmit CAN messages
	sleep(1000)
	Send("b")
	sleep(1000)
	_ConnectDisconnect()																		; Disconnect

	_loadConfig("Sample.cfx")																	; Load Configuration

	$Count1=_FileCountLines($GetLogFile_Path & "\Logtest32.log")								; Fetch the no. of lines in the log file

	if $Count1>=20 Then
		$EndBlockAftLoad=FileReadLine ($GetLogFile_Path & "\Logtest32.log",20)					; Read the 20th line from the Log file
	EndIf

	ConsoleWrite("$EndBlockAftExit:"&$EndBlockAftExit&@CRLF)
	ConsoleWrite("$EndBlockAftLoad:"&$EndBlockAftLoad&@CRLF)

EndIf

if $EndBlockAftExit="***[STOP LOGGING SESSION]***" and $EndBlockAftLoad="***[STOP LOGGING SESSION]***" Then
	_WriteResult("Pass","TS_Log_32")
Else
	_WriteResult("Fail","TS_Log_32")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_32")
EndIf




ConsoleWrite("****End : TS_Log_32.au3****"&@CRLF)
ConsoleWrite(@CRLF)
