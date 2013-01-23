;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_11
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

  _launchApp()																		;launching the app
 WinActivate($mWin)
Sleep(1000)
;~ _createConfig("TestcaseTx5")
Sleep(500)
;~ _loadConfig("TestcaseTx5")
;~  sleep(500)
;~  _AssociateCANDB("New_Data")
;~  _loadDatabase("New_Data")
 _TxWindowOpen()
 Sleep(500)

if (ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"IsChecked")=0) then						;to uncheck the time delay between message block
	ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"Check")

EndIf
Sleep(1000)

ControlSetText ($mWin,"",$timedelayBtmedit,2000)										;setting time delay betn blck as 1000

Sleep(1000)

if (ControlCommand($mWin,"",$txmodemonoshotCtrlID,"IsChecked")=0) then						;to select the monoshot
	ControlCommand($mWin,"",$txmodemonoshotCtrlID,"Check")

EndIf



 $msgblklstvwhWnd=ControlGetHandle ($mWin,"",$msglistInst)								;handler for tx wimdow
    $msg1=_GUICtrlListView_ClickItem($msgblklstvwhWnd,0,"left")								;To get the listview count
 	ConsoleWrite("$msg1 : " &$msg1 &@CRLF)


if (ControlCommand($mWin,"",$txmodemonoshotCtrlID,"IsChecked")=0) then						;to select the monoshot
	ControlCommand($mWin,"",$txmodemonoshotCtrlID,"Check")

EndIf

	$cntToolhWd=ControlGetHandle($mWin,"",128)								; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							; Click on 'Connect' icon
sleep(500)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
sleep(10000)

 $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg11a=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msg11a:" &$msg11a & @CRLF)

$sTime11a=StringSplit($msg11a,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime11a:" &$sTime11a[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg11b=_GUICtrlListView_GetItemTextString($timhWnd, 1)									;selecting 3rd elment in message window
ConsoleWrite("$msg11b:" &$msg11b & @CRLF)

$sTime11b=StringSplit($msg11b,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime11b:" &$sTime11b[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg11c=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting last elment in message window
ConsoleWrite("$msg11c:" &$msg11c & @CRLF)

$sTime11c=StringSplit($msg11c,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime11c:" &$sTime11c[6] & @CRLF)														;Time of msg 1st sent

If ($sTime11a[6]=0289 And $sTime11b[6]=0018 And $sTime11c[6]=0 ) Then

	_ExcelWriteCell($oExcel,"pass",16,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",16,11)

EndIf





















