;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_41
; TestCases			:		Message Type information
; Test Data			:		TS_TxWin_01.cfx
; Test Setup		:		1. Select a message from the message lIst.
;                           2. Check the message type in Message detils view.
;                           3. Change message type using radio buttons std, Ext

; Expected Results  : 		 After step the Details view will show the currently assigned message type
;                           2. After step 3 Message detail view will show the changed message type.
;
;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_41.au3****"&@CRLF)
    _launchApp()
	WinActivate($WIN_BUSMASTER)
	if winexists($WIN_BUSMASTER) Then
		_loadConfig("TS_TxWin_01.cfx")
		Sleep(1000)
		_TxMsgMenu()
		sleep(1000)
		_DeleteMsgBlock()
		 Sleep(1000)
		_ConfigCANTXBlockDetails("Cyclic","Yes",1000,"No","","","No","")						      ; Configure TX block details
	    _AddMsg2TxList(0)																			  ; Add the first msg to Tx list
	    _AddMsg2TxList(1)
		$handle=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)                                 ;Get the handle of message details list view
		_GUICtrlListView_ClickItem($handle,1)                                                          ;Click on second message
		Sleep(1000)
		$handle1=ControlGetHandle($WIN_BUSMASTER,"",$RBTN_Exd_ConfigTX)                                ;Get the handle of std radio button in Tx window
		$handle2=ControlGetHandle($WIN_BUSMASTER,"",$RBTN_Std_ConfigTX)                                 ;Get the handle of Extd radio button in Tx window
		$handle3=ControlGetHandle($WIN_BUSMASTER,"",$CHKB_RTR_ConfigTX)                                 ;Get the handle of RTR checkbox in Tx window
		$state1=_GUICtrlButton_GetCheck($handle1)                                                       ;Get check state of std radio button
		$state2=_GUICtrlButton_GetCheck($handle2)                                                       ;Get check state of Extd radio button
		$state3=_GUICtrlButton_GetCheck($handle3)                                                       ;Get check state of RTR CheckBox

        ConsoleWrite("state1 : "& $state1 & @CRLF)
		ConsoleWrite("state2 : "& $state2 & @CRLF)
		ConsoleWrite("state3 : "& $state3 & @CRLF)
		Sleep(1000)
		ControlClick($WIN_BUSMASTER,"",$RBTN_Exd_ConfigTX)                                            ;Set Extd radio button
		$StateNew=_GUICtrlButton_GetCheck($handle1)                                                   ; Get check state of Extd radio button after setting it
		ConsoleWrite("$StateNew" & $StateNew & @CRLF)
		If($StateNew=1 And $state2=1) Then
			 _WriteResult("Pass","TS_Tx_041")
		Else
		   _WriteResult("Fail","TS_Tx_041")
	    EndIf

EndIf
$isAppNotRes=_CloseApp()														                         ; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Tx_041")
EndIf

ConsoleWrite("****End : TS_TxWin_41.au3****"&@CRLF)
ConsoleWrite(@CRLF)



