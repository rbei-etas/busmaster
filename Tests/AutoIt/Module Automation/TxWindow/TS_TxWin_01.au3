;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_001
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

 sleep(1000)
 ControlClick($mWin,"",$AddblckctrlID)									;Adding the Block
 ControlSetText($mWin,"",$msgnamesCtrlID,11)   							; Enter Non Db msg 11
 ;~ ; Click on Add message button
  ControlClick($mWin, "",$addmsgCtrlID)

ControlSetText($mWin,"",$msgnamesCtrlID,12)   							; Enter Non Db msg 12
 ;~ ; Click on Add message button
  ControlClick($mWin, "",$addmsgCtrlID)

	$cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of tollbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
	sleep(1000)

	WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(1000)

    WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for disconnecting
	 sleep(1000)

$timhWnd=ControlGetHandle ($mWin,"","[CLASS:SysListView32; INSTANCE:4]")								;handler for tx wimdow of J1939
$msg1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment of tx window in j1939
ConsoleWrite("$msg1:" &$msg1 & @CRLF)

$sTime1=StringSplit($msg1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime1:" &$sTime1[7] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($mWin,"","[CLASS:SysListView32; INSTANCE:4]")							;handler for tx wimdow
$msg2=_GUICtrlListView_GetItemTextString($timhWnd, 1)
ConsoleWrite("$msg2:" &$msg2 & @CRLF)

$sTime2=StringSplit($msg2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime2:" &$sTime2[7] & @CRLF)											;Time of msg 2nd sent


$timhWnd=ControlGetHandle ($mWin,"","[CLASS:SysListView32; INSTANCE:4]")							;handler for tx wimdow
$msg3=_GUICtrlListView_GetItemTextString($timhWnd, 2)
ConsoleWrite("$msg3:" &$msg3 & @CRLF)

$sTime3=StringSplit($msg3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime3:" &$sTime3[7] & @CRLF)											;Time of msg 3rd sent

$timhWnd=ControlGetHandle ($mWin,"","[CLASS:SysListView32; INSTANCE:4]")							;handler for tx wimdow
$msg4=_GUICtrlListView_GetItemTextString($timhWnd, 3)
ConsoleWrite("$msg4:" &$msg4 & @CRLF)

$sTime4=StringSplit($msg4,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime4:" &$sTime4[7] & @CRLF)											;Time of msg 3rd sent

$cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

;~ 	For  Disabling Auto update
	if (ControlCommand($mWin,"",$updatectrlID,"IsChecked")=1) Then
		ControlCommand($mWin,"",$updatectrlID,"UnCheck")
	EndIf
    Sleep(500)

If $sTime1[7]=11 And $sTime2[7]=12 And $sTime3[7]=11 And $sTime4[7]=12 Then

	_ExcelWriteCell($oExcel,"pass",6,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",6,11)

EndIf


For  $i = 0 To $msg1
																						;~  TO delete all the message block
 ControlCommand($mWin,"",$delbutmsg,"IsEnabled")
		; click on delete button
		ControlClick($mWin,"",$delbutmsg)

		;click on confirmation button for deletion
		ControlClick($mWin,"",$yesbtndel)
Next

 sleep(1000)
 ControlClick($mWin,"",$AddblckctrlID)													;adding  the message block

 ControlClick($mWin,"",$txmodemonoshotCtrlID)															;click on monoshot

ControlSetText($mWin,"",$msgnamesCtrlID,21)   							; Enter Non Db msg 21
 ;~ ; Click on Add message button
  ControlClick($mWin, "",$addmsgCtrlID)

ControlSetText($mWin,"",$msgnamesCtrlID,22)   							; Enter Non Db msg 22
 ;~ ; Click on Add message button
  ControlClick($mWin, "",$addmsgCtrlID)
 sleep(1000)

  ControlClick($mWin, "",$updatecrtlid)											;click on update button
 sleep(1000)
  $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of tollbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
	sleep(1000)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(1000)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for disconnecting
	 sleep(1000)

$timhWnd1=ControlGetHandle ($mWin,"",$msgwin)							;handler for tx wimdow
$msg6=_GUICtrlListView_GetItemTextString($timhWnd1, 0)
ConsoleWrite("$msg6:" &$msg6 & @CRLF)

$sTime5=StringSplit($msg6,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime5:" &$sTime5[7] & @CRLF)											;Time of msg 2nd sent


$timhWnd1=ControlGetHandle ($mWin,"",$msgwin)							;handler for tx wimdow
$msg7=_GUICtrlListView_GetItemTextString($timhWnd1, 1)
ConsoleWrite("$msg7:" &$msg7 & @CRLF)

$sTime6=StringSplit($msg7,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime6:" &$sTime6[7] & @CRLF)											;Time of msg 3rd sent

$timhWnd1=ControlGetHandle ($mWin,"",$msgwin)							;handler for tx wimdow
$msg8=_GUICtrlListView_GetItemTextString($timhWnd1, 2)
ConsoleWrite("$msg8:" &$msg8 & @CRLF)

$sTime7=StringSplit($msg8,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime7:" &$sTime7[7] & @CRLF)											;Time of msg 3rd sent

;~ 	For  Enabling Auto update
	if (ControlCommand($mWin,"",$updatectrlID,"IsChecked")=0) Then
		ControlCommand($mWin,"",$updatectrlID,"Check")

	EndIf
    Sleep(500)

$cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

If $sTime5[7]=21 And $sTime6[7]=22 And $sTime7[7]=0  Then

	_ExcelWriteCell($oExcel,"pass",6,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",6,11)

EndIf