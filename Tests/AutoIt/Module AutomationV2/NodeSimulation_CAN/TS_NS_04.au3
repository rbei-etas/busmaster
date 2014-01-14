;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_04
; TestCases			:		Message Handler - 1 Message only
; Test Data			:		cfxNSCAN_04.cfx,testNS_03.dbf,
; Test Setup		:		1.Select any node in node simulation system.
;~ 							2. Click on the Edit file on right side
;~ 							3. In message handler Add another 10 "SendMsg" functions inside the message handler.
;~ 							4 Change the ID parameter in the send message function as specified in the test data
;~ 							5. Attach the file to any node then Select "User Programs -> Build and Load all" menu option.
;~ 							6. Press connect tool bar button
;~ 							7. Select "Execute Message Handler" toolbar button
;~ 							8. Send a message from the other node whose message ID is equal to the ID of message "MESSAGE1" only once with 1 msec delay
;~ 							9. Repeat the test case with the same delay but in repeatative mode"

; Expected Results  : 		"1. The message sent from the other node should be displayed on the message window after approximately 1 msec delay.
;~ 							2. Immediately after receiving the above message, the messages with the ID 0x100, 0x101,0x102,0x103,0x104,0x105, 0x106, 0x107, 0x108, 0x109, 0x110 should be transmitted only once to the other node.The transmitted message should be displayed on the mesage window and on the trace window of the other node.
;~ 							3. After step 8 in test setup, step 1 and step 2 above, should be executed repeadedly."
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_04.au3****"&@CRLF)


_launchApp()

local $MsgHandlers=0,$MsgHandler_Tx=0,$MsgHandler_Rx101=0,$MsgHandler_Rx102=0,$MsgHandler_Rx103=0,$MsgHandler_Rx104=0,$MsgHandler_Rx105=0,$MsgHandler_Rx106=0,$MsgHandler_Rx107=0,$MsgHandler_Rx108=0,$MsgHandler_Rx109=0,$MsgHandler_Rx110=0
local $outtxt[200],$MsgHandlers_Trace=0

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxNSCAN_04")																; Load Configuration

	_CANNodeSimConfigMenu()																	; Open CAN node Simulation window

	$TVHWD=_GetCANCppTVHWD()																; Fetch the handle of tree view
	_GUICtrlTreeView_Expand($TVHWD,"",True)													; Expand the tree view items
	sleep(1000)
		
	ControlFocus ( $WIN_BUSMASTER, "", $TVHWD )

	send("{DOWN 2}")																		; Send the down key twice to select the node
	sleep(1000)

	_EditCANCppFile()																		; Clicks on the edit file button
	sleep(1000)
	
	_DelIncludeFile()																		; Deletes the database path from the include headers list
	
	_AddIncludeDBFile("testNS_03_Unions.h")													; Adds the DB union's.h to the include header list

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
	sleep(2000)

	_ConnectDisconnect()																	; Disconnect the tool
	Sleep(1000)

	$HWD= ControlGetHandle($WIN_BUSMASTER,"",$LSTC_CANMsgWin)								; Fetch the Msg Window Handle
	_GUICtrlListView_ClickItem($HWD,0)														; Click on the first item in msg window

	$MsgCount=_GetCANMsgWinItemCount()														; Fetch the Msg window msg count
	$MsgHandler_Trace101=_GetTraceWinInfo(0)												; Fetch the trace window first row info
	$MsgHandler_Trace102=_GetTraceWinInfo(1)												; Fetch the trace window second row info
	$MsgHandler_Trace103=_GetTraceWinInfo(2)												; Fetch the trace window third row info
	$MsgHandler_Trace104=_GetTraceWinInfo(3)												; Fetch the trace window fourth row info
	$MsgHandler_Trace105=_GetTraceWinInfo(4)												; Fetch the trace window fifth row info
	$MsgHandler_Trace106=_GetTraceWinInfo(5)												; Fetch the trace window sixth row info
	$MsgHandler_Trace107=_GetTraceWinInfo(6)												; Fetch the trace window seventh row info
	$MsgHandler_Trace108=_GetTraceWinInfo(7)												; Fetch the trace window eight row info
	$MsgHandler_Trace109=_GetTraceWinInfo(8)												; Fetch the trace window ninth row info
	$MsgHandler_Trace110=_GetTraceWinInfo(9)												; Fetch the trace window tenth row info
	
	_UnLoadALL()																			; Unload All from the menu
	
	if $MsgCount=11 Then
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
		if $Msg_2[1]="Rx" and $Msg_2[2]=1 and $Msg_2[4]=0x101 and $Msg_2[6]=8 and $Msg_2[7]="0A00000014000000" Then					; Compare the Direction, Channel and Msg ID of the second row
			$MsgHandler_Rx101=1
		EndIf
		
		$Msg_3=_GetMsgWinCANInfo(2)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_3[" & $i &"] : "&$Msg_3[$i]&@CRLF)
		next
		if $Msg_3[1]="Rx" and $Msg_3[2]=1 and $Msg_3[4]=0x102 and $Msg_3[6]=8 and $Msg_3[7]="0B00000015000000" Then					; Compare the Direction, Channel and Msg ID of the third row
			$MsgHandler_Rx102=1
		EndIf
		
		$Msg_4=_GetMsgWinCANInfo(3)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_4[" & $i &"] : "&$Msg_4[$i]&@CRLF)
		next
		if $Msg_4[1]="Rx" and $Msg_4[2]=1 and $Msg_4[4]=0x103 and $Msg_4[6]=8 and $Msg_4[7]="0C00000016000000" Then					; Compare the Direction, Channel and Msg ID of the fourth row
			$MsgHandler_Rx103=1
		EndIf
		
		$Msg_5=_GetMsgWinCANInfo(4)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_5[" & $i &"] : "&$Msg_5[$i]&@CRLF)
		next
		if $Msg_5[1]="Rx" and $Msg_5[2]=1 and $Msg_5[4]=0x104 and $Msg_5[6]=8 and $Msg_5[7]="0D00000017000000" Then					; Compare the Direction, Channel and Msg ID of the fifth row
			$MsgHandler_Rx104=1
		EndIf
		
		$Msg_6=_GetMsgWinCANInfo(5)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_6[" & $i &"] : "&$Msg_6[$i]&@CRLF)
		next
		if $Msg_6[1]="Rx" and $Msg_6[2]=1 and $Msg_6[4]=0x105 and $Msg_6[6]=8 and $Msg_6[7]="0E00000018000000" Then					; Compare the Direction, Channel and Msg ID of the sixth row
			$MsgHandler_Rx105=1
		EndIf
		
		$Msg_7=_GetMsgWinCANInfo(6)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_7[" & $i &"] : "&$Msg_7[$i]&@CRLF)
		next
		if $Msg_7[1]="Rx" and $Msg_7[2]=1 and $Msg_7[4]=0x106 and $Msg_7[6]=8 and $Msg_7[7]="0F00000019000000" Then					; Compare the Direction, Channel and Msg ID of the seventh row
			$MsgHandler_Rx106=1
		EndIf
		
		$Msg_8=_GetMsgWinCANInfo(7)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_8[" & $i &"] : "&$Msg_8[$i]&@CRLF)
		next
		if $Msg_8[1]="Rx" and $Msg_8[2]=1 and $Msg_8[4]=0x107 and $Msg_8[6]=8 and $Msg_8[7]="100000001A000000" Then					; Compare the Direction, Channel and Msg ID of the eight row
			$MsgHandler_Rx107=1
		EndIf
	
		$Msg_9=_GetMsgWinCANInfo(8)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_9[" & $i &"] : "&$Msg_9[$i]&@CRLF)
		next
		if $Msg_9[1]="Rx" and $Msg_9[2]=1 and $Msg_9[4]=0x108 and $Msg_9[6]=8 and $Msg_9[7]="110000001B000000" Then					; Compare the Direction, Channel and Msg ID of the ninth row
			$MsgHandler_Rx108=1
		EndIf
	
		$Msg_10=_GetMsgWinCANInfo(9)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_10[" & $i &"] : "&$Msg_10[$i]&@CRLF)
		next
		if $Msg_10[1]="Rx" and $Msg_10[2]=1 and $Msg_10[4]=0x109 and $Msg_10[6]=8 and $Msg_10[7]="120000001C000000" Then					; Compare the Direction, Channel and Msg ID of the tenth row
			$MsgHandler_Rx109=1
		EndIf
		
		$Msg_11=_GetMsgWinCANInfo(10)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_11[" & $i &"] : "&$Msg_11[$i]&@CRLF)
		next
		if $Msg_11[1]="Rx" and $Msg_11[2]=1 and $Msg_11[4]=0x110 and $Msg_11[6]=8 and $Msg_11[7]="130000001D000000" Then					; Compare the Direction, Channel and Msg ID of the eleventh row
			$MsgHandler_Rx109=1
		EndIf
	
	EndIf
	if $MsgHandler_Tx=1 and $MsgHandler_Rx101=1 and $MsgHandler_Rx102=1 and $MsgHandler_Rx103=1 and $MsgHandler_Rx104=1 and $MsgHandler_Rx105=1 and $MsgHandler_Rx106=1 and $MsgHandler_Rx107=1 and $MsgHandler_Rx108=1 and $MsgHandler_Rx109=1 Then
		$MsgHandlers=1
	EndIf
	
	if $MsgHandler_Trace101="Msg Handler 1" and $MsgHandler_Trace102="Msg Handler 2" and $MsgHandler_Trace103="Msg Handler 3" and $MsgHandler_Trace104="Msg Handler 4" and $MsgHandler_Trace105="Msg Handler 5" and $MsgHandler_Trace106="Msg Handler 6" and $MsgHandler_Trace107="Msg Handler 7" and $MsgHandler_Trace108="Msg Handler 8" and $MsgHandler_Trace109="Msg Handler 9" and $MsgHandler_Trace110="Msg Handler 10" Then
		$MsgHandlers_Trace=1
	EndIf
	

EndIf

ConsoleWrite("$MsgCount :"&$MsgCount&@CRLF)
ConsoleWrite("$MsgHandler_Tx :"&$MsgHandler_Tx&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx101&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx102&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx103&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx104&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx105&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx106&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx107&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx108&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx109&@CRLF)
ConsoleWrite("$MsgHandler_Rx101 :"&$MsgHandler_Rx110&@CRLF)
ConsoleWrite("$MsgHandlers_Trace :"&$MsgHandlers_Trace&@CRLF)

if $MsgHandlers=1 and $MsgHandlers_Trace=1 Then
	_WriteResult("Pass","TS_NS_04")
Else
	_WriteResult("Fail","TS_NS_04")
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_04")
EndIf


ConsoleWrite("****End : TS_NS_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)