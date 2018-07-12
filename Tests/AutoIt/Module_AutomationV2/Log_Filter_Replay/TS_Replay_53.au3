;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_53
; TestCases			:		Functionality - Interactive Replay - Break Points
; Test Data			:
; Test Setup		:		1. Double click a message entry in the replay window to set the break point.
;~ 							2. Select an entry before break point and select Go button. Make sure that the selected entry is not having break point and there are few messages before next break point
;~ 							3. Wait till the message just before the break point is transmitted.
;~ 							4. Check the status of replay.

; Expected Results  : 		1. Messages till next break point will be transmitted.
;~ 							2. Once the message just before the break point is transmitted the replay will stop and the selection will be set to the break point line.

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_53.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $TxFirstMsg=0,$TxThirdMsg=0
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_53.cfx")																	; Load Configuration

	;_ConnectDisconnect()																			; Connect
	_Connect_CAN_Menu()

	$RepWinHWD=_GetReplayWinHWD()
	_GUICtrlListView_ClickItem($RepWinHWD,2,"","",2)												; Double click on the 3rd item in the replay window to setup a break point
	sleep(500)

	_GUICtrlListView_ClickItem($RepWinHWD,0)														; Click on the 1st item in the replay window
	sleep(500)

	;$cntToolhWd=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Con_Disconnect)					    ; Get handle of the 'Connect/Disconnect' toolbar
	;_ClickToolBarIcon($cntToolhWd,$Icon_RplyStart_Index)
	_CANReplayOptionsMenu($CANReplayGoMenu)                                                         ; Click on Go menu
	sleep(2000)

	;_ConnectDisconnect()																			; Disconnect

	$MsgWinCount=_GetCANMsgWinItemCount()															; Fetch the count of items in the msg window
	consolewrite("$MsgWinCount :"&$MsgWinCount&@CRLF)

	If $MsgWinCount=4 then
		$FirstMsg=_GetMsgWinCANInfo(1)																; Fetch the First msg in the msg window [Tx]

		consolewrite("$FirstMsg[1] :"&$FirstMsg[1]&@CRLF)
		consolewrite("$FirstMsg[2] :"&$FirstMsg[2]&@CRLF)
		consolewrite("$FirstMsg[3] :"&$FirstMsg[3]&@CRLF)
		consolewrite("$FirstMsg[3] :"&$FirstMsg[4]&@CRLF)


			if $FirstMsg[1]="Tx" and $FirstMsg[2]=1 and $FirstMsg[4]="0x001" Then
				$TxFirstMsg=1
			EndIf


		$ThirdMsg=_GetMsgWinCANInfo(3)																; Fetch the third msg in the msg window [Rx]

		consolewrite("$ThirdMsg[1] :"&$ThirdMsg[1]&@CRLF)
		consolewrite("$ThirdMsg[2] :"&$ThirdMsg[2]&@CRLF)
		consolewrite("$ThirdMsg[3] :"&$ThirdMsg[3]&@CRLF)
		consolewrite("$ThirdMsg[3] :"&$ThirdMsg[4]&@CRLF)

		if $ThirdMsg[1]="Tx" and $ThirdMsg[2]=2 and $ThirdMsg[4]="0x001" Then
			$TxThirdMsg=1
		EndIf

	EndIf

	consolewrite("$TxFirstMsg :"&$TxFirstMsg&@CRLF)
	consolewrite("$TxThirdMsg :"&$TxThirdMsg&@CRLF)

EndIf

If $TxFirstMsg=1 and $TxThirdMsg=1 then
	_WriteResult("Pass","TS_Replay_53")
Else
	_WriteResult("Fail","TS_Replay_53")
EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
_WriteResult("Warning","TS_Replay_53")
EndIf


ConsoleWrite("****End : TS_Replay_53.au3****"&@CRLF)
ConsoleWrite(@CRLF)
