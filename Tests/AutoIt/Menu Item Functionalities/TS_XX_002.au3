; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_002
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		File->Configuration->Load...
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_002"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; 1. Select Load Configuration
Send("!fgl") ; File->Configuration->Load...
Sleep(1000) ; 1 sec
if (WinActive("BUSMASTER", "Configuration File is modified")) Then	
	Send("!y") ; Yes	
EndIf

if WinWaitActive("Load Configuration Filename...", "", 2) Then ; wait till its open
	Send("test.cfx") ; Filename
	ControlClick("Load Configuration Filename...", "","Button2")
	$strCurrConfigFile = StatusbarGetText("BUSMASTER", "", 2)
	$blnResult = StringInStr( $strCurrConfigFile, "test.cfx")	
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
