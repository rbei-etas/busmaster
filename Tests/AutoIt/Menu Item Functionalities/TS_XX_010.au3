; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_010
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Save As...
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_010"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!fdv") ; File -> Database -> Save As...

if WinWaitActive("Save As...", "", 2) Then ; wait till it opens for 2 seconds
	$Result = "ok"
	ControlClick("Save As...", "Cancel","Button3")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
