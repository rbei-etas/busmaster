; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_037
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Test Automation -> Editor
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_037"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Enable Filter for Logging
Send("!uae")	; Functions -> Test Automation -> Editor

; Should be in shown state
if MsgBox(1, "Verify Test Case: TS- 37", "If Test Automation Editor window appears, close it and press 'OK' else press 'Cancel'.") = 1 Then 
	$Result = "ok"	
Else	; Cancel selected
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
