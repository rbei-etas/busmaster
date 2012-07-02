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
	if $crashRes=1 Then
		_writeCrashRes(50,10)
	Endif
EndIf
_loadJ1939Database("J1939Test",25,10,8)
if $crashRes=1 Then
	_writeCrashRes(50,10)
Endif
_associateDB("&J1939","J1939Test.dbf")
if $crashRes=1 Then
	_writeCrashRes(50,10)
Endif
ConsoleWrite("$msgCount : "&$msgCount&@CRLF)
ConsoleWrite("$msg1SigCount : "&$msg1SigCount&@CRLF)
ConsoleWrite("$flag : "&$flag&@CRLF)
if $msgCount=1 and $flag=1 Then
	_ExcelWriteCell($oExcel, "Pass", 50, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 50, 10)
EndIf
ConsoleWrite("****End : TC_UC10_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)