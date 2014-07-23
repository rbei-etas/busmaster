;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_MSGWIN_08
; TestCases			:		Message Expansion.
; Test Data			:
; Test Setup		:		1. Do the Test case mentioned above.
;                           2. Click on the "+" sign displayed next to message which you wish to expand.

; Expected Results  : 		 Signals present in the message should appear.
;                            2. Physical and raw values of the message should be shown.
;                            3. A "-" sign should be shown next to the message that is currently expanded.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_08.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxCANMsgWin_01")															 ; Load Configuration
	_ConnectDisconnect()
	sleep(1000)
	_TransmitMsgsMenu()	                                                                 ;Transmit Data
	sleep(3000)
	_ConnectDisconnect()
	;_EnableOverwriteMode()
	_EnableInterpretMode()
     Sleep(1000)
	 WinActivate($WIN_CANMsgWind)
	 Sleep(500)
	;ControlFocus($WIN_BUSMASTER,"",$LSTC_CANMsgWin)
	Sleep(500)
	$HWD=_GetCANMsgWinHWD()                                                        ;Get handle for CAN message window
	Sleep(1000)
	ControlFocus($WIN_BUSMASTER,"",$LSTC_CANMsgWin)
	_GUICtrlListView_ClickItem($HWD,2)                                                  ;Click on second row in message window
	Sleep(1000)
	Send("{ENTER}")                                                                    ;Expand the message by clicking ENTER
	sleep(1000)
    $count1= _GetCANMsgWinItemCount()                                                   ;Get message count after expansion
	ConsoleWrite("Count1: " & $count1 & @CRLF)
	sleep(1000)
	_GUICtrlListView_ClickItem($HWD,2)                                                 ;Collapse expanded message
	Send("{ENTER}")
	sleep(1000)
	$count2= _GetCANMsgWinItemCount()                                                ;Get message count after collapsing
	ConsoleWrite("Count2: " & $count2 & @CRLF)
	If($count1=5 And $count2=4) Then
		_WriteResult("Pass","TS_MSGWIN_08")
	Else
	   _WriteResult("Fail","TS_MSGWIN_08")
	EndIf

EndIf
$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_MSGWIN_08")
EndIf





ConsoleWrite("****End : TS_MSGWIN_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)




