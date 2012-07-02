;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; Test Cases:		Configure Transmission Messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===


ConsoleWrite("****Start : TC_UC2_04.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase1")
	if $crashRes=1 Then
		_writeCrashRes(14,10)
	Endif
	_associateDB("&File","AutoitTest.dbf")
	if $crashRes=1 Then
		_writeCrashRes(14,10)
	Endif
	if winexists("BUSMASTER") Then
	sleep(500)
	WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
	sleep(1000)
		if $crashRes=1 Then
			_writeCrashRes(14,10)
		Endif
	sleep(500)
		if winexists("Hardware Selection") Then
			ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			if $crashRes=1 Then
				_writeCrashRes(14,10)
			Endif
			ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			if $crashRes=1 Then
				_writeCrashRes(14,10)
			Endif
			$channelCount=ControlListView("Hardware Selection","","SysListView322","GetItemCount")
			ControlClick("Hardware Selection","&OK","[CLASS:Button; INSTANCE:4]")
			sleep(1000)
			if $crashRes=1 Then
				_writeCrashRes(14,10)
			Endif
		EndIf
	EndIf
endif
$Count=2
WinMenuSelectItem("BUSMASTER","","&Configure","&Tx Messages")
if $crashRes=1 Then
	_writeCrashRes(14,10)
Endif
_txMSG("Monoshot",$Count)
if $crashRes=1 Then
	_writeCrashRes(14,10)
Endif
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
	if $crashRes=1 Then
		_writeCrashRes(14,10)
	Endif
	if winexists("BUSMASTER","Do you want to save changes?") Then
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
		if $crashRes=1 Then
			_writeCrashRes(14,10)
		Endif
	EndIf
EndIf
ConsoleWrite("$blocks : "&$blocks&@CRLF)
ConsoleWrite("$msgCount : "&$msgCount&@CRLF)
if $blocks=$Count and $msgCount>0 Then
	_ExcelWriteCell($oExcel, "Pass", 14, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 14, 10)
EndIf
ConsoleWrite("****End : TC_UC2_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)