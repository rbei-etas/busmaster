;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_003
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
ControlClick($mWin,"",$txmodemonoshotCtrlID)							;to select monoshot

 ; Enabling Key Trigger
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=0) Then
	ControlCommand($mWin,"",$keyctrlID,"Check")
	sleep(500)
EndIf

Sleep(500)
ControlSend($mWin,"",$keyeditCtrlID,"a")											;sending key value as 'a'

Sleep(500)
if (ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"IsChecked")=1) then						;to uncheck the time delay between message block
	ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"UnCheck")

EndIf

Sleep(1000)
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

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow of J1939
$msgsec0=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment of tx window in j1939
ConsoleWrite("$msgsec0:" &$msgsec0 & @CRLF)

$sTimesec0=StringSplit($msgsec0,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec0:" &$sTimesec0[7] & @CRLF)														;Time of msg 1st sent

    Send("{a}")
    Sleep(500)
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


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow of J1939
$msgtrd1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgtrd1:" &$msgtrd1 & @CRLF)

$sTimetrd1=StringSplit($msgtrd1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrd1:" &$sTimetrd1[7] & @CRLF)														;Time of msg 1st sent



$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow of J1939
$msgtrd2=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgtrd2:" &$msgtrd2 & @CRLF)

$sTimetrd2=StringSplit($msgtrd2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrd2:" &$sTimetrd2[7] & @CRLF)									;Time of msg 1st sent

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

$timhWndnew=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow of J1939
$msgtrdnew=_GUICtrlListView_GetItemTextString($timhWndnew, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgtrdnew:" &$msgtrdnew & @CRLF)

$sTimetrdnew=StringSplit($msgtrdnew,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrdnew:" &$sTimetrdnew[7] & @CRLF)														;Time of msg 1st sent



         ;~              ;Second Senario
	if (ControlCommand($mWin,"",$updatectrlID,"IsChecked")=0) Then
		ControlCommand($mWin,"",$updatectrlID,"Check")
	EndIf
    Sleep(500)

$timemsglisthWnd=ControlGetHandle ($mWin,"",$msglistInst)										;handler for tx wimdow
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

ControlClick($mWin,"",$txmodemonoshotCtrlID)							;to select monoshot
  ; Check Key Trigger for Single msg type
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=0) Then
	ControlCommand($mWin,"",$keyctrlID,"Check")
	sleep(500)
EndIf

Sleep(500)
ControlSend($mWin,"",$keyeditCtrlID,"a")											;sending key value as 'a'

Sleep(500)

if (ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"IsChecked")=1) then						;to uncheck the time delay between message block
	ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"UnCheck")

EndIf
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


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow of J1939
$msgtrd3=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgtrd3:" &$msgtrd3 & @CRLF)

$sTimetrd3=StringSplit($msgtrd3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrd3:" &$sTimetrd3[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow of J1939
$msgtrd4=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgtrd4:" &$msgtrd4 & @CRLF)

$sTimetrd4=StringSplit($msgtrd4,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrd4:" &$sTimetrd4[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow of J1939
$msgtrdlast=_GUICtrlListView_GetItemTextString($timhWnd, 7)									;selecting last elment in message window
ConsoleWrite("$msgtrdlast:" &$msgtrdlast & @CRLF)

$sTimetrdlast=StringSplit($msgtrdlast,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrdlast:" &$sTimetrdlast[7] & @CRLF)														;Time of msg 1st sent

If ($sTimesec0[7]=0 And $sTimetrd1[7]=1 And $sTimetrd2[7]=0 And $sTimetrdnew[7]=1 And $sTimetrd3[7]=3 And $sTimetrd4[7]=0 And $sTimetrdlast[7]=0) Then

	_ExcelWriteCell($oExcel,"pass",8,11)

    Else
	  _ExcelWriteCell($oExcel,"Fail",8,11)

EndIf
