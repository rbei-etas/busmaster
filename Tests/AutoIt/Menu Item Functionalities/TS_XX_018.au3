; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_018
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Toolbar->Main
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_018"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vtm") ; View -> Toolbar -> Main

; Should be in shown state
if ControlCommand("BUSMASTER", "Main", 128, "IsVisible") =  0 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vtm") ; View -> Toolbar -> Main
	
	; Should be in hidden state
	if ControlCommand("BUSMASTER", "Main", 128, "IsVisible") =  1 Then 
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
