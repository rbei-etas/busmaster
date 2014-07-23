;==========================================================================Test Procedure =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_019
; TestCases			:		Archieving message window attributes.
; Test Data			:
; Test Setup		:		1. Continue from previous test case.
;                           2. Make sure a database is associated to current configuration.
;                           3. Enable overwrite mode and interpret mode.
;                           4. Save configuration and close the application.
;                           5. Start application and make sure the previous configuration is loaded.
;                           6. Check status of overwrite and interpret options.
;                           7. Disable overwrite mode and interpret mode.
;                           8. Save configuration and close the application.
;                           9. Start application and make sure the previous configuration is loaded.
;                           10. Again Check status of overwrite and interpret options.



; Expected Results  : 		1.After step6, The 'overwrite' mode and 'interpret' mode should be in enabled state.
;                          2.After step10, The 'overwrite' mode and 'interpret' mode should be in disabled state.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_019.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_19.cfx")
	  _EnableInterpretMode()
	  WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								; Select File->Exit from menu
	sleep(1000)															;
	if WinWaitActive($WIN_BUSMASTER,$SaveChangesConfigTXT,2) Then						; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,$SaveChangesConfigTXT,$BTN_Yes_SaveConfig)			; Click on Yes button

		ConsoleWrite("Application Closed through menu"&@CRLF)
	EndIf
	Sleep(1000)

	_launchApp()
	Sleep(1000)
	$winhWnd = WinGetHandle($WIN_BUSMASTER)                                                     ; Fetch the window handle
	$hMain = _GUICtrlMenu_GetMenu($winhWnd)                                                     ; Fetch the handle of the menu
	ConsoleWrite("menu handle : "&$hMain& @CRLF)
	If winexists($WIN_CANMsgWind) Then
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
	Else
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)
	EndIf
	$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)                                     ; Fetch the handle of CAN->Msg Window menu
	$res_overwrite=_GUICtrlMenu_GetItemChecked($hSubmenu,6)                             ;Check whether overwrite menu item is disabled or no
	$res_Interpret=_GUICtrlMenu_GetItemChecked($hSubmenu,7)                             ;Check whether overwrite menu item is disabled or no
	ConsoleWrite("$res_overwrite: " & $res_overwrite & @CRLF)
	ConsoleWrite("$res_Interpret" & $res_Interpret & @CRLF)
	Sleep(1000)
	_DisableInterpretMode()
	Sleep(500)
	_DisableOverwriteMode()
	 WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								; Select File->Exit from menu
	sleep(2000)															;
	if WinWaitActive($WIN_BUSMASTER,$SaveChangesConfigTXT,2) Then						; wait for save configuration dialog
		ControlClick($WIN_BUSMASTER,$SaveChangesConfigTXT,$BTN_Yes_SaveConfig)			; Click on No button

		ConsoleWrite("Application Closed through menu"&@CRLF)
	EndIf
	Sleep(1000)
	_launchApp()
	$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)
	$res_overwrite1=_GUICtrlMenu_GetItemChecked($hSubmenu,6)                             ;Check whether overwrite menu item is disabled or no
	$res_Interpret1=_GUICtrlMenu_GetItemChecked($hSubmenu,7)                             ;Check whether overwrite menu item is disabled or no
	ConsoleWrite("$res_overwrite1: " & $res_overwrite1 & @CRLF)
	ConsoleWrite("$res_Interpret1" & $res_Interpret1 & @CRLF)
    If($res_overwrite=True And $res_overwrite1=False And $res_Interpret=True And $res_Interpret1=False) Then
		_WriteResult("Pass","TS_MSGWIN_19")
	Else
	   _WriteResult("Fail","TS_MSGWIN_19")
	EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_19")
EndIf

ConsoleWrite("****End : TS_MSGWIN_019.au3****"&@CRLF)
ConsoleWrite(@CRLF)



