;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_53
; TestCases			:		Archieving Logging configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;							2. Check the options 'Start on Reception of ID 0x' and 'Stop on Reception of ID 0x' and assign values 0x1 and 0xA respectively.
;							3. Click 'Ok' to close the dialog.
;							4. Reopen the 'Configure logging for LIN' dialog.
;							5.Click ok and close
;							6.Save the configuration.
;							7. Close Busmaster application and reopen the application.
;							8. Make sure the previous configuration is loaded.
;							9. Open the 'Configure logging for LIN' dialog.

; Expected Results  : 		1. After step4 and step9, the check boxes  'Start on Reception of ID 0x' and 'Stop on Reception of ID 0x' should be in checked state and 0x1 and 0xA should be the respective values.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_53.au3****" & @CRLF)

$logfilePathAdded1=0
$logfilePathAdded2=0
$ConfigRetainedAfterClickingOK=0
$ConfigRetainedAfterSaving=0


_launchApp() 																										  ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe") 																						  ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)

If WinExists($WIN_BUSMASTER) Then
	_createConfig("TS_LIN_Log_53")
	Sleep(1000)
	_ConfigureLINLogWithLogpath("SampleLog_53") 																	   ;Configuring Can log with only file name
    ;_LINLogMenu()
	If WinExists($WIN_LINLog) Then
		;ControlClick($WIN_LINLog,"",$BTN_Add_ConfigLOG)												                   ;Click on Add button
		Controlcommand ($WIN_LINLog,"",$CHKB_StartTrig_ConfigLOG,"Check")						                       ;Enable the start trigger
		ControlCommand ( $WIN_LINLog, "", $CHKB_StopTrig_ConfigLOG, "Check" )									       ;Click on start on reception ID check boxes
		$hWndStartID=controlgethandle($WIN_LINLog,"",$TXT_StartMsgID_ConfigLOG)		                                   ;Get handle of start on reception ID edit box
		_GUICtrlEdit_SetText($hWndStartID, "1")																	       ;Enter the value
		$hWndStopID=controlgethandle($WIN_LINLog,"",$TXT_StopMsgID_ConfigLOG)		                                   ;Get handle of stop on reception ID edit box
		_GUICtrlEdit_SetText($hWndStopID, "A")																	       ;Enter the value
		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                 ;Click on OK button
		Sleep(1000)
		_LINLogMenu()
		$startOnRecIsChecked1=ControlCommand($WIN_LINLog,"",$CHKB_StartTrig_ConfigLOG,"IsChecked")                ;Checking whether the decimal mode is selected
		$stopOnRecIsChecked1=ControlCommand($WIN_LINLog,"",$CHKB_StopTrig_ConfigLOG,"IsChecked")                  ;Checking whether the overwrite mode is selected
		$hWndStartID=controlgethandle($WIN_LINLog,"",$TXT_StartMsgID_ConfigLOG)		                                   ;Get handle of start on reception ID edit box															       ;Enter the value
		$startIdObtained1=_GUICtrlEdit_GetText($hWndStartID)                                                           ;Getting the value inside the edit box to check whether it allows to enter ID manually
        consolewrite("$startIdObtained1 : "&$startIdObtained1&@CRLF)
		$hWndStopID=controlgethandle($WIN_LINLog,"",$TXT_StopMsgID_ConfigLOG)		                                   ;Get handle of stop on reception ID edit box																       ;Enter the value
		$stopIdObtained1=_GUICtrlEdit_GetText($hWndStopID)                                                             ;Getting the value inside the edit box to check whether it allows to enter ID manually
        consolewrite("$stopIdObtained1 : "&$stopIdObtained1&@CRLF)
		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                 ;Click on OK button
		if $startOnRecIsChecked1=1 and $stopOnRecIsChecked1=1 and $startIdObtained1="1" and $stopIdObtained1="A" Then
			$ConfigRetainedAfterClickingOK=1
		EndIf
		_saveConfig()
        _CloseApp()
	EndIf
EndIf

_launchApp() 																										   ;Check if the App is open else launch it
ProcessWait("BUSMASTER.exe") 																						   ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)
If WinExists($WIN_BUSMASTER) Then
	_LINLogMenu()
	Sleep(1000)
	If WinExists($WIN_LINLog) Then
		$canLogHWD=ControlGetHandle($WIN_LINLog,"",$LSTC_LogFile_ConfigLOG)
		$logfilesCount2=_GUICtrlListView_GetItemCount($canLogHWD)                                                      ;Getting the number of logfiles created
		ConsoleWrite("$logfilesCount2 : " & $logfilesCount2 & @CRLF)
		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									   ;Get the handler for the list view to show the logfiles added
		$logfilePath2 = _GUICtrlListView_GetItemTextString($logfilehwd, 0)										       ;Get the first item in the list view
		ConsoleWrite("$logfilePath2 : " & $logfilePath2 & @CRLF)
		If $logfilePath2 = _OutputDataPath() & "\SampleLog_53.log" Then                                                ;Validating the log file path
			$logfilePathAdded2 = 1
		EndIf
		ConsoleWrite("$logfilePathAdded2 : " & $logfilePathAdded2 & @CRLF)
		$startOnRecIsChecked2=ControlCommand($WIN_LINLog,"",$CHKB_StartTrig_ConfigLOG,"IsChecked")                ;Checking whether the start id is checked
		$stopOnRecIsChecked2=ControlCommand($WIN_LINLog,"",$CHKB_StopTrig_ConfigLOG,"IsChecked")                      ;Checking whether the stop id id checked
		ConsoleWrite("$startOnRecIsChecked2 : " & $startOnRecIsChecked2 & @CRLF)
		ConsoleWrite("$stopOnRecIsChecked2 : " & $stopOnRecIsChecked2 & @CRLF)
		$hWndStartID=controlgethandle($WIN_LINLog,"",$TXT_StartMsgID_ConfigLOG)		                                   ;Get handle of start on reception ID edit box
		$startIdObtained2=_GUICtrlEdit_GetText($hWndStartID)                                                           ;Getting the value inside the edit box to check whether it allows to enter ID manually
        consolewrite("$startIdObtained2 : "&$startIdObtained2&@CRLF)
		$hWndStopID=controlgethandle($WIN_LINLog,"",$TXT_StopMsgID_ConfigLOG)		                                   ;Get handle of stop on reception ID edit box
		$stopIdObtained2=_GUICtrlEdit_GetText($hWndStopID)                                                             ;Getting the value inside the edit box to check whether it allows to enter ID manually
        consolewrite("$stopIdObtained2 : "&$stopIdObtained2&@CRLF)
		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                ;Click on OK button

		if $logfilesCount2=1 and $logfilePathAdded2=1 and $startOnRecIsChecked2=1 and $stopOnRecIsChecked2=1 and $startIdObtained2="1" and $stopIdObtained2="A" Then
			$ConfigRetainedAfterSaving=1
		EndIf
	EndIf
EndIf


ConsoleWrite("$ConfigRetainedAfterSaving : " & $ConfigRetainedAfterSaving & @CRLF)
ConsoleWrite("$ConfigRetainedAfterClickingOK : " & $ConfigRetainedAfterClickingOK & @CRLF)
if  $ConfigRetainedAfterSaving=1 And $ConfigRetainedAfterClickingOK=1 then
	_WriteResult("Pass","TS_LIN_Log_53")
	ConsoleWrite("Pass" & @CRLF)
Else
	_WriteResult("Fail","TS_LIN_Log_53")
	ConsoleWrite("Fail" & @CRLF)
EndIf

$isAppNotRes= _CloseApp()

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_53")
EndIf


ConsoleWrite("****End : TS_LIN_Log_53.au3****"&@CRLF)
ConsoleWrite(@CRLF)