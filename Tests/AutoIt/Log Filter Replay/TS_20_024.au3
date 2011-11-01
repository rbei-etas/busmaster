; === Test Cases/Test Data ===
; Test Case Number:	TS_20_024
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for GUI - Message Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_024"
$Result = "Test did not finish"

; 1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.


; Select Range radio button and give a valid range
ControlCommand("Configure Filter List", "", 1254, "Check")	;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.
WinWaitActive("Configure Filter List","",5) 

; 2. Specify a range with hexadecimal values
;Set Range as 0xAB-0xCD
ControlSetText("Configure Filter List", "", 1041, "AB")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
ControlSetText("Configure Filter List", "", 1042, "CD")	;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.

; Hexadecimal values should be accepted.
$blnVisbFrom = ControlCommand("Configure Filter List", "", 1041, "IsVisible")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
$blnVisbTo   = ControlCommand("Configure Filter List", "", 1042, "IsVisible")	;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.


If $blnVisbFrom = 1 And $blnVisbTo = 1 Then
	$Result = "ok"						
Else
	$Result = "Failed"
EndIf
WinClose("Configure Filter List")

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
