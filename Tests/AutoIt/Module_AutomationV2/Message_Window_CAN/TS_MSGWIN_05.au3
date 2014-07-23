;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_05
; TestCases			:	    Selection of Overwrite mode.
; Test Data			:
; Test Setup		:		Go to menu "Functions-> Message Window->" sub  menu and click on the "Overwrite" menu item.

; Expected Results  : 		The application will now enter the "Overwrite" mode.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_05.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_05")															; Load Configuration
	_EnableOverwriteMode()
	Sleep(1000)
	Send("!CM")																				; Select CAN->Message Window from the menu
	sleep(500)
	send("{ESC}")
	sleep(500)
	send("{ESC}")
	sleep(500)
	If winexists($WIN_CANMsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									; Fetch the handle of CAN->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$MenuTxt=_GUICtrlMenu_GetItemText($hSubmenu, 6)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,6)
		ConsoleWrite("$MenuTxt : "&$MenuTxt& @CRLF)
		ConsoleWrite("$res : "&$res& @CRLF)
		If($res=True) Then
			_WriteResult("Pass","TS_MSGWIN_05")
		Else
			_WriteResult("Fail","TS_MSGWIN_05")
		EndIf
	Else
		$winhWnd1 = WinGetHandle($WIN_BUSMASTER)											; Fetch the window handle
		$hMain1 = _GUICtrlMenu_GetMenu($winhWnd1)											; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain1& @CRLF)
		$hFile1 = _GUICtrlMenu_GetItemSubMenu($hMain1, 1)									; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile1& @CRLF)
		$hSubmenu1=_GUICtrlMenu_GetItemSubMenu($hFile1, 9)									; Fetch the handle of CAN->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu1& @CRLF)
		$MenuTxt=_GUICtrlMenu_GetItemText($hSubmenu1, 6)
		$res1=_GUICtrlMenu_GetItemChecked($hSubmenu1,6)
		ConsoleWrite("$MenuTxt : "&$MenuTxt& @CRLF)
		ConsoleWrite("$res1 : "&$res1& @CRLF)
        If($res1=True) Then
			_WriteResult("Pass","TS_MSGWIN_05")
		Else
	       _WriteResult("Fail","TS_MSGWIN_05")
	   EndIf
   EndIf

EndIf

$isAppNotRes=_CloseApp()																	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_05")
EndIf



ConsoleWrite("****End : TS_MSGWIN_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)



