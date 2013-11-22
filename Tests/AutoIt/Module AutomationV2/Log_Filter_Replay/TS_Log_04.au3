;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_04
; TestCases			:		Functionality test - Trigger
; Test Data			:
; Test Setup		:		1. Continue from the previous test case.
;~ 							2. Select the check box "Start logging on reception of" and give the ID 100 as start trigger.
;~ 							3. Similarly check stop trigger and give 200 as stop trigger.
;~ 							4. Select OK to save log file configuration changes.
;~ 							5. Select TX messages from Configure menu.
;~ 							6. Disable all existing blocks and add a new message block.
;~ 							7. Add messages with the following message IDs: 50,100,150,200,250,300. All ID are in Hex.
;~ 							8. Make this block as cyclic and enable the block.
;~ 							9. Connect the tool and start message transmission.
;~ 							10. After 2-3 cycles of message transmission, disconnect the tool.
;~ 							11. Open the log file and check the content

; Expected Results  : 		1. After step 11, the log file will contain on the messages starting from ID 100 to ID 200 for the first cycle only.
;~ 								Once the ID 200 is received the logging will be stopped and further cycle of message transmission will not be logged.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_04.au3****"&@CRLF)

_launchApp()

Dim $HexMsgID100Rx[10]=["","","","","","","","","",""],$HexMsgID150Tx[10]=["","","","","","","","","",""],$HexMsgID150Rx[10]=["","","","","","","","","",""],$HexMsgID200Rx[10]=["","","","","","","","","",""]
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLog_04")															; Create New Configuration

	_ConfigureCANLog("Log_04_Sample","System","ALL","Hex","Overwrite",100,200,"False")	; Configure Logging details

	_ConnectDisconnect()																; Connect the tool

	_TransmitMsgsMenu()																	; Transmit normal blocks
	sleep(5000)

	_ConnectDisconnect()																; Disconnect the tool

	$GetLogFile_Path=_OutputDataPath()													; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	if _FileCountLines($GetLogFile_Path & "\Log_04_Sample.log")>=21 Then

		$Msg100Rx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",16)					; Read the 16th line from the Log file
		consolewrite("$Msg100Rx :"&$Msg100Rx&@CRLF)
		$HexMsgID100Rx=StringSplit($Msg100Rx," ")
		consolewrite("$HexMsgID_Dir :"&$HexMsgID100Rx[2]&@CRLF)								; Fetch the Direction
		consolewrite("$HexMsgID100 :"&$HexMsgID100Rx[4]&@CRLF)								; Fetch the Hex Msg ID

		$Msg150Tx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",17)					; Read the 17th line from the Log file
		consolewrite("$Msg150Tx :"&$Msg150Tx&@CRLF)
		$HexMsgID150Tx=StringSplit($Msg150Tx," ")
		consolewrite("$HexMsgID_Dir :"&$HexMsgID150Tx[2]&@CRLF)								; Fetch the Direction
		consolewrite("$HexMsgID150 :"&$HexMsgID150Tx[4]&@CRLF)								; Fetch the Hex Msg ID

		$Msg150Rx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",18)					; Read the 18th line from the Log file
		consolewrite("$Msg150Rx :"&$Msg150Rx&@CRLF)
		$HexMsgID150Rx=StringSplit($Msg150Rx," ")
		consolewrite("$HexMsgID_Dir :"&$HexMsgID150Rx[2]&@CRLF)								; Fetch the Direction
		consolewrite("$HexMsgID150 :"&$HexMsgID150Rx[4]&@CRLF)								; Fetch the Hex Msg ID

		$Msg200Tx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",19)					; Read the 19th line from the Log file
		consolewrite("$Msg200Tx :"&$Msg200Tx&@CRLF)
		$HexMsgID200Tx=StringSplit($Msg200Tx," ")
		consolewrite("$HexMsgID_Dir :"&$HexMsgID200Tx[2]&@CRLF)								; Fetch the Direction
		consolewrite("$HexMsgID200 :"&$HexMsgID200Tx[4]&@CRLF)								; Fetch the Hex Msg ID

		$Msg200Rx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",20)					; Read the 20th line from the Log file
		consolewrite("$Msg200Tx :"&$Msg200Rx&@CRLF)
		$HexMsgID200Rx=StringSplit($Msg200Rx," ")
		consolewrite("$HexMsgID_Dir :"&$HexMsgID200Rx[2]&@CRLF)								; Fetch the Direction
		consolewrite("$HexMsgID200 :"&$HexMsgID200Rx[4]&@CRLF)								; Fetch the Hex Msg ID

		$EndOfFile=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",21)				; Read the 21th line from the Log file
		consolewrite("$EndOfFile :"&$EndOfFile&@CRLF)
		$FindEOF=StringSplit($EndOfFile," ")
		consolewrite("$FindEOF:"&$FindEOF[1]&@CRLF)											; Fetch the Direction
	EndIf

EndIf
if $HexMsgID100Rx[2]="Rx" and $HexMsgID150Tx[2]="Tx" and $HexMsgID150Rx[2]="Rx" and $HexMsgID200Tx[2]="Tx" and $HexMsgID200Rx[2]="Rx" Then
	$AllMsgDirection="Success"
Else
	$AllMsgDirection="UnSuccess"
EndIf
if $HexMsgID100Rx[4]=0x100 and $HexMsgID150Tx[4]=0x150 and $HexMsgID150Rx[4]=0x150 and $HexMsgID200Tx[4]=0x200 and $HexMsgID200Rx[4]=0x200 Then
	$AllMsgID="Success"
Else
	$AllMsgID="UnSuccess"
EndIf

consolewrite("$AllMsgDirection:"&$AllMsgDirection&@CRLF)
consolewrite("$AllMsgID:"&$AllMsgID&@CRLF)
consolewrite("$FindEOF:"&$FindEOF[1]&@CRLF)

if $AllMsgDirection="Success" and $AllMsgID="Success" and $FindEOF[1]="***END" Then			; Validate the result
	_WriteResult("Pass","TS_Log_04")
Else
	_WriteResult("Fail","TS_Log_04")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_04")
EndIf


ConsoleWrite("****End : TS_Log_04.au3****"&@CRLF)
ConsoleWrite(@CRLF)