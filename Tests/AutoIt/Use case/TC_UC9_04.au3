;=== Test Cases/Test Data ===
; UseCase 9:		J1939-Request Address
; Critical (C):		Y
; TestCase No.:		TC_UC9_04
; TestCases:		Transmit Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

Dim $RQST
ConsoleWrite("****Start : TC_UC9_04.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
	_loadJ1939Database("J1939Test",25,10,8)
	_associateDB("&J1939","J1939Test.dbf")
	if winexists("BUSMASTER") Then
		_act_dctJ1939("&Activate")
		ConsoleWrite("$sel : "&$sel&@CRLF)
		_goOnline()
		ConsoleWrite("$gLine : "&$gLine&@CRLF)
	EndIf
EndIf
if winexists("BUSMASTER") Then
	WinMenuSelectItem("BUSMASTER","","&J1939","&View","&Transmit Window")
	ControlCommand("J1939 Transmit Message Window","",1335,"Check")
	sleep(500)
	ControlCommand("J1939 Transmit Message Window","",1008,"Check")
	_overWriteMode()															; Call _overWriteMode function to disable overWriteMode
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Connect")
	sleep(1000)
	ControlClick("J1939 Transmit Message Window","",1011,"left")
	sleep(2000)
	Send("{TAB}")
	Send("{ENTER}")
	$hWnd=ControlGetHandle ("BUSMASTER","","SysListView321")
	$row2=_GUICtrlListView_GetItemTextString($hWnd,0)
	ConsoleWrite("$row2 : "&$row2 & @CRLF)
	$hWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($hWd,4,"left")
	sleep(1000)
	ControlClick("J1939 Transmit Message Window","",1001,"left")
EndIf
$RQST=StringSplit($row2,"|")

ConsoleWrite("****End : TC_UC9_04.au3****"&@CRLF)
ConsoleWrite("***********UseCase 9 Script Execution Started************"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)