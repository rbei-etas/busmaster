;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_J1939Log_06
; TestCases			:		Start of logging on reception of a particular message ID
; Test Data			:
; Test Setup		:		"1. Select Configure->Log.
;~ 							2. A Dialog Box will appear.
;~ 							3. Select a log file name
;~ 							4. Select button for Message List from Database
;~ 							5. A Dialog box with message List will be displayed.
;~ 							6. Select message name "Test"
;~ 							7. Select "Select" button
;~ 							8. Select " OK" button in parent Dailog
;~ 							9. Connect the tool by pressing connect tool bar button
;~ 							10. Start Logging by selecting 'Functions -> Start Logging' or tool bar button for logging
;~ 							11. Send a message with message ID=0xff along with some other message from other node ( CANoe ) connect to CAN-Monitor."

; Expected Results  : 		"1. Message ID edit control should be disabled
;~ 							2.Logging will start after reception of message ID=0xff
;~ 							3. Stop logging and open the log file in text mode and the make sure that the message id 0xff is the first entry in the log file"
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_J1939Log_06.au3****"&@CRLF)


_launchApp()

WinActivate($WIN_BUSMASTER,3)
Local $All_Dir=0, $All_PGNID=0
if winexists($mWin) then
	_loadConfig("cfxJ1939Log_06")																		; Load Configuration

	_ActivateJ1939()																					; Activate J1939
	sleep(1000)

	_ConfigureJ1939Log("LogtestJ1939_06","System","ALL","Hex","Overwrite",200,500,"False")				; Configure Logging details

	_ConnectDisconnect()																				; Connect
	sleep(1500)

	_J1939tMsgWin("Data","[0x200]XYZ","","")																	; Transmit "data" message

	_J1939tMsgWin("Data","[0x400]ABC","","")																	; Transmit "data" message

	_ConnectDisconnect()																				; Disconnect
	sleep(1500)

	_DeactJ1939()																						; Deactivate J1939

	$GetLogFile_Path=_OutputDataPath()																	; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	$Count=_FileCountLines($GetLogFile_Path & "\LogtestJ1939_06.log")									; fetch the number of lines in the log file
	ConsoleWrite("$Count:"&$Count&@CRLF)

	if $Count=20 Then

		$Msg200Rx=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_06.log",16)								; Read the 16th line from the Log file
		consolewrite("$Msg200Rx :"&$Msg200Rx&@CRLF)
		$HexMsgID200Rx=StringSplit($Msg200Rx," ")
		consolewrite("$HexMsgID_200 :"&$HexMsgID200Rx[4]&@CRLF)												; Fetch the Direction
		consolewrite("$Hex200_Rx :"&$HexMsgID200Rx[11]&@CRLF)												; Fetch the Hex Msg ID

		$Msg400Tx=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_06.log",17)								; Read the 17th line from the Log file
		consolewrite("$Msg400Tx :"&$Msg400Tx&@CRLF)
		$HexMsgID400Tx=StringSplit($Msg400Tx," ")
		consolewrite("$HexMsgID_400 :"&$HexMsgID400Tx[4]&@CRLF)												; Fetch the Direction
		consolewrite("$Hex400_Tx :"&$HexMsgID400Tx[11]&@CRLF)												; Fetch the Hex Msg ID

		$Msg400Rx=FileReadLine ($GetLogFile_Path & "\LogtestJ1939_06.log",18)								; Read the 18th line from the Log file
		consolewrite("$Msg400Rx :"&$Msg400Rx&@CRLF)
		$HexMsgID400Rx=StringSplit($Msg400Rx," ")
		consolewrite("$HexMsgID_400 :"&$HexMsgID400Rx[4]&@CRLF)												; Fetch the Direction
		consolewrite("$Hex400_Rx :"&$HexMsgID400Rx[11]&@CRLF)												; Fetch the Hex Msg ID

		if $HexMsgID200Rx[11]="Rx" and $HexMsgID400Tx[11]="Tx" and $HexMsgID400Rx[11]="Rx" Then
			$All_Dir=1
		EndIf

		if $HexMsgID200Rx[4]="0x000200" and $HexMsgID400Tx[4]="0x000400" and $HexMsgID400Rx[4]="0x000400" Then
			$All_PGNID=1
		EndIf
	EndIf

EndIf

consolewrite("$All_Dir :"&$All_Dir&@CRLF)
consolewrite("$All_PGNID :"&$All_PGNID&@CRLF)

if $All_Dir=1 and $All_PGNID=1 Then
	_WriteResult("Pass","TS_J1939Log_06")
Else
	_WriteResult("Fail","TS_J1939Log_06")
EndIf

$isAppNotRes=_CloseApp()																				; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_J1939Log_06")
EndIf



ConsoleWrite("****End : TS_J1939Log_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)
