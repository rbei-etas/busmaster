; === Test Cases/Test Data ===
; Test Case Number:	TS_20_026
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Number in "From" text field should be lesser than that in "To" text field
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_026"
$Result = "Test did not finish"

WinActivate("BUSMASTER","")
; 1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.


; Select Range radio button and give a valid range
ControlCommand("Configure Filter List", "", 1254, "Check")	;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.

; 2. Specify a range such that number keyed in "From" field is greater than that in "To" text field
;Set Range as 0x10-0x5
ControlSetText("Configure Filter List", "", 1041, "10")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
ControlSetText("Configure Filter List", "", 1042, "5")	    ;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.

$strWarning = ControlGetText("Configure Filter List", "", 1233)	;1233 is the ID for "Warning" Edit control in Filter Attributes groupbox.
$EnbAdd        = ControlCommand("Configure Filter List", "",    3, "IsEnabled")		;   3 is the ID for "Add" button in Filter Attributes groupbox.

; Check for the warning message display and 'Add' button enable status.
If $strWarning = "Start Range ID is greater then End Range" And $EnbAdd = "0" Then
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
