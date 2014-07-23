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
		Sleep(1000)
		$hWnd = ControlGetHandle($WIN_BUSMASTER,"",$LSTC_Sig_DetailsID)
        $a=_GUICtrlListView_GetItemPosition($hWnd, 0)
	    ControlClick($WIN_BUSMASTER,"",$LSTC_Sig_DetailsID,"Left",2,$a[0],$a[1])                         ;Double Click on Signal name
	    sleep(1000)
		If WinExists($WIN_Signal_Details) Then                                                           ; check if signal details dialog is opened
		     _WriteResult("Pass","TS_Tx_26")
			 Sleep(500)
			 WinClose($WIN_Signal_Details)
	    Else
		    _WriteResult("Fail","TS_Tx_26")
		EndIf

EndIf
$isAppNotRes=_CloseApp()														                         ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_26")
EndIf

ConsoleWrite("****End : TS_TxWin_26.au3****"&@CRLF)
ConsoleWrite(@CRLF)



