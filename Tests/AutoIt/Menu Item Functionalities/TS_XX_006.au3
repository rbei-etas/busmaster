; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_006
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Database->Open...
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_006"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 1. Select 'Open...' menu option in Database
Send("!fdo") ; File->Database->Open...
Sleep(500) ; 0.5 sec


if WinWaitActive("Select BUSMASTER Database Filename...", "", 2) Then ; wait till its open
	$Result = "ok"		
	Send("d:\BUSMASTER\Tests\AutoIt\Common\Empty.dbf")
	Sleep(250)
	ControlClick("Select BUSMASTER Database Filename...", "","Button2")	
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
