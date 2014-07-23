;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_01
; TestCases			:		Messages Interpret status.
; Test Data			:
; Test Setup		:		1. Select "J1939-> Database->Associate" and select the appropriate database file.
;                           2. Make sure the application is in "Connected" state.
 ;                          3. Open J1939 Tx window using menu option "J1939 -> View -> Transmit Window".
;                           4. Switch to "Transport protocol Function" and start transmitting PGNs messages of type "Data" .

; Expected Results  : 		PGN messages that are transmitted and currently present in the associated database will be displayed with a "+" image next to them
;                           and messages not present in database will be displayed without any image next to them.
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MsgWndJ1939_05.au3****"&@CRLF)

_launchApp()																							; launch the application
WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) Then

	_loadConfig("cfxMsgWin_J1939_05.cfx")																; Load cfx

	_ActivateJ1939()                                                                                    ; Activate J1939 Message window

	_AssocJ1939DB("J1939DB.dbf")                                                                        ; Associate DB

	_ConnectDisconnect()																				; Connect

	_J1939tMsgWin("DATA","[0x400]XBR","","")                                                            ; Transmit Data

	_ConnectDisconnect()																				; Disconnect

	_EnableOverwriteMode()																				; Enable overwrite mode

;~ 	WinMenuSelectItem($WIN_BUSMASTER,"",$J1939Menu,$J1939MsgWinMenu,$InterpretMsgMenu)					; Enable interpret mode

	$HWD=_GetCANMsgWinHWD()																				; Fetch Msg window handle

	_GUICtrlListView_ClickItem($HWD,4)																	; Click on the 4th item in msg window
	Send("{ENTER}")																						; expand the msg
	sleep(1000)
	$count1= _GetJ1939MsgWinItemCount()
	ConsoleWrite("Count1: " & $count1 & @CRLF)
	sleep(1000)
	_GUICtrlListView_ClickItem($HWD,4)
	Send("{ENTER}")
	sleep(1000)
	$count2= _GetJ1939MsgWinItemCount()
	ConsoleWrite("Count2: " & $count2 & @CRLF)

	_DeactJ1939()																							; Deactivate J1939

	If($count1=7 And $count2=6) Then
		_WriteResult("Pass","TS_MsgWndJ1939_05")
	Else
	      _WriteResult("Fail","TS_MsgWndJ1939_05")
	EndIf
EndIf
$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MsgWndJ1939_05")
EndIf

ConsoleWrite("****End : TS_MsgWndJ1939_05****"&@CRLF)
ConsoleWrite(@CRLF)







