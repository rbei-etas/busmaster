;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_01
; TestCases			:		Message transmission with Repetition.
; Test Data			:
; Test Setup		:		1.Open CAN Tx Window .
							;2.Add messages in Message List with the following Characteristics,

							;scenario -
							;1. Name: Message Name.
							;2. Repetition : 1000ms.
							;3. Key: unchecked
							;4. Connect  Busmaster and start sending messages.
							;5. observe the message window.

; Expected Results  : 		1. Each message will be transmitted with a delay of 1000ms from the Tx Frame List.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_01****"&@CRLF)

Local $Timedelay=0,$VerifyMsg=1
Local $m1_dbMsg_Rx1=0,$m1_dbMsg_Tx1=0,$m2_dbMsg_Tx1=0,$m2_dbMsg_Rx1=0
Local $m1_dbMsg_Rx2=0,$m1_dbMsg_Tx2=0,$m2_dbMsg_Tx2=0,$m2_dbMsg_Rx2=0
Local $Time1=0,$Time2=0,$Time3=0,$Time4=0
$countRowChckBox=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then

    _loadConfig("TS_TxWin_CAN_01")
	;_CANDriverSelectionXML()
	;_DisableOverwriteMode()
	sleep(500)
	_TxMsgMenu()																				; Select CAN->Transmit->Configure menu
     sleep(1000)
	_ConfigTXWinDetails(0,"db",1,1,"","","",1000,"")											;$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	_ConfigTXWinDetails(1,"db",2,1,"","","",1000,"")											;$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key


	_CloseTxWindowArg("CAN")																	; Close Tx window
	Sleep(1000)
	;_DisableOverwriteMode()																    ; Disable overwrite Mode
	;_DisableOverwriteModeAll($CANMenu)
	Sleep(1000)
	_ConnectDisconnect()																		; Connect CAN
	sleep(1000)
	_ConnectDisconnect()																		; DisConnect CAN

	$rCount=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	If $rCount=28 Then

		;$Data1=_GetMsgWinCANInfo(0)                                                             ;Fetch messages from message window
	    ;$Data2=_GetMsgWinCANInfo(1)
	   ;

	    For $i=0 to 3                                                                            ;Write messages with all information like(time,Channel,ID) to console
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
		For $i=4 to 7
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

		if ($Time3-$Time1)=1 and ($Time4-$Time2)=1  Then
			$Timedelay =1
		EndIf
	EndIf





	ConsoleWrite("$m1_dbMsg_Rx1="&$m1_dbMsg_Rx1&@CRLF)
	ConsoleWrite("$m1_dbMsg_Tx1="&$m1_dbMsg_Tx1&@CRLF)
	ConsoleWrite("$m2_dbMsg_Tx1="&$m2_dbMsg_Tx1&@CRLF)
	ConsoleWrite("$m2_dbMsg_Rx1="&$m2_dbMsg_Rx1&@CRLF)

	ConsoleWrite("$m1_dbMsg_Rx2="&$m1_dbMsg_Rx2&@CRLF)
	ConsoleWrite("$m1_dbMsg_Tx2="&$m1_dbMsg_Tx2&@CRLF)
	ConsoleWrite("$m2_dbMsg_Tx2="&$m2_dbMsg_Tx2&@CRLF)
	ConsoleWrite("$m2_dbMsg_Rx2="&$m2_dbMsg_Rx2&@CRLF)

	ConsoleWrite("$Time1="&$Time1&@CRLF)
	ConsoleWrite("$Time2="&$Time2&@CRLF)
	ConsoleWrite("$Time3="&$Time3&@CRLF)
	ConsoleWrite("$Time4="&$Time4&@CRLF)

	if $m1_dbMsg_Rx1=1 and $m1_dbMsg_Tx1=1 and $m2_dbMsg_Tx2=1 and $m2_dbMsg_Rx2=1 Then
		$VerifyMsg=1
	EndIf


EndIf
	ConsoleWrite("$Timedelay= " & $Timedelay & @CRLF)
	ConsoleWrite("$VerifyMsg= " & $VerifyMsg& @CRLF)


	If($Timedelay=1 And $VerifyMsg=1 ) Then
		_WriteResult("Pass","TS_TxWin_CAN_01")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_01")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_01")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)