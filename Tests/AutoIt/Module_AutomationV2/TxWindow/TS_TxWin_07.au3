;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_007
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_07.au3****"&@CRLF)
  _launchApp()
$timeDiff2=0
$timeDiff1=0
WinActivate($WIN_BUSMASTER)
Local $Id_match1=0,$Id_match2=0,$Channel_Mono1=0,$Channel_Mono2=0,$Dir_Mono=0,$Dir_Mono1=0
if winexists($WIN_BUSMASTER) then
	_loadConfig("TS_TxWin_01.cfx")
	sleep(1000)
	_TxMsgMenu()																				    ; Select CAN->Transmit->Configure menu
     sleep(1000)
 	_EnableAutoUpdate()                                                                             ;Enable Auto update
 	sleep(1000)
	_DeleteMsgBlock()                                                                               ;Delete message blocks if there are any
	sleep(1000)
	_ConfigCANTXBlockDetails("Monoshot","Yes",1000,"Yes","a","All","No","")							; Configure TX block details
	Sleep(1000)
	_AddMsg2TxList(0)																			     ; Add the first msg to Tx list
	_AddMsg2TxList(1)
	_AddMsg2TxList(2)
	_AddMsg2TxList(3)
	Sleep(1000)
	_CloseTxWindow()
	sleep(1000)
	_ConnectDisconnect()																		    ; Connect the tool

	_TransmitMsgsMenu()																			    ; Transmit normal blocks
	Sleep(5000)

	send("{a}")                                                                                     ;Press key 'c'

	sleep(4000)
	 _ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount=_GetCANMsgWinItemCount()                                                                 ;Fetch no of items from message window
	If $rCount>=15 Then

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

	    If ($MonoData1_all[4]=0x015 And $MonoData3_all[4]=0x020 And $MonoData5_all[4]=0x025 And $MonoData7_all[4]=0x066 And $MonoData9_all[4]=0x015) Then                                                       ;Verify for message ID
			$Id_match1=1
		Else
		    $Id_match1=0
	    EndIf
		ConsoleWrite("$Id_match1 :" & $Id_match1 & @CRLF)

	    If ($MonoData1_all[2]=1 And $MonoData3_all[2]=1 And $MonoData5_all[2]=1 And $MonoData7_all[2]=1) Then                     ;Verify for Channel
			$Channel_Mono1=1
	    Else
            $Channel_Mono1=1
        EndIf
        If ($MonoData1_all[1]="Tx" And $MonoData3_all[1]="Tx" And $MonoData5_all[1]="Tx" And $MonoData7_all[1]="Tx" And $MonoData9_all[1]="Tx") Then
	    	$Dir_Mono=1
	    Else
		    $Dir_Mono=0
	    EndIf
	EndIf

	If($Id_match1=1 And $Channel_Mono1=1 And $Dir_Mono=1) Then
		$Pass1=1
	Else
		$Pass1=0
	EndIf
	ConsoleWrite("Pass1" & $Pass1 & @CRLF)
	 $handle=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)					                                    ; Get handle of the 'Clear message window'  toolbar
	_GUICtrlToolbar_ClickIndex($handle,0)											                                    ; Click on 'Clear message window'  toolbar
	sleep(1000)
	;_DeleteMsgBlock()
	_TxMsgMenu()
	 sleep(1000)
	 $handle1=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)                                                    ; Get the handle of TX message list
	_GUICtrlListView_SetItemChecked($handle1,0,False)                                                                  ; Uncheck first message
	Sleep(500)
	_GUICtrlListView_SetItemChecked($handle1,1,False)                                                                   ;Uncheck second message
    Sleep(500)
	_CloseTxWindow()
	sleep(500)
	_ConnectDisconnect()																		                      ; Connect the tool
	_TransmitMsgsMenu()
	 sleep(3000)
	 send("{a}")                                                                                     ;Press key 'c'
	sleep(4000)
	 _ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount1=_GetCANMsgWinItemCount()
    If $rCount1>=8 Then
		$MonoData1_all=_GetMsgWinCANInfo(0)                                                             ;Fetch messages from message window
	    $MonoData2_all=_GetMsgWinCANInfo(1)
     	$MonoData3_all=_GetMsgWinCANInfo(2)
    	$MonoData4_all=_GetMsgWinCANInfo(3)
    	$MonoData5_all=_GetMsgWinCANInfo(4)
    	$MonoData6_all=_GetMsgWinCANInfo(5)
    	$MonoData7_all=_GetMsgWinCANInfo(6)
    	$MonoData8_all=_GetMsgWinCANInfo(7)
    	$MonoData9_all=_GetMsgWinCANInfo(8)
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
         next
		If ($MonoData1_all[4]=0x025 And $MonoData3_all[4]=0x066 And $MonoData5_all[4]=0x025 And $MonoData7_all[4]=0x066) Then
			$Id_match2=1
	    Else
		    $Id_match2=0
	    EndIf
	 	ConsoleWrite("$Id_match2 :" & $Id_match2 & @CRLF)
	    If ($MonoData1_all[2]=1 And $MonoData3_all[2]=1 And $MonoData5_all[2]=1 And $MonoData7_all[2]=1) Then                     ;Verify for Channel
			$Channel_Mono2=1
	    Else
            $Channel_Mono2=1
        EndIf
		ConsoleWrite("$Channel_Mono2 :" & $Channel_Mono2 & @CRLF)
        If ($MonoData1_all[1]="Tx" And $MonoData3_all[1]="Tx" And $MonoData5_all[1]="Tx" And $MonoData7_all[1]="Tx" ) Then
			$Dir_Mono1=1
	    Else
		    $Dir_Mono1=0
		EndIf
		ConsoleWrite("$Dir_Mono1 :" & $Dir_Mono1 & @CRLF)
	EndIf

	If($Id_match2=1 And $Channel_Mono2=1 And $Dir_Mono1=1) Then
		$Pass2=1
	Else
		$Pass2=0
	EndIf
	ConsoleWrite("Pass2" & $Pass2 & @CRLF)
	If($Pass1=1 And $Pass2=1) Then
		_WriteResult("Pass","TS_Tx_07")
	Else
		_WriteResult("Fail","TS_Tx_07")
	EndIf
EndIf
$isAppNotRes=_CloseApp()														; Close the app
if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_07")
EndIf

ConsoleWrite("****End : TS_TxWin_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)





