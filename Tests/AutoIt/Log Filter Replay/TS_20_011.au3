; === Test Cases/Test Data ===
; Test Case Number:	TS_20_011
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Deleting filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_011"
$Result = "Test did not finish"

; Select filter in 'Filter List' groupbox.
ControlListView("Configure Filter List", "", 1000, "Select", 0) ;1000 is the ID for List view in Filter List groupbox.

; Click 'Delete' button in 'Filter List' groupbox.
ControlClick("Configure Filter List","Delete", 1002) ;   1002 is the ID for "Delete" button in Filter List groupbox.

$CntFilterList = ControlListView("Configure Filter List", "", 1000, "GetItemCount") ;1000 is the ID for List view in Filter List groupbox.

If $CntFilterList = 0 Then
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
