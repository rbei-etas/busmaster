; === Test Cases/Test Data ===
; Test Case Number:	TS_XX_034
; Feature:			Menu Item functionalities
; Critical (C):		Y
; Test Cases:		Functions -> Transmit -> Normal Blocks
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
$Number = "TS_XX_034"
$Result = "Test did not finish"

; --- Test SetUp ---
WinActivate("BUSMASTER","")

; Add messages to transmission window.
WinWaitActive("BUSMASTER","",5) 
Send("!ct") ; Configure->Tx Messages.
WinWaitActive("BUSMASTER","Configure Transmission Messages",5) 
ControlClick("BUSMASTER","Configure Transmission Messages", 1226) ;1226 is the ID for 'Add' button in 'Configure Transmission Messages' Dialog.

; Add Hex messages ox12(18) and 0x13(19) to message list
ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "18")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.

ControlSetText("BUSMASTER","Configure Transmission Messages", 1001, "19")	;1001 is the ID for "Name / ID" ComboBox in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1122) ;1122 is the ID for 'Add Message' button in 'Configure Transmission Messages' Dialog.
; Click on Update button.
ControlClick("BUSMASTER","Configure Transmission Messages", 1014) ;1014 is the ID for 'Update' button in 'Configure Transmission Messages' Dialog.
ControlClick("BUSMASTER","Configure Transmission Messages", 1015) ;1015 is the ID for 'Close' button in 'Configure Transmission Messages' Dialog.

; Connect 
Send("!uc")	; Functions -> Connect

WinActivate("BUSMASTER","")
;6. Send some messages across CAN bus, which shall contain a few messages from the filter list"

Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second

; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect

; Message window should mcontain essages which are configured in Tx Window.
If ControlListView("BUSMASTER", "Message Window - CAN", 200, "GetItemCount") > 0 Then 
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
