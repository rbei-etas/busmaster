;========================================Test Cases/Test Data =====================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_28
; TestCases			:		Adding a message handler for all received messages
; Test Data			:		cfxNSJ1939_28.cfx
; Test Setup		:		1.Create sim file.Add a node
;                           2.Add a message handler for all Rx PGNs and add code to transmit ACL PGN and add a address claim event handler
;                           3.Double click on the function added in the event handler and add the code.Save the file.

;                           4.Select Build and Load
;                           5.Select Enable all Handler
;
;                           6.Goto view->Transmit window.Transmit a data PGN
;

; Expected Results  : 		"andler is executed whenever a node claims or loses an address. And the message "Address claimed" is displayed in the trace window
;
;========================================Test Procedure ===========================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_28.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)
Local $SrcAddr=0,$DestAddr=0,$DataBytes=0,$TxRx=0,$Dlc=0,$msgType=0
if winexists($WIN_BUSMASTER) then
     _loadConfig("cfxNsJ1939_28.cfx")
	sleep(1000)
	_ActivateJ1939()                                                                  ;Activate J1939 Message window
	sleep(1000)
	_J1939NodeSimConfigMenu()

	$TVHWD1=_GetJ1939CppTVHWD()	                                                           ; Fetch the handle of tree view
	_GUICtrlTreeView_Expand($TVHWD1,"",True)													; Expand the tree view items
	sleep(1000)
     _ClickNode(0)
	sleep(1000)
																			; Close the C file window
	sleep(500)
	_Click_BuildLoad()
	sleep(500)
	WinWaitActive("",$Msg_BuildLoadError,5)
	if winexists("",$Msg_BuildLoadError) Then

		_BuildLoadError()																	; Close the pop up msg
		$OutputwinHWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTB_Details_TraceWin)			; get the handle of output window
		$itemCount=_GUICtrlListBox_GetCount($OutputwinHWD)									; Get the output window item count
		for $i=0 to $itemCount-1
			$outtxt[$i]=_GUICtrlListBox_GetText($OutputwinHWD, $i)							; Fetch the text in the output window
			ConsoleWrite("$outtxt["&$i&"]:"&$outtxt[$i]&@CRLF)
		Next
		send("^{F4}")																		; Close the output window
	EndIf


	_EnableAllHandlersforNode()
	send("^{F4}")
	Sleep(500)
	_ConnectDisconnect()																	; Connect the tool
	Sleep(500)
	send("^{F4}")
	Sleep(500)
	_J1939tMsgWin("Data","[0x400]Msg_Data",0,50)
	 sleep(1000)
	   ;_DisableOverwriteMode()
	_UnLoadALLJ1939()

    Sleep(1000)
	$OutputwinHWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTB_Details_TraceWinInfo)			; get the handle of output window
	$itemCount=_GUICtrlListBox_GetCount($OutputwinHWD)									; Get the output window item count
	ConsoleWrite("item count" & $itemCount & @CRLF)
	$OutputTxt=_GUICtrlListBox_GetText($OutputwinHWD, 7)
	ConsoleWrite("Text " & $OutputTxt & @CRLF)
	If($OutputTxt="Address claimed") Then
		_WriteResult("Pass","TS_J1939Node_28")
    Else
		_WriteResult("Fail","TS_J1939Node_28")
	EndIf
EndIf
$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Node_28")
EndIf



ConsoleWrite("****End : TS_J1939Node_28.au3****"&@CRLF)
ConsoleWrite(@CRLF)

	   															;


