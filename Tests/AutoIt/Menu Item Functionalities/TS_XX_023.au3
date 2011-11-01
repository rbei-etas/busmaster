; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_023
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Statusbar
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_023"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vs") ; View -> Statusbar

Sleep(100)
; Should be in hidden state
if ControlCommand("BUSMASTER", "", 59393, "IsVisible") = 0 Then 
	; Show message window
	
	WinActivate("BUSMASTER","")
	Send("!vs") ; View -> Statusbar
	Sleep(100)
	; Should be in shown state
	if ControlCommand("BUSMASTER", "", 59393, "IsVisible") = 1 Then		
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
