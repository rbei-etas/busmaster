;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_CAN_35
; TestCases			:		Deleting all messages from Tx Message List
; Test Data			:
; Test Setup		:		1. Make sure that some messages are added in Message List.
;							2. select a message in Tx message list.
;							3. Click on the button "Delete All"
;							4. Click No Button
;							5.Repeate the above step and click on "Yes" button.

; 							Expected Results  :1. A message will displayed asking for confirmation about the deletion of all the messages.
;							2. After step4, the Tx Message list will remain unchanged
;							3.After step5, all the messages in the list will be deleted from the list
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_CAN_35****"&@CRLF)


Local $countMsgListBFRDel=0,$countMsgListAFTRDel=0,$DelALLDialogBoxTXT=0,$countMsgListNO=0,$countMsgListYES=0,$AfterDelAll=0,$AddMsgTxt=""

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then
    _loadConfig("TS_TxWin_CAN_19")
	sleep(500)
	_TxMsgMenu()																				; Select CAN->Transmit->Configure menu
	$countMsgListBFRDel=_CountMsgTxFrameList()
	ConsoleWrite("$countMsgListBFRDel="&$countMsgListBFRDel&@CRLF)
	sleep(1000)
	_DelMsgALLTxWindow()
	if WinWaitActive($WIN_BUSMASTER,$TXT_DelAllMsgsTxWin,2) Then								; wait for Delete ALL dialog
		sleep(500)
		$TXT=ControlGetText($WIN_BUSMASTER,"",$TXT_DeleALL_DialogBox)
		if $TXT= $TXT_DelAllMsgsTxWin Then
			$DelALLDialogBoxTXT=1
		EndIf
		ControlClick($WIN_BUSMASTER,$TXT_DelAllMsgsTxWin,$BTN_No_DelALlDialog)					; Click on No button
	EndIf
	sleep(2000)
	$countMsgListNO=_CountMsgTxFrameList()
	_DelMsgALLTxWindow()

	if WinWaitActive($WIN_BUSMASTER,$TXT_DelAllMsgsTxWin,2) Then								; wait for Delete ALL dialog
		sleep(500)

		ControlClick($WIN_BUSMASTER,$TXT_DelAllMsgsTxWin,$BTN_yes_DelALlDialog)				; Click on Yes button
	EndIf

	;$countMsgListYES=_CountMsgTxFrameList()														; Count Message List After delete ALL
	;$LVhWndTx=_GetTxWindHWD()
	;$countMsgListYES=_GUICtrlListView_GetItemCount($LVhWndTx)
	$LVhWndTx=_GetTxWindHWD()
	$AddMsgTxt=_GUICtrlListView_GetItemText($LVhWndTx, 0)
	ConsoleWrite("$AddMsgTxt="&$AddMsgTxt&@CRLF)

	$countMsgListYES=_CountMsgTxFrameList()
	ConsoleWrite("$countMsgListYES="&$countMsgListYES&@CRLF)

	if $AddMsgTxt="[Add Message]" Or $countMsgListYES=0 Or $countMsgListYES=-1 Then
		$AfterDelAll=1
	EndIf

EndIf

	ConsoleWrite("$countMsgListBFRDel="&$countMsgListBFRDel&@CRLF)
	ConsoleWrite("$countMsgListNO="&$countMsgListNO&@CRLF)
	ConsoleWrite("$countMsgListYES="&$countMsgListYES&@CRLF)
	ConsoleWrite("$AfterDelAll="&$AfterDelAll&@CRLF)


	If ($countMsgListBFRDel=2 And $countMsgListNO=2 And $AfterDelAll=1) Then
		_WriteResult("Pass","TS_TxWin_CAN_35")
	Else
		_WriteResult("Fail","TS_TxWin_CAN_35")
	EndIf


	$isAppNotRes=_CloseApp()														; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TxWin_CAN_35")
EndIf

ConsoleWrite("****End : TS_TxWin_CAN_35.au3****"&@CRLF)
ConsoleWrite(@CRLF)