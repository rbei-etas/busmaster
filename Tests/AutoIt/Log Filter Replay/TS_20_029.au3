; === Test Cases/Test Data ===
; Test Case Number:	TS_20_029
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for Validation - Message Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_029"
$Result = "Test did not finish"

; 1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.


GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_029", 640, 80)
GUICtrlCreateLabel("Verify the test case :'Enter -1 in 'ID' combobox and check if the 'Invalid Message ID' waring is displayed in status text bar'.", 30, 10)
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
WinClose("Configure Filter List")

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
