;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Filter_16
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
ConsoleWrite("****Start : TS_Filter_16.au3****"&@CRLF)

_launchApp()																							; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER)

Local $MsgID=0,$IsAddBTNEnabled=1,$ErrMsg=""

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxFilter_16.cfx")																		; Load Configuration

	_AppFilterMenu()																					; Select CAN->Filter Configuration

	if winexists($WIN_Config_filter) Then
		ControlSetText ($WIN_Config_filter,"",$Edit_ID_ConfigFilter,12)									; Enter Msg ID 12

		ControlClick($WIN_Config_filter,"",$BTN_Add_ConfigFilter)										; Click on Add button
		sleep(500)

		$FltSelhWd=controlgethandle($WIN_Config_filter,"",$LVC_FilterDetails_ConfigFilter)				; Get handle of filter selection list view
		$MsgID=_GUICtrlListView_GetItemText($FltSelhWd, 0 , 0)											; fetch the Msg ID

		ControlSend($WIN_Config_filter,"",$Edit_ID_ConfigFilter,"-1")									; Enter Msg ID -1
		sleep(500)

		$IsAddBTNEnabled=ControlCommand ($WIN_Config_filter,"",$BTN_Add_ConfigFilter,"IsEnabled")		; check whether "Add" button is disabled

		$ErrMsg=ControlGetText ( $WIN_Config_filter, "", 1233)											; Fetch the error message

		ControlClick($WIN_Config_filter,"",$BTN_OK_ConfigFilter)										; Click on OK button
	EndIf
EndIf

ConsoleWrite("$MsgID : "&$MsgID&@CRLF)
ConsoleWrite("$IsAddBTNEnabled : "&$IsAddBTNEnabled&@CRLF)
ConsoleWrite("$ErrMsg : "&$ErrMsg&@CRLF)

if $MsgID=12 and $IsAddBTNEnabled=0 and $ErrMsg="Invalid Message ID" Then								; Validate the result
	_WriteResult("Pass","TS_Filter_16")
Else
	_WriteResult("Fail","TS_Filter_16")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Filter_16")
EndIf



ConsoleWrite("****End : TS_Filter_16.au3****"&@CRLF)
ConsoleWrite(@CRLF)