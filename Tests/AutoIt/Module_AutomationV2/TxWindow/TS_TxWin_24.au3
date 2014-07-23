;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_24
; TestCases			:		Hex/Dec value display in the Tx window
; Test Data			:		TS_TxWin_01.cfx
; Test Setup		:		1. Ensure there is one message block  defined and it is added with some messages.
;                           2.Click on Hex\Dec Icon on the toolbar.
;                           3. check the value displayed in the Tx window.

; Expected Results  : 		As the Tool bar button is toggled between Hex\dec the values in the Tx window will changed accordingly
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_22.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_01.cfx")
		Sleep(1000)
		_TxMsgMenu()
		sleep(1000)
		_DeleteMsgBlock()
		Sleep(1000)
		_ConfigCANTXBlockDetails("Cyclic","Yes",1000,"No","","","No","")						      ; Configure TX block details
	    _AddMsg2TxList(0)																			      ; Add the first msg to Tx list
	    _AddMsg2TxList(1)
		_CloseTxWindow()																			    ; Close Tx window
	sleep(1000)
	_ConnectDisconnect()																		    ; Connect the tool
	_TransmitMsgsMenu()																			    ; Transmit normal blocks
	Sleep(5000)
	_ConnectDisconnect()																		    ; Disconnect the tool
	Sleep(1000)
	_DisableOverwriteMode()
	 $rCount=_GetCANMsgWinItemCount()                                                               ;Fetch the no of entries in message window
 	If $rCount>=4 Then
		$Data1=_GetMsgWinCANInfo(0)                                                                      ;Fetch messages from message window
	    $Data2=_GetMsgWinCANInfo(1)
	    $Data3=_GetMsgWinCANInfo(2)
	    $Data4=_GetMsgWinCANInfo(3)



		For $i=0 to 7                                                                                    ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("Data1 :" &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data2 :" &$Data2[$i] & @CRLF)
		    ConsoleWrite("Data3 :" &$Data3[$i] & @CRLF)
		    ConsoleWrite("Data4 :" &$Data4[$i] & @CRLF)
		next
		 If ($Data1[4]=0x015)  Then                                                                 ;Verify for message ID
			$Id_cyclic=1
	    Else
		    $Id_cyclic=0
	    EndIf

		ConsoleWrite("Id_cyclic" & $Id_cyclic & @CRLF)
		_DisableHex()
		$Data_aft1=_GetMsgWinCANInfo(0)                                                                      ;Fetch messages from message window
	    $Data_aft2=_GetMsgWinCANInfo(1)
	    $Data_aft3=_GetMsgWinCANInfo(2)
	    $Data_aft4=_GetMsgWinCANInfo(3)
		For $i=0 to 7                                                                                    ;Write messages with all information like(time,Channel,ID) to console
			ConsoleWrite("Data_aft1 :" &$Data_aft1[$i] & @CRLF)
		    ConsoleWrite("Data_aft2 :" &$Data_aft2[$i] & @CRLF)
		    ConsoleWrite("Data_aft3 :" &$Data_aft3[$i] & @CRLF)
		    ConsoleWrite("Data_aft4 :" &$Data_aft4[$i] & @CRLF)
		Next
	EndIf

		 If ($Data_aft1[4]=0021)  Then                                                                 ;Verify for message ID
			$Id_cyclicAft=1
	    Else
		    $Id_cyclicAft=0
	    EndIf
		ConsoleWrite("Id_cyclicAft" & $Id_cyclicAft & @CRLF)
		If($Id_cyclic=1 And $Id_cyclicAft=1) Then
			_WriteResult("Pass","TS_Tx_24")
	    Else
		    _WriteResult("Fail","TS_Tx_24")
	    EndIf

EndIf
	$isAppNotRes=_CloseApp()														; Close the app
if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_24")
EndIf

ConsoleWrite("****End : TS_TxWin_24.au3****"&@CRLF)
ConsoleWrite(@CRLF)







