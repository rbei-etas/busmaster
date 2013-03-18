;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_14
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
 WinActivate($mWin)
Sleep(1000)

;~ _loadConfig("TestcaseTx7")
 sleep(500)
 _TxWindowOpen()

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

	 ;~ 	For  Enabling Auto update
	if (ControlCommand($mWin,"",$updatectrlID,"IsChecked")=0) Then
		ControlCommand($mWin,"",$updatectrlID,"Check")
	EndIf

    Sleep(500)
ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block
Sleep(500)

if (ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"IsChecked")=1) then				;to check the time delay between message block
	ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"UnCheck")
EndIf
Sleep(1000)

; Enabling Key Trigger
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=0) Then
	ControlCommand($mWin,"",$keyctrlID,"Check")
	sleep(500)
EndIf

if (ControlCommand($mWin,"",$timdeltrg,"IsChecked")=1) then						;to uncheck the time delay between trigger
	ControlCommand($mWin,"",$timdeltrg,"UnCheck")

EndIf

ControlSend($mWin,"",$keyeditCtrlID,"a")
 sleep(1000)

 ControlCommand($mWin,"","ComboBox3","SelectString","All")				;selecting "Single"messsage type

 ControlSetText($mWin,"",$msgnamesCtrlID,14)   									; Enter Non Db msg 14
    ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

ControlSetText($mWin,"",$msgnamesCtrlID,15)   									; Enter Non Db msg 15
    ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

Sleep(500)
ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block
Sleep(500)


if (ControlCommand($mWin,"",$timdeltrg,"IsChecked")=0) then						;to check the time delay between trigger
	ControlCommand($mWin,"",$timdeltrg,"Check")

EndIf

Sleep(500)
 ControlSetText($mWin,"",$msgnamesCtrlID,16)   									; Enter Non Db msg 14
ControlClick($mWin, "",$addmsgCtrlID)											;~  Click on Add message button

ControlSetText($mWin,"",$msgnamesCtrlID,17)   									; Enter Non Db msg 15
ControlClick($mWin, "",$addmsgCtrlID)											;~  Click on Add message button

Sleep(500)
$cntToolhWd=ControlGetHandle($mWin,"",$connectid)								; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							; Click on 'Connect' icon

sleep(500)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
;~ sleep(500)
send("a")
sleep(100)
$cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of toolbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
sleep(1000)


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg14a=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msg14a:" &$msg14a & @CRLF)

$sTime14a=StringSplit($msg14a,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime14a:" &$sTime14a[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg14b=_GUICtrlListView_GetItemTextString($timhWnd, 1)									;selecting 3rd elment in message window
ConsoleWrite("$msg14b:" &$msg14b & @CRLF)

$sTime14b=StringSplit($msg14b,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime14b:" &$sTime14b[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg14c=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting last elment in message window
ConsoleWrite("$msg14c:" &$msg14c & @CRLF)

$sTime14c=StringSplit($msg14c,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime14c:" &$sTime14c[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg14d=_GUICtrlListView_GetItemTextString($timhWnd, 3)									;selecting last elment in message window
ConsoleWrite("$msg14d:" &$msg14d & @CRLF)

$sTime14d=StringSplit($msg14d,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime14d:" &$sTime14d[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg14e=_GUICtrlListView_GetItemTextString($timhWnd, 4)									;selecting last elment in message window
ConsoleWrite("$msg14e:" &$msg14e & @CRLF)

$sTime14e=StringSplit($msg14e,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime14e:" &$sTime14e[6] & @CRLF)														;Time of msg 1st sent


If ($sTime14a[6]=0016 And $sTime14b[6]=0017 And $sTime14c[6]=0014 And $sTime14d[6]=0016 And $sTime14e[6]=0015) Then

	_ExcelWriteCell($oExcel,"pass",19,2)

    Else
	  _ExcelWriteCell($oExcel,"Fail",19,2)

EndIf






