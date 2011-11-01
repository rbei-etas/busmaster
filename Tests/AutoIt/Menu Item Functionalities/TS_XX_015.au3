; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_015
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		View->Signal Watch Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_015"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!vw") ; View -> Signal Watch Window

; Should be in shown state
if ControlCommand("Signal Watch - CAN", "", "", "IsVisible") =  1 Then 
	; Show message window
	Sleep(200)
	WinActivate("BUSMASTER","")
	Send("!vw") ; View -> Signal Watch Window
	
	; Should be in hidden state
	if ControlCommand("Signal Watch - CAN",  "", "", "IsVisible") =  0 Then
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
