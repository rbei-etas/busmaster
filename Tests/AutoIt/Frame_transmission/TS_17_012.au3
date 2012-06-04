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
	if $crashRes=1 then												; If application crash is found then update the result sheet by calling _writeCrashRes function.
	_writeCrashRes(17,10)
	EndIf
Endif
WinWaitActive("BUSMASTER","",5)
$bClick=ControlClick("BUSMASTER","Add",1226)
sleep(2000)
if $crashRes=1 Then
	_writeCrashRes(17,10)
EndIf
if $bClick=1 then
local $blockName = ControlListView("BUSMASTER", "", "SysListView323", "GetText", "0", "0")  ;fetch 1st column 1st row text
local $blockType = ControlListView("BUSMASTER", "", "SysListView323", "GetText", "0", "1")  ;fetch 1st column 2nd row text
local $blockValue = ControlListView("BUSMASTER", "", "SysListView323", "GetText", "0", "2") ;fetch 1st column 3rd row text
if $blockName="OnTime" and $blockType="CYCLIC" and $blockValue="100ms" then					;compare EV with AV
	_ExcelWriteCell($oExcel, "OK", 17, 10)
else
	_ExcelWriteCell($oExcel, "Error", 17, 10)
EndIf
Else
	_ExcelWriteCell($oExcel, "Add Button doesnt exist", 17, 09)
	_ExcelWriteCell($oExcel, "Error", 17, 10)
EndIf