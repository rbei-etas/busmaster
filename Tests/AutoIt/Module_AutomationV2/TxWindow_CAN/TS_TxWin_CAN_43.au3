;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_43 - 	TS_TxWin_CAN_44
; TestCases			:		Checking the Deleting of single message from Tx Message List during transmission.
; Test Data			:
; Test Setup		:		1. Make sure that some messages are added in Message List.
;							2. Connect Tool and open Tx Window.
;							3. select a message in Tx message list.
;							4. Check the button "Delete" and Delete ALL

; 							Expected Results  :1. Delete button and Delete All will be disable.

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_43-44****"&@CRLF)


Local $DeleteBTNStatus=0,$DeleteALLBTNStatus=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then
    _loadConfig("TS_TxWin_CAN_19")
	sleep(500)
	_ConnectDisconnect()
	sleep(1000)
	_TxMsgMenu()																								; Select CAN->Transmit->Configure menu
	sleep(1000)
	$DeleteBTNStatus=ControlCommand($WIN_BUSMASTER, "Delete", $BTN_Del_TxWindow, "IsEnabled", "")				; Check Delete BTN Status when Tool is c onnected
	$DeleteALLBTNStatus=ControlCommand($WIN_BUSMASTER, "Delete All", $BTN_DelALL_TxWindow, "IsEnabled", "")		; Check Delete All BTN Status when Tool is connected
EndIf

	ConsoleWrite("$DeleteBTNStatus="&$DeleteBTNStatus&@CRLF)
	ConsoleWrite("$DeleteALLBTNStatus="&$DeleteALLBTNStatus&@CRLF)


	If ($DeleteBTNStatus=0 ) Then
		_WriteResult("Pass","TS_TxWin_CAN_43")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_43")
	EndIf

	If ($DeleteALLBTNStatus=0 ) Then
		_WriteResult("Pass","TS_TxWin_CAN_44")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_44")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_43")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_43-44.au3****"&@CRLF)
ConsoleWrite(@CRLF)