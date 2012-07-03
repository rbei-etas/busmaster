;Test Case Number:	TS_17_016
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Delete a Message Handler.
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===


local $res, $listViewCount
WinWaitActive("BUSMASTER","",5)
call ("_delBlockMsg"); call delete message block function
sleep(500)
send("!n")
$listViewCount1=ControlListView("BUSMASTER", "", "SysListView323", "GetItemCount")