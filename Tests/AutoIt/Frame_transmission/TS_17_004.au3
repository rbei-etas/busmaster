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
Endif
if WinWaitActive("BUSMASTER","",5) then
	ControlCommand ("BUSMASTER", "&Send",1229, "IsEnabled")
	ControlCommand ("BUSMASTER", "D&elete",1230, "IsEnabled")
	ControlCommand ("BUSMASTER", "Delete A&ll",1231, "IsEnabled")
EndIf