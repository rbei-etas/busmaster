;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; Test Cases:		Configure Transmission Messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC1_04.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase1")
	_associateDB("&File","AutoitTest.dbf")
	if winexists("BUSMASTER") Then
		sleep(1000)
		WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
	sleep(500)
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
Endif
$Count=2
WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")

_txMSG("Cyclic",$Count)

if winexists("BUSMASTER") Then
	$blocks=ControlListView("BUSMASTER","","SysListView323","GetItemCount")
	for $i=0 to $Count-1
		$msgCount=0
		ControlListView("BUSMASTER","","SysListView323","Select",$i,$i)
		$b1Msgs=ControlListView("BUSMASTER","","SysListView323","GetItemCount")
		$msgCount=$msgCount+1
	Next
	ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:23]","left")
	sleep(1000)

	if winexists("BUSMASTER","Do you want to save changes?") Then
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
	EndIf
	sleep(1000)

EndIf
ConsoleWrite("$blocks : "&$blocks&@CRLF)
ConsoleWrite("$msgCount : "&$msgCount&@CRLF)

ConsoleWrite("****End : TC_UC1_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)

