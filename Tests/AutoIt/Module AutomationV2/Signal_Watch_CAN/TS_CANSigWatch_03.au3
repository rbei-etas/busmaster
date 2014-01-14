;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_CANSigWatch_03
; TestCases			:		"Signal watch window Functionality"
; Test Data			:
; Test Setup		:		1. Connect the CANvas to the bus.
;~ 							2. Start sending messages.
;~ 							3. observe the details of signal watch window.

; Expected Results  : 		1. Signal watch window will show the physical and raw valus of the configured messages.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_CANSigWatch_03.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

Local $Msg1=0, $Msg2=0
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANSigWatch_03")																		; Load Configuration

	_ConnectDisconnect()																					; Connect

	_TransmitMsgsMenu()																						; Transmit CAN messages
	sleep(1000)

	_SignalWatchMenu($CAN,$SigWatchActivateMenu)															; Open signal watch window

	$HWD=ControlGetHandle($WIN_SigWatch&$CAN,"",$LVC_Value_SigWatch)										; Fetch the handle of Signal watch window
	$ItemCount=_GUICtrlListView_GetItemCount($HWD)															; Fetch the count of items

	if $ItemCount=2 Then

		$MsgStdBig=_GetSigWatchWinInfo($CAN,0)

		if $MsgStdBig[1]="MsgStdBig" and $MsgStdBig[2]="Sigstd2" and $MsgStdBig[3]=3.100 and $MsgStdBig[4]=0 Then
			$Msg1=1
		EndIf

		$MsgStdLil=_GetSigWatchWinInfo($CAN,1)

		if $MsgStdLil[1]="MsgStdLil" and $MsgStdLil[2]="Sigstd1" and $MsgStdLil[3]=0.000 and $MsgStdLil[4]=0 Then
			$Msg2=1
		EndIf

	EndIf

	winclose($WIN_SigWatch&$CAN)																			; Close the signal watch window

	_ConnectDisconnect()																					; Disconnect

EndIf

ConsoleWrite("$Msg1 : "&$Msg1&@CRLF)
ConsoleWrite("$Msg2 : "&$Msg2&@CRLF)

if $Msg1=1 and $Msg2=1 Then
	_WriteResult("Pass","TS_CANSigWatch_03")
Else
	_WriteResult("Fail","TS_CANSigWatch_03")
EndIf

$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_CANSigWatch_03")
EndIf



ConsoleWrite("****End : TS_CANSigWatch_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)
