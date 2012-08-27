;Test Case Number:	TS_17_009
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Block details
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

_launchApp()														; check whether the app exists or else launch it.
if $app=0 Then
	Send("!ct")
	sleep(2000)
Endif
WinWaitActive("BUSMASTER","",2)
$bName=ControlCommand ("BUSMASTER", "",1228, "IsVisible")
$bCyc=ControlCommand ("BUSMASTER", "Cyclic",1007, "IsVisible")
$bMono=ControlCommand ("BUSMASTER", "Monoshot",1008, "IsVisible")
$bTd=ControlCommand ("BUSMASTER", "",1236, "IsVisible")
$bTdval=ControlCommand ("BUSMASTER", "",1134, "IsVisible")
$bKey=ControlCommand ("BUSMASTER", "",1237, "IsVisible")
$bKeyval=ControlCommand ("BUSMASTER", "",1137, "IsVisible")
$bcombx=ControlCommand ("BUSMASTER", "","[CLASS:ComboBox; INSTANCE:3]", "IsVisible")
$badd=ControlCommand ("BUSMASTER", "Add",1226, "IsVisible")
$bdel=ControlCommand ("BUSMASTER", "Delete",1227, "IsVisible")
