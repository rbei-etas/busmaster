; === Test Cases/Test Data ===
; Test Case Number:	TS_20_022
; Feature:			Filter
; Critical (C):		Y
; Test Cases:		Check for stop filter mode - Message filter
; Test Strategy:	Black Box
; Test Data:		-
$Number = "TS_20_022"
$Result = "Test did not finish"
WinActivate("BUSMASTER","") 
Send("!co") ; Configure -> Log

Sleep(100)
; Should be in shown state
if ControlCommand("Configure Logging for CAN", "","", "IsVisible") = 1 Then 	
	WinActivate("Configure Logging for CAN","")
	; Click on 'Add' log option.
	ControlClick("Configure Logging for CAN", "Add", 1303)
	; Click on 'Filter' option.
	ControlClick("Configure Logging for CAN", "Add", 1121)
	If WinWaitActive("Filter Selection Dialog","",5) Then
		; Select few filters from the available list of filters and note down the selected filter list.
		ControlClick("Filter Selection Dialog", "", 32946) ;32946 is the ID for 'Add All' button in 'Filter Selection Dialog'.
		; Store the log file path.
		$strLogFile = ControlGetText("Configure Logging for CAN", "", 1062) ;1062 is the ID for log file edit control in 'Configure logging for CAN' dialog.
		ControlClick("Filter Selection Dialog", "", 1) ;1 is the ID for 'OK' button in 'Filter Selection Dialog'.
	Else
		$Result = "Failed"
	EndIf
	ControlClick("Configure Logging for CAN", "OK",  1)	
Else
	$Result = "Failed"
EndIf

WinActivate("BUSMASTER","") 
; 4. Enable Start Logging
Send("!ul")	; Functions -> Start Logging
; Connect 
Send("!uc")	; Functions -> Connect
; Now enable display filter button in the toolbar.
; 5. Enable Log filter
Send("!ufl") ; Funtions -> Enable Filter -> Log
; 6. Send some messages across CAN bus, which shall contain a few messages from the filter list"
Send("!utn")	; Functions -> Transmit -> Normal Blocks
Sleep(1000)	;Sleep for 1 second
; Stop message transmission
Send("!utn")	; Functions -> Transmit -> Normal Blocks
; Disconnect 
Send("!ui")	; Functions -> Disconnect
; Disable Log filter
Send("!ufl") ; Funtions -> Enable Filter -> Log
; Disable Start Logging
Send("!ul")	; Functions -> Start Logging

; Open Log File
ShellExecute("notepad.exe", $strLogFile)

if MsgBox(1, "Verify Test Case: TS- 22", "Verify the log file which is opened for Msg 0x20. If it doesn't contain 0x20 press'OK' else 'Cancel'.") = 1 Then 
	$Result = "ok"	
Else	; Cancel selected
	$Result = "Failed"
EndIf

; --- Write results ---
If $Result = "ok" Then
	$SuccessCount += 1
Else	
	FileWriteLine($file,  $Number & "," & $Result)	
EndIf
