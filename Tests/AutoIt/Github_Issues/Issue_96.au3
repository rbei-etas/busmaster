;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #96
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

_launchApp()
_chkCrash(18,3)
sleep(1000)
if winexists($mWin) Then
	sleep(1000)
	$hWnd = WinGetHandle($mWin)
	$hMain = _GUICtrlMenu_GetMenu($hWnd)
	ConsoleWrite("menu handle : "&$hMain& @CRLF)
	$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 3)
	ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	$hexMenu=_GUICtrlMenu_FindItem($hFile,"Hex",True)
	ConsoleWrite("$hexMenu : "&$hexMenu& @CRLF)
EndIf
if $hexMenu>0 Then
	_ExcelWriteCell($oExcel, "Pass", 18, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 18, 3)
EndIf
