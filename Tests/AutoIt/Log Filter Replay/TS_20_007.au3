; === Test Cases/Test Data ===
; Test Case Number:	TS_20_007
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Adding Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_007"
$Result = "Test did not finish"

; Select Range radio button and give a valid range
ControlCommand("Configure Filter List", "", 1254, "Check")	;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.

;Set Range as 0x10-0x15
$EnbMsgIDFrom  = ControlSetText("Configure Filter List", "", 1041, "10")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
$EnbMsgIDTo    = ControlSetText("Configure Filter List", "", 1042, "15")	;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.

; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

;A filter will be added to the list with the selected range
$CntFilterDetails = ControlListView("Configure Filter List", "", 1003, "GetItemCount");1003 is the ID for List view in Filter Details groupbox.
If $CntFilterDetails > 0 Then
	$strRangeFrom = ControlListView("Configure Filter List", "", 1003, "GetText", 1, 0)	; Column 1 should hold the Range filter
	$strRangeTo   = ControlListView("Configure Filter List", "", 1003, "GetText", 1, 1)
	if $strRangeFrom = 10 And $strRangeTo = 15 Then
		$Result = "ok"
	EndIf
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
