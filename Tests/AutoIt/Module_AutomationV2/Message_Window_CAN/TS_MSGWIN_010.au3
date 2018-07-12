;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_010
; TestCases			:		Message interpretation
; Test Data			:
; Test Setup		:		1. Transmit messages which are present in the database and currently associated to configuration.
;                           2. Double click on a particular message in message window.
;                           3. While the 'Message Interpretation' dialog is visible, click on any other interpretable message in the message window.
;                           4. Close the 'Message Interpretation' dialog.
;                           5. Expand few messages by clicking on '+' sign in message window.
;                           6. Switch to Append mode and back to interpret mode.


; Expected Results  : 		 After step6, the expanded messages in step3 will remain to be in expanded state.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_010.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_01")															 ; Load Configuration
	_ConnectDisconnect()
	sleep(1000)
	_TransmitMsgsMenu()	                                                                     ;Transmit Data
	sleep(3000)
	_ConnectDisconnect()
	_EnableInterpretMode()
     Sleep(1000)
	 WinActivate($WIN_CANMsgWind)
	 Sleep(500)
	$HWD=_GetCANMsgWinHWD()                                                                 ;Get handle for CAN message window
	Sleep(1000)
	ControlFocus($WIN_BUSMASTER,"",$LSTC_CANMsgWin)
	$count1= _GetCANMsgWinItemCount()                                                       ;Get message count before expansion
	Sleep(1000)
	_GUICtrlListView_ClickItem($HWD,2)                                                      ;Expand second message
	Send("{ENTER}")
	sleep(1000)
	_GUICtrlListView_ClickItem($HWD,4)                                                     ;Expand message in fourth row
	Send("{ENTER}")
	sleep(1000)
	$count2=_GetCANMsgWinItemCount()                                                       ;Count after expanding messages
	Sleep(1000)
	_DisableOverwriteMode()                                                                ;Switch to append mode
	Sleep(1000)
	_EnableOverwriteMode()                                                                 ;Enable overwrite mode
	$count3=_GetCANMsgWinItemCount()                                                       ;Count after switching overwrite mode from append mode
	Sleep(500)
	ConsoleWrite("Count1:" & $count1 & @CRLF)
	ConsoleWrite("Count2:" & $count2 & @CRLF)
	ConsoleWrite("Count3:" & $count3 & @CRLF)
	If($count2==$count3) Then
		_WriteResult("Pass","TS_MSGWIN_10")
	Else
	   _WriteResult("Fail","TS_MSGWIN_10")
	EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_10")
EndIf

ConsoleWrite("****End : TS_MSGWIN_010.au3****"&@CRLF)
ConsoleWrite(@CRLF)

