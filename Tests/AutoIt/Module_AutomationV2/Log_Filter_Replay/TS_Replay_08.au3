;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_08
; TestCases			:		Replay a log file in cyclic mode with no messages in the log file
; Test Data			:
; Test Setup		:		1. Select "Configure-> Replay".
;~ 							2. A Dialog Box will appear.
;~ 							3. Select a replay file and select time delay between messages as 50ms.
;~ 							4. Select Mode of Replay as "Cyclic"
;~ 							5. Select a Log file name
;~ 							6. Select OK. Replay Window with message in log file will appear
;~ 							7. Select "File -> Connect" menu option or select "Connect" tool bar button
;~ 							8. Enable logging
;~ 							9. Repeat the test case after editing the log file having four message
;~ 							10. Select Go button

; Expected Results  : 		Replay window should close.
;~ 							All the replay buttons and menu items should be disabled after step 7.
;~ 							After step 10, messages in the message window should match with the log file. The delay between messages should be 50msec with a variation of 1msec.
;~ 							These messages should be sent continuously without any cycle delay
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_08.au3****"&@CRLF)


_launchApp()


if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_08")																	; Load Configuration

	_CANReplayMenu()																			; Open configure replay dialog

	_ADDReplayFile("Replay_08")																	; Add a replay file
	sleep(1000)

	ControlCommand($WIN_CANReplayConfig,"",$CHKB_RecTimeDelay_CANReplayConfig,"UnCheck")		; Uncheck 'Retain recorded time delay' option

	ControlSetText($WIN_CANReplayConfig,"",$TXT_MsgDelay_CANReplayConfig,2000)					; Set 2000 msec as Delay btw replay messages

	ControlClick($WIN_CANReplayConfig,"",$RBTN_Cyclic_CANReplayConfig)							; Check 'Cyclic' option

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)								; Click on 'OK' button

	_ConnectDisconnect()																		; Connect the tool
	sleep(10000)

	_ConnectDisconnect()																		; Disconnect the tool

	$MsgWinCount=_GetCANMsgWinItemCount()														; Fetch the Replay Msg window count
	consolewrite("$MsgWinCount :"&$MsgWinCount&@CRLF)

	$Counter1=0
	$Counter2=0

	if $MsgWinCount>4 Then

		; Fetch the time of First entry in the list view
		$hwd= ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)									; Fetch the Msg Window Handle
		$FirstMsgEntry=_GUICtrlListView_GetItemTextString($hwd,0)									; Fetch the Msg window first item text
		ConsoleWrite("$FirstMsgEntry:"&$FirstMsgEntry&@CRLF)
		$FirstMsg=StringTrimLeft($FirstMsgEntry,1)													; Remove the first character
		ConsoleWrite("$FirstMsg:"&$FirstMsg&@CRLF)
		$RepFirstMsg=StringReplace($FirstMsg,"|"," ")
		ConsoleWrite("$RepFourthMsg:"&$RepFirstMsg&@CRLF)
		$SplitFirstMsg=StringSplit($RepFirstMsg," ")
		ConsoleWrite("$SplitFirstMsg:"&$SplitFirstMsg[1]&@CRLF)
		$FirstMsgTime=StringSplit($SplitFirstMsg[1],":")
		ConsoleWrite("$FirstMsgTime:"&$FirstMsgTime[3]&@CRLF)
		ConsoleWrite("$SplitFirstMsg[5]:"&$SplitFirstMsg[5]&@CRLF)

		; Fetch the time of Second entry in the list view
		$SecondMsgEntry=_GUICtrlListView_GetItemTextString($hwd,1)									; Fetch the Msg window first item text
		ConsoleWrite("$SecondMsgEntry:"&$SecondMsgEntry&@CRLF)
		$SecondMsg=StringTrimLeft($SecondMsgEntry,1)												; Remove the first character
		ConsoleWrite("$SecondMsg:"&$SecondMsg&@CRLF)
		$RepSecondMsg=StringReplace($SecondMsg,"|"," ")
		ConsoleWrite("$RepSecondMsg:"&$RepSecondMsg&@CRLF)
		$SplitSecondMsg=StringSplit($RepSecondMsg," ")
		ConsoleWrite("$SplitSecondMsg:"&$SplitSecondMsg[1]&@CRLF)
		$SecondMsgTime=StringSplit($SplitSecondMsg[1],":")
		ConsoleWrite("$SecondMsgTime:"&$SecondMsgTime[3]&@CRLF)
		ConsoleWrite("$SplitSecondMsg[5]:"&$SplitSecondMsg[5]&@CRLF)

		if $SecondMsgTime[3]>$FirstMsgTime[3] Then
			$Counter1=$SecondMsgTime[3]-$FirstMsgTime[3]
		Elseif $SecondMsgTime[3]<$FirstMsgTime[3] Then
			if $SecondMsgTime[3]=01 and $FirstMsgTime[3]=59 Then
				$Counter1=2
			Elseif $SecondMsgTime[3]=02 and $FirstMsgTime[3]=60 Then
				$Counter1=2
			Elseif $SecondMsgTime[3]=00 and $FirstMsgTime[3]=58 Then
				$Counter1=2
			EndIf
		EndIf

		; Fetch the time of Third entry in the list view
		$ThirdMsgEntry=_GUICtrlListView_GetItemTextString($hwd,2)									; Fetch the Msg window Third item text
		ConsoleWrite("$ThirdMsgEntry:"&$ThirdMsgEntry&@CRLF)
		$ThirdMsg=StringTrimLeft($ThirdMsgEntry,1)													; Remove the Third character
		ConsoleWrite("$ThirdMsg:"&$ThirdMsg&@CRLF)
		$RepThirdMsg=StringReplace($ThirdMsg,"|"," ")
		ConsoleWrite("$RepFourthMsg:"&$RepThirdMsg&@CRLF)
		$SplitThirdMsg=StringSplit($RepThirdMsg," ")
		ConsoleWrite("$SplitThirdMsg:"&$SplitThirdMsg[1]&@CRLF)
		$ThirdMsgTime=StringSplit($SplitThirdMsg[1],":")
		ConsoleWrite("$ThirdMsgTime:"&$ThirdMsgTime[3]&@CRLF)
		ConsoleWrite("$SplitThirdMsg[5]:"&$SplitThirdMsg[5]&@CRLF)

		; Fetch the time of Fourth entry in the list view
		$FourthMsgEntry=_GUICtrlListView_GetItemTextString($hwd,3)									; Fetch the Msg window Fourth item text
		ConsoleWrite("$FourthMsgEntry:"&$FourthMsgEntry&@CRLF)
		$FourthMsg=StringTrimLeft($FourthMsgEntry,1)												; Remove the Third character
		ConsoleWrite("$FourthMsg:"&$FourthMsg&@CRLF)
		$RepFourthMsg=StringReplace($FourthMsg,"|"," ")
		ConsoleWrite("$RepFourthMsg:"&$RepFourthMsg&@CRLF)
		$SplitFourthMsg=StringSplit($RepFourthMsg," ")
		ConsoleWrite("$SplitFourthMsg:"&$SplitFourthMsg[1]&@CRLF)
		$FourthMsgTime=StringSplit($SplitFourthMsg[1],":")
		ConsoleWrite("$FourthMsgTime:"&$FourthMsgTime[3]&@CRLF)
		ConsoleWrite("$SplitFourthMsg[5]:"&$SplitFourthMsg[5]&@CRLF)

		if $FourthMsgTime[3]>$ThirdMsgTime[3] Then
			$Counter2=$FourthMsgTime[3]-$ThirdMsgTime[3]
		Elseif $FourthMsgTime[3]<$ThirdMsgTime[3] Then
			if $FourthMsgTime[3]=01 and $ThirdMsgTime[3]=59 Then
				$Counter2=2
			Elseif $FourthMsgTime[3]=02 and $ThirdMsgTime[3]=60 Then
				$Counter2=2
			Elseif $FourthMsgTime[3]=00 and $ThirdMsgTime[3]=58 Then
				$Counter2=2
			EndIf
		EndIf

	EndIf

	ConsoleWrite("$Counter1:"&$Counter1&@CRLF)
	ConsoleWrite("$Counter2:"&$Counter2&@CRLF)

	if $MsgWinCount=11 and $Counter1=2 and $Counter2=2 Then
		_WriteResult("Pass","TS_Replay_08")
	Else
		_WriteResult("Fail","TS_Replay_08")
	EndIf
EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_08")
EndIf


ConsoleWrite("****End : TS_Replay_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)
