;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; Test Cases:		Message Transmission
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
#Include <GuiListView.au3>
$sendMsg1=0
$recMsg1=0
$sendMsg2=0
$recMsg1=0

ConsoleWrite("****Start : TC_UC3_06.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase1")
	if $crashRes=1 Then
		_writeCrashRes(21,10)
	Endif
	_associateDB("&File","AutoitTest.dbf")
	if $crashRes=1 Then
		_writeCrashRes(21,10)
	Endif
	if winexists("BUSMASTER") Then
		sleep(1000)
		WinMenuSelectItem("BUSMASTER","","&Configure","&Hardware Interface","&Kvaser CAN")
		if $crashRes=1 Then
			_writeCrashRes(21,10)
		Endif
	sleep(500)
		if winexists("Hardware Selection") Then
			ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			if $crashRes=1 Then
				_writeCrashRes(21,10)
			Endif
			ControlClick("Hardware Selection","&Select","[CLASS:Button; INSTANCE:2]")
			sleep(1000)
			if $crashRes=1 Then
				_writeCrashRes(21,10)
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
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(21,10)
	Endif
	_txMSG("Cyclic",$Count)
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(21,10)
	Endif
	ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:23]","left")
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(21,10)
	Endif
	if winexists("BUSMASTER","Do you want to save changes?") Then
		ControlClick("BUSMASTER","","[CLASS:Button; INSTANCE:2]","left")
		sleep(1000)
		if $crashRes=1 Then
			_writeCrashRes(21,10)
		Endif
	EndIf
Endif
if winexists("BUSMASTER") Then
	WinMenuSelectItem("BUSMASTER","","&Window","&1")
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(21,10)
	Endif
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Message Window","&Overwrite")
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(21,10)
	Endif
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Connect")
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(21,10)
	Endif
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(21,10)
	Endif
	sleep(5000)
	send("{ESC}")
	WinMenuSelectItem("BUSMASTER","","F&unctions","D&isconnect	ESC")
 if winexists("BUSMASTER") Then
	$hWnd=ControlGetHandle ("BUSMASTER","","SysListView321")
	$msg1Tx=_GUICtrlListView_GetItemTextString($hWnd, 0)
	$msg1Rx=_GUICtrlListView_GetItemTextString($hWnd, 1)
	$msg2Tx=_GUICtrlListView_GetItemTextString($hWnd, 2)
	$msg2Rx=_GUICtrlListView_GetItemTextString($hWnd, 3)
	sleep(1000)
	ConsoleWrite("$msg1Tx : "&$msg1Tx & @CRLF)
	ConsoleWrite("$msg1Rx : "&$msg1Rx & @CRLF)
	ConsoleWrite("$msg2Tx : "&$msg2Tx & @CRLF)
	ConsoleWrite("$msg2Rx : "&$msg2Rx & @CRLF)

	$sTime=StringSplit($msg1Tx,"|")
	ConsoleWrite($sTime[2] & @CRLF)		;time
	ConsoleWrite($sTime[3] & @CRLF)		;type
	ConsoleWrite($sTime[4] & @CRLF)		;channel
	ConsoleWrite($sTime[5] & @CRLF)		;msgType
	ConsoleWrite($sTime[6] & @CRLF)		;msgID
	ConsoleWrite($sTime[7] & @CRLF)		;msgName
	$msg1sTime = StringReplace($sTime[2], ":", "")
	ConsoleWrite($msg1sTime & @CRLF)
	if $sTime[3]="Tx" and $sTime[4]=1 and $sTime[5]="s" and $sTime[6]=0081 and $sTime[7]="msg1" Then
		$sendMsg1=1
		ConsoleWrite("sendMsg1= "&$sendMsg1 & @CRLF)
	EndIf

	$rTime=StringSplit($msg1Rx,"|")
	ConsoleWrite($rTime[2] & @CRLF)
	ConsoleWrite($rTime[3] & @CRLF)
	ConsoleWrite($rTime[4] & @CRLF)
	ConsoleWrite($rTime[5] & @CRLF)
	ConsoleWrite($rTime[6] & @CRLF)
	$msg1rTime = StringReplace($rTime[2], ":", "")
	ConsoleWrite($msg1rTime & @CRLF)
	if $rTime[3]="Rx" and $rTime[4]=2 and $rTime[5]="s" and $rTime[6]=0081 and $rTime[7]="msg1" Then
		$recMsg1=1
		ConsoleWrite("recMsg1= "&$recMsg1 & @CRLF)
	EndIf

	$sTime1=StringSplit($msg2Tx,"|")
	ConsoleWrite($sTime1[2] & @CRLF)		;time
	ConsoleWrite($sTime1[3] & @CRLF)		;type
	ConsoleWrite($sTime1[4] & @CRLF)		;channel
	ConsoleWrite($sTime1[5] & @CRLF)		;msgType
	ConsoleWrite($sTime1[6] & @CRLF)		;msgID
	ConsoleWrite($sTime1[7] & @CRLF)		;msgName
	$msg2sTime = StringReplace($sTime1[2], ":", "")
	ConsoleWrite($msg2sTime & @CRLF)
	if $sTime1[3]="Tx" and $sTime1[4]=1 and $sTime1[5]="s" and $sTime1[6]=0085 and $sTime1[7]="msg2" Then
		$sendMsg2=1
		ConsoleWrite("sendMsg2= "&$sendMsg2 & @CRLF)
	EndIf

	$rTime1=StringSplit($msg2Rx,"|")
	ConsoleWrite($rTime1[2] & @CRLF)		;time
	ConsoleWrite($rTime1[3] & @CRLF)		;type
	ConsoleWrite($rTime1[4] & @CRLF)		;channel
	ConsoleWrite($rTime1[5] & @CRLF)		;msgType
	ConsoleWrite($rTime1[6] & @CRLF)		;msgID
	ConsoleWrite($rTime1[7] & @CRLF)		;msgName
	$msg2rTime = StringReplace($rTime1[2], ":", "")
	ConsoleWrite($msg2rTime & @CRLF)
	if $rTime1[3]="Rx" and $rTime1[4]=2 and $rTime1[5]="s" and $rTime1[6]=0085 and $rTime1[7]="msg2" Then
		$recMsg2=1
		ConsoleWrite("recMsg2= "&$recMsg2 & @CRLF)
	EndIf

	$res=StringCompare($msg1sTime,$msg1rTime)
	ConsoleWrite("$res= "&$res & @CRLF)
	$res1=StringCompare($msg2sTime,$msg2rTime)
	ConsoleWrite("$res1= "&$res1 & @CRLF)
	if $res=-1 and $res1=-1 Then
		$time=1
	ElseIf $res=1 and $res1=1 Then
		$time=0
	EndIf
EndIf
EndIf
if $time=1 and ($sendMsg1 and $recMsg1 and $sendMsg2 and $recMsg2=1) then
	_ExcelWriteCell($oExcel, "Pass", 21, 10)
elseif $time=0 and ($sendMsg1 and $recMsg1 and $sendMsg2 and $recMsg2=1) then
	_ExcelWriteCell($oExcel, "Fail", 21, 10)
	_ExcelWriteCell($oExcel, "Sent and received time are incorrect!!!known issue", 21, 9)
Else
	_ExcelWriteCell($oExcel, "Fail", 21, 10)
endif

ConsoleWrite("****End : TC_UC3_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
