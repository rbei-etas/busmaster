; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_004
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Configuration->Save As...
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_004"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 1. Select 'Save As...' menu option in Configuration
Send("!fga") ; File->Configuration->Save As...
Sleep(1000) ; 1 sec


if WinWaitActive("SaveAs Configuration Filename...", "", 2) Then ; wait till its open
	Send("test_new.cfx") ; Filename
	ControlClick("SaveAs Configuration Filename...", "Save","Button2")

	Sleep(500)
	if (WinActive("SaveAs Configuration Filename...","Do you want to replace it?")) Then			
			ControlClick("SaveAs Configuration Filename...","Do you want to replace it?","Button1")
	EndIf
	Sleep(250)
	$strCurrConfigFile = StatusbarGetText("BUSMASTER", "", 2)
	$blnResult = StringInStr( $strCurrConfigFile, "test_new.cfx")		
	if $blnResult > 0 Then
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
