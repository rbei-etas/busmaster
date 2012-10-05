#Include <GuiListView.au3>
$sendMsg1=0
$recMsg1=0
$sendMsg2=0
$recMsg1=0
Dim $rTime1
Dim $sTime1

ConsoleWrite("****Start : TC_UC2_05.au3****"&@CRLF)
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
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")
	_txMSG("Monoshot",$Count)
	ControlClick("BUSMASTER","",1015,"left")
	sleep(1000)
	if winexists("BUSMASTER","Do you want to save changes?") Then
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
	EndIf
Endif
if winexists("BUSMASTER") Then
;~ 	WinMenuSelectItem("BUSMASTER","","&Window","&1")
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Message Window","&Overwrite")
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Connect")
	sleep(1000)
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(1000)
	sleep(5000)
	send("{ESC}")
	WinMenuSelectItem("BUSMASTER","","F&unctions","D&isconnect	ESC")
	sleep(1000)
EndIf
ConsoleWrite("****End : TC_UC2_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********UseCase 2 Script Execution Ended************"&@CRLF)