;==========================================================================Test Procedure =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_011
; TestCases			:		Message interpretation
; Test Data			:
; Test Setup		:		1. Transmit messages which are present in the database and currently associated to configuration.
;                           2. Double click on a particular message in message window.
;                           3. While the 'Message Interpretation' dialog is visible, click on any other interpretable message in the message window.
;                           4. Close the 'Message Interpretation' dialog.
;                           5. Expand few messages by clicking on '+' sign in message window.
;                           6.Dissociate all databases from current configuration.
;                           7. Observe the message window contents.


; Expected Results  : 		 All the messages in expanded state will be collapsed and all interpretable messages will loose the '+', '-' symbol in the first column.
;                            2. The 'interpret' option in message window attributes will be disabled.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_011.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_01")															 ; Load Configuration
	_ConnectDisconnect()
	sleep(1000)
	_TransmitMsgsMenu()	                                                                     ;Transmit Data
	sleep(3000)
	_ConnectDisconnect()
	;_EnableInterpretMode()
     Sleep(1000)
	 WinActivate($WIN_CANMsgWind)
	 Sleep(500)
	$HWD=_GetCANMsgWinHWD()                                                                 ;Get handle for CAN message window
	Sleep(1000)
	ControlFocus($WIN_BUSMASTER,"",$LSTC_CANMsgWin)
	$count1= _GetCANMsgWinItemCount()                                                       ;Get message count before expansion
	Sleep(1000)
	_GUICtrlListView_ClickItem($HWD,2)                                                      ;Expand second message
	Send("{ENTER}")
	sleep(1000)
	_GUICtrlListView_ClickItem($HWD,4)                                                     ;Expand message in fourth row
	Send("{ENTER}")
	sleep(1000)
	$count2=_GetCANMsgWinItemCount()                                                       ;Count after expanding messages
	Sleep(1000)
	 _DissociateCANDB(0)
	Sleep(1000)
	If WinWait($WIN_BUSMASTER,"",5) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_disDBtxt)
	EndIf
	Sleep(500)
	WinClose($WIN_Dissociate_CANDB)
	Sleep(500)
	$count3=_GetCANMsgWinItemCount()
	ConsoleWrite("$count1" & $count1 & @CRLF)
	ConsoleWrite("$count2" & $count2 & @CRLF)
	ConsoleWrite("$count3" & $count3 & @CRLF)

	If winexists($WIN_CANMsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)
		$res=_GUICtrlMenu_GetItemGrayed($hSubmenu,7)                                        ;Check whether Interpret menu item is disabled or no
	Else
		$winhWnd1 = WinGetHandle($WIN_BUSMASTER)
		$hMain1 = _GUICtrlMenu_GetMenu($winhWnd1)											; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain1& @CRLF)
		$hFile1 = _GUICtrlMenu_GetItemSubMenu($hMain1, 1)									; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile1& @CRLF)
		$hSubmenu1=_GUICtrlMenu_GetItemSubMenu($hFile1, 9)									; Fetch the handle of CAN->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu1& @CRLF)
		$res=_GUICtrlMenu_GetItemGrayed($hSubmenu1,7)                                       ;Check whether Interpret menu item is disabled or no
	EndIf
     ConsoleWrite("$res"& $res & @CRLF)
	If($count1=4 And $count2=6 And $count3=4 And $res=False) Then
		_WriteResult("Pass","TS_MSGWIN_11")
	Else
	   _WriteResult("Fail","TS_MSGWIN_11")
	EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_11")
EndIf

ConsoleWrite("****End : TS_MSGWIN_011.au3****"&@CRLF)
ConsoleWrite(@CRLF)





