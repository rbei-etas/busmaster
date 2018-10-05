; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_003
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Configuration->Save
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_003"
$Result = "Test did not finish"

WinActivate("BUSMASTER","")
; 1. Select Save Configuration
Send("!fgs") ; File->Configuration->Save
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
