;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_12
; TestCases			:		Delete Message
; Test Data			:		
; Test Setup		:		1. Open Database editor
;~ 							2. Select a message
;~ 							3. Right click on the message and select 'Delete Message' from then menu.
;~ 							4. Select OK to confirm deletion"

; Expected Results  : 		"The message will be deleted from the list"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_12.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)
$msgCount=0
if winexists($WIN_BUSMASTER) then
	_openJ1939DB("testJ1939DBE_12")													; Open DBF file
	$DBTreeVwHWD = ControlGetHandle($WIN_DBEditor_J1939, "", $TVC_DBFile_DBEditor) 	; Fetch tree view control handle
	$DbtreeVwparent = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD) 					; Fetch Parent node handle
	$childHWD = _GUICtrlTreeView_GetNext($DBTreeVwHWD, 0)
	ConsoleWrite("$DbtreeVwparent :"&$DbtreeVwparent & @CRLF)
	ConsoleWrite("$childHWD :"&$childHWD & @CRLF)
	_GUICtrlTreeView_ClickItem($DBTreeVwHWD, $childHWD, "Right") 					; Right Click on the msg node
	Send("d") 																		; Select Delete message menu
	sleep(1000)
	if winexists($DLG_Busmaster,$msgDeltxt) Then
		controlclick($DLG_Busmaster,"",$BTN_Yes_BM)									; Click on 'Yes' button
		consolewrite("Win Exists"&@Crlf)
	EndIf
	$DBTreeVwHWD = ControlGetHandle($WIN_DBEditor_J1939, "", $TVC_DBFile_DBEditor) 	; Fetch tree view control handle
	$DbtreeVwparent = _GUICtrlTreeView_GetFirstItem($DBTreeVwHWD) 					; Fetch Parent node handle
	$msgCount=_GUICtrlTreeView_GetChildCount($DBTreeVwHWD,0)
	ConsoleWrite("$msgCount :"&$msgCount & @CRLF)
EndIf
if $msgCount=-1 Then
	_WriteResult("Pass","TS_J1939_DBE_12")
Else
	_WriteResult("Fail","TS_J1939_DBE_12")
EndIf


$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_12")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_12.au3****"&@CRLF)
ConsoleWrite(@CRLF)