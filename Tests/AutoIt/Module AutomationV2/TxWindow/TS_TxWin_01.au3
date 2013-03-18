;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_001
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

;~   _launchApp()																			;launching the app
 WinActivate($WIN_BUSMASTER)
Sleep(1000)

;~ _loadConfig("TestcaseTx7")
 sleep(500)
 _TxWindowOpen()

$timemsglisthWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LVC_MsgBlock_ConfigTX)				; handler for tx wimdow
    $msg1=_GUICtrlListView_GetItemCount($timemsglisthWnd)										;To get the listview count
 	ConsoleWrite("$msg1 : " &$msg1 &@CRLF)

For  $i = 0 To $msg1
																						;~  To delete all the message block
 ControlCommand($WIN_BUSMASTER,"",$BTN_DeleteBlock_ConfigTX,"IsEnabled")
		; click on delete button
		ControlClick($WIN_BUSMASTER,"",$BTN_DeleteBlock_ConfigTX)

		;click on confirmation button for deletion
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_Busmaster)
Next

 sleep(1000)
 ControlClick($WIN_BUSMASTER,"",$BTN_AddBlock_ConfigTX)									; Adding the Block
 ControlSetText($WIN_BUSMASTER,"",$TXT_MsgID_ConfigTX,11)   							; Enter Non Db msg 11
 ;~ ; Click on Add message button
  ControlClick($WIN_BUSMASTER, "",$BTN_AddMsg_ConfigTX)

ControlSetText($WIN_BUSMASTER,"",$TXT_MsgID_ConfigTX,12)   							; Enter Non Db msg 12
 ;~ ; Click on Add message button
  ControlClick($WIN_BUSMASTER, "",$BTN_AddMsg_ConfigTX)

	_ConnectDisconnect()

	sleep(1000)

	_TransmitMsgsMenu()					; Click on 'Transmit normal blocks' icon for transmiting
	sleep(1000)

    _TransmitMsgsMenu()					; Click on 'Transmit normal blocks' icon for disconnecting
	 sleep(1000)

$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow of J1939
$msg1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment of tx window in j1939
ConsoleWrite("$msg1:" &$msg1 & @CRLF)

$sTime1=StringSplit($msg1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime1:" &$sTime1[7] & @CRLF)														;Time of msg 1st sent


$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)							;handler for tx wimdow
$msg2=_GUICtrlListView_GetItemTextString($timhWnd, 1)
ConsoleWrite("$msg2:" &$msg2 & @CRLF)

$sTime2=StringSplit($msg2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime2:" &$sTime2[7] & @CRLF)											;Time of msg 2nd sent


$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)							;handler for tx wimdow
$msg3=_GUICtrlListView_GetItemTextString($timhWnd, 2)
ConsoleWrite("$msg3:" &$msg3 & @CRLF)

$sTime3=StringSplit($msg3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime3:" &$sTime3[7] & @CRLF)											;Time of msg 3rd sent

$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)							;handler for tx wimdow
$msg4=_GUICtrlListView_GetItemTextString($timhWnd, 3)
ConsoleWrite("$msg4:" &$msg4 & @CRLF)

$sTime4=StringSplit($msg4,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime4:" &$sTime4[7] & @CRLF)											;Time of msg 3rd sent

_ConnectDisconnect()
sleep(1000)

;~ 	For  Disabling Auto update
	if (ControlCommand($WIN_BUSMASTER,"",$BTN_AutoUpdate_ConfigTX,"IsChecked")=1) Then
		ControlCommand($WIN_BUSMASTER,"",$BTN_AutoUpdate_ConfigTX,"UnCheck")
	EndIf
    Sleep(500)

If ($sTime1[7]=11 And $sTime2[7]=12 And $sTime3[7]=11 And $sTime4[7]=12) Then
    $TS1="Pass"
    ConsoleWrite("Test scenario 1 is Pass")
 Else
	ConsoleWrite("Test scenario 1 is Fail")
    $TS1="Fail"
EndIf


For  $i = 0 To $msg1
																						;~  TO delete all the message block
 ControlCommand($WIN_BUSMASTER,"",$BTN_DeleteBlock_ConfigTX,"IsEnabled")
		; click on delete button
		ControlClick($WIN_BUSMASTER,"",$BTN_DeleteBlock_ConfigTX)

		;click on confirmation button for deletion
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_Busmaster)
Next

 sleep(1000)
 ControlClick($WIN_BUSMASTER,"",$AddblckctrlID)													;adding  the message block

 ControlClick($WIN_BUSMASTER,"",$txmodemonoshotCtrlID)															;click on monoshot

ControlSetText($WIN_BUSMASTER,"",$TXT_MsgID_ConfigTX,21)   							; Enter Non Db msg 21
 ;~ ; Click on Add message button
  ControlClick($WIN_BUSMASTER, "",$BTN_AddMsg_ConfigTX)

ControlSetText($WIN_BUSMASTER,"",$TXT_MsgID_ConfigTX,22)   							; Enter Non Db msg 22
 ;~ ; Click on Add message button
  ControlClick($WIN_BUSMASTER, "",$BTN_AddMsg_ConfigTX)
 sleep(1000)

  ControlClick($WIN_BUSMASTER, "",$BTN_Update_ConfigTX)											;click on update button
 sleep(1000)

_ConnectDisconnect()
	sleep(1000)

_TransmitMsgsMenu()					; Click on 'Transmit normal blocks' icon for transmiting
	sleep(1000)

_TransmitMsgsMenu()					; Click on 'Transmit normal blocks' icon for disconnecting
	 sleep(1000)

$timhWnd1=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)							;handler for tx wimdow
$msg6=_GUICtrlListView_GetItemTextString($timhWnd1, 0)
ConsoleWrite("$msg6:" &$msg6 & @CRLF)

$sTime5=StringSplit($msg6,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime5:" &$sTime5[7] & @CRLF)											;Time of msg 2nd sent


$timhWnd1=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)							;handler for tx wimdow
$msg7=_GUICtrlListView_GetItemTextString($timhWnd1, 1)
ConsoleWrite("$msg7:" &$msg7 & @CRLF)

$sTime6=StringSplit($msg7,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime6:" &$sTime6[7] & @CRLF)											;Time of msg 3rd sent

$timhWnd1=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)							;handler for tx wimdow
$msg8=_GUICtrlListView_GetItemTextString($timhWnd1, 2)
ConsoleWrite("$msg8:" &$msg8 & @CRLF)

$sTime7=StringSplit($msg8,"|")														;splitting 1st elements into column
ConsoleWrite("$sTime7:" &$sTime7[7] & @CRLF)											;Time of msg 3rd sent

;~ 	For  Enabling Auto update
	_EnableAutoUpdate()
    Sleep(500)

_ConnectDisconnect()
sleep(1000)

If ($sTime5[7]=21 And $sTime6[7]=22 And $sTime7[7]=0)  Then
     ConsoleWrite("Test scenario 2 is Pass")
    $TS2="Pass"
 Else
		ConsoleWrite("Test scenario 2 is Fail")
    $TS2="Fail"
EndIf

If $TS1="Pass" And $TS2="Pass" Then

	_ExcelWriteCell($oExcel, "Pass", 6, 2)
else
	 _ExcelWriteCell($oExcel,"Fail",6,2)

EndIf