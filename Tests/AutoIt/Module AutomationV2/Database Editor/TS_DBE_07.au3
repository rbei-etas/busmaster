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
ConsoleWrite("****Start : TS_DBE_07.au3****"&@CRLF)

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_AssociateCANDB("testDBE_01")								; Associate DB
	_DissociateCANDB(0)											; Dissociate DB
EndIf
if winexists("",$disDBtxt) Then
	_ExcelWriteCell($oExcel, "Pass", 12, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 12, 2)
EndIf

ConsoleWrite("****End : TS_DBE_07.au3****"&@CRLF)
ConsoleWrite(@CRLF)