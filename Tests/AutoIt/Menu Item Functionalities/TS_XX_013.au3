; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_013
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Properties
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_013"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Select Properties
Send("!fp") ; File -> Properties

if WinWaitActive("Properties", "", 2) Then ; wait till it opens for 2 seconds		
	$Result = "ok"	
	WinClose("Properties")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
