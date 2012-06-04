;Test Case Number:	TS_17_018
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Adding\Modifying non Database messages in Tx Message list
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

WinWaitActive("BUSMASTER","",5)
ControlClick("BUSMASTER","Add",1226)
$ID=2
ControlSetText("BUSMASTER","",1001,$ID)
ControlCommand("BUSMASTER","",1213,"Check")
sleep(100)
controlCommand("BUSMASTER","",1215,"Check")
ControlClick("BUSMASTER","Add &Message",1122)
$listViewCount1=ControlListView("BUSMASTER", "", "SysListView322", "GetItemCount")
if $listViewCount1>0 Then
	ControlListView ("BUSMASTER","","SysListView322","Select",0)
	$mID = ControlListView("BUSMASTER", "", "SysListView322", "GetText", "0", "0")
	if $ID=$mID Then
	_ExcelWriteCell($oExcel, "OK", 23, 10)
else
	_ExcelWriteCell($oExcel, "Error", 23, 10)
	EndIf
Else
	_ExcelWriteCell($oExcel,"No messages Exists in Tx Message List",23,9)
	_ExcelWriteCell($oExcel, "Error", 23, 10)
EndIf
