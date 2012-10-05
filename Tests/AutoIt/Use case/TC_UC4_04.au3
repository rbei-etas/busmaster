;=== Test Cases/Test Data ===
; UseCase 1:		transmit J1939 Message Transmission - Request PGN
; Critical (C):		Y
; TestCase No.:		TC_UC4_03
; TestCases:		Transmit Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC4_04.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")

	_loadJ1939Database("J1939Test",25,10,8)
	_associateDB("&J1939","J1939Test.dbf")
EndIf
_J1939tMsgWin("Request PGN")
_J1939tMsgWin("Data")
_J1939tMsgWin("Broadcast")
;~ send("!jt")								; J1939 Deactivate
$hWd=ControlGetHandle("BUSMASTER","",128)
_GUICtrlToolbar_ClickIndex($hWd,4,"left")	; Disconnect
sleep(1000)

ConsoleWrite("****End : TC_UC4_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
