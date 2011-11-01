; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_040
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Time Mode -> Absolute Time
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_040"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Set Time mode as 'Absolute Time'
Send("!umma")	; Functions -> Message Window-> Time Mode -> Absolute Time
Sleep(200)
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
