;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_03
; TestCases			:		Message transmission with Key event Trigger.
; Test Data			:
; Test Setup		:		1.Open CAN Tx Window .
;~ 							2.Add messages with the following Characteristics,

;~ 							scenario 1-

;~ 							1. Name: Message Name.
;~ 							2. Repetition : checked
;~ 							3. Provide Repetition Value  : 1000ms
;~ 							3. Key: checked
;~ 							4. Provide Key Name: a
;~ 							5. Connect  Busmaster and start sending messages.
;~ 							6. observe the message window.

;~ 							scenario 2-

;~ 							7. Uncheck repetion of the messages.
;~ 							8. Connect  Busmaster and start sending messages.
;~ 							9. observe the message window.

;~ 							scenario 3-
;~ 							10. Uncheck Key of the messages.
;~ 							11. Connect  Busmaster and start sending messages.
;~ 							12. observe the message window.

; Expected Results  : 		1. After step 5 ,Each message will be transmitted with a delay of 1000ms and Each message in key triggered  can also be observed in message window. from theTx Frame List.
;~							2.After step 8 ,No messages will be transmissted further and only mesasges in key triggered can be transmitetd.
;~ 							3.After step 11,No messages will be transmissted further.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_04****"&@CRLF)

Local $VerifyMsgKey=1,$VerifyMsgRep=0
local $m1_Key_Rx1=0,$m1_Key_Tx1=0,$m2_Key_Rx1=0,$m2_Key_Tx1=0
Local $VerifyScn1Count=0,$VerifyScn2Count=0,$VerifyScn3Count=0,$VerifyScn2WOK=0,$VerifyScn2WK=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then

    _loadConfig("TS_TxWin_CAN_04")

	sleep(500)
	;_TxMsgMenu()																				; Select CAN->Transmit->Configure menu
     sleep(1000)

	;_DisableOverwriteMode()																		; Disable overwrite Mode

	; Scenario -1

	_ConnectDisconnect()																		; Connect CAN
	sleep(2000)
	_ConnectDisconnect()																		; DisConnect CAN
	$rCount=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	;If $rCount=16 Then
		;$VerifyScn1Count=1																		; Scenario -1 is Verified in TS_TxWin_CAN_03
	;EndIf

	If(@OSVersion <> "WIN_7") Then
		If $rCount=12 Then
			$VerifyScn1Count=1																		; Scenario -1 is Verified in TS_TxWin_CAN_03
		EndIf
	Else
		If $rCount=16 Then
			$VerifyScn1Count=1																		; Scenario -1 is Verified in TS_TxWin_CAN_03
		EndIf
	EndIf



	;Scenario- 2; Uncheck repetitionon check Box for all the messages confgiured

	_TxMsgMenu()
	_ActivatechildWindow($WinCANTxWindow)
	Sleep(1000)
	$LVhWndCanTx=_GetTxWindHWD()
	$ColWidthTS4=_GetColWidthTxWin($LVhWndCanTx,6)
	$VerifyMsgItemPos1=_GetRowPosTxWin($LVhWndCanTx,0)
	ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx,"Left",2,$VerifyMsgItemPos1[0]+$ColWidthTS4+520,$VerifyMsgItemPos1[1])					; Uncheck Rep check box for first mesasge
	Sleep(1000)
	$VerifyMsgItemPos2=_GetRowPosTxWin($LVhWndCanTx,1)
	ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx,"Left",2,$VerifyMsgItemPos2[0]+$ColWidthTS4+520,$VerifyMsgItemPos2[1])					; Uncheck Rep check box for Second mesasge
	Sleep(1000)
	_CloseTxWindowArg("CAN")
	_ConnectDisconnect()																		; Connect CAN
	sleep(2000)

	$rCount=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	If $rCount=0 Then
		$VerifyScn2WOK=1																		; Verified Without key press
	EndIf
	Send("a")																					; use Key press"a"
	sleep(500)
	_ConnectDisconnect()																		; Disconnect  CAN
	$rCount=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	If $rCount=4 Then

		For $i=0 to 3                                                                            ;Fetch the no of items from message window
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 7
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      ;Verify for Tx/RX
				$m1_Key_Rx1=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      ;Verify for Tx/RX
				$m1_Key_Tx1=1

			EndIf
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                       ;Verify for Tx/RX
				$m2_Key_Tx1=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                       ;Verify for Tx/RX
				$m2_Key_Rx1=1

			EndIf

		Next
	EndIf
	ConsoleWrite("$m1_Key_Rx1="&$m1_Key_Rx1&@CRLF)
	ConsoleWrite("$m1_Key_Tx1="&$m1_Key_Tx1&@CRLF)
	ConsoleWrite("$m2_Key_Tx1="&$m2_Key_Tx1&@CRLF)
	ConsoleWrite("$m2_Key_Rx1="&$m2_Key_Rx1&@CRLF)


	if $m1_Key_Rx1=1 and $m1_Key_Tx1=1 and $m2_Key_Tx1=1 and $m2_Key_Rx1=1 Then
		$VerifyScn2WK=1																			; Verified With key press
	EndIf

	ConsoleWrite("$VerifyScn2WOK="&$VerifyScn2WOK&@CRLF)
	ConsoleWrite("$VerifyScn2WK="&$VerifyScn2WK&@CRLF)


	if $VerifyScn2WOK=1 and $VerifyScn2WK=1 Then
		$VerifyScn2Count=1
	EndIf

	;scenario 3- Uncheck Key of the messages.

	_TxMsgMenu()
	_ActivatechildWindow($WinCANTxWindow)
	Sleep(1000)
	$LVhWndCanTx1=_GetTxWindHWD()
	$ColWidthKeySc3=_GetColWidthTxWin($LVhWndCanTx1,7)
	$VerifyMsgItemPos3=_GetRowPosTxWin($LVhWndCanTx1,0)
	ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx1,"left",2,$VerifyMsgItemPos3[0]+$ColWidthKeySc3+650,$VerifyMsgItemPos3[1])					; Uncheck Key check box for first mesasge
	Sleep(1000)
	$VerifyMsgItemPos4=_GetRowPosTxWin($LVhWndCanTx1,1)
	ControlClick($WIN_BUSMASTER,"",$LVhWndCanTx1,"left",2,$VerifyMsgItemPos4[0]+$ColWidthKeySc3+650,$VerifyMsgItemPos4[1])					; Uncheck Key check box for Second mesasge
	sleep(1000)
	_CloseTxWindowArg("CAN")
	_ConnectDisconnect()																		; Connect CAN
	sleep(2000)
	Send("a")
	_ConnectDisconnect()																		; DisConnect CAN

	$rCount=_GetCANMsgWinItemCount()
	If $rCount=0 Then
		$VerifyScn3Count=1																		; Verified Scenario 3
	EndIf



EndIf

	ConsoleWrite("$VerifyScn1Count="&$VerifyScn1Count&@CRLF)
	ConsoleWrite("$VerifyScn2Count="&$VerifyScn2Count&@CRLF)
	ConsoleWrite("$VerifyScn3Count="&$VerifyScn3Count&@CRLF)


	If($VerifyScn1Count=1 And $VerifyScn2Count=1 And $VerifyScn3Count=1 ) Then
		_WriteResult("Pass","TS_TxWin_CAN_04")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_04")
	EndIf


	$isAppNotRes=_CloseApp()																; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_04")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)