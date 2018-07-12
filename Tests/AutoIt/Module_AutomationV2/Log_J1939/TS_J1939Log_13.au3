;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939Log_13
; TestCases			:		Functionality test - Enable log
; Test Data			:
; Test Setup		:		1.Select Log from Configuration menu.
;~ 							2. disable all log files by unchecking all check boxes associated with the log file.
;~ 							3. Select OK to save changes.
;~ 							4. Connect the tool and start message transmission. Start logging.
;~ 							5. Disconnect the tool and stop logging.
;~ 							6. Check the log files status.

; Expected Results  : 		1. As log files are not enabled, log files will remain unchanged.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939Log_13.au3****"&@CRLF)


_launchApp()																							; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxJ1939Log_13.cfx")																; Load Configuration

	_ActivateJ1939()																				; Activate J1939

	_J1939LogMenu()																					; Open Configure J1939 Logging

	$hWd=controlgethandle($WIN_J1939Log,"",$LSTC_LogFile_ConfigLOG)								; Get handle of log file instance
	_GUICtrlListView_SetItemChecked($hWd, 0,False)												; Uncheck the first log file

	controlclick($WIN_J1939Log,"",$BTN_OK_ConfigLOG)											; Click on 'OK' button
	sleep(500)

	_ConnectDisconnect()																				; Connect

	_J1939tMsgWin("Data","[0x15]Msg1","","")															; Transmit "data" message

	_ConnectDisconnect()																				; Disconnect

	_DeactJ1939()																						; Deactivate J1939

	$GetLogFile_Path=_TestDataPath()															; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	$Count=_FileCountLines($GetLogFile_Path & "\LogtestJ1939_13.log")							; Fetch the no. of lines in the log file

	ConsoleWrite("$Count:"&$Count&@CRLF)

EndIf

if $Count=0 Then																				; Validate the result
	_WriteResult("Pass","TS_J1939Log_13")
Else
	_WriteResult("Fail","TS_J1939Log_13")
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Log_13")
EndIf




ConsoleWrite("****End : TS_J1939Log_13.au3****"&@CRLF)
ConsoleWrite(@CRLF)
