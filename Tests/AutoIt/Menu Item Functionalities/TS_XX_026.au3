; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_026
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Configure->Tx Messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_026"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

Send("!ct") ; Configure -> Tx Messages

Sleep(100)
; Should be in shown state
if ControlCommand("BUSMASTER", "Configure Transmission Messages","", "IsVisible") = 1 Then 
	$Result = "ok"
	ControlClick("BUSMASTER","Configure Transmission Messages", 1015) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
