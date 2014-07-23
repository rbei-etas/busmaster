;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_03
; TestCases			:		Message interpretation
; Test Data			:		
; Test Setup		:		1. Transmit messages which are present in the database and currently associated to configuration.
;~ 							2. Double click on a particular message in message window.
;~ 							3. While the 'Message Interpretation' dialog is visible, click on any other interpretable message in the message window.

; Expected Results  : 		1. After step 2, a dialog 'Message Interpretation' will be displayed which contains message details and signal details.
;~ 							2. After step3, the 'Message Interpretation' dialog will be updated with the message details of the selected message in message window.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_03.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then
	
	_loadConfig("cfxCANMsgWin_01")																					; Load Configuration

	_ConnectDisconnect()																							; Connect
	
	_TransmitMsgsMenu()																								; Transmit CAN messages
	sleep(800)

	$Msg_1=_GetMsgInterpretWinInfo(0)
	
	_ConnectDisconnect()																							; Disconnect

EndIf

if $Msg_1[1]="MsgExtBig" and $Msg_1[2]=104 and $Msg_1[3]="SigExt2" and $Msg_1[4]=1.800 and $Msg_1[5]="0x0" Then
	_WriteResult("Pass","TS_MSGWIN_03")
Else
	_WriteResult("Fail","TS_MSGWIN_03")
EndIf

$isAppNotRes=_CloseApp()																							; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_03")
EndIf



ConsoleWrite("****End : TS_MSGWIN_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
