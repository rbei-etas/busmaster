;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939Log_01
; TestCases			:		Functionality test - Stopping logging
; Test Data			:
; Test Setup		:		1. Make sure log configuration has at least one file.
;~ 							2. Connect the tool and enable logging.
;~ 							3. Start message transmission.
;~ 							4. Close the application.
;~ 							5. Open the log file and check for the session end.

; Expected Results  : 		1. After step 5, log file will have the session end block to denote the end of session.

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939Log_09
; TestCases			:		Configuring Log File
; Test Data			:
; Test Setup		:		1. Select Add button from left side.
;~ 							2. Check log file list and log file details.

; Expected Results  : 		1. After step 2, a new log file will be added in to the list. The new log file will have default values for time, numeric and file mode. Log triggering will be in off state. Filters will be empty.

; Critical (C)		:		Y
; TestCase No.		:		TS_J1939Log_10
; TestCases			:		Configuring Log File
; Test Data			:
; Test Setup		:		1. Select "…" button from log file details.
;~ 							2. Check for file selection dialog.
;~ 							3. Select a log file and select OK.
;~ 							4. Check the File Path.

; Expected Results  : 		1. After step 2, file save dialog will be displayed. While selecting an existing file an overwrite warning will be given to the user.
;~ 							2. After step 4, the File Path will reflect the selected file path.

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939Log_01.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)
$EndBlock=""
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxJ1939Log_01")																		; Load Configuration

	_ActivateJ1939()																					; Activate J1939

	_ConfigureJ1939Log("J1939Logtest_01","System","ALL","Hex","Overwrite","","","False")				; Configure Logging details

	_J1939LogMenu()																						; Open Configure J1939 Logging

	; Fetch the details of configured first log file

	$LogFilehWd=controlgethandle($WIN_J1939Log,"",$LSTC_LogFile_ConfigLOG)									; Fetch handle of log file list view
	_GUICtrlListView_ClickItem($LogFilehWd,0)
	sleep(500)
	$firstLogFilePath=_GUICtrlListView_GetItemText($LogFilehWd,0)											; Fetch the path of the first log file
	$firstFileTimeModeTxt=ControlGetText($WIN_J1939Log, "", $COMB_TimeMode_ConfigLOG )						; Fetch the time mode
	$firstFileNumModeHex=Controlcommand($WIN_J1939Log,"",$RBTN_NumModeHex_ConfigLOG,"IsChecked")				; Check whether Hex Numeric mode is set
	$firstFileModeOverWrite=Controlcommand($WIN_J1939Log,"",$RBTN_FileModeOverWrite_ConfigLOG,"IsChecked")	; Check whether overwrite file mode is set
	ConsoleWrite("$firstLogFilePath : "&$firstLogFilePath&@CRLF)
	ConsoleWrite("$firstFileTimeModeTxt : "&$firstFileTimeModeTxt&@CRLF)
	ConsoleWrite("$firstFileNumModeHex : "&$firstFileNumModeHex&@CRLF)
	ConsoleWrite("$firstFileModeOverWrite : "&$firstFileModeOverWrite&@CRLF)
	$DirPath=_OutputDataPath()

	controlclick($WIN_J1939Log,"",$BTN_OK_ConfigLOG)														; Click on 'OK' button

	if  $firstLogFilePath=$DirPath&"\J1939Logtest_01.log" and $firstFileTimeModeTxt="System" and $firstFileNumModeHex=1 and $firstFileModeOverWrite=1 Then
		$FirstLogFileDetails="Match"
	Else
		$FirstLogFileDetails="No Match"
	EndIf

	_ConnectDisconnect()																				; Connect
	sleep(1500)

	_J1939tMsgWin("Request Address","","","")															; Transmit "Request Address" msg

	_ConnectDisconnect()																				; Disconnect
	sleep(1500)

	_DeactJ1939()																						; Deactivate J1939

	$isAppNotRes=_CloseApp()																			; Close the app

	$GetLogFile_Path=_OutputDataPath()																	; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	$EndBlock=FileReadLine ($GetLogFile_Path & "\J1939Logtest_01.log",17)								; Read the 17th line from the Log file
	ConsoleWrite("$EndBlock:"&$EndBlock&@CRLF)
	ConsoleWrite("$FirstLogFileDetails:"&$FirstLogFileDetails&@CRLF)

EndIf

if $EndBlock="***[STOP LOGGING SESSION]***" and $FirstLogFileDetails="Match" Then
	_WriteResult("Pass","TS_J1939Log_01")
Else
	_WriteResult("Fail","TS_J1939Log_01")
EndIf

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Log_01")
EndIf



ConsoleWrite("****End : TS_J1939Log_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
