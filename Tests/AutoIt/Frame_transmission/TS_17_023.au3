; Test Case Number:	TS_17_023
; Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Signal details of database message
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

if (WinWaitActive("BUSMASTER","",5) ) Then
$listViewCount1=ControlListView("BUSMASTER", "", "SysListView321", "GetItemCount")  
ControlListView ("BUSMASTER","","[SysListView321","Select",0)
$sigName = ControlListView("BUSMASTER", "", "SysListView321", "GetText", "0", "0")
  if $listViewCount1=1 and $sigName="SIG1" Then
	_ExcelWriteCell($oExcel, "OK", 28, 10)
  else
    _ExcelWriteCell($oExcel, "Error", 28, 10)
  EndIf
EndIf