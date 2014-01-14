;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_03
; TestCases			:		Functionality - Interactive Replay - Go
; Test Data			:
; Test Setup		:		1. Continue from the previous test case.
;~ 							2. Select an entry in the replay window and select Go button from the toolbar
;~ 							3. Check the status of replay buttons in the toolbar and check the message transmission.

; Expected Results  : 		1. After step 2, the replay message transmission will start from the selected message entry.
;~ 							2. Step, Skip and Go buttons will be disabled and Stop button will be enabled.
;~ 							3. Message Transmission will stop after transmitting the last message in the list and the selection will be set to the first entry of the list.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_03.au3****"&@CRLF)


_launchApp()

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_03")																	; Load Configuration

	_ConnectDisconnect()																		; Connect the tool
	sleep(1500)

	$RepWinHWD=_GetReplayWinHWD()																; Fetch the replay window handle
	_GUICtrlListView_ClickItem($RepWinHWD,1)													; Click on the second row

	_CANReplayOptionsMenu($CANReplayGoMenu)														; Select "Go" from menu

	$SelectedIndex=_GUICtrlListView_GetSelectedIndices($RepWinHWD)								; fetch the selected item in the replay window

	_ConnectDisconnect()																		; Disconnect the tool
	sleep(1500)

;~ 	_EnableHex()

	_DisableJ1939Win()																			; If J1939 Msg Window is active then disable it

	$Count=_GetCANMsgWinItemCount()																; Fetch the item count in the msg window
	consolewrite("$Count :"&$Count&@CRLF)

	if $Count=4 Then																			; if count=4 then fetch all the item info
		$FirstMsg=_GetMsgWinCANInfo(0)
		$SecondMsg=_GetMsgWinCANInfo(1)
		$ThirdMsg=_GetMsgWinCANInfo(2)
		$FourthMsg=_GetMsgWinCANInfo(3)
		consolewrite("$FirstMsg[1] :"&$FirstMsg[1]&@CRLF)
		consolewrite("$FirstMsg[2] :"&$FirstMsg[2]&@CRLF)
		consolewrite("$FirstMsg[3] :"&$FirstMsg[3]&@CRLF)
		consolewrite("$FirstMsg[4] :"&$FirstMsg[4]&@CRLF)
		$FirstMsgID=336
		if $FirstMsg[1]="Tx" and $FirstMsg[2]=1 and $FirstMsg[3]="s" and $FirstMsg[4]=="0x150" and $FirstMsg[7]=0000000000000000 Then
			$FirstMsgTx="Found"
		Else
			$FirstMsgTx="Not found"
		EndIf
		if $SecondMsg[1]="Tx" and $SecondMsg[2]=2 and $SecondMsg[3]="s" and $SecondMsg[4]=="0x150"  and $SecondMsg[7]=0000000000000000 Then
			$SecondMsgTx="Found"
		Else
			$SecondMsgTx="Not found"
		EndIf
		if $ThirdMsg[1]="Tx" and $ThirdMsg[2]=1 and $ThirdMsg[3]="s" and $ThirdMsg[4]=="0x200" and $ThirdMsg[7]=0000000000000000 Then
			$ThirdMsgTx="Found"
		Else
			$ThirdMsgTx="Not found"
		EndIf
		if $FourthMsg[1]="Tx" and $FourthMsg[2]=2 and $FourthMsg[3]="s" and $FourthMsg[4]=="0x200" and $FourthMsg[7]=0000000000000000 Then
			$FourthMsgTx="Found"
		Else
			$FourthMsgTx="Not found"
		EndIf
		consolewrite("$SelectedIndex :"&$SelectedIndex&@CRLF)
		consolewrite("$FirstMsgTx :"&$FirstMsgTx&@CRLF)
		consolewrite("$SecondMsgTx :"&$SecondMsgTx&@CRLF)
		consolewrite("$ThirdMsgTx :"&$ThirdMsgTx&@CRLF)
		consolewrite("$FourthMsgTx :"&$FourthMsgTx&@CRLF)

		if $SelectedIndex=0 and  $FirstMsgTx="Found" and $SecondMsgTx="Found" and $ThirdMsgTx="Found" and $FourthMsgTx="Found" Then
			_WriteResult("Pass","TS_Replay_03")
		Else
			_WriteResult("Fail","TS_Replay_03")
		EndIf
	Else
		_WriteResult("Fail","TS_Replay_03")
	EndIf
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_03")
EndIf

ConsoleWrite("****End : TS_Replay_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
