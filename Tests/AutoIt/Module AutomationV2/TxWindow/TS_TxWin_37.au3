;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_37
; TestCases			:		Load Old version files
; Test Data			:		TS_TxWin_01.cfx
; Test Setup		:		1. Start the application
;                           2. Select Configure -> Tx Messages.. Option.
;                           3. Add a new message block.
;                           4. Add few messages to this message block.
;                           5. Select checkbox associated with all messages such that alternate messages will be enabled for transmission.
;                           6. Set the channel ID of the messages such that channel ID will be different for consecutive messages.
;                           7. Close the window using 'Close' button.
;                           8. Check for changes save message box.

; Expected Results  : 		Application will throw a message asking to save changes done with Tx message configuration.
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_37.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_01.cfx")
		Sleep(1000)
		_TxMsgMenu()
		sleep(1000)
		_DeleteMsgBlock()
		Sleep(1000)
		 _DisableAutoUpdate()
		 sleep(500)
		_ConfigCANTXBlockDetails("Cyclic","Yes",1000,"No","","","No","")						      ; Configure TX block details
	    _AddMsg2TxList(0)																			      ; Add the first msg to Tx list
	    _AddMsg2TxList(1)
		 _AddMsg2TxList(2)
		  _AddMsg2TxList(3)
		  $handle=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)

		_GUICtrlListView_SetItemChecked($handle,1,False)                                                                  ; Uncheck first message
	_GUICtrlListView_SetItemChecked($handle,3,False)
		Sleep(1000)

		$handle1=ControlGetHandle($WIN_BUSMASTER,"", $COMB_ChannelID_ConfigTX)                           ;Get the handle of the channel id combobox
		_GUICtrlListView_ClickItem($handle,1)
		Sleep(1000)
		_GUICtrlComboBoxEx_ShowDropDown($handle1,True)
		Sleep(1000)
		ControlClick($WIN_BUSMASTER,"",$COMB_ChannelID_ConfigTX)                                         ;Select Second channel
		 Sleep(1000)
		send("{DOWN}")
		Sleep(1000)
		Send("{ENTER}")
		_GUICtrlListView_ClickItem($handle,3)
		Sleep(1000)
		_GUICtrlComboBoxEx_ShowDropDown($handle1,True)
		Sleep(1000)
		ControlClick($WIN_BUSMASTER,"",$COMB_ChannelID_ConfigTX)                                         ;Select Second channel
		 Sleep(1000)
		send("{DOWN}")
		Sleep(1000)
		Send("{ENTER}")
		Sleep(1000)
		ControlClick($WIN_BUSMASTER,"",1015)
		If WinWaitActive("","Do you want to save changes?",2) Then
			$res=1
	        ControlClick($WIN_BUSMASTER,"",7)
        Else
			$res=0
		EndIf
        ConsoleWrite("$res" & $res & @CRLF)
		If($res=1) Then
			_WriteResult("Pass","TS_Tx_037")
	    Else
		    _WriteResult("Fail","TS_Tx_037")
	    EndIf

EndIf
$isAppNotRes=_CloseApp()														; Close the app
if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_037")
EndIf
ConsoleWrite("****End : TS_TxWin_37.au3****"&@CRLF)
ConsoleWrite(@CRLF)






