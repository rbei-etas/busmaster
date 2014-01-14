;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_04
; TestCases			:		"Add Signal Descriptor for signal length >31 bit"
; Test Data			:		"Message Name: Test, essage ID : 0x123,Signal Name : Test64bit, Type : unsigned INT"
; Test Setup		:		1. Select the Signal "Test64bit"
;~ 							2. Select "New Desc" menu or toolbar
;~ 							3. Add the descriptor name and value.
;~ 							4. Select OK.

; Expected Results  : 		"The signal descriptor will be added to the list"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_04.au3****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_openCANDB("testDBE_04")														; Open "testDBE_01" DBF file
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
	_WriteResult("Pass","TS_DBE_04")
Else
	_WriteResult("Fail","TS_DBE_04")
EndIf


$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_DBE_04")
EndIf

ConsoleWrite("****End : TS_DBE_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)