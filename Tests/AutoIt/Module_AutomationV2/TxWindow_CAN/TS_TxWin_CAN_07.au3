;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_07
; TestCases			:		Transmission of multiple messages with different config
; Test Data			:
; Test Setup		:		1. Add two messages to Tx List.

;~ Characteristics of 1st msg -
;~ Repetition - checked
;~ Provide 100 msec
;~ Key-unchecked
;~ Characteristics of 2nd msg -
;~ Repetition - Unchecked
;~ Key-Checked
;~ Provide Key "a"

;~ 2. Connect Busmaster and start sending messages
;~ 3.Press key "a".
;~ 4.Observe the message window.


; 							Expected Results  :1.After step2 Only 1st message will be transmitted and added to the Message window.
;							1. After step4,The message in triggered will be added to the Message window.

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_07****"&@CRLF)

Local $dbMsg1_Rx=0,$dbMsg1_Tx=0,$dbMsg2_Rx=0,$dbMsg2_Tx=0
Local $VerifydbMsg1=0,$VerifydbMsg2=0



_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then

    _loadConfig("TS_TxWin_CAN_01")
	sleep(500)
	_TxMsgMenu()																				; Select CAN->Transmit->Configure menu
     sleep(1000)


	_ConfigTXWinDetails(0,"db",1,1,"","","","1000","")											; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	_ConfigTXWinDetails(1,"db",2,1,"","","","","a")											; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key

	_EnableOverwriteMode()
	_ConnectDisconnect()																		; Connect CAN
	_ActivatechildWindow("Message Window - CAN")
	$rCount=_GetCANMsgWinItemCount()                                                           ; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)

	if $rCount=2 Then
		For $i=0 to 1                                                                            ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 1
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$dbMsg1_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$dbMsg1_Tx=1
			EndIf


		Next

	EndIf
	sleep(1000)
	Send("a")
	Send(1000)
	$rCount1=_GetCANMsgWinItemCount()                                                           ; Fetch the no of items from message window
	ConsoleWrite("$rCount1="&$rCount1&@CRLF)
		if $rCount1=4 Then
		For $i=2 to 3                                                                           ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 1
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                      	;Verify for Tx/RX
				$dbMsg2_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                      	;Verify for Tx/RX
				$dbMsg2_Tx=1
			EndIf


		Next

	EndIf
	_ConnectDisconnect()																		; DisConnect CAN
	_CloseTxWindowArg("CAN")
EndIf

	if $dbMsg1_Rx=1 and $dbMsg1_Rx=1  Then
		$VerifydbMsg1=1
	EndIf

	if $dbMsg2_Rx=1 and $dbMsg2_Tx=1  Then
		$VerifydbMsg2=1
	EndIf

	ConsoleWrite("$VerifydbMsg1="&$VerifydbMsg1&@CRLF)
	ConsoleWrite("$VerifydbMsg2="&$VerifydbMsg2&@CRLF)


	If($VerifydbMsg1=1 And $VerifydbMsg2=1 ) Then
		_WriteResult("Pass","TS_TxWin_CAN_07")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_07")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_07")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)