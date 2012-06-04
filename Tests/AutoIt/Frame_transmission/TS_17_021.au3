;Test Case Number:	TS_17_021
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Exception handling in Adding Message
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===


WinWaitActive("BUSMASTER","",5)
ControlClick("BUSMASTER","Add",1226)
$DLC=9
ControlSetText("BUSMASTER","",1125,$DLC) 
$errDLC=controlGetText ( "BUSMASTER", "", 1233)
ControlCommand("BUSMASTER","",1213,"Check")
sleep(100)
controlCommand("BUSMASTER","",1215,"Check")

$ID="S"
ControlSetText("BUSMASTER","",1001,$ID) 
ControlCommand("BUSMASTER","",1213,"Check")
sleep(100)
controlCommand("BUSMASTER","",1215,"Check")
$errID=ControlGetText ( "BUSMASTER", "", 1233)
$errDLC=controlGetText ( "BUSMASTER", "", 1233)
$button=ControlCommand("BUSMASTER","","[CLASS:Button; INSTANCE:4]","IsEnabled")
if $errDLC="Invalid DLC value" and $errID="Invalid Message ID or Name"and $button=0 then 
   _ExcelWriteCell($oExcel, "OK", 26, 10)
  else
   _ExcelWriteCell($oExcel, "Error", 26, 10)
  
EndIf