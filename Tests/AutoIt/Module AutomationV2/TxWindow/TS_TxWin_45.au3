;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_45
; TestCases			:		Message transmission without Time delay
; Test Data			:		TS_TxWin_01.cfx
; Test Setup		:	1. Open Tx window.
;                       2. Add few message block with few messages.
;                       3. Don’t check any option for time delay.
;                       4. Don’t select key trigger for messages also.
;                       5. Connect to the application and start transmission

; Expected Results  : 		Messages should not be transmitted.
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_45.au3****"&@CRLF)
    ;_launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_01.cfx")
		Sleep(1000)
		_TxMsgMenu()
		sleep(1000)
		_DeleteMsgBlock()
		Sleep(1000)
		_ConfigCANTXBlockDetails("Cyclic","Yes",1000,"No","","","No","")						         ; Configure TX block details
	    _AddMsg2TxList(0)
        _AddMsg2TxList(1)			                                                                      ; Add the first msg to Tx list
         sleep(1000)
		 ControlCommand($WIN_BUSMASTER,"",$CHKB_MsgDelay_ConfigTX,"UnCheck")                             ;Uncheck time delay between messages
         Sleep(1000)
		 ControlCommand($WIN_BUSMASTER,"",$CHKB_BlockDelay_ConfigTX,"UnCheck")                           ;Uncheck time delay between blocks
		  Sleep(1000)
		  _CloseTxWindow()																			    ; Close Tx window
		 sleep(500)
	   _ConnectDisconnect()																		        ; Connect the tool
	   _TransmitMsgsMenu()																			    ; Transmit normal blocks
	    Sleep(2500)
		 _ConnectDisconnect()
		 $rCount=_GetCANMsgWinItemCount()
		 ConsoleWrite("count" & $rCount & @CRLF)
		 If($rCount=0) Then
			 _WriteResult("Pass","TS_Tx_045")
		Else
		   _WriteResult("Fail","TS_Tx_045")
	    EndIf

EndIf
$isAppNotRes=_CloseApp()														                         ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_045")
EndIf

ConsoleWrite("****End : TS_TxWin_45.au3****"&@CRLF)
ConsoleWrite(@CRLF)