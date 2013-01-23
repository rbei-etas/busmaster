;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_13
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

;~   _launchApp()																			;launching the app
 WinActivate($mWin)
Sleep(1000)

;~ _loadConfig("TestcaseTx7")
 sleep(500)
 _TxWindowOpen()

$timemsglisthWnd=ControlGetHandle ($mWin,"",$msglistInst)				;handler for tx wimdow
    $msg1=_GUICtrlListView_GetItemCount($timemsglisthWnd)										;To get the listview count
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

	ControlSetText($mWin,"",$msgnamesCtrlID,121)   									; Enter Non Db msg 12
  ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

	ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block

	; Add the DB msgs to the Tx message list
	ControlCommand($mWin,"",$msgnamecomboID,"SetCurrentSelection",1)			;adding 2nd message
    ControlClick($mWin, "",$addmsgCtrlID)										;~ ; Click on Add message button

	ControlSetText($mWin,"",$msgnamesCtrlID,13)   									; Enter Non Db msg 13
    ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

; Enabling Key Trigger
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=0) Then
	ControlCommand($mWin,"",$keyctrlID,"Check")
	sleep(500)
EndIf

ControlSend($mWin,"",$keyeditCtrlID,"b")
 sleep(1000)
ControlCommand($mWin,"","ComboBox3","SelectString","Single")				;selecting "Single"messsage type

$cntToolhWd=ControlGetHandle($mWin,"",128)								; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							; Click on 'Connect' icon
sleep(500)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
sleep(500)
send("b")
sleep(500)
send("b")

 $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
sleep(1000)


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg12a=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msg12a:" &$msg12a & @CRLF)

$sTime12a=StringSplit($msg12a,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime12a:" &$sTime12a[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg12b=_GUICtrlListView_GetItemTextString($timhWnd, 1)									;selecting 3rd elment in message window
ConsoleWrite("$msg12b:" &$msg12b & @CRLF)

$sTime12b=StringSplit($msg12b,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime12b:" &$sTime12b[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg12c=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting last elment in message window
ConsoleWrite("$msg12c:" &$msg12c & @CRLF)

$sTime12c=StringSplit($msg12c,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime12c:" &$sTime12c[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg12d=_GUICtrlListView_GetItemTextString($timhWnd, 3)									;selecting last elment in message window
ConsoleWrite("$msg12d:" &$msg12d & @CRLF)

$sTime12d=StringSplit($msg12d,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime12d:" &$sTime12d[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg12e=_GUICtrlListView_GetItemTextString($timhWnd, 6)									;selecting last elment in message window
ConsoleWrite("$msg12d:" &$msg12e & @CRLF)

$sTime12e=StringSplit($msg12e,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime12e:" &$sTime12e[6] & @CRLF)														;Time of msg 1st sent


If ($sTime12a[6]=0289 And $sTime12b[6]=0121 And $sTime12c[6]=0018 And $sTime12d[6]=0018 And $sTime12e[6]=0) Then

	_ExcelWriteCell($oExcel,"pass",18,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",18,11)

EndIf













