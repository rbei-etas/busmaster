;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_003
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
ControlClick($WIN_BUSMASTER,"",$RBTN_Monoshot_ConfigTX)							;to select monoshot

 ; Enabling Key Trigger
  If (ControlCommand($WIN_BUSMASTER,"",$CHKB_KeyTrig_ConfigTX,"IsChecked")=0) Then
	ControlCommand($WIN_BUSMASTER,"",$CHKB_KeyTrig_ConfigTX,"Check")
	sleep(500)
EndIf

Sleep(500)
ControlSend($WIN_BUSMASTER,"",$TXT_KeyValue_ConfigTX,"a")											;sending key value as 'a'

Sleep(500)
if (ControlCommand($WIN_BUSMASTER,"",$CHKB_BlockDelay_ConfigTX,"IsChecked")=1) then						;to uncheck the time delay between message block
	ControlCommand($WIN_BUSMASTER,"",$CHKB_BlockDelay_ConfigTX,"UnCheck")

EndIf

Sleep(1000)
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

$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow of J1939
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

    _TransmitMsgsMenu()						; Click on 'Transmit normal blocks' icon for disconnecting
	 sleep(1000)

 _ConnectDisconnect()
	sleep(1000)


$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow of J1939
$msgtrd1=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgtrd1:" &$msgtrd1 & @CRLF)

$sTimetrd1=StringSplit($msgtrd1,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrd1:" &$sTimetrd1[7] & @CRLF)														;Time of msg 1st sent



$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow of J1939
$msgtrd2=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgtrd2:" &$msgtrd2 & @CRLF)

$sTimetrd2=StringSplit($msgtrd2,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrd2:" &$sTimetrd2[7] & @CRLF)									;Time of msg 1st sent

_ConnectDisconnect()
sleep(1000)

_TransmitMsgsMenu()						; Click on 'Transmit normal blocks' icon for transmiting
sleep(1000)

Send("{a}")
Sleep(500)

ConnectDisconnect()
sleep(1000)

$timhWndnew=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow of J1939
$msgtrdnew=_GUICtrlListView_GetItemTextString($timhWndnew, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgtrdnew:" &$msgtrdnew & @CRLF)

$sTimetrdnew=StringSplit($msgtrdnew,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrdnew:" &$sTimetrdnew[7] & @CRLF)														;Time of msg 1st sent



; Second Senario
_EnableAutoUpdate()

    Sleep(500)
If ($sTimesec0[7]=0 And $sTimetrd1[7]=1 And $sTimetrd2[7]=0 And $sTimetrdnew[7]=1 ) Then
  $TS1="Pass"
    ConsoleWrite("Test scenario 1 is Pass")
  Else
		ConsoleWrite("Test scenario 1 is Fail")
         $TS1="Fail"
EndIf
; Second scenario
$timemsglisthWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LVC_MsgBlock_ConfigTX)										;handler for tx wimdow
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

ControlClick($WIN_BUSMASTER,"",$RBTN_Monoshot_ConfigTX)							;to select monoshot
  ; Check Key Trigger for Single msg type
  If (ControlCommand($WIN_BUSMASTER,"",$CHKB_KeyTrig_ConfigTX,"IsChecked")=0) Then
	ControlCommand($WIN_BUSMASTER,"",$CHKB_KeyTrig_ConfigTX,"Check")
	sleep(500)
EndIf

Sleep(500)
ControlSend($WIN_BUSMASTER,"",$TXT_KeyValue_ConfigTX,"a")											;sending key value as 'a'

Sleep(500)

if (ControlCommand($WIN_BUSMASTER,"",$CHKB_BlockDelay_ConfigTX,"IsChecked")=1) then						;to uncheck the time delay between message block
	ControlCommand($WIN_BUSMASTER,"",$CHKB_BlockDelay_ConfigTX,"UnCheck")

EndIf
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


$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow of J1939
$msgtrd3=_GUICtrlListView_GetItemTextString($timhWnd, 0)									;selecting 1st elment in message window
ConsoleWrite("$msgtrd3:" &$msgtrd3 & @CRLF)

$sTimetrd3=StringSplit($msgtrd3,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrd3:" &$sTimetrd3[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow of J1939
$msgtrd4=_GUICtrlListView_GetItemTextString($timhWnd, 2)									;selecting 3rd elment in message window
ConsoleWrite("$msgtrd4:" &$msgtrd4 & @CRLF)

$sTimetrd4=StringSplit($msgtrd4,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrd4:" &$sTimetrd4[7] & @CRLF)														;Time of msg 1st sent

$timhWnd=ControlGetHandle ($WIN_BUSMASTER,"",$LSTC_CANMsgWinInst4)								;handler for tx wimdow of J1939
$msgtrdlast=_GUICtrlListView_GetItemTextString($timhWnd, 7)									;selecting last elment in message window
ConsoleWrite("$msgtrdlast:" &$msgtrdlast & @CRLF)

$sTimetrdlast=StringSplit($msgtrdlast,"|")														;splitting 1st elements into column
ConsoleWrite("$sTimetrdlast:" &$sTimetrdlast[7] & @CRLF)														;Time of msg 1st sent

If ( $sTimetrd3[7]=3 And $sTimetrd4[7]=0 And $sTimetrdlast[7]=0) Then
 ConsoleWrite("Test scenario 2 is Pass")
    $TS2="Pass"
 Else
		ConsoleWrite("Test scenario 2 is Fail")
    $TS2="Fail"
EndIf

If $TS1="Pass" And $TS2="Pass" Then

	_ExcelWriteCell($oExcel, "Pass", 8, 2)
else
	 _ExcelWriteCell($oExcel,"Fail",8,2)

EndIf
