;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_60
; TestCases			:		Functionality - Interactive Replay - Replay Buttons for Cyclic Replay
; Test Data			:
; Test Setup		:		1. Select a replay file in Cyclic mode.
;~ 							2. Connect the tool.
;~ 							3. Select the replay window and check the status of following toolbar buttons: Step, Skip, Go and Stop.

; Expected Results  : 		1. Step  and Skip buttons will be disabled as it is a cyclic replay.
;~ 							2. Go button will be enabled and Stop button will be disabled.

; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_61
; TestCases			:		Archieving Replay configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Uncheck 'Retain the recoprded time delay' checkbox in Time mode' groupbox.
;~ 							3. Enter '100' msec in the edit box for 'Delay between replay messages'.
;~ 							4. Click 'Ok' to close the dialog.
;~ 							5. Reopen the 'Replay configuration dialog'.
;~ 							6.Click 'Ok' to close the dialog.
;~ 							7. Save the configuration.
;~ 							8. Close Busmaster application and reopen the application.
;~ 							9. Make sure the previous configuration is loaded.
;~ 							10. Open the 'Replay configuration dialog'.

; Expected Results  : 		1. After step 5 and step 10, the 'Retain the recoprded time delay' checkbox should be in unchecked state and '100' msec value in 'Delay between replay messages' edit box for the only replay file added

; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_62
; TestCases			:		Archieving Replay configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Select the previously added replay file and click on 'Delete' button.
;~ 							3. A message box 'Do you want to remove selected replay file?' will be displayed.
;~ 							4. Click 'No' in the message box.
;~ 							5. Click 'Yes' in the message box.

; Expected Results  : 		1. After step 4, the replay file should remain in the replay files list in 'Replay configuration dialog'.
;~ 							2.After step 5, the replay file will be removed from the replay files list in 'Replay configuration dialog'

; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_63
; TestCases			:		Archieving Replay configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;~ 							2. Click 'Ok' to close the dialog.
;~ 							3. Reopen the 'Replay configuration dialog'.
;~ 							4. Click 'Ok' to close the dialog.
;~ 							5. Save the configuration.
;~ 							6. Close Busmaster application and reopen the application.
;~ 							7. Make sure the previous configuration is loaded.
;~ 							8. Open the 'Replay configuration dialog'.

; Expected Results  : 		1. After step 3 and step 8, the 'Replay configuration dialog' should not contain any replay files.

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_60.au3****"&@CRLF)


_launchApp()																							; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER,3)


Local $RepFileCount=0,$RecTimeDelBTNState=1,$RepMsgsDelay=0,$CycReplModeBTNState=0,$CycDelay=0,$RepWinSet=0,$RepFileCountAftLoad=1,$FCount=0
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_60.cfx")																	; Load Configuration

	_CANReplayMenu()																				; Open configure replay dialog

	if WinExists($WIN_CANReplayConfig) Then

		_ADDReplayFile("Replay_50.log")																	; Add a replay file

		ControlCommand($WIN_CANReplayConfig,"",$CHKB_RecTimeDelay_CANReplayConfig,"UnCheck")			; Uncheck 'Retain recorded time delay' option
		sleep(500)

		ControlSetText($WIN_CANReplayConfig,"",$TXT_MsgDelay_CANReplayConfig,100)						; Set 100 msec as Delay btw replay messages
		sleep(500)

		ControlCommand($WIN_CANReplayConfig,"",$RBTN_Cyclic_CANReplayConfig,"Check")					; Check 'Cyclic' option
		sleep(500)

		ControlSetText($WIN_CANReplayConfig,"",$TXT_CyclicDelay_CANReplayConfig,100)					; Set 100 msec as Cyclic Delay
		sleep(500)

		ControlClick($WIN_CANReplayConfig,"",$BTN_Filters_CANReplayConfig)								; Click on 'Filter' button

		if WinWaitActive($WIN_FilterSelect,"",3) Then
			$fltSelhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_ConfigFilter_FilterSelect)			; Get handle of filter selection list view
			_GUICtrlListView_ClickItem($fltSelhWd,0)													; Click on the first filter
			ControlClick($WIN_FilterSelect,"",$BTN_Add_FilterSelect)									; Click on Add button
			sleep(800)
			ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)										; Click on OK button
			sleep(800)
		EndIf

		ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)									; Click on 'OK' button

	EndIf

	_saveConfig()																					; Save the config

	_CloseApp()																						; Close the app

	_launchApp()																					; Launch the app

	_CANReplayMenu()																				; Open configure replay dialog

	$RepFileLVhWnD=ControlGetHandle($WIN_CANReplayConfig,"",$LVC_ReplayFile_CANReplayConfig)	; Fetch the handle of Replay File handle

	if WinExists($WIN_CANReplayConfig) Then
		$RepFileCount=_GUICtrlListView_GetItemCount($RepFileLVhWnD)									; Fetch the count of no. of files in Config replay window

		if $RepFileCount=1 then

			$RecTimeDelBTNState=Controlcommand($WIN_CANReplayConfig,"",$CHKB_RecTimeDelay_CANReplayConfig,"IsChecked")					; Fetch the 'Retain recorded time delay' BTN status

			$RepMsgsDelay=ControlGetText($WIN_CANReplayConfig,"",$TXT_MsgDelay_CANReplayConfig)											; Fetch the delay btw replay msgs value

			$CycReplModeBTNState=Controlcommand($WIN_CANReplayConfig,"",$RBTN_Cyclic_CANReplayConfig,"IsChecked")						; Fetch the Cyclic mode BTN status

			$CycDelay=ControlGetText($WIN_CANReplayConfig,"",$TXT_CyclicDelay_CANReplayConfig)											; Fetch the Cyclic delay value

			ControlClick($WIN_CANReplayConfig,"",$BTN_Filters_CANReplayConfig)															; Click on 'Filter' button

			if WinWaitActive($WIN_FilterSelect,"",3) Then
				$SelFilterhWd=controlgethandle($WIN_FilterSelect,"",$LSTC_SelectedFilter_FilterSelect)			; Get handle of selected filter list view
				$FCount=_GUICtrlListView_GetItemCount($SelFilterhWd)											; Fetch the item count
				ControlClick($WIN_FilterSelect,"",$BTN_OK_FilterSelect)											; Click on OK button
			EndIf

		EndIf

		if $RecTimeDelBTNState=0 and $RepMsgsDelay=100 and $CycReplModeBTNState=1 and $CycDelay=100 and $FCount=1 Then
			$RepWinSet=1
		EndIf

		controlclick($WIN_CANReplayConfig,"",$BTN_DELFile_CANReplayConfig)							; Delete the log file from config replay window

		if WinWaitActive($WIN_BUSMASTER,$TXT_DELReplayFile,3) Then
			ControlClick($WIN_BUSMASTER,$TXT_DELReplayFile,$BTN_DellReplayFileNo_BM)				; Click on 'No' button
			sleep(500)
		EndIf

		$RepFileCountDelNo=_GUICtrlListView_GetItemCount($RepFileLVhWnD)							; Fetch the count of no. of files in Config replay window

		controlclick($WIN_CANReplayConfig,"",$BTN_DELFile_CANReplayConfig)							; Delete the log file from config replay window

		if WinWaitActive($WIN_BUSMASTER,$TXT_DELReplayFile,3) Then
			ControlClick($WIN_BUSMASTER,$TXT_DELReplayFile,$BTN_DellReplayFileYes_BM)				; Click on 'Yes' button
			sleep(500)
		EndIf

		$RepFileCountDelYes=_GUICtrlListView_GetItemCount($RepFileLVhWnD)							; Fetch the count of no. of files in Config replay window

		ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)								; Click on 'OK' button

	EndIf

	_saveConfig()																					; Save the config

	_CloseApp()																						; Close the app

	_launchApp()																					; Launch the app

	_CANReplayMenu()																				; Open configure replay dialog

	if WinExists($WIN_CANReplayConfig) Then
		$RepFileCountAftLoad=_GUICtrlListView_GetItemCount($RepFileLVhWnD)							; Fetch the count of no. of files in Config replay window
		ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)								; Click on 'OK' button
	EndIf

EndIf

consolewrite("$RepFileCount :"&$RepFileCount&@CRLF)
consolewrite("$RecTimeDelBTNState :"&$RecTimeDelBTNState&@CRLF)
consolewrite("$RepMsgsDelay :"&$RepMsgsDelay&@CRLF)
consolewrite("$CycReplModeBTNState :"&$CycReplModeBTNState&@CRLF)
consolewrite("$FCount :"&$FCount&@CRLF)
consolewrite("$RepWinSet :"&$RepWinSet&@CRLF)
consolewrite("$RepFileCountDelNo :"&$RepFileCountDelNo&@CRLF)
consolewrite("$RepFileCountDelYes :"&$RepFileCountDelYes&@CRLF)
consolewrite("$RepFileCountAftLoad :"&$RepFileCountAftLoad&@CRLF)

if $RepWinSet=1 and  $RepFileCountDelNo=1 and $RepFileCountDelYes=0 and $RepFileCountAftLoad=0 Then
	_WriteResult("Pass","TS_Replay_60")
Else
	_WriteResult("Fail","TS_Replay_60")
EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_60")
EndIf


ConsoleWrite("****End : TS_Replay_60.au3****"&@CRLF)
ConsoleWrite(@CRLF)
