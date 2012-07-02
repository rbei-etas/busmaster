;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; Test Cases:		Confiugre Log
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC3_05.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase1")
	if $crashRes=1 Then
		_writeCrashRes(20,10)
	Endif
	_associateDB("&File","AutoitTest.dbf")
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(20,10)
	Endif
	if winexists("BUSMASTER") Then
		sleep(1000)
		WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
		if $crashRes=1 Then
			_writeCrashRes(20,10)
		Endif
	sleep(500)
		if winexists("Hardware Selection") Then
			ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			if $crashRes=1 Then
				_writeCrashRes(20,10)
			Endif
			ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			if $crashRes=1 Then
				_writeCrashRes(20,10)
			Endif
			$channelCount=ControlListView("Hardware Selection","","SysListView322","GetItemCount")
			ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
			sleep(1000)
			if $crashRes=1 Then
				_writeCrashRes(19,10)
			Endif
		EndIf
	EndIf
	WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")
	if $crashRes=1 Then
		_writeCrashRes(20,10)
	Endif
	_txMSG("Cyclic",$Count)
	if $crashRes=1 Then
		_writeCrashRes(20,10)
	Endif
	ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:23]","left")
	if winexists("BUSMASTER","Do you want to save changes?") Then
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
	EndIf
Endif
$logMenu=0
$addBtn=0
$oK=0
if winexists("BUSMASTER") Then
	sleep(2000)
	$logMenu=WinMenuSelectItem("BUSMASTER","","&Configure","L&og")
	if winexists("Configure Logging for CAN") Then
		$addBtn=ControlClick("Configure Logging for CAN","&Add","")
		sleep(1000)
		$enableLog=ControlCommand("Configure Logging for CAN","",1092,"Check")
		sleep(1000)
		$oK=ControlClick("Configure Logging for CAN","&OK","[CLASS:Button; INSTANCE:14]")
	EndIf
EndIf
ConsoleWrite("$logMenu : "&$logMenu&@CRLF)
ConsoleWrite("$addBtn : "&$addBtn&@CRLF)
ConsoleWrite("$enableLog : "&$enableLog&@CRLF)
ConsoleWrite("$oK : "&$oK&@CRLF)
if ($logMenu and $addBtn and $oK=1) then
	_ExcelWriteCell($oExcel, "Pass", 20, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 20, 10)
EndIf

ConsoleWrite("****End : TC_UC3_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)



