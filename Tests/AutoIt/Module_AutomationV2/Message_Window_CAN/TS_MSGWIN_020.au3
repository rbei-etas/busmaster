;==========================================================================Test Procedure =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_020
; TestCases			:		Archieving message window attributes
; Test Data			:
; Test Setup		:		1.Continue from previous test case.
;                           2. Choose 'Relative Time' in message window 'Absolute/Relative Time display' option.
;                           3. Save configuration and close the application.
;                           4. Start application and make sure the previous configuration is loaded.
;                           5. Check status of 'Absolute/Relative Time display' option

; Expected Results  : 		 The 'Absolute/Relative Time display' option should be set to 'Relative Time'.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_020.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_20.cfx")
	WinMenuSelectItem($WIN_BUSMASTER,"","&CAN","&Message Window","Time &Mode","&Relative Time")
	Sleep(500)
	 WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								; Select File->Exit from menu
	sleep(1000)															;
	if WinWaitActive($WIN_BUSMASTER,$SaveChangesConfigTXT,2) Then						; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,$SaveChangesConfigTXT,$BTN_Yes_SaveConfig)			; Click on Yes button

		ConsoleWrite("Application Closed through menu"&@CRLF)
	EndIf
	Sleep(1000)

	_launchApp()
	Sleep(1000)

	$winhWnd = WinGetHandle($WIN_BUSMASTER)                                              ; Fetch the window handle
	$hMain = _GUICtrlMenu_GetMenu($winhWnd)                                              ; Fetch the handle of the menu
	ConsoleWrite("menu handle : "&$hMain& @CRLF)
	If winexists($WIN_CANMsgWind) Then
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)								      ; Fetch the handle of CAN menu
	Else
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)
	EndIf
	$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)                                     ; Fetch the handle of CAN->Msg Window menu
	$hsubmenu_time=_GUICtrlMenu_GetItemSubMenu($hSubmenu,5)                              ; Fetch the handle of CAN->Msg Window menu->Time mode
	$text=_GUICtrlMenu_GetItemText($hSubmenu,5)
    $res=_GUICtrlMenu_GetItemChecked($hsubmenu_time,2)                                   ;Check whether Relative time mode is checked
	ConsoleWrite("$text" & $text & @CRLF)
	ConsoleWrite("$res" & $res & @CRLF)
	If($res=True) Then
		_WriteResult("Pass","TS_MSGWIN_20")
	Else
	   _WriteResult("Fail","TS_MSGWIN_20")
	EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_20")
EndIf

ConsoleWrite("****End : TS_MSGWIN_020.au3****"&@CRLF)
ConsoleWrite(@CRLF)






