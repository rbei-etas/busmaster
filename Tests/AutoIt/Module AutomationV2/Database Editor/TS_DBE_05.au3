;=== Test Cases/Test Data ===
; Module			:		Database Editor
; Critical (C)		:		Y
; TestCase No.		:		1
; TestCases			:		Edit signal descriptor and value
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_05.au3****"&@CRLF)

_launchApp()
if $app=0 Then
	_openCANDB("testDBE_01")														; Open "testDBE_01" DBF file
	$sigDetlvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDet_DBeditor)							; Get handle of signal details list view
	_GUICtrlListView_ClickItem($sigDetlvhwd,0)										; Click on the first item in the Signal details list view
	sleep(250)
	ControlClick($WIN_BUSMASTER,"",$BTN_EditSig_DBEditor)											; Click the 'Edit Signal' button
	sleep(1000)
	_addSigDetails("unsigned int","Signal1update",64,0,0)							; Edit the signal details
	_invalidSigDescMsg()															; Handle Changing signal range warning message
	sleep(500)
Endif
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_SelectSigDesc(0)																; select signal description from list view
	ControlClick($WIN_DBEditor_CAN,"",$BTN_EditSigDesc_DBEditor)								; Click on 'Edit Desc' button
	_addSigDesc("Desc1update",2)															; Call _addSigDesc function from 'globalFunc4Module.au3' file to update signal description.
	sleep(500)
	$sigDesclvhwd=controlgethandle($WIN_BUSMASTER,"",$LVC_SigDesc_SigDesc)							; Get handle of signal description list view
	$sigDescName=_GUICtrlListView_GetItemText($sigDesclvhwd,0,0)						; Fetch the signal desc name
	$sigDescVal=_GUICtrlListView_GetItemText($sigDesclvhwd,0,1)							; Fetch the signal desc value
	ConsoleWrite("$sigDescName :"&$sigDescName&@CRLF)
	ConsoleWrite("$sigDescVal :"&$sigDescVal&@CRLF)
EndIf
if $sigDescName="Desc1update" and $sigDescVal=2 Then
	_ExcelWriteCell($oExcel, "Pass", 10, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 10, 2)
EndIf

ConsoleWrite("****End : TS_DBE_05.au3****"&@CRLF)
ConsoleWrite(@CRLF)
