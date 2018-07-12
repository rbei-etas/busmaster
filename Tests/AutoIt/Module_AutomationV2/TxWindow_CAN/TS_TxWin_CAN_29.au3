;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_29
; TestCases			:		Deleting single message from Tx Message List
; Test Data			:
; Test Setup		:		1. Make sure that some messages are added in Message List.
;							2. select a message in Tx message list.
;							3. Click on the button "Delete"
;							4. Click No Button
;							5. Click Yes Button

; 							Expected Results  :11. A message will displayed asking the user to delete the current selected message or not.
;							2.After step4,the selected message will not be deleted from the list
;							3.After step5,the selected message will be deleted from the list
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_28****"&@CRLF)


Local $ListofMsgBFRDel=0,$ListofMsgAFTRDel=0



_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then
    _loadConfig("TS_TxWin_CAN_19")
	sleep(500)
	_TxMsgMenu()																				; Select CAN->Transmit->Configure menu
	sleep(1000)
	_DelMsgTxWindow(0)
EndIf


	ConsoleWrite("$ListofMsgBFRDel="&$ListofMsgBFRDel&@CRLF)
	ConsoleWrite("$ListofMsgAFTRDel="&$ListofMsgAFTRDel&@CRLF)


	If($ListofMsgBFRDel=1 And $ListofMsgAFTRDel=1 ) Then
		_WriteResult("Pass","TS_TxWin_CAN_28")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_28")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_28")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_28.au3****"&@CRLF)
ConsoleWrite(@CRLF)