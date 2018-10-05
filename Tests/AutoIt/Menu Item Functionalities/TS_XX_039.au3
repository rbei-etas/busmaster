; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_039
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Overwrite
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_039"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Disable overwrite mode
Send("!umo")	; Functions -> Message Window-> Overwrite
Sleep(500)
; Enable overwrite mode
Send("!umo")	; Functions -> Message Window-> Overwrite
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
