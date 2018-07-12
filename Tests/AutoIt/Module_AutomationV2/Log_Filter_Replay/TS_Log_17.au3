;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_17
; TestCases			:		Using the Filter Attributes
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;~ 							2. Make a filter with the following attributes:
;~ 							3. Choose 'ID' radio box in 'Filter Attributes' group box.
;~ 							4. Select an 'ID' from the combobox or manually enter it.
;~ 							5. Choose 'ID Type' as 'std'.
;~ 							6. Choose 'Frame' as 'All'.
;~ 							7. Choose 'Direction' as 'All'.
;~ 							8. Choose 'Channel' as 'All'.
;~ 							9. Add the filter by clicking on Add button and close the 'Configure Filters' dialog by clicking on 'OK'.
;~ 							10. Open 'ConfigureLogging for CAN' window by menu 'Configure->Log' and add a new log file.
;~ 							11. Choose the configured filter by opening 'Filter Selection dialog' and close the  'ConfigureLogging for CAN' window by 'Ok' click.
;~ 							12. Now click on connect and enable logging by menu 'Functions->Enable Filter->Log'.
;~ 							13. Transmit a message which matches the filter attributes set for configured filter. Observe the log file contents after transmission.

; Expected Results  : 		1. The log file should not contain any messages which matches the filter attributes configured for this log file.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Log_17.au3****"&@CRLF)


_launchApp()

Dim $MsgIDTx1Split[10]=["","","","","","","","","",""],$MsgIDRx1Split[10]=["","","","","","","","","",""],$MsgIDTx2Split[10]=["","","","","","","","","",""],$MsgIDRx2Split[10]=["","","","","","","","","",""]
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLog_17")															; Load Configuration

	_ConfigureCANLog("LOG_17_Sample","System","ALL","Decimal","Overwrite","","","True")	; Configure Logging details

	_EnableFilterLog()
	sleep(1000)

	_ConnectDisconnect()																; Connect the tool

	_TransmitMsgsMenu()																	; Transmit normal blocks
	sleep(1500)

	_ConnectDisconnect()																; Disconnect the tool

	Sleep(2000)
	_DisableJ1939Win()																	; If J1939 Msg Window is active then disable it

EndIf

$GetLogFile_Path=_OutputDataPath()														; Fetch the Log file path
ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

if _FileCountLines($GetLogFile_Path & "\Log_17_Sample.log")>=19 Then
	$MsgIDTx1=FileReadLine ($GetLogFile_Path & "\LOG_17_Sample.log",16)						; Read the 16th line from the Log file
	consolewrite("$MsgIDTx1 :"&$MsgIDTx1&@CRLF)
	$MsgIDTx1Split=StringSplit($MsgIDTx1," ")
	consolewrite("FirstTxMsgID :"&$MsgIDTx1Split[4]&@CRLF)									; Write the MsgID
	consolewrite("FirstTxMsgType :"&$MsgIDTx1Split[5]&@CRLF)								; Write the Msg type

	$MsgIDRx1=FileReadLine ($GetLogFile_Path & "\LOG_17_Sample.log",17)						; Read the 17th line from the Log file
	consolewrite("$MsgIDRx1 :"&$MsgIDRx1&@CRLF)
	$MsgIDRx1Split=StringSplit($MsgIDRx1," ")
	consolewrite("FirstRxMsgID :"&$MsgIDRx1Split[4]&@CRLF)									; Write the MsgID
	consolewrite("FirstRxMsgType :"&$MsgIDRx1Split[5]&@CRLF)								; Write the Msg type

	$MsgIDTx2=FileReadLine ($GetLogFile_Path & "\LOG_17_Sample.log",18)						; Read the 18th line from the Log file
	consolewrite("$MsgIDTx2 :"&$MsgIDTx2&@CRLF)
	$MsgIDTx2Split=StringSplit($MsgIDTx2," ")
	consolewrite("SecondTxMsgID :"&$MsgIDTx2Split[4]&@CRLF)									; Write the MsgID
	consolewrite("SecondTxMsgType :"&$MsgIDTx2Split[5]&@CRLF)								; Write the Msg type

	$MsgIDRx2=FileReadLine ($GetLogFile_Path & "\LOG_17_Sample.log",19)						; Read the 19th line from the Log file
	consolewrite("$MsgID103Rx2 :"&$MsgIDRx2&@CRLF)
	$MsgIDRx2Split=StringSplit($MsgIDRx2," ")
	consolewrite("SecondRxMsgID :"&$MsgIDRx2Split[4]&@CRLF)									; Write the MsgID
	consolewrite("SecondRxMsgType :"&$MsgIDRx2Split[5]&@CRLF)								; Write the Msg type
EndIf

if $MsgIDTx1Split[4]=0259 and $MsgIDRx1Split[4]=0259 and $MsgIDTx2Split[4]=0259 and $MsgIDRx2Split[4]=0259 then
	$ExtMsgID="Found"
Else
	$ExtMsgID="Not Found"
EndIf

consolewrite("$ExtMsgID :"&$ExtMsgID&@CRLF)

if $MsgIDTx1Split[5]="x" and $MsgIDRx1Split[5]="x" and $MsgIDTx2Split[5]="x" and $MsgIDRx2Split[5]="x" then
	$ExtMsgType="Found"
Else
	$ExtMsgType="Not Found"
EndIf

consolewrite("$ExtMsgType :"&$ExtMsgType&@CRLF)

if $ExtMsgID="Found" and $ExtMsgType="Found" Then
	_WriteResult("Pass","TS_Log_17")
Else
	_WriteResult("Fail","TS_Log_17")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_17")
EndIf

ConsoleWrite("****End : TS_Log_17.au3****"&@CRLF)
ConsoleWrite(@CRLF)