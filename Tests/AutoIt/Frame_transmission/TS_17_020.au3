;Test Case Number:	TS_17_020
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Exception handling in Adding Message
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===


WinWaitActive("BUSMASTER","",5)
ControlClick("BUSMASTER","Add",1226)
$ID="G"
ControlSetText("BUSMASTER","",1001,$ID) 
ControlCommand("BUSMASTER","",1213,"Check")
sleep(100)
controlCommand("BUSMASTER","",1215,"Check")
$errID=ControlGetText ( "BUSMASTER", "", 1233)
$ID=1
ControlSetText("BUSMASTER","",1001,$ID) 
$DLC=9
ControlSetText("BUSMASTER","",1125,$DLC) 
ControlCommand("BUSMASTER","",1213,"Check")
sleep(100)
controlCommand("BUSMASTER","",1215,"Check")
$errDLC=controlGetText ( "BUSMASTER", "", 1233)
$button=ControlCommand("BUSMASTER","","[CLASS:Button; INSTANCE:4]","IsEnabled")