;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_12
; TestCases			:		Adding messages in CAN Tx Window List.
; Test Data			:
; Test Setup		:		1. Ensure there is  some messages are added in the Tx List..
;							2.Click on Hex\Dec Icon on the toolbar.
;							3. check the value displayed in the Tx window.

; 							Expected Results  :1. As the Tool bar button is toggled between Hex\dec the values in the Tx window will changed accordingly

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_12****"&@CRLF)


Local $txtdbMsgEXPCTD[5]=["m1","m2","m3","MsgTest1","MsgTest2"],$txtColDbMsgIDHEX=0,$txtColDbMsgIDDEC=0
Local $countDBMsg=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then
    _loadConfig("TS_TxWin_CAN_01")
	sleep(500)
	_TxMsgMenu()																							; Select CAN->Transmit->Configure menu
	sleep(1000)
	$LVhWndTx=_GetTxWindHWD()
	_ConfigTXWinDetails(0,"db",1,"","","","","","")

	$txtColDbMsgIDHEX=_GUICtrlListView_GetItemTextArray($LVhWndTx,0)
	ConsoleWrite("$txtColDbMsgIdHEX="&$txtColDbMsgIDHEX[2]&@CRLF)											; get the Hex Value of msg id

	_DisableHex()

	$txtColDbMsgIDDEC=_GUICtrlListView_GetItemTextArray($LVhWndTx,0)
	ConsoleWrite("$txtColDbMsgIdDEC="&$txtColDbMsgIDDEC[2]&@CRLF)											; get the Dec Value of msg id


EndIf

	If ($txtColDbMsgIdHEX[2]=0x15 And $txtColDbMsgIDDEC[2]=21) Then
		_WriteResult("Pass","TS_TxWin_CAN_12")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_12")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_12")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_12.au3****"&@CRLF)
ConsoleWrite(@CRLF)