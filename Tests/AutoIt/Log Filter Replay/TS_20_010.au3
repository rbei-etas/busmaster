; === Test Cases/Test Data ===
; Test Case Number:	TS_20_010
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Deleting filter from filter details list
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_010"
$Result = "Test did not finish"

; Select a filter in 'Filter Details' groupbox.
ControlListView("Configure Filter List", "", 1003, "Select", 0)

; Select 'Delete All' button in 'Filter Details' groupbox.
ControlClick("Configure Filter List","Delete", 1251) ;   1251 is the ID for "Delete All" button in Filter Details groupbox.

;Check for delete conformation message and select No.

If (WinWaitActive("BUSMASTER","Do you want to delete all filters from the list?")) Then	
	ControlClick("BUSMASTER","Do you want to delete all filters from the list?", 7) ; 7 is the ID for 'No' button
EndIf

; Select 'Delete All' button in 'Filter Details' groupbox.
ControlClick("Configure Filter List","Delete", 1251) ;   1251 is the ID for "Delete All" button in Filter Details groupbox.

;Check for delete conformation message and select No.

If (WinWaitActive("BUSMASTER","Do you want to delete all filters from the list?")) Then	
	ControlClick("BUSMASTER","Do you want to delete all filters from the list?", 6) ; 6 is the ID for 'Yes' button
EndIf

$CntFilterDetails = ControlListView("Configure Filter List", "", 1003, "GetItemCount");1003 is the ID for List view in Filter Details groupbox.

If $CntFilterDetails = 0 Then
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
