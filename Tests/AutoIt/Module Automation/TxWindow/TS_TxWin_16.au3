;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_16
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

Sleep(1000)
    Sleep(500)
ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block
Sleep(500)

if (ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"IsChecked")=0) then				;to check the time delay between message block
	ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"Check")
EndIf

ControlSetText ($mWin,"",$timedelayBtmedit,1000)										;setting time delay betn blck as 1000

; Add the DB msgs to the Tx message list
	$msgname=ControlCommand($mWin,"",$msgnamecomboID,"SetCurrentSelection",0)			;adding 1st message
	Sleep(500)
	ConsoleWrite("&$msgname:" &$msgname & @CRLF)
	ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button
	Sleep(100)

	ControlSetText($mWin,"",$msgnamesCtrlID,121)   									; Enter Non Db msg 121
  ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

	ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block

	; Add the DB msgs to the Tx message list
	ControlCommand($mWin,"",$msgnamecomboID,"SetCurrentSelection",1)			;adding 2nd message
    ControlClick($mWin, "",$addmsgCtrlID)										;~ ; Click on Add message button

	ControlSetText($mWin,"",$msgnamesCtrlID,13)   									; Enter Non Db msg 13
    ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

	Sleep(500)
$cntToolhWd=ControlGetHandle($mWin,"",128)								; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							; Click on 'Connect' icon

sleep(500)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
sleep(1500)

$cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
sleep(1000)


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg16a=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msg16a:" &$msg16a & @CRLF)

$sTime16a=StringSplit($msg16a,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime16a:" &$sTime16a[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg16b=_GUICtrlListView_GetItemTextString($timhWnd, 1)									;selecting 3rd elment in message window
ConsoleWrite("$msg16b:" &$msg16b & @CRLF)

$sTime16b=StringSplit($msg16b,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime16b:" &$sTime16b[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg16c=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting last elment in message window
ConsoleWrite("$msg16c:" &$msg16c & @CRLF)

$sTime16c=StringSplit($msg16c,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime16c:" &$sTime16c[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg16d=_GUICtrlListView_GetItemTextString($timhWnd, 3)									;selecting last elment in message window
ConsoleWrite("$msg16d:" &$msg16d & @CRLF)

$sTime16d=StringSplit($msg16d,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime16d:" &$sTime16d[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg16e=_GUICtrlListView_GetItemTextString($timhWnd, 4)									;selecting last elment in message window
ConsoleWrite("$msg16e:" &$msg16e & @CRLF)

$sTime16e=StringSplit($msg16e,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime16e:" &$sTime16e[6] & @CRLF)														;Time of msg 1st sent


If ($sTime16a[6]=0289 And $sTime16b[6]=0121 And $sTime16c[6]=0018 And $sTime16d[6]=0013 And $sTime16e[6]=0) Then

	_ExcelWriteCell($oExcel,"pass",21,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",21,11)

EndIf
