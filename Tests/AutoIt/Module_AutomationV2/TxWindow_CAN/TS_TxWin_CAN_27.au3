;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_27
; TestCases			:		Send single Message with Changing channel
; Test Data			:
; Test Setup		:

;~ 							1. Continue from the previous test case.
;							2. Change the channel number of the selected message entry.
;							3. Select "send Message" button to transmit the selected message to selected channel.
;							4. Check the message window for transmitted message entry and check the channel number.

; 							Expected Results  :1. After step 4, message window will show the transmitted message and the channel ID will exactly match with the user selected ID

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_27au3****"&@CRLF)

Local $VerifyCHNLBFRupdate=0,$VerifyCHNLAFTRupdate=0
Local $dbMsg1_Rx=0,$dbMsg1_Tx=0,$dbMsg_ChnlUpdate_Rx=0,$dbMsg_ChnlUpdate_Tx=0

_launchApp()
WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) Then
	_loadConfig("TS_TxWin_CAN_26.cfx")
	Sleep(500)


	_ConnectDisconnect()																				; Connect Tool
	sleep(1000)
	_ConnectDisconnect()																				; DisConnect Tool
	$rCount=_GetCANMsgWinItemCount()                                                           			; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)

	if $rCount=2 Then
																											; Write messages with all information like(time,Channel,ID) to console
		For $i=0 to 1                                                                            		;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 7
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0102030405060708" Then                                      	;Verify for Tx/RX
				$dbMsg1_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0102030405060708" Then                                      	;Verify for Tx/RX
				$dbMsg1_Tx=1
			EndIf

		Next
		ConsoleWrite("$dbMsg1_Rx="&$dbMsg1_Rx&@CRLF)
		ConsoleWrite("$dbMsg1_Tx="&$dbMsg1_Tx&@CRLF)
		If $dbMsg1_Rx=1 And $dbMsg1_Tx=1 Then                                      						;Verify for Tx/RX
			$VerifyCHNLBFRupdate=1
		EndIf


	EndIf
	_TxMsgMenu()																						; open Tx Window -CAN
	sleep(500)
	_ConfigTXWinDetails(0,"","",2,"","","","","")														; Config Tx Window.E.g-$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
	_SendMsgTxWindow()
	_CloseTxWindowArg("CAN")																			; Close Tx Window

	sleep(1000)
	_ConnectDisconnect()																				; Connect Tool
	sleep(1000)
	_ConnectDisconnect()																				; DisConnect Tool

	$rCount=_GetCANMsgWinItemCount()                                                           			; Fetch the no of items from message window
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	if $rCount=2 Then
		For $i=0 to 1                                                                           			;Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo($i)
			for $j=0 to 7
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
			Next
			If $Data[1]="Rx" And $Data[2]=1 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0102030405060708" Then                                      	;Verify for Tx/RX
				$dbMsg_ChnlUpdate_Rx=1
			EndIf
			If $Data[1]="Tx" And $Data[2]=2 And $Data[3]="s" And $Data[4]=0x015 And $Data[5]="m1" And $Data[6]=8 And $Data[7]="0102030405060708" Then                                      	;Verify for Tx/RX
				$dbMsg_ChnlUpdate_Tx=1
			EndIf
		Next
	EndIf
	ConsoleWrite("$dbMsg_ChnlUpdate_Rx="&$dbMsg_ChnlUpdate_Rx&@CRLF)
	ConsoleWrite("$dbMsg_ChnlUpdate_Tx="&$dbMsg_ChnlUpdate_Tx&@CRLF)


	If $dbMsg_ChnlUpdate_Rx=1 And $dbMsg_ChnlUpdate_Tx=1 Then                                      	;Verify for Tx/RX
		$VerifyCHNLAFTRupdate=1
	EndIf
EndIf

ConsoleWrite("$VerifyCHNLBFRupdate="&$VerifyCHNLBFRupdate&@CRLF)
ConsoleWrite("$VerifyCHNLAFTRupdate="&$VerifyCHNLAFTRupdate&@CRLF)




If $VerifyCHNLBFRupdate=1 And $VerifyCHNLAFTRupdate=1 Then   														; Verify Chanel updated																							;when physical value is changed
		_WriteResult("Pass","TS_TxWin_CAN_27")
Else
		_WriteResult("Fail","TS_TxWin_CAN_27")
EndIf

$isAppNotRes=_CloseApp()														                         	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_27")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_27.au3****"&@CRLF)
ConsoleWrite(@CRLF)