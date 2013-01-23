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
WinActivate($mWin,3)
if winexists($mWin) then
	if winexists("",$disDBtxt) Then
		consolewrite("Dissociation will clear Signal Watch List. window exists" & @CRLF)
		WinActivate("",$disDBtxt)
		send("{ENTER}")					  									; Click on 'yes' button in "Dissociation will clear Signal Watch List." warning message
	EndIf
	$disDBlbHWD=ControlGetHandle($disDbDialog,"",$dissocdbflbinst1)			; Fetch the handle of dissociate Db list box
	$assocDBCount=_GUICtrlListBox_GetCount($disDBlbHWD)						; Fetch the count of items in dissociate DB window
	ConsoleWrite("$assocDBCount : " & $assocDBCount &@CRLF)
	ControlClick($disDbDialog,"",$cancelDisBTNctrlID)						; Click on 'Cancel' button in Dissociate database window
EndIf
if $assocDBCount=0 Then
	_ExcelWriteCell($oExcel, "Pass", 13, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 13, 2)
EndIf

ConsoleWrite("****End : TS_DBE_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)