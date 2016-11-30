;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_05
; TestCases			:		Transmission of messages for both database and non-database messages.[ Uisng Send from Tx Window ].
; Test Data			:
; Test Setup		:		Open CAN Tx Window .
;~							Add message with the following Characteristics,

;~ 							scenario 1-

;~ 							1. Add  message from  dropdown.
;~ 							2. Select the message row.
;~ 							3. Uncheck both repetition and key for the selected message.
;~ 							4. Connect  Busmaster and Click on Send message button from Tx Window.
;~ 							5. observe the message window.

;~ 							scenario 2-

;~ 							6. check repetion with value 5000 msec and Key ("a") of the messages.
;~ 							7. Click on Send message button from Tx Window.
;~ 							8. observe the message window.

; 							Expected Results  : 1.Aftre step 4, The selected message id will be transmitted and displayed in CAN message window.
;							2.After 7, The selected message id will be transmitted and displayed in CAN message window.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_05****"&@CRLF)

Local $dbMsg1_Rx=0,$dbMsg1_Tx=0,$dbMsg2_Rx=0,$dbMsg2_Tx=0
Local $ndbMsg1_Rx=0,$ndbMsg1_Tx=0,$ndbMsg2_Rx=0,$ndbMsg2_Tx=0,$ndbMsg1_Rx_Sc2=0,$ndbMsg1_Tx_Sc2=0
Local $VerifydbMsg=0,$VerifyndbMsg=0,$VerifydbMsgSendScn2=0
local $VerifydbMsgSendScn1=0,$VerifyndbMsgSendScn1=0,$dbMsg1_Rx_Sc2=0,$dbMsg1_Tx_Sc2=0,$VerifyndbMsgSendScn2=0
$countRowChckBox=0


_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0

if winexists($WIN_BUSMASTER) then

    _loadConfig("TS_TxWin_CAN_01")

	;_DisableOverwriteMode()																		; Disable overwrite Mode
	sleep(500)
	_TxMsgMenu()																				; Select CAN->Transmit->Configure menu
     sleep(1000)

	 ;----------------scenario 1 -DBMsg---------------------

	_ConfigTXWinDetails(0,"db",1,1,"","","","","")												; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	Send("{ENTER}")
	;_ConfigTXWinDetails(2,"ndb","100",1,"","","","","")										; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key

	;_DisableOverwriteMode()
;~ 	Opt("WinDetectHiddenText", 0)
;~ 	Opt("WinSearchChildren", 1)
;~ 	Opt("WinTitleMatchMode", 1)
;~ 	WinClose($WinCANTxWindow)

	WinActivate($WIN_BUSMASTER,"")
	_ConnectDisconnect()																		; Connect CAN
	sleep(1000)
	;_TxMsgMenu()
	_ActivatechildWindow($WinCANTxWindow)
	_SendMsgTxWindow()
	_ActivatechildWindow("Message Window - CAN")
	$rCount1=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	ConsoleWrite("$rCount1="&$rCount1&@CRLF)
	If $rCount1=2 Then

	    For $i=0 to 1                                                                            ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 7
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
	ConsoleWrite("$dbMsg1_Rx="&$dbMsg1_Rx&@CRLF)
	ConsoleWrite("$dbMsg1_Tx="&$dbMsg1_Tx&@CRLF)

	 ;----------------scenario 2 -DBMsg---------------------
	sleep(1000)
	_TxMsgMenu()
	;sleep(1000)
	;_ActivatechildWindow($WinCANTxWindow)
     $countRowChckBox=0
	_ConfigTXWinDetails(0,"",1,1,"","","","20000","a")											; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	_ClearMsgWindow()
	;sleep(1000)
	_ActivatechildWindow($WinCANTxWindow)
	_SendMsgTxWindow()
	;sleep(500)
	_ActivatechildWindow("Message Window - CAN")
	$rCount2=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	                                                           ; Fetch the no of items from message window
	ConsoleWrite("$rCount2="&$rCount2&@CRLF)
	If $rCount2=2 Then

	    For $i=0 to 1                                                                            ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 1
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$dbMsg1_Rx_Sc2=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$dbMsg1_Tx_Sc2=1
			EndIf

		Next

	EndIf
	ConsoleWrite("$dbMsg1_Rx_Sc2="&$dbMsg1_Rx_Sc2&@CRLF)
	ConsoleWrite("$dbMsg1_Tx_Sc2="&$dbMsg1_Tx_Sc2&@CRLF)

	if $dbMsg1_Rx_Sc2=1 and $dbMsg1_Tx_Sc2=1  Then
		$VerifydbMsgSendScn2=1
	EndIf

	 ;----------------scenario 1 -non DBMsg---------------------
	 sleep(200)
	 _ClearMsgWindow()
	 _ClearMsgWindow()
	_TxMsgMenu()
	;sleep(200)
	;_ActivatechildWindow($WinCANTxWindow)
	$countRowChckBox=0
	_ConfigTXWinDetails(0,"ndb","100",1,"","","","","")											; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	_ClearMsgWindow()
	;sleep(500)
	;WinActivate($WIN_BUSMASTER,"")
	 ;_ClearMsgWindow()
	_ActivatechildWindow($WinCANTxWindow)
	_SendMsgTxWindow()
	sleep(200)

	_ActivatechildWindow("Message Window - CAN")
	;_ConnectDisconnect()																		; DisConnect CAN
	$rCount3=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	ConsoleWrite("$rCount3="&$rCount3&@CRLF)
	If $rCount3=2 Then

	    For $i=0 to 1                                                                            ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 7
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x100 And $Data[5]=0x100 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg1_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x100 And $Data[5]=0x100 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg1_Tx=1
			EndIf

		Next

	EndIf



	ConsoleWrite("$ndbMsg1_Rx="&$ndbMsg1_Rx&@CRLF)
	ConsoleWrite("$ndbMsg1_Tx="&$ndbMsg1_Tx&@CRLF)

	if $dbMsg1_Rx=1 and $dbMsg1_Tx=1  Then
		$VerifydbMsgSendScn1=1
	EndIf

	if $ndbMsg1_Rx=1 and $ndbMsg1_Tx=1  Then
		$VerifyndbMsgSendScn1=1
	EndIf
	;----------------scenario 2 -non DBMsg---------------------
	 sleep(200)
	 _ClearMsgWindow()
	_TxMsgMenu()
	sleep(200)
	;_ActivatechildWindow($WinCANTxWindow)
	$countRowChckBox=0
	_ConfigTXWinDetails(0,"ndb","100",1,"","","","8000","a")											; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	_ClearMsgWindow()
	sleep(500)
	_SendMsgTxWindow()
	sleep(200)
	_ActivatechildWindow("Message Window - CAN")
	;_ConnectDisconnect()																		; DisConnect CAN
	$rCount4=_GetCANMsgWinItemCount()                                                            ; Fetch the no of items from message window
	ConsoleWrite("$rCount4="&$rCount4&@CRLF)
	_ConnectDisconnect()
	If $rCount4=2 Then

	    For $i=0 to 1                                                                            ;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 7
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x100 And $Data[5]=0x100 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg1_Rx_Sc2=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x100 And $Data[5]=0x100 And $Data[6]=8 And $Data[7]="0000000000000000" Then                                      	;Verify for Tx/RX
				$ndbMsg1_Tx_Sc2=1
			EndIf

		Next

	EndIf



	ConsoleWrite("$ndbMsg1_Rx_Sc2="&$ndbMsg1_Rx_Sc2&@CRLF)
	ConsoleWrite("$ndbMsg1_Tx_Sc2="&$ndbMsg1_Tx_Sc2&@CRLF)

	if $ndbMsg1_Rx_Sc2=1 and $ndbMsg1_Tx_Sc2=1  Then
		$VerifyndbMsgSendScn2=1
	EndIf


EndIf



	ConsoleWrite("$VerifydbMsgSendScn1="&$VerifydbMsgSendScn1&@CRLF)
	ConsoleWrite("$VerifydbMsgSendScn2="&$VerifydbMsgSendScn2&@CRLF)
	ConsoleWrite("$VerifyndbMsgSendScn1="&$VerifyndbMsgSendScn1&@CRLF)
	ConsoleWrite("$VerifyndbMsgSendScn2="&$VerifyndbMsgSendScn2&@CRLF)



	If($VerifydbMsgSendScn1=1 And $VerifydbMsgSendScn2=1 And $VerifyndbMsgSendScn1=1 And $VerifyndbMsgSendScn2=1) Then
		_WriteResult("Pass","TS_TxWin_CAN_05")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_05")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_05")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)