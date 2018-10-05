; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_025
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Log
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_025"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!co") ; Configure -> Log

Sleep(100)
; Should be in shown state
if ControlCommand("Configure Logging for CAN", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinActivate("Configure Logging for CAN","")
	ControlClick("Configure Logging for CAN", "Add", 1303)
	ControlClick("Configure Logging for CAN", "OK",  1)
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
