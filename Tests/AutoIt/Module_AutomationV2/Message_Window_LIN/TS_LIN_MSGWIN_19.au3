#cs
;==========================================================================Test Procedure =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_LIN_MSGWIN_19
; TestCases			:		Message interpretation
; Test Data			:
; Test Setup		:		1. Continue from above test case.
							2. Right click on an interpretable message which is in collapsed state to see a popup menu.
							3.Select the option 'Expand' in the popup menu.
							4. Observe the message window.
							5.Again Right click on an interpretable message which is in expanded state to see a popup menu.and select the option 'Collapse' in the pop up menu


; Expected Results  : 		1. After step2, the popup menu should contain a menu item 'Expand'.
							2.After step4,The interpretable message will now gets expanded.
							3.After step5,The interpretable message will now gets collapsed.
;==========================================================================Test Procedure =========================================================================
#ce

;#include <IncludeFiles.au3>
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_MSGWIN_019.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER)

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLINMsgWin_03")															 ; Load Configuration

	_linDriverSelection()																							;DriverSelection
	ConsoleWrite("Driver selection is done" & @CRLF)

	_ConnectDisconnect_LIN()																;Click connect/Disconnect in tool bar-LIN
	sleep(3000)

	_ConnectDisconnect_LIN()																;Click connect/Disconnect in tool bar-LIN
	sleep(1000)

	_EnableInterpretMode()
     Sleep(1000)

	 WinActivate($WIN_LINMsgWind)
	 Sleep(500)

	$HWD=_GetLINMsgWinHWD()                                                                 ;Get handle for lin message window
	Sleep(1000)
	ControlFocus($WIN_BUSMASTER,"",$LSTC_LINMsgWin)
	Sleep(1000)
	_GUICtrlListView_ClickItem($HWD,2,"right")                                              ;Expand third message by right clik
	Sleep(500)
	Send("{DOWN}")                                                                          ;Press down arrow key
	Send("{DOWN}")                                                                          ;Press down arrow key
	Send("{ENTER}")                                                                         ;Press ENTER

	$count1= _GetLINMsgWinItemCount()
	ConsoleWrite("count after expand message : "& @CRLF)

	Sleep(1000)
    _GUICtrlListView_ClickItem($HWD,2,"right")                                             ;Collapse third message by right clik
	Sleep(500)
	Send("{DOWN}")                                                                         ;Press Down arrow key
	Send("{DOWN}")
	Send("{ENTER}")                                                                        ;Press Enter key

    $count2= _GetCANMsgWinItemCount()

	ConsoleWrite("$count1 (count after expanded message : ) = " & $count1 & @CRLF)
	ConsoleWrite("$count2 (count after collapsed message : ) = " & $count2 & @CRLF)

	If($count1=6 And $count2=4) and $count1 > 4 Then
		_WriteResult("Pass","TS_LIN_MSGWIN_19")
		ConsoleWrite("PASS" & @CRLF)
	Else
	   _WriteResult("Fail","TS_LIN_MSGWIN_19")
	   ConsoleWrite("FAIL" & @CRLF)
	EndIf

EndIf
$isAppNotRes=_CloseApp()																	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_LIN_MSGWIN_19")
EndIf

ConsoleWrite("****End : TS_LIN_MSGWIN_19.au3****"&@CRLF)
ConsoleWrite(@CRLF)




