;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_003
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

                                                                                  ;Delete previous message blocks

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_003.au3****"&@CRLF)

_launchApp()
$timeDiff=0
$timeDiff1=0
WinActivate($WIN_BUSMASTER)
Local $time_match=0,$time_match1=0,$Channel_Mono=0,$Channel_Mono1=0,$Dir_Mono=0,$Id_match=0,$Id_match1=0
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
	_ConfigCANTXBlockDetails("Monoshot","Yes",2000,"Yes","c","","No","")							    ; Configure TX block details
	_AddMsg2TxList(0)																			        ; Add the first msg to Tx list
	_AddMsg2TxList(1)
	Sleep(500)

	_ConfigCANTXBlockDetails("Monoshot","Yes",2000,"Yes","d","","No","")							    ; Configure TX block details
	_AddMsg2TxList(2)
	_AddMsg2TxList(3)																			        ; Add the first msg to Tx list
	Sleep(500)
	_CloseTxWindow()
     sleep(500)
	_ConnectDisconnect()																		      ; Connect the tool
	_TransmitMsgsMenu()																			      ; Transmit normal blocks
	Sleep(5000)
	send("{c}")                                                                                       ;Press key 'c'
	sleep(1500)
	send("{d}")                                                                                       ;Press key 'd'
	sleep(1500)

	_ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount=_GetCANMsgWinItemCount()
	If $rCount>=7 Then
		$MonoData1=_GetMsgWinCANInfo(0)                                                                   ;Fetch messages from message window
	    $MonoData2=_GetMsgWinCANInfo(1)
	    $MonoData3=_GetMsgWinCANInfo(2)
	    $MonoData4=_GetMsgWinCANInfo(3)
	    $MonoData5=_GetMsgWinCANInfo(4)
	    $MonoData6=_GetMsgWinCANInfo(5)
	    $MonoData7=_GetMsgWinCANInfo(6)
	    $MonoData8=_GetMsgWinCANInfo(7)
	    For $i=0 to 7                                                                                     ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("MonoData1 :" &$MonoData1[$i] & @CRLF)
		    ConsoleWrite("MonoData2 :" &$MonoData2[$i] & @CRLF)
		    ConsoleWrite("MonoData3 :" &$MonoData3[$i] & @CRLF)
		    ConsoleWrite("MonoData4 :" &$MonoData4[$i] & @CRLF)
        next
		$FirstMsgTime1=StringSplit($MonoData1[0],":")                                                    ;Split time as hours minutes and seconds
	    $FifthMsgTime1=StringSplit($MonoData5[0],":")
	    If ( $FifthMsgTime1[3]>$FirstMsgTime1[3]) Then                                                   ;Compare first message time and fifth message time in seconds to get time difference or delay
			$timeDiff1= $FifthMsgTime1[3]-$FirstMsgTime1[3]
	    ElseIf( $FifthMsgTime1[3]<$FirstMsgTime1[3]) Then
			If ( $FifthMsgTime1[3]=00 And $FirstMsgTime1[3]=58) Then
				$timeDiff1=2
		    ElseIf( $FifthMsgTime1[3]=01 And $FirstMsgTime1[3]=59) Then
			    $timeDiff1=2
			EndIf
		EndIf

	    ConsoleWrite("timeDiff1 " & $timeDiff1 & @CRLF)
	    If $timeDiff1=2 Then                                                                             ;Verify for time difference or delay
			$time_match=1
	    Else
		    $time_match=0
	    EndIf

	    If ($MonoData1[1]="Tx" And $MonoData5[1]="Tx" And $MonoData7[1]="Tx") Then                       ;Verify for Tx/RX
		    $Dir_Mono=1
	    Else
		    $Dir_Mono=0
	    EndIf

	    If ($MonoData1[2]=1 And $MonoData3[2]=1 And $MonoData5[2]=1) Then                                ;Verify for Channel
			$Channel_Mono=1
	    Else
		    $Channel_Mono=0
	    EndIf
		If ($MonoData1[4]=0x025 Or 0x015) Then                                                          ;Verify for message ID
			$Id_match=1
	    Else
		    $Id_match=0
	    EndIf
	EndIf

	    ConsoleWrite("$time_match :" & $time_match & @CRLF)
	    ConsoleWrite("$Id_match :" & $Id_match & @CRLF)
		ConsoleWrite("$Channel_Mono" & $Channel_Mono & @CRLF)
	    ConsoleWrite("$Dir_Mono" & $Dir_Mono & @CRLF)

	If ($time_match=1 And $Id_match=1 And  $Dir_Mono=1 And  $Channel_Mono=1) Then
         $Pass_monoshot1=1
	Else
		 $Pass_monoshot1=0
	EndIf
	ConsoleWrite("Pass_monoshot1!!" & $Pass_monoshot1 & @CRLF)
	sleep(2000)
	$handle=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)					                ; Get handle of the 'Clear message window' toolbar
	_GUICtrlToolbar_ClickIndex($handle,0)											                ; Click on 'Clear message window' icon
	sleep(1000)
	_DeleteMsgBlock()

	_ConfigCANTXBlockDetails("Monoshot","Yes",2000,"Yes","c","All","No","")							; Configure TX block details
	Sleep(1000)
	_AddMsg2TxList(0)																			     ; Add the first msg to Tx list
	_AddMsg2TxList(1)
	Sleep(1000)
	_ConfigCANTXBlockDetails("Monoshot","Yes",2000,"Yes","d","All","No","")							; Configure TX block details
	Sleep(1000)
	_AddMsg2TxList(2)
	_AddMsg2TxList(3)																			    ; Add the first msg to Tx list
	Sleep(1000)
	_CloseTxWindow()
	sleep(1000)
	_ConnectDisconnect()																		    ; Connect the tool

	_TransmitMsgsMenu()																			    ; Transmit normal blocks
	Sleep(5000)

	send("{c}")                                                                                     ;Press key 'c'

	sleep(2000)
	send("{d}")                                                                                     ;Press key 'd'
	sleep(4000)
    _ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount1=_GetCANMsgWinItemCount()
	If $rCount1>=15 Then
		$MonoData1_all=_GetMsgWinCANInfo(0)                                                             ;Fetch messages from message window
	    $MonoData2_all=_GetMsgWinCANInfo(1)
	    $MonoData3_all=_GetMsgWinCANInfo(2)
	    $MonoData4_all=_GetMsgWinCANInfo(3)
	    $MonoData5_all=_GetMsgWinCANInfo(4)
	    $MonoData6_all=_GetMsgWinCANInfo(5)
	    $MonoData7_all=_GetMsgWinCANInfo(6)
	    $MonoData8_all=_GetMsgWinCANInfo(7)
	    $MonoData9_all=_GetMsgWinCANInfo(8)
	    $MonoData10_all=_GetMsgWinCANInfo(9)
	    $MonoData11_all=_GetMsgWinCANInfo(10)
	    $MonoData12_all=_GetMsgWinCANInfo(11)
	    $MonoData13_all=_GetMsgWinCANInfo(12)
	    $MonoData14_all=_GetMsgWinCANInfo(13)
	    $MonoData15_all=_GetMsgWinCANInfo(14)
	    $MonoData16_all=_GetMsgWinCANInfo(15)

	    For $i=0 to 7                                                                                  ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("MonoData1 :" &$MonoData1_all[$i] & @CRLF)
		    ConsoleWrite("MonoData2 :" &$MonoData2_all[$i] & @CRLF)
		    ConsoleWrite("MonoData3 :" &$MonoData3_all[$i] & @CRLF)
		    ConsoleWrite("MonoData4 :" &$MonoData4_all[$i] & @CRLF)
		    ConsoleWrite("MonoData5 :" &$MonoData5_all[$i] & @CRLF)
		    ConsoleWrite("MonoData6 :" &$MonoData6_all[$i] & @CRLF)
		    ConsoleWrite("MonoData7 :" &$MonoData7_all[$i] & @CRLF)
		    ConsoleWrite("MonoData8 :" &$MonoData8_all[$i] & @CRLF)
		    ConsoleWrite("MonoData9 :" &$MonoData9_all[$i] & @CRLF)
		    ConsoleWrite("MonoData10 :" &$MonoData10_all[$i] & @CRLF)
		next
        $FirstMsgTime1_all=StringSplit($MonoData1_all[0],":")                                          ;Split time as hours minutes and seconds
	    $FifthMsgTime1_all=StringSplit($MonoData5_all[0],":")
	    If ( $FifthMsgTime1_all[3]>$FirstMsgTime1_all[3]) Then                                         ;Compare first message time and fifth message time in seconds to get time difference or delay
			$timeDiff2= $FifthMsgTime1_all[3]-$FirstMsgTime1_all[3]
		ElseIf( $FifthMsgTime1_all[3]<$FirstMsgTime1_all[3]) Then
		    If ( $FifthMsgTime1_all[3]=00 And $FirstMsgTime1_all[3]=58) Then
				$timeDiff2=2
			ElseIf( $FifthMsgTime1_all[3]=01 And $FirstMsgTime1_all[3]=59) Then
			    $timeDiff2=2
		    EndIf
	    EndIf
		ConsoleWrite("$timeDiff2 " & $timeDiff2 & @CRLF)
	    If $timeDiff2=2 Then                                                                            ;Verify for time difference or delay
			$time_match1=1
		Else
		    $time_match1=0
	    EndIf
	    If ($MonoData1_all[4]=0x025 Or 0x015) Then                                                       ;Verify for message ID
			$Id_match1=1
	    Else
		    $Id_match1=0
		EndIf
	    If ($MonoData1_all[2]=1 And $MonoData3_all[2]=1 And $MonoData5_all[2]=1) Then                     ;Verify for Channel
			$Channel_Mono1=1
	    Else
	        $Channel_Mono1=0
		EndIf
	EndIf

	ConsoleWrite("$time_match1 :" & $time_match1 & @CRLF)
	ConsoleWrite("$Id_match1 :" & $Id_match1 & @CRLF)
	ConsoleWrite("$Channel_Mono1 :" & $Channel_Mono1 & @CRLF)
	If ($time_match1=1 And $Id_match1=1 And $Channel_Mono1=1)  Then
		$Pass_monoshot2=1
	Else
		$Pass_monoshot2=0
	EndIf
	ConsoleWrite("Pass_monoshot2!!" & $Pass_monoshot2 & @CRLF)
	_DeleteMsgBlock()

	_CloseTxWindow()

	If($Pass_cyclic1=1 And $Pass_cyclic2=1 And $Pass_monoshot1=1 And $Pass_monoshot2=1) Then
         _WriteResult("Pass","TS_Tx_03")
	Else
		_WriteResult("Fail","TS_Tx_03")
	EndIf
EndIf

$isAppNotRes=_CloseApp()																			   ; Close the app

if $isAppNotRes=1 Then
	   _WriteResult("Warning","TS_Tx_03")
   EndIf

ConsoleWrite("****End : TS_TxWin_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)