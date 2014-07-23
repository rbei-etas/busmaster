;==================================================Test Cases/Test Data =============================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TxWin_31
; TestCases			:		Delete functionality using Right Click on a message item in Tx Message list
; Test Data			:		TS_TxWin_01.cfx
; Test Setup		:		1. Make sure a message block is added in Tx window.
;                           2. Add some message in Tx message List.
;                           3. select a message in Tx message list
;                           4. Right click on the selected message.
;                           5. Click on the "Delete" item.
;                           6. select an option and check the message list

; Expected Results  : 		 After step 5 a confirmation message will appear.
 ;                           if Yes option is selected then the selected message will be delete from the list.
;                            If No option is selected the list will be unchanged.

;==================================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TxWin_31.au3****"&@CRLF)
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
		$handle=ControlGetHandle($WIN_BUSMASTER,"",$LSTC_MsgDetailsID)                                   ;Get handle of message details list
	    _GUICtrlListView_ClickItem($handle,1,"Right")                                                     ;Right click on the second message
        Sleep(500)
		_GUICtrlListView_ClickItem($handle,1)
	    sleep(500)
	    Send("{D}")                                                                                          ;click 'D' to delete block
	    If WinExists($WIN_BUSMASTER) Then
			$WinExists=1
		    ControlClick($WIN_BUSMASTER,"",$BTN_Yes_DeleteBlk)
	    Else
		  $WinExists=0
	    EndIf
	    ConsoleWrite("$WinExists" & $WinExists & @CRLF)
	    Sleep(500)
	    $count=_GUICtrlListView_GetItemCount($handle)
	    ConsoleWrite("No of items" & $count & @CRLF)
		If($WinExists=1 And $count=1) Then
			_WriteResult("Pass","TS_Tx_031")
		Else
		   _WriteResult("Fail","TS_Tx_031")
	    EndIf

     EndIf
    $isAppNotRes=_CloseApp()														                         ; Close the app

    if $isAppNotRes=1 Then
		_WriteResult("Warning","TS_Tx_031")
    EndIf

ConsoleWrite("****End : TS_TxWin_31.au3****"&@CRLF)
ConsoleWrite(@CRLF)




