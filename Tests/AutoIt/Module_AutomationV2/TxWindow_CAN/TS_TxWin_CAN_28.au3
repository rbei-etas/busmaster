;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_28
; TestCases			:		Updating the Tx Message list while sending message list
; Test Data			:
; Test Setup		:		1. Add some messages to it.
;							2. Connect Busmaster and start sending messages.
;							3. Add some more message to Tx List.
;							4. Check repetion check box  for newly added messages.

; 							Expected Results  :1.After step2 messages will be added to the Message window.
;							1. After step4,The newly added message will be added to the Message window.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_28****"&@CRLF)

Local $ndbMsg1_Rx=0,$ndbMsg1_Tx=0,$ndbMsg2_Rx=0,$ndbMsg2_Tx=0,$ndbMsg2_Rx=0,$ndbMsg2_Tx=0,$ndbMsg3_Rx=0,$ndbMsg3_Tx=0
Local $VerifymsgBFRAddMsg=0,$VerifymsgAFTRAddMsg=0



_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then

    _loadConfig("TS_TxWin_CAN_01")
	sleep(500)
	_TxMsgMenu()																				; Select CAN->Transmit->Configure menu
     sleep(1000)


	;-----------1. Add some messages in Tx Window.-----------------------


	_ConfigTXWinDetails(0,"ndb","100",1,"","","","1000","")											; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	_ConfigTXWinDetails(1,"ndb","101",1,"","","","1000","")											; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key

	_EnableOverwriteMode()
	_ConnectDisconnect()																		; Connect CAN
	_ActivatechildWindow("Message Window - CAN")
	$rCount=_GetCANMsgWinItemCount()                                                           ; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)

	if $rCount=4 Then
		For $i=0 to 1                                                                            ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 1
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x100 And $Data[5]=0x100 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg1_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x100 And $Data[5]=0x100 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg1_Tx=1
			EndIf
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x101 And $Data[5]=0x101 And $Data[6]=7 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg2_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x101 And $Data[5]=0x101 And $Data[6]=7 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg2_Tx=1
			EndIf

		Next

	EndIf





	_TxMsgMenu()
	sleep(1000)

	;----------------3. Add some more message to Tx List.------------------------

	_ConfigTXWinDetails(2,"ndb","102",1,"","","","1000","")											; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	_ConfigTXWinDetails(3,"ndb","103",1,"","","","1000","")											; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	sleep(1000)
	_ActivatechildWindow("Message Window - CAN")
	$rCount=_GetCANMsgWinItemCount()                                                           ; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	_ConnectDisconnect()																		; DisConnect CAN
	_CloseTxWindowArg("CAN")
	sleep(1000)
	if $rCount=8 Then
		For $i=4 to 7                                                                            ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 1
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x102 And $Data[5]=0x102 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg2_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x102 And $Data[5]=0x102 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg2_Tx=1
			EndIf
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x103 And $Data[5]=0x103 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                  ;Verify for Tx/RX
				$ndbMsg3_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x103 And $Data[5]=0x103 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                   ;Verify for Tx/RX
				$ndbMsg3_Tx=1
			EndIf

		Next

	EndIf

	ConsoleWrite("$ndbMsg1_Rx="&$ndbMsg1_Rx&@CRLF)
	ConsoleWrite("$ndbMsg1_Tx="&$ndbMsg1_Tx&@CRLF)
	ConsoleWrite("$ndbMsg2_Rx="&$ndbMsg2_Rx&@CRLF)
	ConsoleWrite("$ndbMsg2_Tx="&$ndbMsg2_Tx&@CRLF)

	ConsoleWrite("$ndbMsg2_Rx="&$ndbMsg2_Rx&@CRLF)
	ConsoleWrite("$ndbMsg2_Tx="&$ndbMsg2_Tx&@CRLF)
	ConsoleWrite("$ndbMsg3_Rx="&$ndbMsg3_Rx&@CRLF)
	ConsoleWrite("$ndbMsg3_Tx="&$ndbMsg3_Tx&@CRLF)

	if $ndbMsg1_Rx=1 And $ndbMsg1_Tx=1 And $ndbMsg2_Rx=1 And $ndbMsg2_Tx=1 Then
		$VerifymsgBFRAddMsg=1
	EndIf

	if $ndbMsg2_Rx=1 And $ndbMsg2_Tx=1 And $ndbMsg3_Rx=1 And $ndbMsg3_Tx=1 Then
		$VerifymsgAFTRAddMsg=1
	EndIf

EndIf


	ConsoleWrite("$VerifymsgBFRAddMsg="&$VerifymsgBFRAddMsg&@CRLF)
	ConsoleWrite("$VerifymsgAFTRAddMsg="&$VerifymsgAFTRAddMsg&@CRLF)


	If($VerifymsgBFRAddMsg=1 And $VerifymsgAFTRAddMsg=1 ) Then
		_WriteResult("Pass","TS_TxWin_CAN_28")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_28")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_28")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_28.au3****"&@CRLF)
ConsoleWrite(@CRLF)