;==========================================================================Test Procedure =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_025
; TestCases			:		Message window display configuration.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;                           2. Dissociate all the databases from the current configuration.
;                           3. Open message display configuration dialog by menu option 'Configure -> Message display'.
;                           4. Click on 'DB' tab in 'Configure message display - CAN' dialog and observe the contents.


; Expected Results  : 		The list box should be empty.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_025.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_25.cfx")
	WinMenuSelectItem($WIN_BUSMASTER,"",$CANMenu,$CANMsgWinMenu,$CANMsgDispConfigMenu)
      Sleep(1000)
	$hWndConfigMsgDisp=controlgethandle($WIN_MsgDisplay,"",$TabConfigMsgDisp)                    ;Get handle for configure message display
	_GUICtrlTab_ClickTab($hWndConfigMsgDisp, 1)
	$handle=ControlGetHandle($WIN_MsgDisplay,"",$LSTDBMsgs)                                      ;Get handle for DB message list view
	$count=_GUICtrlListView_GetItemCount($handle)
	ConsoleWrite("Db message count" & $count & @CRLF)
	If($count=0) Then
		_WriteResult("Pass","TS_MSGWIN_25")
	Else
	   _WriteResult("Fail","TS_MSGWIN_25")
	EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_25")
EndIf

ConsoleWrite("****End : TS_MSGWIN_025.au3****"&@CRLF)
ConsoleWrite(@CRLF)



