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
	    _AddMsg2TxList(0)																			     ; Add the first msg to Tx list
         sleep(1000)
		_CloseTxWindow()																			    ; Close Tx window
		sleep(1000)
	   _ConnectDisconnect()																		    ; Connect the tool
	   _TransmitMsgsMenu()																			    ; Transmit normal blocks
	    Sleep(2000)
	    _TxMsgMenu()																    ; Disconnect the tool
	    Sleep(1000)
	    $handle=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)
		_GUICtrlListView_ClickItem($handle,0)
		Sleep(1000)
		$handle1=ControlGetHandle($WIN_BUSMASTER,"", $COMB_ChannelID_ConfigTX)                           ;Get the handle of the channel id combobox
		_GUICtrlComboBoxEx_ShowDropDown($handle1,True)
		Sleep(1000)
		ControlClick($WIN_BUSMASTER,"",$COMB_ChannelID_ConfigTX)                                         ;Select Second channel
		 Sleep(1000)
		send("{DOWN}")
		Sleep(1000)
		send("{ENTER}")
		Sleep(500)
		_CloseTxWindow()
		 _ConnectDisconnect()
	   _DisableOverwriteMode()
		 $rCount=_GetCANMsgWinItemCount()
	    	;If $rCount>=4 Then
		$Data1=_GetMsgWinCANInfo(0)                                                                      ;Fetch messages from message window
	    $Data2=_GetMsgWinCANInfo(1)
	    $Data3=_GetMsgWinCANInfo(2)
	    $Data4=_GetMsgWinCANInfo(3)
		$Data5=_GetMsgWinCANInfo(4)
		$Data6=_GetMsgWinCANInfo(5)
		$Data7=_GetMsgWinCANInfo(6)
		$Data8=_GetMsgWinCANInfo(7)
		$Data9=_GetMsgWinCANInfo(8)
		$Data10=_GetMsgWinCANInfo(9)
		$Data11=_GetMsgWinCANInfo(10)
		$Data12=_GetMsgWinCANInfo(11)
		$Data13=_GetMsgWinCANInfo(12)
		$Data14=_GetMsgWinCANInfo(13)



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
			ConsoleWrite("Data10 :" &$Data10[$i] & @CRLF)
			ConsoleWrite("Data11 :" &$Data11[$i] & @CRLF)
			ConsoleWrite("Data12 :" &$Data12[$i] & @CRLF)
			ConsoleWrite("Data13 :" &$Data13[$i] & @CRLF)
			ConsoleWrite("Data14 :" &$Data14[$i] & @CRLF)

		next
		If($Data1[2]=1 And $Data14[2]=2) Then
			 _WriteResult("Pass","TS_Tx_042")
		Else
		   _WriteResult("Fail","TS_Tx_042")
	    EndIf

EndIf
$isAppNotRes=_CloseApp()														                         ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_042")
EndIf

ConsoleWrite("****End : TS_TxWin_42.au3****"&@CRLF)
ConsoleWrite(@CRLF)