;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; TestCase No.:		TC_02
; Test Cases:		Create or load and Associate database(dbf) file
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC1_02.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase1")
EndIf
_loadDatabase("AutoitTest")
send("!FDS")								;Save database
sleep(500)
send("!FDC")								;Close database
_associateDB("&File","AutoitTest.dbf")

ConsoleWrite("$lDb : "&$lDb&@CRLF)
ConsoleWrite("$msgCount : "&$msgCount&@CRLF)
ConsoleWrite("$sigCount : "&$sigCount&@CRLF)
ConsoleWrite("$flag : "&$flag&@CRLF)

ConsoleWrite("****End : TC_UC1_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)

