; Test Case Number:	TS_17_030
; Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Channel Information
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#Include <GuiComboBox.au3>
WinWaitActive("BUSMASTER","",5)
$hWnd = ControlGetHandle("BUSMASTER","",1010)
$count=_GUICtrlComboBox_GetCount($hWnd)


