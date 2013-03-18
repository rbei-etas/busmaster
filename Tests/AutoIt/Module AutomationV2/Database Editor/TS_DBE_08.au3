;=== Test Cases/Test Data ===
; Module			:		Database Editor
; Critical (C)		:		Y
; TestCase No.		:		1
; TestCases			:		Database Usage in Busmaster
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_08.au3****"&@CRLF)

_launchApp()
if $app=0 Then
	_AssociateCANDB("testDBE_01")								; Associate DB
	_DissociateCANDB(0)											; Dissociate DB
EndIf
WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	if winexists("",$disDBtxt) Then
		consolewrite("Dissociation will clear Signal Watch List window exists" & @CRLF)
		WinActivate("",$disDBtxt)
		send("{ENTER}")					  									; Click on 'yes' button in "Dissociation will clear Signal Watch List." warning message
	EndIf
	$disDBlbHWD=ControlGetHandle($WIN_Dissociate_CANDB,"",$LSTB_DBFile_Dis_CANDB)			; Fetch the handle of dissociate Db list box
	$assocDBCount=_GUICtrlListBox_GetCount($disDBlbHWD)						; Fetch the count of items in dissociate DB window
	ConsoleWrite("$assocDBCount : " & $assocDBCount &@CRLF)
	ControlClick($WIN_Dissociate_CANDB,"",$BTN_Cancel_Dis_CANDB)						; Click on 'Cancel' button in Dissociate database window
EndIf
if $assocDBCount=0 Then
	_ExcelWriteCell($oExcel, "Pass", 13, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 13, 2)
EndIf

ConsoleWrite("****End : TS_DBE_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)