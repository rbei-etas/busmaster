;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_002
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

;~   _launchApp()																			;launching the app
 WinActivate($WIN_BUSMASTER)
Sleep(1000)

;~ _loadConfig("TestcaseTx7")
 sleep(500)


;To delete all message block
$timemsglisthWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LVC_MsgBlock_ConfigTX)				;handler for tx wimdow
    $msg1=_GUICtrlListView_GetItemCount($timemsglisthWnd)										;To get the listview count
 	ConsoleWrite("$msg1 : " &$msg1 &@CRLF)

For  $i = 0 To $msg1
																						;~  TO delete all the message block
 ControlCommand($WIN_BUSMASTER,"",$BTN_DeleteBlock_ConfigTX,"IsEnabled")
		; click on delete button
		ControlClick($WIN_BUSMASTER,"",$BTN_DeleteBlock_ConfigTX)

		;click on confirmation button for deletion
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_Busmaster)
Next

sleep(1000)
 ControlClick($WIN_BUSMASTER,"",$BTN_AddBlock_ConfigTX)									;Adding the Block

;~ ControlClick($WIN_BUSMASTER,"",17002)
 ; Check Key Trigger for Single msg type
  If (ControlCommand($WIN_BUSMASTER,"",$CHKB_KeyTrig_ConfigTX,"IsChecked")=0) Then
	ControlCommand($WIN_BUSMASTER,"",$CHKB_KeyTrig_ConfigTX,"Check")
	sleep(500)
EndIf

Sleep(500)
ControlSend($WIN_BUSMASTER,"",$TXT_KeyValue_ConfigTX,"a")											;sending key value as 'a'

Sleep(500)
if (ControlCommand($WIN_BUSMASTER,"",$CHKB_MsgDelay_ConfigTX,"IsChecked")=1) then						;to uncheck the time delay between trigger
	ControlCommand($WIN_BUSMASTER,"",$CHKB_MsgDelay_ConfigTX,"UnCheck")

EndIf

Sleep(500)
 ControlSetText($WIN_BUSMASTER,"",$TXT_MsgID_ConfigTX,1)   							; Enter Non Db msg 11
  ControlClick($WIN_BUSMASTER, "",$BTN_AddMsg_ConfigTX)									;~ ; Click on Add message button

ControlSetText($WIN_BUSMASTER,"",$TXT_MsgID_ConfigTX,2)   							; Enter Non Db msg 12
ControlClick($WIN_BUSMASTER, "",$BTN_AddMsg_ConfigTX)									;~ ; Click on Add message button


	 ;~ 	For  Disabling Auto update
	if (ControlCommand($WIN_BUSMASTER,"",$BTN_AutoUpdate_ConfigTX,"IsChecked")=1) Then
		ControlCommand($WIN_BUSMASTER,"",$BTN_AutoUpdate_ConfigTX,"UnCheck")
	EndIf
    Sleep(500)

   ControlClick($WIN_BUSMASTER, "",$BTN_Update_ConfigTX)											;click on update button
   sleep(1000)

_ConnectDisconnect()
sleep(1000)

_TransmitMsgsMenu()						; Click on 'Transmit normal blocks' icon for transmiting
sleep(1000)


_ConnectDisconnect()
sleep(1000)

$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow
$msgsec1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgsec1:" &$msgsec1 & @CRLF)

$sTimesec1=StringSplit($msgsec1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec1:" &$sTimesec1[7] & @CRLF)														;Time of msg 1st sent
Sleep(500)

_ConnectDisconnect()
sleep(1000)

	_TransmitMsgsMenu()						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(1000)

    Send("{a}")
    Sleep(500)
	Send("{a}")
     Sleep(500)
	Send("{a}")

    _TransmitMsgsMenu()						; Click on 'Transmit normal blocks' icon for disconnecting
	 sleep(1000)

_ConnectDisconnect()
sleep(1000)


	$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow
$msgsec2=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message windowof
ConsoleWrite("$msgsec2:" &$msgsec2 & @CRLF)

$sTimesec2=StringSplit($msgsec2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec2:" &$sTimesec2[7] & @CRLF)														;Time of msg 1st sent



	$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow
$msgsec3=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgsec3:" &$msgsec3 & @CRLF)

$sTimesec3=StringSplit($msgsec3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec3:" &$sTimesec3[7] & @CRLF)														;Time of msg 1st sent

_EnableAutoUpdate()
Sleep(500)

If ($sTimesec1[7]=1 And $sTimesec2[7]=1 And $sTimesec3[7]=1 ) Then
	  $TS1="Pass"
    ConsoleWrite("Test scenario 1 is Pass")
  Else
		ConsoleWrite("Test scenario 1 is Fail")
         $TS1="Fail"
EndIf


;~        Second scenario

$timemsglisthWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LVC_MsgBlock_ConfigTX)				;handler for tx wimdow
    $msg1=_GUICtrlListView_GetItemCount($timemsglisthWnd)										;To get the listview count
 	ConsoleWrite("$msg1 : " &$msg1 &@CRLF)

For  $i = 0 To $msg1
																						;~  TO delete all the message block
 ControlCommand($WIN_BUSMASTER,"",$BTN_DeleteBlock_ConfigTX,"IsEnabled")
		; click on delete button
		ControlClick($WIN_BUSMASTER,"",$BTN_DeleteBlock_ConfigTX)

		;click on confirmation button for deletion
		ControlClick($WIN_BUSMASTER,"",$BTN_Yes_Busmaster)
Next

sleep(1000)
 ControlClick($WIN_BUSMASTER,"",$BTN_AddBlock_ConfigTX)									;Adding the Block

  ; Check Key Trigger for Single msg type
  If (ControlCommand($WIN_BUSMASTER,"",$CHKB_KeyTrig_ConfigTX,"IsChecked")=0) Then
	ControlCommand($WIN_BUSMASTER,"",$CHKB_KeyTrig_ConfigTX,"Check")
	sleep(500)
EndIf

Sleep(500)
ControlSend($WIN_BUSMASTER,"",$TXT_KeyValue_ConfigTX,"a")											;sending key value as 'a'

Sleep(500)

if (ControlCommand($WIN_BUSMASTER,"",$CHKB_MsgDelay_ConfigTX,"IsChecked")=1) then						;to uncheck the time delay between trigger
	ControlCommand($WIN_BUSMASTER,"",$CHKB_MsgDelay_ConfigTX,"UnCheck")

EndIf

ControlCommand($WIN_BUSMASTER,"",$COMB_KeyType_ConfigTX,"SelectString","All")				;selecting "All"messsage type

Sleep(500)
 ControlSetText($WIN_BUSMASTER,"",$TXT_MsgID_ConfigTX,3)   							; Enter Non Db msg 3
  ControlClick($WIN_BUSMASTER, "",$BTN_AddMsg_ConfigTX)									;~ ; Click on Add message button

ControlSetText($WIN_BUSMASTER,"",$TXT_MsgID_ConfigTX,4)   							; Enter Non Db msg 4
ControlClick($WIN_BUSMASTER, "",$BTN_AddMsg_ConfigTX)									;~ ; Click on Add message button

_ConnectDisconnect()
sleep(1000)

	_TransmitMsgsMenu()						; Click on 'Transmit normal blocks' icon for transmiting
	sleep(1000)

    Send("{a}")
    Sleep(500)

_ConnectDisconnect()
sleep(1000)


$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow
$msgsec4=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting  1st elment in message window
ConsoleWrite("$msgsec4:" &$msgsec4 & @CRLF)

$sTimesec4=StringSplit($msgsec4,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec4:" &$sTimesec4[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow
$msgsec5=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgsec5:" &$msgsec5 & @CRLF)

$sTimesec5=StringSplit($msgsec5,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimesec5:" &$sTimesec5[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow
$msglast=_GUICtrlListView_GetItemTextString($timhWnd, 7)									;selecting last elment in message window
ConsoleWrite("$msglast:" &$msglast & @CRLF)

$sTimelast=StringSplit($msglast,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimelast:" &$sTimelast[7] & @CRLF)													;Time of msg 1st sent

If  $sTimesec4[7]=3 And $sTimesec5[7]=3 And $sTimelast[7]=0 Then
	  ConsoleWrite("Test scenario 2 is Pass")
    $TS2="Pass"
 Else
		ConsoleWrite("Test scenario 2 is Fail")
    $TS2="Fail"
EndIf

If $TS1="Pass" And $TS2="Pass" Then

	_ExcelWriteCell($oExcel, "Pass", 7, 2)
else
	 _ExcelWriteCell($oExcel,"Fail",7,2)

EndIf

