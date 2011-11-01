; === Test Cases/Test Data ===
; Test Case Number:	TS_20_033
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Saving of filter mode in the configuration file
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_033"
$Result = "Test did not finish"

WinActivate("BUSMASTER","")
WinClose("BUSMASTER")
ControlClick("BUSMASTER", "", 6)	;6 is the ID for 'Yes' button in Save configuration confirm message box.

MsgBox(0, "User Action-TS 33", "Restart BUSMASTER Application and press OK.")

WinActivate("BUSMASTER","") 
Send("!ca") ; Configure->App Filters.
$strFilterMode = ControlListView("Configure Filter List", "", 1000, "GetText", 0, 1) ;1000 is the ID for List view in Filter List groupbox.
WinClose("Configure Filter List")

; Check if the filter mode stored to configuration retains its filter state of 'Pass'.
If $strFilterMode = "Pass" Then
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
