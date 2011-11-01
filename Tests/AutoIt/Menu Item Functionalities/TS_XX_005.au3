; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_005
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->New
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_005"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 1. Select 'New' menu option in Database
Send("!fdn") ; File->Database->New
Sleep(500) ; 0.5 sec


if WinWaitActive("Save As", "", 2) Then ; wait till its open
	$Result = "ok"		
	ControlClick("Save As", "Cancel", "Button3")	
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
