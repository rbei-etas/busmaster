;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_21
; TestCases			:       Toolbar Hiding through menu

; Test Setup                1. Open the  Busmaster.
;                           2. Select view->Toolbar.
;                           3. Select a toolbar that has to be hide.
;                           4. Check the menu status.
;                           5.Click on the menu which is corresponding to the hidden tool bar.

; Expected Results  : 	1.After step3 the selected toolbar will go into the hidden state.
;                       2. The menu will go into unchecked state.
;                      3. After step 5 the hidden tool bar will appear in its previous position.
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_21.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_21.cfx")
         Sleep(1000)
        Send("!VT")
	    Sleep(500)
 	    Send("{ESC}")
	    Sleep(1000)
	    Send("{ESC}")
	    Sleep(1000)
    WinMenuSelectItem($WIN_BUSMASTER,"","&View","&Toolbar","Message &Window")
		Sleep(1000)
		$winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
	 If winexists($WIN_CANMsgWind) Then
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 6)										; Fetch the handle of View menu
		$text_view=_GUICtrlMenu_GetItemText($hMain,6)
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		ConsoleWrite("View menu item text" & $text_view & @CRLF)
	Else
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 5)									  ; Fetch the handle of View menu
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	EndIf
	$hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 3)									 ; Fetch the handle of View->Toolbar menu
	$text1=_GUICtrlMenu_GetItemText($hFile,3)
	Sleep(1000)
	ConsoleWrite("$text1" & $text1 & @CRLF)
	Sleep(1000)
	Send("!VT")
	Sleep(500)
	Send("{ESC}")
	Sleep(1000)
	Send("{ESC}")
	Sleep(1000)
	$res=_GUICtrlMenu_GetItemChecked($hSubmenu,4)                                      ;Check whether Interpret mode is enabled
	$text=_GUICtrlMenu_GetItemText($hSubmenu,4)
	ConsoleWrite("$text"& $text & @CRLF)
	ConsoleWrite("res" & $res & @CRLF)
	Sleep(2000)

	 WinMenuSelectItem($WIN_BUSMASTER,"","&View","&Toolbar","Message &Window")
	 Sleep(1000)
	 $hSubmenu1=_GUICtrlMenu_GetItemSubMenu($hFile,3)									 ; Fetch the handle of View->Toolbar menu
	Sleep(1000)
	Send("!VT")
	Sleep(500)
	Send("{ESC}")
	Sleep(1000)
	Send("{ESC}")
	Sleep(1000)
	$res1=_GUICtrlMenu_GetItemChecked($hSubmenu1,4)                                      ;Check whether Interpret mode is enabled
	$text1=_GUICtrlMenu_GetItemText($hSubmenu1,4)
	ConsoleWrite("$text1"& $text1 & @CRLF)
	ConsoleWrite("res1" & $res1 & @CRLF)
	If($res=False And $res1=True) Then
		_WriteResult("Pass","TS_UserCfx_21")
	Else
		_WriteResult("Fail","TS_UserCfx_21")
	EndIf

EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_UserCfx_21")
EndIf

ConsoleWrite(@CRLF)
ConsoleWrite("****End : TS_UserCfx_21.au3****"&@CRLF)



