;==============================================Test Cases/Test Data ========================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_03
; TestCases			:		Execute Event Handler
; Test Data			:		cfxNSJ1939_27.cfx
; Test Setup		:		1. Add Data Confirmation event in Event handler
;                           2. Double click on the function added in the event handler. Add the code mentioned in the test data
;                           3.Save the file.
;                           4.Select Build and Load
;                           5.Select Enable Handler
;                           6.Goto function->go online
;                           7.Goto view->Transmit window
;                           8.Select a PGN and message type as Data and add data in the data field.
;                           9.Click on transmit

; Expected Results  : 		Once the data with more than 8bytes is recieved, the message "received" is displayed in the trace window.
;
;===========================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_27.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)
Local $MsgHandler_Trace11=""
if winexists($WIN_BUSMASTER) then
    _loadConfig("cfxNSJ1939_27.cfx")
	sleep(1000)
	_ActivateJ1939()                                                                  ;Activate J1939 Message window
	sleep(1000)
	_J1939NodeSimConfigMenu()
	$TVHWD1=_GetJ1939CppTVHWD()	                                                           ; Fetch the handle of tree view

	_GUICtrlTreeView_Expand($TVHWD1,"",True)													; Expand the tree view items
	sleep(1000)
 _ClickNode(0)
      sleep(1000)

;~ 	    _EditCANCppFile()																		; Clicks on the edit file button
;~ 	    sleep(1000)
;~ 		_AddEventHandlerJ1939("DataConfirmation")                                                ;Add an Data Confirmation event handler
;~ 		$TVHWD=_GetJ1939CppTVHWD()
;~ 	$MsgHWD=_GetJ1939HandlerHWD(2,5)
;~ 	_GUICtrlTreeView_ClickItem($TVHWD, $MsgHWD)					 							; Click on Msg Handler item in the tree view

;~ 	send("{DOWN}")																			; Send the down key to select the Msg handler function
;~     	send("{ENTER}")
;~ 		ControlSend($WIN_BUSMASTER, "", $TXT_EventHandler_CPPFile, "Trace"  & "(" &"" & "Message is transmitted from the node" &  ");")
;~ 		Send("^s")																				; Save the cpp file

;~ 	Send("^{F4}") 																			; Close the C file window
;~ 	sleep(500)
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

	send("^{F4}")
	_ConnectDisconnect()																	; Connect the tool
	Sleep(500)
	_EnableAllHandlersforNode()
	send("^{F4}")
	sleep(1000)
     _EnableOverwriteMode()
	send("^{F4}")
	 send("^{F4}")

	$MsgCount=_GetTraceWinItemCount()													; Fetch the Trace window msg count
	;If $MsgCount>=10 Then

		$MsgHandler_Trace0=_GetTraceWinInfo(0)		;
	    $MsgHandler_Trace1=_GetTraceWinInfo(2)													; Fetch the trace window row info
	    $MsgHandler_Trace2=_GetTraceWinInfo(3)
	    $MsgHandler_Trace3=_GetTraceWinInfo(4)
	    $MsgHandler_Trace4=_GetTraceWinInfo(5)
	    $MsgHandler_Trace5=_GetTraceWinInfo(6)
	    $MsgHandler_Trace6=_GetTraceWinInfo(7)
	    $MsgHandler_Trace7=_GetTraceWinInfo(8)
	    $MsgHandler_Trace8=_GetTraceWinInfo(9)
	    $MsgHandler_Trace9=_GetTraceWinInfo(10)
	    $MsgHandler_Trace10=_GetTraceWinInfo(11)
	    $MsgHandler_Trace11=_GetTraceWinInfo(12)
	    $MsgHandler_Trace12=_GetTraceWinInfo(13)
	;EndIf


	ConsoleWrite("$MsgHandler_Trace10" & $MsgHandler_Trace10 & @CRLF)

	If($MsgHandler_Trace11="Message is transmitted from simulated node") Then
		_WriteResult("Pass","TS_J1939Node_27")
    Else
	   _WriteResult("Fail","TS_J1939Node_27")
	EndIf
EndIf
$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Node_27")
EndIf



ConsoleWrite("****End : TS_NS_J1939_27.au3****"&@CRLF)
ConsoleWrite(@CRLF)




