;Test Case Number:	TS_17_007
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		MFunctions View
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
	if $crashRes=1 then												; If application crash is found then update the result sheet by calling _writeCrashRes function.
	_writeCrashRes(12,10)
	EndIf
Endif
WinWaitActive("BUSMASTER","",5)
$update=ControlCommand ("BUSMASTER", "Update",1014, "IsVisible")
$close=ControlCommand ("BUSMASTER", "Close",1015, "IsVisible")
if $update=1 and $close=1 Then
	_ExcelWriteCell($oExcel, "OK", 12, 10)
else
	_ExcelWriteCell($oExcel, "Error", 12, 10)
EndIf
