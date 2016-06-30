;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_19
; TestCases			:		Change DataByte Value during transmission [Both Dtabase and Non-Database messages]
; Test Data			:
; Test Setup		:

;~ 							1. Select a message from message list and Connect Busmaster .
;							2. Change Data Byte value for the selected message during trasnmission. .



; 							Expected Results  :1. The updated data byte for the message will show in message Window.

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_19.au3****"&@CRLF)

Local $VerifyMsgDataByteBFRCHNGE=0,$VerifyMsgDataByteAFTRCHNGE=0
Local $DtaByteValue[8]=["8","7","6","5","4","3","2","1"],$j=0,$k=0
Local $DtaByteValue1[8]=["1","2","3","4","5","6","7","8"]


    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_CAN_19.cfx")
		Sleep(500)
		_ConnectDisconnect()																				; Connect Tool
		sleep(1000)
		;_ConnectDisconnect()																				; Connect Tool
		$rCount=_GetCANMsgWinItemCount()                                                           			; Fetch the no of items from message window
		ConsoleWrite("$rCount="&$rCount&@CRLF)

		if $rCount=4 Then
																											; Write messages with all information like(time,Channel,ID) to console
			$Data=_GetMsgWinCANInfo(0)
			$Data1=_GetMsgWinCANInfo(1)
			$Data2=_GetMsgWinCANInfo(2)
			$Data3=_GetMsgWinCANInfo(3)
			for $j=0 to 7
				ConsoleWrite("$Data="&$Data[$j]&@CRLF)
				ConsoleWrite("$Data1="&$Data1[$j]&@CRLF)
				ConsoleWrite("$Data2="&$Data1[$j]&@CRLF)
				ConsoleWrite("$Data3="&$Data1[$j]&@CRLF)
			Next
			If $Data[7]="0102030405060708" and  $Data1[7]="0102030405060708" and $Data2[7]="0807060504030201" and $Data3[7]="0807060504030201" Then                                      				; Verify updated databytes in Message Window
					$VerifyMsgDataByteBFRCHNGE=1
			EndIf

		EndIf

		_TxMsgMenu()																						; open Tx Window -CAN
		sleep(1000)
		;_DisableHex()																						; Disable Hex
		Sleep(1000)
		_ConfigTXWinDetails(0,"","",1,"","","","","")													; Config Tx Window.E.g-$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key

		$HWD=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:3]")
		$ItemPos=_GetRowPosTxWin($HWD,0)																	; get Item Position

		sleep(1000)
		for $i=100 to 800
			;$ColWidth=_GetColWidthTxWin($HWD,$i)

			ControlClick($WIN_BUSMASTER,"",$HWD,"Left",2,$ItemPos[0]+$i,$ItemPos[1])                    	; Double Click on databyte
			sleep(1000)
			if $k<=7 Then
				Send($DtaByteValue[$k])																		; Write databyte Value in Text Box.
				Send("{ENTER}")
				sleep(1500)
				$k=$k+1
				ConsoleWrite("$i="&$i&@CRLF)
			EndIf
			$i=$i+90
		Next

		_ConfigTXWinDetails(1,"","",1,"","","","","")													; Config Tx Window.E.g-$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
		$HWD=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:3]")
		$ItemPos=_GetRowPosTxWin($HWD,0)
		;$ItemPos2=_GetRowPosTxWin($HWD,1)																	; get Item Position
		$j=0
		sleep(1000)
		for $i=100 to 800
			;$ColWidth=_GetColWidthTxWin($HWD,$i)

			ControlClick($WIN_BUSMASTER,"",$HWD,"Left",2,$ItemPos[0]+$i,$ItemPos[1])                    	; Double Click on databyte
			sleep(1000)
			if $j<=7 Then
				Send($DtaByteValue1[$j])																		; Write databyte Value in Text Box.
				Send("{ENTER}")
				sleep(1500)
				$j=$j+1
			ConsoleWrite("$i="&$i&@CRLF)
			EndIf
			$i=$i+90
		Next


		;_EnableOverwriteMode()																				; Enable overwrite Mode
		;_ConnectDisconnect()																				; Connect Tool
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
			for $j=0 to 7
				ConsoleWrite("$Data"&$i&"="&$Data[$j]&@CRLF)
				ConsoleWrite("$Data1"&$i&"="&$Data1[$j]&@CRLF)
			Next
			If $Data[7]="0807060504030201" and  $Data1[7]="0807060504030201" and $Data2[7]="0102030405060708" and $Data3[7]="0102030405060708"Then                                      				; Verify updated databytes in Message Window
					$VerifyMsgDataByteAFTRCHNGE=1
			EndIf

	EndIf
	;ConsoleWrite("$MsgTXdatbyte"&$MsgTXdatbyte)
	;ConsoleWrite("$MsgRXdatbyte"&$MsgRXdatbyte)
	ConsoleWrite("$VerifyMsgDataByteBFRCHNGE"&$VerifyMsgDataByteBFRCHNGE&@CRLF)
	ConsoleWrite("$VerifyMsgDataByteAFTRCHNGE"&$VerifyMsgDataByteAFTRCHNGE&@CRLF)

EndIf

If $VerifyMsgDataByteBFRCHNGE And $VerifyMsgDataByteAFTRCHNGE=1 Then   																			; Check data byte value for second message to cross verify for the raw value which got updated when																									;when physical value is changed
		_WriteResult("Pass","TS_TxWin_CAN_19")
Else
		_WriteResult("Fail","TS_TxWin_CAN_19")
EndIf

$isAppNotRes=_CloseApp()														                         	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_19")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_19.au3****"&@CRLF)
ConsoleWrite(@CRLF)