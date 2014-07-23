;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_Tx_006
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_013.au3****"&@CRLF)
  _launchApp()
$timeDiff2=0
$timeDiff1=0
WinActivate($WIN_BUSMASTER)
Local $Dir=0,$Dir2=0,$time_match=0,$Id_match=0,$Channel=0,$Id_match2=0,$Channel2=0
if winexists($WIN_BUSMASTER) then
    _loadConfig("TS_TxWin_01.cfx")
	sleep(1000)
	_TxMsgMenu()																				    ; Select CAN->Transmit->Configure menu
     sleep(1000)
 	_EnableAutoUpdate()                                                                             ;Enable Auto update
 	sleep(1000)
	_DeleteMsgBlock()                                                                               ;Delete message blocks if there are any
	sleep(1000)
	_ConfigCANTXBlockDetails("Cyclic","Yes",1000,"No","","","No","")
	_AddMsg2TxList(0)
	_AddMsg2TxList(1)
	_AddMsg2TxList(2)
	_AddMsg2TxList(3)
	Sleep(500)
	_CloseTxWindow()
	sleep(500)
	_ConnectDisconnect()																		                      ; Connect the tool
	_TransmitMsgsMenu()
	 sleep(5000)
	 _ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount=_GetCANMsgWinItemCount()                                                                               ;Fetch the no of items from message window
	If $rCount>=9 Then
		$Data1=_GetMsgWinCANInfo(0)                                                                                    ;Fetch messages from message window
	    $Data2=_GetMsgWinCANInfo(1)
	    $Data3=_GetMsgWinCANInfo(2)
	    $Data4=_GetMsgWinCANInfo(3)
	    $Data5=_GetMsgWinCANInfo(4)
	    $Data6=_GetMsgWinCANInfo(5)
	    $Data7=_GetMsgWinCANInfo(6)
	    $Data8=_GetMsgWinCANInfo(7)
	    $Data9=_GetMsgWinCANInfo(8)
	    $Data10=_GetMsgWinCANInfo(9)
	    For $i=0 to 7                                                                                                     ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("Data1 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data2 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data3 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data4 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data5 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data6 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data7 :" &$Data1[$i] & @CRLF)
		next
	    If ($Data1[1]="Tx" And $Data3[1]="Tx" And $Data5[1]="Tx" And $Data7[1]="Tx") Then                                         ;Verify for Tx/RX
		    $Dir=1
		 Else
		    $Dir=0
		EndIf
	    If ($Data1[2]=1 And $Data3[2]=1 And $Data5[2]=1 And $Data7[2]=1) Then                                                  ;Verify for Channel
			$Channel=1
	    Else
	        $Channel=0
	    EndIf
	    If ($Data1[4]=0x015) And ($Data3[4]=0x20) And ($Data5[4]=0x025) And ($Data7[4]=0x066)  Then                          ;Verify for message ID
			$Id_match=1
	    Else
		    $Id_match=0
	    EndIf
	EndIf
	ConsoleWrite("Direction " & $Dir & @CRLF)
	ConsoleWrite("Channel" & $Channel& @CRLF)
	ConsoleWrite("Id" & $Id_match & @CRLF)

	If($Dir=1 And $Channel=1 And $Id_match=1) Then
		$Pass_All=1
	Else
		$Pass_All=0
	EndIf
	 $handle=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_OverWrite)					                                    ; Get handle of the 'Clear message window'  toolbar
	_GUICtrlToolbar_ClickIndex($handle,0)											                                    ; Click on 'Clear message window'  toolbar
	sleep(1000)
	_TxMsgMenu()
	 $handle1=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)                                                    ; Get the handle of TX message list
	_GUICtrlListView_SetItemChecked($handle1,0,False)                                                                  ; Uncheck first message
	_GUICtrlListView_SetItemChecked($handle1,1,False)                                                                   ;Uncheck second message
	_CloseTxWindow()
	sleep(500)
	_ConnectDisconnect()																		                      ; Connect the tool
	_TransmitMsgsMenu()
	 sleep(5000)
	 _ConnectDisconnect()
	_DisableOverwriteMode()
	$rCount1=_GetCANMsgWinItemCount()
	If $rCount1>=9 Then
		$Data1=_GetMsgWinCANInfo(0)                                                                                    ;Fetch messages from message window
	    $Data2=_GetMsgWinCANInfo(1)
	    $Data3=_GetMsgWinCANInfo(2)
	    $Data4=_GetMsgWinCANInfo(3)
	    $Data5=_GetMsgWinCANInfo(4)
	    $Data6=_GetMsgWinCANInfo(5)
	    $Data7=_GetMsgWinCANInfo(6)
	    $Data8=_GetMsgWinCANInfo(7)
	    $Data9=_GetMsgWinCANInfo(8)
	    $Data10=_GetMsgWinCANInfo(9)
	    For $i=0 to 7                                                                                                     ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("Data1 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data2 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data3 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data4 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data5 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data6 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data7 :" &$Data1[$i] & @CRLF)
		next

        If ($Data1[1]="Tx" And $Data3[1]="Tx" And $Data5[1]="Tx" And $Data7[1]="Tx") Then                                         ;Verify for Tx/RX
		    $Dir2=1
		 Else
		    $Dir2=0
	    EndIf
	    If ($Data1[2]=1 And $Data3[2]=1 And $Data5[2]=1 And $Data7[2]=1) Then                                                  ;Verify for Channel
			$Channel2=1
	    Else
	        $Channel2=0
	    EndIf

	    If ($Data1[4]=0x025) And ($Data3[4]=0x066) And ($Data5[4]=0x025) And ($Data7[4]=0x066)  Then                          ;Verify for message ID
			$Id_match2=1
		Else
		    $Id_match2=0
	    EndIf

	    If($Dir2=1 And $Channel2=1 And $Id_match2=1) Then
			$Pass_Few=1
	    Else
		    $Pass_Few=0
	    EndIf
	EndIf

	ConsoleWrite("Direction " & $Dir2 & @CRLF)
	ConsoleWrite("Channel" & $Channel2 & @CRLF)
	ConsoleWrite("Id" & $Id_match2 & @CRLF)

	If($Pass_All=1 And $Pass_Few=1) Then
		_WriteResult("Pass","TS_Tx_06")
	Else
		_WriteResult("Fail","TS_Tx_06")

	EndIf
EndIf
$isAppNotRes=_CloseApp()														; Close the app
If $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_06")
EndIf

ConsoleWrite("****End : TS_TxWin_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)








