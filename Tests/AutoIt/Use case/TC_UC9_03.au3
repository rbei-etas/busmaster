;=== Test Cases/Test Data ===
; UseCase 9:		J1939-Request Address
; Critical (C):		Y
; TestCase No.:		TC_UC9_03
; TestCases:		Hardware Switching, Activate and Go Online
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC9_03.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
	if $crashRes=1 Then
		_writeCrashRes(47,10)
	Endif
	_loadJ1939Database("J1939Test",25,10,8)
	if $crashRes=1 Then
		_writeCrashRes(47,10)
	Endif
	_associateDB("&J1939","J1939Test.dbf")
	if $crashRes=1 Then
		_writeCrashRes(47,10)
	Endif
EndIf
if winexists("BUSMASTER") Then
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
	sleep(1000)
	if winexists("Hardware Selection") Then
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		$channelCount=ControlListView("Hardware Selection","","SysListView322","GetItemCount")
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
	ConsoleWrite("$channelCount : "&$channelCount&@CRLF)
	_act_dctJ1939("&Activate")
	ConsoleWrite("$sel : "&$sel&@CRLF)
	_goOnline()
	ConsoleWrite("$gLine : "&$gLine&@CRLF)
EndIf
if $channelCount=1 and $sel=1 and $gLine=1 Then
	_ExcelWriteCell($oExcel, "Pass", 47, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 47, 10)
EndIf
ConsoleWrite("****End : TC_UC9_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)