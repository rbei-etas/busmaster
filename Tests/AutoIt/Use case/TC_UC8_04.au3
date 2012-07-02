;=== Test Cases/Test Data ===
; UseCase 1:		J1939-Claim Address
; Critical (C):		Y
; TestCase No.:		TC_UC8_04
; TestCases:		Transmit window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC8_04.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
	if $crashRes=1 Then
		_writeCrashRes(44,10)
	Endif
	_loadJ1939Database("J1939Test",25,10,8)
	if $crashRes=1 Then
		_writeCrashRes(44,10)
	Endif
	_associateDB("&J1939","J1939Test.dbf")
	if $crashRes=1 Then
		_writeCrashRes(44,10)
	Endif
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
EndIf
if winexists("BUSMASTER") Then
	WinMenuSelectItem("BUSMASTER","","&J1939","&View","&Transmit Window")
	ControlCommand("J1939 Transmit Message Window","",1335,"Check")
	sleep(500)
	ControlCommand("J1939 Transmit Message Window","",1012,"Check")
	ControlSetText("J1939 Transmit Message Window","",1336,"AA")
	_overWriteMode()															; Call _overWriteMode function to disable overWriteMode
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Connect")
	sleep(1000)
	ControlClick("J1939 Transmit Message Window","",1011,"left")
	sleep(2000)
	Send("{TAB}")
	Send("{ENTER}")
	$hWnd=ControlGetHandle ("BUSMASTER","","SysListView321")
	$rCount=_GUICtrlListView_GetItemCount($hWnd)
	$row1=_GUICtrlListView_GetItemTextString($hWnd,0)
	ConsoleWrite("$row1 : "&$row1 & @CRLF)
	$hWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($hWd,4,"left")
	sleep(1000)
	;ControlClick("J1939 Transmit Message Window","",101,"left")
EndIf
$ACL=StringSplit($row1,"|")
if $ACL[5]=060928 and $ACL[7]="ACL" and $ACL[8]=170 and $ACL[11]="Tx" and $ACL[12]=8 then
	_ExcelWriteCell($oExcel, "Pass", 44, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 44, 10)
EndIf
ConsoleWrite("****End : TC_UC8_04.au3****"&@CRLF)
ConsoleWrite("***********UseCase 8 Script Execution Started************"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)

