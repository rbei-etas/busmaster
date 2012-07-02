;=== Test Cases/Test Data ===
; UseCase 1:		transmit J1939 Message Transmission - Data
; Critical (C):		Y
; TestCase No.:		TC_UC5_04
; TestCases:		Transmit Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC7_05.au3****"&@CRLF)
ReDim $Tx[1000],$Rx[1000]
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
	if $crashRes=1 Then
		_writeCrashRes(39,10)
	Endif
	_loadJ1939Database("J1939Test",25,10,8)
	if $crashRes=1 Then
		_writeCrashRes(39,10)
	Endif
	_associateDB("&J1939","J1939Test.dbf")
	if $crashRes=1 Then
		_writeCrashRes(39,10)
	Endif
EndIf
_J1939tMsgWin("Data")
if winexists("BUSMASTER") Then
	sleep(2000)
	$hWnd=ControlGetHandle ("BUSMASTER","","SysListView321")
	$rCount=_GUICtrlListView_GetItemCount($hWnd)
	ConsoleWrite("$rCount : "&$rCount & @CRLF)
	if $rCount=2 then
		$row1=_GUICtrlListView_GetItemTextString($hWnd,0)
		$row2=_GUICtrlListView_GetItemTextString($hWnd,1)
		ConsoleWrite("$row1 : "&$row1 & @CRLF)
		ConsoleWrite("$row2 : "&$row2 & @CRLF)
		$Tx=StringSplit($row1,"|")
		ConsoleWrite("$Tx[2] : "&$Tx[2] & @CRLF)
		ConsoleWrite("$Tx[3] : "&$Tx[3] & @CRLF)
		ConsoleWrite("$Tx[5] : "&$Tx[5] & @CRLF)
		ConsoleWrite("$Tx[6] : "&$Tx[6] & @CRLF)
		ConsoleWrite("$Tx[7] : "&$Tx[7] & @CRLF)
		ConsoleWrite("$Tx[10] : "&$Tx[10] & @CRLF)
		ConsoleWrite("$Tx[11] : "&$Tx[11] & @CRLF)
		ConsoleWrite("$Tx[12] : "&$Tx[12] & @CRLF)
		$Rx=StringSplit($row2,"|")
		ConsoleWrite("$Rx[2] : "&$Rx[2] & @CRLF)
		ConsoleWrite("$Rx[3] : "&$Rx[3] & @CRLF)
		ConsoleWrite("$Rx[5] : "&$Rx[5] & @CRLF)
		ConsoleWrite("$Rx[6] : "&$Rx[6] & @CRLF)
		ConsoleWrite("$Rx[7] : "&$Rx[7] & @CRLF)
		ConsoleWrite("$Rx[10] : "&$Rx[10] & @CRLF)
		ConsoleWrite("$Rx[11] : "&$Rx[11] & @CRLF)
		ConsoleWrite("$Rx[12] : "&$Rx[12] & @CRLF)
		if ($Tx[7] and $Rx[7]="DATA") and ($Tx[10] and $Rx[10]=007)and ($Tx[12] and $Rx[12]=8) Then
			$match=1
		Else
			$match=0
		EndIf
	Else
		$test="Fail"
	EndIf
EndIf
$hWd=ControlGetHandle("BUSMASTER","",128)
_GUICtrlToolbar_ClickIndex($hWd,4,"left")
sleep(1000)
if $Tx[3]=1 and $Rx[3]=2 and $Tx[11]="Tx" and $Rx[11]="Rx" and $match=1 Then
	_ExcelWriteCell($oExcel, "Pass", 39, 10)
Elseif $test="Fail" then
	_ExcelWriteCell($oExcel, "Fail", 39, 10)
	_ExcelWriteCell($oExcel, "Rows in message window is incorrect", 39, 9)
Else
	_ExcelWriteCell($oExcel, "Fail", 39, 10)
EndIf
ConsoleWrite("****End : TC_UC7_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
