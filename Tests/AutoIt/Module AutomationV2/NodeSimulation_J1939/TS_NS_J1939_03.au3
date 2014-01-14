;===============================Test Cases/Test Data ========================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_03
; TestCases			:		Performance of time
; Test Data			:		cfxNSJ1939_01.cfx
; Test Setup		:		1.Open a file.
;                          2.Add  timer handler with time interval 3000 ms.
;                          3.For each handler write the following code with MSG ID changed:  STCAN_MSG sMsg;
;                           sMsg.m_unMsgID = 0x100;                     // Message ID
;                           sMsg.m_ucEXTENDED = FALSE;           // Standard Message type
;                           sMsg.m_ucRTR = FALSE;                      // Not RTR type
;                           sMsg.m_ucDLC = 8;                              // Length is 8 Bytes
;                           sMsg.m_sWhichBit.m_aulData[0] = 10;   // Lower 4 Bytes
;                           sMsg.m_sWhichBit.m_aulData[1] = 20;   // Upper 4 Bytes
;                           sMsg.m_ucChannel = 1;                         // First CAN channel
;                           SendMsg(sMsg);// Send the message
;                          4. Now attach this file to a node
;                          5.build and load
;                          6. Connect the tool
;                          7. Enable all timer handler
;                          8. After some time stop the timer.
;                          9. Observe the message window

; Expected Results  : 		The time of sending messages should be within 3000 Microseconds of the time assigned to them.ie..
;
;================================Test Procedure =======================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_03.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)
local $Time=0,$CANIdInfo=0,$SrcInfo=0,$ChannelInfo=0
if winexists($WIN_BUSMASTER) then
     _loadConfig("cfxNSJ1939_03.cfx")
		 sleep(1000)
         _ActivateJ1939()                                                                  ;Activate J1939 Message window
		sleep(1000)

	_J1939NodeSimConfigMenu()
    _NewSimSysFile("TestNSJ1939_03")

	_AddNodeJ1939("TestNodeJ1939Timer","50","")
	_AddJ1939CppFile("TimeHandlerNSJ1939_03")
	_AddJ1939TimeHandler("Timer","3000")
	$TVHWD=_GetJ1939CppTVHWD()																; Fetch the handle of tree view
	$TimeHWD=_GetJ1939HandlerHWD(2,3)
	_GUICtrlTreeView_ClickItem($TVHWD, $TimeHWD)					 						; Right click on Time Handler item in the tree view

	send("{DOWN}")																			; Send the down key to select the Msg handler function

	send("{ENTER}")
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "STJ1939_MSG msg;" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_byChannel = 1;              // Message ID" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_eType = MSG_TYPE_DATA;          // Standard Message type" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_eDirection = DIR_TX;              // Direction of the message" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress = 50;// source address" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_unDLC = 5;                                            // DLC" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_pbyData = new BYTE[15];" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "BYTE tmp[] = " & "{{}" & "12, 34, 45, 56, 78" & "{}}" & ";" & @CRLF )
	;Send("{LSHIFT}")
	;Send("{{}")
	;ControlSend($WIN_BUSMASTER,"",$TXT_TimeHandler_CPPFile,"12, 34, 45, 56, 78    //Data bytes" & @CRLF)

	sleep(500)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "memcpy(msg.m_pbyData, tmp, 5);"   & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "SendMsg(&msg);" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace" & "(" & Chr(34) & "Time Handler" & Chr(34) & ");")

	Send("^s")																				; Save the cpp file

	Send("^{F4}") 																			; Close the C file window
	sleep(500)

	;Send("^{F4}") 																			; Close the Node Simulation window
	sleep(500)
	_Click_BuildLoad()
	WinWaitActive("",$Msg_BuildLoadError,5)
	if winexists("",$Msg_BuildLoadError) Then

		_BuildLoadError()																	; Close the pop up msg
		$OutputwinHWD=ControlGetHandle($WIN_BUSMASTER,"",$LSTB_Details_TraceWin)			; get the handle of output window
		$itemCount=_GUICtrlListBox_GetCount($OutputwinHWD)									; Get the output window item count
		for $i=0 to $itemCount-1
			$outtxt[$i]=_GUICtrlListBox_GetText($OutputwinHWD, $i)							; Fetch the text in the output window
			ConsoleWrite("$outtxt["&$i&"]:"&$outtxt[$i]&@CRLF)
		Next
																			; Close the output window
	EndIf
     send("^{F4}")
	_SelectHandlerJ1939(1)
	 Click_EnableHandler()
	 send("^{F4}")
	 send("^{F4}")
	 _ConnectDisconnect()																	; Connect the tool
	Sleep(9000)

																			; Close the output window
	 _ConnectDisconnect()
	 sleep(500)
	 _DisableOverwriteMode()
	_UnLoadALLJ1939()

;~ 	$TimeHandler_Trace1=_GetTraceWinInfo(0)													; Fetch the trace window first row info
;~ 	$TimeHandler_Trace2=_GetTraceWinInfo(1)													; Fetch the trace window second row info
;~ 	$TimeHandler_Trace3=_GetTraceWinInfo(2)													; Fetch the trace window third row info
;~

;~ 	ConsoleWrite("TRACE WINDOW" & $TimeHandler_Trace1 & @CRLF)
	$handle=ControlGetHandle($WIN_BUSMASTER,"",200)
	_GUICtrlListView_ClickItem($handle,0)
	sleep(1000)
	$rCount=_GetJ1939MsgWinItemCount()
	If($rCount>=6) Then

	    $Data1=_GetJ1939MsgWinInfo(0)                                                              ;Fetch the column entries of message window
	    $Data2=_GetJ1939MsgWinInfo(1)
	    $Data3=_GetJ1939MsgWinInfo(2)
	    $Data4=_GetJ1939MsgWinInfo(3)
	    $Data5=_GetJ1939MsgWinInfo(4)
	    $Data6=_GetJ1939MsgWinInfo(5)

        for $i=0 to 11
		     ConsoleWrite("Data1 " &$Data1[$i] & @CRLF)
		     ConsoleWrite("Data2 " &$Data2[$i] & @CRLF)
		     ConsoleWrite("Data3 " &$Data3[$i] & @CRLF)
		     ConsoleWrite("Data4 " &$Data4[$i] & @CRLF)
		     ConsoleWrite("Data5 " &$Data5[$i] & @CRLF)
		Next


	     $FirstMsgTime=StringSplit($Data1[0],":")                                                          ;Split time as hours minutes and seconds
	     $ThirdMsgTime=StringSplit($Data3[0],":")



	     If ($ThirdMsgTime[3]>$FirstMsgTime[3]) Then                                                       ;Compare first message time and fifth message time in seconds to get time difference or delay
		     $timeDiff=$ThirdMsgTime[3]-$FirstMsgTime[3]
	    ElseIf($ThirdMsgTime[3]<$FirstMsgTime[3]) Then
		    If ($ThirdMsgTime[3]=00 And $FirstMsgTime[3]=58) Then
			     $timeDiff=3
		    ElseIf($ThirdMsgTime[3]=01 And $FirstMsgTime[3]=59) Then
			     $timeDiff=3
		    EndIf
	    EndIf

		ConsoleWrite("$timeDiff " & $timeDiff & @CRLF)


	    If $timeDiff=3 Then                                                                                ;Verify for time difference or delay
			$Time=1
	    Else
		    $Time=0
	    EndIf

		If($Data1[2]="0x050" And $Data3[2]="0x050" And  $Data5[2]="0x050") Then                          ;Compare CAN ID
			$CANIdInfo=1
	     Else
		    $CANIdInfo=0
	    EndIf
        If($Data1[6]=50 And $Data3[6]=50 And $Data5[6]=50 ) Then                                          ;Compare Source Address
			$SrcInfo=1
	    Else
		    $SrcInfo=0
	    EndIf
	    If($Data1[9]="Rx" And $Data2[9]="Rx" And $Data3[9]="Rx" And $Data4[9]="Rx") Then                  ;Compare Direction
			$ChannelInfo=1
	    Else
		    $ChannelInfo=0
	    EndIf
    EndIf


	If($Time=1 And $CANIdInfo=1 And $SrcInfo=1 And $ChannelInfo=1) Then
		_WriteResult("Pass","TS_J1939Node_03")
    Else
	   _WriteResult("Fail","TS_J1939Node_03")
	EndIf
EndIf
$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Node_03")
EndIf


ConsoleWrite("****End : TS_J1939Node_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)




