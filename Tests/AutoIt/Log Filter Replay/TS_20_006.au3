; === Test Cases/Test Data ===
; Test Case Number:	TS_20_006
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Adding Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_006"
$Result = "Test did not finish"

; Select a DB message name from the Combo box.
ControlCommand("Configure Filter List", "", 1004, "SetCurrentSelection", 0)	

; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

;a filter will be added to the list with the selected message name
$CntFilterDetails = ControlListView("Configure Filter List", "", 1003, "GetItemCount");1003 is the ID for List view in Filter Details groupbox.
If $CntFilterDetails > 0 Then
	$strAddedItem = ControlListView("Configure Filter List", "", 1003, "GetText", 0, 0)
	$strSelItem   = ControlCommand("Configure Filter List", "", 1004, "GetCurrentSelection")		
	Local $MsgName = StringSplit($strSelItem, "]")
	$blnResult = StringInStr($strAddedItem, $MsgName[2])
	if $blnResult > 0 Then
		$Result = "ok"
	EndIf
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
