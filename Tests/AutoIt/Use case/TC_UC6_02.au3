;=== Test Cases/Test Data ===
; UseCase 1:		transmit J1939 Message Transmission - Broadcast
; Critical (C):		Y
; TestCase No.:		TC_UC6_02
; TestCases:		Create or load Database(dbf) files
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC6_02.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase6")
EndIf
_loadJ1939Database("J1939BrdCast",255,10,8)
_associateDB("&J1939","J1939BrdCast.dbf")
ConsoleWrite("$msgCount : "&$msgCount&@CRLF)
ConsoleWrite("$msg1SigCount : "&$msg1SigCount&@CRLF)
ConsoleWrite("$flag : "&$flag&@CRLF)
ConsoleWrite("****End : TC_UC6_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)