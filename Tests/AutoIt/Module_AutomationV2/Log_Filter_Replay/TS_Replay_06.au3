;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_06
; TestCases			:		Functionality - Non Interactive Replay
; Test Data			:		
; Test Setup		:		1. Repeat previous test case in cyclic replay mode

; Expected Results  : 		1. Same as given in the previous test case
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_06.au3****"&@CRLF)


_launchApp()


if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_06")																	; Load Configuration

	_CANReplayMenu()																			; Open configure replay dialog

	ControlClick($WIN_CANReplayConfig,"",$RBTN_Cyclic_CANReplayConfig)							; Check 'Cyclic' option

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)								; Click on 'OK' button

	_ConnectDisconnect()																		; Connect the tool
	sleep(750)

	_ConnectDisconnect()																		; Disconnect the tool

	$MsgWinCount=_GetCANMsgWinItemCount()														; Fetch the Replay Msg window count
	consolewrite("$MsgWinCount :"&$MsgWinCount&@CRLF)

	if $MsgWinCount>6 Then
		_WriteResult("Pass","TS_Replay_06")
	Else
		_WriteResult("Fail","TS_Replay_06")
	EndIf

EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_06")
EndIf


ConsoleWrite("****End : TS_Replay_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)
