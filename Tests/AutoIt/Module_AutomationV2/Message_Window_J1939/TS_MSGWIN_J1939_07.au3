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

Local $ACLMsg[11]=["","","","","","","","","","",""]
if winexists($WIN_BUSMASTER) Then

	_loadConfig("cfxMsgWin_J1939_07.cfx")																; Load cfx

	_ActivateJ1939()                                                                                    ; Activate J1939 Message window

	_ConnectDisconnect()																				; Connect

	$count=_GetJ1939MsgWinItemCount()																	; Fetch the msg win item count
	ConsoleWrite("$count" & $count & @CRLF)

	_ConnectDisconnect()																				; Disconnect

	_DeactJ1939()																						; Deactivate J1939

	If $count=1 Then
		$ACLMsg=_GetJ1939MsgWinInfo(0)																	; Fetch the first item info
	EndIf

	If $ACLMsg[5]="ACL" Then
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




