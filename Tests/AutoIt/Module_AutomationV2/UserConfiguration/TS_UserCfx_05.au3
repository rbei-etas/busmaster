;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_05
; TestCases			:		Menu State when Busmaster is connected to bus.
; Test Data			:		cfx_UserCfx_05.cfx
; Test Setup		:		1. Open the  Busmaster.
;                           2. Disconnect the  Busmaster from the bus and check the state of menu items in File.
;                           3. Connect the  Busmaster to a bus and check the same menu items.


; Expected Results  : 		1. After step2 the all menu items are in enable status.
;                           2. After step 3 the items New, Load are in disable state and other items are in active state.
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_05.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_05.cfx")
		$handle=WinGetHandle($WIN_BUSMASTER)                                                    ;Get the handlle of the window of menu
		$MenuHandle=_GUICtrlMenu_GetMenu($handle)                                                ;Get Menu handle
		$Submenuhandle=_GUICtrlMenu_GetItemSubMenu($MenuHandle,0)
       $SubMenuData1=_GUICtrlMenu_GetItemText($Submenuhandle,0)
       ConsoleWrite("Submenu" & $SubMenuData1 & @CRLF)
	   $SubMenuState=_GUICtrlMenu_GetItemGrayed($Submenuhandle,0)
	   ConsoleWrite("Submenu state" & $SubMenuState & @CRLF)
	     WinMenuSelectItem($WIN_BUSMASTER,"",$fileMenu,$Exit)								; Select File->Exit from menu
	sleep(2000)
	if WinWaitActive("",$SaveChangesConfigTXT,2) Then									; wait for save configuration dialog
		ControlClick("",$SaveChangesConfigTXT,$BTN_No_SaveConfig)						; Click on No button
	EndIf
		 Sleep(1000)
		 _launchApp()
		 sleep(500)
		_ConnectDisconnect()
	   	send("!f")
		send("{ESC}")
		$handle=WinGetHandle($WIN_BUSMASTER)                                                    ;Get the handlle of the window of menu
		$MenuHandle=_GUICtrlMenu_GetMenu($handle)                                                ;Get Menu handle
		send("!f")
		send("{ESC}")
		$Submenuhandle=_GUICtrlMenu_GetItemSubMenu($MenuHandle,0)
       $SubMenuData1=_GUICtrlMenu_GetItemText($Submenuhandle,0)
       ConsoleWrite("Submenu" & $SubMenuData1 & @CRLF)
	   $SubMenuStateAft=_GUICtrlMenu_GetItemGrayed($Submenuhandle,0)                             ; Check if menu item is grayed
	   ConsoleWrite("Submenu state After" & $SubMenuStateAft & @CRLF)
	   If($SubMenuState=False And $SubMenuStateAft=True) Then
		  _WriteResult("Pass","TS_UserCfx_05")
		Else
			_WriteResult("Fail","TS_UserCfx_05")
		EndIf
    EndIf


	$isAppNotRes=_CloseApp()																			; Close the app

	if $isAppNotRes=1 Then
         _WriteResult("Warning","TS_UserCfx_05")

	EndIf

	     ConsoleWrite("****End : TS_UserCfx_05.au3****"&@CRLF)
	    ConsoleWrite(@CRLF)


