;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_18
; TestCases			:		Check for UnDefined message
; Test Data			:
; Test Setup		:		1. Invoke dialog box for filter configuration
;~ 							2. Enter ID of an undefined message not occuring in the filter list
;~ 							3. Select Add button

; Expected Results  : 		1. Filter list should be updated

; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_17
; TestCases			:		Check for Invalid message
; Test Data			:
; Test Setup		:		1. Invoke dialog box for filter configuration
;~ 							2. Enter ID of invalid message(like -1) 3. check add button state

; Expected Results  : 		1. The add button will be in disabled state
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Filter_18.au3****"&@CRLF)

_launchApp()																							; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER)

Local $MsgID101=0

if winexists($WIN_BUSMASTER) then
	_createConfig("cfxFilter_18.cfx")																	; Load Configuration

	_AssociateCANDB("StdExtDB.dbf")																		; Associate DB

	_AddFilter("Stop","[0x101]MsgStdLil","","","Std","Non RTR","Tx",1)									; Configure Filter details

	_saveConfig()																						; Save the config

	_CloseApp()																							; Close the app

	_launchApp()																						; Launch the app

	_AppFilterMenu()																					; Open filter configuration dialog

	$FltSelhWd=controlgethandle($WIN_Config_filter,"",$LVC_FilterDetails_ConfigFilter)					; Get handle of filter selection list view
	$ItemCount=_GUICtrlListView_GetItemCount($FltSelhWd)												; fetch the item count after deleting a filter

	if $ItemCount=1 Then
		$MsgID101=_GUICtrlListView_GetItemText($FltSelhWd, 0 , 0)										; fetch the first Msg ID
	EndIf

	ControlClick($WIN_Config_filter,"",$BTN_OK_ConfigFilter)											; Close filter configuration dialog

EndIf

ConsoleWrite("$ItemCount : "&$ItemCount&@CRLF)
ConsoleWrite("$MsgID101 : "&$MsgID101&@CRLF)

if $MsgID101=101 Then
	_WriteResult("Pass","TS_Filter_18")
Else
	_WriteResult("Fail","TS_Filter_18")
EndIf


ConsoleWrite("****End : TS_Filter_18.au3****"&@CRLF)
ConsoleWrite(@CRLF)