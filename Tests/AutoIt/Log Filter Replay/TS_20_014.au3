; === Test Cases/Test Data ===
; Test Case Number:	TS_20_014
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		display Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_014"
$Result = "Test did not finish"

; Select OK.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.
WinWaitActive("BUSMASTER","",5) 

; 1. Select a new configuration file.
Send("!fgn") ; File->Configuration->New
Sleep(1000) ; 1 sec
if (WinActive("BUSMASTER", "Configuration File is modified")) Then
	Send("!y") ; Yes
EndIf
WinWaitActive("New Configuration Filename...", "", 2) ; wait till its open
Send("test.cfx") ; Filename
Send("!s") ; Save
if (WinWaitActive("New Configuration Filename...", "Do you want to replace it?", 2)) Then
	Send("!y") ; Yestest.cfx
EndIf

; Add few filters
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.

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

; Select OK.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.

WinWaitActive("BUSMASTER","",5) 
; 2. Select 'Configure -> Message Display'.
Send("!cm") 

; 3. Select Filter page.
; 4. Check the list of filters.
; Filter list should be empty as it is a new configuration.
;1226 is the ID for List control in Filter List groupbox of 'Configure Message Display- CAN' dialog.
If ControlListView("Configure Message Display- CAN", "", 1226, "GetItemCount") = 0 Then
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
