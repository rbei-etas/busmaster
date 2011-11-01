; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_027
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Replay
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_027"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!cr") ; Configure -> Replay

Sleep(50)
; Should be in shown state
if ControlCommand("Replay Configuration", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("Replay Configuration")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
