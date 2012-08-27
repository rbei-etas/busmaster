;=== Test Cases/Test Data ===
; UseCase 1:		J1939-Claim Address
; Critical (C):		Y
; TestCase No.:		TC_UC8_03
; TestCases:		Hardware Switching, Activate and Go Online
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC8_03.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
	_loadJ1939Database("J1939Test",25,10,8)
	_associateDB("&J1939","J1939Test.dbf")
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
ConsoleWrite("****End : TC_UC8_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)