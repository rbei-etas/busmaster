;=== Test Cases/Test Data ===
; UseCase 1:		transmit J1939 Message Transmission - Request PGN
; Critical (C):		Y
; TestCase No.:		TC_UC5_03
; TestCases:		Hardware Switching, Activate and Go Online
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC5_03.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
	_loadJ1939Database("J1939Test",25,10,8)
	_associateDB("&J1939","J1939Test.dbf")
EndIf
if winexists("BUSMASTER") Then
	_hdWareSelect()
	ConsoleWrite("$channelCount : "&$channelCount&@CRLF)
	_act_dctJ1939("&Activate")
	ConsoleWrite("$sel : "&$sel&@CRLF)
	_goOnline()
	ConsoleWrite("$gLine : "&$gLine&@CRLF)
EndIf
ConsoleWrite("****End : TC_UC5_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)