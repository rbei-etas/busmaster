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
Endif
WinWaitActive("BUSMASTER","",5)
$update=ControlCommand ("BUSMASTER", "Update",1014, "IsVisible")
$close=ControlCommand ("BUSMASTER", "Close",1015, "IsVisible")
