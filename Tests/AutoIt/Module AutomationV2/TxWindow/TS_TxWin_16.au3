
 ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_16.au3****"&@CRLF)
  _launchApp()

 WinActivate($WIN_BUSMASTER)
 Local $Time_match=0,$Data_DB=0,$Data_NonDB=0
if winexists($WIN_BUSMASTER) then
    _loadConfig("TS_TxWin_01.cfx")
	sleep(1000)
	_TxMsgMenu()																				    ; Select CAN->Transmit->Configure menu
     sleep(1000)
 	_EnableAutoUpdate()                                                                             ;Enable Auto update
 	sleep(1000)
	_DeleteMsgBlock()    																					 ;Delete message blocks if there are any
 	sleep(1000)
	_ConfigCANTXBlockDetails("Monoshot","No","","No","","","Yes",2000)
	_AddMsg2TxList(0)
	_AddMsg2TxList(1)
     sleep(500)
    _ConfigCANTXBlockDetails("Monoshot","No","","No","","","Yes",2000)
    sleep(500)
	 _AddNonDBMsg2TxList(77)
	 _AddNonDBMsg2TxList(88)
	_CloseTxWindow()
 	_ConnectDisconnect()																		                      ; Connect the tool
	_TransmitMsgsMenu()
	 sleep(6000)
	_ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount=_GetCANMsgWinItemCount()
    If $rCount>=7 Then
    	$Data1=_GetMsgWinCANInfo(0)                                                             ;Fetch messages from message window
    	$Data2=_GetMsgWinCANInfo(1)
    	$Data3=_GetMsgWinCANInfo(2)
    	$Data4=_GetMsgWinCANInfo(3)
    	$Data5=_GetMsgWinCANInfo(4)
    	$Data6=_GetMsgWinCANInfo(5)
    	$Data7=_GetMsgWinCANInfo(6)
    	$Data8=_GetMsgWinCANInfo(7)
;~     	$Data9=_GetMsgWinCANInfo(8)
;~     	$Data10=_GetMsgWinCANInfo(9)
    	For $i=0 to 7                                                                                  ;Write messages with all information like(time,Channel,ID) to console
	    	ConsoleWrite("Data1 :" &$Data1[$i] & @CRLF)
	    	ConsoleWrite("Data2 :" &$Data2[$i] & @CRLF)
	     	ConsoleWrite("Data3 :" &$Data3[$i] & @CRLF)
	    	ConsoleWrite("Data4 :" &$Data4[$i] & @CRLF)
	    	ConsoleWrite("Data5 :" &$Data5[$i] & @CRLF)
	    	ConsoleWrite("Data6 :" &$Data6[$i] & @CRLF)
	    	ConsoleWrite("Data7 :" &$Data7[$i] & @CRLF)
	    	ConsoleWrite("Data8 :" &$Data8[$i] & @CRLF)
;~ 	    	ConsoleWrite("Data9 :" &$Data9[$i] & @CRLF)
;~ 	    	ConsoleWrite("Data10 :"&$Data10[$i] & @CRLF)
         next
		$FirstMsgTime=StringSplit($Data1[0],":")                                                       ;Split time as hours minutes and seconds
		$FifthMsgTime=StringSplit($Data5[0],":")

	    If ($FifthMsgTime[3]>$FirstMsgTime[3]) Then                                                     ;Compare first message time and fifth message time in seconds to get time difference or delay
	    	$timeDiff=$FifthMsgTime[3]-$FirstMsgTime[3]
    	ElseIf($FifthMsgTime[3]<$FirstMsgTime[3]) Then
	    	If ($FifthMsgTime[3]=00 And $FirstMsgTime[3]=58) Then
				$timeDiff=2
	    	ElseIf($FifthMsgTime[3]=01 And $FirstMsgTime[3]=59) Then
				$timeDiff=2
		    EndIf
		EndIf
	    ConsoleWrite("$timeDiff " & $timeDiff & @CRLF)
		If $timeDiff=2 Then                                                                               ;Verify for time difference or delay
	        $Time_match=1
	    Else
		    $Time_match=0
        EndIf
	    ConsoleWrite("Time match " & $Time_match & @CRLF)
        If ($Data1[4]=0x015  And $Data3[4]=0x020) Then                                      ; Verify for Id
	    	$Data_DB=1
    	Else
		    $Data_DB=0
    	EndIf
        If($Data5[4]=0x077 And $Data7[4]=0x088) Then
	        $Data_NonDB=1
	    Else
	    	 $Data_NonDB=0
	     EndIf
	 EndIf
	ConsoleWrite("DB data" & $Data_DB & @CRLF)
	ConsoleWrite("non DB " & $Data_NonDB & @CRLF)
	If($Data_DB=1 And $Data_NonDB=1 And $Time_match=1) Then
		_WriteResult("Pass","TS_Tx_16")
	Else
		_WriteResult("Fail","TS_Tx_16")

	EndIf
EndIf
$isAppNotRes=_CloseApp()														; Close the app
if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_16")
EndIf

ConsoleWrite("****End : TS_TxWin_16.au3****"&@CRLF)
ConsoleWrite(@CRLF)







