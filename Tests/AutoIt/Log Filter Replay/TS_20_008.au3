; === Test Cases/Test Data ===
; Test Case Number:	TS_20_008
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Filter Attributes
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_008"
$Result = "Test did not finish"

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
