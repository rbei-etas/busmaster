; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_030
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Signal Watch
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_030"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!cn") ; Configure -> Signal Watch

Sleep(50)
; Should be in shown state
if ControlCommand("Signal Watch List ", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("Signal Watch List ")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
