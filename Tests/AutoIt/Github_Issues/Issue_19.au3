;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #19
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()													; invoke the application and run the Process
_chkCrash(12,3)
sleep(1000)
if WinExists($mWin) then
	_hdWareSelect("&Kvaser CAN")
	$hWnd = WinGetHandle($mWin)
	$hMain = _GUICtrlMenu_GetMenu($hWnd)
	ConsoleWrite("menu handle : "&$hMain& @CRLF)
	$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 2)
	ConsoleWrite("$hFile : "&$hFile& @CRLF)
	$itemTxt=_GUICtrlMenu_GetItemText($hFile,0,True)
	ConsoleWrite("$itemTxt : "&$itemTxt& @CRLF)
	WinMenuSelectItem($mWin,"","&Configure","Har&dware Selection")
	sleep(3000)
	$hwSel=winexists($hWare)
	ConsoleWrite("$hwSel : "&$hwSel& @CRLF)
	ControlClick($hWare,"","[CLASS:Button; INSTANCE:5]")
	sleep(1000)
	if WinExists($kvaserCANWin) Then
		ControlClick($kvaserCANWin,"","[CLASS:Button; INSTANCE:1]")
		sleep(1000)
	EndIf
	WinMenuSelectItem($mWin,"","&Configure","&Hardware Interface","&Simulation")
	WinMenuSelectItem($mWin,"","&File","Confi&guration","&Save")
EndIf
if $itemTxt="Hardware Selection" and $hwSel=1 Then
	_ExcelWriteCell($oExcel, "Pass", 12, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 12, 3)
EndIf
