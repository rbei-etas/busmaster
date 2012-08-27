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

sleep(1000)
if winexists("BUSMASTER") then
	_loadConfig("UseCase2.cfx")
	ConsoleWrite("isAppCrash value="&$crashRes&@CRLF)
EndIf
ConsoleWrite("$funcRes : "&$funcRes&@CRLF)

ConsoleWrite("****End : TC_UC2_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)

