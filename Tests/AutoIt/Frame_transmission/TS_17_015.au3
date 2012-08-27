;Test Case Number:	TS_17_015
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Delete a Message Handler.
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===


local $res, $listViewCount
WinWaitActive("BUSMASTER","",5)
call ("_delBlockMsg")								; call delete_message_block function
send("!y")
$listViewCount1=ControlListView("BUSMASTER", "", "SysListView323", "GetItemCount")  
