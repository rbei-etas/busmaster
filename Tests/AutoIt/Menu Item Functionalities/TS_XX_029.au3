; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_029
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Simulated Systems
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_029"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!cs") ; Configure -> Simulated Systems

; Should be in shown state
if MsgBox(1, "Verify Test Case: TS- 29", "If Simulated Systems window appears, close it and press 'OK' else press 'Cancel'.") = 1 Then 
	$Result = "ok"	
Else	; Cancel selected
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
