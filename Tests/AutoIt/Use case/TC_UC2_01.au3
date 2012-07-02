;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; TestCase No.:		TC_01
; TestCases:		Create or load configuration(cfx) files
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("***********UseCase 2 Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TC_UC2_01.au3****"&@CRLF)
_launchApp()											; invoke the application and run the Process
ConsoleWrite("rVal : "&$rVal&@CRLF)
if $rVal=0 Then
	ConsoleWrite("app not found")
	msgbox(0,"","application not found")
	Exit
EndIf
sleep(1000)
if winexists("BUSMASTER") then
	_loadConfig("UseCase2.cfx")
	ConsoleWrite("isAppCrash value="&$crashRes&@CRLF)
	if $crashRes=1 Then
		_writeCrashRes(11,10)
	Endif
EndIf
ConsoleWrite("$funcRes : "&$funcRes&@CRLF)
if $funcRes=1 Then
	_ExcelWriteCell($oExcel, "Pass", 11, 10)
Else
	_ExcelWriteCell($oExcel, "Fail", 11, 10)
EndIf
ConsoleWrite("****End : TC_UC2_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)

