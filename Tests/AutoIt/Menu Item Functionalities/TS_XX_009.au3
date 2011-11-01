; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_009
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Disassociate
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_009"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")
Send("!fdd") ; File -> Database -> Disassociate

if WinWaitActive("Dissociate Database", "", 2) Then ; wait till it opens for 2 seconds
	$Result = "ok"
	ControlClick("Dissociate Database", "","Button2")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
