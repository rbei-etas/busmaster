;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_27
; TestCases			:		Channel Information
; Test Data			:		TS_TxWin_01.cfx
; Test Setup		:		1. Select a message from message list.
;                           2. click on Channel ID combo box from the details view.
;                           3. Check the number of channels in the combo box.

; Expected Results  : 		The number of channels must be equal to the number of channel in the Busmaster
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_27.au3****"&@CRLF)
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
		ConsoleWrite("$handle" & $handle & @CRLF)
		Sleep(1000)
		$handle1=ControlGetHandle($WIN_BUSMASTER,"", $COMB_ChannelID_ConfigTX)                           ;Get the handle of the channel id combobox
		_GUICtrlComboBoxEx_ShowDropDown($handle1,True)
		ConsoleWrite("$handle1" & $handle1 & @CRLF)
		Sleep(1000)
		$text=_GUICtrlComboBoxEx_GetList($handle1)                                                       ;Retrieve all the items of the combobox
		ConsoleWrite("Combobox items" & $text & @CRLF)
		$item=StringSplit($text,"|")
		ConsoleWrite("$item[0]" & $item[0] & @CRLF)
		ConsoleWrite("$item[1]" & $item[1] & @CRLF)
		ConsoleWrite("$item[2]" & $item[2] & @CRLF)
		If($item[0]=2 And $item[1]=1 And $item[2]=2) Then
			_WriteResult("Pass","TS_Tx_27")
		Else
		   _WriteResult("Fail","TS_Tx_27")
	    EndIf

EndIf
$isAppNotRes=_CloseApp()														                         ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_27")
EndIf

ConsoleWrite("****End : TS_TxWin_27.au3****"&@CRLF)
ConsoleWrite(@CRLF)




