;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_005
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_005.au3****"&@CRLF)

_launchApp()
$timeDiff=0
$timeDiff1=0
WinActivate($WIN_BUSMASTER)
Local $time_match=0,$Channel_Mono=0,$Dir_Mono=0,$Id_cyclic1=0
if winexists($WIN_BUSMASTER) then
	$timeDiff=0                                                                                       ;Initialize time difference values to '0'
	$timeDiff_all=0
	$timeDiff1=0
	$timeDiff2=0
   _loadConfig("TS_TxWin_01.cfx")
	sleep(1000)
	_TxMsgMenu()																				      ; Select CAN->Transmit->Configure menu
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
    _ConfigCANTXBlockDetails("Monoshot","No","","Yes","c","","Yes",2000)							                 ; Configure TX block details
	_AddMsg2TxList(2)																			                     ; Add the first msg to Tx list
	_AddMsg2TxList(3)
	Sleep(500)
	_CloseTxWindow()
	sleep(500)
	_ConnectDisconnect()																		                      ; Connect the tool
	_TransmitMsgsMenu()
     sleep(5000)
     _ConnectDisconnect()
	 _DisableOverwriteMode()
	$rCount=_GetCANMsgWinItemCount()
	If $rCount>=7 Then

		$MonoData1=_GetMsgWinCANInfo(0)                                                                                    ;Fetch messages from message window
	    $MonoData2=_GetMsgWinCANInfo(1)
	    $MonoData2=_GetMsgWinCANInfo(1)
	    $MonoData3=_GetMsgWinCANInfo(2)
	    $MonoData4=_GetMsgWinCANInfo(3)
	    $MonoData5=_GetMsgWinCANInfo(4)
		$MonoData6=_GetMsgWinCANInfo(5)
	    $MonoData7=_GetMsgWinCANInfo(6)
	    $MonoData8=_GetMsgWinCANInfo(7)
		For $i=0 to 7                                                                                                     ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("MonoData1 :" &$MonoData1[$i] & @CRLF)
		    ConsoleWrite("MonoData2 :" &$MonoData2[$i] & @CRLF)
		    ConsoleWrite("MonoData3 :" &$MonoData3[$i] & @CRLF)
		    ConsoleWrite("MonoData4 :" &$MonoData4[$i] & @CRLF)
		    ConsoleWrite("MonoData5 :" &$MonoData5[$i] & @CRLF)
		next
	    $FirstMsgTime1=StringSplit($MonoData1[0],":")                                                                    ;Split time as hours minutes and seconds
	    $FifthMsgTime1=StringSplit($MonoData5[0],":")
	    ConsoleWrite("FirstMsgTime1" & $FirstMsgTime1 & @CRLF)
	    ConsoleWrite("FifthMsgTime1" & $FifthMsgTime1 & @CRLF)

	    If ($FifthMsgTime1[3]>$FirstMsgTime1[3]) Then                                                                   ;Compare first message time and fifth message time in seconds to get time difference or delay
			$timeDiff1=$FifthMsgTime1[3]-$FirstMsgTime1[3]
	    ElseIf($FifthMsgTime1[3]<$FirstMsgTime1[3]) Then
		    If ($FifthMsgTime1[3]=00 And $FirstMsgTime1[3]=58) Then
				$timeDiff1=2
		    ElseIf($FifthMsgTime1[3]=01 And $FirstMsgTime1[3]=59) Then
			    $timeDiff1=2
			EndIf
	    EndIf

	    If $timeDiff1=2 Then                                                                                              ;Verify for time difference or delay
			$time_match=1
	    Else
		    $time_match=0
	    EndIf

	    If ($MonoData1[1]="Tx" And $MonoData5[1]="Tx" And $MonoData7[1]="Tx") Then                                         ;Verify for Tx/RX
			$Dir_Mono=1
		Else
		    $Dir_Mono=0
		EndIf

	    If ($MonoData1[2]=1 And $MonoData3[2]=1 And $MonoData5[2]=1) Then                                                  ;Verify for Channel
		    $Channel_Mono=1
	    Else
	        $Channel_Mono=0
	    EndIf
	EndIf

	If ($time_match=1  And  $Dir_Mono=1 And  $Channel_Mono=1) Then
		$Pass_transmit_all=1
	 Else
		$Pass_transmit_all=0
	EndIf
	$handle=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)					                                 ; Get handle of the 'Clear message window'  toolbar
	_GUICtrlToolbar_ClickIndex($handle,0)											                                 ; Click on 'Clear message window'  toolbar
	sleep(1000)
	_TxMsgMenu()
	$handle1=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgBlk_NameID)
	_GUICtrlListView_SetItemChecked($handle1,0,False)                                                               ;Unchecks time triggered block
     sleep(1000)
	_CloseTxWindow()
	sleep(500)
	_ConnectDisconnect()																		                      ; Connect the tool
	_TransmitMsgsMenu()
     sleep(4000)
	 send("{c}")

	sleep(3000)
    send("{c}")

	sleep(3000)
    _ConnectDisconnect()
	_DisableOverwriteMode()
    $rCount1=_GetCANMsgWinItemCount()
	If $rCount1>=6 Then
		$MonoData1=_GetMsgWinCANInfo(0)                                                                                    ;Fetch messages from message window
	    $MonoData2=_GetMsgWinCANInfo(1)
	    $MonoData2=_GetMsgWinCANInfo(2)
	    $MonoData3=_GetMsgWinCANInfo(3)
	    $MonoData4=_GetMsgWinCANInfo(4)
	    $MonoData5=_GetMsgWinCANInfo(5)
	    $MonoData6=_GetMsgWinCANInfo(6)

	    For $i=0 to 7                                                                                                     ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("MonoData1 :" &$MonoData1[$i] & @CRLF)
		    ConsoleWrite("MonoData2 :" &$MonoData2[$i] & @CRLF)
		    ConsoleWrite("MonoData3 :" &$MonoData3[$i] & @CRLF)
		    ConsoleWrite("MonoData4 :" &$MonoData4[$i] & @CRLF)
		    ConsoleWrite("MonoData5 :" &$MonoData5[$i] & @CRLF)
        next
        If ($MonoData1[4]= 0x025 And $MonoData5[4]=0x025 )  Then                                                          ;Verify for message ID
			$Id_cyclic1=1
	    Else
		    $Id_cyclic1=0
		EndIf
	EndIf

	If ($Id_cyclic1=1 And $Pass_transmit_all=1) Then
		_WriteResult("Pass","TS_Tx_05")
	Else
		_WriteResult("Fail","TS_Tx_05")
	EndIf
EndIf
$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_05")
EndIf

ConsoleWrite("Id_cyclic is " & $Id_cyclic1)

ConsoleWrite("****End : TS_TxWin_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)