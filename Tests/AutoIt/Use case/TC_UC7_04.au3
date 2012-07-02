;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; Test Cases:		Confiugre Log
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC7_04.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
	if $crashRes=1 Then
		_writeCrashRes(38,10)
	Endif
	_loadJ1939Database("J1939Test",25,10,8)
	if $crashRes=1 Then
		_writeCrashRes(38,10)
	Endif
	_associateDB("&J1939","J1939Test.dbf")
	if $crashRes=1 Then
		_writeCrashRes(38,10)
	Endif
	if winexists("BUSMASTER") Then
		_hdWareSelect()
		ConsoleWrite("$channelCount : "&$channelCount&@CRLF)
		_act_dctJ1939("&Activate")
		ConsoleWrite("$sel : "&$sel&@CRLF)
		_goOnline()
		ConsoleWrite("$gLine : "&$gLine&@CRLF)
	EndIf
EndIf
$logMenu=0
$addBtn=0
$oK=0
if winexists("BUSMASTER") Then
	sleep(2000)
	$logMenu=WinMenuSelectItem("BUSMASTER","","&J1939","&Configure","&Log")
	if winexists("Configure Logging for J1939") Then
		$addBtn=ControlClick("Configure Logging for J1939","&Add","")
		sleep(1000)
		$enableLog=ControlCommand("Configure Logging for J1939","",1092,"Check")
		sleep(1000)
		$oK=ControlClick("Configure Logging for J1939","&OK","[CLASS:Button; INSTANCE:14]")
		sleep(1000)
	EndIf
EndIf
ConsoleWrite("$logMenu : "&$logMenu&@CRLF)
ConsoleWrite("$addBtn : "&$addBtn&@CRLF)
ConsoleWrite("$enableLog : "&$enableLog&@CRLF)
ConsoleWrite("$oK : "&$oK&@CRLF)
if ($logMenu and $addBtn and $oK)=1 then
	_ExcelWriteCell($oExcel, "Pass", 38, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 38, 10)
EndIf

ConsoleWrite("****End : TC_UC7_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)



