;===========Test Procedure==============
; Test Cases		: Github Defects Test
; Issue ID			: #180
; Test Strategy		: Black Box
; Critical 			: Y
;===========Test Procedure==============

#Include <GuiListView.au3>
#Include <GuiTreeView.au3>
#Include <GuiMenu.au3>

_launchApp()
if Winexists($mWin) Then
	sleep(2000)
	$hWnd = WinGetHandle($mWin)
	$hMain = _GUICtrlMenu_GetMenu($hWnd)
	ConsoleWrite("menu handle : "&$hMain& @CRLF)
	$hFile = _GUICtrlMenu_GetItemSubMenu($hMain, 6)
	ConsoleWrite("Submenu handle : "&$hFile& @CRLF)
	$subMenu=_GUICtrlMenu_GetItemSubMenu($hFile,1& @CRLF)
	$menu=_GUICtrlMenu_GetItemText($subMenu, 4, True)
EndIf
if $menu="&Save" Then
	_ExcelWriteCell($oExcel, "Pass", 7, 3)
Else
	_ExcelWriteCell($oExcel, "Fail", 7, 3)
EndIf
