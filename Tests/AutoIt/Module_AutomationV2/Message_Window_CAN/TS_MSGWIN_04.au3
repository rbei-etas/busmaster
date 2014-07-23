;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_04
; TestCases			:		Message Window Overwrite/Append Mode
; Test Data			:
; Test Setup		:		Go to menu "Functions-> Message Window->" sub  menu.

; Expected Results  : 		The "Overwrite" menu option should be in unchecked state which signifies Append mode.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_04.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_01")																					; Load Configuration
	If winexists($WIN_CANMsgWind) Then
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									; Fetch the handle of CAN->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
		$res=_GUICtrlMenu_GetItemChecked($hSubmenu,6)
		If($res=True) Then
			_WriteResult("Pass","TS_MSGWIN_04")
		Else
	       _WriteResult("Fail","TS_MSGWIN_04")
		EndIf
	Else
		$winhWnd1 = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain1 = _GUICtrlMenu_GetMenu($winhWnd1)												; Fetch the handle of the menu
		ConsoleWrite("menu handle : "&$hMain1& @CRLF)
		$hFile1 = _GUICtrlMenu_GetItemSubMenu($hMain1, 1)										; Fetch the handle of CAN menu
		ConsoleWrite("Submenu handle : "&$hFile1& @CRLF)
		$hSubmenu1=_GUICtrlMenu_GetItemSubMenu($hFile1, 9)									; Fetch the handle of CAN->Msg Window menu
		ConsoleWrite("$hSubmenu handle : "&$hSubmenu1& @CRLF)
		$res1=_GUICtrlMenu_GetItemChecked($hSubmenu1,6)
        If($res1=True) Then
			_WriteResult("Pass","TS_MSGWIN_04")
		Else
	       _WriteResult("Fail","TS_MSGWIN_04")
	   EndIf
   EndIf
EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_04")
EndIf



ConsoleWrite("****End : TS_MSGWIN_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)




