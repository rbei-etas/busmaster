;Test Case Number:	TS_17_004
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Tx Message List View GUI check
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
	if $crashRes=1 then												; If application crash is found then update the result sheet by calling _writeCrashRes function.
	_writeCrashRes(9,10)
	EndIf
Endif
if WinWaitActive("BUSMASTER","",5) then
	if ControlCommand ("BUSMASTER", "&Send",1229, "IsEnabled")=0 and ControlCommand ("BUSMASTER", "D&elete",1230, "IsEnabled")=0 and ControlCommand ("BUSMASTER", "Delete A&ll",1231, "IsEnabled")=0 then
	_ExcelWriteCell($oExcel, "OK", 9, 10)
	Else
    _ExcelWriteCell($oExcel, "Error", 9, 10)
EndIf
EndIf