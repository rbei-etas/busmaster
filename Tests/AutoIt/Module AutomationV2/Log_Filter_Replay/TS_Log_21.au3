;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Log_21
; TestCases			:		Using the Filter Attributes
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;~ 							2. Make a filter with the following attributes:
;~ 							3. Choose 'ID' radio box in 'Filter Attributes' group box.
;~ 							4. Select an 'ID' from the combobox or manually enter it.
;~ 							5. Choose 'ID Type' as 'All'.
;~ 							6. Choose 'Frame' as 'All'.
;~ 							7. Choose 'Direction' as 'Tx'.
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
ConsoleWrite("****Start : TS_Log_21.au3****"&@CRLF)


_launchApp()

Dim $MsgIDRx1Split[10]=["","","","","","","","","",""],$MsgIDRx2Split[10]=["","","","","","","","","",""],$MsgIDRx3Split[10]=["","","","","","","","","",""],$MsgIDRx4Split[10]=["","","","","","","","","",""]

if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxLog_21")															; Load Configuration

	_ConfigureCANLog("LOG_21_Sample","System","ALL","Decimal","Overwrite","","","True")	; Configure Logging details

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

if _FileCountLines($GetLogFile_Path & "\Log_21_Sample.log")>=19 Then
	$MsgIDRx1=FileReadLine ($GetLogFile_Path & "\LOG_21_Sample.log",16)						; Read the 16th line from the Log file
	consolewrite("$MsgIDRx1 :"&$MsgIDRx1&@CRLF)
	$MsgIDRx1Split=StringSplit($MsgIDRx1," ")
	consolewrite("FirstRxMsgDir :"&$MsgIDRx1Split[2]&@CRLF)									; Write the Msg Direction
	consolewrite("FirstRxMsgCh :"&$MsgIDRx1Split[3]&@CRLF)									; Write the Channel
	consolewrite("FirstRxMsgID :"&$MsgIDRx1Split[4]&@CRLF)									; Write the MsgID
	consolewrite("FirstRxMsgType :"&$MsgIDRx1Split[5]&@CRLF)								; Write the Msg type

	$MsgIDRx2=FileReadLine ($GetLogFile_Path & "\LOG_21_Sample.log",17)						; Read the 17th line from the Log file
	consolewrite("$MsgIDRx2 :"&$MsgIDRx2&@CRLF)
	$MsgIDRx2Split=StringSplit($MsgIDRx2," ")
	consolewrite("SecondRxMsgDir :"&$MsgIDRx2Split[2]&@CRLF)								; Write the Msg Direction
	consolewrite("SecondRxMsgCh :"&$MsgIDRx2Split[3]&@CRLF)									; Write the Channel
	consolewrite("SecondRxMsgID :"&$MsgIDRx2Split[4]&@CRLF)									; Write the MsgID
	consolewrite("SecondRxMsgType :"&$MsgIDRx2Split[5]&@CRLF)								; Write the Msg type

	$MsgIDRx3=FileReadLine ($GetLogFile_Path & "\LOG_21_Sample.log",18)						; Read the 18th line from the Log file
	consolewrite("$MsgIDRx3 :"&$MsgIDRx3&@CRLF)
	$MsgIDRx3Split=StringSplit($MsgIDRx3," ")
	consolewrite("ThirdRxMsgDir :"&$MsgIDRx3Split[2]&@CRLF)									; Write the Msg Direction
	consolewrite("ThirdRxMsgCh :"&$MsgIDRx3Split[3]&@CRLF)									; Write the Channel
	consolewrite("ThirdRxMsgID :"&$MsgIDRx3Split[4]&@CRLF)									; Write the MsgID
	consolewrite("ThirdRxMsgType :"&$MsgIDRx3Split[5]&@CRLF)								; Write the Msg type

	$MsgIDRx4=FileReadLine ($GetLogFile_Path & "\LOG_21_Sample.log",19)						; Read the 19th line from the Log file
	consolewrite("$MsgIDRx4 :"&$MsgIDRx4&@CRLF)
	$MsgIDRx4Split=StringSplit($MsgIDRx4," ")
	consolewrite("FourthRxMsgDir :"&$MsgIDRx4Split[2]&@CRLF)								; Write the MsgID
	consolewrite("FourthRxMsgCh :"&$MsgIDRx4Split[3]&@CRLF)									; Write the Msg type
	consolewrite("FourthRxMsgID :"&$MsgIDRx4Split[4]&@CRLF)									; Write the MsgID
	consolewrite("FourthRxMsgType :"&$MsgIDRx4Split[5]&@CRLF)								; Write the Msg type
EndIf

if $MsgIDRx1Split[2]="Rx" and $MsgIDRx2Split[2]="Rx" and $MsgIDRx3Split[2]="Rx" and $MsgIDRx4Split[2]="Rx" then
	$ExtMsgDir="Found"
Else
	$ExtMsgDir="Not Found"
EndIf

consolewrite("$ExtMsgDir :"&$ExtMsgDir&@CRLF)

if $MsgIDRx1Split[3]=2 and $MsgIDRx2Split[3]=2 and $MsgIDRx3Split[3]=2 and $MsgIDRx4Split[3]=2 then
	$ExtMsgCh="Found"
Else
	$ExtMsgCh="Not Found"
EndIf

consolewrite("$ExtMsgCh :"&$ExtMsgCh&@CRLF)

if $MsgIDRx1Split[4]=0259 and $MsgIDRx2Split[4]=0259 and $MsgIDRx3Split[4]=0259 and $MsgIDRx4Split[4]=0259 then
	$ExtMsgID="Found"
Else
	$ExtMsgID="Not Found"
EndIf

consolewrite("$ExtMsgID :"&$ExtMsgID&@CRLF)

if $MsgIDRx1Split[5]="x" and $MsgIDRx2Split[5]="x" and $MsgIDRx3Split[5]="x" and $MsgIDRx4Split[5]="x" then
	$ExtMsgType="Found"
Else
	$ExtMsgType="Not Found"
EndIf

consolewrite("$ExtMsgType :"&$ExtMsgType&@CRLF)


if $ExtMsgDir="Found" and $ExtMsgCh="Found" and $ExtMsgID="Found" and $ExtMsgType="Found" Then
	_WriteResult("Pass","TS_Log_21")
Else
	_WriteResult("Fail","TS_Log_21")
EndIf

$isAppNotRes=_CloseApp()															; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Log_21")
EndIf

ConsoleWrite("****End : TS_Log_21.au3****"&@CRLF)
ConsoleWrite(@CRLF)