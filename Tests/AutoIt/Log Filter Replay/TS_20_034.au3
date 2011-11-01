; === Test Cases/Test Data ===
; Test Case Number:	TS_20_034
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Applying display Filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_034"
$Result = "Test did not finish"

; 3. Disable display filtering if it is enabled.
; Already in disabled state.

WinActivate("BUSMASTER","") 
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

; Message window should contain all 3 messages as configured messages in Tx Window.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") = 3 Then 
	$Result = "ok"		
	; Getting column values in Msg wnd not working..Need to work on this logic.
	;$MsgID1 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 0, 5) ; 5 is the 'ID' column in message window.	
	;$MsgID2 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 1, 5) ; 5 is the 'ID' column in message window.	
	;$MsgID3 = ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetText", 2, 5) ; 5 is the 'ID' column in message window.	
	;If (Number($MsgID1) <= 16 Or Number($MsgID1) >= 21) Or (Number($MsgID2) <= 16 Or Number($MsgID2) >= 21) Or (Number($MsgID1) <= 16 Or Number($MsgID1) >= 21) Then
	;	$Result = "ok"			
	;Else
	;	$Result = "Failed"	
	;EndIf	
Else
	$Result = "Failed"	
EndIf	

WinActivate("BUSMASTER","") 
; Connect 
Send("!uc")	; Functions -> Connect
; 6. Now enable display filter button in the toolbar.
; Enable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display
; Send some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second
; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect
; Disable display filter
Send("!ufd") ; Funtions -> Enable Filter -> Display


; Message window should contain 2 messages as configured messages in Tx Window which are in 'Pass' range of 0x10 - 0x15.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") = 2 Then 
	If $Result = "ok" Then
		$Result = "ok"			
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

; --- Closure ---
;WinClose("BUSMASTER")
ControlClick("BUSMASTER", "", 7)	;7 is the ID for 'No' button
