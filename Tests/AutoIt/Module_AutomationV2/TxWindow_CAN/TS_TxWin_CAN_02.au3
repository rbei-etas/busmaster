;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_02
; TestCases			:		Message transmission with Key event Trigger.
; Test Data			:
; Test Setup		:		1.Open CAN Tx Window .
;							2.Add messages  in Message List with the following Characteristics,

;							scenario -
;							1. Name: Message Name.
;							2. Repetition : Unchecked
;							3. Key: checked
;							4. Provide Key Name: a
;							5. Connect  Busmaster and start sending messages.
;							6. observe the message window.

; Expected Results  : 		1. Each messages  wii be transmitted on the click of key 'a'.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_02****"&@CRLF)

Local $m1_dbMsg_Rx=0,$m1_dbMsg_Tx=0,$100_ndbMsg_Rx=0,$100_ndbMsg_Tx=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then

    _loadConfig("TS_TxWin_CAN_01")
	sleep(1000)
	;_CANDriverSelectionXML()
	sleep(1000)
	_TxMsgMenu()																				; Select CAN->Transmit->Configure menu
     sleep(1000)
	_ConfigTXWinDetails(0,"db",1,1,"","","No","","a")											; config Tx Window Details -$Rownum,$HSCaseNo(1-dbmessage,2-NonDbmsg),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR,Repetition,Key
	_ConfigTXWinDetails(1,"ndb","100",1,"","","No","","a")										; $Rownum,$HSCaseNo(1-dbmessage,2-NonDbmsg),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR,Repetition,Key


	_CloseTxWindowArg("CAN")																	; Close Tx window
	_DisableOverwriteMode()																		; Enable overwrite Mode
	_ConnectDisconnect()																		; Connect CAN
	;sleep(1000)
	Send("a")
	_ConnectDisconnect()																		; DisConnect CAN
	$rCount=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	If $rCount=4 Then

	    For $i=0 to 3                                                                            ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 7
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                       ;Verify for Tx/RX
				$m1_dbMsg_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                       ;Verify for Tx/RX
				$m1_dbMsg_Tx=1
			EndIf
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x100 And $Data[5]=0x100 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                    ;Verify for Tx/RX
				$100_ndbMsg_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x100 And $Data[5]=0x100 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                     ;Verify for Tx/RX
				$100_ndbMsg_Tx=1
			EndIf
		next

	EndIf
	ConsoleWrite("$m1_dbMsg_Rx=" & $m1_dbMsg_Rx & @CRLF)
	ConsoleWrite("$m1_dbMsg_Tx=" & $m1_dbMsg_Tx& @CRLF)
	ConsoleWrite("$100_ndbMsg_Rx=" & $100_ndbMsg_Rx & @CRLF)
	ConsoleWrite("$100_ndbMsg_Tx=" & $100_ndbMsg_Tx & @CRLF)

	If($m1_dbMsg_Rx=1 And $m1_dbMsg_Tx=1 And $100_ndbMsg_Rx=1 And $100_ndbMsg_Tx=1) Then
		_WriteResult("Pass","TS_TxWin_CAN_02")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_02")
	EndIf

EndIf
	$isAppNotRes=_CloseApp()																								; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_02")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)