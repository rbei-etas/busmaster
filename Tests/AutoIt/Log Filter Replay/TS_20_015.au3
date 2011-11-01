; === Test Cases/Test Data ===
; Test Case Number:	TS_20_015
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		display Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_015"
$Result = "Test did not finish"

WinWaitActive("Configure Message Display- CAN","",5) 
; 2. Select Configure Button.
ControlClick("Configure Message Display- CAN", "", 1031)	;1031 is the ID for 'Configure' button in Filter tab of 'Configure Message Display- CAN' dialog.

; 3. Check for filter configuration dialog.
If WinWaitActive("Filter Selection Dialog","",5) Then
	
	; 4. Select few filters from the available list of filters and note down the selected filter list.
	ControlClick("Filter Selection Dialog", "", 32946) ;32946 is the ID for 'Add All' button in 'Filter Selection Dialog'.
	;1070 is the ID for 'Selected Filters' list in 'Filter Selection Dialog'.
	$cntFilterAdded = ControlListView("Filter Selection Dialog", "", 1070, "GetItemCount") 
	; 5. Select OK to save changes.
	ControlClick("Filter Selection Dialog", "", 1) ;1 is the ID for 'OK' button in 'Filter Selection Dialog'.
	WinWaitActive("Configure Message Display- CAN","",5) 
	; 6. Check the message filter list
	;  the modifications done in the filter configuration dialog will reflect.
	If ControlListView("Configure Message Display- CAN", "", 1226, "GetItemCount") = $cntFilterAdded Then	
		$Result = "ok"			
	Else
		$Result = "Failed"
	EndIf
Else
	$Result = "Failed"
EndIf
WinClose("Configure Message Display- CAN")

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
