; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_024
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Message Display
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_024"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!cm") ; Configure -> Message Display

Sleep(100)
; Should be in shown state
if ControlCommand("Configure Message Display- CAN", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("Configure Message Display- CAN")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
