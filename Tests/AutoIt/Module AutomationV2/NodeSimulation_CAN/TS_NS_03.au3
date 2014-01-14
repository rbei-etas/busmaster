;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_03
; TestCases			:		Message Handler - 1 Message only
; Test Data			:		cfxNSCAN_03.cfx,testNS_03.dbf
; Test Setup		:		"1. open the function editor for a node.
;~ 							2. Make sure "MESSAGE1" is defined in the database
;~ 							3. Add message handler for message "MESSAGE1"
;~ 							4. Add "SendMsg" function inside the above message handler.
;~ 							5. "SendMsg" function should take the parameters specified in the test data.
;~ 							6.Attach the file to any node then Select "User Programs -> Build and Load all" menu option
;~ 							7. Press "Connect" toolbar button
;~ 							8. Select "Execute Message Handler" toolbar button
;~ 							9. Send a message from the other node whose message ID is equal to the ID of message "MESSAGE1" only once"

; Expected Results  : 		"1. The message sent from the other node should be displayed on the message window 
;~ 							2. Immediately after receiving the above message, the message with the ID 0x100 should be transmitted only once to the other node.
;~ 							The transmitted message should be displayed on the mesage window and on the trace window of the other node."
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_03.au3****"&@CRLF)


_launchApp()

local $MsgHandler_Tx=0,$MsgHandler_Rx=0,$MsgHandler_Trace=0
local $outtxt[200]

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxNSCAN_03")																; Load Configuration

	_AssociateCANDB("testNS_03.dbf")														; Associate DB

	_CANNodeSimConfigMenu()																	; Open CAN node Simulation window

	$TVHWD=_GetCANCppTVHWD()																; Fetch the handle of tree view
	_GUICtrlTreeView_Expand($TVHWD,"",True)													; Expand the tree view items
	sleep(1000)
		
	ControlFocus ( $WIN_BUSMASTER, "", $TVHWD )

	send("{DOWN 2}")																		; Send the down key twice to select the node
	sleep(1000)

	_AddCANCppFile("MsgHandlerNS_03")														; Add a new Cpp file

	_AddMsgHandler(4,"Message1","")															; Add Msg Name Handler

	$MsgHWD=_GetCANHandlerHWD(2,2)
	_GUICtrlTreeView_ClickItem($TVHWD, $MsgHWD)					 							; Click on Msg Handler item in the tree view

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
	ControlSend($WIN_BUSMASTER, "", $TXT_MsgHandler_CPPFile, "Trace" & "(" & Chr(34) & "Msg Handler" & Chr(34) & ");")

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
	
	_MsgHandlers()																			; Enable Msg handlers in node simulation tool bar
	sleep(500)
	
	_ConnectDisconnect()																	; Connect the tool
	Sleep(700)

	_TransmitMsgsMenu()																		; Select 'Transmit normal blocks' from menu
	sleep(500)

	_ConnectDisconnect()																	; Disconnect the tool
	Sleep(1000)

	$HWD= ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)								; Fetch the Msg Window Handle
	_GUICtrlListView_ClickItem($HWD,0)														; Click on the first item in msg window

	$MsgCount=_GetCANMsgWinItemCount()														; Fetch the Msg window msg count
	$MsgHandler_Trace=_GetTraceWinInfo(0)													; Fetch the trace window first row info

	_UnLoadALL()																			; Unload All from the menu
	
	if $MsgCount=2 Then
		$Msg_1=_GetMsgWinCANInfo(0)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_1[" & $i &"] : "&$Msg_1[$i]&@CRLF)
		next
		if $Msg_1[1]="Tx" and $Msg_1[2]=1 and $Msg_1[4]=0x100 and $Msg_1[6]=8 and $Msg_1[7]="0011000000000000" Then					; Compare the Direction, Channel and Msg ID of the first row
			$MsgHandler_Tx=1
		EndIf
		
		$Msg_2=_GetMsgWinCANInfo(1)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_2[" & $i &"] : "&$Msg_2[$i]&@CRLF)
		next
		if $Msg_2[1]="Rx" and $Msg_2[2]=1 and $Msg_2[4]=0x101 and $Msg_2[6]=8 and $Msg_2[7]="0A00000014000000" Then					; Compare the Direction, Channel and Msg ID of the first row
			$MsgHandler_Rx=1
		EndIf
	EndIf
EndIf

ConsoleWrite("$MsgCount :"&$MsgCount&@CRLF)
ConsoleWrite("$MsgHandler_Tx :"&$MsgHandler_Tx&@CRLF)
ConsoleWrite("$MsgHandler_Rx :"&$MsgHandler_Rx&@CRLF)
ConsoleWrite("$MsgHandler_Trace :"&$MsgHandler_Trace&@CRLF)

if $MsgHandler_Tx=1 and $MsgHandler_Rx=1 and $MsgHandler_Trace="Msg Handler" Then
	_WriteResult("Pass","TS_NS_03")
Else
	_WriteResult("Fail","TS_NS_03")
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_03")
EndIf


ConsoleWrite("****End : TS_NS_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)