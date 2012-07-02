;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #204
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()													; invoke the application and run the Process
_chkCrash(11,3)
sleep(2000)
if winexists($mWin) Then
	ConsoleWrite("Issue #204"&@CRLF)
	_loadConfig("HWTest")
	_chkCrash(11,3)
	_hdWareSelect("&Vector XL")
	WinMenuSelectItem($mWin,"","&File","Confi&guration","&Save")
	WinClose($mWin)
	sleep(500)
	if WinExists("","You have made changes to the configuration.") Then
		ControlClick("","You have made changes to the configuration.","[CLASS:Button; INSTANCE:1]")
	EndIf
EndIf
_launchApp()													; invoke the application and run the Process
sleep(1000)
if WinExists($mWin) then
	sleep(1000)
	if WinExists("MCNet") Then
		send("{ENTER}")
	EndIf
	send(1000)
	if winexists($hWare) Then																		; if a Hardware Selection dialog appears then click 'OK'
		ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick($hWare,$selectBTN,"[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick($hWare,$okBTN,"[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	if winexists($mWin) Then
		$hWnd = WinGetHandle($mWin)
		$hMain = _GUICtrlMenu_GetMenu($hWnd)
		ConsoleWrite("menu handle : "&$hMain& @CRLF)
		$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)
		ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
		$state=_GUICtrlMenu_GetItemGrayed($hFile,0,True)
	EndIf
EndIf
if $state=True Then
	_ExcelWriteCell($oExcel, "Pass", 11, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 11, 3)
EndIf

