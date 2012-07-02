;=== Test Cases/Test Data ===
; UseCase 1:		transmit J1939 Message Transmission - Broadcast
; Critical (C):		Y
; TestCase No.:		TC_UC6_03
; TestCases:		Hardware Switching, Activate and Go Online
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC6_03.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase6")
	if $crashRes=1 Then
		_writeCrashRes(33,10)
	Endif
	_loadJ1939Database("J1939BrdCast",255,1024,64)
	if $crashRes=1 Then
		_writeCrashRes(33,10)
	Endif
	_associateDB("&J1939","J1939BrdCast.dbf")
	if $crashRes=1 Then
		_writeCrashRes(33,10)
	Endif
EndIf
if winexists("BUSMASTER") Then
	_hdWareSelect()
	ConsoleWrite("$channelCount : "&$channelCount&@CRLF)
	_act_dctJ1939("&Activate")
	ConsoleWrite("$sel : "&$sel&@CRLF)
	_goOnline()
	ConsoleWrite("$gLine : "&$gLine&@CRLF)
EndIf
if $channelCount=2 and $sel=1 and $gLine=1 Then
	_ExcelWriteCell($oExcel, "Pass", 33, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 33, 10)
EndIf
ConsoleWrite("****End : TC_UC6_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)