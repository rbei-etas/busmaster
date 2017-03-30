;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_44
; TestCases			:		Functionality test - Stopping logging
; Test Data			:
; Test Setup		:		1. Make sure log configuration has at least one file.
;							2. Connect the tool and enable logging.
;							3. Start frame transmission.
;							4. Close the application.
;							5. Open the log file and check for the session end.

; Expected Results  : 		1. After step 5, log file will have the session end block to denote the end of session.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_44.au3****" & @CRLF)

$startAndEndCommentProper=0

_launchApp() 																										 ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe") 																						 ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)

If WinExists($WIN_BUSMASTER) Then
	_loadConfig("cfxLogging_49_LIN")																		         ;Loading the already existing config
	Sleep(1000)
	_linDriverSelection()																							 ;DriverSelection
	Sleep(1000)
	_ConfigureLINLogWithLogpath("SampleLog_43") 																	 ;Configuring Can log with only file name

	If WinExists($WIN_LINLog) Then
		ControlCommand($WIN_LINLog, "", $BTN_EnDisable_ConfigLOG, "Check")									         ; Check Enable/Disable Logging during connect/disconnect
		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                               ;Click on OK button
	EndIf

	_ConnectDisconnect_LIN() 																						 ;Connect

    $isAppNotRes=_CloseApp()															                             ;Close the app
	Sleep(2000)
    $startAndEndCommentProper = _CheckStartAndEndOfLogfile("SampleLog_43.log")                                       ;Check the start and end of comment entry
EndIf

consolewrite("$startAndEndCommentProper : "&$startAndEndCommentProper&@CRLF)
if $startAndEndCommentProper=1 then
	_WriteResult("Pass","TS_LIN_Log_44")
	ConsoleWrite("Pass" & @CRLF)
Else
	_WriteResult("Fail","TS_LIN_Log_44")
	ConsoleWrite("Fail" & @CRLF)
EndIf


if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_44")
EndIf


ConsoleWrite("****End : TS_LIN_Log_44.au3****"&@CRLF)
ConsoleWrite(@CRLF)