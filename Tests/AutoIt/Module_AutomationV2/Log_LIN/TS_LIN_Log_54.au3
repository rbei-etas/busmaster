;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_Log_54
; TestCases			:		Configuring Log File
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;							2. Remove the log file by clicking on 'Remove' option in 'Log blocks' groupbox.
;							3. Click 'Ok' to close the dialog.
;							4. Reopen the 'Configure logging for LIN' dialog.
;							5.Click 'Ok' to close the dialog.
;							6. Save the configuration.
;							7. Close BUSMASTER application and reopen the application.
;							8. Make sure the previous configuration is loaded.
;							9. Open the 'Configure logging for LIN' dialog.

; Expected Results  : 		1. Start and stop trigger combo boxes will be filled with the list of database frames. At the same time it will be editable so that user LIN enter ID directly.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_Log_54.au3****" & @CRLF)

$fileCheckedafterPressingOK=True
$fileCheckedafterSaving=True

_launchApp() 																										 ;Check if the App is open else launch it

ProcessWait("BUSMASTER.exe") 																						 ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)

If WinExists($WIN_BUSMASTER) Then
	Sleep(1000)
	_LINLogMenu()
	If WinExists($WIN_LINLog) Then
		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									  ;Get the handler for the list view to show the logfiles added
		_GUICtrlListView_SetItemChecked($logfilehwd,0,False)														  ;Uncheck the first item
		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                ;Click on the ok button
		_LINLogMenu()																								  ;Open LIN->Logging->Configure
		Sleep(1000)
		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									  ;Get the handler for the list view to show the logfiles added
		$fileCheckedafterPressingOK=_GUICtrlListView_GetItemChecked($logfilehwd,0)								  ;Checking whether the item is checked
		consolewrite("$fileCheckedafterPressingOK : "&$fileCheckedafterPressingOK&@CRLF)
		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                ;Click on the ok button
	EndIf
	_saveConfig()
	_CloseApp()
EndIf

_launchApp() 																										   ;Check if the App is open else launch it
ProcessWait("BUSMASTER.exe") 																						   ;Pauses script execution until a given process exists.
Sleep(4000)

WinActivate($WIN_BUSMASTER, 3)
If WinExists($WIN_BUSMASTER) Then
	_LINLogMenu()
	Sleep(1000)
	If WinExists($WIN_LINLog) Then
		$logfilehwd = ControlGetHandle($WIN_LINLog, "", $LSTC_LogFile_ConfigLOG)									  ;Get the handler for the list view to show the logfiles added
		$fileCheckedafterSaving=_GUICtrlListView_GetItemChecked($logfilehwd,0)								  ;Checking whether the item is checked
		consolewrite("$fileCheckedafterSaving : "&$fileCheckedafterSaving&@CRLF)
		ControlClick($WIN_LINLog,"",$BTN_OK_ConfigLOG)                                                                ;Click on the ok button
	EndIf

EndIf

if  $fileCheckedafterPressingOK=False And $fileCheckedafterSaving=False then
	_WriteResult("Pass","TS_LIN_Log_54")
	ConsoleWrite("Pass" & @CRLF)
Else
	_WriteResult("Fail","TS_LIN_Log_54")
	ConsoleWrite("Fail" & @CRLF)
EndIf

$isAppNotRes= _CloseApp()

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_Log_54")
EndIf


ConsoleWrite("****End : TS_LIN_Log_54.au3****"&@CRLF)
ConsoleWrite(@CRLF)







