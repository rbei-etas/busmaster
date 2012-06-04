;Test Case Number:	TS_17_003
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Invoking TX Window Through icon
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

#Include <GuiToolBar.au3>
_launchApp()														; check whether the app exists or else launch it.
if WinWaitActive("BUSMASTER","",5) then
 $handle=ControlGetHandle("BUSMASTER", "","[CLASS:ToolbarWindow32; INSTANCE:3]")
EndIf
_GUICtrlToolbar_clickbutton($handle,32891,"left") 					;Clicks a specific button.32891 is the command ID of the specific button
sleep(2000)
	if $crashRes=1 then												; If application crash is found then update the result sheet by calling _writeCrashRes function.
	_writeCrashRes(8,10)
	EndIf
if ControlCommand("BUSMASTER", "Configure Transmission Messages","", "IsVisible") = 1 Then
	_ExcelWriteCell($oExcel, "OK", 8, 10)
Else
	_ExcelWriteCell($oExcel, "Error", 8, 10)
EndIf