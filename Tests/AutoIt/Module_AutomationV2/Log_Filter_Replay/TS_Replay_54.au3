;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_54
; TestCases			:		Functionality - Interactive Replay - Replay Buttons for Cyclic Replay
; Test Data			:
; Test Setup		:		1. Select a replay file in Cyclic mode.
;~ 							2. Connect the tool.
;~ 							3. Select the replay window and check the status of following toolbar buttons: Step, Skip, Go and Stop.

; Expected Results  : 		1. Step  and Skip buttons will be disabled as it is a cyclic replay.
;~ 							2. Go button will be enabled and Stop button will be disabled.

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_54.au3****"&@CRLF)


_launchApp()																							; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER,3)

Local $StepBTNState=1,$SkipBTNState=1,$StartBTNState=0,$StopBTNState=1
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_54.cfx")																	; Load Configuration

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

	_ConnectDisconnect()																			; Disconnect

EndIf

if $StepBTNState=0 and $SkipBTNState=0 and $StartBTNState=1 and $StopBTNState=0 Then
	_WriteResult("Pass","TS_Replay_54")
Else
	_WriteResult("Fail","TS_Replay_54")
EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
_WriteResult("Warning","TS_Replay_54")
EndIf


ConsoleWrite("****End : TS_Replay_54.au3****"&@CRLF)
ConsoleWrite(@CRLF)
