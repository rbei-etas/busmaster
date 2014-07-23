;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_26
; TestCases			:		Using the Filter Attributes
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;~ 							2. Make a filter with the following attributes:
;~ 							3. Choose 'ID' radio box in 'Filter Attributes' group box.
;~ 							4. Select an 'ID' from the combobox or manually enter it.
;~ 							5. Choose 'ID Type' as 'All'.
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
ConsoleWrite("****Start : TS_Log_26.au3****"&@CRLF)


_launchApp()

Local $EndOfLog=""
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLog_26")															; Load Configuration

	_ConfigureCANLog("LOG_26_Sample","System","ALL","Decimal","Overwrite","","","True")	; Configure Logging details

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

if _FileCountLines($GetLogFile_Path & "\Log_26_Sample.log")=17 Then
	$EndOfLog=FileReadLine ($GetLogFile_Path & "\LOG_26_Sample.log",17)						; Read the 17th line from the Log file
	consolewrite("$EndOfLog :"&$EndOfLog&@CRLF)
EndIf

if $EndOfLog="***[STOP LOGGING SESSION]***" Then
	_WriteResult("Pass","TS_Log_26")
Else
	_WriteResult("Fail","TS_Log_26")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_26")
EndIf

ConsoleWrite("****End : TS_Log_26.au3****"&@CRLF)
ConsoleWrite(@CRLF)