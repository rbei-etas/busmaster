; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_014
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Message Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_014"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Hide message window
Send("!vm") ; View -> Message Window

; Should be in hidden state
if ControlCommand("BUSMASTER", "Message Window - CAN", "", "IsVisible") =  0 Then 
	; Show message window
	Sleep(200)
	Send("!vm") ; View -> Message Window
	
	; Should be in shown state
	if ControlCommand("BUSMASTER", "Message Window - CAN", "", "IsVisible") =  1 Then
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
