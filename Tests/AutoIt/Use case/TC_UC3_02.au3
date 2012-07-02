;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; TestCase No.:		TC_02
; Test Cases:		Create or load and Associate database(dbf) file
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC3_02.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase1")
	if $crashRes=1 Then
		_writeCrashRes(17,10)
	Endif
EndIf
_loadDatabase("AutoitTest")
if $crashRes=1 Then
_writeCrashRes(17,10)
Endif
_associateDB("&File","AutoitTest.dbf")
if $crashRes=1 Then
_writeCrashRes(17,10)
Endif
ConsoleWrite("$lDb : "&$lDb&@CRLF)
ConsoleWrite("$msgCount : "&$msgCount&@CRLF)
ConsoleWrite("$sigCount : "&$sigCount&@CRLF)
ConsoleWrite("$flag : "&$flag&@CRLF)
if $lDb=1 and $msgCount=2 and $sigCount=4 and $flag=1 Then
	_ExcelWriteCell($oExcel, "Pass", 17, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 17, 10)
EndIf
ConsoleWrite("****End : TC_UC3_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)