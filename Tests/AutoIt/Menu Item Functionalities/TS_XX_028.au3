; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_028
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->App Filters
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_028"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!ca") ; Configure -> App Filters

Sleep(50)
; Should be in shown state
if ControlCommand("Configure Filter List", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("Configure Filter List")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
