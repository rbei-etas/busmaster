;=== Test Cases/Test Data ===
; UseCase 1:		transmit J1939 Message Transmission - Data
; Critical (C):		Y
; TestCase No.:		TC_UC5_04
; TestCases:		Transmit Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC5_04.au3****"&@CRLF)
ReDim $Tx[1000],$Rx[1000]
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
	_loadJ1939Database("J1939Test",25,10,8)
	_associateDB("&J1939","J1939Test.dbf")
EndIf
_J1939tMsgWin("Data")
$hWd=ControlGetHandle("BUSMASTER","",128)
_GUICtrlToolbar_ClickIndex($hWd,4,"left")
sleep(1000)

ConsoleWrite("****End : TC_UC5_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
