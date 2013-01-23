;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_17
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

if (ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"IsChecked")=0) then				;to check the time delay between message block
	ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"Check")
EndIf
ControlSetText ($mWin,"",$timedelayBtmedit,1000)										;setting time delay betn blck as 1000

ControlSetText($mWin,"",$msgnamesCtrlID,121)   									; Enter Non Db msg 121
  ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

ControlSetText($mWin,"",$msgnamesCtrlID,122)   									; Enter Non Db msg 122
ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block

ControlSetText($mWin,"",$msgnamesCtrlID,131)   									; Enter Non Db msg 131
ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

ControlSetText($mWin,"",$msgnamesCtrlID,132)   									; Enter Non Db msg 132
ControlClick($mWin, "",$addmsgCtrlID)											;~ ; Click on Add message button

$cntToolhWd=ControlGetHandle($mWin,"",128)								; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							; Click on 'Connect' icon

sleep(500)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
sleep(1500)

$cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
sleep(1000)


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg17a=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msg17a:" &$msg17a & @CRLF)

$sTime17a=StringSplit($msg17a,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime17a:" &$sTime17a[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg17b=_GUICtrlListView_GetItemTextString($timhWnd, 1)									;selecting 3rd elment in message window
ConsoleWrite("$msg17b:" &$msg17b & @CRLF)

$sTime17b=StringSplit($msg17b,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime17b:" &$sTime17b[6] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg17c=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting last elment in message window
ConsoleWrite("$msg17c:" &$msg17c & @CRLF)

$sTime17c=StringSplit($msg17c,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime17c:" &$sTime17c[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg17d=_GUICtrlListView_GetItemTextString($timhWnd, 3)									;selecting last elment in message window
ConsoleWrite("$msg17d:" &$msg17d & @CRLF)

$sTime17d=StringSplit($msg17d,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime17d:" &$sTime17d[6] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msg17e=_GUICtrlListView_GetItemTextString($timhWnd, 4)									;selecting last elment in message window
ConsoleWrite("$msg17e:" &$msg17e & @CRLF)

$sTime17e=StringSplit($msg17e,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime17e:" &$sTime17e[6] & @CRLF)														;Time of msg 1st sent


If ($sTime17a[6]=0121 And $sTime17b[6]=0122 And $sTime17c[6]=0131 And $sTime17d[6]=0132 And $sTime17e[6]=0) Then

	_ExcelWriteCell($oExcel,"pass",22,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",22,11)

EndIf

