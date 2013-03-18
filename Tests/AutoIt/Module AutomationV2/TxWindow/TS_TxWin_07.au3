;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_007
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

 ControlClick($mWin,"",$AddblckctrlID)													;Adding the Block

; Enabling Key Trigger
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=0) Then
	ControlCommand($mWin,"",$keyctrlID,"Check")
	sleep(500)
EndIf

ControlSend($mWin,"",$keyeditCtrlID,"s")
 sleep(1000)

if (ControlCommand($mWin,"",$timdeltrg,"IsChecked")=1) then						;to uncheck the time delay between trigger
	ControlCommand($mWin,"",$timdeltrg,"UnCheck")

EndIf
  ControlSetText($mWin,"",$msgnamesCtrlID,31)   							; Enter Non Db msg 11
  ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button

$txmsglisthWnd=ControlGetHandle ($mWin,"",$msgnamelistInst)					;handler for txmode
$msgck1=_GUICtrlListView_SetItemChecked($txmsglisthWnd,0,True)			;Setting the value of 1st msg checked

ControlSetText($mWin,"",$msgnamesCtrlID,32)   							; Enter Non Db msg 12
ControlClick($mWin, "",$addmsgCtrlID)									;~  Click on Add message button

$txmsglisthWnd=ControlGetHandle ($mWin,"",$msgnamelistInst)					;handler for txmode
$msgck2=_GUICtrlListView_SetItemChecked($txmsglisthWnd,1,True)			;Setting the value of 2nd msg checked

 ConsoleWrite("$msgck1 : " &$msgck1 &@CRLF)
 ConsoleWrite("$msgck2 : " &$msgck2 &@CRLF)

ControlClick($mWin, "",$updatecrtlid)														;click on update button
sleep(1000)

$cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)											; Click on 'Connect' icon
sleep(1000)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
sleep(500)

Send("s")
Send("s")
sleep(500)
 $cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)			;handler for tx wimdow
$msgsevth1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgsevth1:" &$msgsevth1 & @CRLF)

$sTimesevnth1=StringSplit($msgsevth1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesevnth1:" &$sTimesevnth1[7] & @CRLF)														;Time of msg 1st sent



$timhWnd1=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow o
$msgsevth2=_GUICtrlListView_GetItemTextString($timhWnd1, 1)									;selecting 2nd elment in message window
ConsoleWrite("$msgsevth2:" &$msgsevth2 & @CRLF)

$sTimesevnth2=StringSplit($msgsevth2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesevnth2:" &$sTimesevnth2[7] & @CRLF)									;Time of msg 1st sent
If ($sTimesevnth1[7]=31 And $sTimesevnth2[7]=32 And $msgck1="True" And $msgck2="True"  ) Then
	 $TS1="Pass"
    ConsoleWrite("Test scenario 1 is Pass")
  Else
		ConsoleWrite("Test scenario 1 is Fail")
         $TS1="Fail"
EndIf
;Second senario


$txmsglisthWnd=ControlGetHandle ($mWin,"",$msgnamelistInst)					;handler for txmode
$msgck1=_GUICtrlListView_SetItemChecked($txmsglisthWnd,0,False)			;Setting the value of 1st msg Unchecked

ControlClick($mWin, "",$updatecrtlid)														;click on update button
sleep(1000)

$cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
sleep(1000)

	WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(500)
Send("s")
Send("s")
 $cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)			;handler for tx wimdow
$msgsevth3=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgsevth3:" &$msgsevth3 & @CRLF)

$sTimesevnth3=StringSplit($msgsevth3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesevnth3:" &$sTimesevnth3[7] & @CRLF)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)			;handler for tx wimdow
$msgsevth4=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgsevth4:" &$msgsevth4 & @CRLF)

$sTimesevnth4=StringSplit($msgsevth4,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesevnth4:" &$sTimesevnth4[7] & @CRLF)

If ( $sTimesevnth3[7]=32 And $sTimesevnth4[7]=32 ) Then
   ConsoleWrite("Test scenario 2 is Pass")
    $TS2="Pass"
 Else
		ConsoleWrite("Test scenario 2 is Fail")
    $TS2="Fail"
EndIf

If $TS1="Pass" And $TS2="Pass" Then

	_ExcelWriteCell($oExcel, "Pass", 12, 2)
else
	 _ExcelWriteCell($oExcel,"Fail",12,2)

EndIf


