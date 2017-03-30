;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_49
; TestCases			:		Archieving Logging configuration.
; Test Data			:
; Test Setup		:		1. Select a new configuration file.
;							2. Select 'LIN->Logging->Configure' to open "log configuration dialog'.
;							3. Add a new log file by clicking 'Add' button in 'Log blocks' group box.
;							4. Press 'OK' in 'Configure logging for LIN' dialog.
;							5. Reopen the 'Configure logging for LIN' dialog.
;							6. Click 'Ok' to close the dialog.
;							7. Save the configuration.
;							8. Close Busmaster application and reopen the application.
;							9. Make sure the previous configuration is loaded.
;							10. Open the 'Configure logging for LIN' dialog.

; Expected Results  : 		1. After step 10, the previously added 'log file' should be present in  the 'Configure logging for LIN' dialog.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_49.au3****" & @CRLF)

$logfilePathAdded1=0
$logfilePathAdded2=0
$ConfigRetainedAfterClickingOK=0
$ConfigRetainedAfterSaving=0


_launchApp() 																										  ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe") 																						  ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)

If WinExists($WIN_BUSMASTER) Then
	_createConfig("TS_LIN_Log_49")
	Sleep(1000)
	;_ConfigureLINLogWithLogpath("SampleLog_49") 																	   ;Configuring Can log with only file name
	_ConfigureLINLog("SampleLog_49","Relative","ALL","Decimal","Overwrite","","","False")
    _LINLogMenu()
	If WinExists($WIN_LINLog) Then
		;ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                ;Click on OK button
		;Sleep(1000)
		;_LINLogMenu()
		$canLogHWD=ControlGetHandle($WIN_LINLog,"",$LSTC_LogFile_ConfigLOG)
		$logfilesCount1=_GUICtrlListView_GetItemCount($canLogHWD)                                                      ;Getting the number of logfiles created
		ConsoleWrite("$logfilesCount1 : " & $logfilesCount1 & @CRLF)
		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									   ;Get the handler for the list view to show the logfiles added
		$logfilePath1 = _GUICtrlListView_GetItemTextString($logfilehwd, 0)										       ;Get the first item in the list view
		ConsoleWrite("$logfilePath1 : " & $logfilePath1 & @CRLF)
		If $logfilePath1 = _OutputDataPath() & "\SampleLog_49.log" Then                                                ;Validating the log file path
			$logfilePathAdded1 = 1
		EndIf
		ConsoleWrite("$logfilePathAdded1 : " & $logfilePathAdded1 & @CRLF)
		$NumModeDecimalIsChecked1=ControlCommand($WIN_LINLog,"",$RBTN_NumModeDec_ConfigLOG,"IsChecked")                ;Checking whether the decimal mode is selected
		$FileModeOvrWrteIsChecked1=ControlCommand($WIN_LINLog,"",$RBTN_FileModeOverWrite_ConfigLOG,"IsChecked")        ;Checking whether the overwrite mode is selected
		$enableDisableChecked1=ControlCommand($WIN_LINLog,"",$BTN_EnDisable_ConfigLOG,"IsChecked")                     ;Checking whether the enable/disable logging selected
		$SelectedTimemode1=ControlGetText($WIN_LINLog,"",$COMB_TimeMode_ConfigLOG)                                     ;Get the selected timemode
		ConsoleWrite("$NumModeDecimalIsChecked1 : " & $NumModeDecimalIsChecked1 & @CRLF)
		ConsoleWrite("$FileModeOvrWrteIsChecked1 : " & $FileModeOvrWrteIsChecked1 & @CRLF)
		ConsoleWrite("$enableDisableChecked1 : " & $enableDisableChecked1 & @CRLF)
		ConsoleWrite("$SelectedTimemode1 : " & $SelectedTimemode1 & @CRLF)
		$SelectedChannel1=ControlGetText($WIN_LINLog,"",$COMB_Channel_ConfigLOG)                                       ;Get the selected channel details
		ConsoleWrite("$SelectedChannel1 : " & $SelectedChannel1 & @CRLF)
		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                 ;Click on OK button
		if $NumModeDecimalIsChecked1=1 and $FileModeOvrWrteIsChecked1=1 and $SelectedTimemode1="Relative" and $SelectedChannel1="ALL" and $logfilesCount1=1 and $enableDisableChecked1=1 Then
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
		If $logfilePath2 = _OutputDataPath() & "\SampleLog_49.log" Then                                                ;Validating the log file path
			$logfilePathAdded2 = 1
		EndIf
		ConsoleWrite("$logfilePathAdded2 : " & $logfilePathAdded2 & @CRLF)
		$NumModeDecimalIsChecked2=ControlCommand($WIN_LINLog,"",$RBTN_NumModeDec_ConfigLOG,"IsChecked")                ;Checking whether the decimal mode is selected
		$FileModeOvrWrteIsChecked2=ControlCommand($WIN_LINLog,"",$RBTN_FileModeOverWrite_ConfigLOG,"IsChecked")        ;Checking whether the overwrite mode is selected
		$enableDisableChecked2=ControlCommand($WIN_LINLog,"",$BTN_EnDisable_ConfigLOG,"IsChecked")                     ;Checking whether the enable/disable logging selected
		$SelectedTimemode2=ControlGetText($WIN_LINLog,"",$COMB_TimeMode_ConfigLOG)                                     ;Get the selected timemode
		ConsoleWrite("$NumModeDecimalIsChecked2 : " & $NumModeDecimalIsChecked2 & @CRLF)
		ConsoleWrite("$FileModeOvrWrteIsChecked2 : " & $FileModeOvrWrteIsChecked2 & @CRLF)
		ConsoleWrite("$enableDisableChecked2 : " & $enableDisableChecked2 & @CRLF)
		ConsoleWrite("$SelectedTimemode2 : " & $SelectedTimemode2 & @CRLF)
		$SelectedChannel2=ControlGetText($WIN_LINLog,"",$COMB_Channel_ConfigLOG)
		ConsoleWrite("$SelectedChannel2 : " & $SelectedChannel2 & @CRLF)
		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                ;Click on OK button
		if $NumModeDecimalIsChecked2=1 and $FileModeOvrWrteIsChecked2=1 and $SelectedTimemode2="Relative" and $SelectedChannel2="ALL" and $logfilesCount2=1 and $enableDisableChecked2=1 Then
			$ConfigRetainedAfterSaving=1
		EndIf
	EndIf
EndIf


ConsoleWrite("$ConfigRetainedAfterSaving : " & $ConfigRetainedAfterSaving & @CRLF)
ConsoleWrite("$ConfigRetainedAfterClickingOK : " & $ConfigRetainedAfterClickingOK & @CRLF)
if $logfilePathAdded1=1 and $logfilePathAdded2=1 and $ConfigRetainedAfterSaving=1 And $ConfigRetainedAfterClickingOK=1 then
	_WriteResult("Pass","TS_LIN_Log_49")
	ConsoleWrite("Pass" & @CRLF)
Else
	_WriteResult("Fail","TS_LIN_Log_49")
	ConsoleWrite("Fail" & @CRLF)
EndIf

$isAppNotRes= _CloseApp()

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_49")
EndIf


ConsoleWrite("****End : TS_LIN_Log_49.au3****"&@CRLF)
ConsoleWrite(@CRLF)