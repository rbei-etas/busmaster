;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Trace_01
; TestCases			:		Trace window Usage.
; Test Data			:
; Test Setup		:		1. Open Busmaster
;~ 							2. Connect the Busmaster to the bus other than simulation mode.
;~ 							3. Right click on the Trace window.

; Expected Results  : 		1. Verify the functionality of the menu items
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Trace_01.au3****"&@CRLF)

_launchApp()																					; Check if the App is open else launch it
WinActivate($WIN_BUSMASTER)

Local $itemCount1=0, $itemCount2=0,$itemCount3=1

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxTraceWin_01.cfx")															; Load a configuration file

	; Select following hardwares to generate errors in the trace window
	WinRibbonSelectItem($WIN_BUSMASTER,"",$CANMenu,$canDriversel,$ixxatHWMenu)					; Select IXXAT hardware
	sleep(1000)
	WinRibbonSelectItem($WIN_BUSMASTER,"",$CANMenu,$canDriversel,$nsiHWMenu)					; Select NSI CAN hardware
	sleep(1000)
	WinRibbonSelectItem($WIN_BUSMASTER,"",$CANMenu,$canDriversel,$ixxatHWMenu)					; Select IXXAT hardware
	sleep(1000)

	$hWnd=controlgethandle($WIN_BUSMASTER,"","[CLASS:ListBox; INSTANCE:1]")
	$itemCount1=_GUICtrlListBox_GetCount($hWnd)													; Fetch the no. of entries in the trace window
	_GUICtrlListBox_ClickItem($hWnd,1,"left")													; select the second item in the trace window
	_GUICtrlListBox_ClickItem($hWnd,1,"right")													; right click on the second item
	sleep(1000)
	send("d")																					; Select "delete" menu option
	sleep(500)

	$itemCount2=_GUICtrlListBox_GetCount($hWnd)													; Fetch the no. of entries in the trace window
	_GUICtrlListBox_ClickItem($hWnd, 0,"right")													; right click on the first item
	sleep(500)
	send("s")																					; Select "Select All" menu option
	sleep(500)

	$selCount=_GUICtrlListBox_GetSelCount($hWnd)												; Fetch the no. of items that are selected

	_GUICtrlListBox_ClickItem($hWnd, 0,"right")													; right click on the first item
	sleep(500)
	send("c")																					; Select "Clear" menu option
	sleep(500)
	$itemCount3=_GUICtrlListBox_GetCount($hWnd)													; Fetch the no. of entries in the trace window

EndIf

ConsoleWrite("$itemCount1: "&$itemCount1&@crlf)
ConsoleWrite("$itemCount2: "&$itemCount2&@crlf)
ConsoleWrite("$itemCount3: "&$itemCount3&@crlf)

if $itemCount1=3 and $itemCount2=2 and $itemCount3=0 Then										; Verify the expected result
	_WriteResult("Pass","TS_Trace_01")
Else
	_WriteResult("Fail","TS_Trace_01")
EndIf

$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Trace_01")
EndIf

ConsoleWrite("****End : TS_Trace_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)






