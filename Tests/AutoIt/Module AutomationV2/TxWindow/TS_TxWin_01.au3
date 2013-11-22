;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_001
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

_launchApp()
$timeDiff=0
$timeDiff1=0
WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0
if winexists($WIN_BUSMASTER) then
	$timeDiff=0
	$timeDiff1=0
    _loadConfig("TS_TxWin_01.cfx")
	sleep(1000)
	_TxMsgMenu()																				    ; Select CAN->Transmit->Configure menu
     sleep(1000)
 	_EnableAutoUpdate()                                                                             ;Enable Auto update
 	sleep(1000)
	_DeleteMsgBlock()                                                                               ;Delete message blocks if there are any
	sleep(1000)
	_ConfigCANTXBlockDetails("Cyclic","Yes",2000,"No","","","No","")							    ; Configure TX block details
     sleep(500)
	_AddMsg2TxList(0)																			    ; Add the first msg to Tx list
	sleep(500)
	_AddMsg2TxList(1)
	sleep(1000)
	_ConfigCANTXBlockDetails("Cyclic","Yes",2000,"No","","","No","")							    ; Configure TX block details
	_AddMsg2TxList(2)																			    ; Add the first msg to Tx list
	sleep(500)
	_AddMsg2TxList(3)

	_CloseTxWindow()																			    ; Close Tx window
	sleep(1000)
	_ConnectDisconnect()																		    ; Connect the tool
	_TransmitMsgsMenu()																			    ; Transmit normal blocks
	Sleep(5000)
	_ConnectDisconnect()																		    ; Disconnect the tool
	Sleep(1000)
	_DisableOverwriteMode()                                                                          ;Disable overwrite mode
    $rCount=_GetCANMsgWinItemCount()                                                               ;Fetch the no of entries in message window
	If $rCount>=8 Then
		$Data1=_GetMsgWinCANInfo(0)                                                                      ;Fetch messages from message window
	    $Data2=_GetMsgWinCANInfo(1)
	    $Data3=_GetMsgWinCANInfo(2)
	    $Data4=_GetMsgWinCANInfo(3)
		$Data5=_GetMsgWinCANInfo(4)
	    $Data6=_GetMsgWinCANInfo(5)
	    $Data7=_GetMsgWinCANInfo(6)
	    $Data8=_GetMsgWinCANInfo(7)
	    $Data9=_GetMsgWinCANInfo(8)

		For $i=0 to 7                                                                                    ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("Data1 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data2 :" &$Data2[$i] & @CRLF)
		    ConsoleWrite("Data3 :" &$Data3[$i] & @CRLF)
		    ConsoleWrite("Data4 :" &$Data4[$i] & @CRLF)
		    ConsoleWrite("Data5 :" &$Data5[$i] & @CRLF)
		    ConsoleWrite("Data6 :" &$Data6[$i] & @CRLF)
		    ConsoleWrite("Data7 :" &$Data7[$i] & @CRLF)
		    ConsoleWrite("Data8 :" &$Data8[$i] & @CRLF)
		    ConsoleWrite("Data9 :" &$Data9[$i] & @CRLF)
	    next

	    $FirstMsgTime=StringSplit($Data1[0],":")                                                          ;Split time as hours minutes and seconds
	    $FifthMsgTime=StringSplit($Data5[0],":")



	    If ($FifthMsgTime[3]>$FirstMsgTime[3]) Then                                                       ;Compare first message time and fifth message time in seconds to get time difference or delay
			$timeDiff=$FifthMsgTime[3]-$FirstMsgTime[3]
	    ElseIf($FifthMsgTime[3]<$FirstMsgTime[3]) Then
			If ($FifthMsgTime[3]=00 And $FirstMsgTime[3]=58) Then
				$timeDiff=2
		    ElseIf($FifthMsgTime[3]=01 And $FirstMsgTime[3]=59) Then
				$timeDiff=2
		    EndIf
	    EndIf
		ConsoleWrite("$timeDiff " & $timeDiff & @CRLF)
	    If $timeDiff=2 Then                                                                                ;Verify for time difference or delay
			$Time_cyclic=1
	    Else
		    $Time_cyclic=0
	    EndIf
	EndIf

		$handle=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)					                    ; Get handle of the 'Clear message window'  toolbar

	    _GUICtrlToolbar_ClickIndex($handle,0)											                    ; Click on 'Clear message window'  toolbar
	     sleep(1000)
	    _DeleteMsgBlock()

	_ConfigCANTXBlockDetails("Monoshot","Yes",2000,"No","","","No","")							       ; Configure TX block details

	_AddMsg2TxList(0)																			       ; Add the first msg to Tx list
	_AddMsg2TxList(1)

    _ConfigCANTXBlockDetails("Monoshot","Yes",2000,"No","","","No","")							       ; Configure TX block details

	_AddMsg2TxList(2)																			       ; Add the first msg to Tx list
	_AddMsg2TxList(3)

	_CloseTxWindow()

	sleep(1000)
	_ConnectDisconnect()																		       ; Connect the tool

	_TransmitMsgsMenu()																			       ; Transmit normal blocks
	Sleep(6000)
	_ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount1= _GetCANMsgWinItemCount()
	If $rCount1>=7 Then
		$MonoData1=_GetMsgWinCANInfo(0)                                                                     ;Fetch messages from message window
	    $MonoData2=_GetMsgWinCANInfo(1)
	    $MonoData3=_GetMsgWinCANInfo(2)
	    $MonoData4=_GetMsgWinCANInfo(3)
	    $MonoData5=_GetMsgWinCANInfo(4)
	    $MonoData6=_GetMsgWinCANInfo(5)
	    $MonoData7=_GetMsgWinCANInfo(6)
	    $MonoData8=_GetMsgWinCANInfo(7)


	    For $i=0 to 7                                                                                       ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("MonoData1 :" &$MonoData1[$i] & @CRLF)
		    ConsoleWrite("MonoData2 :" &$MonoData2[$i] & @CRLF)
		    ConsoleWrite("MonoData3 :" &$MonoData3[$i] & @CRLF)
		    ConsoleWrite("MonoData4 :" &$MonoData4[$i] & @CRLF)
         next

		$FirstMsgTime1=StringSplit($MonoData1[0],":")                                                       ;Split time as hours minutes and seconds
	    $FifthMsgTime1=StringSplit($MonoData5[0],":")
	    If ($FifthMsgTime1[3]>$FirstMsgTime1[3]) Then                                                       ;Compare first message time and fifth message time in seconds to get time difference or delay
			$timeDiff1=$FifthMsgTime1[3]-$FirstMsgTime1[3]
	    ElseIf($FifthMsgTime1[3]<$FirstMsgTime1[3]) Then
		    If ($FifthMsgTime1[3]=00 And $FirstMsgTime1[3]=58) Then
				$timeDiff1=2
		    ElseIf($FifthMsgTime1[3]=01 And $FirstMsgTime1[3]=59) Then
			    $timeDiff1=2
		    EndIf
	    EndIf

	    ConsoleWrite("$timeDiff1 " & $timeDiff1 & @CRLF)


		If $timeDiff1=2 Then                                                                                ;Verify for time difference or delay
			$time_monoshot=1
	    Else
		    $time_monoshot=0
	    EndIf
	EndIf

	If($Time_cyclic=1 And $time_monoshot=1) Then
		_WriteResult("Pass","TS_Tx_01")
	Else
		_WriteResult("Fail","TS_Tx_01")
	EndIf

EndIf
	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_01")
EndIf

ConsoleWrite("****End : TS_TxWin_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)