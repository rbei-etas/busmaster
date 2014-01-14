;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_016
; TestCases			:       Saving Message Interpretation status

; Test Setup                1. Open Busmaster..
;                           2. switch on the message interpretation.
;                           3. Save the configuration.
;                           4. Close and open the Busmaster..
;                           5. Check the interpretation status.

; Expected Results  : 		1. The interpreation will be on.
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_16.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_16.cfx")
		Sleep(1000)
	    _EnableInterpretMode()
	     Sleep(1000)
	    WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								; Select File->Exit from menu
	    sleep(2000)
	    if WinWaitActive("",$SaveChangesConfigTXT,2) Then									; wait for save configuration dialog
		   ControlClick("",$SaveChangesConfigTXT,$BTN_Yes_SaveConfig)						; Click on No button
	    EndIf
	    _launchApp()
	     Sleep(1000)
	    $winhWnd = WinGetHandle($WIN_BUSMASTER)												; Fetch the window handle
		$hMain = _GUICtrlMenu_GetMenu($winhWnd)												; Fetch the handle of the menu
	    If winexists($WIN_CANMsgWind) Then
		   $hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
		   ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	    Else
		   $hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)									  ; Fetch the handle of CAN menu
		   ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	    EndIf
	    $hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 9)									 ; Fetch the handle of CAN->Msg Window menu
	    ConsoleWrite("$hSubmenu handle : "&$hSubmenu& @CRLF)
	    $res=_GUICtrlMenu_GetItemChecked($hSubmenu,7)                                      ;Check whether Interpret mode is enabled
	    $text=_GUICtrlMenu_GetItemText($hSubmenu,7)
	    ConsoleWrite("$text"& $text & @CRLF)
	    ConsoleWrite("res1" & $res & @CRLF)
  	    If($res=True) Then
		  _WriteResult("Pass","TS_UserCfx_16")
        Else
		  _WriteResult("Fail","TS_UserCfx_16")
		EndIf

EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_UserCfx_16")
EndIf

  ConsoleWrite("****End : TS_UserCfx_16.au3****"&@CRLF)
  ConsoleWrite(@CRLF)





