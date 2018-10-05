; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_001
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Configuration->New
; Test Strategy:	Black Box
; Test Data:		-

; === Test Procedure ===
$Number = "TS_XX_001"
$Result = "Test did not finish"

MsgBox(0, "User Action-TS 1", "Run BUSMASTER Application and press OK.")
; --- Test SetUp ---
WinActivate("BUSMASTER","")

If (WinWaitActive("BUSMASTER","",5) ) Then
	
Else
	MsgBox(0, "GUI Event", "BUSMASTER not active")
	FileWriteLine($file,  "BUSMASTER application is not opened.")	
	Exit
EndIf
 
; 1. Select New Configuration
Send("!fgn") ; File->Configuration->New
Sleep(1000) ; 1 sec
if (WinActive("BUSMASTER", "Configuration File is modified")) Then
	Send("!y") ; Yes
EndIf

if WinWaitActive("New Configuration Filename...", "", 2) Then ; wait till its open
	Send("test.cfx") ; Filename
	Send("!s") ; Save
	if (WinWaitActive("New Configuration Filename...", "Do you want to replace it?", 2)) Then
		Send("!y") ; Yes
	EndIf
	$Result = "ok"
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
