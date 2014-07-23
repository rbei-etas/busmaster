;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_68
; TestCases			:		Archieving Replay configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Add 3 replay files from 3 log files.
;~ 							3. Uncheck the replay files 1 and 3 in 'Replay Files' list box.
;~ 							3. Click 'Ok' to close the dialog.
;~ 							4. Save the configuration.
;~ 							5. Close Busmaster application and reopen the application.
;~ 							6. Make sure the previous configuration is loaded.
;~ 							7. Open the 'Replay configuration dialog'.

; Expected Results  : 		1. After step7, 1 and 3 replay files in list control should be in unchecked state and replay file 2 should be in checked state.

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_68.au3****"&@CRLF)


_launchApp()																							; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER,3)

Local $IsFirstFileChecked=True, $IsSecFileChecked=False, $IsThirdFileChecked=True
if winexists($WIN_BUSMASTER) then

	_CreateConfig("cfxReplay_68.cfx")																; Create Configuration

	_CANReplayMenu()																				; Open configure replay dialog

	If WinExists($WIN_CANReplayConfig) Then

		_ADDReplayFile("Replay_68_1.log")																; Add a replay file

		_ADDReplayFile("Replay_68_2.log")																; Add a replay file

		_ADDReplayFile("Replay_68_3.log")																; Add a replay file

		$RepFileLVhWnD=ControlGetHandle($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig)		; Fetch the handle of Replay File List View

		_GUICtrlListView_SetItemChecked($RepFileLVhWnD,0,False)											; uncheck the first replay  file

		_GUICtrlListView_SetItemChecked($RepFileLVhWnD,2,False)											; uncheck the third replay  file

		ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)									; Click on 'OK' button

	EndIf

	_saveConfig()																					; Save the config

	_CloseApp()																						; Close the app

	_launchApp()																					; Launch the app

	_CANReplayMenu()																				; Open configure replay dialog

	If WinExists($WIN_CANReplayConfig) Then

		$RepFileLVhWnD=ControlGetHandle($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig)	; Fetch the handle of Replay File List View

		$IsFirstFileChecked=_GUICtrlListView_GetItemChecked($RepFileLVhWnD,0)						; Returns the check state of first item

		$IsSecFileChecked=_GUICtrlListView_GetItemChecked($RepFileLVhWnD,1)							; Returns the check state of second item

		$IsThirdFileChecked=_GUICtrlListView_GetItemChecked($RepFileLVhWnD,2)						; Returns the check state of third item

		ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)								; Click on 'OK' button

	EndIf


EndIf

consolewrite("$IsFirstFileChecked :"&$IsFirstFileChecked&@CRLF)
consolewrite("$IsSecFileChecked :"&$IsSecFileChecked&@CRLF)
consolewrite("$IsThirdFileChecked :"&$IsThirdFileChecked&@CRLF)


if $IsFirstFileChecked=False and $IsSecFileChecked=True and $IsThirdFileChecked=False Then
	_WriteResult("Pass","TS_Replay_68")
Else
	_WriteResult("Fail","TS_Replay_68")
EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_68")
EndIf


ConsoleWrite("****End : TS_Replay_68.au3****"&@CRLF)
ConsoleWrite(@CRLF)
