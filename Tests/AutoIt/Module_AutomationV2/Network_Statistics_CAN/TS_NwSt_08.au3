;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_NwSt_08
; TestCases			:		Invoking Network statistics dialog through menu and Hiding Network statistics dialog
; Test Data			:
; Test Setup		:		1.Open Busmaster
;                           2. Select the menu view->Network statistics.
;                           3. Observe the Icon "Show/Hide Network statistics window" and menu state.
;                           4.Click on the menu view->Network statistics.

;                           5.Observe the Icon "Show/Hide Network statistics window" and menu state.
; Expected Results  : 		1. After step 2 a window with title "Network Statistics" will appear.
;                           2. After step 3 the Icon will go to the pressed state.
;                           3. after step 4 network statistics window will go to hidden state.
;                           4. the icon and the menu both will go to the normal state.

;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_NwSt_08.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)


if winexists($mWin) then
	_loadConfig("cfx_NwSt_08.cfx")											     ; Load Configuration

	_CANNWStatsMenu()															; Open Network Statistics winodow
	Sleep(1000)

	$handle=ControlGetHandle($WIN_BUSMASTER,"",$ToolBar_Con_Disconnect)         ;Get the handle of Connect disconnect toolbar

	$id=_GetIconComId($handle,6)                                                ;Get command id for Network statistics toolbar icon
	$state1=_GUICtrlToolbar_IsButtonChecked($handle,$id)                         ;Get current state of the icon
	;ConsoleWrite("Button text:" &$text & @CRLF)
	ConsoleWrite("Button state1" & $state1 & @CRLF)
	_GUICtrlToolbar_ClickIndex($handle,6)                                        ;Hide Network statistics window by clicking on tool
	Sleep(500)
	Send("{ALT}")
	Sleep(500)
	Send("{C}")
	Sleep(500)
	Send("{ESC}")
	$state2=_GUICtrlToolbar_IsButtonChecked($handle,$id)                          ;Check if Network statistics window toolbar is checked
	ConsoleWrite("Button state2" & $state2	& @CRLF)
	If($state1=True And $state2=False) Then
		_WriteResult("Pass","TS_NwSt_08")
    Else
	    _WriteResult("Fail","TS_NwSt_08")
    EndIf

EndIf

$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_NwSt_08")
EndIf

ConsoleWrite("****End : TS_NwSt_08.au3****"&@CRLF)

