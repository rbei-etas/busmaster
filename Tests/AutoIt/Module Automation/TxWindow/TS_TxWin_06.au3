;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_006
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

;~   _launchApp()																		;launching the app
 WinActivate($mWin)
Sleep(1000)

;~ _loadConfig("TestcaseTx7")
 sleep(500)
 _TxWindowOpen()
 ;~ 	For  Disabling Auto update
	if (ControlCommand($mWin,"",$updatectrlID,"IsChecked")=1) Then
		ControlCommand($mWin,"",$updatectrlID,"UnCheck")
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

 sleep(1000)
 ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block

  ControlSetText($mWin,"",$msgnamesCtrlID,31)   							; Enter Non Db msg 11
  ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button

$txmsglisthWnd=ControlGetHandle ($mWin,"",$msgnamelistInst)					;handler for txmode
$msgchk1=_GUICtrlListView_SetItemChecked($txmsglisthWnd,0,True)			;Setting the value of 1st msg checked

ControlSetText($mWin,"",$msgnamesCtrlID,32)   							; Enter Non Db msg 12
ControlClick($mWin, "",$addmsgCtrlID)									;~  Click on Add message button

$txmsglisthWnd=ControlGetHandle ($mWin,"",$msgnamelistInst)					;handler for txmode
$msgchk2=_GUICtrlListView_SetItemChecked($txmsglisthWnd,1,True)			;Setting the value of 2nd msg checked

 ConsoleWrite("$msgchk1 : " &$msgchk1 &@CRLF)
 ConsoleWrite("$msgchk2 : " &$msgchk2 &@CRLF)

ControlClick($mWin, "",1014)														;click on update button
sleep(1000)

$cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
sleep(1000)

	WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(500)

 $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)			;handler for tx wimdow
$msgsixth1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgsixth1:" &$msgsixth1 & @CRLF)

$sTimesixth1=StringSplit($msgsixth1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesixth1:" &$sTimesixth1[7] & @CRLF)														;Time of msg 1st sent



$timhWnd1=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow o
$msgsixth2=_GUICtrlListView_GetItemTextString($timhWnd1, 1)									;selecting 2nd elment in message window
ConsoleWrite("$msgsixth2:" &$msgsixth2 & @CRLF)

$sTimesixth2=StringSplit($msgsixth2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesixth2:" &$sTimesixth2[7] & @CRLF)									;Time of msg 1st sent

;Second senario


$txmsglisthWnd=ControlGetHandle ($mWin,"",$msgnamelistInst)					;handler for txmode
$msg1=_GUICtrlListView_SetItemChecked($txmsglisthWnd,0,False)			;Setting the value of 1st msg Unchecked

ControlClick($mWin, "",1014)														;click on update button
sleep(1000)

$cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
sleep(1000)

	WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(500)

 $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)			;handler for tx wimdow
$msgsixth3=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgsixth3:" &$msgsixth3 & @CRLF)

$sTimesixth3=StringSplit($msgsixth3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesixth3:" &$sTimesixth3[7] & @CRLF)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)			;handler for tx wimdow
$msgsixth4=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgsixth4:" &$msgsixth4 & @CRLF)

$sTimesixth4=StringSplit($msgsixth4,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesixth4:" &$sTimesixth4[7] & @CRLF)

If ($sTimesixth1[7]=31 And $sTimesixth2[7]=32 And $msgchk1="True" And $msgchk2="True" And $sTimesixth3[7]=32 And $sTimesixth4[7]=32 ) Then

	_ExcelWriteCell($oExcel,"pass",11,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",11,11)

EndIf

