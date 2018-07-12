;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_06
; TestCases			:		Start of logging on reception of a particular message ID
; Test Data			:
; Test Setup		:		1. Select Configure->Log.
;~ 							2. A Dialog Box will appear.
;~ 							3. Select a log file name
;~ 							4. Select button for Message List from Database
;~ 							5. A Dialog box with message List will be displayed.
;~ 							6. Select message name "Test"
;~ 							7. Select "Select" button
;~ 							8. Select " OK" button in parent Dailog
;~ 							9. Connect the tool by pressing connect tool bar button
;~ 							10. Start Logging by selecting 'Functions -> Start Logging' or tool bar button for logging
;~ 							11. Send a message with message ID=0xff along with some other message from other node ( CANoe ) connect to CAN-Monitor
; Expected Results  : 		1. Message ID edit control should be disabled
;~ 							2.Logging will start after reception of message ID=0xff
;~ 							3. Stop logging and open the log file in text mode and the make sure that the message id 0xff is the first entry in the log file

; Critical (C)		:		Y
; TestCase No.		:		TS_Log_07
; TestCases			:		Start of logging on reception of a particular message ID defined in database
; Test Data			:
; Test Setup		:		1. Select "Configure->Log".
;~ 							2. A Dialog Box will appear.
;~ 							3. Select a log file name
;~ 							4. Enter a message ID
;~ 							5. Select "OK"
;~ 							6. Select "File -> Connect" menu option or select "Connect"  toolbar button
;~ 							7. Start Logging by selecting "Functions -> Start Logging" or tool bar button for logging
;~ 							8. Send a message with message ID=0xff along with some other message from other node ( CANoe ) connect to CAN-Monitor.
;~ 							9. Stop logging after few seconds
; Expected Results  : 		1. Message name edit control and associated select button should be disabled
;~ 							2.Logging will start after reception of message ID=0xff
;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_06/07.au3****"&@CRLF)

_launchApp()

Dim $HexMsgID100Rx[10]=["","","","","","","","","",""]
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLog_06_07")																; Load Configuration
	$GetLogFile_Path=_OutputDataPath()														; Fetch the Log file path
	ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

	if _FileCountLines($GetLogFile_Path & "\Log_04_Sample.log")>=16 Then
		$Msg100Rx=FileReadLine ($GetLogFile_Path & "\Log_04_Sample.log",16)						; Read the 16th line from the Log file

		$HexMsgID100Rx=StringSplit($Msg100Rx," ")
	EndIf
EndIf
consolewrite("$Msg100Rx :"&$Msg100Rx&@CRLF)
consolewrite("$HexMsgID_Dir :"&$HexMsgID100Rx[2]&@CRLF)										; Fetch the Direction
consolewrite("$HexMsgID100 :"&$HexMsgID100Rx[4]&@CRLF)										; Fetch the Hex Msg ID

if $HexMsgID100Rx[2]="Rx" and $HexMsgID100Rx[4]=0x100 then
	_WriteResult("Pass","TS_Log_06")

	_WriteResult("Pass","TS_Log_07")
Else
	_WriteResult("Fail","TS_Log_06")

	_WriteResult("Fail","TS_Log_07")
EndIf

$isAppNotRes=_CloseApp()																	; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_06")
	_WriteResult("Warning","TS_Log_07")
EndIf


ConsoleWrite("****End : TS_Log_06/07.au3****"&@CRLF)
ConsoleWrite(@CRLF)