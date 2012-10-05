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
	_loadConfig("UseCase3")
EndIf
if winexists("BUSMASTER") Then
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
	sleep(1000)
	if winexists("Hardware Selection") Then
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
		sleep(1000)
		$channelCount=ControlListView("Hardware Selection","","SysListView322","GetItemCount")
		ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
		sleep(1000)
	EndIf
EndIf
ConsoleWrite("$channelCount : "&$channelCount&@CRLF)

ConsoleWrite("****End : TC_UC3_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)