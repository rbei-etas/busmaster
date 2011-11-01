; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_044
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Reset columns
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_044"
$Result = "Test did not finish"

; --- Test SetUp ---

MsgBox(0, "User action needed", "Change the order of columns in Message Window and press 'OK'.")
WinActivate("BUSMASTER","")
Send("!umr")	; Functions -> Message Window-> Reset columns

if MsgBox(1, "Verify Test Case: TS- 44", "If Columns retain their default positions, click 'OK' else 'Cancel'.") = 1 Then 
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
