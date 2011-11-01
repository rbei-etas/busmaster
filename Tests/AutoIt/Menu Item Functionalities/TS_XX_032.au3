; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_032
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Disconnect
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_032"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Disconnect
Send("!ui")	; Functions -> Disconnect
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
