;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_03
; TestCases			:		"Add signal description - for the signal and description selected"
; Test Data			:		-
; Test Setup		:		1. Select "J1939->Database->Open..."
;~ 							2. Select "DBForSTS.dbf" database from the Open file dialog box
;~ 							3. Select "MESSAGE1" from the left view
;~ 							4. Select signal "Signal_4"
;~ 							5. Select "New Desc..." button
;~ 							6. Enter the details specified

; Expected Results  : 		Should successfully add signal description into the signal description list.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_03.au3****"&@CRLF)

_launchApp()																; Check if the App is open else launch it

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_openJ1939DB("testJ1939DBE_03")
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)							; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)										; Click on the first item in the Signal details list view
	sleep(250)
	ControlClick($WIN_BUSMASTER,"",$BTN_SigDes_DBEditor)											; Click the 'New Desc...' button
	sleep(1000)
	_addSigDesc("Desc1",1)																; Call _addSigDesc function from 'globalFunc4Module.au3' file to add signal description.
	sleep(500)
	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)							; Get handle of signal description list view
	$sigDescName=_GUICtrlListView_GetItemText($sigDesclvhwd,0,0)						; Fetch the signal desc name
	$sigDescVal=_GUICtrlListView_GetItemText($sigDesclvhwd,0,1)							; Fetch the signal desc value
	ConsoleWrite("$sigDescName :"&$sigDescName&@CRLF)
	ConsoleWrite("$sigDescVal :"&$sigDescVal&@CRLF)

	_ActivatechildWindow($WIN_DBEditor_J19391)
	WinClose($WIN_DBEditor_J19391)
	if WinWaitActive($WIN_BUSMASTER,$saveDBtxt,2) Then
		ControlClick($WIN_BUSMASTER,"",$BTN_No_SaveDB)
	EndIf
EndIf
if $sigDescName="Desc1" and $sigDescVal=1 Then
	_WriteResult("Pass","TS_J1939_DBE_03")
Else
	_WriteResult("Fail","TS_J1939_DBE_03")
EndIf

$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_03")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_03.au3****"&@CRLF)
ConsoleWrite(@CRLF)