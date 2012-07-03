;=== Test Cases/Test Data ===
; UseCase 1:		transmit J1939 Message Transmission - Request PGN
; Critical (C):		Y
; TestCase No.:		TC_UC4_01
; TestCases:		Create or load configuration(cfx) files
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********UseCase 4 Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TC_UC4_01.au3****"&@CRLF)
_launchApp()													; invoke the application and run the Process
sleep(1000)
if WinExists("BUSMASTER") then
	_loadConfig("UseCase4.cfx")									; load cfx file
	ConsoleWrite("isAppCrash value="&$crashRes&@CRLF)
EndIf
ConsoleWrite("****End : TC_UC4_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
