; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_043
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Interpret
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_043"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Enable 'Interpret' mode
Send("!umi")	; Functions -> Message Window-> Interpret
Sleep(200)
; Disable 'Interpret' mode
Send("!umi")	; Functions -> Message Window-> Interpret
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
