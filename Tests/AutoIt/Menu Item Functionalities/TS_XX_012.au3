; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_012
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Associate
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_012"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 3. Associate database
Send("!fda") ; File -> Database -> Associate...

if WinWaitActive("Select Active Database Filename...", "", 2) Then ; wait till it opens for 2 seconds		
	$Result = "ok"	
	ControlClick("Select Active Database Filename...", "Cancel","Button3")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
