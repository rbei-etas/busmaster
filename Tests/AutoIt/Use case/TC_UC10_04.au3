;=== Test Cases/Test Data ===
; UseCase 9:		J1939-Command Address
; Critical (C):		Y
; TestCase No.:		TC_UC10_04
; TestCases:		Transmit Window
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC10_04.au3****"&@CRLF)
Dim $BAM, $TPDT1, $TPDT2, $BRDCST, $BAMDB, $TPDT1DB, $TPDT2DB, $BRDCSTDB, $r1, $r2, $r3, $r4
if winexists("BUSMASTER") Then
	WinMenuSelectItem("BUSMASTER","","&J1939","&View","&Transmit Window")		; Select J1939-->View-->Transmit Window
	ControlCommand("J1939 Transmit Message Window","",1335,"Check")				; Check 'Network Management' Radio Button
	sleep(500)
	ControlCommand("J1939 Transmit Message Window","",1009,"Check")				; Check 'Command Address' Radio Button
	ControlSetText("J1939 Transmit Message Window","",1336,"AA")
	;_overWriteMode()															; Call _overWriteMode function to disable overWriteMode
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Connect")					; Select 'Connect' button
	sleep(1000)
	ControlClick("J1939 Transmit Message Window","",1011,"left")				; Click 'transmit' button
	sleep(3000)
	Send("{TAB}")
	Send("{ENTER}")
	$hWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($hWd,4,"left")
	sleep(1000)
	;ControlClick("J1939 Transmit Message Window","",1001,"left")
EndIf


ConsoleWrite("****End : TC_UC10_04.au3****"&@CRLF)
ConsoleWrite("***********UseCase 10 Script Execution Started************"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)



