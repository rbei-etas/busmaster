; === Test Cases/Test Data ===
; Test Case Number:	TS_20_012
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		OK and Cancel functionality
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_012"
$Result = "Test did not finish"

;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

;Click 'OK' button to close the dialog.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.

WinWaitActive("BUSMASTER","") 
; 1. Select Configuration -> App Filters.
Send("!ca") ; Configure->App Filters.

; 2. Note down the filter list
$CntStoreFilterList = ControlListView("Configure Filter List", "", 1000, "GetItemCount") ;1000 is the ID for List view in Filter List groupbox.

; 3. Delete all filters in the filter list.

while ControlListView("Configure Filter List", "", 1000, "GetItemCount") >0
	ControlListView("Configure Filter List", "", 1000, "Select", 0)
	; Click 'Delete' button in 'Filter List' groupbox.
	ControlClick("Configure Filter List","Delete", 1002) ;   1002 is the ID for "Delete" button in Filter List groupbox.
WEnd 

; 4. Select Cancel button to close the dialog.
ControlClick("Configure Filter List", "Cancel", 2)		;2 is the ID for Cancel button in Configure Filter List dialog.

; 5. Invoke the dialog again.
WinWaitActive("BUSMASTER","") 
; 1. Select Configuration -> App Filters.
Send("!ca") ; Configure->App Filters.

; 6. Check the filter list.
if $CntStoreFilterList = ControlListView("Configure Filter List", "", 1000, "GetItemCount") Then	
	; 7. Delete all filters in the filter list.

	while ControlListView("Configure Filter List", "", 1000, "GetItemCount") >0
		ControlListView("Configure Filter List", "", 1000, "Select", 0)
		; Click 'Delete' button in 'Filter List' groupbox.
		ControlClick("Configure Filter List","Delete", 1002) ;   1002 is the ID for "Delete" button in Filter List groupbox.
	WEnd 

	; 8. Select OK.
	ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.
	
	; 9. Invoke the dialog again and check the filter list content.
	WinWaitActive("BUSMASTER","") 
	; Select Configuration -> App Filters.
	Send("!ca") ; Configure->App Filters.
	
	; the filter list should be empty.
	if ControlListView("Configure Filter List", "", 1000, "GetItemCount") = 0 Then
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
