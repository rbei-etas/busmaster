;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_06
; TestCases			:		Message Window Format.
; Test Data			:
; Test Setup		:		Start the BUSMASTER application, Activate J1939 using menu option "J1939-> Activate"

; Expected Results  : 		 J1939 Message window should appear in the front as an active window.
;                            2. Message window list should contain following columns by default - "Time, Channel, CAN ID, PGN, PGN Name, Type, Src, Dest, Priority, Tx/Rx, DLC, Databyte(s)"
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MsgWndJ1939_07.au3****"&@CRLF)

    _launchApp()
	WinActivate($WIN_BUSMASTER)
	Local $ColTime="",$Colch="",$ColCANid="",$ColPGN="",$ColPGNname="",$ColType="",$ColSrc="",$ColDest="",$ColPrio="",$ColDir="",$ColDLC="",$ColData=""
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfxMsgWin_J1939_07.cfx")
		 WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$DriverSelectMenu,"&Simulation")
		sleep(1000)
		 _ConnectDisconnect()
		 Sleep(500)
		_ActivateJ1939()                                                                                      ;Activate J1939 Message window
		 sleep(1000)
		 _DisableOverwritemode()
		 $count=_GetJ1939MsgWinItemCount()
		 ConsoleWrite("$count" & $count & @CRLF)
		 _ConnectDisconnect()
		 Sleep(1000)
		 _hdWareSelectKvaser()
		 Sleep(500)
		  _ConnectDisconnect()
		 Sleep(500)
		_ActivateJ1939()                                                                                      ;Activate J1939 Message window
		 sleep(1000)
		  _DisableOverwritemode()
		  $count1=_GetJ1939MsgWinItemCount()
		  ConsoleWrite("$count1" & $count1 & @CRLF)

		If($count=16 and $count1=4) Then
			 _WriteResult("Pass","TS_MsgWndJ1939_07")
         Else
	        _WriteResult("Fail","TS_MsgWndJ1939_07")
	    EndIf
EndIf
$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MsgWndJ1939_07")
EndIf


ConsoleWrite("****End : TS_MsgWndJ1939_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)




