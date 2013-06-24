;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_15
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

 ControlClick($mWin,"",$txmodemonoshotCtrlID)							;to select monoshot

 ControlSetText($mWin,"",$msgnamesCtrlID,14)   									; Enter Non Db msg 14
    ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

ControlSetText($mWin,"",$msgnamesCtrlID,15)   									; Enter Non Db msg 15
    ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

Sleep(500)
ControlClick($mWin,"",$AddblckctrlID)													;Adding the message Block
Sleep(500)


if (ControlCommand($mWin,"",$timdeltrg,"IsChecked")=0) then						;to uncheck the time delay between trigger
	ControlCommand($mWin,"",$timdeltrg,"Check")

EndIf

Sleep(500)
ControlClick($mWin,"",$txmodemonoshotCtrlID)									;to select monoshot
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
$msg15a=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msg15a:" &$msg15a & @CRLF)

$sTime15a=StringSplit($msg15a,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime15a:" &$sTime15a[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg15b=_GUICtrlListView_GetItemTextString($timhWnd, 1)									;selecting 3rd elment in message window
ConsoleWrite("$msg15b:" &$msg15b & @CRLF)

$sTime15b=StringSplit($msg15b,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime15b:" &$sTime15b[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg15c=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting last elment in message window
ConsoleWrite("$msg15c:" &$msg15c & @CRLF)

$sTime15c=StringSplit($msg15c,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime15c:" &$sTime15c[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg15d=_GUICtrlListView_GetItemTextString($timhWnd, 3)									;selecting last elment in message window
ConsoleWrite("$msg15d:" &$msg15d & @CRLF)

$sTime15d=StringSplit($msg15d,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime15d:" &$sTime15d[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg15e=_GUICtrlListView_GetItemTextString($timhWnd, 4)									;selecting last elment in message window
ConsoleWrite("$msg15e:" &$msg15e & @CRLF)

$sTime15e=StringSplit($msg15e,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime15e:" &$sTime15e[6] & @CRLF)														;Time of msg 1st sent


If ($sTime15a[6]=0016 And $sTime15b[6]=0017 And $sTime15c[6]=0014 And $sTime15d[6]=0015 And $sTime15e[6]=0) Then

	_ExcelWriteCell($oExcel,"pass",20,2)

    Else
	  _ExcelWriteCell($oExcel,"Fail",20,2)

EndIf






