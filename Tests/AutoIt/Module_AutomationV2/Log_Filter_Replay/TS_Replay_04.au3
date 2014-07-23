;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_04
; TestCases			:		Functionality - Interactive Replay - Stop
; Test Data			:		
; Test Setup		:		1. Repeat previous test case
;~ 							2. Select Stop button from the tool bar after selecting Go button.
;~ 							3. Check the status of message transmission, toolbar buttons and message selection in the replay window.

; Expected Results  : 		1. After step 3, the message transmission will be stopped.
;~ 							2. Step, Skip and Go buttons will be enabled. Stop button will be disabled.
;~ 							3. Message selection will show the message that has to be transmitted next.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_04.au3****"&@CRLF)


_launchApp()

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_04")																		; Load Configuration

	_ConnectDisconnect()																			; Connect the tool
	sleep(1500)

	_CANReplayOptionsMenu($CANReplayGoMenu)
	sleep(100)

	_CANReplayOptionsMenu($CANReplayStopMenu)

	$RepWinHWD=_GetReplayWinHWD()
	$SelectedIndex=_GUICtrlListView_GetSelectedIndices($RepWinHWD)									; fetch the selected item in the replay window
	consolewrite("$SelectedIndex :"&$SelectedIndex&@CRLF)

	$State=0
	$RepSelMsg=0
	$TxLastMsg=0

	if $SelectedIndex>0 Then

		$GoMenuState=_GetReplayOptionsMenuState(3)													; Fetch the 'Go' menu state

		$StepMenuState=_GetReplayOptionsMenuState(4)												; Fetch the 'Step' menu state

		$SkipMenuState=_GetReplayOptionsMenuState(5)												; Fetch the 'Skip' menu state

		$StopMenuState=_GetReplayOptionsMenuState(6)												; Fetch the 'Stop' menu state

		consolewrite("$GoMenuState :"&$GoMenuState&@CRLF)
		consolewrite("$StepMenuState :"&$StepMenuState&@CRLF)
		consolewrite("$SkipMenuState :"&$SkipMenuState&@CRLF)
		consolewrite("$StopMenuState :"&$StopMenuState&@CRLF)

		$NextMsgRep=_GetReplayWinInfo($SelectedIndex)												; Fetch the selected msg info
		$NextMsgRepSplit=StringSplit($NextMsgRep," ")

		consolewrite("$NextMsgRep :"&$NextMsgRep&@CRLF)
		consolewrite("$NextMsgRepSplit[1] :"&$NextMsgRepSplit[1]&@CRLF)
		consolewrite("$NextMsgRepSplit[2] :"&$NextMsgRepSplit[2]&@CRLF)
		consolewrite("$NextMsgRepSplit[3] :"&$NextMsgRepSplit[3]&@CRLF)

		if $GoMenuState=true and $StepMenuState=true and  $SkipMenuState=true and  $StopMenuState=False Then
			$State=1
		EndIf
		consolewrite("$State :"&$State&@CRLF)

		if $NextMsgRepSplit[2]="Tx" and $NextMsgRepSplit[3]=1 and $NextMsgRepSplit[4]="0x012" Then
			$RepSelMsg=1
		EndIf
		consolewrite("$RepSelMsg :"&$RepSelMsg&@CRLF)
	Endif

	_ConnectDisconnect()																			; Disconnect the tool
	sleep(1500)

	$MsgWinCount=_GetCANMsgWinItemCount()															; Fetch the count of items in the msg window
	consolewrite("$MsgWinCount :"&$MsgWinCount&@CRLF)

	If $MsgWinCount>0 then
		$LastMsg=_GetMsgWinCANInfo($MsgWinCount-1)													; Fetch the last msg in the msg window

		consolewrite("$LastMsg[1] :"&$LastMsg[1]&@CRLF)
		consolewrite("$LastMsg[2] :"&$LastMsg[2]&@CRLF)
		consolewrite("$LastMsg[3] :"&$LastMsg[3]&@CRLF)
		consolewrite("$LastMsg[3] :"&$LastMsg[4]&@CRLF)
		
		if $LastMsg[1]="Tx" and $LastMsg[2]=2 and $LastMsg[4]="0x016" Then
			$TxLastMsg=1
		EndIf
		consolewrite("$TxLastMsg :"&$TxLastMsg&@CRLF)
	EndIf

	if $SelectedIndex=8 and $State=1 and $RepSelMsg=1 and $TxLastMsg=1 Then
		_WriteResult("Pass","TS_Replay_04")
	Else
		_WriteResult("Fail","TS_Replay_04")
	EndIf

EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_04")
EndIf

ConsoleWrite("****End : TS_Replay_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
