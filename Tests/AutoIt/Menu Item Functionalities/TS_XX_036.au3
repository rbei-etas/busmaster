; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_036
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Enable Filter -> Display
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_036"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Enable Filter for Logging
Send("!ufd")	; Functions -> Enable Filter -> Display
Sleep(500)
; Disable Filter for Logging
Send("!ufd")	; Functions -> Enable Filter -> Display
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
