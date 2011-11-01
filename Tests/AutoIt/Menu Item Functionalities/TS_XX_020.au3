; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_020
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Toolbar->Node Simulation
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_020"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vtn") ; View -> Toolbar -> Node Simulation

; Should be in shown state
if ControlCommand("BUSMASTER", "Node Simulation", 238, "IsVisible") =  0 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vtn") ; View -> Toolbar -> Node Simulation
	
	; Should be in hidden state
	if ControlCommand("BUSMASTER", "Node Simulation", 238, "IsVisible") =  1 Then 
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
