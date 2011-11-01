; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_008
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Save and Import
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_008"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

WinActivate("BUSMASTER","")
Send("!fdp") ; File -> Database -> Save and Import
$Result = "ok"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
