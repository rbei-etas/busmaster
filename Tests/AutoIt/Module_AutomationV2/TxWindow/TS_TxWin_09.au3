;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_008
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

 ;~ For  Enabling Auto update
	if (ControlCommand($mWin,"",$updatectrlID,"IsChecked")=0) Then
		ControlCommand($mWin,"",$updatectrlID,"Check")
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

if (ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"IsChecked")=0) then						;to uncheck the time delay between message block
	ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"Check")

EndIf

; Enabling Key Trigger
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=0) Then
	ControlCommand($mWin,"",$keyctrlID,"Check")
	sleep(500)
EndIf

ControlSend($mWin,"",$keyeditCtrlID,"s")
 sleep(1000)

if (ControlCommand($mWin,"",1007,"IsChecked")=0) then						;to select the cyclic
	ControlCommand($mWin,"",1007,"Check")

EndIf

 ControlSetText($mWin,"",$msgnamesCtrlID,81)   							; Enter Non Db msg 81
  ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button

ControlSetText($mWin,"",$msgnamesCtrlID,82)   							; Enter Non Db msg 82
ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button

$cntToolhWd=ControlGetHandle($mWin,"",$connectid)								; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							; Click on 'Connect' icon
sleep(500)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
sleep(500)
Send("{s}")
sleep(500)
Send("{s}")
Sleep(500)
 $cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

 ControlSetText($mWin,"",$msgnamesCtrlID,83)   							; Enter Non Db msg 83
  ControlClick($mWin, "",$addmsgCtrlID)									;~ ; Click on Add message button

$cntToolhWd=ControlGetHandle($mWin,"",$connectid)								; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							; Click on 'Connect' icon
sleep(1000)

	WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
sleep(500)
;~ Send("{s}")
;~ sleep(500)
;~ Send("{s}")
;~ sleep(500)
;~ Send("{s}")
;~ Sleep(500)

 $cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)


$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgninth1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msgninth1:" &$msgninth1 & @CRLF)

$sTimeninth1=StringSplit($msgninth1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimeninth1:" &$sTimeninth1[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgninth2=_GUICtrlListView_GetItemTextString($timhWnd, 1)									;selecting 3rd elment in message window
ConsoleWrite("$msgninth2:" &$msgninth2 & @CRLF)

$sTimeninth2=StringSplit($msgninth2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimeninth2:" &$sTimeninth2[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgninth3=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting last elment in message window
ConsoleWrite("$msgninth3:" &$msgninth3 & @CRLF)

$sTimeninth3=StringSplit($msgninth3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimeninth3:" &$sTimeninth3[7] & @CRLF)														;Time of msg 1st sent

If ($sTimeninth1[7]=81 And $sTimeninth2[7]=82 And $sTimeninth3[7]=83 ) Then

	_ExcelWriteCell($oExcel,"pass",14,2)

    Else
	  _ExcelWriteCell($oExcel,"Fail",14,2)

EndIf