;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_01
; TestCases			:		Execute Key handler
; Test Data			:		cfxNSCAN_01.cfx
; Test Setup		:		1.Open a file. 
;~ 							2.Add key handler for the key 'a'  as:STCAN_MSG sMsg;

;~ 							// Initialise message structure

;~ 							sMsg.m_unMsgID = 0x100;                     // Message ID
;~ 							sMsg.m_ucEXTENDED = FALSE;           // Standard Message type
;~ 							sMsg.m_ucRTR = FALSE;                      // Not RTR type
;~ 							sMsg.m_ucDLC = 8;                              // Length is 8 Bytes
;~ 							sMsg.m_sWhichBit.m_aulData[0] = 10;   // Lower 4 Bytes
;~ 							sMsg.m_sWhichBit.m_aulData[1] = 20;   // Upper 4 Bytes
;~ 							sMsg.m_ucChannel = 1;                         // First CAN channel
;~ 							// Send the message
;~ 							SendMsg(sMsg);
;~ 							3.Attach the file to any node 
;~ 							4.Build and load the DLL
;~ 							5.Enable key handlers
;~ 							6.Set the message window as active window
;~ 							7.Press key 'a' 

; Expected Results  : 		When key  'a' is pressed first time a message of message id 100 will send from the application. This message TX should be displayed on the message window.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_01.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)


local $KeyHandler_A=0,$KeyHandler_Trace=0
local $outtxt[200]
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxNSCAN_01")																; Load Configuration

	_CANNodeSimConfigMenu()																	; Open CAN node Simulation window

	$TVHWD=_GetCANCppTVHWD()																; Fetch the handle of tree view
	_GUICtrlTreeView_Expand($TVHWD,"",True)													; Expand the tree view items
	sleep(1000)
	
	ControlFocus ( $WIN_BUSMASTER, "", $TVHWD )

	send("{DOWN 2}")																		; Send the down key twice to select the node
	sleep(1000)

	_AddCANCppFile("KeyHandlerNS_01")														; Add a new Cpp file

	_AddCANKeyHandler("a")																	; Add key handler for key "a"

	$KeyHWD=_GetCANHandlerHWD(2,4)
	_GUICtrlTreeView_ClickItem($TVHWD, $KeyHWD)					 							; Right click on Key Handler item in the tree view

	send("{DOWN}")																			; Send the down key to select the key handler function

	send("{ENTER}")

	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "STCAN_MSG sMsg;" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "sMsg.m_unMsgID = 0x55;              // Message ID" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "sMsg.m_ucEXTENDED = FALSE;           // Standard Message type" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "sMsg.m_ucRTR = FALSE;                // Not RTR type" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "sMsg.m_ucDLC = 8;                    // Length is 8 Bytes" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "sMsg.m_sWhichBit.m_aulData[0] = 10;  // Lower 4 Bytes" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "sMsg.m_sWhichBit.m_aulData[1] = 20;  // Upper 4 Bytes" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "sMsg.m_ucChannel = 1;                // First CAN channel" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "SendMsg(sMsg);" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_KeyHandler_CPPFile, "Trace" & "(" & Chr(34) & "Key Handler" & Chr(34) & ");")

	Send("^s")																				; Save the cpp file

	Send("^{F4}") 																			; Close the C file window

	_Click_BuildLoad()																		; Click on 'Build and Load' button

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
		send("^{F4}")																		; Close Simulated system window

	Else

		_ConnectDisconnect()																; Connect the tool
		Sleep(700)

		_SelectHandler($KeyHandler_Index)													; select and Enable Key Handler

		send("a")																			; Invoke key handler

		_ConnectDisconnect()																; Disconnect the tool
		Sleep(1000)

		_Click_Unload()																		; Click on unload button

		send("^{F4}")																		; Close output window
		sleep(1000)

		send("^{F4}")																		; Close the simulated system window
		sleep(1000)


		_DisableOverwriteMode()																; Disable overwrite mode

		_EnableHex()																		; Enable Hex display mode

	EndIf

	$HWD= ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)								; Fetch the Msg Window Handle
	_GUICtrlListView_ClickItem($HWD,0)														; Click on the first item in msg window

	$MsgCount=_GetCANMsgWinItemCount()														; Fetch the Msg window msg count
	$KeyHandler_Trace=_GetTraceWinInfo(3)

	if $MsgCount=1 Then
		$Msg_1=_GetMsgWinCANInfo(0)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_1[" & $i &"] : "&$Msg_1[$i]&@CRLF)
		next
		if $Msg_1[1]="Rx" and $Msg_1[2]=1 and $Msg_1[4]=0x055 and $Msg_1[6]=8 and $Msg_1[7]="0A00000014000000" Then					; Compare the Direction, Channel and Msg ID of the first row
			$KeyHandler_A=1
		EndIf
	EndIf
	ConsoleWrite("$MsgCount :"&$MsgCount&@CRLF)
	ConsoleWrite("$KeyHandler_A :"&$KeyHandler_A&@CRLF)
	ConsoleWrite("$KeyHandler_Trace :"&$KeyHandler_Trace&@CRLF)

EndIf

if $KeyHandler_A=1 and $KeyHandler_Trace="Key Handler" Then									; Validate the result
	_WriteResult("Pass","TS_NS_01")
Else
	_WriteResult("Fail","TS_NS_01")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_01")
EndIf


ConsoleWrite("****End : TS_NS_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)