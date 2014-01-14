;==========================================================================Test Procedure =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_014
; TestCases			:		Message Transmission
; Test Data			:
; Test Setup		:		1. Continue from above test case.
;                           2. Select the 'Send' option in popup menu.
;                           3. Observe the message window.

; Expected Results  : 		The selected message should be transmitted and new entry for the same message should be appended in message window list.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_014.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_01")
    _DeleteMsgBlock()
	sleep(1000)
     _EnableInterpretMode()
      Sleep(500)
	WinActivate($WIN_CANMsgWind)
	 Sleep(500)
	$HWD=_GetCANMsgWinHWD()
   _ConfigCANTXBlockDetails("Monoshot","No","","No","","","Yes",2000)								      ; Configure TX block details

	_AddMsg2TxList(0)																			      ; Add the first msg to Tx list
	 Sleep(500)
	_AddMsg2TxList(1)
	_CloseTxWindow()
	_ConnectDisconnect()																		     ; Connect the tool

	_TransmitMsgsMenu()																			     ; Transmit normal blocks
	Sleep(4000)
	$count1= _GetCANMsgWinItemCount()
	Sleep(500)
	_DisableOverwriteMode()
	_GUICtrlListView_ClickItem($HWD,2,"right")                                              ;send third message by right clik
	Sleep(500)
	Send("{DOWN}")                                                                          ;Press down arrow key
	Send("{DOWN}")                                                                          ;Press down arrow key
	Send("{DOWN}")
	Send("{ENTER}")
	$count2= _GetCANMsgWinItemCount()
	ConsoleWrite("$count1" & $count1 & @CRLF)
	ConsoleWrite("$count2" & $count2 & @CRLF)
	If($count1=4 And $count2=6) Then
		_WriteResult("Pass","TS_MSGWIN_14")
	Else
	   _WriteResult("Fail","TS_MSGWIN_14")
	EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_14")
EndIf

ConsoleWrite("****End : TS_MSGWIN_014.au3****"&@CRLF)
ConsoleWrite(@CRLF)



