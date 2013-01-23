;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_002
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

;~   _launchApp()																			;launching the app
 WinActivate($mWin)
Sleep(1000)

;~ _loadConfig("TestcaseTx7")
 sleep(500)


;To delete all message block
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

;~ ControlClick($mWin,"",17002)
 ; Check Key Trigger for Single msg type
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=0) Then
	ControlCommand($mWin,"",$keyctrlID,"Check")
	sleep(500)
EndIf

Sleep(500)
ControlSend($mWin,"",$keyeditCtrlID,"a")											;sending key value as 'a'

Sleep(500)
if (ControlCommand($mWin,"",$timdeltrg,"IsChecked")=1) then						;to uncheck the time delay between trigger
	ControlCommand($mWin,"",$timdeltrg,"UnCheck")

EndIf

Sleep(500)
 ControlSetText($mWin,"",$msgnamesCtrlID,1)   							; Enter Non Db msg 11
  ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button

ControlSetText($mWin,"",$msgnamesCtrlID,2)   							; Enter Non Db msg 12
ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button


	 ;~ 	For  Disabling Auto update
	if (ControlCommand($mWin,"",$updatectrlID,"IsChecked")=1) Then
		ControlCommand($mWin,"",$updatectrlID,"UnCheck")
	EndIf
    Sleep(500)

   ControlClick($mWin, "",1014)											;click on update button
   sleep(1000)

   $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of tollbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
	sleep(1000)

	WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(1000)


$cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgsec1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgsec1:" &$msgsec1 & @CRLF)

$sTimesec1=StringSplit($msgsec1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec1:" &$sTimesec1[7] & @CRLF)														;Time of msg 1st sent
Sleep(500)
 $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of tollbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
	sleep(1000)

	WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(1000)

    Send("{a}")
    Sleep(500)
	Send("{a}")
     Sleep(500)
	Send("{a}")

    WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for disconnecting
	 sleep(1000)

 $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)


	$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgsec2=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message windowof
ConsoleWrite("$msgsec2:" &$msgsec2 & @CRLF)

$sTimesec2=StringSplit($msgsec2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec2:" &$sTimesec2[7] & @CRLF)														;Time of msg 1st sent



	$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgsec3=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgsec3:" &$msgsec3 & @CRLF)

$sTimesec3=StringSplit($msgsec3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec3:" &$sTimesec3[7] & @CRLF)														;Time of msg 1st sent

	if (ControlCommand($mWin,"",$updatectrlID,"IsChecked")=0) Then
		ControlCommand($mWin,"",$updatectrlID,"Check")
	EndIf
    Sleep(500)

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

  ; Check Key Trigger for Single msg type
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=0) Then
	ControlCommand($mWin,"",$keyctrlID,"Check")
	sleep(500)
EndIf

Sleep(500)
ControlSend($mWin,"",$keyeditCtrlID,"a")											;sending key value as 'a'

Sleep(500)

if (ControlCommand($mWin,"",$timdeltrg,"IsChecked")=1) then						;to uncheck the time delay between trigger
	ControlCommand($mWin,"",$timdeltrg,"UnCheck")

EndIf

ControlCommand($mWin,"","ComboBox3","SelectString","All")				;selecting "All"messsage type

Sleep(500)
 ControlSetText($mWin,"",$msgnamesCtrlID,3)   							; Enter Non Db msg 3
  ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button

ControlSetText($mWin,"",$msgnamesCtrlID,4)   							; Enter Non Db msg 4
ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button

 $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of tollbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
	sleep(1000)

	WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(1000)

    Send("{a}")
    Sleep(500)

 $cntToolhWd=ControlGetHandle($mWin,"",128)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgsec4=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msgsec4:" &$msgsec4 & @CRLF)

$sTimesec4=StringSplit($msgsec4,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec4:" &$sTimesec4[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgsec5=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgsec5:" &$msgsec5 & @CRLF)

$sTimesec5=StringSplit($msgsec5,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec5:" &$sTimesec5[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msglast=_GUICtrlListView_GetItemTextString($timhWnd, 7)									;selecting last elment in message window
ConsoleWrite("$msglast:" &$msglast & @CRLF)

$sTimelast=StringSplit($msglast,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimelast:" &$sTimelast[7] & @CRLF)														;Time of msg 1st sent

If ($sTimesec1[7]=1 And $sTimesec2[7]=1 And $sTimesec3[7]=1 And $sTimesec4[7]=3 And $sTimesec5[7]=3 And $sTimelast[7]=0) Then
	_ExcelWriteCell($oExcel,"pass",7,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",7,11)

EndIf
