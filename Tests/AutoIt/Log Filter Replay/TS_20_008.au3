; === Test Cases/Test Data ===
; Test Case Number:	TS_20_008
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Filter Attributes
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_008"
$Result = "Test did not finish"

GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_008", 630, 80)
GUICtrlCreateLabel("Verify the test case :'Changing message attributes of existing filters should be allowed and warnings should be displayed.'", 30, 10)
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
