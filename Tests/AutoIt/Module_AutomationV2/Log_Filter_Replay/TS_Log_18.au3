;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_18
; TestCases			:		Using the Filter Attributes
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;~ 							2. Make a filter with the following attributes:
;~ 							3. Choose 'ID' radio box in 'Filter Attributes' group box.
;~ 							4. Select an 'ID' from the combobox or manually enter it.
;~ 							5. Choose 'ID Type' as Exd'.
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
ConsoleWrite("****Start : TS_Log_18.au3****"&@CRLF)


_launchApp()

Dim $MsgIDTx1Split[10]=["","","","","","","","","",""],$MsgIDTx2Split[10]=["","","","","","","","","",""],$MsgIDTx3Split[10]=["","","","","","","","","",""],$MsgIDTx4Split[10]=["","","","","","","","","",""]
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLog_18")															; Load Configuration

	_ConfigureCANLog("LOG_18_Sample","System","ALL","Decimal","Overwrite","","","True")	; Configure Logging details

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

if _FileCountLines($GetLogFile_Path & "\Log_18_Sample.log")>=19 Then
	$MsgIDTx1=FileReadLine ($GetLogFile_Path & "\LOG_18_Sample.log",16)						; Read the 16th line from the Log file
	consolewrite("$MsgIDTx1 :"&$MsgIDTx1&@CRLF)
	$MsgIDTx1Split=StringSplit($MsgIDTx1," ")
	consolewrite("FirstTxMsgDir :"&$MsgIDTx1Split[2]&@CRLF)									; Write the Msg Direction
	consolewrite("FirstTxMsgCh :"&$MsgIDTx1Split[3]&@CRLF)									; Write the Channel
	consolewrite("FirstTxMsgID :"&$MsgIDTx1Split[4]&@CRLF)									; Write the MsgID
	consolewrite("FirstTxMsgType :"&$MsgIDTx1Split[5]&@CRLF)								; Write the Msg type

	$MsgIDTx2=FileReadLine ($GetLogFile_Path & "\LOG_18_Sample.log",17)						; Read the 17th line from the Log file
	consolewrite("$MsgIDTx2 :"&$MsgIDTx2&@CRLF)
	$MsgIDTx2Split=StringSplit($MsgIDTx2," ")
	consolewrite("SecondTxMsgDir :"&$MsgIDTx2Split[2]&@CRLF)								; Write the Msg Direction
	consolewrite("SecondTxMsgCh :"&$MsgIDTx2Split[3]&@CRLF)									; Write the Channel
	consolewrite("SecondTxMsgID :"&$MsgIDTx2Split[4]&@CRLF)									; Write the MsgID
	consolewrite("SecondTxMsgType :"&$MsgIDTx2Split[5]&@CRLF)								; Write the Msg type

	$MsgIDTx3=FileReadLine ($GetLogFile_Path & "\LOG_18_Sample.log",18)						; Read the 18th line from the Log file
	consolewrite("$MsgIDTx3 :"&$MsgIDTx3&@CRLF)
	$MsgIDTx3Split=StringSplit($MsgIDTx3," ")
	consolewrite("ThirdTxMsgDir :"&$MsgIDTx3Split[2]&@CRLF)									; Write the Msg Direction
	consolewrite("ThirdTxMsgCh :"&$MsgIDTx3Split[3]&@CRLF)									; Write the Channel
	consolewrite("ThirdTxMsgID :"&$MsgIDTx3Split[4]&@CRLF)									; Write the MsgID
	consolewrite("ThirdTxMsgType :"&$MsgIDTx3Split[5]&@CRLF)								; Write the Msg type

	$MsgIDTx4=FileReadLine ($GetLogFile_Path & "\LOG_18_Sample.log",19)						; Read the 19th line from the Log file
	consolewrite("$MsgIDTx4 :"&$MsgIDTx4&@CRLF)
	$MsgIDTx4Split=StringSplit($MsgIDTx4," ")
	consolewrite("FourthTxMsgDir :"&$MsgIDTx4Split[2]&@CRLF)								; Write the MsgID
	consolewrite("FourthTxMsgCh :"&$MsgIDTx4Split[3]&@CRLF)									; Write the Msg type
	consolewrite("FourthTxMsgID :"&$MsgIDTx4Split[4]&@CRLF)									; Write the MsgID
	consolewrite("FourthTxMsgType :"&$MsgIDTx4Split[5]&@CRLF)								; Write the Msg type
EndIf

if $MsgIDTx1Split[2]="Tx" and $MsgIDTx2Split[2]="Tx" and $MsgIDTx3Split[2]="Tx" and $MsgIDTx4Split[2]="Tx" then
	$ExtMsgDir="Found"
Else
	$ExtMsgDir="Not Found"
EndIf

consolewrite("$ExtMsgDir :"&$ExtMsgDir&@CRLF)

if $MsgIDTx1Split[3]=1 and $MsgIDTx2Split[3]=1 and $MsgIDTx3Split[3]=1 and $MsgIDTx4Split[3]=1 then
	$ExtMsgCh="Found"
Else
	$ExtMsgCh="Not Found"
EndIf

consolewrite("$ExtMsgCh :"&$ExtMsgCh&@CRLF)

if $MsgIDTx1Split[4]=0259 and $MsgIDTx2Split[4]=0259 and $MsgIDTx3Split[4]=0259 and $MsgIDTx4Split[4]=0259 then
	$ExtMsgID="Found"
Else
	$ExtMsgID="Not Found"
EndIf

consolewrite("$ExtMsgID :"&$ExtMsgID&@CRLF)

if $MsgIDTx1Split[5]="x" and $MsgIDTx2Split[5]="x" and $MsgIDTx3Split[5]="x" and $MsgIDTx4Split[5]="x" then
	$ExtMsgType="Found"
Else
	$ExtMsgType="Not Found"
EndIf

consolewrite("$ExtMsgType :"&$ExtMsgType&@CRLF)

if $ExtMsgDir="Found" and $ExtMsgCh="Found" and $ExtMsgID="Found" and $ExtMsgType="Found" Then
	_WriteResult("Pass","TS_Log_18")
Else
	_WriteResult("Fail","TS_Log_18")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_18")
EndIf

ConsoleWrite("****End : TS_Log_18.au3****"&@CRLF)
ConsoleWrite(@CRLF)