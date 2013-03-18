;=== Test Cases/Test Data ===
; Module			:		transmit CAN Messages
; Critical (C)		:		Y
; TestCase No.		:		1
; TestCases			:		Edit Signal Details
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_04.au3****"&@CRLF)

_launchApp()
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_openCANDB("testDBE_01")														; Open "testDBE_01" DBF file
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)							; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)										; Click on the first item in the Signal details list view
	sleep(250)
	ControlClick($WIN_BUSMASTER,"",$BTN_EditSig_DBEditor)											; Click the 'Edit Signal' button
	sleep(1000)
	_addSigDetails("unsigned int","Signal1update",64,0,0)							; Edit the signal details
	_invalidSigDescMsg()															; Handle Changing signal range warning message
	sleep(500)
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)							; Get handle of signal details list view
	$sigName=_GUICtrlListView_GetItemText($sigDetlvhwd,0,0)							; Fetch the signal name
	ConsoleWrite("$sigName :" &$sigName & @crlf)
	$sigLen=_GUICtrlListView_GetItemText($sigDetlvhwd,0,3)							; Fetch the signal length
	ConsoleWrite("$sigLen :" &$sigLen & @crlf)
	$sigType=_GUICtrlListView_GetItemText($sigDetlvhwd,0,4)							; Fetch the signal type
	ConsoleWrite("$sigType :" &$sigType & @crlf)
EndIf
if $sigName="Signal1update" and $sigLen=64 and $sigType="unsigned int" Then
	_ExcelWriteCell($oExcel, "Pass", 9, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 9, 2)
EndIf

ConsoleWrite("****End : TS_DBE_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)