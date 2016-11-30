;==================================================Test Cases/Test Data ========================================
; Critical (C)		:		Y
; TestCase No.		:		TS_UserCfx_01
; TestCases			:		Menu Items
; Test Data			:		cfx_UserCfx_01.cfx
; Test Setup		:		1. Open the Busmaster.
;                           2. Check the menu items


; Expected Results  : 		1. After step the Busmaster will containds the following menu items.
;                            1. File.
;                            2. CAN.
;                            3. J1939.
;                            4. View.
;                            5. Window.
;                            6. Help
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_UserCfx_01.au3****"&@CRLF)


    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("cfx_UserCfx_01.cfx")
		Sleep(1000)
		$handle=WinGetHandle($WIN_BUSMASTER)                                                                 ;Get the handlle of the window of menu
		$MenuHandle=_GUICtrlMenu_GetMenu($handle)                                                              ;Get Menu handle
        $MenuData1=_GUICtrlMenu_GetItemText($MenuHandle,0)
        ConsoleWrite("Menu Data1" & $MenuData1 & @CRLF)
		$MenuData2=_GUICtrlMenu_GetItemText($MenuHandle,1)                                 					; Retrieves menu item titles by removing '&' at the ledt side of the name
		$MenuData3=_GUICtrlMenu_GetItemText($MenuHandle,2)
		$MenuData4=_GUICtrlMenu_GetItemText($MenuHandle,3)
		$MenuData5=_GUICtrlMenu_GetItemText($MenuHandle,4)
		$MenuData6=_GUICtrlMenu_GetItemText($MenuHandle,5)
		$MenuData7=_GUICtrlMenu_GetItemText($MenuHandle,6)
		$MenuData8=_GUICtrlMenu_GetItemText($MenuHandle,7)
		;$MenuData9=_GUICtrlMenu_GetItemText($MenuHandle,8)

		If($MenuData1="&File" And $MenuData2="&CAN" And $MenuData3="&J1939" And $MenuData4="Flex&Ray" And $MenuData5="&LIN" And $MenuData6="&View" And $MenuData7="&Window" And $MenuData8="&Help") Then
		;If($MenuData1="&File" And $MenuData2="&CAN" And $MenuData3="&J1939" And $MenuData4="&LIN" And $MenuData5="&View" And $MenuData6="&Window" And $MenuData7="&Help") Then
			  _WriteResult("Pass","TS_UserCfx_01")
		Else
			_WriteResult("Fail","TS_UserCfx_01")
		EndIf
    EndIf


	$isAppNotRes=_CloseApp()																			; Close the app

	if $isAppNotRes=1 Then
         _WriteResult("Warning","TS_UserCfx_01")

	EndIf

	     ConsoleWrite("****End : TS_UserCfx_01.au3****"&@CRLF)
	    ConsoleWrite(@CRLF)





