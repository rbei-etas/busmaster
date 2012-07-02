;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; TestCase No.:		TC_03
; Test Cases:		Hardware Switching
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC3_03.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase1")
	if $crashRes=1 Then
		_writeCrashRes(18,10)
	Endif
EndIf
if winexists("BUSMASTER") Then
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(18,10)
	Endif
	if winexists("Hardware Selection") Then
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		if $crashRes=1 Then
			_writeCrashRes(18,10)
		Endif
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		if $crashRes=1 Then
			_writeCrashRes(18,10)
		Endif
		$channelCount=ControlListView("Hardware Selection","","SysListView322","GetItemCount")
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
		if $crashRes=1 Then
			_writeCrashRes(18,10)
		Endif
	EndIf
EndIf
ConsoleWrite("$channelCount : "&$channelCount&@CRLF)
if $channelCount=2 Then
	_ExcelWriteCell($oExcel, "Pass", 18, 10)
	Else
	_ExcelWriteCell($oExcel, "Fail", 18, 10)
EndIf
ConsoleWrite("****End : TC_UC3_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)