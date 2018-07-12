;==================================================================Test Cases/Test Data ===========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_012
; TestCases			:		Adding a message handler for the range of message Ids
; Test Data			:		cfxNSJ1939_12.cfx
; Test Setup		:		"A defined message sent across the J1939 Bus. Execute the message Handler"

; Expected Results  : 		"The generic message handler message should be displayed for all the range of messageswhose ID falls within the range transmitted over the J1939 Bus."
;
;=================================================================Test Procedure ====================================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_12.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)
Local $SrcAddr=0,$DestAddr=0,$Dlc=0,$DataBytes=0,$msgType=0,$TxRx=0
if winexists($WIN_BUSMASTER) then

    _loadConfig("cfxNSJ1939_12.cfx")

	_EnableHex()

	_ActivateJ1939()                                                                  ;Activate J1939 Message window

	_J1939NodeSimConfigMenu()
	$TVHWD1=_GetJ1939CppTVHWD()	                                                           ; Fetch the handle of tree view

	_GUICtrlTreeView_Expand($TVHWD1,"",True)													; Expand the tree view items
	sleep(1000)
	_ClickNode(0)

	_AddJ1939CppFile("MsgHandlerNS_12_J1939.cpp")

	_AddMsgHandler(2,0,401)

	$TVHWD=_GetJ1939CppTVHWD()
	$MsgHWD=_GetJ1939HandlerHWD(2,2)
	_GUICtrlTreeView_ClickItem($TVHWD, $MsgHWD)					 							; Click on Msg Handler item in the tree view
	sleep(700)

	send("{DOWN}")																			; Send the down key to select the Msg handler function

	send("{ENTER}")
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "STJ1939_MSG msg;" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_byChannel = 1;              // Message ID" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_eType = MSG_TYPE_DATA;          // Standard Message type" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_eDirection = DIR_TX;              // Direction of the message" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress = 50;// source address" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile,"msg.m_sMsgProperties.m_uExtendedID.m_unExtID =0x100; // PGN number" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_unDLC = 5;                                            // DLC" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "msg.m_pbyData = new BYTE[15];" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "BYTE tmp[] = " & "{{}" & "12, 34, 45, 56, 78" & "{}}" & ";" & @CRLF )


	sleep(500)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "memcpy(msg.m_pbyData, tmp, 5);"   & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "SendMsg(&msg);" & @CRLF)
	ControlSend($WIN_BUSMASTER, "", $TXT_TimeHandler_CPPFile, "Trace" & "(" & Chr(34) & "Message Handler" & Chr(34) & ");")

	Send("^s")																				; Save the cpp file

	Send("^{F4}") 																			; Close the C file window
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

	send("^{F4}")
	_EnableAllMsgHandlersJ1939()
	send("^{F4}")
	send("^{F4}")
	_ConnectDisconnect()																	; Connect the tool
	sleep(3000)

	_ClearMsgWindow()

	_J1939tMsgWin("Data","[0x0]Msg_Rqst",0,50)

	_ConnectDisconnect()

	_DisableOverwriteMode()

	_UnLoadALLJ1939()

	$handle=ControlGetHandle($WIN_BUSMASTER,"",200)
	_GUICtrlListView_ClickItem($handle,0)
	sleep(1000)

	$rCount=_GetJ1939MsgWinItemCount()
	ConsoleWrite("$rCount : "&$rCount&@CRLF)

	If $rCount>=6 Then

	    $Data1=_GetJ1939MsgWinInfo(0)                                                              ;Fetch the column entries of message window
	    $Data2=_GetJ1939MsgWinInfo(1)
	    $Data3=_GetJ1939MsgWinInfo(2)
	    $Data4=_GetJ1939MsgWinInfo(3)
	    $Data5=_GetJ1939MsgWinInfo(4)
	    $Data6=_GetJ1939MsgWinInfo(5)


        for $i=0 to 11
			ConsoleWrite("Data1 : " &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data2 : " &$Data2[$i] & @CRLF)
		    ConsoleWrite("Data3 : " &$Data3[$i] & @CRLF)
		    ConsoleWrite("Data4 : " &$Data4[$i] & @CRLF)
		    ConsoleWrite("Data5 : " &$Data5[$i] & @CRLF)
		Next

        If($Data1[6]=00 And $Data3[6]=50 And $Data5[6]=50) Then
		   $SrcAddr=1
	    Else
		   $SrcAddr=0
	    EndIf
	    If($Data1[7]=50 And $Data3[7]=01 And $Data5[7]=01) Then
			$DestAddr=1
	    Else
		    $DestAddr=0
	    EndIf

	    If($Data1[9]="Tx" And $Data3[9]="Rx" And $Data5[9]="Rx") Then
		    $TxRx=1
	    Else
		    $TxRx=0
        EndIf

		ConsoleWrite("$Data3[9] : " & $Data3[9] & @CRLF)
		ConsoleWrite("$Data5[9] : " & $Data5[9] & @CRLF)

        If($Data1[10]=8 And $Data3[10]=5 And $Data5[10]=5) Then
			$Dlc=1
	     Else
		    $Dlc=0
        EndIf
	    If($Data1[11]="1000000000000000" And $Data3[11]="0C222D384E" And $Data5[11]="0C222D384E") Then
		    $DataBytes=1
        Else
		    $DataBytes=0
	    EndIf
        If($Data1[5]="DATA" And $Data3[5]="DATA" And $Data5[5]="DATA") Then
			$msgType=1
		Else
		    $msgType=0
	    EndIf
    EndIf

	_DeactJ1939()																			; Deactivate J1939
EndIf

ConsoleWrite("$SrcAddr : " & $SrcAddr & @CRLF)
ConsoleWrite("$DestAddr : " & $DestAddr & @CRLF)
ConsoleWrite("$TxRx : " & $TxRx & @CRLF)
ConsoleWrite("$Dlc : " & $Dlc & @CRLF)
ConsoleWrite("$DataBytes : " & $DataBytes & @CRLF)
ConsoleWrite("$msgType : " & $msgType & @CRLF)

If($SrcAddr=1 And $DestAddr=1 And $TxRx=1 And $Dlc=1 And $DataBytes=1 And $msgType=1) Then
	_WriteResult("Pass","TS_J1939Node_12")
Else
	_WriteResult("Fail","TS_J1939Node_12")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Node_12")
EndIf


ConsoleWrite("****End : TS_NS_J1939_12.au3****"&@CRLF)
ConsoleWrite(@CRLF)
