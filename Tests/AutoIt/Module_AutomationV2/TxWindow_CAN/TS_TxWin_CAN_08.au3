;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_08
; TestCases			:		Adding messages in CAN Tx Window List.
; Test Data			:
; Test Setup		:		1.Associate any CAN database.
;							2.Open CAN Tx Window.
;							3.Click on dropdown Add message and select any db messages from the list.

; 							Expected Results  :1.After step3 All database messages will displayed in dropdown list.On selecting message will be added to list.

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_08****"&@CRLF)


Local $txtdbMsgEXPCTD[5]=["m1","m2","m3","MsgTest1","MsgTest2"]
Local $countDBMsg=0
$countRowChckBox=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then
    _loadConfig("TS_TxWin_CAN_01")
	sleep(500)
	_TxMsgMenu()																							; Select CAN->Transmit->Configure menu
	sleep(1000)
	$LVhWndTx=_GetTxWindHWD()

	for $i=1 to 5
		_ConfigTXWinDetails(0,"db",$i,"","","","","","")
		$txtColDbMsg=_GUICtrlListView_GetItemText($LVhWndTx, 0)
		ConsoleWrite("$txtColDbMsg="&$txtColDbMsg&@CRLF)
		if $txtColDbMsg=$txtdbMsgEXPCTD[$i-1] Then
			$countDBMsg=$countDBMsg+1
		EndIf

	Next

EndIf

	ConsoleWrite("$countDBMsg="&$countDBMsg&@CRLF)

	If ($countDBMsg=5) Then
		_WriteResult("Pass","TS_TxWin_CAN_08")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_08")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_08")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)