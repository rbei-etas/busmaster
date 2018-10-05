; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_042
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Message Window-> Time Mode -> System Time
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_042"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Set Time mode as 'System Time'
Send("!umms")	; Functions -> Message Window-> Time Mode -> System Time
Sleep(200)
$Result = "ok"	

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
