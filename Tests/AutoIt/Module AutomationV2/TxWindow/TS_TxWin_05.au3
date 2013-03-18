;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_005
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

Sleep(5000)
; Disabling Key Trigger
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=1) Then
	ControlCommand($mWin,"",$keyctrlID,"UnCheck")

EndIf
sleep(1000)

$cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
sleep(500)

WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
sleep(3000)

;~ Send("{b}")

 $cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)			;handler for tx wimdow
$msgfifth1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgfifth1:" &$msgfifth1 & @CRLF)

$sTimefifth1=StringSplit($msgfifth1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimefifth1:" &$sTimefifth1[7] & @CRLF)														;Time of msg 1st sent



$timhWnd1=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow o
$msgfifth2=_GUICtrlListView_GetItemTextString($timhWnd1, 1)									;selecting 2nd elment in message window
ConsoleWrite("$msgfifth2:" &$msgfifth2 & @CRLF)

$sTimefifth2=StringSplit($msgfifth2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimefifth2:" &$sTimefifth2[7] & @CRLF)									;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgfifth3=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgfifth3:" &$msgfifth3 & @CRLF)

$sTimefifth3=StringSplit($msgfifth3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimefifth3:" &$sTimefifth3[7] & @CRLF)														;Time of msg 1st sent

$timhWnd1=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgfifth4=_GUICtrlListView_GetItemTextString($timhWnd1, 3)									;selecting 4th elment in message window
ConsoleWrite("$msgfifth4:" &$msgfifth4 & @CRLF)

$sTimefifth4=StringSplit($msgfifth4,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimefifth4:" &$sTimefifth4[7] & @CRLF)									;Time of msg 1st sent

Sleep(1000)

If ($sTimefifth1[7]=11 And $sTimefifth2[7]=12 And $sTimefifth3[7]=21 And $sTimefifth4[7]=22 ) Then
  $TS1="Pass"
    ConsoleWrite("Test scenario 1 is Pass")
  Else
		ConsoleWrite("Test scenario 1 is Fail")
         $TS1="Fail"
EndIf
      ;~     ; Second senario


; Enabling Key Trigger
  If (ControlCommand($mWin,"",$keyctrlID,"IsChecked")=0) Then
	ControlCommand($mWin,"",$keyctrlID,"Check")
	sleep(500)
EndIf


if (ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"IsChecked")=1) then						;to uncheck the time delay between message block
	ControlCommand($mWin,"",$timedelayBtwblckCtrlID,"UnCheck")

EndIf

Sleep(1000)

if (ControlCommand($mWin,"",$timdeltrg,"IsChecked")=1) then						;to uncheck the time delay between trigger
	ControlCommand($mWin,"",$timdeltrg,"UnCheck")
EndIf

$cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of tollbar
$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)														; Click on 'Connect' icon
sleep(1000)

	WinMenuSelectItem($mWin,"",$fuctions,$transmit,$normalbock)						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(500)

    Send("{b}")

 $cntToolhWd=ControlGetHandle($mWin,"",$connectid)												; Get handle of toolbar
	$conn=_GUICtrlToolbar_ClickIndex($cntToolhWd,4)										; Click on 'DisConnect' icon
	sleep(1000)

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)			;handler for tx wimdow
$msgfifth5=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgfifth5:" &$msgfifth5 & @CRLF)

$sTimefifth5=StringSplit($msgfifth5,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimefifth5:" &$sTimefifth5[7] & @CRLF)														;Time of msg 1st sent



$timhWnd1=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow o
$msgfifth6=_GUICtrlListView_GetItemTextString($timhWnd1, 1)									;selecting 2nd elment in message window
ConsoleWrite("$msgfifth6:" &$msgfifth6 & @CRLF)

$sTimefifth6=StringSplit($msgfifth6,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimefifth6:" &$sTimefifth6[7] & @CRLF)									;Time of msg 1st sent

$timhWnd=ControlGetHandle ($mWin,"",$msgwin)								;handler for tx wimdow
$msgfifth7=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgfifth7:" &$msgfifth7 & @CRLF)

$sTimefifth7=StringSplit($msgfifth7,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimefifth7:" &$sTimefifth7[7] & @CRLF)											;Time of msg 1st sent

If ( $sTimefifth5[7]=11 And $sTimefifth6[7]=12 And $sTimefifth7[7]=11 ) Then
     ConsoleWrite("Test scenario 2 is Pass")
    $TS2="Pass"
 Else
		ConsoleWrite("Test scenario 2 is Fail")
    $TS2="Fail"
EndIf

If $TS1="Pass" And $TS2="Pass" Then

	_ExcelWriteCell($oExcel, "Pass", 10, 2)
else
	 _ExcelWriteCell($oExcel,"Fail",10,2)

EndIf
;~ ControlClick($mWin,"",1015)
