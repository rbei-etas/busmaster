;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_08
; TestCases			:		Using the Filter Attributes
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;~ 							2. Make a filter with the following attributes:
;~ 							3. Choose 'ID' radio box in 'Filter Attributes' group box.
;~ 							4. Select an 'ID' from the combobox or manually enter it.
;~ 							5. Choose 'ID Type' as 'Std'.
;~ 							6. Choose 'Frame' as 'Non RTR'.
;~ 							7. Choose 'Direction' as 'Rx'.
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
ConsoleWrite("****Start : TS_Log_08.au3****"&@CRLF)


_launchApp()

Dim $HexMsgID21Rx1[10]=["","","","","","","","","",""],$HexMsgID21Rx2[10]=["","","","","","","","","",""],$HexMsgID21Rx3[10]=["","","","","","","","","",""]
if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxLog_08")																; Load Configuration

	_ConfigureCANLog("LOG_08_Sample","System","ALL","Decimal","Overwrite","","","True")		; Configure Logging details

	_EnableFilterLog()
	sleep(1000)

	_ConnectDisconnect()																	; Connect the tool

	_TransmitMsgsMenu()																		; Transmit normal blocks
	sleep(3000)

	_ConnectDisconnect()																	; Disconnect the tool

	Sleep(2000)
	_DisableJ1939Win()																		; If J1939 Msg Window is active then disable it

EndIf

$GetLogFile_Path=_OutputDataPath()															; Fetch the Log file path
ConsoleWrite("$GetLogFile_Path:"&$GetLogFile_Path&@CRLF)

if _FileCountLines($GetLogFile_Path & "\Log_08_Sample.log")>=21 Then
	$FindRx1=FileReadLine ($GetLogFile_Path & "\LOG_08_Sample.log",17)							; Read the 17th line from the Log file
	consolewrite("$FindRx1 :"&$FindRx1&@CRLF)
	$HexMsgID21Rx1=StringSplit($FindRx1," ")
	consolewrite("$HexMsgID21Rx1_Dir :"&$HexMsgID21Rx1[2]&@CRLF)								; Fetch the Direction
	consolewrite("$HexMsgID21Rx1_ID :"&$HexMsgID21Rx1[4]&@CRLF)									; Fetch the Hex Msg ID

	$FindRx2=FileReadLine ($GetLogFile_Path & "\LOG_08_Sample.log",19)							; Read the 19th line from the Log file
	consolewrite("$FindRx2 :"&$FindRx2&@CRLF)
	$HexMsgID21Rx2=StringSplit($FindRx2," ")
	consolewrite("$HexMsgID21Rx2_Dir :"&$HexMsgID21Rx2[2]&@CRLF)								; Fetch the Direction
	consolewrite("$HexMsgID21Rx2_ID :"&$HexMsgID21Rx2[4]&@CRLF)									; Fetch the Hex Msg ID

	$FindRx3=FileReadLine ($GetLogFile_Path & "\LOG_08_Sample.log",21)							; Read the 21th line from the Log file
	consolewrite("$FindRx3 :"&$FindRx3&@CRLF)
	$HexMsgID21Rx3=StringSplit($FindRx3," ")
	consolewrite("$HexMsgID21Rx3_Dir :"&$HexMsgID21Rx3[2]&@CRLF)								; Fetch the Direction
	consolewrite("$HexMsgID21Rx3_ID :"&$HexMsgID21Rx3[4]&@CRLF)									; Fetch the Hex Msg ID
EndIf


if $HexMsgID21Rx1[2]="Tx" and $HexMsgID21Rx2[2]="Tx" and $HexMsgID21Rx3[2]="Tx" then
	$RxAllDir="Found"
Else
	$RxAllDir="Not Found"
EndIf

consolewrite("$RxAllDir :"&$RxAllDir&@CRLF)

if $HexMsgID21Rx1[4]=0021 and $HexMsgID21Rx2[4]=0021 and $HexMsgID21Rx3[4]=0021 then
	$RxAllID="Found"
Else
	$RxAllID="Not Found"
EndIf

consolewrite("$RxAllID :"&$RxAllID&@CRLF)

if $RxAllDir="Found" and $RxAllID="Found" Then													; Validate the result
	_WriteResult("Pass","TS_Log_08")
Else
	_WriteResult("Fail","TS_Log_08")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_08")
EndIf


ConsoleWrite("****End : TS_Log_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)