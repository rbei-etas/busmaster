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
	if $crashRes=1 Then
		_writeCrashRes(32,10)
	Endif
EndIf
_loadJ1939Database("J1939BrdCast",255,1024,64)
if $crashRes=1 Then
	_writeCrashRes(32,10)
Endif
_associateDB("&J1939","J1939BrdCast.dbf")
if $crashRes=1 Then
	_writeCrashRes(32,10)
Endif
ConsoleWrite("$msgCount : "&$msgCount&@CRLF)
ConsoleWrite("$msg1SigCount : "&$msg1SigCount&@CRLF)
ConsoleWrite("$flag : "&$flag&@CRLF)
if $msgCount=1 and $flag=1 Then
	_ExcelWriteCell($oExcel, "Pass", 32, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 32, 10)
EndIf
ConsoleWrite("****End : TC_UC6_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)