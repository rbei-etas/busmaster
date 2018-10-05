; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_011
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Close
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_011"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

if WinWaitActive("BUSMASTER", "DatabaseEditor - CAN", 2) Then ; wait till it opens for 2 seconds	
	Send("!fdc") ; File -> Database -> Close
	$Result = "ok"	
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
