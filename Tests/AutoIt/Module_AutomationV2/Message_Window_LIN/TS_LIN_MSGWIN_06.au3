;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_MSGWIN_06
; TestCases			:		Message Transmission
; Test Data			:
; Test Setup		:		1. Continue from above test case.
							;2. Select the 'Send' option in popup menu.
							;3. Observe the message window.

; Expected Results  : 		1. The selected message should be transmitted and new entry for the same message should be appended in message window list.
;==========================================================================Test Procedure =========================================================================
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_MSGWIN_06.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINMsgWin_06")																					; Load Configuration

	;_linDriverSelection()																							;DriverSelection
	_linDriverSelectionXml()
	ConsoleWrite("Driver selection is done" & @CRLF)

	sleep(2000)
	_ConnectDisconnect_LIN()																						; Connect and wait for 5 sec

	Sleep(2000)
	_ConnectDisconnect_LIN()																						; Disconnect

	$rCount=_GetLINMsgWinItemCount()																				;count no. of items in msgwindow
	$Count1=_GetLINMsgWinColCount()																					;count no. of columns in msgwindow

	Local $Msg = ObjCreate("System.Collections.ArrayList") 															; create object
	Local $Msg1 = ObjCreate("System.Collections.ArrayList") 														; create object
	$FirstMsg = 0
	$SecndMsg = 0


	if $rCount <>0 Then
		 ;$msg = _GetMsgWinLINInfo(0)

		$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)																	; Fetch the first row data in the msg window
		ConsoleWrite("$Msg : "& $Msg.Item(1) &@CRLF)
		sleep(2000)
		;$Msg1=_GetMsgWinInfo($LVC_CID_LINMsgWin,1)																	; Fetch the second row data in the msg window
		;ConsoleWrite("$Msg1 : "& $Msg1.Item(1) &@CRLF)

		for $i=0 to $Count1-2
			$Value =$Msg.Item($i)
			ConsoleWrite("$Msg[" & $i &"] : "& $Value &@CRLF)
			;ConsoleWrite("$Msg_1[" & $i &"] : "& $msg1 &@CRLF)
		next

		ConsoleWrite("$Msg.Item(0) = "& $Msg.Item(0) &@CRLF)
		if $Msg.Item(0) <>" " and $Msg.Item(1)="ALT_LINALT_R_01" and $Msg.Item(2)="LIN Message" and $Msg.Item(3)="Tx" and $Msg.Item(4)=1 and $Msg.Item(5)=2 and $Msg.Item(6)=0x012 and $Msg.Item(7)="07 00" and $Msg.Item(8)="0x66 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
			$FirstMsg=1
			ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
		Else
			$FirstMsg=0
			ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
		EndIf

		$Msg1=_GetMsgWinInfo($LVC_CID_LINMsgWin,1,10)																	; Fetch the second row data in the msg window
		ConsoleWrite("$Msg1 : "& $Msg1.Item(1) &@CRLF)

		for $j=0 to $Count1-2
			$Value1 =$Msg1.Item($j)
			ConsoleWrite("$Msg1[" & $j &"] : "& $Value1 &@CRLF)

		next

		ConsoleWrite("$Msg1.Item(0) = "& $Msg1.Item(0) &@CRLF)
		if $Msg1.Item(0) <>" " and $Msg1.Item(1)="ALT_LINALT_R_01" and $Msg1.Item(2)="LIN Message" and $Msg1.Item(3)="Tx" and $Msg1.Item(4)=1 and $Msg1.Item(5)=2 and $Msg1.Item(6)=0x012 and $Msg1.Item(7)="07 00" and $Msg1.Item(8)="0x66 (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
			$SecndMsg=1
			ConsoleWrite("$SecndMsg = "& $SecndMsg &@CRLF)
		Else
			$SecndMsg=0
			ConsoleWrite("$SecndMsg = "& $SecndMsg &@CRLF)
		EndIf


	EndIf


EndIf


if $FirstMsg=1 and $SecndMsg=1 Then
		_WriteResult("Pass","TS_LIN_MSGWIN_06")
		ConsoleWrite("Pass"& @CRLF)
	Else
		_WriteResult("Fail","TS_LIN_MSGWIN_06")
		ConsoleWrite("Fail"& @CRLF)
EndIf



Sleep(1000)
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_MSGWIN_06")
	ConsoleWrite("WARNING")

EndIf



ConsoleWrite("****End : TS_LIN_MSGWIN_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)




