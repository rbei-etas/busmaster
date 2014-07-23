;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_28
; TestCases			:		Channel Information
; Test Data			:		TS_TxWin_01.cfx
; Test Setup		:		1. Select a message from message list.
;                           2. Note the channel ID  displayed in the message list.
;                           3. change the channel Id displayed in the Channel ID combo box in Message details view

; Expected Results  : 		Message detail view will show the selected message channel ID.
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_28.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_01.cfx")
		Sleep(1000)
		_TxMsgMenu()
		sleep(1000)
		_DeleteMsgBlock()
		Sleep(1000)
		_ConfigCANTXBlockDetails("Cyclic","Yes",1000,"No","","","No","")						         ; Configure TX block details
	    _AddMsg2TxList(0)																			     ; Add the first msg to Tx list
	    _AddMsg2TxList(1)
        sleep(1000)
		$handle=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)
		_GUICtrlListView_ClickItem($handle,1)
		Sleep(1000)
		$handle1=ControlGetHandle($WIN_BUSMASTER,"", $COMB_ChannelID_ConfigTX)                           ;Get the handle of the channel id combobox
		_GUICtrlComboBoxEx_ShowDropDown($handle1,True)
		Sleep(1000)
		ControlClick($WIN_BUSMASTER,"",$COMB_ChannelID_ConfigTX)                                         ;Select Second channel
		 Sleep(1000)
		send("{DOWN}")
		$Channel=ControlCommand($WIN_BUSMASTER,"",$COMB_ChannelID_ConfigTX,"GetCurrentSelection")
		ConsoleWrite("selection" & $Channel & @CRLF)
		If($Channel=2) Then
			_WriteResult("Pass","TS_Tx_28")
		Else
		   _WriteResult("Fail","TS_Tx_28")
	    EndIf

     EndIf
    $isAppNotRes=_CloseApp()														                         ; Close the app

    if $isAppNotRes=1 Then
		_WriteResult("Warning","TS_Tx_28")
    EndIf

ConsoleWrite("****End : TS_TxWin_28.au3****"&@CRLF)
ConsoleWrite(@CRLF)




