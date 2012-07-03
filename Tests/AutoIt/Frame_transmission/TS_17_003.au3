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
