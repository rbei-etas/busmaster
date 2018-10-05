; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_045
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Help -> About
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_045"
$Result = "Test did not finish"

; --- Test SetUp ---

WinActivate("BUSMASTER","")
Send("!ha")	; Help -> About
Sleep(200)
; About dialog should be in shown state
if ControlCommand("About", "","", "IsVisible") = 1 Then 
	$Result = "ok"
	WinClose("About")
Else
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else		
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf

; --- Closure ---
;WinClose("BUSMASTER")
;ControlClick("BUSMASTER", "", 7)	;7 is the ID for 'No' button
