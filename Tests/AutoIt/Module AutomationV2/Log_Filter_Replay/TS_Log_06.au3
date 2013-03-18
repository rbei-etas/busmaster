;=== Test Cases/Test Data ===
; Module			:		Logging
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_06
; TestCases			:		Start of logging on reception of a particular message ID
; Test Strategy		:		Black Box
; Test Data			:		-
; === Test Procedure ===

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_06.au3****"&@CRLF)

_launchApp()
if winexists($WIN_BUSMASTER) then

	$GetLogFile_Path=_SetOneFolderUp()												; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	$Msg100Rx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",16)				; Read the 16th line from the Log file

	$HexMsgID100Rx=StringSplit($Msg100Rx," ")

EndIf
consolewrite("$Msg100Rx :"&$Msg100Rx&@CRLF)
consolewrite("$HexMsgID_Dir :"&$HexMsgID100Rx[2]&@CRLF)							; Fetch the Direction
consolewrite("$HexMsgID100 :"&$HexMsgID100Rx[4]&@CRLF)							; Fetch the Hex Msg ID

if $HexMsgID100Rx[2]="Rx" and $HexMsgID100Rx[4]=0x100 then
	_ExcelWriteCell($oExcel, "Pass", 18, 2)
Else
	_ExcelWriteCell($oExcel, "Fail", 18, 2)
EndIf

ConsoleWrite("****End : TS_Log_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)