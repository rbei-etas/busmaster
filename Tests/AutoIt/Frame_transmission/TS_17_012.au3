;Test Case Number:	TS_17_012
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Adding/Modifying Message Block
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
Endif
WinWaitActive("BUSMASTER","",5)
$bClick=ControlClick("BUSMASTER","Add",1226)
sleep(2000)
if $bClick=1 then
	local $blockName = ControlListView("BUSMASTER", "", "SysListView323", "GetText", "0", "0")  ;fetch 1st column 1st row text
	local $blockType = ControlListView("BUSMASTER", "", "SysListView323", "GetText", "0", "1")  ;fetch 1st column 2nd row text
	local $blockValue = ControlListView("BUSMASTER", "", "SysListView323", "GetText", "0", "2") ;fetch 1st column 3rd row text
EndIf