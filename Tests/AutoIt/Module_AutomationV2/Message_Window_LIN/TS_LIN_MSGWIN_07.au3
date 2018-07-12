;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_MSGWIN_07
; TestCases			:		Message Transmission
; Test Data			:
; Test Setup		:		1. Start Application.
							;2.Select any Hardware for LIN
							;3.Add frames to TX list by Selecting frames from available LIN frames list.
							;5. observe Message Type and Data bytes column of message window
; Expected Results  : 		Message Type should be "LIN MESSAGE".Data bytes column should display the data either in decimal or hexadecimal representation
;==========================================================================Test Procedure =========================================================================
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start :TS_LIN_MSGWIN_07.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINMsgWin_06")																					; Load Configuration
	_SetViewToolBarLIN()
	;_linDriverSelection()																							;DriverSelection
	_linDriverSelection()
	ConsoleWrite("Driver selection is done" & @CRLF)

	sleep(2000)
	_ConnectDisconnect_LIN()																						; Connect and wait for 5 sec

	Sleep(2000)
	_ConnectDisconnect_LIN()																						; Disconnect

	$rCount=_GetLINMsgWinItemCount()																				;count no. of items in msgwindow
	$Count1=_GetLINMsgWinColCount()																					;count no. of columns in msgwindow

	Local $Msg = ObjCreate("System.Collections.ArrayList") 															; create object
	$FirstMsg = 0



	if $rCount <>0 Then
		 ;$msg = _GetMsgWinLINInfo(0)

		$Msg=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)																	; Fetch the first row data in the msg window
		sleep(2000)


		for $i=0 to $Count1-2
			$Value =$Msg.Item($i)
			ConsoleWrite("$Msg[" & $i &"] : "& $Value &@CRLF)
			;ConsoleWrite("$Msg_1[" & $i &"] : "& $msg1 &@CRLF)
		next

		ConsoleWrite("$Msg.Item(2) = "& $Msg.Item(2) &@CRLF)
		if $Msg.Item(2)="LIN Message" and $Msg.Item(7)="F3 FF FF FF" Then													; validate Message Type and data byte
			$FirstMsg=1
			ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
		Else
			$FirstMsg=0
			ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)
		EndIf


	EndIf


EndIf
ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)

if $FirstMsg=1 Then
		_WriteResult("Pass","TS_LIN_MSGWIN_07")
		ConsoleWrite("Pass"& @CRLF)
	Else
		_WriteResult("Fail","TS_LIN_MSGWIN_07")
		ConsoleWrite("Fail"& @CRLF)
EndIf



Sleep(1000)
$isAppNotRes=_CloseApp()																							; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_MSGWIN_09")
	ConsoleWrite("WARNING")

EndIf



ConsoleWrite("****End : TS_LIN_MSGWIN_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)




