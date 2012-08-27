;=== Test Cases/Test Data ===
; UseCase 7:		J1939-Log
; Critical (C):		Y
; TestCase No.:		TC_UC7_01
; TestCases:		Create or load configuration(cfx) files
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********UseCase 7 Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TC_UC7_01.au3****"&@CRLF)
_launchApp()													; invoke the application and run the Process
if $crashRes=1 Then
	_writeCrashRes(35,10)
Endif
sleep(1000)
if WinExists("BUSMASTER") then
	_loadConfig("UseCase4.cfx")									; load cfx file
	ConsoleWrite("isAppCrash value="&$crashRes&@CRLF)
	if $crashRes=1 Then
		_writeCrashRes(35,10)
	Endif
EndIf
if $funcRes=1 Then												; write the result to excel sheet
	_ExcelWriteCell($oExcel, "Pass", 35, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 35, 10)
EndIf
ConsoleWrite("****End : TC_UC7_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
