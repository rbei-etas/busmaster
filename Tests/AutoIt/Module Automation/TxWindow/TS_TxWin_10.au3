;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_10
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

 ;~ For  Enabling Auto update
	if (ControlCommand($mWin,"",$updatectrlID,"IsChecked")=0) Then
		ControlCommand($mWin,"",$updatectrlID,"Check")
	EndIf
    Sleep(500)
$timemsglisthWnd=ControlGetHandle ($mWin,"",$msglistInst)								;handler for tx wimdow
    $msg1=_GUICtrlListView_GetItemCount($timemsglisthWnd)								;To get the listview count
 	ConsoleWrite("$msg1 : " &$msg1 &@CRLF)

For  $i = 0 To $msg1
																						;~  TO delete all the message block
 ControlCommand($mWin,"",$delbutmsg,"IsEnabled")
		; click on delete button
		ControlClick($mWin,"",$delbutmsg)

		;click on confirmation button for deletion
		ControlClick($mWin,"",$yesbtndel)
Next

 ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block
Sleep(500)

if (ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"IsChecked")=0) then						;to uncheck the time delay between message block
	ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"Check")

EndIf
Sleep(1000)

ControlSetText ($mWin,"",$timedelayBtmedit,1000)										;setting time delay betn blck as 1000

Sleep(1000)
	; Add the DB msgs to the Tx message list
	$msgname=ControlCommand($mWin,"",$msgnamecomboID,"SetCurrentSelection",0)			;adding 1st message
	Sleep(500)
	ConsoleWrite("&$msgname:" &$msgname & @CRLF)
	ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button
	Sleep(100)
	ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block

	; Add the DB msgs to the Tx message list
	ControlCommand($mWin,"",$msgnamecomboID,"SetCurrentSelection",1)			;adding 2nd message
    ControlClick($mWin, "",$addmsgCtrlID)										;~ ; Click on Add message button

	$cntToolhWd=ControlGetHandle($mWin,"",128)								; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							; Click on 'Connect' icon
sleep(500)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
sleep(10000)

 $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg10a=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msg10a:" &$msg10a & @CRLF)

$sTime10a=StringSplit($msg10a,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime10a:" &$sTime10a[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg10b=_GUICtrlListView_GetItemTextString($timhWnd, 1)									;selecting 3rd elment in message window
ConsoleWrite("$msg10b:" &$msg10b & @CRLF)

$sTime10b=StringSplit($msg10b,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime10b:" &$sTime10b[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg10c=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting last elment in message window
ConsoleWrite("$msg10c:" &$msg10c & @CRLF)

$sTime10c=StringSplit($msg10c,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime10c:" &$sTime10c[6] & @CRLF)														;Time of msg 1st sent

If ($sTime10a[6]=0289 And $sTime10b[6]=0018 And $sTime10c[6]=0289 ) Then

	_ExcelWriteCell($oExcel,"pass",15,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",15,11)

EndIf
