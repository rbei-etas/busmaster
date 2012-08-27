; Test Case Number:	TS_17_025
; Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Signal details of database message
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#Include <GuiListView.au3>
WinWaitActive("BUSMASTER","",5)
$hWnd = ControlGetHandle("BUSMASTER","",1232)
_GUICtrlListView_ClickItem($hWnd,0,"Left","",2)
if WinActive("Signal Details") Then
	$sName=ControlGetText("Signal Details","","[CLASS:Edit; INSTANCE:1]")
	$sType=ControlGetText("Signal Details","","[CLASS:ComboBox; INSTANCE:1]")
	$sLength=ControlGetText("Signal Details","","[CLASS:Edit; INSTANCE:4]")
	ControlClick("Signal Details","","[CLASS:Button; INSTANCE:1]","Left")
EndIf

