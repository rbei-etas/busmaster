;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_11
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_011.au3****"&@CRLF)

 _launchApp()
$timeDiff=0
$timeDiff1=0
WinActivate($WIN_BUSMASTER)
Local $time_match=0,$time_match1=0
if winexists($WIN_BUSMASTER) then
	$timeDiff=0                                                                                       ;Initialize time difference values to '0'
	$timeDiff_all=0
	$timeDiff1=0
	$timeDiff2=0
    _loadConfig("TS_TxWin_01.cfx")
	sleep(1000)
	_TxMsgMenu()																				    ; Select CAN->Transmit->Configure menu
	sleep(1000)
 	_EnableAutoUpdate()
 	sleep(1000)
	_DeleteMsgBlock()
	sleep(1000)
    _ConfigCANTXBlockDetails("Monoshot","No","","No","","","Yes",2000)							            ; Configure TX block details
	_AddMsg2TxList(0)																			            ; Add the first msg to Tx list
	  sleep(500)
	_AddMsg2TxList(1)
      sleep(500)
    _ConfigCANTXBlockDetails("Monoshot","No","","No","","","Yes",2000)							            ; Configure TX block details
	_AddMsg2TxList(2)																			            ; Add the first msg to Tx list
	sleep(500)
	_AddMsg2TxList(3)
	sleep(500)
	_CloseTxWindow()
	sleep(1000)
	_ConnectDisconnect()																		             ; Connect the tool
	_TransmitMsgsMenu()																			             ; Transmit normal blocks
	 Sleep(5000)
    _ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount=_GetCANMsgWinItemCount()
	If $rCount>=7 Then
    	$MonoData1=_GetMsgWinCANInfo(0)                                                                          ;Fetch messages from message window
    	$MonoData2=_GetMsgWinCANInfo(1)
    	$MonoData3=_GetMsgWinCANInfo(2)
    	$MonoData4=_GetMsgWinCANInfo(3)
    	$MonoData5=_GetMsgWinCANInfo(4)
    	$MonoData6=_GetMsgWinCANInfo(5)
    	$MonoData7=_GetMsgWinCANInfo(6)
    	$MonoData8=_GetMsgWinCANInfo(7)
     	For $i=0 to 7                                                                                             ;Write messages with all information like(time,Channel,ID) to console
	    	ConsoleWrite("MonoData1 :" &$MonoData1[$i] & @CRLF)
	    	ConsoleWrite("MonoData2 :" &$MonoData2[$i] & @CRLF)
		    ConsoleWrite("MonoData3 :" &$MonoData3[$i] & @CRLF)
	    	ConsoleWrite("MonoData4 :" &$MonoData4[$i] & @CRLF)
	    next
        $FirstMsgTime1=StringSplit($MonoData1[0],":")                                                             ;Split time as hours minutes and seconds
    	$FifthMsgTime1=StringSplit($MonoData5[0],":")

		ConsoleWrite("$FifthMsgTime1[3] :" &$FifthMsgTime1[3] & @CRLF)
		ConsoleWrite("$FirstMsgTime1[3] :" &$FirstMsgTime1[3] & @CRLF)

    	If ($FifthMsgTime1[3]>$FirstMsgTime1[3]) Then                                                             ;Compare first message time and fifth message time in seconds to get time difference or delay
			$timeDiff=$FifthMsgTime1[3]-$FirstMsgTime1[3]
		ElseIf($FifthMsgTime1[3]<$FirstMsgTime1[3]) Then
	    	If ($FifthMsgTime1[3]=00 And $FirstMsgTime1[3]=58) Then
				$timeDiff=2
		    ElseIf($FifthMsgTime1[3]=01 And $FirstMsgTime1[3]=59) Then
			    $timeDiff=2
		     EndIf
		EndIf
	    ConsoleWrite("$timeDiff " & $timeDiff & @CRLF)
    	If $timeDiff=2 Then                                                                                        ;Verify for time difference or delay
     		$time_match=1
    	Else
		    $time_match=0
    	EndIf
	EndIf
    Sleep(1000)
    _TxMsgMenu()
    ControlSetText($WIN_BUSMASTER,"",$TXT_BlockDelay_ConfigTX,3000)
	sleep(500)
	_CloseTxWindow()
	_ConnectDisconnect()																		             ; Connect the tool
	_TransmitMsgsMenu()																			             ; Transmit normal blocks
	Sleep(6000)
    _ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount1=_GetCANMsgWinItemCount()
	If $rCount1>=7 Then
     	$MonoData1=_GetMsgWinCANInfo(0)                                                                          ;Fetch messages from message window
    	$MonoData2=_GetMsgWinCANInfo(1)
    	$MonoData3=_GetMsgWinCANInfo(2)
    	$MonoData4=_GetMsgWinCANInfo(3)
    	$MonoData5=_GetMsgWinCANInfo(4)
    	$MonoData6=_GetMsgWinCANInfo(5)
    	$MonoData7=_GetMsgWinCANInfo(6)
    	$MonoData8=_GetMsgWinCANInfo(7)

	    For $i=0 to 7                                                                                             ;Write messages with all information like(time,Channel,ID) to console
	    	ConsoleWrite("MonoData1 :" &$MonoData1[$i] & @CRLF)
	    	ConsoleWrite("MonoData2 :" &$MonoData2[$i] & @CRLF)
			ConsoleWrite("MonoData3 :" &$MonoData3[$i] & @CRLF)
	    	ConsoleWrite("MonoData4 :" &$MonoData4[$i] & @CRLF)
	    next
        $FirstMsgTime1=StringSplit($MonoData1[0],":")                                                             ;Split time as hours minutes and seconds
	    $FifthMsgTime1=StringSplit($MonoData5[0],":")

		ConsoleWrite("$FifthMsgTime1[3] :" &$FifthMsgTime1[3] & @CRLF)
		ConsoleWrite("$FirstMsgTime1[3] :" &$FirstMsgTime1[3] & @CRLF)

	    If ($FifthMsgTime1[3]>$FirstMsgTime1[3]) Then                                                             ;Compare first message time and fifth message time in seconds to get time difference or delay
	    	 $timeDiff1=$FifthMsgTime1[3]-$FirstMsgTime1[3]
    	 ElseIf($FifthMsgTime1[3]<$FirstMsgTime1[3]) Then
	    	If ($FifthMsgTime1[3]=00 And $FirstMsgTime1[3]=58) Then
				$timeDiff1=3
		    ElseIf($FifthMsgTime1[3]=01 And $FirstMsgTime1[3]=59) Then
			    $timeDiff1=3
		    EndIf
	     EndIf
		ConsoleWrite("$timeDiff1 " & $timeDiff1 & @CRLF)
     	If $timeDiff1=3 Then                                                                                        ;Verify for time difference or delay
	    	$time_match1=1
    	Else
	    	$time_match1=0
    	EndIf
	EndIf
    If ($time_match=1 And $time_match1=1) Then
		_WriteResult("Pass","TS_Tx_11")
	Else
		_WriteResult("Fail","TS_Tx_11")
	EndIf

EndIf
$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_11")
EndIf


ConsoleWrite("****End : TS_TxWin_11.au3****"&@CRLF)
ConsoleWrite(@CRLF)