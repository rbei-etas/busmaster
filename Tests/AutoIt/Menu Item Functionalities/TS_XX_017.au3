; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_017
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Trace Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_017"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vr") ; View -> Trace Window

; Should be in shown state
if ControlCommand("BUSMASTER", "Trace Window", "", "IsVisible") =  1 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vr") ; View -> Trace Window
	Sleep(200)
	; Should be in hidden state
	if ControlCommand("BUSMASTER",  "Trace Window", "", "IsVisible") =  0 Then
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
