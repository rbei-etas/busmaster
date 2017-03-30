;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_40
; TestCases			:		Configuring Log File
; Test Data			:
; Test Setup		:		1. Select a new configuration file.
;							2. Select LIN->Logging->Configure
;							3. Check for log file configuration dialog.
;							3. Select Add button from left side.
;							4. Check log file list and log file details.

; Expected Results  : 		1. After step 3, Log file  configuration dialog will be displayed with empty log file list. Other than Add button all controls will be disabled.

;							After step 4, a new log file will be added in to the list. The new log file will have default values for time, numeric and file mode.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********Advanced logging for CAN Script Execution Started************" & @CRLF)
ConsoleWrite("****Start : TS_LIN_Log_40.au3****" & @CRLF)

$logFileName="TS_LIN_Log_40_OverwritingFile.log"
$defaultValuesArePresent=0
$overWritingWarningShown=0
$logFilePathIsproper=0

_launchApp() 																										 ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe") 																						 ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)

If WinExists($WIN_BUSMASTER) Then
	_createConfig("TS_LIN_Log_40")																				     ;Loading the already existing config
	Sleep(1000)
	;_ConfigureLINLogWithLogpath("SampleLog") 																		 ;Configuring Can log with only file name
	 _LINLogMenu()                                                           				   						 ;Select Configure->Log
	Sleep(1000)

	If WinExists($WIN_LINLog) Then

		ControlClick($WIN_LINLog,"",$BTN_Add_ConfigLOG)																 ;Click on Add button
		sleep(250)
		$LogFilePath=_TestDataPath()
		ControlClick($WIN_LINLog,"",$BTN_logFPath_ConfigLOG)										                 ;Click on Log File Path button
		sleep(1000)
		if winexists($WIN_Select_LogFile) Then
			controlsend($WIN_Select_LogFile,"",$TXT_FileName_SelLogFile,$LogFilePath & "\" & $logFileName)	         ;Enter the Log file Name
			controlclick($WIN_Select_LogFile,"",$BTN_SaveInst_SelLogFile)									         ;Click on 'Save' button
			Sleep(1000)
			If WinExists($WIN_ConfirmSaveAs) Then																     ;Checking whether confirm save as popup is shown
				$overWritingWarningShown=1
				controlclick($WIN_ConfirmSaveAs,"",$BTN_YesConfirmSaveAsConfig)								         ;Click on 'Yes' button
			EndIf
		EndIf
		ConsoleWrite("$overWritingWarningShown : " & $overWritingWarningShown & @CRLF)


		$NumModeHexIsChecked=ControlCommand($WIN_LINLog,"",$RBTN_NumModeHex_ConfigLOG,"IsChecked")                   ;Checking whether the hex mode is selected by default
		$FileModeAppIsChecked=ControlCommand($WIN_LINLog,"",$RBTN_FileModeApp_ConfigLOG,"IsChecked")                 ;Checking whether the append mode is selected by default
		$canLogHWD=ControlGetHandle($WIN_LINLog,"",$LSTC_LogFile_ConfigLOG)
		$logfilesCount=_GUICtrlListView_GetItemCount($canLogHWD)                                                     ;Getting the number of logfiles created
		ConsoleWrite("$NumModeHexIsChecked : " & $NumModeHexIsChecked & @CRLF)
		ConsoleWrite("$FileModeAppIsChecked : " & $FileModeAppIsChecked & @CRLF)
		ConsoleWrite("$logfilesCount : " & $logfilesCount & @CRLF)
		if($NumModeHexIsChecked=1 and $FileModeAppIsChecked=1 And $logfilesCount=1) Then
			$defaultValuesArePresent=1
		EndIf
		ConsoleWrite("$defaultValuesArePresent : " & $defaultValuesArePresent & @CRLF)


		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									 ;Get the handler for the list view to show the logfiles added
		$logfilePathObtained = _GUICtrlListView_GetItemTextString($logfilehwd, 0)								     ;Get the first item in the list view
		ConsoleWrite("$logfilePathObtained : " & $logfilePathObtained & @CRLF)
		ConsoleWrite("$logfilePath expected : " & $LogFilePath & "\" & $logFileName & @CRLF)
		If $logfilePathObtained = $LogFilePath & "\" & $logFileName Then                                             ;Validating the log file path
			$logFilePathIsproper = 1
		EndIf
		ConsoleWrite("$logFilePathIsproper : " & $logFilePathIsproper & @CRLF)


		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)
	EndIf
EndIf


if $overWritingWarningShown=1 and $defaultValuesArePresent=1 and $logFilePathIsproper=1  then
	_WriteResult("Pass","TS_LIN_Log_40")
	ConsoleWrite("Pass" & @CRLF)
Else                                                                                                        ; Print the pass or fail
	_WriteResult("Fail","TS_LIN_Log_40")
	ConsoleWrite("Fail" & @CRLF)
EndIf

$isAppNotRes=_CloseApp()															                        ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_40")
EndIf


ConsoleWrite("****End : TS_LIN_Log_40.au3****"&@CRLF)
ConsoleWrite(@CRLF)



