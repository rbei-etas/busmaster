;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_02
; TestCases			:		Execute Key handler
; Test Data			:		cfxNSCAN_02.cfx,KeyHandlerNS_02_01.cpp,KeyHandlerNS_02_02.cpp,KeyHandlerNS_02_03.cpp
; Test Setup		:		1.Open a file.
;~ 							2.Add key handler for the key 'a'  as:STCAN_MSG sMsg;
;~ 							3.Press BLA
;~ 							4.If all the DLLs are build properly enable Key handlers for more than one node.
;~ 							5.Make the message window active.
;~ 							6.Press key a

; Expected Results  : 		When key  'a' is pressed first time a message of message id 100 will send from the application. This message TX should be displayed on the message window.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_02.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)


local $KeyHandler_1=0,$KeyHandler_2=0,$KeyHandler_3=0,$KeyHandler_Trace=0,$KeyHandler_Trace=0
local $outtxt[200]
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxNSCAN_02.cfx")															; Load Configuration

	$NodeSimToolbar=_CANNodeSimToolbarActive()												; Check whether CAN node simulation toolbar is active
	ConsoleWrite("$NodeSimToolbar :"&$NodeSimToolbar&@CRLF)

	if $NodeSimToolbar=0 Then
		send("!vtcn")																		; Select View->Toolbar->CAN->Node Simulation menu
	EndIf

	_BuildLoadALL()																			; Click on 'Build and Load All' icon
	sleep(8000)

	_AllKeyHandlers()																		; Click on 'Enable/Disable All key handlers' icon
	sleep(1500)

	_ConnectDisconnect()																	; Connect the tool
	sleep(700)

	send("a")																				; Invoke key handler
	sleep(1500)

	_DisableOverwriteMode()																	; Disable overwrite mode

	_ConnectDisconnect()																	; Disconnect the tool
	sleep(700)

	_UnLoadALL()																			; Click on 'Unload All' icon

	send("^{F4}")																			; Close output window
	sleep(1000)

	$MsgCount=_GetCANMsgWinItemCount()														; Fetch the Msg window msg count
	$KeyHandler_Trace1=_GetTraceWinInfo(0)													; Fetch the trace window first row info
	$KeyHandler_Trace2=_GetTraceWinInfo(1)													; Fetch the trace window second row info
	$KeyHandler_Trace3=_GetTraceWinInfo(2)													; Fetch the trace window third row info

	if $MsgCount=3 Then
		$Msg_1=_GetMsgWinCANInfo(0)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_1[" & $i &"] : "&$Msg_1[$i]&@CRLF)
		next
		if $Msg_1[1]="Rx" and $Msg_1[2]=1 and $Msg_1[4]=0x055 and $Msg_1[6]=8 and $Msg_1[7]="0A00000014000000" Then					; Compare the Direction, Channel and Msg ID of the first row
			$KeyHandler_1=1
		EndIf

		$Msg_2=_GetMsgWinCANInfo(1)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_2[" & $i &"] : "&$Msg_2[$i]&@CRLF)
		next
		if $Msg_2[1]="Rx" and $Msg_2[2]=1 and $Msg_2[4]=0x056 and $Msg_2[6]=8 and $Msg_2[7]="0B00000016000000" Then					; Compare the Direction, Channel and Msg ID of the first row
			$KeyHandler_2=1
		EndIf

		$Msg_3=_GetMsgWinCANInfo(2)															; Fetch the first row data in the msg window
		for $i=0 to 7
			ConsoleWrite("$Msg_3[" & $i &"] : "&$Msg_3[$i]&@CRLF)
		next
		if $Msg_3[1]="Rx" and $Msg_3[2]=1 and $Msg_3[4]=0x057 and $Msg_3[6]=8 and $Msg_3[7]="0D00000017000000" Then					; Compare the Direction, Channel and Msg ID of the first row
			$KeyHandler_3=1
		EndIf

	EndIf

	if $KeyHandler_Trace1="Key Handler Node1" and $KeyHandler_Trace2="Key Handler Node2" and $KeyHandler_Trace3="Key Handler Node3" Then
		$KeyHandler_Trace=1
	EndIf

	ConsoleWrite("$MsgCount :"&$MsgCount&@CRLF)
	ConsoleWrite("$KeyHandler_1 :"&$KeyHandler_1&@CRLF)
	ConsoleWrite("$KeyHandler_2 :"&$KeyHandler_2&@CRLF)
	ConsoleWrite("$KeyHandler_3 :"&$KeyHandler_3&@CRLF)
	ConsoleWrite("$KeyHandler_Trace1 :"&$KeyHandler_Trace1&@CRLF)
	ConsoleWrite("$KeyHandler_Trace2 :"&$KeyHandler_Trace2&@CRLF)
	ConsoleWrite("$KeyHandler_Trace3 :"&$KeyHandler_Trace3&@CRLF)

EndIf

if $KeyHandler_1=1 and $KeyHandler_2=1 and $KeyHandler_3=1 and $KeyHandler_Trace=1 Then			; Validate the result
	_WriteResult("Pass","TS_NS_02")
Else
	_WriteResult("Fail","TS_NS_02")
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NS_02")
EndIf


ConsoleWrite("****End : TS_NS_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)