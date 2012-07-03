;Test Case Number:	TS_17_014
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Delete a Message Handler.
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

;#include<globalFunc.au3>
local $res
WinWaitActive("BUSMASTER","",5)
call ("_delBlockMsg")								; call delete message block function
send("!y")
