;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_10
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
_launchApp()
$timeDiff=0
$timeDiff1=0
WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$Channel_cyclic=0,$Dir_cyclic=0,$Data_cyclic=0,$Id_cyclic=0
if winexists($WIN_BUSMASTER) then
	$timeDiff=0                                                                                       ;Initialize time difference values to '0'
	$timeDiff_all=0
	$timeDiff1=0
	$timeDiff2=0
   _loadConfig("TS_TxWin_01.cfx")
    Sleep(1000)
	_TxMsgMenu()																				    ; Select CAN->Transmit->Configure menu
	sleep(1000)
 	_EnableAutoUpdate()
 	sleep(1000)
	_DeleteMsgBlock()
	sleep(1000)
    _ConfigCANTXBlockDetails("Cyclic","No","","No","","","Yes",2000)							    ; Configure TX block details
	_AddMsg2TxList(0)																			    ; Add the first msg to Tx list
	_AddMsg2TxList(1)
    _ConfigCANTXBlockDetails("Cyclic","No","","No","","","Yes",2000)							   ; Configure TX block details
	_AddMsg2TxList(2)																			   ; Add the first msg to Tx list
	_AddMsg2TxList(3)
	_CloseTxWindow()
	sleep(1000)
	_ConnectDisconnect()																		   ; Connect the tool
	_TransmitMsgsMenu()																			   ; Transmit normal blocks
	Sleep(5000)
	_ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount=_GetCANMsgWinItemCount()                                                                ;Fetch no of items from message window
	If $rCount>=8 Then
		$Data1=_GetMsgWinCANInfo(0)                                                                    ;Fetch messages from message window
     	$Data2=_GetMsgWinCANInfo(1)
    	$Data3=_GetMsgWinCANInfo(2)
    	$Data4=_GetMsgWinCANInfo(3)
    	$Data5=_GetMsgWinCANInfo(4)
    	$Data6=_GetMsgWinCANInfo(5)
    	$Data7=_GetMsgWinCANInfo(6)
    	$Data8=_GetMsgWinCANInfo(7)
    	$Data9=_GetMsgWinCANInfo(8)

	    For $i=0 to 7                                                                                  ;Write messages with all information like(time,Channel,ID) to console
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
			$Time_cyclic=1
	    Else
		    $Time_cyclic=0
        EndIf

	    If ($Data1[1]="Tx" And $Data5[1]="Tx" And $Data7[1]="Tx") Then                                     ;Verify for Tx/RX
			$Dir_cyclic=1
     	Else
	        $Dir_cyclic=0
    	EndIf

	    If ($Data1[2]=1 And $Data3[2]=1 And $Data5[2]=1) Then                                              ;Verify for Channel
			$Channel_cyclic=1
	    Else
		    $Channel_cyclic=0
	    EndIf
	    If ($Data1[7]="0000000000000000" And $Data3[7]="00000000000000") Then                              ; Verify for Data bytes
			$Data_cyclic=1
    	Else
		    $Data_cyclic=0
		EndIf

		If ($Data1[4]= 0x015) And ($Data3[4]=0x020) And ($Data5[4]=0x025) And ($Data7[4]=0x066)  Then          ;Verify for message ID
			$Id_cyclic=1
    	Else
		    $Id_cyclic=0
	    EndIf
	EndIf
		ConsoleWrite("$Data_cyclic " & $Data_cyclic & @CRLF)
	    ConsoleWrite("$Channel_cyclic" & $Channel_cyclic & @CRLF)
	    ConsoleWrite("$Dir_cyclic" & $Dir_cyclic & @CRLF)
    	ConsoleWrite("$Time_cyclic" & $Time_cyclic & @CRLF)
		ConsoleWrite("$Id_cyclic" & $Id_cyclic & @CRLF)
	If $Time_cyclic=1 And $Id_cyclic=1  And $Dir_cyclic=1 And $Channel_cyclic=1 And $Data_cyclic=1 Then
		$Pass_cyclic1=1
	Else
		$Pass_cyclic1=0
    EndIf
	ConsoleWrite("Pass_cyclic1!!" & $Pass_cyclic1 & @CRLF)
	sleep(3000)
	$handle=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)					                        ; Get handle of the 'Connect/Disconnect' toolbar
	_GUICtrlToolbar_ClickIndex($handle,0)											                        ; Click on 'Connect' icon
	 sleep(1000)
	If($Pass_cyclic1=1) Then
		_WriteResult("Pass","TS_Tx_10")
	Else
		_WriteResult("Fail","TS_Tx_10")
	EndIf
EndIf
$isAppNotRes=_CloseApp()																			              ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_10")
EndIf

ConsoleWrite("****End : TS_TxWin_10.au3****"&@CRLF)
ConsoleWrite(@CRLF)
