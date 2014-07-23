;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_02
; TestCases			:		Message Population in Message Window for transmitted messages.
; Test Data			:		
; Test Setup		:		Click on the "Connect" option and click the "Start" option for sending some messages

; Expected Results  : 		The Sent items should get displayed in Message Window with "Tx" notation.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_02.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $FirstMsg=0,$SecondMsg=0,$ThirdMsg=0,$FourthMsg=0
if winexists($WIN_BUSMASTER) then
	
	_loadConfig("cfxCANMsgWin_01")																					; Load Configuration
	
	_ConnectDisconnect()																							; Connect
	
	_TransmitMsgsMenu()																								; Transmit CAN messages
	sleep(800)

	_ConnectDisconnect()																							; Disconnect

	$Msg_1=_GetMsgInterpretWinInfo(0)

	$rCount=_GetCANMsgWinItemCount()

	if $rCount=4 Then
	
		$Msg_1=_GetMsgWinCANInfo(0)																						; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_1[" & $i &"] : "&$Msg_1[$i]&@CRLF)
		next
		if $Msg_1[0]<>" " and $Msg_1[1]="Tx" and $Msg_1[2]=1 and $Msg_1[3]="x" and $Msg_1[4]="0x104" and $Msg_1[5]="MsgExtBig" and $Msg_1[6]=8 and $Msg_1[7]="0000000000000000" Then			; Compare the Direction, Channel and Msg ID of the first row
			$FirstMsg=1
		EndIf

		$Msg_2=_GetMsgWinCANInfo(1)																						; Fetch the second row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_2[" & $i &"] : "&$Msg_2[$i]&@CRLF)
		next
		if $Msg_2[0]<>" " and $Msg_2[1]="Rx" and $Msg_2[2]=2 and $Msg_2[3]="x" and $Msg_2[4]="0x104" and $Msg_2[5]="MsgExtBig" and $Msg_2[6]=8 and $Msg_2[7]="0000000000000000" Then			; Compare the Direction, Channel and Msg ID of the first row
			$SecondMsg=1
		EndIf

		$Msg_3=_GetMsgWinCANInfo(2)																						; Fetch the third row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_3[" & $i &"] : "&$Msg_3[$i]&@CRLF)
		next
		if $Msg_3[0]<>" " and $Msg_3[1]="Tx" and $Msg_3[2]=1 and $Msg_3[3]="x" and $Msg_3[4]="0x103" and $Msg_3[5]="MsgExtLil" and $Msg_3[6]=8 and $Msg_3[7]="0000000000000000" Then			; Compare the Direction, Channel and Msg ID of the first row
			$ThirdMsg=1
		EndIf

		$Msg_4=_GetMsgWinCANInfo(3)																						; Fetch the third row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_4[" & $i &"] : "&$Msg_4[$i]&@CRLF)
		next
		if $Msg_4[0]<>" " and $Msg_4[1]="Rx" and $Msg_4[2]=2 and $Msg_4[3]="x" and $Msg_4[4]="0x103" and $Msg_4[5]="MsgExtLil" and $Msg_4[6]=8 and $Msg_4[7]="0000000000000000" Then			; Compare the Direction, Channel and Msg ID of the first row
			$FourthMsg=1
		EndIf
		
	EndIf

EndIf

consolewrite("$rCount: "&$rCount&@CRLF)
consolewrite("$FirstMsg: "&$FirstMsg&@CRLF)
consolewrite("$SecondMsg: "&$SecondMsg&@CRLF)
consolewrite("$ThirdMsg: "&$ThirdMsg&@CRLF)
consolewrite("$FourthMsg: "&$FourthMsg&@CRLF)

if $FirstMsg=1 and $SecondMsg=1 and $ThirdMsg=1 and $FourthMsg=1 Then
	_WriteResult("Pass","TS_MSGWIN_02")
Else
	_WriteResult("Fail","TS_MSGWIN_02")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_02")
EndIf



ConsoleWrite("****End : TS_MSGWIN_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
