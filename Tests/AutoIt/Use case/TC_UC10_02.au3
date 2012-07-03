;=== Test Cases/Test Data ===
; UseCase 9:		J1939-Command Address
; Critical (C):		Y
; TestCase No.:		TC_UC10_02
; TestCases:		Create or load database files
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC10_02.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
EndIf
_loadJ1939Database("J1939Test",25,10,8)
_associateDB("&J1939","J1939Test.dbf")
ConsoleWrite("$msgCount : "&$msgCount&@CRLF)
ConsoleWrite("$msg1SigCount : "&$msg1SigCount&@CRLF)
ConsoleWrite("$flag : "&$flag&@CRLF)

ConsoleWrite("****End : TC_UC10_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)