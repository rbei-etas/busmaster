;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_10
; TestCases			:		Value assignement for the database messages
; Test Data			:
; Test Setup		:

;~ 							1. Select any database message from the combo box.
;~							2. Select any signal and  double click the RAW value or Physical value filed.
;~ 							3. Change the value in the spin control and click on outside of the list control.
;~ 							4. observe the values in physical filed and data byte filed


; 							Expected Results  :1. After step 2 a spin control will appear in the selected field.
;							2.After step 3 the physical value of the signal will change according to the equation defined in database.
;							The data byte filed will change according to the RAW value.

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_10.au3****"&@CRLF)

Local $VerifyPhysicalValue=0
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_CAN_01.cfx")
		Sleep(1000)
		_TxMsgMenu()
		sleep(1000)

		Sleep(1000)
		_ConfigTXWinDetails(0,"db",1,1,"","","","1000","")												; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
		_ConfigTXWinDetails(1,"db",2,1,"","","","1000","")												; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key

		Sleep(1000)
		$hWnd =ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:2]")					 ; Fetch the handle of Signal Details
		$a=_GetRowPosTxWin($hWnd,0)
	    ControlClick($WIN_BUSMASTER,"",$hWnd,"Left",2,$a[0]+200,$a[1])                    				 ; Double Click on Physical value
		sleep(500)
		send("{UP 2}")
		sleep(1000)
		send("{ENTER}")
		_CloseTxWindowArg("CAN")
		Sleep(500)
		_ConnectDisconnect()																		       ; Connect the tool
		 Sleep(1000)
		_ConnectDisconnect()																		       ; Disconnect the tool
		_DisableOverwriteMode()                                                                            ; Disable overwrite mode
		$rCount=_GetCANMsgWinItemCount()                                                                   ; Fetch the no of entries in message window
		If $rCount>=4 Then
			$Data1=_GetMsgWinCANInfo(0)                                                                    ; Fetch messages from message window
			$Data2=_GetMsgWinCANInfo(1)
			$Data3=_GetMsgWinCANInfo(2)
			$Data4=_GetMsgWinCANInfo(3)

			ConsoleWrite("$Data1[7]="&$Data1[7]&@CRLF)
			ConsoleWrite(" $Data3[7]="& $Data3[7]&@CRLF)
			ConsoleWrite("$Data4[7]="&$Data4[7]&@CRLF)

			If($Data1[7]="0000000000000000" And $Data3[7]="02000000000000" And $Data4[7]="02000000000000") Then   ; Check data byte value for second message to cross verify for the raw value which got updated when																									;when physical value is changed
				$VerifyPhysicalValue =1
			EndIf

		EndIf


EndIf
ConsoleWrite("$VerifyPhysicalValue="&$VerifyPhysicalValue)
If $VerifyPhysicalValue=1 Then   																			; Check data byte value for second message to cross verify for the raw value which got updated when																									;when physical value is changed
		_WriteResult("Pass","TS_TxWin_CAN_10")
Else
		_WriteResult("Fail","TS_TxWin_CAN_10")
EndIf

$isAppNotRes=_CloseApp()														                         	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_10")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_10.au3****"&@CRLF)
ConsoleWrite(@CRLF)