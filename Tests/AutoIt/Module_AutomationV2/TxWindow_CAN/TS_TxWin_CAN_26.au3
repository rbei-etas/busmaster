;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_26
; TestCases			:		Message Type information
; Test Data			:
; Test Setup		:

;~ 							1. Select a message from the message lIst.
;							2. Check the message type in Message detils view.
;							3. Change message type using combo box  std, Ext and Connect Busmaster


; 							Expected Results  :1.After step the Details view will show the currently assigned message type
;												2. After step 3 Message detail view will show the changed message type and the same will be shown in Message Window

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_26.au3****"&@CRLF)

Local $VerifyMsgTypeBFR=0,$VerifyMsgTypeAFTR=0
$countRowChckBox=0


_launchApp()
WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_CAN_26.cfx")
		Sleep(500)
		;_TxMsgMenu()																						; open Tx Window -CAN
		;sleep(500)
		_ConnectDisconnect()																				; Connect Tool
		sleep(1000)
		_ConnectDisconnect()																				; DisConnect Tool
		$rCount=_GetCANMsgWinItemCount()                                                           			; Fetch the no of items from message window
		ConsoleWrite("$rCount="&$rCount&@CRLF)

		if $rCount=2 Then
																											; Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo(0)
			$Data1=_GetMsgWinCANInfo(1)

			for $j=0 to 1
				ConsoleWrite("$Data="&$Data[$j]&@CRLF)
				ConsoleWrite("$Data1="&$Data1[$j]&@CRLF)
			Next
			If $Data[3]="s" and  $Data1[3]="s"  Then                                      					; Verify updated databytes in Message Window
					$VerifyMsgTypeBFR=1
			EndIf

		EndIf
		_TxMsgMenu()																						; open Tx Window -CAN
		sleep(500)
		_ConfigTXWinDetails(0,"","",1,"",2,"","","")														; Config Tx Window.E.g-$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key

		_CloseTxWindowArg("CAN")																			; Close Tx Window
		_ConnectDisconnect()																				; connect Tool
		sleep(1000)
		_ConnectDisconnect()																				; Disconnect Tool

		$rCount=_GetCANMsgWinItemCount()                                                           			; Fetch the no of items from message window
		ConsoleWrite("$rCount="&$rCount&@CRLF)

		if $rCount=2 Then
																											; Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo(0)
			$Data1=_GetMsgWinCANInfo(1)

			for $j=0 to 1
				ConsoleWrite("$Data="&$Data[$j]&@CRLF)
				ConsoleWrite("$Data1="&$Data1[$j]&@CRLF)
			Next
			If $Data[3]="x" and  $Data1[3]="x"  Then                                      				; Verify updated databytes in Message Window
					$VerifyMsgTypeAFTR=1
			EndIf

		EndIf
EndIf

If $VerifyMsgTypeBFR=1 And $VerifyMsgTypeAFTR=1 Then   																			; Check data byte value for second message to cross verify for the raw value which got updated when																									;when physical value is changed
		_WriteResult("Pass","TS_TxWin_CAN_26")
Else
		_WriteResult("Fail","TS_TxWin_CAN_26")
EndIf

$isAppNotRes=_CloseApp()														                         	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_26")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_26.au3****"&@CRLF)
ConsoleWrite(@CRLF)