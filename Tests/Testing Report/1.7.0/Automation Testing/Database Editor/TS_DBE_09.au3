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
ConsoleWrite("****Start : TS_DBE_09.au3****"&@CRLF)

_launchApp()
if $app=0 Then
	_AssociateCANDB("testDBE_01")											; Associate DB
	_DissociateCANDB(0)														; Dissociate DB
	if winexists("",$disDBtxt) Then
		consolewrite("Dissociation will clear Signal Watch List. window exists" & @CRLF)
		WinActivate("",$disDBtxt)
		send("{ENTER}")					  									; Click on 'yes' button in "Dissociation will clear Signal Watch List." warning message
	EndIf
	ControlClick($disDbDialog,"",$cancelDisBTNctrlID)						; Click on 'Cancel' button in Dissociate window
EndIf
WinActivate($mWin,3)
if winexists($mWin) then
	_AssociateCANDB("testDBE_01")												; Associate DB
	WinMenuSelectItem($mWin,"",$fileMenu,$databaseMenu,$DissocCANDB)			; Select File->Database->Dissociate
	$disDBlbHWD=ControlGetHandle($disDbDialog,"",$dissocdbflbinst1)				; Fetch the handle of dissociate Db list box
	$assocDBFilePath=_GUICtrlListBox_GetText($disDBlbHWD,0)						; Fetch the Associated DB file path from list box
	ConsoleWrite("$assocDBFilePath :" & $assocDBFilePath & @CRLF)
	$assocDBFName=StringSplit($assocDBFilePath,"\")								; Split the file path to retrieve the DB file name
	ConsoleWrite("$assocDBFName :" & $assocDBFName[3] & @CRLF)
	ControlClick($disDbDialog,"",$cancelDisBTNctrlID)						; Click on 'Cancel' button in Dissociate database window
EndIf
if $assocDBFName[3]="testDBE_01.dbf" Then
	_ExcelWriteCell($oExcel, "Pass", 14, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 14, 2)
EndIf

ConsoleWrite("****End : TS_DBE_09.au3****"&@CRLF)
ConsoleWrite(@CRLF)
