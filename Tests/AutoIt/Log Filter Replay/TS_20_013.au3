; === Test Cases/Test Data ===
; Test Case Number:	TS_20_013
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Navigation through filter list
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_013"
$Result = "Test did not finish"

;Add Filters and filter details
;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

;Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

; Select filter1 in 'Filter List' groupbox.
ControlListView("Configure Filter List", "", 1000, "Select", 1) ;1000 is the ID for List view in Filter List groupbox

; Select a DB message name from the Combo box.
ControlCommand("Configure Filter List", "", 1004, "SetCurrentSelection", 0)	

; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_013", 640, 80)
GUICtrlCreateLabel("Verify the test case :'Filter details for each entry in 'Filter List' groupbox should get updated upon selection of Filter from 'Filter List''.", 30, 10)
$okbutton1 = GUICtrlCreateButton("Success", 250, 40, 60,20)
$okbutton2 = GUICtrlCreateButton("Failure", 320, 40, 60,20)
GUISetState(@SW_SHOW)

While 1
  $msg = GUIGetMsg()

  Select
	Case $msg = $okbutton1
		$Result = "ok"
		ExitLoop
    Case $msg = $okbutton2
		$Result = "failed"		
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
		$Result = "failed"				
		ExitLoop
  EndSelect
WEnd 
GUIDelete(); 

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
