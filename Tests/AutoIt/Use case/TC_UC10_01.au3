;=== Test Cases/Test Data ===
; UseCase 9:		J1939-Command Address
; Critical (C):		Y
; TestCase No.:		TC_UC10_01
; TestCases:		Create or load configuration(cfx) files
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********UseCase 10 Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TC_UC10_01.au3****"&@CRLF)
_launchApp()													; invoke the application and run the Process
if $crashRes=1 Then
	_writeCrashRes(49,10)
Endif
sleep(1000)
if WinExists("BUSMASTER") then
	_loadConfig("UseCase4.cfx")									; load cfx file
	ConsoleWrite("isAppCrash value="&$crashRes&@CRLF)
	if $crashRes=1 Then
		_writeCrashRes(49,10)
	Endif
EndIf
if $funcRes=1 Then												; write the result to excel sheet
	_ExcelWriteCell($oExcel, "Pass", 49, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 49, 10)
EndIf
ConsoleWrite("****End : TC_UC10_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
