;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_014
; TestCases			:       Saving Hardware Interface details

; Test Setup		:	    1. Open the Busmaster.
;                           2. Change the Hardware interface(for example simulation to Peak).
;                           3. Save the configuration.
;                           4. Close and open the Busmaster.
;                           5. Check the Hardware Interafce.

; Expected Results  : 		After step 5 the hardware interface will be same as selected in step2
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_14.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_14.cfx")
		Sleep(1000)
		_hdWareSelectKvaser()
		Sleep(1000)
		$isAppNotRes=_CloseApp()																		 ; Close the app
        if $isAppNotRes=1 Then
			ConsoleWrite("Application not responding!! Error")
	    EndIf
		 _launchApp()
		 Sleep(2000)
		 Send("!CV")
		 Sleep(1000)
		  Send("{ESC}")
	    Sleep(1000)
	    Send("{ESC}")
	    Sleep(1000)
		 $winhWnd = WinGetHandle($WIN_BUSMASTER)												   ; Fetch the window handle
	    $hMain = _GUICtrlMenu_GetMenu($winhWnd)                                                 ; Fetch the handle of the menu
		if winexists($WIN_CANMsgWind) Then
			$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)										; Fetch the handle of CAN menu
	    Else
		    $hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 1)										; Fetch the handle of CAN menu
		    ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	    EndIf
	    $hSubmenu=_GUICtrlMenu_GetItemSubMenu($hFile, 1)
	    $submenuText=_GUICtrlMenu_GetItemText($hFile,1)
	    ConsoleWrite("$submenuText:" & $submenuText & @CRLF)
        $res=_GUICtrlMenu_GetItemDisabled($hSubmenu,7)										   ; Check whether CAN->Driver Selection->Kvaser CAN is disabled or not
	    $val=_GUICtrlMenu_GetItemText($hSubmenu,7)											   ; Fetch the text of CAN->Driver Selection->Kvaser CAN
	    ConsoleWrite("$res : "&$res& @CRLF)
	    ConsoleWrite("$val : "&$val& @CRLF)
	    If($res=True) Then
		 	_WriteResult("Pass","TS_UserCfx_14")
	    Else
		    _WriteResult("Fail","TS_UserCfx_14")
	EndIf

EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_UserCfx_14")
EndIf

ConsoleWrite(@CRLF)
ConsoleWrite("****End : TS_UserCfx_14.au3****"&@CRLF)





