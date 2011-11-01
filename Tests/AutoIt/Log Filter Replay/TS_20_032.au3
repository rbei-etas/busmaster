; === Test Cases/Test Data ===
; Test Case Number:	TS_20_032
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for pass filter mode
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_032"
$Result = "Test did not finish"

WinActivate("BUSMASTER","")
;1. Invoke dialog box for filter configuration
WinWaitActive("BUSMASTER","",5) 
Send("!ca") ; Configure->App Filters.

;3. Select pass filter mode
GUICreate("BUSMASTER_Filter_Module_test_case_TS_20_032", 660, 80)
GUICtrlCreateLabel("User action needed : Select 'Type' value as 'Pass' for 'Filter_1' in 'Filter List' groupbox and the close this message box by pressing 'OK'.", 30, 10)
$okbutton1 = GUICtrlCreateButton("OK", 280, 40, 60,20)
GUISetState(@SW_SHOW)

While 1
  $msg = GUIGetMsg()

  Select
	Case $msg = $okbutton1
		$Result = "ok"
		ExitLoop
		
    Case $msg = $GUI_EVENT_CLOSE
		$Result = "failed"				
		ExitLoop
  EndSelect
WEnd 
GUIDelete(); 


;4. Confirm by selecting OK button
;Click 'OK' button to close the dialog.
ControlClick("Configure Filter List", "OK", 1256)		;1256 is the ID for OK button in Configure Filter List dialog.

; Add messages to transmission window.
WinWaitActive("BUSMASTER","",5) 
Send("!ct") ; Configure->Tx Messages.
WinWaitActive("BUSMASTER","Configure Transmission Messages",5) 

; Add Hex message ox16(22) to message list
ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "22")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.
; Click on Update button.
ControlClick("BUSMASTER","Configure Transmission Messages", 1014) ;1014 is the ID for 'Update' button in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1015) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.

;5. Enable message display / logging
WinWaitActive("BUSMASTER","",5) 
Send("!ufd") ; Funtions -> Enable Filter -> Display

WinWaitActive("BUSMASTER","",5) 
; Connect 
Send("!uc")	; Functions -> Connect

WinWaitActive("BUSMASTER","") 
;6. Send a some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second

; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect
; Disable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display
; Message window should not contain any messages as the configured messages in Tx Window are filtered.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") = 2 Then 
	$MsgID1 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 0, 5) ; 5 is the 'ID' column in message window.	
	$MsgID2 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 1, 5) ; 5 is the 'ID' column in message window.		
	If ( Number($MsgID1) > 16 And Number($MsgID1) < 21) And (Number($MsgID2) > 16 And Number($MsgID2) < 21) Then
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
