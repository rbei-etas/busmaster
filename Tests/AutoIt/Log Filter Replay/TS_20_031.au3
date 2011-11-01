; === Test Cases/Test Data ===
; Test Case Number:	TS_20_031
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for stop filter mode - Message filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_031"
$Result = "Test did not finish"

;1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.

; Delete all filters in the filter list.

while ControlListView("Configure Filter List", "", 1000, "GetItemCount") > 0
	ControlListView("Configure Filter List", "", 1000, "Select", 0)
	; Click 'Delete' button in 'Filter List' groupbox.
	ControlClick("Configure Filter List","Delete", 1002) ;   1002 is the ID for "Delete" button in Filter List groupbox.
WEnd 

;2. Specify a filter list
;3. Select stop filter mode
; Select Add Button from left side.
ControlClick("Configure Filter List", "Add", 1228)		;1228 is the ID for Add button in Filter List groupbox.

; Select Range radio button and give a valid range
ControlCommand("Configure Filter List", "", 1254, "Check")	;1254 is the ID for "Range" Radio button in Filter Attributes groupbox.
; Specify a range such that number keyed in "From" field is greater than that in "To" text field
; Set Range as 0x10-0x15
ControlSetText("Configure Filter List", "", 1041, "10")	;1041 is the ID for "From" Edit control in Filter Attributes groupbox.
ControlSetText("Configure Filter List", "", 1042, "15")	    ;1042 is the ID for "To" Edit Control in Filter Attributes groupbox.

; Set Direction as 'All'
ControlCommand("Configure Filter List", "", 1011, "SetCurrentSelection", 2) ;1011 is the ID for "Direction" Combobox in Filter Attributes groupbox.
; Select Add button in 'Filter Attributes' groupbox.
ControlClick("Configure Filter List","Add", 3) ;   3 is the ID for "Add" button in Filter Attributes groupbox.

;4. Confirm by selecting OK button
;Click 'OK' button to close the dialog.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.

;5. Enable message display / logging
WinWaitActive("BUSMASTER","",5) 
; Select 'Configure -> Message Display'.
Send("!cm") 
WinWaitActive("Configure Message Display- CAN","",5) 
; Select Configure Button.
ControlClick("Configure Message Display- CAN", "", 1031)	;1031 is the ID for 'Configure' button in Filter tab of 'Configure Message Display- CAN' dialog.
WinWaitActive("Filter Selection Dialog","",5)
; Select few filters from the available list of filters and note down the selected filter list.
ControlClick("Filter Selection Dialog", "", 32946) ;32946 is the ID for 'Add All' button in 'Filter Selection Dialog'.
; Select OK to save changes.
ControlClick("Filter Selection Dialog", "", 1) ;1 is the ID for 'OK' button in 'Filter Selection Dialog'.
WinWaitActive("Configure Message Display- CAN","",5) 
ControlClick("Configure Message Display- CAN", "", 1) ;1 is the ID for 'OK' button in 'Configure Message Display- CAN' dialog.

; Add messages to transmission window.
WinWaitActive("BUSMASTER","",5) 
Send("!ct") ; Configure->Tx Messages.
WinWaitActive("BUSMASTER","Configure Transmission Messages",5) 
;ControlClick("BUSMASTER","Configure Transmission Messages", 1226) ;1226 is the ID for 'Add' button in 'Configure Transmission Messages' Dialog.

; Delete existing messages if any 
ControlClick("BUSMASTER","Configure Transmission Messages", 1231) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.
Sleep(200)
If WinActive("BUSMASTER", "Are you sure you want to delete?") Then
	Send("!y")
EndIf

; Add Hex messages ox12(18) and 0x13(19) to message list
ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "18")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.

ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "19")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.
; Click on Update button.
ControlClick("BUSMASTER","Configure Transmission Messages", 1014) ;1014 is the ID for 'Update' button in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1015) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.

WinWaitActive("BUSMASTER","",5) 
; Enable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display

WinWaitActive("BUSMASTER","",5) 
; Connect 
Send("!uc")	; Functions -> Connect

WinWaitActive("BUSMASTER","") 
;6. Send some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second

; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect
; Disable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display

; Message window should not contain any messages as the configured messages in Tx Window are filtered.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") = 0 Then 
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
