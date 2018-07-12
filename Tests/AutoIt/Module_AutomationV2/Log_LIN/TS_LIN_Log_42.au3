;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_42
; TestCases			:		Configuring Log File
; Test Data			:
; Test Setup		:		1. Select "Start on Reception of ID" option.
;							2. Check the start trigger combo box for database frame list.
;							3. Select "Stop on Reception of ID" option.
;							4. Check the stop trigger combo box for database frame list.

; Expected Results  : 		1. Start and stop trigger combo boxes will be filled with the list of database frames. At the same time it will be editable so that user LIN enter ID directly.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_42.au3****" & @CRLF)

$startOnRecEnabled=1
$startOnRecEnabledAfterChecking=0
$stopOnRecEnabled=1
$stopOnRecEnabledAfterChecking=0
$ableToEnterIDManually=0

_launchApp() 																										 ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe") 																						 ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)

If WinExists($WIN_BUSMASTER) Then
	_createConfig("TS_LIN_Log_41")																						     ;Loading the already existing config
	Sleep(1000)
	_ConfigureLINLogWithLogpath("SampleLog") 																		 ;Configuring Can log with only file name

	If WinExists($WIN_LINLog) Then
		Sleep(1000)
		$startOnRecEnabled=ControlCommand($WIN_LINLog,"",$TXT_StartMsgID_ConfigLOG,"IsEnabled")                    ;Checking whether the start on reception ID edit box is enabled
		Controlcommand ($WIN_LINLog,"",$CHKB_StartTrig_ConfigLOG,"Check")						                   ;Enable the start trigger
		Sleep(1000)
		$startOnRecEnabledAfterChecking=ControlCommand($WIN_LINLog,"",$TXT_StartMsgID_ConfigLOG,"IsEnabled")       ;Checking whether the start on reception ID edit box is enabled
		$stopOnRecEnabled=ControlCommand($WIN_LINLog,"",$TXT_StopMsgID_ConfigLOG,"IsEnabled")                      ;Checking whether the start on reception ID edit box is enabled
		ControlCommand ( $WIN_LINLog, "", $CHKB_StopTrig_ConfigLOG, "Check" )									   ;Click on start on reception ID check boxes
		Sleep(1000)
		$stopOnRecEnabledAfterChecking=ControlCommand($WIN_LINLog,"",$TXT_StopMsgID_ConfigLOG,"IsEnabled")         ;Checking whether the start on reception ID edit box is enabled
		consolewrite("$startOnRecEnabled : "&$startOnRecEnabled&@CRLF)
		consolewrite("$startOnRecEnabledAfterChecking : "&$startOnRecEnabledAfterChecking&@CRLF)
		consolewrite("$stopOnRecEnabled : "&$stopOnRecEnabled&@CRLF)
		consolewrite("$stopOnRecEnabledAfterChecking : "&$stopOnRecEnabledAfterChecking&@CRLF)

		$hWndStartID=controlgethandle($WIN_LINLog,"",$TXT_StartMsgID_ConfigLOG)		                                ;Get handle of start on reception ID edit box
		_GUICtrlEdit_SetText($hWndStartID, "1A")																	;Enter the value
		$startIdObtained=_GUICtrlEdit_GetText($hWndStartID)                                                         ;Getting the value inside the edit box to check whether it allows to enter ID manually
        consolewrite("$startIdObtained : "&$startIdObtained&@CRLF)

		$hWndStopID=controlgethandle($WIN_LINLog,"",$TXT_StopMsgID_ConfigLOG)		                                ;Get handle of stop on reception ID edit box
		_GUICtrlEdit_SetText($hWndStopID, "2E")																	    ;Enter the value
		$stopIdObtained=_GUICtrlEdit_GetText($hWndStopID)                                                           ;Getting the value inside the edit box to check whether it allows to enter ID manually
        consolewrite("$stopIdObtained : "&$stopIdObtained&@CRLF)
        ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)
		if $startIdObtained="1A" and $stopIdObtained="2E" Then
			$ableToEnterIDManually=1
	    EndIf
	EndIf
EndIf

consolewrite("$ableToEnterIDManually : "&$ableToEnterIDManually&@CRLF)
if $ableToEnterIDManually=1 and $startOnRecEnabled=0 and $startOnRecEnabledAfterChecking=1 and $stopOnRecEnabled=0 and $stopOnRecEnabledAfterChecking=1 then
	_WriteResult("Pass","TS_LIN_Log_42")
	ConsoleWrite("Pass" & @CRLF)
Else                                                                                                              ; Print the pass or fail
	_WriteResult("Fail","TS_LIN_Log_42")
	ConsoleWrite("Fail" & @CRLF)
EndIf

$isAppNotRes=_CloseApp()															                              ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_42")
EndIf


ConsoleWrite("****End : TS_LIN_Log_42.au3****"&@CRLF)
ConsoleWrite(@CRLF)