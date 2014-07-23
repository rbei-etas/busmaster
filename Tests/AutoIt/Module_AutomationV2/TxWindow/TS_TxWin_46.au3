;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_42
; TestCases			:		Send single Message with Changing channel
; Test Data			:		TS_TxWin_01.cfx
; Test Setup		:		1. Select a message from message list.
;                           2. Change the channel number of the selected message entry.
;                           3. Select send button to transmit the selected message to selected channel.
;                           4. Check the message window for transmitted message entry and check the channel number.

; Expected Results  : 		. After step 4, message window will show the transmitted message and the channel ID will exactly match with the
;                             user selected ID
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_42.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_01.cfx")
		Sleep(1000)
		_TxMsgMenu()
		sleep(1000)
		_DeleteMsgBlock()
		Sleep(1000)
		_ConfigCANTXBlockDetails("Cyclic","Yes",1000,"No","","","No","")						         ; Configure TX block details
	    _AddMsg2TxList(0)                                                                               ; Add the first msg to Tx list
		_AddMsg2TxList(1)
         sleep(1000)
		 _ConnectDisconnect()																		                      ; Connect the tool
	    _TransmitMsgsMenu()
	     sleep(2000)
		 $handle1=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)                                                    ; Get the handle of TX message list
	    _GUICtrlListView_SetItemChecked($handle1,1,False)                                                                  ; Uncheck first message
	    Sleep(1000)
	   _CloseTxWindow()
	    sleep(500)
	    _ConnectDisconnect()
	   _DisableOverwriteMode()
	    $rCount1=_GetCANMsgWinItemCount()
	 If $rCount1>=8 Then
		$Data1=_GetMsgWinCANInfo(0)                                                                                    ;Fetch messages from message window
	    $Data2=_GetMsgWinCANInfo(1)
	    $Data3=_GetMsgWinCANInfo(2)
	    $Data4=_GetMsgWinCANInfo(3)
	    $Data5=_GetMsgWinCANInfo(4)
	    $Data6=_GetMsgWinCANInfo(5)
	    $Data7=_GetMsgWinCANInfo(6)
	    $Data8=_GetMsgWinCANInfo(7)
	    $Data9=_GetMsgWinCANInfo(8)

	    For $i=0 to 7                                                                                                     ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("Data1 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data2 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data3 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data4 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data5 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data6 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data7 :" &$Data1[$i] & @CRLF)
		next
		 If ($Data1[4]=0x015) And ($Data3[4]=0x020) And ($Data5[4]=0x015) And ($Data7[4]=0x015)  Then                          ;Verify for message ID
			$Id_match=1
		Else
		    $Id_match=0
	    EndIf
	EndIf
	If($Id_match=1) Then
		_WriteResult("Pass","TS_Tx_046")
	Else
		_WriteResult("Fail","TS_Tx_046")

	EndIf
EndIf
$isAppNotRes=_CloseApp()														; Close the app
If $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_046")
EndIf

ConsoleWrite("****End : TS_TxWin_46.au3****"&@CRLF)
ConsoleWrite(@CRLF)








