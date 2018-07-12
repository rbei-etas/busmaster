;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_47
; TestCases			:		Archieving Replay configuration.
; Test Data			:
; Test Setup		:		1. Select a new configuration file.
;~ 							2. Select Configure -> Replay to open 'Replay configuration dialog'.
;~ 							3. Add a new replay file by clicking 'Add' button in 'Replay Files' group box.
;~ 							4. Press 'OK' in 'Replay Configuration dialog.
;~ 							5. Reopen the 'Replay configuration dialog'.
;~ 							6.Click 'Ok' to close the dialog.
;~ 							7. Save the configuration.
;~ 							8. Close Busmaster application and reopen the application.
;~ 							9. Make sure the previous configuration is loaded.
;~ 							10. Open the 'Replay configuration dialog'.
; Expected Results  : 		After step4 and step10. The previously added 'Replay file' should be present in the 'Replay configuration dialog'.

; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_48
; TestCases			:		Archieving Replay configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Check the 'Interactive Replay' checkbox in 'Replay File details' groupbox.
;~ 							3. Click 'Ok' to close the dialog.
;~ 							4. Reopen the 'Replay configuration dialog'.
;~ 							5.Click 'Ok' to close the dialog.
;~ 							6. Save the configuration.
;~ 							7. Close Busmaster application and reopen the application.
;~ 							8. Make sure the previous configuration is loaded.
;~ 							9. Open the 'Replay configuration dialog'.
; Expected Results  : 		1. After step 4 and step 9, the 'Interactive Replay' option should be in checked state for the only replay file added.


;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_47/48.au3****"&@CRLF)
Local $ReplayFilePath1=""
Local $FilePath2=""

_launchApp()

if winexists($WIN_BUSMASTER) then

	_createConfig("cfxReplay_47")																			; Create New Configuration

	_CANReplayMenu()																						; Open configure replay dialog

	_ADDReplayFile("Replay_47")																				; Add a replay file
	sleep(1000)

	ControlCommand($WIN_CANReplayConfig,"",$CHKB_Interactive_CANReplayConfig,"Check")						; Select Interactive replay
	sleep(500)

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)											; Click on 'OK' button
	sleep(500)

	_CANReplayMenu()																						; Open configure replay dialog

	$Count1=ControlListView ($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig,"GetItemCount")		; Fetch the count of files

	$ReplayFilePath1=ControlListView ($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig,"GetText",0,0)		; Fetch the file path
	$BTNState1=ControlCommand ($WIN_CANReplayConfig,"",$CHKB_Interactive_CANReplayConfig,"IsChecked")		; Fetch the Interactive replay option state

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)											; Click on 'OK' button
	sleep(500)

	_saveConfig()																							; save the configuration

	_CloseApp()																								; Close the application

	_launchApp()																							; launch the applciation

	_CANReplayMenu()																						; Open configure replay dialog

	$Count2=ControlListView ($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig,"GetItemCount")		; Fetch the count of files

	$FilePath2=ControlListView ($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig,"GetText",0,0)		; Fetch the file path

	$BTNState2=ControlCommand ($WIN_CANReplayConfig,"",$CHKB_Interactive_CANReplayConfig,"IsChecked")		; Fetch the Interactive replay option state

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)											; Click on 'OK' button
	sleep(500)

	consolewrite("$Count1 :"&$Count1&@CRLF)
	consolewrite("$ReplayFilePath1 :"&$ReplayFilePath1&@CRLF)
	consolewrite("$Count2 :"&$Count2&@CRLF)
	consolewrite("$FilePath2 :"&$FilePath2&@CRLF)
	consolewrite("$BTNState1 :"&$BTNState1&@CRLF)
	consolewrite("$BTNState2 :"&$BTNState2&@CRLF)

	$testDataPath=_TestDataPath()

	if $Count1=1 and $Count2=1  and $ReplayFilePath1=_TestDataPath()&"\Replay_47.log" and $FilePath2=_TestDataPath()&"\Replay_47.log" Then
		_WriteResult("Pass","TS_Replay_47")
	Else
		_WriteResult("Fail","TS_Replay_47")
	EndIf

	if $BTNState1=1 and $BTNState2=1 Then
		_WriteResult("Pass","TS_Replay_48")
	Else
		_WriteResult("Fail","TS_Replay_48")
	EndIf


EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
_WriteResult("Warning","TS_Replay_47")
_WriteResult("Warning","TS_Replay_48")
EndIf


ConsoleWrite("****End : TS_Replay_47/48.au3****"&@CRLF)
ConsoleWrite(@CRLF)
