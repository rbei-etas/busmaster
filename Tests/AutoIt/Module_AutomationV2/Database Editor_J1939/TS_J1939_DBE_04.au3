;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939_DBE_04
; TestCases			:		Edit Signal Details
; Test Data			:		"Message Name: Test, essage ID : 0x123,Signal Name : Test64bit, Type : unsigned INT"
; Test Setup		:		1. Select "J1939->Database->Open..."
;~ 							2. Select "DBForSTS.dbf" database from the Open file dialog box
;~ 							3. Select "MESSAGE1" from the left view
;~ 							4. Select "New Signal..." button"
;~ 							5. Add a signal.
;~ 							6. After adding signal,Select an item in the signal details list
;~ 							7. Select "Edit Signal..." button"
;~ 							8. Update the signal details

; Expected Results  : 		"1. Should display a dialog which will allow the user to add new signal details"
;~ 							 2.After step 8,the signal details should be updated.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939_DBE_04.au3****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_openJ1939DB("testJ1939DBE_04")														; Open "testDBE_01" DBF file
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)			; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)										; Click on the first item in the Signal details list view
	sleep(250)
	ControlClick($WIN_BUSMASTER,"",$BTN_EditSig_DBEditor)							; Click the 'Edit Signal' button
	sleep(1000)
	_addSigDetails("unsigned int","Signal1update",64,0,0)							; Edit the signal details
	_invalidSigDescMsg()															; Handle Changing signal range warning message
	sleep(500)
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)			; Get handle of signal details list view
	$sigName=_GUICtrlListView_GetItemText($sigDetlvhwd,0,0)							; Fetch the signal name
	ConsoleWrite("$sigName :" &$sigName & @crlf)
	$sigLen=_GUICtrlListView_GetItemText($sigDetlvhwd,0,3)							; Fetch the signal length
	ConsoleWrite("$sigLen :" &$sigLen & @crlf)
	$sigType=_GUICtrlListView_GetItemText($sigDetlvhwd,0,4)							; Fetch the signal type
	ConsoleWrite("$sigType :" &$sigType & @crlf)
EndIf
if $sigName="Signal1update" and $sigLen=64 and $sigType="unsigned int" Then
	_WriteResult("Pass","TS_J1939_DBE_04")
Else
	_WriteResult("Fail","TS_J1939_DBE_04")
EndIf


$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939_DBE_04")
EndIf

ConsoleWrite("****End : TS_J1939_DBE_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)