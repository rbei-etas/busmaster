;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_12
; TestCases			:		Execute Time handler
; Test Data			:		cfxNSCAN_12.cfx
; Test Setup		:		1. Write a Time handler for time value 3000 msec
;~ 							2. Initialise a message structure
;~ 							3. Enable Time handler
;~ 							4. Connect

; Expected Results  : 		1. After step 4, the handler should get executed displaying the output in Notification window.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_12.au3****"&@CRLF)


_launchApp()

local $TimeHandler1=0,$TimeHandler2=0,$TimeHandler3=0,$TimeHandlers=0,$TimeHandlers_Trace=0

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxNSCAN_12")																; Load Configuration

	$NodeSimToolbar=_CANNodeSimToolbarActive()												; Check whether CAN node simulation toolbar is active

	if $NodeSimToolbar=0 Then
		send("!vtcn")																		; Select View->Toolbar->CAN->Node Simulation menu
		sleep(1000)
	EndIf

	_CANNodeSimConfigMenu()																	; Open CAN node Simulation window

	_NewSimSysFile("TestNS_12")

	_AddNode("TestNode")																	; Add a node

	_AddCANCppFile("TimeHandlerNS_12")														; Add a new Cpp file

	_AddCANTimeHandler(3000)

	$TVHWD=_GetCANCppTVHWD()																; Fetch the handle of tree view
	$TimeHWD=_GetCANHandlerHWD(2,3)
	_GUICtrlTreeView_ClickItem($TVHWD, $TimeHWD)					 						; Right click on Time Handler item in the tree view

	send("{DOWN}")																			; Send the down key to select the Msg handler function

	send("{ENTER}")

	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "STCAN_MSG sMsg;" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "sMsg.m_unMsgID = 0x101;              // Message ID" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "sMsg.m_ucEXTENDED = FALSE;           // Standard Message type" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "sMsg.m_ucRTR = FALSE;                // Not RTR type" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "sMsg.m_ucDLC = 8;                    // Length is 8 Bytes" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "sMsg.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "sMsg.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "sMsg.m_ucChannel = 1;                // First CAN channel" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "SendMsg(sMsg);" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "Trace" & "(" & Chr(34) & "Time Handler" & Chr(34) & ");")

	Send("^s")																				; Save the cpp file

	Send("^{F4}") 																			; Close the C file window
	sleep(500)

	Send("^{F4}") 																			; Close the Node Simulation window
	sleep(500)

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

	_TimeHandlers()																			; Enable Time handler


	_ConnectDisconnect()																	; Connect the tool
	Sleep(9000)

	_ConnectDisconnect()																	; Disconnect the tool
	Sleep(1000)

	$MsgCount=_GetCANMsgWinItemCount()														; Fetch the Msg window msg count
	$TimeHandler_Trace1=_GetTraceWinInfo(0)													; Fetch the trace window first row info
	$TimeHandler_Trace2=_GetTraceWinInfo(1)													; Fetch the trace window second row info
	$TimeHandler_Trace3=_GetTraceWinInfo(2)													; Fetch the trace window third row info

	_UnLoadALL()																			; Unload All from the menu

	$HWD= ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)								; Fetch the Msg Window Handle
	_GUICtrlListView_ClickItem($HWD,0)														; Click on the first item in msg window

	if $MsgCount>2 Then
		$Msg_1=_GetMsgWinCANInfo(0)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_1[" & $i &"] : "&$Msg_1[$i]&@CRLF)
		next
		if $Msg_1[1]="Rx" and $Msg_1[2]=1 and $Msg_1[4]=0x101 and $Msg_1[6]=8 and $Msg_1[7]="0A00000014000000" Then					; Compare the Direction, Channel and Msg ID of the first row
			$TimeHandler1=1
		EndIf

		$Msg_2=_GetMsgWinCANInfo(1)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_2[" & $i &"] : "&$Msg_2[$i]&@CRLF)
		next
		if $Msg_2[1]="Rx" and $Msg_2[2]=1 and $Msg_2[4]=0x101 and $Msg_2[6]=8 and $Msg_2[7]="0A00000014000000" Then					; Compare the Direction, Channel and Msg ID of the second row
			$TimeHandler2=1
		EndIf

		$Msg_3=_GetMsgWinCANInfo(2)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_3[" & $i &"] : "&$Msg_3[$i]&@CRLF)
		next
		if $Msg_3[1]="Rx" and $Msg_3[2]=1 and $Msg_3[4]=0x101 and $Msg_3[6]=8 and $Msg_3[7]="0A00000014000000" Then					; Compare the Direction, Channel and Msg ID of the third row
			$TimeHandler3=1
		EndIf
	EndIf

	if $TimeHandler1=1 and $TimeHandler2=1 and $TimeHandler3=1 Then
		$TimeHandlers=1
	EndIf

	if $TimeHandler_Trace1="Time Handler" and $TimeHandler_Trace2="Time Handler" and $TimeHandler_Trace3="Time Handler" Then
		$TimeHandlers_Trace=1
	EndIf
EndIf


ConsoleWrite("$MsgCount :"&$MsgCount&@CRLF)
ConsoleWrite("$TimeHandler1 :"&$TimeHandler1&@CRLF)
ConsoleWrite("$TimeHandler2 :"&$TimeHandler2&@CRLF)
ConsoleWrite("$TimeHandler3 :"&$TimeHandler3&@CRLF)
ConsoleWrite("$TimeHandlers :"&$TimeHandlers&@CRLF)
ConsoleWrite("$TimeHandler_Trace1 :"&$TimeHandler_Trace1&@CRLF)
ConsoleWrite("$TimeHandler_Trace2 :"&$TimeHandler_Trace2&@CRLF)
ConsoleWrite("$TimeHandler_Trace3 :"&$TimeHandler_Trace3&@CRLF)
ConsoleWrite("$TimeHandlers_Trace :"&$TimeHandlers_Trace&@CRLF)

if $TimeHandlers=1 and $TimeHandlers_Trace=1 Then
	_WriteResult("Pass","TS_NS_12")
Else
	_WriteResult("Fail","TS_NS_12")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_12")
EndIf


ConsoleWrite("****End : TS_NS_12.au3****"&@CRLF)
ConsoleWrite(@CRLF)