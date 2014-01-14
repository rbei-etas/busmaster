;===============================Test Cases/Test Data ===================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_10
; TestCases			:		Key Handler
; Test Data			:		cfxNSJ1939_10.cfx
; Test Setup		:		 1. Set the baud rate of both the nodes to 1000kbps
;                             2. open new function editor of a node.
;                             3. Add key handler for key "a"
;                             4. Add  "SendMsg" function inside the above key handler.
;                             5. "SendMsg" function should take the parameters specified in the test data.
;                             6. Attach the file to any node then Select "User Programs -> Build and Load all" menu option
;                             7. Press "Connect" toolbar button and start the target node in receive mode
;                             8. Select "Execute Key Handler" toolbar button
;                             9. Press 'a key

; Expected Results  : 		"The message that is programmed to send should be transmitted to the other node only once.
 ;                          the transmitted message should be displayed on the message window.
;
;================================Test Procedure ===============================================================
ConsoleWrite(@CRLF)

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_10.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)
if winexists($WIN_BUSMASTER) then
    local $CANIdInfo=0,$ChannelInfo=0,$DLC=0,$DataBytes=0
	_loadConfig("cfxNSJ1939_10.cfx")
	sleep(1000)
	_ActivateJ1939()                                                                                                                      ;Activate J1939 Message window
	sleep(1000)
	local $KeyHandler_A=0,$KeyHandler_Trace=0
    local $outtxt[200]
	_J1939NodeSimConfigMenu()																; Open J1939 node Simulation window
	$TVHWD=_GetJ1939CppTVHWD()																; Fetch the handle of tree view
	_GUICtrlTreeView_Expand($TVHWD,"",True)													; Expand the tree view items
	sleep(1000)
	ControlFocus ( $WIN_BUSMASTER, "", $TVHWD )
	send("{DOWN 2}")																		; Send the down key twice to select the node
	sleep(1000)
 	_AddJ1939CppFile("KeyHandlerNS_J1939_10.cpp")														; Add a new Cpp file
 	_AddJ1939KeyHandler("a","BUSMASTER - KeyHandlerNS_J1939_10.cpp")																	; Add key handler for key "a"
	sleep(1000)
	$KeyHWD=_GetJ1939HandlerHWD(2,4)
	_GUICtrlTreeView_ClickItem($TVHWD, $KeyHWD)					 							; Right click on Key Handler item in the tree view
	send("{DOWN}")																			; Send the down key to select the key handler function
	send("{ENTER}")
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "STJ1939_MSG msg;" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_byChannel = 1;              // Message ID" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_eType = MSG_TYPE_DATA;          // Standard Message type" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_eDirection = DIR_TX;              // Direction of the message" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress = 50;// source address" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_unDLC = 5;                                            // DLC" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_pbyData = new BYTE[15];" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "BYTE tmp[] = " & "{{}" & "12, 34, 45, 56, 78" & "{}}" & ";" & @CRLF )


	sleep(500)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "memcpy(msg.m_pbyData, tmp, 5);"   & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "SendMsg(&msg);" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace" & "(" & Chr(34) & "Key Handler" & Chr(34) & ");")

	Send("^s")																				; Save the cpp file

	Send("^{F4}") 																			; Close the C file window

	_Click_BuildLoad()
	WinWaitActive("",$Msg_BuildLoadError,5)
	if winexists("",$Msg_BuildLoadError) Then
		_BuildLoadError()																				; Close the pop up msg
		$OutputwinHWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTB_Details_TraceWin)			; get the handle of output window
		$itemCount=_GUICtrlListBox_GetCount($OutputwinHWD)									; Get the output window item count
		for $i=0 to $itemCount-1
			$outtxt[$i]=_GUICtrlListBox_GetText($OutputwinHWD, $i)							; Fetch the text in the output window
			ConsoleWrite("$outtxt["&$i&"]:"&$outtxt[$i]&@CRLF)
		Next
		send("^{F4}")																		; Close the output window
		send("^{F4}")																		; Close Simulated system window
	EndIf

	_ConnectDisconnect()																; Connect the tool
	Sleep(700)
	send("^{F4}")
	_EnableAllKeyHandlersforJ1939()											; select and Enable Key Handler
	send("^{F4}")
	send("a")																			; Invoke key handler
	sleep(2000)
	_ConnectDisconnect()
	 sleep(500)
	 _DisableOverwriteMode()
	_UnLoadALLJ1939()
	$handle=ControlGetHandle($WIN_BUSMASTER,"",200)                                         ;Click on J1939 message window
	_GUICtrlListView_ClickItem($handle,0)
	sleep(1000)
	$rCount=_GetJ1939MsgWinItemCount()                                                       ;Get row count from J1939 message window
	If $rCount>=2 Then
		$Data1=_GetJ1939MsgWinInfo(0)                                                            ;Fetch the column entries of message window
	    $Data2=_GetJ1939MsgWinInfo(1)

	    for $i=0 to 11
		     ConsoleWrite("Data1 " &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data2 " &$Data2[$i] & @CRLF)
	    Next
        If($Data1[2]="0x020" And $Data2[2]="0x020") Then                          ;Compare CAN ID
			$CANIdInfo=1
	     Else
		    $CANIdInfo=0
	    EndIf
	    If($Data1[9]="Rx" And $Data2[9]="Rx") Then                  ;Compare Direction
			$ChannelInfo=1
	    Else
		    $ChannelInfo=0
	    EndIf
	    If($Data1[10]=5 And $Data2[10]=5) Then
			$DLC=1
		Else
		    $DLC=0
	    EndIf
        If($Data1[11]="0C222D384E" And $Data2[11]="0C222D384E") Then
			$DataBytes=1
	     Else
			$DataBytes=0
		EndIf
	EndIf


	 ConsoleWrite("$CANIdInfo" & $CANIdInfo & @CRLF)
	 ConsoleWrite("$ChannelInfo" & $ChannelInfo & @CRLF)
	  ConsoleWrite("$DLC" & $DLC & @CRLF)
	 ConsoleWrite("$DataBytes" & $DataBytes & @CRLF)

	If($CANIdInfo=1 And $ChannelInfo=1 And $DLC=1 And $DataBytes=1) Then
		_WriteResult("Pass","TS_J1939Node_10")
    Else
	   _WriteResult("Fail","TS_J1939Node_10")
	EndIf
EndIf
$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Node_10")
EndIf


ConsoleWrite("****End : TS_J1939Node_10.au3****"&@CRLF)
ConsoleWrite(@CRLF)





