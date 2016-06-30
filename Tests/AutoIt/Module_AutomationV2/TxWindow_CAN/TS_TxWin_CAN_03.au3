;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_03
; TestCases			:		Message transmission with Key event Trigger.
; Test Data			:
; Test Setup		:		1.Open CAN Tx Window .
;							2.Add messages with the following Characteristics,

;							scenario -
;							1. Name: Message Name.
;							2. Repetition : checked
;							3. Provide Repetition Value  : 2000ms
;							3. Key: checked
;							4. Provide Key Name: a
;							5. Connect  Busmaster and start sending messages.
;							6. observe the message window.

; Expected Results  : 		1.Each message will be transmitted with a delay of 2000ms from theTx Frame List.
;							2. Each message in key triggered  can also be observed in message window.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_03****"&@CRLF)

Local $Timedelay=0,$VerifyMsgKey=1,$VerifyMsgRep=0
local $m1_Key_Rx1=0,$m1_Key_Tx1=0,$m2_Key_Rx1=0,$m2_Key_Tx1=0
Local $m1SecondMsgTime=0,$m1FirstMsgTime=0,$m2SecondMsgTime=0,$m2FirstMsgTime=0,$Time1=0,$Time2=0,$Time3=0,$Time4=0
Local $m1_dbMsg_Rx1=0,$m1_dbMsg_Tx1=0,$m2_dbMsg_Tx1=0,$m2_dbMsg_Rx1=0,$m1_dbMsg_Rx2=0,$m1_dbMsg_Tx2=0,$m2_dbMsg_Tx2=0,$m2_dbMsg_Rx2=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then

    _loadConfig("TS_TxWin_CAN_01")
	;_CANDriverSelectionXML()
	sleep(500)
	_TxMsgMenu()																				; Select CAN->Transmit->Configure menu
     sleep(1000)
	_ConfigTXWinDetails(0,"db",1,1,"","","",2000,"a")											;$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	_ConfigTXWinDetails(1,"db",2,1,"","","",2000,"a")											;$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	;_ConfigTXWinDetails(2,"db",3,1,"","","",1000,"b")											;$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	;_ConfigTXWinDetails(3,"db",4,1,"","","",1000,"b")											;$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key


	_CloseTxWindowArg("CAN")																	; Close Tx window
	_DisableOverwriteMode()																		; Disable overwrite Mode
	_ConnectDisconnect()																		; Connect CAN
	Send("a")
	sleep(4000)
	_ConnectDisconnect()																		; DisConnect CAN

	$rCount=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	If $rCount=12 Then

		;$Data1=_GetMsgWinCANInfo(0)                                                             ;Fetch messages from message window
	    ;$Data2=_GetMsgWinCANInfo(1)
	   ;

	    For $i=0 to 3                                                                            ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 7
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                       ;Verify for Tx/RX
				$m1_Key_Rx1=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                       ;Verify for Tx/RX
				$m1_Key_Tx1=1
				;$m1FirstMsgTime=StringSplit($Data[0],":")
			EndIf
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                       ;Verify for Tx/RX
				$m2_Key_Tx1=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                       ;Verify for Tx/RX
				$m2_Key_Rx1=1
				;$m2FirstMsgTime=StringSplit($Data[0],":")
			EndIf

		Next
		For $i=4 to 7
				$Data=_GetMsgWinCANInfo($i)
				for $j=0 to 7
					ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
				Next
				If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                       ;Verify for Tx/RX
					$m1_dbMsg_Rx1=1
				EndIf
				If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                       ;Verify for Tx/RX
					$m1_dbMsg_Tx1=1
					$m1FirstMsgTime=StringSplit($Data[0],":")
					$Time1=$m1FirstMsgTime[3]
				EndIf
				If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                       ;Verify for Tx/RX
					$m2_dbMsg_Tx1=1
				EndIf
				If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                       ;Verify for Tx/RX
					$m2_dbMsg_Rx1=1
					$m2FirstMsgTime=StringSplit($Data[0],":")
					$Time2=$m2FirstMsgTime[3]
				EndIf

		Next
		For $i=8 to 11
				$Data=_GetMsgWinCANInfo($i)
				for $j=0 to 7
					ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
				Next
				If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                       ;Verify for Tx/RX
					$m1_dbMsg_Rx2=1
				EndIf
				If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                       ;Verify for Tx/RX
					$m1_dbMsg_Tx2=1
					$m1SecondMsgTime=StringSplit($Data[0],":")
					$Time3=$m1SecondMsgTime[3]
				EndIf
				If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                       ;Verify for Tx/RX
					$m2_dbMsg_Tx2=1
				EndIf
				If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x020 And $Data[5]="m2" And $Data[6]=7 And $Data[7]="00000000000000" Then                                       ;Verify for Tx/RX
					$m2_dbMsg_Rx2=1
					$m2SecondMsgTime=StringSplit($Data[0],":")
					$Time4=$m2SecondMsgTime[3]
				EndIf

		Next


	EndIf

	ConsoleWrite("$m1_Key_Rx1="&$m1_Key_Rx1&@CRLF)
	ConsoleWrite("$m1_Key_Tx1="&$m1_Key_Tx1&@CRLF)
	ConsoleWrite("$m2_Key_Tx1="&$m2_Key_Tx1&@CRLF)
	ConsoleWrite("$m2_Key_Rx1="&$m2_Key_Rx1&@CRLF)


	ConsoleWrite("$m1_dbMsg_Rx1="&$m1_dbMsg_Rx1&@CRLF)
	ConsoleWrite("$m1_dbMsg_Tx1="&$m1_dbMsg_Tx1&@CRLF)
	ConsoleWrite("$m2_dbMsg_Tx1="&$m2_dbMsg_Tx1&@CRLF)
	ConsoleWrite("$m2_dbMsg_Rx1="&$m2_dbMsg_Rx1&@CRLF)

	ConsoleWrite("$m1_dbMsg_Rx2="&$m1_dbMsg_Rx2&@CRLF)
	ConsoleWrite("$m1_dbMsg_Tx2="&$m1_dbMsg_Tx2&@CRLF)
	ConsoleWrite("$m2_dbMsg_Tx2="&$m2_dbMsg_Tx2&@CRLF)
	ConsoleWrite("$m2_dbMsg_Rx2="&$m2_dbMsg_Rx2&@CRLF)

	;ConsoleWrite("$m1SecondMsgTime[3]="&$m1SecondMsgTime[3]&@CRLF)
	;ConsoleWrite("$m1FirstMsgTime[3]="&$m1FirstMsgTime[3]&@CRLF)
	;ConsoleWrite("$m2SecondMsgTime[3]="&$m2SecondMsgTime[3]&@CRLF)
	;ConsoleWrite("$m2FirstMsgTime[3]="&$m2FirstMsgTime[3]&@CRLF)
	ConsoleWrite("$Time1="&$Time1&@CRLF)
	ConsoleWrite("$Time2="&$Time2&@CRLF)
	ConsoleWrite("$Time3="&$Time3&@CRLF)
	ConsoleWrite("$Time4="&$Time4&@CRLF)

	if  $m1_Key_Rx1=1 and $m1_Key_Tx1=1 and $m2_Key_Tx1=1 and $m2_Key_Rx1=1 Then
		$VerifyMsgKey=1
	EndIf

	if $m1_dbMsg_Rx1=1 and $m1_dbMsg_Tx1=1 and $m2_dbMsg_Tx2=1 and $m2_dbMsg_Rx2=1 Then
		$VerifyMsgRep=1
	EndIf

	;if ($m1SecondMsgTime[3]-$m1FirstMsgTime[3])=2 and ($m2SecondMsgTime[3]-$m2FirstMsgTime[3])=2  Then
	;	$Timedelay =1
	;EndIf

	if ($Time3-$Time1)=2 and ($Time4-$Time2)=2  Then
		$Timedelay =1
	EndIf


EndIf
	ConsoleWrite("$Timedelay= " & $Timedelay & @CRLF)
	ConsoleWrite("$VerifyMsgKey= " & $VerifyMsgKey& @CRLF)
	ConsoleWrite("$VerifyMsgRep= " & $VerifyMsgRep& @CRLF)


	If($Timedelay=1 And $VerifyMsgKey=1 And $VerifyMsgRep=1 ) Then
		_WriteResult("Pass","TS_TxWin_CAN_03")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_03")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_03")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)