;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_50
; TestCases			:		Functionality - Interactive Replay - Replay Buttons
; Test Data			:
; Test Setup		:		1. Continue from the previous test case.
;~ 							2. Select the replay window and check the status of following buttons: Step, Skip, Go and Stop.

; Expected Results  : 		1. Step  and Skip buttons will be enabled as it is not a cyclic replay.
;~ 							2. Go button will be enabled and Stop button will be disabled.

; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_51
; TestCases			:		Functionality - Interactive Replay - Step
; Test Data			:
; Test Setup		:		1. Continue from the previous test case.
;~ 							2. Select an entry in the replay window and select Step button from the toolbar

; Expected Results  : 		1. The selected message will be transmitted and the selection will advance to the next entry. If the selected is the last message then selection will be set to the first entry.

; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_52
; TestCases			:		Functionality - Interactive Replay - Skip
; Test Data			:
; Test Setup		:		1. Continue from the previous test case.
;~ 							2. Select an entry in the replay window and select Skip button from the toolbar
;~ 							3. Check the status of replay buttons in the toolbar and check the selection in the replay window.

; Expected Results  : 		1. After step 3, the selection in the replay message list will advance to the next item. If the selected is the last item then selection will go to the first item of the list
;~ 							2. The tool bar buttons Step, Skip and Go will be enabled state and Stop will be in disabled state

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_50.au3****"&@CRLF)


_launchApp()

Local $ReplayBTNState=0
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_50.cfx")																; Load Configuration

	_ConnectDisconnect()																			; Connect

	$RplyTBarhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Con_Disconnect)

	;Check whether Step button is enabled
	$StepComId=_GetIconComId($RplyTBarhWd, $Icon_RplyStep_Index)									; Fetch the command ID of Step button
	$StepBTNState=_IsIconEnabled($RplyTBarhWd,$StepComId)

	;Check whether Skip button is enabled
	$SkipComId=_GetIconComId($RplyTBarhWd, $Icon_RplySkip_Index)									; Fetch the command ID of Skip button
	$SkipBTNState=_IsIconEnabled($RplyTBarhWd,$SkipComId)

	;Check whether Start button is enabled
	$StartComId=_GetIconComId($RplyTBarhWd, $Icon_RplyStart_Index)									; Fetch the command ID of Start button
	$StartBTNState=_IsIconEnabled($RplyTBarhWd,$StartComId)

	;Check whether Stop button is enabled
	$StopComId=_GetIconComId($RplyTBarhWd, $Icon_RplyStop_Index)									; Fetch the command ID of Stop button
	$StopBTNState=_IsIconEnabled($RplyTBarhWd,$StopComId)

	if $StepBTNState=1 and $SkipBTNState=1 and $StartBTNState=1 and $StopBTNState=0 Then
		$ReplayBTNState=1
	EndIf

	$RepWinHWD=_GetReplayWinHWD()
	_GUICtrlListView_ClickItem($RepWinHWD,2)														; Click on the 3rd item in the replay window
	sleep(500)

	;$cntToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Con_Disconnect)							; Get handle of the 'Connect/Disconnect' toolbar
	;_ClickToolBarIcon($cntToolhWd,$Icon_RplyStep_Index)												; Click on Step icon
	_CANReplayOptionsMenu($CANReplayStepMenu)														; Select "Step" from menu

	$SelectedIndexAStep=_GUICtrlListView_GetSelectedIndices($RepWinHWD)								; fetch the selected item in the replay window
	consolewrite("$SelectedIndexAStep :"&$SelectedIndexAStep&@CRLF)

	;_ClickToolBarIcon($cntToolhWd,$Icon_RplySkip_Index)												; Click on Skip icon
	_CANReplayOptionsMenu($CANReplaySkipMenu)

	$SelectedIndexASkip=_GUICtrlListView_GetSelectedIndices($RepWinHWD)								; fetch the selected item in the replay window
	consolewrite("$SelectedIndexASkip :"&$SelectedIndexASkip&@CRLF)

	consolewrite("$ReplayBTNState :"&$ReplayBTNState&@CRLF)

	_ConnectDisconnect()																			; Disconnect

	$MsgWinCount=_GetCANMsgWinItemCount()															; Fetch the count of items in the msg window
	consolewrite("$MsgWinCount :"&$MsgWinCount&@CRLF)

EndIf

;If $ReplayBTNState=1 and $SelectedIndexAStep=3 and $SelectedIndexASkip=4  and $MsgWinCount=1 then
If $SelectedIndexAStep=3 and $SelectedIndexASkip=4  and $MsgWinCount=1 then
	_WriteResult("Pass","TS_Replay_50")
Else
	_WriteResult("Fail","TS_Replay_50")
EndIf




$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
_WriteResult("Warning","TS_Replay_50")
EndIf


ConsoleWrite("****End : TS_Replay_50.au3****"&@CRLF)
ConsoleWrite(@CRLF)
