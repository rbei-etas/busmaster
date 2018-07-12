;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_14
; TestCases			:		Event for key handler for an alphabet and  "*" key
; Test Data			:		cfxNSCAN_14.cfx,TestNS_14_SIM.sim,KeyHandlerNS_14.cpp
; Test Setup		:		"Edit the event handler OnKey_a(unsigned char KeyValue) and OnKey_All(unsigned char KeyValue).
;~ 							Execute the key handler after building the DLL for it and loading it for a node.

; Expected Results  : 		"Only OnKey_a(unsigned char KeyValue) handler shoule get executed as individual key handler will take priority over the * key."
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_14.au3****"&@CRLF)


_launchApp()

local $KeyHandler_All=0,$KeyHandler_p=0,$KeyHandlers=0,$KeyHandler_Trace_All=0,$KeyHandler_Trace_p=0,$KeyHandlers_Trace=0

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxNSCAN_14")																; Load Configuration

	$NodeSimToolbar=_CANNodeSimToolbarActive()												; Check whether CAN node simulation toolbar is active

	if $NodeSimToolbar=0 Then
		send("!vtcn")																		; Select View->Toolbar->CAN->Node Simulation menu
		sleep(1000)
	EndIf

	_BuildLoadALL()																			; Build and Load All from the menu

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

	send("^{F4}")																			; Close the output window

	_AllKeyHandlers()																		; Click on 'Enable/Disable All key handlers' icon
	sleep(1500)

	_ConnectDisconnect()																	; Connect the tool
	Sleep(2000)

	send("B")																				; Key event fot handler 'ALL'

	send("p")																				; Key event fot handler 'p'

	_ConnectDisconnect()																	; Disconnect the tool
	Sleep(1000)

	$MsgCount=_GetCANMsgWinItemCount()														; Fetch the Msg window msg count
	$KeyHandler_Trace_All=_GetTraceWinInfo(0)												; Fetch the trace window first row info
	$KeyHandler_Trace_p=_GetTraceWinInfo(1)													; Fetch the trace window second row info

	_UnLoadALL()																			; Unload All from the menu

	$HWD= ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)								; Fetch the Msg Window Handle
	_GUICtrlListView_ClickItem($HWD,0)														; Click on the first item in msg window

	if $MsgCount=2 Then
		$Msg_1=_GetMsgWinCANInfo(0)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_1[" & $i &"] : "&$Msg_1[$i]&@CRLF)
		next
		if $Msg_1[1]="Rx" and $Msg_1[2]=1 and $Msg_1[4]=0x57 and $Msg_1[6]=8 and $Msg_1[7]="0D00000017000000" Then					; Compare the Direction, Channel and Msg ID of the first row
			$KeyHandler_All=1
		EndIf

		$Msg_2=_GetMsgWinCANInfo(1)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_2[" & $i &"] : "&$Msg_2[$i]&@CRLF)
		next
		if $Msg_2[1]="Rx" and $Msg_2[2]=1 and $Msg_2[4]=0x60 and $Msg_2[6]=8 and $Msg_2[7]="0D00000017000000" Then					; Compare the Direction, Channel and Msg ID of the second row
			$KeyHandler_p=1
		EndIf
	EndIf

	if $KeyHandler_All=1 and $KeyHandler_p=1 Then
		$KeyHandlers=1
	EndIf

	if $KeyHandler_Trace_All="Key Handler All" and $KeyHandler_Trace_p="Key Handler P" Then
		$KeyHandlers_Trace=1
	EndIf
EndIf


ConsoleWrite("$MsgCount :"&$MsgCount&@CRLF)
ConsoleWrite("$KeyHandler_All :"&$KeyHandler_All&@CRLF)
ConsoleWrite("$KeyHandler_p :"&$KeyHandler_p&@CRLF)
ConsoleWrite("$KeyHandlers :"&$KeyHandlers&@CRLF)
ConsoleWrite("$KeyHandler_Trace_All :"&$KeyHandler_Trace_All&@CRLF)
ConsoleWrite("$KeyHandler_Trace_p :"&$KeyHandler_Trace_p&@CRLF)
ConsoleWrite("$KeyHandlers_Trace :"&$KeyHandlers_Trace&@CRLF)

if $KeyHandlers=1 and $KeyHandlers_Trace=1 Then
	_WriteResult("Pass","TS_NS_14")
Else
	_WriteResult("Fail","TS_NS_14")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_14")
EndIf


ConsoleWrite("****End : TS_NS_14.au3****"&@CRLF)
ConsoleWrite(@CRLF)