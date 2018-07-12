;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_MSGWIN_04
; TestCases			:		Message Window Overwrite/Append Mode
; Test Data			:
; Test Setup		:		1.Go to menu "LIN-> Messsage Window->Overwrite.
							;2.Now Click on the "Overwrite" menu item.

; Expected Results  : 		After step1 If "Overwrite" menu option is in unchecked state it signifies Append mode.
							;2.After step2, OverWrite option is enabled or disabled depending upon the previous state.
;==========================================================================Test Procedure =========================================================================
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_MSGWIN_04.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINMsgWin_06")															; Load Configuration
	;_linDriverSelection()
	_linDriverSelection()

	If winexists($LSTC_LINMsgWin) Then
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
			_WriteResult("Pass","TS_LIN_MSGWIN_04")
			ConsoleWrite("PASS")
		Else
	       _WriteResult("Fail","TS_LIN_MSGWIN_04")
		   ConsoleWrite("FAIL")
	   EndIf
   EndIf
EndIf
Sleep(1000)
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_04")
	ConsoleWrite("WARNING")

EndIf



ConsoleWrite("****End : TS_LIN_MSGWIN_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)




