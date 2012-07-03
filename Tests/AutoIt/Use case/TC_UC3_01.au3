;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; TestCase No.:		TC_01
; TestCases:		Create or load configuration(cfx) files
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("***********UseCase 3 Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TC_UC3_01.au3****"&@CRLF)
_launchApp()													; invoke the application and run the Process
sleep(2000)
if WinExists("BUSMASTER") then
	_loadConfig("UseCase1.cfx")
EndIf
ConsoleWrite("****End : TC_UC3_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)