;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_17
; TestCases			:		DataByte View  [GUI]
; Test Data			:
; Test Setup		:

;~ 							1. Select a message from message list.
;							2. Note the DataBbyte for the selected message   displayed in the DataByte List View [ below Tx message List View ] .
;							3. change the data Byte  displayed in  in Message details view.


; 							Expected Results  :1. Message detail view will show the selected message Data Byte.

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_17.au3****"&@CRLF)

Local $VerifydataByte1=0,$VerifydataByte2=0
Local $DtaByteValue[8]=["8","7","6","5","4","3","2","1"]
Local $DtaByteValue1[8]=["1","2","3","4","5","6","7","8"],$j=0


    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_CAN_19.cfx")
		Sleep(1000)
		_TxMsgMenu()																						; open Tx Window -CAN
																											; Disable Hex
		Sleep(1000)
		_ConfigTXWinDetails(0,"","",1,"","","","","")														; Config Tx Window.E.g-$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key

		$HWD=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:3]")
		$ItemPos=_GetRowPosTxWin($HWD,0)																	; get Item Position

		sleep(500)
		for $i=100 to 800
			;$ColWidth=_GetColWidthTxWin($HWD,$i)

			ControlClick($WIN_BUSMASTER,"",$HWD,"Left",2,$ItemPos[0]+$i,$ItemPos[1])                    	; Double Click on databyte
			sleep(500)
			if $j<=7 Then
				Send($DtaByteValue[$j])																		; Write databyte Value in Text Box.
				Send("{ENTER}")
				sleep(500)
				$j=$j+1
			ConsoleWrite("$i="&$i&@CRLF)
			EndIf
			$i=$i+90
		Next

		_ConfigTXWinDetails(1,"","",1,"","","","","")														; Config Tx Window.E.g-$Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
		$HWD=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:3]")
		$ItemPos=_GetRowPosTxWin($HWD,0)
		;$ItemPos2=_GetRowPosTxWin($HWD,1)																	; get Item Position
		$j=0
		sleep(500)
		for $i=100 to 800
			;$ColWidth=_GetColWidthTxWin($HWD,$i)

			ControlClick($WIN_BUSMASTER,"",$HWD,"Left",2,$ItemPos[0]+$i,$ItemPos[1])                    	; Double Click on databyte
			sleep(500)
			if $j<=7 Then
				Send($DtaByteValue1[$j])																	; Write databyte Value in Text Box.
				Send("{ENTER}")
				sleep(500)
				$j=$j+1
			ConsoleWrite("$i="&$i&@CRLF)
			EndIf
			$i=$i+90
		Next
		_ConfigTXWinDetails(0,"","",1,"","","","","")

		$HWD1=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:3]")
		$txtdataByte=_GUICtrlListView_GetItemTextArray($HWD1,0)
		ConsoleWrite("$txtdataByte="&$txtdataByte[1]&@CRLF)											; 	get the databyte
		ConsoleWrite("$txtdataByte="&$txtdataByte[2]&@CRLF)											;  	get the databyte
		ConsoleWrite("$txtdataByte="&$txtdataByte[3]&@CRLF)											; 	get the databyte
		ConsoleWrite("$txtdataByte="&$txtdataByte[4]&@CRLF)											;  	get the databyte
		ConsoleWrite("$txtdataByte="&$txtdataByte[5]&@CRLF)											;  	get the databyte
		ConsoleWrite("$txtdataByte="&$txtdataByte[6]&@CRLF)											;  	get the databyte
		ConsoleWrite("$txtdataByte="&$txtdataByte[7]&@CRLF)											;  	get the databyte
		ConsoleWrite("$txtdataByte="&$txtdataByte[8]&@CRLF)											; 	get the databyte
		ConsoleWrite("$txtdataByte="&$txtdataByte[8]&@CRLF)											;  	get the databyte

		if $txtdataByte[1]=000 And $txtdataByte[2]=08 And $txtdataByte[3]=07 And $txtdataByte[4]=06 And $txtdataByte[5]=05 And $txtdataByte[6]=04 And $txtdataByte[7]=03 And $txtdataByte[8]=02 And $txtdataByte[9]=01 Then
			$VerifydataByte1=1
		EndIf


		_ConfigTXWinDetails(1,"","",1,"","","","","")

		$HWD2=ControlGetHandle($WIN_BUSMASTER,"","[CLASS:SysListView32; INSTANCE:3]")
		$txtdataByte1=_GUICtrlListView_GetItemTextArray($HWD2,0)
		ConsoleWrite("$txtdataByte1="&$txtdataByte1[1]&@CRLF)											; get the databyte
		ConsoleWrite("$txtdataByte1="&$txtdataByte1[2]&@CRLF)											; get the databyte
		ConsoleWrite("$txtdataByte1="&$txtdataByte1[3]&@CRLF)											; get the databyte
		ConsoleWrite("$txtdataByte1="&$txtdataByte1[4]&@CRLF)											; get the databyte
		ConsoleWrite("$txtdataByte1="&$txtdataByte1[5]&@CRLF)											; get the databyte
		ConsoleWrite("$txtdataByte1="&$txtdataByte1[6]&@CRLF)											; get the databyte
		ConsoleWrite("$txtdataByte1="&$txtdataByte1[7]&@CRLF)											; get the databyte
		ConsoleWrite("$txtdataByte1="&$txtdataByte1[8]&@CRLF)											; get the databyte

		if $txtdataByte1[1]=000 And $txtdataByte1[2]=01 And $txtdataByte1[3]=02 And $txtdataByte1[4]=03 And $txtdataByte1[5]=04 And $txtdataByte1[6]=05 And $txtdataByte1[7]=06 And $txtdataByte1[8]=07 And $txtdataByte1[9]=08 Then
			$VerifydataByte2=1
		EndIf

	EndIf

If $VerifydataByte1=1 And $VerifydataByte2=1 Then   														; Check data byte value in List of Tx window																									;when physical value is changed
		_WriteResult("Pass","TS_TxWin_CAN_17")
Else
		_WriteResult("Fail","TS_TxWin_CAN_17")
EndIf

$isAppNotRes=_CloseApp()														                         	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_17")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_17.au3****"&@CRLF)
ConsoleWrite(@CRLF)