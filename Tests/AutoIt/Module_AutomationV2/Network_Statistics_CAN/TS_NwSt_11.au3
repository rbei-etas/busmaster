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
Local $bTransmission=0, $aTransmission=0,$aDisconnect=0

if winexists($mWin) then
	_loadConfig("cfx_NwSt_11.cfx")
	_CANNWStatsMenu()															; Open Network Statistics winodow
	Sleep(1000)
	$handle= ControlGetHandle($WIN_NW_Stats,"",$LVC_Details_NW_Stats)
	$count1= _GUICtrlListView_GetColumnCount($handle)
	ConsoleWrite("Column count before" & $count1 & @CRLF)
	$handle_header=_GUICtrlListView_GetHeader($handle)
	_GUICtrlListView_ClickItem($handle_header,1,"right")
	Send("{DOWN}")
	Sleep(500)
	Send("{ENTER}")
	Sleep(500)
	$count2= _GUICtrlListView_GetColumnCount($handle)
	ConsoleWrite("Column count after" & $count2& @CRLF)
EndIf

