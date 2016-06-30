;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_18
; TestCases			:		Change DataByte Value and transmit [Both Dtabase and Non-Database messages]
; Test Data			:
; Test Setup		:

;~ 							1. Select a message from message list.
;							2. Note the DataBbyte for the selected message   displayed in the DataByte List View [ below Tx message List View ] .
;							3. change the data Byte  displayed in  in Message details view.
;							4. Connect  Busmaster and start sending messages.


; 							Expected Results  :1. The updated data byte for the message will show in message Window.

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_12.au3****"&@CRLF)

Local $VerifyupdateddataByte=0,$MsgTXdatbyte=0,$MsgRXdatbyte=0
Local $DtaByteValue[8]=["248","249","250","251","252","253","254","255"],$j=0
Local $DtaByteValue1[8]=["1","2","3","4","5","6","7","8"],$j=0


    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_CAN_01.cfx")
		Sleep(1000)
		_TxMsgMenu()																						; open Tx Window -CAN
		sleep(1000)
		_DisableHex()																						; Disable Hex
		Sleep(2000)
		_ConfigTXWinDetails(0,"db",1,1,"","","","1000","")													; Config Tx Window.E.g-$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
		sleep(1000)
		;_DisableHex()
		$HWD=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:3]")
		sleep(1000)
		$ItemPos=_GetRowPosTxWin($HWD,0)																	; get Item Position

		sleep(500)
		for $i=100 to 800
			;$ColWidth=_GetColWidthTxWin($HWD,$i)

			ControlClick($WIN_BUSMASTER,"",$HWD,"Left",2,$ItemPos[0]+$i,$ItemPos[1])                    	; Double Click on databyte
			sleep(1000)
			if $j<=7 Then
				Send($DtaByteValue[$j])																		; Write databyte Value in Text Box.
				Send("{ENTER}")
				sleep(1000)
				$j=$j+1
			ConsoleWrite("$i="&$i&@CRLF)
			EndIf
			$i=$i+90
		Next

		_ConfigTXWinDetails(1,"ndb","100",1,"","","","1000","")													; Config Tx Window.E.g-$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
		$HWD=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:3]")
		$ItemPos=_GetRowPosTxWin($HWD,0)
		;$ItemPos2=_GetRowPosTxWin($HWD,1)																	; get Item Position
		$j=0
		sleep(500)
		for $i=100 to 800
			;$ColWidth=_GetColWidthTxWin($HWD,$i)

			ControlClick($WIN_BUSMASTER,"",$HWD,"Left",2,$ItemPos[0]+$i,$ItemPos[1])                    	; Double Click on databyte
			sleep(1000)
			if $j<=7 Then
				Send($DtaByteValue1[$j])																		; Write databyte Value in Text Box.
				Send("{ENTER}")
				sleep(1000)
				$j=$j+1
			ConsoleWrite("$i="&$i&@CRLF)
			EndIf
			$i=$i+90
		Next


		_EnableOverwriteMode()																				; Enable overwrite Mode
		_ConnectDisconnect()																				; Connect Tool
		sleep(1000)
		_ConnectDisconnect()																				; Disconnect Tool
		_CloseTxWindowArg("CAN")																			; Close Tx Window
		$rCount=_GetCANMsgWinItemCount()                                                           			; Fetch the no of items from message window
		ConsoleWrite("$rCount="&$rCount&@CRLF)

		if $rCount=4 Then
																											; Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo(0)
			$Data1=_GetMsgWinCANInfo(1)
			$Data2=_GetMsgWinCANInfo(2)
			$Data3=_GetMsgWinCANInfo(3)
			for $j=0 to 1
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
				ConsoleWrite("$Data1"&$i&"="&$Data1[$j]&@CRLF)
			Next
			If $Data[7]="248249250251252253254255" and  $Data1[7]="248249250251252253254255" and $Data2[7]="001002003004005006007008" and $Data3[7]="001002003004005006007008"Then                                      				; Verify updated databytes in Message Window
					$VerifyupdateddataByte=1
			EndIf

		EndIf
	;ConsoleWrite("$MsgTXdatbyte"&$MsgTXdatbyte)
	;ConsoleWrite("$MsgRXdatbyte"&$MsgRXdatbyte)
	ConsoleWrite("$VerifyupdateddataByte"&$VerifyupdateddataByte)

	EndIf

If $VerifyupdateddataByte=1 Then   																			; Check data byte value for second message to cross verify for the raw value which got updated when																									;when physical value is changed
		_WriteResult("Pass","TS_TxWin_CAN_18")
Else
		_WriteResult("Fail","TS_TxWin_CAN_18")
EndIf

$isAppNotRes=_CloseApp()														                         	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_18")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_18.au3****"&@CRLF)
ConsoleWrite(@CRLF)