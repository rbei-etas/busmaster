; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_031
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Connect
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_031"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Connect 
Send("!uc")	; Functions -> Connect

$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
