;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; TestCase No.:		TC_01
; TestCases:		Create or load configuration(cfx) files
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********UseCase 1 Script Execution Started************"&@CRLF)
ConsoleWrite("****Start : TC_UC1_01.au3****"&@CRLF)
_launchApp()													; invoke the application and run the Process
sleep(1000)
if WinWaitActive("MCNet","",1) Then
	ControlClick("MCNet","OK","[CLASS:Button; INSTANCE:1]")
	sleep(1000)
	if $crashRes=1 Then
		_writeCrashRes(6,10)
	Endif
EndIf
if WinExists("BUSMASTER") then
	_loadConfig("UseCase1.cfx")									; load cfx file
	ConsoleWrite("isAppCrash value="&$crashRes&@CRLF)
EndIf

ConsoleWrite("****End : TC_UC1_01.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)

