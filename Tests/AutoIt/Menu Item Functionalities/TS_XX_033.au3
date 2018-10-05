; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_033
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Start Logging
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_033"
$Result = "Test did not finish"

; Enable Start Logging
Send("!ul")	; Functions -> Start Logging
Sleep(500)
; Disable Start Logging
Send("!ul")	; Functions -> Start Logging
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
