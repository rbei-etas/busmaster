;=== Test Cases/Test Data ===
; UseCase 1:		Configure Tx message
; Critical (C):		Y
; TestCase No.:		TS_TxWin_14
; Test Cases:		Tx messages
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===
 ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_14.au3****"&@CRLF)
  _launchApp()

WinActivate($WIN_BUSMASTER)
Local $Data_before_prev=0,$Data_before=0,$Data_after=0
if winexists($WIN_BUSMASTER) then
    _loadConfig("TS_TxWin_01.cfx")
	sleep(1000)
 	_TxMsgMenu()																				    ; Select CAN->Transmit->Configure menu
     sleep(1000)
 	_EnableAutoUpdate()                                                                             ;Enable Auto update
 	sleep(1000)
	_DeleteMsgBlock()                                                                               ;Delete message blocks if there are any
	sleep(1000)
	_ConfigCANTXBlockDetails("Cyclic","No","","Yes","a","All","No","")

	_AddMsg2TxList(0)
	_AddMsg2TxList(1)
_ConfigCANTXBlockDetails("Cyclic","Yes",1000,"No","","","No","")

    _AddMsg2TxList(2)
	_AddMsg2TxList(3)
	Sleep(500)
 	_CloseTxWindow()
 	_ConnectDisconnect()																		                      ; Connect the tool
	_TransmitMsgsMenu()
	 sleep(4000)
	 send("{a}")                                                                                     ;Press key 'c'

	sleep(500)
	  _ConnectDisconnect()
	;_DisableOverwriteMode()
	_EnableOverwriteMode()
	$rCount=_GetCANMsgWinItemCount()                                                             ;Fetch no of items from message window
	If $rCount>=7 Then
    	$Data1_all=_GetMsgWinCANInfo(0)                                                             ;Fetch messages from message window
    	$Data2_all=_GetMsgWinCANInfo(1)
    	$Data3_all=_GetMsgWinCANInfo(2)
    	$Data4_all=_GetMsgWinCANInfo(3)
    	$Data5_all=_GetMsgWinCANInfo(4)
    	$Data6_all=_GetMsgWinCANInfo(5)
    	$Data7_all=_GetMsgWinCANInfo(6)
    	$Data8_all=_GetMsgWinCANInfo(7)
;~     	$Data9_all=_GetMsgWinCANInfo(8)
;~     	$Data10_all=_GetMsgWinCANInfo(9)
;~     	$Data11_all=_GetMsgWinCANInfo(10)
;~       	$Data12_all=_GetMsgWinCANInfo(11)

    	For $i=0 to 7                                                                                  ;Write messages with all information like(time,Channel,ID) to console
    		ConsoleWrite("Data1 :" &$Data1_all[$i] & @CRLF)
     		ConsoleWrite("Data2 :" &$Data2_all[$i] & @CRLF)
    		ConsoleWrite("Data3 :" &$Data3_all[$i] & @CRLF)
    		ConsoleWrite("Data4 :" &$Data4_all[$i] & @CRLF)
	    	ConsoleWrite("Data5 :" &$Data5_all[$i] & @CRLF)
    		ConsoleWrite("Data6 :" &$Data6_all[$i] & @CRLF)
	    	ConsoleWrite("Data7 :" &$Data7_all[$i] & @CRLF)
    		ConsoleWrite("Data8 :" &$Data8_all[$i] & @CRLF)
;~      		ConsoleWrite("Data9 :" &$Data9_all[$i] & @CRLF)
;~     		ConsoleWrite("Data10 :" &$Data10_all[$i] & @CRLF)
        next

        If ($Data1_all[4]=0x015  And $Data5_all[4]=0x025) Then
	    	$Data_before_prev=1
    	Else
	    	$Data_before_prev=0
	    EndIf

        If($Data3_all[4]=0x020 And $Data7_all[4]=0x066) Then
			$Data_before=1
    	 Else
	    	 $Data_before=0
	    EndIf
;~     	If ($Data9_all[4]=0x015 And $Data11_all[4]=0x020) Then                                                       ;Verify for message ID
;~      		$Data_after=1
;~     	Else
;~ 	    	$Data_after=0
;~     	EndIf
	EndIf

	ConsoleWrite("Data before" & $Data_before & @CRLF)
	ConsoleWrite("Data before previous" & $Data_before_prev & @CRLF)
	;ConsoleWrite("Data after" & $Data_after & @CRLF)

	If($Data_before_prev=1 And $Data_before=1) Then
		_WriteResult("Pass","TS_Tx_14")
	Else
		_WriteResult("Fail","TS_Tx_14")
	EndIf
EndIf

$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_14")
EndIf

ConsoleWrite("****End : TS_TxWin_14.au3****"&@CRLF)
ConsoleWrite(@CRLF)



