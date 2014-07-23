;============================================================Test Cases/Test Data ==================================================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NS_J1939_08
; TestCases			:		Message Handler - Multiple messages  -  1msec delay
; Test Data			:		cfxNSJ1939_08.cfx
; Test Setup		:		1. Add a msg handler
;                           2. Add another 10 "SendMsg" functions inside the message handler.
;                           3. Change the ID parameter in the send message function as specified in the test data
;                           4. Attach the file to any node then Select "User Programs -> Build and Load all" menu option.
;                           5. Press connect tool bar button
;                           6. Select "Execute Message Handler" toolbar button
;                           7. Send a message from the other node whose message ID is equal to the ID of message "MESSAGE1" only once with 1 msec delay
;                          8. Repeat the test case with the same delay but in repeatative mode"

; Expected Results  : 		"1. The message sent from the other node should be displayed on the message window after approximately 1 msec delay.
;                           2. Immediately after receiving the above message, the messages with the ID 0x100, 0x101,0x102,0x103,0x104,0x105, 0x106, 0x107, 0x108, 0x109, 0x110 should be transmitted only once to the other node.
;                           The transmitted message should be displayed on the mesage window and on the trace window of the other node.
;                           3. After step 8 in test setup, step 1 and step 2 above, should be executed repeadedly."
;
;===============================================================Test Procedure ===================================================================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NS_J1939_08.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER)
local $CANid=0,$DataAll=0,$Src=0,$Dest=0,$DataBytes=0
if winexists($WIN_BUSMASTER) then

    _loadConfig("cfxNSJ1939_08.cfx")

	_ActivateJ1939()                                                                  		; Activate J1939 Message window

	_J1939NodeSimConfigMenu()

	$TVHWD1=_GetJ1939CppTVHWD()	                                                           	; Fetch the handle of tree view
	_GUICtrlTreeView_Expand($TVHWD1,"",True)												; Expand the tree view items
	sleep(1000)

	_ClickNode(0)
	sleep(1000)

	_EditCANCppFile()																		; Clicks on the edit file button
	sleep(1000)

	_DelIncludeFileJ1939()																	; Deletes the database path from the include headers list

	_AddIncludeDBFile("J1939DB_Unions.h")

	Send("^s")																				; Save the cpp file

	Send("^{F4}") 																			; Close the C file window
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
		send("^{F4}")																		; Close the output window
	EndIf

	send("^{F4}")
	_EnableAllMsgHandlersJ1939()
	sleep(500)

	send("^{F4}")
	send("^{F4}")

	_ConnectDisconnect()																	; Connect the tool

	_J1939tMsgWin("Data","[0x400]Msg_Data",0,30)

	_ConnectDisconnect()

	_DisableOverwriteMode()

	_UnLoadALLJ1939()

	$handle=ControlGetHandle($WIN_BUSMASTER,"",200)
	_GUICtrlListView_ClickItem($handle,0)
	sleep(1000)

	$rCount=_GetJ1939MsgWinItemCount()
	ConsoleWrite("$rCount:"&$rCount&@CRLF)

	if $rCount>=22 Then
		$Data1=_GetJ1939MsgWinInfo(8)                                                              ; Fetch the column entries of message window
	    $Data2=_GetJ1939MsgWinInfo(9)
	    $Data3=_GetJ1939MsgWinInfo(10)
	    $Data4=_GetJ1939MsgWinInfo(11)
	    $Data5=_GetJ1939MsgWinInfo(12)
	    $Data6=_GetJ1939MsgWinInfo(13)
	    $Data7=_GetJ1939MsgWinInfo(14)
	    $Data8=_GetJ1939MsgWinInfo(15)
	    $Data9=_GetJ1939MsgWinInfo(16)
	    $Data10=_GetJ1939MsgWinInfo(17)
	    $Data11=_GetJ1939MsgWinInfo(18)
	    $Data12=_GetJ1939MsgWinInfo(19)
	    $Data13=_GetJ1939MsgWinInfo(20)
	    $Data14=_GetJ1939MsgWinInfo(21)

		_GUICtrlListView_ClickItem($handle, 21)

	    $Data15=_GetJ1939MsgWinInfo(22)
	    $Data16=_GetJ1939MsgWinInfo(23)
	    $Data17=_GetJ1939MsgWinInfo(24)
		$Data18=_GetJ1939MsgWinInfo(25)
	    $Data19=_GetJ1939MsgWinInfo(26)
	    $Data20=_GetJ1939MsgWinInfo(27)
	    $Data21=_GetJ1939MsgWinInfo(28)
		$Data22=_GetJ1939MsgWinInfo(29)

        for $i=0 to 11
		    ConsoleWrite("Data1 : " &$Data1[$i] & @CRLF)
		    ConsoleWrite("Data2 : " &$Data2[$i] & @CRLF)
		    ConsoleWrite("Data3 : " &$Data3[$i] & @CRLF)
		    ConsoleWrite("Data4 : " &$Data4[$i] & @CRLF)
		    ConsoleWrite("Data5 : " &$Data5[$i] & @CRLF)
			ConsoleWrite("Data6 : " &$Data6[$i] & @CRLF)
			ConsoleWrite("Data7 : " &$Data7[$i] & @CRLF)
			ConsoleWrite("Data8 : " &$Data8[$i] & @CRLF)
			ConsoleWrite("Data9 : " &$Data9[$i] & @CRLF)
			ConsoleWrite("Data10 : " &$Data10[$i] & @CRLF)
			ConsoleWrite("Data11 : " &$Data11[$i] & @CRLF)
			ConsoleWrite("Data12 : " &$Data12[$i] & @CRLF)
			ConsoleWrite("Data13 : " &$Data13[$i] & @CRLF)
			ConsoleWrite("Data14 : " &$Data14[$i] & @CRLF)
			ConsoleWrite("Data15 : " &$Data15[$i] & @CRLF)
			ConsoleWrite("Data16 : " &$Data16[$i] & @CRLF)
			ConsoleWrite("Data17 : " &$Data17[$i] & @CRLF)
			ConsoleWrite("Data18 : " &$Data18[$i] & @CRLF)
			ConsoleWrite("Data19 : " &$Data19[$i] & @CRLF)
			ConsoleWrite("Data20 : " &$Data20[$i] & @CRLF)
			ConsoleWrite("Data21 : " &$Data21[$i] & @CRLF)
			ConsoleWrite("Data22 : " &$Data22[$i] & @CRLF)
		Next
        If ($Data3[2]="0x130" And $Data5[2]="0x230" And $Data7[2]="0x730" And $Data9[2]="0x830" And $Data11[2]="0x9B30" And $Data13[2]="0x9C30" And $Data15[2]="0x9D30" And $Data17[2]="0x9E30" And $Data19[2]="0x9F30" And $Data21[2]="0xA130") Then
		    $CANid=1
	    Else
		    $CANid=0
	    EndIf

        If($Data1[5]="DATA" And $Data3[5]="DATA" And $Data5[5]="DATA" And $Data7[5]="DATA" And $Data9[5]="DATA" And $Data11[5]="DATA" And $Data13[5]="DATA" And $Data15[5]="DATA" And $Data17[5]="DATA" And $Data19[5]="DATA" And $Data21[5]="DATA") Then
			$DataAll=1
	    Else
		    $DataAll=0
		EndIf

	    If($Data1[6]=00 And $Data3[6]=30 And $Data5[6]=30 And $Data7[6]=30 And $Data9[6]=30 And $Data11[6]=30 And $Data13[6]=30 And $Data15[6]=30 And $Data17[6]=30 And $Data19[6]=30 And $Data21[6]=30) Then
		    $Src=1
	     Else
		    $Src=0
	    EndIf

        If($Data1[7]="30 " And $Data3[7]="01 " And $Data5[7]="02 " And $Data7[7]="07 " And $Data9[7]="08 " And $Data11[7]="9B " And $Data13[7]="9C " And $Data15[7]="9D " And $Data17[7]="9E " And $Data19[7]="9F "  And $Data21[7]="A1 ") Then
		    $Dest=1
	    Else
		    $Dest=0
		EndIf
	EndIf


	ConsoleWrite("$CANid : " & $CANid & @CRLF)
	ConsoleWrite("$DataAll : " & $DataAll & @CRLF)
	ConsoleWrite("$Src : " & $Src & @CRLF)
	ConsoleWrite("$Dest : " & $Dest & @CRLF)

	If($Data1[11]="1000000000000000" And $Data3[11]="0C222D384E0B2C37" And $Data5[11]="0D222D384E" And $Data7[11]="0C232D384E" And $Data9[11]="0C222E384E" And $Data11[11]="0C222D394E" And $Data13[11]="0C222D394E" And  $Data15[11]="0C242D384E" And $Data17[11]="0C222F384E" And $Data19[11]="0C222D3A4E" And $Data21[11]="0C222D3850") Then
		$DataBytes=1
	Else
		$DataBytes=0
	EndIf

	_DeactJ1939()

	If($CANid=1 And $DataAll=1 And $Src=1 And $Dest=1) Then
		_WriteResult("Pass","TS_J1939Node_08")
    Else
	   _WriteResult("Fail","TS_J1939Node_08")
	EndIf
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Node_08")
EndIf


ConsoleWrite("****End : TS_J1939Node_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)


