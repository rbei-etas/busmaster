; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_007
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Save
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_007"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

WinActivate("BUSMASTER","")
Send("!fds") ; File -> Database -> Save
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
