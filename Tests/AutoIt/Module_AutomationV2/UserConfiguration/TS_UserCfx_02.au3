;==================================================Test Cases/Test Data ========================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_02
; TestCases			:		Menu Selection
; Test Data			:		cfx_UserCfx_02.cfx
; Test Setup		:		1. Open the Busmaster
;                           2. Click on the menu items File.
;                           3. Observe the menu items and there status


; Expected Results  : 		1. After step2 a sub menu with menu items, New, Load, Save, Save All will appear.
;                           2. All the menus are in active state.
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_02.au3****"&@CRLF)
 _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_02.cfx")
	     Sleep(1000)
		$handle=WinGetHandle($WIN_BUSMASTER)                                                    ;Get the handlle of the window of menu
		$MenuHandle=_GUICtrlMenu_GetMenu($handle)                                                ;Get Menu handle
        $MenuData1=StringTrimLeft(_GUICtrlMenu_GetItemText($MenuHandle,0),1)                       ;Retrieves menu item titles by removing '&' at the ledt side of the name
        ConsoleWrite("Menu Data1" & $MenuData1 & @CRLF)
		$Submenuhandle=_GUICtrlMenu_GetItemSubMenu($MenuHandle,0)
       $SubMenuData1=_GUICtrlMenu_GetItemText($Submenuhandle,0)
       ConsoleWrite("Submenu" & $SubMenuData1 & @CRLF)
	   ;$SubMenuData1=StringTrimLeft(_GUICtrlMenu_GetItemText($Submenuhandle,0),1)
	   $SubMenuData2=_GUICtrlMenu_GetItemText($Submenuhandle,1)
	   $SubMenuData3=_GUICtrlMenu_GetItemText($Submenuhandle,2)
	   $SubMenuData4=_GUICtrlMenu_GetItemText($Submenuhandle,3)
	   $SubMenuData5=_GUICtrlMenu_GetItemText($Submenuhandle,4)
	   $SubMenuData6=_GUICtrlMenu_GetItemText($Submenuhandle,5)
	   $SubMenuData7=_GUICtrlMenu_GetItemText($Submenuhandle,6)
	   $SubMenuData8=_GUICtrlMenu_GetItemText($Submenuhandle,7)
	   $SubMenuData9=_GUICtrlMenu_GetItemText($Submenuhandle,8)
	    $SubMenuData10=_GUICtrlMenu_GetItemText($Submenuhandle,9)
		 $SubMenuData11=_GUICtrlMenu_GetItemText($Submenuhandle,10)
	   ConsoleWrite("submenu1" & $SubMenuData1 & @CRLF)
	   ConsoleWrite("submenu2" & $SubMenuData2 & @CRLF)
	   ConsoleWrite("submenu3" & $SubMenuData3 & @CRLF)
	   ConsoleWrite("submenu4" & $SubMenuData4 & @CRLF)
	   ConsoleWrite("submenu5" & $SubMenuData5 & @CRLF)
	   ConsoleWrite("submenu6" & $SubMenuData6 & @CRLF)
	   ConsoleWrite("submenu7" & $SubMenuData7 & @CRLF)
	    ConsoleWrite("submenu8" & $SubMenuData8 & @CRLF)
	   if($SubMenuData1="&New" And $SubMenuData2="&Load..." And $SubMenuData3="&Save" And $SubMenuData4="Save &As..." And $SubMenuData5="&Recent Configurations" And $SubMenuData7="&Format Converters" And $SubMenuData9="&Properties" And $SubMenuData11="E&xit") Then
			  _WriteResult("Pass","TS_UserCfx_02")
		Else
			_WriteResult("Fail","TS_UserCfx_02")
		EndIf
    EndIf


	$isAppNotRes=_CloseApp()																			; Close the app

	if $isAppNotRes=1 Then
         _WriteResult("Warning","TS_UserCfx_02")

	EndIf

	     ConsoleWrite("****End : TS_UserCfx_02.au3****"&@CRLF)
	    ConsoleWrite(@CRLF)


