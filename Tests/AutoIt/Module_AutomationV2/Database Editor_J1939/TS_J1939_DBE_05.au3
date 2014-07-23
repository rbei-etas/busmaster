;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_05
; TestCases			:		Edit signal descriptor and value
; Test Data			:		"Message Name: Test, essage ID : 0x123,Signal Name : Test64bit, Type : unsigned INT"
; Test Setup		:		1. Select "J1939->Database->Open..."
;~ 							2. Select "DBForSTS.dbf" database from the Open file dialog box
;~ 							3. Select "MESSAGE1" from the left view
;~ 							4. Select signal "Signal_2"
;~ 							5. Select "PLUS_ONE" signal description
;~ 							6. Select "Edit Desc..." button"
;~ 							7. Update the description.

; Expected Results  : 		"Should display a dialog with selected descriptor and value to be edited. Same should be updated to the list if "Ok" is selected."
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_05.au3****"&@CRLF)

_launchApp()

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_openJ1939DB("testJ1939DBE_05")														; Open dbf file
	_SelectSigDesc(0)																	; select signal description from list view
	ControlClick($WIN_DBEditor_J1939,"",$BTN_EditSigDesc_DBEditor)						; Click on 'Edit Desc' button
	_addSigDesc("Desc1update",2)														; Call _addSigDesc function from 'globalFunc4Module.au3' file to update signal description.
	sleep(500)
	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)				; Get handle of signal description list view
	$sigDescName=_GUICtrlListView_GetItemText($sigDesclvhwd,0,0)						; Fetch the signal desc name
	$sigDescVal=_GUICtrlListView_GetItemText($sigDesclvhwd,0,1)							; Fetch the signal desc value
	ConsoleWrite("$sigDescName :"&$sigDescName&@CRLF)
	ConsoleWrite("$sigDescVal :"&$sigDescVal&@CRLF)
EndIf
if $sigDescName="Desc1update" and $sigDescVal=2 Then
	_WriteResult("Pass","TS_J1939_DBE_05")
Else
	_WriteResult("Fail","TS_J1939_DBE_05")
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_05")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)
