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

	_loadConfig("cfxTraceWin_02.cfx")

	Sleep(1000)

	$handle=WinGetHandle($WIN_BUSMASTER)
	$menuHandle=_GUICtrlMenu_GetMenu($handle)

	$sub=_GUICtrlMenu_GetItemSubMenu($menuHandle,3)
	$text=_GUICtrlMenu_GetItemText($sub,0)
	$state=_GUICtrlMenu_GetItemChecked($sub,0)
	ConsoleWrite("Item text" & $text & @CRLF)
	ConsoleWrite("Item Check state" & $state & @CRLF)
	If($state=True) Then
		_WriteResult("Pass","TS_Trace_02")
    Else
	   _WriteResult("Fail","TS_Trace_02")
   EndIf
EndIf


$isAppNotRes=_CloseApp()																					; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Trace_02")
EndIf

ConsoleWrite("****End : TS_Trace_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)





