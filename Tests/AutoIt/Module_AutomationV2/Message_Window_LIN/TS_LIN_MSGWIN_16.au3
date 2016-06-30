#cs
;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_MSGWIN_16
; TestCases			:		Archieving message window attributes.
; Test Data			:
; Test Setup		:		1. Do the Test case mentioned above.
							2. Click on the "+" sign displayed next to message which you wish to expand.
							3. Click on the "-" sign displayed next to the message that is previously expanded.

; Expected Results  : 		1. Signals present in the message should appear.
							2. Physical and raw values of the message should be shown.
							3. A "-" sign should be shown next to the message that is currently expanded.

							After step3,The message now returns to collapsed stage. It will now have "+" sign displayed next to it in the beginning of the message columns.
;==========================================================================Test Procedure =========================================================================
#ce
;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_LIN_MSGWIN_16.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINMsgWin_01")															 ; Load Configuration
	_SetViewToolBarLIN()

	;_linDriverSelection()																	; DriverSelection for LIN
	_linDriverSelection()
	ConsoleWrite("Driver selection is done" & @CRLF)


	_EnableInterpretMode_LIN()
     Sleep(1000)

	_ConnectDisconnect_LIN()														;Click connect/Disconnect in tool bar-LIN
	sleep(1000)

     Sleep(1000)

	 WinActivate($WIN_LINMsgWind)
	 Sleep(500)


	;ControlFocus($WIN_BUSMASTER,"",$LSTC_CANMsgWin)
	Sleep(500)

	$HWD=_GetLINMsgWinHWD()                                                        		;Get handle for LIN message window
	Sleep(1000)

	ControlFocus($WIN_BUSMASTER,"",$LSTC_LINMsgWin)
	Sleep(2000)


	_GUICtrlListView_ClickItem($HWD,0)                                                  ;Click on first row in message window
	Send("{ENTER}")                                                                    	;Expand the message by clicking ENTER
	sleep(1000)

    $count1= _GetLINMsgWinItemCount()                                                   ;Get message count after expansion
	ConsoleWrite("Get message count after expansion " & $count1 & @CRLF)
	ConsoleWrite("Count1: " & $count1 & @CRLF)
	sleep(1000)

	_GUICtrlListView_ClickItem($HWD,0)                                                 	;Collapse expanded message
	Send("{ENTER}")
	sleep(1000)

	$count2= _GetLINMsgWinItemCount()                                                	;Get message count after collapsing
	ConsoleWrite("Get message count after collapsing " & $count2& @CRLF)
	ConsoleWrite("Count2: " & $count2 & @CRLF)

	If($count1=5 And $count2=1) Then
		_WriteResult("Pass","TS_LIN_MSGWIN_16")
		ConsoleWrite("PASS"  & @CRLF)
	Else
	   _WriteResult("Fail","TS_LIN_MSGWIN_16")
	   ConsoleWrite("FAIL"  & @CRLF)
	EndIf

EndIf
$isAppNotRes=_CloseApp()																; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_MSGWIN_16")
EndIf





ConsoleWrite("****End : TS_LIN_MSGWIN_16.au3****"&@CRLF)
ConsoleWrite(@CRLF)



