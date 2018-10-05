; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_016
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Network Statistics
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_016"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vn") ; View -> Network Statistics

; Should be in shown state
if ControlCommand("Network Statistics", "", "", "IsVisible") =  1 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vn") ; View -> Network Statistics
	
	; Should be in hidden state
	if ControlCommand("Network Statistics",  "", "", "IsVisible") =  0 Then
		$Result = "ok"
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
